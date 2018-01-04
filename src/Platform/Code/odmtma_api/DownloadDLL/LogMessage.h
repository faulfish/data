// LogMessage.h : interface of the CLogMessage class
//

#pragma once

#include "afxtempl.h"

/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/

// message identification.
#define WM_UPDATE_LOG_MESSAGE		WM_USER+0x801
#define WM_ADD_LOG_MESSAGE			WM_USER+0x802
#define WM_CLEAR_LOG_MESSAGE		WM_USER+0x803

// maximum log message length.
#define LM_MAX_LOG_LENGTH			512

class CLogMessage
{
public:
	CLogMessage();
	CLogMessage(HWND hUIWnd);
	~CLogMessage();

// Attributes
protected:
	typedef struct {
		char	sLogMessage[LM_MAX_LOG_LENGTH];
	} T_LOG_ENTRY;

	typedef CArray<T_LOG_ENTRY, T_LOG_ENTRY&> CLogEntryAry;

protected:
	HWND m_hUIWnd;
	CStdioFile m_tLogFile;

	int m_iNextEntryIdx;
	CLogEntryAry m_atLogEntryAry;

	SYSTEMTIME m_tTime;

// Operations
public:
	void SetUIWnd(HWND hWnd) { m_hUIWnd = hWnd; }

	BOOL OpenLog(const char *sFileName, UINT uiOpenFlags);
	void CloseLog();

	void SendLogString(const char *sText);
	void SendLogString(const char *sStatus, const char *sText);
	void SendLogString(const char *sStatus1, const char *sStatus2, const char *sText);

	void PostLogString(const char *sText);
	void PostLogString(const char *sStatus, const char *sText);
	void PostLogString(const char *sStatus1, const char *sStatus2, const char *sText);

	void SendClearLog();
	void PostClearLog();

	void ErrorMessageBox(DWORD uiErrCode);

	BOOL GetLogEntry(int iEntryIdx, char **psLogMessage);

protected:
	void FormatString(const char *sText, char *sOutText, int iOutTextSize);
	void FormatString(const char *sStatus, const char *sText, char *sOutText, int iOutTextSize);
	void FormatString(const char *sStatus1, const char *sStatus2, const char* sText, char* sOutText, int iOutTextSize);
};
