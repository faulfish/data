#if !defined _PIM_DIAG_H_
#define _PIM_DIAG_H_

#include "comdef.h"
#include "nv_benq.h"
#include "benqEFSdiag.h"

typedef enum
{
 BENQDIAG_PIM_MMS = 0,
 BENQDIAG_SW_VERSION =1,
 BENQDIAG_BBTEST,
 BENQDIAG_ERR_LOG_GET,
 BENQDIAG_PROJECT_INFO,
 BENQDIAG_USB_CHARGING,
 BENQDIAG_PB_TOOL,
 BENQDIAG_PB_PORTRAIT,
 BENQDIAG_SCHEDULE,
 BENQDIAG_SMS,
 BENQDIAG_CG,
 BENQDIAG_EFS,
 BENQDIAG_GETGPIO,
 BENQDIAG_SW_FEATURE,
 BENQDIAG_NV_IMAGE,
 BENQDIAG_EXT_FACTORY,
 BENQDIAG_PIM_QUERY,
 BENQDIAG_WCDMA_INFO,
 BENQDIAG_DEL_PBCLLOGSMSSCHALARM,
 BENQDIAG_GSM_INFO = 21,
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
 
 BENQDIAG_MAX
} benqdiag_cmd_code_type;

/* Error codes from errno.h that are returned by the EFS2 Diag interface. */
#define EPERM            1      /* Operation not permitted */
#define ENOENT           2      /* No such file or directory */
#define EEXIST           3      /* File exists */
#define ENOTDIR          4      /* Not a directory */
#define ENOSPC           5      /* No space left on device */
#define ESPIPE           6      /* Seek is not permitted */
#define ENAMETOOLONG     7      /* File name too long */
#define ENOTEMPTY        8      /* Directory not empty */
#define EBUSY            9      /* Device or resource is busy */
#define ELOOP           10      /* Too many symbolic links encountered */
#define EISDIR          11      /* Is a directory */
#define EACCES          12      /* Permission denied */
#define ETXTBSY         13      /* File or directory already open */
#define ENOMEM          14      /* Out of memory. */
#define EBADF          100      /* Bad file descriptor */
#define EMFILE         107      /* Too many open files */
#define EINVAL         124      /* Invalid argument */
/* Non-posix errnos used within EFS. */
#define ENOCARD        301      /* No Media present */
#define EBADFMT        302      /* Bad Formated Media */
#define ENOTITM        303      /* Not an EFS2 item file. */
#define EEOF           0x8000   /* End of file. Internal error code */
/* End of Error codes */

#define BENQ_DIAG_PIM_SYNC_INTERRUPTED	0xFFFD
#define BENQ_DIAG_SYNC_LOCKED_FAIL		0xFFFE
/*******************************************************************
 * Phone Book PIM                                                  *
 *******************************************************************/
#define PIM_NV_MAX_NAME_STR             80  
#define PIM_NV_MAX_NAME_STR_LEN         16
#define PIM_NV_MAX_PHONE_NUM            4
#define PIM_NV_MAX_DIAL_DIGITS          32
#define PIM_MAX_EMAIL_LEN               128
#define PIM_MAX_EMAIL_STR_LEN           64
#define PIM_GUI_FS_FILENAME_MAX_LENGTH_P PIM_FS_FILENAME_MAX_LENGTH_P / 2
#define PIM_BOOK_REMARK_LENGTH          100
#define PIM_BOOK_REMARK_STR_LENGTH      50

#define DIAG_PB_STAT_OK                 0x0000
#define DIAG_PB_STAT_FAIL               0xFFFF
#define DIAG_PB_STAT_FULL				0x0003
#define DIAG_PB_STAT_CARDINIT			0x000F

enum
{
    DIAG_PB_PROFILE = 0,              /* Init pb service book module */
    DIAG_PB_ADD,                      /* Add new record or modify one record */    
    DIAG_PB_READ,                     /* Read one record by location */
    DIAG_PB_DELETE,                   /* Delete one record by location */
    DIAG_PB_UPDATE,                   /* Update phone book init state */
    DIAG_PB_DELETEALL,                /* Delete all records in phone book */
	DIAG_PB_DELETEALL_PB_ME,
    DIAG_PB_DELETEALL_PB_RUIM,
    DIAG_NUMPBOPERATIONS,              /* number of option events  */
	DIAG_PB_MAX = 0xFF
};

typedef struct
{
	WORD	address;		                                      /* record real location */
	BYTE	letters[PIM_NV_MAX_NAME_STR+2];                         /* phone name */
	BYTE	lettersNum;		                                      /* phone name length */
	BYTE	digits[PIM_NV_MAX_PHONE_NUM][PIM_NV_MAX_DIAL_DIGITS];  /* phone number (ASCII code) */
	BYTE	digitsNum[PIM_NV_MAX_PHONE_NUM];		              /* phone number length */
	BYTE    phoneType[PIM_NV_MAX_PHONE_NUM];		              /* phone type */
    BYTE	eMail[(PIM_MAX_EMAIL_STR_LEN+1)*2];  	                          /* eMail */
    BYTE	eMailNum;		                                      /* eMail length */
    BYTE    groupType;                                            /* caller group type */ 
    BYTE	numberUsed;				                              /* how many number used */		
    BYTE 	codeFormat;     		                              /* letters code format (UNICODE / ASCII) */
    BYTE    nDefaultNumber;                                       /* The index of the default phone number of this record */
    BYTE    nRingID;
    BYTE    szRingPath[PIM_FS_FILENAME_MAX_LENGTH_P + 2];
    BYTE    nImageID;
    BYTE    szImagePath[PIM_FS_FILENAME_MAX_LENGTH_P + 2];
    BYTE    szRemark[(PIM_BOOK_REMARK_STR_LENGTH+1)*2]; 	
}benqdiag_pb_info_type;

