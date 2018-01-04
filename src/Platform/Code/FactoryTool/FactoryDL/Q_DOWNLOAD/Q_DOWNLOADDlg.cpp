// Q_DOWNLOADDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Q_DOWNLOAD.h"
#include "Q_DOWNLOADDlg.h"
#include "SO.h"
#include "PASSWORD.h"
#include "Config.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////
////////// mei's setting//////////////////////////////////////////////////
CQ_DOWNLOADDlg* g_pThis = NULL;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#define        DEFAULT_CMD_TIME_OUT  3000
#define Default_SPC_Code            "000000"
#define HW_Version_Size         5
#define SW_Version_Size         4 
#define Mobile_ID_Size          11
#define PASS                                    1
#define FAIL                                    0
#define WM_TERMINATETHREAD              WM_USER+100
#define WM_UPDATEPROGRESS               WM_USER+101
#define WM_QUERYIMEISTATUS              WM_USER+102

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQ_DOWNLOADDlg dialog

CQ_DOWNLOADDlg::CQ_DOWNLOADDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQ_DOWNLOADDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQ_DOWNLOADDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	

}


void CQ_DOWNLOADDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQ_DOWNLOADDlg)
	DDX_Control(pDX, IDC_STATIC_QVCEFS, m_QVCEFS_FILE);
	DDX_Control(pDX, IDC_CONFIG, m_config);
	DDX_Control(pDX, IDC_LIST_MESSAGE, m_LISTBOX);
	DDX_Control(pDX, IDC_TITLE, m_TITLE);
	DDX_Control(pDX, IDC_STATIC_PCNAME, m_PCNAME);
	DDX_Control(pDX, IDC_STATIC_OEMSBL, m_OEMSBL);
	DDX_Control(pDX, IDC_STATIC_TOTAL, m_TOTAL);
	DDX_Control(pDX, IDC_STATIC_AMSSH, m_AMSSH);
	DDX_Control(pDX, IDC_STATIC_SW, m_SW);
	DDX_Control(pDX, IDC_STATIC_SO, m_SO);
	DDX_Control(pDX, IDC_STATIC_QCSBLH, m_QCSBLH);
	DDX_Control(pDX, IDC_STATIC_QCSBL, m_QCSBL);
	DDX_Control(pDX, IDC_STATIC_PASS, m_PASS);
	DDX_Control(pDX, IDC_STATIC_PARTITION, m_PARTITION);
	DDX_Control(pDX, IDC_STATIC_OEMSBLH, m_OEMSBLH);
	DDX_Control(pDX, IDC_STATIC_HW, m_HW);
	DDX_Control(pDX, IDC_STATIC_Hex, m_HEX);
	DDX_Control(pDX, IDC_STATIC_FAIL, m_FAIL);
	DDX_Control(pDX, IDC_STATIC_AMSS, m_AMSS);
	DDX_Control(pDX, IDC_QVCEFS_TIME, m_QVCEFS_TIME);
	DDX_Control(pDX, IDC_QVCEFS_SIZE, m_QVCEFS_SIZE);
	//}}AFX_DATA_MAP
	for(int i=0;i<g_nSlotTotalCount;i++)
	{
		DDX_Control(pDX, IDC_PROGRESS_MAIN1+i, m_MAINProgressCtrl[i]);
		DDX_Control(pDX, IDC_PROGRESS_SUB1+i, m_SUBProgressCtrl[i]);
		DDX_Control(pDX, IDC_STATIC_PROGRESS1+i, m_LabelPROGRESS[i]);
		DDX_Control(pDX, IDC_STATIC_COM3+i, m_LabelCOM[i]);
	}

	for(int i=0; i<18;i++)
		DDX_Control(pDX, IDC_STATIC1+i,m_Static[i]);
}

BEGIN_MESSAGE_MAP(CQ_DOWNLOADDlg, CDialog)
	//{{AFX_MSG_MAP(CQ_DOWNLOADDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//ON_MESSAGE(WM_TERMINATETHREAD, OnTerminateThread)
	//ON_MESSAGE(WM_UPDATEPROGRESS, ThreadSafe_OnUPDATEPROGRESS)
	//ON_MESSAGE(WM_QUERYIMEISTATUS, ThreadSafe_OnQUERYIMEISTATUS)
	ON_BN_CLICKED(IDC_BUTTON_START, OnButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnButtonReset)
	ON_BN_CLICKED(IDC_TEST, OnTest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_Stop, &CQ_DOWNLOADDlg::OnBnClickedBtnStop)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQ_DOWNLOADDlg message handlers
