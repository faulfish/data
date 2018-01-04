#ifndef DIAGCMD_H
#define DIAGCMD_H
/*==========================================================================

      Diagnostic Services Packet Processing Command Code Defintions

Description
  This file contains packet id definitions for the serial interface to
  the dmss.  

Copyright (c) 1993-2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                            Edit History

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/diagcmd.h.-arc   1.2   Apr 01 2009 14:19:24   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/10/02   lcl/jwh FEATURE_HWTC changes.
05/23/02   sfh     Added DIAG_PROTOCOL_LOOPBACK_F  (123) command.
06/27/01   lad     Assigned equipment ID 0 to be for OEMs to use.
05/21/01   sfh     Added DIAG_TRACE_EVENT_REPORT_F for trace event support.
04/17/01   lad     Moved subsystem dispatch IDs from diagpkt.h.
04/06/01   lad     Changed the name of cmd code 111 from DUAG_TUNNEL_F to 
                   DIAG_ROUTE_F.
02/23/01   lad     Cosmetic changes.
09/06/00   bgc     Added support for FEATURE_FACTORY_TESTMODE with
                   DIAG_FTM_CMD_F (set to 59, which is also DIAG_TMOB_F).
08/31/00   lad     Added command code for tunneling capability.
06/23/00   lad     Removed obsolete command codes and marked them "reserved".
06/19/00   lad     Added DIAG_PARM_RETRIEVE_F
05/31/00   jal     Added GPS statistics, session control, and grid support.
05/15/00   lad     Added streaming config support (nice).
02/28/00   lad     Added codes for event reporting service.
02/02/00   lad     Added commands used with FEATURE_DIAG_QCT_EXT.
09/17/99   lcc     Merged in RPC support from PLT archive.
08/17/99   tac     Merged in EFS changes from branch.
07/19/99    sk     Replacing reset_sup_fer with walsh code.
07/19/99    sk     Added walsh code display command.
03/30/99   lad     Added support for FEATURE_IS95B_MDR and FEATURE_SPECIAL_MDR.
11/04/98   lad     Added 1998/1999 copyright information.
10/29/98   jmk     Merged Module command changes into the mainline.
                   (Replaced MOD_GET_STATUS with MOD_EXTENDED_PKT cmd code 75)
10/27/98   jmk     Added cmd IDs for CSS command, and SMS message read/write.
09/11/98   grl     Added feature query command
10/06/97   jjn     Added new commands for the Module Phase 1 interface.  These
                   include Module Status Mask, AKEY and audio control packets.
04/23/97   jjn     Added new packet pair to allow System Unit to access 
                   service option and caller ID information
03/25/97   jjn     Added new command (and packets) that allow writing to NV 
                   without going offline (for the Module only)
02/28/97   jjn     Enabled RSSI packets for the Module, added a packet for
                   module status and sound reporting, and added a pcket for
                   retrieving SMS messages
06/25/96   jmk     Added cmd id for preferred roaming list read.
06/24/96   jmk     Added cmd id for preferred roaming list write.
04/09/96   jmk     Added cmd ids for sending security code, and return code 
                   if phone is not unlocked for operations that require it.
03/06/96   jmk     Added command id for serial mode change (to AT cmd mode)
                   and command id for get rssi (for antenna aiming/WLL only)
08/16/95   jmk     Added command id for parm_get2 (includes MUX2 parms)
08/10/95   jmk     Added command id for Phone State, Pilot Sets and SPC reqs
01/28/95   ptw     Added command id to obtain System Time from the mobile.
12/07/94   jmk     Added command id for portable sleep on/off request.
11/07/94   jmk     Added command to request that seq_nums be used in pkts.
09/26/94   jmk     Put DIAG_ORIG_F and DIAG_END_F back in.
07/23/93   twp     Added DIAG_TMOB_F
01/14/93   twp     First release

===========================================================================*/

/*--------------------------------------------------------------------------

  Command Codes between the Diagnostic Monitor and the mobile. Packets
  travelling in each direction are defined here, while the packet templates
  for requests and responses are distinct.  Note that the same packet id
  value can be used for both a request and a response.  These values
  are used to index a dispatch table in diag.c, so 

  DON'T CHANGE THE NUMBERS ( REPLACE UNUSED IDS WITH FILLERS ). NEW IDs
  MUST BE ASSIGNED AT THE END.
  
----------------------------------------------------------------------------*/

/* Version Number Request/Response            */
#define DIAG_VERNO_F    0

/* Mobile Station ESN Request/Response        */
#define DIAG_ESN_F      1

