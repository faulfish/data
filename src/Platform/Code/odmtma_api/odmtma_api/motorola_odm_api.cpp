#include "stdafx.h"
#include <io.h>
#include <windows.h>
#include <OLEAUTO.H>
#include "motorola_odm_api.h"
#include <string>
#include "InterfaceExport.h"
#include "../../CommonUtil/LogPolicy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char  THIS_FILE[] = __FILE__;
#endif


// Utility Functions signature
FARPROC GetCallback(char* pszFnName, FARPROC pFn=NULL);

typedef struct __tags { char *szTag; char * szValue; int nTagId;} __TAGS;
__VBDECLARATION ODM_IdentifyTag(   __TAGS *pTheTags, char*  szTag  );


/////////////////////////////////////////////////////////////////////////////////////////////////
//typedef bool (*pfnD_GetMotoUSBPort_LONG)(const std::string& strUSB,const std::string& strVID,const std::string& strPID,long& nCOMPort);
//pfnD_GetMotoUSBPort_LONG D_GetMotoUSBPort_LONG = NULL;

typedef bool (*pfnD_NewDiagExportInterface)(IDiagExport**);
pfnD_NewDiagExportInterface D_NewDiagExportInterface = NULL;

typedef bool (*pfnD_DeleteDiagExportInterface)(IDiagExport*);
pfnD_DeleteDiagExportInterface D_DeleteDiagExportInterface = NULL;

typedef void (*pfnD_Log)(const CString& strLog);
pfnD_Log D_Log = NULL;

/////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef _UNICODE
#define TRACE_FILE_LINE_INFO() do{\
	/*CString str;*/\
	/*str.Format(_T("file=%s,line=%u\r\n"),LogPolicy::ansitowide(__FILE__,CP_ACP).c_str(),__LINE__);*/\
	/*D_Log(str);*/\
}while(0)
#else
#define TRACE_FILE_LINE_INFO() do{\
	CString str;\
	str.Format(_T("file=%s,line=%u\r\n"),__FILE__,__LINE__);\
	if(D_Log)\
		D_Log(str);\
}while(0)
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////
// API calls start here //
//////////////////////////
IDiagExport* g_pIDiagExport = NULL;
HMODULE g_hSysDll = NULL;

bool FindMotoUSB(const std::string& strUSB,std::string& strSerial)
{

	bool bRes = false;
	using namespace std;
	string::size_type nFindIndex = strUSB.find("USB");

	if(nFindIndex != string::npos)
	{

		string strUSBNumber = strUSB.substr(nFindIndex + 3);
		const string strKey = "SOFTWARE\\Motorola\\NexTest\\CUsbCSequencer";
		/*
		[HKEY_LOCAL_MACHINE\SOFTWARE\Motorola\NexTest\CUsbCSequencer]
		"PathPrefix1"="7&31718826&0&0001"
		*/
		HKEY hKey;

		LONG lRet = ::RegOpenKeyExA( HKEY_LOCAL_MACHINE,strKey.c_str(),0, KEY_READ, &hKey );

		if( lRet == ERROR_SUCCESS )
		{

			DWORD nBufferSize = 512;
			char szBuffer[512] = {0};
			string strField = "PathPrefix" + strUSBNumber;

			lRet = ::RegQueryValueExA( hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &nBufferSize);

			bRes = lRet == ERROR_SUCCESS;
			if(bRes)
			{	
				strSerial = szBuffer;

			}
			::RegCloseKey(hKey);

		}
	}
	return bRes;
}
bool TransferSerialinUSB(const std::string& strSerial,const std::string& strVID,const std::string& strPID, std::string& strM1Serial)
{
	/*
	HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Enum\USB\Vid_1d45&Pid_4510\"strSerial"
	"ParentIdPrefix"=strM1Serial
	*/
	bool bRes = false;
	using namespace std;

	//MI_01 always is DIAG port for now 2008/4/15
	const string strKeyFormat = "SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%s&Pid_%s\\%s";


	char szKey[1024] = {0};
	::_stprintf(szKey,strKeyFormat.c_str(),strVID.c_str(),strPID.c_str(),strSerial.c_str());
	HKEY hKey;

	if(ERROR_SUCCESS == ::RegOpenKeyExA( HKEY_LOCAL_MACHINE,szKey,0, KEY_READ, &hKey ))
	{
		DWORD nBufferSize = 512;
		char szBuffer[512] = {0};
		string strField = "ParentIdPrefix";

		LONG lRet = ::RegQueryValueExA( hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &nBufferSize);

		bRes = lRet == ERROR_SUCCESS;
		if(bRes)
		{
			strM1Serial = szBuffer;
		}
		::RegCloseKey(hKey);
	}


	return bRes;
}

bool FindComportFromSerial(std::string strSerial,const std::string& strVID,const std::string& strPID,std::string& strCOMPort)
{
	/*
	HKEY_LOCAL_MACHINE\SYSTEM\ControlSet001\Enum\USB\Vid_1d45&Pid_4510&MI_01\7&31718826&0&0001\Device Parameters
	"PortName"="COM87"
	*/
	bool bRes = false;
	using namespace std;

	//MI_01 always is DIAG port for now 2008/4/15
	const string strKeyFormat = "SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%s&Pid_%s&MI_01\\%s\\Device Parameters";	

	char szKey[1024] = {0};
	strSerial += "&0001";
	::_stprintf(szKey,strKeyFormat.c_str(),strVID.c_str(),strPID.c_str(),strSerial.c_str());
	HKEY hKey;

	LONG lRet = ::RegOpenKeyExA( HKEY_LOCAL_MACHINE,szKey,0, KEY_READ, &hKey );

	if( lRet == ERROR_SUCCESS )
	{
		DWORD nBufferSize = 512;
		char szBuffer[512] = {0};
		string strField = "PortName";

		lRet = ::RegQueryValueExA( hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &nBufferSize);

		bRes = lRet == ERROR_SUCCESS;
		if(bRes)
		{
			strCOMPort = szBuffer;

		}
		::RegCloseKey(hKey);

	}


	return bRes;
}

