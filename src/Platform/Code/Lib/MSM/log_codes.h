#ifndef LOG_CODES_H
#define LOG_CODES_H

/*===========================================================================

                         Log Code Definitions

General Description
  This file contains log code definitions and is shared with the tools.

Copyright (c) 1991-2001 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                             Edit History

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/log_codes.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $
   
when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/12/03   wd      Added FTM Logs code.
09/13/02    km     Fixed IRAM build compiler error for LOG_1X_LAST_C defn.
11/02/01   sfh     Featurize common NAS log codes for UMTS.
10/30/01   sfh     Added log code for LOG_GPS_FATPATH_INFO_C.
10/24/01   sfh     Added updates for UMTS equipment ID and log codes.
06/27/01   lad     Added multiple equipment ID support.
05/22/01   sfh     Reserved log codes 158 - 168.
05/21/01   sfh     Keep confusing XXX_BASE_C names for backwards compatibility.
05/16/01   sfh     Reserved log code 155.
05/08/01   sfh     Reserved log codes 150 - 154.
04/06/01   lad     Added definitions of base IDs (such as LOG_WCDMA_BASE_C).
                   This is currently using temporary ID values in the 0x1000 
                   range.
02/23/01   lad     Created file from DMSS log.h.  Log codes only

===========================================================================*/

#include "comdef.h"

/* -------------------------------------------------------------------------
 * Data Declarations
 * ------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------
 * Log equipment IDs.
 * The most significant 4 bits of the 16 bit log code is the equipment ID.
 * Orignally, the mobile was to have an ID, and base stations and other 
 * IDs.  As QCT technology diversifies, new equipment IDs are assigned to new
 * technology areas.  0x2000 and 0x3000 are reserved for legacy reasons, so 
 * the first 
 * addition starts at 0x4000.
 * ------------------------------------------------------------------------- */

#define LOG_1X_BASE_C    ((uint16) 0x1000)
#define LOG_WCDMA_BASE_C ((uint16) 0x4000)
#define LOG_GSM_BASE_C   ((uint16) 0x5000)
#define LOG_LBS_BASE_C   ((uint16) 0x6000)
#define LOG_UMTS_BASE_C  ((uint16) 0x7000)
#define LOG_TDMA_BASE_C  ((uint16) 0x8000)

#define LOG_TOOLS_BASE_C ((uint16) 0xF000)

/* LOG_BASE_C is what was used before expanding the use of the equipment ID. 
 * TODO: Once all targets are using the "core" diag system, this should be
 * ommitted. */
#define LOG_BASE_C LOG_1X_BASE_C 

/* -------------------------------------------------------------------------
 * Log Codes
 *   These codes identify the kind of information contained in a log entry. 
 *   They are used in conjunction with the 'code' field of the log entry 
 *   header.  The data types associated with each code are defined below.
 * ------------------------------------------------------------------------- */

/* The upper 4 bits of the 16 bit log entry code specify which type
 * of equipment created the log entry. */

/* 0 Mobile Station temporal analyzer entry */
#define LOG_TA_C                                       (0x0 + LOG_1X_BASE_C)

/* 1 AGC values and closed loop power control entry */
#define LOG_AGC_PCTL_C                                 (0x1 + LOG_1X_BASE_C)

/* 2 Forward link frame rates and types entry */
#define LOG_F_MUX1_C                                   (0x2 + LOG_1X_BASE_C)

/* 3 Reverse link frame rates and types entry */
#define LOG_R_MUX1_C                                   (0x3 + LOG_1X_BASE_C)

/* 4 Access channel message entry */
#define LOG_AC_MSG_C                                   (0x4 + LOG_1X_BASE_C)

/* 5 Reverse link traffic channel message entry */
#define LOG_R_TC_MSG_C                                 (0x5 + LOG_1X_BASE_C)
                               
/* 6 Sync channel message entry */
#define LOG_SC_MSG_C                                   (0x6 + LOG_1X_BASE_C)

/* 7 Paging channel message entry */ 
#define LOG_PC_MSG_C                                   (0x7 + LOG_1X_BASE_C)

/* 8 Forward link traffic channel message entry */
#define LOG_F_TC_MSG_C                                 (0x8 + LOG_1X_BASE_C)

