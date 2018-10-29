/*************************************************************************
 *
 *  fadc_list.c - Library of routines for the user to write for
 *                readout and buffering of events from JLab FADC using
 *                a JLab TIR module and Linux VME controller.
 *
 */


/* Event Buffer definitions */
#define MAX_EVENT_POOL     100
#define MAX_EVENT_LENGTH   1024*40	/* Size in Bytes */

/* Define Interrupt source and address */
#define TIR_SOURCE
#define TIR_ADDR 0x0ed0
/* TIR_MODE:  0 : interrupt on trigger,
              1 : interrupt from Trigger Supervisor signal
              2 : polling for trigger
              3 : polling for Trigger Supervisor signal  */
#define TIR_MODE 3      //changed to mode 3 on 8/7/17
/*#define TIR_MODE 1*/
/*#define TIR_MODE 3*/
#include "linuxvme_list.c"	/* source required for CODA */
#include "fadcLib.h"		/* library of FADC250 routines */

/* FADC Defaults/Globals */

/*Used in faSetProcMode() */
#define FADC_MODE          10  // 9 - Pulse Parameter (ped, sum, time);  10 - Debug Mode (9 + Raw Samples) 
#define FADC_WINDOW_WIDTH  79 // was 55
#define FADC_LATENCY       79
#define FADC_NSB           2  // # of samples *before* Threshold crossing (TC) to include in sum
#define FADC_NSA           55 // # of samples *after* Threshold crossing (TC) to include in sum
#define FADC_AERO_NSA      15 // # of samples *after* Threshold crossing (TC) to include in sum
#define FADC_THRESHOLD     5
#define FADC_NSAT          4 //# of consecutive samples over threshold required for pulse
#define chan_mask  0x0000 // chan mask for threshold setting 
#define WANT_THRESHOLD 1  //whether or not want threshold settings

int FA_SLOT;
/* FADC Library Variables */
extern int fadcA32Base;
extern int nfadc;
//extern int fadcID[20];
#define NFADC 7

#define FADC_ADDR 0xB01000
#define SDC_ADDR 0xea00

unsigned int MAXFADCWORDS=0;
//unsigned int fadc_window_width=FADC_WINDOW_WIDTH;

int ii;

int sync_or_unbuff;
static int buffered;
static int event_cnt = 0;
static int icnt = 0;


#include "usrstrutils.c" // utils to pick up CODA ROC config parameter line

//F1
#include "f1tdcLib.h"
extern int f1tdcA32Base;
int F1_SLOT;
extern int f1ID[20];
//extern int f1tdcA32Offset;
extern int nf1tdc = 2;
//#define nf1tdc 2
#define F1_ADDR 0x00B000
//#define F1_ADDR 0xFF3000
#define F1TDC_ADDR_inc 0x0001000
#define BLOCKLEVEL 1
//#define BUFFERLEVEL 3
#define BUFFERLEVEL 1

/* function prototype */
void rocTrigger(int arg);

//VETROC
#include "vfTDCLib.h"
extern unsigned int vfTDCA32Base;

//MLU
#include "v1495.h"
int MLUbase = 0x00530000;
  
/*****************************
 * DOWNLOAD
 ****************************/
