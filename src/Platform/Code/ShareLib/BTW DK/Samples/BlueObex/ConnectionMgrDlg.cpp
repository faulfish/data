// ConnectionMgrDlg.cpp : implementation file
//
//  Copyright (c) 2000-2005, Broadcom Corporation, All Rights Reserved.

#include "stdafx.h"
#include "BlueObex.h"
#include "checkheaders.h"
#include "ConnectionMgrDlg.h"
#include "ChooseServerDlg.h"
#include "btversinfo.h"

static char m_serviceNameForServer[BT_MAX_SERVICE_NAME_LEN + 1] = "WIDCOMM DK OBEX Service";
static const CString crlf("\r\n");

// forward references
static CString BdAddrToString(BD_ADDR bd_addr);
static CString GetErrorString(tOBEX_ERRORS code);
static CString GetResponseString(tOBEX_RESPONSE_CODE rsp);
static CString GetTargetString(UINT8 * p_m_target);
static CString GetHeadersString(CObexHeaders *p_headers);
static CString GetBodyString(CObexHeaders *p_headers);

// {98395DEB-7184-4287-8AEE-E78DF5224F8E}
static GUID service_guid = 
{ 0x98395deb, 0x7184, 0x4287, { 0x8a, 0xee, 0xe7, 0x8d, 0xf5, 0x22, 0x4f, 0x8e } };

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
    m_strVersion = _T("BlueObex Version: ");
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


CHeadersListEntry::CHeadersListEntry ():
	obex_req_resp(0),
	mtu(0),
	scn(0),
	final(FALSE),
	backup(FALSE),
	create(FALSE)
{
	p_object = new CObexHeaders;
	memset(bd_addr, 0xff, sizeof(BD_ADDR));
}

 
CHeadersListEntry::CHeadersListEntry (WCHAR * p_file_name)
{
	p_object = new CObexHeaders;
	p_object->SetName(p_file_name);
	file = p_file_name;
}

CHeadersListEntry::~CHeadersListEntry()
{
	if (p_object)
		delete p_object;
}

CHeadersListEntry::CHeadersListEntry(const CHeadersListEntry & x)
{
	file			= x.file;
	obex_req_resp	= x.obex_req_resp;
	mtu				= x.mtu;		
	memcpy (bd_addr, x.bd_addr, sizeof (BD_ADDR));	
	scn				= x.scn;		
	final			= x.final;		
	backup			= x.backup;		
	create			= x.create;		
	p_object = new  CObexHeaders;

	UINT32 length;
	if (x.p_object->GetNameLength(&length))
	{
		WCHAR * p_name = (WCHAR *) malloc (length * sizeof (WCHAR));
		x.p_object->GetName(p_name);
		p_object->SetName(p_name);
		free (p_name);
	}
}

CHeadersListEntry& CHeadersListEntry::operator= (const CHeadersListEntry & x)
{
	file			= x.file;
	obex_req_resp	= x.obex_req_resp;
	mtu				= x.mtu;		
	memcpy (bd_addr, x.bd_addr, sizeof (BD_ADDR));	
	scn				= x.scn;		
	final			= x.final;		
	backup			= x.backup;		
	create			= x.create;
	if (p_object != NULL)
		delete p_object;

	p_object = new  CObexHeaders;

	UINT32 length;
	if (x.p_object->GetNameLength(&length))
	{
		WCHAR * p_name = (WCHAR *) malloc (length * sizeof (WCHAR));
		x.p_object->GetName(p_name);
		p_object->SetName(p_name);
		free (p_name);
	}
		return *this;
}

/////////////////////////////////////////////////////////////////////////////
// CConnectionMgrDlg dialog


CConnectionMgrDlg::CConnectionMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectionMgrDlg::IDD, pParent),
	m_pSdpService(NULL),
	m_isSessionStarted(FALSE),
	m_isServer(FALSE),
	m_pBtIf(NULL),
	m_isConnected(FALSE),
	m_scn(0),
	p_m_target(NULL),
	m_mtu(OBEX_DEFAULT_MTU),
	client_list_index(0),
	client_list_max(0),
    server_list_index(0),
    server_list_max(0)
{
	//{{AFX_DATA_INIT(CConnectionMgrDlg)
	//}}AFX_DATA_INIT
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


	// initialize fixed headers objects
	WCHAR object_name_open[] = L"Client Open Object";
	client_open_headers.SetName(object_name_open);

	WCHAR object_name_close[] = L" Close Object";
	WCHAR object_description_close[] = L" Close Description";
	client_close_headers.SetName(object_name_close);
	client_close_headers.SetDescription(object_description_close);

}


void CConnectionMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectionMgrDlg)
	DDX_Control(pDX, IDC_SEND_RECEIVE_LOG, m_SendReceiveLog);
	DDX_Control(pDX, IDC_SERVER_CHECK, m_ServerCheck);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectionMgrDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectionMgrDlg)
	ON_WM_SYSCOMMAND()
	ON_LBN_DBLCLK(IDC_SEND_RECEIVE_LIST, OnDblclkSendReceiveList)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
    ON_WM_CLOSE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectionMgrDlg message handlers
void CConnectionMgrDlg::OnStartButton() 
{
    // TODO: Add your control notification handler code here
    if (! m_isSessionStarted)
    {
        // Are we a Obex Server (or a Client)?
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
//            memset(m_serviceName, 0, BT_MAX_SERVICE_NAME_LEN + 1);
            // use compiled in service name for client also
            memcpy(m_serviceName, m_serviceNameForServer, BT_MAX_SERVICE_NAME_LEN + 1);

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

void CConnectionMgrDlg::OnDblclkSendReceiveList() 
{
	// TODO: Add your control notification handler code here
	
}

void CConnectionMgrDlg::OnTimer(UINT_PTR nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnTimer(nIDEvent);
}

void CConnectionMgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CConnectionMgrDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	
	// Do not call CDialog::OnPaint() for painting messages
}

extern "C" BOOL CheckHeaders(CString str);

BOOL CConnectionMgrDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_timer_id = SetTimer (1, 200, NULL);
	
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
	// TODO: Add extra initialization here

	// setup some objects for testing
	init_objects();

	// run  exerciser for services to a CObexHeaders object
	CString result;
	result = checker.Fill();
	AddLogText(result);

	result = checker.Verify();
	AddLogText(result);

	AddLogText ("Proceed to Start session as client or server");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectionMgrDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
    if (m_isSessionStarted)
        StopSession();

	// TODO: Add your message handler code here
	if (p_m_target != NULL)
	{
		free (p_m_target);
		p_m_target = NULL;
	}
	
	m_client_list.RemoveAll();
    m_server_list.RemoveAll();

}
                                                                                                             
