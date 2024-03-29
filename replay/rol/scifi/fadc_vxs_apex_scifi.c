/*************************************************************************
 *
 *  ti_tsrev2.c - Library of routines for readout and buffering of
 *                events using a JLAB Pipeline Trigger Interface with
 *                the TS rev2 34-pin branch connection and a Linux VME
 *                controller.
 *
 */

/* Event Buffer definitions */



#define MAX_EVENT_POOL     128
#define MAX_EVENT_LENGTH   (66000<<2)      /* Size in Bytes */
#define MAX_EVENT_LENGTH   1024*32      /* Size in Bytes */

// MAXFADCWORDS = nfadc * (2 + 4 + 16 * blockLevel * (16 + FADC_WINDOW_WIDTH/2));
//  = 12 *( 2+4+16*(16+25)) =  7944 for 50 samples = 32 KB
// 

/* Define Interrupt source and address */
#define TI_MASTER
#define TI_READOUT TI_READOUT_TSREV2_POLL	/* Poll for available data, tsrev2 for evtype */
#define TI_ADDR    0	                        /* 0 = probe for TI */
#define FIBER_LATENCY_OFFSET 0                  /* Not needed */


/* TI Globals */
unsigned int MAXTIWORDS=0;
extern unsigned int tiTriggerSource;


/* Default block level MUST be 1, for use with TSRev2 */
unsigned int BLOCKLEVEL = 1;
int BUFFERLEVEL=10;
unsigned int blockLevel=  1;

/* FADC Defaults/Globals */
#define FADC_DAC_LEVEL    3100
#define FADC_THRESHOLD    1  
#define FADC_WINDOW_WIDTH 23 //  20 // 40 was 106
#define FADC_S2_WINDOW_WIDTH 15 //  20 // 40 was 106
int FADC_NPULSES =           4;
#define FADC_MODE           9

#define FADC_LATENCY       201 // was 88 
#define FADC_S2_LATENCY    195 // was 88 
#define FADC_LA_Sh         200 // was 73 //was 78 // was 62 
#define FADC_WD_Sh         50//80 // was /// RELEVANT
#define FADC_NSB           1  // # of samples *before* Threshold crossing (TC) to include in sum
#define FADC_NSA           40 // 20//15 // was 60 // # of samples *after* Threshold crossing (TC) to include in sum
#define FADC_SH_THRESHOLD     9 // changed 8/6/2017 from 300 : cosmic signals are not large enough to be above threshold
#define chan_mask  0x0000 // chan mask for threshold setting 

#define FADC_MODE_SciFi       10
#define FADC_LA_SciFi         201 // was 73 //was 78 // was 62 
#define FADC_WD_SciFi        100 // was /// RELEVANT
#define FADC_NSB_SciFi        2 
#define FADC_NSA_SciFi        40 

extern int fadcA32Base;
extern int nfadc;

#define NFADC 14
/* Address of first fADC250 */
#define FADC_ADDR 0x280000
/* Increment address to find next fADC250 */
#define FADC_INCR 0x080000

//wether or not to use threshold
#define WANT_THRESHOLD 0

#define WANT_THRESHOLD_SCIFI 1



#include <linux/prctl.h>
#include "dmaBankTools.h"
#include "tiprimary_list.c"	/* source required for CODA */
#include "fadcLib.h"
#include "sdLib.h"

#include "usrstrutils.c" // utils to pick up CODA ROC config parameter line

//MLU
#include "v1495.h"
int MLUbase = 0x00A00000;

int sync_or_unbuff;
static int buffered;
static int event_cnt = 0;
static int icnt = 0;

unsigned int fadcSlotMask=0;

/* for the calculation of maximum data words in the block transfer */
unsigned int MAXFADCWORDS=0;

unsigned int fadc_threshold=FADC_THRESHOLD;
unsigned int fadc_window_lat=FADC_LATENCY, fadc_window_width=FADC_WINDOW_WIDTH;


int tsCrate=1;

