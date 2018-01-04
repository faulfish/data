#include "StdAfx.h"
#include "RFCalibrationTool.h"
#include "CalibrationTest.h"
#include "QLib.h"
#include "..\STE\MiddleWare\CallbackTypes.h"
#include "QisdaMiscellaneousModule.h"
#include "..\STE\MiddleWare\MiddleWare.h"
#include "../../Modules\ODMCoachModule\ODM.h"
#include <direct.h>
#include "CalTestSettingManager.h"

using namespace std;

EXTERN_C IMAGE_DOS_HEADER __ImageBase;

#define DEVICE_NAME_LENGTH 1000
static LPCTSTR MODULENAME = _T("RFCalTest");

void EnumerateDiagSerialPorts(CStringArray& strArray)
{
	LPCTSTR szFunc = _T("EnumerateDiagSerialPorts");
	HKEY hKey = NULL;
	int nRet = 0;
	BOOL bIsSetKey = FALSE;
	DWORD dwDataLength = 0;
	DWORD	dwType = 0 ;
	CString strUserInitCmd;
	CString strRegKeyHeader;
	char	szDeviceName[DEVICE_NAME_LENGTH];
	CHAR     achKey[MAX_PATH]; 
    CHAR     achClass[MAX_PATH] = "";  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // length of class string 
 
    DWORD i; 
    DWORD retCode; 
	unsigned long ulLength = 0;
 
    DWORD cchValue = DEVICE_NAME_LENGTH; 
	CString strRegKey = _T("");
	CString strDebugMessage = _T("");
	HKEY hSubKey = NULL;

	strArray.RemoveAll();

	strRegKeyHeader = _T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E978-E325-11CE-BFC1-08002BE10318}");

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegKeyHeader, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS){
		return;
	}

	i = 0 ;
	while( TRUE)    
	{ 
		
        
		// Enumerate all entry
		retCode = RegEnumKey(hKey, i, achKey, DEVICE_NAME_LENGTH);
		
        if (retCode == (DWORD)ERROR_SUCCESS || retCode == (DWORD) ERROR_MORE_DATA) 
        {
			hSubKey = NULL;
			dwType = REG_SZ;
			strRegKey = strRegKeyHeader + _T("\\") + achKey;
			
			
			nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegKey, 0, KEY_ALL_ACCESS, &hSubKey);
    
			if ( nRet != ERROR_SUCCESS ){
				break;
			}

			ZeroMemory(szDeviceName, DEVICE_NAME_LENGTH);
			dwDataLength = DEVICE_NAME_LENGTH;

			nRet = RegQueryValueEx(hSubKey, _T("AssignedPortForQCDevice"), NULL, &dwType, (LPBYTE)szDeviceName , &dwDataLength);

			if ( nRet != ERROR_SUCCESS ){
				RegCloseKey(hSubKey);
				i++;
				continue;
		
			}
			CString strPortName = szDeviceName;
			strPortName.MakeUpper();
			strArray.Add(strPortName);
		
			RegCloseKey(hSubKey);
      
		}else {
			break;
			
		}
		i++;
	}
	RegCloseKey(hKey);
	
}

CString GetCurrentActivePort()
{
	LPCTSTR szFunc = _T("GetCurrentActivePort");
	CComportLister objLister;
	CString strRet = _T("");
	
	CString strList = objLister.GetComportList();
	strList.MakeUpper();
	
	CStringArray strArray;
	EnumerateDiagSerialPorts(strArray);
	int i = 0;
	for ( i = 0 ; i < strArray.GetSize() ; i ++){
		if (-1 != strList.Find(strArray[i],0) ){
			return strArray[i];
		}
	}
	return strRet;
}

CComportLister::CComportLister()
{
}

CComportLister::~CComportLister()
{

}

