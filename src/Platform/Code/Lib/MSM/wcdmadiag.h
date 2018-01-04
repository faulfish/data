#ifndef DIAG_WCDMA_CMD_H
#define DIAG_WCDMA_CMD_H
/*==========================================================================

      Diagnostic Services Packet Processing Command Code Defintions

Description
  This file contains packet id definitions for the serial interface to
  the dmss.  

Copyright (c) 1993-2002 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                            Edit History
                            
$PVCSPath:  L:/src/asw/MSM5200/common/vcs/diagpkt_wcdma_cmd.h_v   1.4   22 Feb 2002 16:37:26   yiz  $                            
$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/wcdmadiag.h.-arc   1.1   Apr 01 2009 14:19:28   Kevin Tai  $ $DateTime: 2003/07/28 10:50:42 $ $Author:   Kevin Tai  $

when       who     what, where, why
--------   ---    -----------------------------------------------------------
07/28/03   scm    Support for query of various DRX reacquisition statistics.
04/22/03   gs     Added support for Command Code 08 (BLER meas rate change).
09/17/02   gsc    Added support for Command Code 15 (wcdma_additional_status).
02/22/02   abp    Update WCDMA verno packet per Serial ICD revC.
11/02/01   yiz    Added declarations to implement Call Originate and Call End.
09/25/01   abp    Modified rsp packet for WCDMA_DIAG_VERNO_F
09/14/01   abp    Defined req and rsp packets for WCDMA_STATUS_F
04/30/01   cpe    Created the file
===========================================================================*/

#include "diagpkt.h"
#include "customer.h"
#include "srchzz.h"

/* Spec limits downlink tr block to 800 bytes so 2*800 + 20 should suffice */
#define MAX_DATA_LEN  200 /*For now */
/*#define MAX_DATA_LEN  1620 */

/* Max num of diag commands for wcdma subsystem. Can be modfied later */
#define DIAG_WCDMA_MAX_F 16

/* Max num of digits in dialed number for WCDMA call origination packet */
#define DIAG_MAX_WCDMA_DIGITS 32

#define WCDMA_DIAG_VERNO_F             0
#define WCDMA_AL1_DATA_F               1
#define WCDMA_DIAG_BLER_RATE_CHANGE_F  8
#define WCDMA_DIAG_ORIG_F              12
#define WCDMA_DIAG_END_F               13
#define WCDMA_STATUS_F                 14
#define WCDMA_ADDITIONAL_STATUS_F      15
#define WCDMA_DIAG_REACQ_SLEW_STATS_F  17
#define WCDMA_DIAG_REACQ_SRCH_STATS_F  18
#define WCDMA_DIAG_REACQ_CLEAR_STATS_F 19

/************************************************************************** 
    PACKET DEFINITIONS
**************************************************************************/

/* WCDMA version  Diag Request Packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_DIAG_VERNO_F)
DIAGPKT_REQ_END

 /* The response packet is defined below.
 */
DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_DIAG_VERNO_F)
 /* command code */
//-LD  uint16  cmd_code;
  /* WCDMA class mark */
  //uint8 class_mark;
  /* Rev number of WCDMA standard  4 MSBs are used to set
     to represent year of release 0 - 1999  1 - 2000 etc
     4 LSBs will be set to the month */
  //uint8 g3gpp_rev;
  /* Release num, from 3 up */
  //uint8 g3gpp_rel;
  /* Vocoder DSP version num, 0 ...0xffff */
  uint16  vdsp_ver;
  /* MDSP version number(first part) 16 bits */
  uint16  mdsp_ver_rom;
  /* MDSP version number(second part) 16 bits */
  uint16  mdsp_ver_ram;
  /* Indicates if DRX is used for cell updating */
  //uint8 drx_index;
  /* MSM version ( first part) */
  //uint8 msm_ver_maj;
  /* MSM version ( second part) */
  //uint8 msm_ver_min;
  //uint8 model_num;
  /* WCDMA model number */
DIAGPKT_RSP_END

#ifdef FEATURE_L1_AUTO_BLER_MEAS

/* WCDMA Diag L1 BLER meas rate change packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_DIAG_BLER_RATE_CHANGE_F)
 /* BLER meas duration in ms */
 uint16 bler_meas_duration_ms;
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_DIAG_BLER_RATE_CHANGE_F)
DIAGPKT_RSP_END

#endif

/* WCDMA Diag L1 DRX reacquisition slew statistics packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_DIAG_REACQ_SLEW_STATS_F)
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_DIAG_REACQ_SLEW_STATS_F)
  /* Counts of the various slew values observed during DRX reacquisition. */
  srchzz_reacq_slew_stats_type srchzz_reacq_slew_stats;
DIAGPKT_RSP_END

/* WCDMA Diag L1 DRX reacquisition search statistics packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_DIAG_REACQ_SRCH_STATS_F)
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_DIAG_REACQ_SRCH_STATS_F)
  /* Counts of successful and failed reacquisitions, both list and 1/2/3. */
  srchzz_reacq_srch_stats_type srchzz_reacq_srch_stats;
DIAGPKT_RSP_END

/* WCDMA Diag L1 DRX reacquisition clear all statistics packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_DIAG_REACQ_CLEAR_STATS_F)
DIAGPKT_REQ_END

DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_DIAG_REACQ_CLEAR_STATS_F)
DIAGPKT_RSP_END

/* WCDMA Diag Call Origination Request Packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_DIAG_ORIG_F)
 /* DM shall set it to the number of valid digits in DIGITS field */
 uint8  num_digits;

 /* Dialed digits; ASCII chars; 0-9, #, * only */
 uint8  digits[DIAG_MAX_WCDMA_DIGITS];

 /* Rate of the vocoder: 0 ~ 7 for
    4.75, 5.15, 5.90, 6.70, 7.40, 7.95, 10.2, 12.2 kbps
  */
 uint8  amr_rate;
DIAGPKT_REQ_END

 /* The response packet is defined below.
 */
DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_DIAG_ORIG_F)
DIAGPKT_RSP_END

/* WCDMA Diag Call End Request Packet
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_DIAG_END_F)
DIAGPKT_REQ_END

 /* The response packet is defined below.
 */
DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_DIAG_END_F)
DIAGPKT_RSP_END

/* Packet data exchanged between QXDM and Abstract l1 (AL1) running on SURF
 */

DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_AL1_DATA_F)
 /* length of the downlink data */
  uint16 data_len ; 

 /* downlink data as a char array */
 uint8  data[MAX_DATA_LEN] ; 
DIAGPKT_REQ_END

 /* The response packet is defined below.
 */
DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_AL1_DATA_F)
 /* length of the uplink data */
 uint16 data_len ;

 /* uplink data as a char array */
 /* uint8  data[MAX_DATA_LEN] ; */
 char  data[MAX_DATA_LEN] ;

DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    WCDMA TMC STATUS
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_STATUS_F) 
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_STATUS_F)
 /* TMC state */
 uint8 tmc_state;
 
DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    WCDMA ADDITIONAL STATUS
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_ADDITIONAL_STATUS_F) 
DIAGPKT_REQ_END

 /* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_ADDITIONAL_STATUS_F)
 /* International Mobile Equipment ID */
 uint8 imei[9];
 /* International Mobile Subscriber ID */
 uint8 imsi[9];
 /*l1_state*/
 uint8 l1_state;
 
DIAGPKT_RSP_END


#endif
