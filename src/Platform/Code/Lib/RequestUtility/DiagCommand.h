#ifndef _DIAGCOMMAND_H_
#define _DIAGCOMMAND_H_

#pragma once
#include "../msm/diagcmd.h"
#include <tchar.h>

typedef enum
{
	DIAG_ERROR_SUCCESS										= 0,
	DIAG_ERROR_CONNECTION_FAIL								= 2,
	DIAG_ERROR_TXRX_FAIL									= 3,
	DIAG_ERROR_INVALID_FUNCTION_PARAMETER					= 4,
	DIAG_ERROR_READ_NVITEM_INVALID_STATUS					= 5,
	DIAG_ERROR_READ_NVITEM_NOTACTIVE						= 6,	
	DIAG_ERROR_WRITE_NVITEM_INTERNAL_DMSS_USED				= 7,
	DIAG_ERROR_WRITE_NVITEM_UNRECOGNIZED_COMMAND			= 8,
	DIAG_ERROR_WRITE_NVITEM_FULL_NV_MEMORY					= 9,
	DIAG_ERROR_WRITE_NVITEM_COMMAND_FAIL					=10,
	DIAG_ERROR_WRITE_NVITEM_VARIABLE_NOT_ACTIVE				=11, //never written
	DIAG_ERROR_WRITE_NVITEM_BAD_PARAMETER_IN_COMMAND_BLOCK	=12,
	DIAG_ERROR_WRITE_NVITEM_READ_ONLY		 				=13,
	DIAG_ERROR_WRITE_NVITEM_ITEM_NOT_DEFINED				=14, //Item not defined for this target!!
	DIAG_ERROR_WRITE_NVITEM_FREE_MEMORY_EXHAUSTED			=15,
	DIAG_ERROR_WRITE_NVITEM_INTERNAL_USE					=16,
	DIAG_ERROR_WRITE_NVITEM_ERROR							=17,
	DIAG_ERROR_MODE_UNDETECTED								=18,
	DIAG_ERROR_UNEXPECTED_RESPONSE							=19,
	DIAG_ERROR_DLMODE										=20,
	DIAG_ERROR_EMPTY_OTP									=21,
	DIAG_ERROR_DAMAGED_OTP									=22,
	DIAG_ERROR_WRONG_OTP_PROTECTED_CODE						=23,
	DIAG_ERROR_GEN_SIMDATA									=24,
	DIAG_ERROR_CODE_LOCKED									=25,
	DIAG_ERROR_WRITESIMDATA									=26,
	DIAG_ERROR_IMEI_MISMATCH								=27,
	DIAG_ERROR_OTP_UNFREEZED								=28,
	DIAG_ERROR_SIMDATA_UNEXPECTED_RESPONSE					=29,
	DIAG_ERROR_BACKUP										=30,
    DIAG_ERROR_SSCP_NWSCP_SAME								=31,
	DIAG_ERROR_READ_SIMDATA_VERSION						=32,
	DIAG_ERROR_GETRESTORESTATUS_FAIL						=33,
	DIAG_ERROR_WRITE_NVITEM_TIMEOUT							=34
} DIAG_ERROR;


//---------------------------------------------------------------------------
//
//	DIAG_NV_READ_F / DIAG_NV_WRITE_F
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
#pragma pack(push, 1)
typedef struct
{
	BYTE  nDIAGcmd;                     
	WORD  item;                          // Which item - use nv_items_enum_type
	BYTE  item_data[DIAG_NV_ITEM_SIZE]; // Item itself - use nv_item_type
	WORD  nv_stat;                       // Status of operation - use nv_stat_enum_type
}DIAG_NVITEM_PACKET_F_type;
#pragma pack(pop)

