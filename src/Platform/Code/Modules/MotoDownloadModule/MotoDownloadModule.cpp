// MotoDownloadModule.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MotoDownloadModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CMotoDownloadModuleApp

BEGIN_MESSAGE_MAP(CMotoDownloadModuleApp, CWinApp)
END_MESSAGE_MAP()


// CMotoDownloadModuleApp construction

CMotoDownloadModuleApp::CMotoDownloadModuleApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMotoDownloadModuleApp object

CMotoDownloadModuleApp theApp;


// CMotoDownloadModuleApp initialization

BOOL CMotoDownloadModuleApp::InitInstance()
{
	CWinApp::InitInstance();

	//m_pRSDInterface = new CRSDInterface();

	//if (NULL == m_pRSDInterface)
	//{
	//	// TODO: handle this error

	//	return FALSE;
	//}
	return TRUE;
}

int CMotoDownloadModuleApp::ExitInstance()
{
	// UnInitialize the RSD interface if needed
	//if (m_pRSDInterface)
	//{
	//	delete m_pRSDInterface;
	//	m_pRSDInterface = NULL;
	//}

	return CWinApp::ExitInstance();
}
CMotoDownload* g_pMotoDLThis = NULL;
//////////////////////////////////////////////////////////////////////////
CMotoDownload::CMotoDownload(): m_strClassName("CMotoDownload"), 
m_strTitleName("Ghost"), m_hWnd(NULL)
{
	DefineNotify(EVENT_MOTO_DL_PROGRESS);
	DefineNotify(EVENT_MOTO_DL_MESSAGE);

	g_pMotoDLThis = this;
}
CMotoDownload::~CMotoDownload()
{
	
}

bool CMotoDownload::Initialize()
{
	m_bInitiated = m_RSDInterface.Initialize() == TRUE;
	/*CreateGhostWnd();
	if(m_bInitiated && m_hWnd)
	{
		m_bInitiated = theApp.GetRSDInterface()->PSTRegisterForNotifications(m_hWnd) == PST_OK;
		char szTemp[100] = {0};
		sprintf(szTemp, "%08X", m_hWnd);
		AfxMessageBox(szTemp);
	}*/
	return m_bInitiated;
}
void CMotoDownload::UnInitialize()
{
	if(m_bInitiated /*&& m_hWnd*/)
	{
		m_RSDInterface.UnInitialize();
		//theApp.GetRSDInterface()->PSTUnregisterForNotifications(m_hWnd);
		//::DestroyWindow(m_hWnd);
	}
}
//////////////////////////////////////////////////////////////////////////
void CMotoDownload::SetLastError(long nPort, int nError)
{
	CSingleLock lock(&m_LastErrorMapCritSection, TRUE);
	m_ErrorMap[nPort] = nError;
}
void CMotoDownload::AddMsg(const char* szMsg)
{
	Fire(EVENT_MOTO_DL_MESSAGE, (long)(szMsg));
}
void CMotoDownload::SetProgress(int nProgress)
{	
	Fire(EVENT_MOTO_DL_PROGRESS, (long)(nProgress));
}
//////////////////////////////////////////////////////////////////////////
void CMotoDownload::AddLog(int nPort, char* szMsg)
{
	CSingleLock singlelock(&m_LogCritSection, TRUE);
	CString strFileName;
	strFileName.Format(_T("LoopFlash_Port%02d.txt"),nPort);
	LogPolicy::Log(szMsg, strFileName);
}
//////////////////////////////////////////////////////////////////////////
bool CMotoDownload::Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pNotify, strEvent);
}
bool CMotoDownload::Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}
bool CMotoDownload::Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}
bool CMotoDownload::Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}
void CMotoDownload::Fire(const std::string& strEvent,long nParam)
{
	return QSYNC_LIB::CNotifyBase_ANSI::Fire(strEvent, nParam);
}
void CMotoDownload::DefineNotify(const std::string& strNewEvent)
{
	return QSYNC_LIB::CNotifyBase_ANSI::DefineNotify(strNewEvent);
}
//////////////////////////////////////////////////////////////////////////
bool CMotoDownload::GetPrefixFromMap(long nPort,std::string& strPrefix)
{
	bool bRes = false;
	std::map<long, std::string>::iterator iter;
	if(m_PortMap.find(nPort) == m_PortMap.end())
	{
		if(GetPrefix(nPort, strPrefix))
		{
			CSingleLock SingleLock(&m_PortMapCritSection, true);
			m_PortMap[nPort] = strPrefix;
			bRes = true;
		}
	}
	else
	{	
		strPrefix = m_PortMap[nPort];
		bRes = true;
	}
	return bRes;
}
//////////////////////////////////////////////////////////////////////////
bool CMotoDownload::GetPrefix(long nPort,std::string& strPrefix)
{
	bool bRes = false;
	using namespace std;
	
	const string strKey = "SOFTWARE\\Motorola\\NexTest\\CUsbCSequencer";
	/*
	[HKEY_LOCAL_MACHINE\SOFTWARE\Motorola\NexTest\CUsbCSequencer]
	"PathPrefix1"="7&31718826&0&0001"
	*/
	HKEY hKey;

	LONG lRet = ::RegOpenKeyExA( HKEY_LOCAL_MACHINE,strKey.c_str(),0, KEY_READ, &hKey );

	if( lRet == ERROR_SUCCESS )
	{
		char szPort[10] = {0};
		_ltoa(nPort, szPort, 10);
		string strField = "PathPrefix";
		strField+= szPort;
		
		DWORD nBufferSize = 512;
		char szBuffer[512] = {0};
		lRet = ::RegQueryValueExA( hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &nBufferSize);

		bRes = lRet == ERROR_SUCCESS;
		if(bRes)
		{	
			strPrefix = szBuffer;
		}
		::RegCloseKey(hKey);
	}
	
	return bRes;
}