/* 9 Forward link vocoder packet entry */
#define LOG_VOC_FOR_C                                  (0x9 + LOG_1X_BASE_C)

/* 10 Reverse link vocoder packet entry */
#define LOG_VOC_REV_C                                  (0xA + LOG_1X_BASE_C)

/* 11 Temporal analyzer finger info only */
#define LOG_FING_C                                     (0xB + LOG_1X_BASE_C)

/* 12 Searcher pathlog info (Reused old SRCH logtype) */
#define LOG_SRCH_C                                     (0xC + LOG_1X_BASE_C)

/* 13 Position and speed information read from ETAK */
#define LOG_ETAK_C                                     (0xD + LOG_1X_BASE_C)

/* 14 Markov frame statistics */
#define LOG_MAR_C                                      (0xE + LOG_1X_BASE_C)

/* 15 New and improved temporal analyzer searcher info */
#define LOG_SRCH2_C                                    (0xF + LOG_1X_BASE_C)

/* 16 The Fujitsu handset information */
#define LOG_HANDSET_C                                  (0x10 + LOG_1X_BASE_C)

/* 17 Vocoder bit error rate mask */
#define LOG_ERRMASK_C                                  (0x11 + LOG_1X_BASE_C)

/* 18 Analog voice channel information */
#define LOG_ANALOG_INFO_C                              (0x12 + LOG_1X_BASE_C)

/* 19 Access probe information */
#define LOG_ACC_INFO_C                                 (0x13 + LOG_1X_BASE_C)

/* 20 Position & speed info read from GPS receiver */
#define LOG_GPS_C                                      (0x14 + LOG_1X_BASE_C)

/* 21 Test Command information */
#define LOG_TEST_CMD_C                                 (0x15 + LOG_1X_BASE_C)

/* 22 Sparse (20ms) AGC / closed loop power control entry */
#define LOG_S_AGC_PCTL_C                               (0x16 + LOG_1X_BASE_C)
  
/* 23 Notification of a band class change */
#define LOG_BAND_CHANGE_C                              (0x17 + LOG_1X_BASE_C)

/* 24 DM debug messages, if being logged via log services */
#define LOG_DBG_MSG_C                                  (0x18 + LOG_1X_BASE_C)

/* 25 General temporal analyzer entry */
#define LOG_GENRL_TA_C                                 (0x19 + LOG_1X_BASE_C)

/* 26 General temporal analyzer w/supplemental channels */
#define LOG_GENRL_TA_SUP_CH_C                          (0x1A + LOG_1X_BASE_C)

#ifdef FEATURE_PLT
#error code not present
#else
/* 27 EFS Usage Info - No implementation as yet */
#define LOG_EFS_INFO_C                                 (0x1B + LOG_1X_BASE_C)
#endif

/* 28 Analog Forward Channel */
#define LOG_ANALOG_FORW_C                              (0x1C + LOG_1X_BASE_C)

/* 29 Analog Reverse Channel */
#define LOG_ANALOG_REVS_C                              (0x1D + LOG_1X_BASE_C)

/* 30 Analog Handoff Entry */
#define LOG_ANALOG_HANDOFF_C                           (0x1E + LOG_1X_BASE_C)

/* 31 FM Slot Statistis entry */
#define LOG_ANALOG_FMSLOT_C                            (0x1F + LOG_1X_BASE_C)
  
/* 32 FOCC Word Sync Count entry */
#define LOG_ANALOG_WS_COUNT_C                          (0x20 + LOG_1X_BASE_C)

/* 33 */
#define LOG_RLP_PACKET_C                               (0x21 + LOG_1X_BASE_C)

/* 34 */
#define LOG_ASYNC_TCP_SEG_C                            (0x22 + LOG_1X_BASE_C)

/* 35 */
#define LOG_PACKET_DATA_IP_PACKETS_C                   (0x23 + LOG_1X_BASE_C)

/* 36 */
#define LOG_FNBDT_MESSAGE_LOG_C                        (0x24 + LOG_1X_BASE_C)

/* Begin IS-2000 LOG features */
 
/* 37 RLP RX Frames logging */ 
#define LOG_RLP_RX_FRAMES_C                            (0x25 + LOG_1X_BASE_C)
  
