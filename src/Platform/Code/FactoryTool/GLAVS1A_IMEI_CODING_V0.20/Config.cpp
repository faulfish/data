// Config.cpp : implementation file
//

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "Config.h"
#include "Global_Variable_Declare.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConfig dialog


CConfig::CConfig(CWnd* pParent /*=NULL*/)
	: CDialog(CConfig::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConfig)
	//}}AFX_DATA_INIT
}


void CConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfig)
	DDX_Control(pDX, IDC_EDIT_PICS, m_PICS_EDIT);
	DDX_Control(pDX, IDC_STATIC_PICS, m_PICS);
	DDX_Control(pDX, IDC_CHECK_PICS, m_PICS_CHECK);
	DDX_Control(pDX, IDC_STATIC_CHECK_STATION, m_CHECK_STATION);
	DDX_Control(pDX, IDC_CHECK_ACM_BOARD, m_ACM);
	DDX_Control(pDX, IDC_CHECK_FUNCTION_WIRELESS, m_FWT);
	DDX_Control(pDX, IDC_CHECK_LEAKAGE_CHARGER, m_LEAKAGE_CHARGER);
	DDX_Control(pDX, IDC_CHECK_FLEX_OPTION, m_FLEX_OPTION);
	DDX_Control(pDX, IDC_CHECK_CAL_PRE_TEST_3G, m_CAL_PRE_3G);
	DDX_Control(pDX, IDC_CHECK_CAL_PRE_TEST_2G, m_CAL_PRE_2G);
	DDX_Control(pDX, IDC_CHECK_BASE_BAND, m_BASE_BAND);
	DDX_Control(pDX, IDC_STATIC_DEBUG_INFOR, m_DEBUGINFO);
	DDX_Control(pDX, IDC_CHECK_DEBUGINFO, m_DEBUGINFO_CHECK);
	DDX_Control(pDX, IDC_STATIC_BACKUP, m_BACKUP);
	DDX_Control(pDX, IDC_STATIC_TIME_OUT, m_TIMEOUT);
	DDX_Control(pDX, IDC_STATIC_MS, m_MS);
	DDX_Control(pDX, IDC_STATIC_CYCLE_COUNT, m_CYCLECOUNT);
	DDX_Control(pDX, IDC_STATIC_CONNCET, m_CONNECT);
	DDX_Control(pDX, IDC_STATIC_CHECK, m_CHECK);
	DDX_Control(pDX, IDC_STATIC_BACKUP_PATH, m_BACKUPPATH);
	DDX_Control(pDX, IDC_EDIT_TIME_OUT, m_TIMEOUT_EDIT);
	DDX_Control(pDX, IDC_EDIT_CYCLE_COUNT, m_CYCLECOUNT_EDIT);
	DDX_Control(pDX, IDC_EDIT_BACKUP, m_BACKUP_EDIT);
	DDX_Control(pDX, IDC_CHECK_SW, m_SW_CHECK);
	DDX_Control(pDX, IDC_CHECK_PROCESS, m_PROCESS_CHECK);
	DDX_Control(pDX, IDC_CHECK_ME, m_ME_CHECK);
	DDX_Control(pDX, IDC_CHECK_HW, m_HW_CHECK);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_SAVE);
	DDX_Control(pDX, IDC_BUTTON_EXIT, m_EXIT);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfig, CDialog)
	//{{AFX_MSG_MAP(CConfig)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_CHECK_PROCESS, OnCheckProcess)
	ON_BN_CLICKED(IDC_CHECK_BASE_BAND, OnCheckBaseBand)
	ON_BN_CLICKED(IDC_CHECK_CAL_PRE_TEST_2G, OnCheckCalPreTest2g)
	ON_BN_CLICKED(IDC_CHECK_CAL_PRE_TEST_3G, OnCheckCalPreTest3g)
	ON_BN_CLICKED(IDC_CHECK_LEAKAGE_CHARGER, OnCheckLeakageCharger)
	ON_BN_CLICKED(IDC_CHECK_FUNCTION_WIRELESS, OnCheckFunctionWireless)
	ON_BN_CLICKED(IDC_CHECK_FLEX_OPTION, OnCheckFlexOption)
	ON_BN_CLICKED(IDC_CHECK_ACM_BOARD, OnCheckAcmBoard)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfig message handlers

