#ifndef LOG_CODES_UMTS_H
#define LOG_CODES_UMTS_H

/*===========================================================================

                         Log Code Definitions

General Description
  This file contains log code definitions and is shared with the tools.

Copyright (c) 1991-2001 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                             Edit History

$PVCSPath:  L:/src/asw/MSM5200/common/vcs/log_codes_umts.h_v   1.2   12 Jul 2002 15:27:32   gchhabra  $
$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/log_codes_umts.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $ $DateTime: 2003/12/18 10:00:51 $ $Author:   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/12/03  KA       Added log packets for call manager's use (USSD, passwd)
10/07/02   ATM     Moving CFA filtering from subsys cmd to log code range
07/12/02   gsc     Merged MMGPS changes by Ricky Yuen.
02/04/02   ATM     Cleanup -- renamed codes to fit convention
01/24/02   ATM     Created log codes used by UMTS

===========================================================================*/

#include "log_codes.h" // for LOG_UMTS_BASE_C

/* NOTE: Steve is planning on taking all the UMTS/GSM code definitions out */
/*       of log_codes.h. Since this address space will be controlled by    */
/*       this VU, let's define the codes here                              */

/* ========================   Call flow analysis  ======================== */
#define LOG_UMTS_CALL_FLOW_ANALYSIS_C               (0x01  + LOG_UMTS_BASE_C)

/* ========================   MMGPS PACKETS   ============================ */
#define LOG_UMTS_MMGPS_REP_CLK_LOG_PACKET_C         (0x02  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_POS_LOG_PACKET_C         (0x03  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_MEAS_LOG_PACKET_C        (0x05  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_RESERVED0_LOG_PACKET_C   (0x07  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_RF_STATUS_LOG_PACKET_C   (0x08  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_FREQ_CALIBRATION_LOG_PACKET_C \
                                                    (0x09  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_SVR_TX_LOG_PACKET_C      (0x0A  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_SVR_RX_LOG_PACKET_C      (0x0B  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_SV_MEAS_LOG_PACKET_C     (0x0C  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_DWELL_LOG_PACKET_C       (0x0D  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_TIME_TAG_LOG_PACKET_C    (0x0E  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_RESERVED1_LOG_PACKET_C   (0x0F  + LOG_UMTS_BASE_C)

#define LOG_UMTS_MMGPS_REP_GPS_INTFC_TX_LOG_PACKET_C (0x10  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_GPS_INTFC_RX_LOG_PACKET_C (0x11  + LOG_UMTS_BASE_C)
#define LOG_UMTS_MMGPS_REP_EVT_LOG_PACKET_C          (0x12  + LOG_UMTS_BASE_C)


/* ========================   NAS LAYER PACKETS   ======================== */
#define LOG_UMTS_NAS_GMM_STATE_LOG_PACKET_C         (0x130 + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_MM_STATE_LOG_PACKET_C          (0x131 + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_REG_LOG_PACKET_C               (0x132 + LOG_UMTS_BASE_C)
#define LOG_UMTS_CS_CALL_SETUP_INFO_LOG_PACKET_C    (0x133 + LOG_UMTS_BASE_C)
#define LOG_UMTS_PS_CALL_INFO_LOG_PACKET_C          (0x134 + LOG_UMTS_BASE_C)
#define LOG_UMTS_MM_INFO_LOG_PACKET_C               (0x135 + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_PS_CONNECTION_QOS_LOG_PACKET_C (0x136 + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_CS_CONNECTION_BC_LOG_PACKET_C  (0x137 + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_UE_DYNAMIC_ID_LOG_PACKET_C     (0x138 + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_UE_STATIC_ID_LOG_PACKET_C      (0x139 + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_OTA_MESSAGE_LOG_PACKET_C       (0x13A + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_CFA_MESSAGE_LOG_PACKET_C       (0x13B + LOG_UMTS_BASE_C)
#define LOG_UMTS_NAS_ERROR_MESSAGE_LOG_PACKET_C     (0x13C + LOG_UMTS_BASE_C)
#define LOG_UMTS_CS_CALL_RELEASE_INFO_LOG_PACKET_C  (0x13D + LOG_UMTS_BASE_C)
#define LOG_UMTS_CS_CALL_CHANGE_INFO_LOG_PACKET_C   (0x13E + LOG_UMTS_BASE_C)

/* ========================      AMR PACKETS     ========================= */
#define LOG_UMTS_TX_VOCODER_PACKET_C                (0x13F + LOG_UMTS_BASE_C)
#define LOG_UMTS_RX_VOCODER_PACKET_C                (0x140 + LOG_UMTS_BASE_C)

/* ========================  CFA FILTER PACKETS  ========================= */
#define LOG_UMTS_CFA_FIRST_FILTER_C                 (0x200 + LOG_UMTS_BASE_C)
#define LOG_UMTS_CFA_LAST_FILTER_C                  (0x2FF + LOG_UMTS_BASE_C)

/* ========================  CM LOG PACKETS  ========================= */
#define LOG_UMTS_RESPONSE_NOTIFY                    (0x401 + LOG_UMTS_BASE_C)
#define LOG_UMTS_USS_DATA_CONF                      (0x402 + LOG_UMTS_BASE_C)
#define LOG_UMTS_USS_FWD_AOC                        (0x403 + LOG_UMTS_BASE_C)
#define LOG_GET_CALL_INFO                           (0x404 + LOG_UMTS_BASE_C)

/* The last defined UMTS log code */
#define LOG_UMTS_LAST_C                             (0x4FF + LOG_UMTS_BASE_C)

#endif /* LOG_CODES_UMTS_H */
