#ifndef LOG_CODES_GSM_H
#define LOG_CODES_GSM_H

/*===========================================================================

                         Log Code Definitions

General Description
  This file contains log code definitions and is shared with the tools.

Copyright (c) 1991-2003 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                             Edit History

$PVCSPath:  L:/src/asw/KOVU/common/vcs/log_codes_gsm.h_v   1.9   17 Jul 2002 16:52:54   amcintos  $
$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/log_codes_gsm.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $ $DateTime: 2004/02/03 12:16:24 $ $Author:   Kevin Tai  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/30/04   gfr     Added log code for mdsp scaling factors.
01/28/04   gw      Added log codes for GSM auxiliary measurements.
12/05/03    bm      Added logging support for MDSPcommands issued, 
                    and the corresponding pops
09/08/93   ws      Added DL PTCCH message and burst metrics
08/01/03   tb      Added more GtoW log codes.
07/07/03   ws      Added LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C to 
                   LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C
03/27/03   gfr     Added LOG_GPRS_HW_SCHEDULE_C and LOG_GSM_MDSP_CMD_C
01/23/03   ATM     Added more suggested packets for systems
01/22/03   tb      added LOG_GSM_WCDMA_KNOWN_LIST_C 
11/18/02   ATM     Added enhance PDM_ADJUST and SACCH_REPORT packets
11/05/02   WS      Added GPRS L1 log codes
10/07/02   HE      Added GPRS log codes.
09/06/02   ATM     Added USER code space
07/24/02   ATM     Added NCELL_BCCH_DECODE packet
07/11/02   ATM     Added GSTMR, PDM, NCELL acq packets for systems group
07/09/02   ATM     Added GPRS log code range
05/08/02   ATM     Split Rx_Metrics into Message/Burst packets
04/30/02   ATM     Added SERVING_CELL_POWER packet
04/17/02   ATM     Added RECEVIE_TRAFFIC_METRICS packet
04/02/02   ATM     Added CSDATA packets
03/29/02   ATM     Added NCELL packets
03/08/02   ATM     Added jachan's L2 packets and renamed some RR packets
02/04/02   ATM     Cleanup -- renamed codes to fit convention
01/24/02   ATM     Created log codes used by GSM

===========================================================================*/

#include "log_codes.h" // for LOG_GSM_BASE_C

/* ======================================================================== */
// GSM general log codes
#define LOG_GSM_PLT_PKT_C                             (  0  + LOG_GSM_BASE_C)
#define LOG_GSM_OBSOLETE_CFA_C                        (  1  + LOG_GSM_BASE_C)

/* ========================   GSM LAYER1 PACKETS   ======================== */
#define LOG_GSM_POWER_SCAN_C                          (100  + LOG_GSM_BASE_C) /* 0x5064 */
#define LOG_GSM_FCCH_DECODE_C                         (101  + LOG_GSM_BASE_C)
#define LOG_GSM_SCH_DECODE_C                          (102  + LOG_GSM_BASE_C)
#define LOG_GSM_AGC_PARAMS_C                          (103  + LOG_GSM_BASE_C)
#define LOG_GSM_AFC_PARAMS_C                          (104  + LOG_GSM_BASE_C)
#define LOG_GSM_TA_DATA_C                             (105  + LOG_GSM_BASE_C)
#define LOG_GSM_PC_DATA_C                             (106  + LOG_GSM_BASE_C)
#define LOG_GSM_RECEIVE_MSG_METRICS_C                 (107  + LOG_GSM_BASE_C)
#define LOG_GSM_RECEIVE_BURST_METRICS_C               (108  + LOG_GSM_BASE_C)
#define LOG_GSM_MEASUREMENT_REPORT_C                  (109  + LOG_GSM_BASE_C)
#define LOG_GSM_SURROUNDING_CELL_INFO_C               (110  + LOG_GSM_BASE_C)
#define LOG_GSM_PAGING_DATA_C                         (111  + LOG_GSM_BASE_C)
#define LOG_GSM_NCELL_ENGINE_MODE_C                   (112  + LOG_GSM_BASE_C)
#define LOG_GSM_NCELL_BA_LIST_C                       (113  + LOG_GSM_BASE_C)
#define LOG_GSM_SERVING_CELL_POWER_C                  (114  + LOG_GSM_BASE_C)