const char* CComportLister::GetComportList()
{
	OSVERSIONINFO osvinfo;
	/* determine which platform we're running on and forward
	* to the appropriate routine
	*/
	ZeroMemory(&osvinfo,sizeof(osvinfo));
	osvinfo.dwOSVersionInfoSize=sizeof(osvinfo);
	GetVersionEx(&osvinfo);

	// windows 2000 and windows XP
	if( osvinfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		osvinfo.dwMajorVersion ==5 &&
		osvinfo.dwMinorVersion == 0 ||
		osvinfo.dwMinorVersion == 1)
	{
		char szValueName[128];
		unsigned char szValueData[128];
		DWORD retCode;
		int i;
		HKEY   hkEnum=NULL;
		ULONG ulValueNameLength = 128, ulValueDataLength = 128;

		retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE\\DEVICEMAP\\SERIALCOMM",0,KEY_QUERY_VALUE,&hkEnum);

		m_strComportList = "";
		for (i = 0; ; i++)
		{
			ulValueNameLength = 128;
			ulValueDataLength = 128;
			retCode = RegEnumValue(hkEnum, i, szValueName, &ulValueNameLength, NULL, NULL, szValueData, &ulValueDataLength);
			if(retCode != ERROR_SUCCESS)
			{
				break;
			}

			//+ Add by Rick Huang, 2004/12/22
			//+ for avoid get bluetooth comport in O2
			strlwr(szValueName);

			if(strstr(szValueName, "\\device\\qcusb") != NULL)
			{
				if(m_strComportList != "")
				{
					m_strComportList += ";";
				}

				m_strComportList += (char*)szValueData;
			}
			//+
		}

		return m_strComportList.c_str();
	}
	else
	{
		assert(0);
		//char buffer[512];
		//sprintf(buffer, "osvinfo.dwPlatformId = %d\nosvinfo.dwMajorVersion = %d\nosvinfo.dwMinorVersion = %d\nSorry, The EnumPort() dosen't supported in this windows version yet. Please contect Rick Huang: ext 7247.\n", osvinfo.dwPlatformId, osvinfo.dwMajorVersion, osvinfo.dwMinorVersion);
		//MessageBox(buffer, "Worning");
	}
	return NULL;
}

CCalibrationTest::CCalibrationTest(void)
{
	m_MobileModel = MOBILEMODEL_LAST;
	m_TesterDevice = GPIB_DEVICE_UNKNOWN;
	m_PowerSupplyGPIBAddr = 0;
	m_PowerSupplyDevice = GPIB_DEVICE_UNKNOWN;
	m_hCalTestTool = 0;
	m_pfCreateRFCALTEST = NULL;
	m_piRFProcessor = NULL;
	m_MobileHWVer = MOBILEHWVER_LAST;

	::GetModuleFileName((HINSTANCE)&__ImageBase, m_strModulePath.GetBuffer(_MAX_PATH), _MAX_PATH);
	m_strModulePath.ReleaseBuffer();
	m_strModulePath = m_strModulePath.Left(m_strModulePath.ReverseFind(_T('\\')));

	CString strLogFilePath = m_strModulePath + _T("\\CalTest.log");
	m_pLogFile = _tfopen(strLogFilePath, _T("w"));	
}

CCalibrationTest::~CCalibrationTest(void)
{
	if (m_pLogFile != NULL) {
		fclose(m_pLogFile);
		m_pLogFile = NULL;
	}		
}

BOOL CCalibrationTest::RFCalibration(TCHAR *lpszCalItems)
{
	if (!SetupEnvironment()) {
		FreeResource();
		return FALSE;
	}

	BOOL bResult = FALSE;
	switch (m_MobileModel) {
		case MOBILEMODEL_WELLFLEET:
			break;
		case MOBILEMODEL_LAVERNOCK:
			bResult = RFCalTest_Lavernock(_T("Calibration"), lpszCalItems);
			break;
		case MOBILEMODEL_MELOS:
			break;
		case MOBILEMODEL_LAST:
			break;
		default:
			ASSERT(0);
			break;
	}

	FreeResource();
	ChangeToOnlineMode();

	return bResult;
}