/* 38 RLP TX Frames logging */
#define LOG_RLP_TX_FRAMES_C                            (0x26 + LOG_1X_BASE_C)

/* 39 Reserved for additions to RLP frames */
#define LOG_RLP_RSVD1_C                                (0x27 + LOG_1X_BASE_C)

/* 40 Reserved for additions to RLP frames */
#define LOG_RLP_RSVD2_C                                (0x28 + LOG_1X_BASE_C)

/* 41 Forward Link Frame Types logging */
#define LOG_FWD_FRAME_TYPES_C                          (0x29 + LOG_1X_BASE_C)
  
/* 42 Reverse Link Frame Types logging */
#define LOG_REV_FRAME_TYPES_C                          (0x2A + LOG_1X_BASE_C)
  
/* 43 Fast Forward Power Control Parameters logging */
#define LOG_FFWD_PCTRL_C                               (0x2B + LOG_1X_BASE_C)

/* 44 Reverse Power Control Parameters logging */
#define LOG_REV_PCTRL_C                                (0x2C + LOG_1X_BASE_C)

/* 45 Searcher and Finger Information logging */
#define LOG_SRCH_FING_INFO_C                           (0x2D + LOG_1X_BASE_C)
  
/* 46 Service Configuration logging */
#define LOG_SVC_CONFIG_C                               (0x2E + LOG_1X_BASE_C)
  
/* 47 Active Set Configuration logging */
#define LOG_ASET_CONFIG_C                              (0x2F + LOG_1X_BASE_C)

/* 48 Quick Paging Channel logging */
#define LOG_QPCH_C                                     (0x30 + LOG_1X_BASE_C)

/* 49 RLP Statistics logging */
#define LOG_RLP_STAT_C                                 (0x31 + LOG_1X_BASE_C)

/* 50 Simple Test Data Service Option logging */
#define LOG_STDSO_C                                    (0x32 + LOG_1X_BASE_C)

/* 51 Pilot Phase Measurement results logging */
#define LOG_SRCH_PPM_RES_C                             (0x33 + LOG_1X_BASE_C)

/* 52 Pilot Phase Measurement Data Base logging */
#define LOG_SRCH_PPM_DB_C                              (0x34 + LOG_1X_BASE_C)

/* 53 Pilot Phase Measurement search results logging */
#define LOG_SRCH_PPM_C                                 (0x35 + LOG_1X_BASE_C)

/* 54 IS-801 forward link message */
#define LOG_GPS_FWD_MSG_C                              (0x36 + LOG_1X_BASE_C)

/* 55 IS-801 reverse link message */
#define LOG_GPS_REV_MSG_C                              (0x37 + LOG_1X_BASE_C)

/* 56 GPS search session statistics */
#define LOG_GPS_STATS_MSG_C                            (0x38 + LOG_1X_BASE_C)

/* 57 GPS search results */
#define LOG_GPS_SRCH_PEAKS_MSG_C                       (0x39 + LOG_1X_BASE_C)

/* 58 Factory Testmode logging */
#define LOG_FTM_C                                      (0x3A + LOG_1X_BASE_C)

/* 59 Multiple Peak Logging */
#define LOG_SRCH_GPS_MULTI_PEAKS_INFO_C                (0x3B + LOG_1X_BASE_C)

/* 60 Post processed search results logs */
#define LOG_SRCH_GPS_POST_PROC_C                       (0x3C + LOG_1X_BASE_C)

/* 61 FULL Test Data Service Option logging */
#define LOG_FTDSO_C                                    (0x3D + LOG_1X_BASE_C)

/* 62 Bluetooth logging */
#define LOG_BT_RESERVED_CODES_BASE_C                   (0x3E + LOG_1X_BASE_C)
/* Keep confusing name for backwards compatibility.  */
#define LOG_BT_BASE_C LOG_BT_RESERVED_CODES_BASE_C

/* 92 Bluetooth's last log code */
#define LOG_BT_LAST_C                                  (30 + LOG_BT_RESERVED_CODES_BASE_C)