typedef struct
{
	WORD    max_phone_num;
	WORD    max_name_str;
	WORD    max_dial_digit;
	WORD    onphone_id[2];
	WORD    oncard_id[2];
	WORD	sim_used;
	WORD	sim_max;
	WORD	phone_used;
	WORD	phone_max;
}benqdiag_pb_profile_type;

typedef struct
{
    BYTE                            command_code;
    BYTE                            subsys_id;
    WORD                            subsys_cmd_code;
    
    BYTE                            pb_operation;
    WORD                            status;
    union{
        benqdiag_pb_info_type       pb_info;
        benqdiag_pb_profile_type    pb_profile;
    }data;
}benqdiag_pb_rsp_type;

//FDN
typedef struct
{
    uint8   command_code;
    uint8   subsys_id;
    uint16  subsys_cmd_code;    

    byte    getFDNstatus;   

}benqdiag_getFDNstatus_rsp_type; 

/**************************************************************************************
 *   Schedule Data                                                                    *   
 **************************************************************************************/
#define  PIM_NV_MAX_SCHEDULER_EVENT_LEN_V2	100
#define  PIM_NV_MAX_SCHEDULER_EVENT_STR_LEN  50
#define	 MAX_SCHEDULER_RECORDS_V2			100

#define  DIAG_SCH_STAT_OK         0x0000
#define  DIAG_SCH_STAT_FAIL       0xFFFF
#define	 DIAG_SCH_STAT_FULL		  0x0303
#define  DIAG_SCH_STAT_EMPTY	  0x0305
	
#define SCH_ALARM_TYPE_MASK		0x001E
#define SCH_REPEAT_TYPE_MASK	0x00E0

#define UISCH_SET_ALARM_TYPE(status, type)		(status = (byte)((status&~SCH_ALARM_TYPE_MASK) | (type << 1)))
#define UISCH_SET_REPEAT_TYPE(status, type)		(status = (byte)((status&~SCH_REPEAT_TYPE_MASK) | (type << 5)))
#define UISCH_GET_ALARM_TYPE(status)			((status & SCH_ALARM_TYPE_MASK) >> 1)
#define UISCH_GET_REPEAT_TYPE(status)			((status & SCH_REPEAT_TYPE_MASK) >> 5)

enum {
    DIAG_SCH_PROFILE,                  /* Init sch service book module */
    DIAG_SCH_ADD,                      /* Add new record or modify one record */    
    DIAG_SCH_READ,                     /* Read one record by location */
    DIAG_SCH_DELETE,                   /* Delete one record by location */
    DIAG_SCH_UPDATE,                   /* Update phone book init state */
    DIAG_SCH_DELETEALL,                /* Delete all records in phone book */
    DIAG_SCH_IDX_READ,
    DIAG_NUMSCHOPERATIONS              /* number of option events  */
};

typedef struct
{
    BYTE    adress;
    BYTE    status;
    _clk_julian_type_   originalTime;
    _clk_julian_type_	endTime;
    BYTE    curProfileID;
    _clk_julian_type_   alarmTime;  /* Unit: minutes. The offset is since the base date. */
    BYTE    reminderState;   /*record the reminder status*/
    BYTE    alarmoffset[5];
    BYTE    data[(PIM_NV_MAX_SCHEDULER_EVENT_STR_LEN+1)*2];  /* the scheduling data , note or a call number */
    BYTE	subject[(PIM_NV_MAX_SCHEDULER_EVENT_STR_LEN+1)*2];
    BYTE	location[(PIM_NV_MAX_SCHEDULER_EVENT_STR_LEN+1)*2];
    BYTE	attendee[(PIM_NV_MAX_SCHEDULER_EVENT_STR_LEN+1)*2];
} benqdiag_nv_scheduler_V2_s;

typedef struct {
    WORD item;
    benqdiag_nv_scheduler_V2_s item_data;       /* scheduler data */
}benqdiag_sch_info_type;

typedef struct
{
    BYTE cnt;   /* how many scheduler exsit */
    BYTE deleteafterType;  /* record the type of deleteafter */
    BYTE index[MAX_SCHEDULER_RECORDS_V2]; 
} benqdiag_nv_scheduler_index_V3_s;

typedef struct {
    WORD item;
    benqdiag_nv_scheduler_index_V3_s item_data;       /* scheduler data */
}benqdiag_sch_index_type;

typedef struct 
{
    BYTE                      command_code;
    BYTE                      subsys_id;
    WORD                      subsys_cmd_code;
    BYTE                      sch_operation;
    uint16 stat;
	union{
		benqdiag_sch_info_type      sch_info;
		benqdiag_sch_index_type      sch_idx;
	}data;
}benqdiag_sch_rsp_type;

/**************************************************************************************
 *   SMS Data                                                                         *   
 **************************************************************************************/
#define  DIAG_SMS_STAT_OK         0x0000
#define  DIAG_SMS_STAT_FAIL       0xFFFF
#define  PIM_UI_WMS_MSG_LIST_MAX  1101     //NVIM_GW_SMS_FILE_COUNT (1001) + UI_WMS_MSG_SIM_MAX (100)
#define  PIM_UI_WMS_CONCATENATE_MAX   9
#define  PIM_WMS_ADDRESS_MAX          20
#define  PIM_SMS_DATA_LEN_MAX         320

