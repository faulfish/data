#ifndef EVENT_DEFS_H
#define EVENT_DEFS_H

/*===========================================================================

             Event Reporting Service ID and Payload Definitions

General Description
  All definitions of event IDs and payload structures used by the event 
  reporting service.

Copyright (c) 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================
                          Edit History

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/event_defs.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
05/21/01   jal     Removed the #ifndef FEATURE_DIAG_NO_EVENTS -- this stuff
                   still needs to be there, even if the service is turned off.
05/21/01   sfh     Added EVENT_WCDMA_L1_STATE.
05/21/01   lad     Removed version check (now in event.c).
05/08/01   sfh     Merged events dropped when branch was made.  Added ba 
                   approved HDR events.
04/17/01   lad     Created file from event.h.

===========================================================================*/

#include "customer.h" /* Customer specific definitions.   */
#include "comdef.h"   /* Definitions for byte, word, etc. */



/* -------------------------------------------------------------------------
** Definitions and Declarations
** ------------------------------------------------------------------------- */

/* DO NOT MODIFY THIS FILE WITHOUT PRIOR APPROVAL
**
** Event IDs, by design, are a tightly controlled set of values.  Developers
** may not create event IDs at will.  
**
** Request new events using the folloiwng process:
**
** 1. Send email to asw.diag.request requesting event ID assignments.
** 2. Identify each event needed by name.
** 2. Provide a brief description for each event.
** 3. Describe the payload and size, if any.
**
*/

