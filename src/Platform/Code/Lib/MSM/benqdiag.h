#ifndef BENADIAG_H
#define BENADIAG_H

#include "comdef.h"
#include "diagcmd.h"
#include "diagpkt.h"
#include "nv.h"
#include "nv_benq.h"

#include "swfeature.h"
//+ most remove this file!!
#define FEATURE_SMS
#define FEATURE_BROADCAST_SMS
#include "smsi.h"

#define CM_DIAG_MAX_GSM_DIGITS 64 //in cm_log.h
#define CM_DIAG_CALL_ID_MAX 8 //in cm_log.h

typedef enum
{
    PID_ANSWERBY_SEND,
    PID_ANSWERBY_ANY,
    PID_ANSWERBY_MAX   =   0xFF
} uiaud_answerKeyModeE;

/*===========================================================================
   RF Control Mobile Commands
===========================================================================*/
/* RFC set commands */
typedef enum {
  // RF control commands
  // starts at 0x100 so that both PC and ARM use 1 word
  RFC_GET_HDET_F = 0x1000000,   // Get HDET readings
  RFC_GET_ISENSE_F,             // Get I Sense readings
  RFC_GET_VBATT_F,              // Get battery readings
  RFC_GET_VBATT_ADC_F,          // Get battery readings for calibration
  RFC_GET_TEMP_F,               // Get temperature readings
  RFC_GET_VCHARGER_F,           // Get charger voltage readings
  RFC_GET_TBATT_F,              // Get battery temperature readings
  RFC_GET_IDBATT_F,             // Get battery id readings
  RFC_GET_RF_CHAN_F,            // Get RF channel number
  RFC_GET_RX_AGC_F,             // Get RX AGC readings
  RFC_GET_TX_AGC_F,             // Get TX AGC readings
  RFC_GET_RF_STATE_F,           // Get RF status
  RFC_GET_IOFFSET_F,            // Get I offset
  RFC_GET_QOFFSET_F,            // Get Q offset
  RFC_GET_TRKLO_F,              // Get Trk_lo offset
  RFC_GET_TEST_MODE_F,          // Get Test Mode
  RFC_GET_BAND_F,               // Get RF Band (PCS or Cellular)
  RFC_GET_MODE_F,               // Get RF Mode
  RFC_GET_PA_F,                 // Get PA status
  RFC_GET_LOCK_F,               // Get LOCK status
  RFC_GET_AGCVALUE_F,           // Get AGC value
  RFC_GET_TXGAINCTL_F,          // Get Tx Output power
  RFC_GET_LNA_BYPASS_F,         // Get Rx LNA bypass or nonbypass
  RFC_GET_PA_RANGE_F,           // Get Tx PA_RANGE status
  RFC_GET_SATFREQ_F,            // Get SAT tone
  RFC_GET_SLEEP_F,              // Get SLEEP pin state
  RFC_GET_TX_PWR_LIMIT_F,       // Get Maximum Tx output power
  RFC_GET_RFDET_F,              // Get RFDET reading
  RFC_GET_FM_RSSI_F,			// Get FM RSSI value
  RFC_GET_TX_GAIN_ADJ_ACC_F,      // Get Tx gain adjustment accumulator(set by BS)

  // RF control commands
  RFC_SET_RF_BAND_F,            // Set RF band
  RFC_SET_RF_MODE_F,            // Set RF mode
  RFC_SET_RF_CHAN_F,            // Set RF channel number
  RFC_SET_LNA_BYPASS_F,         // Set LNA bypass or nonbypass
  RFC_SET_LNA_RANGE_F,          // Set LNA range offset
  RFC_SET_RX_AGC_F,             // Set RX AGC value
  RFC_SET_RX_AGC_LOOP_F,        // Set RX AGC loop
  RFC_SET_TX_AGC_F,             // Set TX AGC value
  RFC_SET_TX_AGC_LOOP_F,        // Set TX AGC loop
  RFC_SET_PA_ONOFF_F,           // Set Tx PA on/off
  RFC_SET_PA_RANGE_F,           // Set Tx PA range value
  RFC_SET_TX_LIMIT_F,           // Set TX limitation value
  RFC_SET_IOFFSET_F,            // Set I_OFFSET PDM value
  RFC_SET_QOFFSET_F,            // Set Q_OFFSET PDM value
  RFC_SET_TRKLO_F,              // Set TRK_LO PDM value
  RFC_SET_VOC_RATE_F,           // Set Vocoder rate
  RFC_SET_IDLE_F,               // Set IDLE control
  RFC_SET_WAVEFORM_F,           // Select CDMA transmit waveform
  RFC_SET_TEST_MODE_F,          // Set RF subsystem to test mode
  RFC_SET_SATFREQ_F,            // Set SAT tone
  RFC_SET_SLEEP_F,              // Set SLEEP pin state
  RFC_SET_TRK_LO_LOOP_F,        // Set TRK_LO loop
  RFC_SET_I_OFFSET_F,           // Set I_OFFSET loop
  RFC_SET_Q_OFFSET_F,           // Set Q_OFFSET loop    
  RFC_SET_FM_FREQ_SENSE_GAIN_F, // Set FM Tx data gain
  RFC_SET_TX_GAIN_ADJ_ACC_F,    // Set Tx gain adjustment accumulator 
  RFC_SET_CDMA_TX_PWR_CONTROL_F,// Set CDMA power limit control   
  RFC_SET_FM_TX_PWR_CONTROL_F,  // Set AMPS power limit control
  RFC_SET_FM_WBD_F,				// Set AMPS WBD generation control
  RFC_SET_FM_CON_DTMF_F,        // Set AMPS DTMF(on link)  <--- new

  // RF calibration commands
  RFC_SET_RX_LIN_F,             // Load RX linearizer
  RFC_SET_TX_LIN_F,             // Load TX linearizer
  RFC_SET_RX_FREQ_COMP_F,       // Set RX frequency compensation
  RFC_SET_TX_FREQ_COMP_F,       // Set TX frequency compensation
  RFC_SET_TX_LIMIT_FREQ_COMP_F, // Load Limit vs freq Cal. data
  RFC_SET_HDET_VS_AGC_F,        // Load CDMA HDET(vs limit value:768+i*16) Cal. data  
  RFC_SET_HDET_OFF_SPAN_F,      // Load Off and Span Cal. data
  RFC_SET_TX_LIMIT_VS_TEMP_F,   // Load center freq limit Cal. data
  RFC_SET_FM_ERR_SLP_PWR_F,     // Load HDET error Cal. data      ------------       
  RFC_SET_FM_EXP_HDET_PWR_F,    // Load HDET(vs 8 power level) Cal. data      |(*)---> get correct agc value
  RFC_SET_FM_AGC_ADJ_FREQ_F,    // Load HDET error vs agc Cal. data-----------
  RFC_SET_FM_AGC_SET_PWR_F,     // Load Agc value(for 8 power level) Cal. data 
  RFC_SET_FM_AGC_SET_FREQ_F,    // Load Agc vs Freq Cal. data

  RFC_GET_TX_VHF_FREQ_HI_F,        // Get TX IF frequency High
  RFC_GET_TX_VHF_FREQ_LO_F,        // Get TX IF frequency Low
  RFC_GET_TX_STEP_FREQ_F,          // Get TX IF step frequency
  RFC_SET_TX_VHF_FREQ_HI_F,        // Set TX IF frequency High
  RFC_SET_TX_VHF_FREQ_LO_F,        // Set TX IF frequency Low
  RFC_SET_TX_STEP_FREQ_F,          // Set TX IF step frequency

  RFC_GET_RX_VHF_FREQ_HI_F,        // Get RX IF frequency High
  RFC_GET_RX_VHF_FREQ_LO_F,        // Set TX IF frequency Low
  RFC_GET_RX_STEP_FREQ_F,          // Get RX IF step frequency
  RFC_SET_RX_VHF_FREQ_HI_F,        // Set RX IF frequency High
  RFC_SET_RX_VHF_FREQ_LO_F,        // Set TX IF frequency Low
  RFC_SET_RX_STEP_FREQ_F,          // Set RX IF step frequency

  RFC_SET_CDMA_AMP_MAX_F,          // Set CDMA Driver Amplifier max
  RFC_SET_CDMA_AMP_MIN_F,          // Set CDMA Driver Amplifier min
  RFC_SET_RF_LO_BUFFER_F,          // Set RF LO Buffer
  RFC_SET_AB_AMP_F,                // Set RF A and B Amplifiers
  RFC_SET_RF_MOD_MAX_CURR_F,       // Set RF Modulation Max Current
  RFC_SET_RF_MOD_MIN_CURR_F,       // Set RF Modulation Min Current
  RFC_SET_IF_MIXER_GAIN_F,         // Set IF Mixer Gain

  // Ting {
  RFC_ENTER_FACTORY_TEST_F,        // Enter factory test mode
  RFC_GET_DMSS_STATE_F,            // Get DMSS state value for BBTEST
  // Ting }

  RFC_GET_NO_SRVC_STATE_F,         // Get handset srevice state

  RFC_MAX_CMDS_F

} diag_rfc_cmd_enum_type;
/*-------------------------------------------------------------------------

                    RF Control parameter types

--------------------------------------------------------------------------*/

/* Union of RFC command response types */
typedef __packed union {
    byte         byte_val;
    word         word_val;
    int1         int1_val;
    byte         bytes[2];
} diag_rfc_results_type;

// Union of RFC command parameter types
typedef __packed union {
  byte         byte_val;
  word         word_val;
  int1         int1_val;
  byte         bytes[2];

  // RX linearizer
   __packed struct {
     unsigned char off;
     unsigned char slp[NV_CDMA_RX_LIN_SIZ];
   } rx_lin;

  // TX linearizer
   __packed struct {
     unsigned short index;
     nv_tx_linearizer_type linearizer;
   } tx_lin;

  // Rx frequency compensation tables
   int1 freq_comp_table[NV_FREQ_TABLE_SIZ];

  // TX gain compensation versus frequency
   __packed struct {
     unsigned short index;
   int1 freq_comp_table[NV_FREQ_TABLE_SIZ];
   } tx_lin_vs_freq;

  // AMPS Power level tables
   int1 fm_pwr_table[NV_PWR_TABLE_SIZ];

  // CDMA hdet off and span and limit vs temp table
   int1 off_spn_limit_temp_table[2];

  // CDMA HDET vs limit tables
   int1 hdet_agc_table[NV_CDMA_EXP_HDET_VS_AGC_SIZ]; 
} diag_rfc_cmd_parm_type;


typedef enum
{
  PIM_ADD    = 0,
  PIM_UPDATE,
  PIM_DELETEONE,
  PIM_DELETEALL,
  PIM_DELETEALL_PB_ME,
  PIM_DELETEALL_PB_RUIM,
  PIM_OPTION_MAX,
/*-------------------------------------------------------------------------*/
  PIM_OPTION_PAD = 0x7FFF   /* Pad to 16 bits on ARM                    */
} pim_option_type;


typedef enum {
  /* The following RF state descriptions apply when rf_chipx8_needed is FALSE.
     When rf_codec_on(TRUE) occurs, the phone exits sleep (if sleeping), CODEC
     power is turned on (if off), and chipx8 is enabled into the MSM2P (if
     disabled).  When rf_codec_on(FALSE) occurs, it is responsible for
     restoring the hardware according to the following state descriptions. */

  RF_STATE_SLEEP_1,
    /* CDMA sleep state 1 (first catnap).  This state is entered by rf_sleep()
    and exited by either rf_stay_asleep() or rf_enable_digital_rx_pwr().
    The database item DB_RF_MODE == DB_RF_SLEEP, but the RF Rx hardware is
    still powered up.  chipx8 is available.  CODEC power is off.  The Rx
    hardware is set to CDMA mode. */

  RF_STATE_SLEEP_2,
    /* CDMA sleep state 2 (after first catnap).  This state is entered by
    rf_stay_asleep() and exited by rf_enable_digital_rx_pwr().  The database
    item DB_RF_MODE == DB_RF_SLEEP and the RF Rx hardware has been powered
    down.  chipx8 is not available.  CODEC power is off.  The Rx hardware is
    set to CDMA mode. */

  RF_STATE_CDMA_RX,
    /* CDMA Rx state.  This state is entered by either rf_init() or
    rf_enable_digital_rx_pwr() and is exited by either rf_sleep(),
    rf_enable_tx_pwr(), or rf_enable_analog_rx_pwr().  The database item
    DB_RF_MODE has not yet been updated to DB_RF_CDMA or DB_RF_PCN (that
    happens in rf_init_digital()), so DB_RF_MODE indicates either DB_RF_SLEEP,
    DB_RF_CDMA, or DB_RF_ANALOG.  The RF Rx hardware is powered up but not
    completely setup.  chipx8 is available.  CODEC power is off.  The Rx
    hardware is set to CDMA mode. */

  RF_STATE_CDMA_RXTX,
    /* CDMA RxTx state.  This state is entered by rf_enable_tx_pwr() and exited
    by rf_tx_disable().  The database item DB_RF_MODE == DB_RF_CDMA or
    DB_RF_PCN.  The RF  Rx+Tx hardware are powered up but not completely setup.
    chipx8 is available.  CODEC power is on.  The Rx+Tx hardware are set to
    CDMA mode. */

  RF_STATE_FM_RX,
    /* FM Rx state.  This state is entered by rf_enable_analog_rx_pwr() and
    exited by either rf_enable_tx_pwr() or rf_enable_digital_rx_pwr().  The
    database item DB_RF_MODE has not yet been updated to DB_RF_ANALOG (that
    happens in rf_init_analog()), so DB_RF_MODE indicates either DB_RF_CDMA,
    DB_RF_PCN, or DB_RF_ANALOG.  The RF Rx hardware is powered up but not
    completely setup.  chipx8 is not available.  CODEC power is off.  The Rx
    hardware is set to FM mode. */

  RF_STATE_FM_RXTX,
    /* FM RxTx state.  This state is entered by rf_enable_tx_pwr() and
    exited by rf_tx_disable().  The database item DB_RF_MODE == DB_RF_ANALOG.
    The RF Rx+Tx hardware are powered up but not completely setup.  chipx8 is
    available.  CODEC power is on.  The Rx+Tx hardware are set to FM mode. */

  RF_STATE_GPS
  /* GPS State. Entered by a call to rf_digital_band_select() with GPS as the
  passed rf band argument.*/

} rf_state_type;

