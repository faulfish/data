#ifndef RFNV_GSM_H
#define RFNV_GSM_H


/*===========================================================================

                         R F   N V   M a n a g e r

DESCRIPTION
  This header file contains definitions necessary inorder to interface with
  the RF NV Manager.

Copyright (c) 2002 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/rfnv_gsm.h.-arc   1.1   Apr 01 2009 14:19:28   Kevin Tai  $ $DateTime: 2003/11/19 14:42:07 $ $Author:   Kevin Tai  $
//Bonnie update 2005/07/15

when       who     what, where, why
--------   ---     ----------------------------------------------------------
10/20/03   lcl     Added rfnv_gsm_rx_switch_type to reduce code size.
08/21/03   ap      Final modifications to support GSM 850/900/1800/1900.
06/25/03   ap      RFNV Consolidation efforts!
05/27/03   jtn     Use NV_TEST_CODE_VER_I instead of NV_RF_CAL_DATE_I to check for old rx gain settings
04/28/03   jtn     Changes for gain switchpoints in NV
04/17/03   jtn     Added rf_cal_date to RAM structure that holds NV items so only have to read it from NV once
04/02/03   jtn     Added extern boolean rfnv_cal_older_than(uint32 specified_date) called by rfgsm_init().
12/05/02   JC      Added frequency compensated switch point functions.
11/15/02   thh     Removed RFGSM_RTR6200_B2.  B2 is detected via revision #
                   read from the chip.
11/07/02   thh     Added RFGSM_RF_CONFIG_MAX to rfgsm_rf_config_type.
10/15/02   thh     Added RFGSM_RTR6200_B2 for GZIFTRIC B2 support.
08/30/02   JC      Obsoleted super-het radio support.
08/28/02   thh     Added DCS support.
08/12/02   thh     Added RF_CONFIG support.
06/10/02   JC      New APIs for Rx frequency compensation moving forwards
                   using ZIF architecture.
04/03/02   thh     Added support for new Txprofile API.
03/20/02   ih      Added PA start/stop time calibration
03/14/02   thh     Added Tx frequency compensation support.
02/12/02   JC      Removed extraneous NV declarations. Removed coupling
                   to nv.h for GPRS development.
01/21/02   JC      Initial Revision for GSM.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "rex.h"
#include "nv.h"
//#include "rfnv.h"
//#include "mdsp_intf.h" // marked by alice.hy.su.         
#include "gsm.h"		// The data type refered by gsm pa profiles has been abstract to "gsm.h" file.



/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

/* whats the specs for the new bands?) */

#define IS_EGSM900(arfcn) \
  ( arfcn<=124 || (arfcn >=975 && arfcn <=1023) )

#define IS_DCS1800(arfcn) ( arfcn >= 512 && arfcn <= 885)

#define GSM_PA_TEMP_COMP_TBL_SIZE 16

#define RF_NUM_PWR_INDEX_GSM  15
#define RF_MAX_PWR_INDEX_GSM  (RF_NUM_PWR_INDEX_GSM - 1)

#define RF_NUM_PWR_INDEX_DCS  16
#define RF_MAX_PWR_INDEX_DCS  (RF_NUM_PWR_INDEX_DCS - 1)

#ifdef RF_HAS_GSM850
#define RF_NUM_PWR_INDEX_GSM850  15
#define RF_MAX_PWR_INDEX_GSM850  (RF_NUM_PWR_INDEX_GSM850 - 1)
#endif


#ifdef RF_HAS_GSM1900
#define RF_NUM_PWR_INDEX_GSM1900  16
#define RF_MAX_PWR_INDEX_GSM1900  (RF_NUM_PWR_INDEX_GSM1900 - 1)
#endif

/* Maximum power index levels among all effective bands
 */
#define RF_NUM_PWR_INDEX_MAX  16


/* PA configuration
*/
#define RF_PA_RAMP_LUT_MAX    30
#define RF_PA_STEP_SIZE       12

/* format of date is 32-bit number of seconds since Jan 1, 1970 */
#define MARCH_31_2003         0x3E878500
#define APRIL_1_2003          0x3E88D680


