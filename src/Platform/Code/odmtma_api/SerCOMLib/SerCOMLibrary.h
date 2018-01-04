// CSerCOMLibrary : Serial COM port library. Just for end user download.
// 2005/07/19 Embert Tsai.
//////////////////////////////////////////////////////////////////////

#if !defined(_SERCOMLIBRARY_H_)
#define _SERCOMLIBRARY_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SerialIO.h "
#include "MemBuffer.h "
#include "..\..\RequestManager\Request\Requestbase.h"
#include "../../CommonUtil/UnicodeUtility.h"
#include <queue>

#define LOG_MAXIMUM 10

class CRequestLog
{
	friend class CSerCOMLibrary;
public:
	CRequestLog(bool bIsSendRequest,CBufferPtr& ptRequest)
		:m_bIsSendRequest(bIsSendRequest)
		,m_ptRequest(ptRequest)
	{		
		GetSystemTime(&m_UTCTime);
	}
	CRequestLog(const CRequestLog& rhs)
	{
		*this = rhs;
	}
	~CRequestLog(){}
protected:
	bool m_bIsSendRequest;
	SYSTEMTIME m_UTCTime;
	CBufferPtr m_ptRequest;
};

class CSerCOMLibrary : public CSerialIO
{
public:
	enum {
		ERROR_CSERCOMLIBRARY_INVALID_PARAMETER				= ERROR_MASK | 0x00000301,	// Invalid parameter.
		ERROR_CSERCOMLIBRARY_COM_OPEN_FAIL					= ERROR_MASK | 0x00000302,	// Open COM port fail
		ERROR_CSERCOMLIBRARY_COM_SERVER_FAIL				= ERROR_MASK | 0x00000303,	// Init COM server fail.
		ERROR_CSERCOMLIBRARY_COM_SERVER_VERSION_OUTOFDATE	= ERROR_MASK | 0x00000304,	// COM server is out-of-date.
		ERROR_CSERCOMLIBRARY_DL_CHANGE_INTERFACE_FAIL		= ERROR_MASK | 0x00000305,	// Change interface failed
		ERROR_CSERCOMLIBRARY_NULL_INTERFACE					= ERROR_MASK | 0x00000306,	// NULL Interface
		ERROR_CSERCOMLIBRARY_COM_NOT_FOUND					= ERROR_MASK | 0x00000307,	// Can not find COM port
		ERROR_CSERCOMLIBRARY_CHANGE_OFFLINE_FAIL			= ERROR_MASK | 0x00000308,	// Change offline failed
		ERROR_CSERCOMLIBRARY_MODE_CHANGE_FAIL				= ERROR_MASK | 0x00000309,	// Mode change failed
		ERROR_CSERCOMLIBRARY_TX_FAIL						= ERROR_MASK | 0x00000310,	// Mode change failed
		ERROR_CSERCOMLIBRARY_RX_FAIL						= ERROR_MASK | 0x00000311,	// Mode change failed
		ERROR_CSERCOMLIBRARY_DS_CHANGE_INTERFACE_FAIL		= ERROR_MASK | 0x00000312,	// Mode change failed
	};
	CSerCOMLibrary();
	virtual ~CSerCOMLibrary();
	int IsDIAGConnected();  // change to Diag mode
	int IsDLConnected();    // change to Download mode
	bool IsDLMode(){return GetState() == EDL;}
	int OpenComport(LPCTSTR lpszModelName, LPCTSTR lpszComport,bool bRequestFunction = false);
	void CloseComport();
	virtual bool ResetMode();
	int TxRx(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout, bool bOneTime = false);
	int SpeedUpTxRx(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout= DEFAULT_CMD_TIME_OUT);

	int TXRXDiag(CHAR* lpTxData, DWORD dwTxLength, CHAR* lpRxData, DWORD dwRxLength, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int TXRXDl(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int TXRXDlGetRevocation(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int DLSend(CHAR * lpSendData, DWORD dwSendLen, BOOL IsCheckState = TRUE);

	int WriteAndWait(CHAR *lpTxData, DWORD dwTxLen, CHAR *lpRxData, DWORD dwRxLen, UINT uiTimeout);
	bool GetReceiverData(CHAR *lpRxData, DWORD& dwRxLen);

	bool ExecuateProbe(PLATFORM_TOOL::CRequestPtr& ptRequest);
	bool ExecuateRequest(PLATFORM_TOOL::CRequestPtr& ptRequest);
	int GetComPort();
	void ResetRequestMode();

	//For log
	void PopLastLog();
private:
	int DIAGSend(CHAR * lpSendData, DWORD dwSendLen, BOOL IsCheckState = TRUE);
	virtual void OnEvent(EEvent eEvent);
	BOOL	IsOutBufEmpty();
	void ResetReadParameter();
	virtual void AddLog(LPCTSTR szMsg);
	void push(bool bIsSendRequest,CBufferPtr& ptBufferSend);
	void pop();
	void LogBinaryToFile(const char* szDescriptiion,const char* szTemp,DWORD nBufferSize);

private:
	CRCChecker		m_CRCChecker;
	HANDLE			m_hNotifyEvent;		// Event for OnRev
	HANDLE			m_nReadEvent;
	int				m_nPortID;
	int volatile	m_nRxLen;
	int volatile	m_nReadCount;
	BYTE			*m_pbBuffer;
	TSTRING	m_strLogFile;

	//For get revocation mode
	bool m_bIsGetDLRevocation;
	//for Request
	bool m_bIsUsingRequest;
	bool m_bIsSetProbeRequestMode;
	PLATFORM_TOOL::CRequestPtr m_ptRequest;

	std::queue<CRequestLog> m_RequestLogQueue;
};

typedef CRefObjectPtr<CSerCOMLibrary> CSerCOMLibraryPtr;

#endif // !defined(_SIOCOMLIBRARY_H_)