//For CDMA 1x: db.h
/*
** Type for DB_RF_MODE item
*/

typedef enum {
  DB_RF_ANALOG,                   /* RF is in analog (FM) mode of operation */
  DB_RF_CDMA,                     /* RF is in CDMA mode of operation        */
  DB_RF_PCN,                      /* RF is in PCN mode of operation         */
  DB_RF_900MHZ,                   /* RF is in 900 MHz mode of operation     */
  DB_RF_SLEEP,                    /* RF is in sleep mode of operation       */
  DB_RF_GPS,                      /* RF is in GPS mode of operation         */
  DB_RF_HDR                       /* RF is in HDR mode of operation         */
} db_rf_mode_type;

/*
** Type for DB_DMSS_STATE: System state enumeration.
*/

typedef enum {
  DB_DMSS_POWERUP_S,
  DB_DMSS_ONLINE_ANALOG_S,
  DB_DMSS_ONLINE_DIGITAL_S,
  DB_DMSS_OFFLINE_ANALOG_S,
  DB_DMSS_OFFLINE_DIGITAL_S,
  DB_DMSS_RESET_S,
  DB_DMSS_POWERDOWN_S,
  DB_DMSS_GPS_S
} db_dmss_state_type;

//For CDMA 1x : mcc.h
typedef enum
{ 
  MCC_ONLINE = 0, /* Digital online  */
  MCC_OFFLINE = 2 /* Digital offline */
} mcc_entry_reason_type;

//For CDMA 1x : acpmc.h
typedef enum
{
  ACP_ONLINE = 0, /* Analog online  */
  ACP_OFFLINE = 6 /* Analog offline */
} acp_entry_reason_type;

typedef enum 
{
  FTM_BAND_CLASS_CELLULAR,
  FTM_BAND_CLASS_PCS,
  ftm_band_class_sizer = 0x7FFFFFFF
} ftm_band_class_type;

typedef enum 
{
  FTM_ACQ_MODE_FULL,
  FTM_ACQ_MODE_MINI,
  FTM_ACQ_MODE_MICRO,
  ftm_acq_mode_sizer = 0x7FFFFFFF
} ftm_acq_mode_type;

typedef PACKED struct
{
   ftm_band_class_type band_class;
     /* the band class, Cellular or PCS */

   word cdma_freq;
     /* the cdma frequency channel */

   ftm_acq_mode_type acq_mode;
     /* the acquisition mode, Full, Mini, Micro */
} ftm_cmd_acq_pilot_param_type;

//typedef enum {
//  FTM_FIRST_CMD = 100,
//  FTM_ACQUIRE_SC = 100,
//  FTM_SYNC_SC =    101,
//  FTM_FWD_FCH_SC = 102,
//  FTM_FWD_SCH_SC = 103,
//  FTM_REV_FCH_SC = 104,
//  FTM_REV_SCH_SC = 105,
//  FTM_FCH_LOOP_SC = 106,
//  FTM_SCH_LOOP_SC = 107,
//  FTM_RELEASE_SC = 108,
//  FTM_SET_MODE_SC = 109,
//  FTM_FWD_DCCH_SC = 110,
//  FTM_REV_DCCH_SC = 111,
//  FTM_DCCH_LOOP_SC = 112,
//  FTM_MAX_CMD = 113
//} ftm_subcmd_type;

typedef enum 
{
  TEST_SUCCESS,
  TEST_INVALID_CMD,
  TEST_INVALID_STATE,
  TEST_NO_SUCCESS,
  test_cmd_status_sizer = 0x7FFFFFFF
  /* enforces the enums to be 32 bits */
} test_cmd_status_type;

typedef enum 
{
  FTM_FWD_RC_1=0x01,
  FTM_FWD_RC_2,
  FTM_FWD_RC_3,
  FTM_FWD_RC_4,
  FTM_FWD_RC_5,
  FTM_FWD_RC_1X_MAX,
  ftm_fwd_rc_sizer = 0x7FFFFFFF
} ftm_fwd_rc_type;

typedef enum 
{
  FTM_REV_RC_1=0x01,
  FTM_REV_RC_2,
  FTM_REV_RC_3,
  FTM_REV_RC_4,
  FTM_REV_RC_1X_MAX,
  ftm_rev_rc_sizer = 0x7FFFFFFF
} ftm_rev_rc_type;

typedef PACKED struct 
{
  byte walsh;    /* walsh channel number (0-63 for 95A/B, 0-127 for cdma2000) */

  byte qof_mask_id;  /* quasi orthoganal function mask (0-3) where 0 is 
                        true walsh codes */
} ftm_walsh_type;

typedef PACKED struct
{
  byte  target_fer;
  byte  min_setpt;
  byte  max_setpt;
} ftm_fpc_olc_param_type;

typedef PACKED struct
{
   ftm_fwd_rc_type radio_cfg;
   /* the radio configuration of this FCH */

   ftm_walsh_type walsh;
     /* walsh index */

   boolean bypass_lc;
     /* bypass long code (long code set to all zero) */

   byte frame_offset;
     /* frame offset */

   byte subchan_gain;
     /* Power control subchannel gain */

   boolean ffpc_enable;
     /* fast forward power control enable */

   dword fpc_mode;
     /* forward power control mode */

   byte init_setpt;
     /* initial set point for inner loop FPC on FCH */

   ftm_fpc_olc_param_type  olc_params;
   /* outer loop power control parameters */

} ftm_cmd_demod_fch_param_type;

typedef enum 
{
  FTM_REV_FCH_FULL_RATE=0x00,
  FTM_REV_FCH_HALF_RATE=0x01,
  FTM_REV_FCH_QTR_RATE=0x02,
  FTM_REV_FCH_8TH_RATE=0x03,
  ftm_rev_fch_rate_sizer = 0x7FFFFFFF
} ftm_rev_fch_rate_type;

typedef PACKED struct
{
   ftm_rev_rc_type radio_cfg;
     /* Reverse FCH radio config */

   byte frame_offset;
     /* frame offset */

   ftm_rev_fch_rate_type fch_rate;

   boolean enable_rpc;
     /* Enable/disable reverse power control */

   word num_preambles;
     /* Preamble count for traffic channel initialization */

   dword tx_pattern;
     /* 32 bit pattern of bytes to use in the traffic frames */
} ftm_cmd_mod_fch_param_type;

typedef enum{
	NV_IMAGE_OP_UPDATE,
    NV_IMAGE_OP_ERASE,
    NV_IMAGE_OP_DUMP,
	NV_IMAGE_OP_GET_EXT_DEV_INFO,
	NV_IMAGE_OP_SET_CONNECTION,
	NV_IMAGE_OP_GET_LARGE,
	NV_IMAGE_OP_VERIFY
}nv_image_op_type;

typedef enum {

  PIM_STAT_NOT_SUPPORT	= 0xFFFE,
  PIM_STAT_NONE			= 0xFFFF,

  /* Phonebook */             
  PIM_PB_STAT_NONE    = 0,             /* default status, no action */
  PIM_PB_STAT_OK,                      /* no error */
  PIM_PB_STAT_PARERR,                  /* parameter error */
  PIM_PB_STAT_FULL,                    /* full */
  PIM_PB_STAT_NOTFULL,                 /* not full */
  PIM_PB_STAT_EMPTY,                   /* empty */
  PIM_PB_STAT_NOTEMPTY,                /* not empty */
  PIM_PB_STAT_NOTFIND,                 /* not find  */
  PIM_PB_STAT_FOUND,                   /* find the record */
  PIM_PB_STAT_FOUND_FIRST,             /* find the first record */
  PIM_PB_STAT_COPIED,                  /* copy successfully*/
  PIM_PB_STAT_NOTCOPY,                 /* copy error*/
  PIM_PB_STAT_DUPLICATED,              /* duplicated record */
  PIM_PB_STAT_NOCHANGE,                /* no change when check duplicated */
  PIM_PB_STAT_UPDATE_CG_ERROR,         /* Error when update the related caller group */

//#ifdef FEATURE_RUIM_PHONEBOOK 
  PIM_PB_STAT_CARDERR,                 /* card error */
  PIM_PB_STAT_CARDINIT,                /* card initialized */
//#endif                        
                              
  /* Call Group */            
  PIM_PB_CG_STAT_NONE = 0x100,         /* default status, no action */
  PIM_PB_CG_STAT_OK,                   /* no error */
  PIM_PB_CG_STAT_PARERR,               /* parameter error */
  PIM_PB_CG_STAT_FULL,                 /* full */
  PIM_PB_CG_STAT_NOTFULL,               /* not full */
  PIM_PB_CG_STAT_EMPTY,                /* empty */
  PIM_PB_CG_STAT_NOTEMPTY,             /* not empty */
  PIM_PB_CG_STAT_RECORD_ADD,           /* Added a caller group */
  PIM_PB_CG_STAT_RECORD_UPDATE,    /* updated a caller group */
//#ifdef FEATURE_RUIM_PHONEBOOK 
  PIM_PB_CG_STAT_CARDERR,              /* card error */
  PIM_PB_CG_STAT_CARDINIT,             /* card initialized */
//#endif                        
  
  /* Portrait Table. Please don't insert any item into the enum of Portrait table */
  PIM_PB_PT_STAT_NONE   = 0x200,       /* The enum represents the result of portrait API */
  PIM_PB_PT_STAT_SLOT_1,
  PIM_PB_PT_STAT_SLOT_2,
  PIM_PB_PT_STAT_SLOT_3,
  PIM_PB_PT_STAT_SLOT_4,
  PIM_PB_PT_STAT_SLOT_5,
  PIM_PB_PT_STAT_SLOT_6,
  PIM_PB_PT_STAT_SLOT_7,
  PIM_PB_PT_STAT_SLOT_8,
  PIM_PB_PT_STAT_SLOT_9,
  PIM_PB_PT_STAT_SLOT_10,
  PIM_PB_PT_STAT_SLOT_11,
  PIM_PB_PT_STAT_SLOT_12,
  PIM_PB_PT_STAT_SLOT_13,
  PIM_PB_PT_STAT_SLOT_14,
  PIM_PB_PT_STAT_SLOT_15,
  PIM_PB_PT_STAT_SLOT_16,
  PIM_PB_PT_STAT_SLOT_17,
  PIM_PB_PT_STAT_SLOT_18,
  PIM_PB_PT_STAT_SLOT_19,
  PIM_PB_PT_STAT_SLOT_20,

  
  /* Scheduler */             
  PIM_SCH_STAT_NONE   = 0x300,         /* default status, no action */
  PIM_SCH_STAT_OK,                     /* no error */
  PIM_SCH_STAT_PARERR,                 /* parameter error */
  PIM_SCH_STAT_FULL,                   /* full */
  PIM_SCH_STAT_NOTFIND,                /* not find  */
  PIM_SCH_STAT_EMPTY,                  /* empty */
  PIM_SCH_STAT_TIMEEXPIRED,            /* event time is expired*/
                              
  /* Basic Setting */          
  PIM_BS_STAT_NONE    = 0x400,         /* default status, no action */
  PIM_BS_STAT_OK,                      /* no error */
  PIM_BS_STAT_FAIL,                    /* fail */

  /* Lock */
  PIM_LOCK_STAT_NONE       = 0x500,    /* default status, no action */
  PIM_LOCK_STAT_TIMEOUT,               /* timeout */

  PIM_RETURN_STATUS_MAX,
/*-------------------------------------------------------------------------*/

  PIM_RETURN_STATUS_PAD = 0x7FFF   /* Pad to 16 bits on ARM                    */

} pim_return_status_type;

/*===========================================================================
PACKET   diag_pb_type

ID       DIAG_PB_READ_F,DIAG_PB_WRITE_F
===========================================================================*/

typedef PACKED struct {
  byte command_code;
  uint16 item;
  pim_option_type option;         /* PIM option */
  word    address;                /* record real location */
  pbPhoneBookS item_data;         /* phone book data */
  uint16 stat;
}diag_pb_type;

/*===========================================================================
PACKET   diag_cg_type

ID       DIAG_CG_READ_F,DIAG_CG_WRITE_F
===========================================================================*/
typedef PACKED struct {
  byte command_code;
  uint16 item;
  pim_option_type option;         /* PIM option */
  word    address;                /* record real location */
  pbCallGroupV2S item_data;
  uint16 stat;
}diag_cg_type;

