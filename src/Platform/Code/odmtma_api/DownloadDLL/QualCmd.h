#pragma once
#include "../../Diag_Export/QualcommDiag.h"
#include "DLcommand.h"
#include "../../Lib/RequestUtility/DiagCommand.h"
#include <string>

#define DMSS_CMD_TIME_OUT		5000    //without CmdGO

typedef int (*CallBackFunc)(const char* szInfo, int nStatus);
typedef bool (*CallBack)(const char*, int , int);

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

class CQualCmd
{
private:
	CSerCOMLibrary* m_objsercom;
	CQualcommDiag *m_pQualcommDiag;
	CString m_strComPort;
	
	CallBackFunc m_CallBackFunc;
	CallBack m_CallBack;
	
	bool m_bIsComPortOpen;
	HMODULE m_hDuckDll;
	bool m_bIsDuckActive;
	bool bNewSercomByItself;
public:
	CQualCmd(CQualcommDiag* pQualcommDiag);
	CQualCmd();
	virtual ~CQualCmd(void);
	CQualcommDiag* GetQualcommDiag(){ return m_pQualcommDiag; }
	int SetCallBackFunc(LPVOID pfunc);
	void LogFunc(const char* cpMsg, int nRet);
	void LogFunc(const char* cpMsg, int nRet, int nStep);
	bool SetCallBack(LPVOID pfunc);
	
	
	/* Common								*/
	BOOL SetComPort(LPCTSTR szCom);
	LPCTSTR GetComPort();
	int OpenComPort(void);
	void CloseComPort(void);
	int ChangeToDLMode(void);
	int ChangeToDiagMode(bool bMakeSure = false);

	/* DMSS Cmd								*/
	int CmdNopOpeation(void);
	int CmdParameter(  void);
	int CmdSoftwareverion(void);
	int CmdWrite32bit(BYTE* lpWData, WORD dwWLength,DWORD dwAddress);
	int CmdGO(WORD nwSegment,WORD nwoffset);
	int CmdReset(UINT uiTimeout);
	
	/*Stream Cmd							*/
	int CmdHello(void);
	int CmdCloseImage(bool bSpeedUp = false);
	int CmdSercurity(BYTE nMode);
	int CmdPartition(BYTE * lpbyData, WORD wnlength);						//Write Partition.mbn to Phone
	int CmdOpenMutiImage(BYTE nType,BYTE* lpbyData, WORD wnlength);
	int CmdSReset(bool bSpeedUp = false);
	int CmdStrWrite(BYTE* lpData, WORD wnLength, DWORD dwaddress);
	int CmdStrWrite2(BYTE* lpData, WORD wnLength, DWORD dwaddress);

	/*Speed up Stream Cmd							*/
	int CmdSpeedUpOrgHello(void);
	int CmdSpeedUpSercurity(BYTE nMode);
	int CmdSpeedUpPartition(BYTE * lpbyData, WORD wnlength);
	int CmdSpeedUpOpenMutiImage(BYTE nType,		BYTE* lpbyData, WORD wnlength);
	int CmdSpeedUpStrWrite2(BYTE * lpData, WORD wnLength, DWORD dwAddress);

	/* DIAG Cmd								*/
	int CmdDiagTxRx(CHAR* lpTxData, DWORD dwTxLength, CHAR* lpRxData, DWORD dwRxLength, UINT uiTimeout = DEFAULT_CMD_TIME_OUT);
	int CmdDiagIsPhoneConnected(void);
	int CmdDiagNVWrite(USHORT nItemID, UCHAR* pItemData, int nLength, USHORT* nStatus);
	BOOL CmdDiagEFSWrite(CHAR* pszPathInMobile, BYTE* pData, WORD wDataSize);
	BOOL CmdDiagEFSMkDir(CHAR* pszMkDirPath);
	bool CmdWriteNV(USHORT nItemID, UCHAR* pItemData, int nLength, USHORT* nStatus);

	/*Get revocation command*/
	int CmdGetBootLoaderRevocation(UCHAR* pStatus1,UCHAR* pVersion1, UINT uiTimeout= DEFAULT_CMD_TIME_OUT);
	int CmdGetPhonecodeRevocation(UCHAR* pStatus1,UCHAR* pVersion1, UINT uiTimeout= DEFAULT_CMD_TIME_OUT);

    
};