void
rocDownload()
{
 init_strings();
  buffered = getflag(BUFFERED);
  printf ("Buffer flag : %d\n",buffered);
  

  printf("Hello!!!! This is our Test of the FADC\n");
//  int islot, iflag,ifa;
  int islot, iflag;
//  long hexslot;
//  int thrshflag;
//  int L1thrshold[16] = {72,83,37,91,103,61,69,58,71,104,72,63,79,53,74,63};
//  int L2thrshold[16] = {85,89,65,59,72,84,76,69,57,98,60,68,57,103,87,53};
//  int L3thrshold[16] = {128,138,131,131,127,132,122,125,124,135,135,114,134,59,0,0};

  f1tdcA32Base = 0x08000000;
  fadcA32Base  = 0x0A000000; /* 0x400000 above f1tdc multiblock end */
  vfTDCA32Base = 0x0D800000; /* 0x800000 above NEW fadc multiblock end */

  //thrshflag = chan_mask ;
  /* Program/Init FADC Modules Here */
  iflag = SDC_ADDR;		/* SDC Board address */
  iflag |= 1 << 0;		/* Front panel sync-reset */
   /* iflag |= 0<<0;  /\* Software sync-reset *\/  */
  //////////
  iflag |= 1 << 1;		/* Front Panel Input trigger source */
  //iflag |= 0<<4;  /* Internal 250MHz Clock source */
  iflag |= 1 << 4;		/* Front Panel 250MHz Clock source */
  //iflag |= 1<<18; /* Skip Firmware Check*/
  printf("iflag = 0x%x\n", iflag);

  faInit(FADC_ADDR, 0x1000, NFADC, iflag);

   if (nfadc > 1)
   faEnableMultiBlock(0);   //chaned this to 0 for Token Passing via P2     
  for (islot = 0; islot < nfadc; islot++)
    {
      //      if (nfadc > 1)
      //	faEnableMultiBlock(faSlot(islot));	// added 5/25/17 

      faSetBlockLevel(faSlot(islot),1);
       faEnableBusError(faSlot(islot));
      //  faSetThreshold(faSlot(islot), 0, 0xffff);
      if(WANT_THRESHOLD){
        printf("FADC THRESHOLDS ON!\n");
               // DAC values set such that pedestal samples are at 300ch
        if(islot==0) //S2
          faSetThreshold(faSlot(islot), 300+8, 0xffff);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 8ch = 1mV
        if(islot==1) //S2
          faSetThreshold(faSlot(islot), 300+8, 0xffff);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 8ch = 1mV
        if(islot==2) //GC/S0/A2
        {
	  faSetThreshold(faSlot(islot), 300+25, 0x3fff);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 25ch = 3mV
	  faSetThreshold(faSlot(islot), 300+8, 0xc000);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 8ch = 1mV
        }
        if(islot==3) //raster/BPM
          faSetThreshold(faSlot(islot), 1, 0xffff);
        if(islot==4) //A2
	  faSetThreshold(faSlot(islot), 300+8, 0xffff);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 8ch = 1mV
        if(islot==5) //A1/A2
        {
	  faSetThreshold(faSlot(islot), 300+16, 0x00ff);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 16ch = 2.0mV
	  faSetThreshold(faSlot(islot), 300+8, 0xff00);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 8ch = 1mV
        }
        if(islot==6) //A1
	  faSetThreshold(faSlot(islot), 300+16, 0xffff);	//for 0.5V setting, 0.5V/4096 = 0.122 mV/channel, so 16ch = 2.0mV
//	 for (ifa = 0; ifa<16; ifa++)
//	   {
//	     thrshflag |= 1 << ifa;
//	     if(islot==0) faSetThreshold(faSlot(islot), L1thrshold[ifa]+FADC_THRESHOLD , thrshflag);
//	     if(islot==1) faSetThreshold(faSlot(islot), L2thrshold[ifa]+FADC_THRESHOLD , thrshflag);
//	     if(islot==2){ 
//                 if(ifa==13)
//                    faSetThreshold(faSlot(islot), L3thrshold[ifa]+1 , thrshflag);
//                 else
//                    faSetThreshold(faSlot(islot), L3thrshold[ifa]+FADC_THRESHOLD , thrshflag);
//                 
//                  
//              }
//	     if(islot==3) faSetThreshold(faSlot(islot), 1 , thrshflag);
//	     printf("!!! threshold set !!!\n");
//	     
//	     thrshflag = chan_mask;
//	   }
       }
       else{
       printf("FADC THRESHOLDS OFF!\n");
           faSetThreshold(faSlot(islot), 1, 0xffff);
//         if(islot==2){
//	    faSetThreshold(faSlot(islot), 1, 0xdfff); //0xffff sets all channels to same threshold 
//            faSetThreshold(faSlot(islot), 60, 0x2000);
//          }
//         else
//            faSetThreshold(faSlot(islot), 1, 0xffff);
        }

      /* Set input DAC level */
        if(islot==3)
          {
            faSetDAC(faSlot(islot), 2500, 0xf); //changed for busy test : was 3150 
            faSetDAC(faSlot(islot), 3200, 0x1ff0);	//raster and bpms

	    faSetDAC(faSlot(islot), 3003, 0x4000);	//A2 channel 24 alternate
	    faSetDAC(faSlot(islot), 2986, 0x8000);	//A2 channel 25 alternate
          }
        if(islot==2)
          {
            faSetDAC(faSlot(islot),3087,0x0001);	//S0 channels   DAC=3000
	    faSetDAC(faSlot(islot),3093,0x0002);       
	    faSetDAC(faSlot(islot),3087,0x0004);	//GC channels    DAC 3220 
	    faSetDAC(faSlot(islot),3090,0x0008); 
	    faSetDAC(faSlot(islot),3086,0x0010);
	    faSetDAC(faSlot(islot),3090,0x0020);
	    faSetDAC(faSlot(islot),3081,0x0040);
	    faSetDAC(faSlot(islot),3084,0x0080);
	    faSetDAC(faSlot(islot),3084,0x0100);
	    faSetDAC(faSlot(islot),3091,0x0200);
	    faSetDAC(faSlot(islot),3090,0x0400);
	    faSetDAC(faSlot(islot),3075,0x0800);
	    faSetDAC(faSlot(islot),3087,0x1000); //ADCsum
	    faSetDAC(faSlot(islot),3033,0x2000); //L1A Timing Reference
           
           
	    faSetDAC(faSlot(islot), 3001, 0x4000);	//A2 channel
	    faSetDAC(faSlot(islot), 2985, 0x8000);	//A2 channel
          }
        if(islot==0)//s2
	  {
               faSetDAC(faSlot(islot), 3023, 0x0001); // jumper set to 0.5 V
	       faSetDAC(faSlot(islot), 3029, 0x0002); // 3200
	       faSetDAC(faSlot(islot), 2996, 0x0004);
	       faSetDAC(faSlot(islot), 3035, 0x0008);
	       faSetDAC(faSlot(islot), 3044, 0x0010);
	       faSetDAC(faSlot(islot), 3014, 0x0020);
	       faSetDAC(faSlot(islot), 3020, 0x0040);
	       faSetDAC(faSlot(islot), 3012, 0x0080);
	       faSetDAC(faSlot(islot), 3021, 0x0100);
	       faSetDAC(faSlot(islot), 3045, 0x0200);
	       faSetDAC(faSlot(islot), 3021, 0x0400);
	       faSetDAC(faSlot(islot), 3015, 0x0800);
	       faSetDAC(faSlot(islot), 3027, 0x1000);
	       faSetDAC(faSlot(islot), 3008, 0x2000);
	       faSetDAC(faSlot(islot), 3023, 0x4000);
	       faSetDAC(faSlot(islot), 3017, 0x8000);
	  }

        if(islot==1) // s2
	  {
               faSetDAC(faSlot(islot), 3032, 0x0001); // jumper set to 0.5 V
               faSetDAC(faSlot(islot), 3033, 0x0002); // 3200 original value
	       faSetDAC(faSlot(islot), 3018, 0x0004);
	       faSetDAC(faSlot(islot), 3012, 0x0008);
	       faSetDAC(faSlot(islot), 3023, 0x0010);
	       faSetDAC(faSlot(islot), 3031, 0x0020);
	       faSetDAC(faSlot(islot), 3024, 0x0040);
	       faSetDAC(faSlot(islot), 3020, 0x0080);
	       faSetDAC(faSlot(islot), 3010, 0x0100);
	       faSetDAC(faSlot(islot), 3040, 0x0200);
	       faSetDAC(faSlot(islot), 3012, 0x0400);
	       faSetDAC(faSlot(islot), 3018, 0x0800);
	       faSetDAC(faSlot(islot), 3012, 0x1000);
	       faSetDAC(faSlot(islot), 3045, 0x2000);
	       faSetDAC(faSlot(islot), 3031, 0x4000);
	       faSetDAC(faSlot(islot), 3009, 0x8000);
	  }

        if(islot==4) //A2
	  {
            faSetDAC(faSlot(islot), 2982, 0x0001); // jumper set to 0.5 V
	    faSetDAC(faSlot(islot), 2992, 0x0002);
	    faSetDAC(faSlot(islot), 2962, 0x0004);
	    faSetDAC(faSlot(islot), 2981, 0x0008);
	    faSetDAC(faSlot(islot), 2967, 0x0010);
	    faSetDAC(faSlot(islot), 3002, 0x0020);
	    faSetDAC(faSlot(islot), 2975, 0x0040);
	    faSetDAC(faSlot(islot), 2983, 0x0080);
	    faSetDAC(faSlot(islot), 2958, 0x0100);
	    faSetDAC(faSlot(islot), 3000, 0x0200);
	    faSetDAC(faSlot(islot), 2968, 0x0400);
	    faSetDAC(faSlot(islot), 2975, 0x0800);
	    faSetDAC(faSlot(islot), 2997, 0x1000);
	    faSetDAC(faSlot(islot), 2966, 0x2000);
	    faSetDAC(faSlot(islot), 2956, 0x4000);
	    faSetDAC(faSlot(islot), 2979, 0x8000);
	  }

        if(islot==5) //A1/A2
          {
            faSetDAC(faSlot(islot),3312,0x0001);
	    faSetDAC(faSlot(islot),3298,0x0002);
	    faSetDAC(faSlot(islot),3303,0x0004);
	    faSetDAC(faSlot(islot),3335,0x0008);
	    faSetDAC(faSlot(islot),3315,0x0010);
	    faSetDAC(faSlot(islot),3321,0x0020);
	    faSetDAC(faSlot(islot),3321,0x0040);
	    faSetDAC(faSlot(islot),3315,0x0080);

            faSetDAC(faSlot(islot),2985,0x0100);
	    faSetDAC(faSlot(islot),2965,0x0200);
	    faSetDAC(faSlot(islot),2979,0x0400);
	    faSetDAC(faSlot(islot),2969,0x0800);
	    faSetDAC(faSlot(islot),2970,0x1000);
	    faSetDAC(faSlot(islot),2994,0x2000);
	    faSetDAC(faSlot(islot),2972,0x4000);
	    faSetDAC(faSlot(islot),2966,0x8000);
          }
        if(islot==6) //A1
	  {
               // DAC values set such that pedestal samples are at 300ch
            faSetDAC(faSlot(islot), 3205, 0x0001); // jumper set to 0.5 V
	    faSetDAC(faSlot(islot), 3187, 0x0002);
	    faSetDAC(faSlot(islot), 3209, 0x0004);
	    faSetDAC(faSlot(islot), 3219, 0x0008);
	    faSetDAC(faSlot(islot), 3180, 0x0010);
	    faSetDAC(faSlot(islot), 3217, 0x0020);
	    faSetDAC(faSlot(islot), 3201, 0x0040);
	    faSetDAC(faSlot(islot), 3220, 0x0080);
	    faSetDAC(faSlot(islot), 3198, 0x0100);
	    faSetDAC(faSlot(islot), 3227, 0x0200);
	    faSetDAC(faSlot(islot), 3224, 0x0400);
	    faSetDAC(faSlot(islot), 3207, 0x0800);
	    faSetDAC(faSlot(islot), 3338, 0x1000);
	    faSetDAC(faSlot(islot), 3312, 0x2000);
	    faSetDAC(faSlot(islot), 3303, 0x4000);
	    faSetDAC(faSlot(islot), 3331, 0x8000);
	  }



//      faSetDAC(faSlot(islot), 3150, 0xffff);
     // faSetDAC(faSlot(islot), 325, 0);
      /*  Setup option 1 processing - RAW Window Data     <-- */
      /*        option 2            - RAW Pulse Data */
      /*        option 3            - Integral Pulse Data */
      /*  Setup 200 nsec latency (PL  = 50)  */
      /*  Setup  80 nsec Window  (PTW = 20) */
      /*  Setup Pulse widths of 36ns (NSB(3)+NSA(6) = 9)  */
      /*  Setup up to 1 pulse processed */
      /*  Setup for both ADC banks(0 - all channels 0-15) */


      /*********************************************************************************  
       * faSetProcMode(int id, int pmode, unsigned int PL, unsigned int PTW,
       *    int NSB, unsigned int NSA, unsigned int NP, 
       *    unsigned int NPED, unsigned int MAXPED, unsigned int NSAT); 
       *
       *  id    : fADC250 Slot number 
       *  pmode : Processing Mode  
       *          9 - Pulse Parameter (ped, sum, time) 
       *         10 - Debug Mode (9 + Raw Samples) 
       *    PL : Window Latency
       *   PTW : Window Width
       *   NSB : Number of samples before pulse over threshold 
       *   NSA : Number of samples after pulse over threshold 
       *    NP : Number of pulses processed per window  
       *  NPED : Number of samples to sum for pedestal
       * MAXPED : Maximum value of sample to be included in pedestal sum
       *  NSAT : Number of consecutive samples over threshold for valid pulse 
       */


      //      faSetProcMode(faSlot(islot), 10, 85, 40, 5, 60, 1, 0);

      // faSetProcMode(faSlot(islot), 10, 85, 40, 5, 60, 1, 4,250,2);
          if(WANT_THRESHOLD)
            {
            printf("FADC THRESHOLDS ON! (ProcMode Block)\n");
            if(islot==0||islot==1)faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_LATENCY, FADC_NSB, FADC_AERO_NSA, 1, 15,357,FADC_NSAT);
            if(islot==2)faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_LATENCY, FADC_NSB, FADC_AERO_NSA, 1, 15,357,FADC_NSAT);
            if(islot==3)faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_WINDOW_WIDTH, FADC_NSB, FADC_NSA, 1, 15,357,FADC_NSAT); //raster and BPM, no thresholds ever
            if(islot==4||islot==5||islot==6)faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_LATENCY, FADC_NSB, FADC_AERO_NSA, 1, 15,357,FADC_NSAT);
            }
          else
            {
            printf("FADC THRESHOLDS OFF! (ProcMode Block)\n");
            if(islot==3)faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_WINDOW_WIDTH, FADC_NSB, FADC_NSA, 1, 15,357,1);
            if(islot==4||islot==5||islot==6)faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_WINDOW_WIDTH, FADC_NSB, FADC_NSA, 1, 15,357,1);
            if(islot==0||islot==1||islot==2)faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_WINDOW_WIDTH, FADC_NSB, FADC_NSA, 1, 15,357,1);
	    }
	    faSetTriggerBusyCondition(faSlot(islot),8);		//FIXME DO WE NEED THIS?!?!?!

    }


  faSDC_Status(0);
  faGStatus(0);			//  6/7/2017

  /*************************************************************/
  /* VFTDC initialization                                      */
  /*************************************************************/
  //was being initiliazed here August 8th 2018  ( uncommented and slot set to 5 Sept 20 AC )
  vfTDCInit(5<<19, 1<<19, 1,
            VFTDC_INIT_SOFT_SYNCRESET |
            VFTDC_INIT_FP_TRIG      |
            VFTDC_INIT_INT_CLKSRC);
  