/*===========================================================================

PACKET   diag_sw_feature_req_type

ID       DIAG_SW_FEATURE_F
 
PURPOSE  Get software feature from HS

===========================================================================*/
typedef PACKED struct {
    byte    cmd_code;
} diag_sw_feature_req_type;

/*===========================================================================

PACKET   diag_sw_feature_rsp_type

ID       DIAG_SW_FEATURE_F
 
PURPOSE  Get software feature from HS

===========================================================================*/
typedef PACKED struct {
    byte    cmd_code;
    //dword   feature;    /* support 32 kinds of features */
	word sw_feature_mask_size;
	byte diag_sw_feature_mask[SW_FEATURE_MASK_LENGTH];
} diag_sw_feature_rsp_type;

/*==========================================================================

PACKET   diag_ts_req_type

ID       DIAG_TS_F

PURPOSE  Sent by the DM to request a timestamp from the DMSS

RESPONSE The DMSS returns a timestamp in a diag_ts_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;                /* Command code */
} diag_ts_req_type;

/*===========================================================================

PACKET   diag_ts_rsp_type

ID       DIAG_TS_F

PURPOSE  Sent by DMSS, contains a mobile time-stamp in qword format

============================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* Command code */
  qword ts;                    /* Time stamp */
} diag_ts_rsp_type;

/*===========================================================================

PACKET   diag_msg_req_type

ID       DIAG_MSG_F

PURPOSE  Sent by the DM to request a buffered msg

RESPONSE The DMSS selects an appropriate message and sends it in a
         diag_msg_rsp_type packet.

============================================================================*/
typedef PACKED struct
{
 byte cmd_code;                    /* Command code */
 word msg_level;                   /* highest type desired */
} diag_msg_req_type;

/*===========================================================================

PACKET   diag_msg_rsp_type

ID       DIAG_MSG_F

PURPOSE  Sent by DMSS, contains 0 or 1 of the buffered messages the mobile
         is holding.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;               /* command code                              */
  word qty;                    /* number of msg packets in the msg buffer
                                  including the msg packet returned in this
                                  response  -- if this field is 0, no msg
                                  packet is included in this response       */
  dword drop_cnt;              /* number of msgs dropped by error services  */
  dword total_msgs;            /* total number of msgs that have occured    */
  byte  level;                 /* Severity level / Priority of this message */
  char  file[ 13 ];            /* Holds source file name                    */
  word  line;                  /* Line number in source file                */
  char  fmt[ 40 ];             /* printf style format string                */
  dword code1;                 /* parameters to go with the format string   */
  dword code2;
  dword code3;
  qword time;                  /* Time at which message was generated       */
} diag_msg_rsp_type;

/*===========================================================================

PACKET   diag_bbtest_cmd_type

ID       DIAG_BASEBAND_TEST_F

PURPOSE  Set baseband test commands to phone

RESPONSE 

===========================================================================*/
typedef enum
{
    BBTEST_START_F = 0x1000000,
    BBTEST_KEYPAD_TEST_F,
    BBTEST_SND_TEST_F,
    BBTEST_BACKLIGHT_F ,
    BBTEST_LED_F,
    BBTEST_LCD_F,
    BBTEST_VIBRATOR_F,
    BBTEST_RTC_F,
    BBTEST_LCD_MESSAGE_F,
    BBTEST_ADC_READ_F,
    BBTEST_SET_APATH_F,
    BBTEST_RTC_CLEAR_ALARM_F,
    BBTEST_END_F,
    BBTEST_LCD_SUB_F,
    BBTEST_FUN_LCD_CLEAN_F,  //This command is for clean LCD 
	BBTEST_RTC_TIME_READ_F,  /* Read RTC time from DB */    
    BBTEST_BATT_TEMP_READ_F, /* Read battery temperature */
    BBTEST_CHG_STATE_READ_F, /* Read charging state */
	BBTEST_CMD_MAX
} diag_bbtest_cmd_enum_type;

typedef enum
{
    BBTEST_RTC_TEST_ALARM_F = 0,
    BBTEST_RTC_TEST_DISP_F,
    BBTEST_RTC_TEST_CMD_MAX
} diag_bbtest_rtc_cmd_enum_type;

typedef enum
{
    BBTEST_LCD_TEST_CONST_F = 0,
    BBTEST_LCD_TEST_DISP_F,
    BBTEST_LCD_TEST_MODULE_F,
	BBTEST_LCD_TEST_SAVECONSTVAL_F, /* save sub panel contrast value */
    BBTEST_LCD_TEST_CMD_MAX
} diag_bbtest_lcd_cmd_enum_type;

typedef enum
{
    BBTEST_APATH_MIC_TO_SP_F = 0,
    BBTEST_APATH_HFMIC_TO_SP_F,
    BBTEST_APATH_MIC_TO_HFSP_F,
    BBTEST_APATH_TEST_CMD_MAX
} diag_bbtest_apath_enum_type;

typedef PACKED struct
{
    _clk_julian_type_  local_time;
    word             rtc_test_cmd;
    byte             byte_val;
} rtc_parm_type;
typedef PACKED struct
{
    byte       len;
    byte       message_buf[48];/* 12x4 characters */
} lcd_message_type;

typedef enum
{
    BBTEST_SND_TEST_BUZZER_F = 0,
    BBTEST_SND_TEST_SPEAKER_F,
    BBTEST_SND_TEST_MICROPHONE_F,
    BBTEST_SND_TEST_PHONEJACK_F,
    BBTEST_SND_TEST_CMD_MAX
} diag_bbtest_snd_cmd_enum_type;

typedef PACKED struct
{
    word        snd_test_cmd;
    byte        byte_val;    
}snd_test_parm_type;


typedef PACKED struct {
  byte  color;
  byte  is_cyclic;
  byte  period;
  byte  pattern;
} led_test_type;

typedef PACKED struct
{
    word             lcd_test_cmd;
    byte             lcd_constrast;
    led_test_type    led;
    byte             byte_val;
    byte             lcd_module;
} lcd_parm_type;

typedef PACKED struct
{
    word        which_path;
    byte        byte_val;
}set_apath_type;

typedef PACKED union 
{
  byte               byte_val;
  rtc_parm_type      rtc_type;
  lcd_message_type   lcd_message;
  snd_test_parm_type snd_test;
  lcd_parm_type      lcd_test;
  set_apath_type     set_apath;
} diag_bbtest_cmd_parm_type;

typedef PACKED struct {
  byte                       cmd_code;
  diag_bbtest_cmd_enum_type  bbtest_cmd;
  diag_bbtest_cmd_parm_type  bbtest_parm;
} diag_bbtest_cmd_type;

/*===========================================================================

PACKET   diag_bbtest_rsp_type

ID       DIAG_BASEBAND_TEST_F
 
PURPOSE  Sent by DMSS to relay baseband test command results

===========================================================================*/

typedef __packed struct {
  byte                        cmd_code;
  diag_bbtest_cmd_enum_type   bbtest_cmd;
  word                        adc_val;
  byte                        lcd_const;
  byte                        lcd_module;
  boolean                     result;
} diag_bbtest_rsp_type;

//#define DIAG_WAP_READ_F           147
//#define DIAG_WAP_WRITE_F          148
typedef PACKED struct
{
  byte addrLen;
  byte userLen;
  byte pswdLen;
  byte addr[WAP_MAX_APADDR_LEN];
  byte user[WAP_MAX_APUSER_LEN];
  byte pswd[WAP_MAX_APPSWD_LEN];
} wap_DiagAccessPointS;

typedef PACKED struct{
	diagpkt_subsys_header_type xx_header;
	uint8  num_digits;
	uint8  digits[CM_DIAG_MAX_GSM_DIGITS];
}DIAG_SUBSYS_CM_CM_GSM_CALL_ORIGINATION_req_type;



typedef PACKED struct{
	  diagpkt_subsys_header_type xx_header;
	  byte		 call_id_count;
 	PACKED struct {
	  uint8 	   call_id;
	  uint8 	   call_mode_type;			   /* CDMA - 0, GW_CS=1, GW_PS=2 */
	  boolean	   cdma_end_reason_included;
	  uint8 	   cdma_end_reason;
	  boolean	   gw_tear_down_ind_valid;
	  boolean	   gw_tear_down_ind_ti_flag;
	  uint8 	   gw_cause;
 	}end_params[CM_DIAG_CALL_ID_MAX];
}DIAG_SUBSYS_CM_CM_CALL_END_req_type;

typedef PACKED struct{
	diagpkt_subsys_header_type xx_header;
	boolean  status;
}DIAG_SUBSYS_CM_CM_CALL_END_rsp_type;


typedef PACKED struct{
	diagpkt_subsys_header_type xx_header;
	boolean status;
	byte	call_id;
}DIAG_SUBSYS_CM_CM_GSM_CALL_ORIGINATION_rsp_type;


typedef PACKED struct
{
  byte numAddr;
  wap_DiagAccessPointS wap_accessPoint[WAP_MAX_AP_NUMS];
} wap_DiagBearerS;


/*===========================================================================

PACKET   diag_wap_read_req_type

ID       DIAG_WAP_READ_F

PURPOSE  This message requests a read of the WAP user data.

RESPONSE The diag_wap_read_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  byte proxy_index;							   /* proxy0, 1, or 2 */
} diag_wap_read_req_type;


/*===========================================================================

PACKET   diag_wap_write_req_type

ID       DIAG_WAP_WRITE_F

PURPOSE  This message requests a write of the WAP user data into NV.

RESPONSE The diag_wap_write_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  byte proxy_index;	       				       /* proxy0, 1, or 2 */	
  byte numAddr;
  wap_WDPAddressS wap_address[WAP_MAX_WDPADDR_NUMS];
  wap_DiagBearerS wap_bearer[WAP_MAX_BEARER_NUMS];
} diag_wap_write_req_type;

/*===========================================================================

PACKET   diag_wap_read_rsp_type

ID       DIAG_WAP_READ_F

PURPOSE  This response delivers the read WAP user data. 

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                           /* Command code */
  nv_stat_enum_type nv_stat;               /* Status of operation */
  byte numAddr;
  wap_WDPAddressS wap_address[WAP_MAX_WDPADDR_NUMS];
  wap_DiagBearerS wap_bearer[WAP_MAX_BEARER_NUMS];
} diag_wap_read_rsp_type;


/*===========================================================================

PACKET   diag_wap_write_rsp_type

ID       DIAG_WAP_WRITE_F

PURPOSE  This response tells the NV status of the WAP user data write.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                             /* Command code */
  nv_stat_enum_type nv_stat;                 /* Status of operation */
} diag_wap_write_rsp_type;

/* AKey write */
//#define DIAG_AKEY_WRITE_F         149
/*===========================================================================

PACKET   diag_akey_rsp_type

ID       DIAG_AKEY_WRITE_F

PURPOSE  Sent by DMSS to confirm AKEY update

============================================================================*/

#define AKEY_RSP_RESERVED 4

typedef PACKED struct {
  byte                cmd_code;                    /* Command code          */
  word                status;                      /* Status of AKEY update */
  byte                reserved[AKEY_RSP_RESERVED]; /* for compatibility     */
} diag_akey_rsp_type;

/* Read RUIM PhoneBook */
//#define DIAG_RUIM_PB_READ_F       150
/*===========================================================================
PACKET   diag_ruim_pb_type

ID       DIAG_RUIM_PB_READ_F

PURPOSE  Send RUIM phonebook commands to phone

RESPONSE 
===========================================================================*/
typedef PACKED struct {
  byte cmd_code;
  byte num;    /* UIM card max record */
  byte stat;    /* TRUE : UIM card initial status is OK */
}diag_ruim_pb_type;

/* Read SMS message from NV */
//#define DIAG_BENQ_SMS_READ_F      151
/*===========================================================================

PACKET   diag_benq_sms_read_req_type

ID       DIAG_BENQ_SMS_READ_F

PURPOSE  This message requests a read of the given SMS message out of NV

RESPONSE The diag_benq_sms_read_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  byte list_cat;                               /* which SMS type */
  byte number;                                 /* which SMS number */
  byte readitem;                               /* Read SMS data or read SMS record */
} diag_benq_sms_read_req_type;

/* Write SMS message into NV */
//#define DIAG_BENQ_SMS_WRITE_F     152
/*===========================================================================

PACKET   diag_benq_sms_write_req_type

ID       DIAG_BENQ_SMS_WRITE_F

PURPOSE  This message requests a write of the given SMS message into NV.
         It is a variable length request message - only as long as dictated
         by the length field in sms_message.

RESPONSE The diag_benq_sms_write_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  smsi_address_type sms_address;               /* SMS message address */
  smsi_user_data_type sms_userdata;            /* SMS user data */
  smsi_indicator_type sms_indicator;           /* SMS indicator */
  smsi_call_back_type sms_callback;            /* SMS call back Number */
  boolean from_draft;                          /* Draft or new SMS type */
  boolean send;                                /* Send or save SMS */
  byte number;                                 /* which SMS number */
} diag_benq_sms_write_req_type;