/* function prototype */
void rocTrigger(int arg);

/****************************************
 *  DOWNLOAD
 ****************************************/
void
rocDownload()
{
  int iFlag, ifa;
  /* Setup Address and data modes for DMA transfers
   *
   *  vmeDmaConfig(addrType, dataType, sstMode);
   *
   *  addrType = 0 (A16)    1 (A24)    2 (A32)
   *  dataType = 0 (D16)    1 (D32)    2 (BLK32) 3 (MBLK) 4 (2eVME) 5 (2eSST)
   *  sstMode  = 0 (SST160) 1 (SST267) 2 (SST320)
   */

 init_strings();
  buffered = getflag(BUFFERED);
  if (!buffered) BUFFERLEVEL=1 ;
  printf ("Buffer flag : %d\n",buffered);

  vmeDmaConfig(2, 5, 1);
  
  /*****************
   *   TI SETUP
   *****************/
  /* Set crate ID */
  tiSetCrateID(0x04);		/* e.g. ROC 4 */
  
  /* Use TSRev2 34pin-connector for event type and readout acknowledge
     Use Input TRG for Accepted trigger (e.g. L1A) from TSRev2
  */
  tiSetTriggerSource(TI_TRIGGER_TSREV2);
  
  /* Set number of events per block */
  if(BLOCKLEVEL != 1)
    {
      printf("%s: ERROR: Invalid BLOCKLEVEL (%d) for TI_TRIGGER_TSREV2.\n Setting to 1.\n",
	     __func__, BLOCKLEVEL);
      BLOCKLEVEL = 1;
    }
  tiSetBlockLevel(BLOCKLEVEL);
  
  /* Maximum blocks in the system that need read out */
  tiSetBlockBufferLevel(BUFFERLEVEL);
  
  tiStatus(0);
  
  
  /*******************
   *   FADC250 SETUP
   *******************/
  fadcA32Base=0x09000000;
  iFlag = 0;
  iFlag |= FA_INIT_EXT_SYNCRESET; /* External (VXS) SyncReset*/
  iFlag |= FA_INIT_VXS_TRIG;      /* VXS Input Trigger */
  iFlag |= FA_INIT_VXS_CLKSRC;    /* Internal Clock Source (Will switch later) */
  
  vmeSetQuietFlag(1);
  faInit(FADC_ADDR, FADC_INCR, NFADC, iFlag);
  vmeSetQuietFlag(0);
  
  if(nfadc>1)
    faEnableMultiBlock(1);
  
  fadcSlotMask=faScanMask();
  
  for(ifa = 0; ifa< nfadc; ifa++) 
    {
      faEnableBusError(faSlot(ifa));
      
      /* Set the internal DAC level */
      faSetDAC(faSlot(ifa), FADC_DAC_LEVEL, 0xffff);
      
      /* Set the threshold for data readout */
      faSetThreshold(faSlot(ifa), fadc_threshold, 0xffff);
      if (ifa>7)
	{
	fadc_threshold=FADC_SH_THRESHOLD;
	faSetThreshold(faSlot(ifa), fadc_threshold, 0xffff);
	fadc_threshold=FADC_THRESHOLD;
	}
      int ichan;
         for(ichan=0; ichan<16; ichan++)
	{
	    faSetChannelPedestal(faSlot(ifa),ichan,0);
	}
      
  if(WANT_THRESHOLD)
       {
       if(ifa==0)      faSetThreshold(faSlot(ifa), 300+25 , 0xffff);	//GC,SO,Ref
       else if(ifa==1) faSetThreshold(faSlot(ifa), 300+25 , 0xffff);	//S2
       else if(ifa==2) faSetThreshold(faSlot(ifa), 300+25 , 0xffff);	//S2
       else              faSetThreshold(faSlot(ifa), 300+50, 0xffff); //raster, BPMs, PRL1, PRL2 // was 1
       }
     else
       {
       faSetThreshold(faSlot(ifa), fadc_threshold, 0xffff); //0xffff sets all channels to same threshold
       }

  if(WANT_THRESHOLD_SCIFI){
	if(ifa==4||ifa==5||ifa==6||ifa==7)
	  faSetThreshold(faSlot(ifa), 300+200, 0xffff);
  }


if(ifa==0)      {
        faSetDAC(faSlot(ifa), 3072, 0x0001);   ///S0
	faSetDAC(faSlot(ifa), 3088, 0x0002);
	faSetDAC(faSlot(ifa), 3090, 0x0004); // GC
	faSetDAC(faSlot(ifa), 3092, 0x0008);
	faSetDAC(faSlot(ifa), 3091, 0x0010);
	faSetDAC(faSlot(ifa), 3103, 0x0020);
	faSetDAC(faSlot(ifa), 3080, 0x0040);
	faSetDAC(faSlot(ifa), 3077, 0x0080);
	faSetDAC(faSlot(ifa), 3093, 0x0100);
	faSetDAC(faSlot(ifa), 3085, 0x0200);
	faSetDAC(faSlot(ifa), 3096, 0x0400);
	faSetDAC(faSlot(ifa), 3102, 0x0800);
	faSetDAC(faSlot(ifa), 3071, 0x1000); // Gc sum
	faSetDAC(faSlot(ifa), 3027, 0x2000); // L1A Ref
	faSetDAC(faSlot(ifa), 2998, 0x4000); // last two channels are empty
	faSetDAC(faSlot(ifa), 2998, 0x8000);
	//faSetDAC(faSlot(ifa), 3200, 0xffff);  //previous default value
       }
if(ifa==1){
        faSetDAC(faSlot(ifa), 3040, 0x0001); /// S2L
	faSetDAC(faSlot(ifa), 3059, 0x0002);
	faSetDAC(faSlot(ifa), 3034, 0x0004);
	faSetDAC(faSlot(ifa), 3039, 0x0008);
	faSetDAC(faSlot(ifa), 3067, 0x0010);
	faSetDAC(faSlot(ifa), 3060, 0x0020);
	faSetDAC(faSlot(ifa), 3035, 0x0040);
	faSetDAC(faSlot(ifa), 3047, 0x0080);
	faSetDAC(faSlot(ifa), 3057, 0x0100);
	faSetDAC(faSlot(ifa), 3049, 0x0200);
	faSetDAC(faSlot(ifa), 3053, 0x0400);
	faSetDAC(faSlot(ifa), 3037, 0x0800);
	faSetDAC(faSlot(ifa), 3068, 0x1000);
	faSetDAC(faSlot(ifa), 3056, 0x2000);
	faSetDAC(faSlot(ifa), 3052, 0x4000);
	faSetDAC(faSlot(ifa), 3046, 0x8000);
	//faSetDAC(faSlot(ifa), 3200, 0xffff);  //previous default value
       }
if(ifa==2){
        faSetDAC(faSlot(ifa), 3030, 0x0001); // S2R
	faSetDAC(faSlot(ifa), 3047, 0x0002);
	faSetDAC(faSlot(ifa), 3042, 0x0004);
	faSetDAC(faSlot(ifa), 3040, 0x0008);
	faSetDAC(faSlot(ifa), 3049, 0x0010);
	faSetDAC(faSlot(ifa), 3015, 0x0020);
	faSetDAC(faSlot(ifa), 3049, 0x0040);
	faSetDAC(faSlot(ifa), 3024, 0x0080);
	faSetDAC(faSlot(ifa), 3044, 0x0100);
	faSetDAC(faSlot(ifa), 3022, 0x0200);
	faSetDAC(faSlot(ifa), 3046, 0x0400);
	faSetDAC(faSlot(ifa), 3034, 0x0800);
	faSetDAC(faSlot(ifa), 3047, 0x1000);
	faSetDAC(faSlot(ifa), 3039, 0x2000);
	faSetDAC(faSlot(ifa), 3048, 0x4000);
	faSetDAC(faSlot(ifa), 3037, 0x8000);
       // faSetDAC(faSlot(ifa),3220,0xdfff);
       // faSetDAC(faSlot(ifa),3200,0x2000);
       }

       
      if(ifa==3){
	faSetDAC(faSlot(ifa), 2500, 0xf); //changed for busy test : was 3150 
        faSetDAC(faSlot(ifa), 3000, 0x0ff0);//BPM 

	// last four channels for PRLs (PRL1(16),PRL1(33),PRL2(16),PRL2(33))
	faSetDAC(faSlot(ifa), 3279, 0x1000);
	faSetDAC(faSlot(ifa), 3279, 0x2000);
	faSetDAC(faSlot(ifa), 3279, 0x4000);   // 3414
	faSetDAC(faSlot(ifa), 3279, 0x8000);   // 3443
	  //        faSetDAC(faSlot(ifa), 3450, 0xf000);//prl1 and prl2 
      }

      // SciFi slots
      if(ifa==4){
	faSetDAC(faSlot(ifa), 3243, 0x0001); /// SciFi(A)
	faSetDAC(faSlot(ifa), 3264, 0x0002);
	faSetDAC(faSlot(ifa), 3268, 0x0004);
	faSetDAC(faSlot(ifa), 3257, 0x0008);
	faSetDAC(faSlot(ifa), 3236, 0x0010);
	faSetDAC(faSlot(ifa), 3257, 0x0020);
	faSetDAC(faSlot(ifa), 3236, 0x0040);
	faSetDAC(faSlot(ifa), 3257, 0x0080);
	faSetDAC(faSlot(ifa), 3257, 0x0100);
	faSetDAC(faSlot(ifa), 3264, 0x0200);
	faSetDAC(faSlot(ifa), 3263, 0x0400);
	faSetDAC(faSlot(ifa), 3232, 0x0800);
	faSetDAC(faSlot(ifa), 3233, 0x1000);
	faSetDAC(faSlot(ifa), 3249, 0x2000);
	faSetDAC(faSlot(ifa), 3257, 0x4000);
	faSetDAC(faSlot(ifa), 3261, 0x8000);
      }
      if(ifa==5){	
	faSetDAC(faSlot(ifa), 3274, 0x0001); /// SciFi(B)
	faSetDAC(faSlot(ifa), 3240, 0x0002);
	faSetDAC(faSlot(ifa), 3257, 0x0004);
	faSetDAC(faSlot(ifa), 3271, 0x0008);
	faSetDAC(faSlot(ifa), 3279, 0x0010);
	faSetDAC(faSlot(ifa), 3232, 0x0020);
	faSetDAC(faSlot(ifa), 3216, 0x0040);
	faSetDAC(faSlot(ifa), 3226, 0x0080);
	faSetDAC(faSlot(ifa), 3257, 0x0100);
	faSetDAC(faSlot(ifa), 3275, 0x0200);
	faSetDAC(faSlot(ifa), 3290, 0x0400);
	faSetDAC(faSlot(ifa), 3241, 0x0800);
	faSetDAC(faSlot(ifa), 3282, 0x1000);
	faSetDAC(faSlot(ifa), 3293, 0x2000);
	faSetDAC(faSlot(ifa), 3248, 0x4000);
	faSetDAC(faSlot(ifa), 3249, 0x8000); // changex thuis, was 3249
      }

      if(ifa==6){
	faSetDAC(faSlot(ifa), 3275, 0x0001); /// SciFi(C)
	faSetDAC(faSlot(ifa), 3275, 0x0002);
	faSetDAC(faSlot(ifa), 3263, 0x0004);
	faSetDAC(faSlot(ifa), 3271, 0x0008);
	faSetDAC(faSlot(ifa), 3246, 0x0010);
	faSetDAC(faSlot(ifa), 3260, 0x0020);
	faSetDAC(faSlot(ifa), 3266, 0x0040);
	faSetDAC(faSlot(ifa), 3252, 0x0080);
	faSetDAC(faSlot(ifa), 3275, 0x0100);
	faSetDAC(faSlot(ifa), 3275, 0x0200);
	faSetDAC(faSlot(ifa), 3095, 0x0400);
	faSetDAC(faSlot(ifa), 3146, 0x0800);
	faSetDAC(faSlot(ifa), 3253, 0x1000);
	faSetDAC(faSlot(ifa), 3255, 0x2000);
	faSetDAC(faSlot(ifa), 3257, 0x4000);
	faSetDAC(faSlot(ifa), 3243, 0x8000);

      }

      if(ifa==7){
	faSetDAC(faSlot(ifa), 3303, 0x0001); /// SciFi(D)
	faSetDAC(faSlot(ifa), 3272, 0x0002);
	faSetDAC(faSlot(ifa), 3336, 0x0004);
	faSetDAC(faSlot(ifa), 3249, 0x0008);
	faSetDAC(faSlot(ifa), 3257, 0x0010);
	faSetDAC(faSlot(ifa), 3311, 0x0020);
	faSetDAC(faSlot(ifa), 3296, 0x0040);
	faSetDAC(faSlot(ifa), 3296, 0x0080);
	faSetDAC(faSlot(ifa), 3275, 0x0100);
	faSetDAC(faSlot(ifa), 3296, 0x0200);
	faSetDAC(faSlot(ifa), 3311, 0x0400);
	faSetDAC(faSlot(ifa), 3293, 0x0800);
	faSetDAC(faSlot(ifa), 3293, 0x1000);
	faSetDAC(faSlot(ifa), 3291, 0x2000);
	faSetDAC(faSlot(ifa), 3253, 0x4000);
	faSetDAC(faSlot(ifa), 3293, 0x8000);
      }
      // jumper set to 0.5 V
      

      // added due to moving of PRLs

        
      if(ifa==8){
	faSetDAC(faSlot(ifa), 3279, 0xffff);  // PRL1 (0-15)
       }

     
      /*      if(ifa==8){
        faSetDAC(faSlot(ifa), 3534, 0x0001);  PRl1 (0-15)
	faSetDAC(faSlot(ifa), 3502, 0x0002);
	faSetDAC(faSlot(ifa), 3523, 0x0004);
	faSetDAC(faSlot(ifa), 3518, 0x0008);
	faSetDAC(faSlot(ifa), 3514, 0x0010);
	faSetDAC(faSlot(ifa), 3522, 0x0020);
	faSetDAC(faSlot(ifa), 3514, 0x0040);
	faSetDAC(faSlot(ifa), 3547, 0x0080);
	faSetDAC(faSlot(ifa), 3521, 0x0100);
	faSetDAC(faSlot(ifa), 3499, 0x0200);
	faSetDAC(faSlot(ifa), 3532, 0x0400);
	faSetDAC(faSlot(ifa), 3514, 0x0800);
	faSetDAC(faSlot(ifa), 3529, 0x1000);
	faSetDAC(faSlot(ifa), 3522, 0x2000);
	faSetDAC(faSlot(ifa), 3530, 0x4000); PR
	faSetDAC(faSlot(ifa), 3523, 0x8000);
      }
      */
      
      if(ifa==9||ifa==10||ifa==11){         // 9 == PRL1 (17-32), 10 ==  PRl1 (0-15), 11 == PRl1 (17-32)
        faSetDAC(faSlot(ifa), 3250, 0xffff); // jumper set to 0.5 V
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
       *MAXPED : Maximum value of sample to be included in pedestal sum
       *  NSAT : Number of consecutive samples over threshold for valid pulse
       */
      faSetProcMode(faSlot(ifa),
		    FADC_MODE,
		    FADC_LATENCY,
		    FADC_WINDOW_WIDTH,
		    1,   /* NSB */
		    6,   /* NSA */
		    4,   /* NP */
		    4,   /* NPED */
		    320, /* MAXPED */
		    2);  /* NSAT */

   if(WANT_THRESHOLD)
         if(ifa==9||ifa==10||ifa==11||ifa==12)
             faSetProcMode(faSlot(ifa), FADC_MODE, FADC_LA_Sh, FADC_WD_Sh, FADC_NSB, FADC_NSA, FADC_NPULSES, 15,400,3);
         else faSetProcMode(faSlot(ifa), FADC_MODE, FADC_LA_SciFi, FADC_WD_SciFi, FADC_NSB_SciFi, FADC_NSA_SciFi, FADC_NPULSES, 15,357,2); // nsat was 4
   else{
	 printf("\n=================== \n else statement executed, ifa = %d \n",ifa);	 
         if(ifa==1||ifa==2)
	   faSetProcMode(faSlot(ifa), FADC_MODE, FADC_S2_LATENCY, FADC_S2_WINDOW_WIDTH, FADC_NSB, FADC_NSA, 1, 4,400,2); // S2
         if(ifa==0||ifa==3)
	   faSetProcMode(faSlot(ifa), FADC_MODE, FADC_LATENCY, FADC_WINDOW_WIDTH, FADC_NSB, FADC_NSA, 1, 4,400,2);  // S0 and Cherenkov
         // faSetProcMode(faSlot(ifa), FADC_MODE, FADC_LA_Sh, FADC_WD_Sh, FADC_NSB, FADC_NSA, 1, 15,800, 1);
	 if(ifa==4||ifa==5||ifa==6 || ifa==7)
	   faSetProcMode(faSlot(ifa), FADC_MODE_SciFi, FADC_LA_SciFi, FADC_WD_SciFi, FADC_NSB_SciFi, FADC_NSA_SciFi, 1, 15,400,2);	   // SciFi
         if(ifa==8||ifa==9||ifa==10 || ifa==11)
	   faSetProcMode(faSlot(ifa), FADC_MODE, FADC_LA_Sh, FADC_WD_Sh, FADC_NSB, FADC_NSA, FADC_NPULSES, 4,400, 3);  // PRLs ( shower detectors )
       }
  }
  faGStatus(0);
  
  /*****************
   *   SD SETUP
   *****************/
  sdInit(0);
  sdSetActiveVmeSlots(fadcSlotMask);
  sdStatus(1);
  

 v1495InitCODA(MLUbase,0);		// initialize MLU

  printf("%s: User Download Executed\n",
	 __func__);
}


/****************************************
 *  PRESTART
 ****************************************/
void
rocPrestart()
{

 int ifa;

  /* Program/Init VME Modules Here */
  for(ifa=0;ifa<nfadc;ifa++) 
    {
      faEnableSyncSrc(faSlot(ifa));
      faSoftReset(faSlot(ifa),0);
      faResetToken(faSlot(ifa));
      faResetTriggerCount(faSlot(ifa));
    }

  tiStatus(0);


  /* Set number of events per block (broadcasted to all connected TI Slaves) */
  tiSetBlockLevel(BLOCKLEVEL);
  printf("%s: Block Level set to %d\n",
	 __func__, BLOCKLEVEL);

  tiStatus(0);

  printf("%s: User Prestart Executed\n",
	 __func__);

}

/****************************************
 *  GO
 ****************************************/
void
rocGo()
{
  /* Enable modules, if needed, here */

  /* Get the current block level */
  /* Use this info to change block level is all modules */
  BLOCKLEVEL = tiGetCurrentBlockLevel();
  printf("%s: Current Block Level = %d\n", __func__, BLOCKLEVEL);

  tiStatus(0);

  int ifa;

  /* Enable modules, if needed, here */
  //ctpAlignAtSyncReset(0);
  //ctpGetAlignmentStatus(1,10);

  for(ifa=0;ifa<nfadc;ifa++)
    {
      faChanDisable(faSlot(ifa),0x0);
    }

  faGStatus(0);
  faGDisable(0);
  /* Get the current block level */
  blockLevel = tiGetCurrentBlockLevel();
  printf("%s: Current Block Level = %d\n",
	 __FUNCTION__,blockLevel);

  faGSetBlockLevel(blockLevel);

  if(FADC_MODE == 9)
    MAXFADCWORDS = nfadc * (2 + 4 + 16* blockLevel * 16);
  else /* FADC_MODE == 10 */
    MAXFADCWORDS = nfadc * (2 + 4 + 16 * blockLevel * (16 + FADC_WINDOW_WIDTH/2));
 

  faGEnable(0, 0);
  /* Interrupts/Polling enabled after conclusion of rocGo() */
  v1495ClockCountSyncCODA(MLUbase);		//reset MLU clock counter to zero

}

/****************************************
 *  END
 ****************************************/
void
rocEnd()
{
 
  faGDisable(0);
  faGStatus(0);
  
  tiStatus(0);
  sdStatus(1);
  printf("%s: Ended after %d blocks\n",
	 __func__, tiGetIntCount());

}

/****************************************
 *  TRIGGER
 ****************************************/
void
rocTrigger(int arg)
{
  int dCnt;
  int ev_type = 0;
  int ifa, stat, nwords;
  int roCount = 0, blockError = 0;
  int rval = OK;
  int sync_flag = 0, late_fail = 0;
  unsigned int datascan = 0, scanmask = 0;
  unsigned int event_ty = 1, event_no = 0;
  int islot;
 
    roCount = tiGetIntCount();


  tiSetOutputPort(1, 0, 0, 0);

  /* Open an event of event_type = 1 (** redefined later **).
     Data contained in banks (BT_BANK) */
  //EVENTOPEN(1, BT_BANK);

  /* Open a bank of type (for example) 4
     Each data word is a 4-byte unsigned integer */
  BANKOPEN(4, BT_UI4, 0);

  vmeDmaConfig(2, 5, 1);
  dCnt = tiReadBlock(dma_dabufp, 8 + (5 * BLOCKLEVEL), 1);
  if (dCnt <= 0)
    {
      printf("No data or error.  dCnt = %d\n", dCnt);
    }
  else
    {
      
  rval = tiDecodeTSrev2Data((volatile unsigned int *) dma_dabufp, dCnt,
				(int *)&sync_flag, (int *)&late_fail, (int *)&ev_type);

      if(rval == OK)
	{
	  if ((ev_type <= 0) || (ev_type > 0xF))
	    {
	      printf("%s: ERROR: Invalid event type (%d)\n",
		     __func__, ev_type);
	      ev_type = 1;
	    }

	  /* ** Redefine event type in event buffer ** */
	  the_event->type = ev_type | (sync_flag << 8);

	  /*	  printf("%s: the_event->type = 0x%x\t ev_type = %d  sync_flag = %d\n",
	    __func__, the_event->type, ev_type, sync_flag); */
	}

      dma_dabufp += dCnt;
    }

  BANKCLOSE;

   event_ty = ev_type;
   event_no = event_cnt;


 /* fADC250 Readout */
  BANKOPEN(250,BT_UI4,0);

  /* Mask of initialized modules */
  scanmask = faScanMask();
  /* Check scanmask for block ready up to 100 times */
  datascan = faGBlockReady(scanmask, 100); 
  stat = (datascan == scanmask);
    
  if(stat) 
    {
         nwords = faReadBlock(faSlot(0), dma_dabufp, MAXFADCWORDS, 2);

      /* Check for ERROR in block read */
      blockError = faGetBlockError(1);

      if(blockError) 
	{
	  printf("ERROR: in transfer (event = %d), nwords = 0x%x\n",
		 roCount, nwords);
	  for(ifa = 0; ifa < nfadc; ifa++)
	    faResetToken(faSlot(ifa));

	  if(nwords > 0)
	    dma_dabufp += nwords;
	} 
      else 
	{
	  dma_dabufp += nwords;
	  faResetToken(faSlot(0));
	}
    }
  else 
    {
      printf("ERROR: Event %d: Datascan != Scanmask  (0x%08x != 0x%08x)\n",
	     roCount, datascan, scanmask);
    }

  BANKCLOSE;

  ///////
  //MLU//
  ///////

  unsigned int clockcount = v1495ClockCountReadCODA(); //this must come before BCM readout

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
    *dma_dabufp++ = LSWAP(clockcount);			//32 bit Clock Count
    *dma_dabufp++ = LSWAP(bcmu_h);		//upper 32 bits of upstream bcm
    *dma_dabufp++ = LSWAP(bcmu_l);		//lower 32 bits of upstream bcm
    *dma_dabufp++ = LSWAP(bcmd_h);		//upper 32 bits of downstream bcm
    *dma_dabufp++ = LSWAP(bcmd_l);		//lower 32 bits of downstream bcm
    *dma_dabufp++ = LSWAP(v1495BCMcurrent_ReadCODA(0));			//latest upstream BCM current value
    *dma_dabufp++ = LSWAP(v1495BCMcurrent_ReadCODA(1));			//latest downstream BCM current value
    *dma_dabufp++ = LSWAP(0x14950000);
    vmeDmaConfig(2, 5, 1);
  BANKCLOSE;


 BANKOPEN(0xfabc,BT_UI4,0);		//Sync checks
  event_cnt = event_cnt + 1;
  event_ty = (ev_type&0xf);
  icnt = icnt + 1;
  if(icnt > 20000) icnt = 0;
  *dma_dabufp++ = LSWAP(0xfabc0004);
  *dma_dabufp++ = LSWAP(event_ty);
  *dma_dabufp++ = LSWAP(event_cnt);
  *dma_dabufp++ = LSWAP(icnt);
  *dma_dabufp++ = LSWAP(syncFlag);
  *dma_dabufp++ = LSWAP(0xfaaa0001);
  BANKCLOSE;
  if(tiGetSyncEventFlag() == 1|| !buffered)
    {
      /* Flush out TI data, if it's there (out of sync) */
      int davail = tiBReady();
      if(davail > 0)
	{
	  printf("%s: ERROR: TI Data available (%d) after readout in SYNC event \n",
		 __func__, davail);
	  
	  while(tiBReady())
	    {
	      vmeDmaFlush(tiGetAdr32());
	    }
	}
      
      /* Flush out other modules too, if necessary */
      // flush FADCs
      scanmask = faScanMask();
      /* Check scanmask for block ready up to 100 times */
      datascan = faGBlockReady(scanmask, 100); 
      stat = (datascan == scanmask);
      stat =0;
      if (stat > 0)
	{
	  printf("data left in FADC FIFO at sync event\n");
	  //FADC sync event bank
	  // nwords = faReadBlock(0, dma_dabufp, 5000, 2);	//changed rflag = 2 for Multi Block transfer 5/25/17
	  BANKOPEN(0xbad,BT_UI4,0);
	  *dma_dabufp++ = LSWAP(0xfadc250);
	  nwords = faReadBlock(faSlot(0), dma_dabufp, 7200, 2);
	  // nwords = 0;
	  // nwords = 0;
	  if (nwords < 0)
	    {
	      printf("ERROR: in transfer (event = %d), nwords = 0x%x\n",
		     tirGetIntCount(), nwords);
	        *dma_dabufp++ = LSWAP(0xda000bad);
	      
	    }
	  else
	    {
	      dma_dabufp += nwords;
	    }
	    BANKCLOSE;
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
    
    }
  // EVENTCLOSE;
  tiSetOutputPort(0, 0, 0, 0);
}


void
rocCleanup()
{

  
  printf("%s: Reset all FADCs\n",__FUNCTION__);
  faGReset(1);
  

}


/*
  Local Variables:
  compile-command: "make -B ti_tsrev2.so"
  End:
 */
