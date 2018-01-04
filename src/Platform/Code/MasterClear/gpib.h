/*============================================================================*/
/*                        L a b W i n d o w s / C V I                         */
/*----------------------------------------------------------------------------*/
/*    Copyright (c) National Instruments 1987-1999.  All Rights Reserved.     */
/*----------------------------------------------------------------------------*/
/*                                                                            */
/* Title:       gpib.h                                                        */
/* Purpose:     Include file for Win32 and Solaris 2                          */
/*                                                                            */
/*============================================================================*/

#ifndef _GPIB_HEADER_
#define _GPIB_HEADER_

#ifdef _CVI_
#pragma EnableLibraryRuntimeChecking
#endif

#include "cvidef.h"

#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************/
/* Global variables.                                                      */
/**************************************************************************/

extern int  ibsta, iberr, ibcnt;

#if defined(_NI_mswin32_)
extern long ibcntl;

#elif defined(_NI_unix_)
#define  ibcntl  ibcnt

#endif

/**************************************************************************/
/* Function types.                                                        */
/**************************************************************************/

#define CVIGPIBFUNC CVIFUNC

/**************************************************************************/
/*  Device Manager Function Prototypes                                    */
/**************************************************************************/
int CVIGPIBFUNC OpenDev (char devname[], char instrPrefix[]);
int CVIGPIBFUNC CloseDev (int bd);
int CVIGPIBFUNC CloseInstrDevs (char instrPrefix[]);

/**************************************************************************/
/*  NI-488 Function Prototypes                                            */
/**************************************************************************/
int CVIGPIBFUNC ibask (int bdid, int option, int *v);
int CVIGPIBFUNC ibbna (int ud, char udname[]);
int CVIGPIBFUNC ibcac (int ud, int v);
int CVIGPIBFUNC ibclr (int ud);
int CVIGPIBFUNC ibcmd (int ud, void *buf, long cnt);
int CVIGPIBFUNC ibcmda (int ud, void *buf, long cnt);
int CVIGPIBFUNC ibconfig (int bdid, int option, int v);
int CVIGPIBFUNC ibdev (int ud, int pad, int sad, int tmo, int eot, int eos);
int CVIGPIBFUNC ibdiag (int ud, void *buf, long cnt);     /* no function panel */
int CVIGPIBFUNC ibdma (int ud, int v);
int CVIGPIBFUNC ibeos (int ud, int v);
int CVIGPIBFUNC ibeot (int ud, int v);
#if defined(_NI_mswin32_)
int CVIGPIBFUNC ibevent (int ud, short *event);  /* MS Windows only */
#endif
int CVIGPIBFUNC ibfind (char udname[]);
int CVIGPIBFUNC ibgts (int ud, int v);
int CVIGPIBFUNC ibist (int ud, int v);
int CVIGPIBFUNC iblines (int ud, short *result);
int CVIGPIBFUNC ibln (int ud, int pad, int sad, short *listen);
int CVIGPIBFUNC ibloc (int ud);
int CVIGPIBFUNC iblock (int ud);             /* GPIB-ENET only */
int CVIGPIBFUNC ibonl (int ud, int v);
int CVIGPIBFUNC ibpad (int ud, int v);
int CVIGPIBFUNC ibpct (int ud);
int CVIGPIBFUNC ibppc (int ud, int v);
int CVIGPIBFUNC ibrd (int ud, void *buf, long cnt);
int CVIGPIBFUNC ibrda (int ud, void *buf, long cnt);
int CVIGPIBFUNC ibrdf (int ud, char filename[]);
int CVIGPIBFUNC ibrdkey (int ud, void *buf, int cnt);
int CVIGPIBFUNC ibrpp (int ud, char *ppr);
int CVIGPIBFUNC ibrsc (int ud, int v);
int CVIGPIBFUNC ibrsp (int ud, char *spr);
int CVIGPIBFUNC ibrsv (int ud, int v);
int CVIGPIBFUNC ibsad (int ud, int v);
int CVIGPIBFUNC ibsic (int ud);
#ifdef _NI_unix_
int CVIGPIBFUNC ibsgnl (int ud, int mask);  /* UNIX only */
#endif
int CVIGPIBFUNC ibsre (int ud, int v);
int CVIGPIBFUNC ibstop (int ud);
int CVIGPIBFUNC ibtmo (int ud, int v);
int CVIGPIBFUNC ibtrg (int ud);
int CVIGPIBFUNC ibunlock (int ud);          /* GPIB-ENET only */
int CVIGPIBFUNC ibwait (int ud, int mask);
int CVIGPIBFUNC ibwrt (int ud, void *buf, long cnt);
int CVIGPIBFUNC ibwrta (int ud, void *buf, long cnt);
int CVIGPIBFUNC ibwrtf (int ud, char filename[]);
int CVIGPIBFUNC ibwrtkey (int ud, void *buf, int cnt);

