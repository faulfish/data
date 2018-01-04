// INITIAL.cpp : implementation file
//

#include "stdafx.h"
#include "Q_DOWNLOAD.h"
#include "INITIAL.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CINITIAL dialog
//CString CURRENT_DIRECTORY;
//CString SETUP_INI_FILE_PATH;
//
//CString SW_PATH;
//CString PARTITION_FILE_NAME;
//CString HEX_FILE_NAME;
//CString AMSS_FILE_NAME;
//CString AMSSH_FILE_NAME;
//CString OEMSBL_FILE_NAME;
//CString OEMSBLH_FILE_NAME;
//CString QCSBL_FILE_NAME;
//CString QCSBLH_FILE_NAME;
//CString BRT_BACKUP_PATH;
//CString DBL_FILE_NAME;
//CString FSBL_FILE_NAME;
//CString OSBL_FILE_NAME;
//CString QCN_FILE_NAME;
//
//
//bool g_bREWORK_FLAG;
//
//CString SW_VERSION;
//CString HW_VERSION;
//CString MODEL_NAME;
//CString Mode_Name;
//
//CString QVCEFS_FILE_NAME="";//file name


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
	DDX_Control(pDX, IDC_COMBO_QVCEFS_FILE, m_COMBO_QVCEFS);
	DDX_Control(pDX, IDC_COMBO_SW, m_COMBO_SW);
	DDX_Control(pDX, IDC_COMBO_QCSBLH, m_COMBO_QCSBLH);
	DDX_Control(pDX, IDC_COMBO_QCSBL, m_COMBO_QCSBL);
	DDX_Control(pDX, IDC_COMBO_PARTITION, m_COMBO_PARTITION);
	DDX_Control(pDX, IDC_COMBO_OEMSBLH, m_COMBO_OEMSBLH);
	DDX_Control(pDX, IDC_COMBO_OEMSBL, m_COMBO_OEMSBL);
	DDX_Control(pDX, IDC_COMBO_MODE, m_COMBO_MODE);
	DDX_Control(pDX, IDC_COMBO_MODEL, m_COMBO_MODEL);
	DDX_Control(pDX, IDC_COMBO_HW, m_COMBO_HW);
	DDX_Control(pDX, IDC_COMBO_HEX, m_COMBO_HEX);
	DDX_Control(pDX, IDC_COMBO_AMSSH, m_COMBO_AMSSH);
	DDX_Control(pDX, IDC_COMBO_AMSS, m_COMBO_AMSS);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CINITIAL, CDialog)
	//{{AFX_MSG_MAP(CINITIAL)
	ON_CBN_SELCHANGE(IDC_COMBO_MODE, OnSelchangeComboMode)
	ON_CBN_SELCHANGE(IDC_COMBO_MODEL, OnSelchangeComboModel)
	ON_CBN_SELCHANGE(IDC_COMBO_SW, OnSelchangeComboSw)
	ON_CBN_DROPDOWN(IDC_COMBO_QVCEFS_FILE, OnDropdownComboQvcefsFile)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CINITIAL message handlers

