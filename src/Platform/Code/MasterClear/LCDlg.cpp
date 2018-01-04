// LCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LC.h"
#include "LCDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	DEFAULT_CMD_TIME_OUT	8000	//default time out 

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

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
// CLCDlg dialog

CLCDlg::CLCDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLCDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLCDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLCDlg)
	DDX_Control(pDX, IDC_STATIC_PPF, m_ppf);
	DDX_Control(pDX, IDC_STATIC_IMEI, m_imei);
	DDX_Control(pDX, IDC_STATIC_BT, m_bt);
	DDX_Control(pDX, IDC_STATIC_FLAG, m_flag);
	DDX_Control(pDX, IDC_STATIC_SW, m_sw);
	DDX_Control(pDX, IDC_STATIC_HW, m_hw);
	DDX_Control(pDX, IDC_STATIC_SN, m_sn);
	DDX_Control(pDX, IDC_MESS, m_mess);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_STATIC_TITLE, m_title);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLCDlg, CDialog)
	//{{AFX_MSG_MAP(CLCDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLCDlg message handlers

BOOL CLCDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.
    if(IDOK != Initial.DoModal())
		exit(0);
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
	f_MeasureDelay=GetPrivateProfileInt("SETTING","Sleep",2000,".\\setup.ini");
	comport=GetPrivateProfileInt("SETTING","COMPORT",3,".\\SETUP.INI");

    CreateDirectory(".\\log",0);
	m_title.SetTextColor(RGB(0,0,128));
	m_title.SetFontSize(18);

	m_mess.SetTextColor(RGB(255,255,255));
	m_mess.SetBkColor(RGB(0,0,255));
	m_mess.SetFontSize(18);
	m_mess.SetFontBold(true);
	m_mess.SetText("Ready");

	m_sn.SetBkColor(RGB(255,255,255));
	m_sn.SetTextColor(RGB(0,0,255));
	m_sn.SetFontSize(15);

	m_sw.SetBkColor(RGB(255,255,255));
	m_sw.SetTextColor(RGB(0,0,255));
	m_sw.SetFontSize(15);
	m_sw.SetText(SW);

	m_hw.SetBkColor(RGB(255,255,255));
	m_hw.SetTextColor(RGB(0,0,255));
	m_hw.SetFontSize(15);
	m_hw.SetText(HW);

	m_imei.SetBkColor(RGB(255,255,255));
	m_imei.SetTextColor(RGB(0,0,255));
	m_imei.SetFontSize(15);

	m_bt.SetBkColor(RGB(255,255,255));
	m_bt.SetTextColor(RGB(0,0,255));
	m_bt.SetFontSize(15);

	m_ppf.SetBkColor(RGB(255,255,255));
	m_ppf.SetTextColor(RGB(0,0,255));
	m_ppf.SetFontSize(12);

	m_flag.SetBkColor(RGB(255,255,255));
	m_flag.SetTextColor(RGB(255,0,0));
	m_flag.SetFontBold(true);
	m_flag.SetFontSize(20);
	FQC=0;
	OQC=0;
	OOB=0;

	CString display="";
	if(::GetPrivateProfileInt("SETTING","FQC",0,".\\setup.ini"))
	{
//		m_flag.SetText("FQC");
		display=mode+" FQC";
	    FQC=1;
	}
	if(::GetPrivateProfileInt("SETTING","OQC",0,".\\setup.ini"))
	{
//		m_flag.SetText("OQC");
		display=mode+" OQC";
	    OQC=1;
	}
	if(::GetPrivateProfileInt("SETTING","OOB",0,".\\setup.ini"))
	{
//		m_flag.SetText("OOB");
		display=mode+" OOB";
	    OOB=1;
	}
	m_flag.SetText(display);

	if((!FQC) && (!OQC) && (!OOB))
	{ 
		display=mode+"Packing";
		m_flag.SetText(display);
	}

 
