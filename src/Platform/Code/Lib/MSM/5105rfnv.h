#ifndef _5105_RFNV_H
#define _5105_RFNV_H

/* EJECT  */
/*===========================================================================

                         R F   N V   M a n a g e r

DESCRIPTION
  This header file contains definitions necessary inorder to interface with
  the RF NV Manager.

Copyright (c) 1997, 1998, 1999 by QUALCOMM, Inc.  All Rights Reserved.
Copyright (c) 2000             by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/5105rfnv.h.-arc   1.1   Apr 01 2009 14:19:22   Kevin Tai  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
07/22/02   fas     Remove #if 1 and #if 0.
07/11/02   fas     Remove reference to RFR3100.
01/30/02   fas     Restructured enumerated type rfnv_backoff_chan_config_type.
05/25/01   fas     Renamed NV items for rf_pa_backoff.
05/23/01   fas     Added data for rf_pa_backoff.
03/08/01   tn      Wrapped FEATURE_SUBPCG_PA_WARMUP_DELAY around rfnv_subpcg_pa_warmup_delay.
02/28/01   fas     Added IS2000 TX AGC item, rfnv_subpcg_pa_warmup_delay.
11/02/00   dd      Changed Trimode support to be conditional based on 
                   FEATURE_TRIMODE_ITEMS instead of RF_HAS_TRIMODE. 
10/18/00   fas     Copied entirely from PPG:
				   L:/src/asw/MSM3300/VCS/rfnv.h_v   1.4   29 Sep 2000 15:11:00   dbrackma
10/06/00   dsb     Removed TG== code.
09/29/00   dsb     Added support for FEATURE_ENC_BTF_IN_NV.
05/04/00   ymc     Renamed FEATURE_PA_RANGE_DIG_COMP as 
                   FEATURE_PA_RANGE_TEMP_FREQ_COMP.
04/14/00   ymc     Removed rfnv_pa_offset_vs_temp[][] and rfnv_pa_offset_vs_freq[][].
                   Made the following RF NV structure entries as 2-dim. arrays:
                   rfnv_cdma_tx_comp_vs_freq[][], rfnv_cdma_tx_lin_vs_temp[][],
                   rfnv_cdma_tx_slp_vs_temp[][].  This is to support 
                   FEATURE_4PAGE_TX_LINEARIZER.
03/22/00   ymc     Added support for features of FEATURE_4PAGE_TX_LINEARIZER
                   and FEATURE_PA_RANGE_DIG_COMP.
03/01/00   ymc     Added RFR3100 NV items.
11/19/99   ram     Removed externalized functions that are not needed anymore.
10/11/99   rv      Added TRIMODE support.
04/18/99   snn     For pa_range_vs_step_cal, pdm1_vs_freq, pdm2_vs_freq, pdm1_vs_temp
                   and pdm2_vs_temp, removed them from Target related compile_switches
                   to Feature related compile switches.
02/17/99   ychan   Modified return type of function prototype of rfnv_get_nv_lst_addr.
01/06/99   ychan   Modified for targets NGP, SURF, Q-1900.
01/06/99   ychan   Updated Copyright date.
10/14/98   thh     Added frequency compensation support for PDM1 and PDM2.
06/21/98   jjn     Update from code review.
06/05/98   thh     Targetized calibrated PA range step for applicable targets.
                   #ifdef PDM1 and PDM2 per RF capabilities.
05/05/98   jjn     Added support for QCT-7x00.  QCT-7x00 uses RF NV, unlike
                   all previous WLL targets.
03/17/98   thh     Added support for Rx AGC limit and PDM1 and PDM2.
02/08/98   jjn     Renamed rfnv_config_loaded to rf_rfcal_valid, since this
                   boolean is really to verify that RF CAL is valid.  Also,
                   this boolean is used with TX_LIM_VS_TEMP for WLL targets.
                   So, it would have had an inaccurate name.
11/17/97   jjn     Partitioned ISS2 RF to conform to the new RF architecture
09/04/97   thh     Initial version

===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "target.h"
#include "rficap.h"
#include "comdef.h"
#include "rfc.h"
#include "nv.h"
#include "nv_items.h"
#include "rfnv_wcdma.h"
#include "rfnv_cmn.h"
#include "rfnv_gsm.h"

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

#ifndef FEATURE_4PAGE_TX_LINEARIZER
#define RF_NUM_TX_LINEARIZERS  1
#else
#define RF_NUM_TX_LINEARIZERS  NV_NUM_TX_LINEARIZERS
#endif

#ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP
#define RF_NUM_PA_OFFSETS      NV_NUM_PA_RANGES
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

} rfnv_fm_items_type;

/* -----------------------------------------------------------------------
** Digital (CDMA/PCS) NV calibration tables
** ----------------------------------------------------------------------- */
typedef struct {

#ifdef RF_LIMIT_RX_AGC_OUTPUT
  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    Values to limit the AGC output
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
  /* Were we able to read the limit Rx AGC values out of NV?
  */
  boolean rfnv_rx_agc_lim_avail;

  /* Maximum output from the AGC_VALUE accumulator
  */
  int1 rfnv_rx_agc_max;

  /* Minimum output from the AGC_VALUE accumulator
  */
  int1 rfnv_rx_agc_min;

#ifdef RF_HAS_RFR3100
  /* Minimum output from the AGC_VALUE accumulator when both LNAs are bypassed */
  int1 rfnv_rx_agc_min_11;
#endif

#endif

  /* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    PDM1 and PDM2
   - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifdef RF_HAS_PA_RANGE_CAL
  /* PA range step calibrated value
  */
  byte rfnv_pa_range_step_cal;

#else
  #ifdef FEATURE_PA_RANGE_TEMP_FREQ_COMP

    /* PA offsets */
    int2 rfnv_pa_offsets[RF_NUM_PA_OFFSETS];

  #else

    #ifdef RF_CAP_PDM1_FOR_TX
      /* PDM1 frequency compensation table
      */
      int1 rfnv_pdm1_vs_freq[NV_FREQ_TABLE_SIZ];
    #endif

    #ifdef RF_CAP_PDM2_FOR_TX
      /* PDM2 frequency compensation table
      */
      int1 rfnv_pdm2_vs_freq[NV_FREQ_TABLE_SIZ];
    #endif

  #endif   /* FEATURE_PA_RANGE_TEMP_FREQ_COMP */
#endif /*  RF_HAS_PA_RANGE_CAL */

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

#ifdef  RF_HAS_PA_RANGE_CAL
  /* PA range versus temperature
  */
  int2 rfnv_pa_range_vs_temp[NV_TEMP_TABLE_SIZ];
#elif (!defined(FEATURE_PA_RANGE_TEMP_FREQ_COMP))

#ifdef RF_CAP_PDM1_FOR_TX
  /* PDM1 versus tempearture
  */
  int2 rfnv_pdm1_vs_temp[NV_TEMP_TABLE_SIZ];
#endif /* RF_CAP_PDM1_FOR_TX */

#ifdef RF_CAP_PDM2_FOR_TX
  /* PDM2 versus tempearture
  */
  int2 rfnv_pdm2_vs_temp[NV_TEMP_TABLE_SIZ];
#endif /* RF_CAP_PFM2_FOR_TX */

#endif /*  RF_HAS_PA_RANGE_CAL */

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


#ifdef RF_HAS_RFR3100

  /* 2nd LNA range rise value
  */
  int1 rfnv_lna_range_2_rise;

  /* 2nd LNA range fall value
  */
  int1 rfnv_lna_range_2_fall;

  /* 2nd LNA bypass range offset
  */
  int2 rfnv_lna_range_12_offset;

  /* Intermod Level 1
  */
  int1 rfnv_im_level1;

  /* Intermod Level 2
  */
  int1 rfnv_im_level2;

  /* LNA non-bypass hold timer - multiples of 52us
  */
  byte rfnv_nonbypass_timer;

  /* LNA bypass hold timer - multiples of 6.6ms
  */
  byte rfnv_bypass_timer;

  /* 1st LNA bypass range offset vs. freq. compensation
  */
  int1 rfnv_cdma_lna_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  /* 2nd LNA bypass range offset vs. freq. compensation
  */
  int1 rfnv_cdma_lna_12_offset_vs_freq[NV_FREQ_TABLE_SIZ];

  /* Phase rotation setting for LNA decision 01 and 11
  */
  byte rfnv_agc_phase_offset;

#endif


  /* LNA range polarity
  */
  byte rfnv_lna_range_pol;

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

#ifdef FEATURE_ENC_BTF_IN_NV

  /* CHIPX8 delay for SYNC80M, biased +25.
     This is the RF path delay from the antenna to the IQ
     digital MSM pins */
  uint32 rfnv_enc_btf_dly;

#endif

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
} rfnv_digital_items_type;

