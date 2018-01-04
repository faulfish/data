#ifndef RFNV_WCDMA_H
#define RFNV_WCDMA_H

/*===========================================================================

                      R F   N V   f o r   W C D M A

DESCRIPTION
  This header file contains definitions  to interface with RF NV for WCDMA.

Copyright (c) 1997 - 2003 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/rfnv_wcdma.h.-arc   1.1   Apr 01 2009 14:19:28   Kevin Tai  $
//Bonnie update 2005/07/15

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/19/04   dhh     Added tx delays/timing NV items.
08/21/03   ap      Final modifications to support WCDMA 2100/1900
06/17/03   eh      Initial Revision.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "nv.h"
//#include "rfnv.h"

/*===========================================================================
                        
                           CONSTANT DEFINITIONS

===========================================================================*/
/* Constant to make rfnv_wcdma_tx_lim_vs_temp[] back to 10-bits number. */
/* To save NV space, rfnv_wcdma_tx_lim_vs_temp[] is scaled to 8-bits */
/* since TX power limiting is only done on the top 25% of TX power */
#define RFNV_WCDMA_TX_LIM_OFFSET  768   

/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

/* WCDMA NV calibration table */
typedef struct {
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   VGA GAIN Offset NV items
   The description of these NV items can be found in the WCDMA ZIF RF NV items
   document.
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  int16 vga_gain_offset;

  int8 vga_gain_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  int16 vga_gain_offset_vs_temp[NV_TEMP_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
   IM2 NV items
   The description of these NV items can be found in the WCDMA ZIF RF NV items
   document.
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  uint8 im2_i_value;

  uint8 im2_q_value;

  uint8 im2_transconductor_value;
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     LNA related NV items

     LNA rise/fall (dBm to MSM register & NV storage)

        [ ( (dBm Rx + 106) / 85.333 ) * 1024 ] - 512

     LNA rise/fall (MSM register & NV storage to dBm)

        [ ( (value + 512) / 1024 ) * 85.333 ] - 106
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  
  /* LNA range rise value for LNA to switch from high to mid gain state */
  int16 lna_range_rise;

  /* LNA range rise value for LNA to switch from mid to low gain state */
  int16 lna_range_rise_2;

  /* LNA range rise value for Mixer to switch from high to low gain state */
  int16 lna_range_rise_3;
  
  /* LNA range fall value for LNA to switch from mid to high gain state */
  int16 lna_range_fall;

  /* LNA range fall value for LNA to switch from low to mid gain state*/
  int16 lna_range_fall_2;

  /* LNA range fall value for Mixer to switch from low to high gain state */
  int16 lna_range_fall_3;
  
  /* IM threshold level for LNA to switch from high to mid gain state */
  int16 im_level;

  /* IM threshold level for LNA to switch from mid to low gain state */
  int16 im_level_2;

  /* IM threshold level for Mixer to switch from high to low gain state */
  int16 im_level_3;
  
  /* LNA non-bypass hold timer from low to high gain - multiples of 66us */
  uint8 nonbypass_timer;

  /* LNA bypass hold timer from high to low gain - multiples of 6.6ms */
  uint16 bypass_timer;

  /* LNA range offset for LNA from high to mid gain */
  int16 lna_range_offset;

  /* LNA range offset for LNA from mid to low gain */
  int16 lna_range_offset_2;

  /* LNA range offset for Mixer from high to low gain */
  int16 lna_range_offset_3;
  
  /* LNA bypass range offset vs. freq. compensation */
  int8 lna_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  /* LNA bypass range offset vs. freq. compensation */
  int8 lna_offset_vs_freq_2[NV_FREQ_TABLE_SIZ];

  /* LNA bypass range offset vs. freq. compensation */
  int8 lna_offset_vs_freq_3[NV_FREQ_TABLE_SIZ];
  
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Values to limit the RX AGC output
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Minimum output from the RX AGC_VALUE accumulator for LNA mid gain */
  int16 rx_agc_min;

  /* Minimum output from the RX AGC_VALUE accumulator for LNA low gain */
  int16 rx_agc_min_2;

  /* Minimum output from the RX AGC_VALUE accumulator for Mixer low gain */
  int16 rx_agc_min_3;
  
  /* Maximum output from the RX AGC_VALUE accumulator */
  int16 rx_agc_max;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    LNA phase adjustment
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Phase rotation setting for LNA decision */
  uint8 agc_phase_offset;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Tx AGC linearizer NV items
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Tx linearizer master offset, 37 offsets per PA state */
  uint16 tx_lin_master[RFLIN_NUM_TX_LINEARIZERS][NV_WCDMA_TX_LIN_MASTER_SIZ];

  /* Tx gain compensation versus frequency */
  int8 tx_comp_vs_freq[RFLIN_NUM_TX_LINEARIZERS][NV_FREQ_TABLE_SIZ];

  /* Tx linearizer versus temperature */
  int16 tx_lin_vs_temp[RFLIN_NUM_TX_LINEARIZERS][NV_TEMP_TABLE_SIZ];

  /* Tx linearizer slope versus temperature */
  int16 tx_slp_vs_temp[RFLIN_NUM_TX_LINEARIZERS][NV_TEMP_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     PA related NV items
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* PA_R1 rise value for PA switch threshold from Linearizer gain state 0 to 1 */
  uint16 r1_rise;

  /* PA_R1 fall value for PA switch threshold from Linearizer gain state 1 to 0 */
  uint16 r1_fall;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Tx power limit NV items
     Tx power limit data vs temperature, Tx power limit vs frequency, 
     TX_AGC_ADJ vs HDET, and HDET offset and span 
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Tx power limit versus temperature */
  /* This table contain absolute TX power limit, NOT DELTA */
  uint8 tx_lim_vs_temp[NV_TEMP_TABLE_SIZ];

  /* Tx UTRAN Tx Power Limit Compensation */
  int16 utran_tx_lim_vs_temp_offset[NV_TEMP_TABLE_SIZ];

  /* Tx power limit versus frequency */
  int16 tx_lim_vs_freq[NV_FREQ_TABLE_SIZ];

  /* CDMA Tx power limit loop gain, scaled by RF_WCDMA_TX_LIM_ADJ_SCALE. */
  /* stored in NV as NV_WCDMA_ADJ_FACTOR */
  uint8 tx_lim_adj_gain;

  /* Expected HDET versus AGC */
  /* 16+1 to allow slope calculation for last index */
  uint8 exp_hdet_vs_agc[NV_WCDMA_EXP_HDET_VS_AGC_SIZ + 1];

  /* ADC HDET reading offset */
  uint8 hdet_off;

  /* ADC HDET reading span */
  uint8 hdet_spn;   

  /* UE maximum TX power */
  int8  max_tx_power;

  int16 out_of_service_thresh;

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     BTF Delay for RF path
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* CHIPX8 delay for SYNC80M, biased +25.
     This is the RF (RX+TX) path delay from the antenna to the IQ digital MSM pins */
  uint32 enc_btf_dly;

  /* RX path delay */
  int16  rx_delay;
 
  /* tx delays and update agc time */
  uint16 agc_pa_on_rise_delay;
  uint16 agc_pa_on_fall_delay;
  uint16 agc_tx_on_rise_delay;
  uint16 agc_tx_on_fall_delay;
  uint16 agc_update_tx_agc_time;

  /* pa range update time */
  uint16 pa_gain_up_time;
  uint16 pa_gain_down_time;
 
  /* tx rotator angles */
  uint16 tx_rot_ang_pa_00;
  uint16 tx_rot_ang_pa_01;
  uint16 tx_rot_ang_pa_10;
  uint16 tx_rot_ang_pa_11; 

  uint8 rfr_vco_coarse_tuning[12];
 
  /* PA Compensation UP for PA switch threshold from Linearizer gain state 0 to 1 */
  int16 pa_compensate_up; //uint16 pa_compensate_up;  
 
  /* PA Compensation DOWN for PA switch threshold from Linearizer gain state 1 to 0 */
  int16 pa_compensate_down; //uint16 pa_compensate_down;
 
  /* PRACH Switch point from Linearizer gain state 0 to 1 */
  int16 prach_r1_rise_offset;
 
  /* PRACH Switch point from Linearizer gain state 1 to 0 */
  int16 prach_r1_fall_offset;
 
  /* Cal channel list, reserve RF chain index for future use. 1 chain used. */
  uint16 tx_cal_chan[1][NV_FREQ_TABLE_SIZ];
  uint16 rx_cal_chan[1][NV_FREQ_TABLE_SIZ];
 
  uint32 tx_cal_chan_freq[1][NV_FREQ_TABLE_SIZ];
  uint32 rx_cal_chan_freq[1][NV_FREQ_TABLE_SIZ];
 
  boolean interpolation_enable; 
 
  /* No. of Tx linearizer state is increased to 4 */
 
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     PA related NV items
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* PA_R2 rise value for PA switch threshold from Linearizer gain state 1 to 2 */
  uint16 r2_rise;
 
  /* PA_R2 fall value for PA switch threshold from Linearizer gain state 2 to 1 */
  uint16 r2_fall;
 
  /* PA_R3 rise value for PA switch threshold from Linearizer gain state 2 to 3 */
  uint16 r3_rise;
 
  /* PA_R3 fall value for PA switch threshold from Linearizer gain state 3 to 2 */
  uint16 r3_fall;
 
 
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     PA Compensate NV items
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* PA Compensation UP for PA switch threshold from Linearizer gain state 1 to 2 */
  uint16 pa_compensate_up_r2;
 
  /* PA Compensation DOWN for PA switch threshold from Linearizer gain state 2 to 1 */
  uint16 pa_compensate_down_r2;
 
  /* PA Compensation UP for PA switch threshold from Linearizer gain state 2 to 3 */
  uint16 pa_compensate_up_r3;
 
  /* PA Compensation DOWN for PA switch threshold from Linearizer gain state 3 to 2 */
  uint16 pa_compensate_down_r3;
 
 
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     PRACH Switch point NV items
  - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */ 
 
  /* PRACH Switch point from Linearizer gain state 1 to 2 */
  int16 prach_r2_rise_offset;
 
  /* PRACH Switch point from Linearizer gain state 2 to 1 */
  int16 prach_r2_fall_offset;
 
  /* PRACH Switch point from Linearizer gain state 2 to 3 */
  int16 prach_r3_rise_offset;
 
  /* PRACH Switch point from Linearizer gain state 3 to 2 */
  int16 prach_r3_fall_offset;
 
  /* PA Range Map */
  int8 pa_range_map[RFLIN_NUM_TX_LINEARIZERS];
 
} rfnv_wcdma_nv_tbl_type;

/* pointer to WCDMA NV table */

#endif /* RFNV_WCDMA_H */