/**************************************************************************/
/*  NI-488.2 Function Prototypes                                          */
/**************************************************************************/

typedef  short  Addr4882_t;

void CVIGPIBFUNC AllSpoll (int boardID, Addr4882_t taddrs[], short results[]);
void CVIGPIBFUNC DevClear (int boardID, Addr4882_t address);
void CVIGPIBFUNC DevClearList (int boardID, Addr4882_t addrlist[]);
void CVIGPIBFUNC EnableLocal (int boardID, Addr4882_t laddrs[]);
void CVIGPIBFUNC EnableRemote (int boardID, Addr4882_t laddrs[]);
void CVIGPIBFUNC FindLstn (int boardID, Addr4882_t pads[], short results[], int limit);
void CVIGPIBFUNC FindRQS (int boardID, Addr4882_t taddrs[], short *dev_stat);
void CVIGPIBFUNC PPoll (int boardID, short *result);
void CVIGPIBFUNC PPollConfig (int boardID, Addr4882_t laddr, int dataLine, int lineSense);
void CVIGPIBFUNC PPollUnconfig (int boardID, Addr4882_t laddrs[]);
void CVIGPIBFUNC PassControl (int boardID, Addr4882_t talker);
void CVIGPIBFUNC RcvRespMsg (int boardID, void *buf, long cnt, int eotMode);
void CVIGPIBFUNC ReadStatusByte (int boardID, Addr4882_t talker, short *result);
void CVIGPIBFUNC Receive (int boardID, Addr4882_t talker, void *buf, long cnt, int eotMode);
void CVIGPIBFUNC ReceiveSetup (int boardID, Addr4882_t talker);
void CVIGPIBFUNC ResetSys (int boardID, Addr4882_t laddrs[]);
void CVIGPIBFUNC Send (int boardID, Addr4882_t listener, void *buf, long datacnt, int eotMode);
void CVIGPIBFUNC SendCmds (int boardID, void *buf, long cnt);
void CVIGPIBFUNC SendDataBytes (int boardID, void *buf, long cnt, int eot_mode);
void CVIGPIBFUNC SendIFC (int boardID);
void CVIGPIBFUNC SendLLO (int boardID);
void CVIGPIBFUNC SendList (int boardID, Addr4882_t laddrs[], void *buf, long datacnt, int eotMode);
void CVIGPIBFUNC SendSetup (int boardID, Addr4882_t laddrs[]);
void CVIGPIBFUNC SetRWLS (int boardID, Addr4882_t laddrs[]);
void CVIGPIBFUNC TestSRQ (int boardID, short *result);
void CVIGPIBFUNC TestSys (int boardID, Addr4882_t addrs[], short result[]);
void CVIGPIBFUNC Trigger (int boardID, Addr4882_t laddrs);
void CVIGPIBFUNC TriggerList (int boardID, Addr4882_t laddrs[]);
void CVIGPIBFUNC WaitSRQ (int boardID, short *result);