bool CMotoDownload::GetComPortFromParentId(std::string strParentIdPrefix,const std::string& strVID,const std::string& strPID,std::string& strCOMPort)
{
	/*
	HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\Vid_1d45&Pid_4510&MI_01\7&31718826&0&0001\Device Parameters
	"PortName"="COM87"
	*/
	bool bRes = false;
	using namespace std;

	//MI_01 -- QCS6240,6270 
	const string strKeyFormat = "SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%s&Pid_%s&MI_00\\%s\\Device Parameters";	

	char szKey[1024] = {0};
	strParentIdPrefix += "&0000";
	::_stprintf(szKey,strKeyFormat.c_str(),strVID.c_str(),strPID.c_str(),strParentIdPrefix.c_str());
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
bool CMotoDownload::TransferPrefixToComPort(const std::string& strPathPrefix,const std::string& strVID,const std::string& strPID, long& nComport)
{
	bool bRes = false;
	std::string strParentIdPrefix;
	if(PathPrefixToParentId(strPathPrefix, strVID, strPID, strParentIdPrefix))
	{
		std::string strPortName;
		if(GetComPortFromParentId(strParentIdPrefix, strVID, strPID, strPortName))
		{
			strPortName = strPortName.substr(3, strPortName.size()-3);
			nComport = atol(strPortName.c_str());
			bRes = true;
		}
	}
	return bRes;
}

bool CMotoDownload::MotoPortToCOMPort(long nPort/*USB Prefix*/, int& nComport, const std::string& strVID,const std::string& strPID)
{
	bool bRes = false;
	std::string strPrefix;
	if(GetPrefix(nPort,strPrefix))
	{
		std::string strCOMPort;
		if(TransferPrefixToComPort(strPrefix, strVID,strPID, (long&)nComport))
			bRes = true;
	}
	return bRes;
}
bool CMotoDownload::IsInitiated()
{
	return m_bInitiated;
}


bool CMotoDownload::IsConnected(long nPort, CStringA& cstrSymbolicLink, bool& bDLMode)
{
	bool bRes = false;
	std::string strPrefix;
	
	if(GetPrefix(nPort, strPrefix))
	{
		{
			char szTemp[100] = {0};
			sprintf(szTemp, "Prefix: %s", strPrefix.c_str());
			AddLog(nPort, szTemp);
		}
		bDLMode = !IsNormalMode(strPrefix, cstrSymbolicLink);
		bRes = !bDLMode || IsDLMode(strPrefix, cstrSymbolicLink);
	}
	//if(bRes)
	//{
	//	// Register the device as RSD interested
	//	theApp.GetRSDInterface()->RegisterDevice(cstrSymbolicLink, nPort);
	//}

	if(bRes)
	{
		char szTemp[300] = {0};
		sprintf(szTemp, "SymbolicLink: %s", cstrSymbolicLink);
		AddLog(nPort, szTemp);
	}
	else
	{
		AddLog(nPort, "not connected");
	}

	return bRes;
}
bool CMotoDownload::FindSymLink(GUID guid, const std::string& strPathPrefix, CStringA& cstrSymbolicLink)
{
	bool bRes = false;
	CStringArray interfaceArray;
	GetUSBInterfaceList(&guid, interfaceArray);
	if(!interfaceArray.IsEmpty())
	{
		for(int i=0; i < interfaceArray.GetCount(); i++)
		{
			if(interfaceArray.ElementAt(i).Find(strPathPrefix.c_str())!=-1)
			{
				HANDLE  hP2k = INVALID_HANDLE_VALUE;							// the handle for the interface
				hP2k = CreateFile((LPCSTR)interfaceArray.ElementAt(i), 
					GENERIC_READ | GENERIC_WRITE,
					FILE_SHARE_READ | FILE_SHARE_WRITE,
					NULL, 
					OPEN_EXISTING, 
					0, 
					NULL);

				if (hP2k != INVALID_HANDLE_VALUE) {
					bRes = true;
					cstrSymbolicLink = interfaceArray.ElementAt(i);
					CloseHandle(hP2k);
					hP2k = INVALID_HANDLE_VALUE;	// cck fix 09/29/2000
					break;
				}
			}
		}
	}
	return bRes;
}

//////////////////////////////////////////////////////////////////////
//
// METHOD NAME: CRSDUSBNotify
// DESCRIPTION: 
//    - scans the registry for any entries associated with the P2k device 
//    - the entries are created in the registry by the driver for all devices connected
//    - when an entry is detected, the method checks to see if it is valid by opening the handle to the interface associated with it
//    - if the handle is valid, the interface name is added to the list
// PARAMETERS: 
//    - [IN] GUID  pDeviceClsName - Class GUID of the device to detect.
//    - [OUT] CStringArray &interfaceList - stores the entries found in the registry for the connected devices
// RETURN: void 
//
//////////////////////////////////////////////////////////////////////
void CMotoDownload::GetUSBInterfaceList(const GUID*  pDeviceClsName, CStringArray &interfaceList)
{
	HDEVINFO                            hDeviceInfo = INVALID_HANDLE_VALUE;                    // handle to device information structure for the interface to our devices
	PSP_INTERFACE_DEVICE_DETAIL_DATA    pDeviceDetail;                  // the detailed info
	DWORD                               requiredSize;                   // size of buffer reguired to get the symbolic link name
	SP_INTERFACE_DEVICE_DATA            interfaceData;                  // interface data for this device
	LONG                                status = ERROR_SUCCESS ;        // return status from command
	ULONG                               currentOffset = 0;              // the current offset into the user's buffer

	// get handle to devices that have our interface
	hDeviceInfo = SetupDiGetClassDevs((LPGUID)pDeviceClsName, NULL, NULL,
		DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);  

	if (hDeviceInfo == INVALID_HANDLE_VALUE) {
		return;
	}
	// enumerate all interfaces
	for ( int index = 0 ; status ==  ERROR_SUCCESS; index++) {
		// reset our size
		interfaceData.cbSize = sizeof(interfaceData);
		// get the next interface
		if (!SetupDiEnumDeviceInterfaces(hDeviceInfo, 
			NULL, 
			(LPGUID) pDeviceClsName,
			index, 
			&interfaceData)) {
				status = GetLastError();
				if (status == ERROR_NO_MORE_ITEMS) {
					break;
				} else {
					break;
				}
		}   
		// found our device, get the details data to obtain the symbolic
		// link name
		if (!SetupDiGetDeviceInterfaceDetail(hDeviceInfo, 
			&interfaceData, 
			NULL, 
			0,
			&requiredSize, 
			NULL)) {
				if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
					break;
				}
		}

		// allocate a buffer to get detail
		pDeviceDetail = (PSP_INTERFACE_DEVICE_DETAIL_DATA) LocalAlloc(LPTR, requiredSize);
		if (pDeviceDetail == NULL) {
			break;
		}
		pDeviceDetail->cbSize = sizeof(SP_INTERFACE_DEVICE_DETAIL_DATA);
		// get the detail info
		if (!SetupDiGetDeviceInterfaceDetail(hDeviceInfo, 
			&interfaceData, 
			pDeviceDetail, 
			requiredSize,
			NULL, 
			NULL)) {
				LocalFree(pDeviceDetail);
				break;
		}

		interfaceList.Add(CString(pDeviceDetail->DevicePath));
		// cleanup search
		LocalFree(pDeviceDetail);

	}  // end for loop

	SetupDiDestroyDeviceInfoList(hDeviceInfo);
	//SetLastError(ERROR_SUCCESS);
}

