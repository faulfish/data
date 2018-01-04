#if !defined(__HEADER_WBXML_SYNCML_H)
#define __HEADER_WBXML_SYNCML_H
#undef OPAQUE
enum GlobalTokens
	{
	SWITCH_PAGE=0x00,	//	Change the code page for the current token state. Followed by a
						//	single u_int8 indicating the new code page number.
	END=0x01,			//	Indicates the end of an attribute list or the end of an element.
	ENTITY=0x02,		//	A character entity. Followed by a mb_u_int32 encoding the
						//	character entity number.
	STR_I=0x03,			//	Inline string. Followed by a termstr.
	LITERAL=0x04,		//	An unknown attribute name, or unknown tag posessing no
						//	attributes or content.Followed by a mb_u_int32 that encodes
						//	an offset into the string table.
	EXT_I_0=0x40,		//	Inline string document-type-specific extension token. Token is
						//	followed by a termstr.
	EXT_I_1=0x41,		//	Inline string document-type-specific extension token. Token is
						//	followed by a termstr.
	EXT_I_2=0x42,		//	Inline string document-type-specific extension token. Token is
						//	followed by a termstr.
	PI=0x43,			//	Processing instruction.
	LITERAL_C=0x44,		//	An unknown tag posessing content but no attributes.
	EXT_T_0=0x80,		//	Inline integer document-type-specific extension token. Token is
						//	followed by a mb_u_int32.
	EXT_T_1=0x81,		//	Inline integer document-type-specific extension token. Token is
						//	followed by a mb_u_int32.
	EXT_T_2=0x82,		//	Inline integer document-type-specific extension token. Token is
						//	followed by a mb_u_int32.
	STR_T=0x83,			//	String table reference. Followed by a mb_u_int32 encoding a
						//	byte offset from the beginning of the string table.
	LITERAL_A=0x84,		//	An unknown tag posessing attributes but no content.
	EXT_0=0xC0,			//	Single -byte document-type-specific extension token.
	EXT_1=0xC1,			//	Single -byte document-type-specific extension token.
	EXT_2=0xC2,			//	Single -byte document-type-specific extension token.
	OPAQUE=0xC3,		//	Opaque document-type-specific data.
	LITERAL_AC=0xC4
	};
