/*************************************************************************
 *
 *  fadc_list.c - Library of routines for the user to write for
 *                readout and buffering of events from JLab FADC using
 *                a JLab TIR module and Linux VME controller.
 *
 */

/* Event Buffer definitions */
#define MAX_EVENT_LENGTH   1024*128 	/* Size in Bytes */
//MARCO:
#define MAX_EVENT_POOL     25
//#define MAX_EVENT_POOL     80

/* Define Interrupt source and address */
#define TIR_SOURCE
#define TIR_ADDR 0x0ed0
/* TIR_MODE:  0 : interrupt on trigger,
              1 : interrupt from Trigger Supervisor signal
              2 : polling for trigger
              3 : polling for Trigger Supervisor signal  */
//MARCO:
#define TIR_MODE 3
//#define TIR_MODE 2

#include "linuxvme_list.c"	/* source required for CODA */
#include "fadcLib.h"		/* library of FADC250 routines */

/* FADC Defaults/Globals */

/*Used in faSetProcMode() */
#define FADC_MODE          10 // 9 - Pulse Parameter (ped, sum, time);  10 - Debug Mode (9 + Raw Samples) 
#define FADC_WINDOW_WIDTH  88 // was 55
#define FADC_LATENCY       88 // 
#define FADC_LA_Sh         73 //was 78 // was 62 
#define FADC_WD_Sh         60 // was 
#define FADC_NSB           2  // # of samples *before* Threshold crossing (TC) to include in sum
#define FADC_NSA           60 // # of samples *after* Threshold crossing (TC) to include in sum
#define FADC_THRESHOLD     5 // changed 8/6/2017 from 300 : cosmic signals are not large enough to be above threshold
#define chan_mask  0x0000 // chan mask for threshold setting 

//wether or not to use threshold
#define WANT_THRESHOLD 1

int FA_SLOT;
/* FADC Library Variables */
extern int fadcA32Base;
extern int nfadc;
//extern int fadcID[20];
#define NFADC 8

//#define FADC_ADDR 0xA90000
#define FADC_ADDR 0xA80000
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
#define F1_ADDR 0xFF3000
//#define F1_ADDR 0xFF4000
#define F1TDC_ADDR_inc 0x0001000
#define BLOCKLEVEL 1
//#define BUFFERLEVEL 3
#define BUFFERLEVEL 1

//MLU
#include "v1495.h"
int MLUbase = 0x00520000;