/* QUICK PACKETS FOR SYSTEMS 07/11/2002 */
#define LOG_GSM_GSTMR_ADJ_C                           (115  + LOG_GSM_BASE_C)
#define LOG_GSM_PDM_ADJ_C                             (116  + LOG_GSM_BASE_C) /* obsoleted by 120 */
#define LOG_GSM_NCELL_ACQ_C                           (117  + LOG_GSM_BASE_C)
#define LOG_GSM_TXLEV_TA_C                            (118  + LOG_GSM_BASE_C)
#define LOG_GSM_NCELL_BCCH_DECODE_C                   (119  + LOG_GSM_BASE_C)
#define LOG_GSM_PDM_ADJUST_C                          (120  + LOG_GSM_BASE_C)

/* New Message Metrics w/ both hard and soft RxQual */
#define LOG_GSM_RECEIVE_MSG_METRICS_ENHANCED_C        (121  + LOG_GSM_BASE_C)

/* Packets for auxiliary L1 meausrement reports */
#define LOG_GSM_SERVING_AUX_MEAS_C                    (122  + LOG_GSM_BASE_C)
#define LOG_GSM_NCELL_AUX_MEAS_C                      (123  + LOG_GSM_BASE_C)

/* QUICK PACKETS2 FOR SYSTEMS 12/23/2002 */
#define LOG_GSM_MON_BURST_C                           (130  + LOG_GSM_BASE_C)
#define LOG_GSM_SCH_BURST_METRICS_C                   (131  + LOG_GSM_BASE_C)
#define LOG_GSM_SCH_MSG_METRICS_C                     (132  + LOG_GSM_BASE_C)
#define LOG_GSM_BCCH_BURST_METRICS_C                  (133  + LOG_GSM_BASE_C)
#define LOG_GSM_BCCH_MSG_METRICS_C                    (134  + LOG_GSM_BASE_C)

/* MDSP command log */
#define LOG_GSM_MDSP_CMD_C                            (140  + LOG_GSM_BASE_C) /* 0x523A */
#define LOG_GSM_GL1_HW_CMD_C                          (141  + LOG_GSM_BASE_C)
#define LOG_GSM_MDSP_SCALING_FACTORS_C                (142  + LOG_GSM_BASE_C)

/* ========================   GSM LAYER2 PACKETS   ======================== */
#define LOG_GSM_L2_STATE_C                            (200  + LOG_GSM_BASE_C) /* 0x50C8 */
#define LOG_GSM_L2_TRANSMISSION_STATUS_C              (201  + LOG_GSM_BASE_C) 
#define LOG_GSM_L2_OUTSTANDING_FRAME_C                (202  + LOG_GSM_BASE_C) 

/* ========================   GSM LAYER3 PACKETS   ======================== */
#define LOG_GSM_RR_STATE_C                            (300  + LOG_GSM_BASE_C) /* 0x512C */
#define LOG_GSM_RR_LINK_FAILURE_DETECTION_C           (301  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_PROTOCOL_ERROR_C                   (302  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_SIGNALING_MESSAGE_C                (303  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_CELL_RESELECTION_PARAMS_C          (304  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_RACH_CONTROL_PARAMS_C              (305  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_CONTROL_CHANNEL_PARAMS_C           (306  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_CELL_OPTIONS_C                     (307  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_CELL_INFO_C                        (308  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_CHANNEL_CONFIGURATION_C            (309  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_CIPHERING_MODE_C                   (310  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_CELL_RESELECTION_MEASMNTS_C        (311  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_DSC_COUNTER_C                      (312  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_S_COUNTER_C                        (313  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_SACCH_RPT_C                        (314  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_AVAILABLE_PLMN_LIST_C              (315  + LOG_GSM_BASE_C)
#define LOG_GSM_RR_BA_LIST_C                          (316  + LOG_GSM_BASE_C)

/* ========================   GSM CS DATA PACKETS  ======================== */
#define LOG_GSM_CS_DATA_RX_C                          (400  + LOG_GSM_BASE_C) /* 0x5190 */
#define LOG_GSM_CS_DATA_TX_C                          (401  + LOG_GSM_BASE_C)