enum {
    DIAG_SMS_PROFILE,                  /* Init pt service book module */
    DIAG_SMS_WRITE,                    /* Add new record or modify one record */    
    DIAG_SMS_READ,                     /* Read one record by location */
    DIAG_SMS_DELETE,                   /* Delete one record by location */
    DIAG_SMS_SEND,                     /* Dummy, No any effect */
    DIAG_SMS_LIST,
    DIAG_SMS_CONCATENATE,
	DIAG_SMS_SET_ARCHIVE_FLAG,
	DIAG_SMS_COPY_MESSAGE,
	DIAG_SMS_CHECK_UCS2TODEFAULT,
    DIAG_NUMSMSOPERATIONS,              /* number of option events  */
	DIAG_SMS_MAX = 0xFF
};

/* GetList Operation */
typedef struct
{
  /* If 'year' is between 96 and 99, the actual year is 1900 + 'year';
     if 'year' is between 00 and 95, the actual year is 2000 + 'year'.
     NOTE: Each field has two BCD digits and byte arrangement is <MSB, ... ,LSB>
  */
  BYTE             year;        /* 0x00-0x99 */
  BYTE             month;       /* 0x01-0x12 */
  BYTE             day;         /* 0x01-0x31 */
  BYTE             hour;        /* 0x00-0x23 */
  BYTE             minute;      /* 0x00-0x59 */
  BYTE             second;      /* 0x00-0x59 */
  signed char      timezone;    /* +/-, [-48,+48] number of 15 minutes - GW only */
} benqdiag_wms_timestamp_s_type;


typedef struct 
{
    BYTE    store;
    DWORD                            index;
    BYTE                             tag;
    benqdiag_wms_timestamp_s_type    timestamp;
}benqdiag_ui_wms_list_data_s_type;


typedef struct
{
  BYTE          digit_mode;
    /* Indicates 4-bit or 8-bit
    */
 
  BYTE         number_mode;
    /* Used in CDMA only: only meaningful when digit_mode is 8-bit
    */
 
  BYTE         number_type;
    /* In case of CDMA address, this is used only when digit_mode is 8-bit.
    ** To specify an international address for CDMA, use the following:
    **   digit_mode  = 8-bit
    **   number_mode = NONE_DATA_NETWORK
    **   number_type = INTERNATIONAL
    **   number_plan = TELEPHONY
    **   number_of_digits = number of digits
    **   digits = ASCII digits, e.g. '1', '2', '3', '4' and '5'.
    */
 
  BYTE         number_plan;
    /* In case of CDMA address, this is used only when digit_mode is 8-bit.
    */
 
  BYTE                          number_of_digits;
 
  BYTE                          digits[PIM_WMS_ADDRESS_MAX];
    /* Each byte in this array represents a 4-bit or 8-bit digit of
    ** address data.
    */
 
} benqdiag_wms_address_s_type;


typedef struct 
{
    WORD                  totalNum;
    WORD                  subNum;
	WORD				  hidden;
	WORD				  seq;
    benqdiag_ui_wms_list_data_s_type data;
	char				  address[PIM_WMS_ADDRESS_MAX];
	BYTE                  name[PIM_NV_MAX_NAME_STR+2];
	BYTE				  time[7];
}benqdiag_ui_wms_list_info_s_type;


typedef struct
{
    BYTE  folder;
	WORD  seq;
}benqdiag_sms_list_req_parameter_type;


/* GetConcatenateInfo */
typedef struct
{
    BYTE      count;
    BYTE      store[PIM_UI_WMS_CONCATENATE_MAX];
    DWORD     index[PIM_UI_WMS_CONCATENATE_MAX];
}benqdiag_ui_wms_concatenate_info_s_type;

typedef struct
{
    BYTE               folder;     
    BYTE               store;    
    DWORD              index;    
}benqdiag_sms_concatenate_req_parameter_type;

/* Read  */
typedef benqdiag_sms_concatenate_req_parameter_type benqdiag_sms_read_req_parameter_type;

typedef struct{
    benqdiag_wms_address_s_type         addr;
    benqdiag_wms_timestamp_s_type       time;
    BYTE                                encoding;
    WORD                                data_len;
	BYTE                                data[PIM_SMS_DATA_LEN_MAX];
}benqdiag_ui_wms_read_info_s_type;

/* Delete */
typedef benqdiag_sms_concatenate_req_parameter_type benqdiag_sms_delete_req_parameter_type;

/* Write */
typedef struct{
    benqdiag_wms_address_s_type         addr;
    BYTE                                encoding;
	BYTE								seq_num;
	BYTE								total_num;
	BYTE								archive;
    WORD                                data_len;
    BYTE                                data[PIM_SMS_DATA_LEN_MAX];
}benqdiag_sms_write_req_parameter_type;

typedef struct{
    BYTE               store;    
    DWORD              index;    
}benqdiag_ui_wms_write_info_s_type;

/* Send */
typedef benqdiag_sms_write_req_parameter_type benqdiag_sms_send_req_parameter_type;

/* Move to Archive */
typedef struct{
    DWORD         index;
    BYTE          flag;    
 }benqdiag_sms_set_archive_flag_req_parameter_type;