/* 93 HDR log codes */
#define LOG_HDR_RESERVED_CODES_BASE_C                  (0x5D + LOG_1X_BASE_C)
/* Keep confusing name for backwards compatibility.  */
#define LOG_HDR_BASE_C LOG_HDR_RESERVED_CODES_BASE_C

/* 143 is HDR's last log code */
#define LOG_HDR_LAST_C                                 (50 + LOG_HDR_RESERVED_CODES_BASE_C)

/* 144 IS2000 DCCH Forward link channel */
#define LOG_FOR_DCCH_MSG_C                             (0x90 + LOG_1X_BASE_C)
#define LOG_DCCH_FWD_C                                 LOG_FOR_DCCH_MSG_C
  
/* 145 IS2000 DCCH Forward link channel */
#define LOG_REV_DCCH_MSG_C                             (0x91 + LOG_1X_BASE_C)
#define LOG_DCCH_REV_C                                 LOG_REV_DCCH_MSG_C

/* 146 IS2000 DCCH Forward link channel */
#define LOG_ZREX_C                                     (0x92 + LOG_1X_BASE_C)

/* 147 Active set info logging, similar to ASET_CONFIG, but simpler.  */
#define LOG_ASET_INFO_C                                (0x93 + LOG_1X_BASE_C)

/* 148 Pilot Phase Measurement four-shoulder-search resutlts logging */
#define LOG_SRCH_PPM_4SHOULDER_RES_C                   (0x94 + LOG_1X_BASE_C)

/* 149 Extended Pilot Phase Measurement Data Base logging */
#define LOG_SRCH_EXT_PPM_DB_C                          (0x95 + LOG_1X_BASE_C)

/* 150 GPS Visit Parameters */
#define LOG_GPS_VISIT_PARAMETERS_C                     (0x96 + LOG_1X_BASE_C)

/* 151 GPS Measurement */
#define LOG_GPS_MEASUREMENT_C                          (0x97 + LOG_1X_BASE_C)

/* 152 UIM Data */
#define LOG_UIM_DATA_C                                 (0x98 + LOG_1X_BASE_C)

/* 153 STDSO plus P2 */
#define LOG_STDSO_P2_C                                 (0x99 + LOG_1X_BASE_C)

/* 154 FTDSO plus P2 */
#define LOG_FTDSO_P2_C                                 (0x9A + LOG_1X_BASE_C)

/* 155 Search PPM Statistics */
#define LOG_SRCH_PPM_STATS_C                           (0x9B + LOG_1X_BASE_C)

/* 156 PPP Tx Frames */
#define LOG_PPP_TX_FRAMES_C                            (0x9C + LOG_1X_BASE_C)

/* 157 PPP Rx Frames */
#define LOG_PPP_RX_FRAMES_C                            (0x9D + LOG_1X_BASE_C)

/* 158-187 SSL reserved log codes */
#define LOG_SSL_RESERVED_CODES_BASE_C                  (0x9E + LOG_1X_BASE_C)
#define LOG_SSL_LAST_C                                 (29 + LOG_SSL_RESERVED_CODES_BASE_C)

/* 188-199 Puma reserved log codes */
/* 188 QPCH, version 2 */
#define LOG_QPCH_VER_2_C                               (0xBC + LOG_1X_BASE_C)

/* 189 Enhanced Access Probe */
#define LOG_EA_PROBE_C                                 (0xBD + LOG_1X_BASE_C)

/* 190 BCCH Frame Information */
#define LOG_BCCH_FRAME_INFO_C                          (0xBE + LOG_1X_BASE_C)

/* 191 FCCCH Frame Information */
#define LOG_FCCCH_FRAME_INFO_C                         (0xBF + LOG_1X_BASE_C)

/* 192 FDCH Frame Information */
#define LOG_FDCH_FRAME_INFO_C                          (0xC0 + LOG_1X_BASE_C)

/* 193 RDCH Frame Information */
#define LOG_RDCH_FRAME_INFO_C                          (0xC1 + LOG_1X_BASE_C)

/* 194 FFPC Information */
#define LOG_FFPC_INFO_C                                (0xC2 + LOG_1X_BASE_C)

/* 195 RPC Information */
#define LOG_RPC_INFO_C                                 (0xC3 + LOG_1X_BASE_C)

