// ImplementedDiagCommandTesterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImplementedDiagCommandTester.h"
#include "ImplementedDiagCommandTesterDlg.h"
#include "..\..\Modules\ODMCoachModule\ODMExport.h"
#include <string>
#include "..\SPU_Loader\LoaderExport.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "../../Components\ReportCenter\IReport.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

using namespace std;


CImplementedDiagCommandTesterDlg::CImplementedDiagCommandTesterDlg(CWnd* pParent /*=NULL*/)
: CDialog(CImplementedDiagCommandTesterDlg::IDD, pParent)
,m_nPortNum(0)
,m_pDownload(NULL)
,m_bRunSelfTest(false)
{	
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts > 0) 
        m_nPortNum = Ports[0];
}

CImplementedDiagCommandTesterDlg::~CImplementedDiagCommandTesterDlg() 
{
    if(m_pDownload)
    {
        delete m_pDownload;
        m_pDownload = NULL;
    }
}

void CImplementedDiagCommandTesterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_cmdList);
	DDX_Control(pDX, IDC_COMMANDINPUT, m_cmdInput);
	DDX_Control(pDX, IDC_COMPort, m_COMPortCtrl);
	DDX_Control(pDX, IDC_Repeat, m_RepeatCtrl);
	DDX_Control(pDX, IDC_LogLIST, m_LogLISTCtrl);
	DDX_Control(pDX, IDC_LOOP1, m_editLoop1);
	DDX_Control(pDX, IDC_LOOP2, m_editLoop2);
	DDX_Control(pDX, IDC_LOOP3, m_editLoop3);
	DDX_Control(pDX, IDC_LOOP4, m_editLoop4);
	DDX_Control(pDX, IDC_LOOP5, m_editLoop5);
	DDX_Control(pDX, IDC_LOOP6, m_editLoop6);
}

BEGIN_MESSAGE_MAP(CImplementedDiagCommandTesterDlg, CDialog)
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
    ON_BN_CLICKED(IDOK, &CImplementedDiagCommandTesterDlg::OnBnClickedOk)
	ON_BN_CLICKED(ID_UpdatePort, &CImplementedDiagCommandTesterDlg::OnBnClickedUpdateport)
	ON_BN_CLICKED(IDC_LOOPBUTTON, &CImplementedDiagCommandTesterDlg::OnBnClickedLoopbutton)
	ON_BN_CLICKED(IDC_Clear_BUTTON, &CImplementedDiagCommandTesterDlg::OnBnClickedClearButton)
END_MESSAGE_MAP()


// CImplementedDiagCommandTesterDlg message handlers

void CImplementedDiagCommandTesterDlg::LocdFunction() 
{
#ifdef _DEBUG
    HMODULE MobileToolKitDll = ::LoadLibrary(_T("MobileToolKit_StaticMD.dll"));
	HMODULE QisdaNVModuleDll = ::LoadLibrary(_T("QisdaNVModule_StaticMD.dll"));
	HMODULE DownloadExportDll =::LoadLibrary(_T("Download_Export_StaticMD.dll"));
	HMODULE SoundUtilityDll =::LoadLibrary(_T("SoundUtility_StaticUD.dll"));
	HMODULE QCH_DatabastAccessDll =::LoadLibrary(_T("QCH_DatabastAccess_StaticUD.dll"));
	HMODULE GPIBControlDll =::LoadLibrary(_T("GPIBControl_WIN32UD.dll"));
	HMODULE EFSModuleDll =::LoadLibrary(_T("QisdaEFsModule_StaticMD.dll"));	
#else
	HMODULE MobileToolKitDll = ::LoadLibrary(_T("MobileToolKit_StaticM.dll"));
	HMODULE QisdaNVModuleDll = ::LoadLibrary(_T("QisdaNVModule_StaticM.dll"));
	HMODULE DownloadExportDll =::LoadLibrary(_T("Download_Export_StaticM.dll"));
	HMODULE SoundUtilityDll =::LoadLibrary(_T("SoundUtility_StaticU.dll"));
	HMODULE QCH_DatabastAccessDll =::LoadLibrary(_T("QCH_DatabastAccess_StaticU.dll"));
	HMODULE GPIBControlDll =::LoadLibrary(_T("GPIBControl_WIN32U.dll"));
	HMODULE EFSModuleDll =::LoadLibrary(_T("QisdaEFsModule_StaticM.dll"));	
#endif    
	if(EFSModuleDll)
	{
		EFS_ReadFile = (pFnEFS_ReadFile)GetProcAddress(EFSModuleDll, "EFS_ReadFile");
		EFS_ListFolder = (pFnEFS_ListFolder)GetProcAddress(EFSModuleDll, "EFS_ListFolder");
		EFS_GetLocFile = (pFnEFS_GetLocFile)GetProcAddress(EFSModuleDll, "EFS_GetLocFile");
		EFS_GetLocFileName = (pFnEFS_GetLocFileName)GetProcAddress(EFSModuleDll, "EFS_GetLocFileName");
	}
	if(QCH_DatabastAccessDll)
	{
		QuerySPC = (pFnQuerySPC)GetProcAddress(QCH_DatabastAccessDll, "QuerySPC");
	}
	if(SoundUtilityDll)
	{
		AnalysisRecord = (pFnAnalysisRecord)GetProcAddress(SoundUtilityDll, "AnalysisRecord");
		AnalysisRecordStereo = (pFnAnalysisRecordStereo)GetProcAddress(SoundUtilityDll, "AnalysisRecordStereo");
	}
    if(MobileToolKitDll != NULL) 
	{
		USBChargingSwitch = (pFnUSBChargingSwitch)GetProcAddress(MobileToolKitDll, "USBChargingSwitch");
		CoinCellChargingSwitch = (pFnCoinCellChargingSwitch)GetProcAddress(MobileToolKitDll, "CoinCellChargingSwitch");
		KeypadPressTest = (pFnKeypadPressTest)GetProcAddress(MobileToolKitDll, "KeypadPressTest");
		KeypadBacklight = (pFnKeypadBacklight)GetProcAddress(MobileToolKitDll, "KeypadBacklight");

		EnableLog = (pFnEnableLog)GetProcAddress(MobileToolKitDll, "EnableLog");

		RestoreBrt_DOWNLOADEXPORT = (pFnRestoreBrt)GetProcAddress(DownloadExportDll, "RestoreBrt");
        ReadIMEI = (pFnGetIMEI)GetProcAddress(MobileToolKitDll, "GetIMEI");
		WriteIMEI = (pFnWriteIMEI)GetProcAddress(MobileToolKitDll, "WriteIMEI");
        WriteHeadsetBTAddress = (pFnWriteHeadsetBTAddress)GetProcAddress(MobileToolKitDll, "WriteHeadsetBTAddress");
        SetTestMode = (pFnSetTestMode)GetProcAddress(MobileToolKitDll, "SetTestMode");
        SetOnlineMode = (pFnSetOnlineMode)GetProcAddress(MobileToolKitDll, "SetOnlineMode");
        IsDLMode = (pFnIsDLMode)GetProcAddress(MobileToolKitDll, "IsDLMode");

		//SPC functions
        LockSPC = (pFnLockSPC)GetProcAddress(MobileToolKitDll, "LockSPC");
		IsSPCLocked = (pFnIsSPCLocked)GetProcAddress(MobileToolKitDll, "IsSPCLocked");
		UnlockSPC = (pFnUnlockSPC)GetProcAddress(MobileToolKitDll, "UnlockSPC");
		ProgramSPC = (pFnProgramSPC)GetProcAddress(MobileToolKitDll, "ProgramSPC");

		//Backup\Restore NV		
		BackupNVPartition = (pFnBackupNVPartition)GetProcAddress(MobileToolKitDll, "BackupNVPartition");
		RestoreNVPartition = (pFnRestoreNVPartition)GetProcAddress(MobileToolKitDll, "RestoreNVPartition");
		CheckNVPartition = (pFnCheckNVPartition)GetProcAddress(MobileToolKitDll, "CheckNVPartition");
        BackupNV2BRT = (pFnBackupNV2BRT)GetProcAddress(MobileToolKitDll, "BackupNV2BRT");
        RestoreBRT2NV = (pFnRestoreBRT2NV)GetProcAddress(MobileToolKitDll, "RestoreBRT2NV");

        UnlockPhoneCode = (pFnUnlockPhoneCode)GetProcAddress(MobileToolKitDll, "UnlockPhoneCode");
        UnlockNW = (pFnUnlockNW)GetProcAddress(MobileToolKitDll, "UnlockNW");
        UnlockNS = (pFnUnlockNS)GetProcAddress(MobileToolKitDll, "UnlockNS");
        UnlockSP = (pFnUnlockSP)GetProcAddress(MobileToolKitDll, "UnlockSP");
        UnlockCP = (pFnUnlockCP)GetProcAddress(MobileToolKitDll, "UnlockCP");
        UnlockSIM = (pFnUnlockSIM)GetProcAddress(MobileToolKitDll, "UnlockSIM");
        UnlockPROP = (pFnUnlockPROP)GetProcAddress(MobileToolKitDll, "UnlockPROP");
        ReadFactoryArea = (pFnReadFactoryArea)GetProcAddress(MobileToolKitDll, "ReadFactoryArea");
        WriteFactoryArea = (pFnWriteFactoryArea)GetProcAddress(MobileToolKitDll, "WriteFactoryArea");
        ReadFactoryAreaNew = (pFnReadFactoryAreaNew)GetProcAddress(MobileToolKitDll, "ReadFactoryAreaNew");        
		ReadMobileID_ForSPU = (pFnReadMobileID_ForSPU)GetProcAddress(MobileToolKitDll, "ReadMobileID_ForSPU");   
		WriteMobileID_ForSPU = (pFnWriteMobileID_ForSPU)GetProcAddress(MobileToolKitDll, "WriteMobileID_ForSPU");    
		ReadHWVersion_FromFactoryArea = (pFnReadHWVersion_FromFactoryArea)GetProcAddress(MobileToolKitDll, "ReadHWVersion_FromFactoryArea");   
		WriteHWVersion_ToFactoryArea = (pFnWriteHWVersion_ToFactoryArea)GetProcAddress(MobileToolKitDll, "WriteHWVersion_ToFactoryArea");   

		ReadFactoryHWSWMW = (pFnReadFactoryHWSWMW)GetProcAddress(MobileToolKitDll, "ReadFactoryHWSWMW");
		WriteFactoryHWSWMW = (pFnWriteFactoryHWSWMW)GetProcAddress(MobileToolKitDll, "WriteFactoryHWSWMW");
		ReadProcessInfo = (pFnReadProcessInfo)GetProcAddress(MobileToolKitDll, "ReadProcessInfo");
		WriteProcessInfo = (pFnWriteProcessInfo)GetProcAddress(MobileToolKitDll, "WriteProcessInfo");
		ReadTrackID = (pFnReadTrackID)GetProcAddress(MobileToolKitDll, "ReadTrackID");
		WriteTrackID = (pFnWriteTrackID)GetProcAddress(MobileToolKitDll, "WriteTrackID");
		ReadPicassoNumber = (pFnReadPicassoNumber)GetProcAddress(MobileToolKitDll, "ReadPicassoNumber");
		WritePicassoNumber = (pFnWritePicassoNumber)GetProcAddress(MobileToolKitDll, "WritePicassoNumber");
        ClearActivationData = (pFnClearActivationData)GetProcAddress(QisdaNVModuleDll, "CAD");
        StandbyCurrentTest = (pFnStandbyCurrentTest)GetProcAddress(MobileToolKitDll, "StandbyCurrentTest");
        MasterClear = (pFnMasterClear)GetProcAddress(MobileToolKitDll, "MasterClear");
        WriteQFuse = (pFnWriteQFuse)GetProcAddress(MobileToolKitDll, "WriteQFuse");
		ReadQFuse = (pFnReadQFuseState)GetProcAddress(MobileToolKitDll, "ReadQFuseState");
        BTHeadsetChargingPower = (pFnBTHeadsetChargingPower)GetProcAddress(MobileToolKitDll, "BTHeadsetChargingPower");
        CheckSIMCardExistence = (pFnCheckSIMCardExistence)GetProcAddress(QisdaNVModuleDll, "CSIMCE");
		BTHeadsetVoltageReading = (pFnBTHeadsetVoltageReading)GetProcAddress(MobileToolKitDll, "BTHeadsetVoltageReading");
        SetOnline = (pFnSetOnline)GetProcAddress(MobileToolKitDll, "SetOnline");
        SetOffline = (pFnSetOffline)GetProcAddress(MobileToolKitDll, "SetOffline");
        ResetHandset = (pFnResetHandset)GetProcAddress(MobileToolKitDll, "ResetHandset");
		ChangeFTM = (pFnChangeFTM)GetProcAddress(MobileToolKitDll, "ChangeFTM");
		ChangeMode = (pFnChangeMode)GetProcAddress(MobileToolKitDll, "ChangeMode");		
		CheckFTM = (pFnCheckFTM)GetProcAddress(MobileToolKitDll, "CheckFTM");		

        CheckBTHeadset = (pFnCheckBTHeadsetState)GetProcAddress(MobileToolKitDll, "CheckBTHeadsetState");
        ChangeOnline = (pFnChangeOnline)GetProcAddress(MobileToolKitDll, "ChangeOnline");
		WriteSIMLockFileNameToNV = (pFnWriteSIMLockFileNameToNV)GetProcAddress(MobileToolKitDll, "WriteSIMLockFileNameToNV");
		ReadSIMLockFileNameFromNV = (pFnReadSIMLockFileNameFromNV)GetProcAddress(MobileToolKitDll, "ReadSIMLockFileNameFromNV");
		WriteSimLockFile = (pFnWriteSimLockFile)GetProcAddress(MobileToolKitDll, "WriteSimLockFile");

		EnableQualcommDebugEvent = (pFnEnableQualcommDebugEvent)GetProcAddress(MobileToolKitDll, "EnableQualcommDebugEvent");

		GetSWHWVersion = (pFnGetSWHWVersion)GetProcAddress(MobileToolKitDll, "GetSWHWVersion");
		GetLavernockHWVersion = (pFnGetLavernockHWVersion)GetProcAddress(MobileToolKitDll, "GetLavernockHWVersion");
		GetLavernockHWVersionV2_ForSPU = (pFnGetLavernockHWVersionV2_ForSPU)GetProcAddress(MobileToolKitDll, "GetLavernockHWVersionV2_ForSPU");
		GetMicroSDInfo_ForSPU = (pFnGetMicroSDInfo_ForSPU)GetProcAddress(MobileToolKitDll, "GetMicroSDInfo_ForSPU");

		ReadNVItem = (pFnReadNVItem)GetProcAddress(MobileToolKitDll, "ReadNVItem");
		ReadNVItem_ForSPU = (pFnReadNVItem_ForSPU)GetProcAddress(MobileToolKitDll, "ReadNVItem_ForSPU");
		WriteNVItem = (pFnWriteNVItem)GetProcAddress(MobileToolKitDll, "WriteNVItem");

		WriteFailureByte = (pFnWriteFailureByte)GetProcAddress(MobileToolKitDll, "WriteFailureByte");

		TurnOnOffEM = (pFnTurnOnOffEM)GetProcAddress(MobileToolKitDll, "TurnOnOffEM");

		WriteDefaultPhoneCode = (pFnWriteDefaultPhoneCode)GetProcAddress(MobileToolKitDll, "WriteDefaultPhoneCode");

		ReadBatteryVoltage = (pFnReadBatteryVoltage)GetProcAddress(MobileToolKitDll, "ReadBatteryVoltage");
		ReadBatteryTemperature = (pFnReadBatteryTemperature)GetProcAddress(MobileToolKitDll, "ReadBatteryTemperature");

		IsOTPFrozen = (pFnIsOTPFrozen)GetProcAddress(MobileToolKitDll, "IsOTPFrozen");
		FreezeOTP = (pFnFreezeOTP)GetProcAddress(MobileToolKitDll, "FreezeOTP");

		Replace_GrpDList_File = (pFnReplaceEFSFile)GetProcAddress(MobileToolKitDll, "ReplaceEFSFile");

		IsGSensorWorking = (pFnIsGSensorWorking)GetProcAddress(MobileToolKitDll, "IsGSensorWorking");
		IsGSensorWorkingV2 = (pFnIsGSensorWorkingV2)GetProcAddress(MobileToolKitDll, "IsGSensorWorkingV2");
		IsGSensorWorkingV3 = (pFnIsGSensorWorkingV3)GetProcAddress(MobileToolKitDll, "IsGSensorWorkingV3");
		CheckSIMLock = (pFnCheckSIMLock_VC6)GetProcAddress(MobileToolKitDll, "CheckSIMLock_VC6");
		CheckCEFSFileName = (pFnCheckCEFSFileName)GetProcAddress(MobileToolKitDll, "CheckCEFSFileName");		
		GetBTAddr = (pFnGetBTAddr)GetProcAddress(MobileToolKitDll, "GetBTAddr");		

		DLRAWRequest = (pFnDLRAWRequest)GetProcAddress(MobileToolKitDll, "DLRAWRequest");
		DiagRAWRequest = (pFnDiagRAWRequest)GetProcAddress(MobileToolKitDll, "DiagRAWRequest");
		ByPassCellTemperatureCheck = (pFnByPassCellTemperatureCheck)GetProcAddress(MobileToolKitDll, "ByPassCellTemperatureCheck");
    }
	if(GPIBControlDll)
	{
		GPIBSetControlIndex = (pfnGPIB_SetControlIndex)GetProcAddress(GPIBControlDll, "GPIB_SetControlIndex");
		GPIBGetDeviceCount = (pfnGPIB_GetDeviceCount)GetProcAddress(GPIBControlDll, "GPIB_GetDeviceCount");
		GPIBQueryDeviceDescription = (pfnGPIB_QueryDeviceDescription)GetProcAddress(GPIBControlDll, "GPIB_QueryDeviceDescription");
		GPIBSetCurrentVoltage = (pfnGPIB_SetCurrentVoltage)GetProcAddress(GPIBControlDll, "GPIB_SetCurrentVoltage");
		GPIBIsPowerSupply = (pfnGPIB_IsPowerSupply)GetProcAddress(GPIBControlDll, "GPIB_IsPowerSupply");
		GPIBInitialization = (pfnGPIB_Initialization)GetProcAddress(GPIBControlDll, "GPIB_Initialization");
		GPIBSendCommmand = (pfnGPIB_SendCommmand)GetProcAddress(GPIBControlDll, "GPIB_SendCommmand");
		GPIBSendQueryCommmand = (pfnGPIB_SendQueryCommmand)GetProcAddress(GPIBControlDll, "GPIB_SendQueryCommmand");
		GPIBReadCommandResult = (pfnGPIB_ReadCommandResult)GetProcAddress(GPIBControlDll, "GPIB_ReadCommandResult");
		GPIBErrorMsg = (pfnGPIB_ErrorMsg)GetProcAddress(GPIBControlDll, "GPIB_ErrorMsg");
	}
}

