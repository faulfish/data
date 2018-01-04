#ifndef NV_BENQ_H
#define NV_BENQ_H

#include "comdef.h"
#include "nv_items.h"
//#include "qw.h"

// BENQ, Rick Huang, 20040317, {
//#include "clk.h"
typedef PACKED struct {
  word year;            /* Year [1980..2100]                            */
  word month;           /* Month of year [1..12]                        */
  word day;             /* Day of month [1..31] or day of year [1..366] */
  word hour;            /* Hour of day [0..23]                          */
  word minute;          /* Minute of hour [0..59]                       */
  word second;          /* Second of minute [0..59]                     */
  word day_of_week;     /* Day of the week [0..6] Monday..Sunday        */
} _clk_julian_type_;
//#include "fs.h"
#define  FS_FILENAME_MAX_LENGTH_P   128
// } BENQ, Rick Huang, 20040317, {

/* Factory related nv item is moved to nv_factory.h */
#include "nv_factory.h"

/****************************************************************************/
/*  definitions of BENQ added NV items.                                     */
/****************************************************************************/

#define BENQ_NV_FS_FILENAME_MAX_LENGTH	128

/* Maximum phone number in a phone book record */
#define NV_MAX_PHONE_NUM            4

/* Maximum Dial Digits in BCD type. */
#define NV_MAX_DIAL_NUM             16 

/* Same as NV_MAX_DIAL_DIGITS in nv.h */
#define NV_MAX_DIAL_DIGIT           32
#define NV_DM_MAX_DIAL_DIGIT        10

/* max char in SCHDULER DATA */
#define NV_MAX_SCHEDULER_EVENT_LEN	2*NV_MAX_DIAL_DIGIT
#define NV_MAX_SCHEDULER_EVENT_LEN_V2		100

/* Same as NV_TEMP_TABLE_SIZ in nv.h */
#define NV_TEMP_TABLE_SIZ           8  /* size of all tables based on temperature */

/* Maxmum Name String. */
#define NV_MAX_NAME_STR             20

/* Max Call Log records in one Call Log Pool. */
#define MAX_CALLLOG_RECORDS         10

/* Max Call Log Type */
#define    MAX_CALLLOG_TYPES        3

#define BOOK_MAX_RECORDS_IN_RUIM    255
#define BOOK_MAX_RECORDS_IN_ME      500

#define NV_PB_RECORDS_SIZE          (BOOK_MAX_RECORDS_IN_ME/2)

/* Max Call Group */
#define MAX_CALL_GROUPS             20

/* Max Call Group Members */
#define NV_MAX_GROUP_MEMBERS        50
#define NV_DM_MAX_GROUP_MEMBERS      20

/* Max Emergency Calls. */
#define MAX_EMERGENCY_CALLS         3

/* Max Portrait number */
#define MAX_NUM_PORTRAIT            20
#define MAX_PORTRAIT_NAME_LENGTH    FS_FILENAME_MAX_LENGTH_P

#define MAX_SCHEDULER_RECORDS       25
#define MAX_SCHEDULER_RECORDS_V2    100
#define POWER_MANAGER_INDEX         0xF8

/* Define the status when mobile power-up for NV_POWERUP_STATUS_I. */
#define NORMAL_PWRUP_S              0    
#define RESET_PWRUP_S               1
#define RESET_PWROFFCHG_S           2
#define RESET_RTCFACTORYTEST_S      3

#define MAX_FIELD_LEN               1

/* Define the maximum e-mail length */
#define MAX_EMAIL_LEN               40
#define MAX_DM_EMAIL_LEN            10

#define BITSOFBYTE                  8
/* Length for PRL data version. */
#define NV_PRL_DATA_VERSION_LENGTH  16

/* Size of MIN matching table */
#define NV_MIN_MATCHING_TABLE_SIZE  15

/* Size of IMSI array */
#define NV_IMSI_SIZE               10

/* Max number of time stamps */
#define NV_MAX_TIMESTAMP_NUM        10

/* Length for PRI version. */
#define NV_PRI_VERSION_LENGTH  16

/* Length for BenQ file */
#define NV_MAX_FILE_LEN 39

/* Alarm clock num */
#define ALARM_CLOCK_NUM_5			5

/* The Remark field of a phone book record. Define the maximum length user can write.*/
#define BOOK_REMARK_LENGTH  100

/* BenQ NV item enum base */
#define NV_BENQ_BASE_ENUM			0x8000

/* BenQ NV major version */
#define NVM_BENQ_NV_MAJOR_VER       0x0004

/* BenQ NV group version*/
#define NVM_BENQ_AUDIO_VER          0x0003
#define NVM_BENQ_CALLLOG_VER		0x0002
#define NVM_BENQ_CS_SYS_SPC_VER		0x0002
#define NVM_BENQ_EMAIL_VER			0x0002
#define NVM_BENQ_FACTORY_VER		0x0001
#define NVM_BENQ_HWCONFIG_VER		0x0001
#define NVM_BENQ_PB0_VER			0x0003
#define NVM_BENQ_PB1_VER			0X0003
#define NVM_BENQ_RF_VER				0X0001
#define NVM_BENQ_SCH_VER			0x0001
#define NVM_BENQ_SMS_VER			0x0002
#define NVM_BENQ_TOOL_VER			0x0002
#define NVM_BENQ_WAP_VER			0x0001
#define NVM_BENQ_MMS_VER			0x0002
#define NVM_BENQ_BREW_VER			0x0001
#define NVM_BENQ_WMS_VER            0x0001

/* Call logs definition. */
#define CL_CURRENCEY_LEN  4

/* Phone Book definition */

typedef PACKED struct
{
	word			version;						/* NV BenQ Group Version: 0x0001--0xFFFF */
} nv_benq_version_type;

#define BENQ_PHONE_LOCK_CODE_SIZE	8

typedef PACKED struct nv_benq_phone_lock_code_struct
{
  byte	digits[BENQ_PHONE_LOCK_CODE_SIZE];
} nv_benq_phone_lock_code_s;


typedef PACKED struct
{
    byte            address;                        /* Storage address: 1--10 */
    byte            pi;                             /* Presentation Inication */
    byte            digitsNum;                      /* phone number length*/
    byte            digits[NV_MAX_DIAL_DIGIT];      /* BCD Phone Number */
    byte            lettersNum;                     /* name length*/
    byte            letters[NV_MAX_NAME_STR];       /* dialed number's name*/
    byte            numberType;                     /* Number Type */
    word            pbAddress;                      /* PhoneBook address */
    byte            timeNum;                        /*number of time stamps*/
    _clk_julian_type_ time[NV_MAX_TIMESTAMP_NUM];     /* Time/Date */
} CallInfoS;

typedef PACKED struct
{
    byte stack[MAX_CALLLOG_TYPES][MAX_CALLLOG_RECORDS]; /*stack for call logs*/
} CallLogCountS;

typedef PACKED struct nv_CLDisplayStruct
{
    boolean bShowTimerCallActive;
    boolean bShowTimerCallEnd;
    boolean bShowCostCallEnd;
} nv_CLDisplayS;

typedef PACKED struct nv_clCallCostStruct
{
    int nLastCallCost;
    int nAllCallCost;
    int CallCostUnit;
    int CallMaxCost;
    char szCurrency[CL_CURRENCEY_LEN];
} nv_clCallCostS;

typedef PACKED struct
{
//    byte usedMask[(MAX_PB_RECORDS / BITSOFBYTE) + (MAX_PB_RECORDS % BITSOFBYTE ? 1 : 0)];
    byte usedMask[128];	// we use maximum size to prevent from using #ifdef in this file
} pbControlListS;

typedef PACKED struct 
{
	word	address;		        /*record real location*/
    byte	lettersNum;		        /*phone name length*/
	byte	letters[NV_MAX_NAME_STR];	/*phone name*/
	byte	digitsNum[NV_MAX_PHONE_NUM];		/*phone number length*/
	byte	digits[NV_MAX_PHONE_NUM][NV_MAX_DIAL_DIGIT];/*phone number (ASCII code)*/
    byte    phoneType[NV_MAX_PHONE_NUM];     /*phone type*/
	byte	eMailNum;		        /*eMail length*/
	byte	eMail[MAX_EMAIL_LEN];  	/*eMail*/
    byte    groupType;              /*caller group type*/ 
    byte	numberUsed;				/*how many number used*/
	byte	codeFormat;     		/*letters code format (UNICODE / ASCII)*/
    byte    nPortrait;              /* The index of portrait */
    byte    nStarSign;              /* Star sign of this record */
    byte    nBloodtype;             /* Bloodtype of this record */
    byte    nDefaultNumber;         /* The index of the default phone number of this record */

} pbPhoneBookS;

typedef PACKED struct nv_pbPhoneBookVer2Struct
{
	word	address;		        /*record real location*/
    byte    inWhichBook;            /* which book this record is in */
    byte	lettersNum;		        /*phone name length*/
	byte	letters[NV_MAX_NAME_STR];			/*phone name*/
	byte	digitsNum[NV_MAX_PHONE_NUM];		/*phone number length*/
	byte	digits[NV_MAX_PHONE_NUM][NV_MAX_DIAL_DIGIT];/*phone number (ASCII code)*/
    byte    phoneType[NV_MAX_PHONE_NUM];		/*phone type*/
	byte	eMailNum;		        /*eMail length*/
	byte	eMail[MAX_EMAIL_LEN];  	/*eMail*/
    byte    groupType;              /*caller group type*/ 
    byte	numberUsed;				/*how many number used*/
	byte	codeFormat;     		/*letters code format (UNICODE / ASCII)*/
	byte    nImageID;
    byte    szImagePath[BENQ_NV_FS_FILENAME_MAX_LENGTH + 2];
	byte    nRingID;
    byte    szRingPath[BENQ_NV_FS_FILENAME_MAX_LENGTH + 2];
    byte    szRemark[BOOK_REMARK_LENGTH+1];
    byte    nDefaultNumber;         /* The index of the default phone number of this record */
	byte    nTONNPI[NV_MAX_PHONE_NUM];
} pbPhoneBookVer2S;

typedef PACKED struct 
{
	word	address;		        /*record real location*/
	byte	lettersNum;		        /*phone name length*/
	byte	letters[NV_MAX_NAME_STR];	/*phone name*/
    byte	digitsNum[NV_MAX_PHONE_NUM];		/*phone number length*/
	byte	digits[NV_MAX_PHONE_NUM][NV_DM_MAX_DIAL_DIGIT];/*phone number (ASCII code)*/
    byte    phoneType[NV_MAX_PHONE_NUM];     /*phone type*/
	byte	eMailNum;		        /*eMail length*/
	byte	eMail[MAX_DM_EMAIL_LEN];  	/*eMail*/
    byte    groupType;              /*caller group type*/ 
    byte	numberUsed;				/*how many number used*/
	byte	CodeFormat;     		/*letters code format (UNICODE / ASCII)*/
    byte    nPortrait;              /* The index of portrait */
    byte    nStarSign;              /* Star sign of this record */
    byte    nBloodtype;             /* Bloodtype of this record */
    byte    nDefaultNumber;         /* The index of the default phone number of this record */

} nv_pb_dm_type;


typedef PACKED struct
{
    word    address;        
    word    count;    /* The count that will use photographs */
    byte    portraitName[MAX_PORTRAIT_NAME_LENGTH];     /* The file names in WAP FILE STORAGE  and the 
                                                     value of WAP_MAX_PATH currently is 121*/
} nv_PortraitTableS;

typedef PACKED struct nv_pbPrivacyAcceptstruct
{	
    word    nAcceptList[BOOK_MAX_RECORDS_IN_ME + BOOK_MAX_RECORDS_IN_RUIM];
} nv_pbPrivacyListS;

typedef enum
{
    NO_ALARM,
    AUTO_POWER_ON,
    AUTO_POWER_OFF,
    SCHEDULER_ALARM,
    ALARM_CLOCK
} alarmIdS;

typedef PACKED struct
{
    byte cnt;   /* how many call group exsit */
    byte index[MAX_CALL_GROUPS];
} pbCallGroupIndexS;

typedef PACKED struct 
{
    byte    address;
    byte    nameNum;                /* group name length */
    byte    name[NV_MAX_NAME_STR];     /* group name */
    word    memberNum;              /* number of members */
    word    member[NV_MAX_GROUP_MEMBERS];  /* group members */
    byte    ringTone;               /* group ring tone */    
    byte    picture;              /* calling picture */
    byte    LEDPattern;             /* LED pattern */
} pbCallGroupS;

typedef PACKED struct 
{
    byte    address;
    byte    nameNum;                /* group name length */
    byte    name[NV_MAX_NAME_STR];     /* group name */
    word    memberNum;              /* number of members */
    word    member[NV_MAX_GROUP_MEMBERS];  /* group members */
    byte    ringTone;               /* group ring tone */    
    byte    picture;              /* calling picture */
    byte    LEDPattern;             /* LED pattern */
    byte    midiFile[NV_MAX_FILE_LEN];
} pbCallGroupV2S; /* "2" means "version 2" */

typedef PACKED struct 
{
    byte    address;
    byte    nameNum;                /* group name length */
    byte    name[NV_MAX_NAME_STR];     /* group name */
    word    memberNum;              /* number of members */
    word    member[NV_MAX_GROUP_MEMBERS];  /* group members */
    byte    nRingID;               /* group ring tone */  
	byte    szRingPath[BENQ_NV_FS_FILENAME_MAX_LENGTH + 2];
    byte    nImageID;              /* calling picture */
	byte    szImagePath[BENQ_NV_FS_FILENAME_MAX_LENGTH + 2];
    byte    LEDPattern;             /* LED pattern */
} pbCallGroupV3S; /* "3" means "version 3" */

typedef PACKED struct 
{
    byte    address;
    byte    nameNum;                /* group name length */
    byte    name[NV_MAX_NAME_STR];     /* group name */
    word    memberNum;              /* number of members */
    word    member[NV_DM_MAX_GROUP_MEMBERS];  /* group members */
    byte    ringTone;               /* group ring tone */    
    byte    picture;              /* calling picture */
    byte    LEDPattern;             /* LED pattern */
} nv_group_dm_type;

typedef PACKED struct
{
    alarmIdS    alarmId;
    byte        alarmIndex;
} nv_alarm_type_s;

