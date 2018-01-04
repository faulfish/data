// BlueTimeDlg.cpp : implementation file
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#include "stdafx.h"
#include "BlueTime.h"
#include "BlueTimeDlg.h"
#include "ChooseServerDlg.h"
#include "SecurityDlg.h"
#include "ConnStatsDlg.h"
#include "btversinfo.h"

HWND g_hDialog = NULL;

static CString BdAddrToString(BD_ADDR bd_addr);
static const CString crlf("\r\n");

// {CE37CA6E-288A-409a-9796-191882EE44FC}
static GUID service_guid = 
{ 0xce37ca6e, 0x288a, 0x409a, { 0x97, 0x96, 0x19, 0x18, 0x82, 0xee, 0x44, 0xfc } };
static char m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1] = "WIDCOMM SDK L2Cap Service";

const UINT WIDCOMM_WM_ON_INCOMING_L2CAP = RegisterWindowMessage(_T("WIDCOMM_WM_ON_INCOMING_L2CAP"));

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
    CString m_strVersion;
    CString m_strCopyRight;


    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CAboutDlg)
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL
    virtual BOOL OnInitDialog();

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

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_strVersion = _T("BlueTime Version: ");
    m_strVersion += _T(BTWPRODUCTVERSIONSTR);
    m_strCopyRight = _T(BTWLEGALCOPYRIGHTSTR);
    SetDlgItemText(IDC_COPYRIGHT, m_strCopyRight);
    SetDlgItemText(IDC_VERSION, m_strVersion);
    return true;
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
// CBlueTimeDlg dialog

CBlueTimeDlg::CBlueTimeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CBlueTimeDlg::IDD, pParent),
      m_pSdpService(NULL),
      m_pL2CapIf(NULL),
      m_isSessionStarted(FALSE),
      m_isServiceRecordCreated(FALSE)
{
    //{{AFX_DATA_INIT(CBlueTimeDlg)
    m_connected_device = _T("");
    m_connected_text = _T("");
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

    m_hEventStopClockThread = CreateEvent(NULL, FALSE, FALSE, NULL);   // auto reset, initially reset
    m_hEventClockThreadKilled = CreateEvent(NULL, FALSE, FALSE, NULL); // auto reset, initially reset
    m_isDialogClosing = FALSE;
    m_pClockThread = NULL;
    m_IsConnected  = FALSE;
    m_security_level = BTM_SEC_NONE;
}

CBlueTimeDlg::~CBlueTimeDlg()
{
    DWORD dwExitCode;
    if (m_pClockThread != NULL &&
        GetExitCodeThread(m_pClockThread->m_hThread, &dwExitCode) &&
        dwExitCode == STILL_ACTIVE)
    {
        // Kill the worker thread by setting the "kill thread" event.
        SetEvent(m_hEventStopClockThread);
        WaitForSingleObject(m_hEventClockThreadKilled, INFINITE);
    }

    CloseHandle(m_hEventStopClockThread);
    CloseHandle(m_hEventClockThreadKilled);
}


void CBlueTimeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CBlueTimeDlg)
    DDX_Control(pDX, IDC_SECURITY_BUTTON, m_SecurityButton);
    DDX_Control(pDX, IDC_REMOTE_TIME_TEXT, m_remote_time);
    DDX_Control(pDX, IDC_LOCAL_TIME_TEXT, m_local_time);
    DDX_Control(pDX, IDC_SERVER_CHECK, m_ServerCheck);
    DDX_Text(pDX, IDC_CONNECTED_DEVICE, m_connected_device);
    DDX_Text(pDX, IDC_CONNECTED_TEXT, m_connected_text);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBlueTimeDlg, CDialog)
    //{{AFX_MSG_MAP(CBlueTimeDlg)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
    ON_WM_DESTROY()
    ON_BN_CLICKED(IDC_SECURITY_BUTTON, OnSecurityButton)
    ON_BN_CLICKED(IDC_STATS_BUTTON, OnStatsButton)
    //}}AFX_MSG_MAP
    ON_REGISTERED_MESSAGE(WIDCOMM_WM_ON_INCOMING_L2CAP, OnIncomingMessage)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueTimeDlg message handlers