/* function prototype */
void rocTrigger(int arg);


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
  int islot, iflag,ifa;
  int thrshflag;
  long hexslot;
  int L1thrshold[16] = {86,113,105,104,104,125,102,93,112,101,118,127,116,127,102,105};
  int L2thrshold[16] = {88,113,79,86,127,116,82,97,113,100,108,83,126,109,106,96};
  int L3thrshold[16] = {89,110,111,110,122,85,119,82,109,74,115,96,93,40,0,0};
 
  ii=0;
 
  // Marco: make sure there will be no A32 conflict between fadc and f1tdc
  f1tdcA32Base   = 0x08000000;
  fadcA32Base    = 0x0A000000;
  //fadcA32Base    = 0x09000000;

  thrshflag = chan_mask ;
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

   faInit(FADC_ADDR, 0x10000, NFADC, iflag);
   // faInit(FADC_ADDR, 0x1000, NFADC, iflag);

   // faSDC_Config(0, (1 << port0) | (1 << port1) | (1 << port2));
    faSDC_Config(0, (1 << 0) | (1 << 1) | (1 << 2));


   if (nfadc > 1)
   faEnableMultiBlock(0);   //chaned this to 0 for Token Passing via P2     
  for (islot = 0; islot < nfadc; islot++)
    {
      //      if (nfadc > 1)
      //	faEnableMultiBlock(faSlot(islot));	// added 5/25/17 

      faSetBlockLevel(faSlot(islot),1);
       faEnableBusError(faSlot(islot));
      //  faSetThreshold(faSlot(islot), 0, 0xffff);
      
     if(WANT_THRESHOLD)
       {
       if(islot==0)      faSetThreshold(faSlot(islot), 300+25 , 0xffff);	//GC,SO,Ref
       else if(islot==1) faSetThreshold(faSlot(islot), 300+25 , 0xffff);	//S2
       else if(islot==2) faSetThreshold(faSlot(islot), 300+25 , 0xffff);	//S2
       else              faSetThreshold(faSlot(islot), 1, 0xffff); //raster, BPMs, PRL1, PRL2
       }
     else
       {
       faSetThreshold(faSlot(islot), 1, 0xffff); //0xffff sets all channels to same threshold
       }

if(islot==0)      {
        faSetDAC(faSlot(islot), 3072, 0x0001);   ///S0
	faSetDAC(faSlot(islot), 3088, 0x0002);
	faSetDAC(faSlot(islot), 3090, 0x0004); // GC
	faSetDAC(faSlot(islot), 3092, 0x0008);
	faSetDAC(faSlot(islot), 3091, 0x0010);
	faSetDAC(faSlot(islot), 3103, 0x0020);
	faSetDAC(faSlot(islot), 3080, 0x0040);
	faSetDAC(faSlot(islot), 3077, 0x0080);
	faSetDAC(faSlot(islot), 3093, 0x0100);
	faSetDAC(faSlot(islot), 3085, 0x0200);
	faSetDAC(faSlot(islot), 3096, 0x0400);
	faSetDAC(faSlot(islot), 3102, 0x0800);
	faSetDAC(faSlot(islot), 3071, 0x1000); // Gc sum
	faSetDAC(faSlot(islot), 3027, 0x2000); // L1A Ref
	faSetDAC(faSlot(islot), 2998, 0x4000); // last two channels are empty
	faSetDAC(faSlot(islot), 2998, 0x8000);
	//faSetDAC(faSlot(islot), 3200, 0xffff);  //previous default value
       }
if(islot==1){
        faSetDAC(faSlot(islot), 3040, 0x0001); /// S2L
	faSetDAC(faSlot(islot), 3059, 0x0002);
	faSetDAC(faSlot(islot), 3034, 0x0004);
	faSetDAC(faSlot(islot), 3039, 0x0008);
	faSetDAC(faSlot(islot), 3067, 0x0010);
	faSetDAC(faSlot(islot), 3060, 0x0020);
	faSetDAC(faSlot(islot), 3035, 0x0040);
	faSetDAC(faSlot(islot), 3047, 0x0080);
	faSetDAC(faSlot(islot), 3057, 0x0100);
	faSetDAC(faSlot(islot), 3049, 0x0200);
	faSetDAC(faSlot(islot), 3053, 0x0400);
	faSetDAC(faSlot(islot), 3037, 0x0800);
	faSetDAC(faSlot(islot), 3068, 0x1000);
	faSetDAC(faSlot(islot), 3056, 0x2000);
	faSetDAC(faSlot(islot), 3052, 0x4000);
	faSetDAC(faSlot(islot), 3046, 0x8000);
	//faSetDAC(faSlot(islot), 3200, 0xffff);  //previous default value
       }
if(islot==2){
        faSetDAC(faSlot(islot), 3030, 0x0001); // S2R
	faSetDAC(faSlot(islot), 3047, 0x0002);
	faSetDAC(faSlot(islot), 3042, 0x0004);
	faSetDAC(faSlot(islot), 3040, 0x0008);
	faSetDAC(faSlot(islot), 3049, 0x0010);
	faSetDAC(faSlot(islot), 3015, 0x0020);
	faSetDAC(faSlot(islot), 3049, 0x0040);
	faSetDAC(faSlot(islot), 3024, 0x0080);
	faSetDAC(faSlot(islot), 3044, 0x0100);
	faSetDAC(faSlot(islot), 3022, 0x0200);
	faSetDAC(faSlot(islot), 3046, 0x0400);
	faSetDAC(faSlot(islot), 3034, 0x0800);
	faSetDAC(faSlot(islot), 3047, 0x1000);
	faSetDAC(faSlot(islot), 3039, 0x2000);
	faSetDAC(faSlot(islot), 3048, 0x4000);
	faSetDAC(faSlot(islot), 3037, 0x8000);
       // faSetDAC(faSlot(islot),3220,0xdfff);
       // faSetDAC(faSlot(islot),3200,0x2000);
       }

       
      if(islot==3){
	faSetDAC(faSlot(islot), 2500, 0xf); //changed for busy test : was 3150 
        faSetDAC(faSlot(islot), 3000, 0x0ff0);//BPM 
        faSetDAC(faSlot(islot), 3000, 0xf000);//prl1 and prl2 
      }
      if(islot==4||islot==5||islot==6||islot==7){
        faSetDAC(faSlot(islot), 3400, 0xffff); // jumper set to 0.5 V
      }
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
         if(islot==3||islot==4||islot==5||islot==6||islot==7)
             faSetProcMode(faSlot(islot), FADC_MODE, FADC_LA_Sh, FADC_WD_Sh, FADC_NSB, FADC_NSA, 1, 15,800,1);
         else faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_LATENCY, 2, 15, 1, 15,400,4);
       else{
         if(islot==3||islot==4||islot==5||islot==6||islot==7)
             faSetProcMode(faSlot(islot), FADC_MODE, FADC_LA_Sh, FADC_WD_Sh, FADC_NSB, FADC_NSA, 1, 15,800,1);
         else faSetProcMode(faSlot(islot), FADC_MODE, FADC_LATENCY, FADC_WINDOW_WIDTH, FADC_NSB, FADC_NSA, 1, 15,400,1);
       }
      //  faSetProcMode(faSlot(islot), 9, 135, 135, 5, 30, 1, 4,250,1);

    	    faSetTriggerBusyCondition(faSlot(islot),8);
	    //	    faBusyLevel(faSlot(islot),1,0);
	//	    faSetInvertedMode(faSlot(islot));

      faEnableTriggerOut(faSlot(islot), 2);

    }



  faSDC_Status(0);
  faGStatus(0);			//  6/7/2017

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
   vmeCheckMutexHealth(10);

  //FA_SLOT = fadcID[0];

  /* Program/Init VME Modules Here */
  for(ifa=0; ifa < nfadc; ifa++) 
    {
      faSoftReset(faSlot(ifa),0);
      faResetTriggerCount(faSlot(ifa));
      faResetToken(faSlot(ifa));
    }

  faGStatus(1);

  //F1
  int iflag = 0x0; /* with SDC */
  //iflag |= 0xE100;
  iflag |= 0xDD0;
  iflag |= 4;  /* read from file */
  printf("iflag = 0x%x\n",iflag);

  f1ConfigReadFile("/adaqfs/home/dvcs/IntelCPU/fadc_mike/BlkRead/f1_cfg_leftHRS_HighResSync.dat");
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

  //F1
  /* Enable modules, if needed, here */
  //f1GSoftReset();
  //f1ResetToken(f1Slot(0));
  //f1GEnableData(0xff);
  //f1GEnable();