typedef PACKED struct
{
    byte cnt;   /* how many scheduler exsit */
    byte index[MAX_SCHEDULER_RECORDS]; 
} nv_scheduler_index_s;

typedef PACKED struct
{
    byte cnt;   /* how many scheduler exsit */
    byte index[MAX_SCHEDULER_RECORDS_V2]; 
} nv_scheduler_index_V2_s;

typedef PACKED struct
{
    byte cnt;   /* how many scheduler exsit */
    byte deleteafterType;  /* record the type of deleteafter */
    byte index[MAX_SCHEDULER_RECORDS_V2]; 
} nv_scheduler_index_V3_s;

typedef PACKED struct
{
    byte    adress;
    byte    status;
    byte    curProfileID;
    dword   alarmTime;  /* Unit: minutes. The offset is since the base date. */
    byte    alarmType;
    byte    alarmoffset[4];
    byte    data[NV_MAX_SCHEDULER_EVENT_LEN+1];  /* the scheduling data , note or a call number */
	dword   originalTime;
} nv_scheduler_s;

typedef PACKED struct
{
    byte    adress;
    byte    status;
	dword   originalTime;
	dword	endTime;
    byte    curProfileID;
    dword   alarmTime;  /* Unit: minutes. The offset is since the base date. */
    byte    reminderState;   /*record the reminder status*/
    byte    alarmoffset[5];
    byte    data[NV_MAX_SCHEDULER_EVENT_LEN_V2+1];  /* the scheduling data , note or a call number */
	byte	subject[NV_MAX_SCHEDULER_EVENT_LEN_V2+1];
	byte	location[NV_MAX_SCHEDULER_EVENT_LEN_V2+1];
	byte	attendee[NV_MAX_SCHEDULER_EVENT_LEN_V2+1];
} nv_scheduler_V2_s;

typedef PACKED struct
{
    byte    status;             /* 0--3 type , 4-7 valid */
    dword   powerTimeOffset;    /* Unit: minutes. The offset is since the base date. */
} nv_pwr_manager_s;

typedef PACKED struct 
{
    byte powerOnStatus;
    byte powerOffStatus;
} nv_benq_setting_power_ani_status_s;

typedef PACKED struct 
{
    boolean powerSavingMode;
} nv_benq_setting_power_saving_s;

typedef PACKED struct 
{
    byte memoryFirst;
} nv_benq_setting_memory_first_s;

typedef PACKED struct 
{
    boolean pa_dvs_enable;
    word    pa_dvs_volt;
} nv_benq_pa_dvs_conf_type;

/* Emergency Calls Record. */
typedef PACKED struct
{
    byte digits[MAX_EMERGENCY_CALLS][NV_MAX_DIAL_DIGIT+1];
} EmergencyCallS;

/* The max length of message header */
#define UI_SMS_SIGNATURE_SIZ 12

typedef PACKED struct
{
    boolean status;
    /* TRUE:Enable, FALSE:Disable */
    char    chari[UI_SMS_SIGNATURE_SIZ+1];
    /* Buffer for header text */
} nv_smSignatureTypeS;

/* PRL data version */
typedef PACKED struct
{
    byte data[NV_PRL_DATA_VERSION_LENGTH];
} nv_PrlDataVersionS;

/* SMS settings */
typedef PACKED struct
{
    byte msgAlert;
    byte saveMode;
    byte expiryPeriod;
    char cbNumber[NV_MAX_DIAL_DIGIT+1];
    byte entryMode;
    byte alertType;
    byte deliveryAck;
    byte autoErase;
    byte usageWarning;
} nv_smSettingsS;

typedef PACKED struct
{
    byte address; 
    byte currentId;  
    word menu_title_fg;
    word menu_title_bg;

    word menu_item_fg;
    word menu_item_bg;
    word menu_selected_item_fg;
    word menu_selected_item_bg;

    word menu_index_fg;
    word menu_index_bg;
    word menu_selected_index_fg;
    word menu_selected_index_bg;

    word menu_prompt_fg;
    word menu_prompt_bg;
    word menu_selected_prompt_fg;
    word menu_selected_prompt_bg;

    word text_fg;
    word text_bg;
    word text_selected_fg;
    word text_selected_bg;

    word time_date_fg;
    word time_date_bg;

    word text_menuList_fg;
    word text_menuList_bg;

    word calen_fg;
    word calen_bg;
    word calen_selected_fg;
    word calen_selected_bg;

    word text_option_fg;
    word text_option_bg;

    word text_error_fg;
    word text_error_bg;

    word text_info_fg;
    word text_info_bg;

    word softkey_fg;
    word softkey_bg;
    word softkey_selected_fg;
    word softkey_selected_bg;

    word fg_color;
    word bg_color;
} nv_ColorSettingsS;


typedef  PACKED struct
{
    byte powerOnOffSound;
} nv_benq_setting_power_sound_status_s;


/* the maximum outbox messages */
#define MAX_NV_OUTBOX_MSGS  1

/* Maximum destination addresses that a multi-addr nv item can save */
#define MAX_MULTI_ADDR_NUM  10

/* Maximum benq_sms messages */
#define MAX_BENQ_SMS_MSGS   47

/* The maximum multi-address nv items */
// with FEATURE_RUIM_SMS
//#define UI_SMS_MAX_MULTI_ADDR_ITEMS_RUIM   14
// without FEATURE_RUIM_SMS
#define UI_SMS_MAX_MULTI_ADDR_ITEMS        24

/* SMS multi-address */
typedef PACKED struct
{
    byte address;                                       /* max SMS outbox messages */
    byte rel_nv_idx;                                    /* related to which MO SMS NV item index */
    byte count;                                         /* how many destination addresses */
    word msgid[MAX_MULTI_ADDR_NUM];                     /* message ID */
    word flag[MAX_MULTI_ADDR_NUM];                      /* message status flag */
    char destAddr[MAX_MULTI_ADDR_NUM][MAX_EMAIL_LEN+1]; /* destination addresses */
} nv_smsMultiAddrS;

/* SMS dm multi-address */
typedef PACKED struct
{
    byte address;                                       /* max SMS outbox messages */
    byte rel_nv_idx;                                    /* related to which MO SMS NV item index */
    byte count;                                         /* how many destination addresses */
    word msgid[MAX_MULTI_ADDR_NUM];                     /* message ID */
    word flag[MAX_MULTI_ADDR_NUM];                      /* message status flag */
    char destAddr[MAX_MULTI_ADDR_NUM][MAX_EMAIL_LEN+1]; /* destination addresses */
} nv_dm_smsMultiAddrS;

/* Quick Message */
#define UI_QUICK_MSG_MAX    10
#define UI_QUICK_MSG_LENGTH 50
typedef PACKED struct
{
    byte address;                   /* 0-9 : max quick messages */
    char msg[UI_QUICK_MSG_LENGTH];  /* 20 characters can be saved in each quick message */
} nv_QuickMsgS;

/* Chat room */
#define MAX_CHAT_ROOM_SIZE 320  /* same as TTL_INPUT_LEN for SMS_SCROLL */
typedef PACKED struct
{
    char msg[MAX_CHAT_ROOM_SIZE];  /* content of chat room */
} nv_ChatRoomS;

typedef PACKED struct
{
    byte number[NV_MAX_DIAL_DIGIT];
} nv_ChatNumS;

#define NICKNAME_MAX_COUNT  3   /* max nickname can be saved */
#define NV_NICKNAME_MAX_LEN 20  /* max nickname length */

typedef PACKED struct
{
    boolean online;         /* chatting is over or not */
    byte    currentName;    /* current using nickname */
    char    name[NICKNAME_MAX_COUNT][NV_NICKNAME_MAX_LEN];
} nv_NicknameS;

/* Voice mail number */
typedef PACKED struct
{
    byte number[NV_MAX_DIAL_DIGIT];
} nv_VMailNumS;

/* EMS downloaded image */
#define NV_MAX_EMS_IMAGE_SAVE 3     /* max number of images can be saved */
#define NV_MAX_EMS_IMAGE_SIZE 128   /* max image data size */
#define UISM_EMS_INBOX_MAX		5
#define UISM_EMS_OUTBOX_MAX		5

typedef PACKED struct
{
    byte count;                         /* how many images were saved */
    byte type[NV_MAX_EMS_IMAGE_SAVE];   /* image type */
    byte data[NV_MAX_EMS_IMAGE_SAVE][NV_MAX_EMS_IMAGE_SIZE];    /* image data */
} nv_EmsImageS;


/* EMS control list */
typedef PACKED struct
{
	char EMSInboxList[UISM_EMS_INBOX_MAX][FS_FILENAME_MAX_LENGTH_P];
	boolean EMSMTread[UISM_EMS_INBOX_MAX];
	byte EMSInboxCnt;
	char EMSOutboxList[UISM_EMS_OUTBOX_MAX][FS_FILENAME_MAX_LENGTH_P];
	byte EMSMOID[UISM_EMS_OUTBOX_MAX];	
	byte EMSOutboxCnt;
} nv_emslistS;

#define UISM_CONCATE_DATA_MAX_LEN	130	
#define UISM_MAX_SMS_CONCATE_COUNT	3	

/* SMS object download MO */
typedef PACKED struct
{
	byte	DLmax;	    /* The max number of the concatenate message */
    byte    DLcount;    /* Record the number sms need sent */
    word    DLmsgid[UISM_MAX_SMS_CONCATE_COUNT];
    word    DLflag[UISM_MAX_SMS_CONCATE_COUNT];
    char    DLdestAddr[NV_MAX_DIAL_DIGIT+1];
	byte	DLdestAddrLen;
} nv_smsDLMOS;

typedef PACKED struct 
{
    dword second;   /* total handset on time in second*/
} nv_onTimerS;

typedef PACKED struct 
{
    short int serviceCount; /* Failure Byte Identifier */
} nv_serviceCountS;

typedef PACKED struct 
{
    dword flexStatus;   /* flex bit status */
} nv_flexStatusS;

typedef PACKED struct 
{
    byte languageType;  /* language type */
} nv_languageTypeS;

typedef PACKED struct 
{
    word minMatchingTable[NV_MIN_MATCHING_TABLE_SIZE];  /* MIN Matching Table */
} nv_minMatchingTableS;

typedef PACKED struct 
{
    short int checkCount;   /* Failure Byte Identifier */
} nv_otslCheckCountS;

typedef PACKED struct 
{
    byte actItem;       /* whether activate this service */
    byte alarmRingType; /* alarm ring type */
    word timeOffset;    /* alarm clock time */
} nv_alarmClockS;

typedef PACKED struct 
{
	byte address;
    byte actItem;       /* whether activate this service */
    byte alarmRingType; /* alarm ring type */
    word timeOffset;    /* alarm clock time */
} nv_alarmClockSetS;

typedef PACKED struct 
{
	byte address;
    byte actItem;       /* whether activate this service */
	byte ringingType;     /* alarm ringing type */
	uint8   ringingFullPath[BENQ_NV_FS_FILENAME_MAX_LENGTH + 2];	 /* alarm ringing full path for file */
	byte timeDuration;   /* alarm time duration */
    byte alarmRingType; /* alarm repeat type */
    word timeOffset;    /* alarm clock time */
} nv_alarmClockSetS_V2;

#define NV_MAX_SPEED_DIAL_NUM   9
typedef PACKED struct 
{
    boolean activate;                       /* whether activate this service */
    word    pbAddress[NV_MAX_SPEED_DIAL_NUM];   /* PhoneBook Address if any */
    byte    numberLen[NV_MAX_SPEED_DIAL_NUM];   /* the length of number */
    byte    number[NV_MAX_SPEED_DIAL_NUM][NV_MAX_DIAL_DIGIT+1]; /* the phone numbers */
    
} nv_SpeedDialS;

typedef PACKED struct 
{
    boolean isIMSILock;
    byte    imsi_m[NV_IMSI_SIZE];  /* the array to store IMSI_M of the last session */
} nv_ImsiS;

typedef PACKED struct
{
    byte date_format;
    byte time_format;
} nv_DateTimeFormatS;

/* Date/Time/Day show in idle screen */
typedef PACKED struct
{
    boolean showDate;
    boolean showTime;
    boolean showDay;
	boolean showDigitalClock;
} nv_showDateTimeDayS;

typedef PACKED struct
{
    byte dummy;
} nv_reserved_s;

#define MAX_REG_NUM 8       /* if support 256 color, palette will be 8R,8G,4B */
#define MAX_ELEMENT_NUM 3   /* if support color, element will have 3 (R,G,B) */
typedef enum
{
    LCDD_PALETTE_GRAY,
    LCDD_PALETTE_RED,
    LCDD_PALETTE_GREEN,
    LCDD_PALETTE_BLUE,
    NUM_OF_PALETTE_REG_TYPE
} lcdPaletteRegTypeE;

typedef PACKED struct
{
    lcdPaletteRegTypeE type;
    byte num;
    byte value[MAX_REG_NUM];
} lcdPaletteRegDataS;

typedef PACKED struct
{
    lcdPaletteRegDataS data[MAX_ELEMENT_NUM];
} lcdPaletteS;

/* Audio Profile */
#define PROFILE_MAX_NUM_NV  7

typedef PACKED struct
{
    byte            address;              /* 0-7 : max audio profile */
    byte            letters_num;
    byte            letter[NV_MAX_NAME_STR];
    byte            vibrate_mode;
    byte            call_ring_type;
    byte            sms_ring_type;
    byte            alarm_ring_type;
    byte            sch_ring_type;
    byte            greeting_ring_type;  /* new item for Q600 */
    byte            ring_volume;
    byte            key_beep_method;
    byte            key_beep_volume;
    boolean         key_beep_warning_tone;
    word            system_alert;
    boolean         answer_by_flip_activate;
    boolean         answer_by_auto;
    byte            answer_by_keypad_mode;
    boolean         dtmf_mute;
    byte            dtmf_duration;
    boolean         increasing_ring;     /* new item for Q600 */
    byte            mode_LED;            /* new item for Q600 */
} nv_audioProfileRecS;

