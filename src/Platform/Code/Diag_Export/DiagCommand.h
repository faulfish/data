#pragma once
#include "../Lib/msm/diagcmd.h"

typedef enum
{
	DIAG_ERROR_SUCCESS,
	DIAG_ERROR_CONNECTION_FAIL,
	DIAG_ERROR_TXRX_FAIL,
	DIAG_ERROR_INVALID_FUNCTION_PARAMETER,
	DIAG_ERROR_READ_NVITEM_INVALID_STATUS,
	DIAG_ERROR_READ_NVITEM_NOTACTIVE,
	DIAG_ERROR_WRITE_NVITEM_INTERNAL_DMSS_USED,
	DIAG_ERROR_WRITE_NVITEM_UNRECOGNIZED_COMMAND,
	DIAG_ERROR_WRITE_NVITEM_FULL_NV_MEMORY,
	DIAG_ERROR_WRITE_NVITEM_COMMAND_FAIL,
	DIAG_ERROR_WRITE_NVITEM_VARIABLE_NOT_ACTIVE, //never written
	DIAG_ERROR_WRITE_NVITEM_BAD_PARAMETER_IN_COMMAND_BLOCK,
	DIAG_ERROR_WRITE_NVITEM_READ_ONLY,
	DIAG_ERROR_WRITE_NVITEM_ITEM_NOT_DEFINED, //Item not defined for this target!!
	DIAG_ERROR_WRITE_NVITEM_FREE_MEMORY_EXHAUSTED,
	DIAG_ERROR_WRITE_NVITEM_INTERNAL_USE,
	DIAG_ERROR_WRITE_NVITEM_ERROR,
	DIAG_ERROR_MODE_UNDETECTED
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
	GSDIDIAG_PERSO_NW        = 0x00000000,
	GSDIDIAG_PERSO_NS        = 0x00000001,
	GSDIDIAG_PERSO_SP        = 0x00000002,
	GSDIDIAG_PERSO_CP        = 0x00000003,
	GSDIDIAG_PERSO_SIM       = 0x00000004,
	GSDIDIAG_PERSO_PROP1	 = 0x00000010
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
	char szLock[9];
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