/* ===========================   GPRS PACKETS  ============================ */
#define LOG_GPRS_INDICATORS_C                         (500  + LOG_GSM_BASE_C) /* 0x51F4 */
#define LOG_GPRS_GENERAL_PARAMS_C                     (501  + LOG_GSM_BASE_C) /* 0x51F5 */
#define LOG_GPRS_CELL_OPTIONS_C                       (502  + LOG_GSM_BASE_C) /* 0x51F6 */
#define LOG_GPRS_POWER_CONTROL_PARAMS_C               (503  + LOG_GSM_BASE_C) /* 0x51F7 */
#define LOG_GPRS_MOBILE_ALLOCATION_C                  (504  + LOG_GSM_BASE_C) /* 0x51F8 */
#define LOG_GPRS_PBCCH_DESCRIPTION_C                  (505  + LOG_GSM_BASE_C) /* 0x51F9 */
#define LOG_GPRS_GRR_STATE_C                          (506  + LOG_GSM_BASE_C) /* 0x51FA */
#define LOG_GPRS_RR_CELL_RESELECTION_PARAMS_C         (507  + LOG_GSM_BASE_C) /* 0x51FB */
#define LOG_GPRS_RR_CELL_RESELECTION_MEASMNTS_C       (508  + LOG_GSM_BASE_C) /* 0x51FC */
#define LOG_GPRS_RR_PSI1_C                            (509  + LOG_GSM_BASE_C) /* 0x51FD */
#define LOG_GPRS_RR_PSI2_C                            (510  + LOG_GSM_BASE_C) /* 0x51FE */
#define LOG_GPRS_RR_PSI3_C                            (511  + LOG_GSM_BASE_C) /* 0x51FF */

#define LOG_GPRS_RLC_UL_ABNRML_RLS_COUNTS_C           (512  + LOG_GSM_BASE_C) /* 0x5200 */
#define LOG_GPRS_RLC_UL_EVENT_COUNTS_C                (513  + LOG_GSM_BASE_C) /* 0x5201 */
#define LOG_GPRS_RLC_UL_STATS_C                       (514  + LOG_GSM_BASE_C) /* 0x5202 */
#define LOG_GPRS_RLC_DL_STATS_C                       (522  + LOG_GSM_BASE_C) /* 0x520A */
#define LOG_GPRS_RLC_UL_ACKNACK_PARAMS_C              (523  + LOG_GSM_BASE_C) /* 0x520B */
#define LOG_GPRS_RLC_DL_ACKNACK_PARAMS_C              (524  + LOG_GSM_BASE_C) /* 0x520C */
#define LOG_GPRS_RLC_UL_RELEASE_IND_C                 (525  + LOG_GSM_BASE_C) /* 0x520D */
#define LOG_GPRS_RLC_DL_RELEASE_IND_C                 (526  + LOG_GSM_BASE_C) /* 0x520E */
#define LOG_GPRS_RLC_UL_ACKNACK_PARAMS_VER2_C         (527  + LOG_GSM_BASE_C) /* 0x520F */
#define LOG_GPRS_RLC_DL_ACKNACK_PARAMS_VER2_C         (528  + LOG_GSM_BASE_C) /* 0x5210 */



#define LOG_GPRS_LLC_ME_INFO_C                        (530  + LOG_GSM_BASE_C) /* 0x5212 */
#define LOG_GPRS_LLC_SAPI_STATES_C                    (531  + LOG_GSM_BASE_C) /* 0x5213 */
#define LOG_GPRS_LLC_XID_INFO_C                       (532  + LOG_GSM_BASE_C) /* 0x5214 */
#define LOG_GPRS_LLC_PDU_STATS_C                      (533  + LOG_GSM_BASE_C) /* 0x5215 */
#define LOG_GPRS_LLC_PERIODIC_STATS_C                 (534  + LOG_GSM_BASE_C) /* 0x5216 */
#define LOG_GPRS_LLC_READY_TIMER_STATUS_C             (535  + LOG_GSM_BASE_C) /* 0x5217 */
#define LOG_GPRS_LLC_SEND_UI_FRAME_C                  (536  + LOG_GSM_BASE_C) /* 0x5218 */
#define LOG_GPRS_LLC_RECV_UI_FRAME_C                  (537  + LOG_GSM_BASE_C) /* 0x5219 */
#define LOG_GPRS_LLC_READY_TIMER_STATUS_VER2_C        (538  + LOG_GSM_BASE_C) /* 0x521A */


