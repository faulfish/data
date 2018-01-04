// ConfigBZ.cpp : implementation file
//

#include "stdafx.h"
#include "SPU_UI.h"
#include "ConfigBZ.h"


// CConfigBZ dialog

IMPLEMENT_DYNCREATE(CConfigBZ, CDialog)

CConfigBZ::CConfigBZ(CWnd* pParent /*=NULL*/)
	: CDialog(CConfigBZ::IDD, pParent)
{

}

CConfigBZ::~CConfigBZ()
{
}

void CConfigBZ::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LOG_DATA, m_LogData);
	DDX_Control(pDX, IDC_CHECK_CHECK_STATUS, m_Check_Status);
	DDX_Control(pDX, IDC_CHECK_PROFILELOADER, m_ProfileLoader);
	DDX_Control(pDX, IDC_EDIT_SITE, m_site);
	DDX_Control(pDX, IDC_EDIT_FACTORYID, m_FactoryId);
	DDX_Control(pDX, IDC_EDIT_BENCHID, m_BenchId);
	DDX_Control(pDX, IDC_CHECK_PRINTLABEL, m_PrintLabel);
	DDX_Control(pDX, IDC_EDIT_PASSWORD, m_Password);
}

BOOL CConfigBZ::OnInitDialog()
{
	CDialog::OnInitDialog();

	LoadConfigFile();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CConfigBZ, CDialog)
	ON_BN_CLICKED(IDC_CHECK_LOG_DATA, &CConfigBZ::OnBnClickedCheckLogData)
	ON_EN_CHANGE(IDC_EDIT_PASSWORD, &CConfigBZ::OnEnChangeEditPassword)
	ON_BN_CLICKED(IDOK, &CConfigBZ::OnBnClickedOk1)
END_MESSAGE_MAP()


// CConfigBZ message handlers

void CConfigBZ::OnBnClickedCheckLogData()
{
	// TODO: Add your control notification handler code here
}