/*	hDll = ::LoadLibrary(_T("QisdaNVModule_StaticM.dll"));
	ASSERT(hDll);
    if(hDll == NULL) return false;
	pClearActivationData = (ClearActivationData *)::GetProcAddress(hDll, "CAD");
	pMasterClear = (MasterClear *)::GetProcAddress(hDll, "MC");
*/  //nancy delete on 20090324

	//hDll2 = ::LoadLibrary(_T("IMEIStation_StaticM.dll"));
	//ASSERT(hDll2);
 //   if(hDll2 == NULL) {
	//	AfxMessageBox("Load IMEIStation_StaticM.dll");
	//	CDialog::OnCancel();
	//	return false;
	//}

	//pReadIMEI = (ReadIMEI *)::GetProcAddress(hDll2, "ReadIMEI");
	//pReadFactoryArea = (ReadFactoryArea  *)::GetProcAddress(hDll2, "ReadFactoryArea");
 //   pWriteFactoryArea = (WriteFactoryArea  *)::GetProcAddress(hDll2, "WriteFactoryArea");
 //   pReadSWVersion = (ReadSWVersion  *)::GetProcAddress(hDll2, "ReadSWVersion");
	//pReadBTAddress = (ReadBTAddress  *)::GetProcAddress(hDll2, "ReadBTAddress");
	//pIsConnected = (IsConnected  *)::GetProcAddress(hDll2, "IsConnected");

	//MobileToolKitDll = ::LoadLibrary(_T("MobileToolKit_StaticM.dll"));
	//ASSERT(MobileToolKitDll);
 //   if(MobileToolKitDll == NULL) {
	//	AfxMessageBox("Load MobileToolKit_StaticM.dll");
	//	CDialog::OnCancel();
	//	return false;
	//}
	//pReadBatteryTemperature = (pFnReadBatteryTemperature )::GetProcAddress(MobileToolKitDll, "ReadBatteryTemperature");

	//pClearActivationData = (ClearActivationData_ForSPU *)::GetProcAddress(MobileToolKitDll, "ClearActivationData_ForSPU");
	//pMasterClear = (MasterClear_ForSPU *)::GetProcAddress(MobileToolKitDll, "MasterClear_ForSPU");

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CLCDlg::OnPaint() 
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
HCURSOR CLCDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLCDlg::OnOK() 
{
	// TODO: Add extra validation here
	m_mess.SetBkColor(RGB(0,0,255));
	m_mess.SetText("Test");
	bool bRes = false;
	int  nState = 0;
	int  i=0;
	int  nTemperature=0;
    unsigned char szFactoryArea[128]={0};
	char szIMEI[128]={0};
    char szBTadd[13]={0};
	char szReadValue1024[1024]={0};
	unsigned char ID_Flag=0;
	CString HWtemp,SWtemp;

	memset(temp,0,512);
	SN.Empty();
	IMEI.Empty();
	BT.Empty();
	PPF.Empty();
	errmsg.Empty();
	errcode.Empty();
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	m_sn.SetText("");
	m_imei.SetText("");
	m_bt.SetText("");
	m_ppf.SetText("");
   
	//if (pCNV == NULL)
	CNVItem*	pCNV = new CNVItem(comport);
	ASSERT(pCNV);
	if(pCNV == NULL) {
		AfxMessageBox("Load CNVItem fail");
		delete pCNV;
		return ;
	}

	//for(i=0;i<50;i++)
	{
		//Sleep(f_MeasureDelay);
		if(pCNV && ((CCFCDiagCommand*)pCNV)->IsConnected() == false)
		{
			AfxMessageBox("Phone is not connected.");
			GetDlgItem(IDOK)->EnableWindow(TRUE);
			delete pCNV;
			return;
		}
	}

    bRes = pCNV->ReadFactoryAreaNew(szFactoryArea, nState, DEFAULT_CMD_TIME_OUT); 
	ID_Flag=szFactoryArea[52];//byte 7 for ID Flag
	if(ID_Flag==0x00)
	{
		errcode="M30";
		errmsg="Read Factory info fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

	for(i=0;i<10;i++)
		temp[i]=szFactoryArea[94+i];
	SN=temp;
	SN=SN.Left(10);
	m_sn.SetText(SN);

	memset(temp,0,512);
	for(i=0;i<5;i++)
		temp[i]=szFactoryArea[89+i];
    HWtemp=temp;
	HWtemp=HWtemp.Left(5);

    if(SN==""||HWtemp=="")
	{
		errcode="M21";
		errmsg="SN or HW is empty";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

	if(strncmp(HWtemp,HW,5)!=0)
	{
		errcode="M57";
		errmsg="HW version is not right";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}
	
	szFactoryArea[124]=FQC;
	szFactoryArea[125]=OQC;
	szFactoryArea[126]=OOB;

	if((!FQC) && (!OQC) && (!OOB))
	{ 
		szFactoryArea[60]+=0x01;
	}
	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	bRes = pCNV->WriteNVItem(2497, ReadResp, szFactoryArea, 128);
	if(!bRes)
	{
		errcode="E31";
		errmsg="Write flag fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

	bRes = pCNV->ReadFactoryAreaNew(szFactoryArea, nState, DEFAULT_CMD_TIME_OUT); 
	if(!bRes)
	{
		errcode="M30";
		errmsg="Read PPF info fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

/*	for(i=0;i<70;i++)
		temp[i]=szFactoryArea[i];
	
	m_ppf.SetText(temp);*/
	m_ppf.SetText("N/A");

	bRes = ((CCFCDiagCommand*)pCNV)->ReadIMEI(szIMEI, DEFAULT_CMD_TIME_OUT);
	if(!bRes)
	{
	    errcode="M51";
		errmsg="Read IMEI fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}
    IMEI=szIMEI;
	IMEI=IMEI.Left(15);
	m_imei.SetText(IMEI);

	if((mode=="XCVR")&&(IMEI!="000000011234560"))
	{
	    errcode="M52";
		errmsg="XCVR IMEI must be 000000011234560";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

	if((mode!="XCVR")&&(IMEI=="000000011234560"))
	{
	    errcode="M53";
		errmsg="S-PACK IMEI couldn't be 000000011234560";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

	long nBTLength = 0;
	bRes = ((CCFCDiagCommand*)pCNV)->ReadBTAddr(szBTadd, nBTLength, DEFAULT_CMD_TIME_OUT);
	if(!bRes)
	{
		errcode="M76";
		errmsg="Read BT add fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

    BT=szBTadd;
    BT=BT.Left(12);
	m_bt.SetText(BT);

	long nLength;
    bRes = ((CCFCDiagCommand*)pCNV)->ReadSWVersion(szReadValue1024, nLength, DEFAULT_CMD_TIME_OUT);
	if(!bRes)
	{
	    errcode="M52";
		errmsg="Read SW version fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}
	SWtemp=szReadValue1024;
/*	if(atof(SW)>=1.10)  //after V1.10, the read out format as: VE538_1.02
	{
        SWtemp=SWtemp.Mid(6,4);
	}
	else //before V1.10, the read out format as: 1.02
	{
	    SWtemp=SWtemp.Mid(0,4);
	}
	if(strncmp(SW,SWtemp,4)!=0)
	{
	    errcode="M40";
        errmsg="SW version not right";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		return;
	}

	if(atof(SW)>=1.10)
	{
    m_mess.SetText("Read Battery Temperature");
	nState = pReadBatteryTemperature(comport,nTemperature);
	if(!nState)
	{
		errcode="E4";
		errmsg="Read Battery Temperature fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		return;
	}
    if((nTemperature+273)==253)
	{
		errcode="E41";
		errmsg.Format("Battery Temperature value %d", nTemperature);
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		return;
	}
	errmsg.Format("%d", nTemperature);
	}*/
	m_mess.SetText("Clear Activation Data");
	char szPort[5]={0};
	sprintf(szPort,"%d",comport);
	bRes = pCNV->ClearActivationData();
	if(!bRes)
	{
		errcode="E22";
		errmsg="Clear Activation fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	}

	if(::GetPrivateProfileInt("SETTING","MasterClear",0,".\\setup.ini"))
	{
	  m_mess.SetText("Master Clear");
	  bRes = pCNV->MasterClear();
	  if(!bRes)
	  {
		errcode="E23";
	    errmsg="Master clear fail";
		m_mess.SetText(errcode);
		m_mess.SetBkColor(RGB(255,0,0));
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		writeLog();
		delete pCNV;
		return;
	  }
	}

    errcode="PASS";
    m_mess.SetText(errcode);
	m_mess.SetBkColor(RGB(0,255,0));
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	writeLog();
	delete pCNV;
	return;
//	CDialog::OnOK();
}

void CLCDlg::writeLog()
{
	CString		LogfileName,str;
	CStdioFile	Logfile;
    CTime time = CTime::GetCurrentTime();

	LogfileName.Format(".\\log\\%s.LOG",time.Format("%Y%m%d"));	
	if (Logfile.Open(LogfileName,CFile::modeNoTruncate | CFile::modeCreate | CFile::modeReadWrite))
	{
//		char	cErrCode[20];
//		GetDlgItemText(IDC_Result + ID_Num,ErrorMessage,sizeof(ErrorMessage));
		Logfile.SeekToEnd();
		str.Format("%s,%s,%s\n",SN,errcode,errmsg);
		Logfile.WriteString(str);
		Logfile.Close();
		m_list.InsertString(-1,str);
	}
}

void CLCDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	//FreeLibrary(hDll);
	//FreeLibrary(hDll2);
	//FreeLibrary(MobileToolKitDll);
	CDialog::OnCancel();
}
