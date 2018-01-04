#ifndef _ALLDIAG_H_
#define _ALLDIAG_H_

#include "comdef.h"
#include "customer.h"
#include "diagcmd.h"
#include "feature.h"
#include "qw.h"

#include "log_codes.h"

#define FTM_PA_LUT_MAX_SIZE                    64                         
#define FTM_NUM_HDET_TO_RETURN	32

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


#define BQTT_DIAGPKT_REQ_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_req_tag \
                 xx_cmd_code##_req_type; \
  PACKED struct  xx_cmd_code##_req_tag { \
    uint8 cmd_code;
#define BQTT_DIAGPKT_REQ_END };

#define BQTT_DIAGPKT_RSP_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_rsp_tag \
                 xx_cmd_code##_rsp_type; \
  PACKED struct  xx_cmd_code##_rsp_tag { \
    uint8 cmd_code;
#define BQTT_DIAGPKT_RSP_END };

// If the response is the same structure as the request...
#define BQTT_DIAGPKT_DEFINE_RSP_AS_REQ( xx_cmd_code ) \
  typedef xx_cmd_code##_req_type xx_cmd_code##_rsp_type;

typedef struct
{
	uint8 cmd_code;
	uint8 subsys_id;
	uint16 subsys_cmd_code;
} subsys_header_type;

#define BQTT_DIAGPKT_SUBSYS_REQ_DEFINE( xx_subsys, xx_subsys_cmd_code ) \
  typedef struct DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_tag \
                 DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_type; \
  PACKED struct  DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_tag { \
    subsys_header_type cmd_header;

#define BQTT_DIAGPKT_SUBSYS_REQ_END };

#define BQTT_DIAGPKT_SUBSYS_RSP_DEFINE( xx_subsys, xx_subsys_cmd_code ) \
  typedef struct DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag \
                 DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_type; \
  PACKED struct  DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag { \
    subsys_header_type cmd_header;

#define BQTT_DIAGPKT_SUBSYS_RSP_END };


///////////////////////////////////////////////////////////////////////////////////////////
//
//							Serial I/O interface Diag Command Packet
//
///////////////////////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------------
//	DIAG_VERNO_F 
//---------------------------------------------------------------------------
#define VERNO_DATE_STRLEN 11
#define VERNO_TIME_STRLEN  8
#define VERNO_DIR_STRLEN   8
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_VERNO_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_VERNO_F)
	char comp_date[ VERNO_DATE_STRLEN ];  // Compile date Jun 11 1991
	char comp_time[ VERNO_TIME_STRLEN ];  // Compile time hh:mm:ss
	char rel_date [ VERNO_DATE_STRLEN ];  // Release date
	char rel_time [ VERNO_TIME_STRLEN ];  // Release time
	char ver_dir  [ VERNO_DIR_STRLEN ];   // Version directory
	byte scm;                             // Station Class Mark
	byte mob_cai_rev;                     // CAI rev
	byte mob_model;                       // Mobile Model
	word mob_firm_rev;                    // Firmware Rev
	byte slot_cycle_index;                // Slot Cycle Index
	byte hw_maj_ver;                      // Hardware Version MSB
	byte hw_min_ver;                      // Hardware Version LSB
BQTT_DIAGPKT_RSP_END

//---------------------------------------------------------------------------
//	DIAG_STATUS_F
//---------------------------------------------------------------------------
#define  NV_MAX_MINS 2
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_STATUS_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_STATUS_F)
	byte    demod;                  // Demod id byte
	byte    decode;                 // Decoder id byte
	byte    inter;                  // Interleaver id byte
	dword   esn;                    // Mobile Electronic Serial Number
	word    rf_mode;                // 0->Analog,  1->CDMA Cellular,  2->CDMA PCS 3->Sleep mpdes, 4->GPS, 5->HDR
	dword   min1[NV_MAX_MINS];      // all MIN1s
	word    min2[NV_MAX_MINS];      // all MIN2s
	byte    orig_min;               // index (0-3) of the orig MIN
	word    cdma_rx_state;          // current state for cdma only
	int1    cdma_good_frames;       // whether or not frames we are receiving are good -- cdma only
	word    analog_corrected_frames;// good frame count - analog only
	word    analog_bad_frames;      // bad frame count - analog only
	word    analog_word_syncs;      //  -- analog only
	word    entry_reason;           // entry reason
	word    curr_chan;              // center frequency channel
	byte    cdma_code_chan;         // code for current channel - cdma only
	word    pilot_base;             // pilot pn of current cell - cdma only
	word    sid;                    // Current System ID
	word    nid;                    // Current Network ID
	word    locaid;                 // Current Location ID
	word    rssi;                   // Current RSSI level
	byte    power;                  // Current mobile output power level
BQTT_DIAGPKT_RSP_END

//---------------------------------------------------------------------------
//
//	DIAG_MSG_F (legacy)
//
//---------------------------------------------------------------------------
#define MSG_FILE_LEN 13				// Allows full DOS file name + NULL
#define MSG_FMT_LEN  40				// Max len of format string + NULL
typedef enum
{
	DIAG_MSG_LEVEL_LOW = 0,		// All message
	DIAG_MSG_LEVEL_MED,			// Medium and above
	DIAG_MSG_LEVEL_HIG,			// High and above
	DIAG_MSG_LEVEL_ERR,			// Error and above
	DIAG_MSG_LEVEL_FAT,			// Fatal and above
	DIAG_MSG_LEVEL_NON = 0x00ff	// No message
};
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_MSG_F)
	word msg_level;					// highest type desired */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_MSG_F)
	word qty;						// number of msg packets in the msg buffer
									// including the msg packet returned in this
									// response  -- if this field is 0, no msg
									// packet is included in this response
	dword drop_cnt;					// number of msgs dropped by error services
	dword total_msgs;				// total number of msgs that have occured
	byte level;						// Severity level / Priority of this message
	char file[MSG_FILE_LEN];		// Holds source file name
	word line;						// Line number in source file
	char fmt[MSG_FMT_LEN];			// printf style format string
	dword code1;					// parameters to go with the format string
	dword code2;
	dword code3;
	//qword time;						// Time at which message was generated
	unsigned __int64 time;
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_NV_READ_F
//
//---------------------------------------------------------------------------
typedef enum {
	NV_DONE_S,          // Request completed okay
	NV_BUSY_S,          // Request is queued
	NV_BADCMD_S,        // Unrecognizable command field
	NV_FULL_S,          // The NVM is full
	NV_FAIL_S,          // Command failed, reason other than NVM was full
	NV_NOTACTIVE_S,     // Variable was not active
	NV_BADPARM_S,       // Bad parameter in command block
	NV_READONLY_S,      // Parameter is write-protected and thus read only
	NV_BADTG_S,         // Item not valid for Target
	NV_NOMEM_S,         // free memory exhausted
	NV_NOTALLOC_S,      // address is not a valid allocation
	NV_STAT_ENUM_PAD = 0x7FFF     // Pad to 16 bits on ARM
} nv_stat_enum_type;
#define DIAG_NV_ITEM_SIZE 128
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_NV_READ_F)
	uint16 item;                         // Which item - use nv_items_enum_type
	uint8  item_data[DIAG_NV_ITEM_SIZE]; // Item itself - use nv_item_type
	uint16 nv_stat;                      // Status of operation - use nv_stat_enum_type
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_NV_READ_F)
//---------------------------------------------------------------------------
//
//	DIAG_NV_WRITE_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_NV_WRITE_F)
	uint16 item;                         // Which item - use nv_items_enum_type
	uint8  item_data[DIAG_NV_ITEM_SIZE]; // Item itself - use nv_item_type
	uint16 nv_stat;                      // Status of operation - use nv_stat_enum_type
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_NV_WRITE_F)
//---------------------------------------------------------------------------
//
//	DIAG_CONTROL_F
//
//---------------------------------------------------------------------------
// Mode change commands
typedef enum
{
	DIAG_MODE_OFFLINE_A_F = 0,	// Go to offline analog
	DIAG_MODE_OFFLINE_D_F,		// Go to offline digital
	DIAG_MODE_RESET_F,			// Reset. Only exit from offline
	DIAG_MODE_FTM_F,				// FTM mode - if supported
	DIAG_MODE_ONLINE_F,			// Online mode - if supported
	DIAG_MODE_LPM_F,				// Low Power Mode - if supported
	DIAG_MODE_MAX_F				// Last (and invalid) mode enum value
};
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_CONTROL_F)
	word mode;		// Mode to change to
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_CONTROL_F)
//---------------------------------------------------------------------------
//
//	DIAG_LOG_CONFIG_F
//
//---------------------------------------------------------------------------
/*
typedef enum {
	LOG_CONFIG_DISABLE_OP = 0,
	LOG_CONFIG_RETRIEVE_ID_RANGES_OP = 1, 
	LOG_CONFIG_RETRIEVE_VALID_MASK_OP = 2,
	LOG_CONFIG_SET_MASK_OP = 3,
	LOG_CONFIG_GET_LOGMASK_OP = 4
};
typedef enum {
	LOG_CONFIG_SUCCESS_S = 0,          // Operation Sucessful
	LOG_CONFIG_INVALID_EQUIP_ID_S = 1, // (*) Specified invalid equipment ID
	LOG_CONFIG_NO_VALID_MASK_S = 2     // Valid mask not available for this ID
};
typedef struct {
	uint32 equip_id;
	uint32 last_item;
} log_config_range_type;
typedef struct {
	log_config_range_type code_range;	// range of log codes
	byte mask[1];						// Array of 8 bit masks of size (num_bits + 7) / 8
} log_config_mask_type;
typedef struct {
	uint32 last_item[16];				// The last item for each of the 16 equip IDs
} log_config_ranges_rsp_type;
typedef PACKED struct {
	uint32 equip_id;
} log_config_valid_mask_req_type;
typedef log_config_mask_type log_config_valid_mask_rsp_type;
typedef log_config_mask_type log_config_set_mask_req_type;
typedef log_config_mask_type log_config_get_mask_rsp_type;
typedef log_config_mask_type log_config_set_mask_rsp_type;
typedef union {
  log_config_valid_mask_req_type valid_mask;
  log_config_set_mask_req_type set_mask;
} log_config_op_req_type;
typedef union {
  log_config_ranges_rsp_type ranges;
  log_config_valid_mask_rsp_type valid_mask;
  log_config_set_mask_rsp_type set_mask;
  log_config_get_mask_rsp_type get_mask;
} log_config_op_rsp_type;
BQTT_DIAGPKT_REQ_DEFINE(DIAG_LOG_CONFIG_F)
	byte pad[3];			//Force following items to be on 32-bit boundary
	uint32 operation;		// See log_config_command_ops_enum_type
	uint32 op_data[1];		// Pointer to operation data
BQTT_DIAGPKT_REQ_END
BQTT_DIAGPKT_RSP_DEFINE(DIAG_LOG_CONFIG_F)
	byte pad[3];			// Force following items to be on 32-bit boundary
	uint32 operation;		// See log_config_command_ops_enum_type
	uint32 status;
	uint32 op_data[1];		// Pointer to operation data
BQTT_DIAGPKT_RSP_END
//#define DIAG_EXT_LOGMASK_NUM_BITS (LOG_1X_LAST_C & 0x0FFF)
//#define DIAG_EXT_LOGMASK_NUM_BYTES ((DIAG_EXT_LOGMASK_NUM_BITS / 8) + 1)
*/
//---------------------------------------------------------------------------
//
//	DIAG_FEATURE_QUERY_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_FEATURE_QUERY_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_FEATURE_QUERY_F)
	word feature_mask_size;                 // Size of the following Mask
	byte feature_mask[FEATURE_MASK_LENGTH]; // Space for the largest possible feature mask
BQTT_DIAGPKT_RSP_END
// BenQ Alice HY Su 2004/11/16 {
//---------------------------------------------------------------------------
//
//	DIAG_TS_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_TS_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_TS_F)
	qword ts;                    /* Time stamp */
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_HS_LOCK_F
//
//---------------------------------------------------------------------------
typedef enum {
  HS_LOCK_F,                       /* command to lock real handset */
  HS_REL_F                         /* command to release real handset */
};
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_HS_LOCK_F)
	word        lock;        /* diag locks or unlocks real hs
							 Use diag_hs_lock_type.          */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_HS_LOCK_F)