/* Peek byte Request/Response                 */
#define DIAG_PEEKB_F    2
  
/* Peek word Request/Response                 */
#define DIAG_PEEKW_F    3

/* Peek dword Request/Response                */
#define DIAG_PEEKD_F    4  

/* Poke byte Request/Response                 */
#define DIAG_POKEB_F    5  

/* Poke word Request/Response                 */
#define DIAG_POKEW_F    6  

/* Poke dword Request/Response                */
#define DIAG_POKED_F    7  

/* Byte output Request/Response               */
#define DIAG_OUTP_F     8

/* Word output Request/Response               */
#define DIAG_OUTPW_F    9  

/* Byte input Request/Response                */
#define DIAG_INP_F      10 

/* Word input Request/Response                */
#define DIAG_INPW_F     11 

/* DMSS status Request/Response               */
#define DIAG_STATUS_F   12 

/* 13-14 Reserved */

/* Set logging mask Request/Response          */
#define DIAG_LOGMASK_F  15 

/* Log packet Request/Response                */
#define DIAG_LOG_F      16 

/* Peek at NV memory Request/Response         */
#define DIAG_NV_PEEK_F  17 

/* Poke at NV memory Request/Response         */
#define DIAG_NV_POKE_F  18 

/* Invalid Command Response                   */
#define DIAG_BAD_CMD_F  19 

/* Invalid parmaeter Response                 */
#define DIAG_BAD_PARM_F 20 

/* Invalid packet length Response             */
#define DIAG_BAD_LEN_F  21 

/* 22-23 Reserved */

/* Packet not allowed in this mode 
   ( online vs offline )                      */
#define DIAG_BAD_MODE_F     24
                            
/* info for TA power and voice graphs         */
#define DIAG_TAGRAPH_F      25 

/* Markov statistics                          */
#define DIAG_MARKOV_F       26 

/* Reset of Markov statistics                 */
#define DIAG_MARKOV_RESET_F 27 

/* Return diag version for comparison to
   detect incompatabilities                   */
#define DIAG_DIAG_VER_F     28 
                            
/* Return a timestamp                         */
#define DIAG_TS_F           29 

/* Set TA parameters                          */
#define DIAG_TA_PARM_F      30 

/* Request for msg report                     */
#define DIAG_MSG_F          31 

/* Handset Emulation -- keypress              */
#define DIAG_HS_KEY_F       32 

/* Handset Emulation -- lock or unlock        */
#define DIAG_HS_LOCK_F      33 

/* Handset Emulation -- display request       */
#define DIAG_HS_SCREEN_F    34 

/* 35 Reserved */

/* Parameter Download                         */
#define DIAG_PARM_SET_F     36 

/* 37 Reserved */

/* Read NV item                               */
#define DIAG_NV_READ_F  38 
/* Write NV item                              */
#define DIAG_NV_WRITE_F 39 
/* 40 Reserved */

/* Mode change request                        */
#define DIAG_CONTROL_F    41 

/* Error record retreival                     */
#define DIAG_ERR_READ_F   42 

/* Error record clear                         */
#define DIAG_ERR_CLEAR_F  43 

/* Symbol error rate counter reset            */
#define DIAG_SER_RESET_F  44 

/* Symbol error rate counter report           */
#define DIAG_SER_REPORT_F 45 

/* Run a specified test                       */
#define DIAG_TEST_F       46 

/* Retreive the current dip switch setting    */
#define DIAG_GET_DIPSW_F  47 

/* Write new dip switch setting               */
#define DIAG_SET_DIPSW_F  48 

/* Start/Stop Vocoder PCM loopback            */
#define DIAG_VOC_PCM_LB_F 49 

/* Start/Stop Vocoder PKT loopback            */
#define DIAG_VOC_PKT_LB_F 50 

/* 51-52 Reserved */

/* Originate a call                           */
#define DIAG_ORIG_F 53 
/* End a call                                 */
#define DIAG_END_F  54 
/* 55-57 Reserved */

/* Switch to downloader                       */
#define DIAG_DLOAD_F 58 
/* Test Mode Commands and FTM commands        */
#define DIAG_TMOB_F  59 
/* Test Mode Commands and FTM commands        */
#define DIAG_FTM_CMD_F  59 
/* 60-62 Reserved */

#ifdef FEATURE_HWTC
#error code not present
#endif /* FEATURE_HWTC */

/* Return the current state of the phone      */
#define DIAG_STATE_F        63 