typedef PACKED struct
{
    byte            address;              /* 0-7 : max audio profile */
    byte            letters_num;
    byte            letter[NV_MAX_NAME_STR];
    byte            vibrate_mode;
    byte            call_ring_type;
    byte            sms_ring_type;
    byte            alarm_ring_type;
    byte            sch_ring_type;
    byte            greeting_ring_type;  /* new item for Q600 */
    byte            ring_volume;
    byte            key_beep_method;
    byte            key_beep_volume;
    boolean         key_beep_warning_tone;
    word            system_alert;
    boolean         answer_by_flip_activate;
    boolean         answer_by_auto;
    byte            answer_by_keypad_mode;
    boolean         dtmf_mute;
    byte            dtmf_duration;
    boolean         increasing_ring;     /* new item for Q600 */
    byte            mode_LED;            /* new item for Q600 */
    byte            midiFile[NV_MAX_FILE_LEN];
} nv_audioProfileRec2S;		// "2" means "version 2"

typedef PACKED struct
{
    byte            address;              /* 0-7 : max audio profile */
    byte            letters_num;
    byte            letter[NV_MAX_NAME_STR];
    byte            vibrate_mode;
    byte            call_ring_type;//;nCallRingID
    byte            sms_ring_type;//;nMessageRingID
    byte            alarm_ring_type;
    byte            sch_ring_type;//;nCalendarRingID
    byte            greeting_ring_type;  /* new item for Q600 */
    byte            ring_volume;
    byte            key_beep_method;
    byte            key_beep_volume;
    boolean         key_beep_warning_tone;
    word            system_alert;
    boolean         answer_by_flip_activate;
    boolean         answer_by_auto;
    byte            answer_by_keypad_mode;
    boolean         dtmf_mute;
    byte            dtmf_duration;
    boolean         increasing_ring;     /* new item for Q600 */
    byte            mode_LED;            /* new item for Q600 */
    byte            szCallRingPath[NV_MAX_FILE_LEN];
    byte            szMessageRingPath[NV_MAX_FILE_LEN];
    byte            szCalendarRingPath[NV_MAX_FILE_LEN];
    //byte            midiFile[NV_MAX_FILE_LEN];
} nv_audioProfileRec3S;		// "3" means "version 3" For Ulysses Project

/* init input method and case*/
typedef PACKED struct
{
    byte sms_content_mthd;
    byte sms_content_case;
    byte sch_input_mthd;
    byte sch_input_case;
    byte pb_memo_mthd;
    byte pb_memo_case;
    byte mms_subject_mthd;
    byte mms_subject_case;
    byte mms_content_mthd;
    byte mms_content_case;
} nv_initMethodCaseS;

/* Q-key */
#define MAX_QKEY_NUM  9
typedef PACKED struct
{
    byte    qkey_id[MAX_QKEY_NUM];
} nv_qkeyS;

/* World Time */
#define MAX_TIMEZONE_NUM  25
#define MAX_TIMECLOCK_SHOW_NUM  6

typedef PACKED struct
{
    byte    wtime_id[MAX_TIMEZONE_NUM];
} nv_wtimeS;

typedef PACKED struct
{
	byte	record_num;
    byte    wclock_id[MAX_TIMECLOCK_SHOW_NUM];
} nv_wClockS;

typedef PACKED struct 
{
    byte dateTimeFormat;
} nv_benq_setting_datetime_format_s;

/* currency Converter */
#define NUM_OF_CURRENCY 18
#define NUM_OF_CURRENCY_2		15

typedef PACKED struct
{
    byte          local;
    byte          foreign;
    dword         exchangeRate[NUM_OF_CURRENCY];	
} nv_Currency_Converter_s;

typedef PACKED struct
{
	byte			tempCurrency[8];
    byte			local;
    byte			foreign;
    dword         localRate;
    dword         foreignRate;	
} nv_Currency_Converter_V2_s;

typedef PACKED struct
{
	byte			tempCurrency[8];
    byte			local;
    byte			foreign;
} nv_CurrencyConverterName_s;

typedef PACKED struct
{
	byte            address;	/* 0-14 */
    dword			exchangeRate[NUM_OF_CURRENCY_2];	
} nv_CurrencyConverterRate_s;

/* Stopwatch */
typedef enum
{
    STOPWATCH_RESET,
    STOPWATCH_RUNNING,
    STOPWATCH_STOP,
    STOPWATCH_SPLIT_STOP,
    STOPWATCH_SPLIT_RUNNING
} StopWatchStateE;

typedef PACKED struct
{
    byte hour;
    byte minute;
    byte second;
    byte cs;   // 0.01s 
} StopwatchTimeS;

typedef PACKED struct
{
    StopWatchStateE state;
    unsigned long   startTime;
    StopwatchTimeS	splitTime;
    StopwatchTimeS  stopTime;
} nv_stopwatch_s;

/* define all kinds of games*/
typedef enum
{
     SNAKE,
     DIGIT_SORT,
     BRICK,
     FIVESTONE,
     BOXWORLD
} uigame_KindOfGameE;

#define UIGAME_NUM_OF_GAMES 5

typedef PACKED struct
{
    word    TopScore[UIGAME_NUM_OF_GAMES];
	byte    level[UIGAME_NUM_OF_GAMES];
} nv_game_s;

/* Email max inbox size */
#define NV_MAX_EML_INBOX_SIZE       30
/* Email max trash size */
#define NV_MAX_EML_TRASH_SIZE       5
/* Email max draft size */
#define NV_MAX_EML_DRAFT_SIZE       3
/* Email max outbox size */
#define NV_MAX_EML_OUTBOX_SIZE      3
/* Email max string length */
#define NV_MAX_EML_STR_LEN          80
/* Email max recipients */
#define NV_MAX_EML_RECIPIENTS       10

/* Email Mail Info Struct */
typedef PACKED struct
{
    byte        address;
    byte        fileIndex;                      /* file index */
    byte        uid[NV_MAX_EML_STR_LEN];        /* uid */
    uint32      nSize;                          /* index */
    byte        sender[NV_MAX_EML_STR_LEN];     /* sender */
    byte        subject[NV_MAX_EML_STR_LEN];    /* subject */
    byte        date[NV_MAX_EML_STR_LEN];       /* date */
    boolean     mailIncomplete;                 /* mail trimed */
    byte        mailStatus;                     /* email status */
} nv_emlMailInfoS;

/* Email Compose Info Struct */
typedef PACKED struct
{
    byte        address;
    byte        fileIndex;                                      /* file index */
    byte        to[NV_MAX_EML_RECIPIENTS][MAX_EMAIL_LEN];       /* recipients */
    byte        subject[NV_MAX_EML_STR_LEN];                    /* subject */
    byte        num_Of_Recipient;                               /* number of recipients */
    boolean     isForwardMail;                                  /* is forward mail */
    uint32      composedLenWithoutFwMail;                       /* composed length - without forward mail */
    uint32      composedLenWithoutSignature;                    /* composed length - without signature */
} nv_emlComposeInfoS;

/* Email User Profiles Struct */
typedef PACKED struct
{
    byte        User[NV_MAX_EML_STR_LEN];
    byte        Pass[NV_MAX_EML_STR_LEN];
    byte        SmtpServer[NV_MAX_EML_STR_LEN];
    byte        PopServer[NV_MAX_EML_STR_LEN];
    byte        EmailAddress[MAX_EMAIL_LEN];
    byte        Signature[NV_MAX_EML_STR_LEN];
    byte        KeepMail;
} nv_emlUserProfileS;

/* Email Count */
typedef PACKED struct
{
    byte numInboxMails;
    byte numTrashMails;
    byte numDraftMails;
    byte numOutboxMails;
} nv_emlMailCountS;

#define NV_MELODY_MAX_COUNT  4  /* max melody name can be saved */
#define MELODYNAME_MAX_LEN 20    /* max melody name length */
typedef PACKED struct
{
    char melodyName[NV_MELODY_MAX_COUNT][MELODYNAME_MAX_LEN + 1];
} nv_MelodyNameS;

#define MAX_RING_NUMBER 80       /* max ring number of melody */
typedef PACKED struct
{
    byte Note_Data[640];
    word NoteDataLen[20];
    word MeasureStartPos[20];   
    word NoteLen;    
    byte MeasureNum;
    byte InstrumentID;
    byte Volume;
    byte Ch_Num;
}MelodyTrackInfoS;
 
typedef PACKED struct 
{
    byte	address;
    byte  AccompanyID;
    byte  ChordId[20];
    byte  ChordLen;
    MelodyTrackInfoS  Melody_MainMelodyTrack;
}nv_MelodyInfoS;

#define NV_MAX_HAIER_SVC_NUM    34
#define NV_DM_MAX_HAIER_SVC_NUM  5
typedef PACKED struct
{
    int  defaultNum;
    byte number[NV_MAX_HAIER_SVC_NUM][NV_MAX_DIAL_DIGIT];
} nv_HaierSvcNumS;

typedef PACKED struct
{
    int  defaultNum;
    byte number[1][NV_MAX_DIAL_DIGIT];
} nv_Dummy_HaierSvcNumS;

/* PRI version */
typedef PACKED struct
{
    byte data[NV_PRI_VERSION_LENGTH];
} nv_PriVersionS;

#define WAP_PERMANENT_SIZE              342

#define WAP_BEARER_LIST_SIZE            1500

/* device layer supported access point type length */
#define WAP_MAX_APTYPE_LEN              4
/* device layer supported access point address length */
#define WAP_MAX_APADDR_LEN              21
/* device layer supported access point user name length */
#define WAP_MAX_APUSER_LEN              32
/* device layer supported access point user password length */
#define WAP_MAX_APPSWD_LEN              32
/* device layer supported access point script length */
#define WAP_MAX_APSCRIPT_LEN            200
/* device layer wdp address length */
#define WAP_MAX_WDPADDR_LEN             20
/* device layer supported bearer name len */
#define WAP_MAX_BEARNAME_LEN            16
/* device layer supported access point numbers per bearer */
#define WAP_MAX_AP_NUMS                 2
/* device layer supported bearers per proxy */
#define WAP_MAX_BEARER_NUMS             3
/* device layer supported bearer addresses per proxy */
#define WAP_MAX_WDPADDR_NUMS            3
/* device layer supported proxy numbers */
#define WAP_MAX_PROXY_NUMS              3
/* device layer supported number of sid in wap_sid_list */
#define WAP_MAX_SID_LIST_NUMS           10 
/* device layer encryption key length */
#define WAP_MAX_KEY_LEN                 48
/* length for MMC object:autosia	  */
#define WAP_AUTOSIA_LEN					2
/* length for MMC object:autosl 	  */
#define WAP_AUTOSL_LEN					7
//WAP61
#define WAP_SIZE_OF_HOMEPAGE_URL        1024
// length for MMC object: boot.url
#define WAP_SIZE_OF_BOOT_URL            128
// length for MMC object: homepage.user.default
#define WAP_SIZE_OF_HOMEPAGE_USER_DEFAULT 64
// length for MMC object: busy.yield.interval
#define WAP_SIZE_OF_BUSY_YIELD_INTERVAL 21
// length for MMC object: date.format
#define WAP_SIZE_OF_DATE_FORMAT         16
// length for MMC object: proxy.certificate
#define WAP_SIZE_OF_PROXY_CERTIFICATE   700
// length for MMC object: psa.version
#define WAP_SIZE_OF_PSA_VERSION         65
// length for MMC object: proxy.bearer.ap.authtype
#define WAP_SIZE_OF_AUTHTYPE_LEN        5
// length for MMC object: proxy.bearer.maxrequesttime
#define WAP_SIZE_OF_MAXREQUESTTIME      11
// length for MMC object: cdma.is683
#define WAP_SIZE_OF_CDMA_IS683         64 * 1024
// length for MMC object: current.username
#define WAP_SIZE_OF_CURRENT_USER        72
// length for MMC object: current.username
#define WAP_SIZE_OF_CURRENT_PASS        16
// length for MMC object: cdma.prl
// According to MMC document, this value should be set to 64K
// Since China Unicom didn't support this feature, we reduce to 
// 512
#define WAP_SIZE_OF_CDMA_PRL            512

typedef PACKED struct 
{
    byte dummy;
} wap_dm_permanent_storageS;

typedef PACKED struct 
{
    byte dummy;
} wap_dm_UserDataS;

typedef PACKED struct 
{
    byte dummy;
} wap_sid_listS;

typedef PACKED struct 
{
    byte dummy;
} wap_dm_ProxyKeyS;

typedef PACKED struct
{
    char wap_Homepage_URL[WAP_SIZE_OF_HOMEPAGE_URL];
} wap_Homepage_URLS;

typedef PACKED struct
{
    boolean lock;
    char wap_Download_URL[WAP_SIZE_OF_HOMEPAGE_URL];
} wap_Download_URLS;

typedef PACKED struct
{
    byte dummy;
} wap_dm_Bearer_ListS;

typedef PACKED struct
{
	byte dummy[512];
} wap_dm_PushObjectS;

typedef struct wap_AccessPointTag
{
    unsigned char                   typeLen;
    unsigned char                   addrLen;
    unsigned char                   userLen;
    unsigned char                   pswdLen;
    unsigned char                   type[WAP_MAX_APTYPE_LEN];
    unsigned char                   addr[WAP_MAX_APADDR_LEN];
    unsigned char                   user[WAP_MAX_APUSER_LEN];
    unsigned char                   pswd[WAP_MAX_APPSWD_LEN];
    unsigned char                   auth[WAP_SIZE_OF_AUTHTYPE_LEN];
    unsigned char                   scriptLen;
    unsigned char                   script[WAP_MAX_APSCRIPT_LEN];
} wap_AccessPointS;

typedef struct wap_WDPAddressTag
{
    unsigned char                   bearerAddrLen;
    unsigned char                   bearerAddress[WAP_MAX_WDPADDR_LEN];
} wap_WDPAddressS;

typedef struct wap_BearerDataTag
{
    unsigned char                   numAddr;
    unsigned char                   nameLen;
    unsigned char                   name[WAP_MAX_BEARNAME_LEN];
    wap_AccessPointS                accessPoint[WAP_MAX_AP_NUMS];
    unsigned char                   MaxRequestTime[WAP_SIZE_OF_MAXREQUESTTIME];
} wap_BearerDataS;

