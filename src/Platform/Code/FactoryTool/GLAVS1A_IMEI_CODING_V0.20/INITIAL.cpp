// INITIAL.cpp : implementation file
//

#include "stdafx.h"
#include "QUALCOMM_IMEI_CODING.h"
#include "INITIAL.h"
#include "Global_Variable_Declare.h"
#include "PassWord.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CINITIAL dialog


CINITIAL::CINITIAL(CWnd* pParent /*=NULL*/)
	: CDialog(CINITIAL::IDD, pParent)
{
	//{{AFX_DATA_INIT(CINITIAL)
	//}}AFX_DATA_INIT
}


void CINITIAL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CINITIAL)
	DDX_Control(pDX, IDC_STATIC_SO, m_SO);
	DDX_Control(pDX, IDC_EDIT_SO, m_SO_EDIT);
	DDX_Control(pDX, IDC_COMBO_DUCK, m_DUCK);
	DDX_Control(pDX, IDC_STATIC_QZCS_FILE, m_QZCS_STATIC);
	DDX_Control(pDX, IDC_STATIC_QVCEFS_FILE, m_QVCEFS_STATIC);
	DDX_Control(pDX, IDC_STATIC_QSNV_FILE, m_QSNV_STATIC);
	DDX_Control(pDX, IDC_STATIC_PPF_FILE, m_PPF_FILE);
	DDX_Control(pDX, IDC_STATIC_DUCK_FILE, m_DUCK_STATIC);
	DDX_Control(pDX, IDC_COMBO_QZCS, m_QZCS);
	DDX_Control(pDX, IDC_COMBO_QVCEFS, m_QVCEFS);
	DDX_Control(pDX, IDC_COMBO_QSWINFO, m_QSWINFO);
	DDX_Control(pDX, IDC_COMBO_QSNV, m_QSNV);
	DDX_Control(pDX, IDC_COMBO_CSV, m_CSV);
	DDX_Control(pDX, IDC_STATIC_SECURITY_MECHANISM_FLAG, m_SECURITY_MECHANISM_FLAG);
	DDX_Control(pDX, IDC_CHECK_SECURITY_MECHANISM_FLAG, m_SECURITY_MECHANISM_FLAG_CHECK);
	DDX_Control(pDX, IDC_STATIC_MAL_CHECK, m_MAL_CHECK2);
	DDX_Control(pDX, IDC_CHECK_MAL, m_MAL_CHECK);
	DDX_Control(pDX, IDC_COMBO_TARGET_TYPE, m_TARGET_TYPE);
	DDX_Control(pDX, IDC_STATIC_TARGET_TYPE, m_TARGET_TYPE_STATIC);
	DDX_Control(pDX, IDC_STATIC_SW_HW_ME_PN, m_SW_HW_ME_PN);
	DDX_Control(pDX, IDC_STATIC_SW, m_SW_STATIC);
	DDX_Control(pDX, IDC_STATIC_QSWINFO_SIMDATA_CSV, m_QSWINFO_SIMDATA_CSV);
	DDX_Control(pDX, IDC_STATIC_QSWIINFO, m_QSWINFO_STATIC);
	DDX_Control(pDX, IDC_STATIC_PN, m_PN_STATIC);
	DDX_Control(pDX, IDC_STATIC_MODEL_NAME, m_MODEL_NAME_STATIC);
	DDX_Control(pDX, IDC_STATIC_MODEL_MODE, m_MODEL_MODE);
	DDX_Control(pDX, IDC_STATIC_MODE_NAME, m_MODE_NAME_STATIC);
	DDX_Control(pDX, IDC_STATIC_ME, m_ME_STATIC);
	DDX_Control(pDX, IDC_STATIC_HW, m_HW_STATIC);
	DDX_Control(pDX, IDC_STATIC_CSV, m_CSV_STATIC);
	DDX_Control(pDX, IDC_STATIC_BRT_QCN_CHECK, m_BRT_QCN_CHECK);
	DDX_Control(pDX, IDC_COMBO_SW, m_SW);
	DDX_Control(pDX, IDC_COMBO_PN, m_PN);
	DDX_Control(pDX, IDC_COMBO_MODEL_NAME, m_MODEL_NAME);
	DDX_Control(pDX, IDC_COMBO_MODE_NAME, m_MODE_NAME);
	DDX_Control(pDX, IDC_COMBO_ME, m_ME);
	DDX_Control(pDX, IDC_COMBO_HW, m_HW);
	DDX_Control(pDX, IDC_CHECK_QCN, m_QCN_CHECK);
	DDX_Control(pDX, IDC_CHECK_BRT, m_BRT_CHECK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CINITIAL, CDialog)
	//{{AFX_MSG_MAP(CINITIAL)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_BRT, OnCheckBrt)
	ON_BN_CLICKED(IDC_CHECK_QCN, OnCheckQcn)
	ON_CBN_DROPDOWN(IDC_COMBO_PN, OnDropdownComboPn)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnButtonOk)
	ON_CBN_SELCHANGE(IDC_COMBO_TARGET_TYPE, OnSelchangeComboTargetType)
	ON_BN_CLICKED(IDC_CHECK_SECURITY_MECHANISM_FLAG, OnCheckSecurityMechanismFlag)
	ON_CBN_SELCHANGE(IDC_COMBO_SW, OnSelchangeComboSw)
	ON_CBN_DROPDOWN(IDC_COMBO_QSNV, OnDropdownComboQsnv)
	ON_CBN_DROPDOWN(IDC_COMBO_QSWINFO, OnDropdownComboQswinfo)
	ON_CBN_DROPDOWN(IDC_COMBO_QVCEFS, OnDropdownComboQvcefs)
	ON_CBN_DROPDOWN(IDC_COMBO_QZCS, OnDropdownComboQzcs)
	ON_CBN_DROPDOWN(IDC_COMBO_CSV, OnDropdownComboCsv)
	ON_CBN_DROPDOWN(IDC_COMBO_DUCK, OnDropdownComboDuck)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL_NAME, OnSelchangeComboModelName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CINITIAL message handlers