void CQ_DOWNLOADDlg::Event(const std::string& strEvent,long nParam)
{
	if(strEvent == EVENT_UI_MESSAGE)
	{
		AddMessage((const char*)nParam);
	}
	else if(strEvent == EVENT_UI_PROGRESS)
	{
		UIProgress* pStruct = (UIProgress*)nParam;
		SetProgress(pStruct->nSlot, pStruct->nMAINProgress, pStruct->nSUBProgress);
	}
	else if(strEvent == EVENT_DL_RESULT)
	{
		UIResult* pStruct = (UIResult*)nParam;
		bool bPASS = strcmp(pStruct->szResult, "PASS") == 0;
		if(bPASS)
			m_LabelPROGRESS[pStruct->nSlot-1].SetBkColor(RGB(0,255,0));
		else
			m_LabelPROGRESS[pStruct->nSlot-1].SetBkColor(RGB(255,0,0));
		AddTotalCount(bPASS, pStruct->szResult);
		m_LabelPROGRESS[pStruct->nSlot-1].SetWindowText(pStruct->szResult);
		
		if(m_nCycTimes!=0) // it's loop testing
			--m_TempCycle[pStruct->nSlot-1];

		if(m_bStop)
		{
			int nPort = 0;
			if(GetPortBySlot(pStruct->nSlot, nPort))
				SetStopRunningFlag(pStruct->nSlot, nPort);
			if(!IsAnyUnitProcessing())
			{
				UI_SetStart(TRUE);
				m_bStop = false;
			}
		}
		else
		{// not stop, see if it's loop testing.
			if(m_nCycTimes!=0)
			{
				bool bIsFinishLoopTest = true;
				for(int i=0; i< g_nSlotTotalCount; i++)
				{
					bIsFinishLoopTest = bIsFinishLoopTest && m_TempCycle[i]==0 ;
					int nPort = 0;
					if( m_TempCycle[i]==0 && GetPortBySlot(i+1, nPort))
						SetStopRunningFlag(i+1, nPort);
				}
				if(bIsFinishLoopTest)
					OnBnClickedBtnStop();
				else
				{
					int nPort =0;
					if(GetPortBySlot(pStruct->nSlot, nPort) &&  
						m_TempCycle[pStruct->nSlot-1] != 0)
					{
						RunProcess(pStruct->nSlot, nPort);
					}
				}
			}
		}
	}
	else if(strEvent == QSYNC_LIB::DEVICE_DIAG_CONNECT)
	{
		if(m_nCycTimes==0)  // if not loop testing, run process automatically.
		{
			QSYNC_LIB::DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
			QSYNC_LIB::DeviceDataPt spDeviceData = *pspDeviceData;
			//std:string strDeviceClass = spDeviceData->m_strDeviceClass;
			std::string strPort = spDeviceData->m_strPort;
			
			int nPort = atoi(strPort.substr(3, strPort.size()-3).c_str());
			int nSlot = 0;
			if(!m_bStop && !m_bStartON && GetSlotByPort(nPort, nSlot))
			{
				bool bRunning = false;
				if(ISUnitProcessing(nSlot, nPort, bRunning) && !bRunning)
				{
					RunProcess(nSlot, nPort);
				}	
			}
		}	
	}
	else if( strEvent == EVENT_RESET_UI)
	{
		ResetUIStatus(nParam);
	}
	//else if(DEVICE_DIAG_DISCONNECT)
}
BOOL CQ_DOWNLOADDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	/*CMotoDownload dl;
	std::string strPrefix;
	CStringA strSymLink;
	if(dl.GetPrefix(3, strPrefix))	
		dl.IsNormalMode(strPrefix, strSymLink);*/

	m_bSkipComputerName = false;
	QSYNC_LIB::GetDeviceFinder()->Register(this);
	QSYNC_LIB::GetDeviceFinder()->StartMonitor();
	m_bStop = false;  // default value
	m_bStartON = true;  // default value
	
	CSO SODlg;
	if(SODlg.DoModal()==IDCANCEL)
	{
		CDialog::OnCancel();
		return false;
	}
	
	CINITIAL INITIAL;
	if(INITIAL.DoModal(&m_SetupInfo)==IDCANCEL)
	{
		CDialog::OnCancel();
		return false;
	}
	
	Query_SettingParameters(m_SetupInfo.strSETUP_INI_FILE_PATH);
	
	if(!UI_Setting())
	{
		CDialog::OnCancel();
		return false;
	}

	/*if(!Initial_DLL())
	{
		CDialog::OnCancel();
		return false;
	}*/
	
/*	if(Power_Supply_Control_Flag)
	{
		if(!PowerSupply_Handle.Init_PowerSupply())
		{
			AfxMessageBox("Initial Power Supply Fail!!");
			CDialog::OnCancel();
			return false;
		}
 		else
 			PowerSupply_Handle.Power_Down();
		
	}*/

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQ_DOWNLOADDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQ_DOWNLOADDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQ_DOWNLOADDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CQ_DOWNLOADDlg::Query_SettingParameters(CString strSetupFilePath)
{
	char temp[512] = {0};
	::GetPrivateProfileString("TOOL_VERSION","TOOL_VERSION","V1.00",temp,sizeof(temp),strSetupFilePath);
	m_SetupInfo.strTool_Version=temp;
	::GetPrivateProfileString("CONFIG_VERSION","CONFIG_VERSION","V1.00",temp,sizeof(temp),strSetupFilePath);
	m_SetupInfo.strConfig_Version=temp;
	::GetPrivateProfileString("SO_NUMBER","OLD","12345",temp,sizeof(temp),strSetupFilePath);
	m_SetupInfo.strSO = temp;
	::GetPrivateProfileString("Setting","CSV_PATH",".\\NV_Item.csv",temp,sizeof(temp),strSetupFilePath);
	m_SetupInfo.strCSV_PATH = temp;
	m_SetupInfo.bBackupBrt = ::GetPrivateProfileInt("Setting","BackupBrt",0,strSetupFilePath);

	m_SetupInfo.nPowerOnDelay                =::GetPrivateProfileInt("DELAY","PowerOnDelay",1000,strSetupFilePath);
	m_SetupInfo.nDEBUG_ONOFF_FLAG            =::GetPrivateProfileInt("DEBUG_INFOR","ONOFF",1,strSetupFilePath);
	m_SetupInfo.nPower_Supply_Control_Flag   =::GetPrivateProfileInt("POWER_SUPPLY_SETTING","POWER_SUPPLY_CONTROL_FLAG",1,strSetupFilePath);
/*	::GetPrivateProfileString("POWER_SUPPLY_SETTING","POWER_SUPPLY_VOLTAGE","3.3",temp,sizeof(temp),SetupFilePath);
	Power_On_Voltage=temp;
	::GetPrivateProfileString("POWER_SUPPLY_SETTING","POWER_SUPPLY_CURRENT","2.0",temp,sizeof(temp),SetupFilePath);
    Power_On_Current=temp;*/
	m_nTotalNo                     =::GetPrivateProfileInt("TEST_RECORD","TOTAL",0,strSetupFilePath);
	m_nPassNo                      =::GetPrivateProfileInt("TEST_RECORD","PASS",0,strSetupFilePath);
	m_nFailNo                      =::GetPrivateProfileInt("TEST_RECORD","FAIL",0,strSetupFilePath);
	
	int nSkip = ::GetPrivateProfileInt("CHECK","SkipComputerName",0,strSetupFilePath);
	m_bSkipComputerName    = ::GetPrivateProfileInt("CHECK","SkipComputerName",0,strSetupFilePath)==0? false:true;

	m_SetupInfo.nIMEI_CHECK=::GetPrivateProfileInt("CHECK","IMEI_CHECK",1,strSetupFilePath);

	m_SetupInfo.bCheckFactoryArea = ::GetPrivateProfileInt("Setting","CheckFactoryArea",1,strSetupFilePath);
	m_SetupInfo.nOnly_DL_Qvcefs_Flag = ::GetPrivateProfileInt("Setting","Only_DL_Qvcefs_Flag",0,strSetupFilePath);
	m_SetupInfo.nDL_Bootloader_Flag = ::GetPrivateProfileInt("Setting","DL_Bootloader",0,strSetupFilePath);
	m_SetupInfo.nPowerOnTimeout = ::GetPrivateProfileInt("TimeoutSetting","PowerOnTimeout",27000,strSetupFilePath);
	m_SetupInfo.nRestartTimeout = ::GetPrivateProfileInt("TimeoutSetting","RestartTimeout",10000,strSetupFilePath);
	m_SetupInfo.nCreateLog = ::GetPrivateProfileInt("TestTime","CreateLog",0,strSetupFilePath);
	m_SetupInfo.nSaveLog = ::GetPrivateProfileInt("Setting","SaveLog",1,strSetupFilePath);
	
	//m_nTestTime=::GetPrivateProfileInt("TestTime","time",200,strSetupFilePath);
	m_nCycTimes = ::GetPrivateProfileInt("TestTime","CycleTimes",0,strSetupFilePath); // 0= no cylce, more than 0 = n cylce;
	
	char szDate[100] = {0};
	char szSize[100] = {0};
	GetFileSizeTime(m_SetupInfo.strCSV_PATH, szDate, szSize);
	m_SetupInfo.strCSV_TIME = szDate;
	m_SetupInfo.strCSV_SIZE = szSize;

	(CButton *)GetDlgItem(IDC_TEST)->ShowWindow(/*m_nCycTest==1 ? SW_SHOW : */SW_HIDE);
	
	if( m_SetupInfo.strMODEL_NAME.Find("LAVS") != -1)
	{
		FStrCpy(m_SetupInfo.strHEX_FILE_NAME,m_DL6245Setting.szHex);
		FStrCpy(m_SetupInfo.strPARTITION_FILE_NAME,m_DL6245Setting.szPartition);
		FStrCpy(m_SetupInfo.strAMSS_FILE_NAME,m_DL6245Setting.szAmss);
		
		FStrCpy(m_SetupInfo.strQCSBLH_FILE_NAME,m_DL6245Setting.szQcsblhd);
		FStrCpy(m_SetupInfo.strQCSBL_FILE_NAME,m_DL6245Setting.szQcsbl);
		FStrCpy(m_SetupInfo.strOEMSBLH_FILE_NAME,m_DL6245Setting.szOemsblhd);
    	FStrCpy(m_SetupInfo.strOEMSBL_FILE_NAME,m_DL6245Setting.szOemsbl);
		FStrCpy(m_SetupInfo.strAMSSH_FILE_NAME,m_DL6245Setting.szAmsshd);

		if(m_SetupInfo.strQVCEFS_FILE_NAME=="N/A")
			strcpy(m_DL6245Setting.szCefs,"");
		else
			strcpy(m_DL6245Setting.szCefs, m_SetupInfo.strSW_Path+ "Flex_Option\\" + m_SetupInfo.strQVCEFS_FILE_NAME);
	}
	if( m_SetupInfo.strMODEL_NAME.Find("MELB") != -1)
	{
		FStrCpy(m_SetupInfo.strHEX_FILE_NAME,m_DL6240Setting.szHex);
		FStrCpy(m_SetupInfo.strPARTITION_FILE_NAME,m_DL6240Setting.szPartition);
		FStrCpy(m_SetupInfo.strAMSS_FILE_NAME,m_DL6240Setting.szAmss);

		FStrCpy(m_SetupInfo.strDBL_FILE_NAME,m_DL6240Setting.szDBL);
		FStrCpy(m_SetupInfo.strFSBL_FILE_NAME,m_DL6240Setting.szFSBL);
    	FStrCpy(m_SetupInfo.strOSBL_FILE_NAME,m_DL6240Setting.szOSBL);

		if(m_SetupInfo.strQVCEFS_FILE_NAME=="N/A")
			strcpy(m_DL6240Setting.szCefs,"");
		else
			strcpy(m_DL6240Setting.szCefs, m_SetupInfo.strSW_Path+ "Flex_Option\\" + m_SetupInfo.strQVCEFS_FILE_NAME);
	}
}

