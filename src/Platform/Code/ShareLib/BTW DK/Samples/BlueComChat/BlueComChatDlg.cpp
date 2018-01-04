/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueComChatDlg.cpp
//  $Header:
//
//  Function    implementation file for main ComChat dialog
//              
//
//  Date                         Modification
//  -----------------------------------------
//  22Mar2001   Jim French      Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueComChat.h"
#include "BlueComChatDlg.h"
#include "ChooseServerDlg.h"
#include "btversinfo.h"

static CString BdAddrToString(BD_ADDR bd_addr);
static const CString crlf("\r\n");

static GUID service_guid = CBtIf::guid_SERVCLASS_SERIAL_PORT;                                 
//static char m_serviceNameForServer[BT_MAX_SERVICE_NAME_LEN + 1] = "Bluetooth Serial Port";
static char m_serviceNameForServer[BT_MAX_SERVICE_NAME_LEN + 1] = "WIDCOMM DK COM Serial Port";

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
    virtual BOOL OnInitDialog();
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

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_strVersion = _T("BlueComChat Version: ");
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
// CBlueComChatDlg dialog

CBlueComChatDlg::CBlueComChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlueComChatDlg::IDD, pParent), 
      m_pSdpService(NULL),
      m_isSessionStarted(FALSE),
      m_pBtIf(NULL),
      m_isConnected(FALSE),
      m_isConnectedPrevious(FALSE),
      m_ComPort(0)
{
	//{{AFX_DATA_INIT(CBlueComChatDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pBtIf = new CBtIf;
}

CBlueComChatDlg::~CBlueComChatDlg()
{
    // Delete the SDP object (if created).  This
    // removes the service from the 'available' list.
    //
    if (m_pSdpService)
    {
        delete m_pSdpService;
        m_pSdpService = NULL;
    }
    delete (m_pBtIf);
    m_pBtIf = NULL;
}


void CBlueComChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlueComChatDlg)
	DDX_Control(pDX, IDC_NEWLINE_EDIT, m_NewLineEdit);
	DDX_Control(pDX, IDC_SEND_BUTTON, m_SendButton);
	DDX_Control(pDX, IDC_SERVER_CHECK, m_ServerCheck);
	DDX_Control(pDX, IDC_CHATLOG_EDIT, m_ChatLogEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBlueComChatDlg, CDialog)
	//{{AFX_MSG_MAP(CBlueComChatDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
    ON_BN_CLICKED(IDC_READ_LINKMODE, OnReadLinkModeButton)
    ON_BN_CLICKED(IDC_SET_SNIFFMODE, OnSetSniffModeButton)
    ON_BN_CLICKED(IDC_CANCEL_SNIFFMODE, OnCancelSniffModeButton)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueComChatDlg message handlers