bool CImplementedDiagCommandTesterDlg::CheckBreak()
{
	return false;
}

bool CImplementedDiagCommandTesterDlg::WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize)
{
	if(strName == _T("COMPort"))
	{
		CString strCOMPort;
		m_COMPortCtrl.GetWindowText(strCOMPort);
		_tcscpy_s(szBuffer,nBufferSize,strCOMPort);
	}
	else	if(strName == _T("Input"))
	{
		CString strInput;
		m_cmdInput.GetWindowText(strInput);
		_tcscpy_s(szBuffer,nBufferSize,strInput);
	}
	else
	{
		int nSelected = m_cmdList.GetCurSel();
		CString strSelected;
		m_cmdList.GetText(nSelected, strSelected);
		_tcscpy_s(szBuffer,nBufferSize,strSelected);
	}
	return true;
}

bool CImplementedDiagCommandTesterDlg::Print(const TSTRING& strName,const TSTRING& strOutput)
{
	using namespace QSYNC_LIB ;

	if(strName == _T("API"))
	{
		m_cmdList.AddString(strOutput.c_str());
	}
	else if(strName == _T("REMARK"))
	{
		TSTRING strDisplay = strOutput + _T("---------------------REMARK");
		m_cmdList.AddString(strDisplay.c_str());
	}
	else
	{
		TSTRING strVariable,strValue;
		GetToken(strOutput,strVariable,strValue,_T(":"));
		if(strVariable == _T("API"))
			m_cmdList.AddString(strValue.c_str());
		else
			AddLog(strOutput.c_str());
	}
	return true;
}

BOOL CImplementedDiagCommandTesterDlg::OnInitDialog()
{	
	BOOL bRes = CDialog::OnInitDialog();

	HANDLE hSPU = GetSPULoader(this);
	if(hSPU)    
	{
		if(LoadScript(hSPU,_T("AddAPI.script")))
			ExecuteCommand(hSPU);
		ReleaseHandle(hSPU);
	}

	m_RepeatCtrl.SetWindowText(_T("1"));
	UpdatePort(m_nPortNum);
	LocdFunction();

	if(m_cmdList.GetCount())
		m_cmdList.SetCurSel(0);

    return bRes;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CImplementedDiagCommandTesterDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialog::OnPaint();
    }
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CImplementedDiagCommandTesterDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}


void CImplementedDiagCommandTesterDlg::OnBnClickedOk()
{	
	OnBnClickedUpdateport();

	CString strRepeat;
	m_RepeatCtrl.GetWindowText(strRepeat);
	int nRepeat = ::_ttoi(strRepeat);
	for(int i=0;i<nRepeat;i++)
	{
		Function(m_nPortNum);
	}
}