bool CQ_DOWNLOADDlg::UI_Setting()
{
	HBITMAP BitMap_RESET=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_RESET));
	((CButton *)GetDlgItem(IDC_BUTTON_RESET))->SetBitmap(BitMap_RESET);
	
	HBITMAP BitMap_START=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_START));
	((CButton *)GetDlgItem(IDC_BUTTON_START))->SetBitmap(BitMap_START);
    
	HBITMAP BitMap_EXIT=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_EXIT));
	((CButton *)GetDlgItem(IDC_BUTTON_EXIT))->SetBitmap(BitMap_EXIT);

	HBITMAP BitMap_Stop=LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_CONFIG));
	((CButton *)GetDlgItem(IDC_BTN_Stop))->SetBitmap(BitMap_Stop);

	unsigned long nLen=50;
	char temp[512] = "T01D02";
	if(!m_bSkipComputerName)
		GetComputerName(temp,&nLen);
	else
		::AfxMessageBox("Skip Computer Name, Set Default Computer Name=T01D02");
	m_strCOMPUTER_NAME=temp;

 	//COMPUTER_NAME will be like "T01D02";
	
 	if(((m_strCOMPUTER_NAME.GetAt(1)<'0')||(m_strCOMPUTER_NAME.GetAt(1)>'9'))||
		((m_strCOMPUTER_NAME.GetAt(2)<'0')||(m_strCOMPUTER_NAME.GetAt(2)>'9'))||
		((m_strCOMPUTER_NAME.GetAt(4)<'0')||(m_strCOMPUTER_NAME.GetAt(4)>'9'))||
		((m_strCOMPUTER_NAME.GetAt(5)<'0')||(m_strCOMPUTER_NAME.GetAt(5)>'9')))
	{
		CString strMessage_String;
		strMessage_String="The computer name is "+m_strCOMPUTER_NAME+"!!\nit should be like T01D01!!";
		::AfxMessageBox(strMessage_String);
	//	return false;
	}
 	m_PCNAME.SetWindowText(m_strCOMPUTER_NAME);
	
	ZeroMemory(temp,sizeof(temp));
	
	::CreateDirectory(m_SetupInfo.strCurrentDirectory+"\\log",NULL);
	
	m_TITLE.SetBkColor(RGB(158,202,96));
	m_TITLE.SetFontSize(30);
	m_TITLE.SetTextColor(RGB(0,0,255));
	m_TITLE.SetFontBold(TRUE);

 	for(int i=0;i<18;i++)
	{
		m_Static[i].SetBkColor(RGB(227,248,253));
	} 

	m_SetupInfo.bREWORK_FLAG?m_TITLE.SetWindowText(m_SetupInfo.strMODEL_NAME+" Re_Multi_Download "+m_SetupInfo.strTool_Version):m_TITLE.SetWindowText(m_SetupInfo.strMODEL_NAME+" Multi_Download "+m_SetupInfo.strTool_Version);
 
	m_SW.SetWindowText(m_SetupInfo.strSW_VERSION);
	m_HW.SetWindowText(m_SetupInfo.strHW_VERSION);
	m_SO.SetWindowText(m_SetupInfo.strSO);
	m_HEX.SetWindowText(m_SetupInfo.strHEX_FILE_NAME);
	m_PARTITION.SetWindowText(m_SetupInfo.strPARTITION_FILE_NAME);
	m_AMSS.SetWindowText(m_SetupInfo.strAMSS_FILE_NAME);
	
	m_OEMSBLH.SetWindowText(m_SetupInfo.strOEMSBLH_FILE_NAME);


	if( m_SetupInfo.strMODEL_NAME.Find("LAVS") != -1)
	{
		m_OEMSBL.SetWindowText(m_SetupInfo.strOEMSBL_FILE_NAME);
		m_QCSBL.SetWindowText(m_SetupInfo.strQCSBL_FILE_NAME);
    	m_QCSBLH.SetWindowText(m_SetupInfo.strQCSBLH_FILE_NAME);
		m_AMSSH.SetWindowText(m_SetupInfo.strAMSSH_FILE_NAME);
	}
	
	if(	m_SetupInfo.strMODEL_NAME.Find("MELB") != -1)
	{
		m_Static[2].SetWindowText("DBL File:");
		m_Static[5].SetWindowText("OSBL File:");
		m_Static[6].SetWindowText("FSBL File:");
        m_Static[3].SetWindowText("BRT File:");

		m_OEMSBL.SetWindowText(m_SetupInfo.strOSBL_FILE_NAME);
		m_QCSBL.SetWindowText(m_SetupInfo.strFSBL_FILE_NAME);
    	m_QCSBLH.SetWindowText(m_SetupInfo.strDBL_FILE_NAME);
		m_AMSSH.SetWindowText(m_SetupInfo.strQCN_FILE_NAME);
	}

	m_QVCEFS_FILE.SetWindowText(m_SetupInfo.strQVCEFS_FILE_NAME);

	if(m_SetupInfo.strQVCEFS_FILE_NAME.GetLength()>100)
	{
	     AfxMessageBox("PPF File Name is longer than 100......");
	     return false;
	}
	
	char Date[20]={0};
	char Size[20]={0};
	if(m_SetupInfo.strMODEL_NAME.Find("LAVS") != -1)
		GetFileSizeTime(m_DL6245Setting.szCefs,Date,Size);
	else if(m_SetupInfo.strMODEL_NAME.Find("MELB") != -1)
		GetFileSizeTime(m_DL6240Setting.szCefs,Date,Size);
	m_QVCEFS_TIME.SetWindowText(Date);
	m_SetupInfo.strQVCEFS_FILE_TIME = Date;
	m_QVCEFS_SIZE.SetWindowText(Size);
	m_SetupInfo.strQVCEFS_FILE_SIZE = Size;

	sprintf(temp,"%u",m_nTotalNo);
	m_TOTAL.SetWindowText(temp);
	sprintf(temp,"%u",m_nPassNo);
	m_PASS.SetWindowText(temp);
	sprintf(temp,"%u",m_nFailNo);
	m_FAIL.SetWindowText(temp);

	for(int i=0;i<g_nSlotTotalCount;i++)
	{
		m_MAINProgressCtrl[i].SetRange(0,100);
		m_MAINProgressCtrl[i].SetPos(0);
		m_MAINProgressCtrl[i].SendMessage(PBM_SETBKCOLOR,0,RGB(255,255,255));
		m_MAINProgressCtrl[i].SendMessage(PBM_SETBARCOLOR,0,RGB(0,0,255)); 
		m_SUBProgressCtrl[i].SetRange(0,100);
		m_SUBProgressCtrl[i].SetPos(0);
		m_SUBProgressCtrl[i].SendMessage(PBM_SETBKCOLOR,0,RGB(255,255,255));
		m_SUBProgressCtrl[i].SendMessage(PBM_SETBARCOLOR,0,RGB(0,0,255)); 
		m_LabelPROGRESS[i].SetBkColor(RGB(255, 255, 255));
		m_LabelPROGRESS[i].SetTextColor(RGB(0,0,255));
		m_LabelPROGRESS[i].SetFontSize(18);
		m_LabelPROGRESS[i].SetFontBold(TRUE);
		m_LabelPROGRESS[i].SetWindowText("0%");
		m_LabelCOM[i].SetBkColor(RGB(158,202,96));
		m_LabelCOM[i].SetTextColor(RGB(0,0,255));
		m_LabelCOM[i].SetFontBold(TRUE);
	}
	m_LISTBOX.ResetContent();

	for(int i=0;i<g_nSlotTotalCount;i++)
	{
		char strport[10];
		sprintf(strport,"Port%d",i+1);
		CString portstr=strport;
		int nComport=GetPrivateProfileInt("COMPORT",portstr,0,m_SetupInfo.strSETUP_INI_FILE_PATH);
		char szTemp[10]= {0};
        sprintf(szTemp,"COM%d",nComport);
		m_LabelCOM[i].SetWindowText(szTemp);
	}
		
    return true;
}

