/////////////////////////////////////////////////////////////////////////////
//
//  Name        ConnStatsDlg.cpp
//  $Header:
//
//  Function    implementation of the ConnStatsDlg class.
//              
//
//  Date                         Modification
//  -----------------------------------------
//  04/12/2002    H Zong    Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueAudio.h"
#include "ConnStatsDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnStatsDlg dialog

//BOOL CConnStatsDlg::m_bActive = FALSE;
CConnStatsDlg *CConnStatsDlg::m_pDlg = NULL;


CConnStatsDlg::CConnStatsDlg(CBlueAudioDlg *p_dlg, CWnd* pParent /*=NULL*/)
	: CModelessDlg(CConnStatsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnStatsDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_p_dlg = p_dlg;	// points to master dialog so the rfconn methods can be called
}

CConnStatsDlg::~CConnStatsDlg()
{
	m_pDlg = NULL;
}

void CConnStatsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnStatsDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnStatsDlg, CDialog)
	//{{AFX_MSG_MAP(CConnStatsDlg)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnStatsDlg message handlers

void CConnStatsDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
//	CDialog::OnTimer(nIDEvent);

	// save local version of previous connection status
	m_previous_connection_status = m_conn_stats.bIsConnected ? TRUE : FALSE;
	m_p_dlg->GetConnectionStats(&m_conn_stats);

	//present current connection values to user
	DisplayCurrentStats();
}

BOOL CConnStatsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
    m_timer_id = SetTimer (1, 1000, NULL);

	m_previous_connection_status = FALSE;
	m_conn_stats.bIsConnected = 0;
	m_conn_stats.Rssi	   = 0;
	m_conn_stats.BytesSent = 0;
	m_conn_stats.BytesRcvd = 0;
	m_conn_stats.Duration  = 0;
	//present initial connection values to user
//	DisplayCurrentStats();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnStatsDlg::DisplayCurrentStats()
{
	CString msg;
	if (m_conn_stats.bIsConnected)
	    SetDlgItemText(IDC_STATUS_VALUE, "Connected");
	else
	    SetDlgItemText(IDC_STATUS_VALUE, "Not Connected");

	// don't update display of other stats after connection is closed
	// so user can see where they were at disconnect time
	if ((m_previous_connection_status == FALSE) && (m_conn_stats.bIsConnected == 0))
		return;
	msg.Format("%i", m_conn_stats.Rssi);
	SetDlgItemText(IDC_RSSI, msg);
	msg.Format("%i", m_conn_stats.BytesSent);
	SetDlgItemText(IDC_SENT, msg);
	msg.Format("%i", m_conn_stats.BytesRcvd);
	SetDlgItemText(IDC_RECEIVED, msg);

	// pretty friendly format for elapsed connection time
	UINT32 secs_within_day;
	UINT32 days;
	days = m_conn_stats.Duration / 86400;
	secs_within_day = m_conn_stats.Duration % 86400;
	CString days_string;

	if (days ==1)
		days_string.Format("1 day");
	else if (days > 1)
		days_string.Format("%d days", days);


	msg.Format("%s %02d:%02d:%02d", days_string, secs_within_day/3600,
		(secs_within_day%3600)/60, (secs_within_day%3600)%60);
	SetDlgItemText(IDC_DURATION, msg);


}