bool CMotoDownload::IsDLMode(const std::string& strPathPrefix, CStringA& cstrSymbolicLink)
{
	return FindSymLink(GUID_CLASS_P2K, strPathPrefix, cstrSymbolicLink);
}

bool CMotoDownload::PathPrefixToParentId(const std::string& strPathPrefix,const std::string& strVID,const std::string& strPID, std::string& strParentIdPrefix)
{
	/*
	HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB\Vid_1d45&Pid_4510\"strSerial"
	"ParentIdPrefix"=strM1Serial
	*/
	bool bRes = false;
	using namespace std;

	//MI_01 always is DIAG port for now 2008/4/15
	const string strKeyFormat = "SYSTEM\\CurrentControlSet\\Enum\\USB\\Vid_%s&Pid_%s\\%s";

	char szKey[1024] = {0};
	::_stprintf(szKey,strKeyFormat.c_str(),strVID.c_str(),strPID.c_str(),strPathPrefix.c_str());
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
			strParentIdPrefix = szBuffer;
		}
		::RegCloseKey(hKey);
	}
	return bRes;
}
bool CMotoDownload::IsNormalMode(const std::string& strPathPrefix, CStringA& cstrSymbolicLink)
{
	bool bRes = false;
	// transfer to ParentIDPrefix
	std::string strParentIdPrefix;
	if(PathPrefixToParentId(strPathPrefix, g_strMoto_VID, g_strNormal_Melos_PID, strParentIdPrefix))
	{
		strParentIdPrefix +="&0001"/*QCS6240,6270*/;
		bRes = FindSymLink(GUID_CLASS_P2KMDM, strParentIdPrefix, cstrSymbolicLink);
	}
	return bRes;
}


