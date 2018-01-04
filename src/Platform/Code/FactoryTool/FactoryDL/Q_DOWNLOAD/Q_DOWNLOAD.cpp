// Q_DOWNLOAD.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Q_DOWNLOAD.h"
#include "Q_DOWNLOADDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQ_DOWNLOADApp

BEGIN_MESSAGE_MAP(CQ_DOWNLOADApp, CWinApp)
	//{{AFX_MSG_MAP(CQ_DOWNLOADApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQ_DOWNLOADApp construction

CQ_DOWNLOADApp::CQ_DOWNLOADApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CQ_DOWNLOADApp object

CQ_DOWNLOADApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CQ_DOWNLOADApp initialization

BOOL CQ_DOWNLOADApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CQ_DOWNLOADDlg dlg;
	m_pMainWnd = &dlg;
//	SetDialogBkColor(RGB(223,239,255),RGB(0,60,199));
//    SetDialogBkColor(RGB(226,250,254),RGB(11,0,53));
 	SetDialogBkColor(RGB(227,248,253),RGB(11,0,53));
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////

CDLProcess::CDLProcess(long nPort)
: m_nPort(nPort), m_pDiag(NULL),m_nIMEICheck(0), m_bCheckFactoryArea(true), m_strErrorCode("PASS"),
m_nRestartTimeout(27000), m_bBackupBrt(false), m_bLoopTest(false)
{
	DefineNotify(EVENT_UI_MESSAGE);
	DefineNotify(EVENT_UI_PROGRESS);
	DefineNotify(EVENT_DL_RESULT);
	DefineNotify(EVENT_RESET_UI);
}
CDLProcess::~CDLProcess()
{
}
//////////////////////////////////////////////////////////////////////////

bool CDLProcess::Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pNotify, strEvent);
}
bool CDLProcess::Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}
bool CDLProcess::Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}
bool CDLProcess::Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}
void CDLProcess::Fire(const std::string& strEvent,long nParam)
{
	return QSYNC_LIB::CNotifyBase_ANSI::Fire(strEvent, nParam);
}
void CDLProcess::DefineNotify(const std::string& strNewEvent)
{
	return QSYNC_LIB::CNotifyBase_ANSI::DefineNotify(strNewEvent);
}
void CDLProcess::Event(const std::string& strEvent,long nParam)
{
	if(strEvent == EVENT_DL_PROGRESS)
	{
		int nSubProgress = ((DL_PROGRESS*)nParam)->nProgress;

		int nMainProgress = 0;
		switch(((DL_PROGRESS*)nParam)->nStep)
		{
		case ChangeDLMode:
			nMainProgress=5;
			break;
		case DMSSProtocol:
			nMainProgress=10;
			break;
		case StreamingProtocol:
			nMainProgress=65;
			break;
		case ReOpenComport:
			nMainProgress=70;
			break;
		case None:
			nMainProgress=90;
			break;
		}
		if(((DL_PROGRESS*)nParam)->nStep == None)
		{
			AddMessage(((DL_PROGRESS*)nParam)->szMsg);
		}
		else
		{
			SetProgress(nMainProgress, nSubProgress);
			CStringA strMsg = ((DL_PROGRESS*)nParam)->szMsg;
			char szTemp[512] = {0};
			sprintf(szTemp,"%s     %d%%",((DL_PROGRESS*)nParam)->szMsg,nSubProgress);
			AddMessage(szTemp);
			if(nSubProgress == 100)
			{
				sprintf(szTemp,"MAIN[%d]:  %d%%",m_nPort,nMainProgress);
				AddMessage(szTemp);
			}
		}
	}
	//else if( strEvent == EVENT_MOTO_DL_MESSAGE)
	//{
	//	AddMessage((const char*)nParam);
	//}
	//else if( strEvent == EVENT_MOTO_DL_PROGRESS)
	//{
	//	SetProgress(65, nParam);
	//}
}
//////////////////////////////////////////////////////////////////////////
//Setting
void CDLProcess::Set6245DLPath(const tagDLSetting6245& DLPath)
{
	m_nDL6245Setting = DLPath;
}
void CDLProcess::Set6240DLPath(const tagDLSetting6240& DLPath)
{
	m_nDL6240Setting = DLPath;
}
bool CDLProcess::SetSuperFilePath(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strSuperFilePath = strFilePath;
		bRes = true;
	}
	return bRes;
}
void CDLProcess::SetRework(bool bRework){ m_bRework = bRework; }
void CDLProcess::SetCheckFactoryArea(bool bCheck){ m_bCheckFactoryArea = bCheck; }
void CDLProcess::SetIMEICheck(int nIMEICheck)
{ 
	m_nIMEICheck = nIMEICheck; 
	if(m_nIMEICheck == 1)
	{
		HRESULT hresult;
		CLSID clsid;
		CoInitialize(NULL);
		hresult=CLSIDFromProgID(OLESTR("SSS_O2.clsO2Check"), &clsid);
		hresult=CoCreateInstance(clsid,NULL,CLSCTX_INPROC_SERVER,__uuidof(_clsO2Check),(LPVOID *) &m_pITS_O2);
		if(FAILED(hresult))
		{
			WinExec("tregsvr -q SSS_O2.Dll",SW_SHOWDEFAULT);
			AfxMessageBox(" Please Restart Program ! ");

			// do something
			assert(0);
		}
	}
}
void CDLProcess::SetIsLoopTest(bool bIsLoop)
{
	m_bLoopTest = bIsLoop;
}
void CDLProcess::SetRestartTimeout(int nTimeout)
{
	//m_nRestartTimeout = nTimeout == 0 ? 27000 : nTimeout;
	m_nRestartTimeout = nTimeout;
}
void CDLProcess::SetSlotNumber(int nSlot){m_nSlot = nSlot;}
void CDLProcess::SetSo(const char* strSO){m_strSO = strSO;}
void CDLProcess::SetToolVersion(const char* strToolVersion){m_strTool_Version = strToolVersion;}
void CDLProcess::SetConfigVersion(const char* strConfigVersion){m_strConfig_Version = strConfigVersion;}
void CDLProcess::SetSWVersion(const char* strSWVersion){m_strSWVersion = strSWVersion;}
void CDLProcess::SetHWVersion(const char* strHWVersion){m_strHWVersion = strHWVersion;}
void CDLProcess::SetComputerName(const char* strComputerName){m_strComputer_Name = strComputerName;}
void CDLProcess::SetModelName(const char* strModelName){m_strMODEL_NAME = strModelName;}
void CDLProcess::SetOnlyDLCEFS(bool bOnlyDLCEFS){m_bOnlyDLCEFS = bOnlyDLCEFS;}
void CDLProcess::SetDLBootloader(bool bDLBootloader){ m_bDLBootloader = bDLBootloader; }
void CDLProcess::SetBackupBrt(bool bBackupBrt){ m_bBackupBrt = bBackupBrt; }
void CDLProcess::SetBRT_BACKUP_PATH(const char* szBrtFolder){ m_strBRTFolder = szBrtFolder; }
void CDLProcess::SetSaveLog(bool bSaveLog){ m_bSaveLog = bSaveLog; }
void CDLProcess::SetCSV(const char* szCSVPath, const char* szCSVTime, const char* szCSVSize)
{ 
	m_strCSVPath = szCSVPath;
	m_strCSVTime = szCSVTime;
	m_strCSVSize = szCSVSize;
}
void CDLProcess::SetQVCEFS_File_Name(const char* strName){ m_strQVCEFS_File_Name = strName; }
void CDLProcess::SetQVCEFS_File_Time(const char* strTime)
{
	//the file time format will be 2007/10/23 06:05:27, we will write 20071023060527 to factory area
	CStringA cstrTime = strTime;
	cstrTime.Remove('/');
	cstrTime.Remove(' ');
	cstrTime.Remove(':');
	m_strQVCEFS_File_Time = cstrTime;
}
void CDLProcess::SetQVCEFS_File_Size(const char* strSize)
{
	////The file size will be like:29,524 kB, we will write 29524 to factory area
	CStringA cstrSize = strSize;
	cstrSize.Remove(',');
	m_strQVCEFS_File_Size = cstrSize;
}
//////////////////////////////////////////////////////////////////////////
void CDLProcess::AddMessage(const char* cpMsg)
{
	char szTemp[512] = {0};
	CTime time = CTime::GetCurrentTime();
	CStringA strTime = time.Format("%Y/%m/%d %H:%M:%S");
	sprintf(szTemp, "[%s][Port%d] %s", strTime, m_nPort, cpMsg);
	Fire(EVENT_UI_MESSAGE, (long)(szTemp));
	m_strLog += szTemp;
	m_strLog +=	"\n";
}
void CDLProcess::AddErrorAndMessage(const char* cpErrorCode, const char* cpMsg)
{
	m_strErrorCode = cpErrorCode;
	AddMessage(cpMsg);
}
void CDLProcess::SetProgress(int nMainProgress, int nSubProgress)
{
	/*Download->m_MAIN[ArrayNo].SetPos(5);
	char temp[20] = {0};
	sprintf(temp,"%02d%%",5);
	Download->m_PROGRESS[ArrayNo].SetWindowText(temp);*/
	UIProgress temp = {m_nSlot, nMainProgress, nSubProgress};
	Fire(EVENT_UI_PROGRESS, (long)&temp);
}

