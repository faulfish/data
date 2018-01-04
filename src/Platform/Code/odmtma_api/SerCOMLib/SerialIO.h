// SerialIO.h: interface for the CSerialIO class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
//
//	20040913 Embert Tsai: Add Download mode definition.
//	20071113 Embert Tsai
//	1. Add mode detection method. 
//	2. Add a new method for 6280 platform to change modes from DIAG to DL.
//
//////////////////////////////////////////////////////////////////////
#ifndef SERIALIO_H
#define SERIALIO_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Windows.h>
#include <tchar.h>
#include "SimpleCriticalSection.h"
#include "..\..\CommonUtil\RefObject.h"

#define RUN_SUCCESSFULLY	1
#define RUN_FAILED			(-1)
#define ERROR_MASK			0xF0000000

#define LOG_LEVEL_DEBUG		5
#define LOG_LEVEL_WARNING	3
#define	LOG_LEVEL_ERROR		1
#define LOG_LEVEL_NORMAL	0
#define LOG_LEVEL_NONE		(-1)

#define WEIRD_BYTE			0xA3
#define WEIRD_WORD			0xA3A3
#define WEIRD_DWORD			0xA3A3A3A3

#define RECV_BUFFER_SIZE		8192
#define COM_PORT_NAME_LENGTH	16
#define LISTENER_TIME_OUT		180000
#define SPEED_ARRAY_SIZE		3
#define	DEFAULT_CMD_TIME_OUT	20000	//default time out 

typedef enum
{
	MM_DOMINGO,
	MM_ULYSSES,
	MM_TOPAZ,
	MM_UNKNOW
} EMobileMode;

class AutoProtect
{
public:
	AutoProtect(CSimpleCriticalSection& Key):m_Key(Key)	
	{
		m_Key.Lock();
	}
	~AutoProtect()	
	{
		m_Key.Unlock();
	}
private:
	CSimpleCriticalSection& m_Key;
};

class CSerialIO : public CRefObject
{
public:
	// Baudrate
	typedef enum {
		EBaudUnknown = -1,			// Unknown
		EBaud110     = CBR_110,		// 110 bits/sec
		EBaud300     = CBR_300,		// 300 bits/sec
		EBaud600     = CBR_600,		// 600 bits/sec
		EBaud1200    = CBR_1200,	// 1200 bits/sec
		EBaud2400    = CBR_2400,	// 2400 bits/sec
		EBaud4800    = CBR_4800,	// 4800 bits/sec
		EBaud9600    = CBR_9600,	// 9600 bits/sec
		EBaud14400   = CBR_14400,	// 14400 bits/sec
		EBaud19200   = CBR_19200,	// 19200 bits/sec (default)
		EBaud38400   = CBR_38400,	// 38400 bits/sec
		EBaud56000   = CBR_56000,	// 56000 bits/sec
		EBaud57600   = CBR_57600,	// 57600 bits/sec
		EBaud115200  = CBR_115200,	// 115200 bits/sec
		EBaud128000  = CBR_128000,	// 128000 bits/sec
		EBaud230400	 = 230400,
		EBaud256000  = CBR_256000,	// 256000 bits/sec
		EBaud460800	 = 460800,
	} EBaudrate;

	// Event
	typedef enum {
		EOnStop = 0,
		EOnStart,
		EOnConnectDS,
		EOnConnectDIAG,
		EOnConnectDL,
		EOnRecv,
		EOnError
	} EEvent;

	// State
	typedef enum {
		EStop = 0,
		EStart	= 3,
		EDS		= 4,
		EDIAG	= 5,
		EDL		= 6
	} EState;

	// 20071113 Embert Tsai.
	// Add enumeration for 6280 platform.
	typedef enum{
		E_PLAT_DEFAULT	= 0,
		E_PLAT_6280		= 1,
		E_PLAT_MAX		   ,
		E_PLAT_NOT_FOUND
	} EPlatform;

	// 20071113 Embert Tsai.
	// Modified for 6280 platform compliance
	// These parameters come from QPST log.
	#define	DEF_READ_INTERVAL_TIMEOUT			MAXDWORD
	#define	DEF_READ_TOTAL_TIMEOUT_MULTIPLIER	MAXDWORD
	#define	DEF_READ_TOTAL_TIMEOUT_CONSTANT		500
	#define	DEF_WRITE_TOTAL_TIMEOUT_MULTIPLIER	1
	#define	DEF_WRITE_TOTAL_CONSTANT			50

	#define ESCAPE_CHAR			0x7D
	#define END_CHAR			0x7E
	#define COMPLEMENT_CHAR		0x20
	#define START_CHAR			0x7E

	#define	DETECT_DS_RSP				("\x61\x74\x0d\x0d\x0a\x4f\x4b\x0d\x0a\x1c\xd5\x7e")
	#define	DETECT_DIAG_RSP_5105		("\x14\x61\x74\x0d\x1c\xd5\x7d\x5e")
	#define	DETECT_DIAG_RSP_6250		("\x13\x61\x74\x0d\x62\x09\x7e")
	#define	DETECT_DL					("\x7E\x02\x6A\xD3\x7E")
	#define DETECT_DL2					("\x7E\x03\x00\x06\x9E\x4C\x7E")

	#define DIAG_MAX_RESPONSE_LEN	8192	// 20041108 for large MMS packet
											// 20050617 Embert Tsai. for dump target image packet size 4K.
public:
	CSerialIO();
	virtual ~CSerialIO();
	virtual void OnEvent (EEvent eEvent) = 0;