BOOL CINITIAL::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	HBITMAP BitMap_START=LoadBitmap(AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDB_BITMAP_START));
	((CButton *)GetDlgItem(IDOK))->SetBitmap(BitMap_START);
    
	HBITMAP BitMap_EXIT=LoadBitmap(AfxGetInstanceHandle( ),MAKEINTRESOURCE(IDB_BITMAP_EXIT));
	((CButton *)GetDlgItem(IDCANCEL))->SetBitmap(BitMap_EXIT);

	//IniFilePath
	char szTemp[512] = {0};
	::GetCurrentDirectory(sizeof(szTemp),szTemp);
	m_SetupInfo->strCurrentDirectory = szTemp;
	CStringA strIniPath = szTemp;
	strIniPath += "\\setup.ini";
	m_SetupInfo->strSETUP_INI_FILE_PATH = strIniPath;

	int model_count=::GetPrivateProfileInt("MODEL_NAME","MODEL_COUNT",0, strIniPath);
	for(int i=1;i<=model_count;i++)
	{
		char temp[512] = {0};
		sprintf(temp,"MODEL_%d",i);
        ::GetPrivateProfileString("MODEL_NAME",temp,"",temp,sizeof(temp),strIniPath);
        m_COMBO_MODEL.InsertString(-1,temp);
	}
	m_COMBO_MODEL.SetCurSel(0);
	m_COMBO_MODEL.GetWindowText(m_SetupInfo->strMODEL_NAME);

	if(m_SetupInfo->strMODEL_NAME == "")//there is no model selected
	{
		::AfxMessageBox("There Is No Model Name Specified In Setup.ini, Please Check!!");
		CDialog::OnCancel();
		return false;
	}
	else//there is a model selected
	{
		//sw version
		int software_version_count=::GetPrivateProfileInt("SOFTWARE_VERSION","SOFTWARE_VERSION_COUNT",1,strIniPath);
		CString software_version;
		for(int i=1;i<=software_version_count;i++)
		{
			char temp[512] = {0};
			sprintf(temp,"SOFTWARE_VERSION_%d",i);
			::GetPrivateProfileString("SOFTWARE_VERSION",temp,"0.00",temp,sizeof(temp),strIniPath);
			m_COMBO_SW.InsertString(-1,temp);
		}
		m_COMBO_SW.SetCurSel(0);
		m_COMBO_SW.GetWindowText(m_SetupInfo->strSW_VERSION);
	
		//hw version
		int hardware_version_count=::GetPrivateProfileInt("HARDWARE_VERSION","HARDWARE_VERSION_COUNT",1,strIniPath);
		CStringA hardware_version;
		for(int i=1;i<=hardware_version_count;i++)
		{
			char temp[512] = {0};
			sprintf(temp,"HARDWARE_VERSION_%d",i);
			::GetPrivateProfileString("HARDWARE_VERSION",temp,"101A1",temp,sizeof(temp),strIniPath);
			m_COMBO_HW.InsertString(-1,temp);
		}
		m_COMBO_HW.SetCurSel(0);
		
		//for Download Mode
		m_COMBO_MODE.InsertString(-1,"NORMAL");
        m_COMBO_MODE.InsertString(-1,"REWORK");
		m_COMBO_MODE.SetCurSel(0);


		FileFind("partition",IDC_COMBO_PARTITION);
		FileFind("amss",IDC_COMBO_AMSS);
		
		FileFind("oemsblhd",IDC_COMBO_OEMSBLH);

		if( m_SetupInfo->strMODEL_NAME.Find("LAVS") != -1)
		{
			FileFind("qcsbl",IDC_COMBO_QCSBL);
			FileFind("qcsblhd_cfgdata",IDC_COMBO_QCSBLH);
			FileFind("oemsbl",IDC_COMBO_OEMSBL);
			FileFind("amsshd",IDC_COMBO_AMSSH);
			FileFind("NPRG6260",IDC_COMBO_HEX);
		}

		if(m_SetupInfo->strMODEL_NAME.Find("MELB") != -1)
		{
			FileFind("osbl",IDC_COMBO_OEMSBL);
			FileFind("fsbl",IDC_COMBO_QCSBL);
			FileFind("dbl",IDC_COMBO_QCSBLH);
			FileFind("qcn",IDC_COMBO_AMSSH);
			FileFind("NPRG6270",IDC_COMBO_HEX);
		}
		
		char temp[512] ={0};
		::GetPrivateProfileString("SOFTWARE_PATH",m_SetupInfo->strMODEL_NAME,NULL,temp,sizeof(temp),strIniPath);
		m_SetupInfo->strSW_Path=temp;
        m_SetupInfo->strSW_Path+="\\V"+m_SetupInfo->strSW_VERSION;
		if(m_SetupInfo->strSW_Path.Right(1)!="\\")
			m_SetupInfo->strSW_Path+="\\";
		
	}
 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CINITIAL::OnSelchangeComboMode() 
{
	// TODO: Add your control notification handler code here
	char temp[512] = {0};
	
	int index=m_COMBO_MODE.GetCurSel( );
    m_COMBO_MODE.GetLBText(index,temp);
	//Mode_Name=temp;

	//if(Mode_Name=="REWORK")
	//{		
	//	
	//}
	//else
	//{
	//	
	//}


}