void CDLProcess::SetResult(bool bResult)
{
	AddMessage(bResult? "DLProcess Success" : "DLProcess Fail");
	UIResult temp;
	temp.nSlot = m_nSlot;
	if(bResult)
	{
		m_strErrorCode = "PASS";
		strcpy(temp.szResult,  "PASS");
	}
	else	
	{
		strcpy(temp.szResult,  m_strErrorCode.c_str());
		std::string strMsg = "ErrorCode=";
		strMsg += m_strErrorCode;
		AddMessage(strMsg.c_str());
	}
	Fire(EVENT_DL_RESULT, (long)&temp);
}
//////////////////////////////////////////////////////////////////////////
//bool CDLProcess::RunMotoProcess()
//{
//	bool bRes = false;
//	CSpendTime SpendTime("Run MotoDL process", this);
//	//0. Initialize
//	CMotoDownload MotoDL;
//	MotoDL.Initialize();
//	MotoDL.Register(this, EVENT_MOTO_DL_MESSAGE);
//	MotoDL.Register(this, EVENT_MOTO_DL_PROGRESS);
//	bool bIsMotoDLMode = false;
//	if(MotoDL.IsInitiated() && IsMotoDLMode(&MotoDL, bIsMotoDLMode))
//	{
//		//0. Check is able to send command;
//		bool bIsConnectionOpen = false; /*ForResetUI*/
//		if(!bIsMotoDLMode)
//		{
//			int nComport = 0;
//			if(MotoDL.MotoPortToCOMPort(m_nPort, nComport))
//			{			
//				m_pDiag = new CCFCDiagCommand(nComport);
//				bIsConnectionOpen = m_pDiag->IsConnectinOpen();
//			}
//		}
//		else
//			bIsConnectionOpen = true;
//
//		if(bIsMotoDLMode || (!bIsMotoDLMode && m_pDiag && m_pDiag->IsConnected()))
//		{
//			AddMessage("The handset is connected");
//			
//			if(bIsConnectionOpen)
//			{
//				Fire(EVENT_RESET_UI, m_nSlot-1);
//				//1, is DL Mode
//				bool bIsDLMode = bIsMotoDLMode;
//
//				//2.CHECK IMEI STATUS 
//				if(!bIsDLMode && m_nIMEICheck==1)
//				{
//					if( CheckIMEIStationStatus() )
//						bRes = true;
//				}
//				else
//					bRes = true;
//
//				//3 Check signed version
//
//				//3. Check HWVariant/ Read Factory Area/ HWVersion/ Rewrite Redownlaod count
//				if(bRes)
//				{
//					bRes = PreProcessDownloadThreadFunction(bIsDLMode);
//					if(bRes)
//						SetProgress(5, 100);
//				}
//
//				//5.BackupNV to nv partition
//				if(bRes)
//				{
//					if(!bIsDLMode && m_bRework)
//						bRes = BackupNV();
//				}
//				
//				//6.Download
//				if(bRes)
//				{
//					CSpendTime SpendTime("Download", this);
//					if(bRes) // make sure unit connected and command can be sent
//					{
//						SetProgress(65, 0);
//						bRes = MotoDL.RunDLThread(m_nPort, (char*)m_strSuperFilePath.c_str());
//						int nComPort = 0;
//						if(!m_pDiag && MotoDL.MotoPortToCOMPort(m_nPort, nComPort))
//							m_pDiag = new CCFCDiagCommand(nComPort);
//						bRes = IsCommandAbleToSent() && Is6245RestoreSuccessful();
//					}
//					else
//						AddErrorAndMessage("M45", "Download Fail");
//
//					//Download default qcn
//
//				}
//
//				//7.Write factory and Restart
//				if(bRes)
//				{
//					bRes = false;
//					if(WriteFactoryArea()/*NV=2497*//* && WritePPFFileNameTimeSize()*//*NV=2498*/)
//					{
//						SetProgress(95, 100);
//						bRes = Restart();
//
//					}
//				}
//
//				//8. Write Log and Error Code.
//				WritePICSFile();
//
//				CloseConnection();
//				WriteLog();
//				WriteResultLog(&SpendTime);
//			}
//			else
//				AddMessage("Detect closed Connection. Please unplug and plug in the handset.");
//		}
//		else
//			AddErrorAndMessage("M18", "The handset is disconnected");
//	}
//	else
//		AddErrorAndMessage("M18", "Initialize failed, please check the prefix or if your dlls are complete");
//	
//
//	SetProgress(100, 100);
//	SetResult(bRes);
//	MotoDL.Unregister(this, EVENT_MOTO_DL_MESSAGE);
//	MotoDL.Unregister(this, EVENT_MOTO_DL_PROGRESS);
//	MotoDL.UnInitialize();
//	return bRes;
//}
////////////////////////////////////////////////////////////////////////////
//bool CDLProcess::IsMotoDLMode(CMotoDownload* pMotoDL, bool& bDLMode)
//{
//	bool bRes = false;
//	std::string strPrefix;
//	if(pMotoDL->GetPrefix(m_nPort, strPrefix))
//	{
//		CStringA strSymblicLink;
//		bDLMode = pMotoDL->IsDLMode(strPrefix, strSymblicLink);
//		bRes = true;
//	}
//	return bRes;
//}

