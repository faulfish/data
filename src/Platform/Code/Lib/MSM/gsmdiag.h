#ifndef DIAG_GSM_CMD_H
#define DIAG_GSM_CMD_H
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
                            
$PVCSPath:  L:/src/asw/KOVU/common/vcs/diagpkt_gsm_cmd.h_v   1.5   06 May 2002 12:15:54   amcintos  $                            
$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/gsmdiag.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $                             $DateTime: 2002/10/28 12:13:00 $ $Author:   Kevin Tai  $

when       who     what, where, why
--------   ---    -----------------------------------------------------------
09/06/02   ADG    Added GPRS_LOG_PACKET_REQUEST command.
04/26/02   ATM     Replaced MS_STATE with CM_CALL_STATE, 
                                          CM_OPERATING_STATE, 
                                          CM_SYSTEM_STATE
03/26/02   ATM    Fixed packing order of MDSP VERSION (ROM, then RAM)
02/13/02   ATM    Updated command set to match GSM ICD
12/10/01   ATM    Created the file adapted from diagpkt_wcdma_cmd.h
===========================================================================*/

#include "diagpkt.h"

/* Spec limits downlink tr block to 800 bytes so 2*800 + 20 should suffice */
#define MAX_DATA_LEN  200 /*For now */
/*#define MAX_DATA_LEN  1620 */

/* Max num of digits in dialed number for GSM call origination packet */
#define DIAG_MAX_GSM_DIGITS     32

/* Max num of bytes for largest SIM item */
#define DIAG_MAX_SIM_ITEM_SIZE  40 

/* Max num of bytes for largest SIM item */
#define DIAG_MAX_PIN_SIZE       8 


/* This Table is from GSM ICD section 3.2 */
/* NOTE: Values may shift as this document is still under construction */
#define GSM_DIAG_VERNO_F           0
#define GSM_STATUS_F               1
#define GSM_TMC_STATE_RETRIEVAL_F  2
#define GSM_DIAG_CALL_ORIG_F       3
#define GSM_DIAG_CALL_END_F        4
#define GSM_GPRS_LOG_PACKET_REQ_F  5
#define DIAG_GSM_MAX_F             6 /* Must be one greater than highest cmd */


/************************************************************************** 
    FUNCTION PROTOTYPES
**************************************************************************/
#if defined(FEATURE_DIAG_GPRS)

/* Define a GPRS_LOG_PACKET_REQUEST handler */
typedef void (*gprs_log_packet_request_func)(uint16 log_code,
                                             uint8  zero_stats,
                                             uint8  additional_info);

/* Register a GPRS_LOG_PACKET_REQUEST handler for a log packet type */
extern boolean diagpkt_gsm_gprs_lpr_register(
    uint16                       log_code,    /* the log code to be handled */
    gprs_log_packet_request_func handler      /* the function to handle it  */
    );

#endif /* defined(FEATURE_DIAG_GPRS) */

/************************************************************************** 
    PACKET DEFINITIONS
**************************************************************************/
/*---------------------------------------------------------------------------
    GSM VERSION -- SUBCOMMAND 0
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(GSM, GSM_DIAG_VERNO_F)
 /* No additional fields */
DIAGPKT_REQ_END

/* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(GSM, GSM_DIAG_VERNO_F)
  /* Vocoder DSP version num, 0 ...0xffff */
  uint16  vdsp_ver;

  /* MDSP version number(first part) 16 bits */
  uint16  mdsp_ver_rom;
  /* MDSP version number(second part) 16 bits */
  uint16  mdsp_ver_ram;

DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    GSM STATUS -- SUBCOMMAND 1
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(GSM, GSM_STATUS_F) 
 /* No additional fields */
DIAGPKT_REQ_END

/* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(GSM, GSM_STATUS_F)
 /* International Mobile Equipment ID */
 uint8 imei[9];
 /* International Mobile Subscriber ID */
 uint8 imsi[9];
 /* Location Area ID */
 uint8 lai[5];
 /* Cell ID */
 uint16 cell_id;
 /* Call Manager Overall Call State */
 uint8 call_state;
 /* Call Manager Operating Mode */
 uint8 operating_mode;
 /* Call Manager System Mode */
 uint8 system_mode;
DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    TMC_STATE Retrieval -- SUBCOMMAND 2
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(GSM, GSM_TMC_STATE_RETRIEVAL_F) 
 /* No additional fields */
DIAGPKT_REQ_END

/* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(GSM, GSM_TMC_STATE_RETRIEVAL_F)
 /* TMC state */
 uint8 tmc_state;
DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    GSM CALL ORIGINATE -- SUBCOMMAND 3
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(GSM, GSM_DIAG_CALL_ORIG_F)
 /* DM shall set it to the number of valid digits in DIGITS field */
 uint8  num_digits;
 /* Dialed digits; ASCII chars; 0-9, #, * only */
 uint8  digits[DIAG_MAX_GSM_DIGITS];
 /* Rate of the vocoder: 0 ~ 7 for
    4.75, 5.15, 5.90, 6.70, 7.40, 7.95, 10.2, 12.2 kbps
  */
 uint8  amr_rate;
DIAGPKT_REQ_END

/* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(GSM, GSM_DIAG_CALL_ORIG_F)
 /* No additional fields */
DIAGPKT_RSP_END

/*---------------------------------------------------------------------------
    GSM CALL END -- SUBCOMMAND 4
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(GSM, GSM_DIAG_CALL_END_F)
 /* No additional fields */
DIAGPKT_REQ_END

/* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(GSM, GSM_DIAG_CALL_END_F)
 /* No additional fields */
DIAGPKT_RSP_END


#if defined(FEATURE_DIAG_GPRS)

/*---------------------------------------------------------------------------
    GPRS LOG PACKET REQUEST -- SUBCOMMAND 5
---------------------------------------------------------------------------*/
/* Request packet */
DIAGPKT_SUBSYS_REQ_DEFINE(GSM, GSM_GPRS_LOG_PACKET_REQ_F)
 /* The log packet code being requested */
 uint16 log_packet_code;
 /* Reset statistics for this packet: non-zero ==> yes */
 uint8  zero_stats;
 /* Additional information dependent on packet type */
 uint8  additional_info;
DIAGPKT_REQ_END

/* Response packet */
DIAGPKT_SUBSYS_RSP_DEFINE(GSM, GSM_GPRS_LOG_PACKET_REQ_F)
 /* No additional fields */
DIAGPKT_RSP_END

#endif /* defined(FEATURE_DIAG_GPRS) */


#endif
