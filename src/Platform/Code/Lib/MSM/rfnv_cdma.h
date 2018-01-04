#ifndef RFNV_CDMA_H
#define RFNV_CDMA_H

/* EJECT  */
/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/


/*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=

                    SCALE FACTORS FOR FIXED POINT MATH

=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*/

/* Scale factor for raw FM RSSI values
*/
#define RF_FM_RSSI_SCALE      ((word)100)

/* Scale factor for FM transmit agc initial setting vs. power level
*/
#define RF_FM_AGC_PWR_SCALE             2

/* Scale factor for the CDMA transmit power limiter loop gain
*/
#define RF_CDMA_TX_LIM_ADJ_SCALE        8

#ifdef FEATURE_P1_DYNAMIC_PA_SWITCH_POINT_ADJUST
#define NV_P1_SP_OFFSET_TABLE_SIZ       8
#endif 


#define RF_NUM_TX_LINEARIZERS  4

#define RF_NUM_PA_OFFSETS      NV_NUM_PA_RANGES

#ifdef RF_HAS_LINEAR_INTERPOLATION
#define NV_MAX_NUM_CAL_CHAN    16
#endif

#ifdef FEATURE_TX_POWER_BACK_OFF
  typedef enum
  {
    PILOT_FCH                   = 0,  
    PILOT_DCCH_9600             = 1,
    NUMBER_OF_BACKOFF_CONFIGS,
    BACKOFF_STEADY_STATE        = 1000,
    BACKOFF_FIRST_RUN           = 2000,
    REFRESH_BACKOFF             = 3000,
    NO_BACK_OFF_CONFIG          = 4000
  } rfnv_backoff_chan_config_type ;
#endif

/* -----------------------------------------------------------------------
** FM NV calibration tables
** ----------------------------------------------------------------------- */
typedef struct {

  /* FM mode TX_AGC_ADJ initial setting vs power table
  */
  int2 rfnv_fm_agc_set_vs_pwr[NV_PWR_TABLE_SIZ];

  /* FM mode TX_AGC_ADJ initial setting vs frequency compensation table
  */
  int1 rfnv_fm_agc_set_vs_freq[NV_FREQ_TABLE_SIZ];

  /* FM mode TX_AGC_ADJ initial setting vs temperature compensation table
  */
  int2 rfnv_fm_agc_set_vs_temp[NV_TEMP_TABLE_SIZ];

  /* FM AGC adjust vs frequency table
  */
  int1 rfnv_fm_agc_adj_vs_freq[NV_FREQ_TABLE_SIZ];

  /* FM expected HDET vs temperature table
  */
  byte rfnv_fm_exp_hdet_vs_temp[NV_TEMP_TABLE_SIZ];

  /* FM expected HDET vs power level table
  */
  byte rfnv_fm_exp_hdet_vs_pwr[NV_PWR_TABLE_SIZ];

  /* FM error slope vs power level table
  */
  byte rfnv_fm_err_slp_vs_pwr[NV_PWR_TABLE_SIZ];

  /* FM AGC underflow limit
  */
  byte rfnv_dfm_agc_min;

  /* FM AGC overflow limit
  */
  byte rfnv_dfm_agc_max;

  /* FM Frequency-Sense-Gain (Tx Gain)
  */
  byte rfnv_fm_freq_sense_gain;

  /* FM RSSI offset
  */
  word rfnv_fm_rssi_raw_offset;

  /* FM RSSI span
  */
  word rfnv_fm_rssi_raw_span;

  /* New DFM Block */
  int1    rfnv_lna_fall;
  int2    rfnv_lna_offset;
  int1    rfnv_lna_rise;
  int1    rfnv_lna_offset_vs_freq[NV_FREQ_TABLE_SIZ];
  word    rfnv_lna_bypass_timer;
  word    rfnv_lna_nonbypass_timer;
  byte    rfnv_lna_follower_delay;
  word    rfnv_lna_range_delay;
  byte    rfnv_lna_decision_delay;
  byte    rfnv_lna_polarity;
  byte    rfnv_agc_acc_min_1;
  byte    rfnv_agc_im_gain;
  byte    rfnv_agc_dc_gain;
  int1    rfnv_im_level1;
  byte    rfnv_fm_pa_mac_high;  

  /* New Rx Front items */
  int2    rfnv_vga_gain_offset;
  int1    rfnv_vga_gain_offset_vs_freq[NV_FREQ_TABLE_SIZ];
  int2    rfnv_vga_gain_offset_vs_temp[NV_TEMP_TABLE_SIZ];
  int32   rfnv_mis_comp_a_coeff;            
  int32   rfnv_mis_comp_b_coeff;            
  word    rfnv_phase_del;
  word    rfnv_dacc_iaccum0;
  word    rfnv_dacc_iaccum1;
  word    rfnv_dacc_iaccum2;
  word    rfnv_dacc_iaccum3;
  word    rfnv_dacc_iaccum4;
  word    rfnv_dacc_qaccum0;
  word    rfnv_dacc_qaccum1;
  word    rfnv_dacc_qaccum2;
  word    rfnv_dacc_qaccum3;
  word    rfnv_dacc_qaccum4;

} rfnv_fm_items_type;