BOOL CBlueComChatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

    m_timer_id = SetTimer (1, 500, NULL);

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
	
    // Custom initialization...


    // Default the 'Show Events' check-box to be checked.
    //
	//    CheckDlgButton( IDC_SHOW_EVENTS_CHECK, 1 );
    CheckDlgButton( IDC_SHOW_EVENTS_CHECK, 0 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlueComChatDlg::OnDestroy() 
{
	CDialog::OnDestroy();

    // Make sure the session is stopped.  This
    // will close the port and, if we are a server,
    // remove the service record.
    //
    StopSession();
}

void CBlueComChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBlueComChatDlg::OnPaint() 
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
HCURSOR CBlueComChatDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBlueComChatDlg::OnStartButton() 
{
    if (! m_isSessionStarted)
    {
        // Are we a ComChat Server (or a Client)?
        //
        m_isServer = IsDlgButtonChecked(IDC_SERVER_CHECK);

        if ( m_isServer )
        {
            // use compiled in service name for server
            memcpy(m_serviceName, m_serviceNameForServer, BT_MAX_SERVICE_NAME_LEN + 1);
        }
        else
        {
            // clear service name so DoChooseServer can set it
            memset(m_serviceName, 0, BT_MAX_SERVICE_NAME_LEN + 1);

            // The client must choose a ComChat Server to
            // connect to.
            //
            if (! DoChooseServer())
                return;     // User did not choose a server!
        }

        StartSession();
    }
    else
    {
        StopSession();
    }
}

BOOL CBlueComChatDlg::DoChooseServer()
{
    CChooseServerDlg dlg;

    // The dialog needs the GUID.
    dlg.m_pServiceGuid = &service_guid;
            
    if (dlg.DoModal() == IDOK)
    {
        // From the dialog, set the BD Address so that
        // it can be used in the client connect.
        //
        memcpy(m_BdAddr, dlg.m_BdAddr, BD_ADDR_LEN);
        memcpy(m_serviceName, dlg.m_serviceName, BT_MAX_SERVICE_NAME_LEN + 1);
        m_Name = dlg.m_Name;    // friendly name
		m_scn = dlg.m_Scn;		// scn used by selected server

        CString msg;

        msg.Format("Try connecting to %s with service %s on scn %d",
			m_Name, m_serviceName, m_scn);
        SetDlgItemText(IDC_STATUS_TEXT, msg);
        Sleep(3000);

        return TRUE;
    }
    //
    // else the user cancelled.
    //
    else
        return FALSE;
}

void CBlueComChatDlg::StartSession()
{
    CString status;

    // If we are a ComChat Server create the service record.  If
    // it fails then get out now.
    //
    if ( m_isServer && ! DoCreateServiceRecord())
        return;
  
    // Depending on our mode open the port
    // 
    if ( m_isServer )
    {
 	    SPP_SERVER_RETURN_CODE port_rc;

        port_rc = CSppServer::CreateConnection(m_serviceName);

        if (CSppServer::SUCCESS != port_rc)
        {
            CString error;
            error.Format("CSppServer::CreateConnection failed, error code %i!", port_rc);
            SetDlgItemText(IDC_STATUS_TEXT, error);
            return;
        }

        status.Format("Server is now waiting for a client connection");
    }
    else
    {
 	    SPP_CLIENT_RETURN_CODE port_rc;

        port_rc = CSppClient::CreateConnection(m_BdAddr, m_serviceName);
        status = "Attempting a connection with BDA: " + BdAddrToString(m_BdAddr);

        if (CSppClient::SUCCESS != port_rc)
        {
            CString error;
            error.Format("CSppClient::CreateConnection failed, error code %i!", port_rc);
            SetDlgItemText(IDC_STATUS_TEXT, error);
            return;
        }
    }


    // Set text on button and flag to redefine it as the
    // way to stop an active session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Stop Session");

    m_ServerCheck.EnableWindow( FALSE );

    SetDlgItemText(IDC_STATUS_TEXT, status);

    m_isSessionStarted = TRUE;
}

void CBlueComChatDlg::StopSession()
{
	m_isConnected = FALSE;

    // Depending on our mode close the port
    // 
    if ( m_isServer )
    {
        // Close the port.
        //
        SPP_SERVER_RETURN_CODE port_rc = CSppServer::RemoveConnection();
        if (CSppServer::SUCCESS != port_rc && CSppServer::NOT_CONNECTED != port_rc)
        {
            CString error;
            error.Format("ComChat Port close failed,, error code %i", port_rc);
            SetDlgItemText(IDC_STATUS_TEXT, error);
        }
    }
    else
    {
        SetDlgItemText(IDC_STATUS_TEXT, "COM port closed by user");

        Sleep(3000);    // let user read the message

        // Close the Bluetooth connection.
        //
        SPP_CLIENT_RETURN_CODE port_rc = CSppClient::RemoveConnection();
        if (CSppClient::SUCCESS != port_rc && CSppClient::NOT_CONNECTED != port_rc)
        {
            CString error;
            error.Format("ComChat Port close failed,, error code %i", port_rc);
            SetDlgItemText(IDC_STATUS_TEXT, error);
        }
    }

    // Set text on button and flag to redefine it as the
    // way to restart a session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Start Session");

    // Disable things.
    //
    m_SendButton.EnableWindow( FALSE );
    m_NewLineEdit.EnableWindow( FALSE );

    // It should now be possible to change from Client
    // mode to Server, and vice-versa.
    //
    m_ServerCheck.EnableWindow( TRUE );

    // Delete the SDP object (if created).  This
    // removes the service from the 'available' list.
    //
    if (m_pSdpService)
    {
        delete m_pSdpService;
        m_pSdpService = NULL;
    }

    SetDlgItemText(IDC_STATUS_TEXT, "Session has been stopped.");
    m_isSessionStarted = FALSE;
}

BOOL CBlueComChatDlg::DoCreateServiceRecord()
{
	// This is a place holder for future releases which may allow an
	// entirely new SPP service to be created 
    return TRUE;
}

void CBlueComChatDlg::OnSendButton() 
{
    CString text;

    GetDlgItemText(IDC_NEWLINE_EDIT, text);

    LPTSTR msg_to_send = (LPTSTR) text.GetBuffer(256);

    if (!serialComm.WriteData( msg_to_send , text.GetLength()))
        SetDlgItemText(IDC_STATUS_TEXT, "Error trying to write message");
    else
	{
        AddLogText(">> " + text);
		// clear out the edit line for the next messages to be sent
		SetDlgItemText(IDC_NEWLINE_EDIT, "");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NEWLINE_EDIT);
		pEdit->SetFocus();
	}
}

