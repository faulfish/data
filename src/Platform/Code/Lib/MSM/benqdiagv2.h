/* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/benqdiagv2.h.-arc  $
 * 
 *    Rev 1.1   Apr 01 2009 14:19:22   Kevin Tai
 * no change
 * 
 *    Rev 1.0   Apr 22 2008 20:54:46   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.0   Apr 16 2008 08:20:36   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.12   Jul 20 2005 10:14:48   Eugenia Chen
 * update speed dial structure
 * 
 *    Rev 1.11   Jul 07 2005 17:14:42   Eugenia Chen
 * 1. fix bug for DRUIM
 * 2. add structure for speed dial
 * 
 *    Rev 1.10   Jun 24 2005 08:45:18   Eugenia Chen
 * add speed dial structure
 * 
 *    Rev 1.9   Jun 20 2005 14:58:32   Eugenia Chen
 * add speed dial
 * 
 *    Rev 1.8   Jun 10 2005 16:24:08   Ting Liao
 * Make benqdiag_cmd_code_type consist with benqdiag.h
 * CDMAToolSet_CR SCR - for 235: [XQT][PIM]Files Implement
 * 
 *    Rev 1.7   May 04 2005 18:00:24   Eugenia Chen
 * need place for null character
 * CDMAToolSet_CR SCR - for 252: [XQT][PIM] problem with address field using generate button
 * 
 *    Rev 1.6   Apr 27 2005 19:01:28   Ting Liao
 * Remove EFS related diag
 * CDMAToolSet_CR SCR - for 236: [CDMA][MSM]Create benqEFSdiag.h which includes all EFS related diag command
 * 
 *    Rev 1.5   Apr 27 2005 13:46:30   Eugenia Chen
 * change structure for read and write
 * CDMAToolSet_CR SCR - for 233: XQT_PIM: functions for contacts
 * 
 *    Rev 1.4   Mar 23 2005 17:48:18   Ting Liao
 * Modify SMS index from BYTE to WORD
 * 
 *    Rev 1.3   Mar 10 2005 09:21:16   Ting Liao
 * Modify SMS structure
 * 
 *    Rev 1.1   Feb 17 2005 15:45:24   Ting Liao
 * SMS update
 * 
 *    Rev 1.0   Feb 17 2005 09:31:26   Rick Huang
 * Initial revision.
 * 
 *    Rev 1.3   Jan 13 2005 16:44:28   Joe Lo
 * add benqdiag dloader structures
 * Resolution for 29: Benqdiag subsystem
 * 
 *    Rev 1.2   Jan 13 2005 15:11:54   Joe Lo
 * add benqdiag handler functions and callback functions for PIM PB, CG, SCHEDULE, and SMS 
 * Resolution for 29: Benqdiag subsystem
 * 
 *    Rev 1.1   Jan 10 2005 21:46:04   Allen Wc Li
 * LP and OP implmentation
 * 
 *    Rev 1.0   Jan 05 2005 11:23:34   Joe Lo
 * Initial revision.
 * Resolution for 29: Benqdiag subsystem
*/

/***************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */
#ifndef BENQDIAG_H
#define BENQDIAG_H
/*===========================================================================
 
                      BENQDIAG Header File
 
General Description
 
  This file contains the packet definitions and exported functions for the
  BENQDIAG interface.
 
Copyright (c) 2005 by BenQ Corporation. All Rights Reserved.
===========================================================================*/

#include "comdef.h"
#include "benqEFSdiag.h"

typedef struct
{
  byte opaque_header[4];
}
diagpkt_subsys_header_type;

typedef struct
{
  diagpkt_subsys_header_type header;
} benqdiag_sw_version_req_type;
 
typedef struct
{
  diagpkt_subsys_header_type   header;
  byte major;
  byte minor;
  byte area;
  byte custID;
  byte rev;
  byte nvData;
} benqdiag_sw_version_rsp_type;