/* Check message length and whether message format is 7-bit ascii or not */
typedef struct{
	BYTE        align_used;
    WORD		data_len;
    BYTE		data[PIM_SMS_DATA_LEN_MAX];
}benqdiag_sms_check_req_parameter_type;

typedef struct{
    WORD		data_len;
}benqdiag_sms_Check_rsp_info_s_type;

/* Copy */
typedef benqdiag_sms_concatenate_req_parameter_type benqdiag_sms_copy_req_parameter_type;

//typedef benqdiag_ui_wms_write_info_s_type benqdiag_sms_copy_info_s_type;

typedef struct {
    BYTE                        command_code;
    BYTE                        subsys_id;
    WORD                        subsys_cmd_code;
    BYTE                        sms_operation;
    WORD                        stat;
    WORD                        data_len;  /* data length of real data */
    union
	{
		benqdiag_sms_list_req_parameter_type		list_req;
		benqdiag_sms_concatenate_req_parameter_type	concatenate_req;
		benqdiag_sms_read_req_parameter_type		read_req;
		benqdiag_sms_delete_req_parameter_type		delete_req;
		benqdiag_sms_write_req_parameter_type		write_req;
		benqdiag_sms_send_req_parameter_type		send_req;
		benqdiag_sms_set_archive_flag_req_parameter_type	set_archive_req;
		benqdiag_sms_copy_req_parameter_type		copy_req;
		benqdiag_sms_check_req_parameter_type		check_len_req;
        benqdiag_ui_wms_read_info_s_type           read_info;
        benqdiag_ui_wms_concatenate_info_s_type    concatenate_info;
        benqdiag_ui_wms_list_info_s_type           list_info;
		benqdiag_ui_wms_write_info_s_type			write_info;
		benqdiag_sms_Check_rsp_info_s_type			check_len_info;
	}data;
}benqdiag_sms_rsp_type;

/**************************************************************************************
 *   Call Group Data                                                                  *   
 **************************************************************************************/
#define  DIAG_CG_STAT_OK         0x0000
#define  DIAG_CG_STAT_FAIL       0xFFFF
#define  DIAG_CG_STAT_CARDINIT	 0xFFFA
#define  DIAG_CG_STAT_UPDATE	 0x0108

#define  PIM_NV_MAX_GROUP_MEMBERS        50

enum 
{
    DIAG_CG_PROFILE,                  /* Init pt service book module */
    DIAG_CG_ADD,                      /* Add new record or modify one record */    
    DIAG_CG_READ,                     /* Read one record by location */
    DIAG_CG_DELETE,                   /* Dummy, No any effect */
    DIAG_CG_UPDATE,                   /* Update Call Group */
    DIAG_NUMCGOPERATIONS,              /* number of option events  */
	DIAG_CG_MAX = 0xFF
};


typedef struct 
{
    BYTE    address;
    BYTE    nameNum;                /* group name length */
    BYTE    name[PIM_NV_MAX_NAME_STR+2];     /* group name */
    WORD    memberNum;              /* number of members */
    WORD    member[PIM_NV_MAX_GROUP_MEMBERS];  /* group members */
    BYTE    nRingID;               /* group ring tone */  
	BYTE    szRingPath[PIM_FS_FILENAME_MAX_LENGTH_P + 2];
    BYTE    nImageID;              /* calling picture */
	BYTE    szImagePath[PIM_FS_FILENAME_MAX_LENGTH_P + 2];
    BYTE    LEDPattern;             /* LED pattern */
} benqdiag_pbCallGroupV3S; /* "3" means "version 3" */

typedef struct
{
	WORD item;
    benqdiag_pbCallGroupV3S item_data;
}benqdiag_cg_info_type;


typedef struct 
{
    BYTE                        command_code;
    BYTE                        subsys_id;
    WORD                        subsys_cmd_code;
    BYTE                        cg_operation;
    WORD                        stat;
	
    union{
	    benqdiag_cg_info_type  cg_info;
	}data;

}benqdiag_cg_rsp_type;

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
}benqdiag_get_gpio_rsp_type;


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

/* Response packet */
typedef struct 
{
  BYTE     cmd_code;
  BYTE     subsysid;
  WORD     subsys_cmd_code;
  WORD	   stauts;
  BYTE	   pb_ver;
  BYTE	   sch_ver;
  BYTE	   cg_ver;
  BYTE	   efs_ver;
  BYTE	   sms_ver;
  BYTE	   mms_ver;
}pim_query_rsp_type;

/***********************************************************************************
 * MMS Data                                                                        *
 ***********************************************************************************/
#define   DIAG_MMS_STAT_OK     0
#define   DIAG_MMS_STAT_FAIL   0xFFFF
enum {
    DIAG_MMS_PROFILE,                  /* Init pt service book module */
    DIAG_MMS_SAVE,                    /* Add new record or modify one record */    
    DIAG_MMS_VIEW,                     /* Read one record by location */
    DIAG_MMS_DELMSG,                   /* Delete one record by location */
    DIAG_MMS_SEND,                     
    DIAG_MMS_LIST,
    DIAG_MMS_CLRBOX,
	DIAG_MMS_COPY,
	DIAG_MMS_NUM_IN_BOX,
    DIAG_MMS_CHECK_CONNECTION,
    DIAG_NUMMMSOPERATIONS,              /* number of option events  */
	DIAG_MMS_MAX = 0xFF
};