bool D_GetMotoUSBPort(const std::string& strUSB,const std::string& strVID,const std::string& strPID,std::string& strCOMPort)/*strUSB = "/USB3"*/
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;

	using namespace std;
	string strSerial;
	if(FindMotoUSB(strUSB,strSerial))
	{
		TRACE_FILE_LINE_INFO();
		string strM1Serial;
		if(TransferSerialinUSB(strSerial,strVID,strPID, strM1Serial))
			bRes = FindComportFromSerial(strM1Serial,strVID,strPID,strCOMPort);
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool D_GetMotoUSBPort_LONG(const std::string& strUSB,const std::string& strVID,const std::string& strPID,long& nCOMPort)
{
	using namespace std;
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	string strCOMPort;

	bRes = D_GetMotoUSBPort(strUSB,strVID,strPID,strCOMPort);
	TRACE_FILE_LINE_INFO();
	if(bRes)
	{		
		TRACE_FILE_LINE_INFO();
		string::size_type nFindIndex = strCOMPort.find("COM");
		if(nFindIndex != string::npos)
		{
			TRACE_FILE_LINE_INFO();
			string strCOMPortNumber = strCOMPort.substr(nFindIndex + 3);
			nCOMPort = ::atol(strCOMPortNumber.c_str());
			TRACE_FILE_LINE_INFO();
		}
	}
	return bRes;
}




bool GetComPort(long& nPort, char* szPortName)
{
	bool bRes = true;
	if(szPortName)
	{
		TRACE_FILE_LINE_INFO();
		bool bGetComPort = true;
		if(szPortName[0] == '\\')
		{
			TRACE_FILE_LINE_INFO();
			//bGetComPort = D_GetMotoUSBPort_LONG(szPortName, "1d45","4510", nPort);
			bGetComPort = D_GetMotoUSBPort_LONG(szPortName, "22b8","2d01", nPort);
		}
		else if(szPortName[0] == 'C' || szPortName[0] == 'c')
		{
			TRACE_FILE_LINE_INFO();
			char* szPort = NULL;
			szPort = szPortName+3;
			nPort = atoi(szPort);
			TRACE_FILE_LINE_INFO();
		}
		else
		{
			bRes = false;
			TRACE_FILE_LINE_INFO();
		}
	}
	return bRes;
}


__VBDECLARATION  ODM_Initialize( char * szInitializeParameters, char*  szErrMsg500)
{
	int nStatus = ODM_INITIALIZE_ERROR << 8 | TPTK_FAIL;
	if(szErrMsg500)
		_stprintf(szErrMsg500, _T("ODM Initialize Failed \n"));
	if(!g_hSysDll)
	{
	#ifdef _DEBUG
		g_hSysDll = ::LoadLibrary(_T("Diag_Export_StaticMD.dll"));
	#else
		g_hSysDll = ::LoadLibrary(_T("Diag_Export_StaticM.dll"));
	#endif
	}

	if(g_hSysDll)
	{
		//D_GetMotoUSBPort_LONG = (pfnD_GetMotoUSBPort_LONG)GetProcAddress(hSysDll, "D_GetMotoUSBPort_LONG");
		D_NewDiagExportInterface = (pfnD_NewDiagExportInterface)GetProcAddress(g_hSysDll, "D_NewDiagExportInterface");
		D_DeleteDiagExportInterface = (pfnD_DeleteDiagExportInterface)GetProcAddress(g_hSysDll, "D_DeleteDiagExportInterface");
		D_Log = (pfnD_Log)GetProcAddress(g_hSysDll, "D_Log");
		if(D_Log)
			D_Log(szInitializeParameters);

		if(D_NewDiagExportInterface)
			D_NewDiagExportInterface(&g_pIDiagExport);

		if(g_pIDiagExport)
		{
			using namespace std;
			string strInitParas = szInitializeParameters;
			basic_string <char>::size_type nFirst, nThird;
			nFirst = strInitParas.find(";", 0);
			nThird = strInitParas.find_last_of(";");

			if(nFirst != string::npos )
			{
				string strComPort = strInitParas.substr(0, nFirst);
				string strAlwaysFlash = strInitParas.substr(nThird+1, (strInitParas.size() - nThird -1));
				int nAlwaysFlash = 0;
				if(strAlwaysFlash.size()!=0)
					nAlwaysFlash = atoi(strAlwaysFlash.c_str());

				long nComPort = 0;
				if(GetComPort(nComPort, (char*)strComPort.c_str()))
				{
					g_pIDiagExport->InitializeParameter(nComPort, !(nAlwaysFlash == 1));
					TRACE_FILE_LINE_INFO();
					nStatus = TPTK_SUCCESS;
				}
			}
			else
				nStatus = TPTK_SUCCESS;
		}
	}
	

	

	TRACE_FILE_LINE_INFO();
	return nStatus;
}

__VBDECLARATION  ODM_UnInitialize( char*  szErrMsg500 )
{
	TRACE_FILE_LINE_INFO();
	//g_pIDiagExport->DeleteFiles();
	if(D_DeleteDiagExportInterface && g_pIDiagExport)
		D_DeleteDiagExportInterface(g_pIDiagExport);
	if(szErrMsg500)
		_stprintf(szErrMsg500, _T("ODM UnInitialize OK \n"));
	TRACE_FILE_LINE_INFO();
	if(g_hSysDll)
	{
		::FreeLibrary(g_hSysDll);
		g_hSysDll = NULL;
	}

	return TPTK_SUCCESS;
}

__VBDECLARATION	 ODM_ClearErrorMessage(char * szErrMsg500)
{
	TRACE_FILE_LINE_INFO();
	if(szErrMsg500)
	{
		_stprintf(szErrMsg500, "");
		TRACE_FILE_LINE_INFO();
	}
	ASSERT(0);
	TRACE_FILE_LINE_INFO();
	return TPTK_SUCCESS;
}
__VBDECLARATION  ODM_ClearFilePath(char * szFilePath )
{
	TRACE_FILE_LINE_INFO();
	if(szFilePath)
	{
		TRACE_FILE_LINE_INFO();
		_stprintf(szFilePath, "");
	}
	ASSERT(0);
	TRACE_FILE_LINE_INFO();
	return TPTK_SUCCESS;
}
__VBDECLARATION  ODM_CheckDataSize(long nOdmLength, long &nMotoLen, char * szDataType, char * szErrMsg500) //x
{
	TRACE_FILE_LINE_INFO();
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_CheckDataSize is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION	 ODM_AddPortToString(char * szPortszErrMsg500 , char * szPortName, char *szErrMsg500 )
{
	TRACE_FILE_LINE_INFO();
	if(szErrMsg500)
		_stprintf(szPortszErrMsg500, _T("[%s] %s"), szPortName, szErrMsg500);
	ASSERT(0);
	return TPTK_SUCCESS;
}

__VBDECLARATION  ODM_APIVersion( char * szApiVersion1024 , char *szErrMsg500 )
{
	if(szErrMsg500)
		_stprintf(szErrMsg500, "OK!");
	CString strVersion = "odmtpa_qisda_wellfleet_REL.01.00.02";
	if(szApiVersion1024)
		_stprintf(szApiVersion1024, (char*)(LPCTSTR)strVersion);
	return TPTK_SUCCESS;
}



__VBDECLARATION  ODM_ConnectCommPort( long &nPort, char*   szPortName, char*  szErrMsg500 )
{
	TRACE_FILE_LINE_INFO();
	int nStatus = ODM_CONNECTCOMMPORT_ERROR << 8 | TPTK_FAIL;

	if(szPortName && szErrMsg500)
	{
		TRACE_FILE_LINE_INFO();
		using namespace std;
		string strParas = szPortName;
		basic_string <char>::size_type nFirst;
		nFirst = strParas.find(";", 0);
		
		string strComPort = nFirst != string::npos ? strParas.substr(0, nFirst) : szPortName;
#ifndef DISABLE_LOG_FUNCTION
		{
			CStringA strFileName = CStringA(strComPort.c_str()) + ".txt";
			char szBeginPort[10] = {0};
			ltoa(nPort, szBeginPort, 10);
			std::string strLog = "ODM_ConnectCommPort : ";
			strLog += szBeginPort;
			strLog += " , ";
			strLog += szPortName ;
			LogPolicy::FunctionLog(strLog.c_str(), strFileName);
		}
#endif
		
		bool bGetComPort = GetComPort(nPort, (char*)strComPort.c_str());
		if(bGetComPort && g_pIDiagExport)
		{
			g_pIDiagExport->SetUsbIndex(nPort, strComPort.c_str());
			nStatus = g_pIDiagExport->ConnectComPort(nPort) ? TPTK_SUCCESS : nStatus;
			g_pIDiagExport->SetUsbIndex(nPort, strComPort.c_str());
		}
		else
			_stprintf(szErrMsg500, "%d:ODM_ConnectCommPort Failed> Can't get comport", nPort);

#ifndef DISABLE_LOG_FUNCTION
		{
			CStringA strFileName = CString(strComPort.c_str()) + ".txt";
			char szBeginPort[10] = {0};
			ltoa(nPort, szBeginPort, 10);
			std::string strLog = "ODM_ConnectCommPort : ";
			strLog += szBeginPort;
			strLog += " , ";
			strLog += szPortName ;
			LogPolicy::FunctionLog(strLog.c_str(), strFileName);
		}
		{
			CStringA strFileName = CString(strComPort.c_str()) + ".txt";
			char szBeginPort[10] = {0};
			ltoa(nPort, szBeginPort, 10);
			std::string strLog = "ODM_ConnectCommPort : ";
			if(nStatus == TPTK_SUCCESS)
				strLog += "PASS";
			else
				strLog += "FAIL";
			LogPolicy::FunctionLog(strLog.c_str(), strFileName);
		}
#endif
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ConnectCommPort Failed> Invalid function parameter", nPort);
	/*if(nStatus != TPTK_SUCCESS)
		AfxMessageBox("ConnectComPort Fail");*/
//	ASSERT(nStatus == TPTK_SUCCESS);
	return nStatus;
}


__VBDECLARATION ODM_DisConnectCommPort( long &nPort, char*   szPortName, char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strFileName = CString(szPortName) + ".txt";
	LogPolicy::FunctionLog("ODM_DisConnectCommPort", strFileName);
#endif
	TRACE_FILE_LINE_INFO();
	int nStatus = ODM_DISCONNECTCOMMPORT_ERROR << 8 | TPTK_FAIL;
	if(szPortName)
	{
		TRACE_FILE_LINE_INFO();
		bool bGetComPort = GetComPort(nPort, szPortName);
		if(bGetComPort && g_pIDiagExport)
		{	
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->DisconnectComPort(nPort) ? TPTK_SUCCESS : nStatus;
			TRACE_FILE_LINE_INFO();
		}
		else if(!bGetComPort)
		{
			TRACE_FILE_LINE_INFO();
			if(szErrMsg500)
				_stprintf(szErrMsg500, "%d:ODM_DisConnectCommPort Failed> Can't get comport", nPort);
		}
		else
			nStatus = TPTK_SUCCESS;
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ConnectCommPort Failed> Invalid function parameter", nPort);
	
	ASSERT(nStatus == TPTK_SUCCESS);
	TRACE_FILE_LINE_INFO();
	return nStatus;
}


// Unit power up, down and restart

__VBDECLARATION ODM_IsUnitOn( long nPort, long& nUnitOnStatus, char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_IsUnitOn", strFileName);
	}
#endif
	int nStatus = ODM_ISUNITON_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{
		nUnitOnStatus = g_pIDiagExport->IsUnitOn(nPort) ? 1 : 0;
		nStatus = TPTK_SUCCESS;
	}
	else
	{ 
		if(szErrMsg500)
			_stprintf(szErrMsg500, "%d:ODM_IsUnitOn> Dll error", nPort);
	}
	ASSERT(nStatus == TPTK_SUCCESS);
	TRACE_FILE_LINE_INFO();
	return nStatus;
}

__VBDECLARATION ODM_TurnUnitOn( long nPort, long nSleepTime, long nAttempts, char*  szErrMsg500 )
{ // always return TPTK_SUCCESS
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_TurnUnitOn", strFileName);
	}
#endif
	TRACE_FILE_LINE_INFO();
	int nStatus = TPTK_SUCCESS;
	for(long i =0; i<nAttempts; i++)
		Sleep(nSleepTime);
	ASSERT(nStatus == TPTK_SUCCESS);
	TRACE_FILE_LINE_INFO();
	return nStatus;
}

__VBDECLARATION ODM_IsUnitOff( long nPort, long &nUnitOffStatus, char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_IsUnitOff", strFileName);
	}
#endif
	int nStatus = ODM_ISUNITOFF_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{
		nUnitOffStatus = g_pIDiagExport->IsUnitOff(nPort) ? 1 : 0;
		nStatus = TPTK_SUCCESS;
	}
	else
		_stprintf(szErrMsg500, "%d:ODM_IsUnitOff> Dll error", nPort);

	ASSERT(nStatus == TPTK_SUCCESS);
	return nStatus;
}

__VBDECLARATION ODM_TurnUnitOff( long nPort, char*  szErrMsg500 )
{ // reset unit and don't accept any commands
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_TurnUnitOff", strFileName);
	}
#endif
	int nStatus = ODM_TURNUNITOFF_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{
		/*for(int i=0; i<5 && nStatus==(ODM_TURNUNITOFF_ERROR<< 8 | TPTK_FAIL); i++)
		{*/
			nStatus = g_pIDiagExport->TurnUnitOff(nPort) ? TPTK_SUCCESS : nStatus;
			TRACE_FILE_LINE_INFO();
		/*}*/
		if(nStatus!=TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_TURNUNITOFF_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
		}
			
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_TurnUnitOff> Dll error", nPort);
	
	ASSERT(nStatus == TPTK_SUCCESS);
	return nStatus;
}

__VBDECLARATION ODM_RestartUnit( long nPort, char*  szErrMsg500 )
{ //restart to normal mode.
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_RestartUnit", strFileName);
	}
#endif
	int nStatus = ODM_RESTARTUNIT_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{
		/*for(int i=0; i<5 && nStatus==TPTK_FAIL; i++)
		{*/
			nStatus = g_pIDiagExport->Restart(nPort)?TPTK_SUCCESS: nStatus;
			TRACE_FILE_LINE_INFO();
		//}
		if(nStatus!=TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_RESTARTUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_RestartUnit> Dll error", nPort);
	
	ASSERT(nStatus == TPTK_SUCCESS);
	return nStatus;
}


// Test Mode, Status Check and Master Clear

__VBDECLARATION ODM_SetTestMode( long nPort, char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_SetTestMode", strFileName);
	}
#endif
	int nStatus = ODM_SETTESTMODE_ERROR << 8 | TPTK_FAIL;
	
	if(g_pIDiagExport)
	{
		TRACE_FILE_LINE_INFO();
		bool bSetTestMode = false;
		for(int i=0; i<5 && !bSetTestMode; i++)
		{
			bSetTestMode = g_pIDiagExport->SetTestMode(nPort);
			TRACE_FILE_LINE_INFO();
		}
		if(bSetTestMode)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = TPTK_SUCCESS;
		}
		else
		{
			TRACE_FILE_LINE_INFO();
			if(szErrMsg500)
			{
				if(g_pIDiagExport->GetLastError(nPort) == 4)
					_stprintf(szErrMsg500, "%d:ODM_SetTestMode> Not in Test Mode", nPort);
				else
					g_pIDiagExport->GetLastError(nPort ,_T("NVModule"), szErrMsg500);				
				nStatus = ODM_SETTESTMODE_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
			
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_SetTestMode> Dll error", nPort);
	ASSERT(nStatus == TPTK_SUCCESS);
	return nStatus;
}

__VBDECLARATION ODM_IsProgrammingComplete(long nPort,  long &nCompleteStatus, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_IsProgrammingComplete", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_IsProgrammingComplete is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_MasterClear( long nPort,  char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_MasterClear", strFileName);
	}
#endif
	int nStatus = ODM_MASTERCLEAR_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->MasterClear(nPort) ? TPTK_SUCCESS : nStatus;
		}
		if(nStatus!=TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_MASTERCLEAR_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_MasterClear> Dll error", nPort);
	return nStatus;
}


// Unit Processing information

__VBDECLARATION ODM_ReadTrackId( long nPort,  char*  szTrackId100, char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadTrackId", strFileName);
	}
#endif
	int nStatus = ODM_READTRACKID_ERROR << 8 | TPTK_FAIL;
	if(szTrackId100 && g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		ZeroMemory(szTrackId100, sizeof(szTrackId100));
		nStatus = g_pIDiagExport->ReadTrackID(nPort, szTrackId100) ? TPTK_SUCCESS : nStatus;
		if(strlen(szTrackId100) != 10)
			nStatus = ODM_READTRACKID_ERROR << 8 | TPTK_FAIL;
		if(nStatus!=TPTK_SUCCESS && g_pIDiagExport->RestoreNV(nPort, 40000))
		{
			nStatus = g_pIDiagExport->ReadTrackID(nPort, szTrackId100) ? TPTK_SUCCESS : nStatus;
		
			if(strlen(szTrackId100) != 10)
				nStatus = ODM_READTRACKID_ERROR << 8 | TPTK_FAIL;
		}
		if(nStatus!=TPTK_SUCCESS) 
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_READTRACKID_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ReadTrackId> Dll error or Invalid Parameter", nPort);
	return nStatus;
}
__VBDECLARATION ODM_ReadBarcode( long nPort,  char * szBarcode100, char * szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadBarcode", strFileName);
	}
#endif
	int nStatus = ODM_READBARCODE_ERROR << 8 | TPTK_FAIL;
	if(szBarcode100 && g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		ZeroMemory(szBarcode100, sizeof(szBarcode100));
		for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->ReadTrackID(nPort, szBarcode100) ? TPTK_SUCCESS : nStatus;
		}
		if(nStatus!=TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_READBARCODE_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ReadBarcode> Dll error or Invalid Parameter", nPort);
	return nStatus;
}

__VBDECLARATION ODM_ReadTransceiverKitNumber(long nPort, char* szTransceiverKitNumber100, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadTransceiverKitNumber", strFileName);
	}
#endif
	int nStatus = ODM_READTRANSCEIVERKITNUMBER_ERROR << 8 | TPTK_FAIL;
	if(szTransceiverKitNumber100 && g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		sprintf(szTransceiverKitNumber100, "");
		for(int i=0; i<10 && (nStatus != TPTK_SUCCESS); i++)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->ReadTransceiverKitNumber(nPort, szTransceiverKitNumber100) ? TPTK_SUCCESS : nStatus;
			if(strlen(szTransceiverKitNumber100) != 10)
				nStatus = ODM_READTRANSCEIVERKITNUMBER_ERROR << 8 | TPTK_FAIL;
			if(nStatus != TPTK_SUCCESS)
				Sleep(1000);
		}
		if(nStatus!=TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_READTRANSCEIVERKITNUMBER_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
			//AfxMessageBox("ReadXcrs fails!");
		}
	}
	else if(szErrMsg500)
	{
		_stprintf(szErrMsg500, "%d:ODM_ReadTransceiverKitNumber> Dll error or Invalid Parameter", nPort);
		nStatus = ODM_READTRANSCEIVERKITNUMBER_ERROR << 8 |  4;
	}
	return nStatus;
}
__VBDECLARATION ODM_ReadFactoryInfoBlock( long nPort, char * szFactoryInfoBlock1024, long &nLength, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadFactoryInfoBlock", strFileName);
	}