//---------------------------------------------------------------------------
//
//	DIAG_ORIG_F
//
//---------------------------------------------------------------------------
#define CMDIAG_NV_MAX_DIAL_DIGITS 32
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_ORIG_F)
	byte cnt;			/* number of digits in dialed_digits array */
	char dialed_digits[CMDIAG_NV_MAX_DIAL_DIGITS];
	word so;			/* desired service option - 0 = Reserved,  1 = Voice (IS96A), 2 = Loopback,
							8001 = Voice (IS96), 8002 = Markov, 8003 = Data (see cai.h) */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_ORIG_F)
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_END_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_END_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_END_F)
//---------------------------------------------------------------------------
//
//	DIAG_SER_RESET_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_SER_RESET_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_SER_RESET_F)
//---------------------------------------------------------------------------
//
//	DIAG_SER_REPORT_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_SER_REPORT_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_SER_REPORT_F)
  dword frame_cnt;  /* Total frame count */
  dword ser1;   /* Sum of all 192 bit frame symbol error counts */
  dword ser2;   /* Sum of all 96 bit frame symbol error counts */
  dword ser4;   /* Sum of all 48 bit frame symbol error counts */
  dword ser8;   /* Sum of all 24 bit frame symbol error counts */
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_DIPSW_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_DIPSW_F)
  word switches;
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_DIPSW_F)
//---------------------------------------------------------------------------
//
//	DIAG_ERR_READ_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_ERR_READ_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_ERR_READ_F)
  word              log_cnt;            /* how many logged */
  word              ignore_cnt;         /* how many ignored */
 
  PACKED struct { 
    byte      address;      /* Storage address 0 to       */
                            /* ERR_MAX_LOG-1              */
    byte      err_count;    /* Number of occurances       */
                            /* (0=empty,FF=full)          */
    byte      file_name[8]; 
                            /* File name string */
    word      line_num;     /* Line number in file */
    boolean   fatal;        /* TRUE if fatal error */
  } err_logs[ 20 ];
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_ERR_CLEAR_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_ERR_CLEAR_F)
  byte rec; /* record id, or -1 (0xFF) for all */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_ERR_CLEAR_F)
//---------------------------------------------------------------------------
//
//	DIAG_VOC_PCM_LB_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_VOC_PCM_LB_F)
  boolean pcm_lb;        /* TRUE -> start, FALSE -> stop */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_VOC_PCM_LB_F)
//---------------------------------------------------------------------------
//
//	DIAG_VOC_PKT_LB_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_VOC_PKT_LB_F)
  boolean pkt_lb;        /* TRUE -> start, FALSE -> stop */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_VOC_PKT_LB_F)
//---------------------------------------------------------------------------
//
//	DIAG_PARM_GET2_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_PARM_GET2_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_PARM_GET2_F)
  uint64    time;                    /* Time stamp */
  dword     mux_rev[14];             /* Reverse Traffic Channel params */
  dword     mux_for[ 4];             /* Forward Traffic Channel params */
  dword     pag[7];                  /* Paging Channel params */
  dword     acc[8];                  /* Access Channel params */
  dword     layer2[5];               /* Layer2 params */
  uint64    sys_time;                /* OTH_SYS_TIME parameter */
  dword     mux2_rev[26];            /* MUX2 Reverse Traffic Channel params */
  dword     mux2_for[26];            /* MUX2 Forward Traffic Channel params */
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_SPC_F
//
//---------------------------------------------------------------------------
#define  DIAGPKT_SEC_CODE_SIZE 6               /* 6 digit security code */
typedef PACKED struct {
  byte      digits[DIAGPKT_SEC_CODE_SIZE];     /* Security code array */
} diagpkt_sec_code_type;
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_SPC_F)
  diagpkt_sec_code_type sec_code;
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_SPC_F)
  boolean sec_code_ok;
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_LOGMASK_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_LOGMASK_F)
  uint32 mask; /* 32-bit log mask  */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_LOGMASK_F)
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_STATE_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_STATE_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_STATE_F)
  byte phone_state;           /* Current phone state */
  word event_count;           /* Count of possible state-changing events */
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_PILOT_SETS_F
//
//---------------------------------------------------------------------------
typedef PACKED struct
{
    word pilot_pn;            /* Pilot offset   */
    word pilot_strength;      /* Pilot strength */
} diag_pilot_data_type;
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_PILOT_SETS_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_PILOT_SETS_F)
  word pilot_inc;             /* Pilot increment for remaining sets */
  byte active_cnt;            /* Count of pilots in active set (up to 6) */
  byte cand_cnt;              /* Count of pilots in candidate set (up to 6) */
  byte neigh_cnt;             /* Count of pilots in neighbor set (up to 40) */
  diag_pilot_data_type pilot_sets[52];
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_PASSWORD_F
//
//---------------------------------------------------------------------------
#define DIAG_PASSWORD_SIZE 8
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_PASSWORD_F)
  byte password[DIAG_PASSWORD_SIZE];  /* The security password */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_PASSWORD_F)
  boolean password_ok;        /* TRUE if Security Password entered correctly */
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_TEST_F
//
//---------------------------------------------------------------------------
/* Parameters for the set_attn directed test */
typedef PACKED struct {
  byte  attn;
} set_attn_parm_type;
/* Parameters for the switch_nam directed test */
typedef PACKED struct {
  byte  nam;
} nam_type;
/* Parameters for the send_min directed test */
typedef PACKED struct {
  byte  nam;
  byte  min;
} send_min_parm_type;
/* Parameters for the sat directed test */
typedef PACKED struct {
  byte  sat;
} sat_parm_type;
/* Parameters for the CDATA directed test */
typedef PACKED struct {
  byte  cdata[6];
} cdata_parm_type;
/* Parameters for the dtmf directed test */
typedef PACKED struct {
  byte  dtmf;
} dtmf_parm_type;
/* Union of the directed test parameter types */
typedef PACKED union {
  word                    chan;       /* Channel for load-synth cmd  */
  set_attn_parm_type      sa;         /* Attn level for set-attn cmd */
  nam_type                nam;
  send_min_parm_type      sm;
  sat_parm_type           sat;
  cdata_parm_type         cdata;
  dtmf_parm_type          dtmf;
} diag_test_parm_type;
/* Response packet definitions */

/* RCVS1 response (requested with a terminate command) */

typedef PACKED struct {
  byte  num_uncorr;
  byte  num_corr;
} rcvs1_type;

/* WSTS response (requested with a terminate command ) */

typedef PACKED struct {
  word  num_syncs;
} wsts_type;

/* RSSI response */

/* Union of directed test response types */

typedef PACKED union {
  rcvs1_type        rcvs1;              /* Response from RCVS1 command */
  wsts_type         wsts;               /* Response from WSTS command */
  byte              rssi;               /* Response from RSSI command */
} diag_test_results_type;


typedef enum
{
  DT_CARRIER_ON_F,            /* Carrier on command                       */
  DT_CARRIER_OFF_F,           /* Carrier off command                      */
  DT_LOAD_SYNTH_F,            /* Load Synthesizer command                 */
  DT_SET_ATTN_F,              /* Set Attenuation command                  */
  DT_RXMUTE_F,                /* Mute the receiver audio signal           */
  DT_RXUNMUTE_F,              /* Unmute the receier audio signal          */
  DT_TXMUTE_F,                /* Mute the transmit audio signal           */
  DT_TXUNMUTE_F,              /* Unmute the transmit audio signal         */
  DT_ST_ON_F,                 /* Transmit continuous Signalling Tone      */
  DT_ST_OFF_F,                /* Stop transmission of Signalling Tone     */
  DT_SEND_NAM_F,              /* Send the specified NAM                   */
  DT_SWITCH_NAM_F,            /* Change to the specified NAM              */
  DT_SEND_MIN_F,              /* Send the specified MIN                   */
  DT_VERSION_F,               /* Send software version                    */
  DT_SEND_ESN_F,              /* Send the esn                             */
  DT_RCVS1_F,                 /* Count msgs on Analog Control Channel     */
  DT_WSTS_F,                  /* Count word syncs on Analog Control
                                 Channel                                  */
  DT_SAT_ON_F,                /* Enable transmission of SAT               */
  DT_SAT_OFF_F,               /* Disable transmission of SAT              */
  DT_TERMINATE_F,             /* Stop RCVS1 or WSTS and report counts     */
  DT_CDATA_F,                 /* Transmit continuous 5-word Reverse
                                 Control Channel messages                 */
  DT_DTMF_ON_F,               /* Activate the DTMF generator with
                                 specified tones                          */
  DT_DTMF_OFF_F,              /* Stop the DTMF                            */
  DT_COMPANDER_ON_F,          /* Enable the compander                     */
  DT_COMPANDER_OFF_F,         /* Disable the compander                    */
  DT_RSSI_F,                  /* Return the current RSSI measurement      */
  DT_MAX_F                    /* Last and invalid directed test choice    */
} diag_test_enum_type;
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_TEST_F)
  word                  test_code; /* Use diag_test_enum_type */
  diag_test_parm_type   parm;
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_TEST_F)
  word                      test_code; /* Use diag_test_enum_type */
  diag_test_results_type    results;
BQTT_DIAGPKT_RSP_END
//---------------------------------------------------------------------------
//
//	DIAG_HS_KEY_F
//
//---------------------------------------------------------------------------
// Keypad declarations
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
  /* BenQ added end */
  HS_RELEASE_K = 0xff   /* Key to note that all keys are up        */
} hs_key_type;
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_HS_KEY_F)
	boolean hold;                     /* If true, diag witholds key release */
	byte    key;                      /* enumerated key, e.g. HS_DOWN_K */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_HS_KEY_F)
//---------------------------------------------------------------------------
//
//	DIAG_PARM_SET_F
//
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_PARM_SET_F)
  word      parm_id;                /* Parameter value, from
                                       cai.h.  A parm_id of 0 will
                                       cause ALL parmameters to
                                       be cleared.                */
  dword     parm_value;             /* New value for parameter    */
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_PARM_SET_F)
  uint64    time;                    /* Time stamp */
BQTT_DIAGPKT_RSP_END
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/*          SETTABLE AND RETRIEVABLE PARAMETERS                            */
/*-   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -   -*/
/* The following parameters are from IS-95A Table E-1.                     */

#define CAI_MUX_WILDCARD 0
 /* Mux Option to indicate a wildcard MUX option that
    is internal to Call Processing.  This is used to
    indicate that SNM should allow any MUX option for
    this service option.  */

#define CAI_MUX1_REV_1_ID 1
  /* Number of Reverse Traffic Channel 9600 bps frames with primary traffic
     only */
#define CAI_MUX1_REV_1_LEN 24
  /* Length of CAI_MUX1_REV_1 parameter */

#define CAI_MUX1_REV_2_ID 2
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX1_REV_2_LEN 24
  /* Length of CAI_MUX1_REV_2 parameter */

#define CAI_MUX1_REV_3_ID 3
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX1_REV_3_LEN 24
  /* Length of CAI_MUX1_REV_3 parameter */

#define CAI_MUX1_REV_4_ID 4
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX1_REV_4_LEN 24
  /* Length of CAI_MUX1_REV_4 parameter */

#define CAI_MUX1_REV_5_ID 5
  /* Number of Reverse Traffic Channel 9600 bps frame, blank-and-burst with
     signaling traffic only */
#define CAI_MUX1_REV_5_LEN 24
  /* Length of CAI_MUX1_REV_5 parameter */

#define CAI_MUX1_REV_6_ID 6
  /* Number of Reverse Traffic Channel 4800 bps frames with primary traffic
     only */
#define CAI_MUX1_REV_6_LEN 24
  /* Length of CAI_MUX1_REV_6 parameter */

#define CAI_MUX1_REV_7_ID 7
  /* Number of Reverse Traffic Channel 2400 bps frames with primary traffic
     only */
#define CAI_MUX1_REV_7_LEN 24
  /* Length of CAI_MUX1_REV_7 parameter */

#define CAI_MUX1_REV_8_ID 8
  /* Number of Reverse Traffic Channel 1200 bps frmaes primary traffic or
     null Traffic Channel data only */
#define CAI_MUX1_REV_8_LEN 24
  /* Length of CAI_MUX1_REV_8 parameter */

#define CAI_MUX1_REV_9_ID 9
  /* Reserved */
#define CAI_MUX1_REV_10_ID 10
  /* Reserved */

#define CAI_MUX1_REV_11_ID 11
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX1_REV_11_LEN 24
  /* Length of CAI_MUX1_REV_11 parameter */

#define CAI_MUX1_REV_12_ID 12
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX1_REV_12_LEN 24
  /* Length of CAI_MUX1_REV_12 parameter */

#define CAI_MUX1_REV_13_ID 13
  /* Number of Reverse Traffic Channel 9600 bps frames, dim-and_burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX1_REV_13_LEN 24
  /* Length of CAI_MUX1_REV_13 parameter */