void CImplementedDiagCommandTesterDlg::Function(int nPort)
{
    int nSelected = m_cmdList.GetCurSel();
    CString strSelected;
    m_cmdList.GetText(nSelected, strSelected);

	CalFunctionTime calTime(this,strSelected);

#ifdef _DEBUG
	CString strScriptFile = _T("Script\\") + strSelected + _T("_Debug.script");
#else
	CString strScriptFile = _T("Script\\") + strSelected + _T(".script");
#endif
	if(::_taccess(strScriptFile,0) != -1)
	{
		HANDLE hSPU = GetSPULoader(this);
		if(hSPU)    
		{
			if(LoadScript(hSPU,(LPCTSTR)strScriptFile))
				ExecuteCommand(hSPU);
			ReleaseHandle(hSPU);
		}
		return;
	}

	if(strSelected.Find(_T("---------------------REMARK")) >= 0)
		return;

    int nStatus = -1;
    int nVersion = -1;
	if(strSelected == _T("Do All Test"))
	{
		for(int i=0;i<m_cmdList.GetCount();i++)
		{
			CString strSelected;
			m_cmdList.GetText(i, strSelected);
			if(strSelected == _T("Do All Test"))
				continue;
			else
			{
				m_bRunSelfTest = true;
				m_cmdList.SetCurSel(i);
				Function(nPort);
			}
		}
		m_bRunSelfTest = false;
	}
	else if(strSelected == _T("EFS_ReadFile"))
	{
		if(EFS_ReadFile != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);
			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			int nIndex = strInput.ReverseFind('/');

			if(strInput.GetLength() >= 1 && nIndex >= 0)
			{
				TSTRING strFileName;
				strFileName = strInput.Mid(nIndex+1);
				if(EFS_ReadFile((LPSTR)(LPCTSTR)strCOMPort,(LPCTSTR)strInput,strFileName.c_str()))
				{		
					AddLog(strFileName.c_str());
					AddLog(_T("EFS_ReadFile Success"));
				}
				else
					AddLog(_T("EFS_ReadFile Fail"));
			}
			else
				AddLog(_T("Please input folder name which want to list"));
		}
		else
			AddLog(_T("EFS_ReadFile Load dll fail "));
	}
	else if(strSelected == _T("EFS_ListFolder"))
	{
		if(EFS_ListFolder != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() >= 1)
			{
				char szBuffer[2048] = {0};
				if(EFS_ListFolder((LPSTR)(LPCTSTR)strCOMPort,strInput,szBuffer,"2048"))
				{
					int nFolderLength = strInput.GetLength() == 1 ? 1 : strInput.GetLength()+1;

					using namespace QSYNC_LIB;
					TSTRING strVariable,strValue;
					GetToken(szBuffer,strVariable,strValue,_T("^="));
					if(static_cast<int>(strVariable.size()) >= nFolderLength)
						strVariable = strVariable.substr(nFolderLength);
					AddLog(strVariable.c_str());
					while(strValue.empty() == false)
					{
						GetToken(strValue,strVariable,strValue,_T("^="));
						if(static_cast<int>(strVariable.size()) >= nFolderLength)
							strVariable = strVariable.substr(nFolderLength);
						AddLog(strVariable.c_str());
					};					
					AddLog(_T("EFS_ListFolder Success"));
				}
				else
					AddLog(_T("EFS_ListFolder Fail"));
			}
			else
				AddLog(_T("Please input folder name which want to list, try \"/\""));
		}
		else
			AddLog(_T("EFS_ListFolder Load dll fail "));
	}
	else if(strSelected == _T("EFS_GetLocFile"))
	{
		if(EFS_GetLocFile != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	
			char szLocFileName[100] = {0};
				if(EFS_GetLocFile((LPSTR)(LPCTSTR)strCOMPort, szLocFileName))
				{		
					AddLog(szLocFileName);
					AddLog(_T("EFS_GetLocFile Success"));
				}
				else
					AddLog(_T("EFS_GetLocFile Fail"));
		}
		else
			AddLog(_T("EFS_GetLocFile Load dll fail "));
	}
	else if(strSelected == _T("EFS_GetLocFileName"))
	{
		if(EFS_GetLocFileName != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	
			char szLocFileName[100] = {0};
			if(EFS_GetLocFileName((LPSTR)(LPCTSTR)strCOMPort, szLocFileName))
			{		
				AddLog(szLocFileName);
				AddLog(_T("EFS_GetLocFileName Success"));
			}
			else
				AddLog(_T("EFS_GetLocFileName Fail"));
		}
		else
			AddLog(_T("EFS_GetLocFileName Load dll fail "));
	}
	else if(strSelected == _T("KeypadBacklightTest_Open"))
	{
		if(KeypadBacklight != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(KeypadBacklight((LPSTR)(LPCTSTR)strCOMPort,"true"))
			{
				AddLog(_T("KeypadBacklightTest_Open Success"));
			}
			else
				AddLog(_T("KeypadBacklightTest_Open Fail"));
		}
		else
			AddLog(_T("KeypadBacklightTest Load dll fail "));
	}
	else if(strSelected == _T("KeypadBacklightTest_Close"))
	{
		if(KeypadBacklight != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(KeypadBacklight((LPSTR)(LPCTSTR)strCOMPort,"false"))
			{
				AddLog(_T("KeypadBacklightTest_Close Success"));
			}
			else
				AddLog(_T("KeypadBacklightTest_Close Fail"));
		}
		else
			AddLog(_T("KeypadBacklight Load dll fail "));
	}
	else if(strSelected == _T("KeypadPressTest_Open"))
	{
		if(KeypadPressTest != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(KeypadPressTest((LPSTR)(LPCTSTR)strCOMPort,"true"))
			{
				AddLog(_T("KeypadPressTest_Open Success"));
			}
			else
				AddLog(_T("KeypadPressTest_Open Fail"));
		}
		else
			AddLog(_T("KeypadPressTest Load dll fail "));
	}
	else if(strSelected == _T("KeypadPressTest_Close"))
	{
		if(KeypadPressTest != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(KeypadPressTest((LPSTR)(LPCTSTR)strCOMPort,"false"))
			{
				AddLog(_T("KeypadPressTest_Close Success"));
			}
			else
				AddLog(_T("KeypadPressTest_Close Fail"));
		}
		else
			AddLog(_T("KeypadPressTest Load dll fail "));
	}
	else if(strSelected == _T("CoinCellChargingSwitch_Open"))
	{
		if(CoinCellChargingSwitch != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(CoinCellChargingSwitch((LPSTR)(LPCTSTR)strCOMPort,"true"))
			{
				AddLog(_T("CoinCellChargingSwitch_Open Success"));
			}
			else
				AddLog(_T("CoinCellChargingSwitch_Open Fail"));
		}
		else
			AddLog(_T("CoinCellChargingSwitch Load dll fail "));
	}
	else if(strSelected == _T("CoinCellChargingSwitch_Close"))
	{
		if(CoinCellChargingSwitch != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(CoinCellChargingSwitch((LPSTR)(LPCTSTR)strCOMPort,"false"))
			{
				AddLog(_T("USBChargingSwitch_Close Success"));
			}
			else
				AddLog(_T("CoinCellChargingSwitchClose Fail"));
		}
		else
			AddLog(_T("CoinCellChargingSwitch Load dll fail "));
	}
	else if(strSelected == _T("USBChargingSwitch_Open"))
	{
		if(USBChargingSwitch != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(USBChargingSwitch((LPSTR)(LPCTSTR)strCOMPort,"true"))
			{
				AddLog(_T("USBChargingSwitch_Open Success"));
			}
			else
				AddLog(_T("USBChargingSwitch_Open Fail"));
		}
		else
			AddLog(_T("CoinCellChargingSwitch Load dll fail "));
	}
	else if(strSelected == _T("USBChargingSwitch_Close"))
	{
		if(USBChargingSwitch != NULL) 
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			if(USBChargingSwitch((LPSTR)(LPCTSTR)strCOMPort,"false"))
			{
				AddLog(_T("USBChargingSwitch_Close Success"));
			}
			else
				AddLog(_T("USBChargingSwitch_Close Fail"));
		}
		else
			AddLog(_T("USBChargingSwitch Load dll fail "));
	}
	else if(strSelected == _T("QuerySPC"))
	{
		AddLog(_T("The API should use in the special PC to access QCH's database"));
        if(QuerySPC != NULL) 
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() >= 15)
			{
				char szBuffer[1024] = {0};
				if(QuerySPC((LPTSTR)(LPCTSTR)strInput,szBuffer))
				{
					AddLog(szBuffer);
					AddLog(_T("QuerySPC Success"));
				}
				else
					AddLog(_T("QuerySPC Fail"));
			}
			else
			{
				AddLog(_T("Input IMEI wrong."));
			}
        }
		else
			AddLog(_T("QuerySPC Load dll fail "));
	}
	else if(strSelected == _T("EnableLog"))
	{
		if(EnableLog != NULL) 
		{
			if(EnableLog(true))
			{
				AddLog(_T("EnableLog Success"));
			}
			else
				AddLog(_T("EnableLog Fail"));
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("DisableLog"))
	{
		if(EnableLog != NULL) 
		{
			if(EnableLog(false))
			{
				AddLog(_T("DisableLog Success"));
			}
			else
				AddLog(_T("DisableLog Fail"));
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("AnalysisRecord"))
	{
		if(AnalysisRecord != NULL) 
		{
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			if(strInput.IsEmpty() == false)
			{
				int nAmplitude = 0;
				int nFrequence = 0;
				if(AnalysisRecord(_ttoi(strInput),nAmplitude,nFrequence,"test.wav"))
				{
					CString strFormat;
					strFormat.Format(_T("Amplitude %d , Frequence %d"),nAmplitude,nFrequence);
					AddLog(strFormat);
					AddLog(_T("AnalysisRecord Success"));
				}
				else
					AddLog(_T("AnalysisRecord Fail"));
			}
			else
			{
				AddLog(_T("Please input duration by minsecond"));
			}
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("AnalysisRecordStereo"))
	{
		if(AnalysisRecordStereo != NULL) 
		{
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			if(strInput.IsEmpty() == false)
			{
				int nAmplitudeLeft = 0;
				int nFrequenceLeft = 0;
				int nAmplitudeRight = 0;
				int nFrequenceRight = 0;
				if(AnalysisRecordStereo(_ttoi(strInput),nAmplitudeLeft,nFrequenceLeft,nAmplitudeRight,nFrequenceRight,"test.wav"))
				{
					CString strFormat;
					strFormat.Format(_T("AmplitudeLeft %d , FrequenceLeft %d"),nAmplitudeLeft,nFrequenceLeft);
					AddLog(strFormat);
					strFormat.Format(_T("AmplitudeRight %d , FrequenceRight %d"),nAmplitudeRight,nFrequenceRight);
					AddLog(strFormat);
					AddLog(_T("AnalysisRecordStereo Success"));
				}
				else
					AddLog(_T("AnalysisRecordStereo Fail"));
			}
			else
			{
				AddLog(_T("Please input duration by minsecond"));
			}
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("ByPassCellTemperatureCheck"))
	{
		if(ByPassCellTemperatureCheck != NULL) 
		{
			if(ByPassCellTemperatureCheck(nPort))
				AddLog(_T("ByPassCellTemperatureCheck Success"));
			else
				AddLog(_T("ByPassCellTemperatureCheck Fail"));
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("GetBTAddr"))
	{
		if(GetBTAddr != NULL) 
		{
			char szBTAddr[13] = {0};
			if(GetBTAddr(nPort,szBTAddr,13))
			{
				AddLog(szBTAddr);
				AddLog(_T("GetBTAddr Success"));
			}
			else
				AddLog(_T("GetBTAddr Fail"));
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("CheckCEFSFileName"))
	{
		if(CheckCEFSFileName != NULL) 
		{
			char szCEFSFileName[256] = {0};
			if(CheckCEFSFileName(nPort,szCEFSFileName,256))
			{
				AddLog(szCEFSFileName);
				AddLog(_T("CheckCEFSFileName Success"));
			}
			else
				AddLog(_T("CheckCEFSFileName Fail"));
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("CheckSIMLock"))
	{
		if(CheckSIMLock != NULL) 
		{
			char szSIMLock[64] = {0};
			if(CheckSIMLock(nPort,szSIMLock,64))
			{
				AddLog(szSIMLock);
				AddLog(_T("CheckSIMLock Success"));
			}
			else
				AddLog(_T("CheckSIMLock Fail"));
		}
		else
			AddLog(_T("Load function Fail "));
	}
	//IsGSensorWorkingV3
	else if(strSelected == _T("IsGSensorWorkingV3"))
	{
		if(IsGSensorWorkingV3 != NULL) 
		{
			int nX = 0;
			int nY = 0;
			int nZ = 0;
			if(IsGSensorWorkingV3(nPort,nX,nY,nZ))
			{
				CString strFormat;
				strFormat.Format(_T("GSensor X %d,Y %d,Z %d"),nX,nY,nZ);
				AddLog(strFormat);
				AddLog(_T("IsGSensorWorkingV3 Success"));
			}
			else
			{
				AddLog(_T("IsGSensorWorkingV3 Fail"));
			}
		}
		else
			AddLog(_T("Load function Fail "));
	}
	//IsGSensorWorkingV2
	else if(strSelected == _T("IsGSensorWorkingV2"))
	{
		if(IsGSensorWorkingV2 != NULL) 
		{
			bool bIsGSensorWork = false;
			bool bIsUpWork = false;
			if(IsGSensorWorkingV2(nPort,bIsGSensorWork,bIsUpWork))
				AddLog(_T("IsGSensorWorkingV2 Success"));
			else
			{
				CString strFormat;
				strFormat.Format(_T("GSensor %d, Up %d"),bIsGSensorWork,bIsUpWork);
				AddLog(strFormat);
				AddLog(_T("IsGSensorWorkingV2 Fail"));
			}
		}
		else
			AddLog(_T("Load function Fail "));
	}
	//IsGSensorWorking
	else if(strSelected == _T("IsGSensorWorking"))
	{
		if(IsGSensorWorking != NULL) 
		{
			if(IsGSensorWorking(nPort))
				AddLog(_T("IsGSensorWorking Success"));
			else
				AddLog(_T("IsGSensorWorking Fail"));
		}
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("Replace_GrpDList_File"))
	{
        if(Replace_GrpDList_File != NULL) 
		{
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			if(strInput.IsEmpty() == false && ::_taccess(strInput,0) != -1)
			{
				if(Replace_GrpDList_File(nPort,_T("\\brew\\GrpDList ver 01.db"),(LPCTSTR)strInput))
				{
					AddLog(_T("Replace_GrpDList_File Success"));
				}
				else
					AddLog(_T("Replace_GrpDList_File Fail"));
			}
			else
			{
				AddLog(_T("The file from input is not exist"));
			}
        }
		else
			AddLog(_T("Load function Fail "));
	}
	else if(strSelected == _T("RestoreBrt_DOWNLOADEXPORT"))
	{
        if(RestoreBrt_DOWNLOADEXPORT != NULL) 
		{
            if(RestoreBrt_DOWNLOADEXPORT(nPort,_T("test.brt"),true))
			{
				AddLog(_T("RestoreBrt_DOWNLOADEXPORT Success"));
			}
			else
				AddLog(_T("RestoreBrt_DOWNLOADEXPORT Fail"));
        }
		else
			AddLog(_T("Load function Fail "));
	}
    else if(strSelected == _T("FreezeOTP"))
    {
        if(FreezeOTP != NULL) 
		{
            if(FreezeOTP(nPort))
			{
				AddLog(_T("FreezeOTP Success"));
			}
			else
				AddLog(_T("FreezeOTP Fail"));
        }
		else
			AddLog(_T("Load function Fail "));
    }
    else if(strSelected == _T("IsOTPFrozen"))
    {
        if(IsOTPFrozen != NULL) 
		{
			int nLastError = 0;
			bool bIsFrozen = false;
            if(IsOTPFrozen(nPort,bIsFrozen,nLastError))
			{
				CString strFormat;
				strFormat.Format(_T("%d"),nLastError);
				AddLog(strFormat);
				if(bIsFrozen)
					AddLog(_T("Is OTPFrozen"));
				else
					AddLog(_T("Isn't OTPFrozen"));
			}
			else
			{
				CString strFormat;
				strFormat.Format(_T("%d"),nLastError);
				AddLog(strFormat);
				AddLog(_T("IsOTPFrozen fail"));
			}
        }
		else
			AddLog(_T("Load function Fail "));
    }//ReadBatteryTemperature
    else if(strSelected == _T("ReadBatteryTemperature"))
    {
        if(ReadBatteryTemperature != NULL) 
		{
			int nTemperature = 0;
            if(ReadBatteryTemperature(m_nPortNum,nTemperature))
			{
				CString strFormat;
				strFormat.Format(_T("%d C"),nTemperature);
				AddLog(strFormat);
				AddLog(_T("ReadBatteryTemperature Success"));
			}
			else
				AddLog(_T("ReadBatteryTemperature Fail"));
        }
		else
			AddLog(_T("ReadBatteryTemperature Load function Fail "));
    }
    else if(strSelected == _T("ReadBatteryVoltage"))
    {
        if(ReadBatteryVoltage != NULL) 
		{
			int nVoltage = 0;
            if(ReadBatteryVoltage(m_nPortNum,nVoltage))
			{
				CString strFormat;
				strFormat.Format(_T("%d mV"),nVoltage);
				AddLog(strFormat);
				AddLog(_T("ReadBatteryVoltage Success"));
			}
			else
				AddLog(_T("ReadBatteryVoltage Fail"));
        }
		else
			AddLog(_T("ReadBatteryVoltage Load function Fail "));
    }
    else if(strSelected == _T("WriteDefaultPhoneCode"))
    {
        if(WriteDefaultPhoneCode != NULL) 
		{
            if(WriteDefaultPhoneCode(nPort))
			{
				AddLog(_T("WriteDefaultPhoneCode Success"));
			}
			else
				AddLog(_T("WriteDefaultPhoneCode Fail"));
        }
		else
			AddLog(_T("WriteDefaultPhoneCode Load function Fail "));
    }
    else if(strSelected == _T("TurnOnEM"))
    {
        if(TurnOnOffEM != NULL) 
		{
            if(TurnOnOffEM(nPort,true))
			{
				AddLog(_T("TurnOnEM Success"));
			}
			else
				AddLog(_T("TurnOnEM Fail"));
        }
		else
			AddLog(_T("TurnOnEM Load function Fail "));
    }
    else if(strSelected == _T("TurnOffEM"))
    {
        if(TurnOnOffEM != NULL) 
		{
            if(TurnOnOffEM(nPort,false))
			{
				AddLog(_T("TurnOffEM Success"));
			}
			else
				AddLog(_T("TurnOffEM Fail"));
        }
		else
			AddLog(_T("TurnOffEM Load function Fail "));
    }
    else if(strSelected == _T("WriteFailureByte"))
    {
        if(WriteFailureByte != NULL) 
		{
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			BYTE nFailureByte = ::_ttoi(strInput);
            if(WriteFailureByte(nPort,nFailureByte))
			{
				AddLog(_T("WriteFailureByte Success"));
			}
			else
				AddLog(_T("WriteFailureByte Fail"));
        }
		else
			AddLog(_T("WriteFailureByte Load function Fail "));
    }
    else if(strSelected == _T("WriteNVItem"))
    {
		AddLog(_T("Didn't support WriteNVItem on this test function"));
	}
    else if(strSelected == _T("ReadNVItem"))
    {
        if(ReadNVItem != NULL) 
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() == 4)
			{
				WORD NVItem = atoi(strInput);
				BYTE  item_data[128] = {0};
				WORD  nv_stat;

				CString strCOMPort;
				m_COMPortCtrl.GetWindowText(strCOMPort);	
				char szBASE64Buffer[2048] = {0};
				char szStateBuffer[2048] = {0};
				ReadNVItem_ForSPU((LPTSTR)(LPCTSTR)strCOMPort,(LPTSTR)(LPCTSTR)strInput,szBASE64Buffer,szStateBuffer);

				if(ReadNVItem(nPort,NVItem,item_data,nv_stat))
				{
					AddLog(strInput);
					CString strFormat;
					strFormat.Format(_T("NV State : %d"),nv_stat);
					AddLog(strFormat);
					LogBinary((const char *)item_data,128);					
					AddLog(_T("ReadNVItem Success"));
				}
				else
				{					
					AddLog(strInput);
					CString strFormat;
					strFormat.Format(_T("NV State : %d"),nv_stat);
					AddLog(strFormat);
					AddLog(_T("ReadNVItem Fail"));
				}
			}
			else
			{
				AddLog(_T("Input parameer Fail, Please input like 0453"));
			}
        }
		else
			AddLog(_T("ReadNVItem Load function Fail "));
    }
    else if(strSelected == _T("GetLavernockHWVersion"))
    {
        if(GetLavernockHWVersion != NULL) 
		{
			int nVersion = -1;
            if(GetLavernockHWVersion(nPort,nVersion))
			{
				CString strFormat;
				strFormat.Format(_T("Lavernock HW version is %d"),nVersion);
				AddLog(strFormat);
				AddLog(_T("GetLavernockHWVersion Success"));
			}
			else
				AddLog(_T("GetLavernockHWVersion Fail"));
        }
		else
			AddLog(_T("GetLavernockHWVersion Load function Fail "));
    }
    else if(strSelected == _T("GetLavernockHWVersionV2_ForSPU"))
    {
        if(GetLavernockHWVersionV2_ForSPU != NULL) 
		{			
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			char szVarent[1024] = {0};
			char szEVT[1024] = {0};
            if(GetLavernockHWVersionV2_ForSPU((LPSTR)(LPCTSTR)strCOMPort,szVarent,szEVT))
			{
				CString strFormat;
				strFormat.Format(_T("Lavernock Varient version is %s"),szVarent);
				AddLog(strFormat);
				strFormat.Format(_T("Lavernock EVT version is %s"),szEVT);
				AddLog(strFormat);
				AddLog(_T("GetLavernockHWVersionV2_ForSPU Success"));
			}
			else
				AddLog(_T("GetLavernockHWVersionV2_ForSPU Fail"));
        }
		else
			AddLog(_T("GetLavernockHWVersionV2_ForSPU Load function Fail "));
    }
    else if(strSelected == _T("GetMicroSDInfo_ForSPU"))
    {
        if(GetMicroSDInfo_ForSPU != NULL) 
		{			
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);	

			char sznBlockSize[1024] = {0};
			char sznTotalBlock[1024] = {0};
            if(GetMicroSDInfo_ForSPU((LPSTR)(LPCTSTR)strCOMPort,sznBlockSize,sznTotalBlock))
			{
				CString strFormat;
				strFormat.Format(_T("Block size is %s"),sznBlockSize);
				AddLog(strFormat);
				strFormat.Format(_T("Total block is %s"),sznTotalBlock);
				AddLog(strFormat);
				AddLog(_T("GetMicroSDInfo_ForSPU Success"));
			}
			else
				AddLog(_T("GetMicroSDInfo_ForSPU Fail"));
        }
		else
			AddLog(_T("GetMicroSDInfo_ForSPU Load function Fail "));
    }
    else if(strSelected == _T("GetSWHWVersion"))
    {
        if(GetSWHWVersion != NULL) 
		{
			char szSW[512] = {0};
			char szHW[512] = {0};
            if(GetSWHWVersion(nPort,szSW,szHW))
			{
				AddLog(szSW);
				AddLog(szHW);
				AddLog(_T("GetSWHWVersion Success"));
			}
			else
				AddLog(_T("GetSWHWVersion Fail"));
        }
		else
			AddLog(_T("GetSWHWVersion Load function Fail "));
    }
    else if(strSelected == _T("EnableGAP"))
    {
        if(EnableGAP != NULL) 
		{
            if(EnableGAP(nPort,true))
				AddLog(_T("EnableGAP Success"));
			else
				AddLog(_T("EnableGAP Fail"));
        }
		else
			AddLog(_T("EnableGAP Load function Fail "));
    }
    else if(strSelected == _T("EnterBTTestMode"))
    {
        if(EnterBTTestMode != NULL) 
		{
            if(EnterBTTestMode(nPort,true))
				AddLog(_T("EnterBTTestMode Success"));
			else
				AddLog(_T("EnterBTTestMode Fail"));
        }
		else
			AddLog(_T("EnterBTTestMode Load function Fail "));
    }
    else if(strSelected == _T("LeaveBTTestMode"))
    {
        if(EnterBTTestMode != NULL) 
		{
            if(EnterBTTestMode(nPort,false))
				AddLog(_T("LeaveBTTestMode Success"));
			else
				AddLog(_T("LeaveBTTestMode Fail"));
        }
		else
			AddLog(_T("LeaveBTTestMode Load function Fail "));
    }
    else if(strSelected == _T("StartBT hige Power"))
    {
        if(StartBTPower != NULL) 
		{
            if(StartBTPower(nPort,0xF0))
				AddLog(_T("StartBT hige Power Success"));
			else
				AddLog(_T("StartBT hige Power Fail"));
        }
		else
			AddLog(_T("StartBTPower Load function Fail "));
    }
    else if(strSelected == _T("StartBT middle Power"))
    {
        if(StartBTPower != NULL) 
		{
            if(StartBTPower(nPort,0xF1))
				AddLog(_T("StartBT middle Power Success"));
			else
				AddLog(_T("StartBT middle Power Fail"));
        }
		else
			AddLog(_T("StartBTPower Load function Fail "));
    }
    else if(strSelected == _T("StartBT low Power"))
    {
        if(StartBTPower != NULL) 
		{
            if(StartBTPower(nPort,0xF2))
				AddLog(_T("StartBT low Power Success"));
			else
				AddLog(_T("StartBT low Power Fail"));
        }
		else
			AddLog(_T("StartBTPower Load function Fail "));
    }
    else if(strSelected == _T("DisableGAP"))
    {
        if(EnableGAP != NULL) 
		{
            if(EnableGAP(nPort,false))
				AddLog(_T("DisableGAP Success"));
			else
				AddLog(_T("DisableGAP Fail"));
        }
		else
			AddLog(_T("DisableGAP Load function Fail "));
    }
    else if(strSelected == _T("FMRegistration"))
    {
        if(FMRegistration != NULL) 
		{
            if(FMRegistration(nPort))
				AddLog(_T("FMRegistration Success"));
			else
				AddLog(_T("FMRegistration Fail"));
        }
		else
			AddLog(_T("FMRegistration Load function Fail "));
    }
    else if(strSelected == _T("FMConnect"))
    {
        if(FMConnect != NULL) 
		{
            if(FMConnect(nPort,true))
				AddLog(_T("FMConnect Success"));
			else
				AddLog(_T("FMConnect Fail"));
        }
		else
			AddLog(_T("FMConnect Load function Fail "));
    }
    else if(strSelected == _T("FMDisonnect"))
    {
        if(FMConnect != NULL) 
		{
            if(FMConnect(nPort,false))
				AddLog(_T("FMDisonnect Success"));
			else
				AddLog(_T("FMDisonnect Fail"));
        }
		else
			AddLog(_T("FMDisonnect Load function Fail "));
    }
    else if(strSelected == _T("FMInitialize"))
    {
        if(FMInitialize != NULL) 
		{
            if(FMInitialize(nPort))
				AddLog(_T("FMInitialize Success"));
			else
				AddLog(_T("FMInitialize Fail"));
        }
		else
			AddLog(_T("FMInitialize Load function Fail "));
    }
    else if(strSelected == _T("FMSetStation"))
    {
		CString strInput;
		m_cmdInput.GetWindowText(strInput);

		if(strInput.IsEmpty() == false)
		{
			if(FMSetStation != NULL) 
			{
				if(FMSetStation(nPort,_ttoi(strInput)))
					AddLog(_T("FMSetStation Success"));
				else
					AddLog(_T("FMSetStation Fail"));
			}
			else
				AddLog(_T("FMSetStation Load function Fail "));
		}
		else
			AddLog(_T("Please input the frequence ( 8750 ~ 10800) on input"));
    }
    else if(strSelected == _T("FMMute"))
    {
        if(FMMute != NULL) 
		{
            if(FMMute(nPort,true))
				AddLog(_T("FMMute Success"));
			else
				AddLog(_T("FMMute Fail"));
        }
		else
			AddLog(_T("FMMute Load function Fail "));
    }
    else if(strSelected == _T("FMNotMute"))
    {
        if(FMMute != NULL) 
		{
            if(FMMute(nPort,false))
				AddLog(_T("FMNotMute Success"));
			else
				AddLog(_T("FMNotMute Fail"));
        }
		else
			AddLog(_T("FMNotMute Load function Fail "));
    }
    else if(strSelected == _T("FMReceiveRSSI"))
    {
        if(FMReceiveRSSI != NULL) 
		{
			WORD nRSSI = 0;
            if(FMReceiveRSSI(nPort,nRSSI))
			{
				CString strFormat;
				strFormat.Format(_T("RSSI is %d"),nRSSI);
				AddLog(strFormat);
				AddLog(_T("FMReceiveRSSI Success"));
			}
			else
				AddLog(_T("FMReceiveRSSI Fail"));
        }
		else
			AddLog(_T("FMReceiveRSSI Load function Fail "));
    }
    else if(strSelected == _T("FMSpeaker"))
    {
        if(FMSpeaker != NULL) 
		{
            if(FMSpeaker(nPort))
				AddLog(_T("FMSpeaker Success"));
			else
				AddLog(_T("FMSpeaker Fail"));
        }
		else
			AddLog(_T("FMSpeaker Load function Fail "));
    }
    else if(strSelected == _T("FMMonoHeadset"))
    {
        if(FMMonoHeadset != NULL) 
		{
            if(FMMonoHeadset(nPort))
				AddLog(_T("FMMonoHeadset Success"));
			else
				AddLog(_T("FMMonoHeadset Fail"));
        }
		else
			AddLog(_T("FMMonoHeadset Load function Fail "));
    }
    else if(strSelected == _T("FMStereoHeadset"))
    {
        if(FMStereoHeadset != NULL) 
		{
            if(FMStereoHeadset(nPort))
				AddLog(_T("FMStereoHeadset Success"));
			else
				AddLog(_T("FMStereoHeadset Fail"));
        }
		else
			AddLog(_T("FMStereoHeadset Load function Fail "));
    }
    else if(strSelected == _T("LockSPC"))
    {
        if(LockSPC != NULL) 
		{
            if(LockSPC(nPort))
				AddLog(_T("LockSPC Success"));
			else
				AddLog(_T("LockSPC Fail"));
        }
		else
			AddLog(_T("LockSPC Load function Fail "));
    }
    else if(strSelected == _T("IsSPCLocked"))
    {
        if(IsSPCLocked != NULL) 
		{
			long nStatus = 0;
            if(IsSPCLocked(nPort,nStatus))
			{
				CString strFormat;
				strFormat.Format(_T("Status : %d"),nStatus);
				AddLog(strFormat);
				AddLog(_T("IsSPCLocked Success"));
			}
			else
				AddLog(_T("IsSPCLocked Fail"));
        }
		else
			AddLog(_T("IsSPCLocked Load dll fail "));
    }
    else if(strSelected == _T("UnlockSPC"))
    {
        if(UnlockSPC != NULL) 
		{
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() == 6)
			{
				bool bUnlockOK = false;
				if(UnlockSPC(nPort,(LPCTSTR)strInput,bUnlockOK))
				{
					AddLog(strInput);
					CString strFormat;
					strFormat.Format(_T("Unlock : %d"),bUnlockOK);
					AddLog(strFormat);
					AddLog(_T("UnlockSPC Success"));
				}
				else
					AddLog(_T("UnlockSPC Fail"));
			}
			else
			{
				AddLog(_T("Input length Fail"));
			}
        }
		else
			AddLog(_T("UnlockSPC Load function fail "));
    }
    else if(strSelected == _T("ProgramSPC"))
    {
        if(IsSPCLocked != NULL) 
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() == 6)
			{
				if(ProgramSPC(nPort,strInput))
				{
					AddLog(strInput);
					AddLog(_T("ProgramSPC Success"));
				}
				else
					AddLog(_T("ProgramSPC Fail"));
			}
			else
			{
				AddLog(_T("Input length Fail"));
			}
        }
		else
			AddLog(_T("ProgramSPC Load function fail "));
    }
    else if(strSelected == _T("WriteSimLockFile"))
    {
        if(WriteSimLockFile != NULL) 
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() > 0)
			{
				if(WriteSimLockFile(m_nPortNum,(LPCSTR)(LPCTSTR)strInput))
				{
					AddLog(strInput);
					AddLog(_T("WriteSimLockFile Success"));
				}
				else
					AddLog(_T("WriteSimLockFile Fail"));
			}
			else
			{
				AddLog(_T("Input length > 0 Fail"));
			}
        }
		else
			AddLog(_T("WriteSimLockFile Load function fail "));
    }
	else if(strSelected == _T("BackupNVPartition"))
	{
		if(BackupNVPartition != NULL) 
		{	
			if(BackupNVPartition(m_nPortNum))
			{
				AddLog(_T("BackupNVPartition Success"));
			}
			else
				AddLog(_T("BackupNVPartition Fail"));
		}
		else
			AddLog(_T("BackupNVPartition Load function fail "));
    }
	else if(strSelected == _T("RestoreNVPartition"))
	{
		if(RestoreNVPartition != NULL) 
		{	
			if(RestoreNVPartition(m_nPortNum))
			{
				AddLog(_T("RestoreNVPartition Success"));
			}
			else
				AddLog(_T("RestoreNVPartition Fail"));
		}
		else
			AddLog(_T("RestoreNVPartition Load function fail "));
    }	
	else if(strSelected == _T("CheckNVPartition"))
	{
		if(CheckNVPartition != NULL) 
		{	
			int nBackupTimes  =0;
			 CTime LastBackupTime;
			if(CheckNVPartition(m_nPortNum,nBackupTimes,LastBackupTime))
			{
				CString strTime = LastBackupTime.Format( "%H:%M:%S %A, %B %d, %Y" );
				AddLog(strTime);
				AddLog(_T("CheckNVPartition Success"));
			}
			else
				AddLog(_T("CheckNVPartition Fail"));
		}
		else
			AddLog(_T("CheckNVPartition Load function fail "));
    }		
	else if(strSelected == _T("BackupNV2BRT"))
	{
        if(BackupNV2BRT != NULL) 
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() > 0)
			{
				CString strBRT = "output.brt";
				if(BackupNV2BRT(m_nPortNum,(LPCSTR)(LPCTSTR)strBRT,(LPCSTR)(LPCTSTR)strInput))
				{
					AddLog(strInput);
					AddLog(strBRT);
					AddLog(_T("BackupNV2BRT Success"));
				}
				else
					AddLog(_T("BackupNV2BRT Fail"));
			}
			else
			{
				AddLog(_T("Input length > 0 Fail"));
			}
        }
		else
			AddLog(_T("BackupNV2BRT Load function fail "));
	}
	else if(strSelected == _T("RestoreBRT2NV"))
	{
        if(RestoreBRT2NV != NULL) 
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() > 0)
			{
				if(RestoreBRT2NV(m_nPortNum,(LPCSTR)(LPCTSTR)strInput))
				{
					AddLog(strInput);
					AddLog(_T("RestoreBRT2NV Success"));
				}
				else
					AddLog(_T("RestoreBRT2NV Fail"));
			}
			else
			{
				AddLog(_T("Input length > 0 Fail"));
			}
        }
		else
			AddLog(_T("RestoreBRT2NV Load function fail "));
	}
    else if(strSelected == _T("Get DL Boot loader revocation Command")) 
    {
		if(m_bRunSelfTest == false)
		{
			if(m_nPortNum > 0 && m_pDownload == NULL)
			{
				//m_pDownload = new CDownload(nPort);
			}
			if(m_pDownload)
			{
				/*
				if(m_pDownload->GetDLBootLoaderRevocation(nStatus,nVersion))
				{
					CString strFormat;
					strFormat.Format(_T("Status : %02d,Version : %02d"),nStatus,nVersion);
					AddLog(strFormat);
					AddLog(_T("GetDLBootLoaderRevocation Success"));
				}
				else
					AddLog(_T("GetDLBootLoaderRevocation fail"));
				*/
			}
		}
		else
			AddLog(_T("GetDLBootLoaderRevocation will not be execuated in the sample."));
    }
    else if(strSelected == _T("Get DL Phone code revocation Command")) 
    {
		if(m_bRunSelfTest == false)
		{
			if(nPort > 0 && m_pDownload == NULL)
			{
				//m_pDownload = new CDownload(nPort);
			}
			if(m_pDownload)
			{
				/*
				if(m_pDownload->GetDLPhoneCodeRevocation(nStatus,nVersion))
				{
					CString strFormat;
					strFormat.Format(_T("Status : %02d,Version : %02d"),nStatus,nVersion);
					AddLog(strFormat);
					AddLog(_T("GetDLPhoneCodeRevocation Success"));
				}
				else
					AddLog(_T("GetDLPhoneCodeRevocation fail"));
				*/
			}
		}
		else
			AddLog(_T("GetDLPhoneCodeRevocation will not be execuated in the sample."));
    }
    else if(strSelected == _T("Get IMEI"))
    {
        if(ReadIMEI != NULL) 
		{
			char szIMEI[128] = {0};
            if(ReadIMEI(nPort,szIMEI))
			{
				AddLog(szIMEI);
				AddLog(_T("ReadIMEI Success"));
			}
			else
				AddLog(_T("ReadIMEI Fail"));
        }
		else
			AddLog(_T("ReadIMEI Load dll fail "));
    }
    else if(strSelected == _T("Write IMEI"))
    {
        if(WriteIMEI != NULL) 
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if(strInput.GetLength() >= 16)
			{
				if(WriteIMEI(nPort,strInput))
				{
					AddLog(strInput);
					AddLog(_T("WriteIMEI Success"));
				}
				else
					AddLog(_T("WriteIMEI Fail"));
			}
			else
			{
				AddLog(_T("Input length >= 16 Fail"));
			}
        }
		else
			AddLog(_T("WriteIMEI Load dll fail "));
    }
    else if(strSelected == _T("Write Headset BT Address"))
    {
		AddLog(_T("Make sure for AthensBT only"));

        if(WriteHeadsetBTAddress != NULL) 
		{
			CString strBTAddr;
			m_cmdInput.GetWindowText(strBTAddr);
			if(strBTAddr.GetLength() != 12)
			{
				AddLog(_T("Length of parameter must  be 12"));
			}
            else if(WriteHeadsetBTAddress(strBTAddr.GetString(), nPort))
			{				
				while(CheckBTHeadsetState(strBTAddr.GetString(), nPort) == false)
				{
					Sleep(1000);
					AddLog(_T("WriteHeadsetBTAddress Fail on check"));
				};
				AddLog(_T("WriteHeadsetBTAddress Success"));
			}
			else
				AddLog(_T("WriteHeadsetBTAddress Fail"));
        }
		else
			AddLog(_T("WriteHeadsetBTAddress Load dll fail "));
    }
	else if(_T("Check BT headset") == strSelected) 
	{
		if(CheckBTHeadset)
		{
			CString strBTAddr;
			m_cmdInput.GetWindowText(strBTAddr);
			if(strBTAddr.GetLength() != 12)
			{
				AddLog(_T("Length of parameter must  be 12"));
			}
			else
			{
				if(CheckBTHeadset(strBTAddr.GetString(),nPort))
					AddLog(_T("CheckBTHeadset Success"));
				else
					AddLog(_T("CheckBTHeadset Fail"));  
			}
		}
		else
			AddLog(_T("Load function fail "));
    } 
    else if(strSelected == _T("ClearActivationData"))
    {
        if(ClearActivationData != NULL) 
		{
            if(ClearActivationData(nPort))
				AddLog(_T("ClearActivationData Success"));
			else
				AddLog(_T("ClearActivationData Fail"));
        }
		else
			AddLog(_T("ClearActivationData Load dll fail "));
    }
	else if(_T("Check SIM Card Existence") == strSelected)
	{		
		if(CheckSIMCardExistence)
		{
			int nType = 0;
            if(CheckSIMCardExistence(nPort, nType))
				AddLog(_T("Check SIM Card Existence Success"));
			else
				AddLog(_T("Check SIM Card Existence Fail"));            
		}
		else
			AddLog(_T("Load dll fail "));
	}
    else if(strSelected == _T("Write QFuse"))
    {		
		AddLog(_T("Make sure use it on FTM mode"));
		if(WriteQFuse)
		{
			/*
			unsigned long reg1;
			unsigned long reg2;
            if(WriteQFuse(nPort, reg1,reg2))
			{
				CString strFormat;
				strFormat.Format(_T("%04X, %04X"));
				AddLog(strFormat);
				AddLog(_T("WriteQFuse Success"));
			}
			else
				AddLog(_T("WriteQFuse Fail"));
			*/
			AddLog(_T("WriteQFuse will not be execuated in the sample."));
		}
		else
			AddLog(_T("Load dll fail "));
    }
	else if(strSelected == _T("Read QFuse"))
	{		
		AddLog(_T("Make sure use it on FTM mode"));
		if(ReadQFuse)
		{
			unsigned long reg1;
			unsigned long reg2;
			bool bRes = false;
            if(ReadQFuse(nPort, reg1,reg2 ,bRes))
			{
				CString strFormat;
				strFormat.Format(_T("%04X, %04X,bSuccess = %d"),reg1,reg2,bRes);
				AddLog(strFormat);
				AddLog(_T("ReadQFuse Success"));
			}
			else
				AddLog(_T("ReadQFuse Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
	}
    else if(strSelected == _T("MasterClear"))
    {
		if(MasterClear)
		{			
			if(m_bRunSelfTest == false)
			{
				CString strInput;
				m_cmdInput.GetWindowText(strInput);
				int nDelay = ::_ttoi(strInput);

				if(nDelay > 0)
				{
					if(MasterClear(nPort))
						AddLog(_T("MasterClear Success"));
					else
						AddLog(_T("MasterClear Fail"));        

					Sleep(nDelay);//For Athens case, 65000
				}
				else
				{
						AddLog(_T("Please input a dealy time"));
				}
			}
			else
				AddLog(_T("MasterClear will not be execuated in the sample."));
		}
		else
			AddLog(_T("Load function fail "));
    }
    else if(strSelected == _T("BT Headset charging off"))
    {
		if(BTHeadsetChargingPower)
		{
            if(BTHeadsetChargingPower(nPort,false))
				AddLog(_T("BTHeadsetChargingPower off Success"));
			else
				AddLog(_T("BTHeadsetChargingPower off Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    }
    else if(strSelected == _T("BT Headset charging on"))
    {
		if(BTHeadsetChargingPower)
		{
            if(BTHeadsetChargingPower(nPort,true))
				AddLog(_T("BTHeadsetChargingPower on Success"));
			else
				AddLog(_T("BTHeadsetChargingPower on Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    }
	else if(strSelected == _T("SetOnline")   ) 
	{
		if(SetOnline)
		{
            if(SetOnline(nPort))
				AddLog(_T("SetOnline Success"));
			else
				AddLog(_T("SetOnline Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    } 
	else if(strSelected == _T("SetOffline")  ) 
	{
		if(SetOffline)
		{
            if(SetOffline(nPort))
				AddLog(_T("SetOffline Success"));
			else
				AddLog(_T("SetOffline Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    } 
	else if(strSelected == _T("SetFTM")      ) 
	{
		if(ChangeMode)
		{
            if(ChangeMode(nPort,3/*MODE_FTM_F*/))
				AddLog(_T("ChangeMode FTM Success"));
			else
				AddLog(_T("ChangeMode FTM Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    } 
	else if(strSelected == _T("ResetHandset")) 
	{
		if(ResetHandset)
		{			
			if(m_bRunSelfTest == false)
			{
				if(ResetHandset(nPort))
					AddLog(_T("ResetHandset Success"));
				else
					AddLog(_T("ResetHandset Fail"));
			}
			else
				AddLog(_T("ResetHandset will not be execuated in the sample."));
		}
		else
			AddLog(_T("Load function fail "));
    } 
	else if(strSelected == _T("Set FTM NV453")      ) 
	{
		if(ChangeFTM)
		{
            if(ChangeFTM(nPort))
				AddLog(_T("ChangeFTM Success"));
			else
				AddLog(_T("ChangeFTM Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    } 
	else if(strSelected == _T("CheckFTM"))
	{
		if(CheckFTM)
		{
			int nFTMValue  = -2;
            if(CheckFTM(nPort,nFTMValue))
			{
				CString strFormat;				
				strFormat.Format(_T("%d"),nFTMValue);
				AddLog(strFormat);

				AddLog(_T("CheckFTM FTM Success"));
			}
			else
			{
				CString strFormat;				
				strFormat.Format(_T("%d"),nFTMValue);
				AddLog(strFormat);
				AddLog(_T("CheckFTM FTM Fail"));     
			}
		}
		else
			AddLog(_T("Load function fail "));
	}
	else if(strSelected == _T("Change Online Mode")      ) 
	{
		if(ChangeMode)
		{
            if(ChangeMode(nPort,4/*MODE_ONLINE_F*/))
				AddLog(_T("ChangeMode Online Success"));
			else
				AddLog(_T("ChangeMode Online Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    } 
	else if(_T("BT Headset Voltage Reading") == strSelected) 
	{
		if(BTHeadsetVoltageReading)
		{
			long nVoltage = 0;
            if(BTHeadsetVoltageReading(nPort, nVoltage, 3000))
			{
				CString strFormat;
				strFormat.Format(_T("%d"),nVoltage);
				AddLog(strFormat);
				AddLog(_T("BTHeadsetVoltageReading Success"));
			}
			else
				AddLog(_T("BTHeadsetVoltageReading Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    } 
	else  if(_T("Set Online NV453") == strSelected) 
	{
		if(ChangeOnline)
		{
            if(ChangeOnline(nPort))
				AddLog(_T("ChangeOnline Success"));
			else
				AddLog(_T("ChangeOnline Fail"));            
		}
		else
			AddLog(_T("Load function fail "));
    }
	else if(_T("GetMotoXprs_Athens") == strSelected)
	{
#ifdef _DEBUG
		HMODULE QisdaODMModuleDll = ::LoadLibrary(_T("ODMCoachModule_StaticMD.dll"));
#else
		HMODULE QisdaODMModuleDll = ::LoadLibrary(_T("ODMCoachModule_StaticM.dll"));
#endif   
		if(QisdaODMModuleDll)
		{
			pFnGetMotoXprs_Athens GetMotoXprs_Athens = (pFnGetMotoXprs_Athens)GetProcAddress(QisdaODMModuleDll, "GetMotoXprs_Athens");
			if(GetMotoXprs_Athens)
			{
				TCHAR szFile[MAX_PATH] = {0};
				if(GetMotoXprs_Athens(szFile,MAX_PATH,nPort,5000))
				{
					AddLog(szFile);
				}
				else
					AddLog(_T("Execuate function ""GetMotoXprs_Athens"" fail"));
			}
			else
				AddLog(_T("Load function ""GetMotoXprs_Athens"" fail"));
		}
		else
			AddLog(_T("Load function fail "));
	}
	else if(_T("GetMotoXprs_Lavernock") == strSelected)
	{
#ifdef _DEBUG
		HMODULE QisdaODMModuleDll = ::LoadLibrary(_T("ODMCoachModule_StaticMD.dll"));
#else
		HMODULE QisdaODMModuleDll = ::LoadLibrary(_T("ODMCoachModule_StaticM.dll"));
#endif   
		if(QisdaODMModuleDll)
		{
			pFnGetMotoXprs_Lavernock GetMotoXprs_Lavernock = (pFnGetMotoXprs_Athens)GetProcAddress(QisdaODMModuleDll, "GetMotoXprs_Lavernock");
			if(GetMotoXprs_Lavernock)
			{
				TCHAR szFile[MAX_PATH] = {0};
				if(GetMotoXprs_Lavernock(szFile,MAX_PATH,nPort,5000))
				{
					AddLog(szFile);
				}
				else
					AddLog(_T("Execuate function ""GetMotoXprs_Lavernock"" fail"));
			}
			else
				AddLog(_T("Load function ""GetMotoXprs_Lavernock"" fail"));
		}
		else
			AddLog(_T("Load function fail "));
	}
	else if(_T("GetMotoXprs_Melos") == strSelected)
	{
#ifdef _DEBUG
		HMODULE QisdaODMModuleDll = ::LoadLibrary(_T("ODMCoachModule_StaticMD.dll"));
#else
		HMODULE QisdaODMModuleDll = ::LoadLibrary(_T("ODMCoachModule_StaticM.dll"));
#endif   
		if(QisdaODMModuleDll)
		{
			pFnGetMotoXprs_Melos GetMotoXprs_Melos = (pFnGetMotoXprs_Melos)GetProcAddress(QisdaODMModuleDll, "GetMotoXprs_Melos");
			if(GetMotoXprs_Melos)
			{
				TCHAR szFile[MAX_PATH] = {0};
				if(GetMotoXprs_Melos(szFile,MAX_PATH,nPort,5000))
				{
					AddLog(szFile);
				}
				else
					AddLog(_T("Execuate function ""GetMotoXprs_Melos"" fail"));
			}
			else
				AddLog(_T("Load function ""GetMotoXprs_Melos"" fail"));
		}
		else
			AddLog(_T("Load function fail "));
	}
    else if(strSelected == _T("Standby Current Test"))
    {
        if(StandbyCurrentTest != NULL) 
		{
			if(m_bRunSelfTest == false)
			{
				if(StandbyCurrentTest(nPort))
					AddLog(_T("Standby Current Test Success"));
				else
					AddLog(_T("Standby Current Test Fail"));
			}
			else
				AddLog(_T("StandbyCurrentTest will not be execuated in the sample."));
        }
		else
			AddLog(_T("Standby Current Test Load dll fail "));
    }
	else if(strSelected == _T("EnableQualcommEvent"))
	{
		if(NewResetHandset != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				if(EnableQualcommDebugEvent(nPort,true))
					AddLog(_T("EnableQualcommEvent Success"));
				else
					AddLog(_T("EnableQualcommEvent Fail"));
			}
			else
				AddLog(_T("EnableQualcommEvent will not be execuated in the sample."));
        }
		else
			AddLog(_T("EnableQualcommEvent Load function fail "));
	}
	else if(strSelected == _T("DisableQualcommEvent"))
	{
		if(EnableQualcommDebugEvent != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				if(EnableQualcommDebugEvent(nPort,false))
					AddLog(_T("DisableQualcommEvent Success"));
				else
					AddLog(_T("DisableQualcommEvent Fail"));
			}
			else
				AddLog(_T("DisableQualcommEvent will not be execuated in the sample."));
        }
		else
			AddLog(_T("DisableQualcommEvent Load function fail "));
	}
	else if(strSelected == _T("WriteSIMLockFileNameToNV"))
	{
		if(WriteSIMLockFileNameToNV != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				CString strInput;
				m_cmdInput.GetWindowText(strInput);

				if(strInput.GetLength() > 0)
				{
					std::string strFileName = (LPCTSTR)strInput;
					AddLog(strFileName.c_str());
					if(WriteSIMLockFileNameToNV(nPort,strFileName))
						AddLog(_T("WriteSIMLockFileNameToNV Success"));
					else
						AddLog(_T("WriteSIMLockFileNameToNV Fail"));
				}
				else
				{
					AddLog(_T("Input parameter is empty"));
				}
			}
			else
				AddLog(_T("WriteSIMLockFileNameToNV will not be execuated in the sample."));
        }
		else
			AddLog(_T("WriteSIMLockFileNameToNV Load function fail "));
	}
	else if(strSelected == _T("ReadSIMLockFileNameFromNV"))
	{
		if(ReadSIMLockFileNameFromNV != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				std::string strFileName;
				if(ReadSIMLockFileNameFromNV(nPort,strFileName))
				{
					AddLog(strFileName.c_str());
					AddLog(_T("ReadSIMLockFileNameFromNV Success"));
				}
				else
					AddLog(_T("ReadSIMLockFileNameFromNV Fail"));
			}
			else
				AddLog(_T("ReadSIMLockFileNameFromNV will not be execuated in the sample."));
        }
		else
			AddLog(_T("ReadSIMLockFileNameFromNV Load function fail "));
	}
	else if (strSelected == _T("RAWRequest"))
	{
		if(DLRAWRequest != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				//Analysis the input command, translate to integer.
				CString strInput;
				m_cmdInput.GetWindowText(strInput);
				vector<CString> vecInput;
				QSYNC_LIB::Tokenize<CString>(_T(" "), strInput, vecInput, true);
				char* cmd = new char[vecInput.size()];
				for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
				{
					if(vecInput.at(nIndex).Left(2).MakeLower() == _T("0x"))
						vecInput.at(nIndex) = vecInput.at(nIndex).Mid(2);
					cmd[nIndex] = static_cast<char>(QSYNC_LIB::HEX2Ineger((vecInput.at(nIndex).GetString())));
				}
				//Analysis the input command end.

				if (strInput.IsEmpty())
					AddLog(_T("Command Input is empty!"));
				else if(DLRAWRequest(nPort, cmd, static_cast<int>(vecInput.size()), true))
					AddLog(_T("RAWRequest Success"));
				else
					AddLog(_T("RAWRequest Fail"));

				delete [] cmd;
			}
			else
				AddLog(_T("RAWRequest will not be execuated in the sample."));
		}
		else
			AddLog(_T("RAWRequest Load function fail "));
	}
	else if (strSelected == _T("RAWRequest_NoCRC"))
	{
		if(DLRAWRequest != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				//Analysis the input command, translate to integer.
				CString strInput;
				m_cmdInput.GetWindowText(strInput);
				vector<CString> vecInput;
				QSYNC_LIB::Tokenize<CString>(_T(" "), strInput, vecInput, true);
				char* cmd = new char[vecInput.size()];
				{
					for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
					{
						if(vecInput.at(nIndex).Left(2).MakeLower() == _T("0x"))
							vecInput.at(nIndex) = vecInput.at(nIndex).Mid(2);
						cmd[nIndex] = static_cast<char>(QSYNC_LIB::HEX2Ineger((vecInput.at(nIndex).GetString())));
					}
					//Analysis the input command end.

					if (strInput.IsEmpty())
						AddLog(_T("Command Input is empty!"));
					else if(DLRAWRequest(nPort, cmd,static_cast<int>(vecInput.size()), false))
						AddLog(_T("RAWRequest_NoCRC Success"));
					else
						AddLog(_T("RAWRequest_NoCRC Fail"));
				}
				delete [] cmd;
			}
			else
				AddLog(_T("RAWRequest_NoCRC will not be execuated in the sample."));
		}
		else
			AddLog(_T("RAWRequest_NoCRC Load function fail "));
	}
	else if (strSelected == _T("Camera initial"))
	{		
		if(DiagRAWRequest != NULL)
		{
			char szSetSensor[] = {0x4B, 0x0B, 0x0F, 0x00, 0x0A, 0x00, 0x0B, 0x00, 0x0C, 0x00, 0x00};
			DiagRAWRequest(nPort,szSetSensor,11,true,true);

			char szStopCamera[] = {0x4B, 0x0B, 0x0F, 0x00, 0x01, 0x00, 0x0A, 0x00, 0x0C, 0x00};
			DiagRAWRequest(nPort,szStopCamera,10,true,true);

			char szStartCamera[] = {0x4B, 0x0B, 0x0F, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x0C, 0x00};
			DiagRAWRequest(nPort,szStartCamera,10,true,true);
		}
			else
				AddLog(_T("Diag_RAWRequest will not be execuated in the sample."));
	}
	else if (strSelected == _T("Diag_RAWRequest"))
	{
		if(DiagRAWRequest != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				//Analysis the input command, translate to integer.
				CString strInput;
				m_cmdInput.GetWindowText(strInput);
				vector<CString> vecInput;
				QSYNC_LIB::Tokenize<CString>(_T(" "), strInput, vecInput, true);
				char* cmd = new char[vecInput.size()];
				{
					for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
					{
						if(vecInput.at(nIndex).Left(2).MakeLower() == _T("0x"))
							vecInput.at(nIndex) = vecInput.at(nIndex).Mid(2);
						cmd[nIndex] = static_cast<char>(QSYNC_LIB::HEX2Ineger((vecInput.at(nIndex).GetString())));
					}
					//Analysis the input command end.

					if (strInput.IsEmpty())
						AddLog(_T("Command Input is empty!"));
					else if(DiagRAWRequest(nPort, cmd, static_cast<int>(vecInput.size()), true,false))
						AddLog(_T("Diag_RAWRequest Success"));
					else
						AddLog(_T("Diag_RAWRequest Fail"));
				}
				delete [] cmd;
			}
			else
				AddLog(_T("Diag_RAWRequest will not be execuated in the sample."));
		}
		else
			AddLog(_T("Diag_RAWRequest Load function fail "));
	}
	else if (strSelected == _T("Diag_RAWRequest_NoCRC"))
	{
		if(DiagRAWRequest != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				//Analysis the input command, translate to integer.
				CString strInput;
				m_cmdInput.GetWindowText(strInput);
				vector<CString> vecInput;
				QSYNC_LIB::Tokenize<CString>(_T(" "), strInput, vecInput, true);
				char* cmd = new char[vecInput.size()];
				{
					for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
					{
						if(vecInput.at(nIndex).Left(2).MakeLower() == _T("0x"))
							vecInput.at(nIndex) = vecInput.at(nIndex).Mid(2);
						cmd[nIndex] = static_cast<char>(QSYNC_LIB::HEX2Ineger((vecInput.at(nIndex).GetString())));
					}
					//Analysis the input command end.

					if (strInput.IsEmpty())
						AddLog(_T("Command Input is empty!"));
					else if(DiagRAWRequest(nPort, cmd, static_cast<int>(vecInput.size()), false,false))
						AddLog(_T("Diag_RAWRequest_NoCRC Success"));
					else
						AddLog(_T("Diag_RAWRequest_NoCRC Fail"));
				}
				delete [] cmd;
			}
			else
				AddLog(_T("Diag_RAWRequest_NoCRC will not be execuated in the sample."));
		}
		else
			AddLog(_T("Diag_RAWRequest_NoCRC Load function fail "));
	}
	else if(strSelected == _T("WriteDummyFactoryArea"))
	{
		if(WriteNVItem != NULL)
		{
			if(m_bRunSelfTest == false)
			{
				BYTE FactoryArea[128] = {0};
				//Edit FactoryArea
				FactoryArea[51] = 1;
				FactoryArea[3] |= 0x02;
				FactoryArea[3] |= 0x04;
				FactoryArea[3] |= 0x08;
				FactoryArea[3] |= 0x10;
				//Edit FactoryArea end

				WORD nv_stat;
				if(WriteNVItem(nPort, 2497, FactoryArea, 128, nv_stat))
				{
					AddLog(_T("WriteDummyFactoryArea Success"));
				}
				else
					AddLog(_T("WriteDummyFactoryArea Fail"));
			}
			else
				AddLog(_T("WriteDummyFactoryArea will not be execuated in the sample."));
		}
		else
			AddLog(_T("WriteDummyFactoryArea Load function fail "));
	}
	else if(strSelected == _T("GPIB_Initialization"))
	{		
		CString		cstrOutputMsg;
		BOOL			bResult;		

		bResult = GPIBInitialization();

		cstrOutputMsg.Format(_T("exe result:%d"),bResult);

		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_SetControlIndex"))
	{		
#define MAX_STRING_LENGTH 1024

		CString		cstrInput;
		CString		cstrOutputMsg;
		BOOL			bResult;
		char			chInputParam[MAX_STRING_LENGTH];

		m_cmdInput.GetWindowText(cstrInput);
		strcpy_s(chInputParam,MAX_STRING_LENGTH,LPCSTR(cstrInput));

		bResult = GPIBSetControlIndex(chInputParam);

		cstrOutputMsg.Format(_T("exe result:%d"),bResult);

		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_GetDeviceCount"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;		
		BOOL			bResult;
		char			chInputParam[MAX_STRING_LENGTH];
		int				iDeviceCount;				

		bResult = GPIBGetDeviceCount(chInputParam);
		iDeviceCount = atoi(chInputParam);

		cstrOutputMsg.Format(_T("device count:%d"),iDeviceCount);
		AddLog(cstrOutputMsg);
		cstrOutputMsg.Format(_T("exe result:%d"),bResult);
		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_QueryDeviceDescription"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;
		CString		cstrInput;
		BOOL			bResult;
		char			chInputParam0[MAX_STRING_LENGTH];
		char			chInputParam1[MAX_STRING_LENGTH];

		m_cmdInput.GetWindowText(cstrInput);
		strcpy_s(chInputParam0,MAX_STRING_LENGTH,LPCSTR(cstrInput));

		bResult = GPIBQueryDeviceDescription(chInputParam0,chInputParam1);		

		cstrOutputMsg.Format(_T("device description:%s"),chInputParam1);
		AddLog(cstrOutputMsg);
		cstrOutputMsg.Format(_T("exe result:%d"),bResult);
		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_IsPowerSupply"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;
		CString		cstrInput;
		BOOL			bResult;
		char			chInputParam0[MAX_STRING_LENGTH];
		char			chInputParam1[MAX_STRING_LENGTH];

		m_cmdInput.GetWindowText(cstrInput);
		strcpy_s(chInputParam0,MAX_STRING_LENGTH,LPCSTR(cstrInput));

		bResult = GPIBIsPowerSupply(chInputParam0,chInputParam1);		

		cstrOutputMsg.Format(_T("is power supply:%s"),chInputParam1);
		AddLog(cstrOutputMsg);
		cstrOutputMsg.Format(_T("exe result:%d"),bResult);
		AddLog(cstrOutputMsg);	
	}
	else if(strSelected == _T("GPIB_SetCurrentVoltage"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;
		CString		cstrInput;
		BOOL			bResult;

		CString strInput;

		m_cmdInput.GetWindowText(strInput);
		vector<CString> vecInput;
		QSYNC_LIB::Tokenize<CString>(_T("^"), strInput, vecInput, true);
		char (*cmd)[MAX_STRING_LENGTH] = new char[vecInput.size()][MAX_STRING_LENGTH];		

		for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
		{
			strcpy_s(cmd[nIndex],MAX_STRING_LENGTH,LPCSTR(vecInput[nIndex]));
		}


		bResult = GPIBSetCurrentVoltage(cmd[0],cmd[1],cmd[2],cmd[3]);		

		cstrOutputMsg.Format(_T("exe result:%d"),bResult);
		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_SendCommmand"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;		
		CString		cstrInput;
		BOOL			bResult;


		m_cmdInput.GetWindowText(cstrInput);
		vector<CString> vecInput;
		QSYNC_LIB::Tokenize<CString>(_T("^"), cstrInput, vecInput, true);
		char (*cmd)[MAX_STRING_LENGTH] = new char[vecInput.size()][MAX_STRING_LENGTH];		

		for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
		{
			strcpy_s(cmd[nIndex],MAX_STRING_LENGTH,LPCSTR(vecInput[nIndex]));
		}

		bResult = GPIBSendCommmand(cmd[0],cmd[1]);		

		cstrOutputMsg.Format(_T("exe result:%d"),bResult);

		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_ReadCommandResult"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;	
		CString		cstrInput;
		BOOL			bResult;
		char			chInputParam0[MAX_STRING_LENGTH] = {0};
		char			chInputParam1[MAX_STRING_LENGTH] = {0};

		m_cmdInput.GetWindowText(cstrInput);
		strcpy_s(chInputParam0,MAX_STRING_LENGTH,LPCSTR(cstrInput));

		bResult = GPIBReadCommandResult(chInputParam0,chInputParam1);		

		cstrOutputMsg.Format(_T("command result:%s"),chInputParam1);
		AddLog(cstrOutputMsg);
		cstrOutputMsg.Format(_T("exe result:%d"),bResult);
		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_SendQueryCommmand"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;		
		CString		cstrInput;
		BOOL			bResult;
		char			chInputParam[MAX_STRING_LENGTH];

		m_cmdInput.GetWindowText(cstrInput);
		vector<CString> vecInput;
		QSYNC_LIB::Tokenize<CString>(_T("^"), cstrInput, vecInput, true);
		char (*cmd)[MAX_STRING_LENGTH] = new char[vecInput.size()][MAX_STRING_LENGTH];		

		for (vector<CString>::size_type nIndex = 0; nIndex < vecInput.size(); ++nIndex)
		{
			strcpy_s(cmd[nIndex],MAX_STRING_LENGTH,LPCSTR(vecInput[nIndex]));
		}

		bResult = GPIBSendQueryCommmand(cmd[0],cmd[1],chInputParam);	

		cstrOutputMsg.Format(_T("command result:%s"),chInputParam);
		AddLog(cstrOutputMsg);
		cstrOutputMsg.Format(_T("exe result:%d"),bResult);
		AddLog(cstrOutputMsg);
	}
	else if(strSelected == _T("GPIB_ErrorMsg"))
	{
#define MAX_STRING_LENGTH 1024
		CString		cstrOutputMsg;		
		BOOL			bResult;
		char			chInputParam[MAX_STRING_LENGTH];

		bResult = GPIBErrorMsg(chInputParam);		

		cstrOutputMsg.Format(_T("error msg:%s"),chInputParam);
		AddLog(cstrOutputMsg);
		cstrOutputMsg.Format(_T("exe result:%d"),bResult);
		AddLog(cstrOutputMsg);
	}

	if (strSelected == _T("ReadFactoryHWSWMW"))
	{
		if(ReadFactoryHWSWMW != NULL)
		{	
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);

			char szHW5[5] = {0};
			char szSW9[9] = {0};
			char szMW2[5] = {0};
			if (ReadFactoryHWSWMW((LPTSTR)(LPCTSTR)strCOMPort, szHW5,szSW9,szMW2))
			{
				AddLog(szHW5);
				AddLog(szSW9);
				AddLog(szMW2);
			}
			else
				AddLog(_T("ReadFactoryHWSWMW Fail"));
		}
		else
			AddLog(_T("ReadFactoryHWSWMW Load function fail "));
	}
	else if (strSelected == _T("WriteFactoryHWSWMW"))
	{
		if(WriteFactoryHWSWMW != NULL)
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			if(strInput.GetLength() == 8 )
			{
				CString strCOMPort;
				m_COMPortCtrl.GetWindowText(strCOMPort);
				
				char szHW5[5] = {"101A"};
				//char szSW9[9] = {"01900000"};
				char szMW2[5] = {"A"};
				if (WriteFactoryHWSWMW((LPTSTR)(LPCTSTR)strCOMPort,szHW5,(LPTSTR)(LPCTSTR)strInput,szMW2))
				{
					AddLog(_T("WriteFactoryHWSWMW Success"));
				}
				else
					AddLog(_T("WriteFactoryHWSWMW Fail"));
			}
			else
				AddLog(_T("Input the index of szSW9(ex: 01000000), HW and WM will write default value\"101A\" , \"A\""));				
		}
		else 
			AddLog(_T("WriteFactoryHWSWMW Load function fail "));
	}
	else if (strSelected == _T("ReadProcessInfo"))
	{
		if(ReadProcessInfo != NULL)
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			if(strInput.GetLength() > 0 )
			{
				CString strCOMPort;
				m_COMPortCtrl.GetWindowText(strCOMPort);

				char szYear[5] = {0};
				char szMonth[5] = {0};
				char szDay[5] = {0};
				char szLine[5] = {0};
				char szStation[5] = {0};
				if (ReadProcessInfo((LPTSTR)(LPCTSTR)strCOMPort, (LPTSTR)(LPCTSTR)strInput,szYear,szMonth,szDay,szLine,szStation))
				{
					AddLog(szYear);
					AddLog(szMonth);
					AddLog(szDay);
					AddLog(szLine);
					AddLog(szStation);
				}
				else
					AddLog(_T("ReadProcessInfo Fail"));
			}
			else
				AddLog(_T("ReadProcessInfo Load function fail "));
		}
		else 
			AddLog(_T("Input the index of Process(1 - 10)"));
	}
	else if (strSelected == _T("WriteProcessInfo"))
	{
		if(WriteProcessInfo != NULL)
		{			
			CString strInput;
			m_cmdInput.GetWindowText(strInput);
			if(strInput.GetLength() > 0 )
			{
				CString strCOMPort;
				m_COMPortCtrl.GetWindowText(strCOMPort);

				if (WriteProcessInfo((LPTSTR)(LPCTSTR)strCOMPort, (LPTSTR)(LPCTSTR)strInput))
				{
					AddLog(_T("WriteProcessInfo Success"));
				}
				else
					AddLog(_T("WriteProcessInfo Fail"));
			}
			else
				AddLog(_T("WriteProcessInfo Load function fail "));
		}
		else 
			AddLog(_T("Input the index of Process(1 - 10)"));
	}
	else if (strSelected == _T("ReadTrackID"))
	{
		if(ReadTrackID != NULL)
		{
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);

			char szTrackID[11] = {0};
			if (ReadTrackID((LPTSTR)(LPCTSTR)strCOMPort, szTrackID))
			{
				AddLog(szTrackID);
			}
			else
				AddLog(_T("ReadTrackID Fail"));
		}
		else
			AddLog(_T("ReadTrackID Load function fail "));
	}
	else if (strSelected == _T("WriteTrackID"))
	{
		CString strInput;
		m_cmdInput.GetWindowText(strInput);
		if(strInput.GetLength() == 10)
		{
			if(WritePicassoNumber != NULL)
			{
				CString strCOMPort;
				m_COMPortCtrl.GetWindowText(strCOMPort);

				if (WriteTrackID((LPTSTR)(LPCTSTR)strCOMPort, (LPTSTR)(LPCTSTR)strInput))
				{
					AddLog(_T("WriteTrackID Success"));
				}
				else
					AddLog(_T("WriteTrackID Fail"));
			}
			else
				AddLog(_T("WriteTrackID Load function fail "));
		}
		else
			AddLog(_T("Please input TrackID"));
	}
	else if (strSelected == _T("ReadPicassoNumber"))
	{
		if(ReadPicassoNumber != NULL)
		{
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);

			char szPicassoNumber[11] = {0};
			if (ReadPicassoNumber((LPTSTR)(LPCTSTR)strCOMPort, szPicassoNumber))
			{
				AddLog(szPicassoNumber);
			}
			else
				AddLog(_T("ReadPicassoNumber Fail"));
		}
		else
			AddLog(_T("ReadPicassoNumber Load function fail "));
	}
	else if (strSelected == _T("WritePicassoNumber"))
	{
		CString strInput;
		m_cmdInput.GetWindowText(strInput);
		if(strInput.GetLength() == 10)
		{
			if(WritePicassoNumber != NULL)
			{
				CString strCOMPort;
				m_COMPortCtrl.GetWindowText(strCOMPort);

				if (WritePicassoNumber((LPTSTR)(LPCTSTR)strCOMPort, (LPTSTR)(LPCTSTR)strInput))
				{
					AddLog(_T("WritePicassoNumber Success"));
				}
				else
					AddLog(_T("WritePicassoNumber Fail"));
			}
			else
				AddLog(_T("WritePicassoNumber Load function fail "));
		}
		else
			AddLog(_T("Please input picasso number"));
	}
	else if (strSelected == _T("ReadMobileID_ForSPU"))
	{
		if(ReadMobileID_ForSPU != NULL)
		{
			char szLine7[50] = {0};
			char szAdapter3[50] = {0};
			char szSlot3[50] = {0};
			char szDay3[50] = {0};
			char szMonth3[50] = {0};
			char szYear3[50] = {0};
			char szHour3[50] = {0};
			char szMinute3[50] = {0};
			char szSecond3[50] = {0};

			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);

			bool bReadMobileID = ReadMobileID_ForSPU((LPTSTR)(LPCTSTR)strCOMPort, szLine7,szAdapter3,szSlot3,szDay3,szMonth3,szYear3,szHour3,szMinute3,szSecond3);

			CString szMobileID;
			szMobileID = szLine7;
			szMobileID.Replace(_T("T"),_T(""));
			szMobileID.Replace(_T("D"),_T(""));
			szMobileID += _T("00");
			szMobileID += szAdapter3;
			szMobileID += szSlot3;
			szMobileID += szDay3;
			szMobileID += szMonth3;
			szMobileID += szYear3;
			szMobileID += szHour3;
			szMobileID += szMinute3;
			szMobileID += szSecond3;

			if (bReadMobileID)
			{
				AddLog(CString("MOBILEID : ") + szMobileID);
			}
			else
				AddLog(_T("ReadMobileID_ForSPU Fail"));
		}
		else
			AddLog(_T("ReadMobileID_ForSPU Load function fail "));
	}
	else if (strSelected == _T("WriteMobileID_ForSPU"))
	{
		if(WriteMobileID_ForSPU != NULL)
		{
			CString strCOMPort;
			m_COMPortCtrl.GetWindowText(strCOMPort);

			//Analysis the input command, translate to integer.
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if (strInput.IsEmpty())
				AddLog(_T("WriteMobileID_ForSPU need slot number."));
			else if(WriteMobileID_ForSPU((LPTSTR)(LPCTSTR)strCOMPort, (LPTSTR)(LPCTSTR)strInput))
				AddLog(_T("WriteMobileID_ForSPU Success"));
			else
				AddLog(_T("WriteMobileID_ForSPU Fail"));
		}
		else
			AddLog(_T("WriteMobileID_ForSPU Load function fail "));
	}
	else if (strSelected == _T("ReadHWVersion_FromFactoryArea"))
	{
		if(ReadHWVersion_FromFactoryArea != NULL)
		{
			char szHWVersion[10] = {0};
			if (ReadHWVersion_FromFactoryArea(nPort, szHWVersion))
			{
				AddLog(szHWVersion);
				AddLog(_T("ReadHWVersion_FromFactoryArea Success"));
			}
			else
				AddLog(_T("ReadHWVersion_FromFactoryArea Fail"));
		}
		else
			AddLog(_T("ReadHWVersion_FromFactoryArea Load function fail "));
	}
	else if (strSelected == _T("WriteHWVersion_ToFactoryArea"))
	{
		if(WriteHWVersion_ToFactoryArea != NULL)
		{
			CString strInput;
			m_cmdInput.GetWindowText(strInput);

			if (strInput.IsEmpty())
				AddLog(_T("The input HW version is empty, please check it."));
			else if(WriteHWVersion_ToFactoryArea(nPort, strInput.GetBuffer()))
				AddLog(_T("WriteHWVersion_ToFactoryArea Success"));
			else
				AddLog(_T("WriteHWVersion_ToFactoryArea Fail"));
			strInput.ReleaseBuffer();
		}
		else
			AddLog(_T("WriteHWVersion_ToFactoryArea Load function fail "));
	}
}

void CImplementedDiagCommandTesterDlg::OnBnClickedUpdateport()
{
	CString strCOMPort;
	m_COMPortCtrl.GetWindowText(strCOMPort);	
	UpdatePort(::_ttoi(strCOMPort));
}

void CImplementedDiagCommandTesterDlg::UpdatePort(int nPort)
{
	m_nPortNum = nPort;
	CString strCOMPort;
	strCOMPort.Format(_T("%d"),m_nPortNum);
	m_COMPortCtrl.SetWindowText(strCOMPort);
}
void CImplementedDiagCommandTesterDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CImplementedDiagCommandTesterDlg::OnLbnSelchangeLoglist()
{
	// TODO: Add your control notification handler code here
}

void CImplementedDiagCommandTesterDlg::AddLog(const CString& strLog)
{
	m_LogLISTCtrl.SetTopIndex(m_LogLISTCtrl.AddString(strLog));
	IReportCenter::Log("TEST_UI",9999,-1,4,(LPCSTR)strLog);
	m_LogLISTCtrl.UpdateWindow();	
}

void CImplementedDiagCommandTesterDlg::LogBinary(const char* szTemp,DWORD nBufferSize)
{
#ifndef DISABLE_LOG

	const int nLenMax = 32;
	char szCharTemp[nLenMax+1] = {0};
	int nLineCount = 0;

	CStringA strLog;

	for(DWORD i=0;i<nBufferSize;i++)
	{		
		if(i > 0 && i % nLenMax == 0)
		{
			strLog += "   ";
			for(int j=0;j<nLenMax;j++)
			{
				CStringA strTemp;
				strTemp.Format("%c",szCharTemp[j]);
				strLog += strTemp;
			}
			strLog += "\n";
			nLineCount = 0;
		}
		BYTE nTest = szTemp[i];
		CStringA strTemp;
		strTemp.Format("%02X ",nTest);
		strLog += strTemp;
		szCharTemp[i%nLenMax] = (nTest <= 0x1F || nTest >= 0xFF) ? '.' : szTemp[i];
		nLineCount++;
	}

	for(int nSpaceCount = 0;nSpaceCount < (nLenMax - nLineCount);nSpaceCount++)	
		strLog += "   ";

	strLog += "   ";
	for(int j=0;j<nLineCount;j++)
	{		
		CStringA strTemp;
		strTemp.Format("%c",szCharTemp[j]);
		strLog += strTemp;
	}
	strLog += "\n\n";

	AddLog(strLog);

#endif//#ifndef DISABLE_LOG
}

CImplementedDiagCommandTesterDlg* g_pThis = NULL;
DWORD WINAPI CImplementedDiagCommandTesterDlg::ThreadProc( LPVOID lpParam ) 
{ 
	CString strRepeat;
	g_pThis->m_RepeatCtrl.GetWindowText(strRepeat);
	int nRepeat = ::_ttoi(strRepeat);
	for(int i=0;i<nRepeat;i++)
	{
		if(g_pThis)
			g_pThis->Function((int)lpParam);
	}
	return 0;
}

void CImplementedDiagCommandTesterDlg::OnBnClickedLoopbutton()
{
	g_pThis  = this;
	for(int nID = IDC_LOOP_START;nID < IDC_LOOP_END;nID++)
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(nID);
		if(pEdit && ::IsWindow(pEdit->GetSafeHwnd()))
		{
			CString strCOMPort;
			pEdit->GetWindowText(strCOMPort);
			if(strCOMPort.IsEmpty() == false)
			{
				DWORD nThreadID = 0;
				::CreateThread(NULL,0,ThreadProc,(LPVOID)::_ttoi(strCOMPort),0,&nThreadID);
			}
		}
	}
}

void CImplementedDiagCommandTesterDlg::OnBnClickedClearButton()
{
	while( m_LogLISTCtrl.GetCount() > 0)
		m_LogLISTCtrl.DeleteString( 0 );
}

#pragma warning(pop)