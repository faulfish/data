#ifndef __GLOBAL_DEF_H__
#define __GLOBAL_DEF_H__

#include "..\MobileInterface\Interface_Include\ISMS.h"

typedef enum enumServiceTypeTag
{
	SERVICE_CONNECTION,					// com port connection
	SERVICE_AT_CMD,						// AT command
	SERVICE_OBEX_FS,					// OBEX files system
	SERVICE_OBEX_SYNCML,				// OBEX SyncML system
	SERVICE_SWITCH_POTOCOL,				// SERVICE_SWITCH_POTOCOL
} enumServiceType;

typedef enum enumRequestStateTag
{
	STATE_WAITING,						// waiting
	STATE_RUNNING,						// running
	STATE_COMPLETED_SUCCEEDED,			// completed and succeeded
	STATE_COMPLETED_FAILED,				// completed but failed
} enumRequestState;

typedef enum enumProtocolConnectionTag	// currently used by OBEX file system
{
	CONNECTION_SUCCEEDED,				// connection request succeeded
	CONNECTION_FAILED,					// connection request failed
	CONNECTION_REJECTED,				// connection request reject by OBEX related object
} enumProtocolConnection;

typedef enum enumAtCmdSetTag
{
	AT_CMD_STANDARD,					// standard AT command
	AT_CMD_BENQ,						// BenQ AT command
    AT_CMD_NEW_BENQ,                    // New BenQ AT command. Used in 
    AT_CMD_UNKNOWN                      // Default value or really unknown
} enumAtCmdSet;

typedef enum enumMsgFormatTag
{
	PDU_MODE,							// message PDU mode
	TEXT_MODE							// message text mode
} enumMsgFormat;


typedef enum enumProtocolStateTag
{
	STATE_AT_CMD,						// protocol manager in AT command mode
	STATE_OBEX,							// protocol manager in OBEX mode
} enumProtocolState;

typedef enum enumFileTypteTag
{
	FT_XTC_PFILE,						// XTC pre-defined file.
	FT_XTC_UFILE,						// XTC user-defined file (used by ReadMsFile, RenameMsFile & DeleteMsFile).
	FT_XTC_UWFILE,						// XTC user-defined whole file.
	FT_XTC_USFILE,						// XTC user-defined split file.
	FT_XMP_PFILE,						// XMP pre-defined file (reserved).
	FT_XMP_UFILE,						// XMP user-defined file.
	FT_FOLDER =	256,
} enumFileTypte;

#define FILE_RIGHT_READ_ONLY							"R-"
#define FILE_RIGHT_READ_WRITE							"RW"
#define FILE_RIGHT_WRITE_ONLY							"-W"
#define FILE_RIGHT_NONE									"--"

#define FILE_RIGHT_READ_ONLY_WITH_QUOTE					"\"R-\""
#define FILE_RIGHT_READ_WRITE_WITH_QUOTE				"\"RW\""
#define FILE_RIGHT_WRITE_ONLY_WITH_QUOTE				"\"-W\""
#define FILE_RIGHT_NONE_WITH_QUOTE						"\"--\""

/*None-forward folder.*/
#define NON_FORWARD										"NonFW"

typedef enum enumPIMActionTag
{
	DELETE_ONE,						// delete one record
	DELETE_ALL,						// delete all records
} enumPIMAction;

//================= Steven CC Chen added codes ==================BEGIN//
typedef struct CallergroupSetting_t
{
	DWORD	nID;						// the "ID" used to represent the CallerGroup
	char	szName[256];				// the "Name" of the CallerGroup
	DWORD	dwRingtoneIdType;			// the "Media Item Type" of the associated Ringtone for the CallerGroup
	char	szRingtoneIdPath[1024];		// the "ID" or "Full File Path" of the associated Ringtone for the CallerGroup
//	char	szRingtoneIdName[256];		// the "Media Item Name" of the associated Ringtone for the CallerGroup
	DWORD	dwImageIdType;				// the "Media Item Type" of the associated Image for the CallerGroup
	char	szImageIdPath[1024];		// the "ID" or "Full File Path" of the associated Image for the CallerGroup
//	char	szImageIdName[256];			// the "Media Item Name" of the associated Image for the CallerGroup
} CallergroupSetting_t;
//================= Steven CC Chen added codes ==================END//
#endif