/* SAVE */
typedef struct
{
	DWORD  		boxId;
	char		PIMListFileName[PIM_FS_FILENAME_MAX_LENGTH_P];
	BYTE		PIMListFileNameLen;
	char		PIMMmsInfoName[PIM_FS_FILENAME_MAX_LENGTH_P];
	BYTE		PIMMmsInfoNameLen;
	char		PIMSMILName[PIM_FS_FILENAME_MAX_LENGTH_P];
	BYTE		PIMSMILNameLen;
}benqdiag_mms_SAVE_req_parameter_type;

typedef struct {
	DWORD          msgID;
}benqdiag_mms_SAVE_info_s_type;

/* SEND */
typedef struct {
	DWORD          msgID;
}benqdiag_mms_SEND_req_parameter_type;

/* LIST */
#define PIM_MC_MAX_URL_LEN			100
typedef struct {
	DWORD			boxID;
	WORD			index;
}benqdiag_mms_LIST_req_parameter_type;

typedef struct
{
    /* Data coding scheme */
    BYTE   			dcs;
    /* length of URL in characters */
    DWORD			len;
    /* data of URL */
    BYTE			data[PIM_MC_MAX_URL_LEN];

}benqdiag_mc_UrlS;

#define PIM_SMS_MAX_ADDR_LENGTH		20	/* Max length of SMS address */
#define PIM_MMS_MAX_ADDR_LEN		51  /* Max length of MMS address */
#define PIM_MMS_MAX_ALIAS_LEN		30  /* Max length of MMS alias */

typedef struct
{
	BYTE  		        addrType;
	BYTE				data[PIM_MMS_MAX_ADDR_LEN];
	BYTE				alias[PIM_MMS_MAX_ALIAS_LEN];
	BYTE                name[PIM_NV_MAX_NAME_STR+2]; 
}benqdiag_mc_MmsAddrS;

typedef benqdiag_mc_MmsAddrS	benqdiag_mc_FromS;
typedef benqdiag_mc_MmsAddrS	benqdiag_mc_ToS;
typedef benqdiag_mc_MmsAddrS	benqdiag_mc_CcS;
typedef benqdiag_mc_MmsAddrS	benqdiag_mc_BccS;

/* MAX length of MMS subject */
// MC has modified the MAX_SUBJECT_LEN from 50 to 76 on 2005/06/14
#define PIM_MC_MAX_SUBJECT_LEN		50
typedef struct 
{
    /* Data coding scheme */
//    BYTE		dcs;   
    DWORD		len;    
    WORD		data[PIM_MC_MAX_SUBJECT_LEN];

}benqdiag_mc_SubjectS;

/* date time structure */
typedef struct
{
    BYTE      year;		/* current year minus 1900 */
	BYTE      month;		/* Month (0 每 11; January = 0) */
	BYTE      day;	    	/* Day of month (1 每 31) */
	BYTE      hour;		/* Hours after midnight (0 每 23) */
	BYTE      minute;		/* Minutes after hour (0 每 59) */
	BYTE      second;		/* Seconds after minute (0 每 59) */
	char      timeZone;	
    BYTE      wDay;		/* Day of week (0 每 6; Sunday = 0) */
	WORD	  yDay;		/* Day of year (0 每 365; January 1 = 0) */	
	
} benqdiag_mc_DateTimeS;

typedef struct
{
    /* The size of the PDU which is in bytes */
    DWORD						pduSize;

    /* URL  */
    benqdiag_mc_UrlS			url;

	/* add by steven 2004-06-04 for notify content enhanced  */
	benqdiag_mc_FromS  		   from;
    BYTE  		               msgClass;    
    BYTE  			           priority;   
    benqdiag_mc_SubjectS	   subject;
	/* end add */
    /*add by colin 2004-10-11*/
    benqdiag_mc_DateTimeS            dateTime;
    
}benqdiag_mc_NotifyS;

typedef struct
{
	BYTE	typeOfNumber;	/*The type of the address*/
	BYTE	numberPlanId;	/*The number plan ID*/
	BYTE	addrLen;		/*The length of the address*/
	BYTE	addrValue[PIM_SMS_MAX_ADDR_LENGTH];	/*The buffer to store the actual value of the address*/

}benqdiag_mc_SmsAddrS;

typedef struct
{
    /*
     *  This is the original address of the SM
     */
    benqdiag_mc_SmsAddrS            smeAddr;

    /*
     *  The time when the SM arrives SC
     */
    BYTE              sentTime[6];   //mc_Bcd_Time
    /* Is it a concatenated short message ? */
    BYTE                 bConcatenated;
    /*is status report request on/off*/
    BYTE                 bSrr;
    /*
     *  If the MC received new message's Reply Path on/off
     *  NOTE: it's different to bRP at sms setting
     */
    BYTE                 bMTReplyPath;
    /*
     *  When get header, if Reply Path on, get the receiving message's SCA.
     *  When set header, the SCA from receiving message or from setting 
     *  depends on the bMTReplyPath.
     */    
    benqdiag_mc_SmsAddrS             scAddr;

}benqdiag_mc_SmsHdrS;

#define PIM_MC_MAX_BCC_NUM			1
#define PIM_MC_MAX_CC_NUM			1
#define PIM_MC_MAX_TO_NUM			10
typedef struct
{
    benqdiag_mc_DateTimeS			date;
    BYTE						    bDeliveryReport;
    benqdiag_mc_FromS			    from;
	BYTE							msgClass;
    DWORD							msgSize;
    BYTE				            priority;	// mc_MmsPriorityE
    BYTE		                    bReadReport;
    benqdiag_mc_SubjectS			subject;
	BYTE							to_num;
    benqdiag_mc_ToS					to[PIM_MC_MAX_TO_NUM];

}benqdiag_mc_MmsHdrFieldS;