/* The largest delta between the requested ARFCN and the smallest channel
** number that was cal'd.  Arbitrarily select 2000.  In EGSM, for exammple
** you may cal ARFCN=1 and try to compensate for ARFCN 1023 (delta=1022).
*/
#define RF_LARGEST_DELTA_CH_SPACE 2000

//extern const rfnv_items_list_type rfnv_gsm_cmn_nv_items_list[];
/*Jack J Huang BQH 29 Jun 2005*/
/*-------------------------New NV Item Definitions for Polar RF-----------------*/

/* AMAM Master Table Size */
#define RF_AMAM_MASTER_TBL_SIZ 512 

/* AMPM Master Table Size */
#define RF_AMPM_MASTER_TBL_SIZ 256 

/* Reference frequencies F1 and F2 */

#define RF_NUM_REF_FREQ 3
#define RF_NUMBER_OF_PCLS 16

#define RF_POLAR_MULTIPLEFACTOR 1024
#define RF_PWR_PER_SEG 8192/* 8*RF_POLAR_MULTIPLEFACTOR */
#define AMAM_DB_STEP 128//0.125*RF_POLAR_MULTIPLEFACTOR
#define AMAM_LOG2_DB_STEP 7//log2(AMAM_DB_STEP)
#define RFNV_AMAM_xDBM_UNITS 100

extern boolean rf_config_read;
//extern typedef mdsp_pa_ramp_type;

// Need to modify this one to reflect only required items
typedef struct
{
  
  /* Number of calibration reference frequency.
  */
  uint16   cal_arfcn_siz;

  /* Calibration reference frequency
  */

  uint16  cal_arfcn[NV_DCS_CAL_ARFCN_SIZ];

  /* --------------------------------------------------------------------- */
  /*    GSM receiver, band gain range frequency compensation items   */
  /* --------------------------------------------------------------------- */
  /* do these names need to change? */
  int16  rx_gain_range_1_freq_comp[NV_DCS_CAL_ARFCN_SIZ];
  int16  rx_gain_range_2_freq_comp[NV_DCS_CAL_ARFCN_SIZ];
  int16  rx_gain_range_3_freq_comp[NV_DCS_CAL_ARFCN_SIZ];
  int16  rx_gain_range_4_freq_comp[NV_DCS_CAL_ARFCN_SIZ];

  /* --------------------------------------------------------------------- */
  /*                     GSM PA Profiles                                   */
  /* --------------------------------------------------------------------- */
  /* do these names need to change? */
  mdsp_pa_ramp_type pa_ramp_tbl[RF_NUM_PWR_INDEX_DCS];

  /* GSM band compensation parameters
  */
  /* Tx frequency compensation table
  */
  int8    tx_freq_comp[NV_DCS_CAL_ARFCN_SIZ];

  /* GSM PA gain slope (DAC value per dB)
  */
  byte    pa_gain_slope[RF_NUM_PWR_INDEX_DCS];

  /* VBATT compensation parameters
  */
  byte    vbatt_hi_pa_comp;
  byte    vbatt_lo_pa_comp;

  /* RF PA start/stop time offsets
  */
  int8   pa_start_time_offset;
  int8   pa_stop_time_offset;
  

  //#ifdef FEATURE_GSM_PA_TEMP_COMP_PWRLVL    
  int8 pa_temp_comp_pwrlvl_tbl[RF_NUM_PWR_INDEX_MAX][GSM_PA_TEMP_COMP_TBL_SIZE];  
  //#else
  int8 pa_temp_comp_tbl[GSM_PA_TEMP_COMP_TBL_SIZE]; 
  //#endif

  /* RF Switch Points for Gain Ranges
  */
  nv_gainrange_switchpoint_type    switchpoints;
  

  /* Max and Min Power for AMAM Table 
  */
//  nv_autocal_pwr_dac_type  autocal_pwr_dac_f1;
//  nv_autocal_pwr_dac_type  autocal_pwr_dac_f2;
  
  /* Dynamic range  for the AMAM master table
  */
  nv_amam_dynamic_range  amam_dynamic_range;
  
  /* AMAM master table for 3 frequencies.
  ** DAC value( unsigned), power in dBm( signed)
  */
  uint16 amam_master_tbl_f1[2][RF_AMAM_MASTER_TBL_SIZ]; /*Jack J Huang modify it from int32 to uint 16*/

  uint16 amam_master_tbl_f2[2][RF_AMAM_MASTER_TBL_SIZ]; /*Jack J Huang modify it from int32 to uint 16*/

  uint16 amam_master_tbl_f3[2][RF_AMAM_MASTER_TBL_SIZ]; /*Jack J Huang modify it from int32 to uint 16*/

  /* AMAM master table for 3 frequencies.
  ** AMPM hw table is signed 19 bit number.
  */
  int32 ampm_master_tbl_f1[RF_AMPM_MASTER_TBL_SIZ];

  int32 ampm_master_tbl_f2[RF_AMPM_MASTER_TBL_SIZ];

  int32 ampm_master_tbl_f3[RF_AMPM_MASTER_TBL_SIZ];
 
  /* Calpath RSB_A and RSB_B values for Gain State 1
  */
//  nv_calpath_rsb_type calpath_rsb;

  /* Gain in percent and DC in mV applied to AMAM Table.
  */

  /*nv_amam_gain_dc_corr_type amam_gain_dc_corr;*////not required for jan release

  /* Systematic Gain and DC in percent applied to AMAM Table.
  */

//  nv_amam_sys_gain_dc_corr amam_sys_gain_dc_corr;    

  /* Reference ARFCNs for AMAM and PM Calibration 
  */

 uint16 amam_arfcn[RF_NUM_REF_FREQ];

  /* Polar path delay 
  */
  uint16 polar_path_delay;
  
  /* OPLL bandwidth 
  */
  uint8  opll_bw_val;

  /* Baseband bandwidth 
  */
  uint8  baseband_bw_val;

  /* Power levels for each PCL */
  int16  power_levels[RF_NUMBER_OF_PCLS];

  /* Value to advance or retrack the PA_EN_START GRFC */
  int16  pa_en_start;
  /* Antenna timing relative to the PA_EN_START GRFC */
  int16  ant_timing_rel_to_pa_en_start;
  /* Value to advance or retrack the PA_EN_STOP GRFC */
  int16  pa_en_stop;
  /* Antenna timing relative to the PA_EN_STOP GRFC */
  int16  ant_timing_rel_to_pa_en_stop;
  /* Timing adjustment in quarter-symbols for GSM TX */
  int16                                            gsm_tx_burst_offset_adj;

  /* Polar Ramp Configuration */
  nv_gsm_polar_ramp_profile_type gsm_polar_paramp;
 
} rfnv_gsm_nv_tbl_type;