BOOL CCalibrationTest::RFTest(TCHAR *lpszTestItems)
{
	if (!SetupEnvironment()) {
		FreeResource();
		return FALSE;
	}

	BOOL bResult = FALSE;
	switch (m_MobileModel) {
		case MOBILEMODEL_WELLFLEET:
			break;
		case MOBILEMODEL_LAVERNOCK:
			bResult = RFCalTest_Lavernock(_T("Test"), lpszTestItems);
			break;
		case MOBILEMODEL_MELOS:
			break;
		case MOBILEMODEL_LAST:
			break;
		default:
			ASSERT(0);
			break;
	}
	
	FreeResource();
	ChangeToOnlineMode();

	return bResult;
}

BOOL CCalibrationTest::RFCalAndTest(TCHAR *lpszCalTestItems)
{
	if (!SetupEnvironment()) {
		FreeResource();
		return FALSE;
	}

	BOOL bResult = FALSE;
	switch (m_MobileModel) {
		case MOBILEMODEL_WELLFLEET:
			break;
		case MOBILEMODEL_LAVERNOCK:
			bResult = RFCalTest_Lavernock(_T("Calibration"), lpszCalTestItems);
			if (bResult) {
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("ChangeMobileMode"));	
				ChangeToOnlineMode();
				Sleep(25000);
				ChangeToFTMMode();
				Sleep(25000);
				bResult = RFCalTest_Lavernock(_T("Test"), lpszCalTestItems);
			}
			break;
		case MOBILEMODEL_MELOS:
			break;
		case MOBILEMODEL_LAST:
			break;
		default:
			ASSERT(0);
			break;
	}

	FreeResource();
	ChangeToOnlineMode();

	return bResult;
}

VOID CCalibrationTest::RFAbort()
{
	if (m_piRFProcessor != NULL)
		m_piRFProcessor->AbortProcess();
}

VOID CCalibrationTest::TraceLog(LPCTSTR lpszModule, LPCSTR lpszSrcFile, UINT iSrcLine, EMsgLevel msgLevel, LPCTSTR lpszMsg)
{
	USES_CONVERSION;

	CString strSrcFile = CA2T(lpszSrcFile);
	CString strSrcLine;
	strSrcLine.Format(_T("%3d"), iSrcLine);

	_ftprintf(m_pLogFile, _T("TraceLog:   %s,%s,%s,%s\n"), 
		lpszModule,
		PathFindFileName(strSrcFile),
		strSrcLine,
		lpszMsg);	

	if (msgLevel >= MSG_HIGH) {
		fflush(m_pLogFile);

		CStringA strMessageA = CT2A(lpszMsg);
		strMessageA.Replace(_T(','), _T(' ')); // important!!
		OutputData((LPSTR)(LPCSTR)strMessageA, _T(""), (std::string)(""));
	}

	return;
}