void CQ_DOWNLOADDlg::ResetUIStatus(int nSlot)
{
	m_MAINProgressCtrl[nSlot].SetRange(0,100);
	m_MAINProgressCtrl[nSlot].SetPos(0);
	m_MAINProgressCtrl[nSlot].SendMessage(PBM_SETBKCOLOR,0,RGB(255,255,255));
	m_MAINProgressCtrl[nSlot].SendMessage(PBM_SETBARCOLOR,0,RGB(0,0,255)); 
	m_SUBProgressCtrl[nSlot].SetRange(0,100);
	m_SUBProgressCtrl[nSlot].SetPos(0);
	m_SUBProgressCtrl[nSlot].SendMessage(PBM_SETBKCOLOR,0,RGB(255,255,255));
	m_SUBProgressCtrl[nSlot].SendMessage(PBM_SETBARCOLOR,0,RGB(0,0,255)); 
	m_LabelPROGRESS[nSlot].SetBkColor(RGB(255, 255, 255));
	m_LabelPROGRESS[nSlot].SetTextColor(RGB(0,0,255));
	m_LabelPROGRESS[nSlot].SetFontSize(18);
	m_LabelPROGRESS[nSlot].SetFontBold(TRUE);
	m_LabelPROGRESS[nSlot].SetWindowText("0%");
	m_LabelCOM[nSlot].SetBkColor(RGB(158,202,96));
	m_LabelCOM[nSlot].SetTextColor(RGB(0,0,255));
	m_LabelCOM[nSlot].SetFontBold(TRUE);
	
}
void CQ_DOWNLOADDlg::OnButtonStart() 
{
	m_LISTBOX.ResetContent();	
	char temp[512] = {0};

	UI_Setting();
	UI_SetStart(FALSE);

    m_config.EnableWindow(FALSE);
  
/*	if(Power_Supply_Control_Flag)
	{
		PowerSupply_Handle.Power_On();
	}*/
	
	for(int i=0;i<g_nSlotTotalCount;i++)
	{	
		m_TempCycle[i] = m_nCycTimes; // Beginning cycle times

		long nPort =0;
		char szPort[10] = {0};
		sprintf(szPort,"Port%d",i+1);
	
		nPort = GetPrivateProfileInt("COMPORT",szPort,0, m_SetupInfo.strSETUP_INI_FILE_PATH);
		int nComPort = 0;
		HandsetInfo info = {i+1, nPort, nComPort, false};
		//if( m_SetupInfo.strMODEL_NAME.Find("MELB") != -1 )
		//{
		//	if(CMotoDownload::MotoPortToCOMPort(nPort, nComPort))
		//		info.nCOMPort = nComPort;
		//}
		m_vRunningUnit[i] = info;
		RunProcess(i+1, nPort);
	}	

}
struct StructThreadParameter
{
	int nSlot;
	long nPort;
	CQ_DOWNLOADDlg* pThis;
};
void CQ_DOWNLOADDlg::RunProcess(int nSlot, long nPort)
{
	g_pThis = this;
	
	if(m_SetupInfo.strMODEL_NAME.Find("LAVS") != -1)
	{
		m_TempSlot[nSlot-1] = nSlot;
		AfxBeginThread(AfxRun6245,&m_TempSlot[nSlot-1],THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
	else if(m_SetupInfo.strMODEL_NAME.Find("MELB") != -1)
	{
		m_TempSlot[nSlot-1] = nSlot;
		//AfxBeginThread(AfxRunMotoProcess,&m_TempSlot[nSlot],THREAD_PRIORITY_NORMAL,0,0,NULL);
		AfxBeginThread(AfxRun6240,&m_TempSlot[nSlot-1],THREAD_PRIORITY_NORMAL,0,0,NULL);
	}
}

UINT CQ_DOWNLOADDlg::AfxRun6240(LPVOID pParam)
{
	int nSlot = *((int*)pParam);

	int nPort = 0;
	if(g_pThis->GetPortBySlot(nSlot, nPort))
	{
		g_pThis->ModifyUnitStatus(nSlot, nPort, true);
			CDLProcess DLProcess(nPort);

		DLProcess.Register(g_pThis, EVENT_UI_MESSAGE);
		DLProcess.Register(g_pThis, EVENT_UI_PROGRESS);
		DLProcess.Register(g_pThis, EVENT_DL_RESULT);
		DLProcess.Register(g_pThis, EVENT_RESET_UI);

		//////////////////////////////////////////////////////////////////////////
		//6240 Setting

		DLProcess.Set6240DLPath(g_pThis->m_DL6240Setting);

		DLProcess.SetRework(			g_pThis->m_SetupInfo.bREWORK_FLAG);
		DLProcess.SetCheckFactoryArea(  g_pThis->m_SetupInfo.bCheckFactoryArea);
		DLProcess.SetIMEICheck(			g_pThis->m_SetupInfo.nIMEI_CHECK);
		DLProcess.SetRestartTimeout(	g_pThis->m_SetupInfo.nRestartTimeout);

		DLProcess.SetSlotNumber(		nSlot);
		DLProcess.SetComputerName(		g_pThis->m_strCOMPUTER_NAME);
		DLProcess.SetSo(				g_pThis->m_SetupInfo.strSO);
		DLProcess.SetToolVersion(		g_pThis->m_SetupInfo.strTool_Version);
		DLProcess.SetConfigVersion(		g_pThis->m_SetupInfo.strConfig_Version);
		DLProcess.SetSWVersion(			g_pThis->m_SetupInfo.strSW_VERSION);
		DLProcess.SetHWVersion(			g_pThis->m_SetupInfo.strHW_VERSION);
		DLProcess.SetModelName(			g_pThis->m_SetupInfo.strMODEL_NAME);
		DLProcess.SetOnlyDLCEFS(		g_pThis->m_SetupInfo.nOnly_DL_Qvcefs_Flag);
		DLProcess.SetDLBootloader(		g_pThis->m_SetupInfo.nDL_Bootloader_Flag);
		DLProcess.SetBRT_BACKUP_PATH(   g_pThis->m_SetupInfo.strBRT_BACKUP_PATH);
		DLProcess.SetBackupBrt(			g_pThis->m_SetupInfo.bBackupBrt);
		DLProcess.SetCSV( g_pThis->m_SetupInfo.strCSV_PATH, g_pThis->m_SetupInfo.strCSV_TIME, g_pThis->m_SetupInfo.strCSV_SIZE);
		DLProcess.SetSaveLog(			g_pThis->m_SetupInfo.nSaveLog==0 ? false : true);
		DLProcess.SetIsLoopTest(		g_pThis->m_nCycTimes > 0);

		DLProcess.SetQVCEFS_File_Name(	g_pThis->m_SetupInfo.strQVCEFS_FILE_NAME);
		DLProcess.SetQVCEFS_File_Time(	g_pThis->m_SetupInfo.strQVCEFS_FILE_TIME);
		DLProcess.SetQVCEFS_File_Size(	g_pThis->m_SetupInfo.strQVCEFS_FILE_SIZE);
		//////////////////////////////////////////////////////////////////////////

		DLProcess.Run6240();

		DLProcess.Unregister(g_pThis, EVENT_UI_MESSAGE);
		DLProcess.Unregister(g_pThis, EVENT_UI_PROGRESS);
		DLProcess.Unregister(g_pThis, EVENT_DL_RESULT);
		DLProcess.Unregister(g_pThis, EVENT_RESET_UI);
		g_pThis->ModifyUnitStatus(nSlot, nPort, false);
	}

	return 0;
}
UINT CQ_DOWNLOADDlg::AfxRun6245(LPVOID pParam)
{
	int nSlot = *((int*)pParam);
	
	int nPort = 0;
	if(g_pThis->GetPortBySlot(nSlot, nPort))
	{
		g_pThis->ModifyUnitStatus(nSlot, nPort, true);
		CDLProcess DLProcess(nPort);
		
		DLProcess.Register(g_pThis, EVENT_UI_MESSAGE);
		DLProcess.Register(g_pThis, EVENT_UI_PROGRESS);
		DLProcess.Register(g_pThis, EVENT_DL_RESULT);
		DLProcess.Register(g_pThis, EVENT_RESET_UI);
		
		//////////////////////////////////////////////////////////////////////////
		//6245 Setting

		DLProcess.Set6245DLPath(g_pThis->m_DL6245Setting);

		DLProcess.SetRework(			g_pThis->m_SetupInfo.bREWORK_FLAG);
		DLProcess.SetCheckFactoryArea(  g_pThis->m_SetupInfo.bCheckFactoryArea);
		DLProcess.SetIMEICheck(			g_pThis->m_SetupInfo.nIMEI_CHECK);
		DLProcess.SetRestartTimeout(	g_pThis->m_SetupInfo.nRestartTimeout);

		DLProcess.SetSlotNumber(		nSlot);
		DLProcess.SetComputerName(		g_pThis->m_strCOMPUTER_NAME);
		DLProcess.SetSo(				g_pThis->m_SetupInfo.strSO);
		DLProcess.SetToolVersion(		g_pThis->m_SetupInfo.strTool_Version);
		DLProcess.SetConfigVersion(		g_pThis->m_SetupInfo.strConfig_Version);
		DLProcess.SetSWVersion(			g_pThis->m_SetupInfo.strSW_VERSION);
		DLProcess.SetHWVersion(			g_pThis->m_SetupInfo.strHW_VERSION);
		DLProcess.SetModelName(			g_pThis->m_SetupInfo.strMODEL_NAME);
		DLProcess.SetOnlyDLCEFS(		g_pThis->m_SetupInfo.nOnly_DL_Qvcefs_Flag);
		DLProcess.SetDLBootloader(		g_pThis->m_SetupInfo.nDL_Bootloader_Flag);
		DLProcess.SetBRT_BACKUP_PATH(   g_pThis->m_SetupInfo.strBRT_BACKUP_PATH);
		DLProcess.SetBackupBrt(			g_pThis->m_SetupInfo.bBackupBrt);
		DLProcess.SetCSV( g_pThis->m_SetupInfo.strCSV_PATH, g_pThis->m_SetupInfo.strCSV_TIME, g_pThis->m_SetupInfo.strCSV_SIZE);
		DLProcess.SetSaveLog(			g_pThis->m_SetupInfo.nSaveLog==0 ? false : true);
		DLProcess.SetIsLoopTest(		g_pThis->m_nCycTimes > 0);

		DLProcess.SetQVCEFS_File_Name(	g_pThis->m_SetupInfo.strQVCEFS_FILE_NAME);
		DLProcess.SetQVCEFS_File_Time(	g_pThis->m_SetupInfo.strQVCEFS_FILE_TIME);
		DLProcess.SetQVCEFS_File_Size(	g_pThis->m_SetupInfo.strQVCEFS_FILE_SIZE);
		//////////////////////////////////////////////////////////////////////////
		
		DLProcess.Run6245();

		DLProcess.Unregister(g_pThis, EVENT_UI_MESSAGE);
		DLProcess.Unregister(g_pThis, EVENT_UI_PROGRESS);
		DLProcess.Unregister(g_pThis, EVENT_DL_RESULT);
		DLProcess.Unregister(g_pThis, EVENT_RESET_UI);
		g_pThis->ModifyUnitStatus(nSlot, nPort, false);
	}
	
	return 0;
}

//UINT CQ_DOWNLOADDlg::AfxRunMotoProcess(LPVOID pParam)
//{
//	int nSlot = *((int*)pParam);
//	int nPort = 0;
//	if(g_pThis->GetPortBySlot(nSlot, nPort))
//	{
//		CDLProcess DLProcess(nPort);
//
//		DLProcess.Register(g_pThis, EVENT_UI_MESSAGE);
//		DLProcess.Register(g_pThis, EVENT_UI_PROGRESS);
//		DLProcess.Register(g_pThis, EVENT_DL_RESULT);
//
//		//////////////////////////////////////////////////////////////////////////
//		//Moto Setting
//		DLProcess.SetSuperFilePath(		g_pThis->m_DL6240Setting.szCefs);
//		DLProcess.SetRework(			g_pThis->m_SetupInfo.bREWORK_FLAG);
//		DLProcess.SetCheckFactoryArea(  g_pThis->m_SetupInfo.bCheckFactoryArea);
//		DLProcess.SetIMEICheck(			g_pThis->m_SetupInfo.nIMEI_CHECK);
//		DLProcess.SetRestartTimeout(	g_pThis->m_SetupInfo.nRestartTimeout);
//
//		DLProcess.SetSlotNumber(nSlot);
//		DLProcess.SetComputerName(		g_pThis->m_strCOMPUTER_NAME);
//		DLProcess.SetSo(				g_pThis->m_SetupInfo.strSO);
//		DLProcess.SetToolVersion(		g_pThis->m_SetupInfo.strTool_Version);
//		DLProcess.SetConfigVersion(		g_pThis->m_SetupInfo.strConfig_Version);
//		DLProcess.SetSWVersion(			g_pThis->m_SetupInfo.strSW_VERSION);
//		DLProcess.SetHWVersion(			g_pThis->m_SetupInfo.strHW_VERSION);
//		DLProcess.SetModelName(			g_pThis->m_SetupInfo.strMODEL_NAME);
//		DLProcess.SetOnlyDLCEFS(		g_pThis->m_SetupInfo.nOnly_DL_Qvcefs_Flag);
//		DLProcess.SetBRT_BACKUP_PATH(   g_pThis->m_SetupInfo.strBRT_BACKUP_PATH);
//		DLProcess.SetBackupBrt(			g_pThis->m_SetupInfo.bBackupBrt);
//		DLProcess.SetCSV( g_pThis->m_SetupInfo.strCSV_PATH, g_pThis->m_SetupInfo.strCSV_TIME, g_pThis->m_SetupInfo.strCSV_SIZE);
//		DLProcess.SetIsLoopTest(		g_pThis->m_nCycTimes > 0);
//
//		DLProcess.SetQVCEFS_File_Name(	g_pThis->m_SetupInfo.strQVCEFS_FILE_NAME);
//		DLProcess.SetQVCEFS_File_Time(	g_pThis->m_SetupInfo.strQVCEFS_FILE_TIME);
//		DLProcess.SetQVCEFS_File_Size(	g_pThis->m_SetupInfo.strQVCEFS_FILE_SIZE);
//
//		DLProcess.RunMotoProcess();
//
//		DLProcess.Unregister(g_pThis, EVENT_UI_MESSAGE);
//		DLProcess.Unregister(g_pThis, EVENT_UI_PROGRESS);
//		DLProcess.Unregister(g_pThis, EVENT_DL_RESULT);
//		g_pThis->ModifyUnitStatus(nSlot, nPort, false);
//	}
//	return 0;
//}

BOOL CQ_DOWNLOADDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch (pMsg->message)
	{
	case WM_KEYDOWN:
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return true;
			break;
		case VK_RETURN:
			OnButtonStart();
			return true;
			break;
		default:
			break;
		}
		default:
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CQ_DOWNLOADDlg::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}
void CQ_DOWNLOADDlg::AddTotalCount(bool bPASS, const char* szErrorCode)
{
	if(bPASS)
	{
		SetCount(m_nTotalNo+1, m_nPassNo+1, m_nFailNo);
	}
	else if(strcmp(szErrorCode, "M17") !=0 && strcmp(szErrorCode, "M18") !=0)
	{
		SetCount(m_nTotalNo+1, m_nPassNo, m_nFailNo+1);
	}

}
void CQ_DOWNLOADDlg::SetCount(int nTotal, int nPassNo, int nFailNo)
{
	CSingleLock singleLock(&m_SetCountCritSection,TRUE);
	m_nTotalNo = nTotal;
	m_nFailNo = nFailNo;
	m_nPassNo = nPassNo;
	
	char temp[25] ={0};
	sprintf(temp,"%u",m_nTotalNo);
	m_TOTAL.SetWindowText(temp);
	::WritePrivateProfileString("TEST_RECORD","TOTAL",temp,m_SetupInfo.strSETUP_INI_FILE_PATH);

	sprintf(temp,"%u",m_nPassNo);
	m_PASS.SetWindowText(temp);
	::WritePrivateProfileString("TEST_RECORD","PASS",temp,m_SetupInfo.strSETUP_INI_FILE_PATH);

	sprintf(temp,"%u",m_nFailNo);
	m_FAIL.SetWindowText(temp);
	::WritePrivateProfileString("TEST_RECORD","FAIL",temp,m_SetupInfo.strSETUP_INI_FILE_PATH);
	
}

bool CQ_DOWNLOADDlg::ModifyUnitStatus(int nSlot, int nPort, bool bRunning)
{
	bool bRes = false;
	CSingleLock singleLock(&m_ModifyUnitStatusCritSection,TRUE);
	for(int i=0; i<g_nSlotTotalCount; i++)
	{
		if(m_vRunningUnit[i].nSlot == nSlot && m_vRunningUnit[i].nPort == nPort)
		{
			m_vRunningUnit[i].bRunning = bRunning;
			bRes = true;
			break;
		}
	}
	return bRes;
}
bool CQ_DOWNLOADDlg::ISUnitProcessing(int nSlot, int nPort, bool& bRunning)
{
	bool bRes = false;
	for(int i=0; i<g_nSlotTotalCount; i++)
	{
		if(m_vRunningUnit[i].nSlot == nSlot && m_vRunningUnit[i].nPort == nPort)
		{
			bRunning = m_vRunningUnit[i].bRunning;
			bRes = true;
			break;
		}
	}
	return bRes;
}
bool CQ_DOWNLOADDlg::IsAnyUnitProcessing()
{
	bool bRes = false;
	for(int i=0; i<g_nSlotTotalCount; i++)
	{
		if(m_vRunningUnit[i].bRunning)
		{
			bRes = true;
			break;
		}
	}
	return bRes;
}
bool CQ_DOWNLOADDlg::SetStopRunningFlag(int nSlot, int nPort)
{
	bool bRes = false;
	CSingleLock singleLock(&m_DeleteUnitCritSection,TRUE);
	for(int i=0; i<g_nSlotTotalCount; i++)
	{
		if(m_vRunningUnit[i].nSlot == nSlot && m_vRunningUnit[i].nPort == nPort)
		{
			m_vRunningUnit[i].bRunning = false;
			bRes = true;
		}
	}
	return bRes;
}
bool CQ_DOWNLOADDlg::GetSlotByPort(int nPort, int& nSlot)
{
	bool bRes = false;
	for(int i=0; i<g_nSlotTotalCount; i++)
	{
		if(m_vRunningUnit[i].nPort == nPort)
		{
			nSlot = m_vRunningUnit[i].nSlot;
			bRes = true;
			break;
		}
	}
	return bRes;
}
bool CQ_DOWNLOADDlg::GetPortBySlot(int nSlot, int& nPort)
{
	bool bRes = false;
	for(int i=0; i<g_nSlotTotalCount; i++)
	{
		if(m_vRunningUnit[i].nSlot == nSlot)
		{
			nPort = m_vRunningUnit[i].nPort;
			bRes = true;
			break;
		}
	}
	return bRes;
}
void CQ_DOWNLOADDlg::SetProgress(int nSlot, int nMainProgress, int nSubProgress)
{
	m_SUBProgressCtrl[nSlot-1].SetPos(nSubProgress);
	if(nSubProgress==100)
	{
		m_MAINProgressCtrl[nSlot-1].SetPos(nMainProgress);
		char szTemp[10] = {0};
		sprintf(szTemp,"%d%%",nMainProgress);
		m_LabelPROGRESS[nSlot-1].SetWindowText(szTemp);
	}
}
void CQ_DOWNLOADDlg::AddMessage(const char* szMsg)
{
	CSingleLock singleLock(&m_AddMsgCritSection,TRUE);
	m_LISTBOX.InsertString(-1,szMsg);
	m_LISTBOX.SetTopIndex(m_LISTBOX.GetCount()-1);
}


void CQ_DOWNLOADDlg::OnButtonReset() 
{
	// TODO: Add your control notification handler code here
	CPASSWORD PassWordDlg;
	CStringA strSOPW;
	if(PassWordDlg.DoModal()==IDCANCEL)
	{
		return;
	}
	SetCount(0, 0, 0);
}

void CQ_DOWNLOADDlg::OnTest() 
{
	// TODO: Add your control notification handler code here
	//SetTimer(1, 1000, NULL);
	//if(m_tCycle==(m_nTestTime-1))
	//	KillTimer(1);
}

void CQ_DOWNLOADDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	//if(m_tCycle<m_nTestTime)
	//{
	//	if(m_nThreadCount==0)
	//	{
	//		m_tCycle++;
	//		OnButtonStart();		
	//	}	
	//}
	CDialog::OnTimer(nIDEvent);
}