void CINITIAL::Set_Ctrl_Property()
{
	int count=0;
	int cycle_count=0;
	BRT_QCN_FLAG=1;//back up BRT
	Slot_ID=None;
	m_brush.CreateSolidBrush(RGB(200,200,100));
	CSxLogFont Arial10b(90,FW_BOLD,false,"Arial");

	((CButton *)GetDlgItem(IDC_BUTTON_OK))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_OK)));
    ((CButton *)GetDlgItem(IDC_BUTTON_EXIT))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_EXIT)));
	//((CButton *)GetDlgItem(IDC_BUTTON_QSWINFO))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BROWSE)));
	//((CButton *)GetDlgItem(IDC_BUTTON_SIMDATA))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BROWSE)));
	//((CButton *)GetDlgItem(IDC_BUTTON_CSV))->SetBitmap(::LoadBitmap(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_BITMAP_BROWSE)));

	m_MODEL_MODE.SetTextColor(RGB(255,255,255));
	m_MODEL_MODE.SetBoxColors(RGB(0,0,255));
	m_MODEL_MODE.SetLineThickness(1,TRUE);
	m_MODEL_MODE.SetLineStyle(BS_RECT);
	m_MODEL_MODE.SetFont(&Arial10b);

	m_BRT_QCN_CHECK.SetTextColor(RGB(255,255,255));
	m_BRT_QCN_CHECK.SetBoxColors(RGB(0,0,255));
	m_BRT_QCN_CHECK.SetLineThickness(1,TRUE);
	m_BRT_QCN_CHECK.SetLineStyle(BS_RECT);
	m_BRT_QCN_CHECK.SetFont(&Arial10b);

	m_QSWINFO_SIMDATA_CSV.SetTextColor(RGB(255,255,255));
	m_QSWINFO_SIMDATA_CSV.SetBoxColors(RGB(0,0,255));
	m_QSWINFO_SIMDATA_CSV.SetLineThickness(1,TRUE);
	m_QSWINFO_SIMDATA_CSV.SetLineStyle(BS_RECT);
	m_QSWINFO_SIMDATA_CSV.SetFont(&Arial10b);

	m_PPF_FILE.SetTextColor(RGB(255,255,255));
	m_PPF_FILE.SetBoxColors(RGB(0,0,255));
	m_PPF_FILE.SetLineThickness(1,TRUE);
	m_PPF_FILE.SetLineStyle(BS_RECT);
	m_PPF_FILE.SetFont(&Arial10b);
	
	m_SW_HW_ME_PN.SetTextColor(RGB(255,255,255));
	m_SW_HW_ME_PN.SetBoxColors(RGB(0,0,255));
	m_SW_HW_ME_PN.SetLineThickness(1,TRUE);
	m_SW_HW_ME_PN.SetLineStyle(BS_RECT);
	m_SW_HW_ME_PN.SetFont(&Arial10b);

	m_MAL_CHECK2.SetTextColor(RGB(255,255,255));
	m_MAL_CHECK2.SetBoxColors(RGB(0,0,255));
	m_MAL_CHECK2.SetLineThickness(1,TRUE);
	m_MAL_CHECK2.SetLineStyle(BS_RECT);
	m_MAL_CHECK2.SetFont(&Arial10b);

	m_SECURITY_MECHANISM_FLAG.SetTextColor(RGB(255,255,255));
	m_SECURITY_MECHANISM_FLAG.SetBoxColors(RGB(0,0,255));
	m_SECURITY_MECHANISM_FLAG.SetLineThickness(1,TRUE);
	m_SECURITY_MECHANISM_FLAG.SetLineStyle(BS_RECT);
	m_SECURITY_MECHANISM_FLAG.SetFont(&Arial10b);
    //***************************SW/HW/ME/PN STATIC
	m_SW_STATIC.SetBkColor(RGB(200, 200, 100));
	m_SW_STATIC.SetTextColor(RGB(0, 0, 255));
    m_SW_STATIC.SetFontBold(TRUE);

	m_HW_STATIC.SetBkColor(RGB(200, 200, 100));
	m_HW_STATIC.SetTextColor(RGB(0, 0, 255));
    m_HW_STATIC.SetFontBold(TRUE);

	m_ME_STATIC.SetBkColor(RGB(200, 200, 100));
	m_ME_STATIC.SetTextColor(RGB(0, 0, 255));
    m_ME_STATIC.SetFontBold(TRUE);

	m_SO.SetBkColor(RGB(200, 200, 100));
	m_SO.SetTextColor(RGB(0, 0, 255));
    m_SO.SetFontBold(TRUE);

	CFont   font1;   
    font1.CreatePointFont(15*8,   "Arial");   
    m_SO_EDIT.SetFont(&font1,1);
	m_SO_EDIT.SetBkColor(RGB(255,255,255));
	m_SO_EDIT.SetTextColor(RGB(125,0,255));
	m_SO_EDIT.LimitText(5);

	m_PN_STATIC.SetBkColor(RGB(200, 200, 100));
	m_PN_STATIC.SetTextColor(RGB(0, 0, 255));
	m_PN_STATIC.SetFontBold(TRUE);
	//***************************MODE/MOMEL NAME STATIC
	m_MODE_NAME_STATIC.SetBkColor(RGB(200, 200, 100));
	m_MODE_NAME_STATIC.SetTextColor(RGB(0, 0, 255));
	m_MODE_NAME_STATIC.SetFontBold(TRUE);

	m_MODEL_NAME_STATIC.SetBkColor(RGB(200, 200, 100));
	m_MODEL_NAME_STATIC.SetTextColor(RGB(0, 0, 255));
	m_MODEL_NAME_STATIC.SetFontBold(TRUE);

	m_TARGET_TYPE_STATIC.SetBkColor(RGB(200, 200, 100));
	m_TARGET_TYPE_STATIC.SetTextColor(RGB(0, 0, 255));
	m_TARGET_TYPE_STATIC.SetFontBold(TRUE);
   
	//***************************QSWINFO/SIMDATA/CSV STATIC
	m_QSWINFO_STATIC.SetBkColor(RGB(200, 200, 100));
	m_QSWINFO_STATIC.SetTextColor(RGB(0, 0, 255));
	m_QSWINFO_STATIC.SetFontBold(TRUE);

	//m_QSWINFO2_STATIC.SetBkColor(RGB(255, 255, 255));
	//m_QSWINFO2_STATIC.SetTextColor(RGB(125, 0, 255));
	//m_QSWINFO2_STATIC.SetFontBold(TRUE);

	//m_SIMDATA_STATIC.SetBkColor(RGB(200, 200, 100));
	//m_SIMDATA_STATIC.SetTextColor(RGB(0, 0, 255));
	//m_SIMDATA_STATIC.SetFontBold(TRUE);

	//m_SIMDATA2_STATIC.SetBkColor(RGB(255, 255, 255));
	//m_SIMDATA2_STATIC.SetTextColor(RGB(125, 0, 255));
	//m_SIMDATA2_STATIC.SetFontBold(TRUE);

	m_CSV_STATIC.SetBkColor(RGB(200, 200, 100));
	m_CSV_STATIC.SetTextColor(RGB(0, 0, 255));
	m_CSV_STATIC.SetFontBold(TRUE);

	//m_CSV2_STATIC.SetBkColor(RGB(255, 255, 255));
	//m_CSV2_STATIC.SetTextColor(RGB(125, 0, 255));
	//m_CSV2_STATIC.SetFontBold(TRUE);

	m_QVCEFS_STATIC.SetBkColor(RGB(200, 200, 100));
	m_QVCEFS_STATIC.SetTextColor(RGB(0, 0, 255));
	m_QVCEFS_STATIC.SetFontBold(TRUE);

	m_QSNV_STATIC.SetBkColor(RGB(200, 200, 100));
	m_QSNV_STATIC.SetTextColor(RGB(0, 0, 255));
	m_QSNV_STATIC.SetFontBold(TRUE);

	m_QZCS_STATIC.SetBkColor(RGB(200, 200, 100));
	m_QZCS_STATIC.SetTextColor(RGB(0, 0, 255));
	m_QZCS_STATIC.SetFontBold(TRUE);

	m_DUCK_STATIC.SetBkColor(RGB(200, 200, 100));
	m_DUCK_STATIC.SetTextColor(RGB(0, 0, 255));
	m_DUCK_STATIC.SetFontBold(TRUE);

	if(!BRT_QCN_FLAG)
	{
		//m_CSV2_STATIC.SetWindowText("N/A");
		m_CSV_STATIC.EnableWindow(FALSE);
		//m_CSV2_STATIC.EnableWindow(FALSE);
		m_CSV.SetWindowText("N/A");
		m_CSV.EnableWindow(false);
		//((CButton *)GetDlgItem(IDC_BUTTON_CSV))->EnableWindow(FALSE);
	}

	if(!SECURITY_MECHANISM_FLAG)
	{
		m_QSWINFO_STATIC.EnableWindow(FALSE);
		m_QSWINFO.SetWindowText("N/A");
		m_QSWINFO.EnableWindow(false);
	}

	CFont   font;      
	font.CreateFont(
					   15,                        // nHeight
					   0,                         // nWidth
					   0,                         // nEscapement
					   0,                         // nOrientation
					   FW_DONTCARE,               // nWeight
					   FALSE,                     // bItalic
					   FALSE,                     // bUnderline
					   0,                         // cStrikeOut
					   ANSI_CHARSET,              // nCharSet
					   OUT_DEFAULT_PRECIS,        // nOutPrecision
					   CLIP_DEFAULT_PRECIS,       // nClipPrecision
					   DEFAULT_QUALITY,           // nQuality
					   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
					   "Arial");                  // lpszFacename


   font_BRT_QCN.CreateFont(15,							//   nHeight   
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

    m_BRT_CHECK.SetFont(&font_BRT_QCN,1);   

    m_QCN_CHECK.SetFont(&font_BRT_QCN,1);

	m_MAL_CHECK.SetFont(&font_BRT_QCN,1);

	m_SECURITY_MECHANISM_FLAG_CHECK.SetFont(&font_BRT_QCN,1);
    
	m_SECURITY_MECHANISM_FLAG_CHECK.SetCheck(SECURITY_MECHANISM_FLAG);


	m_BRT_CHECK.SetCheck(BRT_QCN_FLAG);

	m_QCN_CHECK.SetCheck(!BRT_QCN_FLAG);
	m_BRT_CHECK.EnableWindow(FALSE);
	m_QCN_CHECK.EnableWindow(FALSE);

	m_MAL_CHECK.SetCheck(0);
    //**********************COMBOBOX
	m_MODEL_NAME.SetTextColor(RGB(125,0,255));
	m_MODEL_NAME.SetFont(&font,TRUE);

	m_MODE_NAME.SetTextColor(RGB(125,0,255));
	m_MODE_NAME.SetFont(&font,TRUE);

	m_TARGET_TYPE.SetTextColor(RGB(125,0,255));
	m_TARGET_TYPE.SetFont(&font,TRUE);

	m_QSWINFO.SetTextColor(RGB(125,0,255));
	m_QSWINFO.SetFont(&font,TRUE);

	m_CSV.SetTextColor(RGB(125,0,255));
	m_CSV.SetFont(&font,TRUE);

	m_QVCEFS.SetTextColor(RGB(125,0,255));
	m_QVCEFS.SetFont(&font,TRUE);

	m_QSNV.SetTextColor(RGB(125,0,255));
	m_QSNV.SetFont(&font,TRUE);

	m_QZCS.SetTextColor(RGB(125,0,255));
	m_QZCS.SetFont(&font,TRUE);

	m_DUCK.SetTextColor(RGB(125,0,255));
	m_DUCK.SetFont(&font,TRUE);

	m_HW.SetTextColor(RGB(125,0,255));
	m_HW.SetFont(&font,TRUE);

	m_SW.SetTextColor(RGB(125,0,255));
	m_SW.SetFont(&font,TRUE);

	m_ME.SetTextColor(RGB(125,0,255));
	m_ME.SetFont(&font,TRUE);

	m_PN.SetTextColor(RGB(125,0,255));
	m_PN.SetFont(&font,TRUE);

	::GetCurrentDirectory(sizeof(Temp_Global),Temp_Global);
	Current_Directory=Temp_Global;
    CreateDirectory(Current_Directory+"\\SIM_FILE",NULL);
	strcat(Temp_Global,"\\setup.ini");
	Ini_File_Path=Temp_Global;

	::GetPrivateProfileString("TOOL_VERSION","TOOL_VERSION","",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
    Tool_Version=Temp_Global;

	TotalNo         =::GetPrivateProfileInt("TEST_RECORD","TOTAL",0,Ini_File_Path);
	PassNo          =::GetPrivateProfileInt("TEST_RECORD","PASS",0,Ini_File_Path);
	FailNo          =::GetPrivateProfileInt("TEST_RECORD","FAIL",0,Ini_File_Path);
	sprintf(Total_String,"TEST TOTAL:  %d",TotalNo);
	sprintf(Pass_String,"TEST PASS:  %d",PassNo);
	sprintf(Fail_String,"TEST FAIL:  %d",FailNo);

	//m_MODEL_NAME.InsertString(-1,"");
	count=::GetPrivateProfileInt("MODEL_NAME","MODEL_COUNT",0,Ini_File_Path);
	if(!count)
	{
		Message_String="There is no model name specified!\nPlease Check "+Ini_File_Path;
        Message_String+="\nSection:[MODEL_NAME]\nKeyname:MODEL_COUNT";
		::AfxMessageBox(Message_String);
		OnCancel();
	}
	else
	{
		for(cycle_count=1;cycle_count<=count;cycle_count++)
		{
			sprintf(Temp_Global,"MODEL_%d",cycle_count);
			::GetPrivateProfileString("MODEL_NAME",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
			m_MODEL_NAME.InsertString(-1,Temp_Global);
		}
		m_MODEL_NAME.SetCurSel(0);
	}
    
	//m_TARGET_TYPE.InsertString(-1,"");
	m_TARGET_TYPE.InsertString(-1,"PHONE");
	m_TARGET_TYPE.InsertString(-1,"MODULE");
    m_TARGET_TYPE.SetCurSel(0);

	//m_MODE_NAME.InsertString(-1,"");
	m_MODE_NAME.InsertString(-1,"NORMAL");
	m_MODE_NAME.InsertString(-1,"TRANSCEIVER");
    m_MODE_NAME.SetCurSel(0);

	//m_SW.InsertString(-1,"");
	count=::GetPrivateProfileInt("SOFTWARE_VERSION","VERSION_COUNT",0,Ini_File_Path);
	if(!count)
	{
		Message_String="There is no software version specified!\nPlease Check "+Ini_File_Path;
        Message_String+="\nSection:[SOFTWARE_VERSION]\nKeyname:VERSION_COUNT";
		::AfxMessageBox(Message_String);
		OnCancel();
	}
	else
	{
		for(cycle_count=1;cycle_count<=count;cycle_count++)
		{
			sprintf(Temp_Global,"VERSION_%d",cycle_count);
			::GetPrivateProfileString("SOFTWARE_VERSION",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
			m_SW.InsertString(-1,Temp_Global);
		}
		m_SW.SetCurSel(0);
	}

	//m_HW.InsertString(-1,"");
	count=::GetPrivateProfileInt("HARDWARE_VERSION","VERSION_COUNT",0,Ini_File_Path);
	if(!count)
	{
		Message_String="There is no hardware version specified!\nPlease Check "+Ini_File_Path;
        Message_String+="\nSection:[HARDWARE_VERSION]\nKeyname:VERSION_COUNT";
		::AfxMessageBox(Message_String);
		OnCancel();
	}
	else
	{
		for(cycle_count=1;cycle_count<=count;cycle_count++)
		{
			sprintf(Temp_Global,"VERSION_%d",cycle_count);
			::GetPrivateProfileString("HARDWARE_VERSION",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
			m_HW.InsertString(-1,Temp_Global);
		}
		m_HW.SetCurSel(0);
	}

	//m_ME.InsertString(-1,"");
	count=::GetPrivateProfileInt("ME_VERSION","VERSION_COUNT",0,Ini_File_Path);
	if(!count)
	{
		Message_String="There is no mechanical version specified!\nPlease Check "+Ini_File_Path;
        Message_String+="\nSection:[ME_VERSION]\nKeyname:VERSION_COUNT";
		::AfxMessageBox(Message_String);
		OnCancel();
	}
	else
	{
		for(cycle_count=1;cycle_count<=count;cycle_count++)
		{
			sprintf(Temp_Global,"VERSION_%d",cycle_count);
			::GetPrivateProfileString("ME_VERSION",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
			m_ME.InsertString(-1,Temp_Global);
		}
		m_ME.SetCurSel(0);
	}

	//SIM file
	count=::GetPrivateProfileInt("SIM_FILE_NAME","SIM_FILE_NAME_COUNT",0,Ini_File_Path);
	if(!count)
	{
		Message_String="There is no sim file name specified!\nPlease Check "+Ini_File_Path;
        Message_String+="\nSection:[SIM_FILE_NAME]\nKeyname:SIM_FILE_NAME_COUNT";
		::AfxMessageBox(Message_String);
		OnCancel();
	}
	else
	{
		for(cycle_count=1;cycle_count<=count;cycle_count++)
		{
			sprintf(Temp_Global,"SIM_FILE_NAME_%d",cycle_count);
			::GetPrivateProfileString("SIM_FILE_NAME",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
//			CStdioFile file;
//			if(file.Open(Current_Directory+"\\SIM_FILE\\"+Temp_Global,CFile::modeCreate))
//			{
//				file.Close();
//			}
			m_QSWINFO.InsertString(-1,Temp_Global);
		}
		m_QSWINFO.SetCurSel(0);
	}


	//m_PN.InsertString(-1,"");
	count=::GetPrivateProfileInt("PART_NUMBER","NUMBER_COUNT",0,Ini_File_Path);
	if(!count)
	{
		Message_String="There is no part number specified!\nPlease Check "+Ini_File_Path;
        Message_String+="\nSection:[PART_NUMBER]\nKeyname:NUMBER_COUNT";
		::AfxMessageBox(Message_String);
		OnCancel();
	}
	else
	{
		for(cycle_count=1;cycle_count<=count;cycle_count++)
		{
			sprintf(Temp_Global,"NUMBER_%d",cycle_count);
			::GetPrivateProfileString("PART_NUMBER",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
			m_PN.InsertString(-1,Temp_Global);
		}
		m_PN.SetCurSel(0);
	}
    
	::GetPrivateProfileString("SO_NUMBER","OLD","12345",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
	SO_Str_Old=Temp_Global;


	if(::GetPrivateProfileInt("BRT_BACKUP_SET","BRT2_BACKUP",1,Ini_File_Path) == 0)
	{
		m_CSV.EnableWindow(FALSE);
	}

	return;

}

HBRUSH CINITIAL::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	// TODO: Change any attributes of the DC here
	if(nCtlColor ==CTLCOLOR_DLG)
         return m_brush;
	if((pWnd->GetDlgCtrlID()==IDC_CHECK_QCN)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_BRT)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_MAL)||
	   (pWnd->GetDlgCtrlID()==IDC_CHECK_SECURITY_MECHANISM_FLAG))
	{
		//pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(0,0,255));
		pDC->SetBkColor(RGB(200,200,100)); 
		return m_brush;  
        //pDC->SetBkMode(TRANSPARENT); 
	}
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CINITIAL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	Set_Ctrl_Property();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

/*void CINITIAL::OnButtonQswinfo() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(TRUE,".qswinfo",NULL,OFN_PATHMUSTEXIST,"QSWINFO Files(*.qswinfo)|*.qswinfo",this);
	if(FileDlg.DoModal()==IDOK)
	{
		QSWINFO_File_Name=FileDlg.GetPathName();
		m_QSWINFO2_STATIC.SetWindowText(QSWINFO_File_Name);
		return;
	}
	
}

void CINITIAL::OnButtonSimdata() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(TRUE,".sim",NULL,OFN_PATHMUSTEXIST,"SIM DATA Files(*.sim)|*.sim",this);
	if(FileDlg.DoModal()==IDOK)
	{
		Sim_Data_File_Name_Path=FileDlg.GetPathName();
		Sim_Data_File_Name=FileDlg.GetFileName();
		m_SIMDATA2_STATIC.SetWindowText(Sim_Data_File_Name_Path);
		return;
	}
	
}

void CINITIAL::OnButtonCsv() 
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(TRUE,".csv",NULL,OFN_PATHMUSTEXIST,"CSV DATA Files(*.csv)|*.csv",this);
	if(FileDlg.DoModal()==IDOK)
	{
		CSV_File_Name=FileDlg.GetPathName();
		m_CSV2_STATIC.SetWindowText(CSV_File_Name);
		return;
	}
	return;	
}*/

void CINITIAL::OnCheckBrt() 
{
	// TODO: Add your control notification handler code here
	if(m_BRT_CHECK.GetCheck())//check
	{
		m_QCN_CHECK.SetCheck(0);
		BRT_QCN_FLAG=1;
		//m_CSV2_STATIC.SetWindowText("");
		m_CSV_STATIC.EnableWindow(TRUE);
		m_CSV.EnableWindow(TRUE);
		m_CSV.ResetContent();
		//m_CSV2_STATIC.EnableWindow(TRUE);
		//((CButton *)GetDlgItem(IDC_BUTTON_CSV))->EnableWindow(TRUE);
	}
	else
	{
		m_QCN_CHECK.SetCheck(1);
		BRT_QCN_FLAG=0;
		//m_CSV2_STATIC.SetWindowText("N/A");
		m_CSV_STATIC.EnableWindow(FALSE);
		m_CSV.ResetContent();
		m_CSV.SetWindowText("N/A");
		m_CSV.EnableWindow(FALSE);
		//m_CSV2_STATIC.EnableWindow(FALSE);
		//((CButton *)GetDlgItem(IDC_BUTTON_CSV))->EnableWindow(FALSE);
	}
	return;	
	
}

void CINITIAL::OnCheckQcn() 
{
	// TODO: Add your control notification handler code here
	if(m_QCN_CHECK.GetCheck())//check
	{
		m_BRT_CHECK.SetCheck(0);
		BRT_QCN_FLAG=0;
		//m_CSV2_STATIC.SetWindowText("N/A");
		m_CSV_STATIC.EnableWindow(FALSE);
		m_CSV.ResetContent();
		m_CSV.SetWindowText("N/A");
		m_CSV.EnableWindow(FALSE);
		//m_CSV2_STATIC.EnableWindow(FALSE);
		//((CButton *)GetDlgItem(IDC_BUTTON_CSV))->EnableWindow(FALSE);
	}
	else
	{
		m_BRT_CHECK.SetCheck(1);
		BRT_QCN_FLAG=1;
		//m_CSV2_STATIC.SetWindowText("");
		m_CSV_STATIC.EnableWindow(TRUE);
		m_CSV.EnableWindow(TRUE);
		m_CSV.ResetContent();
		//m_CSV2_STATIC.EnableWindow(TRUE);
		//((CButton *)GetDlgItem(IDC_BUTTON_CSV))->EnableWindow(TRUE);
	}
	return;			
}

void CINITIAL::OnDropdownComboPn() 
{
	// TODO: Add your control notification handler code here
	m_PN.ResetContent();
	int count=0;
	int cycle_count=0;
	count=::GetPrivateProfileInt("PART_NUMBER","NUMBER_COUNT",0,Ini_File_Path);
	if(!count)
	{
		Message_String="There is no part number specified!\nPlease Check "+Ini_File_Path;
		Message_String+="\nSection:[PART_NUMBER]\nKeyname:NUMBER_COUNT";
		::AfxMessageBox(Message_String);
		return;
	}
	else
	{
		for(cycle_count=1;cycle_count<=count;cycle_count++)
		{
			sprintf(Temp_Global,"NUMBER_%d",cycle_count);
			::GetPrivateProfileString("PART_NUMBER",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
			m_PN.InsertString(-1,Temp_Global);
		}
		m_PN.SetCurSel(0);
	}
	return;
	/*if(Target_Type=="PHONE")
	{
		CString string1="";
		CString string2="";
		CString string3=""; 
		CString PN="";
		::GetPrivateProfileString("SOFTWARE_PATH",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
		Load_Path=Temp_Global;
		if(Load_Path=="")
		{
			sprintf(Temp_Global,"There is no load path specified for %s!\nPlease Check %s\nSection:[SOFTWARE_PATH]\nKeyname:%s",Model_Name,Ini_File_Path,Model_Name);
			Message_String=Temp_Global;
			::AfxMessageBox(Message_String);
			return;
		}
		Load_Path+="\\V"+Software_Version;
		count=::GetPrivateProfileInt("PART_NUMBER_FOR_PHONE","NUMBER_COUNT",0,Ini_File_Path);
		if(!count)
		{
			Message_String="There is no part number specified!\nPlease Check "+Ini_File_Path;
			Message_String+="\nSection:[PART_NUMBER_FOR_PHONE]\nKeyname:NUMBER_COUNT";
			::AfxMessageBox(Message_String);
			return;
		}
		else
		{
			for(cycle_count=1;cycle_count<=count;cycle_count++)
			{
			 /*
			 ppf file name will be like:
			 PLATINUM_AB_NSL_136J1_126.qzcs
			 PLATINUM_AB_NSL_136J1_126.qvcefs
			 PLATINUM_AB_NSL_136J1_126.qsnv
			 */
				/*sprintf(Temp_Global,"NUMBER_%d",cycle_count);
				::GetPrivateProfileString("PART_NUMBER_FOR_PHONE",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
				PN=Temp_Global;
				int Flag=1;
				CFileFind FileFind_Ctrl;
				if(!FileFind_Ctrl.FindFile(Load_Path+"\\Flex_Option\\*.qzcs"))
				{
					sprintf(Temp_Global,"There is no any .qzcs/.qvcefs/.qsnv file in %s\\Flex_Option folder!!",Load_Path);
					Message_String=Temp_Global;
					::AfxMessageBox(Message_String);
					return;
				}

				while(Flag)
				{
					if(!FileFind_Ctrl.FindNextFile())//if 0, it is the last file
						Flag=0;
					QZCS_File_Name=FileFind_Ctrl.GetFilePath();//file full path
					string3=FileFind_Ctrl.GetFileName();//file name
					string1=string3.Mid(string3.Find(".qzcs")-3,3);//three numbers, such as "123"
					string2=string3.Mid(string3.Find('_',0)+1,2);//country code, 2 characters
					sprintf(Temp_Global,"%s%s.%s",PN,string2,string1);
					Part_Number=Temp_Global;
					m_PN.InsertString(-1,Part_Number);
				}
				FileFind_Ctrl.Close();
			}
			m_PN.SetCurSel(0);
		}
	}
	else//for module
	{
		count=::GetPrivateProfileInt("PART_NUMBER_FOR_MODULE","NUMBER_COUNT",0,Ini_File_Path);
		if(!count)
		{
			Message_String="There is no part number specified!\nPlease Check "+Ini_File_Path;
			Message_String+="\nSection:[PART_NUMBER_FOR_MODULE]\nKeyname:NUMBER_COUNT";
			::AfxMessageBox(Message_String);
			OnCancel();
		}
		else
		{
			for(cycle_count=1;cycle_count<=count;cycle_count++)
			{
				sprintf(Temp_Global,"NUMBER_%d",cycle_count);
				::GetPrivateProfileString("PART_NUMBER_FOR_MODULE",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
				m_PN.InsertString(-1,Temp_Global);
			}
			m_PN.SetCurSel(0);
		}

	}
	return;*/
	
}

void CINITIAL::OnButtonExit() 
{
	// TODO: Add your control notification handler code here
	OnCancel();
}

void CINITIAL::OnButtonOk() 
{
	// TODO: Add your control notification handler code here
	CString QVCEFS_FirstPart="";
	CString QSNV_FirstPart="";
	CString QZCS_FirstPart="";
	CString DUCK_FirstPart="";

	m_MODEL_NAME.GetWindowText(Model_Name);
	m_MODE_NAME.GetWindowText(Mode_Name);
	if(Mode_Name == _T("TRANSCEIVER"))
		::WritePrivateProfileString(_T("TRANCIVER_INFO"),_T("ONOFF"),_T("1"),Ini_File_Path);
	else
		::WritePrivateProfileString(_T("TRANCIVER_INFO"),_T("ONOFF"),_T("0"),Ini_File_Path);

	m_TARGET_TYPE.GetWindowText(Target_Type);

	m_SW.GetWindowText(Software_Version);
	m_HW.GetWindowText(Hardware_Version);
	m_PN.GetWindowText(Part_Number);
	if(Target_Type=="MODULE")
		Mechanical_Version="N/A";
	else
		m_ME.GetWindowText(Mechanical_Version);	

	if((Model_Name=="")||
	   (Mode_Name=="")||
	   (Target_Type=="")||
	   (Software_Version=="")||
	   (Hardware_Version=="")||
	   (Part_Number=="")||
	   (Mechanical_Version==""))
	{
		Message_String="Please select all the necessary items!!";
		::AfxMessageBox(Message_String);
		return;
	}

	SECURITY_MECHANISM_FLAG=m_SECURITY_MECHANISM_FLAG_CHECK.GetCheck();//if 1, write public key and SIM data; if 0, not write
	if(SECURITY_MECHANISM_FLAG)
	{
	   //m_QSWINFO2_STATIC.GetWindowText(QSWINFO_File_Name);
	   m_QSWINFO.GetWindowText(QSWINFO_File_Name);
	   if((QSWINFO_File_Name==""))
	   {
		   Message_String="You have select to write public key and sim data, so you must select QSWINFO file!!";
		   AfxMessageBox(Message_String);
		   return;
	   }
	   if((QSWINFO_File_Name=="N/A"))
	   {
		   Message_String="You have select to write public key and sim data, but the QSWINFO file does not exist!!";
		   AfxMessageBox(Message_String);
		   return;
	   }

	   //to find the sim file name
 /*    ::GetPrivateProfileString("SIM_FILE_NAME",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
	   Sim_Data_File_Name=Temp_Global;
	   if(Sim_Data_File_Name=="")
	   {
		    Message_String="There is no sim file specified!\nPlease Check "+Ini_File_Path;
			Message_String+="\nSection:[SIM_FILE_NAME]\nKeyname:"+Model_Name;
			::AfxMessageBox(Message_String);
			return;
	   }*///Cancel to check, sunlight 081203
	}
	else
	{
		QSWINFO_File_Name="N/A";
		Sim_Data_File_Name="N/A";
	}
	if(::GetPrivateProfileInt("BRT_BACKUP_SET","BRT2_BACKUP",1,Ini_File_Path) == 1)
	{

		if(BRT_QCN_FLAG)//back up brt file
		{
			//m_CSV2_STATIC.GetWindowText(CSV_File_Name);
			m_CSV.GetWindowText(CSV_File_Name);
			if(CSV_File_Name=="")
			{
				Message_String="You have select to backup BRT file, so you must select CSV file!!";
				AfxMessageBox(Message_String);
				return;
			}
			if(CSV_File_Name=="N/A")
			{
				Message_String="You have select to backup BRT file, but the CSV file does not exist!!";
				AfxMessageBox(Message_String);
				//			return;
			}

		}
		else
		{
			CSV_File_Name="N/A";
		}

	}
	else
	{
		CSV_File_Name="N/A";
	}

	if(Target_Type=="MODULE")//no ppf file
	{
		QSNV_File_Name="N/A";
		QVCEFS_File_Name="N/A";
		QZCS_File_Name="N/A";
		DUCK_File_Name="N/A";

		QSNV_File="N/A";
		QVCEFS_File="N/A";
		QZCS_File="N/A";
		DUCK_File="N/A";
	}
	else//for phone, there is ppf file
	{
		m_QVCEFS.GetWindowText(QVCEFS_File_Name);
//		m_QSNV.GetWindowText(QSNV_File_Name);
//		m_QZCS.GetWindowText(QZCS_File_Name);
//		m_DUCK.GetWindowText(DUCK_File_Name);
		if(QVCEFS_File_Name!="" && QVCEFS_File_Name!="N/A")
		{
			//QVCEFS_File:M7_NothAsia_CHT_01_0009.qvcefs
			//QVCEFS_FirstPart:M7_NothAsia_CHT_01_0009
			int index=0;
			index=QVCEFS_File_Name.Find("Flex_Option");
			index+=12;
            QVCEFS_File=QVCEFS_File_Name.Mid(index);
		}
		else
		{
			//Message_String="Please select all the necessary items!!";
			//AfxMessageBox(Message_String);
			//return;
		}

/*		if((DUCK_File_Name==""))
		{
			Message_String="Please select all the necessary items!!";
			AfxMessageBox(Message_String);
			return;
		}
		if((DUCK_File_Name=="N/A"))
		{
			DUCK_Flag=0;
			DUCK_File_Name="N/A";
			DUCK_File="N/A";
		}
		else
		{
			int index=0;
			index=DUCK_File_Name.Find("Flex_Option");
			index+=12;
            DUCK_File=DUCK_File_Name.Mid(index);
            
			index=DUCK_File.Find('.');
			DUCK_FirstPart=DUCK_File.Left(index);

			DUCK_Flag=1;
		}*/
	}

	if(m_MAL_CHECK.GetCheck())//checked
		Write_MAL_Flag=1;//write MAL
	else
        Write_MAL_Flag=0;

	if(Model_Name=="ATHB1A"
		|| Model_Name == "GLAVS1A" || Model_Name == "GLAVS2A" || Model_Name == "GLAVS4A"
		|| Model_Name == "GMELB1A" || Model_Name == "GMELB2A" || Model_Name == "GMELB3A"
		)
	{
		ATHB1A_Flag=1;
		char temp[100];
		char section[20];
		ZeroMemory(temp,sizeof(temp));
		ZeroMemory(section,sizeof(section));
		sprintf(section,"%s",Part_Number);
		if(!GetPrivateProfileString(section,"TRANCEIVER_NO","",temp,sizeof(temp),Ini_File_Path))//there is no transceiver NO defined for [section]
		{
			sprintf(temp,"There is no TRANCEIVER_NO defined for %s, please check!!",Part_Number);
			Message_String=temp;
			AfxMessageBox(Message_String);
			return;
		}
		else
			TRANS_NO=temp;
	}

	//SO
	m_SO_EDIT.GetWindowText(SO_Str);
//	if(SO_Str=="")
//	{
//		AfxMessageBox("Please input SO number!!");
////		return;
//	}
//
//	if(SO_Str.GetLength()!=5)
//	{
//		AfxMessageBox("The length of SO number is less than 5!!");
////		return;
//	}
//
//	if(SO_Str!=SO_Str_Old)
//	{
//		CPassWord PassWordDlg;
//		if(PassWordDlg.DoModal()==IDCANCEL)
//		{
////			return;
//		}
//	}
//	::WritePrivateProfileString("SO_NUMBER","OLD",SO_Str,Ini_File_Path);
	CDialog::OnOK();
}

void CINITIAL::OnSelchangeComboTargetType() 
{
	// TODO: Add your control notification handler code here
	//m_PN.ResetContent();
	CString Target_Type_temp="";
    m_TARGET_TYPE.GetWindowText(Target_Type_temp);
	int index=m_TARGET_TYPE.GetCurSel( );
	ZeroMemory(Temp_Global,sizeof(Temp_Global));
    m_TARGET_TYPE.GetLBText(index,Temp_Global);
    Target_Type=Temp_Global;
	if(Target_Type_temp==Target_Type)
		return;
	if(Target_Type=="MODULE")
	{
       m_ME_STATIC.EnableWindow(FALSE);
	   m_ME.SetWindowText("N/A");
	   m_ME.EnableWindow(FALSE);
       
	   m_QVCEFS_STATIC.EnableWindow(FALSE);
	   m_QSNV_STATIC.EnableWindow(FALSE);
	   m_QZCS_STATIC.EnableWindow(FALSE);
	   m_DUCK_STATIC.EnableWindow(FALSE);

	   m_QVCEFS.ResetContent();
	   m_QSNV.ResetContent();
	   m_QZCS.ResetContent();
	   m_DUCK.ResetContent();
  
	   m_QVCEFS.SetWindowText("N/A");
	   m_QSNV.SetWindowText("N/A");
	   m_QZCS.SetWindowText("N/A");
	   m_DUCK.SetWindowText("N/A");

	   m_QVCEFS.EnableWindow(FALSE);
	   m_QSNV.EnableWindow(FALSE);
	   m_QZCS.EnableWindow(FALSE);
	   m_DUCK.EnableWindow(FALSE);
	}
	else
	{
	   m_ME_STATIC.EnableWindow(TRUE);
	   m_ME.EnableWindow(TRUE);
	   m_ME.ResetContent();
       int count=0;
	   int cycle_count=0;
	   count=::GetPrivateProfileInt("ME_VERSION","VERSION_COUNT",0,Ini_File_Path);
	   if(!count)
	   {
		 Message_String="There is no mechanical version specified!\nPlease Check "+Ini_File_Path;
		 Message_String+="\nSection:[ME_VERSION]\nKeyname:VERSION_COUNT";
		 ::AfxMessageBox(Message_String);
		 OnCancel();
	   }
	   else
	   {
		 for(cycle_count=1;cycle_count<=count;cycle_count++)
		 {
			sprintf(Temp_Global,"VERSION_%d",cycle_count);
			::GetPrivateProfileString("ME_VERSION",Temp_Global,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
			m_ME.InsertString(-1,Temp_Global);
		 }
		 m_ME.SetCurSel(0);
	   }

	   m_QVCEFS_STATIC.EnableWindow(true);
	   m_QSNV_STATIC.EnableWindow(true);
	   m_QZCS_STATIC.EnableWindow(true);
	   m_DUCK_STATIC.EnableWindow(true);

	   m_QVCEFS.ResetContent();
	   m_QSNV.ResetContent();
	   m_QZCS.ResetContent();
	   m_DUCK.ResetContent();
  
	   m_QVCEFS.EnableWindow(true);
	   m_QSNV.EnableWindow(true);
	   m_QZCS.EnableWindow(true);
	   m_DUCK.EnableWindow(true);

	}
	return;
}

void CINITIAL::OnCheckSecurityMechanismFlag() 
{
	// TODO: Add your control notification handler code here
	if(m_SECURITY_MECHANISM_FLAG_CHECK.GetCheck())//check,write public key and SIMDATA
	{
		SECURITY_MECHANISM_FLAG=1;
		//m_QSWINFO2_STATIC.SetWindowText("");
		m_QSWINFO_STATIC.EnableWindow(TRUE);
		m_QSWINFO.EnableWindow(TRUE);
		m_QSWINFO.ResetContent();
		//m_QSWINFO2_STATIC.EnableWindow(TRUE);
		//((CButton *)GetDlgItem(IDC_BUTTON_QSWINFO))->EnableWindow(TRUE);

		//m_SIMDATA2_STATIC.SetWindowText("");
		//m_SIMDATA_STATIC.EnableWindow(TRUE);
		//m_SIMDATA2_STATIC.EnableWindow(TRUE);
		//((CButton *)GetDlgItem(IDC_BUTTON_SIMDATA))->EnableWindow(TRUE);
		
	}
	else//not check,not write public key and SIMDATA
	{
        SECURITY_MECHANISM_FLAG=0;
		//m_QSWINFO2_STATIC.SetWindowText("N/A");
		m_QSWINFO_STATIC.EnableWindow(FALSE);
		m_QSWINFO.ResetContent();
		m_QSWINFO.SetWindowText("N/A");
		m_QSWINFO.EnableWindow(FALSE);
		//m_QSWINFO2_STATIC.EnableWindow(FALSE);
		//((CButton *)GetDlgItem(IDC_BUTTON_QSWINFO))->EnableWindow(FALSE);

		//m_SIMDATA2_STATIC.SetWindowText("N/A");
		//m_SIMDATA_STATIC.EnableWindow(FALSE);
		//m_SIMDATA2_STATIC.EnableWindow(FALSE);
		//((CButton *)GetDlgItem(IDC_BUTTON_SIMDATA))->EnableWindow(FALSE);
	}
	return;
}

void CINITIAL::OnSelchangeComboSw() 
{
	// TODO: Add your control notification handler code here
	//m_PN.ResetContent();
	m_QVCEFS.ResetContent();
	m_QSNV.ResetContent();
	m_QZCS.ResetContent();
	m_DUCK.ResetContent();
	if(BRT_QCN_FLAG)
		m_CSV.ResetContent();
//	if(SECURITY_MECHANISM_FLAG)
//		m_QSWINFO.ResetContent();
	return;
}

void CINITIAL::OnDropdownComboQsnv() 
{
	// TODO: Add your control notification handler code here
	m_TARGET_TYPE.GetWindowText(Target_Type);
	if(Target_Type=="PHONE")//there is no any ppf file for module
	{
		m_QSNV.ResetContent();
		m_MODEL_NAME.GetWindowText(Model_Name);
		m_SW.GetWindowText(Software_Version);
		if((Model_Name=="")||(Software_Version==""))
		{
			Message_String="Please select model name\\software version first!!";
			AfxMessageBox(Message_String);
			return;
		}

		CString File_Name="";
		::GetPrivateProfileString("SOFTWARE_PATH",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
		Load_Path=Temp_Global;
		if(Load_Path=="")
		{
			sprintf(Temp_Global,"There is no load path specified for %s!\nPlease Check %s\nSection:[SOFTWARE_PATH]\nKeyname:%s",Model_Name,Ini_File_Path,Model_Name);
			Message_String=Temp_Global;
			::AfxMessageBox(Message_String);
			return;
		}
		Load_Path+="\\V"+Software_Version;

	    int Flag=1;
		CFileFind FileFind_Ctrl;
		if(!FileFind_Ctrl.FindFile(Load_Path+"\\Flex_Option\\*.qsnv"))
		{
			QSNV_File_Name="N/A";
			m_QSNV.InsertString(-1,QSNV_File_Name);
			m_QSNV.SetCurSel(0);
			return;
		}
		else
		{
			while(Flag)
			{
				if(!FileFind_Ctrl.FindNextFile())//if 0, it is the last file
					Flag=0;
				File_Name=FileFind_Ctrl.GetFilePath();//file full path
				m_QSNV.InsertString(-1,File_Name);
			}
			FileFind_Ctrl.Close();
		}
		m_QSNV.SetCurSel(0);
	}
	else
		return;
}

void CINITIAL::OnDropdownComboQswinfo() 
{
	// TODO: Add your control notification handler code here
/*	if(SECURITY_MECHANISM_FLAG)
	{
		m_QSWINFO.ResetContent();
		m_MODEL_NAME.GetWindowText(Model_Name);
		m_SW.GetWindowText(Software_Version);
		if((Model_Name=="")||(Software_Version==""))
		{
			Message_String="Please select model name\\software version first!!";
			AfxMessageBox(Message_String);
			return;
		}

		CString File_Name="";
		::GetPrivateProfileString("SOFTWARE_PATH",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
		Load_Path=Temp_Global;
		if(Load_Path=="")
		{
			sprintf(Temp_Global,"There is no load path specified for %s!\nPlease Check %s\nSection:[SOFTWARE_PATH]\nKeyname:%s",Model_Name,Ini_File_Path,Model_Name);
			Message_String=Temp_Global;
			::AfxMessageBox(Message_String);
			return;
		}
		Load_Path+="\\V"+Software_Version;

	    int Flag=1;
		CFileFind FileFind_Ctrl;
		if((!FileFind_Ctrl.FindFile(Load_Path+"\\*.qswinfo"))&&(!FileFind_Ctrl.FindFile(Load_Path+"\\*.QSWINFO")))
		{
			QSWINFO_File_Name="N/A";
			m_QSWINFO.InsertString(-1,QSWINFO_File_Name);
			m_QSWINFO.SetCurSel(0);
			return;
		}
		else
		{
			while(Flag)
			{
				if(!FileFind_Ctrl.FindNextFile())//if 0, it is the last file
					Flag=0;
				File_Name=FileFind_Ctrl.GetFilePath();//file full path
				m_QSWINFO.InsertString(-1,File_Name);
			}
			FileFind_Ctrl.Close();
		}
		m_QSWINFO.SetCurSel(0);
	}
	else*/
		return;
	
}

void CINITIAL::OnDropdownComboQvcefs() 
{
	// TODO: Add your control notification handler code here
	m_TARGET_TYPE.GetWindowText(Target_Type);
	if(Target_Type=="PHONE")//there is no any ppf file for module
	{
		m_QVCEFS.ResetContent();
		m_MODEL_NAME.GetWindowText(Model_Name);
		m_SW.GetWindowText(Software_Version);
		if((Model_Name=="")||(Software_Version==""))
		{
			Message_String="Please select model name\\software version first!!";
			AfxMessageBox(Message_String);
			return;
		}

		CString File_Name="";
		::GetPrivateProfileString("SOFTWARE_PATH",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
		Load_Path=Temp_Global;
		if(Load_Path=="")
		{
			sprintf(Temp_Global,"There is no load path specified for %s!\nPlease Check %s\nSection:[SOFTWARE_PATH]\nKeyname:%s",Model_Name,Ini_File_Path,Model_Name);
			Message_String=Temp_Global;
			::AfxMessageBox(Message_String);
			return;
		}
		Load_Path+="\\V"+Software_Version;

	    int Flag=1;
		CFileFind FileFind_Ctrl;
		if(!FileFind_Ctrl.FindFile(Load_Path+"\\Flex_Option\\*.qvcefs"))
		{
			QVCEFS_File_Name="N/A";
			m_QVCEFS.InsertString(-1,QVCEFS_File_Name);
			m_QVCEFS.SetCurSel(0);
			return;
		}
		else
		{
			while(Flag)
			{
				if(!FileFind_Ctrl.FindNextFile())//if 0, it is the last file
					Flag=0;
				File_Name=FileFind_Ctrl.GetFilePath();//file full path
				m_QVCEFS.InsertString(-1,File_Name);
			}
			FileFind_Ctrl.Close();
		}
		m_QVCEFS.SetCurSel(0);
	}
	else
		return;
	
}

void CINITIAL::OnDropdownComboQzcs() 
{
	// TODO: Add your control notification handler code here
	m_TARGET_TYPE.GetWindowText(Target_Type);
	if(Target_Type=="PHONE")//there is no any ppf file for module
	{
		m_QZCS.ResetContent();
		m_MODEL_NAME.GetWindowText(Model_Name);
		m_SW.GetWindowText(Software_Version);
		if((Model_Name=="")||(Software_Version==""))
		{
			Message_String="Please select model name\\software version first!!";
			AfxMessageBox(Message_String);
			return;
		}

		CString File_Name="";
		::GetPrivateProfileString("SOFTWARE_PATH",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
		Load_Path=Temp_Global;
		if(Load_Path=="")
		{
			sprintf(Temp_Global,"There is no load path specified for %s!\nPlease Check %s\nSection:[SOFTWARE_PATH]\nKeyname:%s",Model_Name,Ini_File_Path,Model_Name);
			Message_String=Temp_Global;
			::AfxMessageBox(Message_String);
			return;
		}
		Load_Path+="\\V"+Software_Version;

	    int Flag=1;
		CFileFind FileFind_Ctrl;
		if(!FileFind_Ctrl.FindFile(Load_Path+"\\Flex_Option\\*.qzcs"))
		{
			QZCS_File_Name="N/A";
			m_QZCS.InsertString(-1,QZCS_File_Name);
			m_QZCS.SetCurSel(0);
			return;
		}
		else
		{
			while(Flag)
			{
				if(!FileFind_Ctrl.FindNextFile())//if 0, it is the last file
					Flag=0;
				File_Name=FileFind_Ctrl.GetFilePath();//file full path
				m_QZCS.InsertString(-1,File_Name);
			}
			FileFind_Ctrl.Close();
		}
		m_QZCS.SetCurSel(0);
	}
	else
		return;
}

void CINITIAL::OnDropdownComboCsv() 
{
	// TODO: Add your control notification handler code here
	if(BRT_QCN_FLAG)//backup BRT
	{
		m_CSV.ResetContent();
		m_MODEL_NAME.GetWindowText(Model_Name);
		m_SW.GetWindowText(Software_Version);
		if((Model_Name=="")||(Software_Version==""))
		{
			Message_String="Please select model name\\software version first!!";
			AfxMessageBox(Message_String);
			return;
		}

		CString File_Name="";
		::GetPrivateProfileString("SOFTWARE_PATH",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
		Load_Path=Temp_Global;
		if(Load_Path=="")
		{
			sprintf(Temp_Global,"There is no load path specified for %s!\nPlease Check %s\nSection:[SOFTWARE_PATH]\nKeyname:%s",Model_Name,Ini_File_Path,Model_Name);
			Message_String=Temp_Global;
			::AfxMessageBox(Message_String);
			return;
		}
		Load_Path+="\\V"+Software_Version;

	    int Flag=1;
		CFileFind FileFind_Ctrl;
		if((!FileFind_Ctrl.FindFile(Load_Path+"\\*.csv"))&&(!FileFind_Ctrl.FindFile(Load_Path+"\\*.CSV")))
		{
			CSV_File_Name="N/A";
			m_CSV.InsertString(-1,CSV_File_Name);
			m_CSV.SetCurSel(0);
			return;
		}
		else
		{
			while(Flag)
			{
				if(!FileFind_Ctrl.FindNextFile())//if 0, it is the last file
					Flag=0;
				File_Name=FileFind_Ctrl.GetFilePath();//file full path
				m_CSV.InsertString(-1,File_Name);
			}
			FileFind_Ctrl.Close();
		}
		m_CSV.SetCurSel(0);
	}
	else
		return;
}

void CINITIAL::OnDropdownComboDuck() 
{
	// TODO: Add your control notification handler code here
	m_TARGET_TYPE.GetWindowText(Target_Type);
	if(Target_Type=="PHONE")//there is no any ppf file for module
	{
		m_DUCK.ResetContent();
		m_MODEL_NAME.GetWindowText(Model_Name);
		m_SW.GetWindowText(Software_Version);
		if((Model_Name=="")||(Software_Version==""))
		{
			Message_String="Please select model name\\software version first!!";
			AfxMessageBox(Message_String);
			return;
		}

		CString File_Name="";
		::GetPrivateProfileString("SOFTWARE_PATH",Model_Name,"",Temp_Global,sizeof(Temp_Global),Ini_File_Path);
		Load_Path=Temp_Global;
		if(Load_Path=="")
		{
			sprintf(Temp_Global,"There is no load path specified for %s!\nPlease Check %s\nSection:[SOFTWARE_PATH]\nKeyname:%s",Model_Name,Ini_File_Path,Model_Name);
			Message_String=Temp_Global;
			::AfxMessageBox(Message_String);
			return;
		}
		Load_Path+="\\V"+Software_Version;

	    int Flag=1;
		CFileFind FileFind_Ctrl;
		if(!FileFind_Ctrl.FindFile(Load_Path+"\\Flex_Option\\*.du"))
		{
			DUCK_File_Name="N/A";
			m_DUCK.InsertString(-1,DUCK_File_Name);
			m_DUCK.SetCurSel(0);
			return;
		}
		else
		{
			while(Flag)
			{
				if(!FileFind_Ctrl.FindNextFile())//if 0, it is the last file
					Flag=0;
				File_Name=FileFind_Ctrl.GetFilePath();//file full path
				m_DUCK.InsertString(-1,File_Name);
			}
			FileFind_Ctrl.Close();
		}
		m_DUCK.SetCurSel(0);
	}
	else
		return;
}

void CINITIAL::OnSelchangeComboModelName() 
{
	// TODO: Add your control notification handler code here
	m_QVCEFS.ResetContent();
	m_QSNV.ResetContent();
	m_QZCS.ResetContent();
	m_DUCK.ResetContent();
	if(BRT_QCN_FLAG)
		m_CSV.ResetContent();
//	if(SECURITY_MECHANISM_FLAG)
//		m_QSWINFO.ResetContent();
	return;
}