VOID CCalibrationTest::FactoryLog(INT iProcessCode, LPCTSTR lpszBand, LPCTSTR lpszChannel, INT iErrorCode, LPCTSTR lpszUL, LPCTSTR lpszDL, LPCTSTR lpszMeasured, LPCTSTR lpszUnit, BOOL bPass, LPCTSTR lpszMsg)
{
	USES_CONVERSION;

	_ftprintf(m_pLogFile, _T("FactoryLog: %d,%s,%s,%d,%s,%s,%s,%s,%d,%s\n"), 
		iProcessCode,
		lpszBand,
		lpszChannel,
		iErrorCode, 
		lpszUL, 
		lpszDL, 
		lpszMeasured, 
		lpszUnit,
		bPass, 
		lpszMsg);

	CStringA strPassFailFlagA;
	if (bPass)
		strPassFailFlagA = "P";
	else
		strPassFailFlagA = "F";

	CStringA strBandA = CT2A(lpszBand);
	CStringA strChannelA = CT2A(lpszChannel);
	CStringA strUnitA = CT2A(lpszUnit);
	CStringA strMeasuredA = CT2A(lpszMeasured);
	CStringA strHighLimitA = CT2A(lpszUL);
	CStringA strLowLimitA = CT2A(lpszDL);

	CString strProcessCode;
	strProcessCode.Format(_T("%04d"), iProcessCode);

	CStringA strDescriptionA;
	if (m_ItemCodeXML.IsLoad()) {
		CString strItemXMLPath;
		strItemXMLPath.Format(_T("//ProcessItems/ProcessItem[@Code=\"%s\"]"), strProcessCode);
		XMLNode nodeItem = m_ItemCodeXML.SearchNode(strItemXMLPath);
		CString strItemName = m_ItemCodeXML.GetNodeAttrValue(nodeItem, _T("Name"));
		strDescriptionA = CT2A(strItemName);
	}

	strBandA.Replace(_T(','), _T(' '));
	strChannelA.Replace(_T(','), _T(' '));
	strUnitA.Replace(_T(','), _T(' '));
	strDescriptionA.Replace(_T(','), _T(' '));
	strLowLimitA.Replace(_T(','), _T(' '));
	strHighLimitA.Replace(_T(','), _T(' '));
	strMeasuredA.Replace(_T(','), _T(' '));
	strPassFailFlagA.Replace(_T(','), _T(' '));
	OutputDetailResult(
		(LPSTR)(LPCSTR)strDescriptionA, 
		(LPSTR)(LPCSTR)strBandA, 
		(LPSTR)(LPCSTR)strChannelA, 
		(LPSTR)(LPCSTR)strLowLimitA, 
		(LPSTR)(LPCSTR)strHighLimitA, 
		(LPSTR)(LPCSTR)strMeasuredA, 
		(LPSTR)(LPCSTR)strUnitA, 
		(LPSTR)(LPCSTR)strPassFailFlagA);

	return;
}

VOID CCalibrationTest::CommandLog(LPCTSTR lpszType, LPCTSTR lpszMessage)
{
	_ftprintf(m_pLogFile, _T("CommandLog: %s,%s,\n"), lpszType, lpszMessage);
	return;
}