HBRUSH CConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor ==CTLCOLOR_DLG)
        return m_brush;

	if((pWnd->GetDlgCtrlID()==IDC_CHECK_SW)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_HW)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_ME)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_PROCESS)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_DEBUGINFO)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_BASE_BAND)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_CAL_PRE_TEST_2G)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_CAL_PRE_TEST_3G)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_FLEX_OPTION)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_ACM_BOARD)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_LEAKAGE_CHARGER)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_FUNCTION_WIRELESS)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_PICS))
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkColor(RGB(200,200,100)); 
		return m_brush;  
        //pDC->SetBkMode(TRANSPARENT); 
	}

	if((pWnd->GetDlgCtrlID()==IDC_EDIT_CYCLE_COUNT)||
		(pWnd->GetDlgCtrlID()==IDC_EDIT_TIME_OUT)||
		(pWnd->GetDlgCtrlID()==IDC_EDIT_BACKUP)||
		(pWnd->GetDlgCtrlID()==IDC_EDIT_PICS))   
	{   
		pDC->SetBkColor(RGB(255,255,255));
		pDC->SetTextColor(RGB(125,0,255));   
	} 

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetCtrlProperty();

	SetCtrlText();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfig::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
	
}

void CConfig::OnButtonSave() 
{
	// TODO: Add your control notification handler code here
	ZeroMemory(Temp_Global,sizeof(Temp_Global));
	SW_CHECK=m_SW_CHECK.GetCheck();
	sprintf(Temp_Global,"%d",SW_CHECK);
	::WritePrivateProfileString("CHECK","SW_CHECK",Temp_Global,Ini_File_Path);

	HW_CHECK=m_HW_CHECK.GetCheck();
	sprintf(Temp_Global,"%d",HW_CHECK);
	::WritePrivateProfileString("CHECK","HW_CHECK",Temp_Global,Ini_File_Path);

	ME_CHECK=m_ME_CHECK.GetCheck();
	sprintf(Temp_Global,"%d",ME_CHECK);
	::WritePrivateProfileString("CHECK","ME_CHECK",Temp_Global,Ini_File_Path);

	PROCESS_CHECK=m_PROCESS_CHECK.GetCheck();
	sprintf(Temp_Global,"%d",PROCESS_CHECK);
	::WritePrivateProfileString("CHECK","PROCESS_CHECK",Temp_Global,Ini_File_Path);
    
	BASEBAND_TEST_CHECK_FLAG=m_BASE_BAND.GetCheck();
	CAL_PRETEST_2G_CHECK_FLAG=m_CAL_PRE_2G.GetCheck();
	CAL_PRETEST_3G_CHECK_FLAG=m_CAL_PRE_3G.GetCheck();
	LEAKAGE_CHARGER_CHECK_FLAG=m_LEAKAGE_CHARGER.GetCheck();
	FUNCTION_WIRELESS_TEST_CHECK_FLAG=m_FWT.GetCheck();
	PPF_CHECK_FLAG=m_FLEX_OPTION.GetCheck();
	ACM_BOARD_CHECK_FLAG=m_ACM.GetCheck();

	DEBUG_ONOFF_FLAG=m_DEBUGINFO_CHECK.GetCheck();
	sprintf(Temp_Global,"%d",DEBUG_ONOFF_FLAG);
	::WritePrivateProfileString("DEBUG_INFOR","ONOFF",Temp_Global,Ini_File_Path);
    
	SAVE_PICS_FILE_FLAG=m_PICS_CHECK.GetCheck();

	CString TEMP;
	m_TIMEOUT_EDIT.GetWindowText(TEMP);
    Connect_Server_Timeout=atoi(TEMP);
	::WritePrivateProfileString("CONNECT_SERVER","ConnectServerTimeout",TEMP,Ini_File_Path);

	m_CYCLECOUNT_EDIT.GetWindowText(TEMP);
    Cycle_Count=atoi(TEMP);
	::WritePrivateProfileString("CONNECT_SERVER","ConnectServerCycleCount",TEMP,Ini_File_Path);
	 
	//m_BACKUP_EDIT.GetWindowText(Backup_Path);
	//::WritePrivateProfileString("QCN_BACKUP_PATH","QCN_BACKUP_PATH",Backup_Path,Ini_File_Path);
	//CreateDirectory(Backup_Path,NULL); 
    //not to change the backup path
	
	m_PICS_EDIT.GetWindowText(PICSFile_Path);
	::WritePrivateProfileString("PICSFILE_PATH","PICSFILE_PATH",PICSFile_Path,Ini_File_Path);
	OnOK();
}

