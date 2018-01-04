/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueChatDlg.cpp
//  $Header:
//
//  Function    implementation file for main Chat dialog
//              
//
//  Date                         Modification
//  -----------------------------------------
//  12/22/2000    P Chaisson     Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BlueChat.h"
#include "BlueChatDlg.h"
#include "ChooseServerDlg.h"
#include "ConnStatsDlg.h"
#include "btversinfo.h"

static CString BdAddrToString(BD_ADDR bd_addr);
static const CString crlf("\r\n");

static GUID service_guid = { 0x5fc2a42e, 0x144e, 0x4bb5, { 0xb4, 0x3f, 0x4e, 0x61, 0x71, 0x1d, 0x1c, 0x32 } };
static char m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1] = "WIDCOMM SDK RFComm Service";
                                 
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

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BOOL CAboutDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    m_strVersion = _T("BlueChat Version: ");
    m_strVersion += _T(BTWPRODUCTVERSIONSTR);
    m_strCopyRight = _T(BTWLEGALCOPYRIGHTSTR);
    SetDlgItemText(IDC_COPYRIGHT, m_strCopyRight);
    SetDlgItemText(IDC_VERSION, m_strVersion);
    return true;
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueChatDlg dialog

CBlueChatDlg::CBlueChatDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlueChatDlg::IDD, pParent), 
      m_pSdpService(NULL),
      m_isSessionStarted(FALSE),
      m_isDsrDtrToggleOn(TRUE),
      m_isCtsRtsToggleOn(TRUE),
      m_pBtIf(NULL),
      m_isConnected(FALSE)
{
	//{{AFX_DATA_INIT(CBlueChatDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pBtIf = new CBtIf;
}

CBlueChatDlg::~CBlueChatDlg()
{
    delete (m_pBtIf);
    m_pBtIf = NULL;
}


void CBlueChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlueChatDlg)
	DDX_Control(pDX, IDC_CTSRTS_LIGHT, m_CtsRtsStatusLight);
	DDX_Control(pDX, IDC_CTSRTS_BUTTON, m_CtsRtsButton);
	DDX_Control(pDX, IDC_DSRDTR_LIGHT, m_DsrDtrStatusLight);
	DDX_Control(pDX, IDC_DSRDTR_BUTTON, m_DsrDtrButton);
	DDX_Control(pDX, IDC_NEWLINE_EDIT, m_NewLineEdit);
	DDX_Control(pDX, IDC_SEND_BUTTON, m_SendButton);
	DDX_Control(pDX, IDC_SERVER_CHECK, m_ServerCheck);
	DDX_Control(pDX, IDC_CHATLOG_EDIT, m_ChatLogEdit);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBlueChatDlg, CDialog)
	//{{AFX_MSG_MAP(CBlueChatDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	ON_BN_CLICKED(IDC_SEND_BUTTON, OnSendButton)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DSRDTR_BUTTON, OnDsrdtrButton)
	ON_BN_CLICKED(IDC_CTSRTS_BUTTON, OnCtsrtsButton)
	ON_BN_CLICKED(IDC_STATS_BUTTON, OnStatsButton)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueChatDlg message handlers

BOOL CBlueChatDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    m_timer_id = SetTimer (1, 1000, NULL);

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

    // Load bitmaps for the status lights.
    // 
  	m_bmRed.LoadBitmap(IDB_RED_LIGHT);
	m_bmGreen.LoadBitmap(IDB_GREEN_LIGHT);

   	m_DsrDtrStatusLight.SetBitmap((HBITMAP) m_bmRed);
   	m_CtsRtsStatusLight.SetBitmap((HBITMAP) m_bmRed);

    // Default the 'Show Events' check-box to be checked.
    //
    CheckDlgButton( IDC_SHOW_EVENTS_CHECK, 1 );

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlueChatDlg::OnDestroy() 
{
	CDialog::OnDestroy();

    // Make sure the session is stopped.  This
    // will close the port and, if we are a server,
    // remove the service record.
    //
    StopSession();
}

void CBlueChatDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBlueChatDlg::OnPaint() 
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
HCURSOR CBlueChatDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBlueChatDlg::OnStartButton() 
{
    if (! m_isSessionStarted)
    {
        // Are we a Chat Server (or a Client)?
        //
        m_isServer = IsDlgButtonChecked(IDC_SERVER_CHECK);

        if (! m_isServer )
        {
            // The client must choose a Chat Server to
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

BOOL CBlueChatDlg::DoChooseServer()
{
    CChooseServerDlg dlg;
    // Set the security level.
    //
    UINT8 sec_level = BTM_SEC_NONE;

    // The dialog needs the GUID.
    dlg.m_pServiceGuid = &service_guid;
            
    if (dlg.DoModal() == IDOK)
    {
        // From the dialog, set the BD Address so that
        // it can be used in the client connect.
        //
        memcpy(m_BdAddr, dlg.m_BdAddr, BD_ADDR_LEN);

        // Assign the SCN associating it with the GUID.
        // If return is FALSE then there was a problem.
        //
        if (! m_RfCommIf.AssignScnValue(&service_guid, dlg.m_Scn))
        {
            CString error;
            error.Format("RFCOMM Interface failed to assign SCN %d with GUID", dlg.m_Scn);
            SetDlgItemText(IDC_STATUS_TEXT, error);
            return FALSE;
        }

        return TRUE;
    }
    //
    // else the user cancelled.
    //
    else
        return FALSE;
}

void CBlueChatDlg::StartSession()
{
	PORT_RETURN_CODE port_rc;
    CString status;

    // If we are a Chat Server create the service record.  If
    // it fails then get out now.
    //
    if ( m_isServer && ! DoCreateServiceRecord())
        return;

    // Set the security level.
    //
    UINT8 sec_level = BTM_SEC_NONE;

    if (! m_RfCommIf.SetSecurityLevel(m_serviceName, sec_level , m_isServer))
    {
        SetDlgItemText(IDC_STATUS_TEXT, "Error setting security level");
        return;
    }
  
    // Depending on our mode open the port
    // 
    if ( m_isServer )
    {
        // Open the RFCOMM port as a server using the SCN acquired from
        // service creation.  Note, a second argument could be provided 
        // to indicate the desired MTU. In this case we have elected to
        // take the default (RFCOMM_DEFAULT_MTU).
        // 
        port_rc = OpenServer(m_RfCommIf.GetScn());

        status.Format("Server is now waiting for a client connection on SCN %i", m_RfCommIf.GetScn());
    }
    else
    {
        // Open the RFCOMM port as a client using the SCN acquired from
        // service discovery and the chosen device address.  Note, a third
        // argument could be provided to indicate the desired MTU. In this
        // case we have elected to take the default (RFCOMM_DEFAULT_MTU).
        //
        port_rc = OpenClient(m_RfCommIf.GetScn(), m_BdAddr);
        status = "Attempting a connection with BDA: " + BdAddrToString(m_BdAddr);
    }

    if (SUCCESS != port_rc)
    {
        CString error;
        error.Format("Chat Port Open failed, error code %i!", port_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return;
    }

    // Set text on button and flag to redefine it as the
    // way to stop an active session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Stop Session");

    m_ServerCheck.EnableWindow( FALSE );

    SetDlgItemText(IDC_STATUS_TEXT, "Port opened. " + status);

    m_isSessionStarted = TRUE;
}

void CBlueChatDlg::StopSession()
{
    // Close the RFCOMM port.
    //
    PORT_RETURN_CODE port_rc = Close();

    if (SUCCESS != port_rc && NOT_OPENED != port_rc)
    {
        CString error;
        error.Format("Chat Port close failed,, error code %i", port_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
//        return;
    }
    
    // Set text on button and flag to redefine it as the
    // way to restart a session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Start Session");

    // Disable things.
    //
    m_SendButton.EnableWindow( FALSE );
    m_NewLineEdit.EnableWindow( FALSE );
    m_NewLineEdit.SetReadOnly(TRUE);
    m_DsrDtrButton.EnableWindow( FALSE );
    m_CtsRtsButton.EnableWindow( FALSE );

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

BOOL CBlueChatDlg::DoCreateServiceRecord()
{
    SDP_RETURN_CODE sdp_rc;
    CString error;
    
    // Assign a SCN for the service.
    //
    if ( ! m_RfCommIf.AssignScnValue(&service_guid))
    {
        SetDlgItemText(IDC_STATUS_TEXT, "ERROR - SCN assignment failed");
        return FALSE;
    }
    
    // Create a SDP Service object.
    //
    if (m_pSdpService)
        delete m_pSdpService;

    m_pSdpService = new CSdpService();
    
    // Create a service record in the database and add the
    // sequence to that record.
    //
    sdp_rc = m_pSdpService->AddServiceClassIdList (1, &service_guid);

    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddServiceClassIdList() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    // Add Service Name attribute to the service record.
    //
    sdp_rc = m_pSdpService->AddServiceName (m_serviceName);

    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddServiceName() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    // Add the protocol descriptor.
    //
    sdp_rc = m_pSdpService->AddRFCommProtocolDescriptor (m_RfCommIf.GetScn());

    if ( SDP_OK != sdp_rc )
    {
        error.Format("AddRFCommProtocolDescriptor() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    // Make service browsable.
    //
    sdp_rc = m_pSdpService->MakePublicBrowseable();

    if ( SDP_OK != sdp_rc )
    {
        error.Format("MakePublicBrowseable() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

	// Publish the record.
    //
    sdp_rc = m_pSdpService->CommitRecord();

    if ( SDP_OK != sdp_rc )
    {
        error.Format("CommitRecord() failed, error code %i", sdp_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return FALSE;
    }

    return TRUE;
}

void CBlueChatDlg::OnSendButton() 
{
    PORT_RETURN_CODE port_rc;
    CString text;

    GetDlgItemText(IDC_NEWLINE_EDIT, text);

	unsigned short length_written;
    LPCTSTR msg_to_send = (LPCTSTR) text;

    port_rc = Write( (void *) msg_to_send, text.GetLength(), &length_written);

	if (SUCCESS != port_rc)
	{
        CString error;
		error.Format("Write failed, with code %i", port_rc);
        AddLogText(error);
    }
    else
	{
        AddLogText(">> " + text);
		// clear out the edit line for the next messages to be sent
		SetDlgItemText(IDC_NEWLINE_EDIT, "");
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NEWLINE_EDIT);
		pEdit->SetFocus();
	}
}

void CBlueChatDlg::AddLogText(const CString &new_text)
{
	CString current_text;

	m_ChatLogEdit.GetWindowText(current_text);
	m_ChatLogEdit.SetWindowText(current_text + new_text + crlf);
	m_ChatLogEdit.LineScroll(m_ChatLogEdit.GetLineCount());
	CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NEWLINE_EDIT);
	pEdit->SetFocus();
}

void CBlueChatDlg::SetStatusLights()
{
    PORT_RETURN_CODE port_rc;
    UINT8 signal;

    port_rc = GetModemStatus(&signal);

    if (SUCCESS != port_rc)
    {
        CString error;
        error.Format("GetModemStatus() failed, error code %i", port_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return;
    }

    if (PORT_DTRDSR_ON & signal)
		m_DsrDtrStatusLight.SetBitmap((HBITMAP) m_bmGreen);
    else
		m_DsrDtrStatusLight.SetBitmap((HBITMAP) m_bmRed);

    if (PORT_CTSRTS_ON & signal)
		m_CtsRtsStatusLight.SetBitmap((HBITMAP) m_bmGreen);
    else
		m_CtsRtsStatusLight.SetBitmap((HBITMAP) m_bmRed);
}

void CBlueChatDlg::OnDsrdtrButton() 
{
    PORT_RETURN_CODE port_rc;
    UINT8 signal;

    // If the toggle is on we send a
    // a CLEAR, otherwise a SET.
    //
    if (m_isDsrDtrToggleOn)
        signal = PORT_CLR_DTRDSR;
    else
        signal = PORT_SET_DTRDSR;

    port_rc = SetModemSignal(signal);

    if (SUCCESS != port_rc)
    {
        CString error;
        error.Format("SetModemSignal() failed, error code %i", port_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return;
    }

    // Flip the toggle flag.
    //
    m_isDsrDtrToggleOn = ! m_isDsrDtrToggleOn;
}

void CBlueChatDlg::OnCtsrtsButton() 
{
    PORT_RETURN_CODE port_rc;
    UINT8 signal;

    // If the toggle is on we send a
    // a CLEAR, otherwise a SET.
    //
    if (m_isCtsRtsToggleOn)
        signal = PORT_CLR_CTSRTS;
    else
        signal = PORT_SET_CTSRTS;

    port_rc = SetModemSignal(signal);

    if (SUCCESS != port_rc)
    {
        CString error;
        error.Format("SetModemSignal() failed, error code %i", port_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return;
    }

    // Flip the toggle flag.
    //
    m_isCtsRtsToggleOn = ! m_isCtsRtsToggleOn;
}

//////////////////////////////////////////////////////////////////////
//
//  Called when data is recieved from the port.
//
void CBlueChatDlg::OnDataReceived (void *p_data, UINT16 len)
{
    CString msg( (LPCTSTR) p_data, len);
    AddLogText("<< " + msg);
}

//////////////////////////////////////////////////////////////////////
//
//  Called when and event is recieved on the port.
//
void CBlueChatDlg::OnEventReceived (UINT32 event_code)
{
    // Was a connection established?
    //
    if (PORT_EV_CONNECTED & event_code)
    {
        m_isConnected = TRUE;

        BD_ADDR connected_bd_addr;

        if ( IsConnected(&connected_bd_addr) )
        {
            m_SendButton.EnableWindow( TRUE );
            m_NewLineEdit.EnableWindow( TRUE );
            m_NewLineEdit.SetReadOnly(FALSE);
            m_DsrDtrButton.EnableWindow( TRUE );
            m_CtsRtsButton.EnableWindow( TRUE );
			SetDlgItemText(IDC_NEWLINE_EDIT, "");
			CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NEWLINE_EDIT);
			pEdit->SetFocus();

            SetDlgItemText(IDC_STATUS_TEXT, "Connected with " + BdAddrToString(connected_bd_addr));
        }
    }

    // Did the connection go down?
    //
    if ( PORT_EV_CONNECT_ERR & event_code )
    {
        m_SendButton.EnableWindow( FALSE );
        m_NewLineEdit.EnableWindow( FALSE );
        m_NewLineEdit.SetReadOnly();
        m_DsrDtrButton.EnableWindow( FALSE );
        m_CtsRtsButton.EnableWindow( FALSE );

        // Always stop the session.
        //
        BOOL bSessionWasStarted = m_isSessionStarted;
        StopSession();

        // If we are a server, and had been started, automatically
        // restart the session to be ready
        // for a new chat client.
        //
        if ( m_isServer && bSessionWasStarted )
            StartSession();
    }
    else
    {
        // Did port status change?
        //
        if ( (PORT_EV_CTS & event_code) || (PORT_EV_DSR & event_code) )
        {
            SetStatusLights();
        }
    }
    
    // Format event flags to the log edit if 'Show Events"
    // check-box is checked.
    //
    if (IsDlgButtonChecked(IDC_SHOW_EVENTS_CHECK))
    {
        CString msg("Event received: ");

        if (PORT_EV_RXCHAR & event_code)        // Any Character received
            msg += "RXCHAR ";

        if (PORT_EV_RXFLAG & event_code)        // Received certain character
            msg += "RXFLAG ";

        if (PORT_EV_TXEMPTY & event_code)       // Transmitt Queue Empty
            msg += "TXEMPTY ";

        if (PORT_EV_CTS & event_code)           // CTS changed state
            msg += "CTS ";

        if (PORT_EV_DSR & event_code)           // DSR changed state
            msg += "DSR ";

        if (PORT_EV_RLSD & event_code)          // RLSD changed state
            msg += "RLSD ";

        if (PORT_EV_BREAK & event_code)         // BREAK received
            msg += "BREAK ";

        if (PORT_EV_ERR & event_code)           // Line status error occurred
            msg += "ERR ";

        if (PORT_EV_RING & event_code)          // Ring signal detected
            msg += "RING ";

        if (PORT_EV_CTSS & event_code)          // CTS state
            msg += "CTSS ";
            
        if (PORT_EV_DSRS & event_code)          // DSR state
            msg += "DSRS ";

        if (PORT_EV_RLSDS & event_code)         // RLSD state
            msg += "RLSDS ";

        if (PORT_EV_OVERRUN & event_code)       // receiver buffer overrun
            msg += "OVERRUN ";

        if (PORT_EV_TXCHAR & event_code)        // Any character transmitted
            msg += "TXCHAR ";

        if (PORT_EV_CONNECTED & event_code)     // RFCOMM connection established
            msg += "CONNECTED ";

        if (PORT_EV_CONNECT_ERR & event_code)   // Was not able to establish connection
        {
            if (m_isConnected == TRUE)
                msg += "DISCCONNECT";
            else
                msg += "CONNECT_ERR ";

            m_isConnected = FALSE;
        }

        AddLogText(msg);
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

void CBlueChatDlg::OnStatsButton() 
{
	// TODO: Add your control notification handler code here


	// bring up a modeless window to show the connection statistics

    if (!CConnStatsDlg::m_pDlg)
    {
        CConnStatsDlg::m_pDlg = new CConnStatsDlg (this);
        CConnStatsDlg::m_pDlg->Create ();
    }

}

void CBlueChatDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);

	BD_ADDR connected_bd_addr;

    if ( IsConnected(&connected_bd_addr) )
	{
		CEdit* pEdit = (CEdit*)GetDlgItem(IDC_NEWLINE_EDIT);
		pEdit->SetFocus();
	}
}
