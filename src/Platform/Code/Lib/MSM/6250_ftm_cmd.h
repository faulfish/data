#ifndef _6250_FTM_CMD_H
#define _6250_FTM_CMD_H

//+ this block is copied from 6250's ftm_gsm.h

/*===========================================================================

							 G S M    S P E C I F I C   D E F I N I T I O N S
												 H E A D E R  F I L E

DESCRIPTION
	 This file contains GSM definitions which are also 
	 contained in a file on the PC.  This keeps them in a place where they 
	 can be easily modified and updated as necessary to aid in it's maintenance.

 
	 Copyright (c) 2001 by QUALCOMM Incorporated.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

											EDIT HISTORY FOR FILE

	This section contains comments describing changes made to the module.
	Notice that changes are listed in reverse chronological order.

	Initially, comments will be entered manually. Later, as the module is
	placed under version control, the version control tool will automatic-
	ally insert change comments into this section. [Delete this comment].

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/6250_ftm_cmd.h.-arc   1.1   Apr 01 2009 14:19:22   Kevin Tai  $ $DateTime: 2003/10/23 18:05:27 $ $Author:   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
12/10/02   xw      Remove grfc_setup_info, mdsp_buffer and chfilt_cfg in 
                   ftm_gsm_data_union.
12/10/02   xw      Code clean up and renamed it to ftm_gsm.h
06/11/01   avi     Created.

=============================================================================*/

#if !(defined (__WIN32__) || defined(WIN32))

	#include "comdef.h"

#endif /* !(defined (__WIN32__) || defined(WIN32)) */

typedef word enum_type;

/* Number of dummy RSSI reads to clear up the Rx debug buffer before starting to collect data */
#define MIN_NUM_OF_INVLID_RSSI_READ        5

/* Mask for 20 GRFC                             */
#define FTM_GRFC_MASK                          0x000FFFFFL                        

/* Number of entries in the PA LUT              */
#define FTM_PA_LUT_MAX_SIZE                    64                         

#define FTM_DIV_FACTOR                         10

#define FTM_MIN_RX_FRAME_CNT                   2

#define RX_TX_START_INIT_CNT               20      /* Wait 0.1 seconds to start Rx/Tx init   */
#define RX_TX_INIT_DONE_CNT                40      /* Wait 0.2 seconds to start Rx/Tx func   */

#define MOD_CFG_TRL_GB_POS                 8
#define MOD_CFG_LDN_GB_POS                 12
#define MOD_CFG_OSR_POS                    19
#define MOD_CFG_GB_POL_POS                 21
#define MOD_CFG_SPEC_INV_POS               22
#define MOD_CFG_GB_LEN_POS                 23
#define MOD_CFG_MDSP_IQ_SEL_POS            29
#define MOD_CFG_USE_ROTATOR_POS            30
#define MOD_CFG_CONST_MOD_EN_POS           31

#ifndef FEATURE_GSM
#define FEATURE_GSM
#endif

/**************************************************
**   GRFC Communication buffer
**************************************************/
typedef enum
{
  FTM_GRFC_ACTIVE_HIGH,
  FTM_GRFC_ACTIVE_LOW
} ftm_enum_grfc_polarity;

typedef enum
{
  FTM_GRFC_INPUT,
  FTM_GRFC_OUTPUT
} ftm_enum_grfc_direction;

typedef enum
{
  FTM_GRFC_STATIC_OUTPUT,
  FTM_GRFC_TIMED_OUTPUT
} ftm_enum_grfc_output_mode;

typedef enum
{
  FTM_GRFC_0,
  FTM_GRFC_1,
  FTM_GRFC_2,
  FTM_GRFC_3,
  FTM_GRFC_4,
  FTM_GRFC_5,
  FTM_GRFC_6,
  FTM_GRFC_7,
  FTM_GRFC_8,
  FTM_GRFC_9,
  FTM_GRFC_10,
  FTM_GRFC_11,
  FTM_GRFC_12,
  FTM_GRFC_13,
  FTM_GRFC_14,
  FTM_GRFC_15,
  FTM_GRFC_16,
  FTM_GRFC_17,
  FTM_GRFC_18,
  FTM_GRFC_19,
  FTM_GRFC_MAX
} ftm_enum_grfc_id;

typedef enum
{
  FTM_GRFC_CHFILT_TIME,
  FTM_GRFC_PA_RAMP_TIME,
  FTM_GRFC_PA_PRECHARGE_TIME,
  FTM_GRFC_TX_SLEEP,
  FTM_GRFC_AUX_MAX
} ftm_enum_aux_grfc_id;