/* Return all current sets of pilots          */
#define DIAG_PILOT_SETS_F   64 

/* Send the Service Prog. Code to allow SP    */
#define DIAG_SPC_F          65 

/* Invalid nv_read/write because SP is locked */
#define DIAG_BAD_SPC_MODE_F 66 

/* get parms obsoletes PARM_GET               */
#define DIAG_PARM_GET2_F    67 

/* Serial mode change Request/Response        */
#define DIAG_SERIAL_CHG_F   68 

/* 69 Reserved */

/* Send password to unlock secure operations  
   the phone to be in a security state that
   is wasn't - like unlocked.                 */
#define DIAG_PASSWORD_F     70 

/* An operation was attempted which required  */
#define DIAG_BAD_SEC_MODE_F 71 

/* Write Preferred Roaming list to the phone. */
#define DIAG_PR_LIST_WR_F   72 

/* Read Preferred Roaming list from the phone.*/
#define DIAG_PR_LIST_RD_F   73 

/* 74 Reserved */

/* Subssytem dispatcher (extended diag cmd)   */
#define DIAG_SUBSYS_CMD_F   75 

/* 76-80 Reserved */

/* Asks the phone what it supports            */
#define DIAG_FEATURE_QUERY_F   81 

/* 82 Reserved */

/* Read SMS message out of NV                 */
#define DIAG_SMS_READ_F        83 

/* Write SMS message into NV                  */
#define DIAG_SMS_WRITE_F       84 

/* info for Frame Error Rate          
   on multiple channels                       */
#define DIAG_SUP_FER_F         85 

/* Supplemental channel walsh codes           */
#define DIAG_SUP_WALSH_CODES_F 86 

/* Sets the maximum # supplemental 
   channels                                   */
#define DIAG_SET_MAX_SUP_CH_F  87 

/* get parms including SUPP and MUX2: 
   obsoletes PARM_GET and PARM_GET_2          */
#define DIAG_PARM_GET_IS95B_F  88 

/* Performs an Embedded File System
   (EFS) operation.                           */
#define DIAG_FS_OP_F           89 

/* AKEY Verification.                         */
#define DIAG_AKEY_VERIFY_F     90 

/* Handset emulation - Bitmap screen          */
#define DIAG_BMP_HS_SCREEN_F   91 

/* Configure communications                   */
#define DIAG_CONFIG_COMM_F        92 

/* Extended logmask for > 32 bits.            */
#define DIAG_EXT_LOGMASK_F        93 

/* 94-95 reserved */

/* Static Event reporting.                    */
#define DIAG_EVENT_REPORT_F       96 

/* Load balancing and more!                   */
#define DIAG_STREAMING_CONFIG_F   97 

/* Parameter retrieval                        */
#define DIAG_PARM_RETRIEVE_F      98 

 /* A state/status snapshot of the DMSS.      */
#define DIAG_STATUS_SNAPSHOT_F    99
 
/* Used for RPC                               */
#define DIAG_RPC_F               100 

/* Get_property requests                      */
#define DIAG_GET_PROPERTY_F      101 

/* Put_property requests                      */
#define DIAG_PUT_PROPERTY_F      102 

/* Get_guid requests                          */
#define DIAG_GET_GUID_F          103 

/* Invocation of user callbacks               */
#define DIAG_USER_CMD_F          104 

/* Get permanent properties                   */
#define DIAG_GET_PERM_PROPERTY_F 105 

/* Put permanent properties                   */
#define DIAG_PUT_PERM_PROPERTY_F 106 

/* Permanent user callbacks                   */
#define DIAG_PERM_USER_CMD_F     107 

/* GPS Session Control                        */
#define DIAG_GPS_SESS_CTRL_F     108 

/* GPS search grid                            */
#define DIAG_GPS_GRID_F          109 

/* GPS Statistics                             */
#define DIAG_GPS_STATISTICS_F    110 

/* Packet routing for multiple instances of diag */
#define DIAG_ROUTE_F             111 

/* IS2000 status                              */
#define DIAG_IS2000_STATUS_F     112

/* RLP statistics reset                       */
#define DIAG_RLP_STAT_RESET_F    113

/* (S)TDSO statistics reset                   */
#define DIAG_TDSO_STAT_RESET_F   114

/* Logging configuration packet               */
#define DIAG_LOG_CONFIG_F        115

/* Static Trace Event reporting */
#define DIAG_TRACE_EVENT_REPORT_F 116

/* SBI Read */
#define DIAG_SBI_READ_F           117