typedef enum
{
	BENQDIAG_PIM_MMS = 0,
	BENQDIAG_SW_VERSION =1,
	BENQDIAG_BBTEST = 2,
	BENQDIAG_ERR_LOG_GET = 3,
	BENQDIAG_PROJECT_INFO = 4,
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
	BENQDIAG_GSM_INFO  = 21,
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
	 BENQDIAG_SPEED_DIAL = 40,

	BENQDIAG_MAX
} benqdiag_cmd_code_type;

typedef struct {
	word	year;           /* Year [1980..2100]                            */
	word	month;          /* Month of year [1..12]                        */
	word	day;            /* Day of month [1..31] or day of year [1..366] */
	word	hour;           /* Hour of day [0..23]                          */
	word	minute;         /* Minute of hour [0..59]                       */
	word	second;         /* Second of minute [0..59]                     */
	word	day_of_week;    /* Day of the week [0..6] Monday..Sunday        */
} _clk_julian_type_;

#define  BENQ_DIAG_PIM_SYNC_DEFAULT_FAIL   0xFFFF
#define  BENQ_DIAG_PIM_SYNC_DEFAULT_OK     0
#define  BENQ_DIAG_PIM_SYNC_LOCKED_FAIL    0xFFFE
#define  BENQ_DIAG_PIM_SYNC_INTERRUPTED    0xFFFD 
/**************************************************************************************
 *   Phonebook                                                                        *
 **************************************************************************************/
#define	PIM_NV_MAX_PHONE_NUM      3    /* The max phone number of each phonebook record */
#define PIM_NV_MAX_DIAL_DIGIT     50   
#define PIM_NV_MAX_PB_NAME_STR    42   /* Name of phonebook record in Unicode, null-terminated string(20+1) */
#define PIM_MAX_ADD_LEN           130   /* Resident address of phonebook record in Unicode, null-terminated string(64+1) */
#define PIM_MAX_EMAIL_LEN         130   /* Email address of phonebook record in Unicode, null-terminated string(40+1) */
#define PIM_MAX_NOTE_LEN          130   /* Note length of phonebook record in Unicode, null-terminated string(64+1) */
#define PIM_MAX_RECORDS_PB_ME     500  /* max phonebook records in handset */
#define PIM_MAX_RECORDS_PB_RUIM	  225

typedef enum {
    BENQDIAG_PB_PROFILE,                  /* Init pb service book module */
    BENQDIAG_PB_ADD,                      /* Add new record or modify one record */    
    BENQDIAG_PB_READ,                     /* Read one record by location */
    BENQDIAG_PB_DELETE,                   /* Delete one record by location */
    BENQDIAG_PB_UPDATE,                   /* Update phone book init state */
    BENQDIAG_PB_DELETEALL,                /* Delete all records in phone book */
	BENQDIAG_PB_DELETEALL_PB_ME,
    BENQDIAG_PB_DELETEALL_PB_RUIM,
	BENQDIAG_PB_LIST,
	BENQDIAG_PB_SIM_LIST,
    BENQDIAG_NUMPBOPERATIONS              /* number of pb option events  */
}benqdiag_pb_operation_e_type;

typedef struct
{
	byte	max_phone_set;
	byte	max_name_str;
	byte	max_dial_digit;
	byte	max_email_str;
	byte	max_addr_str;
	byte	max_note_str;	
	byte    max_card_set;
	byte    max_card_name_str;
	byte    max_card_dial_digit;	
/* The sequence of following 4 fields shoul be the same with pb_MemoryStatus */	
	word	sim_max;
	word	phone_max;
	byte	cg_max;
    byte    cg_name_str;
	byte	ringpath_str;
	byte	fdn;
}benqdiag_pb_profile_s_type;

typedef struct
{
	byte	phone_type;/* the number of phone of each phonebook record */
	byte	digits_num;
	byte	digits[PIM_NV_MAX_DIAL_DIGIT];	/* each phone number(ASCII code) */	
}benqdiag_phone_s_type;