typedef struct wap_ProxyInfoTag
{
    unsigned char                   numAddr;
    wap_BearerDataS                 bearer[WAP_MAX_BEARER_NUMS];
    wap_WDPAddressS                 addr[WAP_MAX_WDPADDR_NUMS];	
} wap_ProxyInfoS;

typedef struct wap_UserDataTag
{
    wap_ProxyInfoS                  proxy[WAP_MAX_PROXY_NUMS];
} wap_UserDataS;

typedef struct wap_SidListTag	
{
    unsigned char                   sidLen;                           
    unsigned short                  sidlist[WAP_MAX_SID_LIST_NUMS];   
} wap_SidListS;

typedef PACKED struct wap_KeyTag	/* 684 */
{
    unsigned char   keyLen;
    unsigned char   key[WAP_MAX_KEY_LEN];
} wap_KeyS;

typedef PACKED struct wap_ProxyKeyTag
{
    wap_KeyS        keys[WAP_MAX_PROXY_NUMS];
} wap_ProxyKeyS;

typedef PACKED struct wap_Proxy_IPTag
{
    unsigned char   ip[4];
	unsigned short  port;
} wap_Proxy_IPTags;

typedef PACKED struct wap_ProxyAddrTag
{
    wap_Proxy_IPTags proxy_ip[2];
} wap_ProxyAddrTagS;


typedef struct wap_PushObject
{
	unsigned char autoslLen;
	unsigned char autosia[WAP_MAX_PROXY_NUMS][WAP_AUTOSIA_LEN];
	unsigned char autosl[WAP_AUTOSL_LEN];
} wap_PushObejctS;

//WAP61
typedef PACKED struct wap_BootUrl
{
	unsigned char boot_url[WAP_SIZE_OF_BOOT_URL];
} wap_BootUrlS;

typedef PACKED struct wap_DateFormat
{
	unsigned char date_format[WAP_SIZE_OF_DATE_FORMAT];
} wap_DateFormatS;

typedef PACKED struct wap_ProxyCertificate
{
	byte proxy_certificate[WAP_SIZE_OF_PROXY_CERTIFICATE];
    unsigned long ProxyCert_Len;
} wap_ProxyCertificateS;

typedef PACKED struct wap_CurrentUser
{
	unsigned char wap_current_username[WAP_SIZE_OF_CURRENT_USER];
} wap_CurrentUserS;

typedef PACKED struct wap_CurrentPass
{
	unsigned char wap_current_password[WAP_SIZE_OF_CURRENT_PASS];
} wap_CurrentPassS;

typedef PACKED struct wap_PsaVersion
{
	unsigned char wap_psa_version[WAP_SIZE_OF_PSA_VERSION];
} wap_PsaVersionS;

/*---------------------------------------------------------------------------
                     MMS Data Structure
---------------------------------------------------------------------------*/
#define UI_MMS_EDT_TEMPLATE_TEXT_MAX_LEN   5

#define UI_MMS_SET_SIGNATURE_MAX_LEN     40
#define UI_MMS_SET_BLACKLIST_CNT         5
#define UI_MMS_SET_BLACKLIST_MAX_LEN     128
#define UI_MMS_SET_SERVER_CNT            5
#define UI_MMS_SET_SERVER_NAME_MAX_LEN   20
#define UI_MMS_SET_HOMEPAGE_URL_MAX_LEN  WAP_SIZE_OF_HOMEPAGE_URL  
#define UI_MMS_SET_DATA_CALL_MAX_LEN     WAP_MAX_APADDR_LEN  
#define UI_MMS_SET_USER_NAME_MAX_LEN     WAP_MAX_APUSER_LEN  
#define UI_MMS_SET_PASSWD_MAX_LEN        WAP_MAX_APPSWD_LEN  
#define UI_MMS_SET_REJECT_LIST_MAX_CNT	 10
#define UI_MMS_SET_MAX_ADDR				 50


typedef PACKED struct nv_mms_SET_RejectListStruct
{
	byte count;
	char addr[UI_MMS_SET_REJECT_LIST_MAX_CNT][UI_MMS_SET_MAX_ADDR];
	char name[UI_MMS_SET_REJECT_LIST_MAX_CNT][NV_MAX_NAME_STR];
} nv_mms_SET_RejectListS;

typedef PACKED struct nv_mms_SET_SignatureStruct
{
	boolean bStatus;               
	char    szChari[UI_MMS_SET_SIGNATURE_MAX_LEN+1];
} nv_mms_SET_SignatureS;

typedef PACKED struct nv_mms_SET_BlackListStruct
{
	char       szBlacklist[UI_MMS_SET_BLACKLIST_CNT][UI_MMS_SET_BLACKLIST_MAX_LEN+1];
    byte       nCnt;
    byte       nArrayIdx[UI_MMS_SET_BLACKLIST_CNT];
} nv_mms_SET_BlackListS;

typedef PACKED struct nv_mms_SET_FilterStruct
{
	boolean    bAdvertisement;               
	boolean    bAnonymous;
    nv_mms_SET_BlackListS  sBlackList;	
} nv_mms_SET_FilterS;

typedef PACKED struct nv_mms_SET_AccessPointStruct
{
	char    szCallNumber[UI_MMS_SET_DATA_CALL_MAX_LEN+1];               
	char    szUserName[UI_MMS_SET_USER_NAME_MAX_LEN+1];
	char    szPasswd[UI_MMS_SET_PASSWD_MAX_LEN+1];
} nv_mms_SET_AccessPointS;

typedef PACKED struct nv_mms_SET_ServerSettingStruct
{
    byte                             address;
    byte                             currentID;
	char                             szName[UI_MMS_SET_SERVER_NAME_MAX_LEN];               
	char                             szHomePage[UI_MMS_SET_HOMEPAGE_URL_MAX_LEN+1];
	byte                             eDataBearer;
	nv_mms_SET_AccessPointS          sAccessPoint;
	unsigned char                    nIP[4];
	unsigned short                   nPort;
} nv_mms_SET_ServerSettingS;

typedef PACKED struct nv_mms_SET_ServerInfoStruct
{
	int    nCount;
	int    nMenuSelect;
    byte   nNvIdx[UI_MMS_SET_SERVER_CNT];
} nv_mms_SET_ServerInfoS;

typedef PACKED struct nv_mms_SET_DLOptionStruct
{
    byte    eHost;
    byte    eRoam;
} nv_mms_SET_DLOptionS;

typedef PACKED struct nv_mms_SET_TemplateStruct
{
	unsigned char     nIndex;
	char              szText[UI_MMS_EDT_TEMPLATE_TEXT_MAX_LEN+1];
	unsigned short    nMelodyID;
	unsigned short    nImageID;
} nv_mms_SET_TemplateS;

/*---------------------------------------------------------------------------
                     WMS Data Structure
---------------------------------------------------------------------------*/
/* WMS */
#define NV_WMS_ADDR_MAX 32 /* WMS_ADDRESS_MAX */
#define NV_WMS_GW_SMS_MAX 1001 /* NVIM_GW_SMS_FILE_COUNT */
#define NV_WMS_CB_LANG_MAX 18 /* UI_WMS_MENU_ITEM_LANG_MAX */

typedef PACKED struct
{
    byte status_report;             /* status report */
    byte direct_reply;              /* direct reply */
    byte auto_save;                 /* auto save MO message */
    byte bearer_mode;               /* bearer mode */
    byte input_mode;                /* input mode */
    byte sc_addr[NV_WMS_ADDR_MAX];  /* service center address */
    byte cb_lang[NV_WMS_CB_LANG_MAX];   /* CB language preference */

} nv_wmsSettingS;

typedef PACKED struct
{
    word address;
    byte archive;    /* archive flag */
    byte year;       /* timestamp: year */
    byte month;      /* timestamp: month */
    byte day;        /* timestamp: day */
    byte hour;       /* timestamp: hour */
    byte minute;     /* timestamp: minute */
    byte second;     /* timestamp: second */
} nv_wmsListExtendS;

/*---------------------------------------------------------------------------
                     Tools Related NV Structure
---------------------------------------------------------------------------*/

typedef PACKED struct
{
	byte  menuStyle;
} nv_benq_tool_menuStyleS;

typedef PACKED struct
{
	byte  screensaver;
} nv_benq_tool_ScreenSaverS;

typedef PACKED struct
{
    byte    u8WallpaperFileType;
    byte    u8WallpaperFilePath[BENQ_NV_FS_FILENAME_MAX_LENGTH+1];
    uint16  u16WallpaperPredefID;
}nv_benq_setting_WallpaperCfgS;




/*---------------------------------------------------------------------------
                     BREW Related NV Structure
---------------------------------------------------------------------------*/

#define BENQ_BREW_MAX_BREW_SERVER_NAME_LENGTH			64
#define BENQ_BREW_AUTH_B_KEY_DIGITS						26

typedef PACKED struct nv_benq_brew_mobile_id_s
{
	byte    mobileID[16];
} nv_benq_brew_mobile_id_s;

typedef PACKED struct nv_benq_brew_download_setting_s
{
	dword  carrierID;
	dword  platformID;
	byte   Server[BENQ_BREW_MAX_BREW_SERVER_NAME_LENGTH];
	word   flags;	
	byte   auth;	
	dword  policy;
} nv_benq_brew_download_setting_s;

typedef PACKED struct nv_benq_brew_b_key_s
{
	byte   Bkey[BENQ_BREW_AUTH_B_KEY_DIGITS];
} nv_benq_brew_b_key_s;

typedef PACKED struct nv_benq_brew_connection_timeout_s
{
	dword  ms;
} nv_benq_brew_connect_timeout_s;

/*---------------------------------------------------------------------------
                     Engr Related NV Structure
---------------------------------------------------------------------------*/
#define BENQ_ENGR_SW_LOAD_NAME_LENGTH			11
#define BENQ_ENGR_SMT_DATE_LENGTH				8
#define BENQ_ENGR_ASSEMBLY_DATE_LENGTH			8
#define BENQ_ENGR_PACK_DATE_LENGTH				8

typedef PACKED struct nv_benq_engr_sw_load_name_s
{
	byte   swName[BENQ_ENGR_SW_LOAD_NAME_LENGTH];
} nv_benq_engr_sw_load_name_s;

typedef PACKED struct nv_benq_engr_flip_count_s
{
	dword  flipCount;
} nv_benq_engr_flip_count_s;

typedef PACKED struct nv_benq_engr_pack_date_s
{
	byte smtDate[BENQ_ENGR_SMT_DATE_LENGTH];
	byte assemblyDate[BENQ_ENGR_ASSEMBLY_DATE_LENGTH];
	byte packDate[BENQ_ENGR_PACK_DATE_LENGTH];
} nv_benq_engr_pack_date_s;

typedef PACKED struct nv_PrePayStatusS
{
	byte status;
	byte pwd[8];
} nv_PrePayStatusS;

/****************************************************************************/
/*  NV Group Check List                                                     */
/****************************************************************************/

/* NV Group Check List */
#define NV_BENQ_GROUP_VER_CHECKLIST									\
								NV_BENQ_AUDIO_GROUP_VER_I,			\
								NV_BENQ_CALLLOG_GROUP_VER_I,		\
								NV_BENQ_CS_SYS_SPC_GROUP_VER_I,		\
								NV_BENQ_EMAIL_GROUP_VER_I,			\
								NV_BENQ_FACTORY_GROUP_VER_I,		\
								NV_BENQ_HWCONFIG_GROUP_VER_I,		\
								NV_BENQ_PB0_GROUP_VER_I,			\
								NV_BENQ_PB1_GROUP_VER_I,			\
								NV_BENQ_RF_GROUP_VER_I,				\
								NV_BENQ_SCH_GROUP_VER_I,			\
								NV_BENQ_SMS_GROUP_VER_I,			\
								NV_BENQ_TOOL_GROUP_VER_I,			\
								NV_BENQ_WAP_GROUP_VER_I,