typedef enum
{
  EVENT_DROP_ID = 0,

  EVENT_BAND_CLASS_CHANGE = 0x0100,/* Includes band class as payload */
  EVENT_CDMA_CH_CHANGE,            /* Includes cdma channel as payload */
  EVENT_BS_P_REV_CHANGE,           /* Includes BS p_rev as payload */
  EVENT_P_REV_IN_USE_CHANGE,       /* Includes p_rev_in_use as payload */
  EVENT_SID_CHANGE,                /* Includes SID as payload */
  EVENT_NID_CHANGE,                /* Includes NID as payload */
  EVENT_PZID_CHANGE,               /* Includes PZID as payload */
  EVENT_PDE_SESSION_END,           /* No payload */
  EVENT_OP_MODE_CHANGE,               /* Includes operation mode as payload */
  EVENT_MESSAGE_RECEIVED,             /* Includes channel and message ID as 
                                         payload */
  EVENT_MESSAGE_TRANSMITTED,          /* Includes channel and message ID as 
                                         payload */
  EVENT_TIMER_EXPIRED,                /* Includes timer ID as payload */
  EVENT_COUNTER_THRESHOLD,            /* Includes counter ID as payload */
  EVENT_CALL_PROCESSING_STATE_CHANGE, /* Includes new state and old state as
                                         payload */ 
  EVENT_CALL_CONTROL_INSTANTIATED,    /* Includes con_ref as payload */
  EVENT_CALL_CONTROL_STATE_CHANGE,    /* Includes con_ref, old substate and
                                         new substate as payload */
  EVENT_CALL_CONTROL_TERMINATED,      /* Includes con_ref as payload */
  EVENT_REG_ZONE_CHANGE,              /* Includes reg_zone as payload */ 
  EVENT_SLOTTED_MODE_OPERATION,       /* Includes enter/exit bit as payload */
  EVENT_QPCH_IN_USE,                  /* Includes enable/disable bit as payload */
  EVENT_IDLE_HANDOFF,                 /* Includes pn_offset as payload */
  EVENT_ACCESS_HANDOFF,               /* Includes pn_offset as payload */
  EVENT_ACCESS_PROBE_HANDOFF,         /* Includes pn_offset as payload */
  EVENT_SOFT_HANDOFF,   
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_HARD_HANDOFF_FREQ_CHANGE, 
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_HARD_HANDOFF_FRAME_OFFSET_CHANGE, 
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_HARD_HANDOFF_DISJOINT_ASET,
    /* Includes pn_offsets of each BS in aset and indicators whether BS in SCH 
       aset*/ 
  EVENT_UNSUCCESSFUL_HARD_HANDOFF,    /* No payload */
  EVENT_TMSI_ASSIGNED,                /* Includes TMSI as payload */
  EVENT_SERVICE_NEGOTIATION_COMPLETED,/* No payload */
  EVENT_SO_NEGOTIATION_COMPLETED,     /* No payload */
  EVENT_ENTER_CONTROL_HOLD_MODE,      /* No payload */
  EVENT_EXIT_CONTROL_HOLD_MODE,       /* No payload */
  EVENT_START_FWD_SUPP_BURST_ASSGN,   /* Includes SCH rate as payload */
  EVENT_END_FWD_SUPP_BURST_ASSGN,     /* No payload */
  EVENT_START_REV_SUPP_BURST_ASSGN,   /* Includes SCH rate as payload */
  EVENT_END_REV_SUPP_BURST_ASSGN,     /* No payload */
  EVENT_DTX,                          /* No payload */
  EVENT_T_ADD_ABORT,                  /* No payload */
  EVENT_CH_IND_CHANGE,                /* Include ch_ind as payload */
  EVENT_TRANSMITTER_DISABLED,         /* No payload */
  EVENT_TRANSMITTER_ENABLED,          /* No payload */
  EVENT_SMS_RECEIVED,                 /* No payload */
  EVENT_SMS_SENT,                     /* No payload */
  EVENT_INACTIVITY_TIMER_EXPIRED,     /* No payload */
  EVENT_DORMANT_TIMER_EXPIRED,        /* No payload */
  EVENT_ACCESS_ATTEMPT_FAIL_MAX_PROBES_SENT, /* No payload */
  EVENT_ACCESS_ATTEMPT_FAIL_LOSS_OF_PC_OR_FCCC, /* No payload */
  EVENT_PCH_ACQUIRED,                 /* Includes pagech and pn_offset
                                         as payload */
  EVENT_BCCH_ACQUIRED,                /* Includes walsh code for BCCH and 
                                         pn_offset as payload */
  EVENT_FFCH_ACQUIRED,                /* Payload: 14 bytes */
  EVENT_FDCCH_ACQUIRED,               /* Payload: 14 bytes */
  EVENT_FFCH_PLUS_DCCH_ACQUIRED,      /* No payload */
  EVENT_REGISTRATION_PERFORMED,       /* Includes reg_type as payload */ 
  EVENT_NEW_SYSTEM_IDLE_HANDOFF,      /* No payload */
  EVENT_SYSTEM_RESELECTION,           /* Includes ecio and ps as payload */
  EVENT_RESCAN,                       /* No payload */
  EVENT_PROTOCOL_MISMATCH,            /* No payload */
  EVENT_LOCK,                         /* No payload */
  EVENT_UNLOCK,                       /* No payload */
  EVENT_ACCESS_DENIED,                /* No payload */
  EVENT_NDSS_OFF,                     /* No payload */
  EVENT_RELEASE,                      /* Payload: 1 byte */
  EVENT_ERROR,                        /* No payload */
  EVENT_REDIRECTION,                  /* No payload */
  EVENT_REGISTRATION_REJECTED,        /* No payload */
  EVENT_WRONG_SYSTEM,                 /* No payload */
  EVENT_WRONG_NETWORK,                /* No payload */
  EVENT_LOSS_OF_ACQ_AFTER_SLEEP,      /* No payload */
  EVENT_POWER_DOWN,                   /* No payload */
  EVENT_CALL_RELEASE_REQUEST,         /* No payload */
  EVENT_SERVICE_INACTIVE,             /* No payload */
  EVENT_EXTENDED_RELEASE,             /* No payload */

  EVENT_HDR_MSG_RX,                   /* protocol, msg- 3 bytes */
  EVENT_HDR_RXMSG_IGNORED_STATE,      /* protocol, msg- 3 bytes */
  EVENT_HDR_RXMSG_IGNORED_SEQ,        /* protocol, msg- 3 bytes */
  EVENT_HDR_TXMSG_ACKED,              /* protocol, msg- 3 bytes */
  EVENT_HDR_TXMSG_DROPPED,            /* protocol, msg- 3 bytes */      
  EVENT_HDR_STATE_CHANGE,             /* protocol, from, to - 5 bytes */
  EVENT_HDR_ALMP_OBEYING_REDIRECTION, /* No payload */
  EVENT_HDR_ALMP_CONNECTION_CLOSED,   /* No payload */
  EVENT_HDR_ALMP_T_SD_RESELECT,       /* No payload */
  EVENT_HDR_ALMP_CONNECTION_OPENED,   /* No payload */
  EVENT_HDR_HMP_QUEUED_MSG,           /* protocol, msg- 3 bytes */
  EVENT_HDR_HMP_SENT_MSG,             /* protocol, msg, chan, is_reliable - 5 bytes */
  EVENT_HDR_HMP_ABORTING_ACMAC_ACTIVATION, /* No payload */
  EVENT_HDR_IDLE_T_CONFIG_RSP,        /* No payload */
  EVENT_HDR_IDLE_T_AT_SETUP,          /* No payload */
  EVENT_HDR_IDLE_T_SUSPEND,           /* No payload */
  EVENT_HDR_IDLE_CONNECTION_DENIED,   /* No payload */
  EVENT_HDR_INIT_T_SYNC_ACQ,          /* No payload */
  EVENT_HDR_INIT_PROTOCOL_MISMATCH,   /* No payload */
  EVENT_HDR_OVHD_INFO_CURRENT,        /* No payload */
  EVENT_HDR_OVHD_T_QC_SUPERVISION,    /* No payload */
  EVENT_HDR_OVHD_T_SP_SUPERVISION,    /* No payload */
  EVENT_HDR_OVHD_T_AP_SUPERVISION,    /* No payload */
  EVENT_HDR_OVHD_IGNORED_MSG_UNEXPECTED_LINK, /* msg, exp_link.chan_num,
                                              exp_link.pilot, rx_link.chan_num,
                                              rx_link.pilot - 10 bytes */
  EVENT_HDR_OVHD_IGNORED_SP_MSG_DIFF_SEC_SIG, /* exp_sig, rx_sig - 8 bytes */
  EVENT_HDR_OVHD_IGNORED_AP_MSG_DIFF_ACC_SIG, /* exp_sig, rx_sig - 8 bytes */
  EVENT_HDR_OVHD_IGNORED_SP_MSG_DIFF_SEC_ID,  /* No payload */
  EVENT_HDR_OVHD_SP_MSG_RX,           /* No payload */
  EVENT_HDR_OVHD_AP_MSG_RX,           /* No payload */
  EVENT_HDR_RUP_T_CONNECTION_SETUP,   /* No payload */
  EVENT_HDR_SLP_MAX_RETRIES,          /* msg - 2 bytes */
  EVENT_HDR_LMAC_ACQ_FAIL_PILOT,      /* No payload */
  EVENT_HDR_LMAC_ACQ_SUCCESS,         /* No payload */
  EVENT_HDR_LMAC_NETWORK_LOST,        /* No payload */
  EVENT_HDR_LMAC_IDLE_HO,             /* new_pilot - 2 bytes */
  EVENT_HDR_LMAC_CHAN_CHANGE_COMPLETE, /* No payload */
  EVENT_HDR_LMAC_ACCESS_HO_NEEDED,    /* suggested_pilot - 2 bytes */
  EVENT_HDR_LMAC_ACCESS_HO_COMPLETE,  /* new_pilot - 2 bytes */
  EVENT_HDR_LMAC_ACQUIRE,             /* channel 2 bytes */
  EVENT_HDR_LMAC_CHANGING_CC_HASH,    /* cc_hash - 1 byte */
  EVENT_HDR_LMAC_IDLE_CHAN_CHANGE,    /* channel - 2 bytes */
  EVENT_HDR_CMAC_T_SUPERVISION,       /* No payload */
  EVENT_HDR_AMAC_START_ACCESS,        /* No payload */
  EVENT_HDR_AMAC_PROBING_STOPPED,     /* No payload */
  EVENT_HDR_AMAC_ACCESS_COMPLETE,     /* No payload */
  EVENT_HDR_AMAC_ACCESS_ABORTED,      /* No payload */
  EVENT_HDR_AMAC_MAX_PROBES,          /* No payload */
  EVENT_HDR_FMAC_DROP_PKT,            /* No payload */
  EVENT_HDR_RMAC_T_RATE_LIMIT,        /* No payload */
  EVENT_HDR_RMAC_TX_STARTED,          /* No payload */
  EVENT_HDR_RMAC_TX_STOPPED,          /* No payload */
  EVENT_HDR_SMP_T_KEEP_ALIVE,         /* No payload */
  EVENT_HDR_AMP_ASSIGN_MSG_IGNORED_FRESH, /* No payload */
  EVENT_HDR_AMP_T_AT_RESPONSE,        /* No payload */
  EVENT_HDR_AMP_T_DUAL_ADDRESS,       /* No payload */
  EVENT_HDR_SCP_BEGIN_CONFIGURATION,  /* No payload */
  EVENT_HDR_SCP_T_CONFIG_RSP,         /* No payload */
  EVENT_HDR_SCP_T_AN_INIT_STATE,      /* No payload */

  EVENT_WCDMA_L1_STATE,               /* l1_state - 1 byte */
  EVENT_WCDMA_IMSI,                   /* IMSI - 9 bytes */
  EVENT_GSM_L1_STATE,                 /* GSM l1_state - 1 byte */
  EVENT_RANDOM_ACCESS_REQUEST,        /* GSM Random Access Request - 4 bytes */
  EVENT_HIGH_LEVEL_CALL_PROCESSING_STATE_CHANGE, /* Puma requested event */
                                                 /* (same payload as CALL_PROCESSING_STATE_CHANGE) */
  EVENT_ENCRYPTION_FAILURE,                  /* Puma event, no payload */
  EVENT_ACCT_BLOCKED,                        /* Puma event, no payload */
  EVENT_COMMON_CHANNEL_MONITORED,            /* Puma event, 1 byte payload */
  EVENT_SOFT_HANDOFF_V2,                     /* Puma event, 14 byte payload */
  EVENT_HARD_HANDOFF_FREQ_CHANGE_V2,         /* Puma event, 14 byte payload */
  EVENT_HARD_HANDOFF_FRAME_OFFSET_CHANGE_V2, /* Puma event, 14 byte payload */
  EVENT_HARD_HANDOFF_DISJOINT_ASET_V2,       /* Puma event, 14 byte payload */
  EVENT_WCDMA_NEW_REFERENCE_CELL,
  EVENT_CALL_CONTROL_CONREF_CHANGE,          /* Puma event, 2 byte payload */

  EVENT_GPS_SESSION_BEGIN,
  EVENT_GPS_SESSION_END,
  EVENT_GPS_WAITING_ON_SA,
  EVENT_GPS_PPM_START,
  EVENT_GPS_PPM_RESULTS,
  EVENT_GPS_PPM_END,
  EVENT_GPS_VISIT_BEGIN,
  EVENT_GPS_VISIT_END,
  EVENT_GPS_CDMA_RESUMED_AFTER_GPS_VISIT,
  EVENT_GPS_PD_SESSION_BEGIN,
  EVENT_GPS_PD_SESSION_END,                  /* Payload: 1 byte PDSM substate */
  EVENT_GPS_IS801_RX,                        /* Payload, 1 byte msg_type */
  EVENT_GPS_IS801_TX,                        /* Payload: 1 byte msg_type */
  EVENT_POWERUP,
  EVENT_WCDMA_ASET,
  EVENT_CM_CALL_STATE,                       /* (1 byte payload: overall call state) */
  EVENT_CM_OPERATIONAL_MODE,                 /* (1 byte payload: op mode) */
  EVENT_CM_SYSTEM_MODE,                      /* (1 byte payload: sys_mode) */

  EVENT_DEEP_SLEEP,                          /* no payload */
  EVENT_WAKEUP,                              /* unsigned long (4 bytes) payload */
  EVENT_ACQUISITION_MODE,                    /* unsigned char (1 byte) payload */
  EVENT_ACQUISITION_TYPE,                    /* unsigned char (1 byte) payload */
  EVENT_ACP_EXIT,                            /* unsigned char (1 byte) payload */
  EVENT_CDMA_EXIT,                           /* unsigned char (1 byte) payload */

  EVENT_HDR_HYBRID_POWER_SAVE,               /* No payload */
  EVENT_HDR_DEEP_SLEEP,                      /* No payload */
  EVENT_HDR_RESELECTION,                     /* No payload */
  EVENT_SAM_LOCK_GRANTED,                    /* <NewOwner (1 byte) <Duration (2 bytes) */
  EVENT_SAM_LOCK_RELEASED,                   /* <OldOwner (1 byte) */

  EVENT_GSM_HANDOVER_START,
  EVENT_GSM_HANDOVER_END,
  EVENT_GSM_LINK_FAILURE,
  EVENT_GSM_RESELECT_START,
  EVENT_GSM_RESELECT_END,
  EVENT_GSM_CAMP_ATTEMPT_START,
  EVENT_GSM_RR_IN_SERVICE,
  EVENT_GSM_RR_OUT_OF_SERVICE,
  EVENT_GSM_PAGE_RECEIVED,
  EVENT_GSM_CAMP_ATTEMPT_END,
  EVENT_GPS_IS801_TIMEOUT,
  EVENT_GPS_IS801_DISCARD,                   /* Payload: 1 byte msg type */
  EVENT_GSM_CELL_SELECTION_START,            /* Payload: 1 byte cell_selection_reason */
  EVENT_GSM_CELL_SELECTION_END,              /* Payload: 1 byte end_status success/failure reason */
  EVENT_GSM_POWER_SCAN_STATUS,               /* Payload: 1 byte status (started or completed) */
  EVENT_GSM_PLMN_LIST_START,                 /* Payload: 1 byte */
  EVENT_GSM_PLMN_LIST_END,
  EVENT_WCDMA_INTER_RAT_HANDOVER_START,      /* Payload: 4 bytes handover type, BCCH ARFCN, BSIC */
  EVENT_WCDMA_INTER_RAT_HANDOVER_END,        /* Payload: 2 bytes success, failure reason */

  EVENT_GSM_MESSAGE_SENT,                    /* Payload: 2 bytes indicating identity of the message */
  EVENT_GSM_MESSAGE_RECEIVED,                /* Payload: 2 bytes indicating identity of the message */
  EVENT_GSM_TIMER_EXPIRED,                   /* Payload: 2 bytes indicating identity of the message */
  EVENT_GSM_COUNTER_EXPIRED,                 /* Payload: 2 bytes indicating identity of the message */

  EVENT_NAS_MESSAGE_SENT,
  EVENT_NAS_MESSAGE_RECEIVED,
  EVENT_RRC_MESSAGE_SENT,
  EVENT_RRC_MESSAGE_RECEIVED,
                                             /* Camera events: No Payload */
  EVENT_CAMERA_CANNOT_CAPTURE, /* Cannot capture, transition to ready */
  EVENT_CAMERA_CANNOT_CONFIG_JPEG, /* Cannot config JPEG, transition to ready */
  EVENT_CAMERA_CANNOT_CONFIG_VFE, /* Cannot config VFE, transition to ready */
  EVENT_CAMERA_CANNOT_ENCODE, /* Cannot encode, transition to ready */
  EVENT_CAMERA_CANNOT_IDLE_DSP, /* Cannot idle DSP, transition to ready */
  EVENT_CAMERA_CANNOT_LOAD_DSP, /* Cannot load DSP, transition to ready */
  EVENT_CAMERA_DSP_FATAL, /* DSP fatal error, transition to ready */
  EVENT_CAMERA_DSP_REQ_ILLEGAL, /* DSP request illegal, transition to ready */
  EVENT_CAMERA_EFS_FAILED, /* EFS failed, transition to ready */
  EVENT_CAMERA_EXIT, /* Stop, transition to ready */
  EVENT_CAMERA_FORMAT_NOT_SUPPORTED, /* Format not supported */
  EVENT_CAMERA_FUNCTION_REJECTED, /* Function rejected, transition to ready */
  EVENT_CAMERA_IMAGE_CORRUPT, /* Image corrupted, reconfig */
  EVENT_CAMERA_INVALID_CONFIG_PARM, /* Invalid config parm, to ready */
  EVENT_CAMERA_INVALID_SET_ID, /* Invalid set param ID type */
  EVENT_CAMERA_INVALID_STATE, /* Invalid state */
  EVENT_CAMERA_JPEG_ENCODED, /* JPEG encoded */
  EVENT_CAMERA_NO_MEMORY, /* No memory, transition to ready */
  EVENT_CAMERA_NO_PICTURE, /* Picture not ready */
  EVENT_CAMERA_PICTURE_SAVED, /* Picture saved, transition to ready */
  EVENT_CAMERA_PICTURE_TAKEN, /* Picture taken, transition to ready */
  EVENT_CAMERA_PREVIEW, /* Enter preview */
  EVENT_CAMERA_RECORD, /* Enter recording */
  EVENT_CAMERA_SAVE_PICTURE, /* Enter save picture */
  EVENT_CAMERA_SET_FAILED, /* Set failed */
  EVENT_CAMERA_SET_SUCCEEDED, /* Set succeeded */
  EVENT_CAMERA_START, /* Start, transition to ready */
  EVENT_CAMERA_STOP, /* Stop, transition to init */
  EVENT_CAMERA_TAKE_PICTURE, /* Enter take picture */

  EVENT_DIAG_STRESS_TEST_NO_PAYLOAD,
  EVENT_DIAG_STRESS_TEST_WITH_PAYLOAD,

  EVENT_CM_CALL_ORIG_START_P1,      /* Payload: 5 single bytes */
  EVENT_CM_CALL_ORIG_START_P2,      /* Payload: 3 single bytes */
  EVENT_CM_CALL_ORIG_START_P3,      /* Payload: 3 single bytes */
  EVENT_CM_CALL_ORIG_SWITCH_TO_HDR, /* No payload */
  EVENT_CM_CALL_ORIG_REDIAL,        /* Payload: 4 bytes */
  EVENT_CM_CALL_ORIG_SEND_HDR_ORIG, /* No payload */
  EVENT_CM_CALL_ORIG_SEND_MC_ORIG,  /* No payload */
  EVENT_CM_CALL_ORIG_END,           /* Payload: 2 bytes */
  EVENT_CM_CALL_ORIG_CONNECTED,     /* Payload: 3 bytes */

  EVENT_MT_SMS_NOTIFY,              /* Payload: 1 byte mem_type, 4 byte msg_index */
  EVENT_SMS_SLOT_WAKEUP,            /* Payload: 1 byte slot_mask */
  EVENT_MO_SMS_STATUS,              /* Payload: 4 byte transaction_id, 1 byte mem_type,
                                                4 byte msg_index, 1 byte report_status, 
                                                1 byte case_code */
  EVENT_GPRS_SURROUND_SEARCH_START,
  EVENT_GPRS_SURROUND_SEARCH_END,
  EVENT_GPRS_MAC_RESELECT_IND,
  EVENT_GPRS_PAGE_RECEIVED,
  EVENT_GPRS_LINK_FAILURE,
  EVENT_GPRS_CELL_UPDATE_START,
  EVENT_GPRS_CELL_UPDATE_END,
  EVENT_GPRS_EARLY_CAMPING,
  EVENT_PACKET_RANDOM_ACCESS_REQ,
  EVENT_GPRS_MAC_MSG_SENT,          /* Payload: 3 bytes giving message identity and channel */
  EVENT_GPRS_MAC_MSG_RECEIVED,      /* Payload: 3 bytes giving message identity and channel */
  EVENT_GPRS_SMGMM_MSG_SENT,        /* Payload: 3 bytes giving message identity and channel */
  EVENT_GPRS_SMGMM_MSG_RECEIVED,    /* Payload: 3 bytes giving message identity and channel */

  EVENT_CP_MATCHED_MSG,             /* Payload: 1 byte */
  EVENT_PREF_SYS_RESEL,             /* Payload: none */ 

  EVENT_WCDMA_LAYER1_PRACH,         /* Payload: 4 bytes */
  EVENT_WCDMA_LAYER1_MEASUREMENT,   /* Payload: Unspecified */

  EVENT_MOBILITY_MANAGEMENT_STATE_CHANGE, /* Payload: 1 byte old state, 1 byte new state */

  EVENT_LSM_STATE_CHANGE,           /* Payload: 1 byte */
  EVENT_RLP,                        /* Payload: 2 bytes */

  EVENT_CM_MODE_PREF,               /* Payload: 4 bytes */
  EVENT_CM_BAND_PREF,               /* Payload: 4 bytes */
  EVENT_CM_ROAM_PREF,               /* Payload: 4 bytes */
  EVENT_CM_SRV_DOMAIN_PREF,         /* Payload: 4 bytes */
  EVENT_CM_GW_ACQ_ORDER_PREF,       /* Payload: 4 bytes */
  EVENT_CM_HYBRID_PREF,             /* Payload: 4 bytes */
  EVENT_CM_NETWORK_SEL_MODE_PREF,   /* Payload: 4 bytes */

  EVENT_WCDMA_L1_SUSPEND,                 /* Payload: 3 bytes */
  EVENT_WCDMA_L1_RESUME,                  /* Payload: 5 bytes */
  EVENT_WCDMA_L1_STOPPED,                 /* Payload: 4 bytes */
  EVENT_WCDMA_TO_WCDMA_RESELECTION_START, /* Payload: 6 bytes */
  EVENT_WCDMA_TO_GSM_RESELECTION_START,   /* Payload: 4 bytes */
  EVENT_WCDMA_TO_GSM_RESELECTION_END,     /* Payload: 2 bytes */
  EVENT_WCDMA_TO_WCDMA_RESELECTION_END,   /* Payload: 4 bytes */
  EVENT_WCDMA_RACH_ATTEMPT,               /* Payload: 3 bytes */

  EVENT_START_FWD_SUPP_BURST_ASSIGN,      /* Payload: 3 bytes */
  EVENT_START_REV_SUPP_BURST_ASSIGN,      /* Payload: 3 bytes */
  EVENT_REV_FCH_GATING_IN_USE,            /* Payload: 1 byte  */
  EVENT_PPP,                              /* Payload: 7 bytes */
  EVENT_MIP,                              /* Payload: 2 bytes */
  EVENT_TCP,                              /* Payload: 5 bytes */
  EVENT_CAMERA_EXIF_FAILED,               /* EXIF encoding failed */
  EVENT_CAMERA_VIDEO_FAILED,              /* Video encoding failed */
  EVENT_CAMERA_NO_SENSOR,                 /* No sensor */
  EVENT_CAMERA_ABORT,                     /* Operation aborted */

  EVENT_CM_BLOCK_HDR_ORIG_DURING_GPS,     /* No payload */
  EVENT_CM_ALLOW_HDR_ORIG_DURING_GPS,     /* No payload */

  EVENT_GSM_AMR_STATE_CHANGE,             /* 2 bytes payload */
  EVENT_GSM_RATSCCH_IN_DTX,               /* No Payload */
  EVENT_GSM_FACCH_IN_DTX,                 /* No Payload */
  EVENT_GSM_FACCH_AND_RATSCCH_COLLISION,  /* No Payload */
  EVENT_GSM_FACCH_AND_SID_UPDATE_COLLISION,  /* No Payload */
  EVENT_GSM_RATSCCH_AND_SID_UPDATE_COLLISION,/* No Payload */
  EVENT_GSM_RATSCCH_CMI_PHASE_CHANGE,     /* 2 bytes payload */
  EVENT_GSM_RATSCCH_REQ_ACT_TIMER_EXPIRY, /* 8 bytes payload */
  EVENT_GSM_RATSCCH_ACK_ACT_TIMER_EXPIRY, /* 2 bytes payload */
  EVENT_GSM_AMR_CMC_TURNAROUND_TIME,      /* 2 bytes payload */
  EVENT_CM_PLMN_FOUND,                    /* 12 bytes */
  EVENT_CM_SERVICE_CONFIRMED,             /* 12 bytes */

  EVENT_GPRS_MAC_CAMPED_ON_CELL,          /* Event Id : 559, No payload */
  EVENT_GPRS_LLC_READY_TIMER_START,       /* Event Id : 560, No payload */
  EVENT_GPRS_LLC_READY_TIMER_END,         /* Event Id : 561, No payload */

  EVENT_WCDMA_PHYCHAN_ESTABLISHED,        /* Payload TBD */
  EVENT_HS_DISPLAY_BMP_CAPTURE_STATUS,    /* Payload 4 bytes */

  EVENT_WCDMA_CELL_SELECTED,              /* 4 byte Payload */
  EVENT_WCDMA_PAGE_RECEIVED,              /* 2 byte Payload */
  EVENT_WCDMA_SEND_KEY,                   /* Payload TBD */
  EVENT_WCDMA_RL_FAILURE,                 /* No Payload */
  EVENT_WCDMA_MAX_RESET,                  /* 2 byte Payload */
  EVENT_WCDMA_CALL_SETUP,                 /* Payload TBD */
  EVENT_WCDMA_CALL_DROPPED,               /* Payload TBD */
  EVENT_WCDMA_RRC_STATE,                  /* 3 byte Payload */
  EVENT_GPS_PD_CONNECTION_TIMEOUT,        /* No Payload */
  EVENT_GPS_PD_DISCONNECTION_COMPLETE,    /* No Payload */

  EVENT_MEDIA_PLAYER_START,               /* media player starts playing a clip, no payload */
  EVENT_MEDIA_PLAYER_STOP,                /* media player stopped playing a clip, no payload */
  EVENT_MEDIA_PLAYER_SEEK,                /* media player repositioned itself, no payload */

  EVENT_GPS_SRCH_START,             /* Payload: session_type (1 byte) */

  EVENT_GPS_SRCH_END,               /* No Payload */
  EVENT_GPS_PPM_PAUSE,              /* Payload: pause_reason (1 byte) */
  EVENT_GPS_PPM_RESUME,             /* No Payload */
  EVENT_GPS_SA_RECEIVED,            /* Payload: REF_BIT_NUM (2 bytes), 
                                                DR_SIZE     (1byte) 
                                    */
  EVENT_GPS_CLK_ON,                 /* No Payload */
  EVENT_GPS_CLK_OFF,                /* No Payload */
  EVENT_GPS_VISIT_REQUEST,          /* No Payload */ 
  EVENT_GPS_VISIT_RESPONSE,         /* Payload: response_result (1 byte)
                                    */ 
  EVENT_GPS_TA_START,               /* No Payload */
  EVENT_GPS_DSP_READY,              /* No Payload */
  EVENT_GPS_DSP_CHANNEL_START,      /* Payload: SV_ID         (1 byte), 
                                                SRCH_MODE     (1 byte), 
                                                CHANNEL_NUM   (1 byte),
                                                RESERVED      (1 byte)
                                    */
  EVENT_GPS_DSP_CHANNEL_DONE,       /* Payload: channel_num (1 byte) */
  EVENT_GPS_DSP_STOP,               /* No Payload */
  EVENT_GPS_DSP_DONE,               /* No Payload */
  EVENT_GPS_TB_END,                 /* No Payload */
  EVENT_GPS_SRCH_LARGE_DOPP_WIN,    /* Payload: sv_prn_num (1 byte),
                                                srch_mode (1 byte),
                                                dopp_wind (2 byte)
                                    */ 
  EVENT_GPS_SRCH_EXCEPTION,         /* Payload: grid_log_id (2 byte),
                                                exception_type (1 byte)
                                    */
  EVENT_GPS_SRCH_HW_POLLING1,       /* Payload: agc_val (2 byte),
                                                dci_off (2 byte),
                                                dcq_off (2 byte),
                                                trk_lo (2 byte),
                                                lo_bias (2 byte)
                                    */

  EVENT_GPS_SRCH_HW_POLLING2,       /* Payload: sync80 (2 byte)
                                    */
  EVENT_GPS_PGI_ACTION_PROCESS,     /* Payload: pgi_substate (1 byte),
                                                pgi_cmd (1 byte)
                                    */
  EVENT_GPS_GSC_ACTION_PROCESS,     /* Payload: gsc_substate (1 byte),
                                                gsc_cmd (1 byte)
                                    */
  EVENT_GPS_PGI_ABORT,              /* Payload: pgi_subsate (1 byte) */
  EVENT_GPS_GSC_ABORT,              /* Payload: gsc_subsate (1 byte) */



  EVENT_GPS_PD_FIX_START,                /* Payload: event_log_cnt  (2 byte), 
                                                operation_mode (1 byte)
                                         */
  EVENT_GPS_PD_FIX_END,                  /* Payload: end_status     (1 byte) */
  EVENT_GPS_DATA_DOWNLOAD_START,         /* Payload: data_type   (1 byte),
                                                     sv_mask     (4 byte)
                                         */
  EVENT_GPS_DATA_DOWNLOAD_END,           /* Payload: end_status      (1 byte) */
  EVENT_GPS_PD_SESSION_START,            /* Payload: start_source    (4 bit)
                                                     operation_mode  (4 bit)
                                                     session_type    (4 bit)
                                                     privacy         (4 bit)
                                                     num_fixed       (2 byte)
                                                     fix_period      (2 byte)
                                                     nav_data_dl     (4 bit)
                                                     prq             (1 byte)
                                                     threshold       (2 byte)
                                                     transport_type  (4 bit)
                                          */
  EVENT_GPS_DORMANCY_BEGIN,               /* No Payload */
  EVENT_GPS_DORMANCY_END,                 /* No Payload */
  EVENT_GPS_PRQ_TIMEOUT,                  /* No Payload */
  EVENT_GPS_PD_CONNECTION_START,          /* No Payload */
  EVENT_GPS_PD_CONNECTION_ESTABLISHED,    /* No Payload */
  EVENT_GPS_PD_DISCONNECTION_START,       /* No Payload */
  EVENT_GPS_FTEST_FIX_START,              /* Payload: reserved (4 byte) */
  EVENT_GPS_FTEST_FIX_END,                /* Payload: reserved (4 byte) */
  EVENT_GPS_PD_POSITION,                  /* No Payload */
  EVENT_GPS_E911_START,                   /* No Payload */
  EVENT_GPS_E911_END,                     /* No Payload */
  EVENT_GPS_DBM_SEND_FAILURE,             /* No Payload */
  EVENT_GPS_UAPDMS_STATE_CHANGE,          /* Payload: new_state (1 byte)
                                                      reason    (1 byte)
                                          */
  EVENT_WCDMA_OUT_OF_SERVICE,             /* No Payload */
  EVENT_GSM_L1_SUBSTATE,                  /* 2 bytes payload */
  EVENT_SD_EVENT_ACTION,                  /* 8 byte payload */
  EVENT_SD_EVENT_ACTION_HYBR,             /* 8 byte payload */

  EVENT_UMTS_CALLS_STATISTICS,            /* 1 byte payload */
  EVENT_PZID_HAT_STARTED,                 /* No payload */
  EVENT_WCDMA_DRX_CYCLE,                  /* 3 byte payload */
  EVENT_WCDMA_RE_ACQUISITION_FAIL,        /* No payload */
  EVENT_WCDMA_RRC_RB0_SETUP_FAILURE,      /* No payload */
  EVENT_WCDMA_RRC_PHYCHAN_EST_FAILURE,    /* No payload */
  EVENT_CM_CALL_EVENT_ORIG,               /* 3 byte payload */
  EVENT_CM_CALL_EVENT_CONNECT,            /* 3 byte payload */
  EVENT_CM_CALL_EVENT_END,                /* 2 byte payload */
  EVENT_CM_ENTER_EMERGENCY_CB,            /* No payload */
  EVENT_CM_EXIT_EMERGENCY_CB,             /* No payload */
  EVENT_PZID_HAT_EXPIRED,                 /* No payload */
  EVENT_HDR_SMP_SESSION_CLOSED,           /* 1 byte payload */
  EVENT_WCDMA_MEMORY_LEAK,                /* No payload */
  EVENT_PZID_HT_STARTED,                  /* 1 byte payload */
  EVENT_PZID_HT_EXPIRED,                  /* 1 byte payload */
  EVENT_ACCESS_ENTRY_HANDOFF,             /* 2 byte payload */

  EVENT_BREW_APP_START,                   /* 8 byte payload */
  EVENT_BREW_APP_STOP,                    /* 8 byte payload */
  EVENT_BREW_APP_PAUSE,                   /* 8 byte payload */
  EVENT_BREW_APP_RESUME,                  /* 8 byte payload */
  EVENT_BREW_EXT_MODULE_START,            /* 8 byte payload */
  EVENT_BREW_EXT_MODULE_STOP,             /* 8 byte payload */
  EVENT_BREW_ERROR,                       /* 8 byte payload */
  EVENT_BREW_RESERVED_647,                /* BREW internal use only */
  EVENT_BREW_RESERVED_648,                /* BREW internal use only */
  EVENT_BREW_RESERVED_649,                /* BREW internal use only */
  EVENT_BREW_RESERVED_650,                /* BREW internal use only */
  EVENT_BREW_RESERVED_651,                /* BREW internal use only */
  EVENT_BREW_RESERVED_652,                /* BREW internal use only */
  EVENT_BREW_RESERVED_653,                /* BREW internal use only */
  EVENT_BREW_RESERVED_654,                /* BREW internal use only */
  EVENT_BREW_RESERVED_655,                /* BREW internal use only */
  EVENT_BREW_USER_656,                    /* Start of BREW user events */
  EVENT_BREW_GENERIC,                     /* 8 byte payload: clsid + data */
  EVENT_BREW_MEDIAPLAYER_SELECT_FILE,     /* no payload */
  EVENT_BREW_MEDIAPLAYER_CONTROL,         /* no payload */
  EVENT_BREW_UNDEFINED_660,
  EVENT_BREW_UNDEFINED_661,
  EVENT_BREW_UNDEFINED_662,
  EVENT_BREW_UNDEFINED_663,
  EVENT_BREW_UNDEFINED_664,
  EVENT_BREW_UNDEFINED_665,
  EVENT_BREW_UNDEFINED_666,
  EVENT_BREW_UNDEFINED_667,
  EVENT_BREW_UNDEFINED_668,
  EVENT_BREW_UNDEFINED_669,
  EVENT_BREW_UNDEFINED_670,
  EVENT_BREW_UNDEFINED_671,
  EVENT_BREW_UNDEFINED_672,
  EVENT_BREW_UNDEFINED_673,
  EVENT_BREW_UNDEFINED_674,
  EVENT_BREW_UNDEFINED_675,
  EVENT_BREW_UNDEFINED_676,
  EVENT_BREW_UNDEFINED_677,
  EVENT_BREW_UNDEFINED_678,
  EVENT_BREW_UNDEFINED_679,
  EVENT_BREW_UNDEFINED_680,
  EVENT_BREW_UNDEFINED_681,
  EVENT_BREW_UNDEFINED_682,
  EVENT_BREW_UNDEFINED_683,
  EVENT_BREW_UNDEFINED_684,
  EVENT_BREW_UNDEFINED_685,
  EVENT_BREW_UNDEFINED_686,
  EVENT_BREW_UNDEFINED_687,
  EVENT_BREW_UNDEFINED_688,
  EVENT_BREW_UNDEFINED_689,
  EVENT_BREW_UNDEFINED_690,
  EVENT_BREW_UNDEFINED_691,
  EVENT_BREW_UNDEFINED_692,
  EVENT_BREW_UNDEFINED_693,
  EVENT_BREW_UNDEFINED_694,
  EVENT_BREW_UNDEFINED_695,
  EVENT_BREW_UNDEFINED_696,
  EVENT_BREW_UNDEFINED_697,
  EVENT_BREW_UNDEFINED_698,
  EVENT_BREW_UNDEFINED_699,
  EVENT_BREW_UNDEFINED_700,
  EVENT_BREW_UNDEFINED_701,
  EVENT_BREW_UNDEFINED_702,
  EVENT_BREW_UNDEFINED_703,
  EVENT_BREW_UNDEFINED_704,
  EVENT_BREW_UNDEFINED_705,
  EVENT_BREW_UNDEFINED_706,
  EVENT_BREW_UNDEFINED_707,
  EVENT_BREW_UNDEFINED_708,
  EVENT_BREW_UNDEFINED_709,
  EVENT_BREW_UNDEFINED_710,
  EVENT_BREW_UNDEFINED_711,
  EVENT_BREW_UNDEFINED_712,
  EVENT_BREW_UNDEFINED_713,
  EVENT_BREW_UNDEFINED_714,
  EVENT_BREW_UNDEFINED_715,
  EVENT_BREW_UNDEFINED_716,
  EVENT_BREW_UNDEFINED_717,
  EVENT_BREW_UNDEFINED_718,
  EVENT_BREW_UNDEFINED_719,
  EVENT_BREW_UNDEFINED_720,
  EVENT_BREW_UNDEFINED_721,
  EVENT_BREW_UNDEFINED_722,
  EVENT_BREW_UNDEFINED_723,
  EVENT_BREW_UNDEFINED_724,
  EVENT_BREW_UNDEFINED_725,
  EVENT_BREW_UNDEFINED_726,
  EVENT_BREW_UNDEFINED_727,
  EVENT_BREW_UNDEFINED_728,
  EVENT_BREW_UNDEFINED_729,
  EVENT_BREW_UNDEFINED_730,
  EVENT_BREW_UNDEFINED_731,
  EVENT_BREW_UNDEFINED_732,
  EVENT_BREW_UNDEFINED_733,
  EVENT_BREW_UNDEFINED_734,
  EVENT_BREW_UNDEFINED_735,
  EVENT_BREW_UNDEFINED_736,
  EVENT_BREW_UNDEFINED_737,
  EVENT_BREW_UNDEFINED_738,
  EVENT_BREW_UNDEFINED_739,
  EVENT_BREW_UNDEFINED_740,
  EVENT_BREW_UNDEFINED_741,
  EVENT_BREW_UNDEFINED_742,
  EVENT_BREW_UNDEFINED_743,
  EVENT_BREW_UNDEFINED_744,
  EVENT_BREW_UNDEFINED_745,
  EVENT_BREW_UNDEFINED_746,
  EVENT_BREW_UNDEFINED_747,
  EVENT_BREW_UNDEFINED_748,
  EVENT_BREW_UNDEFINED_749,
  EVENT_BREW_UNDEFINED_750,
  EVENT_BREW_UNDEFINED_751,
  EVENT_BREW_UNDEFINED_752,
  EVENT_BREW_UNDEFINED_753,
  EVENT_BREW_UNDEFINED_754,
  EVENT_BREW_UNDEFINED_755,
  EVENT_BREW_UNDEFINED_756,
  EVENT_BREW_UNDEFINED_757,
  EVENT_BREW_UNDEFINED_758,
  EVENT_BREW_UNDEFINED_759,
  EVENT_BREW_UNDEFINED_760,
  EVENT_BREW_UNDEFINED_761,
  EVENT_BREW_UNDEFINED_762,
  EVENT_BREW_UNDEFINED_763,
  EVENT_BREW_UNDEFINED_764,
  EVENT_BREW_UNDEFINED_765,
  EVENT_BREW_UNDEFINED_766,
  EVENT_BREW_UNDEFINED_767,
  EVENT_BREW_UNDEFINED_768,
  EVENT_BREW_UNDEFINED_769,
  EVENT_BREW_UNDEFINED_770,
  EVENT_BREW_UNDEFINED_771,
  EVENT_BREW_UNDEFINED_772,
  EVENT_BREW_UNDEFINED_773,
  EVENT_BREW_UNDEFINED_774,
  EVENT_BREW_UNDEFINED_775,
  EVENT_BREW_UNDEFINED_776,
  EVENT_BREW_UNDEFINED_777,
  EVENT_BREW_UNDEFINED_778,
  EVENT_BREW_UNDEFINED_779,
  EVENT_BREW_UNDEFINED_780,
  EVENT_BREW_UNDEFINED_781,
  EVENT_BREW_UNDEFINED_782,
  EVENT_BREW_UNDEFINED_783,
  EVENT_BREW_UNDEFINED_784,
  EVENT_BREW_UNDEFINED_785,
  EVENT_BREW_UNDEFINED_786,
  EVENT_BREW_UNDEFINED_787,
  EVENT_BREW_UNDEFINED_788,
  EVENT_BREW_UNDEFINED_789,
  EVENT_BREW_UNDEFINED_790,
  EVENT_BREW_UNDEFINED_791,
  EVENT_BREW_UNDEFINED_792,
  EVENT_BREW_UNDEFINED_793,
  EVENT_BREW_UNDEFINED_794,
  EVENT_BREW_UNDEFINED_795,
  EVENT_BREW_UNDEFINED_796,
  EVENT_BREW_UNDEFINED_797,
  EVENT_BREW_UNDEFINED_798,
  EVENT_BREW_UNDEFINED_799,
  EVENT_BREW_UNDEFINED_800,
  EVENT_BREW_UNDEFINED_801,
  EVENT_BREW_UNDEFINED_802,
  EVENT_BREW_UNDEFINED_803,
  EVENT_BREW_UNDEFINED_804,
  EVENT_BREW_UNDEFINED_805,
  EVENT_BREW_UNDEFINED_806,
  EVENT_BREW_UNDEFINED_807,
  EVENT_BREW_UNDEFINED_808,
  EVENT_BREW_UNDEFINED_809,
  EVENT_BREW_UNDEFINED_810,
  EVENT_BREW_UNDEFINED_811,
  EVENT_BREW_UNDEFINED_812,
  EVENT_BREW_UNDEFINED_813,
  EVENT_BREW_UNDEFINED_814,
  EVENT_BREW_UNDEFINED_815,
  EVENT_BREW_UNDEFINED_816,
  EVENT_BREW_UNDEFINED_817,
  EVENT_BREW_UNDEFINED_818,
  EVENT_BREW_UNDEFINED_819,
  EVENT_BREW_UNDEFINED_820,
  EVENT_BREW_UNDEFINED_821,
  EVENT_BREW_UNDEFINED_822,
  EVENT_BREW_UNDEFINED_823,
  EVENT_BREW_UNDEFINED_824,
  EVENT_BREW_UNDEFINED_825,
  EVENT_BREW_UNDEFINED_826,
  EVENT_BREW_UNDEFINED_827,
  EVENT_BREW_UNDEFINED_828,
  EVENT_BREW_UNDEFINED_829,
  EVENT_BREW_UNDEFINED_830,
  EVENT_BREW_UNDEFINED_831,
  EVENT_BREW_UNDEFINED_832,
  EVENT_BREW_UNDEFINED_833,
  EVENT_BREW_UNDEFINED_834,
  EVENT_BREW_UNDEFINED_835,
  EVENT_BREW_UNDEFINED_836,
  EVENT_BREW_UNDEFINED_837,
  EVENT_BREW_UNDEFINED_838,
  EVENT_BREW_UNDEFINED_839,
  EVENT_BREW_UNDEFINED_840,
  EVENT_BREW_UNDEFINED_841,
  EVENT_BREW_UNDEFINED_842,
  EVENT_BREW_UNDEFINED_843,
  EVENT_BREW_UNDEFINED_844,
  EVENT_BREW_UNDEFINED_845,
  EVENT_BREW_UNDEFINED_846,
  EVENT_BREW_UNDEFINED_847,
  EVENT_BREW_UNDEFINED_848,
  EVENT_BREW_UNDEFINED_849,
  EVENT_BREW_UNDEFINED_850,
  EVENT_BREW_UNDEFINED_851,
  EVENT_BREW_UNDEFINED_852,
  EVENT_BREW_UNDEFINED_853,
  EVENT_BREW_UNDEFINED_854,
  EVENT_BREW_UNDEFINED_855,
  EVENT_BREW_UNDEFINED_856,
  EVENT_BREW_UNDEFINED_857,
  EVENT_BREW_UNDEFINED_858,
  EVENT_BREW_UNDEFINED_859,
  EVENT_BREW_UNDEFINED_860,
  EVENT_BREW_UNDEFINED_861,
  EVENT_BREW_UNDEFINED_862,
  EVENT_BREW_UNDEFINED_863,
  EVENT_BREW_UNDEFINED_864,
  EVENT_BREW_UNDEFINED_865,
  EVENT_BREW_UNDEFINED_866,
  EVENT_BREW_UNDEFINED_867,
  EVENT_BREW_UNDEFINED_868,
  EVENT_BREW_UNDEFINED_869,
  EVENT_BREW_UNDEFINED_870,
  EVENT_BREW_UNDEFINED_871,
  EVENT_BREW_UNDEFINED_872,
  EVENT_BREW_UNDEFINED_873,
  EVENT_BREW_UNDEFINED_874,
  EVENT_BREW_UNDEFINED_875,
  EVENT_BREW_UNDEFINED_876,
  EVENT_BREW_UNDEFINED_877,
  EVENT_BREW_UNDEFINED_878,
  EVENT_BREW_UNDEFINED_879,
  EVENT_BREW_UNDEFINED_880,
  EVENT_BREW_UNDEFINED_881,
  EVENT_BREW_UNDEFINED_882,
  EVENT_BREW_UNDEFINED_883,
  EVENT_BREW_UNDEFINED_884,
  EVENT_BREW_UNDEFINED_885,
  EVENT_BREW_UNDEFINED_886,
  EVENT_BREW_UNDEFINED_887,
  EVENT_BREW_UNDEFINED_888,
  EVENT_BREW_UNDEFINED_889,
  EVENT_BREW_UNDEFINED_890,
  EVENT_BREW_UNDEFINED_891,
  EVENT_BREW_UNDEFINED_892,
  EVENT_BREW_UNDEFINED_893,
  EVENT_BREW_UNDEFINED_894,
  EVENT_BREW_UNDEFINED_895,

  EVENT_WCDMA_PS_DATA_RATE,                  /* 2 byte payload */
  EVENT_GSM_TO_WCDMA_RESELECT_END,           /* 5 byte payload */
  EVENT_PZID_HAI_ENABLED,                    /* No payload*/
  EVENT_PZID_HAI_DISABLED,                   /* No payload*/
  EVENT_GSM_TO_WCDMA_HANDOVER_START,         /* 4 byte payload */
  EVENT_WCDMA_RRC_MODE,                      /* 1 byte payload */
  EVENT_WCDMA_L1_ACQ_SUBSTATE,               /* 1 byte payload */
  EVENT_WCDMA_PHYCHAN_CFG_CHANGED,           /* 1 byte payload */

  EVENT_QTV_CLIP_STARTED,                    /* 7 byte payload */
  EVENT_QTV_CLIP_ENDED,                      /* 5 byte payload */
  EVENT_QTV_SDP_PARSER_REJECT,               /* No payload */
  EVENT_QTV_CLIP_PAUSE,                      /* 4 byte payload */
  EVENT_QTV_CLIP_REPOSITIONING,              /* 4 byte payload */
  EVENT_QTV_CLIP_ZOOM_IN,                    /* No payload */
  EVENT_QTV_CLIP_ZOOM_OUT,                   /* No payload */
  EVENT_QTV_CLIP_ROTATE,                     /* 4 byte payload */
  EVENT_QTV_CLIP_PAUSE_RESUME,               /* 4 byte payload */
  EVENT_QTV_CLIP_REPOSITION_RESUME,          /* 4 byte payload */
  EVENT_QTV_DSP_INIT,                        /* No payload */
  EVENT_QTV_STREAMING_SERVER_URL,            /* 22 byte payload */
  EVENT_QTV_SERVER_PORTS_USED,               /* 4 byte payload */
  EVENT_QTV_USING_PROXY_SERVER,              /* 6 byte payload */
  EVENT_QTV_STREAMER_STATE_IDLE,             /* No payload */
  EVENT_QTV_STREAMER_STATE_CONNECTING,       /* No payload */
  EVENT_QTV_STREAMER_STATE_SETTING_TRACKS,   /* No payload */
  EVENT_QTV_STREAMER_STATE_STREAMING,        /* No payload */
  EVENT_QTV_STREAMER_STATE_PAUSED,           /* No payload */
  EVENT_QTV_STREAMER_STATE_SUSPENDED,        /* No payload */
  EVENT_QTV_STREAMER_CONNECTED,              /* No payload */
  EVENT_QTV_STREAMER_INITSTREAM_FAIL,        /* No payload */
  EVENT_QTV_BUFFERING_STARTED,               /* 5 byte payload */
  EVENT_QTV_BUFFERING_ENDED,                 /* 5 byte payload */
  EVENT_QTV_CLIP_FULLSCREEN,                 /* No payload */
  EVENT_QTV_UNDEFINED_929,                   /* No payload */
  EVENT_QTV_UNDEFINED_930,
  EVENT_QTV_UNDEFINED_931,
  EVENT_QTV_UNDEFINED_932,
  EVENT_QTV_UNDEFINED_933,
  EVENT_QTV_UNDEFINED_934,
  EVENT_QTV_UNDEFINED_935,
  EVENT_QTV_UNDEFINED_936,
  EVENT_QTV_UNDEFINED_937,
  EVENT_QTV_UNDEFINED_938,
  EVENT_QTV_UNDEFINED_939,
  EVENT_QTV_UNDEFINED_940,
  EVENT_QTV_UNDEFINED_941,
  EVENT_QTV_UNDEFINED_942,
  EVENT_QTV_UNDEFINED_943,
  EVENT_QTV_UNDEFINED_944,
  EVENT_QTV_UNDEFINED_945,
  EVENT_QTV_UNDEFINED_946,
  EVENT_QTV_UNDEFINED_947,
  EVENT_QTV_UNDEFINED_948,
  EVENT_QTV_UNDEFINED_949,
  EVENT_QTV_UNDEFINED_950,
  EVENT_QTV_UNDEFINED_951,
  EVENT_QTV_UNDEFINED_952,
  EVENT_QTV_UNDEFINED_953,
  EVENT_QTV_UNDEFINED_954,
  EVENT_QTV_UNDEFINED_955,
  EVENT_QTV_UNDEFINED_956,
  EVENT_QTV_UNDEFINED_957,
  EVENT_QTV_UNDEFINED_958,
  EVENT_QTV_UNDEFINED_959,
  EVENT_QTV_UNDEFINED_960,
  EVENT_QTV_UNDEFINED_961,
  EVENT_QTV_UNDEFINED_962,
  EVENT_QTV_UNDEFINED_963,
  EVENT_QTV_UNDEFINED_964,
  EVENT_QTV_UNDEFINED_965,
  EVENT_QTV_UNDEFINED_966,
  EVENT_QTV_UNDEFINED_967,

  EVENT_DS_SETS_ARM_CLOCK_FASTER,      /* No payload */
  EVENT_DS_SETS_ARM_CLOCK_SLOWER,      /* No payload */

  EVENT_SMS_STATISTICS,                /* 2 byte payload */
  EVENT_SM_PDP_STATE,                  /* 4 byte payload */
  EVENT_MVS_STATE,                     /* 2 byte payload */

  EVENT_SECSSL,                        /* 16 byte payload */
  EVENT_SECTEST,                       /* 16 byte payload */
  EVENT_SECVPN,                        /* 16 byte payload */
  EVENT_SECCRYPT,                      /* 16 byte payload */
  EVENT_SECCRYPT_CMD,                  /* 16 byte payload */

  EVENT_SEC_RESERVED_978,              /* unknown payload */
  EVENT_SEC_RESERVED_979,              /* unknown payload */
  EVENT_SEC_RESERVED_980,              /* unknown payload */
  EVENT_SEC_RESERVED_981,              /* unknown payload */

  EVENT_ARM_CLK_FREQUENCY_CHANGE,      /* 12 byte payload */
  EVENT_ADSP_CLK_FREQUENCY_CHANGE,     /* 4 byte payload */
  EVENT_MDSP_CLK_FREQUENCY_CHANGE,     /* 4 byte payload */

  EVENT_CELL_CHANGE_INDICATION,        /* 1 byte payload */
  EVENT_CB_STATE_CHANGE,               /* 4 byte payload */
  EVENT_SMSCB_L1_STATE_CHANGE,         /* 3 byte payload */
  EVENT_SMSCB_L1_COLLISION,            /* 1 byte payload */
  EVENT_WMS_SEARCH_REQUEST,            /* 1 byte payload */
  EVENT_CM_GET_PASSWORD_IND,           /* 2 byte payload */
  EVENT_CM_PASSWORD_AUTHENTICATION_STATUS, /* 2 byte payload */
  EVENT_CM_USS_RESPONSE_NOTIFY_IND,    /* 3 byte payload */
  EVENT_CM_USS_CONF,                   /* 4 byte payload */
  EVENT_CM_RELEASE_USS_IND,            /* 4 byte payload */
  EVENT_CM_FWD_AOC_IND,                /* 1 byte payload */
  EVENT_PZID_ID,                       /* 2 byte payload */
  EVENT_PZID_HT_VALUE,                 /* 9 byte payload */
  EVENT_PZID_EXISTS_IN_LIST,           /* 1 byte payload */
  EVENT_GSDI_GET_FILE_ATTRIBUTES,      /* 6 byte payload */
  EVENT_GSDI_SIM_READ,                 /* 6 byte payload */
  EVENT_GSDI_SIM_WRITE,                /* 6 byte payload */
  EVENT_GSDI_GET_PIN_STATUS,           /* 8 byte payload */
  EVENT_GSDI_VERIFY_PIN,               /* 7 byte payload */
  EVENT_GSDI_UNBLOCK_PIN,              /* 7 byte payload */
  EVENT_GSDI_DISABLE_PIN,              /* 7 byte payload */
  EVENT_GSDI_ENABLE_PIN,               /* 7 byte payload */
  EVENT_GSDI_SIM_INCREASE,             /* 6 byte payload */
  EVENT_GSDI_EXECUTE_APDU_REQ,         /* 6 byte payload */
  EVENT_SEG_UPM_ADDR_MISMATCH,         /* 2 byte payload */
  EVENT_WCDMA_PRACH,                   /* 3 byte payload */
  EVENT_GSDI_SELECT,                   /* 6 byte payload */
  EVENT_WCDMA_RAB_RATE_RECONFIG,       /* 2 byte payload */
  EVENT_WCDMA_RLC_RESETS,              /* 3 byte payload */
  EVENT_WCDMA_RLC_OPEN_CLOSE,          /* 2 byte payload */
  EVENT_WCDMA_RLC_MRW,                 /* 3 byte payload */
  EVENT_UMTS_QVP_APP_STATE,            /* 2 byte payload */
  EVENT_UMTS_QVP_APP_STATE_CHANGED_EVENT,  /* 5 byte payload */
  EVENT_UMTS_QVP_APP_CALL_CONNECTED_EVENT, /* 1 byte payload */
  EVENT_GSDI_CARD_EVENT_NOTIFICATION,  /* 4 byte payload */
  EVENT_CM_DATA_AVAILABLE,             /* 1 byte payload */
  EVENT_CM_DS_INTERRAT_STATE,          /* 2 byte payload */

  EVENT_RSVD_START = 0x0800,

  EVENT_MAX_ID
} event_id_enum_type;