bool CMotoDownload::Download(long nPort/*USB Prefix*/, char* szFlashFilePath, CRSDUSBNotify* pRSDUSBNotify)
{
	bool bRes = false;
	// TODO: switch the phone to Flash(MBM) mode here
	// Get SymbolicLink and guid
	CStringA sSymbolicLink;

//	CRSDUSBNotify RSDNotify(&m_RSDInterface);
	bool bDLMode = false;
	if (IsConnected( nPort, sSymbolicLink, bDLMode ))
	{
		// Allocate DeviceId for PST
		UINT nDeviceId = nPort;
		
		{// Register the device as RSD interested
			std::string strPathPrefix;
			if(pRSDUSBNotify && GetPrefix(nPort, strPathPrefix))
				pRSDUSBNotify->RegisterDevice(strPathPrefix.c_str(), nDeviceId); 
		}
		

		// Notify PST framework to process device
		MotDeviceNotificationStruct MessyMotDeviceNotificationStruct;
		MessyMotDeviceNotificationStruct.guid               = bDLMode ? GUID_CLASS_P2K : GUID_CLASS_P2KMDM;
		MessyMotDeviceNotificationStruct.eNotificationType  = DETECTION_ARRIVAL;
		MessyMotDeviceNotificationStruct.eTransportType     = USB;
		MessyMotDeviceNotificationStruct.eDeviceId          = nDeviceId;
		MessyMotDeviceNotificationStruct.pStrSymLink        = (char*)(LPCSTR)sSymbolicLink;

		EPSTStatus eStatus = m_RSDInterface.PSTProcessDevice(nDeviceId, &MessyMotDeviceNotificationStruct);


		if (PST_OK == eStatus)
		{
			// Do flashing
			CFlashErr ErrObj;
			LPTSTR ArgList[] = { szFlashFilePath, (LPTSTR)nDeviceId };
			EFlashStatus FlashStatus = m_RSDInterface.FlashDeviceWOptions(2, 
				E_RESTART_ASYNC_NOWAITFORREENUM, ErrObj, (va_list)ArgList, E_NO_EXTRA_ACTION);


			// Notify PST framework to ignore device

			MessyMotDeviceNotificationStruct.eNotificationType  = DETECTION_REMOVAL;
			MessyMotDeviceNotificationStruct.pStrSymLink        = NULL;
			eStatus = m_RSDInterface.PSTIgnoreDevice(nDeviceId, &MessyMotDeviceNotificationStruct);

			if (FLASH_STATUS_OK == FlashStatus && PST_OK == eStatus)
			{
				bRes = true;
				AddLog(nPort, "[Success]FlashStatus == FLASH_STATUS_OK && eStatus == PST_OK");
			}
			else
			{
				// TODO: handle this error, can use ErrObj to get detail info
				SetLastError(nPort, FlashStatus);
				char szTemp[100] = {0};
				sprintf(szTemp, "[Fail]FlashStatus=%d, eStatus=%d", FlashStatus, eStatus);
				AddLog(nPort, szTemp);
			}
		}
		else
		{		
			char szTemp[100] = {0};
			sprintf(szTemp, "[Fail]PSTProcessDevice: eStatus=%d", eStatus);
			AddLog(nPort, szTemp);
			
			// TODO: handle this error
		}

		
		{	// UnRegister the device
			std::string strPathPrefix;
			if(pRSDUSBNotify && GetPrefix(nPort, strPathPrefix))
				pRSDUSBNotify->UnRegisterDevice(strPathPrefix.c_str(), nDeviceId);
		}
	}
	else
	{
		AddLog(nPort, "[Fail] Disconnected");
		// Can't find device
		// TODO: handle this error
	}
	return bRes;
}