typedef struct
{
    word    address;		                	/* location of PB record */
    byte	read_from_phone;                    /* PhoneBook Location 0:Phone, 1:RUIM */  
    byte    groupType;                          /* caller group type */ 
    byte    numberUsed;		               	 	/* how many number used */
	benqdiag_phone_s_type	phone[PIM_NV_MAX_PHONE_NUM];
    byte   letters[PIM_NV_MAX_PB_NAME_STR];							/* phone name */    
	byte   addr[PIM_MAX_ADD_LEN];								/* address field */
    byte   eMail[PIM_MAX_EMAIL_LEN];  							/* eMail */
    byte   note[PIM_MAX_NOTE_LEN];								/* note field */
}benqdiag_pb_info_s_type;

typedef struct
{
	byte	read_from_phone;
	word	cnt;
	word	indexList[PIM_MAX_RECORDS_PB_ME];
}benqdiag_pb_list_s_type;

#define	DIAG_PB_STAT_OK      0x0000
#define DIAG_PB_STAT_FAIL    0xFFFF

//Request type for Profile
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
}benqdiag_pb_profile_req_type;

//Response type for profile
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	uint16                          status;
	benqdiag_pb_profile_s_type		pb_profile;
}benqdiag_pb_profile_rsp_type;

//Request type for list
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
}benqdiag_pb_list_req_type;

//Response type for list
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	uint16                          status;
	benqdiag_pb_list_s_type			pb_list;
}benqdiag_pb_list_rsp_type;

//Request type for read
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	word							address;
	byte							read_from_phone; 
}benqdiag_pb_read_req_type;

//Response type for read
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	uint16                          status;
	word    address;		                	/* location of PB record */
    byte	read_from_phone;                    /* PhoneBook Location 0:Phone, 1:RUIM */  
    byte    groupType;                          /* caller group type */ 
    byte    numberUsed;
	byte	data[512];
//	benqdiag_pb_info_s_type			pb_info;
}benqdiag_pb_read_rsp_type;

//Request type for write
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	word    address;		                	/* location of PB record */
	byte	read_from_phone;                    /* PhoneBook Location 0:Phone, 1:RUIM */  
    byte    groupType;                          /* caller group type */ 
    byte    numberUsed;
	byte	data[512];
//	benqdiag_pb_info_s_type			pb_info;
}benqdiag_pb_write_req_type;

//Response type for write
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	uint16                          status;
	word							address;
	byte							write_to_phone; 
}benqdiag_pb_write_rsp_type;

//Request type for delete
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	word							address;
	byte							delete_from_phone; 
}benqdiag_pb_del_req_type;

//Response type for delete
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    pb_operation;
	uint16                          status;
}benqdiag_pb_del_rsp_type;


/**************************************************************************************
 *   Caller Group                                                                     *   
 **************************************************************************************/
#define PIM_NV_MAX_CG_NUM           6
#define PIM_NV_MAX_CG_NAME_STR      20 /* Name of caller group record in Unicode, null-terminated string(10+1) */
/* !!!Need to be changed!!! */
#define PIM_NV_MAX_FILE_NAME_STR    51 /* Max number of bytes available for ring tone's file name with extention, null-terminated string(50+1) */
#define	DIAG_CG_STAT_OK      0x0000
#define DIAG_CG_STAT_FAIL    0xFFFF

typedef enum {
    BENQDIAG_CG_PROFILE,                  /* Init pt service book module */
    BENQDIAG_CG_ADD,                      /* Add new record or modify one record */    
    BENQDIAG_CG_READ,                     /* Read one record by location */
    BENQDIAG_CG_DELETE,                   /* Dummy, No any effect */
    BENQDIAG_CG_UPDATE,                   /* Update caller group */
    BENQDIAG_CG_LIST,                     /* List caller group */
    BENQDIAG_NUMCGOPERATIONS              /* number of cg option events  */
}benqdiag_cg_operation_e_type;

typedef struct 
{
    byte    address;    
	byte	name_num;
	byte   name[PIM_NV_MAX_CG_NAME_STR];	/* group name */
    byte    ringTone;                            /* group ring tone */  						        
    byte    ringPath[PIM_NV_MAX_FILE_NAME_STR];
} benqdiag_cg_info_s_type;