//---------------------------------------------------------------------------
//
//	DIAG_CONTROL_F
//
//---------------------------------------------------------------------------
typedef enum
{
	MODE_OFFLINE_A_F = 0,	//!<' Go to offline analog
	MODE_OFFLINE_D_F = 1,	//!<' Go to offline digital 
	MODE_RESET_F = 2,		//!<' Reset. Only exit from offline 
	MODE_FTM_F = 3,			//!<' FTM mode
	MODE_ONLINE_F = 4,		//!<' Go to Online 
	MODE_LPM_F = 5,			//!<' Low Power Mode (if supported)
	MODE_POWER_OFF_F = 6,	//!<' Power off (if supported)
	MODE_MAX_F = 7			//!<' Last (and invalid) mode enum value
}DIAG_CONTROL_F_MODE;

#pragma pack(push, 1)
typedef struct
{
	BYTE	nDIAGcmd;
	WORD	nMode;
}DIAG_CONTROL_F_PACKET_type;
#pragma pack(pop)


/*===========================================================================

PACKET   DIAG_EXT_BUILD_ID_F
PURPOSE  Sent from the DM to the DMSS to request the Mobile's ESN

============================================================================*/
#pragma pack(push, 1)
typedef struct
{
	BYTE cmd_code;
	BYTE reserved[3]; /* for alignment / future use */

	unsigned long msm_hw_version;
	unsigned long mobile_model_id;

	/* The following character array contains 2 NULL terminated strings:
	'build_id' string, followed by 'model_string' */
	char ver_strings[512];

} toolsdiag_ext_build_id_rsp_type;
#pragma pack(pop)

/*===========================================================================

PACKET   diagpkt_subsys_hdr_type
PURPOSE  close connection

============================================================================*/
#pragma pack(push, 1)
typedef struct
{
	BYTE cmd_code;			// DIAG_SUBSYS_CMD_F (75)
	BYTE subsys_id;			// DIAG_SUBSYS_USB (35)
	WORD subsys_cmd_code;	// 1000
}diagpkt_subsys_hdr_type;	
#pragma pack(pop)


/*===========================================================================

PACKET   ftm_pmic_pkt_type/ftm_pmic_rsp_header_type
PURPOSE  pmic request/response type

4B 0B 0B 00 00 00 00 00 00 00 00 00 02(24hr) 01(Jan) 1E(Day) C0 
07[1984] 01(Hour) 02(Min) 03(Second) C3 76 7E                     


============================================================================*/

#pragma pack(push, 1)
typedef struct
{
	BYTE cmd_code;			// DIAG_SUBSYS_CMD_F (75)
	BYTE subsys_id;			// DIAG_SUBSYS_FTM (11)
	WORD subsys_cmd_code;	// 11
	BYTE unknown[8];		// 0
	BYTE Mode;				// 24hr = 2
	BYTE Month;
	BYTE Day;
	WORD YEAR;
	BYTE Hour;
	BYTE Min;
	BYTE Sec;

} ftm_pmic_pkt_type;
#pragma pack(pop)
/*===========================================================================

PACKET   diagpkt_sec_code_type

ID       

PURPOSE  SPC

===========================================================================*/

#define  DIAGPKT_SEC_CODE_SIZE 6               /* 6 digit security code */
#pragma pack(push, 1)
typedef struct {
	BYTE cmd_code; //65
	BYTE digits[DIAGPKT_SEC_CODE_SIZE];     /* Security code array */
} diagpkt_sec_code_req_type;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct {
	BYTE cmd_code; //65
	bool sec_code_ok;
} diagpkt_sec_code_resp_type;
#pragma pack(pop)


/*===========================================================================

PACKET   diagpkt_subsidy_lock_req/resp  

ID       

PURPOSE  isLock (general)

===========================================================================*/

#pragma pack(push, 1)
typedef struct {
	BYTE  cmd_code; // 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
	BYTE  subsys_id; // 0x21 DIAG_SUBSYS_GSDI(33)
	WORD  subsys_cmd_code; // 0x42
	DWORD status; // 0
	WORD  delayed_rsp_id; // 0
	WORD  rsp_cnt; // 0
	BYTE  sim_slot_number; // GSDI_SLOT1(1)
} diagpkt_subsidy_lock_req;