#define CAI_MUX1_REV_14_ID 14
  /* Number of Reverse Traffic Channel 9600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX1_REV_14_LEN 24
  /* Length of CAI_MUX1_REV_14 parameter */

#define CAI_MUX1_FOR_1_ID 15
  /* Number of Forward Traffic Channel 9600 bps primary traffic only frames */
#define CAI_MUX1_FOR_1_LEN 24
  /* Length of CAI_MUX1_FOR_1 parameter */

#define CAI_MUX1_FOR_2_ID 16
  /* Number of Forward Traffic Channel 9600 bps frames dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX1_FOR_2_LEN 24
  /* Length of CAI_MUX1_FOR_2 parameter */

#define CAI_MUX1_FOR_3_ID 17
  /* Number of Forward Traffic Channel 9600 bps frames dim-and-burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX1_FOR_3_LEN 24
  /* Length of CAI_MUX1_FOR_3 parameter */

#define CAI_MUX1_FOR_4_ID 18
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX1_FOR_4_LEN 24
  /* Length of CAI_MUX1_FOR_4 parameter */

#define CAI_MUX1_FOR_5_ID 19
  /* Number of Forward Traffic Channel 9600 bps frames, blank-and-burst with
     signaling traffic only */
#define CAI_MUX1_FOR_5_LEN 24
  /* Length of CAI_MUX1_FOR_5 parameter */

#define CAI_MUX1_FOR_6_ID 20
  /* Number of Forward Traffic Channel 4800 bps frames, primary traffic only */
#define CAI_MUX1_FOR_6_LEN 24
  /* Length of CAI_MUX1_FOR_6 parameter */

#define CAI_MUX1_FOR_7_ID 21
  /* Number of Forward Traffic Channel 2400 bps frames, primary traffic only */
#define CAI_MUX1_FOR_7_LEN 24
  /* Length of CAI_MUX1_FOR_7 parameter */

#define CAI_MUX1_FOR_8_ID 22
  /* Number of Forward Traffic Channel 1200 bps frames, primary traffic or
     null Traffic Channel data only */
#define CAI_MUX1_FOR_8_LEN 24
  /* Length of CAI_MUX1_FOR_8 parameter */

#define CAI_MUX1_FOR_9_ID 23
  /* Number of Forward Traffic Channel 9600 bps frames primary traffic only,
     probable bit errors */
#define CAI_MUX1_FOR_9_LEN 24
  /* Length of CAI_MUX1_FOR_9 parameter */

#define CAI_MUX1_FOR_10_ID 24
  /* Number of Forward Traffic Channel frames with frame quality insufficient
     to decide upon rate */
#define CAI_MUX1_FOR_10_LEN 24
  /* Length of CAI_MUX1_FOR_10 parameter */


#define CAI_MUX1_FOR_11_ID 25
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX1_FOR_11_LEN 24
  /* Length of CAI_MUX1_FOR_11 parameter */

#define CAI_MUX1_FOR_12_ID 26
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX1_FOR_12_LEN 24
  /* Length of CAI_MUX1_FOR_12 parameter */

#define CAI_MUX1_FOR_13_ID 27
  /* Number of Forward Traffic Channel 9600 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX1_FOR_13_LEN 24
  /* Length of CAI_MUX1_FOR_13 parameter */

#define CAI_MUX1_FOR_14_ID 28
  /* Number of Forward Traffic Channel 9600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX1_FOR_14_LEN 24
  /* Length of CAI_MUX1_FOR_14 parameter */

#define CAI_PAG_1_ID 29
  /* Number of Paging Channel messages attempted to be received */
#define CAI_PAG_1_LEN 24
  /* Length of CAI_PAG_1_ID parameter */

#define CAI_PAG_2_ID 30
  /* Number of Paging Channel msgs received with a CRC that does not check */
#define CAI_PAG_2_LEN 24
  /* Length of CAI_PAG_2_ID parameter */

#define CAI_PAG_3_ID 31
  /* Number of Paging Channel messages received, addressed to mobile */
#define CAI_PAG_3_LEN 16
  /* Length of CAI_PAG_3 parameter */

#define CAI_PAG_4_ID 32
  /* Number of Paging Channel half frames received by the mobile */
#define CAI_PAG_4_LEN 24
  /* Length of CAI_PAG_4 parameter */

#define CAI_PAG_5_ID 33
  /* Number of Paging Channel half frames that contain any part of a message
     with a CRC that checks */
#define CAI_PAG_5_LEN 24
  /* Length of CAI_PAG_5 parameter */

#define CAI_PAG_6_ID 34
  /* Number of times that the mobile station declared a loss of the Paging
     Channel */
#define CAI_PAG_6_LEN 16
  /* Length of CAI_PAG_6 parameter */

#define CAI_PAG_7_ID 35
  /* Number of mobile station idle handoffs */
#define CAI_PAG_7_LEN 16
  /* Length of CAI_PAG_7 parameter */

#define CAI_ACC_1_ID 36
  /* Number of Access Channel request messages generated by layer 3 */
#define CAI_ACC_1_LEN 16
  /* Length of CAI_ACC_1 parameter */

#define CAI_ACC_2_ID 37
  /* Number of Access Channel response messages generated by layer 3 */
#define CAI_ACC_2_LEN 16
  /* Length of CAI_ACC_2 parameter */

#define CAI_ACC_3_ID 38
  /* Number of times access probe transmitted at least twice */
#define CAI_ACC_3_LEN 16
  /* Length of CAI_ACC_3 parameter */

#define CAI_ACC_4_ID 39
  /* Number of times access probe transmitted at least 3 times */
#define CAI_ACC_4_LEN 16
  /* Length of CAI_ACC_4 parameter */

#define CAI_ACC_5_ID 40
  /* Number of times access probe transmitted at least 4 times */
#define CAI_ACC_5_LEN 16
  /* Length of CAI_ACC_5 parameter */

#define CAI_ACC_6_ID 41
  /* Number of times access probe transmitted at least 5 times */
#define CAI_ACC_6_LEN 16
  /* Length of CAI_ACC_6 parameter */

#define CAI_ACC_7_ID 42
  /* Number of times access probe transmitted at least 6 times */
#define CAI_ACC_7_LEN 16
  /* Length of CAI_ACC_7 parameter */

#define CAI_ACC_8_ID 43
  /* Number of unsuccessful access attempts */
#define CAI_ACC_8_LEN 16
  /* Length of CAI_ACC_8 parameter */

#define CAI_LAYER2_RTC1_ID 44
  /* Number of messages requring acknowledgement which were transmitted at
     least once on the Reverse Traffic Channel */
#define CAI_LAYER2_RTC1_LEN 16
  /* Length of CAI_LAYER2_RTC1 parameter */

#define CAI_LAYER2_RTC2_ID 45
  /* Number of messages requring acknowledgement which were transmitted at
     least twice on the Reverse Traffic Channel */
#define CAI_LAYER2_RTC2_LEN 16
  /* Length of CAI_LAYER2_RTC2 parameter */

#define CAI_LAYER2_RTC3_ID 46
  /* Number of messages requring acknowledgement which were transmitted at
     least 3 times on the Reverse Traffic Channel */
#define CAI_LAYER2_RTC3_LEN 16
  /* Length of CAI_LAYER2_RTC3 parameter */

#define CAI_LAYER2_RTC4_ID 47
  /* Number of times mobile aborted call because of timeout expiring after
     the N1m transmission of a message requring an acknowledgement.  */
#define CAI_LAYER2_RTC4_LEN 16
  /* Length of CAI_LAYER2_RTC4 parameter */

#define CAI_LAYER2_RTC5_ID 48
  /* Number of times a message not requiring an acknowledgement was sent on
     the Reverse Traffic Channel */
#define CAI_LAYER2_RTC5_LEN 16
  /* Length of CAI_LAYER2_RTC5 parameter */

#define CAI_OTH_SYS_TIME_ID 49
  /* The SYS_TIME field from the most recently received Sync Channel Message */
#define CAI_OTH_SYS_TIME_LEN 36
  /* Length of CAI_OTH_SYS_TIME parameter */

#define CAI_MUX2_REV_1_ID 50
  /* Number of Reverse Traffic Channel 14400 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_1_LEN 24
  /* Length of CAI_MUX2_REV_1 parameter */

#define CAI_MUX2_REV_2_ID 51
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX2_REV_2_LEN 24
  /* Length of CAI_MUX2_REV_2 parameter */

#define CAI_MUX2_REV_3_ID 52
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and_burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX2_REV_3_LEN 24
  /* Length of CAI_MUX2_REV_3 parameter */

#define CAI_MUX2_REV_4_ID 53
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and_burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX2_REV_4_LEN 24
  /* Length of CAI_MUX2_REV_4 parameter */

#define CAI_MUX2_REV_5_ID 54
  /* Number of Reverse Traffic Channel 14400 bps frame, blank-and-burst with
     signaling traffic only */
#define CAI_MUX2_REV_5_LEN 24
  /* Length of CAI_MUX2_REV_5 parameter */

#define CAI_MUX2_REV_6_ID 55
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX2_REV_6_LEN 24
  /* Length of CAI_MUX2_REV_6 parameter */

#define CAI_MUX2_REV_7_ID 56
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_REV_7_LEN 24
  /* Length of CAI_MUX2_REV_7 parameter */

#define CAI_MUX2_REV_8_ID 57
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_REV_8_LEN 24
  /* Length of CAI_MUX2_REV_8 parameter */

#define CAI_MUX2_REV_9_ID 58
  /* Number of Reverse Traffic Channel 14400 bps frame, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_9_LEN 24
  /* Length of CAI_MUX2_REV_9 parameter */

#define CAI_MUX2_REV_10_ID 59
  /* Number of Reverse Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_REV_10_LEN 24
  /* Length of CAI_MUX2_REV_10 parameter */

#define CAI_MUX2_REV_11_ID 60
  /* Number of Reverse Traffic Channel 7200 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_11_LEN 24
  /* Length of CAI_MUX2_REV_11 parameter */

#define CAI_MUX2_REV_12_ID 61
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/4 primary and signalling traffic */
#define CAI_MUX2_REV_12_LEN 24
  /* Length of CAI_MUX2_REV_12 parameter */

#define CAI_MUX2_REV_13_ID 62
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_REV_13_LEN 24
  /* Length of CAI_MUX2_REV_13 parameter */

#define CAI_MUX2_REV_14_ID 63
  /* Number of Reverse Traffic Channel 7200 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_REV_14_LEN 24
  /* Length of CAI_MUX2_REV_14 parameter */

#define CAI_MUX2_REV_15_ID 64
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_REV_15_LEN 24
  /* Length of CAI_MUX2_REV_15 parameter */

#define CAI_MUX2_REV_16_ID 65
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_REV_16_LEN 24
  /* Length of CAI_MUX2_REV_16 parameter */

#define CAI_MUX2_REV_17_ID 66
  /* Number of Reverse Traffic Channel 7200 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_17_LEN 24
  /* Length of CAI_MUX2_REV_17 parameter */

#define CAI_MUX2_REV_18_ID 67
  /* Number of Reverse Traffic Channel 7200 bps frames, dim-and_burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_REV_18_LEN 24
  /* Length of CAI_MUX2_REV_18 parameter */

#define CAI_MUX2_REV_19_ID 68
  /* Number of Reverse Traffic Channel 3600 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_19_LEN 24
  /* Length of CAI_MUX2_REV_19 parameter */

#define CAI_MUX2_REV_20_ID 69
  /* Number of Reverse Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_REV_20_LEN 24
  /* Length of CAI_MUX2_REV_20 parameter */

#define CAI_MUX2_REV_21_ID 70
  /* Number of Reverse Traffic Channel 14400 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_REV_21_LEN 24
  /* Length of CAI_MUX2_REV_21 parameter */

#define CAI_MUX2_REV_22_ID 71
  /* Number of Reverse Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_REV_22_LEN 24
  /* Length of CAI_MUX2_REV_22 parameter */

#define CAI_MUX2_REV_23_ID 72
  /* Number of Reverse Traffic Channel 3600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_23_LEN 24
  /* Length of CAI_MUX2_REV_23 parameter */

#define CAI_MUX2_REV_24_ID 73
  /* Number of Reverse Traffic Channel 1800 bps frames with primary traffic
     or Null Traffic only */
#define CAI_MUX2_REV_24_LEN 24
  /* Length of CAI_MUX2_REV_24 parameter */

#define CAI_MUX2_REV_25_ID 74
  /* Number of Reverse Traffic Channel 1800 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_REV_25_LEN 24
  /* Length of CAI_MUX2_REV_25 parameter */

#define CAI_MUX2_REV_26_ID 75
  /* Reserved */