BOOL CCalibrationTest::SetupEnvironment()
{
	if (!LoadLib()) {
		return FALSE;
	}

	if (!GetComPort())
		return FALSE;

	if (!GetGPIBDevice())
		return FALSE;

	ReadIMEI();
	ReadSWVer();

	if (!GetMobileModel())
		return FALSE;	

	if (!HWVerVerify())
		return FALSE;

	CString strItemCodeFilePath =  m_strModulePath + _T("\\ProcessItemCode.xml");
	if (m_ItemCodeXML.Load(strItemCodeFilePath) != 0) {
		OutputDetailResult("Load ProcessItemCode.xml failed", "-", "-", "-", "-", "-", "-", "-");
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Load ProcessItemCode.xml success"));

	return TRUE;
}

BOOL CCalibrationTest::ReadIMEI()
{
	// report imei
	CStringA strIMEIA;
	bool bSuccess = GetIMEI(strIMEIA.GetBuffer(20));
	strIMEIA.ReleaseBuffer();

	std::string strOutput;
	if (bSuccess) {
		OutputData("IMEI", (LPSTR)(LPCSTR)strIMEIA, strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("IMEI: %s"), strIMEIA);
	}
	else {
		OutputData("IMEI", "N/A", strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("IMEI: N/A"));
	}
	
	
	return (BOOL)bSuccess;
}

BOOL CCalibrationTest::ReadSWVer()
{
	CQisdaMiscellaneousModule cMiscellaneous(m_iComPort);
	TCHAR szSW[20] = {0}, szHW[20] = {0};
	BOOL bResult = FALSE;
	for (int i = 0; i < 3; i++)
		if (cMiscellaneous.GetSWHWVersion(szSW, szHW)) {
			bResult = TRUE;
			break;
		}

	std::string strOutput;
	if (bResult) {
		OutputData(_T("SW"), szSW, strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("SW Ver: %s"), szSW);
	}
	else {
		OutputData(_T("SW"), _T("N/A"), strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("SW Ver: N/A"));
	}
			
	return bResult;
}

VOID CCalibrationTest::ChangeToOnlineMode()
{
	CNVItem nv(m_iComPort);
	CControlModeSet controlMode(m_iComPort);

	controlMode.DiagControlMode(4);
	nv.ChangeOnline();
	controlMode.DiagControlMode(1);
	controlMode.DiagControlMode(2);
}

VOID CCalibrationTest::ChangeToFTMMode()
{
	CNVItem nv(m_iComPort);
	CControlModeSet controlMode(m_iComPort);

	controlMode.DiagControlMode(4);
	nv.ChangeFTM();
	controlMode.DiagControlMode(1);
	controlMode.DiagControlMode(2);
}

BOOL CCalibrationTest::HWVerVerify()
{
	MOBILEHWVER verGetFromDiagCmd, verGetFromNVItem;
	CODM odm(m_iComPort);
	int nVersion;
	odm.GetLavernockHWVersion(nVersion);
	if (nVersion == 1)
		verGetFromDiagCmd = MOBILEHWVER_GLAVS1A_Lavernock;
	else if (nVersion == 2)
		verGetFromDiagCmd = MOBILEHWVER_GLAVS2A_Lavernock;
	else if (nVersion == 4)
		verGetFromDiagCmd = MOBILEHWVER_GLAVS4A_Lavernock;;

	CNVItem nvItem(m_iComPort);
	DIAG_NVITEM_PACKET_F_type nvValue;
	nvItem.ReadNVItem(7104, nvValue);

	CStringA strQCNFileName = (char*)(nvValue.item_data);
	if (strQCNFileName.Find("GLAVS1A"))
		verGetFromNVItem = MOBILEHWVER_GLAVS1A_Lavernock;
	else if (strQCNFileName.Find("GLAVS2A"))
		verGetFromNVItem = MOBILEHWVER_GLAVS2A_Lavernock;
	else if (strQCNFileName.Find("GLAVS4A"))
		verGetFromNVItem = MOBILEHWVER_GLAVS4A_Lavernock;
	else if (strQCNFileName.Find("GMELB1A"))
		verGetFromNVItem = MOBILEHWVER_GMELB1A_Melos;
	else if (strQCNFileName.Find("GMELB2A"))
		verGetFromNVItem = MOBILEHWVER_GMELB2A_Melos;
	else if (strQCNFileName.Find("GMELB3A"))
		verGetFromNVItem = MOBILEHWVER_GMELB3A_Melos;
	else
		verGetFromNVItem = MOBILEHWVER_LAST;

	if (verGetFromDiagCmd != verGetFromNVItem) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("SW(%d) and HW(%d) is not match"), verGetFromNVItem, verGetFromDiagCmd);
		return FALSE;
	}

	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("SW(%d) and HW(%d) is not match"), verGetFromNVItem, verGetFromDiagCmd);
	m_MobileHWVer = verGetFromDiagCmd;
	return TRUE;
}

VOID  CCalibrationTest::DelNonSupportedBand(CStringArray &vecBandItems)
{
	CString strBands = SUPPORTEDBANDS[m_MobileHWVer];
	for (int i = 0; i < vecBandItems.GetSize(); i++) {		
		TRACE("%s", vecBandItems[i]);
		if (strBands.Find(vecBandItems[i]) == -1) {
			vecBandItems.RemoveAt(i);
			i--;
		}
	}
}

BOOL CCalibrationTest::LoadLib()
{
	// RFCalTestDLL
	CString strRFCalTestDllPath = m_strModulePath + _T("\\RFCalTestDLL_ShareM.dll");
	m_hCalTestTool = ::LoadLibrary(strRFCalTestDllPath);
	if (m_hCalTestTool == 0) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("Load dll failed - RFCalTestDLL.dll"));
		return FALSE;
	}

	m_pfCreateRFCALTEST = (pfCreateRFCALTEST)GetProcAddress(m_hCalTestTool, "CreateRFCALTEST");
	if (m_pfCreateRFCALTEST == NULL) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("Load function failed - CreateRFCALTEST"));
		return FALSE;
	}

	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Load RFCalTestDLL success"));

	return TRUE;
}