typedef struct {
	BYTE  cmd_code;			// DIAG_SUBSYS_CMD_VER_2_F (128)
	BYTE  subsys_id;		// DIAG_SUBSYS_GSDI  (33)
	WORD  subsys_cmd_code;	// GSDI_GET_FEATURE_INDICATION_CMD (0x42)
	DWORD status;			// 0 means success, 1 means error
	WORD  delayed_rsp_id;	// don't care
	WORD  rsp_cnt;			//1
	BYTE  sim_slot_number;		//(always GSDI_SLOT 1)

	/*
	status:
	GSDI_PERSO_ENG_NOT_ACTIVE_STATE = 0
	GSDI_PERSO_ENG_AUTOLOCK_STATE = 1
	GSDI_PERSO_ENG_ACTIVE_STATE = 2
	*/
	BYTE  network_personalization_status;           //SUBSIDY_LOCK_GSM_NWSCP
	BYTE  network_subsuet_personalization_status;
	BYTE  service_provider_personalization_status;
	BYTE  corporation_personalization_status;
	BYTE  sim_personalization_status;
	BYTE  antitheft_personalization_status;         // (user lock for "This SIM only") // MasterCode(SSCP)
} diagpkt_subsidy_lock_resp;
#pragma pack(pop)

/*===========================================================================

PACKET   diagpkt_subsidy_unlock_req/resp

ID       

PURPOSE  unlock except Master code

===========================================================================*/

typedef enum
{
	GSDIDIAG_PERSO_NW        = 0x00000000,	// Network Personalization
	GSDIDIAG_PERSO_NS        = 0x00000001,	// Network Subset Personalization
	GSDIDIAG_PERSO_SP        = 0x00000002,	// Service Provider Personalization
	GSDIDIAG_PERSO_CP        = 0x00000003,	// Corporate Personalization
	GSDIDIAG_PERSO_SIM       = 0x00000004,	// SIM Personalization
	GSDIDIAG_PERSO_PROP1	 = 0x00000010	// Antitheft Personalization

}GSDIDIAG_PERSO;

#pragma pack(push, 1)
typedef struct {
	BYTE cmd_code;			// 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
	BYTE subsys_id;			// 0x21 DIAG_SUBSYS_GSDI(33)
	WORD subsys_cmd_code;	// (0x41)
	DWORD status;			// 0
	WORD  delayed_rsp_id;	// 0
	WORD rsp_cnt;			// 0
	BYTE sim_slot_number;	// GSDI_SLOT1 = 1
	BYTE lock_type;
	char szLock[17];
} diagpkt_subsidy_unlock_req;

typedef struct {
	BYTE cmd_code;			// 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
	BYTE subsys_id;			// 0x21 DIAG_SUBSYS_GSDI(33)
	WORD subsys_cmd_code;	// GSDI_GET_FEATURE_INDICATION_CMD (0x42)
	DWORD status;			// 0 means success, otherwise fail
	WORD delayed_rsp_id;	// don't care
	WORD  rsp_cnt;			// 1
} diagpkt_subsidy_unlock_resp;

#pragma pack(pop)

/*===========================================================================

PACKET   diagpkt_subsidy_unlock_req/resp

ID       

PURPOSE  unlock Master code

===========================================================================*/

#pragma pack(push, 1)

#define GSDI_MAX_APP_ID_LEN 16

typedef struct {
	BYTE	cmd_code;			// 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
	BYTE	subsys_id;			// 0x21 DIAG_SUBSYS_GSDI(33)
	WORD	subsys_cmd_code;	// (0x48)
	DWORD	aps_as_transport; 	//0
	DWORD	subcommand; //93
	BYTE	slot_id;   //0
	BYTE	sim_app_id_len;  //0
	BYTE	sim_app_id[GSDI_MAX_APP_ID_LEN];  //GSDI_MAX_APP_ID_LEN=16  //0
	DWORD	master_key_len; //8
	char	master_key [8];  // lock
	DWORD	new_phone_code_len;  // 4
	char	new_phone_code [4];  // 1,2,3,4
} gsdidiag_subsidy_unlock_PhoneCode_req; 