#define CAI_MUX2_FOR_1_ID  76
  /* Number of Forward Traffic Channel 14400 bps primary traffic only frames */
#define CAI_MUX2_FOR_1_LEN 24
  /* Length of CAI_MUX2_FOR_1 parameter */

#define CAI_MUX2_FOR_2_ID 77
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/2 primary and signaling traffic */
#define CAI_MUX2_FOR_2_LEN 24
  /* Length of CAI_MUX2_FOR_2 parameter */

#define CAI_MUX2_FOR_3_ID 78
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/4 primary and signaling traffic */
#define CAI_MUX2_FOR_3_LEN 24
  /* Length of CAI_MUX2_FOR_3 parameter */

#define CAI_MUX2_FOR_4_ID 79
  /* Number of Forward Traffic Channel 14400 bps frames, dim-and-burst with
     rate 1/8 primary and signaling traffic */
#define CAI_MUX2_FOR_4_LEN 24
  /* Length of CAI_MUX2_FOR_4 parameter */

#define CAI_MUX2_FOR_5_ID 80
  /* Number of Forward Traffic Channel 14400 bps frames, blank-and-burst with
     signaling traffic only */
#define CAI_MUX2_FOR_5_LEN 24
  /* Length of CAI_MUX2_FOR_5 parameter */

#define CAI_MUX2_FOR_6_ID 81
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/2 primary and secondary traffic */
#define CAI_MUX2_FOR_6_LEN 24
  /* Length of CAI_MUX2_FOR_6 parameter */

#define CAI_MUX2_FOR_7_ID 82
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_FOR_7_LEN 24
  /* Length of CAI_MUX2_FOR_7 parameter */

#define CAI_MUX2_FOR_8_ID 83
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_FOR_8_LEN 24
  /* Length of CAI_MUX2_FOR_8 parameter */

#define CAI_MUX2_FOR_9_ID 84
  /* Number of Forward Traffic Channel 14400 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_9_LEN 24
  /* Length of CAI_MUX2_FOR_9 parameter */

#define CAI_MUX2_FOR_10_ID 85
  /* Number of Forward Traffic Channel 14400 bps frames dim-and-burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_FOR_10_LEN 24
  /* Length of CAI_MUX2_FOR_10 parameter */

#define CAI_MUX2_FOR_11_ID 86
  /* Number of Forward Traffic Channel 7200 bps frames, primary traffic only */
#define CAI_MUX2_FOR_11_LEN 24
  /* Length of CAI_MUX2_FOR_11 parameter */

#define CAI_MUX2_FOR_12_ID 87
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/4 primary and signalling traffic */
#define CAI_MUX2_FOR_12_LEN 24
  /* Length of CAI_MUX2_FOR_12 parameter */

#define CAI_MUX2_FOR_13_ID 88
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_FOR_13_LEN 24
  /* Length of CAI_MUX2_FOR_13 parameter */

#define CAI_MUX2_FOR_14_ID 89
  /* Number of Forward Traffic Channel 7200 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_FOR_14_LEN 24
  /* Length of CAI_MUX2_FOR_14 parameter */

#define CAI_MUX2_FOR_15_ID 90
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/4 primary and secondary traffic */
#define CAI_MUX2_FOR_15_LEN 24
  /* Length of CAI_MUX2_FOR_15 parameter */

#define CAI_MUX2_FOR_16_ID 91
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_FOR_16_LEN 24
  /* Length of CAI_MUX2_FOR_16 parameter */

#define CAI_MUX2_FOR_17_ID 92
  /* Number of Forward Traffic Channel 7200 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_17_LEN 24
  /* Length of CAI_MUX2_FOR_17 parameter */

#define CAI_MUX2_FOR_18_ID 93
  /* Number of Forward Traffic Channel 7200 bps frames, dim-and-burst with
     rate 1/8 primary, secondary and signalling traffic */
#define CAI_MUX2_FOR_18_LEN 24
  /* Length of CAI_MUX2_FOR_18 parameter */

#define CAI_MUX2_FOR_19_ID 94
  /* Number of Forward Traffic Channel 3600 bps frames, primary traffic only */
#define CAI_MUX2_FOR_19_LEN 24
  /* Length of CAI_MUX2_FOR_19 parameter */

#define CAI_MUX2_FOR_20_ID 95
  /* Number of Forward Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and signalling traffic */
#define CAI_MUX2_FOR_20_LEN 24
  /* Length of CAI_MUX2_FOR_20 parameter */

#define CAI_MUX2_FOR_21_ID 96
  /* Number of Forward Traffic Channel 3600 bps frames, blank-and-burst with
     signalling traffic only */
#define CAI_MUX2_FOR_21_LEN 24
  /* Length of CAI_MUX2_FOR_21 parameter */

#define CAI_MUX2_FOR_22_ID 97
  /* Number of Forward Traffic Channel 3600 bps frames, dim-and-burst with
     rate 1/8 primary and secondary traffic */
#define CAI_MUX2_FOR_22_LEN 24
  /* Length of CAI_MUX2_FOR_22 parameter */

#define CAI_MUX2_FOR_23_ID 98
  /* Number of Forward Traffic Channel 3600 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_23_LEN 24
  /* Length of CAI_MUX2_FOR_23 parameter */

#define CAI_MUX2_FOR_24_ID 99
  /* Number of Forward Traffic Channel 1800 bps frames, primary traffic or
     null Traffic Channel data only */
#define CAI_MUX2_FOR_24_LEN 24
  /* Length of CAI_MUX2_FOR_24 parameter */

#define CAI_MUX2_FOR_25_ID 100
  /* Number of Forward Traffic Channel 1800 bps frames, blank-and-burst with
     secondary traffic only */
#define CAI_MUX2_FOR_25_LEN 24
  /* Length of CAI_MUX2_FOR_25 parameter */

#define CAI_MUX2_FOR_26_ID 101
  /* Number of Forward Traffic Channel frames with frame quality insufficient
     to decide upon rate */
#define CAI_MUX2_FOR_26_LEN 24
  /* Length of CAI_MUX2_FOR_26 parameter */

#if defined(FEATURE_IS95B_MDR) || defined(FEATURE_SPECIAL_MDR) || defined (FEATURE_DS_RLP3)
#define CAI_SUPP1_REV_S_ID 102
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP1_REV_S_LEN 24
  /* Length of CAI_SUPP1_REV_S parameter */
#define CAI_SUPP1_REV_P_ID 103
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP1_REV_P_LEN 24
  /* Length of CAI_SUPP1_REV_P parameter */
#define CAI_SUPP2_REV_S_ID 104
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP2_REV_S_LEN 24
  /* Length of CAI_SUPP2_REV_S parameter */
#define CAI_SUPP2_REV_P_ID 105
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP2_REV_P_LEN 24
  /* Length of CAI_SUPP2_REV_P parameter */
#define CAI_SUPP3_REV_S_ID 106
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP3_REV_S_LEN 24
  /* Length of CAI_SUPP3_REV_S parameter */
#define CAI_SUPP3_REV_P_ID 107
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP3_REV_P_LEN 24
  /* Length of CAI_SUPP3_REV_P parameter */
#define CAI_SUPP4_REV_S_ID 108
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP4_REV_S_LEN 24
  /* Length of CAI_SUPP4_REV_S parameter */
#define CAI_SUPP4_REV_P_ID 109
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP4_REV_P_LEN 24
  /* Length of CAI_SUPP4_REV_P parameter */
#define CAI_SUPP5_REV_S_ID 110
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP5_REV_S_LEN 24
  /* Length of CAI_SUPP5_REV_S parameter */
#define CAI_SUPP5_REV_P_ID 111
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP5_REV_P_LEN 24
  /* Length of CAI_SUPP5_REV_P parameter */
#define CAI_SUPP6_REV_S_ID 112
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP6_REV_S_LEN 24
  /* Length of CAI_SUPP6_REV_S parameter */
#define CAI_SUPP6_REV_P_ID 113
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP6_REV_P_LEN 24
  /* Length of CAI_SUPP6_REV_P parameter */
#define CAI_SUPP7_REV_S_ID 114
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP7_REV_S_LEN 24
  /* Length of CAI_SUPP7_REV_S parameter */
#define CAI_SUPP7_REV_P_ID 115
  /* Number of Reverse Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP7_REV_P_LEN 24
  /* Length of CAI_SUPP7_REV_P parameter */

#define CAI_SUPP1_FWD_S_ID 116
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP1_FWD_S_LEN 24
  /* Length of CAI_SUPP1_FWD_S parameter */
#define CAI_SUPP1_FWD_P_ID 117
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP1_FWD_P_LEN 24
  /* Length of CAI_SUPP1_FWD_P parameter */
#define CAI_SUPP2_FWD_S_ID 118
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP2_FWD_S_LEN 24
  /* Length of CAI_SUPP2_FWD_S parameter */
#define CAI_SUPP2_FWD_P_ID 119
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP2_FWD_P_LEN 24
  /* Length of CAI_SUPP2_FWD_P parameter */
#define CAI_SUPP3_FWD_S_ID 120
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP3_FWD_S_LEN 24
  /* Length of CAI_SUPP3_FWD_S parameter */
#define CAI_SUPP3_FWD_P_ID 121
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP3_FWD_P_LEN 24
  /* Length of CAI_SUPP3_FWD_P parameter */
#define CAI_SUPP4_FWD_S_ID 122
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP4_FWD_S_LEN 24
  /* Length of CAI_SUPP4_FWD_S parameter */
#define CAI_SUPP4_FWD_P_ID 123
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP4_FWD_P_LEN 24
  /* Length of CAI_SUPP4_FWD_P parameter */
#define CAI_SUPP5_FWD_S_ID 124
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP5_FWD_S_LEN 24
  /* Length of CAI_SUPP5_FWD_S parameter */
#define CAI_SUPP5_FWD_P_ID 125
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP5_FWD_P_LEN 24
  /* Length of CAI_SUPP5_FWD_P parameter */
#define CAI_SUPP6_FWD_S_ID 126
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP6_FWD_S_LEN 24
  /* Length of CAI_SUPP6_FWD_S parameter */
#define CAI_SUPP6_FWD_P_ID 127
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP6_FWD_P_LEN 24
  /* Length of CAI_SUPP6_FWD_P parameter */
#define CAI_SUPP7_FWD_S_ID 128
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, blank-and-burst
     with secondary traffic only */
#define CAI_SUPP7_FWD_S_LEN 24
  /* Length of CAI_SUPP7_FWD_S parameter */
#define CAI_SUPP7_FWD_P_ID 129
  /* Number of Forward Traffic Channel 9600 or 14400 bps frame, primary
     traffic only */
#define CAI_SUPP7_FWD_P_LEN 24
  /* Length of CAI_SUPP7_FWD_P parameter */
#endif /* FEATURE_IS95B_MDR || FEATURE_SPECIAL_MDR */
///////////////////////////////////////////////////////////////////////////////////////////
//
//							BENQ Diag Request Packet
//
///////////////////////////////////////////////////////////////////////////////////////////
#define	BENQ_DIAG_PIM_MMS  0
#define	BENQ_DIAG_SW_VERSION 1
#define	BENQ_DIAG_BBTEST 2
#define	BENQ_DIAG_ERR_LOG_GET 3
#define	BENQ_DIAG_PROJECT_INFO 4
#define	BENQ_DIAG_CHARGER_CTL 5
#define	BENQ_DIAG_PB_TOOL 6
#define	BENQ_DIAG_PT_TABLE 7
#define	BENQ_DIAG_SCHEDULE 8
#define	BENQ_DIAG_SMS 9
#define	BENQ_DIAG_CG 10
#define	BENQ_DIAG_EFS 11
#define	BENQ_DIAG_GETGPIO 12
#define	BENQ_DIAG_SW_FEATURE 13
#define	BENQ_DIAG_NV_IMAGE 14
#define	BENQ_DIAG_EXT_FACTORY 15
#define	BENQ_DIAG_PIM_QUERY 16
#define	BENQ_DIAG_WCDMA_INFO 17
#define BENQ_DIAG_DEL_PBCLLOGSMSSCHALARM 18
#define	BENQ_DIAG_GSM_INFO  21
#define BENQ_DIAG_GETFDNSTATUS 22
#define BENQ_DIAG_NV 24		
#define BENQ_DIAG_PREPAY_STATUS	25

#define BENQ_DIAG_CRC_CHECK 31
#define BENQ_DIAG_GET_AGC_VALUE 38
#define	BENQ_DIAG_LCD_DUMP	39
#define BENQ_DIAG_CM_SET_BAND	41
#define DIAG_SET_DIPSW_F  48		