#if defined (__WIN32__) || defined(WIN32)
typedef struct
{
  int2         start_offset;
  int2          stop_offset;
#else
typedef PACKED struct
{
  int16        start_offset;
  int16         stop_offset;
#endif
} ftm_grfc_timing_offset_type;

#if defined (__WIN32__) || defined(WIN32)
typedef struct
{
  int2           ftm_grfc_id;
  union
  {
    BOOLEAN      ftm_state;
#else
typedef PACKED struct
{
  enum_type      ftm_grfc_id;
  PACKED union
  {
    boolean      ftm_state;
#endif
    ftm_grfc_timing_offset_type ftm_grfc_timing;
  } ftm_grfc_cfg;
} ftm_grfc_type;

/*-----------------------------------------------------------------------
** Define the data type for the PA LUT
-----------------------------------------------------------------------*/
typedef enum
{
  FTM_POWER_LEVEL_0_GSM_0900,
  FTM_POWER_LEVEL_1_GSM_0900 = FTM_POWER_LEVEL_0_GSM_0900,
  FTM_POWER_LEVEL_2_GSM_0900 = FTM_POWER_LEVEL_0_GSM_0900,
  FTM_POWER_LEVEL_3_GSM_0900,
  FTM_POWER_LEVEL_4_GSM_0900,
  FTM_POWER_LEVEL_5_GSM_0900,
  FTM_POWER_LEVEL_6_GSM_0900,
  FTM_POWER_LEVEL_7_GSM_0900,
  FTM_POWER_LEVEL_8_GSM_0900,
  FTM_POWER_LEVEL_9_GSM_0900,
  FTM_POWER_LEVEL_10_GSM_0900,
  FTM_POWER_LEVEL_11_GSM_0900,
  FTM_POWER_LEVEL_12_GSM_0900,
  FTM_POWER_LEVEL_13_GSM_0900,
  FTM_POWER_LEVEL_14_GSM_0900,
  FTM_POWER_LEVEL_15_GSM_0900,
  FTM_POWER_LEVEL_16_GSM_0900,
  FTM_POWER_LEVEL_17_GSM_0900,
  FTM_POWER_LEVEL_18_GSM_0900,
  FTM_POWER_LEVEL_19_GSM_0900,
  FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_21_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_22_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_23_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_24_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_25_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_26_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_27_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_28_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_29_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,
  FTM_POWER_LEVEL_30_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,  
  FTM_POWER_LEVEL_31_GSM_0900 = FTM_POWER_LEVEL_20_GSM_0900,

  FTM_POWER_LEVEL_0_GSM_1800= FTM_POWER_LEVEL_0_GSM_0900,
  FTM_POWER_LEVEL_1_GSM_1800= FTM_POWER_LEVEL_0_GSM_0900,
  FTM_POWER_LEVEL_2_GSM_1800= FTM_POWER_LEVEL_0_GSM_0900,
  FTM_POWER_LEVEL_3_GSM_1800,
  FTM_POWER_LEVEL_4_GSM_1800,
  FTM_POWER_LEVEL_5_GSM_1800,
  FTM_POWER_LEVEL_6_GSM_1800,
  FTM_POWER_LEVEL_7_GSM_1800,
  FTM_POWER_LEVEL_8_GSM_1800,
  FTM_POWER_LEVEL_9_GSM_1800,
  FTM_POWER_LEVEL_10_GSM_1800,
  FTM_POWER_LEVEL_11_GSM_1800,
  FTM_POWER_LEVEL_12_GSM_1800,
  FTM_POWER_LEVEL_13_GSM_1800,
  FTM_POWER_LEVEL_14_GSM_1800,
  FTM_POWER_LEVEL_15_GSM_1800,
  FTM_POWER_LEVEL_16_GSM_1800,
  FTM_POWER_LEVEL_17_GSM_1800,
  FTM_POWER_LEVEL_18_GSM_1800,
  FTM_POWER_LEVEL_19_GSM_1800,
  FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_21_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_22_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_23_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_24_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_25_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_26_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_27_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_28_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_29_GSM_1800 = FTM_POWER_LEVEL_20_GSM_1800,
  FTM_POWER_LEVEL_30_GSM_1800,
  FTM_POWER_LEVEL_31_GSM_1800,

  FTM_POWER_LEVEL_0_GSM_1900 = FTM_POWER_LEVEL_0_GSM_0900,
  FTM_POWER_LEVEL_1_GSM_1900,
  FTM_POWER_LEVEL_2_GSM_1900,
  FTM_POWER_LEVEL_3_GSM_1900,
  FTM_POWER_LEVEL_4_GSM_1900,
  FTM_POWER_LEVEL_5_GSM_1900,
  FTM_POWER_LEVEL_6_GSM_1900,
  FTM_POWER_LEVEL_7_GSM_1900,
  FTM_POWER_LEVEL_8_GSM_1900,
  FTM_POWER_LEVEL_9_GSM_1900,
  FTM_POWER_LEVEL_10_GSM_1900,
  FTM_POWER_LEVEL_11_GSM_1900,
  FTM_POWER_LEVEL_12_GSM_1900,
  FTM_POWER_LEVEL_13_GSM_1900,
  FTM_POWER_LEVEL_14_GSM_1900,
  FTM_POWER_LEVEL_15_GSM_1900,
  FTM_POWER_LEVEL_16_GSM_1900,
  FTM_POWER_LEVEL_17_GSM_1900,
  FTM_POWER_LEVEL_18_GSM_1900,
  FTM_POWER_LEVEL_19_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,
  FTM_POWER_LEVEL_20_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_21_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,    
  FTM_POWER_LEVEL_22_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,
  FTM_POWER_LEVEL_23_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_24_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_25_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_26_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_27_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_28_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_29_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_30_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_31_GSM_1900 = FTM_POWER_LEVEL_18_GSM_1900,  
  FTM_POWER_LEVEL_NUM_LEVELS = 19

} ftm_enum_tx_power_level_type;

/* This enum maps the HW groups power level definitions into array indices 
  for the gsm_dacCount_power and dcs_dacCount_poewr arrays defined in 
  ftm_gsm_paprofile.c 
  NOTE: This is not to be confused with the POWER LEVEL's described above 
  and used by the FTM software
*/
typedef enum {
  FTM_GSM_DAC_LEVEL19 = 0,
  FTM_GSM_DAC_LEVEL18,  
  FTM_GSM_DAC_LEVEL17,  
  FTM_GSM_DAC_LEVEL16,  
  FTM_GSM_DAC_LEVEL15,  
  FTM_GSM_DAC_LEVEL14,  
  FTM_GSM_DAC_LEVEL13,  
  FTM_GSM_DAC_LEVEL12,  
  FTM_GSM_DAC_LEVEL11,  
  FTM_GSM_DAC_LEVEL10,  
  FTM_GSM_DAC_LEVEL9 ,  
  FTM_GSM_DAC_LEVEL8 ,  
  FTM_GSM_DAC_LEVEL7 ,  
  FTM_GSM_DAC_LEVEL6 ,  
  FTM_GSM_DAC_LEVEL5 ,  
  FTM_DCS_DAC_LEVEL15 = 0,
  FTM_DCS_DAC_LEVEL14,
  FTM_DCS_DAC_LEVEL13,
  FTM_DCS_DAC_LEVEL12,
  FTM_DCS_DAC_LEVEL11,
  FTM_DCS_DAC_LEVEL10,
  FTM_DCS_DAC_LEVEL9, 
  FTM_DCS_DAC_LEVEL8, 
  FTM_DCS_DAC_LEVEL7, 
  FTM_DCS_DAC_LEVEL6, 
  FTM_DCS_DAC_LEVEL5, 
  FTM_DCS_DAC_LEVEL4, 
  FTM_DCS_DAC_LEVEL3, 
  FTM_DCS_DAC_LEVEL2, 
  FTM_DCS_DAC_LEVEL1, 
  FTM_DCS_DAC_LEVEL0
} dac_power_level;

#if defined (__WIN32__) || defined(WIN32)
typedef struct
{
  BYTE                                ftm_ramp_step_size;
  BYTE                                   ftm_pa_lut_size;
#else
typedef PACKED struct
{
  uint8                               ftm_ramp_step_size;
  uint8                                  ftm_pa_lut_size;
#endif /* defined(__WIN32__) || defined(WIN32) */
} ftm_pa_lut_cfg_type;

#if defined (__WIN32__) || defined(WIN32)
typedef struct
{
  WORD                      ftm_ramp_up[FTM_PA_LUT_MAX_SIZE];
  WORD                    ftm_ramp_down[FTM_PA_LUT_MAX_SIZE];
#else
typedef PACKED struct
{
  uint16                    ftm_ramp_up[FTM_PA_LUT_MAX_SIZE];
  uint16                  ftm_ramp_down[FTM_PA_LUT_MAX_SIZE];
#endif /* defined(__WIN32__) || defined(WIN32) */
  ftm_pa_lut_cfg_type                             ftm_pa_cfg;
} ftm_pa_lut_type;

/*------------------------------------------------------------------------------
** Define the data type for the modulator config
*/

#if defined(__WIN32__) || defined(WIN32)
typedef struct
{
  BOOLEAN         cont_mod_en;     /* set cont modulation;                 bit [31]    in mod_cfg */
  BOOLEAN         use_rotator;     /* use rotation;                        bit [30]    in mod_cfg */
  BOOLEAN         mdsp_iq_sel;     /* mdsp I/Q selector;                   bit [29]    in mod_cfg */
  BOOLEAN            spec_inv;     /* use spectral inversion;              bit [22]    in mod_cfg */
  BOOLEAN          gd_bit_pol;     /* guard bit polarity;                  bit [21]    in mod_cfg */
  BYTE          gd_bit_bursts;     /* number of guard bits between bursts; bits[28:23] in mod_cfg */
  BYTE                os_rate;     /* oversampling rate;                   bits[20:19] in mod_cfg */
  BYTE            gd_bit_lead;     /* number of leading guard bits;        bits[18:12] in mod_cfg */
  BYTE           gd_bit_trail;     /* number of trailing guard bits;       bits[11:8]  in mod_cfg */
  BYTE           burst_length;     /* number of bits in a burst;           bits[7:0]   in mod_cfg */
#else
typedef PACKED struct
{
  boolean         cont_mod_en;     /* set cont modulation;                 bit [31]    in mod_cfg */
  boolean         use_rotator;     /* use rotation;                        bit [30]    in mod_cfg */
  boolean         mdsp_iq_sel;     /* mdsp I/Q selector;                   bit [29]    in mod_cfg */
  boolean            spec_inv;     /* use spectral inversion;              bit [22]    in mod_cfg */
  boolean          gd_bit_pol;     /* guard bit polarity;                  bit [21]    in mod_cfg */
  uint8         gd_bit_bursts;     /* number of guard bits between bursts; bits[28:23] in mod_cfg */
  uint8               os_rate;     /* oversampling rate;                   bits[20:19] in mod_cfg */
  uint8           gd_bit_lead;     /* number of leading guard bits;        bits[18:12] in mod_cfg */
  uint8          gd_bit_trail;     /* number of trailing guard bits;       bits[11:8]  in mod_cfg */
  uint8          burst_length;     /* number of bits in a burst;           bits[7:0]   in mod_cfg */
#endif /* defined(__WIN32__) || defined(WIN32) */
} ftm_mod_cfg_type;


/*------------------------------------------------------------------------------
** Define the data type for the Burst Transmit Command
** and Continuous Burst Transmit Command
*/

typedef enum
{
  FTM_TX_DATA_SOURCE_PSDRND,
  FTM_TX_DATA_SOURCE_TONE,
  FTM_TX_DATA_SOURCE_BUFFER
} ftm_enum_tx_data_source_type;

#if defined(__WIN32__) || defined(WIN32)
typedef struct
{
  BYTE                      slot_num;
  int2                   data_source;
  BYTE                     tsc_index;
  DWORD                   num_bursts; /* this number is examined if infinite_duration == FALSE */
  BOOLEAN          infinite_duration; /* this flags sets the infinite duration of */
#else
typedef PACKED struct
{
  uint8                     slot_num;
  enum_type              data_source;
  uint8                    tsc_index;
  uint32                  num_bursts; /* this number is examined if infinite_duration == FALSE */
  boolean          infinite_duration; /* this flags sets the infinite duration of */
#endif /* defined(__WIN32__) || defined(WIN32) */
} ftm_tx_burst_type;


/*------------------------------------------------------------------------------
** Types to set CHFILT parameters
*/

typedef enum
{
  FTM_DMA_ACKNOWLEDGE,
  FTM_AUTO_ACKNOWLEDGE
} ftm_enum_chfilt_auto_ack_type;

typedef enum
{
  FTM_DMA_DISABLE,
  FTM_DMA_ENABLE
} enum_chfilt_dma_ena_type;

typedef enum
{
  FTM_DMA_SEL_A,
  FTM_DMA_SEL_B,
  FTM_DMA_SEL_C,
  FTM_DMA_SEL_D
} ftm_enum_chfilt_dma_sel_type;

typedef enum
{
  FTM_INVERT_Y1_ENABLE,
  FTM_INVERT_Y1_DISABLE
} ftm_enum_chfilt_invert_y1_type;

typedef enum
{
  FTM_ROTATOR_DISABLE,
  FTM_ROTATOR_ENABLE
} ftm_enum_chfilt_use_rotator_type;

/*------------------------------------------------------------------------------
** Define the data type for the channel filter config
*/

#if defined(__WIN32__) || defined(WIN32)
typedef struct
{
  BOOLEAN         auto_ack;  /* 0 - use real DMA acknowledge signal (normal)    */
  /* 1 - use auto-acknowledge signal (test & debug)  */
  BOOLEAN          dma_ena;  /* 0 - disable DMA transfer of CHFILT data to MDSP */
  /* 1 - enable  DMA transfer of CHFILT data to MDSP */
  BYTE            dma_sel;   /* 0 - input to stage 1 is DMA'd to MDSP           */
  /* 1 - input to stage 2 is DMA'd to MDSP           */
  /* 2 - input to stage 3 is DMA'd to MDSP           */
  /* 3 - output of stage 3 is DMA'd to MDSP          */
  BOOLEAN       invert_y1i;  /* 0 - map 0 to +1, 1 to -1                        */
                             /* 1 - map 0 to -1, 1 to +1                        */
  BOOLEAN       invert_y1q;  /* see invert_y1i                                  */
  BOOLEAN      use_rotator;  /* 0 - do not use rotator between stages 2 & 3     */
  /* 1 - use rotator between stages 2 & 3            */

#else
typedef PACKED struct
{
  boolean         auto_ack;  /* 0 - use real DMA acknowledge signal (normal)    */
                             /* 1 - use auto-acknowledge signal (test & debug)  */
  boolean          dma_ena;  /* 0 - disable DMA transfer of CHFILT data to MDSP */
  uint8            dma_sel;  /* 0 - input to stage 1 is DMA'd to MDSP           */
  /* 1 - input to stage 2 is DMA'd to MDSP           */
  /* 2 - input to stage 3 is DMA'd to MDSP           */
  /* 3 - output of stage 3 is DMA'd to MDSP          */
  boolean       invert_y1i;  /* 0 - map 0 to +1, 1 to -1                        */
                             /* 1 - map 0 to -1, 1 to +1                        */
  boolean       invert_y1q;  /* see invert_y1i                                  */
  boolean      use_rotator;  /* 0 - do not use rotator between stages 2 & 3     */
                             /* 1 - use rotator between stages 2 & 3            */
#endif /* defined(__WIN32__) || defined(WIN32) */
} ftm_chfilt_cfg_type;

/*------------------------------------------------------------------------------
** Define the data type for the Burst Receive Command
*/
typedef enum
{
  FTM_RX_FUNCTION_GET_RSSI,
  FTM_RX_FUNCTION_GET_DC_OFFSETS,
  FTM_RX_FUNCTION_STOP_CONTINOUS_RX,
  FTM_RX_FUNCTION_RF_RX_HW_ON,
  FTM_RX_FUNCTION_RF_RX_HW_OFF
} ftm_enum_rx_function_type;

#if defined(__WIN32__) || defined(WIN32)
typedef struct
{
  WORD         ftm_i_dc_offset;
  WORD         ftm_q_dc_offset;
#else
typedef PACKED struct
{
  uint16       ftm_i_dc_offset;
  uint16       ftm_q_dc_offset;
#endif /* defined(__WIN32__) || defined(WIN32) */
} ftm_i_q_dc_offset_type;

#if defined(__WIN32__) || defined(WIN32)
typedef struct
{
  BYTE         slot_num;
  int2         ftm_rx_funct;
#else
typedef PACKED struct
{
  uint8        slot_num;
  enum_type    ftm_rx_funct;
#endif /* defined(__WIN32__) || defined(WIN32) */
} ftm_rx_burst_type;

typedef PACKED struct
{
  uint16 dac_start;
  uint16 dac_stop;
  uint16 dac_step;
}ftm_gsm_tx_cal_sweep_type;

/*Enums*/
typedef enum
{
  FTM_PDM_GSM_VCTCXO_ADJ,
  FTM_PDM_GSM_I_DC_OFFSET,
  FTM_PDM_GSM_Q_DC_OFFSET,
  FTM_PDM_GSM_MAX_NUM
} ftm_gsm_pdm_id_type;

/* Return the current phone state */
#if defined(__WIN32__) || defined(WIN32)
typedef struct
{
  /* Miscellaneous Status */
  int2                     ftm_rf_mode;   /* Current Mode               */
  int2                    ftm_phone_id;   /* Phone Type                 */
  /* RF status            */
  WORD                     ftm_channel;   /* Current Channel            */
  int2                ftm_tx_power_lvl;   /* Current TX PCL             */
  BOOLEAN                 ftm_tx_state;   /* TX state                   */
  BOOLEAN                 ftm_pa_state;   /* PA state                   */
  int2                   ftm_lna_range;   /* LNA gain range             */
  BOOLEAN               ftm_lna_on_off;   /* LNA power supply state     */
  BOOLEAN              ftm_mixer_state;   /* Mixer state                */
  int2                    ftm_vga_gain;   /* VGA gain (DP2000) support  */
  DWORD                       ftm_rssi;   /* RSSI returned by MDSP      */
  /* GRFC llaneous Status */
  DWORD              ftm_grfc_io_state;   /* GRFC input/output states   */
  DWORD                 ftm_grfc_omode;   /* GRFC output mode           */
  DWORD              ftm_grfc_polarity;   /* GRFC polarity(acHI/acLO)   */
  DWORD                  ftm_grfc_data;   /* GRFC data (register values)*/

  /* PDM status */
  WORD    ftm_pdm_val[FTM_PDM_GSM_MAX_NUM];    /* PDM Values                 */
#else
typedef PACKED struct
{
  /* Miscellaneous Status */
  enum_type               ftm_rf_mode;    /* Current Mode               */
  enum_type               ftm_phone_id;   /* Phone Type                 */
  /* RF status            */
  uint16                   ftm_channel;   /* Current Channel            */
  enum_type           ftm_tx_power_lvl;   /* Current TX PCL             */
  boolean                 ftm_tx_state;   /* TX state                   */
  boolean                 ftm_pa_state;   /* PA state                   */
  enum_type              ftm_lna_range;   /* LNA gain range             */
  boolean               ftm_lna_on_off;   /* LNA power supply state     */
  boolean              ftm_mixer_state;   /* Mixer state                */
  int16                   ftm_vga_gain;   /* VGA gain (DP2000) support  */
  uint32                      ftm_rssi;   /* RSSI returned by MDSP      */
  /* GRFC llaneous Status */
  uint32             ftm_grfc_io_state;   /* GRFC input/output states   */
  uint32                ftm_grfc_omode;   /* GRFC output mode           */
  uint32             ftm_grfc_polarity;   /* GRFC polarity(acHI/acLO)   */
  uint32                 ftm_grfc_data;   /* GRFC data (register values)*/

  /* PDM status */
  uint16  ftm_pdm_val[FTM_PDM_GSM_MAX_NUM];    /* PDM Values                               */
#endif /* defined(__WIN32__) || defined(WIN32) */
  ftm_mod_cfg_type             ftm_mod_cfg;    /* Modulator config information             */
  ftm_chfilt_cfg_type       ftm_chfilt_cfg;    /* Channel filter config information        */
  ftm_grfc_timing_offset_type     ftm_grfc_timing[FTM_GRFC_MAX];/* GRFC timing information */
  ftm_i_q_dc_offset_type ftm_i_q_dc_offset;    /* I and Q DC offsets calculated by MDSP    */
} ftm_gsm_state_type;

#if defined(__WIN32__) || defined(WIN32)
typedef struct
{
  int2                       ftm_gsm_test_id;
  union
  {
    BOOLEAN                   generic_bool;
    BYTE                    generic_8bit_u;
    WORD                   generic_16bit_u;
    DWORD                  generic_32bit_u;
    int2                        ftm_tx_pwr_lvl;
    int2                      ftm_vga_gain;
#else
typedef PACKED struct
{
  enum_type                    ftm_gsm_test_id;
  PACKED union
  {
    boolean                       generic_bool;
    uint8                       generic_8bit_u;
    uint16                     generic_16bit_u;
    uint32                     generic_32bit_u;
    enum_type                   ftm_tx_pwr_lvl;
    int16                         ftm_vga_gain;
#endif
    ftm_grfc_type                     ftm_grfc;    /* generic state data for a GRFC      */
    ftm_pa_lut_cfg_type         ftm_pa_lut_cfg;    /* PA LUT configuration & power level */
    ftm_pa_lut_type                 ftm_pa_lut;    /* PA LUT values and cfg information  */
    ftm_mod_cfg_type               ftm_mod_cfg;    /* modultaor configuration            */
    ftm_tx_burst_type             ftm_tx_burst;    /* TX burst configuration             */
    ftm_rx_burst_type             ftm_rx_burst;    /* RX burst command                   */
    ftm_gsm_tx_cal_sweep_type   ftm_gsm_tx_cal;    /* TX CAL sweep parameters            */
    ftm_gsm_state_type           ftm_gsm_state;    /* Phone state                        */
    ftm_i_q_dc_offset_type   ftm_i_q_dc_offset;    /* I/Q DC offsets computed by MDSP    */
  } ftm_gsm_data_union;
} gsm_option_type;


//+ this block is base on 6250's ftm.h
#define FEATURE_GSM
#define FTM_IQ_RECORD_BLOCK_SIZE   256

typedef enum 
{
  AMSS_MODE,
  FTM_MODE
} ftm_mode_type;

/*
typedef enum 
{
  AMSS_MODE,
  DMSS_MODE = AMSS_MODE,
  FTM_MODE
} ftm_mode_type;
*/
enum 
{
  FTM_HWTC = 10,
  FTM_RF,

  FTM_MAX
};

/* FTM Command Codes. */
enum 
{
  FTM_1X_C,
  FTM_WCDMA_C,
  FTM_GSM_C,
  FTM_I2C    = 5,
  FTM_LOG    = 9,
  FTM_AGPS_C = 10,
  FTM_MODE_MAX
}; 
   
typedef enum 
{
  FTM_DB_RF_WCDMA_IMT,                /* RF is in WCDMA IMT mode of operation          */
  FTM_DB_RF_WCDMA_1900MHZ_A,          /* RF is in 1900MHz WCDMA, band A of operation   */
  FTM_DB_RF_WCDMA_1900MHZ_B,          /* RF is in 1900MHz WCDMA, band B of operation   */
  FTM_DB_RF_GSM_900,                  /* RF is in EGSM mode of operation               */
  FTM_DB_RF_GSM_1800,                 /* RF is in GSM DCS mode of operation            */
  FTM_DB_RF_GPS,                      /* RF is in GPS mode of operation                */
  FTM_DB_RF_SLEEP,                    /* RF is in sleep mode of operation              */
  FTM_DB_RF_IDLE,                     /* RF is in idle FTM mode of operation           */
  FTM_DB_RF_GSM_850,                  /* RF is in 850 band                             */
  FTM_DB_RF_GSM_1900,                 /* RF is in 1900 band                            */

  FTM_DB_RF_MAX
} ftm_rf_mode_type;

typedef enum
{
  FTM_PHONE_MODE_CDMA = 0,
  FTM_PHONE_MODE_FM,
  FTM_PHONE_MODE_GPS_SINAD = 4,
  FTM_PHONE_MODE_CDMA_800,
  FTM_PHONE_MODE_CDMA_1900,
  FTM_PHONE_MODE_HDR,
  FTM_PHONE_MODE_CDMA_1800,
  FTM_PHONE_MODE_WCDMA_IMT,
  FTM_PHONE_MODE_GSM_900,
  FTM_PHONE_MODE_GSM_1800,
  FTM_PHONE_MODE_GSM_1900,
  FTM_PHONE_MODE_BLUETOOTH,
  FTM_PHONE_MODE_JCDMA = 14,
  FTM_PHONE_MODE_WCDMA_1900A,
  FTM_PHONE_MODE_WCDMA_1900B,
  FTM_PHONE_MODE_CDMA_450,
  FTM_PHONE_MODE_GSM_850=18,    /* Just added for band 850 */

  FTM_PHONE_MODE_MAX = 255
} ftm_mode_id_type;

typedef enum
{
  FTM_PA_R0,
  FTM_PA_R1,
  FTM_PA_R2,
  FTM_PA_R3,

  FTM_PA_RMAX
} ftm_pa_range_type;

typedef enum
{
  FTM_LNA_RANGE_0,
  FTM_LNA_RANGE_1,
  FTM_LNA_RANGE_2,
  FTM_LNA_RANGE_3,
  FTM_LNA_RANGE_4,

  FTM_LNA_RANGE_MAX
} ftm_lna_range_type;

typedef PACKED struct
{
  word id;
  int2 val;
} ftm_id_val_type;

typedef PACKED struct
{
  byte byte0;
  byte byte1;
  byte byte2;
} ftm_byte_type;

typedef enum
{
  FTM_PDM_TX_AGC_ADJ = 2,
  FTM_PDM_TRK_LO_ADJ = 4,

  FTM_PDM_MAX_NUM
} ftm_pdm_id_type;

//Leon ++ user defined data type
//typedef PACKED struct {
//		unsigned char off;
//		unsigned char slp[NV_CDMA_RX_LIN_SIZ];
//} rx_lin_type;
//
//typedef PACKED struct {
//		unsigned short index;
//		nv_tx_linearizer_type linearizer;
//} tx_lin_type;
//
//typedef PACKED struct {
//		unsigned short index;
//		int1 freq_comp_table[NV_FREQ_TABLE_SIZ];
//} tx_lin_vs_freq_type;


typedef PACKED struct
{
  word ftm_rf_cmd;
  /*  Rf sub-command enums from host */

  PACKED union 
  {
    word                    gen_w;      /* Generic word                      */
    byte                    gen_b;      /* Generic byte                      */
    dword                   gen_d;      /* Generic dword                     */
    word                    chan;       /* For tuning to a specific channel  */
    boolean                 on_off;     /* For anything with an on_off state */
    ftm_mode_id_type        mode;       /* For Setting Phone operation mode  */
    ftm_pa_range_type       range;      /* Set the LNA/PA range              */
    ftm_id_val_type         id_val;     /* For all indexed values            */
    ftm_byte_type           byte_struct;
#ifdef FEATURE_GSM
    gsm_option_type         gsm_struct;
#endif

	// Leon

	uint16 tx_lin_master[NV_WCDMA_NUM_TX_LINEARIZERS][NV_WCDMA_TX_LIN_MASTER_SIZ];
//	int8 tx_comp_vs_freq[NV_WCDMA_NUM_TX_LINEARIZERS][NV_FREQ_TABLE_SIZ];
//	int1 off_spn_limit_temp_table[2];			/* CDMA hdet off and span and limit vs temp table */
//	uint8 exp_hdet_vs_agc[NV_WCDMA_EXP_HDET_VS_AGC_SIZ]; /* WCDMA HDET vs limit tables */


	//++ WilliamC
	/*
	 *	User-defined  ftm rf factory data types
	 */
    //rx_lin_type rx_lin;							/* RX linearizer */
    //tx_lin_type tx_lin;				    		/* TX linearizer */
    
    //tx_lin_vs_freq_type tx_lin_vs_freq;			/* TX gain compensation versus frequency */
	int1 freq_comp_table[NV_FREQ_TABLE_SIZ]; 	/* Rx frequency compensation tables */
    //int1 fm_pwr_table[NV_PWR_TABLE_SIZ];		/* AMPS Power level tables */    
    //int1 hdet_agc_table[NV_WCDMA_EXP_HDET_VS_AGC_SIZ]; /* CDMA HDET vs limit tables */
    //WilliamC ++

  }  ftm_rf_factory_data;               /* union of RF commands data types   */
} ftm_rf_factory_parms;

typedef PACKED struct
{
  diagpkt_subsys_header_type header;
  ftm_rf_factory_parms rf_params;
}  ftm_pkt_type;

/* Structure to implement variable length command */
typedef PACKED struct{
    word  cmd_id;
    word  cmd_len;
    word  cmd_crc;
} ftm_cmd_header_type;

typedef PACKED struct
{
  int16 data[FTM_IQ_RECORD_BLOCK_SIZE];
  word recordIndex;
} ftm_iq_data_type;


//+ this part are based on 6250's ftm_rf_cmd.h

/*****************************************************
**
**     s U B - E NU M E R A T I O N
**
**====================================================
**
** This sub-enumeration is used to distinguish between different
** cdmaOne/CDMA2000/WCDMA and some of GSM tests
**
**/ 
  
typedef enum
{
  FTM_SET_PDM,              
  FTM_SET_PDM_CLOSE,        
  FTM_SET_TX_ON,            
  FTM_SET_TX_OFF,           
  FTM_SET_MODE = 7,             
  FTM_SET_CHAN,             
  FTM_GET_RSSI,             
  FTM_GET_SYNTH_STATE = 16,      
  FTM_SET_FM_TX_ST = 37,            
  FTM_SET_CDMA_CW_WAVEFORM = 50,     
  FTM_GET_CAGC_RX_AGC = 53,
  FTM_SET_PA_RANGE = 54,
  FTM_SET_LNA_RANGE = 58,
  FTM_GET_ADC_VAL = 59,         
  FTM_SET_LNA_OFFSET = 81,       
  FTM_SET_DVGA_OFFSET = 111, 
  FTM_GET_CDMA_IM2 = 114, 
  FTM_SET_FREQUENCY_SENSE_GAIN = 115, 
  FTM_TX_SWEEP_CAL = 116,       
  FTM_GET_DVGA_OFFSET = 117,
  FTM_GET_LNA_OFFSET = 118,
  FTM_GET_HDET = 119,
  FTM_GET_FM_IQ = 120,
  FTM_LNA_RANGE_OVERRIDE=121,
  FTM_SET_HDET_TRACKING = 124,

  //++ Leon
  /*
   *	QualComm document CL93-V5419-1 Rev. B states that Reserved values (0 to 1023) are used by QualComm without noitce
   *	Therefore, our user-defined FTM RF subcommands starts from 1024 or larger.
   */
  FTM_SET_TX_AGC_LOOP=1024,
  FTM_SET_TX_LIMIT=1025,
  FTM_GET_TX_AGC=1026,
  FTM_SET_DVGA_OFFSET_FREQ_COMP=1027,
  FTM_SET_TX_LIN=1028,
  FTM_SET_TX_FREQ_COMP=1029,
  FTM_SET_PA_ONOFF=1030,
  FTM_SET_TX_AGC=1031,
  FTM_SET_TEST_MODE=1032,
  FTM_GET_TEST_MODE=1033,
  FTM_SET_RF_BAND=1034,
  FTM_GET_BAND=1035,
  FTM_GET_RF_STATE=1036,
  FTM_SET_SLEEP=1037,
  FTM_SET_RX_AGC_LOOP = 1038,
  FTM_SET_TXRX_PWR_OFF = 1039,  
  //Leon ++

  FTM_ID_MAX 
} ftm_id_type;

/*****************************************************
**
**     s U B - E NU M E R A T I O N
**
**====================================================
**
** This sub-enumeration is used to distinguish between different
** GSM tests
**
**/ 

typedef enum{
  FTM_PEEK_DSP_INPORT = 200,		  /*   200  - retrieving the content of the QDSP3 MEMB                                */
  FTM_POKE_DSP_OUTPORT,           /*   201  - send user data to the QDSP3 MEMB                                        */
  FTM_SET_GRFC_ALL,               /*   202  - set all GRFCs (configured as output)                                    */
  FTM_GET_GRFC_ALL,               /*   203  - sense all GRFCs (configured as input/output)                            */
  FTM_GRFC_CONFIG_ALL,            /*   204  - configure the GRFC (timing, direction, polarity, mode)                  */
  FTM_SET_GRFC_DATA,              /*   205  - set a specific GRFC to the specified state (HI/LOW)                     */
  FTM_SET_GRFC_OMODE,             /*   206  - configure the output mode (static/timed) for a GRFC                     */
  FTM_SET_GRFC_TRISTATE,          /*   207  - tristate a specific GRFC                                                */
  FTM_SET_GRFC_POLARITY,          /*   208  - set a specific GRFC polarity (active HI/active LOW)                     */
  FTM_SET_SYNTH_ON,               /*   209  - turn RF synthesizer circuitry on                                        */

  FTM_SET_SYNTH_OFF,              /*   210  - turn RF synthesizer circuitry off                                       */
  FTM_SET_ANTENNA_CONFIG,         /*   211  - configure the antenna switch/duplexer for RX/TX operation (band)        */
  FTM_SET_PA_PROFILE,             /*   212  - download GSM PA power profile for specified power level                 */
  FTM_GET_PA_PROFILE,             /*   213  - retrieve GSM PA power profile for specified power level                 */
  FTM_GET_PA_CONFIG,              /*   214  - retrieve GSM PA configuartion                                           */
  FTM_CONF_MODULATE_DATA,         /*   215  - configure GMSK modulator                                                */
  FTM_SET_TRANSMIT_CONT,          /*   216  - transmit a tone (all 1's to GMSK modulator). Causes RF controls setup   */
  FTM_SET_TRANSMIT_BURST,         /*   217  - transmit user defined data pattern. Causes RF controls setup            */
  FTM_SET_VGA_GAIN_VAL,           /*   218  - program a gain value onto the VGA in RF module                          */
  FTM_SET_LNA_ON,                 /*   219  - turn LNA On                                                             */

  FTM_SET_LNA_OFF,                /*   220  - turns LNA off                                                           */
  FTM_SET_RX_ON,                  /*   221  - perform all operations required to turn on RX circuitry                 */
  FTM_SET_RX_OFF,                 /*   222  - perform all operations required to turn off RX circuitry                */
  FTM_GET_RX_DC_OFFSET,           /*   223  - get the residual DC reading from the RX samples                         */
  FTM_GET_RX_IQ_DATA,             /*   224  - receive a specified number of raw RX samples                            */
  FTM_SET_RX_BURST,               /*   225  - set up Rx burst type                                                    */
  FTM_CHFILT_CONFIG,              /*   226  - configure the channel filter                                            */
  FTM_SET_TX_POWER_LEVEL,         /*   227  - select TX power level by choosing a pre-configured PA LUT values        */
  FTM_GET_PHONE_STATE,            /*   228  - query the state of the phone, such as current channel, power level, etc */
  FTM_SET_GRFC_TIMING,            /*   229  - set the timing information for a GRFC (as well as TX/RX/General)        */
  FTM_GET_GSM_RSSI,               /*   230  - get raw RSSI value                                                      */
  
  FTM_SET_PA_START_DELTA,		      /*   231  - move the Pa ramp start time by the specified delta											*/
  FTM_SET_PA_STOP_DELTA,		      /*   232	- move the Pa ramp stop time by the specified delta  	  		              */ 
  FTM_SET_PA_DAC_INPUT,  		      /*   233  - set pa dac input                                                        */ 					 
  FTM_SET_RX_CONTINUOUS,          /*   234  - set Rx continuous                                                       */
	FTM_DO_GSM_TX_CAL_SWEEP,        /*   235  - Tx cal sweep                                                            */

  FTM_GSM_TEST_ID_MAX             /*   236  - illegal test ID                                                         */
} ftm_gsm_id_type;

typedef PACKED union 
{
  //ftm_cmd_acq_pilot_param_type   acq;   /* acquisition */
  //ftm_cmd_demod_fch_param_type   f_fch; /* forward FCH */
  //ftm_cmd_demod_dcch_param_type  f_dcch; /* forward DCCH */
  //ftm_cmd_demod_sch_param_type   f_sch; /* forward SCH */
  //ftm_cmd_mod_fch_param_type  r_fch;    /* reverse FCH */
  //ftm_cmd_mod_dcch_param_type r_dcch;    /* reverse DCCH */
  //ftm_cmd_mod_sch_param_type  r_sch;    /* reverse SCH */
  //ftm_cmd_mode_type mode;  /* change mode of phone */
  ftm_mode_type mode;
} ftm_packed_param_type; 

typedef enum
  {
#ifndef FEATURE_ADC_USE_API_EXCLUSIVELY
// For backwards compatibility
   ADC_VBATT,                // RAW reading: Main battery voltage at jack
   ADC_BATT_THERM,           // RAW reading: Main battery temperature
   ADC_VCHG,                 // RAW reading: Wall charger voltage at jack
   ADC_PA_THERM,             // RAW reading: PA thermistor
   ADC_PA_POWER_DETECT,      // RAW reading: HDET1: RF power detect
   ADC_GSM_BANDGAP_VREF,     // RAW reading: GSM Bandgap reference voltage
   ADC_MSM_THERM,            // RAW reading: MSM thermistor
#endif // FEATURE_ADC_USE_API_EXCLUSIVELY

                                   // ADIE_MUX_0 for the following PMIC muxed channels
   ADC_VCOIN_MV,                     // Coin cell battery/capacitor voltage
   ADC_VBATT_MV,                     // Main battery Voltage
   ADC_VCHG_MV,                      // Wall charger voltage at jack
   ADC_ICHG_MV,                      // Voltage across the charger current sense resistor
   ADC_ICHG_OUT_MV,                  // Handset supply voltage (Vdd)
   ADC_BATT_ID_MV,                   // Main battery ID voltage ???
   ADC_BATT_THERM_DEGC,              // Main battery temperature
   ADC_USB_VBUS_MV,                  // USB charger voltage at jack
   ADC_PMIC_THERM_DEGC,              // PMIC die temperature
   
   ADC_CHG_THERM,                  // Unconnected: Charger transistor temperature
   ADC_CHG_THERM_DEGC,             // Unconnected: Charger temperature
   ADC_SDRAM_THERM_DEGC,           // ADIE_MUX_1: SDRAM temperature
   ADC_PA_POWER_DETECT_10TH_DBM,   // ADIE_MUX_2: HDET1: RF power detect
   ADC_PA_THERM_DEGC,              // ADIE_MUX_3: RF power amplifier thermistor
   ADC_MSM_THERM_DEGC,             // Unconnected: MSM thermistor
                                   // ADIE_MUX_4: Not used
                                   // ADIE_MUX_5: HDET0: Not used
   ADC_GSM_BANDGAP_VREF_MV,        // ADIE_MUX_6: GSM Bandgap reference voltage


   //     !!!  Following are for ADC's INTERNAL use only  !!!
   //        !!!  Clients of ADC shall NEVER use them  !!!
   ADC_INTERNAL_USE_CHAN_1,
   ADC_INTERNAL_USE_CHAN_2,

   // Number of available logical ADC channels
   NUMBER_OF_LOGICAL_ADC_CHANNELS

  } adc_logical_channel_type;

/***********************************************/
/* Specifies the thermistor used by therm_read */
/* 6250 uses PA therm for RF compensation      */
/***********************************************/
#define ADC_THERM ADC_PA_THERM

#endif // _6250_FTM_CMD_H