void CConfig::SetCtrlText() 
{
	ZeroMemory(Temp_Global,sizeof(Temp_Global));
	sprintf(Temp_Global,"%d",Connect_Server_Timeout);
	m_TIMEOUT_EDIT.SetWindowText(Temp_Global);

	sprintf(Temp_Global,"%d",Cycle_Count);
	m_CYCLECOUNT_EDIT.SetWindowText(Temp_Global);

	m_BACKUP_EDIT.SetWindowText(Backup_Path);
	m_PICS_EDIT.SetWindowText(PICSFile_Path);
	
	m_SW_CHECK.SetCheck(SW_CHECK);
    m_HW_CHECK.SetCheck(HW_CHECK);
	m_ME_CHECK.SetCheck(ME_CHECK);
	m_PROCESS_CHECK.SetCheck(PROCESS_CHECK);
	m_DEBUGINFO_CHECK.SetCheck(DEBUG_ONOFF_FLAG);
	m_PICS_CHECK.SetCheck(SAVE_PICS_FILE_FLAG);
    
	m_BASE_BAND.SetCheck(BASEBAND_TEST_CHECK_FLAG);
	m_CAL_PRE_2G.SetCheck(CAL_PRETEST_2G_CHECK_FLAG);
	m_CAL_PRE_3G.SetCheck(CAL_PRETEST_3G_CHECK_FLAG);
	m_LEAKAGE_CHARGER.SetCheck(LEAKAGE_CHARGER_CHECK_FLAG);
	m_FWT.SetCheck(FUNCTION_WIRELESS_TEST_CHECK_FLAG);
	m_FLEX_OPTION.SetCheck(PPF_CHECK_FLAG);
	m_ACM.SetCheck(ACM_BOARD_CHECK_FLAG);
	return;
}

void CConfig::SetCtrlProperty()
{
	m_brush.CreateSolidBrush(RGB(200,200,100));
	CSxLogFont Arial10b(90,FW_BOLD,false,"Arial");

	m_SAVE.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_SAVE)));
    m_EXIT.SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_EXIT)));
	
	m_CONNECT.SetTextColor(RGB(255,255,255));
	m_CONNECT.SetBoxColors(RGB(0,0,255));
	m_CONNECT.SetLineThickness(1,TRUE);
	m_CONNECT.SetLineStyle(BS_RECT);
	m_CONNECT.SetFont(&Arial10b);

	m_CHECK.SetTextColor(RGB(255,255,255));
	m_CHECK.SetBoxColors(RGB(0,0,255));
	m_CHECK.SetLineThickness(1,TRUE);
	m_CHECK.SetLineStyle(BS_RECT);
	m_CHECK.SetFont(&Arial10b);

	m_CHECK_STATION.SetTextColor(RGB(255,255,255));
	m_CHECK_STATION.SetBoxColors(RGB(0,0,255));
	m_CHECK_STATION.SetLineThickness(1,TRUE);
	m_CHECK_STATION.SetLineStyle(BS_RECT);
	m_CHECK_STATION.SetFont(&Arial10b);

	m_DEBUGINFO.SetTextColor(RGB(255,255,255));
	m_DEBUGINFO.SetBoxColors(RGB(0,0,255));
	m_DEBUGINFO.SetLineThickness(1,TRUE);
	m_DEBUGINFO.SetLineStyle(BS_RECT);
	m_DEBUGINFO.SetFont(&Arial10b);
	//********************************

	m_BACKUPPATH.SetTextColor(RGB(255,255,255));
	m_BACKUPPATH.SetBoxColors(RGB(0,0,255));
	m_BACKUPPATH.SetLineThickness(1,TRUE);
	m_BACKUPPATH.SetLineStyle(BS_RECT);
	m_BACKUPPATH.SetFont(&Arial10b);

	m_CYCLECOUNT.SetBkColor(RGB(200, 200, 100));
	m_CYCLECOUNT.SetTextColor(RGB(0, 0, 255));
    m_CYCLECOUNT.SetFontBold(TRUE);

	m_TIMEOUT.SetBkColor(RGB(200, 200, 100));
	m_TIMEOUT.SetTextColor(RGB(0, 0, 255));
    m_TIMEOUT.SetFontBold(TRUE);

	m_BACKUP.SetBkColor(RGB(200, 200, 100));
	m_BACKUP.SetTextColor(RGB(0, 0, 255));
    m_BACKUP.SetFontBold(TRUE);
	m_BACKUP.EnableWindow(false);//not to change backup path

	m_PICS.SetBkColor(RGB(200, 200, 100));
	m_PICS.SetTextColor(RGB(0, 0, 255));
    m_PICS.SetFontBold(TRUE);

	m_MS.SetBkColor(RGB(200, 200, 100));
	m_MS.SetTextColor(RGB(125,0,255));
	m_MS.SetFontSize(10);
    m_MS.SetFontBold(TRUE);

	font_check.CreateFont(15,							//   nHeight   
						   0,							//   nWidth   
						   0,							//   nEscapement   
						   0,							//   nOrientation   
						   FW_BOLD,						//   nWeight   
						   FALSE,                       //   bItalic   
						   FALSE,                       //   bUnderline   
						   0,                           //   cStrikeOut   
						   ANSI_CHARSET,                //   nCharSet   
						   OUT_DEFAULT_PRECIS,          //   nOutPrecision   
						   CLIP_DEFAULT_PRECIS,         //   nClipPrecision   
						   DEFAULT_QUALITY,             //   nQuality   
						   DEFAULT_PITCH   |   FF_SWISS,//   nPitchAndFamily   
						   _T("Arial"));                //   lpszFac 

    m_SW_CHECK.SetFont(&font_check,1);  
	m_HW_CHECK.SetFont(&font_check,1);  
	m_ME_CHECK.SetFont(&font_check,1);  
	m_PROCESS_CHECK.SetFont(&font_check,1); 
	m_DEBUGINFO_CHECK.SetFont(&font_check,1);
	m_PICS_CHECK.SetFont(&font_check,1);
	m_BASE_BAND.SetFont(&font_check,1); 
	m_CAL_PRE_2G.SetFont(&font_check,1); 
	m_CAL_PRE_3G.SetFont(&font_check,1); 
	m_LEAKAGE_CHARGER.SetFont(&font_check,1); 
	m_FWT.SetFont(&font_check,1); 
	m_FLEX_OPTION.SetFont(&font_check,1); 
	m_ACM.SetFont(&font_check,1); 
	
	CFont   font1;   
    font1.CreatePointFont(15*8,   "Arial");   
    m_CYCLECOUNT_EDIT.SetFont(&font1,1);
	m_TIMEOUT_EDIT.SetFont(&font1,1);
	m_BACKUP_EDIT.SetFont(&font1,1);
	m_BACKUP_EDIT.EnableWindow(false);//not to change backup path
	m_PICS_EDIT.SetFont(&font1,1);
	return;
}