//20050427 Embert Tsai
// Battery voltage diag
#define BENQDIAG_BATT_CHG_STATE  0x24

//---------------------------------------------------------------------------
//  BENQ SW VERSION
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(BENQ, SW_VERSION_F) 
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(BENQ, SW_VERSION_F)
	byte major;
	byte minor;
	byte custID;
	byte model;
	byte rev;
	byte nvData;
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  BENQ NV
//---------------------------------------------------------------------------
#define BENQ_DIAG_NV_ITEM_SIZE   256
#define BENQDIAG_NV_READ_F       0x01
#define BENQDIAG_NV_WRITE_F      0x02
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(BENQ, NV_F) 
    uint8  operation;
    uint8  more;
    uint8  seqnum;
    uint16 item;								/* Which item - use nv_items_enum_type */
    uint8  item_data[BENQ_DIAG_NV_ITEM_SIZE];	/* Item itself - use nv_item_type      */
    uint16 nv_stat;								/* Status of operation  - use nv_stat_enum_type */    
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(BENQ, NV_F)
    uint8  operation;
    uint8  more;
    uint8  seqnum;
    uint16 item;								/* Which item - use nv_items_enum_type */
    uint8  item_data[BENQ_DIAG_NV_ITEM_SIZE];	/* Item itself - use nv_item_type      */
    uint16 nv_stat;								/* Status of operation  - use nv_stat_enum_type */    
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  BENQ PREPAY STATUS
//---------------------------------------------------------------------------
#define BENQDIAG_PREPAY_SET_STATUS_F       0x01
#define BENQDIAG_PREPAY_SET_PWD_F          0x02
typedef enum{
    BENQDIAG_PREPAY_SET_OK = 0,
    BENQDIAG_PREPAY_NVWRITE_ERR,
    BENQDIAG_PREPAY_PWD_ERR,
}benqdiag_prepay_status_type;
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(BENQ, PREPAY_STATUS_F) 
    uint8                        operation;
    uint8                        status;	/* benqdiag_prepay_status_type */
    uint8                        prepay;
    uint8                        pwd[8];  
    uint8                        oldpwd[8];  
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(BENQ, PREPAY_STATUS_F)
    uint8                        operation;
    uint8                        status;	/* benqdiag_prepay_status_type */
    uint8                        prepay;
    uint8                        pwd[8];  
    uint8                        oldpwd[8];  
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  BENQ CRC CHECK
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(BENQ, CRC_CHECK_F)
	uint32 uiCRC;
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(BENQ, CRC_CHECK_F)
	uint32 uiCRC;
BQTT_DIAGPKT_SUBSYS_RSP_END

///////////////////////////////////////////////////////////////////////////////////////////
//
//						EFS2 Diag Request Packet
//
///////////////////////////////////////////////////////////////////////////////////////////
#define FS_NAME_MAX 80
#define FS_PATH_MAX 128
#define FS_DIR_ENTRY_SIZE 50
#define FS_MAX_BUFFER_SIZE 512

#define O_ACCMODE          0003
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100 /* not fcntl */
#define O_EXCL             0200 /* not fcntl */
#define O_NOCTTY           0400 /* not fcntl */
#define O_TRUNC           01000 /* not fcntl */
#define O_APPEND          02000
#define O_NONBLOCK        04000
#define O_NDELAY        O_NONBLOCK
#define O_SYNC           010000
#define FASYNC           020000 /* fcntl, for BSD compatibility */
#define O_DIRECT         040000 /* direct disk access hint */
#define O_LARGEFILE     0100000
#define O_DIRECTORY     0200000 /* must be a directory */
#define O_NOFOLLOW      0400000 /* don't follow links */
#define O_ITEMFILE     01000000 /* Create special ITEM file. */
/* File OPEN operation clean-up options */
typedef enum {
  FS_OC_CLOSE     = 0x00, /* Close file as-is                              */
  FS_OC_DISCARD   = 0x01, /* Delete file and remove it from directory      */
  FS_OC_TRUNCATE  = 0x02, /* Truncate file from specified position         */
  FS_OC_REVERT    = 0x03  /* Revert to last checkpointed version           */
} fs_cleanup_option_type; 
/* File OPEN operation buffering options */
typedef enum {
  FS_OB_PROHIBIT,         /* Prohibit file buffering                       */
  FS_OB_ALLOW             /* Allow file buffering                          */
} fs_buffering_option_type; 
/* File SET ATTRIBUTE MASK operation file attribute mask values */
typedef enum {
  FS_FA_UNRESTRICTED =0x00FF, /* No file access restrictions               */
  FS_FA_READONLY     =0x00BF, /* File cannot be opened for update access   */
  FS_FA_PERMANENT    =0x007F, /* File cannot be removed nor truncated      */
  FS_FA_SYS_PERMANENT=0x0037, /* File persists across system reformats     */
  FS_FA_REMOTE       =0x001F  /* Remote file (resides outside the space    */
} fs_attribute_mask_type;     /*   managed by the file system)             */
/* -------------------------------------------------------------------------
 * Definitions of EFS2 diagnostic packets.
 * ------------------------------------------------------------------------- */
#define EFS2_DIAG_HELLO_F     0 /* Parameter negotiation packet               */
#define EFS2_DIAG_QUERY_F     1 /* Send information about EFS2 params         */
#define EFS2_DIAG_OPEN_F      2 /* Open a file                                */
#define EFS2_DIAG_CLOSE_F     3 /* Close a file                               */
#define EFS2_DIAG_READ_F      4 /* Read a file                                */
#define EFS2_DIAG_WRITE_F     5 /* Write a file                               */
#define EFS2_DIAG_SYMLINK_F   6 /* Create a symbolic link                     */
#define EFS2_DIAG_READLINK_F  7 /* Read a symbolic link                       */
#define EFS2_DIAG_UNLINK_F    8 /* Remove a symbolic link or file             */
#define EFS2_DIAG_MKDIR_F     9 /* Create a directory                         */
#define EFS2_DIAG_RMDIR_F    10 /* Remove a directory                         */
#define EFS2_DIAG_OPENDIR_F  11 /* Open a directory for reading               */
#define EFS2_DIAG_READDIR_F  12 /* Read a directory                           */
#define EFS2_DIAG_CLOSEDIR_F 13 /* Close an open directory                    */
#define EFS2_DIAG_RENAME_F   14 /* Rename a file or directory                 */
#define EFS2_DIAG_STAT_F     15 /* Obtain information about a named file      */
#define EFS2_DIAG_LSTAT_F    16 /* Obtain information about a symbolic link   */
#define EFS2_DIAG_FSTAT_F    17 /* Obtain information about a file descriptor */
#define EFS2_DIAG_CHMOD_F   18 /* Change file permissions                    */
#define EFS2_DIAG_STATFS_F   19 /* Obtain file system information             */
#define EFS2_DIAG_ACCESS_F   20 /* Check a named file for accessibility       */
#define EFS2_DIAG_NAND_DEV_INFO_F    21 /* Get NAND device info               */
#define EFS2_DIAG_FACT_IMAGE_START_F 22 /* Start data output for Factory Image*/
#define EFS2_DIAG_FACT_IMAGE_READ_F  23 /* Get data for Factory Image         */
#define EFS2_DIAG_FACT_IMAGE_END_F   24 /* End data output for Factory Image  */
#define EFS2_DIAG_PREP_FACT_IMAGE_F  25 /* Prepare file system for image dump */
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_HELLO_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_HELLO_F) 
  uint32 targ_pkt_window;  /* Window size in packets for sends from phone  */
  uint32 targ_byte_window; /* Window size in bytes for sends from phone    */
  uint32 host_pkt_window;  /* Window size in packets for sends from host   */
  uint32 host_byte_window; /* Window size in bytes for sends from host     */
  uint32 iter_pkt_window;  /* Window size in packets for dir iteration     */
  uint32 iter_byte_window; /* Window size in bytes for dir iteration       */
  uint32 version;          /* Protocol version number                      */
  uint32 min_version;      /* Smallest supported protocol version number   */
  uint32 max_version;      /* Highest supported protocol version number    */
  uint32 feature_bits;     /* Feature bit mask; one bit per feature        */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_HELLO_F)
  uint32 targ_pkt_window;  /* Window size in packets for sends from phone  */
  uint32 targ_byte_window; /* Window size in bytes for sends from phone    */
  uint32 host_pkt_window;  /* Window size in packets for sends from host   */
  uint32 host_byte_window; /* Window size in bytes for sends from host     */
  uint32 iter_pkt_window;  /* Window size in packets for dir iteration     */
  uint32 iter_byte_window; /* Window size in bytes for dir iteration       */
  uint32 version;          /* Protocol version number                      */
  uint32 min_version;      /* Smallest supported protocol version number   */
  uint32 max_version;      /* Highest supported protocol version number    */
  uint32 feature_bits;     /* Feature bit mask; one bit per feature        */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_OPENDIR_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_OPENDIR_F) 
  char  path[FS_PATH_MAX];           /* Pathname (null-terminated string)            */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_OPENDIR_F)
  uint32 dirp;             /* Directory pointer. NULL if error             */
  int32 err_no;             /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_CLOSEDIR_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_CLOSEDIR_F) 
  int32 dirp;              /* Directory pointer.                           */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_CLOSEDIR_F)
  int32 err_no;             /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_READDIR_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_READDIR_F) 
  uint32 dirp;             /* Directory pointer.                           */
  int32 seqno;             /* Sequence number of directory entry to read   */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_READDIR_F)
  uint32 dirp;             /* Directory pointer.                           */
  int32  seqno;            /* Sequence number of directory entry  positive integers  */
  int32  err_no;           /* Error code if error, 0 otherwise             */
  int32  entry_type;       /* 0 if file, 1 if directory, 2 if symlink      */
  int32  mode;             /* File mode                                    */
  int32  size;             /* File size in bytes                           */
  int32  atime;            /* Time of last access                          */
  int32  mtime;            /* Time of last modification                    */
  int32  ctime;            /* Time of last status change                   */
  char   entry_name[FS_PATH_MAX];    /* Name of directory entry (not full pathname)  */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_MKDIR_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_MKDIR_F) 
  int16 mode;              /* The creation mode                            */
  char  path[FS_PATH_MAX]; /* Pathname (null-terminated string)            */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_MKDIR_F)
  int32  err_no;           /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_RMKDIR_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_RMKDIR_F) 
  char  path[FS_PATH_MAX]; /* Pathname (null-terminated string)            */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_RMKDIR_F)
  int32  err_no;           /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_UNLINK_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_UNLINK_F) 
  char  path[FS_PATH_MAX]; /* Pathname (null-terminated string)            */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_UNLINK_F)
  int32  err_no;           /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_OPEN_F
//---------------------------------------------------------------------------
#define S_IREAD  1
#define S_IWRITE 2
#define S_IEXEC  4
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_OPEN_F) 
  int32 oflag;             /* Open flags                                   */
  int32 mode;              /* Mode                                         */
  char  path[FS_PATH_MAX];           /* Pathname (null-terminated string)            */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_OPEN_F)
  int32 fd;                /* File descriptor if successful, -1 otherwise  */
  int32 err_no;             /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_CLOSE_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_CLOSE_F) 
  int32 fd;                /* File descriptor                              */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_CLOSE_F)
  int32 err_no;             /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_READ_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_READ_F) 
  int32  fd;               /* File descriptor                              */
  uint32 nbyte;            /* Number of bytes to read                      */
  uint32 offset;           /* Offset in bytes from the origin              */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_READ_F)
  int32  fd;               /* File descriptor                              */
  uint32 offset;           /* Requested offset in bytes from the origin    */
  int32  bytes_read;       /* bytes read if successful, -1 otherwise       */
  int32  err_no;            /* Error code if error, 0 otherwise             */
  char   data[FS_MAX_BUFFER_SIZE]; /* The data read out                            */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_WRITE_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_WRITE_F) 
  int32 fd;                /* File descriptor                              */
  uint32 offset;           /* Offset in bytes from the origin              */
  char  data[FS_MAX_BUFFER_SIZE];           /* The data to be written                       */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_WRITE_F)
  int32 fd;                /* File descriptor                              */
  uint32 offset;           /* Requested offset in bytes from the origin    */
  int32 bytes_written;     /* The number of bytes written                  */
  int32 err_no;             /* Error code if error, 0 otherwise             */
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  EFS EFS2_DIAG_STAT_F
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(EFS2, DIAG_STAT_F) 
  char path[FS_PATH_MAX];         /* Pathname (null-terminated string)               */
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(EFS2, DIAG_STAT_F)
  int32 err_no;             /* Error code if error, 0 otherwise             */
  int32 mode;              /* File mode                                    */
  int32 size;              /* File size in bytes                           */
  int32 nlink;             /* Number of links                              */
  int32 atime;             /* Time of last access                          */
  int32 mtime;             /* Time of last modification                    */
  int32 ctime;             /* Time of last status change                   */