#endif
	int nStatus = ODM_READFACTORYINFOBLOCK_ERROR << 8 | TPTK_FAIL;
	if(szFactoryInfoBlock1024 && g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		ZeroMemory(szFactoryInfoBlock1024, sizeof(szFactoryInfoBlock1024));
		for(int i=0; i< 10 && (nStatus != TPTK_SUCCESS); i++)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->ReadFactoryInforBlock(nPort, szFactoryInfoBlock1024, nLength) ? TPTK_SUCCESS : nStatus;
			if(strlen(szFactoryInfoBlock1024) != 2)
				nStatus = ODM_READFACTORYINFOBLOCK_ERROR << 8 | TPTK_FAIL;
		}
		if(nStatus == TPTK_SUCCESS && (0 != strcmp(szFactoryInfoBlock1024, "be") && 0 != strcmp(szFactoryInfoBlock1024, "9e") ))
		{
			nStatus = ODM_READFACTORYINFOBLOCK_ERROR << 8 | TPTK_FAIL;
			if(szErrMsg500)
				_stprintf(szErrMsg500, "It hasn't passed all stations. This FactoryInfoBlock is %s", szFactoryInfoBlock1024);
		}
		else
		{
			//AfxMessageBox("ReadFactory block fails!");
			if(szErrMsg500)
				_stprintf(szErrMsg500, "%d> ODM_ReadFactoryInfoBlock Failed: Can't read FactoryInfoBlock", nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ReadFactoryInfoBlock> Dll error or Invalid Parameter", nPort);
	return nStatus;
}
__VBDECLARATION ODM_SetFactoryInfoBlock( long nPort, char * szFactoryInfoBlock, long nLength, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_SetFactoryInfoBlock", strFileName);
	}
#endif
	int nStatus = ODM_SETFACTORYINFOBLOCK_ERROR << 8 | TPTK_FAIL;
	if(szFactoryInfoBlock && g_pIDiagExport && strlen(szFactoryInfoBlock) == 2)
	{	
		TRACE_FILE_LINE_INFO();
		for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->WriteFactoryInforBlock(nPort, szFactoryInfoBlock) ? TPTK_SUCCESS : nStatus;
		}
		if(nStatus != TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_SETFACTORYINFOBLOCK_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ReadFactoryInfoBlock> Dll error or Invalid Parameter", nPort);
	return nStatus;
}
__VBDECLARATION ODM_SetInFactoryFlag(long nPort,  long nOnOff, char * szErrMsg500)
{ //nOnOff - 0:out of Factory
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_SetInFactoryFlag", strFileName);
	}
#endif
	int nStatus = ODM_SETINFACTORYFLAG_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
		{
			TRACE_FILE_LINE_INFO();
			bool bOnOff = (nOnOff==0) ? false : true;
			nStatus = g_pIDiagExport->SetInFactoryFlag(nPort, bOnOff) ? TPTK_SUCCESS : nStatus;
		}
		if(nStatus != TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_SETINFACTORYFLAG_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_SetInFactoryFlag> Dll error", nPort);
	return nStatus;
}
__VBDECLARATION ODM_FactoryFlexSwitch(long nPort,  long nToFactory, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_FactoryFlexSwitch", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_FactoryFlexSwitch is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}

__VBDECLARATION ODM_ReadUnitInfo( long nPort,  char * szUnitInfo1024, long &nLength, char * szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadUnitInfo", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ReadUnitInfo is not Implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}


__VBDECLARATION ODM_ProgramIMEI(long nPort, char * szIMEI, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramIMEI", strFileName);
	}
#endif
	int nStatus = ODM_PROGRAMIMEI_ERROR << 8 | TPTK_FAIL;
	if(szIMEI && g_pIDiagExport)
	{
		char szUnitIMEI[100] = {0};
		if(!( ODM_ReadIMEI(nPort, szUnitIMEI, szErrMsg500) == TPTK_SUCCESS  
			&& strcmp(szUnitIMEI, "000000011234560")==0 && strcmp(szIMEI, "000000011234560")==0 ))
		{
			long nSPCStatus = 1, nSSCPStatus = 1, nNWSCPStatus = 1;
			if(g_pIDiagExport->IsSPCLocked(nPort, nSPCStatus) && nSPCStatus==0
			&& g_pIDiagExport->IsUnitLocked(nPort, nNWSCPStatus, 0) && nNWSCPStatus==0
			&& g_pIDiagExport->IsUnitLocked(nPort, nSSCPStatus, 1) && nSSCPStatus==0)
			{
				nStatus = g_pIDiagExport->ProgramIMEI(nPort, szIMEI) ? TPTK_SUCCESS : nStatus;
				
				if(nStatus != TPTK_SUCCESS)
				{
					if(szErrMsg500)
						g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					nStatus = ODM_PROGRAMIMEI_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
				}
			}
			else
			{
				nStatus = ODM_PROGRAMIMEI_ERROR << 8 | 25;
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d:ODM_ProgramIMEI> SPC, NWSCP or SSCP locked", nPort);
			}
		}
		else
		{
			nStatus = TPTK_SUCCESS;
		}

	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ProgramIMEI> Dll error or Invalid Parameter", nPort);
	ASSERT(nStatus == TPTK_SUCCESS);
	return nStatus;
}


__VBDECLARATION ODM_ReadIMEI( long nPort, char*   szIMEI100, char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadIMEI", strFileName);
	}
#endif
	int nStatus = ODM_READIMEI_ERROR << 8 | TPTK_FAIL;
	if(szIMEI100 && g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		_stprintf(szIMEI100, "");
		for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->ReadIMEI(nPort, szIMEI100) ? TPTK_SUCCESS : ODM_READIMEI_ERROR;
		}
		if(nStatus != TPTK_SUCCESS)
		{
			//AfxMessageBox("ReadIMEI fails!");
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_READIMEI_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ReadIMEI> Dll error or Invalid Parameter", nPort);
	ASSERT(nStatus == TPTK_SUCCESS);
	TRACE_FILE_LINE_INFO();
	return nStatus;
}

__VBDECLARATION ODM_VerifyIMEI(long nPort, char * szIMEI, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_VerifyIMEI", strFileName);
	}
#endif
	int nStatus = ODM_VERIFYIMEI_ERROR << 8 | TPTK_FAIL;
	char szIMEI100[100] = {0};
	if(szIMEI && szErrMsg500)
	{
		nStatus = ODM_ReadIMEI(nPort, szIMEI100, szErrMsg500);
		if(nStatus == TPTK_SUCCESS)
		{
			nStatus = (0== strcmp(szIMEI100, szIMEI)) ? TPTK_SUCCESS : (ODM_VERIFYIMEI_ERROR << 8 | TPTK_FAIL);
			if(nStatus !=TPTK_SUCCESS && szErrMsg500)
				_stprintf(szErrMsg500, "%d> ODM_VerifyIMEI Failed: OrginalIMEI=%s", nPort, szIMEI100);
		}
		else
		{
			g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_VERIFYIMEI_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
		}
	}
	else
		_stprintf(szErrMsg500, "%d:ODM_VerifyIMEI> Invalid Parameter", nPort);
	return nStatus;
}
__VBDECLARATION ODM_ProgramESN(long nPort, char * szEsn, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramESN", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramESN is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}

__VBDECLARATION ODM_ReadESN( long nPort, char*   szEsn100, char*  szErrMsg500 ) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadESN", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ReadESN is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_VerifyESN(long nPort, char * szEsn, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_VerifyESN", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_VerifyESN is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}

__VBDECLARATION ODM_CompareFlash(long nPort,  char* szFlashFilePath, char* szFlashFileType, char* szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_CompareFlash", strFileName);
	}
#endif
	int nStatus = ODM_COMPAREFLASH_ERROR << 8 | TPTK_FAIL;
	if(szFlashFilePath && szFlashFileType && 
		(strcmp(szFlashFileType, "FLASH_APP") == 0 || strcmp(szFlashFileType, "FLASH_FFS") == 0))
	{
		if(g_pIDiagExport)
		{
			char szReadValue1024[1024] = {0};
			long nLength = 0;
			if(TPTK_SUCCESS == ODM_ReadVersion(nPort, "SWVERSION", szReadValue1024, nLength, szErrMsg500 ) )
			{
				CStringA strFlashFilePath = szFlashFilePath;
				strFlashFilePath.MakeUpper();
				CStringA strReadValue = szReadValue1024;
				strReadValue.MakeUpper();
				if(strFlashFilePath == strReadValue)
				{
					nStatus = TPTK_SUCCESS;
				}
			}
			if(nStatus != TPTK_SUCCESS)
			{	
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d> ODM_CompareFlash: Flash version ain't the same", nPort);
				nStatus = ODM_COMPAREFLASH_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
			}
		}
		else
		{
			if(szErrMsg500)
				_stprintf(szErrMsg500, "%d> ODM_CompareFlash: dll error", nPort, nStatus);
		}
	}
	else
	{
		if(szErrMsg500)
			_stprintf(szErrMsg500, "%d> ODM_CompareFlash: wrong parameter or not supported flash type(0x%x)", nPort, nStatus);
	}
	return nStatus;
}
__VBDECLARATION ODM_CompareFlex(long nPort,  char * szFlexFilePath, char * szFlexFileType, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_CompareFlex", strFileName);
	}
#endif
	int nStatus = ODM_COMPAREFLEX_ERROR << 8 | TPTK_FAIL;
	if(szFlexFilePath && 
		(szFlexFileType && strcmp(szFlexFileType, "FLEX_APP") == 0 || strcmp(szFlexFileType, "FLEX_CUST") == 0))
	{
		if(g_pIDiagExport)
		{
			char szReadValue1024[1024] = {0};
			long nLength = 0;
			if(TPTK_SUCCESS == ODM_ReadVersion(nPort, "FLEXVERSION", szReadValue1024, nLength, szErrMsg500 ))
			{
				CStringA strFlexFilePath = szFlexFilePath;
				strFlexFilePath.MakeUpper();
				CStringA strReadValue = szReadValue1024;
				strReadValue.MakeUpper();
				if(strFlexFilePath == strReadValue)
					nStatus = TPTK_SUCCESS;
			}
			if(nStatus != TPTK_SUCCESS)
			{	
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d> ODM_CompareFlex: Flex version ain't the same", nPort);
				nStatus = ODM_COMPAREFLEX_ERROR << 8 |  g_pIDiagExport->GetLastError(nPort);
			}
		}
		else
		{
			if(szErrMsg500)
				_stprintf(szErrMsg500, "%d> ODM_CompareFlex: dll error", nPort, nStatus);
		}
	}
	else
	{
		if(szErrMsg500)
			_stprintf(szErrMsg500, "%d> ODM_CompareFlex: wrong parameter or not supported flex type(0x%x)", nPort, nStatus);
	}
	return nStatus;
}
__VBDECLARATION ODM_Flash(long nPort,  char* szFlashFilePath, char* szFlashFileType, char* szErrMsg500)
{
	int nStatus = ODM_FLASH_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{
		if(szFlashFileType && strcmp(szFlashFileType, "FLASH_RECOVER") == 0 && szFlashFilePath)
		{
			std::string strPort = "\\USB";
			char szPort[20] = {0};
			char* pszPort =_itoa(nPort, szPort, 10);
			if(pszPort)
				strPort += pszPort;
#ifndef DISABLE_LOG_FUNCTION
			CStringA strFileName = CStringA(strPort.c_str()) + ".txt";
			LogPolicy::FunctionLog("ODM_Flash:FLASH_RECOVER", strFileName);
#endif
			
			if(GetComPort(nPort, (char*)strPort.c_str()) && g_pIDiagExport->TempDisconnectComPort(nPort))
			{
				Sleep(3000);
				bool bConnect = g_pIDiagExport->ConnectComPort(nPort);
				int nTimeOut = 57000;
				int nSleep = 3000;

				for(int i = 0; i< nTimeOut/nSleep && !bConnect; i++)
				{
					Sleep(nSleep);
					bConnect = g_pIDiagExport->ConnectComPort(nPort);
				}
				
				if(bConnect)
				{
					nStatus = g_pIDiagExport->Flash(nPort, szFlashFilePath)? TPTK_SUCCESS : nStatus;
					TRACE_FILE_LINE_INFO();
					if(nStatus != TPTK_SUCCESS)
					{
						if(szErrMsg500)
							g_pIDiagExport->GetLastError(nPort, "Download", szErrMsg500);
						nStatus = ODM_FLASH_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
					}
				}
				else
				{
					if(szErrMsg500)
						_stprintf(szErrMsg500, "%d:ODM_Flash> Connection Fail", nPort);
					int nConnectionFail = 70;
					nStatus = ODM_FLASH_ERROR << 8 | nConnectionFail;
				}
			}
			else
			{
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d:ODM_Flash>Invalid function parameter", nPort);
				int nInvaild = 69;
				nStatus = ODM_FLASH_ERROR << 8 | nInvaild;
			}
		}
		else if(szFlashFileType && strcmp(szFlashFileType, "FLASH_EMU") == 0 )
		{
			std::string strPort = "\\USB";
			char szPort[20] = {0};
			char* pszPort =_itoa(nPort, szPort, 10);
			if(pszPort)
				strPort += pszPort;
#ifndef DISABLE_LOG_FUNCTION
			CStringA strFileName = CStringA(strPort.c_str()) + ".txt";
			LogPolicy::FunctionLog("ODM_Flash:FLASH_EMU", strFileName);
#endif

			if(GetComPort(nPort, (char*)strPort.c_str()) && g_pIDiagExport->TempDisconnectComPort(nPort))
			{
				Sleep(3000);
				bool bConnect = g_pIDiagExport->ConnectComPort(nPort);
				
				int nTimeOut = 57000;
				int nSleep = 3000;
				
				CTime InitTime = CTime::GetCurrentTime();
#ifndef DISABLE_LOG_FUNCTION
				CStringA strFileName = CStringA(strPort.c_str()) + ".txt";
				LogPolicy::FunctionLog("ODM_Flash:FLASH_EMU:initial time", strFileName);
#endif
				for(int i = 0; i< nTimeOut/nSleep && !bConnect; i++)
				{
					Sleep(nSleep);
					bConnect = g_pIDiagExport->ConnectComPort(nPort);
					CTime TempTime = CTime::GetCurrentTime();
					CTimeSpan tSpan = TempTime-InitTime;
					if(tSpan.GetTotalSeconds() > 60)
					{	
#ifndef DISABLE_LOG_FUNCTION
						CStringA strFileName = CStringA(strPort.c_str()) + ".txt";
						LogPolicy::FunctionLog("ODM_Flash:FLASH_EMU:tSpan.GetTotalSeconds() > 60", strFileName);
#endif
						break;
					}
				}
				
				if(bConnect)
				{
					bool bIsTestMode = g_pIDiagExport->SetTestMode(nPort);
					TRACE_FILE_LINE_INFO();
					if(bIsTestMode)
					{
						nStatus = 102;
						if(szErrMsg500)
							_stprintf(szErrMsg500, "%d:ODM_Flash> in Test Mode", nPort);
					}
					else if(!bIsTestMode && g_pIDiagExport->GetLastError(nPort) == 20)
					{
						nStatus = TPTK_SUCCESS;
					}
					else
					{
						if(szErrMsg500)
							_stprintf(szErrMsg500, "%d:ODM_Flash> in Normal Mode", nPort);
					}
				}
				else
				{
					if(szErrMsg500)
						_stprintf(szErrMsg500, "%d:ODM_Flash> Connection Fail", nPort);
					int nConnectionFail = 70;
					nStatus = ODM_FLASH_ERROR << 8 | nConnectionFail;
				}
			}
		}
		else
		{
			if(szFlashFilePath)
			{
#ifndef DISABLE_LOG_FUNCTION
				CStringA strUSBIndex;
				if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
				{
					CStringA strFileName = strUSBIndex + ".txt";
					LogPolicy::FunctionLog("ODM_Flash:FLASH_APP/FLASH_FFS", strFileName);
				}
#endif
				nStatus = g_pIDiagExport->Flash(nPort, szFlashFilePath)? TPTK_SUCCESS : nStatus;
				if(nStatus != TPTK_SUCCESS && g_pIDiagExport->GetLastError(nPort) == 61)
				{
					Sleep(10000);
#ifndef DISABLE_LOG_FUNCTION
					CStringA strUSBIndex;
					if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
					{
						CStringA strFileName = strUSBIndex + ".txt";
						LogPolicy::FunctionLog("ODM_Flash:FLASH_APP/FLASH_FFS - fail in hello and run Flash again", strFileName);
					}
#endif
					nStatus = g_pIDiagExport->Flash(nPort, szFlashFilePath)? TPTK_SUCCESS : nStatus;
				}
				if(nStatus != TPTK_SUCCESS)
				{
					if(szErrMsg500)
						g_pIDiagExport->GetLastError(nPort, "Download", szErrMsg500);
					nStatus = ODM_FLASH_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
				}
			}
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_Flash> Dll error", nPort);

	return nStatus;
}
__VBDECLARATION ODM_Flex(long nPort,  char* szFlexFilePath, char * szFlexFileType, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_Flex", strFileName);
	}
#endif
	int nStatus = ODM_FLEX_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{
		if(szFlexFileType 
			&& (strcmp(szFlexFileType, "FLEX_APP") == 0 || strcmp(szFlexFileType, "FLEX_CUST") == 0)
			&& szFlexFilePath)
		{
			TRACE_FILE_LINE_INFO();
			nStatus = g_pIDiagExport->Flex(nPort, szFlexFilePath)? TPTK_SUCCESS : nStatus;
			TRACE_FILE_LINE_INFO();
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
					g_pIDiagExport->GetLastError(nPort,  "Download", szErrMsg500);
				nStatus = ODM_FLEX_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else
		{
			nStatus = TPTK_NOTIMPLEMENTED;
			if(szErrMsg500)
				_stprintf(szErrMsg500, "%d:ODM_Flex> Not implemented", nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_Flex> Dll error", nPort);

	return nStatus;
}
__VBDECLARATION ODM_ProgramPhoneBook(long nPort,  char * szPhoneBookFilePath,  char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramPhoneBook", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramPhoneBook is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}

__VBDECLARATION ODM_ReadVersion( long nPort, char * szValueType,  char * szReadValue1024, long &nLength, char * szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadVersion", strFileName);
	}
#endif
	int nStatus = ODM_READVERSION_ERROR << 8 | TPTK_FAIL;

	if(szValueType && szReadValue1024 && g_pIDiagExport && szErrMsg500)
	{
		memset(szReadValue1024, 0 , sizeof(szReadValue1024));
		if(0 == strcmp(szValueType, "SWVERSION"))
		{
			if(g_pIDiagExport->ReadSWVersion(nPort, szReadValue1024, nLength))
			{
				CStringA strReadValue = szReadValue1024;
				strReadValue.MakeUpper();
				strcpy(szReadValue1024, (const char*)strReadValue);
				nStatus = TPTK_SUCCESS;
			}
			else
			{
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
				nStatus = ODM_READVERSION_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else if(0 == strcmp(szValueType, "FLEXVERSION"))
		{
			TRACE_FILE_LINE_INFO();
			if(g_pIDiagExport->ReadFlexVersion(nPort, szReadValue1024, nLength))
			{
				CStringA strReadValue = szReadValue1024;
				strReadValue.MakeUpper();
				strcpy(szReadValue1024, (const char*)strReadValue);
				nStatus = TPTK_SUCCESS;
			}	
			else
			{
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
				nStatus = ODM_READVERSION_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ReadVersion> Dll error or Invalid Parameter", nPort);
	return nStatus;
}


__VBDECLARATION ODM_ProgramBlueTooth(long nPort,  char * szAddress, char * szErrMsg500) 
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramBlueTooth", strFileName);
	}
#endif
	int nStatus = ODM_PROGRAMBLUETOOTH_ERROR << 8 | TPTK_FAIL;
	if(szAddress && g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		nStatus = g_pIDiagExport->WriteBTAddr(nPort, szAddress) ? TPTK_SUCCESS : nStatus;
		if(nStatus != TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_PROGRAMBLUETOOTH_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ProgramBlueTooth> Dll error or Invalid Parameter", nPort);
	ASSERT(nStatus == TPTK_SUCCESS);
	TRACE_FILE_LINE_INFO();
	return nStatus;
}
__VBDECLARATION ODM_ReadBlueTooth(long nPort,  char * szAddress100, long &nLength, char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadBlueTooth", strFileName);
	}
#endif
	int nStatus = ODM_READBLUETOOTH_ERROR << 8 | TPTK_FAIL;
	if(szAddress100 && g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		nStatus = g_pIDiagExport->ReadBTAddr(nPort, szAddress100, nLength) ? TPTK_SUCCESS : nStatus;
		if(nStatus != TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_READBLUETOOTH_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ReadBlueTooth> Dll error or Invalid Parameter", nPort);
	ASSERT(nStatus == TPTK_SUCCESS);
	TRACE_FILE_LINE_INFO();
	return nStatus;
}
__VBDECLARATION ODM_ProgramWLAN(long nPort,  char * szAddress, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramWLAN", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramWLAN is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_ReadWLAN(long nPort,  char * szAddress100, long &nLength, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadWLAN", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ReadWLAN is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}

__VBDECLARATION  ODM_SetRealTimeClock(long nPort,  char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_SetRealTimeClock", strFileName);
	}
#endif
	int nStatus = ODM_SETREALTIMECLOCK_ERROR << 8 | TPTK_FAIL;
	if(g_pIDiagExport)
	{	
		TRACE_FILE_LINE_INFO();
		nStatus = g_pIDiagExport->SetRTC(nPort)? TPTK_SUCCESS : nStatus;
		if(nStatus != TPTK_SUCCESS)
		{
			if(szErrMsg500)
				g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
			nStatus = ODM_SETREALTIMECLOCK_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_SetRealTimeClock> Dll error", nPort);

	return nStatus;
}

__VBDECLARATION ODM_IsUnitLocked(long nPort,  char* szLockType, long &nLockStatus, char * szErrMsg500)
{
	//Sleep(5000);
	int nStatus = ODM_ISUNITLOCKED_ERROR << 8 | TPTK_FAIL;
	if(szLockType && g_pIDiagExport)
	{
		if(strcmp(szLockType, "SERVICE_PASSWORD") == 0)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_IsUnitLocked:SERVICE_PASSWORD", strFileName);
			}
#endif
			TRACE_FILE_LINE_INFO();
			for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
			{
				TRACE_FILE_LINE_INFO();
				nStatus = g_pIDiagExport->IsSPCLocked(nPort, nLockStatus) ? TPTK_SUCCESS : nStatus;
			}
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
				{
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					std::string strErrMsg = "[SPC] ";
					strErrMsg += szErrMsg500;
					strcpy(szErrMsg500, strErrMsg.c_str());
				}
				nStatus = ODM_ISUNITLOCKED_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else if(strcmp(szLockType, "SUBSIDY_LOCK_GSM_NWSCP") == 0)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_IsUnitLocked:SUBSIDY_LOCK_GSM_NWSCP", strFileName);
			}
#endif
			TRACE_FILE_LINE_INFO();
			for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
			{
				TRACE_FILE_LINE_INFO();
				nStatus = g_pIDiagExport->IsUnitLocked(nPort, nLockStatus, 0) ? TPTK_SUCCESS : nStatus;
			}
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
				{
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					std::string strErrMsg = "[NWSCP] ";
					strErrMsg += szErrMsg500;
					strcpy(szErrMsg500, strErrMsg.c_str());
				}
				nStatus = ODM_ISUNITLOCKED_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else if(strcmp(szLockType, "SUBSIDY_LOCK_GSM_SSCP") == 0)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_IsUnitLocked:SUBSIDY_LOCK_GSM_SSCP", strFileName);
			}
#endif
	
			for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
			{
				TRACE_FILE_LINE_INFO();
				nStatus = g_pIDiagExport->IsUnitLocked(nPort, nLockStatus, 1) ? TPTK_SUCCESS : nStatus;
			}
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
				{
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					std::string strErrMsg = "[SSCP] ";
					strErrMsg += szErrMsg500;
					strcpy(szErrMsg500, strErrMsg.c_str());
				}
				nStatus = ODM_ISUNITLOCKED_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else
		{
			nStatus = TPTK_NOTIMPLEMENTED;
			if(szErrMsg500)
				_stprintf(szErrMsg500, "ODM_IsUnitLocked is not implemented");
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_IsUnitLocked> Dll error or Invalid Parameter", nPort);
	return nStatus;
}

__VBDECLARATION ODM_UnLockUnit(long nPort,  char * szLock, char* szLockType , long &nLockStatus, char * szErrMsg500)
{ // nLockStatus =  0 : unlocked, 1 : locked
	TRACE_FILE_LINE_INFO();
	int nStatus = ODM_UNLOCKUNIT_ERROR << 8 | TPTK_FAIL;
	if(szLockType && szLock && g_pIDiagExport)
	{
		nLockStatus = 0;
		if(strcmp(szLockType, "SERVICE_PASSWORD") == 0 && strlen(szLock) >= 6)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_UnLockUnit:SERVICE_PASSWORD", strFileName);
			}
#endif
			TRACE_FILE_LINE_INFO();
			bool bUnLockOk = true;
		
			nStatus = g_pIDiagExport->UnlockSPC(nPort, szLock, bUnLockOk) ? TPTK_SUCCESS : nStatus;
			if(!bUnLockOk)
			{	
				Sleep(3000);
				nStatus = g_pIDiagExport->UnlockSPC(nPort, "000000", bUnLockOk) ? TPTK_SUCCESS : nStatus;
				if(bUnLockOk)
					nLockStatus = 2;
			}
			if(!bUnLockOk)
			{
				nLockStatus = 1;
				nStatus = nStatus = ODM_UNLOCKUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
				{
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					std::string strErrMsg = "[SPC] ";
					strErrMsg += szErrMsg500;
					strcpy(szErrMsg500, strErrMsg.c_str());
				}
				nStatus = ODM_UNLOCKUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
			
			
		}
		else if(strcmp(szLockType, "SUBSIDY_LOCK_GSM_NWSCP") == 0 && strlen(szLock) == 8)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_UnLockUnit:SUBSIDY_LOCK_GSM_NWSCP", strFileName);
			}
#endif
			bool bNWUnlocked = false, bNSUnlocked = false, bSPUnlocked = false, bCPUnlocked = false, bSIMUnlocked = false;
			if(g_pIDiagExport->IsNWSCPLocked(nPort, bNWUnlocked, bNSUnlocked,  bSPUnlocked, bCPUnlocked, bSIMUnlocked))
			{
				bool bUnlockUnit = true;
				if(!bNWUnlocked)
					bUnlockUnit = g_pIDiagExport->UnLockUnit(nPort, 0, szLock, bNWUnlocked);
				if(bUnlockUnit && !bNSUnlocked)
					bUnlockUnit = g_pIDiagExport->UnLockUnit(nPort, 1, szLock, bNSUnlocked);
				if(bUnlockUnit && !bSPUnlocked)
					bUnlockUnit = g_pIDiagExport->UnLockUnit(nPort, 2, szLock, bSPUnlocked);
				if(bUnlockUnit && !bCPUnlocked)
					bUnlockUnit = g_pIDiagExport->UnLockUnit(nPort, 3, szLock, bCPUnlocked);
				if(bUnlockUnit && !bSIMUnlocked)
					bUnlockUnit = g_pIDiagExport->UnLockUnit(nPort, 4, szLock, bSIMUnlocked);
				nLockStatus = (bNWUnlocked && bNSUnlocked && bSPUnlocked && bCPUnlocked && bSIMUnlocked)? 0 : 1;
				if(nLockStatus == 1)
					nStatus = ODM_UNLOCKUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
				else
					nStatus = TPTK_SUCCESS;
			}
			else
				nStatus = ODM_UNLOCKUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
	
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
				{
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					std::string strErrMsg = "[NWSCP] ";
					strErrMsg += szErrMsg500;
					strcpy(szErrMsg500, strErrMsg.c_str());
				}
				nStatus = ODM_UNLOCKUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else if(strcmp(szLockType, "SUBSIDY_LOCK_GSM_SSCP") == 0 && strlen(szLock) == 8)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_UnLockUnit:SUBSIDY_LOCK_GSM_SSCP", strFileName);
			}
#endif
			TRACE_FILE_LINE_INFO();
			bool bUnLockOk = true;
			/*for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
			{*/
				TRACE_FILE_LINE_INFO();
				nStatus = g_pIDiagExport->UnLockPhoneCode(nPort, szLock, bUnLockOk) ? TPTK_SUCCESS : nStatus;
				nLockStatus = bUnLockOk ? 0 : 1;
			/*}*/
			if(nLockStatus == 1)
				nStatus = ODM_UNLOCKUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			else
				nStatus = TPTK_SUCCESS;
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
				{
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					std::string strErrMsg = "[NWSCP] ";
					strErrMsg += szErrMsg500;
					strcpy(szErrMsg500, strErrMsg.c_str());
				}
				nStatus = ODM_UNLOCKUNIT_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else
		{
			nStatus = TPTK_NOTIMPLEMENTED;
			if(szErrMsg500)
				_stprintf(szErrMsg500, "ODM_UnLockUnit is not implemented or Wrong Unlock Code");
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_UnLockUnit> Dll error or Invalid Parameter", nPort);
	return nStatus;
}
__VBDECLARATION ODM_ProgramLock(long nPort,  char * szLock, char* szLockType , char * szErrMsg500)
{
	int nStatus = ODM_PROGRAMLOCK_ERROR << 8 | TPTK_FAIL;
	if(szLockType && szLock && g_pIDiagExport)
	{
		if(strcmp(szLockType, "SERVICE_PASSWORD") == 0 && strlen(szLock) == 8)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_ProgramLock:SERVICE_PASSWORD", strFileName);
			}
#endif
			bool bUnLockOk = true;
			long nSPCStatus = 1;
			if(ODM_IsUnitLocked(nPort, "SERVICE_PASSWORD", nSPCStatus, szErrMsg500) == TPTK_SUCCESS && nSPCStatus!=1 )
			{
			/*for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
			{*/
					TRACE_FILE_LINE_INFO();
					nStatus = g_pIDiagExport->ProgramSPC(nPort, szLock) ? TPTK_SUCCESS : nStatus;
			/*}*/
				if(nStatus != TPTK_SUCCESS)
				{
					if(szErrMsg500)
						g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					nStatus = ODM_PROGRAMLOCK_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
				}
			}
			else
			{
				nStatus = ODM_PROGRAMLOCK_ERROR << 8 | 25;
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d:ODM_ProgramLock> SPC locked", nPort);
			}

		}
		else if(strcmp(szLockType, "SUBSIDY_LOCK_GSM_NWSCP") == 0 && strlen(szLock) == 8)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_ProgramLock:SUBSIDY_LOCK_GSM_NWSCP", strFileName);
			}
#endif

			bool bUnLockOk = true;
			for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
			{
				TRACE_FILE_LINE_INFO();
				nStatus = g_pIDiagExport->ProgramNWSCP(nPort, szLock) ? TPTK_SUCCESS : nStatus;
			}
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
				nStatus = ODM_PROGRAMLOCK_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else if(strcmp(szLockType, "SUBSIDY_LOCK_GSM_SSCP") == 0 && strlen(szLock) == 8)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_ProgramLock:SUBSIDY_LOCK_GSM_SSCP", strFileName);
			}
#endif
			bool bUnLockOk = true;
			for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
			{
				TRACE_FILE_LINE_INFO();
				nStatus = g_pIDiagExport->ProgramPhoneCode(nPort, szLock) ? TPTK_SUCCESS : nStatus;
			}
			if(nStatus != TPTK_SUCCESS)
			{
				if(szErrMsg500)
					g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
				nStatus = ODM_PROGRAMLOCK_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
		}
		else
		{
			nStatus = TPTK_NOTIMPLEMENTED;
			if(szErrMsg500)
				_stprintf(szErrMsg500, "ODM_ProgramLock is not implemented");
		}
	}
	else if(szErrMsg500)
		_stprintf(szErrMsg500, "%d:ODM_ProgramLock> Dll error or Invalid Parameter", nPort);
	return nStatus;
}
__VBDECLARATION ODM_ReadLock(long nPort,  char * szLock100, char* szLockType, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadLock", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ReadLock is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_ProgramAkey(long nPort,  char * szAkey, char* szAkeyType, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramAkey", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramAkey is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_ProgramMobileId(long nPort,  char * szMobileId, char* szIdType, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramMobileId", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramMobileId is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_ProgramSystemId(long nPort,  char * szSystemId, char* szIdType , char * szErrMsg500) //x
{	
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramSystemId", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramSystemId is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_ProgramSoc(long nPort,  char * szSoc, char* szSocType, char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramSoc", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramSoc is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_ProgramIMSI(long nPort,  char * szValue, char* szType , char * szErrMsg500) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ProgramIMSI", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ProgramIMSI is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}

__VBDECLARATION ODM_GenericWrite( long nPort, char*  szData1024, char*  szTag, char*  szErrMsg500 )
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_GenericWrite", strFileName);
		LogPolicy::FunctionLog(szTag, strFileName);
	}
#endif
	int nStatus = ODM_GENERICWRITE_ERROR << 8 | TPTK_FAIL;
	if(szTag && g_pIDiagExport)
	{
		if(strcmp(szTag, "TRACKID") == 0)
		{
			TRACE_FILE_LINE_INFO();
			if(szData1024 && strlen(szData1024) == 10)
			{
				for(int i=0; i<5 && nStatus != TPTK_SUCCESS; i++)
				{
					TRACE_FILE_LINE_INFO();
					nStatus = g_pIDiagExport->WriteTrackID(nPort, szData1024) ? TPTK_SUCCESS : nStatus;
				}
				if(nStatus != TPTK_SUCCESS  && szErrMsg500)
					_stprintf(szErrMsg500, "%d> Program TRACKID Failed", nPort);
				if(nStatus != TPTK_SUCCESS)
					nStatus = ODM_GENERICWRITE_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
			else
			{
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d> Program TRACKID Failed(0x%x): Parameter incorrect", nPort, nStatus);
			}
		}
		else if(strcmp(szTag, "SUG") == 0)
		{
			TRACE_FILE_LINE_INFO();
			if(szData1024 && strlen(szData1024) == 10)
			{
				for(int i=0; i<5 && (nStatus != TPTK_SUCCESS); i++)
				{
					TRACE_FILE_LINE_INFO();
					nStatus = g_pIDiagExport->WriteTransceiverKitNumber(nPort, szData1024) ? TPTK_SUCCESS : nStatus;
				}
				if(nStatus != TPTK_SUCCESS && szErrMsg500)
					_stprintf(szErrMsg500, "%d> Program Xcvr number Failed", nPort);
				if(nStatus != TPTK_SUCCESS)
					nStatus = ODM_GENERICWRITE_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
			}
			else
			{
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d> Program Xcvr number Failed: Parameter incorrect", nPort);
			}
		}
		else if(strcmp(szTag, "RUIM") == 0)
		{
#ifndef DISABLE_LOG_FUNCTION
			CStringA strUSBIndex;
			if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
			{
				CStringA strFileName = strUSBIndex + ".txt";
				LogPolicy::FunctionLog("ODM_GenericWrite::RUIM", strFileName);
			}
#endif
			long nSPCStatus = 1, nNWSCPStatus = 1, nSSCPStatus = 1;
			if(g_pIDiagExport->IsSPCLocked(nPort, nSPCStatus) && nSPCStatus==0
			&& g_pIDiagExport->IsUnitLocked(nPort, nNWSCPStatus, 0) && nNWSCPStatus==0
			&& g_pIDiagExport->IsUnitLocked(nPort, nSSCPStatus, 1) && nSSCPStatus==0)
			{
				nStatus = g_pIDiagExport->WriteSimData(nPort)? TPTK_SUCCESS : nStatus;
				if(nStatus == TPTK_SUCCESS)			
					nStatus = g_pIDiagExport->LockSPC(nPort) ? TPTK_SUCCESS : (ODM_GENERICWRITE_ERROR << 8 | TPTK_FAIL);
				if(nStatus != TPTK_SUCCESS)
				{
					if(szErrMsg500)
						g_pIDiagExport->GetLastError(nPort, "NVModule", szErrMsg500);
					nStatus = ODM_GENERICWRITE_ERROR << 8 | g_pIDiagExport->GetLastError(nPort);
				}
			}
			else
			{
				nStatus = ODM_GENERICWRITE_ERROR << 8 | 25;
				if(szErrMsg500)
					_stprintf(szErrMsg500, "%d:ODM_GenericWrite(WriteSimData) > SPC, NWSCP or SSCP locked", nPort);
			}
		}
		else
		{
			nStatus = TPTK_NOTIMPLEMENTED;
			if(szErrMsg500)
				_stprintf(szErrMsg500, "ODM_GenericWrite is not implemented");
		}
	}
	return nStatus;
}

__VBDECLARATION ODM_GenericRead( long nPort, long nDSize, char* szData, char* szTag,char* szErrMsg500 ) //x
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_GenericRead", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_GenericRead is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}
__VBDECLARATION ODM_ExecuteFlow(long nPort, char *szFileName, char *szErrMsg500)
{
	TRACE_FILE_LINE_INFO();
	return TPTK_SUCCESS;
}
__VBDECLARATION ODM_ReadBatteryLevel(long nPort,  char * szLevel100,  char * szErrMsg500)
{
#ifndef DISABLE_LOG_FUNCTION
	CStringA strUSBIndex;
	if(g_pIDiagExport && g_pIDiagExport->GetUsbIndex(nPort, strUSBIndex))
	{
		CStringA strFileName = strUSBIndex + ".txt";
		LogPolicy::FunctionLog("ODM_ReadBatteryLevel", strFileName);
	}
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500, "ODM_ReadBatteryLevel is not implemented");
	ASSERT(0);
	return TPTK_NOTIMPLEMENTED;
}


__VBDECLARATION ODM_RunTestProfile(int nRef,char * szModeType ,  char * szErrMsg500)
{
	TRACE_FILE_LINE_INFO();
	int nStatus	= TPTK_SUCCESS;
	
		// do some stuff
	//...
	//
#if 0
	// Now need to call fn from app
	int(*pExecuteExternalFunction)(int nRef, char*, char *)=(int(*)(int, char*, char *))GetCallback("ExecuteExternalFunction");
	// if (pfnDoSomething)
	nStatus=(*pExecuteExternalFunction)(nRef, "Teste","Teste");

	// do some more stuff
	//...
	//
#endif
	if(szErrMsg500)
		_stprintf(szErrMsg500,"ODM_RunTestProfile Sample Test OK!\n");

	ASSERT(0);
	return nStatus;
}

__VBDECLARATION  ODM_SetCallBack(char* szFnName, FARPROC pFn,char * szErrMsg500)
{
	TRACE_FILE_LINE_INFO();
	// set pointer to fn for szFnName
	GetCallback(szFnName, pFn);
	ASSERT(0);
	return 0;
}
//////////////////////////////////
// Utility Functions start here //
//////////////////////////////////

__VBDECLARATION ODM_IdentifyTag(   __TAGS *pTheTags, char*  szTag  )
{
	TRACE_FILE_LINE_INFO();
	int ReturnValue = -1;
	for( int i = 0;  pTheTags+i; i++  )
		if( pTheTags[i].szTag && szTag && !lstrcmpi(szTag, pTheTags[i].szTag) )
		{
			ReturnValue = pTheTags[i].nTagId;
			break;
		}
	ASSERT(0);
	return ReturnValue;
}

FARPROC GetCallback(char* pszFnName, FARPROC pFn)
{
	TRACE_FILE_LINE_INFO();
	if (pszFnName)
	{		
		if (strcmp(pszFnName,"ExecuteExternalFunction")==0)
		{
			static FARPROC pExecuteExternalFunction=NULL;
			if (pFn)
				pExecuteExternalFunction=pFn;
			pFn=pExecuteExternalFunction;
		}
		else if (strcmp(pszFnName,"SetBenchState")==0)
		{
			static FARPROC pSetBenchState=NULL;
			if (pFn)
				pSetBenchState=pFn;
			pFn=pSetBenchState;
		}
		
	}
	ASSERT(0);
	return pFn;
}