/**************************************************************************/
/* Thread-specific status variable functions.                             */
/**************************************************************************/
int  CVIGPIBFUNC ThreadIbsta (void);
int  CVIGPIBFUNC ThreadIberr (void);
int  CVIGPIBFUNC ThreadIbcnt (void);
long CVIGPIBFUNC ThreadIbcntl (void);

    /* the following function is undocumented */
void CVIGPIBFUNC SetGpibStatusVariablesForThread (int sta, int err, long cntl);

/**************************************************************************/
/* Asynchronous callback function prototypes.                             */
/**************************************************************************/

#if defined(_NI_mswin32_)
typedef int (__stdcall * GpibNotifyCallback_t) (int ud, int ibsta, int iberr, long ibcntl,
                                                void *callbackData);

int CVIGPIBFUNC ibnotify (int ud, int mask, GpibNotifyCallback_t callbackFunction,
                          void *callbackData);
#endif

/**************************************************************************/
/* Synchronous callback function prototypes.                              */
/**************************************************************************/

#if defined(_NI_mswin32_)
typedef void (CVICALLBACK *GPIBCallbackPtr) (int ud, int mask, void *callbackData);

int CVIGPIBFUNC ibInstallCallback (int ud, int mask, GPIBCallbackPtr callbackFunction, 
                                   void *callbackData);
#endif

/**************************************************************************/
/*  VXI Extensions Function Prototypes                                    */
/**************************************************************************/

#if defined(_NI_mswin32_)
void CVIGPIBFUNC GenerateREQF(int boardID, unsigned short addr);
void CVIGPIBFUNC GenerateREQT(int boardID, unsigned short addr);
void CVIGPIBFUNC GotoMultAddr(int boardID,
                              unsigned short type,
                              unsigned short (*addrfunc)(int boardID, 
                                                         unsigned short type, 
                                                         unsigned short addr),
                              unsigned short (*spollfunc)(int boardID, 
                                                          unsigned short addr));
#endif

/***************************************************************************/
/*    HANDY CONSTANTS FOR USE BY APPLICATION PROGRAMS ...                  */
/***************************************************************************/

/* GPIB Commands                                           */

#define UNL  0x3f  /* GPIB unlisten command                */
#define UNT  0x5f  /* GPIB untalk command                  */
#define GTL  0x01  /* GPIB go to local                     */
#define SDC  0x04  /* GPIB selected device clear           */
#define PPC  0x05  /* GPIB parallel poll configure         */
#define GET  0x08  /* GPIB group execute trigger           */
#define TCT  0x09  /* GPIB take control                    */
#define LLO  0x11  /* GPIB local lock out                  */
#define DCL  0x14  /* GPIB device clear                    */
#define PPU  0x15  /* GPIB parallel poll unconfigure       */
#define SPE  0x18  /* GPIB serial poll enable              */
#define SPD  0x19  /* GPIB serial poll disable             */
#define PPE  0x60  /* GPIB parallel poll enable            */
#define PPD  0x70  /* GPIB parallel poll disable           */

/* GPIB status bit vector :                                */
/*       global variable ibsta and wait mask               */

#define ERR     (1<<15) /* Error detected                  */
#define TIMO    (1<<14) /* Timeout                         */
#define END     (1<<13) /* EOI or EOS detected             */
#define SRQI    (1<<12) /* SRQ detected by CIC             */
#define RQS     (1<<11) /* Device needs service            */
#define SPOLL   (1<<10) /* Board has been serially polled  */
#define EVENT   (1<<9)  /* An event has occured            */
#define CMPL    (1<<8)  /* I/O completed                   */
#define LOK     (1<<7)  /* Local lockout state             */
#define REM     (1<<6)  /* Remote state                    */
#define CIC     (1<<5)  /* Controller-in-Charge            */
#define ATN     (1<<4)  /* Attention asserted              */
#define TACS    (1<<3)  /* Talker active                   */
#define LACS    (1<<2)  /* Listener active                 */
#define DTAS    (1<<1)  /* Device trigger state            */
#define DCAS    (1<<0)  /* Device clear state              */

/* Error messages returned in global variable iberr        */

