// BlueHeadphoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BlueHeadphone.h"
#include "BlueHeadphoneDlg.h"
#include "ChooseServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static const CString crlf("\r\n");

/////////////////////////////////////////////////////////////////////////////
// CBlueHeadphoneDlg dialog
static GUID service_guid = CBtIf:: guid_SERVCLASS_AUDIO_SINK;

static CString BdAddrToString(BD_ADDR bd_addr);

void OnHAGStatusChangeCB(void *userData, BD_ADDR bda, DEV_CLASS dev_class, BD_NAME bd_name, long lHandle, long lStatus)
{
    CString msg;
    CBlueHeadphoneDlg *pThis = (CBlueHeadphoneDlg *)userData;

    msg.Format("Status:%d, handle:%lx\n",lStatus, lHandle);
    pThis->AddLogText(msg);

    pThis->m_handle = lHandle;
}

CBlueHeadphoneDlg::CBlueHeadphoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlueHeadphoneDlg::IDD, pParent),
      m_isConnected(FALSE),
      m_handle(0)
{
	//{{AFX_DATA_INIT(CBlueHeadphone)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    WIDCOMMSDK_Init();
}


CBlueHeadphoneDlg :: ~CBlueHeadphoneDlg()
{
    if (m_handle)
        DisconnectHeadphone(m_handle);

    m_handle = 0;

}


void CBlueHeadphoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlueHeadphone)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
   DDX_Control(pDX, IDC_LOG_EDIT, m_LogEdit);
}

BEGIN_MESSAGE_MAP(CBlueHeadphoneDlg, CDialog)
	//{{AFX_MSG_MAP(CBlueHeadphone)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_START_BUTTON, OnStartButton)
	ON_BN_CLICKED(IDC_DISCONNECT, OnDisconnect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlueHeadphone message handlers

BOOL CBlueHeadphoneDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CBlueHeadphoneDlg::OnPaint() 
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
HCURSOR CBlueHeadphoneDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void CBlueHeadphoneDlg::OnStartButton() 
{
    CString msg;
    HEADPHONE_RETURN_CODE ret;
     

    if ( m_isConnected == TRUE)
        return;

    if (! DoChooseServer())
        return;     // User did not choose a server!
     
    ret = RegStatusChangeCB (OnHAGStatusChangeCB, this);
    if (ret != SUCCESS)
    {
        msg.Format("Set Callback failed:%lx", ret);
        return;
    }

    ret = ConnectHeadphone(m_BdAddr, m_serviceName);
    
    msg.Format("ConnectHeadphone:%lx", ret);
    SetDlgItemText(IDC_STATUS_TEXT, msg);
}

void CBlueHeadphoneDlg::OnDisconnect() 
{
   if (!m_handle)
       return;

   HEADPHONE_RETURN_CODE ret = DisconnectHeadphone(m_handle);

   m_handle = 0;
   
   CString msg;

   msg.Format("DisconnectHeadphone:%lx", ret);
   SetDlgItemText(IDC_STATUS_TEXT, msg);
   m_isConnected = FALSE;
}

BOOL CBlueHeadphoneDlg::DoChooseServer()
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

         //need to get  record
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

void CBlueHeadphoneDlg::AddLogText(const CString &new_text)
{
	CString current_text;

	m_LogEdit.GetWindowText(current_text);
	m_LogEdit.SetWindowText(current_text + new_text + crlf);
	m_LogEdit.LineScroll(m_LogEdit.GetLineCount());
}

//////////////////////////////////////
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