/* NV item BenQ Group ID */
#define NV_ADD_BENQ_GROUP_ITEM_NAME									\
                                NV_SMS_SRVC_OPT_I = NV_BENQ_BASE_ENUM,	\
                                NV_BWS_MESSAGE_ALERT_I,             \
                                NV_BWS_MESSAGE_SND_I,               \
                                NV_WAP_USERDATA_STORAGE_I,          \
                                NV_WAP_PROXY_KEY_I,                 \
                                NV_WAP_PERMANENT_STORAGE_I,         \
                                NV_WAP_LINGER_TIME_I,               \
                                NV_WAP_CURRENT_PROXY_I,             \
                                NV_WAP_SID_LIST_I,                  \
                                NV_WAP_HOMEPAGE_URL_I,              \
                                NV_WAP_DOWNLOAD_URL_I,              \
                                NV_WAP_BEARER_LIST_I,               \
                                NV_WAP_PUSH_OBJECT_I,               \
                                NV_WAP_BOOT_URL_I,                  \
                                NV_WAP_DATE_FORMAT_I,               \
                                NV_WAP_PROXY_CERTIFICATE_I,         \
                                NV_WAP_CURRENT_USER_I,              \
                                NV_WAP_CURRENT_PASS_I,              \
                                NV_WAP_PSA_VERSION_I,               \
                                NV_SCHEDULER_I,                     \
                                NV_SCHEDULER_INDEX_I,               \
                                NV_ALARM_TYPE_I,                    \
                                NV_AUTO_PWR_ON_I,                   \
                                NV_AUTO_PWR_OFF_I,                  \
                                NV_SCH_RING_I,                      \
                                NV_BENQ_SMS_I,                      \
                                NV_SMS_DRAFT_I,                     \
                                NV_VOICE_MAIL_I,                    \
                                NV_SMS_SIGNATURE_I,                 \
                                NV_SMS_ALERT_STATUS_I,              \
                                NV_SMS_EXPIRY_PERIOD_I,             \
                                NV_SMS_MAX_MSG_LENGTH_I,            \
                                NV_LAST_DIALED_I,                   \
                                NV_LAST_RECEIVED_I,                 \
                                NV_LAST_MISSED_I,                   \
                                NV_PHONE_BOOK_I,                    \
                                NV_ROAMING_ALERT_I,                 \
                                NV_ANALOG_ALERT_I,                  \
                                NV_LIMIT_CALLTIME_I,                \
                                NV_EMERGENCY_I,                     \
                                NV_POWERUP_STATUS_I,                \
                                NV_CALL_GROUP_I,                    \
                                NV_CALL_LOG_COUNT_I,                \
                                NV_CALL_GROUP_INDEX_I,              \
                                NV_PB_CONTROL_LIST_I,               \
                                NV_PRL_DATA_VERSION_I,              \
                                NV_SMS_SETTINGS_I,                  \
                                NV_DTMF_MUTE_I,                     \
                                NV_ALARM_CLOCK_I,                   \
                                NV_SPEED_DIAL_I,                    \
                                NV_IMSI_M_I,                        \
                                NV_DATE_TIME_FORMAT_I,              \
                                NV_AUDIO_PROFILE_ID_I,              \
                                NV_AUDIO_SETTINGS_I,                \
                                NV_PROFILE_AUTO_ACTIVATE_I,         \
                                NV_QKEY_I,                          \
                                NV_BACKLIGHT_BRIGHT_I,              \
                                NV_HOUR_REMINDER_I,                 \
                                NV_CURRENCY_CONVERTER_I,            \
                                NV_STOPWATCH_I,                     \
                                NV_GAME_I,                          \
                                NV_QUICK_MSG_I,                     \
                                NV_PHONE_BOOK_1_I,                  \
                                NV_EML_INBOX_I,                     \
                                NV_EML_TRASH_I,                     \
                                NV_EML_DRAFT_I,                     \
                                NV_EML_OUTBOX_I,                    \
                                NV_EML_SETTINGS_I,                  \
                                NV_EML_MAIL_COUNT_I,                \
                                NV_CHAT_ROOM_I,                     \
                                NV_VMAIL_NUM_I,                     \
                                NV_CHAT_NUM_I,                      \
                                NV_EMS_IMAGE_I,                     \
                                NV_NICKNAME_I,                      \
                                NV_SHOW_DATE_TIME_DAY_I,            \
                                NV_MELODY_NAME_I,                   \
                                NV_MELODY_CONTENT_I,                \
                                NV_HAIER_SVC_NUM_I,                 \
                                NV_PRI_VERSION_I,                   \
                                NV_COLOR_SETTINGS_I,                \
                                NV_SMS_MULTI_ADDR_I,                \
                                NV_EMS_LIST_I,		                \
                                NV_SMS_DOWNLOAD_MO_I,               \
                                NV_WORLDTIME_I,                     \
                                NV_LCD_SUB_I,		                \
                                NV_PORTRAIT_TABLE_I,                \
                                NV_WALLPAPERID_I,                   \
                                NV_LAST_CALL_TIME_I,                \
                                NV_ON_TIMER_I,                      \
                                NV_SERVICE_COUNT_I,                 \
                                NV_FLEX_I,                          \
                                NV_LANGUAGE_I,                      \
                                NV_MIN_TABLE_I,                     \
                                NV_SEC_CODE_OTSL_I,                 \
                                NV_OTSL_CHECK_COUNT_I,              \
                                NV_ORIGINAL_PALETTE_I,              \
                                NV_ACTIVE_PALETTE_I,                \
                                NV_LCD_MODULE_I,                    \
                                NV_FACTORY_DATA_I,					\
								NV_BENQ_AUDIO_GROUP_VER_I,			\
								NV_BENQ_CALLLOG_GROUP_VER_I,		\
								NV_BENQ_CS_SYS_SPC_GROUP_VER_I,		\
								NV_BENQ_EMAIL_GROUP_VER_I,			\
								NV_BENQ_FACTORY_GROUP_VER_I,		\
								NV_BENQ_HWCONFIG_GROUP_VER_I,		\
								NV_BENQ_PB0_GROUP_VER_I,			\
								NV_BENQ_PB1_GROUP_VER_I,			\
								NV_BENQ_RF_GROUP_VER_I,				\
								NV_BENQ_SCH_GROUP_VER_I,			\
								NV_BENQ_SMS_GROUP_VER_I,			\
								NV_BENQ_TOOL_GROUP_VER_I,			\
								NV_BENQ_WAP_GROUP_VER_I,			\
								NV_BENQ_MAJOR_VER_I,				\
								NV_BENQ_LCD_I,						\
								NV_EXT_FACTORY_DATA_I,              \
                                NV_BENQ_IDLE_LED_I,                 \
                                NV_AUDIO_SETTINGS_2_I,				\
								NV_CALL_GROUP_V2_I,					\
								NV_BENQ_TOOL_MENU_STYLE_I,			\
								NV_BENQ_TOOL_SCREENSAVER_I,			\
                                NV_BENQ_MMS_GROUP_VER_I,            \
								NV_BENQ_MMS_PRIORITY_I,             \
								NV_BENQ_MMS_EXP_TIME_I,             \
								NV_BENQ_MMS_DLIVERY_REPORT_I,       \
								NV_BENQ_MMS_READ_REPLY_I,           \
								NV_BENQ_MMS_SIGNATURE_I,            \
                                NV_BENQ_MMS_HIDE_PHONE_I,           \
								NV_BENQ_MMS_SAVE_SENT_I,            \
                                NV_BENQ_MMS_REPLY_TYPE_I,           \
								NV_BENQ_MMS_SERVER_SET_I,           \
								NV_BENQ_MMS_SERVER_INFO_I,          \
								NV_BENQ_MMS_FILTER_I,               \
                                NV_BENQ_MMS_SIZE_LIMIT_I,           \
								NV_BENQ_MMS_DL_OPTION_I,            \
								NV_BENQ_MMS_TEMPLATE_I,				\
								NV_CURRENCYCONVERTERNAME_I,			\
								NV_CURRENCYCONVERTERRATE_I,         \
                                NV_BENQ_TOOL_THEME_I,				\
								NV_ALARM_CLOCK_5_I,					\
								NV_BENQ_SMS_CODING_SCHEME_I,		\
								NV_BENQ_BREW_GROUP_VER_I,			\
								NV_BENQ_BREW_MOBILE_ID_I,			\
								NV_BENQ_BREW_DOWNLOAD_SETTING_I,	\
								NV_BENQ_BREW_B_KEY_I,				\
								NV_BENQ_BREW_CONNECT_TIMEOUT_I,		\
								NV_PORTRAIT_TABLE_V2_I,				\
								NV_WALLPAPERID_V2_I,                \
                                NV_BENQ_WMS_GROUP_VER_I,            \
                                NV_BENQ_WMS_SETTINGS_I,             \
                                NV_BENQ_WMS_LIST_EXTEND_I,			\
								NV_SCHEDULER_V2_I,					\
								NV_SCHEDULER_INDEX_V2_I,            \
								NV_BENQ_ENGR_SW_LOAD_NAME_I,		\
								NV_BENQ_ENGR_FLIP_COUNT_I,			\
								NV_BENQ_ENGR_PACK_DATE_I,			\
								NV_PHONE_BOOK_V2_I,                 \
                                NV_PHONE_BOOK_1_V2_I,				\
								NV_ALARM_CLOCK_SET_V2_I,			\
								NV_SCHEDULER_INDEX_V3_I,			\
								NV_PB_PRIVACY_ACTION_I,				\
								NV_PB_PRIVACY_ACCEPT_I,				\
								NV_PB_PRIVACY_REJECT_I,				\
								NV_KPLIGHTMODE_I,					\
								NV_CL_DISPLAY_I,   			        \
                            	NV_CL_CALL_COST_I,                  \
								NV_CURRENCY_CONVERTER_V2_I,         \
								NV_BENQ_MMS_NOTIFY_SETTING_I,		\
								NV_BENQ_MMS_SECURITY_CHECK_I,		\
								NV_BENQ_MMS_MESSAGE_BACKUP_I,		\
								NV_BENQ_MMS_REJECT_LIST_I,			\
								NV_WORLDCLOCK_I,                    \
								NV_BENQ_PHONE_LOCK_CODE_I,			\
								NV_BENQ_SETTING_POWER_ANI_STATUS_I,	\
								NV_BENQ_SETTING_DATETIME_FORMAT_I,  \
								NV_BENQ_SETTING_POWER_SAVING_I,		\
								NV_BENQ_SETTING_MEMORY_FIRST_I,		\
								NV_CALL_GROUP_V3_I,					\
								NV_BENQ_PA_DVS_CONF_I,				\
                                NV_PREPAY_STATUS_I,                 \
                                NV_BENQ_CS_SHOW_NUMBER_I,           \
                                NV_BENQ_CS_CONNECT_ALERT_I,         \
								NV_BENQ_SETTING_POWER_SOUND_STATUS_I,\
                                NV_BENQ_SETTING_MAIN_DISP_WP_CFG_I, \
                                NV_BENQ_SETTING_EXT_DISP_WP_CFG_I,  \
                                NV_BENQ_WCLOCK_TIME_ZONE_I,         \
                                NV_BENQ_CS_ANSWER_MODE_I,           \
                                NV_BENQ_CS_MINUTE_MINDER_I,         \
                                NV_AUDIO_SETTINGS_3_I,				\
                                NV_INIT_METHOD_CASE_I,				\
								NV_PREPAY_STATUS_V2_I,				\
								NV_BENQ_ENGR_PPF_FILENAME_I,		\
								NV_BENQ_ENGR_LOCK_I,				\


/* -------------------------------------------------------------------------------------- */