void CINITIAL::OnSelchangeComboModel() 
{
	// TODO: Add your control notification handler code here

 	m_COMBO_SW.SetCurSel(0);
 	m_COMBO_SW.SetWindowText("");

	m_COMBO_AMSS.EnableWindow(TRUE);
	m_COMBO_AMSS.ResetContent();

	m_COMBO_AMSSH.EnableWindow(TRUE);
	m_COMBO_AMSSH.ResetContent();

	m_COMBO_OEMSBL.EnableWindow(TRUE);
	m_COMBO_OEMSBL.ResetContent();

	m_COMBO_OEMSBLH.EnableWindow(TRUE);
	m_COMBO_OEMSBLH.ResetContent();

	m_COMBO_PARTITION.EnableWindow(TRUE);
	m_COMBO_PARTITION.ResetContent();


	m_COMBO_QCSBL.EnableWindow(TRUE);
	m_COMBO_QCSBL.ResetContent();

	m_COMBO_QCSBLH.EnableWindow(TRUE);
	m_COMBO_QCSBLH.ResetContent();

	m_COMBO_HEX.EnableWindow(TRUE);
	m_COMBO_HEX.ResetContent();

	m_COMBO_QVCEFS.EnableWindow(TRUE);
	m_COMBO_QVCEFS.ResetContent();


	return;
}

void CINITIAL::OnSelchangeComboSw() 
{
	// TODO: Add your control notification handler code here
 
	m_COMBO_AMSS.EnableWindow(TRUE);
	m_COMBO_AMSS.ResetContent();
	
	m_COMBO_AMSSH.EnableWindow(TRUE);
	m_COMBO_AMSSH.ResetContent();
	
	m_COMBO_OEMSBL.EnableWindow(TRUE);
	m_COMBO_OEMSBL.ResetContent();
	
	m_COMBO_OEMSBLH.EnableWindow(TRUE);
	m_COMBO_OEMSBLH.ResetContent();
	
	m_COMBO_PARTITION.EnableWindow(TRUE);
	m_COMBO_PARTITION.ResetContent();
	
	m_COMBO_QCSBL.EnableWindow(TRUE);
	m_COMBO_QCSBL.ResetContent();
	
	m_COMBO_QCSBLH.EnableWindow(TRUE);
	m_COMBO_QCSBLH.ResetContent();

	m_COMBO_HEX.EnableWindow(TRUE);
	m_COMBO_HEX.ResetContent();

	m_COMBO_QVCEFS.EnableWindow(TRUE);
	m_COMBO_QVCEFS.ResetContent();

	char temp[512] = {0};
	
	int index=m_COMBO_MODE.GetCurSel( );
    m_COMBO_MODE.GetLBText(index,temp);
	m_SetupInfo->strMode_Name=temp;
	
	m_COMBO_MODEL.GetWindowText(m_SetupInfo->strMODEL_NAME);


	index=m_COMBO_SW.GetCurSel();
    m_COMBO_SW.GetLBText(index,temp);
	m_SetupInfo->strSW_VERSION=temp;

	FileFind("partition",IDC_COMBO_PARTITION);
	FileFind("amss",IDC_COMBO_AMSS);
	FileFind("oemsblhd",IDC_COMBO_OEMSBLH);
	
	
	if( m_SetupInfo->strMODEL_NAME.Find("LAVS") != -1)
	{
		FileFind("NPRG6260",IDC_COMBO_HEX);
		FileFind("qcsbl",IDC_COMBO_QCSBL);
		FileFind("qcsblhd_cfgdata",IDC_COMBO_QCSBLH);
		FileFind("oemsbl",IDC_COMBO_OEMSBL);
		FileFind("amsshd",IDC_COMBO_AMSSH);
	}
	
	if(m_SetupInfo->strMODEL_NAME.Find("MELB") != -1)
	{
		FileFind("NPRG6270",IDC_COMBO_HEX);
		FileFind("fsbl",IDC_COMBO_QCSBL);
		FileFind("dbl",IDC_COMBO_QCSBLH);
		FileFind("osbl",IDC_COMBO_OEMSBL);
		FileFind("qcn",IDC_COMBO_AMSSH);
	}

 
	return;
}