BQTT_DIAGPKT_SUBSYS_RSP_END
// }BenQ
///////////////////////////////////////////////////////////////////////////////////////////
//
//						File system Diag Request Packet (old style)
//
///////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
  FSDIAG_MK_DIR      = 0,      /* Create directory                */
  FSDIAG_RM_DIR      = 1,      /* Remove Directory                */
  FSDIAG_DISP_DIRS   = 2,      /* Display directories list        */
  FSDIAG_DISP_FILES  = 3,      /* Display file list               */
                                /* NOTE: Display ops are temporary 
                                   implementations                 */ 
  FSDIAG_READ_FILE   = 4,      /* Read a file in FS               */
  FSDIAG_WRITE_FILE  = 5,      /* Write a file in FS              */  
  FSDIAG_REMOVE_FILE = 6,      /* Remove a file from FS           */  
  FSDIAG_GET_ATTRIB  = 7,      /* Get file attributes             */
  FSDIAG_SET_ATTRIB  = 8,      /* Set file attributes             */
  FSDIAG_REMOTE_LINK = 9,      /* Create a remote file link       */
  FSDIAG_ITER_DIRS   = 10,     /* Iterative display directories   */
  FSDIAG_ITER_FILES  = 11,     /* Iterative display files         */
  FSDIAG_SPACE_AVAIL = 12,     /* Amount of free space in EFS     */
  FSDIAG_LAST_OP               /* Last OP.  For range checking.   */
} fsdiag_op_enum_type;

/*--------------------------------------------------------------
   Generic structure definitions used in multiple operations.
--------------------------------------------------------------*/
#define FSDIAG_MAX_FILENAME_LEN      60 /* Specified by FS              */
#define FSDIAG_MAX_FILE_BLOCK_SIZE  256 /* Limited by req buffer of 256 */

typedef PACKED struct
{
  byte len;                            /* Length of filename string 
                                          including NULL terminating char   */
  char name[FSDIAG_MAX_FILENAME_LEN]; /* Filename string.  NULL terminated.
                                          Valid data len == filename_len+1  */
} fsdiag_filename_type;

typedef PACKED struct
{
  word len;                               /* Length of data block */
  byte data[FSDIAG_MAX_FILE_BLOCK_SIZE]; /* Data block           */
} fsdiag_data_block_type;

/* File attribute mask */
typedef enum {
  FSDIAG_FA_UNRESTRICTED =0x00FF,  /* No file access restrictions           */
  FSDIAG_FA_PERMANENT    =0x007F,  /* File can't be removed nor truncated   */
  FSDIAG_FA_READONLY     =0x00BF,  /* File can't be opened for update       */
  FSDIAG_FA_SYS_PERMANENT=0x0037,  /* File persists across system reformats */
  FSDIAG_FA_REMOTE       =0x001F   /* Remote file (resides outside file     */
                                    /* system address space)                 */
} fsdiag_attribute_mask_type;     

/* File OPEN operation buffering options */
typedef enum {
  DIAGPKT__FS_OB_PROHIBIT,             /* Prohibit file buffering                */
  DIAGPKT__FS_OB_ALLOW                 /* Allow file buffering                   */
} fsdiag_buffering_option_type;

/* File OPEN operation clean-up options */
typedef enum {
  DIAGPKT__FS_OC_CLOSE     = 0x00, /* Close file as-is                           */
  DIAGPKT__FS_OC_DISCARD   = 0x01, /* Delete file and remove it from directory   */
  DIAGPKT__FS_OC_TRUNCATE  = 0x02, /* Truncate file from specified position      */
  DIAGPKT__FS_OC_REVERT    = 0x03  /* Revert to last checkpointed version        */
} fsdiag_cleanup_option_type;

typedef PACKED struct
{
  word      attribute_mask;  /* Use fsdiag_attribute_mask_type   */
  byte    buffering_option;  /* Use fsdiag_buffering_option_type */
  byte      cleanup_option;  /* Use fsdiag_cleanup_option_type   */
} fsdiag_attributes_type;

/*-------------------------------------------------------
  Definitions of data for specific operations.
-------------------------------------------------------*/
/* "Create Directory" operation */
typedef fsdiag_filename_type fsdiag_mkdir_req_type;

/* "Remove Directory" operation */
typedef fsdiag_mkdir_req_type fsdiag_rmdir_req_type;

/* "Display Directory List" operation */
typedef fsdiag_filename_type fsdiag_disp_dirs_req_type;

/* "Display File List" operation */
typedef fsdiag_disp_dirs_req_type fsdiag_disp_files_req_type;

/* "Read File" operation */
typedef PACKED struct
{
  byte seq_num;                        /* Sequence number for mult blocks */
  
  fsdiag_filename_type filename_info; /* File name info                  */
                                       /* Used only if seq_num == 0       */
} fsdiag_read_req_type;

/* "Write File" operation */
typedef enum
{
  FSDIAG_NO_OVERWRITE = 0,
  FSDIAG_OVERWRITE    = 1
} fsdiag_write_mode_enum_type;

typedef PACKED struct
{ 
  byte                          mode; /* Use fsdiag_write_mode_enum_type  */
  dword                 total_length; /* Total length of this file         */
  fsdiag_attributes_type     attrib; /* Attributes for this file          */
  
  PACKED union
  {
    fsdiag_filename_type  name_info;    /* File name info                 */
    byte raw_data[sizeof(fsdiag_filename_type) +
                  sizeof(fsdiag_data_block_type)];
                                         /* Full possible size of variable
                                            length buffer                  */
  } var_len_buf;
} fsdiag_write_begin_req_type;

typedef PACKED union
{
  fsdiag_write_begin_req_type  begin; /* First block of a write           */
  fsdiag_data_block_type      subseq; /* Subsequent blocks for write      */
} fsdiag_write_block_type;

typedef PACKED struct
{
  byte seq_num;                        /* Sequence number for mult blocks    */
  byte    more;                        /* Flag if more packets are needed to
                                          complete write                     */
  fsdiag_write_block_type     block;  /* Block for this portion of the file */
} fsdiag_write_req_type;

/* "Get File Attributes" operation */
typedef fsdiag_filename_type fsdiag_get_attrib_req_type;

/* "Set File Attributes" operation */
typedef PACKED struct
{
  fsdiag_attributes_type       attribs;
  dword                   creation_date;
  fsdiag_filename_type   filename_info;
} fsdiag_set_attrib_req_type;

/* "Remove File" operation */
typedef fsdiag_filename_type fsdiag_rmfile_req_type;

/* "Remote File Link" operation */
typedef PACKED struct
{
  dword               base_address;
  dword                     length;
  fsdiag_filename_type  name_info;
} fsdiag_remote_link_req_type;

/* "Iterate Directories" operation */
typedef PACKED struct
{
  dword seq_num;
  fsdiag_filename_type dir_name;
} fsdiag_iter_dirs_req_type;

/* "Iterate Files" operation */
typedef fsdiag_iter_dirs_req_type fsdiag_iter_files_req_type;

typedef struct
{
	byte cmd_code;
	byte file_op;                    /* From fsdiag_op_enum_type */
} file_op_header_type;
/*
#define BQTT_DIAGPKT_FILE_OP_REQ_DEFINE( xx_file_op_code ) \
  typedef struct DIAG_FILE_OP_##xx_file_op_code##_req_tag \
                 DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_type; \
  PACKED struct  DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_tag { \
    subsys_header_type cmd_header;

#define BQTT_DIAGPKT_SUBSYS_REQ_END };

#define BQTT_DIAGPKT_SUBSYS_RSP_DEFINE( xx_subsys, xx_subsys_cmd_code ) \
  typedef struct DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag \
                 DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_type; \
  PACKED struct  DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag { \
    subsys_header_type cmd_header;

#define BQTT_DIAGPKT_SUBSYS_RSP_END };

*/

/*

// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_FS_OP_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_VERNO_F)
	char comp_date[ VERNO_DATE_STRLEN ];  // Compile date Jun 11 1991
	char comp_time[ VERNO_TIME_STRLEN ];  // Compile time hh:mm:ss
	char rel_date [ VERNO_DATE_STRLEN ];  // Release date
	char rel_time [ VERNO_TIME_STRLEN ];  // Release time
	char ver_dir  [ VERNO_DIR_STRLEN ];   // Version directory
	byte scm;                             // Station Class Mark
	byte mob_cai_rev;                     // CAI rev
	byte mob_model;                       // Mobile Model
	word mob_firm_rev;                    // Firmware Rev
	byte slot_cycle_index;                // Slot Cycle Index
	byte hw_maj_ver;                      // Hardware Version MSB
	byte hw_min_ver;                      // Hardware Version LSB
BQTT_DIAGPKT_RSP_END
*/
/* Union of all possible operations.  Determined by cmd_code */
typedef PACKED union
{                               
  byte                             seq_num; /* Seq number in same place for
                                               all packets that use them */
  fsdiag_mkdir_req_type             mkdir;
  fsdiag_rmdir_req_type             rmdir;
  fsdiag_read_req_type               read;
  fsdiag_write_req_type             write;
  fsdiag_disp_dirs_req_type     disp_dirs;
  fsdiag_disp_files_req_type   disp_files;
  fsdiag_get_attrib_req_type   get_attrib;
  fsdiag_set_attrib_req_type   set_attrib;
  fsdiag_rmfile_req_type           rmfile;
  fsdiag_remote_link_req_type remote_link;
  fsdiag_iter_dirs_req_type     iter_dirs;
  fsdiag_iter_files_req_type   iter_files;
} fsdiag_req_type;

typedef PACKED struct 
{
  byte cmd_code;
  byte file_op;                    /* From fsdiag_op_enum_type */
  fsdiag_req_type fs_req;      /* Filesystem request data union */

} fsdiag_req_pkt_type;

#define DIAG_FS_REQ_LEN_DIFF FPOS(fsdiag_req_pkt_type, fs_req)

/*===========================================================================

PACKET   fsdiag_op_rsp_type

ID       DIAG_FS_OP_F

PURPOSE  This response contains status and results data from an EFS 
         operation.  The contents of the response depends on the file
         system operation (sub-command) requested.

===========================================================================*/
/*------------------------------------------------
  Operation status values can be found in fs.h
------------------------------------------------*/

/*------------------------------------------------
  Definitions of data for specific operations.
------------------------------------------------*/
/* "Display Directory List" operation */
typedef PACKED struct
{
  word num_dirs;
  fsdiag_data_block_type  dir_list;
} fsdiag_disp_dirs_rsp_type;

/* "Display File List" operation */
typedef PACKED struct
{
  word num_files;
  fsdiag_data_block_type  file_list;
} fsdiag_disp_files_rsp_type;

/* "Read File" operation */
typedef PACKED union
{ 
  PACKED struct
  {
    dword                    total_length;   /* Total length of this file */
    fsdiag_data_block_type  block;
  } begin;

  fsdiag_data_block_type  block;
 
} fsdiag_read_block_type;

typedef PACKED struct
{
  byte                  seq_num;  /* Sequence number for mult blocks */
  byte                     more;  /* Flag if more packets are needed to
                                      complete transfer */
  fsdiag_read_block_type  data;  /* Current portion of the file */
} fsdiag_read_rsp_type;

/* "Write File" operation */
typedef PACKED struct
{
  byte seq_num;
} fsdiag_write_rsp_type;

/* "Get File Attributes" operation */
typedef PACKED struct
{
  fsdiag_attributes_type attrib; /* Attributes */
  dword            creation_date; /* File creation date */
  dword                     size; /* File size */
} fsdiag_get_attrib_rsp_type;

/* "Iterate Directories" and "Iterate Files" operation */
typedef PACKED struct
{
  dword seq_num;
  
  fsdiag_attributes_type   attrib; /* Attributes */
  dword              creation_date; /* File creation date */
  dword               logical_size; /* Size of data */
  dword              physical_size; /* Physical space on device */
  byte              dirname_length; /* Length of directory portion */
                                    /* of file name. */
  fsdiag_filename_type  item_name;
} fsdiag_iter_rsp_type;

/* "Space Available" operation */
typedef dword fsdiag_space_avail_rsp_type;

typedef PACKED union
{
  byte                             seq_num; /* Seq number in same place for 
                                               all packets that use them */
  fsdiag_disp_dirs_rsp_type     disp_dirs;
  fsdiag_disp_files_rsp_type   disp_files;
  fsdiag_read_rsp_type               read;
  fsdiag_write_rsp_type             write;
  fsdiag_get_attrib_rsp_type   get_attrib;
  fsdiag_iter_rsp_type               iter;
  fsdiag_space_avail_rsp_type space_avail;
} fsdiag_rsp_type;