VOID CCalibrationTest::FreeResource()
{
	if (m_hCalTestTool != 0) {
		FreeLibrary(m_hCalTestTool);
		m_hCalTestTool = 0;
	}
}

BOOL CCalibrationTest::GetComPort()
{
	//unsigned short iNumPorts = 0x1, portList;
	//char res = QLIB_GetAvailablePhonesPortList(&iNumPorts, &portList, 0, NULL);

	//if (iNumPorts == 0)
	//	return FALSE;

	CString strPortList = GetCurrentActivePort();
	if (strPortList.IsEmpty()) {
		std::string strOutput;
		OutputHeader(_T("Fail to get comport!!"), strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_LOW, _T("Fail to get comport!!"), m_iComPort);
		return FALSE;
	}
	
	strPortList.Replace(_T("COM"), _T(""));

	m_iComPort = _ttoi(strPortList);

	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_LOW, _T("COM (%d) is detected"), m_iComPort);
	return TRUE;
}

BOOL CCalibrationTest::GetMobileModel()
{
	CQisdaMiscellaneousModule Miscellaneous(m_iComPort);
	CStringA strSW, strHW;
	bool bSuccess = Miscellaneous.GetSWHWVersion(strSW.GetBuffer(100), strHW.GetBuffer(100));
	strSW.ReleaseBuffer();
	strHW.ReleaseBuffer();
	
	if (!bSuccess) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("MobileModel detect failed"));
		m_MobileModel = MOBILEMODEL_LAST;
		return FALSE;
	}

	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("MobileModel detect success - %s"), strHW);
	
	if (strHW.CompareNoCase(_T("Athens")) == 0)
		m_MobileModel = MOBILEMODEL_WELLFLEET;
	else if (strHW.CompareNoCase(_T("Lavernock")) == 0)
		m_MobileModel = MOBILEMODEL_LAVERNOCK;
	else if (strHW.CompareNoCase(_T("Melos")) == 0)
		m_MobileModel = MOBILEMODEL_MELOS;
	else
		m_MobileModel = MOBILEMODEL_LAST;

	return TRUE;
}