void CBlueComChatDlg::AddLogText(const CString &new_text)
{
	CString current_text;

	m_ChatLogEdit.GetWindowText(current_text);
	m_ChatLogEdit.SetWindowText(current_text + new_text + crlf);
	m_ChatLogEdit.LineScroll(m_ChatLogEdit.GetLineCount());
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NEWLINE_EDIT);
	pEdit->SetFocus();
}

void CBlueComChatDlg::OnClientStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, SPP_STATE_CODE state)
{
    // Was a connection established?
    //
    if (state == SPP_CONNECTED)
    {
        m_ComPort = com_port;
        m_isConnected = TRUE;

        CString msg;
        msg.Format("Connected with %s, on COM port %d", BdAddrToString(bda), com_port);
        SetDlgItemText(IDC_STATUS_TEXT, msg);
    }
    else
    {
		if (m_isServer == FALSE)
        {
			m_isConnected = FALSE;
            if (state == SPP_DISCONNECTED)
                SetDlgItemText(IDC_STATUS_TEXT, "Disconnected from " + BdAddrToString(bda));
            else
            {
                CString msg;
                msg.Format("StateChange Error Code: %d", state);
                SetDlgItemText(IDC_STATUS_TEXT, msg);
                // OnTimer doesn't handle this, since there isn't an actual state change
                SetDlgItemText(IDC_START_BUTTON, "Start Session");
                m_ServerCheck.EnableWindow( TRUE );
                m_isSessionStarted = FALSE;
            }
        }
        else
            SetDlgItemText(IDC_STATUS_TEXT, "Ready to start a session");
    }

}