typedef struct {
	BYTE	cmd_code;			// 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
	BYTE	subsys_id;			// 0x21 DIAG_SUBSYS_GSDI(33)
	WORD	subsys_cmd_code;	// (0x48)
	DWORD	aps_as_transport; 	//0
	DWORD	subcommand; //93
	BYTE	slot_id;   //0
	BYTE	sim_app_id_len;  //0
	BYTE	sim_app_id[GSDI_MAX_APP_ID_LEN];  //GSDI_MAX_APP_ID_LEN=16  //0
	DWORD	master_key_len; //16
	char	master_key [16];  // lock
	DWORD	new_phone_code_len;  // 4
	char	new_phone_code [4];  // 1,2,3,4
} gsdidiag_subsidy_unlock_PhoneCode16_req; 

typedef struct {
	BYTE   command_code;    // 0x80 DIAG_SUBSYS_CMD_VER_2_F(128)
	BYTE   subsys_id;       // 0x21 DIAG_SUBSYS_GSDI(33)
	WORD   subsys_cmd_code; // (0x48)
	DWORD  unknown;			// 0
	WORD   delayed_rsp_id;  // delayed response identifier
	WORD   rsp_count;       // Response count (0-immediate; 1-delayed)
	DWORD  aps_as_transport;// 0
	DWORD  subcommand;      // shall be 93
	DWORD  status;          // 0 means success, otherwise fails.
} gsdidiag_subsidy_unlock_PhoneCode_resp;

#pragma pack(pop)



/*===========================================================================

PACKET   diagpkt_IsEMU

ID       

PURPOSE  unlock Master code

===========================================================================*/
#pragma pack(push, 1)
typedef struct  
{
	BYTE   command_code;    // 0x4B
	BYTE   subsys_id;       // 0x24
	WORD   subsys_cmd_code; // (0x05)
	WORD   IsEMU;
}diagpkt_IsEMU;
#pragma pack(pop)


/*===========================================================================

PACKET   diagpkt_subsys_hdr_type_v2

ID       

PURPOSE  backup/restore

===========================================================================*/
#pragma pack(push, 1)
typedef struct
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	DWORD status;  
	WORD delayed_rsp_id;
	WORD rsp_cnt; /* 0, means one response and 1, means two responses */
}diagpkt_subsys_hdr_type_v2;
#pragma pack(pop)

/*===========================================================================

PACKET   diagpkt_sim_secure_store_cmd request

ID       

PURPOSE  write encrypted SIM lock data to the flash

===========================================================================*/
#pragma pack(push, 1)
typedef struct
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	DWORD aps_as_transport;
	DWORD subcommand;
	BYTE slot_id;
	BYTE sim_app_in_len;
	BYTE sim_app_id[16];
	DWORD simsecure_data_len;
	BYTE simsecure_data[512];
}GSIDDIAG_SIM_SECURE_STORE_REQ;
#pragma pack(pop)

#pragma pack(push, 1)
typedef struct
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	DWORD aps_as_transport;
	DWORD subcommand;
	BYTE slot_id;
	BYTE sim_app_in_len;
	BYTE sim_app_id[16];
	DWORD simsecure_data_len;
	BYTE simsecure_data[576];
}GSIDDIAG_SIM_SECURE_STORE_REQ_576;	// for Project Lavernock
#pragma pack(pop)

/*===========================================================================

PACKET   diagpkt_sim_secure_store_cmd response		 

ID       

PURPOSE  
===========================================================================*/
#pragma pack(push, 1)
typedef struct 
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	DWORD command_status;
	WORD delayed_response_id;
	WORD delayed_response_count;
	DWORD aps_as_transport;
	DWORD subcommand;
	DWORD status;
}GSIDDIAG_SIM_SECURE_STORE_RESP;
#pragma pack(pop)