void CConfig::OnCheckProcess() 
{
	// TODO: Add your control notification handler code here
	if(!m_PROCESS_CHECK.GetCheck())//not check
	{
		m_ACM.SetCheck(0);
		m_FWT.SetCheck(0);
		m_LEAKAGE_CHARGER.SetCheck(0);
		m_FLEX_OPTION.SetCheck(0);
		m_CAL_PRE_3G.SetCheck(0);
		m_CAL_PRE_2G.SetCheck(0);
		m_BASE_BAND.SetCheck(0);
	}
	return;
}

void CConfig::OnCheckBaseBand() 
{
	// TODO: Add your control notification handler code here
	if(m_BASE_BAND.GetCheck())
	{
		if(!m_PROCESS_CHECK.GetCheck())
		{
			AfxMessageBox("Please select to check process first!!");
			m_BASE_BAND.SetCheck(0);
		}
	}
	return;
}

void CConfig::OnCheckCalPreTest2g() 
{
	// TODO: Add your control notification handler code here
	if(m_CAL_PRE_2G.GetCheck())
	{
		if(!m_PROCESS_CHECK.GetCheck())
		{
			AfxMessageBox("Please select to check process first!!");
			m_CAL_PRE_2G.SetCheck(0);
		}
	}
	return;
}

void CConfig::OnCheckCalPreTest3g() 
{
	// TODO: Add your control notification handler code here
	if(m_CAL_PRE_3G.GetCheck())
	{
		if(!m_PROCESS_CHECK.GetCheck())
		{
			AfxMessageBox("Please select to check process first!!");
			m_CAL_PRE_3G.SetCheck(0);
		}
	}
	return;
}

void CConfig::OnCheckLeakageCharger() 
{
	// TODO: Add your control notification handler code here
	if(m_LEAKAGE_CHARGER.GetCheck())
	{
		if(!m_PROCESS_CHECK.GetCheck())
		{
			AfxMessageBox("Please select to check process first!!");
			m_LEAKAGE_CHARGER.SetCheck(0);
		}
	}
	return;
}

void CConfig::OnCheckFunctionWireless() 
{
	// TODO: Add your control notification handler code here
	if(m_FWT.GetCheck())
	{
		if(!m_PROCESS_CHECK.GetCheck())
		{
			AfxMessageBox("Please select to check process first!!");
			m_FWT.SetCheck(0);
		}
	}
	return;
}

void CConfig::OnCheckFlexOption() 
{
	// TODO: Add your control notification handler code here
	if(m_FLEX_OPTION.GetCheck())
	{
		if(!m_PROCESS_CHECK.GetCheck())
		{
			AfxMessageBox("Please select to check process first!!");
			m_FLEX_OPTION.SetCheck(0);
		}
	}
	return;
}

void CConfig::OnCheckAcmBoard() 
{
	// TODO: Add your control notification handler code here
	if(m_ACM.GetCheck())
	{
		if(!m_PROCESS_CHECK.GetCheck())
		{
			AfxMessageBox("Please select to check process first!!");
			m_ACM.SetCheck(0);
		}
	}
	return;
}