void CQ_DOWNLOADDlg::OnConfig() 
{
	// TODO: Add your control notification handler code here	
	CPASSWORD pwd;
	CStringA strSOPW;
	if(pwd.DoModal()==IDCANCEL)
	{
		return;
	}
	CConfig cConfigDlg;
    if(cConfigDlg.DoModal(m_SetupInfo.strSETUP_INI_FILE_PATH) == IDOK)
	{
		m_SetupInfo.nOnly_DL_Qvcefs_Flag = ::GetPrivateProfileInt("Setting","Only_DL_Qvcefs_Flag",0,m_SetupInfo.strSETUP_INI_FILE_PATH);
		m_SetupInfo.nPowerOnTimeout = ::GetPrivateProfileInt("TimeoutSetting","PowerOnTimeout",27000,m_SetupInfo.strSETUP_INI_FILE_PATH);
		m_SetupInfo.nRestartTimeout = ::GetPrivateProfileInt("TimeoutSetting","RestartTimeout",27000,m_SetupInfo.strSETUP_INI_FILE_PATH);

	}
	return;
}


void CQ_DOWNLOADDlg::GetFileSizeTime(const char *FileName, char *Date, char *Size)
{
	if( strcmp(FileName,"N/A") == 0 )
	{
		sprintf(Date,"%s","N/A");
		sprintf(Size,"%s","N/A");
	}
	else
	{
		CFile        pFile;
		CFileStatus  fileStatus;
		CString      strTemp="";
		
		strTemp.Empty();
		pFile.GetStatus(FileName,fileStatus);    //get the time of sourcefile
		strTemp=fileStatus.m_mtime.Format("%Y/%m/%d %H:%M:%S");
		memcpy(Date,strTemp,20);	
		
		strTemp.Empty();
		strTemp.Format("%ld",(fileStatus.m_size+1023)/1024);
		
		if((strTemp.GetLength()>3)&&(strTemp.GetLength()<7))
			strTemp.Insert(strTemp.GetLength()-3,',');
		if(strTemp.GetLength()>=7)
		{
			strTemp.Insert(strTemp.GetLength()-3,',');
			strTemp.Insert(strTemp.GetLength()-6,',');
		}
		memcpy(Size,strTemp,20);
	}
}