BOOL CCalibrationTest::GetGPIBDevice()
{
	m_TesterDevice = GPIB_DEVICE_UNKNOWN;
	m_PowerSupplyDevice = GPIB_DEVICE_UNKNOWN;
	m_PowerSupplyGPIBAddr = 0;
	int device[3] = {GPIB_DEVICE_UNKNOWN, GPIB_DEVICE_UNKNOWN, GPIB_DEVICE_UNKNOWN}, deviceAddr[3] = {0};

	const int iMaxDeviceCount = 3;
	CGPIBDevice gpibDevice;
	gpibDevice.IdAndInitDevice(device[0], device[1], device[2], deviceAddr[0], deviceAddr[1], deviceAddr[2]);

	BOOL bTesterDeviceFound = FALSE,  bPowerDevice1Found = FALSE, bPowerDevice2Found = FALSE;
	for (int i = 0; i < iMaxDeviceCount; i++) {
		if (device[i] >= RsCmu200 && device[i] <= WillTek) {
			m_TesterDevice = (eGPIBDeviceType)device[i];
			bTesterDeviceFound = TRUE;
			break;
		}
	}

	for (int i = 0; i < iMaxDeviceCount; i++) {
		if (device[i] >= HpE3631 && device[i] <= Hp66332A) {
			m_PowerSupplyDevice = (eGPIBDeviceType)device[i];
			m_PowerSupplyGPIBAddr = deviceAddr[i];
			bPowerDevice1Found = TRUE;
			break;
		}
	}

	eGPIBDeviceType powerSupply2;
	for (int i = 0; i < iMaxDeviceCount; i++) {
		if (device[i] >= HpE3631 && device[i] <= Hp66332A && m_PowerSupplyGPIBAddr != deviceAddr[i]) {
			powerSupply2 = (eGPIBDeviceType)device[i];
			bPowerDevice2Found = TRUE;
			break;
		}
	}

	// output detection result
	std::string strOutput;
	if (!bTesterDeviceFound) {
		OutputData("TestSet", "N/A", strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("TestSet: N/A"));
	}
	else {		
		switch (m_TesterDevice) {
			case RsCmu200:
				OutputData("TestSet", "CMU 200", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("TestSet: CMU200"));
				break;
			case Hp8960:
				OutputData("TestSet", "HP8960", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("TestSet: HP8960"));
				break;
			case WillTek:
				OutputData("TestSet", "WillTek", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("TestSet: WillTek"));
				break;
			default:
				ASSERT(0);
				OutputData("TestSet", "Unknown", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("TestSet: Unknown"));
				break;
		}
	}

	if (!bPowerDevice1Found) {
		OutputData("PowerSupply1", "N/A", strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply1: N/A"));
	}
	else {
		switch (m_PowerSupplyDevice) {
			case HpE3631:
				OutputData("PowerSupply1", "HpE3631", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply1: HpE3631"));
				break;
			case Hp66311:
				OutputData("PowerSupply1", "Hp66311", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply1: Hp66311"));
				break;
			case Hp66332A:
				OutputData("PowerSupply1", "Hp66332A", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply1: Hp66332A"));
				break;
			default:
				ASSERT(0);
				OutputData("PowerSupply1", "Unknown", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply1: Unknown"));
				break;
		}
	}

	if (!bPowerDevice2Found) {
		OutputData("PowerSupply2", "N/A", strOutput);
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply2: N/A"));
	}
	else {
		switch (powerSupply2) {
			case HpE3631:
				OutputData("PowerSupply2", "HpE3631", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply2: HpE3631"));
				break;
			case Hp66311:
				OutputData("PowerSupply2", "Hp66311", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply2: Hp66311"));
				break;
			case Hp66332A:
				OutputData("PowerSupply2", "Hp66332A", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply2: Hp66332A"));
				break;
			default:
				ASSERT(0);
				OutputData("PowerSupply2", "Unknown", strOutput);
				ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("PowerSupply2: Unknown"));
				break;
		}
	}
	return bTesterDeviceFound;
}

BOOL CCalibrationTest::RFCalTest_Lavernock(LPCTSTR lpszFunc, LPCTSTR lpszItems)
{
	std::string sAbortStatus;
	CheckAbortStatus(sAbortStatus);
	if (sAbortStatus == _T("True")) 
		return TRUE;

	// load required xml file
	CString strRFXMLFilePath = GetRFXMLFilePath();
	CMyMSXML rfXML;
	if (rfXML.Load(strRFXMLFilePath) != 0) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("Load file failed - %s"), strRFXMLFilePath);
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Load file success - %s"), strRFXMLFilePath);

	CString strConfigXMLFilePath = GetRFConfigXMLFilePath();
	CMyMSXML configXML;
	if (configXML.Load(strConfigXMLFilePath) != 0) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("Load file failed - %s"), strConfigXMLFilePath);
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Load file success - %s"), strConfigXMLFilePath);

	CStringArray vecItems;
	ParseCalTestItemsString(lpszItems, vecItems);
	DelNonSupportedBand(vecItems);
	
	// convert keyword UMTS to WCDMA
	for (int j = 0; j < vecItems.GetSize(); j++)
		for (int i = 0; i < sizeof(BANDMAPPINGTABLE)/sizeof(BANDMAPPINGTABLE[0])/2; i++ )
			vecItems[j].Replace(BANDMAPPINGTABLE[2*i], BANDMAPPINGTABLE[(2*i)+1]);

	CString strUpdatedBands = vecItems.GetAt(0);
	for (int i = 1; i < vecItems.GetSize(); i++)
		strUpdatedBands = strUpdatedBands + _T(",") + vecItems.GetAt(i);

	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_LOW, _T("BandItems: %s"), strUpdatedBands);

	// get config settings from xml file0
	CCalTestSettingManager configSetting;
	configSetting.SetComPortNum(m_iComPort);
	configSetting.SetMessageHandlingHwnd(((CRFCalibrationToolApp*)AfxGetApp())->GetRFWnd()->GetSafeHwnd());
	configSetting.SetCalTestItems(lpszFunc, vecItems);
	configSetting.SetTesterDevice(m_TesterDevice);
	configSetting.SetPowerSupplyDevice(m_PowerSupplyDevice, m_PowerSupplyGPIBAddr);
	if (!configSetting.ConvertFrom(&rfXML, &configXML)) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("Get config settings failed"));
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Set config settings success"));

	//if (!configSetting.CheckPowerSupplyValid()) {
	//	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("Please check the the setting EquipmentSettings\\PowerSupplyId\\GpibAddress in the config.xml"));
	//	return FALSE;
	//}
	
	// pass config settings to RF.dll to do calibration/test	
	if (!m_pfCreateRFCALTEST(&m_piRFProcessor)) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Failed to create RFProcessor object"));
		m_piRFProcessor->Free();
		m_piRFProcessor = NULL;
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Create RFProcessor object success"));

	if (!m_piRFProcessor->SetLogger(ToolFactoryLog, ToolTraceLog, ToolCommandLog)) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Set log functions failed"));
		m_piRFProcessor->Free();
		m_piRFProcessor = NULL;
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Set log functions success"));

	if (!m_piRFProcessor->InitData(configSetting.GetConfigSetting())) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Initial RFProcessor failed"));
		m_piRFProcessor->Free();
		m_piRFProcessor = NULL;
		std::string sAbortStatus;
		CheckAbortStatus(sAbortStatus);
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("Initial RFProcess success"));

	if (!m_piRFProcessor->StartProcess()) {
		ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("CalTest Process failed"));
		m_piRFProcessor->Free();
		m_piRFProcessor = NULL;
		return FALSE;
	}
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_WARNING, _T("CalTest Process success"));

	m_piRFProcessor->Free();
	m_piRFProcessor = NULL;
	ToolTraceLog(MODULENAME, __FILE__, __LINE__, MSG_HIGH, _T("----- END -----"));	

	return TRUE;
}

