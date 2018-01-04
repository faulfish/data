// MonitorUPNPDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MonitorUPNP.h"
#include "MonitorUPNPDlg.h"
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMonitorUPNPDlg::CMonitorUPNPDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorUPNPDlg::IDD, pParent)
{
}

void CMonitorUPNPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOGLIST, m_listLog);
}

BEGIN_MESSAGE_MAP(CMonitorUPNPDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_LBN_SELCHANGE(IDC_LOGLIST, &CMonitorUPNPDlg::OnLbnSelchangeLoglist)
END_MESSAGE_MAP()


BOOL CMonitorUPNPDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	using namespace QSYNC_LIB;
	GetDeviceFinder()->StartMonitor();
	GetDeviceFinder()->Register(this);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMonitorUPNPDlg::Event(const TSTRING& strEvent,long nParam)
{
	using namespace QSYNC_LIB;

	DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
	DeviceDataPt spDeviceData = *pspDeviceData; 
	
	if (DEVICE_DIAG_CONNECT == strEvent)
	{
		m_listLog.AddString(spDeviceData->ID().c_str());
		m_listLog.AddString(strEvent.c_str());
	}
	else if (DEVICE_DIAG_DISCONNECT == strEvent)
	{
		m_listLog.AddString(spDeviceData->ID().c_str());
		m_listLog.AddString(strEvent.c_str());
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMonitorUPNPDlg::OnPaint()
{
	CDialog::OnPaint();
}


void CMonitorUPNPDlg::OnLbnSelchangeLoglist()
{
	// TODO: Add your control notification handler code here
}