/* Delete SMS message into NV */
//#define DIAG_BENQ_SMS_DELETE_F    153
/*===========================================================================

PACKET   diag_benq_sms_delete_req_type

ID       DIAG_BENQ_SMS_DELETE_F

PURPOSE  This message requests a delete of the given SMS message into NV.
         It is a variable length request message - only as long as dictated
         by the length field in sms_message.

RESPONSE The diag_benq_sms_delete_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  byte list_cat;                               /* which SMS type */
  byte number;                                 /* which SMS number */
} diag_benq_sms_delete_req_type;

/* Read the Service Prog. Code 1 */
//#define DIAG_NV_SPC_READ_F        154
/*===========================================================================

PACKET   diag_nv_spc_read_req_type

ID       DIAG_NV_SPC_READ_F

PURPOSE  This message requests a read of the SPC1 data.

RESPONSE The diag_nv_spc_read_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte                cmd_code;      /* Command code */
  nv_items_enum_type  item;          /* Which item   */
  nv_item_type        item_data;     /* Item itself  */
  nv_stat_enum_type   nv_stat;       /* Status of operation */
} diag_nv_spc_read_req_type;


/* Write the Service Prog. Code 1 */
//#define DIAG_NV_SPC_WRITE_F       155
/*===========================================================================

PACKET   diag_nv_spc_write_req_type

ID       DIAG_NV_SPC_WRITE_F

PURPOSE  This message requests a write of the SPC1 data into NV.

RESPONSE The diag_nv_spc_write_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte                cmd_code;      /* Command code */
  nv_items_enum_type  item;          /* Which item   */
  nv_item_type        item_data;     /* Item itself  */
  nv_stat_enum_type   nv_stat;       /* Status of operation */
} diag_nv_spc_write_req_type;

/* Send the Super Service Prog. Code to allow SP    */
//#define DIAG_SUPER_SPC_F          156
#define NV_SUPER_SPC_CODE_SIZE   8

typedef PACKED struct {
  byte      digits[NV_SUPER_SPC_CODE_SIZE];     /* Security code array */
} nv_super_spc_code_type;

typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  nv_super_spc_code_type sec_code;  /* The security code */
} diag_super_spc_req_type;

typedef PACKED struct
{
  byte cmd_code;              /* Command code */
  boolean sec_code_ok;        /* The SPC was entered correctly or not */
} diag_super_spc_rsp_type;


/* Get System Parameters such as A_KEY, SSD_A, SSD_B, and SPC */
//#define DIAG_SYS_PARAM_F          157 

/* NV Image Backup, Dump and Verify */
#define DIAG_BENQ_NV_IMAGE_F      158

//#define DIAG_BENQ_FACTORY_WRITE_F 159
/*===========================================================================

PACKET   diag_benq_factory_data_write_req_type

ID       DIAG_BENQ_FACTORY_WRITE_F
 
PURPOSE  Write Factory Data

===========================================================================*/
#define DIAG_FACTORY_NV_ITEM_SIZE  128
typedef PACKED struct 
{

	byte        cmd_code;
	byte	    seq_num;
	byte		more;
    uint8		item_data[DIAG_FACTORY_NV_ITEM_SIZE]; 
	uint16		item;

} diag_benq_factory_data_write_req_type;
typedef PACKED struct 
{
	byte        	cmd_code;
	uint16		nv_stat;
} diag_benq_factory_data_write_rsp_type;

//#define DIAG_BENQ_FACTORY_READ_F  160
/*===========================================================================

PACKET   diag_benq_factory_data_read_req_type

ID       DIAG_BENQ_FACTORY_READ_F
 
PURPOSE  Read Factory Data

===========================================================================*/
typedef PACKED struct 
{
	byte        cmd_code;
	byte	    seq_num;
	byte		more;
	uint16		item;                         /* Which item - use nv_items_enum_type */
	uint8		item_data[DIAG_FACTORY_NV_ITEM_SIZE]; /* Item itself - use nv_item_type      */
	uint16		nv_stat;                      /* Status of operation
							 - use nv_stat_enum_type */
} diag_benq_factory_data_read_req_type;

/* Power test command */
//#define DIAG_BENQ_POWER_TEST_F    161
/*===========================================================================

PACKET   diag_power_test_rsp_type

ID       DIAG_BENQ_POWER_TEST_F
 
PURPOSE  Sent by DMSS to relay power test command results

===========================================================================*/
typedef enum
{
    PWTEST_START_F = 0x1000000,
    PWTEST_UNDER_VOLT_TEST_F,
    PWTEST_OVER_VOLT_TEST_F,
    PWTEST_EMPTY_VOLT_TEST_F,
    PWTEST_CHARG_UNDER_VOLT_TEST_F,
    PWTEST_CHARG_OVER_VOLT_TEST_F,
    PWTEST_LOW_THERM_TEST_F ,    
    PWTEST_HIGH_THERM_TEST_F,
    PWTEST_END_F,
	PWTEST_CMD_MAX
} diag_pwtest_cmd_enum_type;
typedef PACKED struct {
  byte                        cmd_code;
  diag_pwtest_cmd_enum_type   pwtest_cmd;
  boolean                     result;
  //diag_pwtest_cmd_parm_type   pwtest_parm;  
} diag_power_test_rsp_type;

/* MMI to UNICODE or UNICODE to MMI */
//#define DIAG_BENQ_CONV_FONTCODE_F 162
/*===========================================================================
PACKET   diag_benq_conv_fontcode_type

ID       DIAG_BENQ_CONV_FONTCODE_F
===========================================================================*/
#define DIAG_BENQ_CONV_DATA_SIZE 128
typedef enum 
{
  MMI_CONVERT_TO_UNICODE = 0,
  UNICODE_CONVERT_TO_MMI,
  PIM_CONVERT_MAX,
  /*-------------------------------------------------------------------------*/

  PIM_CONVERTMODE_PAD = 0x7FFF   /* Pad to 16 bits on ARM                    */

} pim_convertmode_type;
typedef PACKED struct {
  byte cmd_code;
  pim_convertmode_type mode;
  byte len;                               /* Length of data block */
  byte data[DIAG_BENQ_CONV_DATA_SIZE];    /* Data block           */
}diag_benq_conv_fontcode_type;

/* Scheduler read command */
//#define DIAG_BENQ_SCH_READ_F      163
/* Scheduler write command */
//#define DIAG_BENQ_SCH_WRITE_F     164
/*===========================================================================
PACKET   diag_benq_sch_type

ID       DIAG_BENQ_SCH_READ_F,DIAG_BENQ_SCH_WRITE_F
===========================================================================*/
typedef PACKED struct {
  byte cmd_code;
  uint16 item;
  pim_option_type option;         /* PIM option */
  word    schIndex;               /* record real location */
  nv_scheduler_s item_data;       /* scheduler data */
  uint16 stat;
}diag_benq_sch_type;

/* PIM sync lock command */
//#define DIAG_BENQ_PIM_LOCK_F      165
/*===========================================================================
PACKET   diag_benq_pim_lock_type

ID       DIAG_BENQ_PIM_LOCK_F
===========================================================================*/
typedef PACKED struct {
  byte cmd_code;
  boolean         pimlock;          /* TRUE => lock, FALSE => unlock */  
  dword           lock_time;	    /* ms unit */
  uint16          stat;
}diag_benq_pim_lock_type;

/* PIM portraittable read command */
//#define DIAG_BENQ_PT_READ_F       166
/* PIM portraittable write command */
//#define DIAG_BENQ_PT_WRITE_F      167
/*===========================================================================
PACKET   diag_benq_pt_type

ID       DIAG_BENQ_PT_READ_F,DIAG_BENQ_PT_WRITE_F
===========================================================================*/
typedef PACKED struct {
  byte cmd_code;
  uint16 item;
  pim_option_type option;         /* PIM option */
  word    address;                /* record real location */
  nv_PortraitTableS item_data;    /* portraittable data */
  uint16 stat;
}diag_benq_pt_type;

//#define DIAG_QUERY_PROJECT_INFO_F 168
/*===========================================================================

PACKET   diag_project_info_req_type

ID       DIAG_QUERY_PROJECT_INFO_F
 
PURPOSE  Get project code from HS

===========================================================================*/
typedef PACKED struct {
    byte    cmd_code;
} diag_project_info_req_type;

/*===========================================================================
PACKET   diag_benq_speed_dial_type

ID       DIAG_BENQ_SPEED_DIAL_READ_F,DIAG_BENQ_SPEED_DIAL_WRITE_F
===========================================================================*/
typedef PACKED struct {
  byte cmd_code;
  uint16 item;
  nv_SpeedDialS item_data;    /* NV_SPEED_DIAL_I data */
  uint16 stat;
}diag_benq_speed_dial_type;

#define DIAG_EXT_MAX_F            171

/*===========================================================================

PACKET   diag_power_test_req_type

ID       DIAG_BENQ_POWER_TEST_F

PURPOSE  Set power test commands to phone

RESPONSE 

===========================================================================*/
typedef PACKED struct {
  byte                       cmd_code;
  diag_pwtest_cmd_enum_type  pwtest_cmd;
  boolean                    result;
  //diag_pwtest_cmd_parm_type  pwtest_parm;
} diag_power_test_req_type;

/*===========================================================================

PACKET   diag_rfc_rsp_type

ID       DIAG_RFCAL_F
 
PURPOSE  Sent by DMSS to relay RF calibration command results

===========================================================================*/
typedef __packed struct {
  byte                        cmd_code;
  diag_rfc_cmd_enum_type      rfc_cmd;
  boolean                     result;
  diag_rfc_results_type       data;
} diag_rfc_rsp_type;

/*===========================================================================

PACKET   diag_rfc_cmd_type

ID       DIAG_RFCAL_CMD_F

PURPOSE  RFCal set commands to phone

RESPONSE 

===========================================================================*/



typedef PACKED struct {
  byte                         cmd_code;
  diag_rfc_cmd_enum_type       rfc_cmd;
  diag_rfc_cmd_parm_type       rfc_parm;
} diag_rfc_cmd_type;

/*===============================================================
 sub command
=================================================================*/
typedef PACKED enum
{
  BENQDIAG_PIM_MMS = 0,
  BENQDIAG_SW_VERSION =1,
  BENQDIAG_BBTEST = 2,
  BENQDIAG_ERR_LOG_GET = 3,
  BENQDIAG_PROJECT_INFO = 4,
  BENQDIAG_USB_CHARGING = 5,
  BENQDIAG_CHARGER_CTL = 5,
  BENQDIAG_PB_TOOL = 6,
  BENQDIAG_PT_TABLE = 7,
  BENQDIAG_SCHEDULE = 8,
  BENQDIAG_SMS = 9,
  BENQDIAG_CG = 10,
  BENQDIAG_EFS = 11,
  BENQDIAG_GETGPIO = 12,
  BENQDIAG_SW_FEATURE = 13,
  BENQDIAG_NV_IMAGE = 14,
  BENQDIAG_EXT_FACTORY = 15,
  BENQDIAG_PIM_QUERY = 16,
  BENQDIAG_WCDMA_INFO = 17,
  BENQDIAG_DEL_PBCLLOGSMSSCHALARM = 18,
  BENQDIAG_GSM_INFO   = 21,  
  BENQDIAG_GETFDNSTATUS = 22,
     BENQDIAG_EFS_RESET = 23,
     BENQDIAG_NV = 24,
     BENQDIAG_PREPAY = 25, 
/* The following command codes are for Topaz! */

     BENQDIAG_PHONEBOOK_V2 = 26,
     BENQDIAG_CALLERGROUP_V2 = 27,
     BENQDIAG_SCHEDULE_V2 = 28,
     BENQDIAG_SMS_V2 = 29,
     BENQDIAG_DLOAD = 30,  

/* 31 reserved for Ulysses */

     BENQDIAG_BBTEST_V2 = 32,
     BENQDIAG_EFS_V2 = 33,
	 BENQDIAG_AKEY_WRITE = 34,
	 BENQDIAG_USB_MS_MODE = 35,

  //Melody Chiu for ULYSSES3, 2004/12/23 {
  BENQDIAG_KEYPAD_SIMULATION = 1025,
  BENQDIAG_CHARGER_INFORMATION = 1026,
  BENQDIAG_HW_MONITOR = 1027,
  BENQDIAG_CM_SET_BAND_PREF = 1029, 
  //}
  BENQDIAG_CNV_IMAGE = 1030,
  //}
  BENQDIAG_IS_ALREADY_POWER_ON  = 1031,
  BENQDIAG_MAX
} benqdiag_cmd_code_type;

/*================================================================
  Eugenia temp
  sw version

==================================================================*/
#define SOFTWARE_VERSION_STRING 80

typedef struct
{
  //+ diagpkt_subsys_header_type header;
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;

} benqdiag_sw_version_req_type;

typedef struct
{
	//+ diagpkt_subsys_header_type   header;
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;

	byte major;
	byte minor;
	byte area;
	byte custID;
	byte model;
	byte rev;
	byte nvData;
	char strver[SOFTWARE_VERSION_STRING];  /* save software version string */
} benqdiag_sw_version_rsp_type;