void CINITIAL::OnOK() 
{
	// TODO: Add extra validation here
	char temp[512] = {0};

	m_COMBO_MODEL.GetWindowText(m_SetupInfo->strMODEL_NAME);

	m_SetupInfo->strBRT_BACKUP_PATH="C:\\"+m_SetupInfo->strMODEL_NAME+"_BRT_BACKUP";

	m_COMBO_MODE.GetWindowText(temp,sizeof(temp));
	m_SetupInfo->bREWORK_FLAG=(!strcmp(temp,"REWORK"))?1:0;//0 for normal,1 for rework
	
	m_COMBO_SW.GetWindowText(m_SetupInfo->strSW_VERSION);
	m_COMBO_HW.GetWindowText(m_SetupInfo->strHW_VERSION);
	m_COMBO_QVCEFS.GetWindowText(m_SetupInfo->strQVCEFS_FILE_NAME);
	m_COMBO_PARTITION.GetWindowText(m_SetupInfo->strPARTITION_FILE_NAME);
	m_COMBO_HEX.GetWindowText(m_SetupInfo->strHEX_FILE_NAME);
	m_COMBO_AMSS.GetWindowText(m_SetupInfo->strAMSS_FILE_NAME); 
	
	m_COMBO_OEMSBLH.GetWindowText(m_SetupInfo->strOEMSBLH_FILE_NAME);  

	if( m_SetupInfo->strMODEL_NAME.Find("LAVS") != -1)
	{
		m_COMBO_QCSBL.GetWindowText(m_SetupInfo->strQCSBL_FILE_NAME);
		m_COMBO_QCSBLH.GetWindowText(m_SetupInfo->strQCSBLH_FILE_NAME);
    	m_COMBO_OEMSBL.GetWindowText(m_SetupInfo->strOEMSBL_FILE_NAME);
		m_COMBO_AMSSH.GetWindowText(m_SetupInfo->strAMSSH_FILE_NAME);
	}

	if( m_SetupInfo->strMODEL_NAME.Find("MELB") != -1)
	{
		m_COMBO_QCSBL.GetWindowText(m_SetupInfo->strFSBL_FILE_NAME);
		m_COMBO_QCSBLH.GetWindowText(m_SetupInfo->strDBL_FILE_NAME);
		m_COMBO_OEMSBL.GetWindowText(m_SetupInfo->strOSBL_FILE_NAME);
		m_COMBO_AMSSH.GetWindowText(m_SetupInfo->strQCN_FILE_NAME);
	}
	

	if( m_SetupInfo->strMODEL_NAME.Find("LAVS") != -1)
	{
		if((m_SetupInfo->strPARTITION_FILE_NAME == "N/A")
			||(m_SetupInfo->strHEX_FILE_NAME == "N/A")
			||(m_SetupInfo->strQCSBL_FILE_NAME == "N/A")
			||(m_SetupInfo->strQCSBLH_FILE_NAME == "N/A")
			||(m_SetupInfo->strOEMSBL_FILE_NAME == "N/A")
			||(m_SetupInfo->strOEMSBLH_FILE_NAME == "N/A")
			||(m_SetupInfo->strAMSS_FILE_NAME == "N/A")
			||(m_SetupInfo->strAMSSH_FILE_NAME == "N/A")
			||(m_SetupInfo->strQVCEFS_FILE_NAME == "N/A"))
		{
			::AfxMessageBox("Please select all Items first!!");
			return;
		}
	}

	if( m_SetupInfo->strMODEL_NAME.Find("MELB") != -1)
	{
		if((   m_SetupInfo->strPARTITION_FILE_NAME == "N/A")
			||(m_SetupInfo->strHEX_FILE_NAME == "N/A")
			||(m_SetupInfo->strFSBL_FILE_NAME == "N/A")
			||(m_SetupInfo->strDBL_FILE_NAME == "N/A")
			||(m_SetupInfo->strOSBL_FILE_NAME == "N/A")
			||(m_SetupInfo->strAMSS_FILE_NAME == "N/A")
		//	||(m_SetupInfo->strQCN_FILE_NAME == "N/A")
			||(m_SetupInfo->strQVCEFS_FILE_NAME == "N/A"))
		{
			::AfxMessageBox("Please select all Items first!!");
			return;
		}
	}
	if(m_SetupInfo->strQVCEFS_FILE_NAME == "")
	{
		::AfxMessageBox("Please select Qvcefs File first!!");
		return;
	}
	
	if((m_SetupInfo->strHW_VERSION=="")||(m_SetupInfo->strSW_VERSION==""))
	{
		::AfxMessageBox("Please Open Setup.ini To Check HW Version Or SW Version!!");
		return;
	}

	CDialog::OnOK();
}