bool CDLProcess::TestConnection(int nComPort)
{
	CCFCDiagCommand TempDiag(nComPort);
	return TempDiag.IsConnectinOpen();
}
//////////////////////////////////////////////////////////////////////////
bool CDLProcess::Run6245()
{
	bool bRes = false;
	CSpendTime SpendTime("Run 6245DL process", this);
	//0. Initialize
	CDownload6245 dl(m_nPort);
	dl.Register(this, EVENT_DL_PROGRESS);
	m_pDiag = new CCFCDiagCommand(m_nPort);
	
	bool bIsConnectionOpen = true;
	if(m_pDiag->IsConnected())
	{
		AddMessage("The handset is connected");

		//0. Check is able to send command;
		bIsConnectionOpen = m_pDiag->IsConnectinOpen();
		if(bIsConnectionOpen)
		{
			Fire(EVENT_RESET_UI, m_nSlot-1);

			//1, is DL Mode
			bool bIsDLMode = m_pDiag->IsDLMode();

			//1.5 BackupBrt
			if(!bIsDLMode && m_bBackupBrt)
			{
				bRes = BackupBrt();
			}
			else 
				bRes = true;

			//2.CHECK IMEI STATUS 
			if(!bIsDLMode && m_nIMEICheck==1)
			{
				bRes = bRes && CheckIMEIStationStatus();
			}
			

			//3 Check signed version

			//3.then Check HWVariant/ Read Factory Area/ HWVersion/ Rewrite Redownlaod count
			if(bRes)
			{
				bRes = PreProcessDownloadThreadFunction(bIsDLMode);
				if(bRes && bIsDLMode)
					bRes = IsHWVersionMatchedInDLMode((CDLFormatCommand*)&dl);
				if(bRes)
					SetProgress(5, 100);
			}

			//5.BackupNV to nv partition
			if(bRes)
			{
				if(!bIsDLMode && m_bRework)
					bRes = BackupNV();
			}
			//6.Download
			if(bRes)
			{
				bRes = DL6245(&dl);		
			}

			//7.Write factory and Restart
			if(bRes)
			{
				bRes = false;
				if(WriteFactoryArea()/*NV=2497*/ /*&& WritePPFFileNameTimeSize()*//*NV=2498*/)
				{
					SetProgress(95, 100);
					bRes = SetOnlineWithoutResetting();
				}
			}

			//8. Write Log and Error Code.
			WritePICSFile();
			
			CloseConnection();
			WriteLog();
			WriteResultLog(&SpendTime);
		}
		else
			AddMessage("Detect closed Connection. Please unplug and plug in the handset.");
		
	}
	else
		AddErrorAndMessage("M18", "The handset is disconnected");
	
	if(bIsConnectionOpen)
	{
		SetProgress(100, 100);
		SetResult(bRes);
	}
	dl.Unregister(this, EVENT_DL_PROGRESS);
	return bRes;
}

bool CDLProcess::Run6240()
{
	bool bRes = false;
	CSpendTime SpendTime("Run 6240DL process", this);
	//0. Initialize
	CDownload6240 dl(m_nPort);
	dl.Register(this, EVENT_DL_PROGRESS);
	m_pDiag = new CCFCDiagCommand(m_nPort);

	bool bIsConnectionOpen = true;
	if(m_pDiag->IsConnected())
	{
		AddMessage("The handset is connected");

		//0. Check is able to send command;
		bIsConnectionOpen = m_pDiag->IsConnectinOpen();
		if(bIsConnectionOpen)
		{
			Fire(EVENT_RESET_UI, m_nSlot-1);

			//1, is DL Mode
			bool bIsDLMode = m_pDiag->IsDLMode();

			//1.5 BackupBrt
			if(!bIsDLMode && m_bBackupBrt)
			{
				bRes = BackupBrt();
			}
			else
				bRes = true;

			//2.CHECK IMEI STATUS 
			if(!bIsDLMode && m_nIMEICheck==1)
			{
				bRes = bRes && CheckIMEIStationStatus();
			}
			else
				bRes = true;

			//3 Check signed version

			//3. Check HWVariant/ Read Factory Area/ HWVersion/ Rewrite Redownlaod count
			if(bRes)
			{
				bRes = PreProcessDownloadThreadFunction(bIsDLMode);
				if(bRes)
					SetProgress(5, 100);
			}

			//5.BackupNV to nv partition
			if(bRes)
			{
				if(!bIsDLMode && m_bRework)
					bRes = BackupNV();
			}
			//6.Download
			if(bRes)
			{
				bRes = DL6240(&dl);

				//Download default qcn

			}

			//7.Write factory and Restart
			if(bRes)
			{
				bRes = false;
				if(WriteFactoryArea()/*NV=2497*/ /*&& WritePPFFileNameTimeSize()*//*NV=2498*/)
				{
					SetProgress(95, 100);
					bRes = SetOnlineWithoutResetting();
				}
			}

			//8. Write Log and Error Code.
			WritePICSFile();

			CloseConnection();
			WriteLog();
			WriteResultLog(&SpendTime);
		}
		else
			AddMessage("Detect closed Connection. Please unplug and plug in the handset.");

	}
	else
		AddErrorAndMessage("M18", "The handset is disconnected");

	if(bIsConnectionOpen)
	{
		SetProgress(100, 100);
		SetResult(bRes);
	}
	dl.Unregister(this, EVENT_DL_PROGRESS);
	return bRes;
}
bool CDLProcess::DL6240(CDownload6240* pDownload)
{
	bool bRes = false;
	CSpendTime SpendTime("Download", this);
	Download6240Setting(pDownload);
	bRes = pDownload->CFCDownload(true);	
	if(bRes) // make sure unit connected and command can be sent
	{
		AddMessage("Download Succeed");
		char szTemp[512] = {0};
		sprintf(szTemp, "Waiting for resetting complete: sleep %ds", (m_nRestartTimeout/1000));
		AddMessage(szTemp);
		Sleep(m_nRestartTimeout);
		bRes = IsCommandAbleToSent() && Is6240RestoreSuccessful();
	}
	else
		AddErrorAndMessage("M45", "Download Fail");
	return bRes;
}