	virtual LONG Open (LPCTSTR lpszDevice, EBaudrate eBaudrate = EBaud115200,bool bRequestFunction = false);
	virtual LONG Close (void);
	virtual LONG Write (const void* pData, size_t iLen, DWORD* pdwWritten = 0);
	virtual LONG Read (void* pData, size_t iLen, DWORD* pdwRead = 0);	
	virtual bool MakesureToDIAGMode(void);
	virtual bool IsDLorDIAGMode();
	virtual void AddLog(LPCTSTR szMsg) = 0;
	virtual bool ResetMode() = 0;

	BOOL ChangeToDSMode(void);
	BOOL ChangeToDIAGMode(void);
	BOOL ChangeToDLMode(void);
	int  ChangeBaudRate(EBaudrate eBaudRate);	// 20050113 Embert Tsai. Add this method for Topaz download.
	
	BOOL IsCOMPortAlive(long nComPort =-1);

	EBaudrate	GetBaudrate (void);
	HANDLE		GetCommHandle (void)		{ return m_hFile; }
	EState		GetState (void) const		{ return m_eState; }
	LPCTSTR		GetStateStr (void)
	{
		switch(m_eState)
		{
		case EStop:
			return (LPCTSTR)"Stop";
		case EStart:
			return (LPCTSTR)"Start";
		case EDS:
			return (LPCTSTR)"DS mode";
		case EDIAG:
			return (LPCTSTR)"DIAG mode";
		case EDL:
			return (LPCTSTR)"DL mode";
		}
		return NULL;
	};
	LONG		GetLastError (void) const	{ return m_lLastError; }
	DWORD		GetCOMNum()					{ return m_dwCOMNum; }	// Get current COM port number.
	LPCTSTR		GetComport(void);
	
	LONG StartListener (void);
	LONG StopListener(DWORD dwTimeout = LISTENER_TIME_OUT);
	
	int ReOpen();
	BOOL IsWriteable(){ return (!m_bIsPreventOuterWrite);}
	void LogBinary(const char* szTemp,DWORD nBufferSize);
	
protected:
	CSimpleCriticalSection m_csWrite;
	CSimpleCriticalSection m_csRead;

	TCHAR	  m_szDevice[COM_PORT_NAME_LENGTH];		//device full name, for example COM10 --> \\.\COM10
	TCHAR	  m_szCOM[COM_PORT_NAME_LENGTH];		//device short name, not including prefix string.
	EBaudrate m_eBaudrate;
	EBaudrate m_ePreferBaudRate;
	EBaudrate m_TestSpeed[SPEED_ARRAY_SIZE];

	LONG	m_lLastError;
	HANDLE	m_hFile;

	HANDLE	m_hWriteEvent;
	HANDLE	m_hReadEvent;
	HANDLE  m_hStopEvent;
	HANDLE  m_hModeEvent;

	HANDLE	m_hWaitEvent;
	HANDLE	m_hThread;
	BOOL	m_bDestroy;
	BOOL	m_fStopping;  //force stop
	EState	m_eState;

	BYTE	*m_pbRecvBuf;
	// make sure these two variables will not be optimized. // 20041110 Embert Tsai
	DWORD	volatile m_dwBufStart, m_dwBufEnd;
	DWORD	volatile m_dwBufSize;
	DWORD	volatile m_dwIsExternalWriteEnable;
	LONG	m_dwCurWriteCount;

	BOOL m_bIsCOMReopen;
	BOOL m_bIsReOpen;
	BOOL m_bIsPreventOuterRead;
	BOOL m_bIsPreventOuterWrite;
	DWORD	m_dwCOMNum;
	int	m_nCurrentSpeedTry;
	bool m_bIsRequestFunction;
protected:

	EMobileMode GetMobileMode();
	int	GetCOMNum(LPCTSTR szDevice);	// Get COM port number from szDevice.
	int GetSpeedIndex(EBaudrate eBaudrate);

	LONG InternalOpen();
	LONG InternalClose(void);
	long InternalWrite(const void* pData, size_t iLen, DWORD* pdwWritten = 0);
	
	// Add the following method and variable for 6280 Diag2DL. // 20071113 Embert Tsai.
	EPlatform GetPlatformID();
	EPlatform MakeSureGetPlatformID();
	int InternalChange2DLDefault();
	int InternalChange2DL6280();
	int DIAG2DL();
	int InternalResetCOM();
	bool MakesureCOMPortReadyToUse(int nCOMPort = -1);
	
	static DWORD WINAPI ThreadProc (LPVOID lpArg);
	DWORD ThreadProc (void);
	BOOL SendProbe(void);
	
	BOOL ErrorIOPending( OVERLAPPED &ovInternal, DWORD dwRead, char * szBuffer, BOOL bContinueRecv );
	BOOL DetectMode( DWORD dwRead, char * szBuffer );
	BOOL NotErrorIOPending( long lLastError, BOOL bContinueRecv );

	// 20090913	Embert Tsai: Add change mode for DL mode.
	BOOL ChangeDS2DIAG(void);
	BOOL ChangeDIAG2DS(void);
	BOOL ChangeDL2DIAG(void);
	BOOL ChangeDIAG2DL(void);
	BOOL ChangeDS2DL(void);
	BOOL ChangeToOffline(void);

	int	AppendBuffer(const void *pvSrc, int nLen);
	int ReadBuffer(void *pvDest, int nLen);
	long WaitandReadBuffer(void* pData, size_t iLen, DWORD dwExpectedLen, DWORD* pdwRead = 0);
	
	void StopLogEventMessagePacket();

	//Set Probe mode
	bool m_bIsSetProbeMode;
};

#endif //SERIALIO_H