typedef struct
{
	byte	cnt;
	byte	indexList[PIM_NV_MAX_CG_NUM];
}benqdiag_cg_list_s_type;

//CG list request type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
}benqdiag_cg_list_req_type;

//CG list response type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
	uint16                          status; 
	benqdiag_cg_list_s_type			cg_list; 
}benqdiag_cg_list_rsp_type;

//CG read request type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
	byte    address;
}benqdiag_cg_read_req_type;

//CG read response type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
	uint16                          status; 
	byte    address;    
	byte	name_num;
	byte data[100];
}benqdiag_cg_read_rsp_type;

//CG write request type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
	byte    address;    
	byte	name_num;
	byte data[100];
}benqdiag_cg_write_req_type;

//CG write response type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
	uint16                          status;
	byte							address;
}benqdiag_cg_write_rsp_type;

//CG delete request type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
	byte							address;
}benqdiag_cg_del_req_type;

//CG delete response type
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    cg_operation;
	uint16                          status;
}benqdiag_cg_del_rsp_type;

/**************************************************************************************
 *   Schedule                                                                         *   
 **************************************************************************************/
#define  DIAG_SCH_STAT_OK      0x0000
#define  DIAG_SCH_STAT_FAIL    0xFFFF
#define  PIM_NV_MAX_SCHEDULER_EVENT_LEN_TPZ    101  /* Subject of the event, must be Unicode in big-endian, null-terminated string(100+1) */     
#define  PIM_MAX_SCHEDULER_RECORDS             25     

typedef enum {
    BENQDIAG_SCH_PROFILE,                   /* Init sch service book module */
    BENQDIAG_SCH_ADD,                       /* Add new record or modify one record */    
    BENQDIAG_SCH_READ,                      /* Read one record by location */
    BENQDIAG_SCH_DELETE,                    /* Delete one record by location */
    BENQDIAG_SCH_UPDATE,                    /* Update schedule init state */
    BENQDIAG_SCH_DELETEALL,                 /* Delete all records in schedule */
    BENQDIAG_SCH_LIST,                      /* List all records in schedule */
    BENQDIAG_SCH_ALARM_AUTODEL_TIME,	/* write NV items about alarm_time and auto_del_offset */
    BENQDIAG_NUMSCHOPERATIONS               /* number of option events  */
}benqdiag_sch_operation_e_type;

typedef struct
{
	word	sch_max;
	byte	max_evtName_str;
}benqdiag_sch_profile_s_type;

typedef struct
{
    word                          address;
    _clk_julian_type_			  start_time;     /*define in Clk.h*/
    word	                      duration;
    byte                          is_alarm_on;
    byte						  evt_name[PIM_NV_MAX_SCHEDULER_EVENT_LEN_TPZ];
}benqdiag_sch_info_s_type;

typedef struct 
{
    word   cnt;
    word   indexList[PIM_MAX_SCHEDULER_RECORDS];    /* scheduler data */
}benqdiag_sch_list_s_type;

//Request type for profile
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sch_operation;
}benqdiag_sch_profile_req_type;

//Response type for profile
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	uint16                          status; 
	benqdiag_sch_profile_s_type		sch_profile; 
}benqdiag_sch_profile_rsp_type;

//Request type for list
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
}benqdiag_sch_list_req_type;

//Response type for list
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	uint16                          status; 
	benqdiag_sch_list_s_type		sch_list;
}benqdiag_sch_list_rsp_type;

//Request type for read
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	word							address;
}benqdiag_sch_read_req_type;

//Response type for read
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	uint16                          status; 
	benqdiag_sch_info_s_type		sch_info;
}benqdiag_sch_read_rsp_type;

//Request type for write
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	benqdiag_sch_info_s_type		sch_info;
}benqdiag_sch_write_req_type;

//Response type for write
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	uint16                          status;
	word							address;
}benqdiag_sch_write_rsp_type;

//Request type for delete
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	word							address;
}benqdiag_sch_del_req_type;

