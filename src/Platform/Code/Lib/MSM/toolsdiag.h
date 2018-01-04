#ifndef TOOLSDIAG_H
#define TOOLSDIAG_H
/*==========================================================================

                      Diagnostic Packet Definitions

  Description: Packet definitions between the diagnostic subsystem
  and the external device.

  !!! NOTE: All member structures of DIAG packets must be PACKED.
  
  !!! WARNING: Each command code number is part of the externalized
  diagnostic command interface.  This number *MUST* be assigned
  by a member of QCT's tools development team.

Copyright (c) 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                            Edit History

  $PVCSPath:  L:/src/asw/MSM5200/common/vcs/diagpkt_tools.h_v   1.1   23 Aug 2001 13:33:22   jlindqui  $
  $Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/toolsdiag.h.-arc   1.1   Apr 01 2009 14:19:28   Kevin Tai  $ $DateTime: 2003/01/06 16:25:02 $ $Author:   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/23/01   jal     Removed redundant DIAG_TS_F packet definitions
05/08/01   cpe	   renamed this file to diagpkt_tools.h from diagpkt_wcdma.h
05/07/01   lad     Cleaned up file.
02/23/01   lad     Created file from what used to be diagpkt.h

===========================================================================*/

#include "comdef.h"
#include "diagpkt.h"


/* -------------------------------------------------------------------------
** Packet definitions.
** ------------------------------------------------------------------------- */
#if defined(T_WINNT)
#error code not present
#endif
/*============================================================================

PACKET   DIAG_VERNO_F
PURPOSE  Sent from the DM to the DMSS requesting the DMSS send its
         version number.

============================================================================*/
#define VERNO_DATE_STRLEN 11
#define VERNO_TIME_STRLEN  8
#define VERNO_DIR_STRLEN   8

#ifndef DIAG_VERNO_F
#define DIAG_VERNO_F 0
#endif

/* Request packet */
DIAGPKT_REQ_DEFINE(DIAG_VERNO_F)
DIAGPKT_REQ_END

/* Response packet */
DIAGPKT_RSP_DEFINE(DIAG_VERNO_F)

  char comp_date[ VERNO_DATE_STRLEN ];  /* Compile date Jun 11 1991   */
  char comp_time[ VERNO_TIME_STRLEN ];  /* Compile time hh:mm:ss      */
  char rel_date [ VERNO_DATE_STRLEN ];  /* Release date               */
  char rel_time [ VERNO_TIME_STRLEN ];  /* Release time               */
  char ver_dir  [ VERNO_DIR_STRLEN ];   /* Version directory          */
  byte scm;                             /* Station Class Mark         */
  byte mob_cai_rev;                     /* CAI rev                    */
  byte mob_model;                       /* Mobile Model               */
  word mob_firm_rev;                    /* Firmware Rev               */
  byte slot_cycle_index;                /* Slot Cycle Index           */
  byte hw_maj_ver;                      /* Hardware Version MSB       */
  byte hw_min_ver;                      /* Hardware Version LSB       */

DIAGPKT_RSP_END

typedef DIAG_VERNO_F_req_type diag_verno_req_type;
typedef DIAG_VERNO_F_rsp_type diag_verno_rsp_type;

/*===========================================================================

PACKET   DIAG_ESN_F
PURPOSE  Sent from the DM to the DMSS to request the Mobile's ESN

============================================================================*/
DIAGPKT_REQ_DEFINE(DIAG_ESN_F)
DIAGPKT_REQ_END

DIAGPKT_RSP_DEFINE(DIAG_ESN_F)

  dword esn;

DIAGPKT_RSP_END

typedef DIAG_ESN_F_req_type diag_esn_req_type;
typedef DIAG_ESN_F_rsp_type diag_esn_rsp_type;

/*===========================================================================

PACKET   DIAG_EXT_BUILD_ID_F
PURPOSE  Sent from the DM to the DMSS to request the Mobile's ESN

============================================================================*/
typedef struct
{
  byte cmd_code;
  byte reserved[3]; /* for alignment / future use */

  uint32 msm_hw_version;
  uint32 mobile_model_id;

  /* The following character array contains 2 NULL terminated strings:
     'build_id' string, followed by 'model_string' */
  char ver_strings[1];

} toolsdiag_ext_build_id_rsp_type;

/*==========================================================================

PACKET   DIAG_DLOAD_F

PURPOSE  Request sent from the DM to the DMSS to enter the firmware
         downloader.

============================================================================*/
typedef struct
{
  byte cmd_code;
} toolsdiag_dload_req_type;

typedef struct
{
  byte cmd_code;
} toolsdiag_dload_rsp_type;

#if defined(T_WINNT)
#error code not present
#endif

//---------------------------------------------------------------------------
//
//	DIAG_CONTROL_F
//
//---------------------------------------------------------------------------
// Mode change commands
typedef enum
{
	DIAG_MODE_OFFLINE_A_F = 0,	// Go to offline analog
	DIAG_MODE_OFFLINE_D_F,		// Go to offline digital
	DIAG_MODE_RESET_F,			// Reset. Only exit from offline
	DIAG_MODE_FTM_F,				// FTM mode - if supported
	DIAG_MODE_ONLINE_F,			// Online mode - if supported
	DIAG_MODE_LPM_F,				// Low Power Mode - if supported
	DIAG_MODE_MAX_F				// Last (and invalid) mode enum value
};
// Request packet 
DIAGPKT_REQ_DEFINE(DIAG_CONTROL_F)
	word mode;		// Mode to change to
DIAGPKT_REQ_END
// Response packet
DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_CONTROL_F)

#endif /* TOOLSDIAG_H */