/* 196 Searcher and Finger Information */
#define LOG_SRCH_FING_INFO_VER_2_C                     (0xC4 + LOG_1X_BASE_C)

/* 197 Service Configuration, version 2 */
#define LOG_SRV_CONFIG_VER_2_C                         (0xC5 + LOG_1X_BASE_C)

/* 198 Active Set Information, version 2 */
#define LOG_ASET_INFO_VER_2_C                          (0xC6 + LOG_1X_BASE_C)

/* 199 Reduced Active Set */
#define LOG_REDUCED_ASET_INFO_C                        (0xC7 + LOG_1X_BASE_C)

/* 200 Search Triage Info */
#define LOG_SRCH_TRIAGE_INFO_C                         (0xC8 + LOG_1X_BASE_C)

/* 201 RDA Frame Information */
#define LOG_RDA_FRAME_INFO_C                           (0xC9 + LOG_1X_BASE_C)

/* 202 gpsOne fatpath information */
#define LOG_GPS_FATPATH_INFO_C                         (0xCA + LOG_1X_BASE_C)

/* 203 Extended AGC */
#define LOG_EXTENDED_AGC_C                             (0xCB + LOG_1X_BASE_C)

/* 204 Transmit AGC */
#define LOG_TRANSMIT_AGC_C                             (0xCC + LOG_1X_BASE_C)

/* 205 I/Q Offset registers */
#define LOG_IQ_OFFSET_REGISTERS_C                      (0xCD + LOG_1X_BASE_C)

/* 206 DACC I/Q Accumulator registers */
#define LOG_DACC_IQ_ACCUMULATOR_C                      (0xCE + LOG_1X_BASE_C)

/* 207 Register polling results */
#define LOG_REGISTER_POLLING_RESULTS_C                 (0xCF + LOG_1X_BASE_C)

/* 208 System arbitration module */
#define LOG_AT_SAM_C                                   (0xD0 + LOG_1X_BASE_C)

/* 209 Diablo searcher finger log */
#define LOG_DIABLO_SRCH_FING_INFO_C                    (0xD1 + LOG_1X_BASE_C)

/* 210 log reserved for dandrus */
#define LOG_SD20_LAST_ACTION_C                         (0xD2 + LOG_1X_BASE_C)

/* 211 log reserved for dandrus */
#define LOG_SD20_LAST_ACTION_HYBRID_C                  (0xD3 + LOG_1X_BASE_C)

/* 212 log reserved for dandrus */
#define LOG_SD20_SS_OBJECT_C                           (0xD4 + LOG_1X_BASE_C)

/* 213 log reserved for dandrus */
#define LOG_SD20_SS_OBJECT_HYBRID_C                    (0xD5 + LOG_1X_BASE_C)

/* 214 log reserved for jpinos */
#define LOG_BCCH_SIGNALING_C                           (0xD6 + LOG_1X_BASE_C)

/* 215 log reserved for jpinos */
#define LOG_REACH_SIGNALING_C                          (0xD7 + LOG_1X_BASE_C)

/* 216 log reserved for jpinos */
#define LOG_FCCCH_SIGNALING_C                          (0xD8 + LOG_1X_BASE_C)

/* 217 RDA Frame Information 2 */
#define LOG_RDA_FRAME_INFO_2_C                         (0xD9 + LOG_1X_BASE_C)

/* 218 */
#define LOG_GPS_BIT_EDGE_RESULTS_C                     (0xDA + LOG_1X_BASE_C)

/* 219 */
#define LOG_PE_DATA_C                                  (0xDB + LOG_1X_BASE_C)

/* 220 */
#define LOG_PE_PARTIAL_DATA_C                          (0xDC + LOG_1X_BASE_C)

/* 221 */
#define LOG_GPS_SINGLE_PEAK_SRCH_RESULTS_C             (0xDD + LOG_1X_BASE_C)

/* 222 */
#define LOG_SRCH4_SAMPRAM_C                            (0xDE + LOG_1X_BASE_C)

/* 223 */
#define HDR_AN_PPP_TX_FRAMES                           (0xDF + LOG_1X_BASE_C)

/* 224 */
#define HDR_AN_PPP_RX_FRAMES                           (0xE0 + LOG_1X_BASE_C)