/*============================================================
 sub command for BBTest DIAG
==============================================================*/
typedef enum {
	// Copy from Domingo DIAG_BBTEST_F related{
	BENQDIAG_BBTEST_START_F,
    BENQDIAG_BBTEST_KEYPAD_TEST_F,
	BENQDIAG_BBTEST_SND_TEST_F,
	BENQDIAG_BBTEST_BACKLIGHT_F,
	BENQDIAG_BBTEST_LED_F,
	BENQDIAG_BBTEST_LCD_F,
    BENQDIAG_BBTEST_VIBRATOR_F,
    BENQDIAG_BBTEST_RTC_F,
    BENQDIAG_BBTEST_LCD_MESSAGE_F,
    BENQDIAG_BBTEST_ADC_READ_F,
    BENQDIAG_BBTEST_SET_APATH_F,
    BENQDIAG_BBTEST_RTC_CLEAR_ALARM_F,
    BENQDIAG_BBTEST_END_F,
    BENQDIAG_BBTEST_LCD_SUB_F,
    BENQDIAG_BBTEST_FUN_LCD_CLEAN_F,  //This command is for clean LCD 
    // }
    BENQDIAG_BBTEST_RTC_TIME_READ_F,  /* Read RTC time from DB */
    BENQDIAG_BBTEST_BATT_TEMP_READ_F, /* Read battery temperature */
    BENQDIAG_BBTEST_CHG_STATE_READ_F, /* Read charging state */
    BENQDIAG_BBTEST_KEY_LOG_START_F,  /* Start to log key press. */
    BENQDIAG_BBTEST_KEY_LOG_STOP_F,   /* Stop to log key press. */
    BENQDIAG_BBTEST_KEY_LOG_READ_F,    /* Read key pressed log. */
	BENQDIAG_BBTEST_READ_IMSI_F,
	BENQDIAG_BBTEST_DSC_F,
	BENQDIAG_BBTEST_FLASH_F,
	BENQDIAG_BBTEST_MP3_F,
    BENQDIAG_BBTEST_IRDA_F, /* Irda test */
    BENQDIAG_BBTEST_VIDEO_F, /* Video test */
    BENQDIAG_BBTEST_BT_F, /*Bluetooth connection test*/
    BENQDIAG_BBTEST_BT_LOOPBACK_F, /*Bluetooth audio Loopback test*/
    BENQDIAG_BBTEST_PEAKPWR_F,
    BENQDIAG_BBTEST_VGA_F,    //Owen Chou for UlyssesB1A
    BENQDIAG_BBTEST_BATT_VOLTAGE_READ_F,
    BENQDIAG_BBTEST_COINBATT_VOLTAGE_READ_F,
    BENQDIAG_BBTEST_PA_TEMP_READ_F,
    BENQDIAG_BBTEST_CHG_CUR_READ_F,
	BENQDIAG_BBTEST_VBATT_TEMP_SW_F,
	BENQDIAG_BBTEST_AUTO_KEY_LOCK_F,
	//Melody Chiu for ULYSSES3 {
	BENQDIAG_BBTEST_BT_RF_TEST_F=100,
	//}
    BENQDIAG_BBTEST_PWRLVL_F=200

} benqdiag_bbtest_cmd_enum_type;


/*==================================================================
 BBTest DIAG
===================================================================*/

typedef PACKED struct {
    uint8        on;       
    uint8        mode;     
    uint8	   level;
    uint16	   channel;
}benqdiag_level_pwr_type;


typedef PACKED struct {
    uint8        on;       
    uint8        mode;  
}benqdiag_peak_pwr_type;


typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    benqdiag_bbtest_cmd_enum_type   bbtest_cmd;
} benqdiag_bbtest_req_type;


typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    uint8   bbtest_cmd; /* benqdiag_bbtest_cmd_enum_type */
    boolean                         result;        /* TRUE if bbtest success. FALSE if failed. */
    PACKED union {
        int                         batt_therm;    /* Battery temperature in Celsius degree */
        uint8                       chg_state;     /* Charging state(db_charging_state_type) */
		_clk_julian_type_           current_time;  /* Current time */
        uint8                       key_codes[20]; /* Pressed keys since key log starts.
                                                      Suppose 20 keys in maximum. */
		snd_test_parm_type          snd_test;
		byte                        byte_val;
		lcd_parm_type               lcd_test;
		word                        adc_val;
        byte                        lcd_const;
        byte                        lcd_module;
		lcd_message_type            lcd_message;
		set_apath_type              set_apath;
		rtc_parm_type               rtc_type;
		byte                        irda_test;
        boolean                     result;
		byte                        imsi[10];
		
	        benqdiag_peak_pwr_type      peakpwr;
		benqdiag_level_pwr_type		levelpwr;
    } param;
} benqdiag_bbtest_rsp_type;
 

/*==================================================================
 BTTest DIAG
===================================================================*/
typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    uint8   bbtest_cmd;
    boolean         result;      /* Don't Care */
    uint8           bt_addr[6];  /* BT address */
    char            data[100]; 
    int             OP; /* 0:disconnect BT, 1:connect BT*/
} benqdiag_bttest_req_type;


typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    uint8   bbtest_cmd; /* benqdiag_bbtest_cmd_enum_type */
    boolean   result;        /* TRUE if bbtest success. FALSE if failed. */
    
} benqdiag_bttest_rsp_type;

/*==================================================================
 Peak Power Out DIAG
===================================================================*/
typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    uint8   bbtest_cmd;
    boolean         result;    /* Don't Care */
    uint8           on;        //(1 = TURN ON, 0 = TURN OFF)                       
    uint8           mode;      //( 0 = WCDMA, 1 = GSM, 2 = DCS)
} benqdiag_PeakPowerOut_req_type;


typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    uint8   bbtest_cmd; /* benqdiag_bbtest_cmd_enum_type */
    boolean   result;        /* TRUE if bbtest success. FALSE if failed. */    
} benqdiag_PeakPowerOut_rsp_type;

typedef PACKED struct
{
	/* Four bytes header: diagpkt_subsys_header_type */    
	uint8   command_code;
	uint8   subsys_id;
	uint16  subsys_cmd_code;
 
    uint8   bbtest_cmd;
    boolean         result;    /* Don't Care */
    uint8           on;        //(1 = TURN ON, 0 = TURN OFF)                       
    uint8           mode;      //( 0 = WCDMA, 1 = GSM, 2 = DCS)
    uint8		level;
    uint16		channel;
	
} benqdiag_LevelPowerOut_req_type;


typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    uint8   bbtest_cmd; /* benqdiag_bbtest_cmd_enum_type */
    boolean   result;        /* TRUE if bbtest success. FALSE if failed. */    
} benqdiag_LevelPowerOut_rsp_type;


typedef enum {
    BENQDIAG_BBTEST_WCDMA_PEAKPWR_MODE,
    BENQDIAG_BBTEST_GSM_PEAKPWR_MODE,
    BENQDIAG_BBTEST_DCS_PEAKPWR_MODE,
    BENQDIAG_BBTEST_PCS_PEAKPWR_MODE, //BenQ, Owen.Chou, 25.12.2005
    BENQDIAG_BBTEST_PEAKPWR_MODE_PAD = 0xFF
}benqdiag_peakpwr_test_mode_type;


typedef PACKED struct
{
	/* Four bytes header: diagpkt_subsys_header_type */    
	uint8   command_code;
	uint8   subsys_id;
	uint16  subsys_cmd_code;
} benqdiag_err_log_get_req_type;

//+ rick huang, add temporary
#define     NV_BENQ_ERR_FILE_NAME_LEN 8        /* 8 characters file name length */
typedef PACKED struct nv_benq_err_log_type
{
  byte      address;                         /* Storage address 0 to       */
                                             /*  NV_MAX_ERR_LOG-1          */
  byte      err_count;                       /* Number of occurances       */
                                             /* (0=empty,FF=full)          */
  byte      file_name[NV_BENQ_ERR_FILE_NAME_LEN]; /* File name string */
  word      line_num;                        /* Line number in file */  
  boolean   fatal;                           /* TRUE if fatal error */
  dword     code1;                           /* Message parameter 1 */
  dword     code2;                           /* Message parameter 2 */
  dword     code3;                           /* Message parameter 3 */
} nv_benq_err_log_type;
//+


typedef PACKED struct
{
	/* Four bytes header: diagpkt_subsys_header_type */    
	uint8   command_code;
	uint8   subsys_id;
	uint16  subsys_cmd_code;
	word              log_cnt;            /* how many logged */
	word              ignore_cnt;         /* how many ignored */
	nv_benq_err_log_type   err_logs[ NV_MAX_ERR_LOG ];  /* error records */
} benqdiag_err_log_get_rsp_type;

#define PROJECT_INFO_LENGTH		20		/* project code :10 , custID : 4, productID : 4 */
 
typedef PACKED struct
{
    /* Four bytes header: diagpkt_subsys_header_type */
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    word project_info_size;
    byte diag_project_info[PROJECT_INFO_LENGTH];
} benqdiag_prj_info_rsp_type;

//BENQDIAG_STOP_USB_CHARGING
typedef struct
{
  diagpkt_subsys_header_type header;
  boolean b_chg_onoff;
} benqdiag_usb_charging_req_type;


///////////////////////////////////////////////////////
// copy from hs.h
/*---------------------------------------------------------------------------
  Keypad declarations
---------------------------------------------------------------------------*/

