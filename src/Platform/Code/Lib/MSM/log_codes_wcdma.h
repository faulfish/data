#ifndef LOG_CODES_WCDMA_H
#define LOG_CODES_WCDMA_H

/*===========================================================================

                         Log Code Definitions

General Description
  This file contains log code definitions and is shared with the tools.

Copyright (c) 1991-2001 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                             Edit History

$PVCSPath:  L:/src/asw/MSM5200/common/vcs/log_codes_wcdma.h_v   1.3   28 Jun 2002 14:31:04   tliou  $
$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/log_codes_wcdma.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $ $DateTime: 2003/08/11 20:04:27 $ $Author:   Kevin Tai  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/11/03   asr     Changed LOG_WCDMA_DRX_MODE_C to be offset by 0x004 instead of 0x107
                   from LOG_WCDMA_BASE_C. 
06/09/03   jac     Added QVPHONE log packets
05/08/03   gsc     Added LOG_WCDMA_L1_CM_BLER_C.
12/11/02   gsc     Synced up with Rev. K of the Serial Interface Control
                   Document for W-CDMA (80-V2708-1). 
09/04/02   gsc     Synced up with Rev. H of the Serial Interface Control 
                   Document for W-CDMA (80-V2708-1). 
06/28/02   ttl     Added LOG_WCDMA_DL_AM_PDU, LOG_WCDMA_UL_NAK_AM_PDU and
                   LOG_WCDMA_DL_NAK_AM_PDU.
                   Changed LOG_WCDMA_UL_AM_STATISTIC, LOG_WCDMA_DL_AM_STATISTIC,
                   LOG_WCDMA_UL_AM_PDU and LOG_WCDMA_LAST_C.
05/17/02   gsc     Synced up with Rev. E of the Serial Interface Control 
                   Document for W-CDMA (80-V2708-1). 
04/23/02   ttl     Added LOG_WCDMA_UL_AM_STATISTIC, LOG_WCDMA_DL_AM_STATISTIC
                   and LOG_WCDMA_UL_AM_PDU.
02/12/02   ATM     Created log codes used by WCDMA

===========================================================================*/

#include "log_codes.h" // for LOG_WCDMA_BASE_C

/* NOTE: Steve is planning on taking all the WCDMA/GSM code definitions out */
/*       of log_codes.h. Since this address space will be controlled by    */
/*       this VU, let's define the codes here                              */

/* ========================   LAYER 1 PACKETS   ======================== */
#define LOG_WCDMA_L1_POWER_CONTROL_WITH_CM_C         (0x001 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_100_C                     (0x100 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_101_C                     (0x101 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_102_C                     (0x102 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_103_C                     (0x103 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_104_C                     (0x104 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_AGC_C                              (0x105 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RACH_PARAMETERS_C                  (0x106 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DRX_MODE_C                         (0x004 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_TRANSPORT_DL_C                     (0x108 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_TRANSPORT_UL_C                     (0x109 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_COMMON_CHANNEL_DL_C                (0x10A + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DEDICATED_CHANNEL_DL_C             (0x10B + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_PHYSICAL_CHANNEL_UL_C              (0x10C + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_PRACH_C                            (0x10D + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RM_UL_C                            (0x10E + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_10F_C                     (0x10F + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_ACTIVE_SET_C                       (0x110 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_NEIGHBOR_SET_C                     (0x111 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DL_TRANSPORT_DEBUGING_C            (0x112 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_CRC_C                              (0x113 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_114_C                     (0x114 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DL_FORMAT_COMB_C                   (0x115 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_BLER_C                             (0x116 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_FINGER_TA_INFO_C                   (0x147 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_148_C                     (0x148 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_STEP_1_SEARCH_C                    (0x14C + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_STEP_2_SEARCH_C                    (0x14D + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_STEP_3_SEARCH_C                    (0x14E + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_LIST_SEARCH_C                      (0x14F + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_UL_DL_POWER_CONTROL_C              (0x150 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_L1_CIPHERING_C                     (0x153 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_L1_COMPACT_CRC_C                   (0x760 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_L1_TFCI_C                          (0x770 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_L1_CM_BLER_C                       (0x775 + LOG_WCDMA_BASE_C)