/* NV item BenQ Group ID */
#define NV_ADD_BENQ_GROUP_ITEM_VALUE										\
                                byte					sms_srvc_opt;		\
                                byte                    bws_message_alert;  \
                                byte                    bws_message_snd;    \
                                wap_dm_UserDataS        *wap_user_data_str; \
                                wap_dm_ProxyKeyS        *wap_proxy_key_str; \
                                wap_dm_permanent_storageS   *wap_permanent_storage_str; \
                                word                    wap_linger_time_def;\
                                byte                    wap_current_proxy;  \
                                wap_sid_listS           *wap_sid_list;      \
                                wap_Homepage_URLS       wap_Homepage_URL;   \
                                wap_Download_URLS       wap_Download_URL;   \
                                wap_dm_Bearer_ListS     *wap_Bearer_List;   \
                                wap_dm_PushObjectS      *wap_PushObject;    \
                                wap_BootUrlS            *wap_BootUrl;       \
                                wap_DateFormatS         *wap_DateFormat;    \
                                wap_ProxyCertificateS   *wap_ProxyCertificate;\
                                wap_CurrentUserS        *wap_CurrentUser;   \
                                wap_CurrentPassS        *wap_CurrentPass;   \
                                wap_PsaVersionS         *wap_PsaVersion;    \
                                nv_scheduler_s          scheduler;          \
                                nv_scheduler_index_s    schIndex;           \
                                nv_alarm_type_s         alarmType;          \
                                nv_pwr_manager_s        pwrOnOffset;        \
                                nv_pwr_manager_s        pwrOffOffset;       \
                                byte                    schRing;            \
                                nv_sms_dm_type          benq_sms;           \
                                nv_sms_dm_type          uism_msgDraft;      \
                                nv_sms_dm_type          uism_msgVMail;      \
                                nv_smSignatureTypeS     uism_msgSignature;  \
                                byte                    uism_msg_alert;     \
                                byte                    uism_eExpiryPeriod; \
                                byte                    uism_maxMsgLength;  \
                                CallInfoS               lastDialed;         \
                                CallInfoS               lastReceived;       \
                                CallInfoS               lastMissed;         \
                                nv_pb_dm_type           pbRecord;           \
                                byte                    roam_alert;         \
                                byte                    analog_alert;       \
                                dword                   limit_calltime;     \
                                EmergencyCallS          emergencyCall;      \
                                byte                    powerUpStatus;      \
                                nv_group_dm_type        pbCallGroup;        \
                                CallLogCountS           callLogCount;       \
                                pbCallGroupIndexS       pbCallGroupIndex;   \
                                pbControlListS          pbControlList;      \
                                nv_PrlDataVersionS      prlDataVersion;     \
                                nv_smSettingsS          smSettings;         \
                                byte                    dtmf_mute;          \
                                nv_alarmClockS          alarmClock;         \
                                nv_SpeedDialS           speedDial;          \
                                nv_ImsiS                imsi_m;             \
                                nv_DateTimeFormatS      date_time_format;   \
                                byte                    activate_profile;   \
                                nv_audioProfileRecS     audioSettings;      \
                                byte                    profileAutoAct;     \
                                nv_qkeyS                qkey;               \
                                byte                    backlight;		    \
                                byte                    bHourReminder;      \
                                nv_Currency_Converter_s CurrencyConverter;  \
                                nv_stopwatch_s          stopwatch;          \
                                nv_game_s               gameData;           \
                                nv_QuickMsgS            quickMsg;           \
                                nv_pb_dm_type           pbRecord1;          \
                                nv_emlMailInfoS         emlInboxMailInfo;   \
                                nv_emlMailInfoS         emlTrashMailInfo;   \
                                nv_emlComposeInfoS      emlDraftMailInfo;   \
                                nv_emlComposeInfoS      emlOutboxMailInfo;  \
                                nv_emlUserProfileS      emlSettings;        \
                                nv_emlMailCountS        emlMailCount;       \
                                nv_ChatRoomS            chatRoom;           \
                                nv_VMailNumS            vmailNum;           \
                                nv_ChatNumS             chatNum;            \
                                nv_EmsImageS            emsImage;           \
                                nv_NicknameS            nickname;           \
                                nv_showDateTimeDayS     showDateTimeDay;    \
                                nv_MelodyNameS          melodyName;         \
                                nv_MelodyInfoS          melodyContent;      \
                                nv_Dummy_HaierSvcNumS   haierSvcNum;        \
                                nv_PriVersionS          priVersion;         \
                                nv_ColorSettingsS       colorSetting;       \
                                nv_smsMultiAddrS        smsMultiAddr;       \
                                nv_emslistS             emslist;		    \
                                nv_smsDLMOS             smsdlmo;			\
                                nv_wtimeS               wtime;              \
                                byte					lcdSub;				\
                                nv_PortraitTableS       portraitTable;      \
                                byte                    wallpaperID;        \
                                dword                   nLastCallTime;      \
                                nv_onTimerS             onTimer;            \
                                nv_serviceCountS        serviceCount;       \
                                nv_flexStatusS          flexStatus;         \
                                nv_languageTypeS        languageType;       \
                                nv_minMatchingTableS    minMatchingTable;   \
                                nv_sec_code_type        sec_code_otsl;      \
                                nv_otslCheckCountS      otslCheckCount;     \
                                lcdPaletteS             originalPalette;    \
                                lcdPaletteS             activePalette;      \
                                byte                    nvLcdModule;        \
                                nv_FactoryDataS         nvFactoryData;		\
								nv_benq_version_type	BenqAudioGroupVer;		\
								nv_benq_version_type	BenqCalllogGroupVer;	\
								nv_benq_version_type	BenqCsSysSpcGroupVer;	\
								nv_benq_version_type	BenqEmailGroupVer;		\
								nv_benq_version_type	BenqFactoryGroupVer;	\
								nv_benq_version_type	BenqHwconfigGroupVer;	\
								nv_benq_version_type	BenqPb0GroupVer;		\
								nv_benq_version_type	BenqPb1GroupVer;		\
								nv_benq_version_type	BenqRfGroupVer;			\
								nv_benq_version_type	BenqSchGroupVer;		\
								nv_benq_version_type	BenqSmsGroupVer;		\
								nv_benq_version_type	BenqToolGroupVer;		\
								nv_benq_version_type	BenqWapGroupVer;		\
								nv_benq_version_type	BenqMajorVer;			\
								byte					BenqLcd;				\
								nv_Ext_FactoryDataS		nvExtFactoryData;       \
                                byte                    BenqIdleLED;			\
								nv_audioProfileRec2S    audioSettings2;         \
								pbCallGroupV2S          pbcallgroupV2;			\
								nv_benq_tool_menuStyleS	menuStyle;				\
								nv_benq_tool_ScreenSaverS	ScreenSaver;		\
								nv_benq_version_type	BenqMmsGroupVer;		\
								byte                    nvMMSPriority;          \
								dword                   nvMMSExpTime;           \
								boolean                 nvMMSDeliveryReport;    \
								boolean                 nvMMSReadReply;         \
								nv_mms_SET_SignatureS   nvMMSSignature;         \
								boolean                 nvMMSHidePhone;         \
								byte                    nvMMSSaveSent;          \
                                byte                    nvMMSReplyType;         \
								nv_mms_SET_ServerSettingS    nvMMSServerSettings;    \
								nv_mms_SET_ServerInfoS  nvMMSServerInfo;        \
								nv_mms_SET_FilterS      nvMMSFilter;            \
                                dword                   nvMMSSizeLimit;         \
								nv_mms_SET_DLOptionS    nvMMSDLOption;          \
								nv_mms_SET_TemplateS    nvMMSTemplate;			\
								nv_CurrencyConverterName_s CurrencyConverterName;	\
								nv_CurrencyConverterRate_s CurrencyConverterRate;   \
                                byte                    Theme;					\
								nv_alarmClockSetS		alarmClockSet;			\
								byte                    nvSMSEncodeScheme;		\
								nv_benq_version_type	BenqBrewGroupVer;		\
								nv_benq_brew_mobile_id_s BenqBrewMobileID;		\
								nv_benq_brew_download_setting_s BenqBrewDownloadSetting;	\
								nv_benq_brew_b_key_s	BenqBrewBKey;			\
								nv_benq_brew_connect_timeout_s BenqBrewConnectTimeout;		\
								nv_PortraitTableS       portraitTableV2;		\
								dword                   wallpaperIDv2;          \
                                nv_benq_version_type	BenqWmsGroupVer;        \
                                nv_wmsSettingS          nvWmsSettings;          \
                                nv_wmsListExtendS       nvWmsListExtend;        \
								nv_scheduler_V2_s		schedulerV2;			\
								nv_scheduler_index_V2_s schIndexV2;				\
								nv_benq_engr_sw_load_name_s		BenqEngrSwLoadName; \
								nv_benq_engr_flip_count_s		BenqEngrFlipCount;	\
								nv_benq_engr_pack_date_s		BenqEngrPackDate;	\
								pbPhoneBookVer2S		pbRecordv2;				\
                                pbPhoneBookVer2S		pbRecord1v2;			\
								nv_alarmClockSetS_V2	alarmClockSetV2;		\
								nv_scheduler_index_V3_s schIndexV3;				\
								byte                    pbPrivacyAction;        \
								nv_pbPrivacyListS       pbPrivacyAccept;        \
                                nv_pbPrivacyListS       pbPrivacyReject;		\
								byte					kplightmode;			\
								nv_CLDisplayS           CLDisplayData;          \
                               	nv_clCallCostS          CLCallCost;             \
								nv_Currency_Converter_V2_s CurrencyConverterV2; \
								boolean					bNotifySetting;			\
								boolean					bSecurityCheck;			\
								boolean					bMessageBackup;			\
								nv_mms_SET_RejectListS	RejectList;				\
								nv_wClockS              wclock;			        \
								nv_benq_phone_lock_code_s BenqPhoneLockCode;	\
								nv_benq_setting_power_ani_status_s BenqSettingPowerAniStatus;\
								nv_benq_setting_datetime_format_s BenqSettingDateTimeFormat; \
								nv_benq_setting_power_saving_s BenqSettingPowerSaving;		 \
								nv_benq_setting_memory_first_s BenqSettingMemoryFrist;		 \
								pbCallGroupV3S          pbcallgroupV3;			\
								nv_benq_pa_dvs_conf_type pa_dvs_conf;			\
                                byte                    nPrePayStatus;          \
                                byte                    showNumber;             \
                                byte                    connectAlert;           \
                                nv_benq_setting_power_sound_status_s    BenqSettingPowerSoundStatus;\
                                nv_benq_setting_WallpaperCfgS   BenqSettingMainDispWpCfgs;\
                                nv_benq_setting_WallpaperCfgS   BenqSettingExtDispWpCfgs;\
                                byte                    timeZone;               \
                                byte                    answerMode;             \
                                byte                    minuteMinder;           \
                                nv_audioProfileRec3S    audioSettings3;			\
                                nv_initMethodCaseS      initMethodCase;         \
								nv_PrePayStatusS		sPrePayStatus;			\
								nv_engr_ppf_filename_s	BenqEngrPpfFilename;	\
								nv_engr_lock_s			BenqEngrLock;


                                // End of NV file Item
typedef enum {
	NV_ADD_BENQ_GROUP_ITEM_NAME
	NV_BENQ_MAX_I,
	NV_BENQ_ITEMS_ENUM_PAD = 0x7FFF   /* Pad to 16 bits on ARM                    */
} nv_benq_items_enum_type; 

typedef PACKED union {
	NV_ADD_BENQ_GROUP_ITEM_VALUE
} nv_benq_item_type;

/* -------------------------------------------------------------------------------------- */

/* Internal definitions of BENQ added NV items. */


typedef PACKED struct
{
	boolean			active;
	word			version;						/* NV BenQ Group Version: 0x0001--0xFFFF */
} nvi_benq_version_type;

typedef PACKED struct
{
    boolean         active;                         /* has this item ever been initialized? */
    byte            pi;                             /* Presentation Inication */
    byte            digitsNum;                      /* phone number length*/
    byte            digits[NV_MAX_DIAL_DIGIT];      /* BCD Phone Number */
    byte            lettersNum;                     /* name length*/
    byte            letters[NV_MAX_NAME_STR];       /* dialed number's name*/
    byte            numberType;                     /* Number Type */
    word            pbAddress;                      /* PhoneBook address */
    byte            timeNum;                        /*number of time stamps*/
    _clk_julian_type_ time[NV_MAX_TIMESTAMP_NUM];     /* Time/Date */
} nvi_callInfoS;

typedef PACKED  struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    stack[MAX_CALLLOG_TYPES][MAX_CALLLOG_RECORDS];  /*stack for call logs*/
} nvi_callLogCountS;

typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
//    byte usedMask[(MAX_PB_RECORDS / BITSOFBYTE) + (MAX_PB_RECORDS % BITSOFBYTE ? 1 : 0)];
    byte    usedMask[128];	// we use maximum size to prevent from using #ifdef in this file
} nvi_pbControlListS;

typedef PACKED struct 
{
    boolean active;                 /* has this item ever been initialized? */
    byte	lettersNum;		        /*phone name length*/
	byte	letters[NV_MAX_NAME_STR];	/*phone name*/
	byte	digitsNum[NV_MAX_PHONE_NUM];		/*phone number length*/
	byte	digits[NV_MAX_PHONE_NUM][NV_MAX_DIAL_DIGIT];/*phone number (ASCII code)*/
    byte    phoneType[NV_MAX_PHONE_NUM];     /*phone type*/
	byte	eMailNum;		        /*eMail length*/
	byte	eMail[MAX_EMAIL_LEN];  	/*eMail*/
    byte    groupType;              /*caller group type*/ 
    byte	numberUsed;				/*how many number used*/
	byte	CodeFormat;     		/*letters code format (UNICODE / ASCII)*/
    byte    nPortrait;              /* The index of portrait */
    byte    nStarSign;              /* Star sign of this record */
    byte    nBloodtype;             /* Bloodtype of this record */
    byte    nDefaultNumber;         /* The index of the default phone number of this record */
} nvi_pbPhoneBookS;

typedef PACKED struct
{
    boolean     active;     /* has this item ever been initialized? */
    word        count;              /* The count that will use photographs */
    byte        portraitName[MAX_PORTRAIT_NAME_LENGTH];     /* The file names in WAP FILE STORAGE  and the 
                                                     value of WAP_MAX_PATH currently is 121*/
} nvi_PortraitTableS;


typedef PACKED struct
{
    boolean     active;     /* has this item ever been initialized? */
    alarmIdS    alarmId;
    byte        alarmIndex;
} nvi_alarm_type_s;

typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    cnt;        /* how many scheduler exsit */
    byte    index[MAX_CALL_GROUPS]; 
} nvi_pbCallGroupIndexS;

typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    byte    nameNum;                /* group name length */
    byte    name[NV_MAX_NAME_STR];     /* group name */
    word    memberNum;              /* number of members */
    word    member[NV_MAX_GROUP_MEMBERS];  /* group members */
    byte    ringTone;               /* group ring tone */    
    byte    picture;              /* calling picture */
    byte    LEDPattern;             /* LED pattern */

} nvi_pbCallGroupS;

typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    nameNum;                /* group name length */
    byte    name[NV_MAX_NAME_STR];     /* group name */
    word    memberNum;              /* number of members */
    word    member[NV_MAX_GROUP_MEMBERS];  /* group members */
    byte    ringTone;               /* group ring tone */    
    byte    picture;              /* calling picture */
    byte    LEDPattern;             /* LED pattern */
    byte    midiFile[NV_MAX_FILE_LEN];
} nvi_pbCallGroupV2S; /* "2" means "version 2" */

typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    cnt;        /* how many scheduler exsit */
    byte    index[MAX_SCHEDULER_RECORDS]; 
} nvi_scheduler_index_s;

typedef PACKED struct
{
    boolean active;
    byte    status;
    byte    curProfileID;
    dword   alarmTime;      /* Unit: minutes. The offset is since the base date. */
    byte    alarmType;
    byte    alarmoffset[4];
    byte    data[NV_MAX_SCHEDULER_EVENT_LEN+1];  /* the scheduling data , note or a call number */
	dword   originalTime;
} nvi_scheduler_s;

typedef  PACKED struct
{
    boolean active;             /* has this item ever been initialized? */
    byte    status;             /* 0--3 type , 4-7 valid */
    dword   powerTimeOffset;    /* Unit: minutes. The offset is since the base date. */
} nvi_pwr_manager_s;

typedef  PACKED struct
{
    boolean active;                     /* has this item ever been initialized? */
    int2    value[NV_TEMP_TABLE_SIZ];   /* table value */
} nvi_pa_range_for_pdm1_vs_temp_s;

typedef  PACKED struct
{
    boolean active;                     /* has this item ever been initialized? */
    int2    value[NV_TEMP_TABLE_SIZ];   /* table value */
} nvi_fm_freq_sense_gain_vs_temp_s;

/* Emergency Calls Record. */
typedef PACKED struct {
    boolean active;     /* has this item ever been initialized? */
    byte    digits[MAX_EMERGENCY_CALLS][NV_MAX_DIAL_DIGIT+1];
} nvi_EmergencyCallS;

typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    boolean status;
    byte    chari[UI_SMS_SIGNATURE_SIZ+1];
} nvi_smSignatureTypeS;


typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    dword   value;
} nvi_ui_dword_type;

typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    byte    value;
} nvi_ui_byte_type;

typedef PACKED struct 
{
    boolean active;         /* has this item ever been initialized? */
    byte    actItem;        /* whether activate this service */
    byte    alarmRingType;  /* alarm ring type */
    word    timeOffset;     /* alarm clock time */
} nvi_alarmClockS;

/* PRL data version */
typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    data[NV_PRL_DATA_VERSION_LENGTH];
} nvi_PrlDataVersionS;

/* SMS settings */
typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    msgAlert;
    byte    expiryPeriod;
    char    cbNumber[NV_MAX_DIAL_DIGIT+1];
    byte    saveMode;
    byte    entryMode;
    byte    alertType;
    byte    deliveryAck;
    byte    autoErase;
    byte    usageWarning;
} nvi_smSettingsS;

/* SMS multi-address */
typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte rel_nv_idx;    /* related to which MO SMS NV item index */
    byte    count;
    word    msgid[MAX_MULTI_ADDR_NUM];
    word    flag[MAX_MULTI_ADDR_NUM];
    char    destAddr[MAX_MULTI_ADDR_NUM][MAX_EMAIL_LEN+1];
} nvi_smsMultiAddrS;

/* Quick Message */
typedef PACKED struct
{
    boolean active;                    /* has this item ever been initialized? */
    char    msg[UI_QUICK_MSG_LENGTH];  /* 20 characters can be saved in each quick message */
} nvi_QuickMsgS;

/* Chat room */
typedef PACKED struct
{
    boolean active;           /* has this item ever been initialized? */
    char msg[MAX_CHAT_ROOM_SIZE];  /* content of chat room */
} nvi_ChatRoomS;

typedef PACKED struct
{
    boolean active;           /* has this item ever been initialized? */
    byte number[NV_MAX_DIAL_DIGIT];
} nvi_ChatNumS;