bool CDLProcess::DL6245(CDownload6245* pDownload)
{
	bool bRes = false;
	CSpendTime SpendTime("Download", this);

	Download6245Setting(pDownload);
	bRes = pDownload->CFCDownload(true);	
	if(bRes) // make sure unit connected and command can be sent
	{
		AddMessage("Download Succeed");
		char szTemp[512] = {0};
		sprintf(szTemp, "Waiting for resetting complete: sleep %ds", (m_nRestartTimeout/1000));
		AddMessage(szTemp);
		Sleep(m_nRestartTimeout);
		bRes = IsCommandAbleToSent() && Is6240RestoreSuccessful();
	}
	else
		AddErrorAndMessage("M45", "Download Fail");
	return bRes;
}
bool CDLProcess::Is6240RestoreSuccessful()
{
	bool bRes = false;
	CSpendTime SpendTime("Check 6240Restore Status", this);
	bool bNVRestoreSuccess = false;
	bool bCNVRestoreSuccess = false;

	if(m_pDiag && m_pDiag->GetMelosRestoreNVStatus(bNVRestoreSuccess, bCNVRestoreSuccess, g_nDefaultTimeout) && bNVRestoreSuccess && bCNVRestoreSuccess)
	{
		bRes = true;
		AddMessage("Auto-restore Succeed.");
	}
	else
	{
		AddErrorAndMessage("M46", "Auto-restore Failed");
	}
	return bRes;
}
bool CDLProcess::Is6245RestoreSuccessful()
{
	bool bRes = false;
	CSpendTime SpendTime("Check 6245Restore Status", this);
	bool bNVRestoreSuccess = false;
	bool bCNVRestoreSuccess = false;
	
	if(m_pDiag && m_pDiag->GetRestoreNVStatus(bNVRestoreSuccess, bCNVRestoreSuccess, g_nDefaultTimeout) && bNVRestoreSuccess && bCNVRestoreSuccess)
	{
		bRes = true;
		AddMessage("Auto-restore Succeed.");
	}
	else
	{
		AddErrorAndMessage("M46", "Auto-restore Failed");
	}
	return bRes;
}
bool CDLProcess::IsHWVersionMatchedInDLMode(CDLFormatCommand* pDl)
{
	bool bRes = false;
	CSpendTime SpendTime("Check HW Variant in DL mode", this);
	int nVariant = 0;
	int nEVT = 0;

	if(pDl && pDl->DLGetHWVariant(nEVT, nVariant))
	{
		std::string strTemp = m_strMODEL_NAME.substr(m_strMODEL_NAME.size()-2, 1);
		int nModelVariant = atoi(strTemp.c_str());
		if(nModelVariant == nVariant)
		{
			bRes = true;
			AddMessage("HW Variant matched with SW.");
		}
		else
		{
			char szTemp[100] = {0};
			_itot(nVariant, szTemp, 10);
			std::string strMsg = "HW Variant of the handset is ";
			strMsg+= szTemp;
			AddMessage(strMsg.c_str());
			AddErrorAndMessage("M41", "SW mismatched with HW variant.");
		}
	}
	else
		AddErrorAndMessage("M41", "Fail in getting HW Variant from the handset");
	return bRes;
}
bool CDLProcess::IsHWVersionMatched()
{
	bool bRes = false;
	CSpendTime SpendTime("Check HW Variant", this);
	int nVariant = 0;
	int nEVT = 0;

	if(m_pDiag && m_pDiag->GetLavernockVersion(nVariant, nEVT, g_nDefaultTimeout))
	{
		std::string strTemp = m_strMODEL_NAME.substr(m_strMODEL_NAME.size()-2, 1);
		int nModelVariant = atoi(strTemp.c_str());
		if(nModelVariant == nVariant)
		{
			bRes = true;
			AddMessage("HW Variant matched with SW.");
		}
		else
		{
			char szTemp[100] = {0};
			_itot(nVariant, szTemp, 10);
			std::string strMsg = "HW Variant of the handset is ";
			strMsg+= szTemp;
			AddMessage(strMsg.c_str());
			AddErrorAndMessage("M41", "SW mismatched with HW variant.");
		}
	}
	else
		AddErrorAndMessage("M41", "Fail in getting HW Variant from the handset");
	return bRes;
}
void CDLProcess::Download6240Setting(CDownload6240* pDownload)
{
	if(pDownload)
	{
		pDownload->SetHex(		m_nDL6240Setting.szHex);
		pDownload->SetPartition( m_nDL6240Setting.szPartition);
		if(!m_bOnlyDLCEFS)
		{
			if(m_bDLBootloader)
			{
				//pDownload->SetDBL(	m_nDL6240Setting.szDBL);
				//pDownload->SetFSBL(	m_nDL6240Setting.szFSBL);
				pDownload->SetOSBL(	m_nDL6240Setting.szOSBL);
			}
			pDownload->SetAMSS(	m_nDL6240Setting.szAmss);
		}
		pDownload->SetCEFS(		m_nDL6240Setting.szCefs);
	}
}
void CDLProcess::Download6245Setting(CDownload6245* pDownload)
{
	if(pDownload)
	{
		//AfxMessageBox("Download Setting: hex, partition and amss only.");
		pDownload->SetHex(		m_nDL6245Setting.szHex);
		pDownload->SetPartition( m_nDL6245Setting.szPartition);
		if(!m_bOnlyDLCEFS)
		{
			if(m_bDLBootloader)
			{
				//pDownload->SetQCSBLHD(	m_nDL6245Setting.szQcsblhd);
				//pDownload->SetQCSBL(		m_nDL6245Setting.szQcsbl);
				pDownload->SetOEMSBLHD(	m_nDL6245Setting.szOemsblhd);
				pDownload->SetOEMSBL(	m_nDL6245Setting.szOemsbl);
			}
			
			pDownload->SetAMSSHD(	m_nDL6245Setting.szAmsshd);
			pDownload->SetAMSS(		m_nDL6245Setting.szAmss);
		}
		pDownload->SetCEFS(		m_nDL6245Setting.szCefs);
	}
}

bool CDLProcess::IsCommandAbleToSent()
{
	bool bRes = false;
	CSpendTime SpendTime("IsCommandAbleToSent", this);
	char szIMEI[100] = {0};
	int nRetryTime = 0;
	while (m_pDiag && m_pDiag->ReadIMEI(szIMEI) == false && ++nRetryTime < 100);
	bRes = m_pDiag->ReadIMEI(szIMEI);
	if(bRes)
		AddMessage("Be able to send commands");
	else
		AddErrorAndMessage("M61", "Not be able to send commands");
	return bRes;
}

