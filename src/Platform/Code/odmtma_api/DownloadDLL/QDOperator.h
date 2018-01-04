#pragma once

#include <string>
#include <vector>
#include "QualCmd.h"
#include "../../Diag_Export/QualcommDiag.h"
#include "HexFile.h"
#include "NV_ItemHolder.h"
#include "LogMessage.h"
#include "NVArray.h"
//using namespace std;


#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif


typedef int (*CallBackFunc)(const char* szInfo, int nStatus);
typedef bool (*CallBack)(const char*, int , int);
class CQDOperator
{
public:
	CString m_szDu;
public:
	CQDOperator(CQualcommDiag* pQualcommDiag);
	CQDOperator();
	~CQDOperator(void);
	
	int Flash();
	int CFCSpeedUpFlash();
	int SpeedUpFlash();
	int DownloadDU();
	
	void SetBackup(bool bBackup);
	void SetRestore(bool bRestore);
	
	int SetCOMPort(LPCTSTR szCOM);
	int SetPbl(LPCTSTR szPbl);
	int SetQcsbl(LPCTSTR szQcsbl);
	int SetQcsblhd(LPCTSTR szQcsblhd);
	int SetOemsbl(LPCTSTR szOemsbl);
	int SetOemsblhd(LPCTSTR szOemsblhd);
	int SetAMSS(LPCTSTR szAmss);
	int SetAMSShd(LPCTSTR szAmsshd);
	int SetPartition(LPCTSTR szPartition);
	int SetCEFS(LPCTSTR szCEFS);
	int SetHex(LPCTSTR szHex);
	int SetCsv(LPCTSTR szCsv);
	int SetBrt(LPCTSTR szBrt);
	int SetDu(LPCTSTR szDu);
	CString GetDu(){return m_szDu;}

	int SetCallBackFunc(LPVOID pfunc);
	bool SetCallback(LPVOID pfunc);
	
	int OpenHexFile(const CString& szfilename);
	int WritePartitiontoPhone(const CString& szfilename, bool bSpeedUp = false);
	bool OpenandWriteAllImage(int& nRet, bool bSpeedUp = false);
	int OpenandWriteoneImage(const std::string& strMsg, BYTE byImageType,const CString& szImageFile,bool bSpeedUp, const CString& szHeaderFile = _T(""),  bool bForce= false);
	int OpenMutilImage(BYTE byImageType, bool bSpeedUp, const CString& szfilename = _T(""));
	int WriteImagetoPhone(const CString& szfilename, bool bSpeedUp, int nPrograss = 0);
	int BackupBRT(CString& strErrorNVItemList);
	int RestoreBRT(bool bFilterRealonly);
	int BackupNV(int nTimeout);
	int RestoreNV(int nTimeout);
	bool GetHDVersion(int& nPhoneCodeVersion, int& nBootloaderVersion, int nTimeOut=DEFAULT_CMD_TIME_OUT);

	int CountCSVLines(CFile& nvfile);
	int ReadArrayItem(CNV_ItemHolder & cNV_item);
	int WriteArrayItem(CNV_ItemHolder & cNV_item);
	void SetSkipChecking(){m_bSkipChecking = TRUE;}

	/* Customization */
	bool DuckFile2NVArray(LPCTSTR pstrDuckFile); 
	bool NVArray2DuckFile(LPCTSTR pstrDuckFile);  
	bool NVArray2Mobile(void);
	bool DMSSDownload(int& nRet);

	///////////////////////////////////////////
	int DownloadFunc(void);
	int BackupFunc(void);
	int RestoreFunc(bool bFilterRealonly);
    //////////////////////////////////////////////////////////////////////////
    bool InstallDriver();

private:
	void LogFunc(const char* cpMsg, int nRet);
	void LogFunc(const char* cpMsg, int nRet, int nStep, int nPercent = 100);
	
	bool StreamingDownload(int& nRet);
	bool SpeedUpStreamingDownload(int& nRet);
	bool Initiate(const char* cpMsg, int& nRet);
	void Checking(int& nRet);

	int LineToNVItem(const CString& strLine, std::vector<CNV_ItemHolder>& vNVItem,int& nNVItemID,BYTE& nStatus);
	bool WriteBrt(int nCount, CNV_ItemHolder* pNVlist); // Backup
	bool WriteBrt(int nCount, std::vector<CNV_ItemHolder>& vNVItem); // Backup
	bool ReadBrt(CNV_ItemHolder* pNVlist, int& nvCount); //Restore
	bool ReadBrt(std::vector<CNV_ItemHolder>& vNVItem);

	bool AddArray(FILE* hDuckFile, int nNvId, const CString& strName, std::vector<CNV_ItemHolder>& vNVArray);
	void AddLog(LPCTSTR szMsg);

private:
	CQualCmd m_CQualCmd;
	CQualcommDiag* m_pQualcommDiag;
	CHexFile m_chexfile;

	bool m_bBackup;
	bool m_bRestore;

	UINT	m_nwaitforreadcount;
	WORD	m_dwaddress;
	BOOL	m_bSkipChecking;

	CString m_szCsv;
	CString m_szBrt;
	CString	m_szPbl;
	CString	m_szQcsbl;
	CString	m_szQcsblhd;
	CString	m_szOemsbl;
	CString	m_szOemsblhd;
	CString	m_szAmss;
	CString	m_szAmsshd;
	CString	m_szPartition;
	CString	m_szCEFS;
	CString m_szHex;
	
	//for 7200A
	CString m_szAppsboot;
	CString m_szAppsboothd;
	CString m_szFlash;

	CallBackFunc m_CallBackFunc;
	CallBack m_CallBack; // for bool CallBack(const char* szMsg, int& nStep, int& nProgress);
	//vector <CNV_ItemHolder> m_vCNV_ItemHolder;

	/* Customization */
	std::vector<CNV_ItemHolder> m_vNVArray;
	std::vector<CNV_ItemHolder> m_vPDPArray;

	CNVArray m_NVArray;
	CNVArray m_PDPArray;

	TSTRING m_strLogFile;

};