void CConnectionMgrDlg::init_objects()		// setup some objects for testing
{
    // construct a list of client objects to be sent to server to exercise the OBEX protocol layer
    // Simple PUT request - has a non-empty body header
    WCHAR c_object_name_0[] = L"Put Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_0));
    char c_object_body_0[] = "Put Body ";
    m_client_list[0].p_object->SetBody((UINT8 *)c_object_body_0, (UINT32) strlen (c_object_body_0), TRUE);
    m_client_list[0].final = TRUE;
    m_client_list[0].obex_req_resp = OBEX_REQ_PUT;

    // Simple GET request
    WCHAR c_object_name_1[] = L"Get Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_1));
    char c_object_body_1[] = "Get Body";
    m_client_list[1].p_object->SetBody((UINT8 *)c_object_body_1, (UINT32) strlen (c_object_body_1), TRUE);
    m_client_list[1].final = TRUE;
    m_client_list[1].obex_req_resp = OBEX_REQ_GET;

    // SetPath request
    WCHAR c_object_name_2[] = L"Set Path Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_2));
    char c_object_body_2[] = "Set Path Body";
    m_client_list[2].p_object->SetBody((UINT8 *)c_object_body_2, (UINT32) strlen (c_object_body_2), TRUE);
    m_client_list[2].final = TRUE;
    m_client_list[2].obex_req_resp = OBEX_REQ_SETPATH;

    // PUT create request - has an 'empty' body
    WCHAR c_object_name_3[] = L"Put Create Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_3));
    m_client_list[3].p_object->SetBody(NULL, 0, TRUE);
    m_client_list[3].final = TRUE;
    m_client_list[3].obex_req_resp = OBEX_REQ_PUT;

    // PUT delete request - has no body header at all
    WCHAR c_object_name_4[] = L"Put Delete Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_4));
    m_client_list[4].final = TRUE;
    m_client_list[4].obex_req_resp = OBEX_REQ_PUT;	// no body at all

    // three part GET - server sends part of the body in each of the three GET responses
    WCHAR c_object_name_5[] = L"Get Request Part 1 of 3";
    m_client_list.Add(CHeadersListEntry(c_object_name_5));
    m_client_list[5].final = TRUE;
    m_client_list[5].obex_req_resp = OBEX_REQ_GET;

    WCHAR c_object_name_6[] = L"Get Request Part 2 of 3";
    m_client_list.Add(CHeadersListEntry(c_object_name_6));
    m_client_list[6].final = TRUE;
    m_client_list[6].obex_req_resp = OBEX_REQ_GET;

    WCHAR c_object_name_7[] = L"Get Request Part 3 of 3";
    m_client_list.Add(CHeadersListEntry(c_object_name_7));
    m_client_list[7].final = TRUE;
    m_client_list[7].obex_req_resp = OBEX_REQ_GET;

#ifndef OBEX_ABORT_NOT_OK
    // multi-part GET interrupted by an ABORT from the client after the first response
    WCHAR c_object_name_8[] = L"Get Request Abort Part 1";
    m_client_list.Add(CHeadersListEntry(c_object_name_8));
    m_client_list[8].final = TRUE;
    m_client_list[8].obex_req_resp = OBEX_REQ_GET;

    // client aborts the multi-part GET
    WCHAR c_object_name_9[] = L"Abort Get Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_9));
    char c_object_body_9[] = "Abort Request Body Part";
    m_client_list[9].p_object->SetBody((UINT8 *)c_object_body_9, (UINT32) strlen (c_object_body_9), TRUE);
    m_client_list[9].obex_req_resp = OBEX_REQ_ABORT;
#else
    // avoids ABORT confirm problem from WIN CE
    WCHAR c_object_name_8[] = L"Simple Get Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_8));
    m_client_list[8].final = TRUE;
    m_client_list[8].obex_req_resp = OBEX_REQ_GET;

    WCHAR c_object_name_9[] = L"Simple Put Request";
    m_client_list.Add(CHeadersListEntry(c_object_name_9));
    char c_object_body_9[] = "Simple Put Request Body";
    m_client_list[9].p_object->SetBody((UINT8 *)c_object_body_9, (UINT32) strlen (c_object_body_9), TRUE);
    m_client_list[9].obex_req_resp = OBEX_REQ_PUT;
    m_client_list[9].final = TRUE;
#endif

    // three part PUT - the body is sent in the 2nd and 3rd part, no body in the 1st part
    WCHAR c_object_name_10[] = L"Put Request Part 1 of 3";
    m_client_list.Add(CHeadersListEntry(c_object_name_10));
    char c_object_body_10a[] = "Put Body Part 1 of 2";
    char c_object_body_10b[] = "Put Body Part 2 of 2";
    // total length of all body parts sent in 1st block
    m_client_list[10].p_object->SetLength((UINT32) strlen(c_object_body_10a) + (UINT32) strlen(c_object_body_10b));
    m_client_list[10].final = FALSE;	// no body in first PUT request
    m_client_list[10].obex_req_resp = OBEX_REQ_PUT;

    WCHAR c_object_name_11[] = L"Put Request Part 2 of 3";
    m_client_list.Add(CHeadersListEntry(c_object_name_11));
    m_client_list[11].p_object->SetBody((UINT8 *)c_object_body_10a, (UINT32) strlen (c_object_body_10a), FALSE);
    m_client_list[11].final = FALSE;
    m_client_list[11].obex_req_resp = OBEX_REQ_PUT;

    WCHAR c_object_name_12[] = L"Put Request Part 3 of 3";
    m_client_list.Add(CHeadersListEntry(c_object_name_12));
    m_client_list[12].p_object->SetBody((UINT8 *)c_object_body_10b, (UINT32) strlen (c_object_body_10b), TRUE);
    m_client_list[12].final = TRUE;
    m_client_list[12].obex_req_resp = OBEX_REQ_PUT;


    // Server responses to the client sequence above
    WCHAR s_object_name_0[] = L"ServerResponse0";
    m_server_list.Add(CHeadersListEntry(s_object_name_0));
    m_server_list[0].final = TRUE;
    WCHAR s_object_description_0[] = L"Server Description 0";
    m_server_list[0].p_object->SetDescription(s_object_description_0);
    m_server_list[0].obex_req_resp = OBEX_RSP_OK;

    WCHAR s_object_name_1[] = L"ServerResponse1";
    m_server_list.Add(CHeadersListEntry(s_object_name_1));
    m_server_list[1].final = TRUE;
    WCHAR s_object_description_1[] = L"Server Description 1";
    m_server_list[1].p_object->SetDescription(s_object_description_1);
    char s_object_body_1[] = "Get Response Body";
    m_server_list[1].p_object->SetBody((UINT8 *)s_object_body_1, (UINT32) strlen (s_object_body_1), TRUE);
    m_server_list[1].obex_req_resp = OBEX_RSP_OK;

    WCHAR s_object_name_2[] = L"ServerResponse2";
    m_server_list.Add(CHeadersListEntry(s_object_name_2));
    m_server_list[2].final = TRUE;
    m_server_list[2].obex_req_resp = OBEX_RSP_OK;

    WCHAR s_object_name_3[] = L"ServerResponse3";
    m_server_list.Add(CHeadersListEntry(s_object_name_3));
    m_server_list[3].p_object->SetBody(NULL, 0, TRUE);
    m_server_list[3].final = TRUE;
    m_server_list[3].obex_req_resp = OBEX_RSP_OK;

    WCHAR s_object_name_4[] = L"ServerResponse4";
    m_server_list.Add(CHeadersListEntry(s_object_name_4));
    m_server_list[4].final = TRUE;
    m_server_list[4].obex_req_resp = OBEX_RSP_OK;

    WCHAR s_object_name_5[] = L"Get Response Part 1 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_5));
    char s_server_body_5[] = "Get Response Body Part 1 of 3 ";
    char s_server_body_6[] = "Get Response Body Part two of three ";
    char s_server_body_7[] = "Get Response Body Part three of three";
    m_server_list[5].p_object->SetBody((UINT8 *)s_server_body_5, (UINT32) strlen (s_server_body_5), FALSE);
    m_server_list[5].p_object->SetLength((UINT32)(strlen(s_server_body_5) + strlen(s_server_body_6) + strlen(s_server_body_7)));
    m_server_list[5].final = TRUE;
    m_server_list[5].obex_req_resp = OBEX_RSP_CONTINUE;

    WCHAR s_object_name_6[] = L"Get Response Part 2 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_6));
    m_server_list[6].p_object->SetBody((UINT8 *)s_server_body_6, (UINT32) strlen (s_server_body_6), FALSE);
    m_server_list[6].final = TRUE;
    m_server_list[6].obex_req_resp = OBEX_RSP_CONTINUE;

    WCHAR s_object_name_7[] = L"Get Response Part 3 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_7));
    m_server_list[7].p_object->SetBody((UINT8 *)s_server_body_7, (UINT32) strlen (s_server_body_7), TRUE);
    m_server_list[7].final = TRUE;
    m_server_list[7].obex_req_resp = OBEX_RSP_OK;

#ifndef OBEX_ABORT_NOT_OK
    WCHAR s_object_name_8[] = L"Get Response Part 1 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_5));
    char s_server_body_8a[] = "Get Response (ABORT) Body Part 1 of 3 ";
    char s_server_body_8b[] = "Get Response (ABORT) Body Part two of three ";	// never sent
    char s_server_body_8c[] = "Get Response (ABORT) Body Part three of three";	// never sent
    m_server_list[8].p_object->SetBody((UINT8 *)s_server_body_8a, (UINT32) strlen (s_server_body_8a), FALSE);
    m_server_list[8].p_object->SetLength((UINT32)(strlen(s_server_body_8a) + strlen(s_server_body_8b) + strlen(s_server_body_8c)));
    m_server_list[8].final = TRUE;
    m_server_list[8].obex_req_resp = OBEX_RSP_CONTINUE;

    WCHAR s_object_name_9[] = L"Response to Abort";
    m_server_list.Add(CHeadersListEntry(s_object_name_9));
    m_server_list[9].final = TRUE;
    m_server_list[9].obex_req_resp = OBEX_RSP_OK;
#else
    WCHAR s_object_name_8[] = L"Get Response Part 1 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_5));
    char s_server_body_8a[] = "Simple Get Response";
    m_server_list[8].p_object->SetBody((UINT8 *)s_server_body_8a, strlen (s_server_body_8a), TRUE);
    m_server_list[8].p_object->SetLength(strlen(s_server_body_8a));
    m_server_list[8].final = TRUE;
    m_server_list[8].obex_req_resp = OBEX_RSP_OK;

    WCHAR s_object_name_9[] = L"Response to Simple Put";
    m_server_list.Add(CHeadersListEntry(s_object_name_9));
    m_server_list[9].final = TRUE;
    m_server_list[9].obex_req_resp = OBEX_RSP_OK;
#endif

    WCHAR s_object_name_10[] = L"Put Response Part 1 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_10));
    m_server_list[10].final = TRUE;
    m_server_list[10].obex_req_resp = OBEX_RSP_CONTINUE;

    WCHAR s_object_name_11[] = L"Put Response Part 2 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_11));
    m_server_list[11].final = TRUE;
    m_server_list[11].obex_req_resp = OBEX_RSP_CONTINUE;

    WCHAR s_object_name_12[] = L"Put Response Part 3 of 3";
    m_server_list.Add(CHeadersListEntry(s_object_name_12));
    m_server_list[12].final = TRUE;
    m_server_list[12].obex_req_resp = OBEX_RSP_OK;

}