/* SBI Write */
#define DIAG_SBI_WRITE_F          118

/* SSD Verify */
#define DIAG_SSD_VERIFY_F         119

/* Log on Request */
#define DIAG_LOG_ON_DEMAND_F      120

/* Request for extended msg report */
#define DIAG_EXT_MSG_F            121 

/* ONCRPC diag packet */
#define DIAG_ONCRPC_F             122

/* Diagnostics protocol loopback. */
#define DIAG_PROTOCOL_LOOPBACK_F  123

/* Extended build ID text */
#define DIAG_EXT_BUILD_ID_F       124

/* Request for extended msg report */
#define DIAG_EXT_MSG_CONFIG_F     125

/* Extended messages in terse format */
#define DIAG_EXT_MSG_TERSE_F      126

/* Translate terse format message identifier */
#define DIAG_EXT_MSG_TERSE_XLATE_F 127

/* Subssytem dispatcher Version 2 (delayed response capable) */
#define DIAG_SUBSYS_CMD_VER_2_F    128

/* Number of packets defined. */
#define DIAG_MAX_F                 128

/* RESERVED CODES: 128-139 */

/*BenQ extened define command*/
#define DIAG_EXT_MIN_F            140

/* PhoneBook read command */
#define DIAG_PB_READ_F            141   

/* PhoneBook write command */
#define DIAG_PB_WRITE_F           142

/* Call Group read command */
#define DIAG_CG_READ_F            143

/* Call Group write command */
#define DIAG_CG_WRITE_F           144

/* Software feature */
#define DIAG_SW_FEATURE_F         145

/* Baseband test command */
#define DIAG_BASEBAND_TEST_F      146

/* Read WAP user data */
#define DIAG_WAP_READ_F           147

/* Write WAP user data */
#define DIAG_WAP_WRITE_F          148

/* AKey write */
#define DIAG_AKEY_WRITE_F         149

/* Read RUIM PhoneBook */
#define DIAG_RUIM_PB_READ_F       150

/* Read SMS message from NV */
#define DIAG_BENQ_SMS_READ_F      151

/* Write SMS message into NV */
#define DIAG_BENQ_SMS_WRITE_F     152

/* Delete SMS message into NV */
#define DIAG_BENQ_SMS_DELETE_F    153

//#ifdef FEATURE_THREE_LEVEL_SPC
/* Read the Service Prog. Code 1 */
#define DIAG_NV_SPC_READ_F        154

/* Write the Service Prog. Code 1 */
#define DIAG_NV_SPC_WRITE_F       155
//#endif /* FEATURE_THREE_LEVEL_SPC */

/* Send the Super Service Prog. Code to allow SP    */
#define DIAG_SUPER_SPC_F          156

/* Get System Parameters such as A_KEY, SSD_A, SSD_B, and SPC */
#define DIAG_SYS_PARAM_F          157 

/* NV Image Backup, Dump and Verify */
#define DIAG_BENQ_NV_IMAGE_F      158

#define DIAG_BENQ_FACTORY_WRITE_F 159

#define DIAG_BENQ_FACTORY_READ_F  160

/* Power test command */
//#ifdef FEATURE_BENQ_POWER_TEST
#define DIAG_BENQ_POWER_TEST_F    161
//#endif

/* MMI to UNICODE or UNICODE to MMI */
#define DIAG_BENQ_CONV_FONTCODE_F 162

/* Scheduler read command */
#define DIAG_BENQ_SCH_READ_F      163
/* Scheduler write command */
#define DIAG_BENQ_SCH_WRITE_F     164

/* PIM sync lock command */
#define DIAG_BENQ_PIM_LOCK_F      165

/* PIM portraittable read command */
#define DIAG_BENQ_PT_READ_F       166

/* PIM portraittable write command */
#define DIAG_BENQ_PT_WRITE_F      167


#define DIAG_QUERY_PROJECT_INFO_F 168

/* NV_SPEED_DIAL_I read command    */
#define DIAG_BENQ_SPEED_DIAL_READ_F       169

/* NV_SPEED_DIAL_I write command   */
#define DIAG_BENQ_SPEED_DIAL_WRITE_F      170

#define DIAG_EXT_MAX_F            171
/*End of BenQ extened define command*/ 