/*
static const char*	syncml10 = "-//SYNCML//DTD SyncML 1.0//EN";
static const char*	syncml11 = "-//SYNCML//DTD SyncML 1.1//EN";
static const char*	syncmlXmlsn = "syncml:SYNCML";
static const char*	devinf10 = "-//SyncML//DTD DevInf 1.0//EN";
static const char*	devinf11 = "-//SyncML//DTD DevInf 1.1//EN";
static const char*	devinfXmlsn = "syncml:DEVINF";		//***???
static const char*	metinf10 = "-//SyncML//DTD MetInf 1.0//EN";
static const char*	metinf11 = "-//SyncML//DTD MetInf 1.1//EN";
static const char*	metinfXmlsn = "syncml:METINF";
*/
//
static const TCHAR* syncml10 =			_T("-//SYNCML//DTD SyncML 1.0//EN");
static const TCHAR* syncml11 =			_T("-//SYNCML//DTD SyncML 1.1//EN");
static const TCHAR* syncmlXmlsn =		_T("syncml:SYNCML");
static const TCHAR* devinf10 =			_T("-//SyncML//DTD DevInf 1.0//EN");
static const TCHAR* devinf11 =			_T("-//SyncML//DTD DevInf 1.1//EN");
static const TCHAR* devinfXmlsn =		_T("syncml:DEVINF");		//***???
static const TCHAR* metinf10 =			_T("-//SyncML//DTD MetInf 1.0//EN");
static const TCHAR* metinf11 =			_T("-//SyncML//DTD MetInf 1.1//EN");
static const TCHAR* metinfXmlsn =		_T("syncml:METINF");
//
static const TCHAR* MIME_TYPE_WBXML = 	_T("application/vnd.syncml+wbxml");
static const TCHAR* MIME_TYPE_XML = 	_T("application/vnd.syncml+xml");
static const TCHAR* MIME_TYPE_WBXML_DEVINF = 	_T("application/vnd.syncml-devinf+wbxml");
static const TCHAR* MIME_TYPE_XML_DEVINF =	 	_T("application/vnd.syncml-devinf+xml");
//
static const int	cHasAttributes = 0x80;
static const int	cHasContent = 0x40;
//
//
//	Alert Codes used for user alerts
//
#define	SYNCML_DISPLAY				100		//	Show. The Data element type contains content information that should be processed and displayed through the user agent.
//
//	101-150 - Reserved for future SyncML usage.
//
//	Alert Codes used at the synchronization initialization
//
#define	SYNCML_TWO_WAY				200		//	Specifies a client-initiated, two-way sync.
#define	SYNCML_SLOW_SYNC			201		//	Specifies a client-initiated, two-way slow-sync.
#define	SYNCML_ONE_WAY_FROM_CLIENT	202		//	Specifies the client-initiated, one-way only sync from the client to the server.
#define	SYNCML_REFRESH_FROM_CLIENT	203		//	Specifies the client-initiated, refresh operation for the oneway only sync from the client to the server.
#define	SYNCML_ONE_WAY_FROM_SERVER	204		//	Specifies the client-initiated, one-way only sync from the server to the client.
#define	SYNCML_REFRESH_FROM_SERVER	205		//	Specifies the client-initiated, refresh operation of the oneway only sync from the server to the client.
//
//	Alert Codes used by the server when alerting the sync.
//
#define	SYNCML_TWO_WAY_BY_SERVER				206		//	Specifies a server-initiated, two-way sync.
#define	SYNCML_ONE_WAY_FROM_CLIENT_BY_SERVER	207		//	Specifies the server-initiated, one-way only sync from the client to the server.
#define	SYNCML_REFRESH_FROM_CLIENT_BY_SERVER	208		//	Specifies the server-initiated, refresh operation for the one-way only sync from the client to the server.
#define	SYNCML_ONE_WAY_FROM_SERVER_BY_SERVER	209		//	Specifies the server-initiated, one-way only sync from the server to the client.
#define	SYNCML_REFRESH_FROM_SERVER_BY_SERVER	210		//	Specifies the server-initiated, refresh operation of the oneway only sync from the server to the client.
//
//	211-220 - Reserved for future SyncML usage.
//
//	Special Alert Codes
//
#define	SYNCML_RESULT_ALERT		221		//	Specifies a request for sync results.
#define	SYNCML_NEXT_MESSAGE		222		//	Specifies a request for the next message in the package.
#define	SYNCML_NO_END_OF_DATA	223		//	End of Data for chunked object not received.
//
//	224-250 - Reserved for future SyncML usage.
//
struct TMap
{
	const TCHAR*	m_xml;
	int				m_wbxml;
};
struct TMapStack
{
	const TMap*		m_entry;
	TMapStack*		m_next;
};

struct TXmlLine
{
public:
	TXmlLine(int level,int codePage,bool endTag)
		:m_next(NULL),m_indent(level),m_codePage(codePage),m_endTag(endTag){}
	~TXmlLine(){}
	CString			m_strData;
	TXmlLine*		m_next;
	int				m_indent;
	int				m_codePage;
	bool			m_endTag;
};

typedef enum _XptStatus
{
    XPT_STATUS_SUCCESS       = 0x00000000,
    XPT_STATUS_FAILED        = 0x00000001,
    XPT_STATUS_PENDING       = 0x00000002,
    XPT_STATUS_CONTINUE      = 0x00000003,
    XPT_STATUS_DISCONNECTED  = 0x00000004,
    XPT_STATUS_BUSY          = 0x00000005,
    XPT_STATUS_ABORTED       = 0x00000006,
    XPT_STATUS_NO_RESOURCES  = 0x00000007,
    XPT_STATUS_FATAL_ERR     = 0x00000008,
    XPT_STATUS_NO_CONNECTION = 0x00000009,
    XPT_STATUS_INVALID_PARM  = 0x00000010,
    XPT_STATUS_TIMED_OUT     = 0x00000011
}XptStatus;

XptStatus XmlToWbxml(const TCHAR* xml,int xmlLength,char** pwbxml,int* wbxmlLength);
XptStatus WbxmlToXml(const BYTE* wbxml,int wbxmlLength,CString& xml,DWORD& xmlLength);

extern const TMap		tDevInf[];
extern const TMap		tMetInf[];
extern const TMap		tSyncML[];
extern const TMap*		codePages[];
extern const TCHAR*		codePageNames[];

#endif //!defined(__HEADER_WBXML_SYNCML_H)