typedef PACKED struct
{
    boolean active;         /* has this item ever been initialized? */
    boolean online;         /* chatting is over or not */
    byte    currentName;    /* current using nickname */
    char    name[NICKNAME_MAX_COUNT][NV_NICKNAME_MAX_LEN];
} nvi_NicknameS;

/* Voice mail number */
typedef PACKED struct
{
    boolean active;           /* has this item ever been initialized? */
    byte number[NV_MAX_DIAL_DIGIT];
} nvi_VMailNumS;

/* EMS downloaded image */
typedef PACKED struct
{
    boolean active;                     /* has this item ever been initialized? */
    byte count;                         /* how many images were saved */
    byte type[NV_MAX_EMS_IMAGE_SAVE];   /* image type */
    byte data[NV_MAX_EMS_IMAGE_SAVE][NV_MAX_EMS_IMAGE_SIZE];    /* image data */
} nvi_EmsImageS;

/* EMS control list */
typedef PACKED struct
{
    boolean active;  
	char EMSInboxList[UISM_EMS_INBOX_MAX][FS_FILENAME_MAX_LENGTH_P];
	boolean EMSMTread[UISM_EMS_INBOX_MAX];
	byte EMSInboxCnt;
	char EMSOutboxList[UISM_EMS_OUTBOX_MAX][FS_FILENAME_MAX_LENGTH_P];
	byte EMSMOID[UISM_EMS_OUTBOX_MAX];	
	byte EMSOutboxCnt;
} nvi_emslistS;

/* SMS object download MO */
typedef PACKED struct
{
    boolean active;              
	byte	DLmax;	    /* The max number of the concatenate message */
    byte    DLcount;    /* Record the number sms need sent */
    word    DLmsgid[UISM_MAX_SMS_CONCATE_COUNT];
    word    DLflag[UISM_MAX_SMS_CONCATE_COUNT];
    char    DLdestAddr[NV_MAX_DIAL_DIGIT+1];
	byte	DLdestAddrLen;
} nvi_smsDLMOS;

typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    byte    dummy;
} nvi_reserved_s;

/* Only for nv_item_type, actual size depends on wap_UserDataS in wpnram.h */
typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    byte    permanent[WAP_PERMANENT_SIZE];
} nvi_wap_permanent_storageS;

typedef PACKED struct 
{
    boolean         active;     /* has this item ever been initialized? */
    wap_ProxyInfoS  proxy[WAP_MAX_PROXY_NUMS];  /* 684 * 3 */
} nvi_wap_UserDataS;

typedef PACKED struct 
{
    byte  sidLen;     /* has this item ever been initialized? */
    word sidlist[WAP_MAX_SID_LIST_NUMS];
} nvi_wap_SidListS;

typedef PACKED struct 
{
    boolean     active;     /* has this item ever been initialized? */
    wap_KeyS    keys[WAP_MAX_PROXY_NUMS];
} nvi_wap_ProxyKeyS;

typedef PACKED struct
{
    boolean     active;     /* has this item ever been initialized? */
    char wap_Homepage_URL[WAP_SIZE_OF_HOMEPAGE_URL];
} nvi_wap_Homepage_URLS;

typedef PACKED struct
{
    boolean     active;     /* has this item ever been initialized? */
    boolean     lock;
    char wap_Download_URL[WAP_SIZE_OF_HOMEPAGE_URL];
} nvi_wap_Download_URLS;

typedef PACKED struct
{
    boolean     active;     /* has this item ever been initialized? */
    char wap_Bearer_List[WAP_BEARER_LIST_SIZE];
} nvi_wap_Bearer_ListS;

typedef PACKED struct
{
	unsigned char autoslLen;
	unsigned char autosia[WAP_MAX_PROXY_NUMS][WAP_AUTOSIA_LEN];
	unsigned char autosl[WAP_AUTOSL_LEN];
} nvi_wap_PushObejctS;
//WAP61
typedef PACKED struct
{
	unsigned char boot_url[WAP_SIZE_OF_BOOT_URL];
} nvi_wap_BootUrlS;

typedef PACKED struct
{
	unsigned char date_format[WAP_SIZE_OF_DATE_FORMAT];
} nvi_wap_DateFormatS;

typedef PACKED struct
{
	byte proxy_certificate[WAP_SIZE_OF_PROXY_CERTIFICATE];
    unsigned long ProxyCert_Len;
} nvi_wap_ProxyCertificateS;

typedef PACKED struct
{
	unsigned char wap_current_username[WAP_SIZE_OF_CURRENT_USER];
} nvi_wap_CurrentUserS;

typedef PACKED struct
{
	unsigned char wap_current_password[WAP_SIZE_OF_CURRENT_PASS];
} nvi_wap_CurrentPassS;

typedef PACKED struct
{
	unsigned char wap_psa_version[WAP_SIZE_OF_PSA_VERSION];
} nvi_wap_PsaVersionS;

typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    dword   second;     /* total handset on time in second*/
} nvi_onTimerS;

typedef PACKED struct 
{
    boolean     active;         /* has this item ever been initialized? */
    short int   serviceCount;   /* Failure Byte Identifier */
} nvi_serviceCountS;

typedef PACKED struct 
{
    boolean active;         /* has this item ever been initialized? */
    dword   flexStatus;     /* flex bit status */
} nvi_flexStatusS;

typedef PACKED struct 
{
    boolean active;         /* has this item ever been initialized? */
    byte    languageType;   /* language type */
} nvi_languageTypeS;

typedef PACKED struct 
{
    boolean active;         /* has this item ever been initialized? */
    word    minMatchingTable[NV_MIN_MATCHING_TABLE_SIZE];   /* MIN Matching Table */
} nvi_minMatchingTableS;

typedef PACKED struct 
{
    boolean     active;     /* has this item ever been initialized? */
    short int   checkCount; /* Failure Byte Identifier */
} nvi_otslCheckCountS;

typedef PACKED struct 
{
    boolean active;                         /* has this item ever been initialized? */
    boolean activate;                       /* whether activate this service */
    word    pbAddress[NV_MAX_SPEED_DIAL_NUM];   /* PhoneBook Address if any */
    byte    numberLen[NV_MAX_SPEED_DIAL_NUM];   /* the length of number */
    byte    number[NV_MAX_SPEED_DIAL_NUM][NV_MAX_DIAL_DIGIT+1]; /* the phone numbers */
} nvi_SpeedDialS;

typedef PACKED struct 
{
    boolean active;                 /* has this item ever been initialized? */
    boolean isIMSILock;
    byte    imsi_m[NV_IMSI_SIZE];   /* the array to store IMSI_M of the last session */
} nvi_ImsiS;

typedef PACKED struct 
{
    boolean active;     /* has this item ever been initialized? */
    byte    date_format;
    byte    time_format;
} nvi_DateTimeFormatS;

/* Date/Time/Day show in idle screen */
typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    boolean showDate;
    boolean showTime;
    boolean showDay;
	boolean showDigitalClock;
} nvi_showDateTimeDayS;

typedef PACKED struct
{
    boolean             active;     /* has this item ever been initialized? */
    lcdPaletteRegDataS  data[MAX_ELEMENT_NUM];
} nvi_lcdPaletteS;

typedef PACKED struct
{
    boolean         active;     /* has this item ever been initialized? */
    byte            letters_num;
    byte            letter[NV_MAX_NAME_STR];
    byte            vibrate_mode;
    byte            call_ring_type;
    byte            sms_ring_type;
    byte            alarm_ring_type;
    byte            sch_ring_type;
    byte            greeting_ring_type;  /* new item for Q600 */
    byte            ring_volume;
    byte            key_beep_method;
    byte            key_beep_volume;
    boolean         key_beep_warning_tone;
    word            system_alert;
    boolean         answer_by_flip_activate;
    boolean         answer_by_auto;
    byte            answer_by_keypad_mode;
    boolean         dtmf_mute;
    byte            dtmf_duration;
    boolean         increasing_ring;     /* new item for Q600 */
    byte            mode_LED;            /* new item for Q600 */
} nvi_audioProfileRecS;

typedef PACKED struct
{
    boolean         active;     /* has this item ever been initialized? */
    byte            letters_num;
    byte            letter[NV_MAX_NAME_STR];
    byte            vibrate_mode;
    byte            call_ring_type;
    byte            sms_ring_type;
    byte            alarm_ring_type;
    byte            sch_ring_type;
    byte            greeting_ring_type;  /* new item for Q600 */
    byte            ring_volume;
    byte            key_beep_method;
    byte            key_beep_volume;
    boolean         key_beep_warning_tone;
    word            system_alert;
    boolean         answer_by_flip_activate;
    boolean         answer_by_auto;
    byte            answer_by_keypad_mode;
    boolean         dtmf_mute;
    byte            dtmf_duration;
    boolean         increasing_ring;     /* new item for Q600 */
    byte            mode_LED;            /* new item for Q600 */
    byte            midiFile[NV_MAX_FILE_LEN];
} nvi_audioProfileRec2S;	// "2" means "version 2"

typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    qkey_id[MAX_QKEY_NUM];
} nvi_qkeyS;

typedef PACKED struct
{
    boolean    active;   /* has this item ever been initialized? */
    byte       wtime_id[MAX_TIMEZONE_NUM];
} nvi_wtimeS;

typedef PACKED struct
{
    boolean         active;
    byte            local;
    byte            foreign;
    dword           exchangeRate[NUM_OF_CURRENCY];	
} nvi_Currency_Converter_s;

typedef PACKED struct
{
    boolean         active;
    StopWatchStateE state;
    unsigned long   startTime;
    StopwatchTimeS  splitTime;
    StopwatchTimeS  stopTime;
} nvi_stopwatch_s;

typedef PACKED struct
{
	boolean     active;
    word        TopScore[UIGAME_NUM_OF_GAMES];
	byte        level[UIGAME_NUM_OF_GAMES];
} nvi_game_s;

/* Email Mail Info Struct */
typedef PACKED struct
{
    boolean     active;                         /* has this item ever been initialized? */
    byte        fileIndex;                      /* file index */
    byte        uid[NV_MAX_EML_STR_LEN];        /* uid */
    uint32      nSize;                          /* index */
    byte        sender[NV_MAX_EML_STR_LEN];     /* sender */
    byte        subject[NV_MAX_EML_STR_LEN];    /* subject */
    byte        date[NV_MAX_EML_STR_LEN];       /* date */
    boolean     mailIncomplete;                 /* mail trimed */
    byte        mailStatus;                     /* email status */
} nvi_emlMailInfoS;

/* Email Compose Info Struct */
typedef PACKED struct
{
    boolean     active;                                     /* has this item ever been initialized? */
    byte        fileIndex;                                  /* file index */
    byte        to[NV_MAX_EML_RECIPIENTS][MAX_EMAIL_LEN];   /* recipients */
    byte        subject[NV_MAX_EML_STR_LEN];                /* subject */
    byte        num_Of_Recipient;                           /* number of recipients */
    boolean     isForwardMail;                              /* is forward mail */
    uint32      composedLenWithoutFwMail;                   /* composed length - without forward mail */
    uint32      composedLenWithoutSignature;                /* composed length - without signature */
} nvi_emlComposeInfoS;

/* Email User Profiles Struct */
typedef PACKED struct
{
    boolean     active;                         /* has this item ever been initialized? */
    byte        User[NV_MAX_EML_STR_LEN];
    byte        Pass[NV_MAX_EML_STR_LEN];
    byte        SmtpServer[NV_MAX_EML_STR_LEN];
    byte        PopServer[NV_MAX_EML_STR_LEN];
    byte        EmailAddress[MAX_EMAIL_LEN];
    byte        Signature[NV_MAX_EML_STR_LEN];
    byte        KeepMail;
} nvi_emlUserProfileS;

/* Email Count */
typedef PACKED struct
{
    boolean active;
    byte    numInboxMails;
    byte    numTrashMails;
    byte    numDraftMails;
    byte    numOutboxMails;
} nvi_emlMailCountS;

typedef PACKED struct
{
    boolean active;
    char    melodyName[NV_MELODY_MAX_COUNT][MELODYNAME_MAX_LEN + 1];
} nvi_MelodyNameS;

typedef PACKED struct
{
    boolean active;
    byte      AccompanyID;
    byte      ChordId[20];
    byte      ChordLen;
    MelodyTrackInfoS  Melody_MainMelodyTrack;
}nvi_MelodyInfoS;

/* Haier Service Numbers */
typedef PACKED struct
{
    boolean active;           /* has this item ever been initialized? */
    int  defaultNum;
    byte number[NV_MAX_HAIER_SVC_NUM][NV_MAX_DIAL_DIGIT];
} nvi_HaierSvcNumS;

#define NV_MAX_COLOR_SETTING_NUM    3

typedef PACKED struct
{
    boolean active; 
    byte currentId;
    word menu_title_fg;
    word menu_title_bg;

    word menu_item_fg;
    word menu_item_bg;
    word menu_selected_item_fg;
    word menu_selected_item_bg;

    word menu_index_fg;
    word menu_index_bg;
    word menu_selected_index_fg;
    word menu_selected_index_bg;

    word menu_prompt_fg;
    word menu_prompt_bg;
    word menu_selected_prompt_fg;
    word menu_selected_prompt_bg;

    word text_fg;
    word text_bg;
    word text_selected_fg;
    word text_selected_bg;

    word time_date_fg;
    word time_date_bg;

    word text_menuList_fg;
    word text_menuList_bg;

    word calen_fg;
    word calen_bg;
    word calen_selected_fg;
    word calen_selected_bg;

    word text_option_fg;
    word text_option_bg;

    word text_error_fg;
    word text_error_bg;

    word text_info_fg;
    word text_info_bg;

    word softkey_fg;
    word softkey_bg;
    word softkey_selected_fg;
    word softkey_selected_bg;

    word fg_color;
    word bg_color;
} nvi_ColorSettingsS;


/* PRI version */
typedef PACKED struct
{
    boolean active;     /* has this item ever been initialized? */
    byte    data[NV_PRI_VERSION_LENGTH];
} nvi_PriVersionS;