void CConnectionMgrDlg::StartSession ()
{
    CString status;

    // If we are an Obex Server create the service record.  If
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
  
    // Prepare for connection
    // 
    if ( m_isServer )
    {
        tOBEX_ERRORS rsp;

        rsp = Register(m_scn, p_m_target);

        CString msg;

        msg.Format(" scn: %d, %s, %s", m_scn, GetErrorString(rsp), GetTargetString(p_m_target));

        AddLogText(">> REGISTER: " + msg);

        if (rsp != OBEX_SUCCESS)
        {
            CString error;
            error.Format("CObexServer::OBEX_ServerRegister, error code %i!", rsp);
            SetDlgItemText(IDC_STATUS_TEXT, error);
            return;
        }

        status.Format("Server is now waiting for client connection");
    }
    else
    {
        tOBEX_ERRORS rsp;

        rsp = CObexClient::Open(m_scn, m_BdAddr, &client_open_headers, m_mtu);
        status = "Attempting a connection with BDA: " + BdAddrToString(m_BdAddr);

        CString msg;

        msg.Format(" scn: %d, %s, server is: %s", m_scn, GetErrorString(rsp), BdAddrToString(m_BdAddr));

        AddLogText(">> OPEN: " + msg);

        if (rsp != OBEX_SUCCESS)
        {
            CString error;
            error.Format("CObexClient::Open failed, error code %i!", rsp);
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

void CConnectionMgrDlg::StopSession ()
{
    // Depending on our mode close the port
    // 
    if ( m_isServer )
    {
        tOBEX_ERRORS rsp;

        rsp = Unregister();

        if (rsp != OBEX_SUCCESS)
        {
            CString error;
            error.Format("CObexServer::Unregister failed,, error code %i", rsp);
            SetDlgItemText(IDC_STATUS_TEXT, error);
            Sleep (5000);
        }
    }
    else
    {
        m_isConnected = FALSE;
        SetDlgItemText(IDC_STATUS_TEXT, "Session closed by user");

        tOBEX_ERRORS rsp;

        rsp = CObexClient::Close(&client_close_headers);

        if (rsp != OBEX_SUCCESS)
        {
            CString error;
            error.Format("CObexClient::Close, error code %i", rsp);
            SetDlgItemText(IDC_STATUS_TEXT, error);
        }
        else
        {
            AddLogText(">> CLOSE: ");
        }
    }

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

    SetDlgItemText(IDC_STATUS_TEXT, "Session has been stopped.");
    m_isSessionStarted = FALSE;
}

void CConnectionMgrDlg::AddLogText(const CString &new_text)
{
    CString current_text;

    m_SendReceiveLog.GetWindowText(current_text);
    m_SendReceiveLog.SetWindowText(current_text + new_text + crlf);
    m_SendReceiveLog.LineScroll(m_SendReceiveLog.GetLineCount());
}

BOOL CConnectionMgrDlg::DoChooseServer()
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
//        memcpy(m_serviceName, dlg.m_serviceName, BT_MAX_SERVICE_NAME_LEN + 1);
        m_Name = dlg.m_Name;    // friendly name
        m_scn = dlg.m_Scn;		// scn used by selected server

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

BOOL CConnectionMgrDlg::DoCreateServiceRecord()
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
    
    m_scn = m_RfCommIf.GetScn();

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

CString GetErrorString( tOBEX_ERRORS code)
{
	if (code == OBEX_SUCCESS)
		return "SUCCESS";				// just show failures
	else
	{
		CString msg;
		msg.Format("ERROR: %d", code);	// limit in case array not terminated
		return msg;
	}
}

CString GetResponseString( tOBEX_RESPONSE_CODE response)
{
	if (response == OBEX_RSP_OK)
		return "OK";				// just show failures
	else
	{
		CString msg;
		msg.Format("RESPONSE: %d", response);	// limit in case array not terminated
		return msg;
	}
}

CString GetTargetString(UINT8 * p_m_target)
{

	if (p_m_target == NULL)
		return "target: NULL";
	else
	{
		CString msg;
		msg.Format("target: \'%20.20s\'", p_m_target);	// limit in case array not terminated
		return msg;
	}
}

CString GetHeadersString(CObexHeaders *p_headers)
{
	CString msg;
	if (p_headers == NULL)
		return "headers NULL";
	else
	{
		UINT32 length;
		// object name?
		if (p_headers->GetNameLength(&length))
		{
			WCHAR * p_object_name = (WCHAR *) malloc (length * sizeof (WCHAR));
			p_headers->GetName(p_object_name);
			char * p_object_name_char = (char *) malloc (length*2);
			wcstombs(p_object_name_char, p_object_name, length);
			msg.Format("Name: \'%*.*s\'", length, length, p_object_name_char);
			free (p_object_name);
			free (p_object_name_char);
			return msg;
		}
		else
			return "No object name header";
	}
}

CString GetBodyString(CObexHeaders *p_headers)
{
	if (p_headers == NULL)
		return "headers NULL";
	else
	{
		UINT32 length;
		CString msg;
		// object name?
		if (p_headers->GetBodyLength(&length))
		{
			UINT8 * p_object_body = (UINT8 *) malloc (length);
			BOOL body_end;

			p_headers->GetBody(p_object_body, &body_end);
			msg.Format("Body: \'%*.*s\' %s", length, length, p_object_body, body_end ? "BODY END" : "");
			free (p_object_body);
			return msg;
		}
		else
			return "No object body header";
	}
}

///////////////////////////////////////////////////////////
////////////////  Methods derived from CObexClient ////////
///////////////////////////////////////////////////////////

void CConnectionMgrDlg::OnOpen(CObexHeaders *p_confirm, UINT16 tx_mtu, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response)
{
	CString msg;

	msg.Format("%s %s %s tx_mtu: %d", GetErrorString(code), GetResponseString(response), GetHeadersString(p_confirm), tx_mtu);

	AddLogText("<< OPEN CONFIRM: " + msg);


	msg.Format("Code %s %s", GetErrorString(code), GetResponseString(response));
    SetDlgItemText(IDC_STATUS_TEXT, msg);

	// init client msg index
	client_list_max = (int) m_client_list.GetSize();

	client_list_index = 0;
    
    // To test SetWriteTimeOut, put breakpoint in SendNextServerObject on server side before one of the object response cases
    // Should see client side timeout after 10 seconds, instead of expected 20 (default HCI link supervision timeout)
    SetWriteTimeOut(10);  

	SendNextClientObject();		

}

void CConnectionMgrDlg::OnClose(CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response)
{
	CString msg;

	msg.Format("%s %s %s", GetErrorString(code), GetResponseString(response), GetHeadersString(p_confirm));

	AddLogText("<< CLOSE CONFIRM: " + msg);

	msg.Format("Code %s %s", GetErrorString(code), GetResponseString(response));
    SetDlgItemText(IDC_STATUS_TEXT, "Close confirm. " + msg);
    if (!m_isServer)
        StopSession();
}

void CConnectionMgrDlg::OnAbort (CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response)
{
	CString msg;

	msg.Format("%s %s, %s", GetErrorString(code), GetResponseString(response), GetHeadersString(p_confirm));

	AddLogText("<< ABORT CONFIRM: " + msg);

	msg.Format("Code %s %s", GetErrorString(code), GetResponseString(response));
    SetDlgItemText(IDC_STATUS_TEXT, "Abort confirm. " + msg);

	SendNextClientObject();		
}

void CConnectionMgrDlg::OnPut (CObexHeaders *p_confirm, tOBEX_ERRORS  code, tOBEX_RESPONSE_CODE response)
{
	CString msg;

	msg.Format("%s %s %s", GetErrorString(code), GetResponseString(response), GetHeadersString(p_confirm));

	AddLogText("<< PUT CONFIRM: " + msg);

	SendNextClientObject();		
}

void CConnectionMgrDlg::OnGet (CObexHeaders *p_confirm, tOBEX_ERRORS  code, BOOL final, tOBEX_RESPONSE_CODE response)
{
	CString msg;

	msg.Format("%s %s %s %s %s", GetErrorString(code), GetResponseString(response), GetHeadersString(p_confirm), final ? "FINAL" : "", GetBodyString(p_confirm));

	AddLogText("<< GET CONFIRM: " + msg);
	SendNextClientObject();		
}

void CConnectionMgrDlg::OnSetPath (CObexHeaders *p_confirm, tOBEX_ERRORS code, tOBEX_RESPONSE_CODE response)
{
	CString msg;

	msg.Format("%s %s %s", GetErrorString(code), GetResponseString(response), GetHeadersString(p_confirm));

	AddLogText("<< SET PATH CONFIRM: " + msg);
	SendNextClientObject();		
}


///////////////////////////////////////////////////////////
////////////////  Methods derived from CObexServer ////////
///////////////////////////////////////////////////////////

void CConnectionMgrDlg::OnOpenInd(CObexHeaders *p_request)
{
	CString msg;
	BD_ADDR	bd_addr;

	GetRemoteBDAddr(bd_addr);

	msg.Format("Open Indication from %s, %s", BdAddrToString(bd_addr), GetHeadersString(p_request));
	SetDlgItemText(IDC_STATUS_TEXT, msg);

	msg.Format("From %s: %s", BdAddrToString(bd_addr), GetHeadersString(p_request));

	AddLogText("<< OPEN IND: " + msg);

#if 0
// test for user-defined target/who fix - disabled - modification of a received headers object and then tx it back caused mem leak - CR 51402
	CObexUserDefined ud;
	UINT32  cid = 0x12345678; // a value for testing
	BOOL bResult = ud.SetHeader(0x33, cid); 
	UINT8 user_type;
	UINT8 user_id;
	user_type = ud.GetUserType(&user_id);
	TRACE("Type ID of ConnectionID: type:0x%02x id:0x%02x\n", user_type, user_id);
	// user_type & user_id are set and retrieved currectly

	ASSERT(bResult); // return TRUE
	bResult = p_request->AddUserDefined(&ud);
	ASSERT(bResult); // return TRUE
	bResult = p_request->SetWho((UINT8 *)"target", (UINT32) strlen("target"));
	ASSERT(bResult); // return TRUE
	bResult = p_request->SetDescription(L"Description"); // for testing only
	ASSERT(bResult); // return TRUE
#endif

	tOBEX_ERRORS rsp;

	// send back the open confirm with the request headers
    rsp = CObexServer::OpenCnf(OBEX_SUCCESS, OBEX_RSP_OK, p_request);

	msg.Format("%d %s", rsp,  GetHeadersString(p_request));

	AddLogText(">> OPEN CNF: rtn: " + msg);

    SetDlgItemText(IDC_STATUS_TEXT, "Connection Opened");

	// init server msg index
	server_list_max = (int) m_server_list.GetSize();
	server_list_index = 0;

	SwitchRole(NEW_MASTER);

}

void CConnectionMgrDlg::OnSetPathInd (CObexHeaders * p_request, BOOL backup, BOOL create)
{
	CString msg;

	msg.Format("%s %s %s", GetHeadersString(p_request), backup ? "BACKUP" : "", create ? "CREATE" : "");

	AddLogText("<< SET PATH IND: " + msg);
	SendNextServerObject(OBEX_REQ_SETPATH);		
}

void CConnectionMgrDlg::OnPutInd (CObexHeaders * p_request, BOOL final)
{
	CString msg;

	msg.Format("%s %s %s", GetHeadersString(p_request), final ? "FINAL" : "", GetBodyString(p_request));

	AddLogText("<< PUT IND: " + msg);
	SendNextServerObject(OBEX_REQ_PUT);		
}

void CConnectionMgrDlg::OnPutCreateInd(CObexHeaders * p_request)
{
	CString msg;

	msg.Format("%s", GetHeadersString(p_request));

	AddLogText("<< PUT CREATE IND: " + msg);
	SendNextServerObject(OBEX_REQ_PUT, TRUE);		
}

void CConnectionMgrDlg::OnPutDeleteInd (CObexHeaders * p_request)
{
	CString msg;

	msg.Format("%s", GetHeadersString(p_request));

	AddLogText("<< PUT DELETE IND: " + msg);
	SendNextServerObject(OBEX_REQ_PUT, FALSE, TRUE);		
}

void CConnectionMgrDlg::OnGetInd (CObexHeaders * p_request, BOOL final)
{
	CString msg;

	msg.Format("%s %s", GetHeadersString(p_request), final ? "FINAL" : "");

	AddLogText("<< GET IND: " + msg);
	SendNextServerObject(OBEX_REQ_GET);		
}

void CConnectionMgrDlg::OnAbortInd (CObexHeaders * p_request)
{
	CString msg;

	msg.Format("%s", GetHeadersString(p_request));

	AddLogText("<< ABORT IND: " + msg);
	SendNextServerObject(OBEX_REQ_ABORT);		
}

void CConnectionMgrDlg::OnCloseInd(CObexHeaders *p_request)
{
	CString msg;

	msg.Format("%s", GetHeadersString(p_request));

	AddLogText("<< CLOSE IND: " + msg);

	tOBEX_ERRORS rsp;

	// send back the open confirm with the request headers
    rsp = CObexServer::CloseCnf(OBEX_SUCCESS, OBEX_RSP_OK, p_request);

	msg.Format("%d %s", rsp,  GetHeadersString(p_request));

	AddLogText(">> CLOSE CNF: rtn: " + msg);

    SetDlgItemText(IDC_STATUS_TEXT, "Client Closed. Waiting for a new client connection");
}

void CConnectionMgrDlg::SendNextClientObject()
{
	tOBEX_ERRORS result = OBEX_SUCCESS;
	CString msg;
	CString operation;

	if (client_list_index < client_list_max)
	{
		switch (m_client_list[client_list_index].obex_req_resp)
		{
		case	OBEX_REQ_PUT:
			operation = "PUT";
			result = Put( m_client_list[client_list_index].p_object, m_client_list[client_list_index].final);
			break;
		case	OBEX_REQ_GET:
			operation = "GET";
			result = Get( m_client_list[client_list_index].p_object, m_client_list[client_list_index].final);
			break;
		case	OBEX_REQ_SETPATH:
			operation = "SET PATH";
			result = SetPath( m_client_list[client_list_index].p_object, m_client_list[client_list_index].backup, m_client_list[client_list_index].create);
			break;
		case	OBEX_REQ_ABORT:
			operation = "ABORT";
			result = Abort( m_client_list[client_list_index].p_object);
			break;
		}

		UINT32 length;
		if (m_client_list[client_list_index].p_object->GetNameLength(&length))
		{
			WCHAR * buf = (WCHAR *) malloc (length * sizeof (WCHAR));
			m_client_list[client_list_index].p_object->GetName(buf);
			char * buf_char = (char *) malloc (length*2);
			wcstombs(buf_char, buf, length);
			msg.Format("%d Client object %d, %s \'%*s\'", result,  client_list_index, operation, length,  buf_char);
			free (buf);
			free (buf_char);
		}
		else
			msg.Format("%d Client object %d No Name", result, client_list_index);

		AddLogText(">> " + operation + " rtn: " + msg);
		client_list_index++;
	}
}

void CConnectionMgrDlg::SendNextServerObject(UINT8 request_code, BOOL create_req, BOOL delete_req)
{
	tOBEX_ERRORS result = OBEX_SUCCESS;
	CString msg;
	BOOL send_response = TRUE;

	if (server_list_index < server_list_max)
	{
		switch (request_code)
		{
		case	OBEX_REQ_PUT:
			if (create_req)
				result = PutCreateCnf( OBEX_SUCCESS, (tOBEX_RESPONSE_CODE)m_server_list[server_list_index].obex_req_resp, m_server_list[server_list_index].p_object);
			else if (delete_req)
				result = PutDeleteCnf( OBEX_SUCCESS, (tOBEX_RESPONSE_CODE)m_server_list[server_list_index].obex_req_resp, m_server_list[server_list_index].p_object);
			else
				result = PutCnf( OBEX_SUCCESS, (tOBEX_RESPONSE_CODE)m_server_list[server_list_index].obex_req_resp, m_server_list[server_list_index].p_object);
			break;
		case	OBEX_REQ_GET:
			result = GetCnf (OBEX_SUCCESS, (tOBEX_RESPONSE_CODE)m_server_list[server_list_index].obex_req_resp, m_server_list[server_list_index].final, m_server_list[server_list_index].p_object);
			break;
		case	OBEX_REQ_SETPATH:
			result = SetPathCnf (OBEX_SUCCESS, (tOBEX_RESPONSE_CODE)m_server_list[server_list_index].obex_req_resp, m_server_list[server_list_index].p_object);
			break;
		case	OBEX_REQ_ABORT:
			result = AbortCnf (OBEX_SUCCESS, (tOBEX_RESPONSE_CODE)m_server_list[server_list_index].obex_req_resp, m_server_list[server_list_index].p_object);
			break;
		default:
			send_response = FALSE;
			break;
		}

		UINT32 length;
		if (m_server_list[server_list_index].p_object->GetNameLength(&length))
		{
			WCHAR * buf = (WCHAR *) malloc (length * sizeof (WCHAR));
			m_server_list[server_list_index].p_object->GetName(buf);
			char * buf_char = (char *) malloc (length*2);
			wcstombs(buf_char, buf, length);
			msg.Format("%d Server object %d \'%*s\'", result, server_list_index, length, buf_char);
			free (buf);
			free (buf_char);
		}
		else
			msg.Format("%d Server object %d No Name", result, server_list_index );

		if (send_response)
			AddLogText(">> RESPONSE: rtn: " + msg);
		else
			AddLogText("-- No response sent");

		server_list_index++;
	}
}



LRESULT CConnectionMgrDlg::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DefWindowProc(message, wParam, lParam);
}

BOOL CConnectionMgrDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::OnCommand(wParam, lParam);
}

void CConnectionMgrDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CConnectionMgrDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
}