/*===========================================================================
80-V7032-1
PACKET   GSDIDIAG_SECURE_DEVICE_INFO_CMD request response

ID       

PURPOSE  This function is used to save IMEI, Serial Number to the Flash and 
perform the activation procedure
===========================================================================*/
#pragma pack(push, 1)
typedef struct
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	DWORD aps_as_transport;
	DWORD subcommand;
	BYTE slot_id;
	BYTE sim_app_in_len;
	BYTE sim_app_id[16];
}GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ;
#pragma pack(pop)


#pragma pack(push, 1)
typedef struct 
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	DWORD command_status;
	WORD delayed_response_id;
	WORD delayed_response_count;
	DWORD aps_as_transport;
	DWORD subcommand;
	DWORD status;
}GSDIDIAG_SECURE_DEVICE_ACTIVATE_RESP;
#pragma pack(pop)

/*===========================================================================
PACKET   OTP_STATUS_CHK request response

ID       

PURPOSE  to check if OTP has been frozen
===========================================================================*/
#pragma pack(push, 1)
typedef struct  
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	WORD subsys_cmd_id;
	WORD req_data_length;
	WORD rsp_pkt_size;
	WORD protect_code;
}OTP_STATUS_CHK_REQ;

typedef struct  
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	WORD subsys_cmd_id;
	WORD req_data_length;
	WORD rsp_pkt_size;
	WORD otp_status;
	WORD boot_ver_baseline;
	WORD phonecode_ver_baseline;
}OTP_STATUS_CHK_RESP;

#pragma pack(pop)

/*===========================================================================
PACKET   GET_NV_PARTITION_STATUS request response

ID       

PURPOSE  to get backup NV Partition status

response:
typedef struct {
diagpkt_subsys_hdr_type header;// Sub System header 
cnv_backup_checked_type  cnv_backup_checked;
} nv_cnv_diag_backup_status_rsp_type;

typedef struct {
byte    name[4];    // This shall contain "CNV" 
uint16  flag;       // 1 if has been checked 
uint16  pad;
uint16                                      BackupCount;        
pm_rtc_julian_type                          BackupTime;     
} cnv_backup_checked_type;

typedef struct
{
word year;            // Year [1980..2100]
word month;           // Month of year [1..12]
word day;             // Day of month [1..31]
word hour;            // Hour of day [0..23]
word minute;          // Minute of hour [0..59]
word second;          // Second of minute [0..59]
word day_of_week;     // Day of the week [0..6] Monday..Sunday
} pm_rtc_julian_type;

===========================================================================*/
#pragma pack(push, 1)

typedef struct
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
}diagpkt_subsys_hdr;

typedef struct
{
	diagpkt_subsys_hdr subsys_hdr;
}GET_NV_PARTITION_STATUS_REQ;


typedef struct
{
	WORD year;            // Year [1980..2100]
	WORD month;           // Month of year [1..12]
	WORD day;             // Day of month [1..31]
	WORD hour;            // Hour of day [0..23]
	WORD minute;          // Minute of hour [0..59]
	WORD second;          // Second of minute [0..59]
	WORD day_of_week;     // Day of the week [0..6] Monday..Sunday
} pm_rtc_julian_type;

typedef struct {
	BYTE   name[4];    /* This shall contain "CNV" */
	WORD  flag;      /* 1 if has been checked */
	WORD  pad;
	WORD  BackupCount;        
	pm_rtc_julian_type   BackupTime;     
} cnv_backup_checked_type;

typedef struct
{
	diagpkt_subsys_hdr subsys_hdr;
	cnv_backup_checked_type cnv_backup_checked;
}GET_NV_PARTITION_STATUS_RESP;

#pragma pack(pop)


/*===========================================================================
PACKET   QFUSE_Read request response

ID       

PURPOSE  to get Qfuse status 
{6, 0, 0, 0, 0} = empty

===========================================================================*/

#pragma pack(push, 1)
typedef struct  
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code; //0x18
	WORD FTM_CDM_ID;      //0
	WORD FTM_REQ_DATA_LEN;//0x02
	WORD FTM_RSP_PKT_SIZE;//0x20
	WORD chain_select;
}QFUSE_Read_REQ;