typedef union
{
    benqdiag_mc_NotifyS            notify;
    benqdiag_mc_SmsHdrS            smsHdr;
    benqdiag_mc_MmsHdrFieldS       mmsHdr;

}benqdiag_mc_MsgHdrU;

typedef struct
{
    /* What kind of message this message was */
    BYTE                       msgType;
    /* The header information of the message */
    benqdiag_mc_MsgHdrU	        msgHdr;

}benqdiag_mc_MsgHdrS;

typedef struct
{
    /* What kind of message this message was */
    BYTE			msgType;
    /* Id of the box where the message locates */
    DWORD			boxId;
    /* The time when the message sent from originor */
    BYTE			sentTime[6];  //mc_BcdTime
    /* Indicate the message is read or not */
    BYTE			bRead;

//#ifdef MC_SUPPORT_FORWARD_LOCK
    BYTE			  drmFlag;		/* whether forward-lock is set */
//#endif
} benqdiag_mc_MsgInfoS;


typedef struct
{
	benqdiag_mc_MsgInfoS	msgInfo;
	benqdiag_mc_MsgHdrS		msgHdr;
	DWORD					msgID;
}benqdiag_mms_list_info_s_type;

/* VIEW */
#define	PIM_MC_MAX_FILENAME_LEN		100
#define PIM_MC_MAX_UCS2_NAME_LEN    PIM_MC_MAX_FILENAME_LEN
typedef struct 
{
    /* length of name in characters */
    WORD			len;
    /* data of name */
    WORD			data[PIM_MC_MAX_UCS2_NAME_LEN];

}benqdiag_mc_UcsNameS;

typedef struct
{
    benqdiag_mc_MmsHdrFieldS       mmsHdr;		/* MMS header info */    
    benqdiag_mc_UcsNameS           dirName;		/* dir the MMS message locates */
    BYTE               bPresent;		/* whether SMIL exist */	
//#ifdef MC_SUPPORT_ATTACHMENT
	BYTE				  bAttachment;	/* whether attachment included */
//#endif

}benqdiag_mc_MmsInfoS;

typedef struct
{
	BYTE	            	    status;	
	benqdiag_mc_MmsInfoS		mmsInfo;
	
} benqdiag_mc_PcStartViewerIndS;

typedef struct
{
	DWORD      msgID;
}benqdiag_mms_VIEW_req_parameter_type;

typedef struct
{
	benqdiag_mc_PcStartViewerIndS  ViewerInd;
}benqdiag_mms_VIEW_info_s_type;

/* DELETE MSG */
typedef struct
{
	DWORD      msgID;
}benqdiag_mms_DELMSG_req_parameter_type;

/* CLEAR BOX */
typedef struct
{
	DWORD      boxID;
}benqdiag_mms_CLRBOX_req_parameter_type;

/* COPY to Archive */
typedef struct
{
	DWORD		msgID;
	WORD		msgNum;
	DWORD		boxID;
}benqdiag_mms_COPY_req_parameter_type;

/* NUM in BOX */
typedef struct
{
	DWORD		boxID;
}benqdiag_mms_NumInBox_req_parameter_type;

typedef struct
{
	DWORD		msg_num;
	DWORD		msg_unop_num;
}benqdiag_mms_NumInBox_info_s_type;

typedef struct {
    BYTE                        command_code;
    BYTE                        subsys_id;
    WORD						subsys_cmd_code;
    BYTE						mms_operation;
    WORD						stat;
    WORD						data_len;  /* data length of real data */
    union
	{
		benqdiag_mms_SAVE_req_parameter_type		save_req;
		benqdiag_mms_SEND_req_parameter_type		send_req;
		benqdiag_mms_LIST_req_parameter_type		list_req;
		benqdiag_mms_VIEW_req_parameter_type		view_req;
		benqdiag_mms_DELMSG_req_parameter_type		delete_msg_req;
        benqdiag_mms_CLRBOX_req_parameter_type		clear_box_req;
		benqdiag_mms_COPY_req_parameter_type		copy_rep;
		benqdiag_mms_NumInBox_req_parameter_type	NumInBox_req;
	}data;
}benqdiag_mms_req_type;

typedef struct {
    BYTE                        command_code;
    BYTE                        subsys_id;
    WORD						subsys_cmd_code;
    BYTE						mms_operation;
    WORD						stat;
    WORD						data_len;  /* data length of real data */
    union
	{
		benqdiag_mms_SAVE_info_s_type				save_info;
		benqdiag_mms_list_info_s_type				list_info;
		benqdiag_mms_VIEW_info_s_type				view_info;
		benqdiag_mms_NumInBox_info_s_type			NumInBox_info;
	}data;
}benqdiag_mms_rsp_type;

//////////////////////////////////////////////////////////////////////////
// Copy from mc_dtd.h
//////////////////////////////////////////////////////////////////////////
#define	MC_MAX_FILENAME_LEN		100

/* Max attachment support */
#define MC_MAX_ATTACH_OBJ_NUM	10

/* Max number of entry (part) in an MMS message */
#define MMS_MAX_SLIDE_NUM		20
// 2005/06/24, MC modified size of MMS_MAX_ENTRY_NUM from 61 to 71(these extra 10 is for attachment)	
#define	MMS_MAX_ENTRY_NUM		(MMS_MAX_SLIDE_NUM*3 + 1 + MC_MAX_ATTACH_OBJ_NUM) /* at least support 20 slide */