void CConfigBZ::LoadConfigFile()
{
	CString str;
	CString strConfigFilePath = ".\\config.ini";
	CString strStationFilePath = ".\\station.ini";
	char strData[256];

	//Check if file exist ( config.ini )
	::GetPrivateProfileString("SETTINGS", "ISFILE", "NONE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();


 	::GetPrivateProfileString("SETTINGS", "CHECK_STATUS", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_Check_Status.SetCheck(0);
	else
		m_Check_Status.SetCheck(1);

	::GetPrivateProfileString("SETTINGS", "PRINT_LABEL", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_PrintLabel.SetCheck(0);
	else
		m_PrintLabel.SetCheck(1);
	
	::GetPrivateProfileString("SETTINGS", "LOG_DATA", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_LogData.SetCheck(0);
	else
		m_LogData.SetCheck(1);

	// Profile Loader
	::GetPrivateProfileString("SETTINGS", "PROFILE_LOADER", "ENABLE", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	if (str == "DISABLE")
		m_ProfileLoader.SetCheck(0);
	else
		m_ProfileLoader.SetCheck(1);

	// Site
	::GetPrivateProfileString("SETTINGS", "SITE", "BZ", strData, 20, strConfigFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();
	m_site.SetWindowTextA(str);
		


	//Check if file exist ( station.ini )
	::GetPrivateProfileString("SETTINGS", "ISFILE", "NONE", strData, 20, strStationFilePath);
	str.Format("%s", (LPCSTR)strData);
	str.TrimLeft(); str.TrimRight();

	if ( "NONE" == str)
	{
		::WritePrivateProfileString("SETTINGS", "ISFILE", "TRUE", strStationFilePath);		
		::WritePrivateProfileString("SETTINGS", "BENCHID", "XXXX", strStationFilePath);
		::WritePrivateProfileString("SETTINGS", "FACTORYID", "MFG01", strStationFilePath);
		
		// BenchId
		::GetPrivateProfileString("SETTINGS", "BENCHID", "XXXXX", strData, 20, strStationFilePath);
		str.Format("%s", (LPCSTR)strData);
		str.TrimLeft(); str.TrimRight();
		m_BenchId.SetWindowTextA(str);

		// Factory Id
		::GetPrivateProfileString("SETTINGS", "FACTORYID", "MFG01", strData, 20, strStationFilePath);
		str.Format("%s", (LPCSTR)strData);
		str.TrimLeft(); str.TrimRight();
		m_FactoryId.SetWindowTextA(str);
	}
	else
	{
		// BenchId
		::GetPrivateProfileString("SETTINGS", "BENCHID", "XXXXX", strData, 20, strStationFilePath);
		str.Format("%s", (LPCSTR)strData);
		str.TrimLeft(); str.TrimRight();
		m_BenchId.SetWindowTextA(str);

		// Factory Id
		::GetPrivateProfileString("SETTINGS", "FACTORYID", "MFG01", strData, 20, strStationFilePath);
		str.Format("%s", (LPCSTR)strData);
		str.TrimLeft(); str.TrimRight();
		m_FactoryId.SetWindowTextA(str);
	}
}
void CConfigBZ::OnEnChangeEditPassword()
{
	CString strPassword	=_T("");
	m_Password.GetWindowTextA(strPassword);
	if (strPassword == "QISDA")
	{
		m_LogData.EnableWindow(true);
		m_Check_Status.EnableWindow(true);
		m_ProfileLoader.EnableWindow(true);
		m_site.EnableWindow(true);
		m_FactoryId.EnableWindow(true);
		m_BenchId.EnableWindow(true);
		m_PrintLabel.EnableWindow(true);
	}
}



void CConfigBZ::OnBnClickedOk()
{
	//CString strConfigFilePath = ".\\config.ini";
	//CString strStationFilePath = ".\\station.ini";
	//CString str;

	//CString strCheckStatus		= "ENABLE";
	//CString strProfileLoader	= "ENABLE";
	//CString strLogDAta			= "ENABLE";
	//CString strPrintLabel		= "ENABLE";
	//CString strBenchId			= "ENABLE";
	//CString strFactoryID		= "ENABLE";
	//CString strSite				= "ENABLE";

	//if (!m_Check_Status.GetCheck())
	//	strCheckStatus = "DISABLE";
	//
	//if (!m_ProfileLoader.GetCheck())
	//	strProfileLoader = "DISABLE";

	//if (!m_LogData.GetCheck())
	//	strLogDAta = "DISABLE";

	//if (!m_PrintLabel.GetCheck())
	//	strPrintLabel = "DISABLE";

	//m_site.GetWindowTextA(strSite);
	//m_BenchId.GetWindowTextA(strBenchId);
	////m_PrintLabel.GetWindowTextA(strPrintLabel);
	//m_FactoryId.GetWindowTextA(strFactoryID);


	//::WritePrivateProfileString("SETTINGS", "BENCHID", strBenchId, strStationFilePath);	
	//::WritePrivateProfileString("SETTINGS", "FACTORYID", strFactoryID, strStationFilePath);

	//::WritePrivateProfileString("SETTINGS", "CHECK_STATUS", strCheckStatus, strConfigFilePath);	
	//::WritePrivateProfileString("SETTINGS", "PRINT_LABEL", strPrintLabel, strConfigFilePath);	
	//::WritePrivateProfileString("SETTINGS", "LOG_DATA", strLogDAta, strConfigFilePath);	
	//::WritePrivateProfileString("SETTINGS", "PROFILE_LOADER", strProfileLoader, strConfigFilePath);	
	//::WritePrivateProfileString("SETTINGS", "SITE", strSite, strConfigFilePath);	

	//OnOK();

}

void CConfigBZ::OnBnClickedOk1()
{
CString strConfigFilePath = ".\\config.ini";
	CString strStationFilePath = ".\\station.ini";
	CString str;

	CString strCheckStatus		= "ENABLE";
	CString strProfileLoader	= "ENABLE";
	CString strLogDAta			= "ENABLE";
	CString strPrintLabel		= "ENABLE";
	CString strBenchId			= "ENABLE";
	CString strFactoryID		= "ENABLE";
	CString strSite				= "ENABLE";

	if (!m_Check_Status.GetCheck())
		strCheckStatus = "DISABLE";
	
	if (!m_ProfileLoader.GetCheck())
		strProfileLoader = "DISABLE";

	if (!m_LogData.GetCheck())
		strLogDAta = "DISABLE";

	if (!m_PrintLabel.GetCheck())
		strPrintLabel = "DISABLE";

	m_site.GetWindowTextA(strSite);
	m_BenchId.GetWindowTextA(strBenchId);
	//m_PrintLabel.GetWindowTextA(strPrintLabel);
	m_FactoryId.GetWindowTextA(strFactoryID);


	::WritePrivateProfileString("SETTINGS", "BENCHID", strBenchId, strStationFilePath);	
	::WritePrivateProfileString("SETTINGS", "FACTORYID", strFactoryID, strStationFilePath);

	::WritePrivateProfileString("SETTINGS", "CHECK_STATUS", strCheckStatus, strConfigFilePath);	
	::WritePrivateProfileString("SETTINGS", "PRINT_LABEL", strPrintLabel, strConfigFilePath);	
	::WritePrivateProfileString("SETTINGS", "LOG_DATA", strLogDAta, strConfigFilePath);	
	::WritePrivateProfileString("SETTINGS", "PROFILE_LOADER", strProfileLoader, strConfigFilePath);	
	::WritePrivateProfileString("SETTINGS", "SITE", strSite, strConfigFilePath);	

	OnOK();
}