bool CDLProcess::ReadIMEI(char* szIMEI100)
{
	bool bRes = false;
	CSpendTime SpendTime("Read IMEI", this);

	AddMessage("Begin to Read IMEI From Phone");
	if(m_pDiag && m_pDiag->ReadIMEI(szIMEI100, g_nDefaultTimeout))
	{
		std::string strMsg = "Read IMEI From Phone Success : ";
		strMsg+= szIMEI100;		
		AddMessage(strMsg.c_str());
		if(strcmp(szIMEI100, Dummy_IMEI) != 0)
			bRes = true;
		else
			AddMessage("The IMEI read from phone is a dummy IMEI......");
	}
	else
		AddErrorAndMessage("M56", "Read IMEI From Phone Fail");

	return bRes;
}

bool CDLProcess::ReadBTAddress(char* szBTAddress100)
{
	bool bRes = false;
	CSpendTime SpendTime("Read BTAddress", this);
	AddMessage("Begin to Read BT Address From Phone");

	long nLength = 0;
	if(m_pDiag && m_pDiag->ReadBTAddr(szBTAddress100, nLength, g_nDefaultTimeout))
	{
		std::string strMsg = "Read BT Address From Phone Success : ";
		strMsg+= _strupr(szBTAddress100);
		AddMessage(strMsg.c_str());

		bRes = true;
	}
	else
		AddErrorAndMessage("M56-1", "Read BT Address From Phone Fail");

	return bRes;
}
bool CDLProcess::ITS_QueryIMEI(char *szIMEI_BT, char *szStationtState, char *szErrMsg)
{
	bool bRes = false;	
	CComBSTR	combstr_GSN(20), 
		combstr_BT(14),  
		combstr_SW(10), 
		combstr_HW(10), 
		combstr_MDate(20), 
		combstr_MTime(20), 
		combstr_Carton(32), 
		combstr_DBDate(20),
		combstr_DBTime(20), 
		combstr_Phone(20), 
		combstr_Mode(2), 
		combstr_ErrCode(12), 
		combstr_ErrDes(104), 
		combstr_NAL(25);

	bRes = m_pITS_O2->funQueryIMEI_Data(szIMEI_BT,
		szStationtState,
		&combstr_GSN,
		&combstr_BT,
		&combstr_HW,
		&combstr_SW,
		&combstr_MDate,
		&combstr_MTime,
		&combstr_Carton,
		&combstr_DBDate,
		&combstr_DBTime,
		&combstr_Phone,
		&combstr_Mode,
		&combstr_ErrCode,
		&combstr_ErrDes,
		&combstr_NAL);


	_stprintf(szErrMsg, _T("%s"), (LPCTSTR)combstr_Carton.Copy());
	combstr_Carton.Empty();

	char szErrCode[10] = {0};
	_stprintf(szErrCode, _T("%s"), (LPCTSTR)combstr_ErrCode.Copy());
	combstr_ErrCode.Empty();

	if ( strcmp(szErrCode, _T("0000")) == 0 )
		bRes = true;

	return bRes;
}
bool CDLProcess::CheckIMEIStationStatus()
{
	bool bRes = false;
	AddMessage("Check IMEI/BT Status in SSS ......");

	char szQueryKey[50] = {0};

	char szIMEI[100] = {0};
	char szBTAddress[100] = {0};
	bool bITSQuery = true;
	if(ReadIMEI(szIMEI)) //IMEI exists but not Dummy_IMEI
	{
		strcpy(szQueryKey, szIMEI); 
	}
	else if(ReadBTAddress(szBTAddress))
	{
		strcpy(szQueryKey, szBTAddress); 
	}
	else
		bITSQuery = false;

	if(bITSQuery)
	{
		char szErrMsg[512] = {0};
		if(ITS_QueryIMEI(szQueryKey,"B", szErrMsg))
		{
			char temp_log[512] = {0};
			sprintf(temp_log,"IMEI/BT Status in SSS is '%s'",szErrMsg);	
			AddMessage(temp_log);
			bRes = true;
		}
		else if(ITS_QueryIMEI(szQueryKey,"P", szErrMsg))
		{
			char temp_log[512] = {0};
			sprintf(temp_log,"IMEI/BT Status in SSS is '%s' ,Please do release first ",szErrMsg);	
			AddErrorAndMessage("M57", temp_log);
		}
		else
			AddErrorAndMessage("M58", "Query IMEI/BT Status Fail, Contact with Jackie (1825)......");
	}
	else
	{
		bRes = true;
		m_strErrorCode = "PASS";
	}
	return bRes;
}

bool CDLProcess::GetMobileID(std::string& strMobileID, int nTimeout)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(m_pDiag && m_pDiag->ReadNVItem(2497, resp, nTimeout))
	{
		BYTE szMobileID[12] = {0};
		memcpy(szMobileID, resp.item_data+45, 11);
		CStringA strChar;
		if(QSYNC_LIB::ByteToChar(szMobileID, 11, strChar, false))
		{
			strMobileID = strChar;
			bRes = true;
		}
	}
	return bRes;
}

bool CDLProcess::BackupBrt(int nTimeout)
{
	bool bRes = false;
	CSpendTime SpendTime("BackupBrt", this);
	
	std::string strMobileID;
	GetMobileID(strMobileID, nTimeout);
	
	// Prepare Log Infomation and gather brt name
	CTime time = CTime::GetCurrentTime();
	std::string strBrtLog = time.Format("[%Y/%m/%d %H:%M:%S]\n");  //1st line : Date, Time

	char szBrtPath[512] ={0};
	sprintf(szBrtPath,"%s\\%s_%s.brt", m_strBRTFolder.c_str(), strMobileID.c_str(),time.Format("%Y%m%d%H%M%S"));
	char szMsg[512] = {0};
	sprintf(szMsg, "BRT_Path= %s", szBrtPath);
	AddMessage(szMsg);
	strBrtLog += szMsg;  //2nd line: Brt Path
	strBrtLog += "\n";  

	sprintf(szMsg, "CSV_Path= %s, CSV_FILE_TIME= %s, CSV_FILE_SIZE= %s", m_strCSVPath.c_str(), m_strCSVTime.c_str(), m_strCSVSize.c_str());
	AddMessage(szMsg);
	strBrtLog += szMsg; //3rd line: CSV File Information
	strBrtLog += "\n";

	::CreateDirectory(m_strBRTFolder.c_str(),NULL);
	if(m_pDiag && m_pDiag->BackupBRT(szBrtPath, m_strCSVPath))
	{
		bRes = true;
		AddMessage("BackupBRT succeed.");
		strBrtLog += "BackupBrt PASS\n";
	}
	else
	{
		strBrtLog += "BackupBrt Failed\n";
		AddErrorAndMessage("M62", "BackupBRT Fail");
	}

	// add BackupBrt.log for verifying easily
	{  
		CSingleLock singleLock(&m_WriteResultLogCritSection,true);
		CStdioFile File_Ctrl1;
		char szBrtLogFilePath[512] ={0};
		sprintf(szBrtLogFilePath,"%s\\BackupBrt.log",m_strBRTFolder.c_str());
		if(File_Ctrl1.Open(szBrtLogFilePath,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate,NULL))
		{
			File_Ctrl1.SeekToEnd();
			strBrtLog+="=============================END============================";
			File_Ctrl1.WriteString(strBrtLog.c_str());
			File_Ctrl1.WriteString("\n\n");
			File_Ctrl1.Close();
		}
	}
	return bRes;
}