typedef struct
{
  /* --------------------------------------------------------------------- */
  /*                   GSM TCXO calibration items                          */
  /* --------------------------------------------------------------------- */
  uint16 tcxo_trk_lo_adj_pdm_init_val;
  uint16 tcxo_trk_lo_adj_pdm_gain_slope;

  /* PA precharge data and duration
  */
  word    precharge;
  word    precharge_dur;

  /* PA DAC value */
  word    vbatt_3200_mv_adc;
  /* PA DAC value */
  word    vbatt_3700_mv_adc;
  /* PA DAC value */
  word    vbatt_4200_mv_adc;

  /* RF cal revision date
  */
  char    test_code_ver[NV_SIZE_OF_VERSION];

  /* Fudge factor adjustment + or - in quarter symbols for TX timing
  */
  int16 gsm_tx_burst_offset_adj;
  //Add by Ethan Tsai 30 Aug 2005
  /* ramp up and ramp down profile for polar GSM/EDGE. */
  nv_gsm_polar_ramp_profile_type	gsm_polar_ramp;	
 
  /* Temporary support for Sony antenna switches evaluation
  */
  uint16 spare_1;
 
  /* Antenna select truth tables
  */
  uint8 rf_antsel_gsm_default[4];
  uint8 rf_antsel_gsm_850_tx[4];
  uint8 rf_antsel_gsm_850_rx[4];
  uint8 rf_antsel_gsm_900_tx[4];
  uint8 rf_antsel_gsm_900_rx[4];
  uint8 rf_antsel_gsm_1800_tx[4];
  uint8 rf_antsel_gsm_1800_rx[4];
  uint8 rf_antsel_gsm_1900_tx[4];
  uint8 rf_antsel_gsm_1900_rx[4];
  uint8 rf_antsel_umts_800[4];
  uint8 rf_antsel_umts_1900[4];
  uint8 rf_antsel_umts_2100[4];
  uint8 rf_antsel_umts_1800[4];

  /* Polar PA ramp config items 
  */
  nv_gsm_polar_ramp_profile_type gsm_polar_paramp;
//  mdsp_pa_transition_profile_type gsm_pa_transition_tbl;
           
} rfnv_gsm_cmn_nv_tbl_type;