/*===========================================================================

                        TYPE DEFINITIONS FOR EVENTS

          NOTE: Type definitions for new events are now added by
                the responsible engineer in the target VU:

                (Do not add payload definitions to this file).

===========================================================================*/
typedef unsigned char event_band_class_change_type;
typedef unsigned short event_cdma_ch_change_type;
typedef unsigned char event_bs_p_rev_change_type;
typedef unsigned char event_p_rev_in_use_change_type;
typedef unsigned short event_sid_change_type;
typedef unsigned short event_nid_change_type;
typedef unsigned short event_pzid_change_type;
typedef unsigned char event_op_mode_change_type;

typedef PACKED struct
{
   unsigned char channel;
   unsigned char msg_id;
} event_message_type;

typedef unsigned char event_timer_expired_type;
typedef unsigned char event_counter_threshold_type;

typedef PACKED struct
{
   unsigned char old_state;
   unsigned char new_state;
} event_call_processing_state_change_type;

typedef unsigned char event_call_control_instantiated_type;

typedef PACKED struct
{
   int con_ref      : 8;
   int old_substate : 4;
   int new_substate : 4;
} event_call_control_state_change_type;

typedef unsigned char event_call_control_terminated_type;
typedef unsigned short event_reg_zone_change_type;
typedef unsigned char event_slotted_mode_operation_type;
typedef unsigned char event_qpch_in_use_type;
typedef unsigned short event_idle_handoff_type;
typedef unsigned short event_ms_access_handoff_type;
typedef unsigned short event_ms_access_probe_handoff_type;

typedef PACKED struct
{
  unsigned short pn[6];
  PACKED struct
  {
    int s1       : 1;
    int s2       : 1;
    int s3       : 1;
    int s4       : 1;
    int s5       : 1;
    int s6       : 1;
    int reserved : 10;
  } in_sch_aset;
} event_ms_handoff_type;

typedef unsigned long event_tmsi_assigned_type;
typedef unsigned char event_end_rev_supp_burst_assgn_type;
typedef unsigned char event_ch_ind_type;

typedef PACKED struct
{
   unsigned char pagech;
   unsigned short pn_offset;
} event_pch_acquired_type;

typedef PACKED struct
{
   unsigned char bcch;
   unsigned short pn_offset;
} event_bcch_acquired_type;

typedef unsigned char event_registration_performed_type;

typedef PACKED struct
{
   unsigned char ecio;
   unsigned char ps;
} event_system_reselection_type;

#endif /* EVENT_DEFS_H */