bool CDLProcess::RestoreNV(int nTimeout)
{
	bool bRes = false;
	CSpendTime SpendTime("RestoreNV", this);
	if(m_pDiag && m_pDiag->Restore(nTimeout))
	{
		bRes = true;
		AddMessage("Restore NV succeed.");
	}
	else
		AddMessage("Restore NV Failed. (Didn't set error code)");
	return bRes;
}
bool CDLProcess::PreProcessDownloadThreadFunction(bool bIsDLMode)
{
	bool bRes = false;
	if(bIsDLMode)
	{
		AddMessage("Phone is in DL Mode, skip reading factory area");
		bRes = true;
	}
	else
	{
		if(IsHWVersionMatched())
		{
			if(m_bCheckFactoryArea)
			{
				DIAG_NVITEM_PACKET_F_type resp = {0};
				if(!ReadFactoryArea(resp))
				{
					if(resp.nv_stat==NV_NOTACTIVE_S && m_bRework)
					{
						
						for(int i=0; i<3 && !bRes; i++)
							bRes = RestoreNV();
						
						if(bRes)
						{
							ZeroMemory(&resp, sizeof(resp));
							bRes = ReadFactoryArea(resp);
						}
					}
				}
				else
					bRes = true;

				if(bRes && resp.nv_stat!=NV_NOTACTIVE_S)
				{
					ReadHWVersion();
					RewriteRedownloadCount();
				}
			}
			else
			{
				AddMessage("CheckFactorySkip=false, Skip ReadFactoryArea");
				bRes = true;
			}
		}
	}
	return bRes;
}

bool CDLProcess::ReadFactoryArea(DIAG_NVITEM_PACKET_F_type& resp)
{
	bool bRes = false;

	CSpendTime SpendTime("Read Factory Area", this);
	if(m_pDiag && m_pDiag->ReadNVItem(2497, resp, g_nDefaultTimeout))
	{
		char temp_log[512] = {0};
		BYTE nID_Flag = resp.item_data[51]; // MobileID month
		sprintf(temp_log,"Mobile ID flag:0x%02X",nID_Flag);
		AddMessage(temp_log);

		if(nID_Flag == 0x00)
		{
			if(m_bRework)
				AddErrorAndMessage("M14", "Namely mobile ID does not exist, but the tool is in REWORK mode");	
			else
			{
				AddMessage("Namely mobile ID does not exist");
				bRes = true;
			}
		}
		else //ID_Flag==0xFF,means Mobile ID has been written
		{
			AddMessage("Namely mobile ID exists");
			/************************************************************************/
			AddMessage("Read Mobile ID from phone");
			BYTE nMobileID[12] = {0};
			memcpy(nMobileID, resp.item_data+45, 11);

			CStringA strChar;
			if(QSYNC_LIB::ByteToChar(nMobileID, 11,strChar, true))
			{
				// show mobileID
				CString strTemp = "The mobile ID from phone: 0x";
				strTemp += strChar;
				AddMessage(strTemp);

				strChar.Remove(' ');
				int nLength = strChar.GetLength();
				for(int i=0; i<22; i++)
				{// every char should be digit
					if(strChar[i] > '9' || strChar[i]< '0')
					{
						m_nRewriteMobileIDFlag = 1;
						break;
					}
				}
				if(m_nRewriteMobileIDFlag!=1 && !m_bRework)
					AddErrorAndMessage("M14", "Namely mobile ID exist, but the tool is in NORMAL mode");
				else if(m_nRewriteMobileIDFlag ==1 && m_bRework)
					AddErrorAndMessage("M12", " Mobile ID in phone is error, need to Rewrite, Please do in normal mode......");
				else
				{
					m_strMobileID = strChar;
					bRes = true;
				}
			}
		}
	}
	else
	{
		if(resp.nv_stat == NV_NOTACTIVE_S)
		{
			if(m_bRework)
				AddErrorAndMessage("M13", "Factory area is not active, but in Rework mode.....");
			else
			{
				AddMessage("Factory area is not active......");
				bRes = true;
			}
		}
		else
			AddErrorAndMessage("M15", "Read factory area fail......");
	}
	return bRes;
}

void CDLProcess::ReadHWVersion()
{	
	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(m_pDiag && m_pDiag->ReadNVItem(2497, resp, g_nDefaultTimeout))
	{
		//89 to 93 is for HW version
		BYTE nHWVersion[100] = {0};
		memcpy(nHWVersion, (resp.item_data)+89, g_nHW_Version_Size);
		m_strHWVersion = (char*)nHWVersion;
		char szTemp[100] = {0};
		sprintf(szTemp, "HW Version: %s", nHWVersion);
		AddMessage(szTemp);
	}	
}

void CDLProcess::RewriteRedownloadCount()
{
	DIAG_NVITEM_PACKET_F_type resp = {0};
	if(m_pDiag && m_pDiag->ReadNVItem(2497, resp, g_nDefaultTimeout))
	{
		if( (resp.item_data[59] & 0xF0) >= 160)
			resp.item_data[59] = 0;
		resp.item_data[59] += 0x10;
		AddMessage("Write re-download Test count to phone......");

		if(m_pDiag->WriteNVItem(resp, g_nDefaultTimeout))
			AddMessage("Write re-download Test count to phone Success......");
		else
			AddMessage("Write re-download Test count to phone Fail......");
	}
}