DWORD _MessageLoop (  HANDLE* lphObjects, int  cObjects	,DWORD dwMilliseconds = INFINITE)
{ 
	DWORD result; 

	// The message loop lasts until we get a WM_QUIT message,
	// upon which we shall return from the function.
	while (TRUE)
	{
		// block-local variable 		
		MSG msg ; 

		// Read all of the messages in this next loop, 
		// removing each message as we read it.
		while (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{ 
			// If it is a quit message, exit.
			if (msg.message == WM_QUIT)  
				return 1; 
			// Otherwise, dispatch the message.
			::DispatchMessage(&msg); 
		} // End of PeekMessage while loop.

		// Wait for any message sent or posted to this queue 
		// or for one of the passed handles be set to signaled.
		result = ::MsgWaitForMultipleObjects(cObjects, lphObjects, FALSE, dwMilliseconds, QS_ALLINPUT); 

		// The result tells us the type of event we have.
		if (result == (WAIT_OBJECT_0 + cObjects))
		{
			// New messages have arrived. 
			// Continue to the top of the always while loop to 
			// dispatch them and resume waiting.
			continue;
		} 
		else 
		{ 
			// One of the handles became signaled. 
			break;
		} // End of else clause.
	} // End of the always while loop. 

	return result;
} // End of function.





bool CMotoDownload::RunDLThread(long nPort/*USB Prefix*/, char* szFlashFilePath)
{
	g_pMotoDLThis = this;

	char szEventName[50] = {0};
	sprintf(szEventName, "ExecuteMotoDL%d", nPort);
	HANDLE hExcuteEvent = ::CreateEvent(NULL,TRUE,FALSE, szEventName);
	ResetEvent(hExcuteEvent);
 
	DLParams dlParam = {nPort, szFlashFilePath, hExcuteEvent, false, new CRSDUSBNotify(&m_RSDInterface)};
	AfxBeginThread(AfxRunDL,&dlParam ,THREAD_PRIORITY_NORMAL,0,0,NULL);
	_MessageLoop(&hExcuteEvent, 1, INFINITE);
	if(dlParam.pRSDNotify)
		delete dlParam.pRSDNotify;

	return dlParam.bDLResult;
}
UINT CMotoDownload::AfxRunDL(LPVOID pParam)
{
	DLParams* pDLParam = (DLParams*)pParam;
	pDLParam->bDLResult = g_pMotoDLThis->Download(pDLParam->nPort,  pDLParam->szFlashFilePath, pDLParam->pRSDNotify);
	SetEvent(pDLParam->hExecuteEvent);
	return 0;
}


//BOOL CMotoDownload::CreateGhostWnd()
//{
//	if(!IsWindow(m_hWnd))
//	{
//		if(RegisterWindow())
//		{
//			m_hWnd = ::CreateWindow(
//				m_strClassName,m_strTitleName,WS_OVERLAPPEDWINDOW, 
//				CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
//				NULL,NULL,::AfxGetApp()->m_hInstance,(void *)this);			
//		}
//	}	
//	assert(m_hWnd);
//	return IsWindow(m_hWnd);
//}
//
//BOOL CMotoDownload::RegisterWindow()
//{
//	WNDCLASSEX wcx;
//	memset(&wcx,0,sizeof(WNDCLASSEX));
//
//	wcx.cbSize = sizeof(WNDCLASSEX);						// Size of structure
//	wcx.style = CS_HREDRAW | CS_VREDRAW;					// Redraw if size changes
//	wcx.lpfnWndProc = CMotoDownload::stWinMsgHandler;		// Points to window procedure
//	wcx.cbClsExtra = 0;										// No extra class memory
//	wcx.cbWndExtra = 0;										// No extra window memory
//	wcx.hInstance = ::AfxGetApp()->m_hInstance;				// Handle to instance
//	wcx.hIcon = LoadIcon(NULL, IDI_APPLICATION);			// Predefined app. icon
//	wcx.hCursor = LoadCursor(NULL, IDC_ARROW);				// Predefined arrow
//	wcx.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);// White background brush
//	wcx.lpszMenuName = NULL;								// Name of menu resource
//	wcx.lpszClassName = m_strClassName.GetBuffer(0);		// Name of window class
//	wcx.hIconSm = LoadIcon(NULL, IDI_APPLICATION);			// Small class icon
//
//	return ::RegisterClassEx(&wcx) != NULL;
//}
//
//LRESULT CALLBACK CMotoDownload::stWinMsgHandler(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
//{			
//	if (uMsg == WM_NCCREATE)
//	{
//		SetWindowLong(hwnd, GWL_USERDATA,(long)((LPCREATESTRUCT(lParam))->lpCreateParams));
//		return TRUE;
//	}
//	if (uMsg == WM_PST_DISPLAY_STATUS )
//	{
//		CStringA strStatus = (LPCSTR)wParam;
//		g_pMotoDLThis->AddMsg(strStatus);
//		return TRUE;
//	}
//	if (uMsg == WM_PST_DISPLAY_PROGRESS )
//	{
//		DWORD dwPercent = (DWORD) (wParam < 100) ? wParam : 100;
//		g_pMotoDLThis->SetProgress((int) dwPercent);
//		return TRUE;
//	}
//	if (uMsg == WM_PST_DISPLAY_ERROR )
//	{
//		CStringA strError = (LPCSTR)wParam;
//		if(!strError.IsEmpty())
//			g_pMotoDLThis->AddMsg(strError);
//		return TRUE;
//	}
//	CMotoDownload* pWnd = GetObjectFromWindow(hwnd);
//	if (pWnd)
//		return pWnd->WinMsgHandler(hwnd, uMsg, wParam, lParam);
//	else
//		return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
//}
//
//CMotoDownload* CMotoDownload::GetObjectFromWindow(HWND hWnd)
//{
//	return reinterpret_cast<CMotoDownload *>(GetWindowLong(hWnd, GWL_USERDATA));
//}
//
//LRESULT CALLBACK CMotoDownload::WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
//{
//	return ::DefWindowProc(m_hWnd,uMsg, wParam, lParam);
//}