for(if1=0; if1<nf1tdc; if1++) {
  F1_SLOT = f1ID[if1];
  f1EnableData(F1_SLOT,0xff);
}

  /* Interrupts/Polling enabled after conclusion of rocGo() */


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

  v1495ClockCountSyncCODA(MLUbase);		//reset MLU clock counter to zero

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

  printf("rocEnd: Ended after %d events\n", tirGetIntCount());
  ii=0;
}

/*****************************
 * TRIGGER 
 ****************************/
void
rocTrigger(int arg)
{

  int if1;
  int ii, nwords, islot,stat;
  int dCnt;
  unsigned int datascan = 0, scanmask = 0;
  unsigned int event_ty = 1, event_no = 0;

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
  // tirIntOutput(1<<0 | 1<<2);
  // tirIntOutput(1); commented out 11/30

  tirIntOutput(1<<2); //new

  // tir[1] ->tir_oport = 1; // added 9/19/2017

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
      //             nwords = faReadBlock(0, dma_dabufp, 5000, 2);	//changed rflag = 2 for Multi Block transfer 5/25/17
       
      nwords = faReadBlock(faSlot(0), dma_dabufp, 25000, 2);
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
    }
  else
    {
      printf("ERROR: Data not ready in event %d\n", tirGetIntCount());
       *dma_dabufp++ = LSWAP(0xda000bad);
    }

/*
    *dma_dabufp++ = LSWAP(0xbadbad1);
    *dma_dabufp++ = LSWAP(0xbadbad2);
    *dma_dabufp++ = LSWAP(0xbadbad3);
*/
    for (islot = 0; islot < nfadc; islot++)	// 5/25/17
      faResetToken(faSlot(islot));
  
  //  *dma_dabufp++ = LSWAP(0xda0000ff);	/* Event EOB */

  BANKCLOSE;

  ///////
  //F1 //
  ///////

  BANKOPEN(8,BT_UI4,0);
//  tirIntOutput(2);
  // Marco:
vmeDmaConfig(2,3,0);
  *dma_dabufp++ = LSWAP(tirGetIntCount());

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
  else
    {
      //printf("ERROR: Data not ready in event %d\n",tirGetIntCount());
      //Marco:
      printf("ERROR: Data not ready in event %d\n",tirGetIntCount());
      *dma_dabufp++ = LSWAP(0xda000bad);
    }
//}
  *dma_dabufp++ = LSWAP(0xda0000ff); // Event EOB 
}
vmeDmaConfig(2, 5, 1);

  BANKCLOSE;

  ///////
  //MLU//
  ///////

  BANKOPEN(1495,BT_UI4,0);		//MLU Clock Count bank
vmeDmaConfig(2,3,0);
  *dma_dabufp++ = LSWAP(0x14951495);
  *dma_dabufp++ = LSWAP(v1495ClockCountReadCODA());
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
	  printf("data in FADC FIFO\n");
	  		//FADC sync event bank
          nwords = faReadBlock(0, dma_dabufp, 25000, 2);	//changed rflag = 2 for Multi Block transfer 5/25/17
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
  // tir[0] -> tir_oport = 0; // added 9/19/2017
   tirIntOutput(0);
   //   tirIntOutput(2); // new
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