#define LOG_GPRS_SNDCP_ACTIVE_PDP_CONTEXTS_C          (540  + LOG_GSM_BASE_C) /* 0x521C */
#define LOG_GPRS_SNDCP_PDP_CONTEXT_INFO_C             (541  + LOG_GSM_BASE_C) /* 0x521D */
#define LOG_GPRS_SNDCP_STATES_C                       (542  + LOG_GSM_BASE_C) /* 0x521E */
#define LOG_GPRS_SNDCP_PDU_STATS_C                    (543  + LOG_GSM_BASE_C) /* 0x521F */
#define LOG_GPRS_SNDCP_UL_TCP_HDR_C                   (544  + LOG_GSM_BASE_C) /* 0x5220 */
#define LOG_GPRS_SNDCP_DL_TCP_HDR_C                   (545  + LOG_GSM_BASE_C) /* 0x5221 */

#define LOG_GPRS_MAC_SIGNALLING_MESSAGE_C             (550  + LOG_GSM_BASE_C) /* 0x5226 */
#define LOG_GPRS_MAC_STATE_C                          (551  + LOG_GSM_BASE_C) /* 0x5227 */
#define LOG_GPRS_MAC_UL_TBF_ESTABLISH_C               (552  + LOG_GSM_BASE_C) /* 0x5228 */
#define LOG_GPRS_MAC_UL_TBF_RELEASE_C                 (553  + LOG_GSM_BASE_C) /* 0x5229 */
#define LOG_GPRS_MAC_DL_TBF_ESTABLISH_C               (554  + LOG_GSM_BASE_C) /* 0x522A */
#define LOG_GPRS_MAC_DL_TBF_RELEASE_C                 (555  + LOG_GSM_BASE_C) /* 0x522B */

#define LOG_GPRS_SM_GMM_OTA_MESSAGE_C                 (560  + LOG_GSM_BASE_C) /* 0x5230 */
#define LOG_GPRS_AIR_INTERFACE_SUMMARY_C              (561  + LOG_GSM_BASE_C) /* 0x5231 */
#define LOG_GPRS_AGC_C                                (562  + LOG_GSM_BASE_C) /* 0x5232 */
#define LOG_GPRS_TIMING_ADVANCE_C                     (563  + LOG_GSM_BASE_C) /* 0x5233 */
#define LOG_GPRS_POWER_CONTROL_C                      (564  + LOG_GSM_BASE_C) /* 0x5234 */
#define LOG_GPRS_TRANSFER_SUMMARY_C                   (565  + LOG_GSM_BASE_C) /* 0x5235 */
#define LOG_GPRS_TX_SCHEDULED_RESULTS_C               (567  + LOG_GSM_BASE_C) /* 0x5237 */

#define LOG_GPRS_HW_SCHEDULE_C                        (570  + LOG_GSM_BASE_C) /* 0x523A */