/* NVI item BenQ Group ID */
#define NVI_ADD_BENQ_GROUP_ITEM_NAME								\
                                NVI_SMS_SRVC_OPT_I = NV_BENQ_BASE_ENUM,	\
                                NVI_BWS_MESSAGE_ALERT_I,            \
                                NVI_BWS_MESSAGE_SND_I,              \
                                NVI_WAP_USERDATA_STORAGE_I,         \
                                NVI_WAP_PROXY_KEY_I,                \
                                NVI_WAP_PERMANENT_STORAGE_I,        \
                                NVI_WAP_LINGER_TIME_I,              \
                                NVI_WAP_CURRENT_PROXY_I,            \
                                NVI_WAP_SID_LIST_I,                 \
                                NVI_WAP_HOMEPAGE_URLS_I,            \
                                NVI_WAP_DOWNLOAD_URLS_I,            \
                                NVI_WAP_BEARER_LIST_I,              \
                                NVI_WAP_PUSH_OBJECT_I,              \
                                NVI_WAP_BOOT_URL_I,                 \
                                NVI_WAP_DATE_FORMAT_I,              \
                                NVI_WAP_PROXY_CERTIFICATE_I,        \
                                NVI_WAP_CURRENT_USER_I,             \
                                NVI_WAP_CURRENT_PASS_I,             \
                                NVI_WAP_PSA_VERSION_I,              \
                                NVI_SCHEDULER_I,                    \
                                NVI_SCHEDULER_INDEX_I,              \
                                NVI_ALARM_TYPE_I,                   \
                                NVI_AUTO_PWR_ON_I,                  \
                                NVI_AUTO_PWR_OFF_I,                 \
                                NVI_SCH_RING_I,                     \
                                NVI_BENQ_SMS_I,                     \
                                NVI_SMS_DRAFT_I,                    \
                                NVI_VOICE_MAIL_I,                   \
                                NVI_SMS_SIGNATURE_I,                \
                                NVI_SMS_ALERT_STATUS_I,             \
                                NVI_SMS_EXPIRY_PERIOD_I,            \
                                NVI_SMS_MAX_MSG_LENGTH_I,           \
                                NVI_LAST_DIALED_I,                  \
                                NVI_LAST_RECEIVED_I,                \
                                NVI_LAST_MISSED_I,                  \
                                NVI_PHONE_BOOK_I,                   \
                                NVI_ROAMING_ALERT_I,                \
                                NVI_ANALOG_ALERT_I,                 \
                                NVI_LIMIT_CALLTIME_I,               \
                                NVI_EMERGENCY_I,                    \
                                NVI_POWERUP_STATUS_I,               \
                                NVI_CALL_GROUP_I,                   \
                                NVI_CALL_LOG_COUNT_I,               \
                                NVI_CALL_GROUP_INDEX_I,             \
                                NVI_PB_CONTROL_LIST_I,              \
                                NVI_PRL_DATA_VERSION_I,             \
                                NVI_SMS_SETTINGS_I,                 \
                                NVI_DTMF_MUTE_I,                    \
                                NVI_ALARM_CLOCK_I,                  \
                                NVI_SPEED_DIAL_I,                   \
                                NVI_IMSI_M_I,                       \
                                NVI_DATE_TIME_FORMAT_I,             \
                                NVI_AUDIO_PROFILE_ID_I,             \
                                NVI_AUDIO_SETTINGS_I,               \
                                NVI_PROFILE_AUTO_ACTIVATE_I,        \
                                NVI_QKEY_I,                         \
                                NVI_BACKLIGHT_BRIGHT_I,             \
                                NVI_HOUR_REMINDER_I,                \
                                NVI_CURRENCY_CONVERTER_I,           \
                                NVI_STOPWATCH_I,                    \
                                NVI_GAME_I,                         \
                                NVI_QUICK_MSG_I,                    \
                                NVI_PHONE_BOOK_1_I,                 \
                                NVI_EML_INBOX_I,                    \
                                NVI_EML_TRASH_I,                    \
                                NVI_EML_DRAFT_I,                    \
                                NVI_EML_OUTBOX_I,                   \
                                NVI_EML_SETTINGS_I,                 \
                                NVI_EML_MAIL_COUNT_I,               \
                                NVI_CHAT_ROOM_I,                    \
                                NVI_VMAIL_NUM_I,                    \
                                NVI_CHAT_NUM_I,                     \
                                NVI_EMS_IMAGE_I,                    \
                                NVI_NICKNAME_I,                     \
                                NVI_SHOW_DATE_TIME_DAY_I,           \
                                NVI_MELODY_NAME_I,                  \
                                NVI_MELODY_CONTENT_I,               \
                                NVI_HAIER_SVC_NUM_I,                \
                                NVI_PRI_VERSION_I,                  \
                                NVI_COLOR_SETTINGS_I,               \
                                NVI_SMS_MULTI_ADDR_I,               \
                                NVI_EMS_LIST_I,			            \
                                NVI_SMS_DOWNLOAD_MO_I,              \
                                NVI_WORLDTIME_I,                    \
                                NVI_LCD_SUB_I,		                \
                                NVI_PORTRAIT_TABLE_I,               \
                                NVI_WALLPAPERID_I,                  \
                                NVI_LAST_CALL_TIME_I,               \
                                NVI_ON_TIMER_I,                     \
                                NVI_SERVICE_COUNT_I,                \
                                NVI_FLEX_I,                         \
                                NVI_LANGUAGE_I,                     \
                                NVI_MIN_TABLE_I,                    \
                                NVI_SEC_CODE_OTSL_I,                \
                                NVI_OTSL_CHECK_COUNT_I,             \
                                NVI_ORIGINAL_PALETTE_I,             \
                                NVI_ACTIVE_PALETTE_I,               \
                                NVI_LCD_MODULE_I,                   \
                                NVI_FACTORY_DATA_I,					\
								NVI_BENQ_AUDIO_GROUP_VER_I,			\
								NVI_BENQ_CALLLOG_GROUP_VER_I,		\
								NVI_BENQ_CS_SYS_SPC_GROUP_VER_I,	\
								NVI_BENQ_EMAIL_GROUP_VER_I,			\
								NVI_BENQ_FACTORY_GROUP_VER_I,		\
								NVI_BENQ_HWCONFIG_GROUP_VER_I,		\
								NVI_BENQ_PB0_GROUP_VER_I,			\
								NVI_BENQ_PB1_GROUP_VER_I,			\
								NVI_BENQ_RF_GROUP_VER_I,			\
								NVI_BENQ_SCH_GROUP_VER_I,			\
								NVI_BENQ_SMS_GROUP_VER_I,			\
								NVI_BENQ_TOOL_GROUP_VER_I,			\
								NVI_BENQ_WAP_GROUP_VER_I,			\
								NVI_BENQ_MAJOR_VER_I,				\
								NVI_BENQ_LCD_I,						\
								NVI_EXT_FACTORY_DATA_I,             \
                                NVI_BENQ_IDLE_LED_I,                \
								NVI_AUDIO_SETTINGS_2_I,             \
                                NVI_CALL_GROUP_V2_I,

/* -------------------------------------------------------------------------------------- */

/* NVI item BenQ Group ID */
#define NVI_ADD_BENQ_GROUP_ITEM_VALUE											\
                                nvi_byte_type		    sms_srvc_opt;           \
                                nvi_byte_type           bws_message_alert;      \
                                nvi_byte_type           bws_message_snd;        \
                                nvi_wap_UserDataS       wap_user_data_str;      \
                                nvi_wap_ProxyKeyS       wap_proxy_key_str;      \
                                nvi_wap_permanent_storageS  wap_permanent_storage_str;  \
                                nvi_word_type           wap_linger_time_def;    \
                                nvi_byte_type           wap_current_proxy;      \
                                nvi_wap_SidListS        wap_sid_list;           \
                                nvi_wap_Homepage_URLS   wap_Homepage_URL;       \
                                nvi_wap_Download_URLS   wap_Download_URL;       \
                                nvi_wap_Bearer_ListS    wap_Bearer_List;        \
                                nvi_wap_PushObejctS     wap_PushObject;         \
                                nvi_wap_BootUrlS        wap_BootUrl;            \
                                nvi_wap_DateFormatS     wap_DateFormat;         \
                                nvi_wap_ProxyCertificateS wap_ProxyCertificate; \
                                nvi_wap_CurrentUserS    wap_CurrentUser;        \
                                nvi_wap_CurrentPassS    wap_CurrentPass;        \
                                nvi_wap_PsaVersionS     wap_PsaVersion;         \
                                nvi_scheduler_s         scheduler;              \
                                nvi_scheduler_index_s   schIndex;               \
                                nvi_alarm_type_s        alarmType;              \
                                nvi_pwr_manager_s       pwrOnOffset;            \
                                nvi_pwr_manager_s       pwrOffOffset;           \
                                nvi_ui_byte_type        schRing;                \
                                nvi_sms_type            benq_sms;               \
                                nvi_sms_type            uism_msgDraft;          \
                                nvi_sms_type            uism_msgVMail;          \
                                nvi_smSignatureTypeS    uism_msgSignature;      \
                                nvi_ui_byte_type        uism_msg_alert;         \
                                nvi_ui_byte_type        uism_eExpiryPeriod;     \
                                nvi_ui_byte_type        uism_maxMsgLength;      \
                                nvi_callInfoS           lastDialed;             \
                                nvi_callInfoS           lastReceived;           \
                                nvi_callInfoS           lastMissed;             \
                                nvi_pbPhoneBookS        pbRecord;               \
                                nvi_ui_byte_type        roam_alert;             \
                                nvi_ui_byte_type        analog_alert;           \
                                nvi_ui_dword_type       limit_calltime;         \
                                nvi_EmergencyCallS      emergencyCall;          \
                                nvi_ui_byte_type        powerUpStatus;          \
                                nvi_pbCallGroupS        pbCallGroup;            \
                                nvi_callLogCountS       callLogCount;           \
                                nvi_pbCallGroupIndexS   pbCallGroupIndex;       \
                                nvi_pbControlListS      pbControlList;          \
                                nvi_PrlDataVersionS     prlDataVersion;         \
                                nvi_smSettingsS         smSettings;             \
                                nvi_ui_byte_type        dtmf_mute;              \
                                nvi_alarmClockS         alarmClock;             \
                                nvi_SpeedDialS          speedDial;              \
                                nvi_ImsiS               imsi_m;                 \
                                nvi_DateTimeFormatS     date_time_format;       \
                                nvi_ui_byte_type        activate_profile;       \
                                nvi_audioProfileRecS    audioSettings;          \
                                nvi_ui_byte_type        profileAutoAct;         \
                                nvi_qkeyS               qkey;                   \
                                nvi_ui_byte_type        backlight;		        \
                                nvi_byte_type           bHourReminder;          \
                                nvi_Currency_Converter_s CurrencyConverter;     \
                                nvi_stopwatch_s         stopwatch;              \
                                nvi_game_s              gameData;               \
                                nvi_QuickMsgS           quickMsg;               \
                                nvi_pbPhoneBookS        pbRecord1;              \
                                nvi_emlMailInfoS        emlInboxMailInfo;       \
                                nvi_emlMailInfoS        emlTrashMailInfo;       \
                                nvi_emlComposeInfoS     emlDraftMailInfo;       \
                                nvi_emlComposeInfoS     emlOutboxMailInfo;      \
                                nvi_emlUserProfileS     emlSettings;            \
                                nvi_emlMailCountS       emlMailCount;           \
                                nvi_ChatRoomS           chatRoom;               \
                                nvi_VMailNumS           vmailNum;               \
                                nvi_ChatNumS            chatNum;                \
                                nvi_EmsImageS           emsImage;               \
                                nvi_NicknameS           nickname;               \
                                nvi_showDateTimeDayS    showDateTimeDay;        \
                                nvi_MelodyNameS         melodyName;             \
                                nvi_MelodyInfoS         melodyContent;          \
                                nvi_HaierSvcNumS        haierSvcNum;            \
                                nvi_PriVersionS         priVersion;             \
                                nvi_ColorSettingsS      colorSetting;           \
                                nvi_smsMultiAddrS       smsMultiAddr;           \
                                nvi_emslistS            emslist;			    \
                                nvi_smsDLMOS            smsdlmo;				\
                                nvi_wtimeS              wtime;                  \
                                nvi_ui_byte_type		lcdSub;					\
                                nvi_PortraitTableS      portraitTable;          \
                                nvi_ui_byte_type        wallpaperID;            \
                                nvi_ui_dword_type       nLastCallTime;          \
                                nvi_onTimerS            onTimer;                \
                                nvi_serviceCountS       serviceCount;           \
                                nvi_flexStatusS         flexStatus;             \
                                nvi_languageTypeS       languageType;           \
                                nvi_minMatchingTableS   minMatchingTable;       \
                                nvi_sec_code_type       sec_code_otsl;          \
                                nvi_otslCheckCountS     otslCheckCount;         \
                                lcdPaletteS             originalPalette;        \
                                lcdPaletteS             activePalette;          \
                                nvi_byte_type           nvLcdModule;            \
                                nvi_FactoryDataS        nvFactoryData;			\
								nvi_benq_version_type	BenqAudioGroupVer;	    \
   								nvi_benq_version_type	BenqCalllogGroupVer;    \
								nvi_benq_version_type	BenqCsSysSpcGroupVer;   \
								nvi_benq_version_type	BenqEmailGroupVer;	    \
								nvi_benq_version_type	BenqFactoryGroupVer;    \
								nvi_benq_version_type	BenqHwconfigGroupVer;   \
								nvi_benq_version_type	BenqPb0GroupVer;	    \
								nvi_benq_version_type	BenqPb1GroupVer;	    \
								nvi_benq_version_type	BenqRfGroupVer;		    \
								nvi_benq_version_type	BenqSchGroupVer;	    \
								nvi_benq_version_type	BenqSmsGroupVer;	    \
								nvi_benq_version_type	BenqToolGroupVer;       \
								nvi_benq_version_type	BenqWapGroupVer;	    \
								nvi_benq_version_type	BenqMajorVer;		    \
                                nvi_byte_type           BenqLcd;                \
								nvi_Ext_FactoryDataS	nvExtFactoryData;       \
                                nvi_byte_type           BenqIdleLED;			\
								nvi_audioProfileRec2S   audioSettings2;         \
                                nvi_pbCallGroupV2S      pbCallGroupV2;

                                // End of NVi file Item

/* -------------------------------------------------------------------------------------- */

#endif /* NV_BENQ_H */