/* Max length of content location and id */
#define MC_MMS_MAX_CONTENT_LOCATION_LEN		100
#define MC_MMS_MAX_CID_LEN					100

typedef enum
{
     MCSM_PREDEFINED_INBOX       = 0xFFFFFF00,
     MCSM_PREDEFINED_OUTBOX,
     MCSM_PREDEFINED_SIM_INBOX,
     MCSM_PREDEFINED_SIM_OUTBOX,
     MCSM_PREDEFINED_NOTIFYBOX,
     MCSM_PREDEFINED_DRAFTBOX,	 
     MCSM_PREDEFINED_SENTBOX, 
     MCSM_PREDEFINED_TEMPLATEBOX,
	 MCSM_PREDEFINED_ARCHIVEBOX,

    MCSM_PREDEFINED_BOX_NUM = 9
 
}mc_PredefinedBoxE;

typedef enum
{
    DCS_NOT_PRESENT = -1,

    DCS_ASC2,       /* 8 bit encoding */
    DCS_UCS2,       /* 16 bit encoding */

    DCS_TOTAL_NUM

}mc_DcsE;

/* X-Mms-Message-Class field */
typedef enum
{
	MMS_MESSAGE_CLASS_NOT_PRESENT		=	-1,
		MMS_MESSAGE_CLASS_PERSONAL		=	128,
		MMS_MESSAGE_CLASS_ADVERTISEMENT	=	129,
		MMS_MESSAGE_CLASS_INFORMATIONAL	=	130,
		MMS_MESSAGE_CLASS_AUTO			=	131,
		MMS_MESSAGE_CLASS_NUMBER		=	4

}mc_MmsMessageClassE;

/* Mms-Priority field */
typedef enum
{
	MMS_PRIORITY_NOT_PRESENT	=	-1,
		MMS_PRIORITY_LOW		=	128,
		MMS_PRIORITY_NORMAL		=	129,
		MMS_PRIORITY_HIGH		=	130,

		MMS_PRIORITY_NUMBER		=	3
        
}mc_MmsPriorityE;

/* DRM flag enum */
typedef enum
{
    DRM_FLAG_NOT_PRESENT = -1,
        DRM_FLAG_NOT_SET,
        DRM_FLAG_SET,
        
        DRM_FLAG_NUM
        
} mc_DrmFlagE;

typedef enum
{
	CT_NOT_PRESENT			=	-1,
		CT_UNKNOW			=	0X00,
		CT_TEXT_UNKNOW		=	0X01,
		CT_TEXT_HTML		=	0X02,
		CT_TEXT_PLAIN		=	0X03,
		CT_TEXT_VCAL		=	0X06,
		CT_TEXT_VCARD		=	0X07,
		CT_TEXT_WML			=	0X08,
		CT_MULT_UNKNOW		=	0X0B,
		CT_MULT_MIXED		=	0X0C,
		CT_MULT_ALTER		=	0X0F,
		CT_IMAGE_UNKNOW		=	0X1C,
		CT_IMAGE_GIF		=	0X1D,
		CT_IMAGE_JPEG		=	0X1E,
		CT_IMAGE_TIFF		=	0X1F,
		CT_IMAGE_PNG		=	0X20,
		CT_IMAGE_WBMP		=	0X21,
		CT_APPL_MULT_UNKNOW	=	0X22,
		CT_APPL_MULT_MIXED	=	0X23,
		CT_APPL_MULT_ALTER	=	0X26,
		CT_APPL_XML			=	0X27,
		CT_TEXT_XML			=	0X28,
		CT_APPL_MULT_RELATED=	0X33,
		CT_APPL_MMS_MESSAGE	=	0X3E,

		/* add by steven 2004-08-18 */
		CT_APPL_DRM_MESSAGE	=	0x48,
		CT_APPL_DRM_CONTENT	=	0x49,
		CT_APPL_DRM_RIGHTSXML  =0x4A,
		CT_APPL_DRM_RIGHTSWBXML=0x4B,
		/* end add 2004-08-18 */
		
        /* define by BENQ */
        CT_X_APPL_SMIL      =   0xFF00, /* application/smil */
        CT_X_IMAGE_BMP      =   0xFF01, /* image/bmp */

		CT_X_AUDIO_MIDI     =   0xFF02, /* audio/midi, audio/mid, audio/sp-midi, audio/x-midi */
		CT_X_AUDIO_AMR      =   0xFF03, /* audio/amr */
		CT_X_AUDIO_WAV		=	0XFF04, /* audio/wav */
		CT_X_AUDIO_SMAF		=	0XFF05, /* audio/smaf */
		CT_X_AUDIO_3GPP		=	0XFF06, /* audio/3gpp */
		CT_X_AUDIO_3GP2		=	0XFF07, /* audio/3gpp */
		CT_X_AUDIO_MP4		=	0XFF08, /* audio/mp4 */
		CT_X_AUDIO_CMX		=	0XFF09, /* audio/cmx */
		CT_X_AUDIO_BENQ_MELODY = 0XFF0A, /* audio/cmx */
		CT_X_AUDIO_MP3		=	0xFF0B,
		CT_X_AUDIO_AAC		=	0xFF0C,
		CT_X_AUDIO_QCP		=	0xFF0D,
		CT_X_AUDIO_IMELODY	=	0xFF0E,
		CT_X_AUDIO_PMD		=	0xFF0F,
		CT_X_AUDIO_UNK		=	0xFF10,
        CT_X_AUDIO_UNKNOWN  =   0xFF11, /* audio/unknown */
		
		CT_X_VIDEO_3GPP		=	0xFF12,	/* video/3gpp */
		CT_X_VIDEO_3GP2		=	0xFF13,	/* video/3gpp */
		CT_X_VIDEO_MP4		=	0xFF14,	/* video/mp4  */
		CT_X_VIDEO_AVI		=	0xFF15,	/* video/avi */
		CT_X_VIDEO_UNKNOWN  =   0xFF16, /* video/unknown */

		CT_X_APPL_OCTET     =   0xFF17, /* application/ocet-stream */

		CT_NUMBER			=	48		
		
}wsp_ContentTypeE;