/* These are the keycodes returned by calls to hs_key_get().  Do not depend
** on their numeric value, just use the symbolic values.  Some of these have
** been set to specific values to make decoding the hardware responses
** more convenient.  Note that not all keys apply to all targets and in fact
** some are very ancient in origin.  However, for backward compatiblity
** we never delete keys.
**
** NOTE: These values are used in the interface to external devices.  When
** defining new keys, be sure to choose a unique new value, to preserve the
** interface.
*/
// BenQ Rick Huang, 2004/6/18 SCR no. {
#define FEATURE_NAV_KEYPAD
typedef enum {          /* KEYS                                    */
  HS_NONE_K = 0,        /* No more keycodes available              */
  HS_ON_HOOK_K,         /* phone has gone on-hook                  */
  HS_OFF_HOOK_K,        /* phone has gone off-hook                 */
  HS_RING_VOL_0_K,      /* ringer volume 0 (Keep 0-2 in order)     */
  HS_RING_VOL_1_K,      /* ringer volume 1 ( and sequential!!)     */
  HS_RING_VOL_2_K,      /* ringer volume 2                         */
  HS_EAR_UP_K,          /* earpiece up                             */
  HS_EAR_UP_END_K,      /* earpiece up + end pressed               */
  HS_EAR_DOWN_K,        /* earpiece down                           */
  HS_PF1_K,             /* softkey #1 (left-most)                  */
#ifdef FEATURE_NAV_KEYPAD
  HS_LEFT_K = HS_PF1_K, /* left key                                */
#endif /* FEATURE_NAV_KEYPAD */
  HS_PF2_K,             /* softkey #2 (right-most)                 */
#ifdef FEATURE_NAV_KEYPAD
  HS_RIGHT_K = HS_PF2_K,/* right key                               */
#endif /* FEATURE_NAV_KEYPAD */
  HS_MSG_K,             /* message waiting                         */
  HS_POUND_K = 0x23,    /* '#' key, ASCII '#'                      */
  HS_STAR_K = 0x2A,     /* '*' key, ASCII '*'                      */
  HS_0_K = 0x30,        /* '0' key, ASCII '0'                      */
  HS_1_K = 0x31,        /* '1' key, ASCII '1'                      */
  HS_2_K = 0x32,        /* '2' key, ASCII '2'                      */
  HS_3_K = 0x33,        /* '3' key, ASCII '3'                      */
  HS_4_K = 0x34,        /* '4' key, ASCII '4'                      */
  HS_5_K = 0x35,        /* '5' key, ASCII '5'                      */
  HS_6_K = 0x36,        /* '6' key, ASCII '6'                      */
  HS_7_K = 0x37,        /* '7' key, ASCII '7'                      */
  HS_8_K = 0x38,        /* '8' key, ASCII '8'                      */
  HS_9_K = 0x39,        /* '9' key, ASCII '9'                      */
  HS_SEND_K = 0x50,     /* Send key                                */
  HS_END_K = 0x51,      /* End key or Power key (Based on Target)  */
  HS_CLR_K = 0x52,      /* Clear key                               */
  HS_STO_K = 0x53,      /* Store key                               */
  HS_UP_K = 0x54,       /* Up-arrow key was pressed                */
  HS_DOWN_K = 0x55,     /* Down-arrow key was pressed              */
  HS_MUTE_K = 0x56,     /* Mute Key                                */
  HS_RCL_K = 0x57,      /* Recall key                              */
  HS_SD1_K = 0x58,      /* speed dial #1                           */
  HS_SD2_K = 0x59,      /* speed dial #2                           */
  HS_SD3_K = 0x5A,      /* speed dial #3                           */
  HS_MENU_K = 0x5B,     /* Menu key                                */
  HS_ALPHA_K = 0x5C,    /* Alpha key                               */
  HS_DEC_POUND_K,       /* Decoded # DTMF                          */
  HS_DEC_STAR_K,        /* Decoded * DTMF                          */
  HS_DEC_0_K,           /* Decoded 0 DTMF                          */
  HS_DEC_1_K,           /* Decoded 1 DTMF                          */
  HS_DEC_2_K,           /* Decoded 2 DTMF                          */
  HS_DEC_3_K,           /* Decoded 3 DTMF                          */
  HS_DEC_4_K,           /* Decoded 4 DTMF                          */
  HS_DEC_5_K,           /* Decoded 5 DTMF                          */
  HS_DEC_6_K,           /* Decoded 6 DTMF                          */
  HS_DEC_7_K,           /* Decoded 7 DTMF                          */
  HS_DEC_8_K,           /* Decoded 8 DTMF                          */
  HS_DEC_9_K,           /* Decoded 9 DTMF                          */
  HS_DEC_A_K,           /* Decoded A DTMF                          */
  HS_DEC_B_K,           /* Decoded B DTMF                          */
  HS_DEC_C_K,           /* Decoded C DTMF                          */
  HS_DEC_D_K,           /* Decoded D DTMF                          */
  HS_PWR_ON_K,          /* Power key is switched 'on'              */
  HS_PWR_OFF_K,         /* Power key is switched 'off'             */
  HS_PWR_K,             /* Power key                               */
  HS_INFO_K,            /* Info key                                */
#if defined(FEATURE_NAV_KEYPAD) || defined(FEATURE_RESISTIVE_KEYPAD)
  HS_SEL_K = HS_INFO_K, /* Select key                              */
#endif /* FEATURE_NAV_KEYPAD */
  HS_FREE_K,            /* Phone was placed in hands-free cradle   */
  HS_PHONE_K,           /* Phone was lifted from hands-free cradle */
  HS_IGN_OFF_K,         /* Ignition was turned off                 */
  HS_IGN_ON_K,          /* Ignition was turned on                  */
  HS_TMR_ON_K,          /* Power-up timer 'on' pseudo-key          */
  HS_TMR_OFF_K,         /* Power-up timer 'off' pseudo-key         */
  HS_BAD_BATT_K,        /* The car battery is failing              */
  HS_EXT_PWR_ON_K,      /* External power was turned on            */
  HS_EXT_PWR_OFF_K,     /* External power was turned off           */
  HS_REDIAL_K,          /* Redial key                              */
  HS_RSSI_K,            /* RSSI key                                */
  HS_HFK_CORD_K,        /* Coil cord carkit                        */
  HS_HFK_NOCORD_K,      /* Non-coil cord carkit                    */
  HS_HEADSET_K,         /* (Mono) Headset connected                */
  HS_STEREO_HEADSET_K,  /* Stereo Headset connected                */
  HS_HEADSET_OFF_K,     /* Headset disconnected                    */
#ifdef FEATURE_HEADSET_SWITCH
  HS_HEADSET_SWITCH_K,  /* Headset Switch on                       */
  HS_HEADSET_SWITCH_OFF_K,/* Headset Switch off                    */
#endif /* FEATURE_HEADSET_SWITCH */
  HS_LSPKR_K,           /* Loudspeaker key                         */
  HS_PTT_K,             /* PTT key                                 */
  HS_DEV_UNKNOWN_K,     /* Unknown device ID                       */
  HS_EXT_DEV_K,         /* ONES detector finds external device     */
  HS_CHARGING_ON_K,     /* Key to note battery charging active     */
  HS_CHARGING_OFF_K,    /* Key to note battery charging stopped    */
#ifdef FEATURE_STEREO_DAC
  HS_SDAC_ON_K,         /* Key to note SDAC in use                 */
  HS_SDAC_OFF_K,        /* Key to note SDAC not in use             */
#endif
#ifdef FEATURE_USB_ISOC_AUDIO
  HS_USB_HFK_ON_K,      /* Key to note USB in use                  */
  HS_USB_HFK_OFF_K,     /* Key to note USB not in use              */
#endif /* FEATURE_USB_ISOC_AUDIO */
  /* BenQ added start */
//#ifdef FEATURE_NAV_KEYPAD
  HS_LSOFT_K = 0xD0,    /* left softkey key                        */
  HS_RSOFT_K = 0xD1,    /* right softkey key                       */
//#endif /* FEATURE_NAV_KEYPAD */
  HS_L_USRDEF_K = 0xD2, /* Message key                             */
  HS_R_USRDEF_K,        /* Mute key                                */
  HS_SPIN_UP_K,         /* Up-Side key                             */
  HS_SPIN_DOWN_K,       /* Down-Side key                           */
  HS_LOUD_SPK_K,        /* Loud speaker key                        */
  HS_FUNC1_K,           /* Function 1 key                          */
  HS_FUNC2_K,           /* Function 2 key                          */
  HS_OK_K,              /* Ok key                                  */
  HS_CANCEL_K,          /* Cancel key                              */
  HS_CLAM_K,            /* Clam shell                              */
  HS_MP3_K,				/* MP3 Key                                 */ //BenQ Melody Chiu, 2005/01/18
  /* BenQ added end */
  HS_RELEASE_K = 0xff   /* Key to note that all keys are up        */
} hs_key_type;
// }BenQ

/*************************************************************************************
 *   BenQ EXT FACTORY DATA                                                           *
 *************************************************************************************/
#define DIAG_EXT_FACTORY_PKTLEN      128
#define BENQDIAG_EXT_FACTORY_FAIL    0xFFFF
#define DIAG_EXT_FACTORY_SEQ_MAX     4

 typedef PACKED enum {
    DIAG_EXT_FACTORY_READ = 2,                     /* Read one record by location */
    DIAG_EXT_FACTORY_UPDATE = 4,                   /* Update */
    DIAG_NUMEXTFACTORYOPERATIONS              /* number of option events  */
}benqdiag_factory_operation_type;

typedef PACKED struct 
{
    uint8                       command_code;
    uint8                       subsys_id;
    uint16                      subsys_cmd_code;
	uint8						op;
	uint8                       seq;
	uint16                      status; 
	uint8                       data[DIAG_EXT_FACTORY_PKTLEN];
}benqdiag_ext_factory_rsp_type;


/**************************************************************************************
 *   Get GPIO Setting                                                                 *   
 **************************************************************************************/
typedef struct
{
    BYTE                       command_code;
    BYTE                       subsys_id;
    WORD                       subsys_cmd_code;

	BYTE                       ischarger;
	BYTE                       isminisd;
	BYTE                       issim;
}benqdiag_get_gpio_rsp_type;


/**************************************************************************************
 * Copy from UMTSDIAG.h
 *************************************************************************************/
/* This Table is from UMTS ICD (80-V4083-1 Rev A) section 3.1 */
#define UMTS_DIAG_VERNO_F      0
#define UMTS_SIM_READ_F        1
#define UMTS_SIM_WRITE_F       2
#define UMTS_SIM_STATUS_F      3
#define UMTS_VERIFY_PIN_F      4
#define UMTS_CHANGE_PIN_F      5
#define UMTS_DISABLE_PIN_F     6
#define UMTS_ENABLE_PIN_F      7
#define UMTS_UNLOCK_PIN_F      8
#define UMTS_CFA_CONFIG_F      9
#define UMTS_CM_STATE_INFO_F   10
#define UMTS_AGC_INFO_F        11
#define DIAG_UMTS_MAX_F        12 /* Must be one greater than highest UMTS cmd */

/**************************************************************************************
 *   RX AGC Value                                                                     *   
 **************************************************************************************/
typedef struct 
{
  BYTE     cmd_code;
  BYTE     subsysid;
  WORD     subsys_cmd_code;
  DWORD    format;     /* AGC specific format       */
}DIAG_SUBSYS_UMTS_UMTS_AGC_INFO_F_req_type;

typedef struct 
{
  /* the RX AGC value */
  short rx_agc;
 
  /* the TX AGC value */
  short tx_agc;
 
  /* the RX AGC ADJ PDM value */
  short rx_agc_adj_pdm;
 
  /* the TX AGC ADJ PDM value */
  short tx_agc_adj_pdm;
 
  /* The TX power limit */
  short tx_pwr_lim;
 
  /* a bit mask for the following bits:
     [1:0] ==> 00 - both LNA used
               01 - LNA 1 bypassed
               10 - reserved
               11 - both LNA's bypassed
 
     [2]   ==> 0 - PA OFF
               1 - PA ON
 
     [3]   ==> 0 - TX OFF
               1 - TX ON
 
     [5:4] ==> Selected PA Range {0..3}
     [7:6] ==> Reserved */
  BYTE amp_flags;
} agc_sample_struct_type;


typedef struct
{
    DWORD              FN;
    WORD               arfcn;
    DWORD              rssi;
    signed short       pwr_dBm_x16;
    short              dc_offset_i;
    short              dc_offset_q;
    short              freq_offset;
    short              timing_offset;
    WORD               snr;
    BYTE               gain_state;
} l1_log_burst_metrics_T;

/* Response packet */
typedef struct 
{
  BYTE     cmd_code;
  BYTE     subsysid;
  WORD     subsys_cmd_code;
  DWORD    format;     /* AGC specific format       */
  DWORD    length;     /* data length of next field */
  union{
      agc_sample_struct_type  wcdma;
      l1_log_burst_metrics_T  gsm;
  }data;    /* Variable length data */
}DIAG_SUBSYS_UMTS_UMTS_AGC_INFO_F_rsp_type;


/*************************************************************************************
 *   BenQ WCDMA Information                                                          *
 *************************************************************************************/
 typedef PACKED struct
 {
    uint8                       command_code;
    uint8                       subsys_id;
    uint16                      subsys_cmd_code;

    uint16                      wcdma_channel; 
    uint8                       wcdma_rssi;	
}benqdiag_wcdma_info_rsp_type;


 //BENQDIAG_STOP_USB_CHARGING
typedef struct
{
  diagpkt_subsys_header_type header;
  byte chg_ctl;     /* 0: usb stop, 1: usb start, 2: std stop, 3: std start */
} benqdiag_charger_ctl_req_type;
  

/*************************************************************************************
 *   BenQ GSM Information                                                          *
 *************************************************************************************/


typedef PACKED struct
 {
    uint8                       command_code;
    uint8                       subsys_id;
    uint16                      subsys_cmd_code;
 
    int32                       gsm_txpwr;
}benqdiag_gsm_info_rsp_type;

/*************************************************************************************** 
 * IrDA Test 
 **************************************************************************************/
typedef enum

{
    BBTEST_IRDA_TESTTARGET_F = 0,

    BBTEST_IRDA_REPRETER_F,

    BBTEST_IRDA_TEST_CMD_MAX

} diag_bbtest_irda_cmd_enum_type;

// BenQ Alice HY Su, 2004/12/01 {
// copy from sys.h
/*---------------------------------------------------------------------------
  Macro to convert bit number to bit mask
---------------------------------------------------------------------------*/
#define SYS_BM( val ) (1<<(int)(val))
/*---------------------------------------------------------------------------
** Enumeration of system band classes and band classes' sub-bands.
** If there is a new band class, append to the end of list.
---------------------------------------------------------------------------*/
typedef enum {
  SYS_SBAND_BC0_A = 0,
    /* Band Class 0, A-System */
 
  SYS_SBAND_BC0_B = 1,
    /* Band Class 0, B-System */
 
    /* Band Class 0 AB , GSM 850 Band*/
 
  SYS_SBAND_BC1 = 2,
    /* Band Class 1, all blocks */
 
  SYS_SBAND_BC2 = 3,
    /* Band Class 2 place holder */
 
  SYS_SBAND_BC3 = 4,
    /* Band Class 3, A-System */
 
  SYS_SBAND_BC4 = 5,
    /* Band Class 4, all blocks */
 
  SYS_SBAND_BC5 = 6,
    /* Band Class 5, all blocks */
 
  SYS_SBAND_GSM_DCS_1800 = 7,
    /* GSM DCS band */
 
  SYS_SBAND_GSM_EGSM_900 = 8,
    /* GSM Extended GSM (E-GSM) band */
 
  SYS_SBAND_GSM_PGSM_900 = 9,
    /* GSM Primary GSM (P-GSM) band */
 
  SYS_SBAND_BC6 = 10,
    /* Band Class 6 */
 
  SYS_SBAND_BC7 = 11,
    /* Band Class 7 */
 
  SYS_SBAND_BC8 = 12,
    /* Band Class 8 */
 
  SYS_SBAND_BC9 = 13,
    /* Band Class 9*/
 
  SYS_SBAND_BC10 = 14,
    /* Band Class 10 */
 
  SYS_SBAND_BC11 = 15,
    /* Band Class 11 */
 
  SYS_SBAND_GSM_450 = 16,
    /* GSM 450 band */
 
  SYS_SBAND_GSM_480 = 17,
    /* GSM 480 band */
 
  SYS_SBAND_GSM_750 = 18,
    /* GSM 750 band */
 
  SYS_SBAND_GSM_850 = 19,
    /* GSM 850 band */
 
  SYS_SBAND_GSM_RGSM_900 = 20,
    /* GSM Railways GSM Band */
 
  SYS_SBAND_GSM_PCS_1900 = 21,
    /* GSM PCS band */
 
  SYS_SBAND_WCDMA_I_IMT_2000 = 22,
    /* WCDMA I IMT 2000 band */
 
  SYS_SBAND_WCDMA_II_PCS_1900 = 23,
    /* WCDMA II PCS band */
 
  SYS_SBAND_WCDMA_III_1700 = 24,
    /* WCDMA III 1700 band */
 
  SYS_SBAND_WCDMA_IV_1700 = 25,
    /* WCDMA IV 1700 band */
 
  SYS_SBAND_WCDMA_V_850 = 26,
    /* WCDMA V US850 band */
 
  SYS_SBAND_WCDMA_VI_800 = 27,
    /* WCDMA VI JAPAN 800 band */
 
  SYS_SBAND_MAX
    /* FOR INTERNAL USE OF CM ONLY! */
 
} sys_sband_e_type;
// copy from cm.h


