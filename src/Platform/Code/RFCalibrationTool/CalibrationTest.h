#pragma once
#include "IRFCalTestDLL.h"
#include "MyMSXML.h"
#include "GPIB_Device.h"
#include <string>

typedef bool (__cdecl *pfCreateRFCALTEST)(PPIRFCalTestDLL);

typedef enum {
		MOBILEHWVER_GLAVS1A_Lavernock, 
		MOBILEHWVER_GLAVS2A_Lavernock, 
		MOBILEHWVER_GLAVS4A_Lavernock, 
		MOBILEHWVER_GMELB1A_Melos,
		MOBILEHWVER_GMELB2A_Melos,
		MOBILEHWVER_GMELB3A_Melos,
		MOBILEHWVER_LAST
}MOBILEHWVER;

static TCHAR *SUPPORTEDBANDS[] = {
	_T("GSM850,GSM900,GSM1800,GSM1900,UMTSBand1,UMTSBand5"),	// =>MOBILEHWVER_GLAVS1A_Lavernock
	_T("GSM850,GSM1800,GSM1900,UMTSBand2,UMTSBand5"),			// =>MOBILEHWVER_GLAVS2A_Lavernock
	_T("GSM850,GSM900,GSM1800,GSM1900,UMTSBand1"),				// =>MOBILEHWVER_GLAVS4A_Lavernock
	_T(""),
	_T(""),
	_T("")
};

static TCHAR *BANDMAPPINGTABLE[] = {
	_T("UMTSBand1"), _T("WCDMA2100"),
	_T("UMTSBand2"), _T("WCDMA1900"),
	_T("UMTSBand5"), _T("WCDMA850"),
	_T("UMTSBand8"), _T("WCDMA900")
};

class CComportLister  
{
public:
	CComportLister();
	virtual ~CComportLister();

	const char* GetComportList();
private:
	std::string m_strComportList;
};


class CCalibrationTest
{
// Operations
public:
	BOOL RFCalibration(TCHAR *lpszCalItems);
	BOOL RFTest(TCHAR *lpszTestItems);
	BOOL RFCalAndTest(TCHAR *lpszCalTestItems);
	VOID RFAbort();

	VOID TraceLog(LPCTSTR lpszModule, LPCSTR lpszSrcFile, UINT iSrcLine, EMsgLevel msgLevel, LPCTSTR lpszMsg);
	VOID FactoryLog(INT iProcessCode, LPCTSTR lpszBand, LPCTSTR lpszChannel, INT iErrorCode, LPCTSTR lpszUL, LPCTSTR lpszDL, LPCTSTR lpszMeasured, LPCTSTR lpszUnit, BOOL bPass, LPCTSTR lpszMsg);
	VOID CommandLog(LPCTSTR lpszType, LPCTSTR lpszMessage);

// Operations
private:
	BOOL SetupEnvironment();
	BOOL ReadIMEI();
	BOOL ReadSWVer();
	VOID ChangeToOnlineMode();
	VOID ChangeToFTMMode();
	BOOL HWVerVerify();
	BOOL LoadLib();	
	VOID FreeResource();
	BOOL GetComPort();
	BOOL GetMobileModel();
	BOOL GetGPIBDevice(); // get TesterDevice and PowerSupplyDevice
	BOOL RFCalTest_Lavernock(LPCTSTR lpszFunc, LPCTSTR lpszCalItems);
	VOID DelNonSupportedBand(CStringArray &vecBandItems);
	VOID ParseCalTestItemsString(LPCTSTR lpszItems, CStringArray &vecItems);

	CString GetRFXMLFilePath();
	CString GetRFConfigXMLFilePath();

// Attributes
private:
	enum {MOBILEMODEL_WELLFLEET, MOBILEMODEL_LAVERNOCK, MOBILEMODEL_MELOS, MOBILEMODEL_LAST} m_MobileModel;
	eGPIBDeviceType m_TesterDevice;
	eGPIBDeviceType m_PowerSupplyDevice;
	INT m_PowerSupplyGPIBAddr;
	MOBILEHWVER m_MobileHWVer;

	INT m_iComPort;
	HMODULE m_hCalTestTool;
	CString m_strModulePath;

	CMyMSXML m_ItemCodeXML;

	PIRFCalTestDLL m_piRFProcessor;

	FILE *m_pLogFile;

	pfCreateRFCALTEST m_pfCreateRFCALTEST;
	
public:
	CCalibrationTest(void);
	~CCalibrationTest(void);
};