typedef PACKED struct 
{
  byte cmd_code;
  byte file_op;                      /* Operation requested              */
  byte fs_status;                    /* Status of operation - values in  
                                        fs_status_enum_type              */
  fsdiag_rsp_type  fs_rsp;          /* Only used if returning more than 
                                        just status                      */

} fsdiag_rsp_pkt_type;

///////////////////////////////////////////////////////////////////////////////////////////
//
//							WCDMA Diag Request Packet
//
///////////////////////////////////////////////////////////////////////////////////////////
#define WCDMA_DIAG_VERNO_F             0
#define WCDMA_AL1_DATA_F               1
#define WCDMA_DIAG_BLER_RATE_CHANGE_F  8
#define WCDMA_DIAG_ORIG_F              12
#define WCDMA_DIAG_END_F               13
#define WCDMA_STATUS_F                 14
#define WCDMA_ADDITIONAL_STATUS_F      15
#define WCDMA_DIAG_REACQ_SLEW_STATS_F  17
#define WCDMA_DIAG_REACQ_SRCH_STATS_F  18
#define WCDMA_DIAG_REACQ_CLEAR_STATS_F 19
//---------------------------------------------------------------------------
//	WCDMA TMC STATUS
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_STATUS_F) 
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_STATUS_F)
	uint8 tmc_state;	// TMC state 
BQTT_DIAGPKT_SUBSYS_RSP_END
//---------------------------------------------------------------------------
//  WCDMA ADDITIONAL STATUS
//---------------------------------------------------------------------------
// Request packet
BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(WCDMA, WCDMA_ADDITIONAL_STATUS_F) 
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(WCDMA, WCDMA_ADDITIONAL_STATUS_F)
	uint8 imei[9];	// International Mobile Equipment ID 
	uint8 imsi[9];	// International Mobile Subscriber ID
	uint8 l1_state; // l1_state
BQTT_DIAGPKT_SUBSYS_RSP_END

//---------------------------------------------------------------------------
//  PACKET   DIAG_STATUS_SNAPSHOT_F
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_STATUS_SNAPSHOT_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_STATUS_SNAPSHOT_F)
  uint32 esn;
  PACKED struct {
    uint32 imsi_s1;       /* First part of Operational IMSI (IMSI_O_S1) */
    uint16 imsi_s2;        /* Second part of Operational IMSI (IMSI_O_S2) */
    uint64 imsi_s;        /* IMSI_S1 and IMSI_S2 converted to a qword imsi_s */
    uint8  imsi_11_12;     /* Mobile Network Code (MNC) for IMSI_O address */
    uint16 mcc;            /* Mobile Country Code (MCC) for IMSI_O address */
    uint8  imsi_addr_num;  /* Number of digits in NMSI - 4, or class 0 IMSI flag */
  } full_imsi;
  uint16 sid;
  uint16 nid;
  uint8  p_rev;
  uint8  prev_in_use;
  uint8  mob_p_rev;
  uint8  band_class;
  uint16 frequency;
  uint8  operation_mode; //CDMA, Analog, GPS, etc (AKA rf_mode) -LD
  uint8  state;
  uint8  sub_state;
BQTT_DIAGPKT_RSP_END

//---------------------------------------------------------------------------
//  PACKET   DIAG_EVENT_REPORT_F
//---------------------------------------------------------------------------
/*--------------------------------------
  Special bit flags in the event ID.
--------------------------------------*/
#define EVENT_PAY_LENGTH   0x3
#define EVENT_PAY_TWO_BYTE 0x2
#define EVENT_PAY_ONE_BYTE 0x1
#define EVENT_PAY_NONE     0x0
// Request packet
/* Bitfields may not be ANSI, but all our compilers
** recognize it and *should* optimize it.
** Not that bit-packed structures are only as long as they need to be.
** Even though we call it uint32, it is a 16 bit structure.
*/
typedef struct
{
  uint16 id              : 12;
  uint16 reserved        : 1;
  uint16 payload_len     : 2; /* payload length (0, 1, 2, see payload) */
  uint16 time_trunc_flag : 1;
} event_id_type;

typedef PACKED struct
{
  uint16 id; /* event_id_type id; */
  qword ts;

} event_type;

/* Used in lieu of event_type if 'time_trunc_flag' is set in event_id_type */
typedef PACKED struct
{
  uint16 id; /* event_id_type id; */
  uint16 trunc_ts;
} event_trunc_type;

/* The event payload follows the event_type structure */
typedef struct
{
  uint8 length;
  uint8 payload[1]; /* 'length' bytes */
} event_payload_type;

typedef PACKED struct
{
  uint16 id; /* event_id_type id; */
  qword ts;
  uint32 drop_cnt;
} event_drop_type;

typedef struct
{
  event_id_type id;
  uint16 ts;
  uint32 drop_cnt;
} event_drop_trunc_type;
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_EVENT_REPORT_F)
  uint8 enable;
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_EVENT_REPORT_F)
  uint16 length;
  uint8 events[256];
BQTT_DIAGPKT_RSP_END

typedef PACKED struct
{
  uint8  cmd_code;
  uint16 length;    /* Number of bytes to follow */
  uint8  events[1]; /* Series of 'event_type' structures, 'length' bytes long */
} event_rpt_type;



//---------------------------------------------------------------------------
//  PACKET   DIAG_FTM_CMD_F
//---------------------------------------------------------------------------
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

// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_FTM_CMD_F)
  word sub_cmd;
  word length;
  word rf_test_cmd;
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
  }  ftm_rf_factory_data;
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_FTM_CMD_F)

typedef word enum_type;


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


/*Enums*/
typedef enum
{
  FTM_PDM_GSM_VCTCXO_ADJ,
  FTM_PDM_GSM_I_DC_OFFSET,
  FTM_PDM_GSM_Q_DC_OFFSET,
  FTM_PDM_GSM_MAX_NUM
} ftm_gsm_pdm_id_type;

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

typedef PACKED struct
{
  byte  byte0;
  byte  byte1;
  byte  byte2;
  int16 intv;
} ftm_mix_type;

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
	word					GetSynth;
	word					Offset;
    ftm_mode_id_type        mode;       /* For Setting Phone operation mode  */
    ftm_pa_range_type       range;      /* Set the LNA/PA range              */
    ftm_id_val_type         id_val;     /* For all indexed values            */
    ftm_byte_type           byte_struct;
    gsm_option_type         gsm_struct;
	ftm_mix_type            mix_struct;
    byte                    hdet_array[FTM_NUM_HDET_TO_RETURN]; /* ftm hdet block request/response type */
//	uint16 tx_lin_master[NV_WCDMA_NUM_TX_LINEARIZERS][NV_WCDMA_TX_LIN_MASTER_SIZ];
//	int1 freq_comp_table[NV_FREQ_TABLE_SIZ]; 	/* Rx frequency compensation tables */
  }  ftm_rf_factory_data;               /* union of RF commands data types   */
} ftm_rf_factory_parms;

typedef PACKED struct
{
  byte opaque_header[4];
}diagpkt_subsys_header;

typedef PACKED struct
{
  diagpkt_subsys_header header;
  ftm_rf_factory_parms rf_params;
}  ftm_pkt_type;


/*===========================================================================

PACKET   DIAG_LOG_CONFIG_F
PURPOSE  Sent by the DM to set the equipment ID logging mask in the DMSS.  
         This is necessary to use logging services with MS Equip ID != 1.

!!!Note that the log mask is now sanely ordered LSB to MSB using little endian 
32-bit integer arrays.  This is not the same way the mask was done in 
DIAG_EXT_LOGMASK_F.

TERMINOLOGY:
  'equipment ID' - the 4-bit equipment identifier
  'item ID' - the 12-bit ID that specifies the log item within this equip ID
  'code' - the entire 16-bit log code (contains both equip and item ID)

===========================================================================*/
typedef enum {
  LOG_CONFIG_DISABLE_OP = 0,
  LOG_CONFIG_RETRIEVE_ID_RANGES_OP = 1, 
  LOG_CONFIG_RETRIEVE_VALID_MASK_OP = 2,
  LOG_CONFIG_SET_MASK_OP = 3,
  LOG_CONFIG_GET_LOGMASK_OP = 4
} log_config_command_ops_enum_type;

/* Logging config return status types. 
 * (*) denotes applicable to all commands 
 */
typedef enum {
  LOG_CONFIG_SUCCESS_S = 0,          /* Operation Sucessful */
  LOG_CONFIG_INVALID_EQUIP_ID_S = 1, /* (*) Specified invalid equipment ID */
  LOG_CONFIG_NO_VALID_MASK_S = 2     /* Valid mask not available for this ID */
} log_config_status_enum_type;

/* Operation data */
/* DISABLE OP: LOG_CONFIG_DISAPLE_OP -no no supporting data */

/* These member structures are not packed intentionally.  Each data member will 
 * align on a 32-bit boundary.
 */
typedef PACKED struct {
  uint32 equip_id;

  uint32 last_item;

} log_config_range_type;

typedef PACKED struct {
  log_config_range_type code_range; /* range of log codes */
  
  byte mask[1]; /* Array of 8 bit masks of size (num_bits + 7) / 8 */
} log_config_mask_type;

/* ID_RANGE_OP  response type */
typedef PACKED struct {
  uint32 last_item[16]; /* The last item for each of the 16 equip IDs */ 
} log_config_ranges_rsp_type;

/* VALID_MASK_OP request type */
typedef PACKED struct {
  uint32 equip_id;
} log_config_valid_mask_req_type;

/* VALID_MASK_OP response type */
typedef log_config_mask_type log_config_valid_mask_rsp_type;

/* SET_MASK_OP request type */
typedef log_config_mask_type log_config_set_mask_req_type;

/* GET_MASK_OP response type */
typedef log_config_mask_type log_config_get_mask_rsp_type;

/* SET_MASK_OP response type */
typedef log_config_mask_type log_config_set_mask_rsp_type;

/* This is not packed.  We use uint32 which is always aligned */
typedef PACKED union {
  /* LOG_CONFIG_DISABLE_OP */
  /* no additional data */

  /* LOG_CONFIG_RETRIEVE_ID_RANGES_OP */
  /* no additional data */
  
  /* LOG_CONFIG_RETRIEVE_VALID_MASK_OP */
  log_config_valid_mask_req_type valid_mask;

  /* LOG_CONFIG_SET_MASK_OP */
  log_config_set_mask_req_type set_mask;

  /* LOG_CONFIG_GET_MASK_OP */
  /* no additional data */

} log_config_op_req_type;

typedef PACKED union {
  /* LOG_CONFIG_DISABLE_OP */
  /* no additional data */

  /* LOG_CONFIG_RETRIEVE_ID_RANGES_OP */
  log_config_ranges_rsp_type ranges;
  
  /* LOG_CONFIG_RETRIEVE_VALID_MASK_OP */
  log_config_valid_mask_rsp_type valid_mask;

  /* LOG_CONFIG_SET_MASK_OP */
  log_config_set_mask_rsp_type set_mask;

  /* LOG_CONFIG_GET_MASK_OP */
  log_config_get_mask_rsp_type get_mask;

} log_config_op_rsp_type;


BQTT_DIAGPKT_REQ_DEFINE(DIAG_LOG_CONFIG_F)

  byte pad[3]; /* Force following items to be on 32-bit boundary */

  uint32 operation;  /* See log_config_command_ops_enum_type */

  uint32 op_data[1]; /* Pointer to operation data */

BQTT_DIAGPKT_REQ_END

BQTT_DIAGPKT_RSP_DEFINE(DIAG_LOG_CONFIG_F)

  byte pad[3]; /* Force following items to be on 32-bit boundary */

  uint32 operation;  /* See log_config_command_ops_enum_type */

  uint32 status;

  uint32 op_data[1]; /* Pointer to operation data */

BQTT_DIAGPKT_RSP_END


/* Number of bits in a log mask.
*/
#define DIAG_EXT_LOGMASK_NUM_BITS (LOG_1X_LAST_C & 0x0FFF)

/* Max # of bytes in a valid log mask.
*/
#define DIAG_EXT_LOGMASK_NUM_BYTES ((DIAG_EXT_LOGMASK_NUM_BITS / 8) + 1)

