#ifndef CMDIAG_H
#define CMDIAG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/*==========================================================================

                    Call Manager Diagnostic Packets

General Description
  This file contains packet definitions for CM diagnostic packets.

Copyright (c) 2002 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/cmdiag.h.-arc   1.1   Apr 01 2009 14:19:22   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/24/03   vt      removed FEATURE_SPECIAL_MDR
02/28/03   ws      Added #def of CMDIAG_H 
11/20/01   as      Created file.

===========================================================================*/
#include "comdef.h"
#include "customer.h"

/* This must be called during initializationfor the packet support to work. */
void cmdiag_init (void);

/* Mode change commands */
typedef enum
{
  CMDIAG_MODE_OFFLINE_A_F = 0,	/* Go to offline analog */
  CMDIAG_MODE_OFFLINE_D_F,	    /* Go to offline digital */
  CMDIAG_MODE_RESET_F,		    /* Reset. Only exit from offline */
  CMDIAG_MODE_FTM_F,            /* FTM mode - if supported */
  CMDIAG_MODE_ONLINE_F,         /* Online mode - if supported */
  CMDIAG_MODE_LPM_F,            /* Low Power Mode - if supported */
  CMDIAG_MODE_MAX_F		/* Last (and invalid) mode enum value */
}
cmdiag_mode_enum_type;


/*==========================================================================

PACKET   DIAG_CONTROL_F

PURPOSE  Request sent from the DM to the DMSS to change the operating mode

============================================================================*/
typedef PACKED struct
{
  byte xx_cmd_code;
  word mode;			/* Mode to change to */
}
cmdiag_mode_control_req_type;

typedef cmdiag_mode_control_req_type cmdiag_mode_control_rsp_type;


/* Service options for call origination. */

#define CMDIAG_SO_NONE            0x00
#define CMDIAG_SO_LOOPBACK        0x02
#define CMDIAG_SO_MARKOV          0x03
#define CMDIAG_SO_DATA            0x04
#define CMDIAG_SO_VOICE_96A       0x05
#define CMDIAG_SO_VOICE13         0x06	/* 13K (MSM2 only) Voice call */
#define CMDIAG_SO_RS2_MARKOV      0x07	/* 13K (MSM2 only) Markov call (new) */
#define CMDIAG_SO_RS1_MARKOV      0x08
#define CMDIAG_SO_LOOPBACK_13K    0x09
#define CMDIAG_SO_MARKOV_13K      0x0A	/* 13K (MSM2 only) Markov call (old) */
#define CMDIAG_SO_EVRC            0x0B	/* EVRC voice option */
/* CAI SMS SO (6) value is overridden with a proprietary value from this 
   list, so use unlikely SO value. */
#define CMDIAG_SO_SMS             0xFFFE
/* CAI_SO_RS2_SMS (14) is not in this proprietary list, so use the CAI SO. */
#define CMDIAG_SO_RS2_SMS         0x0E
#define CMDIAG_SO_VOICE_13K_IS733 0x11	/* IS-733 s.o. number */
#define CMDIAG_SO_MARKOV_SO54     0x36
#define CMDIAG_SO_LOOPBACK_SO55   0x37

#if defined(FEATURE_IS95B_MDR)
#error code not present
#endif /* defined(FEATURE_IS95B_MDR) */

/*===========================================================================

PACKET   DIAG_ORIG_F

PURPOSE  Sent by DM to originate a call.

RESPONSE DMSS originates a call using the specified number.

===========================================================================*/
#define CMDIAG_NV_MAX_DIAL_DIGITS 32

typedef PACKED struct
{
  byte cmd_code;		/* Command code */
  byte cnt;			/* number of digits in dialed_digits array */
  char dialed_digits[CMDIAG_NV_MAX_DIAL_DIGITS];
  word so;
  /* desired service option - 0 = Reserved,  1 = Voice (IS96A), 2 = Loopback, 
     8001 = Voice (IS96), 8002 = Markov, 8003 = Data (see cai.h) */
}
cmdiag_orig_req_type;

typedef PACKED struct
{
  byte cmd_code;
}
cmdiag_orig_rsp_type;


/*===========================================================================

PACKET   DIAG_END_F

PURPOSE  Sent by DM to end a call.

RESPONSE DMSS ends the current call.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;
}
cmdiag_end_req_type;

typedef PACKED struct
{
  byte cmd_code;
}
cmdiag_end_rsp_type;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CMDIAG_H */