VOID CCalibrationTest::ParseCalTestItemsString(LPCTSTR lpszItems, CStringArray &vecItems)
{
	vecItems.RemoveAll();

	CString strItems = lpszItems;
	//for (int i = 0; i < sizeof(BANDMAPPINGTABLE)/sizeof(BANDMAPPINGTABLE[0])/2; i++ )
	//	strItems.Replace(BANDMAPPINGTABLE[2*i], BANDMAPPINGTABLE[(2*i)+1]);

	int iCurPos = 0;
	CString strSeparate = _T("|");
	CString strToken= strItems.Tokenize(strSeparate, iCurPos);
	while (strToken != _T("")) {
		vecItems.Add(strToken);
		strToken= strItems.Tokenize(strSeparate, iCurPos);
	}
}

CString CCalibrationTest::GetRFXMLFilePath()
{
	CString strFilePath;
	switch (m_MobileModel) {
		case MOBILEMODEL_WELLFLEET:
			break;
		case MOBILEMODEL_LAVERNOCK:
			strFilePath = m_strModulePath + _T("\\RF_ServiceTool_Lavernock_GLAVS1A_V001.xml");
			break;
		case MOBILEMODEL_MELOS:
			strFilePath = m_strModulePath + _T("\\RF_ServiceTool_Melos_GMELB1A_V001.xml");
			break;
		case MOBILEMODEL_LAST:
			break;
		default:
			ASSERT(0);
			break;
	}

	return strFilePath;
}

CString CCalibrationTest::GetRFConfigXMLFilePath()
{
	CString strFilePath = m_strModulePath;
	strFilePath = strFilePath.Left(strFilePath.ReverseFind(_T('\\')));
	strFilePath = strFilePath + _T("\\config.xml");
	return strFilePath;
}