/*===========================================================================

PACKET   DIAG_EXT_LOGMASK_F
PURPOSE  Sent by the DM to set the logging mask in the DMSS.  This is
         necessary for logmasks > 32 bits.

===========================================================================*/
BQTT_DIAGPKT_REQ_DEFINE(DIAG_EXT_LOGMASK_F)
  word  num_bits;                 /* Number of valid bits */
  byte  mask[DIAG_EXT_LOGMASK_NUM_BYTES]; /* mask to use          */
BQTT_DIAGPKT_REQ_END

BQTT_DIAGPKT_RSP_DEFINE(DIAG_EXT_LOGMASK_F)
  word  num_valid_bits;                     /* Number of valid bits    */
  byte  valid_mask[DIAG_EXT_LOGMASK_NUM_BYTES]; /* mask of valid log codes */
BQTT_DIAGPKT_RSP_END

//---------------------------------------------------------------------------
//  PACKET   DIAG_LOG_F
//---------------------------------------------------------------------------
// Request packet 
BQTT_DIAGPKT_REQ_DEFINE(DIAG_LOG_F)
BQTT_DIAGPKT_REQ_END
// Response packet
BQTT_DIAGPKT_RSP_DEFINE(DIAG_LOG_F)
  uint8 more;   /* Indicates how many log entries, not including the one 
                   returned with this packet, are queued up in the Mobile
                   Station.  If DIAG_DIAGVER >= 8, this should be set to 0 */
  uint16 len;   /* Indicates the length, in bytes, of the following log entry */
  uint8 log[512]; /* Contains the log entry data. */
BQTT_DIAGPKT_RSP_END

/*===========================================================================

PACKET   DIAG_PR_LIST_WR_F

PURPOSE  Sent by external device to write a Preferred Roaming List to
         the phone.

RESPONSE The diag_pr_list_wr_rsp_type will be sent in response.

===========================================================================*/
#define DIAG_PR_LIST_BLOCK_SIZE 120   /* Size in bytes of the PR_LIST block */
#define DIAG_RL_WR_OK          0    /* No errors */
#define DIAG_RL_WR_OK_DONE     1    /* No errors, PR_LIST is complete */
#define DIAG_RL_WR_OUT_OF_SEQ  2    /* Sequence number out of order */
#define DIAG_RL_WR_OVERFLOW    3    /* PR_LIST overflow */

BQTT_DIAGPKT_REQ_DEFINE(DIAG_PR_LIST_WR_F)
  byte seq_num;                       /* Sequence number */
  byte more;                          /* More to come? */
  byte nam;                           /* which NAM this is associated with */
  word num_bits;                      /* length in bits of valid data */
  byte pr_list_data[DIAG_PR_LIST_BLOCK_SIZE];  /* The block of PR_LIST */
BQTT_DIAGPKT_REQ_END

BQTT_DIAGPKT_RSP_DEFINE(DIAG_PR_LIST_WR_F)
  byte rl_status;                   /* Status of block, as above */
  uint16 nv_stat;                   // Status of operation - use nv_stat_enum_type
BQTT_DIAGPKT_RSP_END

/*===========================================================================

PACKET   DIAG_PR_LIST_RD_F

PURPOSE  Sent by external device to read a Preferred Roaming List from
         the phone.

RESPONSE The diag_pr_list_rd_rsp_type will be sent in response.

===========================================================================*/
BQTT_DIAGPKT_REQ_DEFINE(DIAG_PR_LIST_RD_F)
  byte seq_num;                       /* Sequence number */
  byte nam;                           /* PR_LIST for this NAM */
BQTT_DIAGPKT_REQ_END

#define DIAG_RL_RD_OK         0     /* No error, using non-standard PRL */
#define DIAG_RL_RD_NV_ERR     1     /* NV error */
#define DIAG_RL_RD_683A_OK    2     /* No error, using IS683A PRL */
#define DIAG_RL_RD_NV_683A_ERR   3  /* NV error, using IS683A PRL */

BQTT_DIAGPKT_RSP_DEFINE(DIAG_PR_LIST_RD_F)
	byte rl_status;			// Status of block, as above
	uint16 nv_stat;			// Status of operation - use nv_stat_enum_type
	byte seq_num;				// Sequence number
	byte more;				// More to come?
	word num_bits;			// Number of valid data bits
	byte pr_list_data[DIAG_PR_LIST_BLOCK_SIZE];  // The block of PR_LIST
BQTT_DIAGPKT_RSP_END


BQTT_DIAGPKT_SUBSYS_REQ_DEFINE(BENQ, GET_AGC_VALUE_F) 
BQTT_DIAGPKT_SUBSYS_REQ_END
// Response packet
BQTT_DIAGPKT_SUBSYS_RSP_DEFINE(BENQ, GET_AGC_VALUE_F)
    int16       rx_agc_val;
    int16       tx_agc_val;
    int16       tx_agc_adj;
    byte		rf_lna_pa_state;
BQTT_DIAGPKT_SUBSYS_RSP_END

//------------- DumpLCDMonitor ----------------

typedef enum 
{
        BENQDIAG_LCD_DUMP_PROFILE,
        BENQDIAG_LCD_DUMP_READ,    
        BENQDIAG_LCD_DUMP_NUMBERS = 0xff  /* To make sure that enum is 1-byte! */
} benqdiag_lcd_dump_e_type;

typedef enum 
{
	BENQDIAG_MAIN_PANEL = 1,   /* 16-bits represents one RGB pixel  */
    BENQDIAG_SUB_PANEL = 2,  /* 18-bits represents one RGB pixel  */   
	BENQDIAG_PANEL_NUMBERS = 0xff  /* To make sure that enum is 1-byte! */
} benqdiag_lcd_dump_panel_e_type;

/* To show which the pixel type is */
typedef enum 
{
        BENQDIAG_RGB = 1,
        BENQDIAG_PIXEL_TYPE_NUMBERS = 0xff  /* To make sure that enum is 1-byte! */
} benqdiag_lcd_dump_pixel_e_type;

/* Profile */
typedef struct
{
	char panel; //benqdiag_lcd_dump_panel_e_type  /* which panel */
} benqdiag_lcd_dump_profile_req_s_type;

typedef struct
{
        word row_size;
        word  column_size;
        char pixel_type; //benqdiag_lcd_dump_pixel_e_type
        dword pixel_size;
        byte panel_num;    /* panel number */

} benqdiag_lcd_dump_profile_info_s_type;
/* Read */
typedef struct
{
	char panel; //benqdiag_lcd_dump_panel_e_type  /* which panel */
	word row_index;  /* which row */
} benqdiag_lcd_dump_read_req_s_type;

typedef struct
{       
        dword    option;       /* reserved for future extension */
        byte     row_data[1];  /* Containing the data of one row */  
} benqdiag_lcd_dump_read_info_s_type;


typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    lcd_dump_operation;
	BYTE	panel;
} DIAG_SUBSYS_BENQ_DIAG_LCD_DUMP_F_profile_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    lcd_dump_operation;
	BYTE	status;
	benqdiag_lcd_dump_profile_info_s_type info;
} DIAG_SUBSYS_BENQ_DIAG_LCD_DUMP_F_profile_rsp_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    lcd_dump_operation;
	BYTE	panel; //benqdiag_lcd_dump_panel_e_type  /* which panel */
	WORD	row_index;  /* which row */
} DIAG_SUBSYS_BENQ_DIAG_LCD_DUMP_F_read_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    lcd_dump_operation;
	BYTE	status;
    DWORD	option;       /* reserved for future extension */
    BYTE	row_data[1024];  /* Containing the data of one row */
	
} DIAG_SUBSYS_BENQ_DIAG_LCD_DUMP_F_read_rsp_type;

// 20050427 Embert Tsai
// DIAG command for setting battery voltage. 
typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;
	BYTE	bat_op;
	BYTE	result;
	WORD	voltage;
 
} DIAG_SUBSYS_BENQ_DIAG_BAT_VOL_F_read_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;
	BYTE	bat_op;
	BYTE	result;
	WORD	voltage;
	
} DIAG_SUBSYS_BENQ_DIAG_BAT_VOL_F_read_rsp_type;

typedef PACKED enum 
{
        BENQDIAG_BATTERY_BAR = 0,
        BENQDIAG_BATTERY_CHARGING = 1,
        BENQDIAG_BATTERY_CHARGING_OFF = 2,
        BENQDIAG_BATTERY_HIGH_TEMP = 3,                    /* Show alarm message */
        BENQDIAG_BATTERY_OVER_VOLTAGE = 4,               /* Show alarm message */
        BENQDIAG_BATTERY_CHARGING_OVER_VOLTAGE = 5,   /* Show alarm message */
        BENQDIAG_BATTERY_CHARGING_COMPLETE = 6,        /* Show alarm message */
        BENQDIAG_BATTERY_MSG_OFF = 7,                     /* Cleat alarm message */
        BENQDIAG_BATTERY_TEST_OFF = 8,  
        BENQDIAG_BATTERY_TEST_NUMBERS

}benqdiag_battery_state_e_type;

// 20050513 Embert Tsai
// Add constant for NV Image backup
typedef enum
{
	BENQDIAG_NV_IMG_UPDATE = 0,
	BENQDIAG_NV_IMG_RESTORE = 1,
	BENQDIAG_NV_IMG_ERASE = 2,
	BENQDIAG_NV_IMG_CHECK = 3,
    BENQDIAG_NV_IMG_ESN_CHECK = 4,
    BENQDIAG_NV_IMG_NV_NUM_CHECK = 5
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
    NV_IMG_RF_ITEM_BACKUP_FAIL = 13  /* The RF NV item wasn backup incompletely. */
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
    struct {
               uint16     nv_num_image;    /* the rf nv item count in image */
               uint16     nv_num_target;    /* the rf nv item count in target */
    }nv_num;                       
	} param;
}benqdiag_nv_image_rsp_type;

/*---------------------------------------------------------------------------
  Macro to convert bit number to bit mask
---------------------------------------------------------------------------*/
#define SYS_BM( val ) (1<<(int)(val))
/*---------------------------------------------------------------------------
** Enumeration of system band classes and band classes' sub-bands.
** If there is a new band class, append to the end of list.
---------------------------------------------------------------------------*/


typedef enum
{
	CM_MODE_PREF_AUTOMATIC	=	2,
	CM_MODE_PREF_GSM_ONLY	=	13,
	CM_MODE_PREF_WCDMA_ONLY	=	14,

}cm_mode_pref_e_type;

typedef enum
{
	SYS_SBAND_GSM_EGSM_900	=	8,    /* GSM Extended GSM (E-GSM) band */
	SYS_SBAND_GSM_DCS_1800	=	7,    /* GSM DCS band */
	SYS_SBAND_GSM_PCS_1900	=	21,   /* GSM PCS band */
	SYS_SBAND_WCDMA_I_IMT_2000 = 22,    /* WCDMA I IMT 2000 band */

}sys_sband_e_type;

typedef enum cm_band_pref_e
{
	CM_BAND_PREF_GSM_900_1800 = SYS_BM(SYS_SBAND_GSM_EGSM_900)|SYS_BM(SYS_SBAND_GSM_DCS_1800),
	CM_BAND_PREF_GSM_PCS_1900= SYS_BM( SYS_SBAND_GSM_PCS_1900 ),
	CM_BAND_PREF_WCDMA_I_IMT_2000=	SYS_BM(SYS_SBAND_WCDMA_I_IMT_2000),
	
}cm_band_pref_e_type;

typedef PACKED struct
{
    BYTE   command_code;
    BYTE   subsys_id;
    WORD   subsys_cmd_code;  

    DWORD  mode_pref;   // Mode prefernce, see cm_mode_pref_e
    DWORD  band_pref;    // Band preference, see cm_band_pref_e

}benqdiag_cm_set_band_pref_req_type;

typedef PACKED struct
{
    BYTE   command_code;
    BYTE   subsys_id;
    WORD   subsys_cmd_code;
    BYTE   result;

} benqdiag_cm_set_band_pref_rsp_type;


typedef enum DIPSWT
{
	BIO_SW1_M	=	0x0080,	//128
	BIO_SW2_M	=	0x0040,	//64
	BIO_SW3_M	=	0x0020,	//32	
	BIO_SW4_M	=	0x0010,	//16
	BIO_SW5_M	=	0x0008,	//8
	BIO_SW6_M	=	0x0004,	//4
	BIO_SW7_M	=	0x0002,	//2	
	BIO_SW8_M	=	0x0001,	//1
	BIO_SW9_M	=	0x0100,	//256
	BIO_SW10_M	=	0x0200,	//512
};

typedef PACKED struct
{
  byte cmd_code;
  word switches;

}diag_dipsw_req_type;

typedef diag_dipsw_req_type diag_dipsw_rsp_type;

#endif //_ALLDIAG_H_