bool CDLProcess::BackupNV(int nTimeout)
{
	bool bRes = false;
	CSpendTime SpendTime("BackupNV", this);
	if(m_pDiag && m_pDiag->Backup(nTimeout))
	{
		AddMessage("BackupNV Success");
		bRes = true;
	}
	else
		AddErrorAndMessage("M44", "BackupNV Fail");
	return bRes;
}
void CDLProcess::PrepareFactoryInfoForWriting(DIAG_NVITEM_PACKET_F_type& resp, bool bInNormalMode)
{
	if(bInNormalMode)
	{
		//Reset
		memset(resp.item_data, 0, DIAG_NV_ITEM_SIZE);		
		// 0-2 : reserved
		resp.item_data[0]=0xFF;
		resp.item_data[1]=0xFF;
		resp.item_data[2]=0xFF;	

		// 3: KLF Byte
		resp.item_data[3]=0x00;

		// 5-7: Process1 Info(Download)- Current Time
		CTime Current_Time = CTime::GetCurrentTime();
		resp.item_data[5]=Current_Time.GetDay();
		resp.item_data[6]=Current_Time.GetMonth();
		resp.item_data[7]=Current_Time.GetYear()-2000;

		//	8: Process1 Info(Download)-Test station
		CStringA cstrComputerName = m_strComputer_Name.c_str();
		resp.item_data[8]=(BYTE)(((atoi(cstrComputerName.Mid(4,2)))<<4) + atoi(cstrComputerName.Mid(1,2)));

		// 45-55: MobileID = line number(3 bytes)+adapter+DUT number+ datetime(6 bytes)
		CStringA cstrMobileID;
		//1. line number = COMPUTER_NAME will be like T01D02, we remove the character and add "00" to create a new computer name and the new name will be like 010200--->line number
		CStringA strComputerName = m_strComputer_Name.c_str();
		strComputerName = strComputerName.Mid(1,2)+strComputerName.Mid(4,2)+"00";
		//2. adapter = "00"
		//3. DUT number = slot
		CStringA strSlot;
		strSlot.Format("%2u", m_nSlot);
		//4. date time
		CStringA strDataTime;
		strDataTime = Current_Time.Format("%d%m%y%H%M%S");
		cstrMobileID = strComputerName+ "00" + strSlot + strDataTime;
		m_strMobileID = cstrMobileID;
		if(cstrMobileID.GetLength() != g_nMobile_ID_Size*2)
			AfxMessageBox("MOBILE Length Wrong!!");
		BYTE nMobileID[100] = {0};
		QSYNC_LIB::CharToByte(cstrMobileID, cstrMobileID.GetLength(), nMobileID);
		memcpy(resp.item_data+45, nMobileID, g_nMobile_ID_Size);

		// 89-93: HW Version
		memcpy(resp.item_data+89, m_strHWVersion.c_str(), g_nHW_Version_Size);

		//Show message
		CStringA strMsg;
		strMsg.Format("Factory area information: SW version---%s HW version---%s SLOT---%s KLF byte---0x00 MobileID---%s",
			m_strSWVersion.c_str(), m_strHWVersion.c_str(), strSlot, cstrMobileID);
		AddMessage(strMsg);
	}
	else // in rework mode
	{
		// 3: KLF Byte
		resp.item_data[3]&= 0xDF;
		// 33-35: Process 8 Info(Re-download)- Current Time

		CTime Current_Time = CTime::GetCurrentTime();
		resp.item_data[33]=Current_Time.GetDay();
		resp.item_data[34]=Current_Time.GetMonth();
		resp.item_data[35]=Current_Time.GetYear()-2000;

		// 36: Process 8 Info(Re-download)-Test station
		CStringA cstrComputerName = m_strComputer_Name.c_str();
		resp.item_data[36]=(BYTE)(((atoi(cstrComputerName.Mid(4,2)))<<4) + atoi(cstrComputerName.Mid(1,2)));
	}

	//Common Writing Data
	// 85-88: SW VERSION
	CStringA cstrSWVersion = m_strSWVersion.c_str();
	cstrSWVersion.Remove('.'); //0.07 ->007
	if(cstrSWVersion.GetLength()!=7)//SW version+PID+LPID
		cstrSWVersion+="00000";
	BYTE nSWForPhone[100] = {0};
	QSYNC_LIB::CharToByte(cstrSWVersion, cstrSWVersion.GetLength(), nSWForPhone);
	memcpy(resp.item_data+85, nSWForPhone, g_nSW_Version_Size);//85 to 88 is for SW
}
bool CDLProcess::WriteFactoryArea()
{
	bool bRes = false;
	if(m_bCheckFactoryArea)
	{
		CSpendTime SpendTime("Write Factory Area", this);

		AddMessage("Write FactoryArea");
		DIAG_NVITEM_PACKET_F_type resp = {0};
		if(ReadFactoryArea(resp))
		{
			bool bInNormalMode = !m_bRework 
				&& (resp.item_data[51]==0x00 || resp.nv_stat == NV_NOTACTIVE_S || m_nRewriteMobileIDFlag ==1);

			PrepareFactoryInfoForWriting(resp, bInNormalMode);

			// Write Factory Area
			if(m_pDiag->WriteNVItem(resp, g_nDefaultTimeout))
			{			
				AddMessage("Write factory area success");

				AddMessage("Sleep 5s");
				Sleep(5000);
				AddMessage("Compare NV2497:");
				//compare write and read
				DIAG_NVITEM_PACKET_F_type temp = {0};
				if(m_pDiag->ReadNVItem(2497, temp, g_nDefaultTimeout) && memcmp(resp.item_data, temp.item_data, 128)==0)
				{				
					bRes = true;
					AddMessage("WriteNV2497=ReadNV2497, Factory area Matched Successfully");
				}
				else
					AddErrorAndMessage("M21", "Factory area Matching Error");
			}
			else
				AddErrorAndMessage("M21", "Write factory area fail");
		}
	}
	else
		bRes = true;


	return bRes;
}