typedef struct  
{
	BYTE command_code;
	BYTE subsys_id;
	WORD subsys_cmd_code;
	WORD FTM_CDM_ID;
	WORD FTM_REQ_DATA_LEN;
	WORD FTM_RSP_PKT_SIZE;
	WORD status;
	int nConfig_Register[5];
}QFUSE_Read_RESP;

#pragma pack(pop)

/*===========================================================================
PACKET   QFUSE_Write request response

ID       

PURPOSE  

===========================================================================*/

#pragma pack(push, 1)
typedef struct  
{
	BYTE command_code;  //75
	BYTE subsys_id;     //11
	WORD subsys_cmd_code; //0x18
	WORD FTM_CDM_ID;      //1
	WORD FTM_REQ_DATA_LEN;//22
	WORD FTM_RSP_PKT_SIZE;//12
	WORD chain_select;
	int config_register[5];
}QFUSE_Write_REQ;

typedef struct  
{
	BYTE command_code;   //75
	BYTE subsys_id;      //11
	WORD subsys_cmd_code;//24
	WORD FTM_CDM_ID;     //1
	WORD FTM_REQ_DATA_LEN;//22
	WORD FTM_RSP_PKT_SIZE;//12
	WORD status; //0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported
}QFUSE_Write_RESP;

#pragma pack(pop)

/*===========================================================================
PACKET   FailureByte_Read\Write request response

ID       

PURPOSE  

===========================================================================*/

#pragma pack(push, 1)
typedef struct  
{
BYTE command_code;  //75 (0x45)
BYTE subsys_id;     //67 (0x43)
WORD subsys_cmd_code; //1 Read,2 Write, 3 Clear
BYTE FailureByte;      //InOut
}FailureByte_REQ;

typedef struct  
{
BYTE command_code;  //75 (0x45)
BYTE subsys_id;     //67 (0x43)
WORD subsys_cmd_code; //1 Read,2 Write, 3 Clear
BYTE Success;      //Out
}FailureByte_RESP;
#pragma pack(pop)


/*===========================================================================
PACKET   GetCMState request response

ID       

PURPOSE  Protocol test

===========================================================================*/

#pragma pack(push, 1)
typedef struct  
{
BYTE   cmd_code; //= DIAG_SUBSYS_CMD_F = 0x4B
BYTE   subsys_id; //= DIAG_SUBSYS_CM = 0x0F
WORD   subsys_cmd_code;// = CM_STATE_INFO = 0x0000
}CM_STATE_Req;

typedef struct  
{
BYTE    cmd_code;// = DIAG_SUBSYS_CMD_F = 0x4B
BYTE    subsys_id; // = DIAG_SUBSYS_CM = 0x0F
WORD   subsys_cmd_code; // = CM_STATE_INFO = 0x0000
DWORD  call_state;                 /* ENUM for EVENT_ID 416 */
DWORD  oprt_mode;                  /* ENUM for EVENT_ID 417 */
DWORD  system_mode;                /* ENUM for EVENT_ID 418 */
DWORD  mode_pref;                  /* ENUM for EVENT_ID 520 */
DWORD  band_pref;                  /* ENUM for EVENT_ID 521 */
DWORD  roam_pref;                  /* ENUM for EVENT_ID 522 */
DWORD  srv_domain_pref;            /* ENUM for EVENT_ID 523 */
DWORD  acq_order_pref;             /* ENUM for EVENT_ID 524 */
DWORD  hybr_pref;                  /* ENUM for EVENT_ID 525 */
DWORD  network_sel_mode_pref;      /* ENUM for EVENT_ID 526 */
DWORD  srv_status;

}CM_STATE_Resp;
#pragma pack(pop)

/*===========================================================================
PACKET   Version Number Request/Response

ID       

PURPOSE  

===========================================================================*/

#pragma pack(push, 1)
typedef struct  
{
	BYTE   cmd_code; //= 0x00
}Version_Number_Req;