typedef enum
{
	MMS_PRESENT_NOT_PRESENT = -1,
		MMS_PRESENT_SMIL    =   CT_X_APPL_SMIL,
		MMS_PRESENT_XHTML, /* T.B.D */
		MMS_PRESENT_WML,   /* T.B.D */
		
		MMS_PRESENT_NUMBER = 3
		
}mc_MmsPresentTypeE;

typedef enum
{
	MMS_ADDR_TYPE_NOT_PRESENT	= -1,
		MMS_ADDR_TYPE_PLMN      = 128,
		MMS_ADDR_TYPE_IPV4,
		MMS_ADDR_TYPE_IPV6,
		MMS_ADDR_TYPE_RFC822,
		MMS_ADDR_TYPE_NUMBER    = 4

}mc_MmsAddrTypeE;

typedef struct
{
    wsp_ContentTypeE    ctType;

    /* only for multipart\related */
    //u8                  type[30];
    //u8                  start[30];

}mc_MmsContentTypeS;

typedef struct
{
	/* add by steven 2004-07-26 for drm support */
	mc_DrmFlagE			drmFlag;						/* whether forward-lock is set */
	/* add by steven 2004-04-20 for attachment support */
	BYTE				bAttachment;					/* whether an attachment or slide object */	
	mc_MmsContentTypeS  contentType;                    /* Media object (attachment) 's type */
	/* add by pesalo 2004-09-28 for separate the concept of C-Location, C-ID and fileName */
	// 2004/10/29 after confirm with Steven, cLocation could be the same as filename 
	BYTE					cLocation[MC_MMS_MAX_CONTENT_LOCATION_LEN + 1];  /* c-location of object */
	// 2004/10/29 after confirm with Steven, cId could be empty if cLocation is given
	BYTE					cId[MC_MMS_MAX_CID_LEN + 1];       /* cid of object 100 */
	BYTE                  fileName[MC_MAX_FILENAME_LEN];  /* Media object (attachment) 's filename, not full one */
	
}mc_MmsEntryS;

typedef struct
{
	BYTE					bPresent;                           /* Whether presention language exist */
	mc_MmsPresentTypeE		presentType;                        /* indicate which language used for presentation */
	DWORD					presentEntryIdx;                    /* SMIL's index in aMmsEntry, calculate from 0 */
	DWORD					entryNum;                           /* Media object number */
	mc_MmsEntryS			aMmsEntry[ MMS_MAX_ENTRY_NUM ];     /* Info of each media object */
	
}mc_ListFileInfoS;

typedef struct
{
	BYTE					bPresent;                           /* Whether presention language exist */
	mc_MmsPresentTypeE		presentType;                        /* indicate which language used for presentation */
	DWORD					presentEntryIdx;                    /* SMIL's index in aMmsEntry, calculate from 0 */
	DWORD					entryNum;                           /* Media object number */
	mc_MmsEntryS			aMmsEntry[ MMS_MAX_ENTRY_NUM-10 ];     /* Info of each media object */
	
}mc_ListFileInfo_Small_S;

typedef struct
{
	BYTE						bcc_num;
    benqdiag_mc_BccS			bcc[PIM_MC_MAX_BCC_NUM];
	BYTE						cc_num;
    benqdiag_mc_CcS				cc[PIM_MC_MAX_CC_NUM];
    benqdiag_mc_UrlS			xContentLocation;
//    s32							contentType;//
    benqdiag_mc_DateTimeS		date;
    BYTE						bDeliveryReport;   
    benqdiag_mc_FromS			from;
    mc_MmsMessageClassE			msgClass;   
//    s32				            xMsgType;//
//    u8							xVersion;//
    DWORD						msgSize;
    mc_MmsPriorityE				priority;
    BYTE		                bReadReport;
//    boolean						xReportAllowed;   
    BYTE						bSenderVisibility;
//    s32							xStatus;//
    benqdiag_mc_SubjectS		subject;
	BYTE						to_num;
    benqdiag_mc_ToS				to[PIM_MC_MAX_TO_NUM];

}mc_MmsHdrFieldS;

typedef struct
{
    mc_MmsHdrFieldS			mmsHdr;		/* MMS header info */    
    benqdiag_mc_UcsNameS	dirName;		/* dir the MMS message locates */
    BYTE					bPresent;		/* whether SMIL exist */	
//#ifdef MC_SUPPORT_ATTACHMENT
	BYTE					bAttachment;	/* whether attachment included */
//#endif

}mc_MmsInfoS;

/*================================================================
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

#endif //_PIM_DIAG_H_