bool CDLProcess::WritePPFFileNameTimeSize()
{
	bool bRes = false;
	CSpendTime SpendTime("Write PPF File info", this);

	DIAG_NVITEM_PACKET_F_type req = {0};
	req.item = 2498;

	//[0 to 100 , total 100 bytes is for .qvcefs file name]
	memcpy(req.item_data, m_strQVCEFS_File_Name.c_str(), m_strQVCEFS_File_Name.size());

	if(m_strQVCEFS_File_Time.size() == 14)
	{
		//[100 to 113 , total 14 bytes is for .qvcefs file time]
		memcpy(req.item_data+100, m_strQVCEFS_File_Time.c_str(), m_strQVCEFS_File_Time.size());

		if(m_strQVCEFS_File_Size.size() <7)
		{
			//[114 to 119 , total 6 bytes is for .qvcefs file size]
			memcpy(req.item_data+114, m_strQVCEFS_File_Size.c_str(), m_strQVCEFS_File_Size.size());

			for(int i=0; i<2; i++) // For Retry WriteNVItem
			{
				if(m_pDiag && m_pDiag->WriteNVItem(req, g_nDefaultTimeout))
				{
					bRes = true;
					AddMessage("Write PPF File Name/Time/Size succeed");
					DIAG_NVITEM_PACKET_F_type temp = {0};
					if(m_pDiag->ReadNVItem(2498, temp, g_nDefaultTimeout) && memcmp(req.item_data, temp.item_data, 128)==0)
					{				
						bRes = true;
						AddMessage("WriteNV2498=ReadNV2498, PPF File Name/Time/Size Matched Successfully");
					}
					break;
				}
				if(i>0)
					Sleep(200);
			}
			if(!bRes)
			{
				AddErrorAndMessage("M48", "Write PPF File Name/Time/Size Fail");
			}
		}
		else
			AddErrorAndMessage("M48", "The length of QVCEFS File size is longer than 6.");
	}
	else
		AddErrorAndMessage("M48","The length of	QVCEFS File Time is not 14.");

	return bRes;
}
bool CDLProcess::SetOnlineWithoutResetting()
{
	bool bRes = false;
	CSpendTime SpendTime("SetOnlineWithoutResetting", this);
	DIAG_NVITEM_PACKET_F_type req = {0};
	req.nDIAGcmd = DIAG_NV_WRITE_F;
	req.item = NV_FTM_MODE_I;
	req.item_data[0] = 0; //bFTMMode = (1 = FTM mode), (0 = ONLINE mode) 
	if(m_pDiag && m_pDiag->WriteNVItem(req, g_nDefaultTimeout))
	{
		AddMessage("SetOnlineWithoutResetting: Set Online mode success");
		AddMessage("Sleep 5s");
		Sleep(5000);
		AddMessage("Compare NV453:");
		DIAG_NVITEM_PACKET_F_type temp = {0};
		if(m_pDiag->ReadNVItem(NV_FTM_MODE_I, req, g_nDefaultTimeout) 
			&& memcmp(req.item_data, temp.item_data, 128)==0)
		{
			AddMessage("WriteNV453=ReadNV453, SetOnlineWithoutResetting succeed");
			bRes = true;
		}
		else
			bRes = Restart();
		
	}
	return bRes;
}
bool CDLProcess::Restart()
{
	bool bRes = false;
	CSpendTime SpendTime("Restart", this);
	if(m_pDiag && m_pDiag->Restart(10000, false))
	{
		char szTemp[512] = {0};
		sprintf(szTemp, "Waiting for Restart: sleep %ds", (m_nRestartTimeout/1000));
		AddMessage(szTemp);
		Sleep(m_nRestartTimeout);
		bRes = IsCommandAbleToSent();
	}
	return bRes;
}

//************************************
// Method:    WritePICSFile
// FullName:  CDLStationProcess::WritePICSFile
// Access:    private 
// Returns:   bool
// Purpose:   For updating MobileID related information to server by save a file in c:\picsdata\ 
//************************************
bool CDLProcess::WritePICSFile()
{
	bool bRes = false;
	if(m_strMobileID.size() != 0)
	{
		CSpendTime SpendTime("Write PICS File", this);

		::CreateDirectory("C:\\picsdata",NULL);
		char szFilePath[512] = {0};
		CTime time= CTime::GetCurrentTime();
		if(m_bOnlyDLCEFS)
			sprintf(szFilePath,"C:\\picsdata\\%s%s%s.%sFLX",m_strMODEL_NAME.c_str(),m_strMobileID.c_str(),time.Format("_%H%M%S"),m_strMODEL_NAME.c_str());
		else
			sprintf(szFilePath,"C:\\picsdata\\%s%s%s.%sTRG",m_strMODEL_NAME.c_str(),m_strMobileID.c_str(),time.Format("_%H%M%S"),m_strMODEL_NAME.c_str());

		CPICSFile file;
		if(file.Open(szFilePath ,CFile::modeCreate | CFile::modeWrite))
		{
			file.WriteALine(m_strSO.c_str());//1
			file.WriteALine(m_strMobileID.c_str());//2
			file.WriteALine(m_strMODEL_NAME.c_str());//3	
			//host
			std::string strT = m_strComputer_Name.substr(1, 2);
			file.WriteALine(strT.c_str()); //4
			std::string strL = m_strComputer_Name.substr(5, 1);
			file.WriteALine(strL.c_str());//5
			char szBuffer[65] = {0};
			_itoa(m_nSlot, szBuffer, 10);
			file.WriteString(szBuffer);
			file.WriteString("\n");//6
			file.WriteString("OK\n");//7
			file.WriteString("PASS\n");//8

			file.WriteALine(m_strHWVersion.c_str());//9
			file.WriteALine(m_strSWVersion.c_str());//10
			file.WriteALine(m_strHWVersion.c_str());//11
			file.WriteString("\n");//12
			file.WriteALine(m_strTool_Version.c_str());//13
			file.WriteALine(m_strConfig_Version.c_str());//14

			file.Close();
			AddMessage("Write PICS file succeed");
			bRes = true;
		}
		else
		{
			char szTemp[512] = {0};
			sprintf(szTemp,"Write PICS file to %s fail......",szFilePath);
			AddMessage(szTemp);
		}
	}
	else
		bRes = true;
	return bRes;
}
bool CDLProcess::CloseConnection()
{
	bool bRes = false;
	if(!m_bLoopTest)
	{
		if(m_pDiag && !m_pDiag->IsDLMode())
		{
			if(m_pDiag->CloseConnection(g_nDefaultTimeout))
			{
				AddMessage("Close connection succeed");
				bRes = true;	
			}
			else
				AddMessage("Close connection fail.");		
		}
		else
			AddErrorAndMessage("M53","The handset is in DL mode. Can't close connection. ");
	}
	else
		bRes = true;
	return bRes;
}
void CDLProcess::WriteLog()
{
	if(m_bSaveLog)
	{
		CSingleLock singleLock(&m_WriteLogCritSection,true);

		CStdioFile File_Ctrl1;
		char temp[512] ={0};
		m_strLog+="=============================END============================";
		CTime time = CTime::GetCurrentTime();
		sprintf(temp,".\\log\\Port%d_%s.log",m_nPort,time.Format("%Y%m%d"));
		if(File_Ctrl1.Open(temp,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate,NULL))
		{
			File_Ctrl1.SeekToEnd();
			File_Ctrl1.WriteString(m_strLog);
			File_Ctrl1.WriteString("\n\n");
			File_Ctrl1.Close();
		}
	}
}
void CDLProcess::WriteResultLog(CSpendTime* pSpendTime)
{
	CSingleLock singleLock(&m_WriteResultLogCritSection,true);

	//1. MobileID
	std::string strLog =m_strMobileID;
	strLog += ", ";
	//2. ErrorCode
	strLog += m_strErrorCode;
	strLog += ", ";

	//3. total time
	char cpTemp[100] = {0};
	sprintf(cpTemp,"%.3f",(GetTickCount()-(pSpendTime->GetStartTime()))/1000.0);
	strLog += cpTemp;

	CStdioFile File_Ctrl1;
	char temp[512] ={0};
	CTime time = CTime::GetCurrentTime();
	sprintf(temp,".\\log\\Result_Port%d_%s.log",m_nPort,time.Format("%Y%m%d"));
	File_Ctrl1.Open(temp,CFile::modeWrite|CFile::modeCreate|CFile::modeNoTruncate,NULL);
	File_Ctrl1.SeekToEnd();
	File_Ctrl1.WriteString(strLog.c_str());
	File_Ctrl1.WriteString("\n\n");
	File_Ctrl1.Close();
}