/* ========================   LAYER 2 PACKETS   ======================== */
#define LOG_WCDMA_UL_MAC_PARAMS_C                    (0x117 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_UL_MAC_TRAFFIC_VOL_MEAS_RPT_C      (0x002 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_118_C                     (0x118 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_UL_STATES_C                    (0x119 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_UL_TM_C                        (0x11A + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_UL_UM_C                        (0x11B + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_UL_AM_C                        (0x11C + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DL_MAC_PARAMS_C                    (0x11D + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_11E_C                     (0x11E + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_DL_STATES_C                    (0x11F + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_DL_TM_C                        (0x120 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_DL_UM_C                        (0x121 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_DL_AM_C                        (0x122 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RACH_CONTROL_PARAMS_C              (0x123 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_UL_FORMAT_COMB_C                   (0x124 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DL_AM_PDU                          (0x144 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_UL_NAK_AM_PDU                      (0x145 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DL_NAK_AM_PDU                      (0x146 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_UL_AM_STATISTIC                    (0x149 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DL_AM_STATISTIC                    (0x14A + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_UL_AM_PDU                          (0x14B + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_UL_CIPHERING_C                 (0x151 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RLC_DL_CIPHERING_C                 (0x152 + LOG_WCDMA_BASE_C)

/* ========================   LAYER 3 PACKETS   ======================== */
#define LOG_WCDMA_RRC_STATES_C                       (0x125 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RRC_PROTOCOL_ERRORS_C              (0x126 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_CELL_ID_C                          (0x127 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_UL_CHANNEL_MAPPING_C               (0x128 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_DL_CHANNEL_MAPPING_C               (0x129 + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_RESERVED_12A_C                     (0x12A + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_SIB_C                              (0x12B + LOG_WCDMA_BASE_C)
#define LOG_WCDMA_SIGNALING_MSG_C                    (0x12F + LOG_WCDMA_BASE_C)

/* ========================     NAS PACKETS    ========================= */
/* ========================     AMR PACKETS    ========================= */
/* These codes are defined in log_codes_umts.h */

#ifdef FEATURE_QVPHONE

#define LOG_H324_DL_223_PDU                          (0x900 + LOG_WCDMA_BASE_C) /* 0x4900 H324 Downlink 223 MUX PDU */
#define LOG_H324_DL_AUD_SDU                          (0x901 + LOG_WCDMA_BASE_C) /* 0x4901 H324 Downlink Audio SDU */
#define LOG_H324_DL_VID_SDU                          (0x902 + LOG_WCDMA_BASE_C) /* 0x4902 H324 Downlink Video SDU*/
#define LOG_H324_DL_SIG_SDU                          (0x903 + LOG_WCDMA_BASE_C) /* 0x4903 H324 Downlink 245 SDU */
#define LOG_H324_UL_223_PDU                          (0x904 + LOG_WCDMA_BASE_C) /* 0x4904 H324 Uplink 223 MUX PDU */
#define LOG_H324_UL_AUD_SDU                          (0x905 + LOG_WCDMA_BASE_C) /* 0x4905 H324 Uplink Audio SDU */
#define LOG_H324_UL_VID_SDU                          (0x906 + LOG_WCDMA_BASE_C) /* 0x4906 H324 Uplink Video SDU */
#define LOG_H324_UL_SIG_SDU                          (0x907 + LOG_WCDMA_BASE_C) /* 0x4907 H324 Uplink Video SDU */
#define LOG_H324_UL_LL_SIG                           (0x908 + LOG_WCDMA_BASE_C) /* 0x4908 H324 Uplink Adaptation Layer */
#define LOG_H324_DL_LL_SIG                           (0x909 + LOG_WCDMA_BASE_C) /* 0x4909 H324 Downlink Adaptation Layer */

/* The last defined WCDMA log code */
#define LOG_WCDMA_LAST_C                             (0x909 + LOG_WCDMA_BASE_C)

#else

/* The last defined WCDMA log code */
#define LOG_WCDMA_LAST_C                             (0x775 + LOG_WCDMA_BASE_C)

#endif /* FEATURE_QVP_LOGGING */

#endif /* LOG_CODES_WCDMA_H */