/* 225 */
#define LOG_GPS_SCHEDULER_TRACE_C                      (0xE1 + LOG_1X_BASE_C)

/* 226 */
#define LOG_MPEG4_YUV_FRAME_C                          (0xE2 + LOG_1X_BASE_C)

/* 227 */
#define LOG_MPEG4_CLIP_STATS_C                         (0xE3 + LOG_1X_BASE_C)

/* 228 */
#define LOG_MPEG4_CLIP_STATS_VER2_C                    (0xE4 + LOG_1X_BASE_C)

/* 226-241 MMEG reserved. */
#define LOG_MPEG_RESERVED_CODES_BASE_C                 (0xF1 + LOG_1X_BASE_C)

/* 242-274 BREW reserved log range */
#define LOG_BREW_RESERVED_CODES_BASE_C                 (0xF2 + LOG_1X_BASE_C)
#define LOG_BREW_LAST_C                                (32 + LOG_BREW_RESERVED_CODES_BASE_C)

/* 275-339 PPP Extended Frames */
#define LOG_PPP_FRAMES_RESERVED_CODES_BASE_C           (0x113 + LOG_1X_BASE_C)
#define LOG_PPP_FRAMES_LAST_C                          (64 + LOG_PPP_FRAMES_RESERVED_CODES_BASE_C)

#define LOG_PPP_EXT_FRAMED_RX_UM_C                     (0x113 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_RX_RM_C                     (0x114 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_RX_AN_C                     (0x115 + LOG_1X_BASE_C)

#define LOG_PPP_EXT_FRAMED_TX_UM_C                     (0x123 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_TX_RM_C                     (0x124 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_FRAMED_TX_AN_C                     (0x125 + LOG_1X_BASE_C)

#define LOG_PPP_EXT_UNFRAMED_RX_UM_C                   (0x133 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_RX_RM_C                   (0x134 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_RX_AN_C                   (0x135 + LOG_1X_BASE_C)

#define LOG_PPP_EXT_UNFRAMED_TX_UM_C                   (0x143 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_TX_RM_C                   (0x144 + LOG_1X_BASE_C)
#define LOG_PPP_EXT_UNFRAMED_TX_AN_C                   (0x145 + LOG_1X_BASE_C)

/* 340 LOG_PE_DATA_EXT_C */
#define LOG_PE_DATA_EXT_C                              (0x154 + LOG_1X_BASE_C)

/* REX Subsystem logs */
#define LOG_MEMDEBUG_C                                 (0x155 + LOG_1X_BASE_C)
#define LOG_SYSPROFILE_C                               (0x156 + LOG_1X_BASE_C)
#define LOG_TASKPROFILE_C                              (0x157 + LOG_1X_BASE_C)
#define LOG_COREDUMP_C                                 (0x158 + LOG_1X_BASE_C)

/* 341-349 REX subsystem logs */
#define LOG_REX_RESERVED_CODES_BASE_C                  (0x155 + LOG_1X_BASE_C)
#define LOG_REX_LAST_C                                 (8 + LOG_REX_RESERVED_CODES_BASE_C)

/* 350 LOG_PE_PARTIAL_DATA_EXT_C */
#define LOG_PE_PARTIAL_DATA_EXT_C                      (0x15E + LOG_1X_BASE_C)

/* 351 LOG_DIAG_STRESS_TEST_C */
#define LOG_DIAG_STRESS_TEST_C                         (0x15F + LOG_1X_BASE_C)

/* 352  LOG_WMS_READ_C */
#define LOG_WMS_READ_C                                 (0x160 + LOG_1X_BASE_C)

/* 353 Search Triage Info Version 2 */
#define LOG_SRCH_TRIAGE_INFO2_C                         (0x161 + LOG_1X_BASE_C)

/* 354 RLP Rx FDCH Frames */
#define LOG_RLP_RX_FDCH_FRAMES_C                        (0x162 + LOG_1X_BASE_C)


/* 355 RLP Tx FDCH Frames */
#define LOG_RLP_TX_FDCH_FRAMES_C                        (0x163 + LOG_1X_BASE_C)