typedef struct  
{
	BYTE cmd_code;
	BYTE Comp_Date[11];
	BYTE Comp_Time[8];
	BYTE Rel_Date[11];
	BYTE Rel_Time[8];
	BYTE Ver_Dir[8];
	BYTE SCM;
	BYTE MOB_CAI_REV;
	BYTE MOB_MODEL;
	BYTE MOB_FIRM_REV[2];
	BYTE SLOT_CYCLE_INDEX;
	BYTE MSM_VER[2];
}Version_Number_Resp;
#pragma pack(pop)

/*===========================================================================
  PRL Read/Write

===========================================================================*/
#define DIAG_PR_LIST_BLOCK_SIZE 120

typedef enum{
	PRL_WR_OK,			/* No errors */
	PRL_WR_OK_DONE,		/* No errors, PR_LIST is complete */
	PRL_WR_OUT_OF_SEQ,	/* Sequence number out of order */
	PRL_WR_OVERFLOW		/* PR_LIST overflow */
}DIAG_PR_LIST_WR_STATUS;

typedef enum{
	PRL_RD_OK,			/* No error, using non-standard PRL */
	PRL_RD_NV_ERR,		/* NV error */
	PRL_RD_683A_OK,		/* No error, using IS683A PRL */
	PRL_RD_NV_683A_ERR	/* NV error, using IS683A PRL */
}DIAG_PR_LIST_RD_STATUS;

#pragma pack(push, 1)
typedef struct{
	BYTE cmd_code;
	BYTE seq_num;
	BYTE more;
	BYTE nam;	// Number Assignment Module
	WORD num_bits;
	BYTE pr_list_data[DIAG_PR_LIST_BLOCK_SIZE];
}DIAG_PR_LIST_WR_REQ_TYPE;

typedef struct{
	BYTE cd_code;
	BYTE rl_status;
	WORD nv_stat;
}DIAG_PR_LIST_WR_RSP_TYPE;

typedef struct{
	BYTE cmd_code;
	BYTE seq_num;
	BYTE nam;
}DIAG_PR_LIST_RD_REQ_TYPE;

typedef struct{
	BYTE cmd_code;
	BYTE rl_status;
	WORD nv_stat;
	BYTE seq_num;
	BYTE more;
	WORD num_bits;
	BYTE pr_list_data[DIAG_PR_LIST_BLOCK_SIZE];
}DIAG_PR_LIST_RD_RSP_TYPE;
#pragma pack(pop)


/*===========================================================================
PACKET   for getting simlock file name.

ID       gsdidiag_get_simlock_name_delayed_rsp_type

PURPOSE  

===========================================================================*/
#pragma pack(push, 1)
typedef struct {
	diagpkt_subsys_hdr_type_v2  header ;
	BYTE  name[64];
} gsdidiag_get_simlock_name_delayed_rsp_type;
#pragma pack(pop)




/*===========================================================================
PACKET   DIAG_SUBSYS_WCDMA_STATUS_F_req_type

ID       

PURPOSE  if state == 0, then we can send change2dl command

===========================================================================*/
#pragma pack(push, 1)
typedef struct {
	BYTE   cmd_code; //= DIAG_SUBSYS_CMD_F = 0x4B
	BYTE   subsys_id; //= DIAG_SUBSYS_WCDMA = 0x04
	WORD   subsys_cmd_code;// =  0x0E00
} DIAG_SUBSYS_WCDMA_STATUS_F_req_type;

typedef struct {
	BYTE   cmd_code; //= DIAG_SUBSYS_CMD_F = 0x4B
	BYTE   subsys_id; //= DIAG_SUBSYS_WCDMA = 0x04
	WORD   subsys_cmd_code;// =  0x0E00
	BYTE   tmc_state;   // = if tmc_state==0x00, then we can request change2DL
} DIAG_SUBSYS_WCDMA_STATUS_F_rsp_type;

#pragma pack(pop)

#endif // _DIAGCOMMAND_H_