#define EDVR  0  /* Operating system error                 */
#define ECIC  1  /* Function requires GPIB board to be CIC */
#define ENOL  2  /* Write function detected no Listeners   */
#define EADR  3  /* Interface board not addressed correctly*/
#define EARG  4  /* Invalid argument to function call      */
#define ESAC  5  /* Function requires GPIB board to be SAC */
#define EABO  6  /* I/O operation aborted                  */
#define ENEB  7  /* Non-existent interface board           */
#define EDMA  8  /* Error performing DMA buffer            */
#define EOIP 10  /* I/O operation started before previous  */
                 /* operation completed                    */
#define ECAP 11  /* No capability for intended operation   */
#define EFSO 12  /* File system operation error            */
#define EBUS 14  /* Command error during device call       */
#define ESTB 15  /* Serial poll status byte lost           */
#define ESRQ 16  /* SRQ remains asserted                   */
#define ETAB 20  /* The return buffer is full.             */
#define ELCK 21  /* Address or board is locked.            */
#define ECFG 24  /* The GPIB-ENET was already on-line, and */
                 /* the default board configuration sent   */
                 /* to it differs from than configuration  */
                 /* under which it was already operating.  */
                 /* This is only a warning.  The board     */
                 /* configuration has not been changed,    */
                 /* but the operation has otherwise        */
                 /* completed successfully.                */

#define ELNK 200 /* The GPIB library was not linked;       */
                 /* dummy functions were linked instead    */
#define EDLL 201 /* Error loading GPIB32.DLL;  ibcnt       */
                 /* contains the MS Windows error code     */
#define EFNF 203 /* Unable to find the function in         */
                 /* in GPIB32.DLL;  ibcnt contains the     */
                 /* MS Windows error code                  */
#define EGLB 205 /* Globals or copyright string not found  */
                 /* in GPIB32.DLL.                         */
#define ENNI 206 /* Not a National Instruments GPIB32.DLL  */
#define EMTX 207 /* Unable to acquire Mutex for loading    */
                 /* DLL.  The MS Windows error is in ibcnt */

#define EMSG 210 /* Unable to register callback function   */
                 /* with Windows                           */
#define ECTB 211 /* Callback table is full                 */

/* ibnotify rearm failure:  value in ibcntl                */
#define IBNOTIFY_REARM_FAILED     0xE00A003F

/* EOS mode bits                                           */

#define BIN  (1<<12) /* Eight bit compare                  */
#define XEOS (1<<11) /* Send END with EOS byte         */
#define REOS (1<<10) /* Terminate read on EOS              */

/* Timeout values and meanings                             */

#define TNONE    0   /* Infinite timeout (disabled)        */
#define T10us    1   /* Timeout of 10 us (ideal)           */
#define T30us    2   /* Timeout of 30 us (ideal)           */
#define T100us   3   /* Timeout of 100 us (ideal)          */
#define T300us   4   /* Timeout of 300 us (ideal)          */
#define T1ms     5   /* Timeout of 1 ms (ideal)            */
#define T3ms     6   /* Timeout of 3 ms (ideal)            */
#define T10ms    7   /* Timeout of 10 ms (ideal)           */
#define T30ms    8   /* Timeout of 30 ms (ideal)           */
#define T100ms   9   /* Timeout of 100 ms (ideal)          */
#define T300ms  10   /* Timeout of 300 ms (ideal)          */
#define T1s     11   /* Timeout of 1 s (ideal)             */
#define T3s     12   /* Timeout of 3 s (ideal)             */
#define T10s    13   /* Timeout of 10 s (ideal)            */
#define T30s    14   /* Timeout of 30 s (ideal)            */
#define T100s   15   /* Timeout of 100 s (ideal)           */
#define T300s   16   /* Timeout of 300 s (ideal)           */
#define T1000s  17   /* Timeout of 1000 s (ideal)          */


/*  Constants used for the ibln function                      */
#define NO_SAD   0
#define ALL_SAD -1

/*  Constants used for the ibevent function                   */
#define  EventDTAS   1
#define  EventDCAS   2
#define  EventIFC    3