/* -----------------------------------------------------------------------
** NV data
** ----------------------------------------------------------------------- */
typedef struct {
  boolean	rfnv_rfcal_valid;                       /* RF CAL Valid?         */
  byte		rfnv_config;                            /* RF Config             */
  rfnv_fm_items_type		*rfnv_fm_items_ptr;     /* Ptr to FM NV items    */
  rfnv_digital_items_type	*rfnv_cdma_items_ptr;   /* Ptr to CDMA NV items  */
  rfnv_digital_items_type	*rfnv_pcs_items_ptr;    /* Ptr to PCS NV items*/
  rfnv_gsm_nv_tbl_type		*rfnv_gsm_nv_items_ptr; /* Ptr to GSM NV items*/
  rfnv_gsm_cmn_nv_tbl_type	*rfnv_gsm_cmn_nv_items_ptr; /* Ptr to GSM Common NV items*/
  rfnv_gsm_nv_tbl_type		*rfnv_gsm_1800_nv_items_ptr;
  rfnv_gsm_cmn_nv_tbl_type	*rfnv_gsm_1800_cmn_nv_items_ptr;
  rfnv_gsm_nv_tbl_type		*rfnv_gsm_1900_nv_items_ptr;
  rfnv_gsm_cmn_nv_tbl_type	*rfnv_gsm_1900_cmn_nv_items_ptr;
  rfnv_wcdma_nv_tbl_type	*rfnv_wcdma_nv_items_ptr; /* Ptr to WCDMA NV items */
  rfnv_cmn_nv_tbl_type		*rfnv_common_nv_items_ptr; /* Ptr to Common NV items*/
} rfnv_all_data_type;