/* -----------------------------------------------------------------------
** Digital (CDMA/PCS) NV calibration tables
** ----------------------------------------------------------------------- */
typedef struct {

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Values to limit the AGC output
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Were we able to read the limit Rx AGC values out of NV?
  */
  boolean rfnv_rx_agc_lim_avail;

  /* Maximum output from the AGC_VALUE accumulator
  */
  int1 rfnv_rx_agc_max;

  /* Minimum output from the AGC_VALUE accumulator when:   
   *    - one LNAs is bypassed in 3 stage LNA control 
   *    - in LNA Gain State 1 when in 5 stage LNA control
  */
  int1 rfnv_rx_agc_min;

  /* Minimum output from the AGC_VALUE accumulator when :
   *    - both LNAs are bypassed in 3 stage LNA control 
   *    - LNA Gain State 2 when in 5 stage LNA control
   */
  int1 rfnv_rx_agc_min_11;

  /* Minimum output from the AGC_VALUE accumulator when :
   *    - LNA Gain State 3 when in 5 stage LNA control
  */
  int1 rfnv_rx_agc_min_3;

   /* Minimum output from the AGC_VALUE accumulator when :
   *    - LNA Gain State 4 when in 5 stage LNA control
   */
  int1 rfnv_rx_agc_min_4;

  byte rfnv_pa_range_step_cal;
   
  /* PDM1 frequency compensation table
  */
  int1 rfnv_pdm1_vs_freq[NV_FREQ_TABLE_SIZ];

  /* PDM2 frequency compensation table
  */
  int1 rfnv_pdm2_vs_freq[NV_FREQ_TABLE_SIZ];  

  /* PA offsets */
  int2 rfnv_pa_offsets[RF_NUM_PA_OFFSETS];

  /* Rx linearizer offsets.  There is one extra segment to allow for
  ** extrapolation.
  */
  byte rfnv_cdma_rx_lin_off[NV_CDMA_RX_LIN_SIZ+1];

  /* Rx Linearizer slopes.
  */
  byte rfnv_cdma_rx_lin_slp[NV_CDMA_RX_LIN_SIZ];

  /* Rx Linearizer versus temperature table.
  */
  int2 rfnv_cdma_rx_lin_vs_temp[NV_TEMP_TABLE_SIZ];

  /* Rx slope versus temperature table.
  */
  int2 rfnv_cdma_rx_slp_vs_temp[NV_TEMP_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode RX/TX_GAIN_COMP values vs frequency
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* Rx gain compensation versus frequency.
  */
  int1 rfnv_cdma_rx_comp_vs_freq[NV_FREQ_TABLE_SIZ];

  /* Tx gain compensation versus frequency
  */
  int1 rfnv_cdma_tx_comp_vs_freq[RF_NUM_TX_LINEARIZERS][NV_FREQ_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode Tx AGC linearization (MSM2P data is derived from these tables)
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* CDMA Tx linearizer master offset
  */
  int2 rfnv_cdma_tx_lin_master_off[RF_NUM_TX_LINEARIZERS][NV_CDMA_TX_LIN_MASTER_SIZ];

  /* CDMA Tx linearizer versus temperature
  */
  int2 rfnv_cdma_tx_lin_vs_temp[RF_NUM_TX_LINEARIZERS][NV_TEMP_TABLE_SIZ];

  /* CDMA Tx slope versus temperature
  */
  int2 rfnv_cdma_tx_slp_vs_temp[RF_NUM_TX_LINEARIZERS][NV_TEMP_TABLE_SIZ];


  /* PA range versus temperature
  */
  int2 rfnv_pa_range_vs_temp[NV_TEMP_TABLE_SIZ];

  /* PDM1 versus tempearture
  */
  int2 rfnv_pdm1_vs_temp[NV_TEMP_TABLE_SIZ];

  /* PDM2 versus tempearture
  */
  int2 rfnv_pdm2_vs_temp[NV_TEMP_TABLE_SIZ];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode Tx power limit data vs temperature,
     frequency, TX_AGC_ADJ PDM setting, and HDET reading
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* CDMA Tx limit versus temperature
  */
  int2 rfnv_cdma_tx_lim_vs_temp[NV_TEMP_TABLE_SIZ];

  /* CDMA Tx limit versus frequency
  */
  int1 rfnv_cdma_tx_lim_vs_freq[NV_FREQ_TABLE_SIZ];

  /* CDMA expected HDET versus AGC
  */
  byte rfnv_cdma_exp_hdet_vs_agc[NV_CDMA_EXP_HDET_VS_AGC_SIZ+1];

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     ADC offset and gain values for calculating indices to the above tables
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* ADC HDET reading offset
  */
  byte rfnv_hdet_off;

  /* ADC HDET reading span
  */
  byte rfnv_hdet_spn;   /* ADC HDET reading span */

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     CDMA mode LNA on/off thresholds

     LNA rise/fall (dBm to MSM register & NV storage)

        [ (dBm Rx + 106) / 85.333 * 256 ] - 128

     LNA rise/fall (MSM register & NV storage to dBm)

        [ (value + 128) / 256 * 85.333 ] - 106
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* LNA range rise value
  */
  int1 rfnv_lna_range_rise;     /* equivalent to -80dBm input */

  /* LNA range fall value
  */
  int1 rfnv_lna_range_fall;     /* equivalent to -90dBm input */

  /* LNA range offset
  */
  int2 rfnv_lna_range_offset;   /* equivalent to 16dB (180 => 15dB) */

  /* 2nd LNA range rise value
  */
  int1 rfnv_lna_range_2_rise;

  /* 2nd LNA range fall value
  */
  int1 rfnv_lna_range_2_fall;

  /* 2nd LNA bypass range offset
  */
  int2 rfnv_lna_range_12_offset;

  /* 3rd LNA range rise value
  */
  int1 rfnv_lna_range_3_rise;

  /* 3rd LNA range fall value
  */
  int1 rfnv_lna_range_3_fall;

  /* 3rd LNA bypass range offset
  */
  int2 rfnv_lna_range_3_offset;

  /* 4th LNA range rise value
  */
  int1 rfnv_lna_range_4_rise;

  /* 4th LNA range fall value
  */
  int1 rfnv_lna_range_4_fall;

  /* 4th LNA bypass range offset
  */
  int2 rfnv_lna_range_4_offset;

  /* Intermod Level 1
  */
  int1 rfnv_im_level1;

  /* Intermod Level 2
  */
  int1 rfnv_im_level2;

  /* Intermod Level 3
  */
  int1 rfnv_im_level3;

  /* Intermod Level 4
  */
  int1 rfnv_im_level4;

  /* LNA non-bypass hold timer - multiples of 52us
  */
  byte rfnv_nonbypass_timer;

  /* LNA bypass hold timer - multiples of 6.6ms
  */
  byte rfnv_bypass_timer;

  /* LNA bypass timer 0 (5 Stage LNA Control) - multiples of 26us
  */
  int2 rfnv_bypass_timer_0;

  /* LNA non bypass timer 0 (5 Stage LNA Control) - multiples of 26us
  */
  int2 rfnv_non_bypass_timer_0;

  /* LNA bypass timer 1
  */
  int2 rfnv_bypass_timer_1;

  /* LNA non bypass timer 1
  */
  int2 rfnv_non_bypass_timer_1;

  /* LNA bypass timer 2
  */
  int2 rfnv_bypass_timer_2;

  /* LNA non bypass timer 2
  */
  int2 rfnv_non_bypass_timer_2;

  /* LNA bypass timer 3
  */
  int2 rfnv_bypass_timer_3;

  /* LNA non bypass timer 3
  */
  int2 rfnv_non_bypass_timer_3;

  /* 1st LNA bypass range offset vs. freq. compensation
  */
  int1 rfnv_cdma_lna_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  /* 2nd LNA bypass range offset vs. freq. compensation
  */
  int1 rfnv_cdma_lna_12_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  /* 3rd LNA bypass range offset vs. freq. compensation
  */
  int1 rfnv_cdma_lna_3_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  /* 4th LNA bypass range offset vs. freq. compensation
  */
  int1 rfnv_cdma_lna_4_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  /* Phase rotation setting for LNA Stages 2 to 5
  */
  byte rfnv_agc_phase_offset;

  /* LNA range polarity
  */
  byte rfnv_lna_range_pol;

  /* Mixer range polarity
  */
  byte rfnv_mixer_range_pol;
  
  /* LNA range delay time from CAGC to RX Front
  */
  byte rfnv_lna_range_delay;

  /* LNA range ON value
  */
  byte rfnv_lna_range_on;

  /* LNA range OFF value
  */
  byte rfnv_lna_range_off;

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
     Miscellaneous other NV items
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

  /* CDMA Tx limiter loop gain, scaled by RF_CDMA_TX_LIM_ADJ_SCALE.
  */
  int2 rfnv_cdma_tx_lim_adj_gain;

  /* CDMA/PCS TX Attenuation Limit */
  int2 rfnv_cdma_tx_atten_limit;
  
  /* PA_R1 rise value
  */
  byte rfnv_r1_rise;       /* approximately - 4dBm */

  /* PA_R1 fall value
  */
  byte rfnv_r1_fall;       /* approximately - 8dBm */

  /* PA_R2 rise value
  */
  byte rfnv_r2_rise;       /* disabled             */

  /* PA_R2 fall value
  */
  byte rfnv_r2_fall;       /* disabled             */

  /* PA_R3 rise value
  */
  byte rfnv_r3_rise;       /* approximately +17dBm */

  /* PA_R3 fall value
  */
  byte rfnv_r3_fall;       /* disabled             */


#ifdef FEATURE_P1_DYNAMIC_PA_SWITCH_POINT_ADJUST
  /* P1 PA Rise/Fall adjustments  
  */
  byte rfnv_p1_pa_rise_fall_off[ NV_P1_SP_OFFSET_TABLE_SIZ ];
#endif /* FEATURE_P1_DYNAMIC_PA_SWITCH_POINT_ADJUST */

  /* CHIPX8 delay for SYNC80M, biased +25.
     This is the RF path delay from the antenna to the IQ
     digital MSM pins */
  uint32 rfnv_enc_btf_dly;

#ifdef FEATURE_SUBPCG_PA_WARMUP_DELAY
  int2 rfnv_subpcg_pa_warmup_delay ;
#endif

#ifdef FEATURE_TX_POWER_BACK_OFF
  /* dB backoff for low and medium voltage as a function of
  ** channel configuration. 
  ** This table tracks enumeration type rfnv_backoff_chan_config_type
  ** Contents are dB values, scaled 3
  */
  uint8 rfnv_chan_config_backoff_med[ NV_TX_BACKOFF_TABLE_SIZE ] ;
  uint8 rfnv_chan_config_backoff_low[ NV_TX_BACKOFF_TABLE_SIZE ] ;

  // Low, med, and hi volts for PA backoff calibration.
  uint8 rfnv_pa_backoff_cal_volts[ 3 ] ;

  // Volts*10 for upper and lower Vbatt calibration.
  nv_minmax_type rfnv_vbatt_cal_volts ;
#endif
  /* ---------------------- New RF NV Items ------------------------------ */
    
  /* RxFront Digital items */
  
  int2    rfnv_vga_gain_offset;
  int1    rfnv_vga_gain_offset_vs_freq[NV_FREQ_TABLE_SIZ];
  int2    rfnv_vga_gain_offset_vs_temp[NV_TEMP_TABLE_SIZ];
  int32   rfnv_mis_comp_a_coeff;            
  int32   rfnv_mis_comp_b_coeff;            
  word    rfnv_dacc_iaccum0;
  word    rfnv_dacc_iaccum1;
  word    rfnv_dacc_iaccum2;
  word    rfnv_dacc_iaccum3;
  word    rfnv_dacc_iaccum4;
  word    rfnv_dacc_qaccum0;
  word    rfnv_dacc_qaccum1;
  word    rfnv_dacc_qaccum2;
  word    rfnv_dacc_qaccum3;
  word    rfnv_dacc_qaccum4;

  /* RF Chipset */
  byte    rfnv_im2_i_value;
  byte    rfnv_im2_q_value;
  byte    rfnv_im2_transconductor_value;

#ifdef RF_HAS_LINEAR_INTERPOLATION
  uint2    rfnv_tx_cal_channels[ NV_MAX_NUM_CAL_CHAN];
  uint2    rfnv_rx_cal_channels[ NV_MAX_NUM_CAL_CHAN];
#endif /* RF_HAS_LINEAR_INTERPOLATION */

  /* ---------------------- End of new RF NV Items ----------------------- */
} rfnv_digital_items_type;

/* -----------------------------------------------------------------------
** Common NV calibration tables
** ----------------------------------------------------------------------- */
typedef struct {
    
  /* General RF Items */    
  
  byte    rfnv_test_code_ver[NV_SIZE_OF_VERSION];
  byte    rfnv_sys_sw_ver[NV_SIZE_OF_VERSION];
  byte    rfnv_rf_cal_ver[NV_SIZE_OF_VERSION];
  byte    rfnv_rf_config_ver[NV_SIZE_OF_VERSION];
  dword   rfnv_rf_cal_date;
  dword   rfnv_rf_nv_loaded_date;
  byte    rfnv_rf_cal_dat_file[NV_QC_SERIAL_NUM_LEN];
  
  /* RX Front items */
  dword   rfnv_dacc_dc_offset_polarity;       

  /* RF Chipset items */
  byte    rfnv_rtc_time_adjust;
  byte    rfnv_digital_pll_lock_timer;

  /* CAGC dynamic range items */
  int2    rfnv_cdma_min_rx_rssi;
  int2    rfnv_cdma_dynamic_range;    

  /* TX_Warmup Duration       */
  word    tx_warmup;

  byte    rfnv_vco_coarse_tune_table[NV_VCO_COARSE_TUNE_TABLE_SIZ];

  byte    rfnv_rfr_bb_filter;  

} rfnv_common_items_type;


/*===========================================================================

FUNCTION RFNV_FM_TABLE                                      REGIONAL FUNCTION

DESCRIPTION
  Returns pointer to FM table.

DEPENDENCIES
  None

RETURN VALUE
  Returns pointer to FM table.

SIDE EFFECTS
  None

===========================================================================*/
//rfnv_fm_items_type *rfnv_fm_table( void);


/*===========================================================================

FUNCTION RFNV_DIGITAL_TABLE                                 REGIONAL FUNCTION

DESCRIPTION
  Returns pointer to digital table.

DEPENDENCIES
  None

RETURN VALUE
  Returns pointer to digital table.

SIDE EFFECTS
  None

===========================================================================*/
//rfnv_digital_items_type *rfnv_digital_table( void);


#endif