typedef struct {

  /* pointer to frequency compensated switch point 1_2_dBm.
  */
	int16 *rx_switch_1_2_dBm;

  /* pointer to frequency compensated switch point 2_3_dBm.
  */
	int16 *rx_switch_2_3_dBm;

  /* pointer to frequency compensated switch point 3_4_dBm.
  */
	int16 *rx_switch_3_4_dBm;

} rfnv_gsm_rx_switch_type;

typedef struct {
mdsp_pa_lut_entry_type  rampon[MAX_RAMPON_ENTRIES+1];
mdsp_pa_lut_entry_type  rampoff[MAX_RAMPOFF_ENTRIES+1];
} rfnv_backup_type;
extern rfnv_backup_type *rfnv_backup_ptr;

extern rfnv_gsm_cmn_nv_tbl_type rfnv_gsm_cmn_nv_tbl;
extern rfnv_gsm_nv_tbl_type *rfnv_gsm_tbl; /* Pointer to the GSM table */
extern rfnv_gsm_cmn_nv_tbl_type *rfnv_gsm_cmn_nv_tbl_ptr; /* Pointer to the common GSM table */
extern rfnv_gsm_rx_switch_type *rfnv_gsm_rx_freq_comp_switch_ptr; /* Pointer to freq comp switch point */


extern boolean rfnv_gsm_old_cal( void );


/*===========================================================================

FUNCTION RFNV_GSM_CMN_COPY_NV_ITEM

DESCRIPTION
  This function updates the specfied item (item_code) in the rfnv_gsm_cmn_nv_tbl
  with the data supplied via the item_ptr.  This function used to be called
  rfnv_read_nv_item().

DEPENDENCIES
  Note that there must NOT be a dependency on the read order of the NV
  calibration items.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rfnv_gsm_cmn_copy_nv_item
(
  nv_items_enum_type item_code,
  nv_item_type *item_ptr
);

/*===========================================================================

FUNCTION rfnv_gsm_get_nv_tbl_ptr                                 

DESCRIPTION
  Returns pointer to GSM RF NV table.

DEPENDENCIES
  None

RETURN VALUE
  Returns pointer to GSM RF NV table.

SIDE EFFECTS
  None

===========================================================================*/
//extern rfnv_gsm_nv_tbl_type *rfnv_gsm_get_nv_tbl_ptr( rfnv_supported_mode_type band );

/*===========================================================================

FUNCTION rfnv_gsm_cmn_get_nv_tbl_ptr                                 

DESCRIPTION
  Returns pointer to GSM CMN RF NV table.

DEPENDENCIES
  None

RETURN VALUE
  Returns pointer to GSM CMN RF NV table.

SIDE EFFECTS
  None

===========================================================================*/
//extern rfnv_gsm_cmn_nv_tbl_type *rfnv_gsm_cmn_get_nv_tbl_ptr(void);


/*===========================================================================

FUNCTION RFNV_GSM_GET_RX_FREQ_COMP_SWITCH_PTR                                 

DESCRIPTION
  Returns pointer to GSM RX frequency compensation switch.

DEPENDENCIES
  None

RETURN VALUE
  Returns pointer to GSM RX frequency compensation switch.

SIDE EFFECTS
  None

===========================================================================*/
//extern rfnv_gsm_rx_switch_type *rfnv_gsm_get_rx_freq_comp_switch_ptr
//(
//  rfnv_supported_mode_type band
//);


/*===========================================================================

FUNCTION RFNV_GSM_CMN_LIST_SIZE                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF Common items NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
//extern uint16 rfnv_gsm_cmn_list_size( void );


/*===========================================================================

FUNCTION RFNV_GSM_CMN_ITEM_TABLE                            EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
//extern void *rfnv_gsm_cmn_item_table(void);



#endif /* RFNV_GSM_H */