void CINITIAL::FileFind(CString szFile, int iCombo)
{
	char szTemp[100] = {0};

	int index=m_COMBO_SW.GetCurSel( );
    m_COMBO_SW.GetLBText(index,szTemp);
	m_SetupInfo->strSW_VERSION=szTemp;

	m_COMBO_MODEL.GetWindowText(m_SetupInfo->strMODEL_NAME);
	
	::GetPrivateProfileString("SOFTWARE_PATH",m_SetupInfo->strMODEL_NAME,NULL,szTemp,100,m_SetupInfo->strSETUP_INI_FILE_PATH);

	m_SetupInfo->strSW_Path=szTemp;

	m_SetupInfo->strSW_Path+="\\V"+m_SetupInfo->strSW_VERSION;
	if(m_SetupInfo->strSW_Path.Right(1)!="\\")
		m_SetupInfo->strSW_Path+="\\";
	
	m_SetupInfo->nWriteQFuseFlag = ::GetPrivateProfileInt("WRITE_QFUSE","WriteQFuseFlag",0,m_SetupInfo->strSETUP_INI_FILE_PATH);
	CString MidName;
	if(m_SetupInfo->nWriteQFuseFlag == 1)
		MidName = "_" + m_SetupInfo->strMODEL_NAME + "_SIGNED" + "_SWV0" + m_SetupInfo->strSW_VERSION.Left(1) + m_SetupInfo->strSW_VERSION.Right(2);
	else
		MidName = "_" + m_SetupInfo->strMODEL_NAME + "_UNSIGNED" + "_SWV0" + m_SetupInfo->strSW_VERSION.Left(1) + m_SetupInfo->strSW_VERSION.Right(2);
	
	
	CFileFind file_find;
	
	BOOL bFind = FALSE;
	
	if(1057 != iCombo)
		bFind = file_find.FindFile(m_SetupInfo->strSW_Path + szFile + MidName + "*.mbn");
	
    if(1057 == iCombo)
		bFind = file_find.FindFile(m_SetupInfo->strSW_Path + szFile + MidName + "*.hex");

	if((1063 == iCombo) && (m_SetupInfo->strMODEL_NAME.Find("MELB")!=-1))
		bFind = file_find.FindFile(m_SetupInfo->strSW_Path + "*.brt");

	if(bFind)
	{
		file_find.FindNextFile();
		((CComboBox *)GetDlgItem(iCombo))->InsertString(-1,file_find.GetFileName());
		((CComboBox *)GetDlgItem(iCombo))->SetCurSel(0);
	}
	else
	{
		((CComboBox *)GetDlgItem(iCombo))->InsertString(-1,"N/A");
		((CComboBox *)GetDlgItem(iCombo))->SetCurSel(0);
		((CComboBox *)GetDlgItem(iCombo))->EnableWindow(FALSE);
	}
}

void CINITIAL::OnDropdownComboQvcefsFile() 
{
	m_COMBO_QVCEFS.ResetContent();
	
	CString szTemp = "";
	
	if(1 == m_SetupInfo->nWriteQFuseFlag)
	{
		szTemp = m_SetupInfo->strSW_Path + "Flex_Option\\" + m_SetupInfo->strMODEL_NAME + "*_SIGNED" + "_SWV0" + m_SetupInfo->strSW_VERSION.Left(1) + m_SetupInfo->strSW_VERSION.Right(2) + "_*.qvcefs";
	}
	
	if(0 == m_SetupInfo->nWriteQFuseFlag)
	{
		szTemp = m_SetupInfo->strSW_Path + "Flex_Option\\" + m_SetupInfo->strMODEL_NAME + "*_UNSIGNED" + "_SWV0" + m_SetupInfo->strSW_VERSION.Left(1) + m_SetupInfo->strSW_VERSION.Right(2) + "_*.qvcefs";
	}
	
	CString szFilePath = "";
	
	CFileFind file_find;
	
	BOOL Binfind = false;
	
	Binfind = file_find.FindFile(szTemp);
	
	if(Binfind)//found
	{   
		while(Binfind)
		{
			Binfind = file_find.FindNextFile();
			szFilePath=file_find.GetFileName();
			m_COMBO_QVCEFS.InsertString(-1,szFilePath);
			m_COMBO_QVCEFS.SetCurSel(0);
			szFilePath = "";
		}	
	}
	else//not found
	{
		m_COMBO_QVCEFS.InsertString(-1,"N/A");
		m_COMBO_QVCEFS.SetCurSel(0);
		m_COMBO_QVCEFS.EnableWindow(FALSE);
	}
}