void CBlueComChatDlg::OnServerStateChange(BD_ADDR bda, DEV_CLASS dev_class, BD_NAME name, short com_port, SPP_STATE_CODE state)
{
    // Was a connection established?
    //
    if (state == SPP_CONNECTED)
    {
        m_ComPort = com_port;
        m_isConnected = TRUE;

        CString msg;
        msg.Format("Connected with %s, on COM port %d", BdAddrToString(bda), com_port);
        SetDlgItemText(IDC_STATUS_TEXT, msg);
		m_SendButton.EnableWindow( TRUE );  // don't allow user to send a message
        m_NewLineEdit.EnableWindow( TRUE );
    }
    else
    {
        if (state == SPP_DISCONNECTED)
            SetDlgItemText(IDC_STATUS_TEXT, "Disconnected from " + BdAddrToString(bda));
        else
        {
            CString msg;
            msg.Format("StateChange Error Code: %d", state);
            SetDlgItemText(IDC_STATUS_TEXT, msg);
        }

        m_SendButton.EnableWindow( FALSE );  // don't allow user to send a message
        m_NewLineEdit.EnableWindow( FALSE );
		AddLogText("-- Ready for a new client connection");
           
    }
}


///////////////////////////////////////
////////// Utility Functions //////////
///////////////////////////////////////

// Return a CString based on a
//  a BD Address (BD_ADDR type).
//
CString BdAddrToString(BD_ADDR bd_addr)
{
    CString s;

    s.Format("%x:%x:%x:%x:%x:%x",
             bd_addr[0],
             bd_addr[1],
             bd_addr[2],
             bd_addr[3],
             bd_addr[4],
             bd_addr[5]);

    return s;
}

void CBlueComChatDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);

    if (m_isConnectedPrevious != m_isConnected)
    {
        if (m_isConnected)
        {
            // now Bluetooth connected, open the COM port as a file
            if (serialComm.OpenCommPort((UINT8)m_ComPort))
            {
                SetDlgItemText(IDC_STATUS_TEXT, "COM port open successful");
                m_SendButton.EnableWindow( TRUE );  // allow user to send a message
				SetDlgItemText(IDC_NEWLINE_EDIT, "");
                m_NewLineEdit.EnableWindow( TRUE );
		        AddLogText("-- START NEW SESSION");
            }
            else
            {
                SetDlgItemText(IDC_STATUS_TEXT, "COM port open failed");
                m_SendButton.EnableWindow( FALSE );  // don't allow user to send a message
                m_NewLineEdit.EnableWindow( FALSE );
            }
        }
        else
        {
            // now disconnected
            serialComm.CloseCommPort();
            SetDlgItemText(IDC_STATUS_TEXT, "COM port closed due to Bluetooth disconnect");
            m_SendButton.EnableWindow( FALSE );  // don't allow user to send a message
            m_NewLineEdit.EnableWindow( FALSE );
            SetDlgItemText(IDC_START_BUTTON, "Start Session");
            m_ServerCheck.EnableWindow( TRUE );
            m_isSessionStarted = FALSE;
        }
        m_isConnectedPrevious = m_isConnected;
    }

	// see if there is any data to be read 
	if (m_isConnected)
	{
		int bytes_in;
        char msg_block[1000];

		bytes_in = serialComm.ReadData(msg_block, sizeof msg_block );

		if (bytes_in)
		{
			// pass buffered input to the dialog box
			CString msg( msg_block, bytes_in);
			AddLogText("<< " + msg);
		}
	}
}
void CBlueComChatDlg::OnSetSniffModeButton() 
{
    if (FALSE == CBtIf::SetSniffMode(m_BdAddr))
    {
        AddLogText("SetSniffMode() fail");
    }
    else
    {
        AddLogText("SetSniffMode() success");
    }
}
void CBlueComChatDlg::OnCancelSniffModeButton() 
{
    
    if (FALSE == CBtIf::CancelSniffMode(m_BdAddr))
    {
        AddLogText("CancelSniffMode() fail");
    }
    else
    {
        AddLogText("CancelSniffMode() success");
    }
}

void CBlueComChatDlg::OnReadLinkModeButton() 
{
    UINT8   mode; 
    char    msg[200];
    
    if (FALSE == CBtIf::ReadLinkMode(m_BdAddr, &mode))
    {
        AddLogText("ReadLinkMode() fail");
    }
    else
    {
        wsprintf(msg, "ReadLinkMode() mode:%d", mode);
        AddLogText(msg);
    }

}
