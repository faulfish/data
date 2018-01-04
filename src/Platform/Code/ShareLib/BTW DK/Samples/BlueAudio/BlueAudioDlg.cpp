/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueAudioDlg.cpp
//  $Header:
//
//  Function    implementation file for main Audio dialog
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
#include "BlueAudioDlg.h"
#include "ChooseServerDlg.h"
#include "ConnStatsDlg.h"
#include "btversinfo.h"
#include "escodlg.h"

HWND g_hDialog = NULL;

static CString BdAddrToString(BD_ADDR bd_addr);
static const CString crlf("\r\n");

static GUID service_guid = { 0x8c66104a, 0x1671, 0x4b25, { 0x8b, 0xf6, 0x6d, 0x49, 0xbd, 0x5c, 0x1, 0xcd } };
static char m_serviceName[BT_MAX_SERVICE_NAME_LEN + 1] = "WIDCOMM SDK RFComm-Audio Service";

const UINT WIDCOMM_WM_CONNECT2AUDIO = RegisterWindowMessage(_T("WIDCOMM_WM_CONNECT2AUDIO"));
                              
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
    m_strVersion = _T("BlueAudio Version:");
    m_strVersion += _T(BTWPRODUCTVERSIONSTR);
    m_strCopyRight = _T(BTWLEGALCOPYRIGHTSTR);
    SetDlgItemText(IDC_COPYRIGHT, m_strCopyRight);
    SetDlgItemText(IDC_VERSION, m_strVersion);
    return true;
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
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
// CBlueAudioDlg dialog

CBlueAudioDlg::CBlueAudioDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlueAudioDlg::IDD, pParent), 
      m_pSdpService(NULL),
      m_isSessionStarted(FALSE),
      m_pBtIf(NULL),
      m_isConnected(FALSE)
{
    memset(m_BdAddr, 0xff, BD_ADDR_LEN);
	//{{AFX_DATA_INIT(CBlueAudioDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pBtIf = new CBtIf;
    m_audio_handle = 0;
}

CBlueAudioDlg::~CBlueAudioDlg()
{
    delete (m_pBtIf);
    m_pBtIf = NULL;
}


void CBlueAudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlueAudioDlg)
	DDX_Control(pDX, IDC_SERVER_CHECK, m_ServerCheck);
	DDX_Control(pDX, IDC_CHATLOG_EDIT, m_ChatLogEdit);
    DDX_Control(pDX, IDC_SCO, m_Sco);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CBlueAudioDlg, CDialog)
	//{{AFX_MSG_MAP(CBlueAudioDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_STATS_BUTTON, OnStatsButton)
    ON_BN_CLICKED(IDC_CHANGEESCO, OnChangeesco)
	ON_WM_TIMER()
    ON_BN_CLICKED(IDC_SCO, OnSco)
	ON_BN_CLICKED(IDC_ESCO, OnEsco)
	ON_BN_CLICKED(IDC_READESCO, OnReadesco)

	//}}AFX_MSG_MAP
    ON_REGISTERED_MESSAGE(WIDCOMM_WM_CONNECT2AUDIO, OnConnect2Audio)
    
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueAudioDlg message handlers

BOOL CBlueAudioDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
    m_timer_id = SetTimer (1, 1000, NULL);

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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	

    // Default the 'Show Events' check-box to be checked.
    //
    CheckDlgButton( IDC_SHOW_EVENTS_CHECK, 1 );


    // Default the 'Esco' check-box to be checked.
    //
    m_Sco.SetCheck( BST_CHECKED );

    m_EScoParams.tx_bw = BTM_64KBITS_RATE;
    m_EScoParams.rx_bw = BTM_64KBITS_RATE;
    m_EScoParams.max_latency = BTM_ESCO_MAX_LAT_DONTCARE;;
    m_EScoParams.voice_contfmt = 96;
    m_EScoParams.packet_types = 8;
    m_EScoParams.retrans_effort = BTM_ESCO_RETRANS_POWER;

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBlueAudioDlg::OnDestroy() 
{
	CDialog::OnDestroy();

    // Make sure the session is stopped.  This
    // will close the port and, if we are a server,
    // remove the service record.
    //
    StopSession();
}

void CBlueAudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBlueAudioDlg::OnPaint() 
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
HCURSOR CBlueAudioDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CBlueAudioDlg::OnStartButton() 
{
    CRfCommPort tmp;
    if (! m_isSessionStarted)
    {
        // Are we a Audio Server (or a Client)?
        //
        m_isServer = IsDlgButtonChecked(IDC_SERVER_CHECK);

        if (! m_isServer )
        {
            // The client must choose a Audio Server to
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

BOOL CBlueAudioDlg::DoChooseServer()
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

void CBlueAudioDlg::StartSession()
{
	PORT_RETURN_CODE port_rc;
    CString status;

    // If we are a Audio Server create the service record.  If
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
        error.Format("RfComm Port Open failed, error code %i!", port_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        return;
    }
    SetDlgItemText(IDC_STATUS_TEXT, "Port opened. " + status);

    // Set text on button and flag to redefine it as the
    // way to stop an active session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Stop Session");

    m_ServerCheck.EnableWindow( FALSE );

    m_isSessionStarted = TRUE;
}

void CBlueAudioDlg::StopSession()
{
    CString msg;
    AUDIO_RETURN_CODE bRet;

    //close audio first
    bRet = m_pBtIf->RemoveAudioConnection(m_audio_handle);
    m_audio_handle = 0;

    if (AUDIO_SUCCESS != bRet)
    {
        msg.Format("RemoveAudioConnection:%d", bRet);
        AddLogText(msg);
    }

    // Close the RFCOMM port.
    //
    PORT_RETURN_CODE port_rc = Close();

    if (SUCCESS != port_rc && NOT_OPENED != port_rc)
    {
        CString error;
        error.Format("Audio Port close failed,, error code %i", port_rc);
        SetDlgItemText(IDC_STATUS_TEXT, error);
    }
    else
        SetDlgItemText(IDC_STATUS_TEXT, "Connection Closed");
    
    // Set text on button and flag to redefine it as the
    // way to restart a session.
    //
    SetDlgItemText(IDC_START_BUTTON, "Start Session");

    
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

    //SetDlgItemText(IDC_STATUS_TEXT, "Session has been stopped.");
    m_isSessionStarted = FALSE;
    m_isConnected = FALSE;
    
}

BOOL CBlueAudioDlg::DoCreateServiceRecord()
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

    // Activate the record.
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


void CBlueAudioDlg::AddLogText(const CString &new_text)
{
	CString current_text;

	m_ChatLogEdit.GetWindowText(current_text);
	m_ChatLogEdit.SetWindowText(current_text + new_text + crlf);
	m_ChatLogEdit.LineScroll(m_ChatLogEdit.GetLineCount());
}


//////////////////////////////////////////////////////////////////////
//
//  Called when data is recieved from the port.
//
void CBlueAudioDlg::OnDataReceived (void *p_data, UINT16 len)
{
    CString msg( (LPCTSTR) p_data, len);
    AddLogText("<< " + msg);
}

//////////////////////////////////////////////////////////////////////
//
//  Called when and event is recieved on the port.
//
void CBlueAudioDlg::OnEventReceived (UINT32 event_code)
{
    // Was a connection established?
    //
    if (PORT_EV_CONNECTED & event_code)
    {
        if (m_isConnected == TRUE)
            return;

        m_isConnected = TRUE;
        
        BD_ADDR connected_bd_addr;

        if ( IsConnected(&connected_bd_addr) )
        {
			SetDlgItemText(IDC_STATUS_TEXT, "Connected(RfCommPort) with " + BdAddrToString(connected_bd_addr));

            memcpy(m_BdAddr, connected_bd_addr, BD_ADDR_LEN);
            ::PostMessage(g_hDialog, WIDCOMM_WM_CONNECT2AUDIO, 0, 0);
        }
    }

    // Did the connection go down?
    //
    if ( PORT_EV_CONNECT_ERR & event_code )
    {
        // Always stop the session.
        StopSession();
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

void CBlueAudioDlg::OnStatsButton() 
{
	// TODO: Add your control notification handler code here


	// bring up a modeless window to show the connection statistics

    if (!CConnStatsDlg::m_pDlg)
    {
        CConnStatsDlg::m_pDlg = new CConnStatsDlg (this);
        CConnStatsDlg::m_pDlg->Create ();
    }

}

void CBlueAudioDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	return;

	CDialog::OnTimer(nIDEvent);

}

LRESULT CBlueAudioDlg::OnConnect2Audio(WPARAM wParam, LPARAM lParam)
{
CString status;

    if (!m_isServer) //give the server enough time to createAudioConnection
        Sleep(5000);

    AUDIO_RETURN_CODE audio_rc = CreateAudioConnection(!m_isServer, &m_audio_handle);
    if (audio_rc != AUDIO_SUCCESS)
    {
        CString error;
        error.Format("CreateAudioConnection failed, error code %i!, audioHandle:%d",
                      audio_rc,m_audio_handle);
        SetDlgItemText(IDC_STATUS_TEXT, error);
        AddLogText(error);
        return 0;
    }
    else
        status.Format("Connecting to: %s Handle:%ld", BdAddrToString(m_BdAddr) , m_audio_handle);

    SetDlgItemText(IDC_STATUS_TEXT, status);
    return 0;
    
}

void CBlueAudioDlg :: OnAudioConnected(UINT16 audioHandle)
{
    CString status;

    status.Format("Audio Connection Created: %s Handle:%ld", BdAddrToString(m_BdAddr) , m_audio_handle);
    SetDlgItemText(IDC_STATUS_TEXT, status);
}

void CBlueAudioDlg :: OnAudioDisconnect(UINT16 audioHandle)
{
    CString status;

    status.Format("Disconnected from:%s(handle:%ld)", BdAddrToString(m_BdAddr), audioHandle);
    SetDlgItemText(IDC_STATUS_TEXT, status);
}
/********************************************************
DoESco(): open esco setting dialog box
********************************************************/
/********************************************************
DoESco(): open esco setting dialog box
********************************************************/
BOOL CBlueAudioDlg::DoESco(BOOL bChangeESco)
{
    CEScoDlg dlg;

    // The dialog needs the GUID.
    dlg.m_pEScoParams = &m_EScoParams;
    dlg.m_bChangeEsco = bChangeESco;
    
    if (dlg.DoModal() == IDOK)
    {
        if(bChangeESco)
        {
            m_chgEscoParam.max_latency = m_EScoParams.max_latency;
            m_chgEscoParam.packet_types = m_EScoParams.packet_types;
            m_chgEscoParam.retrans_effort = m_EScoParams.retrans_effort;
        }
        return TRUE;
    }
    else // else the user cancelled.
        return FALSE;
}

void CBlueAudioDlg::OnSco() 
{
    CString     msg;
    AUDIO_RETURN_CODE rc;
    
	// TODO: Add your control notification handler code here
    m_scoType = BTM_LINK_TYPE_SCO;
    rc = SetEScoMode (m_scoType,&m_EScoParams);
    msg.Format("Set to SCO mode:%d", rc);
    AddLogText(msg);
}

void CBlueAudioDlg::OnEsco() 
{
    CString     msg;
    AUDIO_RETURN_CODE rc;

	// TODO: Add your control notification handler code here
    m_scoType = BTM_LINK_TYPE_ESCO;
    if(DoESco())
    {
        rc = SetEScoMode (m_scoType,&m_EScoParams);
        msg.Format("Set to eSCO mode:%d", rc);
    }
    else
    {
        msg.Format("Set to eSCO mode cancel");
    }

    AddLogText(msg);
}

void CBlueAudioDlg::OnReadesco() 
{
    CString msg;
    tBTM_ESCO_DATA eScoData;
    AUDIO_RETURN_CODE audio_rc;
    
	// TODO: Add your control notification handler code here
    if (AUDIO_SUCCESS == (audio_rc = ReadEScoLinkData (m_audio_handle, &eScoData)))
    {
        msg.Format("rx_pkt_len:%ld, tx_pkt_len:%ld\r\nlink_type:%d,tx_interval:%d\r\nretrans_windows:%d, air_mode:%d",
                   eScoData.rx_pkt_len, eScoData.tx_pkt_len,
                   eScoData.link_type, eScoData.tx_interval,
                   eScoData.retrans_window, eScoData.air_mode);    
    }
    else
    {
        msg.Format("Read eSco Error:%d", audio_rc);
    }

    AddLogText(msg);
	
}

void CBlueAudioDlg::OnChangeesco() 
{
    CString msg;
    AUDIO_RETURN_CODE audio_rc;
	
    if(!DoESco(TRUE))
        return;

    if (AUDIO_SUCCESS == (audio_rc = ChangeEScoLinkParms(m_audio_handle, &m_chgEscoParam)))
    {
        msg.Format("CHGEScoLink successful");
    }
    else
    {
        msg.Format("CHGEScoLink error:%d", audio_rc);
    }
    AddLogText(msg);
	
}