/*
 *    GotoMultAddr (VXI) flags
 */
#define  MultAddrPrimary      0x00
#define  MultAddrSecondary    0x01

#define  MultAddrListen       0x00
#define  MultAddrTalk         0x01
#define  MultAddrSerialPoll   0x81



/*  The following constants are used for the second parameter of the
 *  ibconfig function.  They are the "option" selection codes.
 */
#define  IbcPAD        0x0001      /* Primary Address                      */
#define  IbcSAD        0x0002      /* Secondary Address                    */
#define  IbcTMO        0x0003      /* Timeout Value                        */
#define  IbcEOT        0x0004      /* Send EOI with last data byte?        */
#define  IbcPPC        0x0005      /* Parallel Poll Configure              */
#define  IbcREADDR     0x0006      /* Repeat Addressing                    */
#define  IbcAUTOPOLL   0x0007      /* Enable/Disable Auto Serial Polling   */
#define  IbcCICPROT    0x0008      /* Use the CIC Protocol?                */
#define  IbcIRQ        0x0009      /* Use PIO for I/O                      */
#define  IbcSC         0x000A      /* Board is System Controller?          */
#define  IbcSRE        0x000B      /* Assert SRE on device calls?          */
#define  IbcEOSrd      0x000C      /* Terminate reads on EOS               */
#define  IbcEOSwrt     0x000D      /* Send EOI with EOS character          */
#define  IbcEOScmp     0x000E      /* Use 7 or 8-bit EOS compare           */
#define  IbcEOSchar    0x000F      /* The EOS character.                   */
#define  IbcPP2        0x0010      /* Use Parallel Poll Mode 2.            */
#define  IbcTIMING     0x0011      /* NORMAL, HIGH, or VERY_HIGH timing.   */
#define  IbcDMA        0x0012      /* Use DMA for I/O                      */
#define  IbcReadAdjust 0x0013      /* Swap bytes during an ibrd.           */
#define  IbcWriteAdjust 0x014      /* Swap bytes during an ibwrt.          */
#define  IbcEventQueue 0x0015      /* Enable/disable the event queue (MS Windows only). */
#define  IbcSPollBit   0x0016      /* Enable/disable the visibility of SPOLL. */
#define  IbcSendLLO    0x0017      /* Enable/disable the sending of LLO.      */
#define  IbcSPollTime  0x0018      /* Set the timeout value for serial polls. */
#define  IbcPPollTime  0x0019      /* Set the parallel poll length period.    */
#define  IbcEndBitIsNormal 0x001A  /* Remove EOS from END bit of IBSTA.       */
#define  IbcUnAddr         0x001B  /* Enable/disable device unaddressing.     */
#define  IbcSignalNumber   0x001C  /* Set signal number (UNIX only)           */
#define  IbcBlockIfLocked  0x001D  /* Enable/disable blocking for locked boards/devices */
                                   /* (only on GPIB-ENET under Windows 95/NT)           */
#define  IbcHSCableLength  0x001F  /* Length of cable specified for high speed timing.*/
#define  IbcIst        0x0020      /* Set the IST bit.                     */
#define  IbcRsv        0x0021      /* Set the RSV byte.                    */
#define  IbcLON        0x0022      /* Enter listen only mode.              */

#define  IbcNoEndBitOnEOS  IbcEndBitIsNormal   /* for backwards compatibility */

/*
 *    Constants that can be used (in addition to the ibconfig constants)
 *    when calling the ibask() function.
 */