void CQ_DOWNLOADDlg::FStrCpy(CString filename, char *filepath)
{
	if(filename=="N/A")
		strcpy(filepath,"");
	else
		strcpy(filepath, m_SetupInfo.strSW_Path + filename);
}

//bool CQ_DOWNLOADDlg::ThreadSafe_DownloadDefaultQCN(int ArrayNo, CQ_DOWNLOADDlg *pDownload)
//{
//	bool bReturn = false;
//	
//	char temp_log[512] = {0};
//		
//	sprintf(temp_log,"[COM%d]:  Normal Mode, Need to Download Default QCN %s......",g_nArrayComPort[ArrayNo],QCN_File);
//	pDownload->SetListboxSetLogString(temp_log, ArrayNo);
//	
//	bReturn = pRestoreBrt[ArrayNo](g_nArrayComPort[ArrayNo], QCN_File, false);
//	
//	if(!bReturn)
//	{
//		bReturn = pRestoreBrt[ArrayNo](g_nArrayComPort[ArrayNo], QCN_File, true);
//	}
//	
//	if(bReturn)
//	{
//		sprintf(temp_log,"[COM%d]:  Download Default QCN Success......",g_nArrayComPort[ArrayNo]);	
//	}
//	else
//	{
//		sprintf(temp_log,"[COM%d]:  Download Default QCN Fail......",g_nArrayComPort[ArrayNo]);
//		ErrorCode[ArrayNo]="M34";
//		
//	}
//	pDownload->ThreadSafe_SetListboxSetLogString(temp_log, ArrayNo);	
//	
//	pDownload->m_MAIN[ArrayNo].SetPos(90);
//	pDownload->m_SUB[ArrayNo].SetPos(100);
//	char temp[20] = {0};
//	sprintf(temp,"%02d%%",90);
//	pDownload->m_PROGRESS[ArrayNo].SetWindowText(temp);
//	
//	return bReturn;
//
//}