/* 356-371 QTV subsystem logs */
#define LOG_QTV_RESERVED_CODES_BASE_C                   (0x164 + LOG_1X_BASE_C)
#define LOG_QTV_LAST_C                                  (15 + LOG_QTV_RESERVED_CODES_BASE_C)

/* 372 Searcher 4 1X */
#define LOG_SRCH4_1X_C                                  (0x174 + LOG_1X_BASE_C)

/* 373 Searcher sleep statistics */
#define LOG_SRCH_SLEEP_STATS_C                          (0x175 + LOG_1X_BASE_C)

/* 374 Service Configuration, version 3 */
#define LOG_SRV_CONFIG_VER_3_C                          (0x176 + LOG_1X_BASE_C)

/* 375 Searcher 4 HDR */
#define LOG_SRCH4_HDR_C                                 (0x177 + LOG_1X_BASE_C)

/* 376 Searcher 4 AFLT */
#define LOG_SRCH4_AFLT_C                                (0x178 + LOG_1X_BASE_C)

/* 377 Enhanced Finger Information */
#define LOG_ENH_FING_INFO_C                             (0x179 + LOG_1X_BASE_C)

/* 378 DV Information */
#define LOG_DV_INFO_C                                   (0x17A + LOG_1X_BASE_C)

/* 379 WMS set routes information */
#define LOG_WMS_SET_ROUTES_C                            (0x17B + LOG_1X_BASE_C)

/* 380 FTM Version 2 Logs */
#define LOG_FTM_VER_2_C                                 (0x17C + LOG_1X_BASE_C)

/* 381 GPS Multipeak logging - Internal use only */
#define LOG_SRCH_GPS_MULTI_PEAKS_SIMPLIFIED_INFO_C      (0x17D + LOG_1X_BASE_C)

/* 382 GPS Multipeak logging - Internal use only */
#define LOG_SRCH_GPS_MULTI_PEAKS_VERBOSE_INFO_C         (0x17E + LOG_1X_BASE_C)

/* 383-403 HDR reserved logs */
#define LOG_HDR_RESERVED_CODES_BASE_2_C                 (0x17F + LOG_1X_BASE_C)
#define LOG_HDR_LAST_2_C                                (20 + LOG_HDR_RESERVED_CODES_BASE_2_C)

/* The last defined DMSS log code */
#define LOG_1X_LAST_C                                  ((0x193) + LOG_1X_BASE_C)

/* This is only here for old (pre equipment ID update) logging code */
#define LOG_LAST_C ( LOG_1X_LAST_C & 0xFFF )

/* -------------------------------------------------------------------------
 * Log Equipment IDs.
 * The number is represented by 4 bits.
 * ------------------------------------------------------------------------- */
typedef enum {
  LOG_EQUIP_ID_OEM = 0, /* 3rd party OEM (licensee) use */
  LOG_EQUIP_ID_1X = 1,  /* Traditional 1X line of products */
  LOG_EQUIP_ID_RSVD2 = 2,
  LOG_EQUIP_ID_RSVD3 = 3,
  LOG_EQUIP_ID_WCDMA = 4,
  LOG_EQUIP_ID_GSM   = 5,
  LOG_EQUIP_ID_LBS   = 6,
  LOG_EQUIP_ID_UMTS  = 7,
  LOG_EQUIP_ID_TDMA  = 8,
  LOG_EQUIP_ID_BOA   = 9,

  LOG_EQUIP_ID_LAST_DEFAULT = LOG_EQUIP_ID_BOA

} log_equip_id_enum_type;

#define LOG_EQUIP_ID_MAX 0xF /* The equipment ID is 4 bits */

/* Note that these are the official values and are used by default in 
   diagtune.h.
*/
#define LOG_EQUIP_ID_0_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_1_LAST_CODE_DEFAULT LOG_1X_LAST_C
#define LOG_EQUIP_ID_2_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_3_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_4_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_5_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_6_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_7_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_8_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_9_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_10_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_11_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_12_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_13_LAST_CODE_DEFAULT 0 
#define LOG_EQUIP_ID_14_LAST_CODE_DEFAULT 0
#define LOG_EQUIP_ID_15_LAST_CODE_DEFAULT 0

#endif /* LOG_CODES_H */