/*---------------------------------------------------------------------------
  Enumeration of mode preferences
---------------------------------------------------------------------------*/
typedef enum cm_mode_pref_e {

  CM_MODE_PREF_NONE=-1,
    /* FOR INTERNAL USE OF CM ONLY! */

  CM_MODE_PREF_AMPS_ONLY=0,
    /* = NV_MODE_ANALOG_ONLY, Service is limited to analog only */

  CM_MODE_PREF_DIGITAL_ONLY=1,
    /* = NV_MODE_DIGITAL_ONLY, Service is limited to digital
    ** (CDMA, HDR, GW) only */

  CM_MODE_PREF_AUTOMATIC=2,
    /* = NV_MODE_AUTOMATIC, Determine the mode automatically */

  CM_MODE_PREF_EMERGENCY=3,
    /* = NV_MODE_EMERGENCY, Emergency mode
    **
    **  It's intended to be used internally by CM.
    **  Client is not allowed to use it as parameter to change system
    **  selection preference command.
    **
     */

  /* For compatibility with QPST, do not change values or
  ** order. We start with NV_MODE_CELL_CDMA_ONLY+1 (i.e. 9).
  */
  CM_MODE_PREF_CDMA_ONLY = 9,
    /*
    ** Service is limited to CDMA only
    */

  CM_MODE_PREF_HDR_ONLY=10,
    /*
    ** Service is limited to HDR only
    */

  CM_MODE_PREF_CDMA_AMPS_ONLY=11,
    /*
    ** Service is limited to CDMA and AMPS only
    */

  CM_MODE_PREF_GPS_ONLY=12,
    /*
    ** Service is limited to GPS only
    */

  /*
  ** The following enums are the radio access technologies for
  ** WCDMA and GSM.
  */
  CM_MODE_PREF_GSM_ONLY=13,
    /*
    ** Service is limited to GSM only
    */

  CM_MODE_PREF_WCDMA_ONLY=14,
    /*
    ** Service is limited to WCMDA only
    */

  /*
  ** Others
  */
  CM_MODE_PREF_PERSISTENT=15,
    /*
    ** Return to persistent mode preference.
    ** Will be mapped to sth else by CM,
    ** ph_ptr->mode_pref is never set to this value
    */

  CM_MODE_PREF_NO_CHANGE=16,
    /*
    ** Do not change the mode preference.
    ** Will be mapped to sth else by CM,
    ** ph_ptr->mode_pref is never set to this value
    */

  /*
  ** Even though logically the below belog with
  ** enums listed above, they were added at the end
  ** to not break compatibility with test tools
  */

  CM_MODE_PREF_ANY_BUT_HDR=17,
    /*
    ** Use any service but HDR
    */

  CM_MODE_PREF_CURRENT_LESS_HDR=18,
    /*
    ** Remove HDR component from current mode preference.
    ** Will be mapped to sth else by CM,
    ** ph_ptr->mode_pref is never set to this value
    */

  CM_MODE_PREF_GSM_WCDMA_ONLY=19,
    /*
    ** Service is limited to GSM and WCDMA only
    */

  CM_MODE_PREF_DIGITAL_LESS_HDR_ONLY=20,
    /* Acquire digital, non-HDR mode systems only
    ** (CDMA or GSM or WCDMA )
    */

  CM_MODE_PREF_CURRENT_LESS_HDR_AND_AMPS=21,
    /*
    ** Remove HDR and AMPS component from current mode preference.
    ** Will be mapped to sth else by CM,
    ** ph_ptr->mode_pref is never set to this value
    */

  CM_MODE_PREF_CDMA_HDR_ONLY=22,
    /*
    ** Acquire CDMA or HDR systems only
    */

  CM_MODE_PREF_CDMA_AMPS_HDR_ONLY=23,
    /*
    ** Acquire CDMA or AMPS or HDR systems only
    */

  CM_MODE_PREF_CURRENT_LESS_AMPS=24,
    /*
    ** Remove AMPS component from current mode preference.
    ** Will be mapped to sth else by CM,
    ** ph_ptr->mode_pref is never set to this value
    */


  CM_MODE_PREF_MAX
    /* FOR INTERNAL USE OF CM ONLY!
    */
} cm_mode_pref_e_type;

/*---------------------------------------------------------------------------
  Enumeration of band preferences
---------------------------------------------------------------------------*/
typedef enum cm_band_pref_e {
 
  CM_BAND_PREF_NONE=0,
    /* FOR INTERNAL USE OF CM ONLY! */
 
  CM_BAND_PREF_BC0_A=SYS_BM(SYS_SBAND_BC0_A),
    /* Acquire band class 0, A-Side systems only */
 
  CM_BAND_PREF_BC0_B=SYS_BM(SYS_SBAND_BC0_B),
    /* Acquire band class 0, B-Side systems only */
 
  CM_BAND_PREF_BC0=SYS_BM(SYS_SBAND_BC0_A)|SYS_BM(SYS_SBAND_BC0_B),
    /* Acquire band class 0 systems only */
 
  CM_BAND_PREF_BC1=SYS_BM(SYS_SBAND_BC1),
    /* Acquire band class 1 systems only */
 
  CM_BAND_PREF_BC3=SYS_BM(SYS_SBAND_BC3),
    /* Acquire band class 3 systems only */
 
  CM_BAND_PREF_BC4=SYS_BM(SYS_SBAND_BC4),
    /* Acquire band class 4 systems only */
 
  CM_BAND_PREF_BC5=SYS_BM(SYS_SBAND_BC5),
    /* Acquire band class 5 systems only */
 
  CM_BAND_PREF_GSM_DCS_1800=SYS_BM(SYS_SBAND_GSM_DCS_1800),
    /* Acquire GSM DCS systems only */
 
  CM_BAND_PREF_GSM_EGSM_900=SYS_BM(SYS_SBAND_GSM_EGSM_900),
    /* Acquire GSM Extended GSM 900 systems only */
  
  CM_BAND_PREF_GSM_900_1800=SYS_BM(SYS_SBAND_GSM_EGSM_900)|SYS_BM(SYS_SBAND_GSM_DCS_1800),
    /* Acquire GSM 900/1800 systems both */
 
  CM_BAND_PREF_GSM_PGSM_900=SYS_BM(SYS_SBAND_GSM_PGSM_900),
    /* Acquire GSM Primary GSM 900 systems only */
 
  CM_BAND_PREF_BC6=SYS_BM(SYS_SBAND_BC6),
    /* Acquire band class 6 systems only */
 
  CM_BAND_PREF_BC7=SYS_BM(SYS_SBAND_BC7),
    /* Acquire band class 7 systems only */
 
  CM_BAND_PREF_BC8=SYS_BM(SYS_SBAND_BC8),
    /* Acquire band class 8 systems only */
 
  CM_BAND_PREF_BC9=SYS_BM(SYS_SBAND_BC9),
    /* Acquire band class 9 systems only */
 
  CM_BAND_PREF_BC10=SYS_BM(SYS_SBAND_BC10),
    /* Acquire band class 10 systems only */
 
  CM_BAND_PREF_BC11=SYS_BM(SYS_SBAND_BC11),
    /* Acquire band class 11 systems only */
 
  CM_BAND_PREF_GSM_450=SYS_BM( SYS_SBAND_GSM_450 ),
    /* Acquire GSM 450 systems only */
 
  CM_BAND_PREF_GSM_480=SYS_BM( SYS_SBAND_GSM_480 ),
    /* Acquire GSM 480 systems only */
 
  CM_BAND_PREF_GSM_750=SYS_BM( SYS_SBAND_GSM_750 ),
    /* Acquire GSM 750 systems only */
 
  CM_BAND_PREF_GSM_850=SYS_BM( SYS_SBAND_GSM_850 ),
    /* Acquire GSM 850 systems only */
 
  CM_BAND_PREF_GSM_RGSM_900=SYS_BM(SYS_SBAND_GSM_RGSM_900),
    /* Acquire GSM Railway GSM 900 systems only */
 
  CM_BAND_PREF_GSM_PCS_1900=SYS_BM( SYS_SBAND_GSM_PCS_1900 ),
    /* Acquire GSM PCS systems only */
 
  CM_BAND_PREF_WCDMA_I_IMT_2000=SYS_BM( SYS_SBAND_WCDMA_I_IMT_2000 ),
    /* Acquire WCDMA I IMT 2000 systems only */
 
  CM_BAND_PREF_WCDMA_II_PCS_1900=SYS_BM( SYS_SBAND_WCDMA_II_PCS_1900 ),
    /* Acquire WCDMA II PCS systems only */
 
  CM_BAND_PREF_WCDMA_III_1700=SYS_BM( SYS_SBAND_WCDMA_III_1700 ),
    /* Acquire WCDMA III 1700 systems only */
 
  CM_BAND_PREF_WCDMA_IV_1700=SYS_BM( SYS_SBAND_WCDMA_IV_1700 ),
    /* Acquire WCDMA IV 1700 systems only */
 
  CM_BAND_PREF_WCDMA_V_850=SYS_BM( SYS_SBAND_WCDMA_V_850 ),
    /* Acquire WCDMA V 850 systems only */
 
  CM_BAND_PREF_WCDMA_VI_800=SYS_BM( SYS_SBAND_WCDMA_VI_800 ),
    /* Acquire WCDMA VI 800 systems only */
 
  CM_BAND_PREF_ANY=0x3FFFFFFF,
    /* Acquire any band class systems */
 
  CM_BAND_PREF_NO_CHANGE,
    /* Do not change the band preference */
 
  CM_BAND_PREF_MAX
    /* FOR INTERNAL USE OF CM ONLY! */
 
  #ifdef FEATURE_RPC
#error code not present
  #endif /* FEATURE_RPC */
 
} cm_band_pref_e_type;
/*===========================================================================

PACKET   benqdiag_cm_set_band_pref_req_type

ID       BENQDIAG_CM_SET_BAND_PREF 

PURPOSE  This message requests to change phone preferences.

RESPONSE The benqdiag_cm_set_band_pref_rsp_type will send change state in response.

===========================================================================*/
 typedef PACKED struct
{
    uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
 
    uint32    mode_pref;  /* Mode preference, see cm_mode_pref_e */
	uint32    band_pref;  /* Band preference, see cm_band_pref_e */
}benqdiag_cm_set_band_pref_req_type;
 
typedef PACKED struct
{
    uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
 
    uint8     result;
}benqdiag_cm_set_band_pref_rsp_type;
// }BenQ  

/*===========================================================================
PACKET   benqdiag_del_pbcllogsmsschalarm_rsp_type

ID       BENQDIAG_DEL_PBCLLOGSMSSCHALARM

PURPOSE	 Clear Call log, phone book, schedule, SMS, and alarm
===========================================================================*/
typedef PACKED struct
{
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;    

    byte  pb_status;
    byte  lnm_status;
    byte  lnr_status;
    byte  lnd_status;
    byte  sms_status;
    byte  schedule_status;
    byte  alarm_status;
} benqdiag_del_pbcllogsmsschalarm_rsp_type;

typedef struct
{
    uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code; 

    uint8      bbtest_cmd;    /* benqdiag_bbtest_cmd_enum_type */
    byte	   byte_val;        
}benqdiag_LED_req_type;

typedef struct
{
    uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code; 

    uint8      bbtest_cmd;    /* benqdiag_bbtest_cmd_enum_type */
    boolean    result;        /* TRUE if bbtest success. FALSE if failed. */
}benqdiag_LED_Rsp_type;


//BenQ Melody Chiu, for ULYSSES3, 2004/12/23{

//Copy from qcameradiag.h
/*============================================================================
   Enumerated types for qcamera diag
============================================================================*/