//bool CQ_DOWNLOADDlg::ThreadSafe_Signed_CheckVersion(int ArrayNo, CQ_DOWNLOADDlg* pDownload)
//{
//	bool bReturn = false;
//	
//	bReturn = pSigned_CheckVersion[ArrayNo](g_nArrayComPort[ArrayNo],AMSS_File, OEMSBL_File, bDownloadOEMSBL[ArrayNo],8000);
//
//	char temp_log[512] = {0};
//	if(bReturn)
//	{
//		sprintf(temp_log,"[COM%d]:  Signed_CheckVersion Success......",g_nArrayComPort[ArrayNo]);		
//	}
//	else
//	{
//		sprintf(temp_log,"[COM%d]:  Signed_CheckVersion Fail......",g_nArrayComPort[ArrayNo]);
//		ErrorCode[ArrayNo]="M38";
//	}
//
//	pDownload->ThreadSafe_SetListboxSetLogString(temp_log, ArrayNo);
//	
//	return bReturn;
//	
//}

void CQ_DOWNLOADDlg::UI_SetStart(BOOL bON)
{
	(CButton *)GetDlgItem(IDC_BUTTON_START)->EnableWindow(bON);
	(CButton *)GetDlgItem(IDC_BUTTON_RESET)->EnableWindow(bON);
	(CButton *)GetDlgItem(IDC_CONFIG)->EnableWindow(bON);
	(CButton *)GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(!bON);
	(CButton *)GetDlgItem(IDC_BUTTON_EXIT)->EnableWindow(bON);
	m_bStartON = bON == TRUE;
}
void CQ_DOWNLOADDlg::OnBnClickedBtnStop()
{
	(CButton *)GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(FALSE);
	m_bStop = true;
	if(!IsAnyUnitProcessing())
	{
		UI_SetStart(TRUE);
		m_bStop = false; // reset to default value
	}
}

void CQ_DOWNLOADDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	QSYNC_LIB::GetDeviceFinder()->Unregister(this);
	QSYNC_LIB::GetDeviceFinder()->StopMonitor();
	QSYNC_LIB::DeleteDeviceFinder();
	__super::OnClose();
}