////////////////////
//            VFTDC_INIT_VXS_SYNCRESET |
//            VFTDC_INIT_VXS_TRIG      |
//            VFTDC_INIT_VXS_CLKSRC);

//  vfTDCInit(14<<19, 1<<19, 1,
//          VFTDC_INIT_VXS_SYNCRESET |
//          VFTDC_INIT_VXS_TRIG      |
//          VFTDC_INIT_VXS_CLKSRC);
  int window_width   = 250; /* 250 = 250*4ns = 1000ns */
  int window_latency = 100; /* 100 = 100*4ns =  400ns */
  vfTDCSetWindowParamters(0, window_latency, window_width);

  v1495InitCODA(MLUbase,0);		// initialize MLU
  
  printf("rocDownload: User Download Executed\n");
}

/*****************************
 * PRESTART
 ****************************/
void
rocPrestart()
{
  int ifa, if1;

  //vmeCheckMutexHealth(10); // will try to revive mutex

  //FA_SLOT = fadcID[0];

  /* Program/Init VME Modules Here */
  for(ifa=0; ifa < nfadc; ifa++) 
    {
      faSoftReset(faSlot(ifa),0);
      faResetTriggerCount(faSlot(ifa));
      faResetToken(faSlot(ifa));
    }

  faGStatus(0);

  //F1
  int iflag = 0x0; /* with SDC */
  iflag |= 0xDD0;
  iflag |= 4;  /* read from file */
  printf("iflag = 0x%x\n",iflag);

  f1ConfigReadFile("/adaqfs/home/adaq/tritium_fadc/BlkRead/f1_cfg_righHRS_HighResSync.dat");
  f1Init(F1_ADDR,F1TDC_ADDR_inc,nf1tdc,iflag);

  /* Setup F1TDC */
for(if1=0; if1<nf1tdc; if1++) {
  F1_SLOT = f1ID[if1];

  f1SetBlockLevel(F1_SLOT,1);
  f1EnableBusError(F1_SLOT);

  f1Clear(F1_SLOT);

  // MARCO:
  f1ConfigShow(0, 0);

  /* lock the resolution using the TIR output bit */
  //tirIntOutput(1<<0 | 1<<1);
  tirIntOutput(0);

  /* wait for resolution to lock */
  //usleep(50000);
  //f1Status(F1_SLOT,0);
}
f1SDC_Sync();
usleep(50000);
for(if1=0; if1<nf1tdc; if1++) {
  F1_SLOT = f1ID[if1];
  f1Status(F1_SLOT,0);
}

  //*********
  //* VFTDC *
  //*********
  vfTDCSyncReset(0);
  vfTDCStatus(0,0);

  printf("rocPrestart: User Prestart Executed\n");
}