/* EJECT  */
/*===========================================================================

                          EXTERNALIZED DATA

===========================================================================*/

/* EJECT  */
/*===========================================================================

                         FUNCTION PROTOTYPES

===========================================================================*/


/*===========================================================================

FUNCTION RFNV_LIST_SIZE                                 EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
extern word rfnv_list_size( void );


/*===========================================================================

FUNCTION RFNV_GET_NV_LST_ADDR                           EXTERNALIZED FUNCTION

DESCRIPTION
  Return the address of the NV calibration items list.

DEPENDENCIES
  None

RETURN VALUE
  Pointer to a array of NV items.

SIDE EFFECTS
  None

===========================================================================*/
extern const nv_items_enum_type *rfnv_get_nv_lst_addr( void );


/*===========================================================================

FUNCTION RFNV_GET_NV_DATA                               EXTERNALIZED FUNCTION

DESCRIPTION
  This function fills the structure pointed to by the pass-in parameter with
  NV items and pointers to calibration tables.  A NULL pointer indicates that
  the specific calibration table is unavailable.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rfnv_get_nv_data
(
  rfnv_all_data_type *data_ptr   /* Pointer to the NV data structure */
);


/*===========================================================================

FUNCTION RFNV_READ_NV_ITEM                              EXTERNALIZED FUNCTION

DESCRIPTION
  This function copies one RF data item, as specified by "item_code" into a
  local data structure.  This function must be called during initialization
  for each and every RF NV item.

DEPENDENCIES
  Note that there must NOT be a dependency on the read order of the NV
  calibration items.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rfnv_read_nv_item
(
  nv_items_enum_type  item_code,    /* Item to put */
  nv_item_type        *item_ptr     /* Pointer where to get the item */
);


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
rfnv_fm_items_type *rfnv_fm_table( void);


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
rfnv_digital_items_type *rfnv_digital_table( void);



#endif /* _5105_RFNV_H */

