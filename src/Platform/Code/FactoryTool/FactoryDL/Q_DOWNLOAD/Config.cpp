// Config.cpp : implementation file
//

#include "stdafx.h"
#include "q_download.h"
#include "Config.h"

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
	DDX_Control(pDX, IDC_DL_QVCEFS, m_DL_QVCEFS_Flag);
	DDX_Control(pDX, IDC_RESTART_TIMEOUT, m_Restart_Timeout);
	DDX_Control(pDX, IDC_WAITE_POWERON, m_Waite_PowerOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfig, CDialog)
	//{{AFX_MSG_MAP(CConfig)
	ON_BN_CLICKED(IDSAVE, OnSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfig message handlers

void CConfig::OnSave() 
{
	// TODO: Add your control notification handler code here

	CStringA szTemp;
	//cefs
	m_nOnly_DL_Qvcefs_Flag = m_DL_QVCEFS_Flag.GetCheck();
	
	szTemp.Format("%d", m_nOnly_DL_Qvcefs_Flag);
	
	::WritePrivateProfileString("Setting", "Only_DL_Qvcefs_Flag", szTemp, m_strINIPath.c_str());


	//PowerON timeout
	CString szTimeout = "";

	m_Waite_PowerOn.GetWindowText(szTimeout);

	m_nPowerOnTimeout = atoi(szTimeout);

	::WritePrivateProfileString("TimeoutSetting","PowerOnTimeout",szTimeout, m_strINIPath.c_str());
	
	//restart timeout
	szTimeout = "";
	
	m_Restart_Timeout.GetWindowText(szTimeout);
	
	m_nRestartTimeout = atoi(szTimeout);
	
	::WritePrivateProfileString("TimeoutSetting","RestartTimeout",szTimeout, m_strINIPath.c_str());

	
	OnOK();
}

BOOL CConfig::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_nOnly_DL_Qvcefs_Flag = ::GetPrivateProfileInt("Setting","Only_DL_Qvcefs_Flag",0,m_strINIPath.c_str());
	m_nPowerOnTimeout = ::GetPrivateProfileInt("TimeoutSetting","PowerOnTimeout",27000,m_strINIPath.c_str());
	m_nRestartTimeout = ::GetPrivateProfileInt("TimeoutSetting","RestartTimeout",27000,m_strINIPath.c_str());

	// TODO: Add extra initialization here
    m_DL_QVCEFS_Flag.SetCheck(m_nOnly_DL_Qvcefs_Flag);


	CString szTimeout = "";
	szTimeout.Format("%d",m_nPowerOnTimeout);
    m_Waite_PowerOn.SetWindowText(szTimeout);

	szTimeout = "";
	szTimeout.Format("%d",m_nRestartTimeout);
    m_Restart_Timeout.SetWindowText(szTimeout);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