/*****************************
 * GO
 ****************************/
void
rocGo()
{
  int ifa, if1;

  /*  Enable FADC */
  faGEnable(0, 0);

  taskDelay(1); // taskDelay(int ticks) : # of ticks to delay task. 1 ticks = 16.7 ms

  /*  Send Sync Reset to FADC */
  faSDC_Sync();

  taskDelay(1); // taskDelay(int ticks) : # of ticks to delay task. 1 ticks = 16.7 ms


  MAXFADCWORDS = nfadc * (2 + 4 + 1 * (8 + FADC_WINDOW_WIDTH/2));

// F1
for(if1=0; if1<nf1tdc; if1++) {
  F1_SLOT = f1ID[if1];
  f1EnableData(F1_SLOT,0xff);
}

  // Marco: checking some status
printf("\n\n\n--> faStatus:\n");
  for(ifa=0; ifa < nfadc; ifa++)
    {
      printf("\n  ** BOARD %d **\n",ifa);
      faStatus(faSlot(ifa),0);
    }

printf("\n\n\n--> f1Status:\n");
for(if1=0; if1<nf1tdc; if1++) {
  F1_SLOT = f1ID[if1];
  f1Status(F1_SLOT,0);
}

  //*********
  //* VFTDC *
  //*********
  /* Use this info to change block level is all modules */
  vfTDCSetBlockLevel(0, BLOCKLEVEL);

  v1495ClockCountSyncCODA(MLUbase);		//reset MLU clock counter to zero
  
  /* Interrupts/Polling enabled after conclusion of rocGo() */
}