#define  IbaBNA            0x0200   /* A device's access board.                  */
#define  IbaPAD            IbcPAD
#define  IbaSAD            IbcSAD
#define  IbaTMO            IbcTMO
#define  IbaEOT            IbcEOT
#define  IbaPPC            IbcPPC
#define  IbaREADDR         IbcREADDR
#define  IbaAUTOPOLL       IbcAUTOPOLL
#define  IbaCICPROT        IbcCICPROT
#define  IbaIRQ            IbcIRQ
#define  IbaSC             IbcSC
#define  IbaSRE            IbcSRE
#define  IbaEOSrd          IbcEOSrd
#define  IbaEOSwrt         IbcEOSwrt
#define  IbaEOScmp         IbcEOScmp
#define  IbaEOSchar        IbcEOSchar
#define  IbaPP2            IbcPP2
#define  IbaTIMING         IbcTIMING
#define  IbaDMA            IbcDMA
#define  IbaReadAdjust     IbcReadAdjust
#define  IbaWriteAdjust    IbcWriteAdjust
#define  IbaEventQueue     IbcEventQueue
#define  IbaSPollBit       IbcSPollBit
#define  IbaSendLLO        IbcSendLLO
#define  IbaSPollTime      IbcSPollTime
#define  IbaPPollTime      IbcPPollTime
#define  IbaEndBitIsNormal IbcEndBitIsNormal
#define  IbaUnAddr         IbcUnAddr
#define  IbaSignalNumber   IbcSignalNumber
#define  IbaHSCableLength  IbcHSCableLength
#define  IbaBlockIfLocked  IbcBlockIfLocked
#define  IbaIst            IbcIst
#define  IbaRsv            IbcRsv
#define  IbaLON            IbcLON

#define  IbaNoEndBitOnEOS  IbcNoEndBitOnEOS   /* for backwards compatibility */



/*  488.2 CONSTANTS  */

/*
 *  These are the values are used for the eotMode in the 488.2 Send command.
 */
#define  NULLend    0x00  /* Do nothing at the end of a transfer.*/
#define  NLend      0x01  /* Send NL with EOI after a transfer.  */
#define  DABend     0x02  /* Send EOI with the last DAB.         */

/*
 *  This value is for the eotMode in the 488.2 Receive command.
 */
#define  STOPend    0x0100

/*
 *  This macro can be used to easily create an entry in address list
 *  that is required by many of the 488.2 functions.  An address list is
 *  just an array of shorts.  The primary address goes in the
 *  lower 8-bits and the secondary address goes in the upper 8-bits.
 */
#define  MakeAddr(pad, sad)   ((Addr4882_t) ((pad) | ((sad)<<8)))


/*
 *  The following two macros are used to "break apart" an address list
 *  entry.  They take an integer and return either the primary
 *  or secondary address stored in the integer.
 */
#define  GetPAD(val)    ((val) & 0xFF)
#define  GetSAD(val)    (((val) >> 8) & 0xFF)

/*
 *  This value is used to terminate an address list.  It should be
 *  assigned to the last entry.
 */
#define  NOADDR    (-1)


/*
 *  The following values are used by the iblines function.
 *
 *  The "result" argument is set by iblines as follows:
 *
 *    -  The lower byte contains a "monitor" bit mask.  If a bit is
 *       set (1) in this mask, then the corresponding line can be
 *       monitiored by the driver.  If the bit is clear (0),
 *       then the line cannot be monitored.
 *
 *    -  The upper byte contains the status of the bus lines.
 *       Each bit corresponds to a certain bus line, and has
 *       a correcponding "monitor" bit in the lower byte.
 *
 */
#define  ValidEOI   ((short)0x0080)
#define  ValidATN   ((short)0x0040)
#define  ValidSRQ   ((short)0x0020)
#define  ValidREN   ((short)0x0010)
#define  ValidIFC   ((short)0x0008)
#define  ValidNRFD  ((short)0x0004)
#define  ValidNDAC  ((short)0x0002)
#define  ValidDAV   ((short)0x0001)
#define  BusEOI     ((short)0x8000)
#define  BusATN     ((short)0x4000)
#define  BusSRQ     ((short)0x2000)
#define  BusREN     ((short)0x1000)
#define  BusIFC     ((short)0x0800)
#define  BusNRFD    ((short)0x0400)
#define  BusNDAC    ((short)0x0200)
#define  BusDAV     ((short)0x0100)


#ifdef __cplusplus
}
#endif

#endif /* _GPIB_HEADER_ */