//Response type for delete
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE	sch_operation;
	uint16                          status;
}benqdiag_sch_del_rsp_type;

//response type for profile, list, read
typedef struct 
{
    uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sch_operation;
    uint16                           status;  /* remember to change the same field for Kyocera */
	PACKED union
	{
		benqdiag_sch_profile_s_type    sch_profile; 
        benqdiag_sch_info_s_type       sch_info;		
		benqdiag_sch_list_s_type       sch_list;
	}data;
}benqdiag_sch_rsp_type;

/**************************************************************************************
 *   SMS                                                                              *   
 **************************************************************************************/
#define  DIAG_SMS_STATE_OK             0x0000
#define  DIAG_SMS_STATE_FAIL           0xFFFF

#define  PIM_SMS_ADDRESS_MAX           50   
#define  PIM_UI_SMS_MAX_MSGS           23
#define  PIM_UI_SMS_MAX_TEMPLATES      8
#define  PIM_MAX_NV_MSGS               (PIM_UI_SMS_MAX_MSGS + PIM_UI_SMS_MAX_TEMPLATES) 
#define	 PIM_SMS_DATA_LEN_MAX		   320

typedef enum {
    BENQDIAG_SMS_PROFILE,                /* Init pt service book module */
    BENQDIAG_SMS_WRITE,                  /* Add new record or modify one record */    
    BENQDIAG_SMS_READ,                   /* Read one record by location */
    BENQDIAG_SMS_DELETE,                 /* Delete one record by location */
    BENQDIAG_SMS_SEND,                   /* Dummy, No any effect */
    BENQDIAG_SMS_LIST,
    BENQDIAG_SMS_CONCATENATE,
    BENQDIAG_SMS_SET_ARCHIVE_FLAG,
    BENQDIAG_SMS_COPY_MESSAGE,
    BENQDIAG_SMS_CHECK_BOUNDARY,		 /* Check ucs2_to_default and detLen */
	BENQDIAG_NUMSMSOPERATIONS            /* number of option events  */
}benqdiag_sms_operation_e_type;

typedef struct
{
	/* Indicates 4-bit or 8-bit */
    uint8    digit_mode;
    
    /* Used in CDMA only: only meaningful when digit_mode is 8-bit */
    uint8    number_mode;
    
    /* In case of CDMA address, this is used only when digit_mode is 8-bit.
    ** To specify an international address for CDMA, use the following:
    **   digit_mode  = 8-bit
    **   number_mode = NONE_DATA_NETWORK
    **   number_type = INTERNATIONAL
    **   number_plan = TELEPHONY
    **   number_of_digits = number of digits
    **   digits = ASCII digits, e.g. '1', '2', '3', '4' and '5'.
    */ 
    uint8    number_type;
    
    /* In case of CDMA address, this is used only when digit_mode is 8-bit. */ 
    uint8    number_plan;
     
    uint8    number_of_digits;
    
    /* Each byte in this array represents a 4-bit or 8-bit digit of address data. */ 
    uint8    digits[PIM_SMS_ADDRESS_MAX];
    
}benqdiag_sms_address_s_type;

typedef benqdiag_sms_address_s_type benqdiag_sms_callback_s_type;

/* Profile */
typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
}benqdiag_sms_profile_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	WORD	status;
	WORD    max_message_len;    /* maximum number of bytes available for message content */
	WORD	max_addr_len;       /* maximum number of bytes available for address */
}benqdiag_sms_profile_rsp_type;

/* Write */
typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	BYTE	send;        /* 0:Send, 1:Save */
	BYTE	priority;    /* 0:Normal, 1:Urgent */
	WORD	addr_len;
	WORD	address[PIM_SMS_ADDRESS_MAX+1];
	WORD	content_len;
	WORD	content[PIM_SMS_DATA_LEN_MAX];
}benqdiag_sms_write_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	WORD	status;
}benqdiag_sms_write_rsp_type;