typedef enum
{
  QCAMERADIAG_START = 0,
  QCAMERADIAG_END,
  QCAMERADIAG_PREVIEW,
  QCAMERADIAG_TAKE_PICTURE,
  QCAMERADIAG_INCREASE_SHARPNESS,
  QCAMERADIAG_DECREASE_SHARPNESS,
  QCAMERADIAG_INCREASE_CONTRAST,
  QCAMERADIAG_DECREASE_CONTRAST,
  QCAMERADIAG_INCREASE_BRIGHTNESS,
  QCAMERADIAG_DECREASE_BRIGHTNESS,
  QCAMERADIAG_INCREASE_ZOOM,
  QCAMERADIAG_DECREASE_ZOOM,
  QCAMERADIAG_SMART_FLASH_ON,
  QCAMERADIAG_SMART_FLASH_OFF,
  QCAMERADIAG_INCREASE_HUE,
  QCAMERADIAG_DECREASE_HUE,
  QCAMERADIAG_INCREASE_SATURATION,
  QCAMERADIAG_DECREASE_SATURATION,
  QCAMERADIAG_ROTATE_IMAGE,
  QCAMERADIAG_FLIP_IMAGE,
  QCAMERADIAG_CONVERT_TO_GRAYSCALE,
  QCAMERADIAG_INVERT_COLORS,
  QCAMERADIAG_RESTORE_IMAGE,
  QCAMERADIAG_DELETE_ALL_IMAGES,
  QCAMERADIAG_ALBUM_MODE,
  QCAMERADIAG_CHANGE_SNAP_SHOT_SELF_TMR_LONG,
  QCAMERADIAG_CHANGE_SNAP_SHOT_SELF_TMR_MED,
  QCAMERADIAG_CHANGE_SNAP_SHOT_SELF_TMR_SHORT,
  QCAMERADIAG_CHANGE_SNAP_SHOT_SELF_TMR_OFF,
  QCAMERADIAG_CHANGE_SHUTTER_SOUND,
  QCAMERADIAG_CHANGE_SLIDE_SHOW_SPEED_1SEC,
  QCAMERADIAG_CHANGE_SLIDE_SHOW_SPEED_2SEC,
  QCAMERADIAG_CHANGE_SLIDE_SHOW_SPEED_4SEC,
  QCAMERADIAG_INCREASE_BRIGHTNESS_EDIT,
  QCAMERADIAG_DECREASE_BRIGHTNESS_EDIT,
  QCAMERADIAG_INCREASE_SHARPNESS_EDIT,
  QCAMERADIAG_DECREASE_SHARPNESS_EDIT,
  QCAMERADIAG_INCREASE_CONTRAST_EDIT,
  QCAMERADIAG_DECREASE_CONTRAST_EDIT,
  QCAMERADIAG_START_SLIDE_SHOW,
  QCAMERADIAG_STOP_SLIDE_SHOW,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_SXGA,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_4VGA,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_XGA,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_VGA,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_QVGA,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_QCIF,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_QQVGA,
  QCAMERADIAG_CHANGE_IMAGE_SIZE_STAMP,
  QCAMERADIAG_CHANGE_PICTURE_QUALITY_LOW,
  QCAMERADIAG_CHANGE_PICTURE_QUALITY_MEDIUM,
  QCAMERADIAG_CHANGE_PICTURE_QUALITY_HIGH,
  QCAMERADIAG_CHANGE_PICTURE_QUALITY_RAW,
  QCAMERADIAG_ADD_FRAME_QQVGA,
  QCAMERADIAG_REMOVE_FRAME_QQVGA,
  QCAMERADIAG_WHITEBALANCE_AUTO,
  QCAMERADIAG_WHITEBALANCE_FLUORESCENT,
  QCAMERADIAG_WHITEBALANCE_INCANDESCENT,
  QCAMERADIAG_WHITEBALANCE_DAYLIGHT,
  QCAMERADIAG_EFFECTS_OFF,
  QCAMERADIAG_EFFECTS_MONOCHROME,
  QCAMERADIAG_EFFECTS_SEPIA,
  QCAMERADIAG_SHUTTER_SOUND_OFF,
  QCAMERADIAG_SHUTTER_SOUND_WAV,
  QCAMERADIAG_SHUTTER_SOUND_QCP,
  QCAMERADIAG_MAX_PIC_10,
  QCAMERADIAG_MAX_PIC_20,
  QCAMERADIAG_MAX_PIC_50,
  QCAMERADIAG_MAX_PIC_NL,
  QCAMERADIAG_REFLECTION_OFF,
  QCAMERADIAG_REFLECTION_MIRROR,
  QCAMERADIAG_REFLECTION_WATER,
  QCAMERADIAG_FRAME_OVERLAY_PREVIEW,
  QCAMERADIAG_ENCODE_STAMP,
  QCAMERADIAG_MEMORY_USAGE,
  QCAMERADIAG_FLASH_OFF,
  QCAMERADIAG_FLASH_AUTO,
  QCAMERADIAG_FLASH_LOW,
  QCAMERADIAG_FLASH_MEDIUM,
  QCAMERADIAG_FLASH_HIGH,
  QCAMERADIAG_EXIT_ALBUM_MODE,
  QCAMERADIAG_ALBUM_MODE_PICTURE_SAVE,
  QCAMERADIAG_ALBUM_SEPIA,
  QCAMERADIAG_ALBUM_UNDO_SEPIA,
  QCAMERADIAG_ALBUM_POSTERIZE_INCREASE,
  QCAMERADIAG_ALBUM_POSTERIZE_DECREASE,
  QCAMERADIAG_ALBUM_SOLARIZE_INCREASE,
  QCAMERADIAG_ALBUM_SOLARIZE_DECREASE,
  QCAMERADIAG_ALBUM_COPY,
  QCAMERADIAG_ALBUM_SEND,
  QCAMERADIAG_ALBUM_DELETE,
  QCAMERADIAG_EXIT_EDIT,
  QCAMERADIAG_MAX_CMD,
  QCAMERADIAG_FORCE_4_BYTES = 0x10000000
} QCAMERADIAG_CommandsType;

typedef struct
{
  diagpkt_subsys_header_type header;

} qcameradiag_cmd_rsp_type;

/******************************************************************************
 *     BENQ UI Keypad Simulator
 ******************************************************************************/
typedef PACKED struct
{
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;
 
    byte    key;
    byte    state;
} benqdiag_keypad_simulation_req_type;
 
typedef enum benqdiag_keypad_simulation_key_state_enum_type 
{
    SHORT_PRESSED = 0,		// auto released
    LONG_PRESSED = 1,		// auto released
    EXTRA_LONG_PRESSED = 2, // auto released, for power on/off 
    PRESSED = 3,			// user released. It would be long pressed for slow diag
    RELEASED = 4,
} benqdiag_keypad_simulation_key_state_enum_type;

typedef benqdiag_keypad_simulation_req_type benqdiag_keypad_simulation_rsp_type;

/******************************************************************************
 *   Charger Information Collection
 ******************************************************************************/
typedef PACKED struct
{
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;

    word    Vchg;
    word    Ichg;
    word    Vbatt;
    word    BattTemp;

    byte ChargeType;	// chg_charger_type
    byte ChargeState;	// batt_chg_state_type
    byte ChargeTimeHR;
    byte ChargeTimeMIN;
    byte ChargeTimeSEC;
    byte BattID;		// batt_id_type

} benqdiag_charger_information_req_type;


typedef enum
{
  CHG_NORMAL_CHARGER,
  CHG_USB_CHARGER,
  CHG_INVALID_CHARGER
} chg_charger_type;

/* Battery ID */
typedef enum {
  BATT_ID_NONE,       /* No Battery       */
  BATT_ID_LI42,       /* Lithium Ion 4.2V */
  BATT_ID_LI42_THICK
} batt_id_type;


/******************************************************************************
 *     Hardware Status Monitor
 ******************************************************************************/
typedef enum { 
 BATTERY_VOLTAGE = 0,
 BT = 1,
 IRDA = 2,
 GPIO_SLEEP = 3,
 LED_BACKLIGHT = 4,
} hw_monitor_entry_enum_type;
 
typedef PACKED struct {
 dword timestamp;
 byte type;
 word status;
} hw_monitor_record_type;
 
typedef PACKED struct {
    byte command_code;
    byte subsys_id;
    word subsys_cmd_code;
 
    byte option;
 PACKED union {
  byte mask;
  byte entry_num;
 } param;
} benqdiag_hw_monitor_req_type;
 
typedef PACKED struct {
    uint8 command_code;
    uint8 subsys_ide;
    uint16 sybsys_cmd_code;
 
    uint8 option; // see hw_monitor_diag_option_enum_type;
    boolean result;
    uint16 drop_cnt;
    uint8 entry_num;
    PACKED union {
 uint16 mask; // see hw_monitor_mask_type
     hw_monitor_record_type log[100]; 
    } param;
} benqdiag_hw_monitor_rsp_type;
 
typedef enum {
 SET_RUNTIME_MASK = 0,
 GET_RUNTIME_MASK,
 GET_STATUS_LOG
} hw_monitor_diag_option_enum_type;
 
//------------------
// AKey
//---------------
#define UI_A_KEY_DIGITS 26
#define AKEY_REQ_RESERVED 8
#define AKEY_RSP_RESERVED 4

typedef struct
{
	uint8	command_code;
	uint8	subsys_id;	
	uint16	subsys_cmd_code; 

    byte    akey[UI_A_KEY_DIGITS];    
    byte    nam;              
	byte    reserved[AKEY_REQ_RESERVED];  /* for future expansion.      */
}benqdiag_akey_req_type;

typedef struct
{
	uint8	command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;        

    word    status; 
    byte    reserved[AKEY_RSP_RESERVED]; 
}benqdiag_akey_rsp_type;

//-----------------
// Backup/restore/erase NV image
//------------------------
typedef enum
{
	BENQDIAG_NV_IMG_UPDATE = 0,
	BENQDIAG_NV_IMG_RESTORE = 1,
	BENQDIAG_NV_IMG_ERASE = 2,
	BENQDIAG_NV_IMG_CHECK = 3,
    BENQDIAG_NV_IMG_ESN_CHECK = 4,
    BENQDIAG_NV_IMG_NV_NUM_CHECK = 5,
	BENQDIAG_NV_IMG_SPC3_CHECK = 6,
	BENQDIAG_NV_IMG_ESN_ERASE = 0x07,       /* Erase the ESN in NV image. */
    BENQDIAG_NV_IMG_SPC3_ERASE = 0x08,     /* Erase the SPC3 in Nv image. */
    BENQDIAG_NV_IMG_ACT_DATE_STAMP_ERASE = 0x09,       /* Erase the ACT_DATE_STAMP in NV image. */
    BENQDIAG_NV_IMG_ALL_ONE_TIME_ITEM_ERASE = 0x0A,     /* Erase All one time programmable item in Nv image. */
    BENQDIAG_NV_IMG_PAD = 0xFF
}benqdiag_nv_img_enum_type;
	
typedef enum {
	NV_IMG_OPERATION_SUCCESS   = 0,
    NV_IMG_UNKNOWN_CMD           = 1,
    NV_IMG_ERASE_FAIL         = 2,   /* Flash erase fail */
    NV_IMG_RESTORE_FAIL        = 3, 
    NV_IMG_UPDATE_FAIL         = 5,
    NV_IMG_READ_NVFS_FAIL      = 6,
    NV_IMG_INVALID                      = 7,   /* Backup image is invalid */
    NV_IMG_INTERNAL_ERR     = 8,
	NV_IMG_TIMEOUT             = 9,   /* Fail to wait until FS stops doing GC */
    NV_IMG_WRITE_FAIL          = 10,  /* Flash write fail */
    NV_IMG_MEM_ALLOC_FAIL      = 11,   /* Fail to allocate memory */
    NV_IMG_ESN_BACKUP_FAIL     =  12,  /* There is no esn number in nv image. */
    NV_IMG_RF_ITEM_BACKUP_FAIL = 13,  /* The RF NV item wasn backup incompletely. */
	NV_IMG_INVALID_PAR                = 14,   /* Invalid parameter */
    NV_IMG_SPC3_BACKUP_FAIL     =  15,  /* There is no spc3 in nv image. */
	NV_IMG_SPC3_ERASE_FAIL          = 16,
    NV_IMG_ACT_DATE_STAMP_ERASE_FAIL  = 17,
    NV_IMG_SPC3_LOCK                 = 18,
    NV_IMG_PAD                              = 0xFFFF
} benqdiag_nv_img_status_type;

typedef struct 
{
    uint8                       command_code;
    uint8                       subsys_id;
    uint16                      subsys_cmd_code;
    uint8                        nv_img_cmd; 
}benqdiag_nv_image_req_type;

typedef struct
{
	uint8     command_code;
	uint8     subsys_id;
	uint16    subsys_cmd_code;	
	uint8     nv_img_cmd;           /* benqdiag_dload_cmd_enum_type */
	uint16    status;              /* status of the operation */
	union 
	{
    uint32     esn_number;    /* the esn number in image */
	uint32     spc3_number;   /* the spc3 number in image */
    struct {
               uint16     nv_num_image;    /* the rf nv item count in image */
               uint16     nv_num_target;    /* the rf nv item count in target */
    }nv_num;                       
	} param;
}benqdiag_nv_image_rsp_type;
//}

/*===========================================================================

PACKET   benqdiag_usb_ms_mode_req_type

ID       BENQDIAG_USB_MS_MODE 

PURPOSE  This message requests to change phone to USB mass storage.

RESPONSE none

===========================================================================*/
typedef PACKED struct {
    uint8                        command_code;
    uint8                        subsys_id;
    uint16                       subsys_cmd_code;
}benqdiag_usb_ms_mode_req_type;

typedef PACKED struct {
    uint8                        command_code;
    uint8                        subsys_id;
    uint16                       subsys_cmd_code;
    uint8                        result;
}benqdiag_usb_ms_mode_rsp_type;

/*===========================================================================

PACKET   benqdiag_is_already_power_on_handler

ID       BENQDIAG_IS_ALREADY_POWER_ON 

PURPOSE  This message requests to get if MMI is in back idle

RESPONSE none

===========================================================================*/
typedef PACKED struct {
    uint8                        command_code;
    uint8                        subsys_id;
    uint16                       subsys_cmd_code;
}benqdiag_is_already_power_on_req_type;

typedef PACKED struct {
    uint8                        command_code;
    uint8                        subsys_id;
    uint16                       subsys_cmd_code;
    uint8                        result;
}benqdiag_is_already_power_on_rsp_type;


#endif /* BENADIAG_H */