BOOL CBlueTimeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    //IMPORTANT, global handle for callbacks
    g_hDialog = GetSafeHwnd();

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
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    
    // TODO: Add extra initialization here
    SetDlgItemText(IDC_SC_LEFT_TEXT, "");
    SetDlgItemText(IDC_SC_RIGHT_TEXT, "");
    
    return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlueTimeDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBlueTimeDlg::OnPaint() 
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
HCURSOR CBlueTimeDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

void CBlueTimeDlg::OnStartButton() 
{
    // TODO: Add your control notification handler code here

    if (! m_isSessionStarted)
    {

        // Did user ask to be a Time Server or a Client?
        m_isServer = IsDlgButtonChecked(IDC_SERVER_CHECK);

        if ( !m_isServer && SecurityOK())
        {
            if (! DoChooseServer())     // D0 Inquiry and Discovery to find a server
                return;     // User did not choose a server!
        }
        StartSession();
    }
    else
    {
        SetDlgItemText(IDC_STATUS_TEXT, "Session stopped by user.");
        StopSession();
    }

}

void CBlueTimeDlg::StartSession()
{
    int rc;

    // don't proceed if security setting is inconsistent with client/server choice
    if (!SecurityOK())
    {
        MessageBox(_T("Set Security consistent with client/server choice"), _T("SECURITY"), MB_OK);
        return;
    }

    // If we are a Time Server create the service record.  If
    // it fails then get out now.
    //
    if ( m_isServer && ! DoCreateServiceRecord())
        return;

    //
    // Set a security level.
    //
    if (!m_pL2CapIf->SetSecurityLevel(m_serviceName, m_security_level, m_isServer))
    {
        CString error;
        error.Format("Set Security failed");
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return;
    }
  
    if ( m_isServer )
        rc = Listen(m_pL2CapIf);
    else
        rc = Connect(m_pL2CapIf,m_BdAddr, L2CAP_DEFAULT_MTU );

    if (!rc)
    {
        CString error;
        error.Format("Connect Open failed, error code %i!", rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return;
    }

    if ( m_isServer )
    {
    SetDlgItemText(IDC_SC_LEFT_TEXT, "SERVER");
    SetDlgItemText(IDC_SC_RIGHT_TEXT, "CLIENT");
    }
    else
    {
    SetDlgItemText(IDC_SC_LEFT_TEXT, "CLIENT");
    SetDlgItemText(IDC_SC_RIGHT_TEXT, "SERVER");
    }

    SetDlgItemText(IDC_LOCAL_TIME_TEXT, "");    // clear time fields
    SetDlgItemText(IDC_REMOTE_TIME_TEXT, "");

    // Set text on button and flag to redefine it as the
    // way to stop an active session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Stop Session");

    m_ServerCheck.EnableWindow( FALSE );
    m_SecurityButton.EnableWindow(FALSE);   

    if(!m_IsConnected)
    {
        // on MS stack, clients are connected already by the time we get here, leave the text as it was
        if(m_isServer)
            SetDlgItemText(IDC_STATUS_TEXT, "Session is started.  Ready for a connection.");
    }

    m_isSessionStarted = TRUE;

    // Start a thread that keeps time.
    //
    ResetEvent(m_hEventStopClockThread);
    m_pClockThread = AfxBeginThread(CBlueTimeDlg::RunningClock, this);

}

UINT CBlueTimeDlg::RunningClock(LPVOID pParam)
{
    CBlueTimeDlg* p_dlg = static_cast<CBlueTimeDlg*>(pParam);
    CTime   local_time;
    CString time_str;
    UINT16  len_written;
    
    do
    {
        // get local time
        local_time = CTime::GetCurrentTime();
        time_str = local_time.Format("%c");
        p_dlg->SetDlgItemText(IDC_LOCAL_TIME_TEXT, (LPCTSTR) time_str); // set time in dlg box

        // send local time to remote Bluetooth device
        int int_length = time_str.GetLength();
        if (p_dlg->m_IsConnected)
            p_dlg->Write((void *)((LPCTSTR) time_str), (UINT16) int_length, &len_written);

    } while (WaitForSingleObject(p_dlg->m_hEventStopClockThread, 1000) == WAIT_TIMEOUT); 

    SetEvent(p_dlg->m_hEventClockThreadKilled); 
    return 0;
}

void CBlueTimeDlg::StopSession()
{
    Disconnect();
    
    // Set text on button and flag to redefine it as the
    // way to restart a session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Start Session");
    SetDlgItemText(IDC_CONNECTED_TEXT, "");
    SetDlgItemText(IDC_CONNECTED_DEVICE, "");

    m_ServerCheck.EnableWindow( TRUE );
    m_SecurityButton.EnableWindow(TRUE);

    m_IsConnected = FALSE;
    m_isSessionStarted = FALSE;
    //
    // Delete the SDP object (if created).  This
    // removes the service from the 'available' list.
    if (m_pSdpService)
    {
        delete m_pSdpService;
        m_pSdpService = NULL;
    }

    if (m_pL2CapIf)
    {
        m_pL2CapIf->Deregister();
        delete m_pL2CapIf;
        m_pL2CapIf = NULL;
    }

    SetEvent(m_hEventStopClockThread);

    SetDlgItemText(IDC_LOCAL_TIME_TEXT, "");    // clear time fields
    SetDlgItemText(IDC_REMOTE_TIME_TEXT, "");


}

BOOL CBlueTimeDlg::DoCreateServiceRecord()
{
    SDP_RETURN_CODE sdp_rc;
    CString error;
    
    // Create an L2Cap interface object.
    //
    if (m_pL2CapIf)
    {
        m_pL2CapIf->Deregister();
        delete m_pL2CapIf;
        m_pL2CapIf = NULL;
    }

    m_pL2CapIf = new CL2CapIf();

    // Assign a PSM for the service.
    //
    if ( !m_pL2CapIf->AssignPsmValueEx(&service_guid, 0, TRUE))
    {
        SetDlgItemText(IDC_STATUS_TEXT, "ERROR - PSM assignment failed");
        return FALSE;
    }

    m_pL2CapIf->Register ();

    // Create a SDP Service object.
    //
    if (m_pSdpService)
        delete m_pSdpService;

    m_pSdpService = new CSdpService();

    //
    //
    sdp_rc = m_pSdpService->AddServiceClassIdList (1, &service_guid);
    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddServiceClassIdList() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    sdp_rc = m_pSdpService->AddServiceName (m_serviceName);
    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddServiceName() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }


    sdp_rc = m_pSdpService->AddL2CapProtocolDescriptor (m_pL2CapIf->GetPsm());
    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddL2CapProtocolDescriptor() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    // test for 16 byte INT attribute support
    UINT8 aValue[] = {0x80, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f};
    sdp_rc = m_pSdpService->AddAttribute(0x05FF, TWO_COMP_INT_DESC_TYPE, 16, aValue);
    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddAttribute() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    // test for attribute longer than MAX_ATTR_LEN
    UINT8 strValue[MAX_ATTR_LEN + 5];
    memset(strValue, 0x44, MAX_ATTR_LEN + 5);
    strValue[MAX_ATTR_LEN - 1] = 0x0;

    sdp_rc = m_pSdpService->AddAttribute(0x05Fe, TEXT_STR_DESC_TYPE, MAX_ATTR_LEN + 5, strValue);
    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddAttribute() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    sdp_rc = m_pSdpService->MakePublicBrowseable();
    if ( SDP_OK != sdp_rc )
    {
        error.Format("MakePublicBrowseable() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    sdp_rc = m_pSdpService->CommitRecord();
    if ( SDP_OK != sdp_rc )
    {
        error.Format("CommitRecord() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    m_isServiceRecordCreated = TRUE;
    return TRUE;
}

BOOL CBlueTimeDlg::DoChooseServer()
{
    CChooseServerDlg dlg;

    // Create an L2Cap interface object.
    //
    if (m_pL2CapIf)
        delete m_pL2CapIf;

    m_pL2CapIf = new CL2CapIf();

    // The dialog needs the GUID.
    dlg.m_pServiceGuid = &service_guid;
            
    if (dlg.DoModal() == IDOK)
    {
        // From the dialog, set the BD Address
        // and assign the SCN

        memcpy(m_BdAddr, dlg.m_BdAddr, BD_ADDR_LEN);

        if (!m_pL2CapIf->AssignPsmValueEx(&service_guid, dlg.m_Psm, FALSE))
        {
            CString msg;
            msg.Format("Failed to assign PSM %d", dlg.m_Psm); 
            SetDlgItemText(IDC_STATUS_TEXT, msg);
            return FALSE;
        }

        m_pL2CapIf->Register ();

        return TRUE;
    }
    //
    // else the user cancelled.
    //
    else
        return FALSE;
}


// methods to replace the L2CapConn virtual functions
void CBlueTimeDlg::OnIncomingConnection ()
{
    SetDlgItemText(IDC_STATUS_TEXT, "Incoming Client");
    ::PostMessage(g_hDialog, WIDCOMM_WM_ON_INCOMING_L2CAP, 0, 0);
}

LRESULT CBlueTimeDlg::OnIncomingMessage(WPARAM wParam, LPARAM lParam)
{
    Accept();

    return 0;
}

void CBlueTimeDlg::OnConnected ()
{
    BD_ADDR m_BdAddr;

    SetDlgItemText(IDC_STATUS_TEXT, "Connected");
    m_IsConnected = TRUE;

    memcpy(m_BdAddr, m_RemoteBdAddr, BD_ADDR_LEN);
    CString s;
    s.Format("%02x:%02x:%02x:%02x:%02x:%02x", m_BdAddr[0], m_BdAddr[1], m_BdAddr[2], m_BdAddr[3], m_BdAddr[4], m_BdAddr[5]);

    SetDlgItemText(IDC_CONNECTED_TEXT, "Connected to:");
    SetDlgItemText(IDC_CONNECTED_DEVICE, s);

}

void CBlueTimeDlg::OnDataReceived(void *p_data, UINT16 length)
{
    CString msg( (LPCTSTR) p_data, length);
    SetDlgItemText(IDC_REMOTE_TIME_TEXT, (LPCTSTR)msg);
}

void CBlueTimeDlg::OnRemoteDisconnected (UINT16 reason)
{
    // Set text on button and flag to redefine it as the
    // way to restart a session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Start Session");

    m_ServerCheck.EnableWindow( TRUE );
    m_SecurityButton.EnableWindow(TRUE);

    SetDlgItemText(IDC_STATUS_TEXT, "Session stopped by remote.");

    m_isSessionStarted = FALSE;

    // Always stop the session.
    //
    StopSession();

    // If we are a server, automatically
    // restart the session to be ready
    // for a new time client.
    //
    if ( m_isServer )
        StartSession();
}

void CBlueTimeDlg::OnCongestionStatus (BOOL is_congested)
{
}

// handlers for events from controls in dialog
//
void CBlueTimeDlg::OnDestroy() 
{
    CDialog::OnDestroy();

    // TODO: Add your message handler code here
    StopSession();
}

void CBlueTimeDlg::OnSecurityButton() 
{
    // TODO: Add your control notification handler code here
    CSecurityDlg dlg;

    // The dialog needs the GUID.
    dlg.m_security_level_in = m_security_level;
    m_isServer = IsDlgButtonChecked(IDC_SERVER_CHECK);
    dlg.m_isServer = m_isServer;
            
    if (dlg.DoModal() == IDOK)
    {
        // From the dialog, set the new security flags selected by user
        if (dlg.m_security_level_out != dlg.m_security_level_in)
        {
            SetDlgItemText(IDC_STATUS_TEXT, "Security level modified.");
        }
        else
            SetDlgItemText(IDC_STATUS_TEXT, "No change in security level.");

        m_security_level = dlg.m_security_level_out;
    }
    //
    // else the user cancelled.
    //
    else
        SetDlgItemText(IDC_STATUS_TEXT, "No change in security level.");
}

void CBlueTimeDlg::OnStatsButton() 
{
    // TODO: Add your control notification handler code here

    // bring up a modeless window to show the connection statistics

    if (!CConnStatsDlg::m_pDlg)
    {
        CConnStatsDlg::m_pDlg = new CConnStatsDlg (this);
        CConnStatsDlg::m_pDlg->Create ();
    }
}

BOOL CBlueTimeDlg::SecurityOK()
{
    // no security is OK for both client and server
    if (m_security_level == BTM_SEC_NONE)
        return TRUE;

    if (m_isServer)
    {
        if (m_security_level & (BTM_SEC_OUT_AUTHENTICATE | BTM_SEC_OUT_ENCRYPT))
            return FALSE;
    }
    else
    {
        if (m_security_level & (BTM_SEC_IN_AUTHORIZE | BTM_SEC_IN_AUTHENTICATE | BTM_SEC_IN_ENCRYPT))
            return FALSE;
    }

    return TRUE;
}