/* Read */
typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	BYTE	folder;
	WORD	index;
}benqdiag_sms_read_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	WORD	status;
	WORD	addr_len;
	WORD	address[PIM_SMS_ADDRESS_MAX+1];
	WORD	callback_len;
	WORD	callback[PIM_SMS_ADDRESS_MAX+1];
	BYTE	priority;
	_clk_julian_type_	received_time;
	_clk_julian_type_	sent_time;
	WORD	content_len;
	WORD	content[PIM_SMS_DATA_LEN_MAX];
}benqdiag_sms_read_rsp_type;

/* Delete */
typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	BYTE	folder;
	WORD	index;
}benqdiag_sms_delete_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	WORD	status;
}benqdiag_sms_delete_rsp_type;

/* List */
typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	BYTE	folder;    /* Message folder 0:Inbox, 1:Outbox, 2:Drafts */
}benqdiag_sms_list_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	WORD	status;
	WORD	cnt;          /* Number of messages in the specific folder */
	WORD	indexList[PIM_MAX_NV_MSGS];    /* Index that the requested SMS located in the sprcific folder */
}benqdiag_sms_list_rsp_type;

/* Check content space available */
typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	WORD	addr_len;
	WORD	address[PIM_SMS_ADDRESS_MAX+1];
	WORD	content_len;
	WORD	content[PIM_SMS_DATA_LEN_MAX];
}benqdiag_sms_check_boundary_req_type;

typedef struct
{
	BYTE	command_code;
	BYTE	subsys_id;
	WORD	subsys_cmd_code;

	BYTE    sms_operation;
	WORD	status;
	WORD	space_avail;
}benqdiag_sms_check_boundary_rsp_type;

/***************************************
 Speed Dial
 **************************************/
#define PIM_SPEED_DIAL_NUM    99    /* The max number of speed dial list */
#define	DIAG_SD_STATE_OK      0x0000
#define DIAG_SD_STATE_FAIL    0xFFFF

typedef enum
{
    BENQDIAG_SD_PROFILE,          /* Init speed dial service */
    BENQDIAG_SD_ADD,              /* Add new record or modify one record */    
    BENQDIAG_SD_READ,             /* Read one record by location */
    BENQDIAG_SD_DELETE,           /* Delete one record by location */
    BENQDIAG_SD_LIST,             /* List all records in speed dial */
    BENQDIAG_NUM_SD_OPERATIONS    /* Number of speed dial option events  */
}benqdiag_sd_operation_e_type;

typedef struct
{
	byte	cnt;
	byte	indexList[PIM_SPEED_DIAL_NUM];
}benqdiag_sd_list_info_s_type;

typedef struct
{
	byte  index;      /* Location of Speed Dial, 99 records in total.         *
                      * The 1st is voicemail, 2~99(index:1~98) are unassigned. */
    byte   isRUIM;     /* PhoneBook Location 0:Phone, 1:RUIM */
    word  address;    /* Location of PB record */
    byte   phonetype;    /* phoneType(1 byte), digitNum(1 byte), digits(variable) */  
	byte digitNum;
	byte phone[PIM_NV_MAX_DIAL_DIGIT];
}benqdiag_sd_info_s_type;

//Request type for list
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
}benqdiag_sd_list_req_type;

//Response type for list
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
	uint16                          status;
	benqdiag_sd_list_info_s_type			sd_list;
}benqdiag_sd_list_rsp_type;

//Request type for write
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
	benqdiag_sd_info_s_type sd_info;
}benqdiag_sd_write_req_type;

//Response type for write
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
	uint16  status;
}benqdiag_sd_write_rsp_type;

//Request type for read
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
	byte  index;
}benqdiag_sd_read_req_type;

//Response type for read
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
	uint16  status;
	benqdiag_sd_info_s_type sd_info;
}benqdiag_sd_read_rsp_type;

//Request type for delete
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
	word	index;
}benqdiag_sd_delete_req_type;

//Response type for delete
typedef struct 
{
	uint8     command_code;
    uint8     subsys_id;
    uint16    subsys_cmd_code;
    
    BYTE    sd_operation;
	uint16  status;
}benqdiag_sd_delete_rsp_type;
#endif  //  BENQDIAG_H