#define LOG_GPRS_RECEIVE_BURST_METRICS_A_C            (581  + LOG_GSM_BASE_C) /* 0x5245 */
#define LOG_GPRS_RECEIVE_BURST_METRICS_B_C            (582  + LOG_GSM_BASE_C) /* 0x5246 */
#define LOG_GPRS_RECEIVE_BURST_METRICS_C_C            (583  + LOG_GSM_BASE_C) /* 0x5247 */
#define LOG_GPRS_RECEIVE_BURST_METRICS_D_C            (584  + LOG_GSM_BASE_C) /* 0x5248 */
#define LOG_GPRS_RECEIVE_MSG_METRICS_A_C              (585  + LOG_GSM_BASE_C) /* 0x5249 */
#define LOG_GPRS_RECEIVE_MSG_METRICS_B_C              (586  + LOG_GSM_BASE_C) /* 0x524A */
#define LOG_GPRS_RECEIVE_MSG_METRICS_C_C              (587  + LOG_GSM_BASE_C) /* 0x524B */
#define LOG_GPRS_RECEIVE_MSG_METRICS_D_C              (588  + LOG_GSM_BASE_C) /* 0x524C */
#define LOG_GPRS_RECEIVE_MSG_METRICS_A_C              (585  + LOG_GSM_BASE_C) /* 0x5249 */
#define LOG_GPRS_RECEIVE_MSG_METRICS_B_C              (586  + LOG_GSM_BASE_C) /* 0x524A */
#define LOG_GPRS_RECEIVE_MSG_METRICS_C_C              (587  + LOG_GSM_BASE_C) /* 0x524B */
#define LOG_GPRS_RECEIVE_MSG_METRICS_D_C              (588  + LOG_GSM_BASE_C) /* 0x524C */
#define LOG_GPRS_RECEIVE_MSG_METRICS_A_VER2_C         (589  + LOG_GSM_BASE_C) /* 0x524D */
#define LOG_GPRS_RECEIVE_MSG_METRICS_B_VER2_C         (590  + LOG_GSM_BASE_C) /* 0x524E */
#define LOG_GPRS_RECEIVE_MSG_METRICS_C_VER2_C         (591  + LOG_GSM_BASE_C) /* 0x524F */
#define LOG_GPRS_RECEIVE_MSG_METRICS_D_VER2_C         (592  + LOG_GSM_BASE_C) /* 0x5250 */
#define LOG_GPRS_DLPTCCH_BURST_METRICS_C              (593  + LOG_GSM_BASE_C) /* 0x5251 */
#define LOG_GPRS_DLPTCCH_MSG_METRICS_C                (594  + LOG_GSM_BASE_C) /* 0x5252 */

#define LOG_GPRS_LAST_C                               LOG_GPRS_RECEIVE_MSG_METRICS_D_C


/* ==============   GSM MODE WCDMA MEASUREMENT PACKETS   ================== */
#define LOG_GSM_WCDMA_KNOWN_LIST_C                   (600 + LOG_GSM_BASE_C)
#define LOG_GSM_WCDMA_FILL_SAMPLE_RAM_C              (601 + LOG_GSM_BASE_C)
#define LOG_GSM_WCDMA_DED_CELL_LIST_C                (602 + LOG_GSM_BASE_C)

/* ==============   GSM MODE WCDMA MEASUREMEMTS REPORT FROM RR ============ */
#define LOG_GSM_DED_3G_MEASUREMENTS_C                (603 + LOG_GSM_BASE_C)

/* ============== GSM to WCDMA Reselection Rejected at WCDMA ============= */
#define LOG_GSM_IDLE_MODE_3G_RESELECT_REJ_C          (604 + LOG_GSM_BASE_C)

/* ==============   GSM/GPRS INTERRAT PACKETS            ================== */
#define LOG_GPRS_RR_3G_MEASUREMENT_PARAMS_C           (610  + LOG_GSM_BASE_C) /* 0x5262 */
#define LOG_GPRS_RR_3G_RESELECTION_MEASUREMENTS_C     (611  + LOG_GSM_BASE_C) /* 0x5263 */


/* ========================       NAS PACKETS      ======================== */
/* ========================       AMR PACKETS      ======================== */
/* These codes are defined in log_codes_umts.h */

/* ======================================================================== */
// User defined codes (starting at bit 1024, or 0x5400, incrementing by 0x10)
#define LOG_GSM_FN_ADJ_C                             (0x400 + LOG_GSM_BASE_C)
#define LOG_GSM_SACCH_RPT_C                          (0x410 + LOG_GSM_BASE_C) /* formalized as 314 */
#define LOG_GSM_NEXT_USR_CODE_C                      (0x420 + LOG_GSM_BASE_C)

/* ======================================================================== */
// Last GSM code
// - Must be beyond the GPRS range or GPRS logging will not work
#define LOG_GSM_LAST_C                               LOG_GSM_NEXT_USR_CODE_C

#endif /* LOG_CODES_GSM_H */