typedef enum {
  DIAG_SUBSYS_OEM    = 0,
  DIAG_SUBSYS_ZREX   = 1,
  DIAG_SUBSYS_SD     = 2,
  DIAG_SUBSYS_BT     = 3,
  DIAG_SUBSYS_WCDMA  = 4,
  DIAG_SUBSYS_HDR    = 5,
  DIAG_SUBSYS_DIABLO = 6,
  DIAG_SUBSYS_TREX   = 7,
  DIAG_SUBSYS_GSM    = 8,
  DIAG_SUBSYS_UMTS   = 9,
  DIAG_SUBSYS_HWTC   = 10,
  DIAG_SUBSYS_FTM    = 11,
  DIAG_SUBSYS_REX    = 12,
  DIAG_SUBSYS_GPS    = 13,
  DIAG_SUBSYS_WMS    = 14,
  DIAG_SUBSYS_CM     = 15,
  DIAG_SUBSYS_HS     = 16,
  DIAG_SUBSYS_AUDIO_SETTINGS   = 17,
  DIAG_SUBSYS_DIAG_SERV = 18,
  DIAG_SUBSYS_EFS        = 19,
  DIAG_SUBSYS_PORT_MAP_SETTINGS  = 20,
  DIAG_SUBSYS_MEDIAPLAYER        = 21,
  DIAG_SUBSYS_QCAMERA            = 22,
  DIAG_SUBSYS_MOBIMON            = 23,
  DIAG_SUBSYS_GUNIMON            = 24,
  DIAG_SUBSYS_LSM                = 25,
  DIAG_SUBSYS_QCAMCORDER         = 26,
  DIAG_SUBSYS_MUX1X              = 27,
  DIAG_SUBSYS_DATA1X             = 28,
  DIAG_SUBSYS_SRCH1X             = 29,
  DIAG_SUBSYS_CALLP1X            = 30,
  DIAG_SUBSYS_APPS               = 31,
  DIAG_SUBSYS_SETTINGS           = 32,
  DIAG_SUBSYS_GSDI               = 33,
  DIAG_SUBSYS_TMC                = 34,
  DIAG_SUBSYS_USB                = 35,
  DIAG_SUBSYS_PM                 = 36,      /* Power Management */
  DIAG_SUBSYS_DEBUG              = 37,
  DIAG_SUBSYS_QTV                = 38,
  DIAG_SUBSYS_CLKRGM             = 39,      /* Clock Regime */
  DIAG_SUBSYS_DEVICES            = 40,
  DIAG_SUBSYS_WLAN               = 41,      /* 802.11 Technology */
  DIAG_SUBSYS_PS_DATA_LOGGING    = 42,      /* Data Path Logging */
  DIAG_SUBSYS_MFLO               = 43,      /* MediaFLO */
  DIAG_SUBSYS_DTV                = 44,      /* Digital TV */
  DIAG_SUBSYS_RRC                = 45,      /* WCDMA Radio Resource Control state */
  DIAG_SUBSYS_PROF               = 46,      /* Miscellaneous Profiling Related */
  DIAG_SUBSYS_TCXOMGR            = 47,
  DIAG_SUBSYS_NV                 = 48,      /* Non Volatile Memory */
  DIAG_SUBSYS_AUTOCONFIG         = 49,
  DIAG_SUBSYS_PARAMS             = 50,      /* Parameters required for debugging subsystems */
  DIAG_SUBSYS_MDDI               = 51,      /* Mobile Display Digital Interface */
  DIAG_SUBSYS_DS_ATCOP           = 52,
  DIAG_SUBSYS_L4LINUX            = 53,      /* L4/Linux */
  DIAG_SUBSYS_MVS                = 54,      /* Multimode Voice Services */
  DIAG_SUBSYS_CNV                = 55,      /* Compact NV */
  DIAG_SUBSYS_APIONE_PROGRAM     = 56,      /* apiOne */
  DIAG_SUBSYS_HIT                = 57,      /* Hardware Integration Test */
  DIAG_SUBSYS_DRM                = 58,      /* Digital Rights Management */
  DIAG_SUBSYS_DM                 = 59,      /* Device Management */
  DIAG_SUBSYS_FC                 = 60,      /* Flow Controller */
  DIAG_SUBSYS_MEMORY             = 61,      /* Malloc Manager */
  DIAG_SUBSYS_FS_ALTERNATE       = 62,      /* Alternate File System */
  DIAG_SUBSYS_REGRESSION         = 63,      /* Regression Test Commands */
  DIAG_SUBSYS_SENSORS            = 64,      /* The sensors subsystem */
  DIAG_SUBSYS_FLUTE              = 65,      /* FLUTE */

  DIAG_SUBSYS_LAST
} diagpkt_subsys_cmd_enum_type;

#endif  /* DIAGCMD_H */