/*****************************
 * END
 ****************************/
void
rocEnd()
{

  int if1;

  /* FADC Disable */
  faGDisable(0);

  faSDC_Status(0);

  /* FADC Event status - Is all data read out */
  faGStatus(0);

  //F1
for(if1=0; if1<nf1tdc; if1++) {
  F1_SLOT = f1ID[if1];
  //f1GDisable();
  //f1GStatus(0);
  //tiStatus();
  f1Status(F1_SLOT,0);
  f1Reset(F1_SLOT,0);
}

  //*********
  //* VFTDC *
  //*********
  vfTDCStatus(0,0);

  printf("rocEnd: Ended after %d events\n", tirGetIntCount());
  ii=0;
}

/*****************************
 * TRIGGER 
 ****************************/
void
rocTrigger(int arg)
{

  //FA_SLOT = fadcID[0];
  int if1;
  int ii, nwords, islot,stat;
  unsigned int datascan = 0, scanmask = 0;
  unsigned int event_ty = 1, event_no = 0;
  int dCnt, blkReady=0, timeout=0;

  /* Setup Address and data modes for DMA transfers
   *   
   *  vmeDmaConfig(addrType, dataType, sstMode);
   *
   *  addrType = 0 (A16)    1 (A24)    2 (A32)
   *  dataType = 0 (D16)    1 (D32)    2 (BLK32) 3 (MBLK) 4 (2eVME) 5 (2eSST)
   *  sstMode  = 0 (SST160) 1 (SST267) 2 (SST320)
   */
  vmeDmaConfig(2, 5, 1);

  //vmeDmaConfig(2,3,0); 

  event_ty = EVTYPE;
  event_no = EVNUM;

  /* Open an event bank of banks with event type EVTYPE (obtained from TI) */
  EVENTOPEN(event_ty, BT_BANK);
  BANKOPEN(250,BT_UI4,0);
  /* Example: Raise the 0th (1<<0) and 2nd (1<<2) output level on the TI */
  //tirIntOutput(1<<0 | 1<<2);
   tirIntOutput(1<<2); // added 12/2/17

  /* Insert trigger count  - Make sure bytes are ordered little-endian (LSWAP) */
  // *dma_dabufp++ = LSWAP(0xda000022);
  // *dma_dabufp++ = LSWAP(tirGetIntCount());

  /* Check for valid data here */
  /* Mask of initialized modules */
  scanmask = faScanMask();
  /* Check scanmask for block ready up to 100 times */
  datascan = faGBlockReady(scanmask, 100); 
  stat = (datascan == scanmask);

  if (stat > 0)
    {
       nwords = faReadBlock(0, dma_dabufp, 10000, 2);	//changed rflag = 2 for Multi Block transfer 5/25/17
       //  nwords = faReadBlock(faSlot(0), dma_dabufp, 3000, 2);

      if (nwords < 0)
	{
	  printf("ERROR: in transfer (event = %d), nwords = 0x%x\n",
		 tirGetIntCount(), nwords);
	  //  *dma_dabufp++ = LSWAP(0xda000bad);

	}
      else
	{
	  dma_dabufp += nwords;
	}
    }
  else
    {
      printf("ERROR fadc: Data not ready in event %d\n", tirGetIntCount());
      // *dma_dabufp++ = LSWAP(0xda000bad);
    }

    for (islot = 0; islot < nfadc; islot++)	// 5/25/17
      faResetToken(faSlot(islot));
  
  //  *dma_dabufp++ = LSWAP(0xda0000ff);	/* Event EOB */

  BANKCLOSE;

  ///////
  //F1 //
  ///////
  BANKOPEN(8,BT_UI4,0);
//  tirIntOutput(2);
//tirIntOutput(1<<2); // added 12/2/17
  // Marco:
vmeDmaConfig(2,3,0);
  *dma_dabufp++ = LSWAP(tirGetIntCount());

for(if1=0; if1<nf1tdc; if1++) {
  F1_SLOT = f1ID[if1];
  /* Check for valid data here */
  for(ii=0;ii<100;ii++)
    {
      datascan = f1Dready(F1_SLOT);
      if (datascan>0)
        {
          break;
        }
    }
//printf("F1_datascan: %d\n",datascan);
  if(datascan>0)
    {
      nwords = f1ReadEvent(F1_SLOT,dma_dabufp,500,1);
// nwords = f1PrintEvent(F1_SLOT,0);
//printf("  -> nwords = %d\n",nwords);

      if(nwords < 0)
        {
          //printf("ERROR: in transfer (event = %d), status = 0x%x\n", tirGetIntCount(),nwords);
          // Marco:
          printf("ERROR F1: in transfer (event = %d), status = 0x%x\n", tirGetIntCount(),nwords);
          *dma_dabufp++ = LSWAP(0xda000bad);
        }
      else
        {
          dma_dabufp += nwords;
        }
    }
  else
    {
      //printf("ERROR: Data not ready in event %d\n",tirGetIntCount());
      //Marco:
      printf("ERROR F1: Data not ready in event %d\n",tirGetIntCount());
      *dma_dabufp++ = LSWAP(0xda000bad);
    }
//}
  *dma_dabufp++ = LSWAP(0xda0000ff); /* Event EOB */
}
vmeDmaConfig(2, 5, 1);

  BANKCLOSE;

  //*********
  //* VFTDC *
  //*********
  BANKOPEN(7,BT_UI4,0);
  
  blkReady = vfTDCBReady(0);
  if(blkReady==0 && timeout<100)
    {
      blkReady = vfTDCBReady(0);
      timeout++;
    }

  if(timeout>=100)
    {
      printf("%s: Data not ready in vfTDC.\n",__FUNCTION__);
      return;
    }
  
  /* e.g. Max number of words = Blocklevel * (10 hits per channel + 10 other words) */
  dCnt = vfTDCReadBlock(0,dma_dabufp,BLOCKLEVEL*(10*192+10),1);
  if(dCnt<=0)
    {
      printf("%s: No vfTDC data or error.  dCnt = %d\n",__FUNCTION__,dCnt);
    }
  else
    {
      dma_dabufp += dCnt;
    }
  
  BANKCLOSE;

    ///////
    //MLU//
    ///////

  //Get Upstream BCM words
  unsigned short bcmu_0 = v1495BCM_ReadCODAindi(0,0);	//get lowest 16 bits from upstream bcm sum
  unsigned short bcmu_1 = v1495BCM_ReadCODAindi(0,1);
  unsigned short bcmu_2 = v1495BCM_ReadCODAindi(0,2);
  unsigned short bcmu_3 = v1495BCM_ReadCODAindi(0,3);
  unsigned int bcmu_h = (bcmu_3<<16) + bcmu_2;
  unsigned int bcmu_l = (bcmu_1<<16) + bcmu_0;

  //Get Downstream BCM words
  unsigned short bcmd_0 = v1495BCM_ReadCODAindi(1,0);	//get lowest 16 bits from downstream bcm sum
  unsigned short bcmd_1 = v1495BCM_ReadCODAindi(1,1);
  unsigned short bcmd_2 = v1495BCM_ReadCODAindi(1,2);
  unsigned short bcmd_3 = v1495BCM_ReadCODAindi(1,3);
  unsigned int bcmd_h = (bcmd_3<<16) + bcmd_2;
  unsigned int bcmd_l = (bcmd_1<<16) + bcmd_0;

  BANKOPEN(1495,BT_UI4,0);		//MLU Readout bank
    vmeDmaConfig(2,3,0);
    *dma_dabufp++ = LSWAP(0x14951495);
    *dma_dabufp++ = LSWAP(v1495ClockCountReadCODA());			//32 bit Clock Count
    *dma_dabufp++ = LSWAP(bcmu_h);		//upper 32 bits of upstream bcm
    *dma_dabufp++ = LSWAP(bcmu_l);		//lower 32 bits of upstream bcm
    *dma_dabufp++ = LSWAP(bcmd_h);		//upper 32 bits of downstream bcm
    *dma_dabufp++ = LSWAP(bcmd_l);		//lower 32 bits of downstream bcm
    *dma_dabufp++ = LSWAP(0x14950000);
    vmeDmaConfig(2, 5, 1);
  BANKCLOSE;
  
  BANKOPEN(0xfabc,BT_UI4,0);		//Sync checks
  event_cnt = event_cnt + 1;
  event_ty = (EVTYPE&0xf);
  icnt = icnt + 1;
  if(icnt > 20000) icnt = 0;
  *dma_dabufp++ = LSWAP(0xfabc0004);
  *dma_dabufp++ = LSWAP(event_ty);
  *dma_dabufp++ = LSWAP(event_cnt);
  *dma_dabufp++ = LSWAP(icnt);
  *dma_dabufp++ = LSWAP(syncFlag);
  *dma_dabufp++ = LSWAP(0xfaaa0001);
  
  //check if FIFO is empty at sync event or when no buffering
  sync_or_unbuff = syncFlag || !buffered;
  //  printf("sync or unbuff : %d\n",sync_or_unbuff);
   if (sync_or_unbuff!=0)
    {
      scanmask = faScanMask();
      /* Check scanmask for block ready up to 100 times */
      datascan = faGBlockReady(scanmask, 100); 
      stat = (datascan == scanmask);
      
      if (stat > 0)
	{
	  printf("data left in FADC FIFO at sync event\n");
	  		//FADC sync event bank
          nwords = faReadBlock(0, dma_dabufp, 5000, 2);	//changed rflag = 2 for Multi Block transfer 5/25/17
	  *dma_dabufp++ = LSWAP(0xfadc250);
	  //	  nwords = faReadBlock(faSlot(0), dma_dabufp, 25000, 2);
	  // nwords = 0;
	  // nwords = 0;
	  if (nwords < 0)
	    {
	      printf("ERROR: in transfer (event = %d), nwords = 0x%x\n",
		     tirGetIntCount(), nwords);
	      //  *dma_dabufp++ = LSWAP(0xda000bad);
	      
	    }
	  else
	    {
	      dma_dabufp += nwords;
	    }
	  for (islot = 0; islot < nfadc; islot++)	// 5/25/17
	    faResetToken(faSlot(islot));
	  for(islot = 0; islot < nfadc; islot++)
	    {
	      int davail = faBready(faSlot(islot));
	      if(davail > 0)
		{
		  printf("%s: ERROR: fADC250 Data available after readout in SYNC event \n",
			 __func__, davail);
		  
		  while(faBready(faSlot(islot)))
		    {
		      vmeDmaFlush(faGetA32(faSlot(islot)));
		    }
		}
	    }
	}
  

      /*
       *dma_dabufp++ = LSWAP(0xbadbad1);
       *dma_dabufp++ = LSWAP(0xbadbad2);
       *dma_dabufp++ = LSWAP(0xbadbad3);
       */
      
      
      //  *dma_dabufp++ = LSWAP(0xda0000ff);	/* Event EOB */
      
      vmeDmaConfig(2,3,0);
      // *dma_dabufp++ = LSWAP(tirGetIntCount());
      
      for(if1=0; if1<nf1tdc; if1++) {
	F1_SLOT = f1ID[if1];
	// Check for valid data here 
	for(ii=0;ii<100;ii++)
	  {
	    datascan = f1Dready(F1_SLOT);
	    if (datascan>0)
	      {
		break;
	      }
	  }
	//printf("F1_datascan: %d\n",datascan);
	if(datascan>0)
	  {
	    printf ("Data left in F1 FIFO\n");
	    *dma_dabufp++ = LSWAP(0xf1ddc);
	    nwords = f1ReadEvent(F1_SLOT,dma_dabufp,500,1);
	    // nwords = f1PrintEvent(F1_SLOT,0);
	    //printf("  -> nwords = %d\n",nwords);
	    
	    if(nwords < 0)
	      {
		//printf("ERROR: in transfer (event = %d), status = 0x%x\n", tirGetIntCount(),nwords);
		// Marco:
		printf("ERROR: in transfer (event = %d), status = 0x%x\n", tirGetIntCount(),nwords);
		*dma_dabufp++ = LSWAP(0xda000bad);
	      }
	    else
	      {
		dma_dabufp += nwords;
	      }
	  }
      }
    }
 BANKCLOSE;
	EVENTCLOSE;
	tirIntOutput(0);
}

void
rocCleanup()
{
  int islot;

  faGReset(1); 

  /*  This runs just before download.  */
  /*  If this readout list does anything clever in resource
   *   allocation, this is the place to clean that up.      */
}
