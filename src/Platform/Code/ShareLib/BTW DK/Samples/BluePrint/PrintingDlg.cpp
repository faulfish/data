/****************************************************************************/
/*                                                                          */
/*  Name        PrintingDlg.cpp                                             */
/*                                                                          */
/*  Function    A dialog shown during printing. It has a Cancel button to   */
/*              let the user terminate the printing. It uses two time-outs: */
/*              1) A printer timeout, which is the max time between sending */
/*                 packets. If it times out, then no packets are being sent */
/*                 which indicates some sort of error.                      */
/*              2) Wakeup timout, which is used once - at init time - to    */
/*                 the GWES refreshing the dialog box before starting the   */
/*                 actual monitoring.                                       */
/*                                                                          */
/*  Date        Modification                                                */
/*  ------------------------                                                */
/*  2001-11-30  JLG         Creation                                        */
/*                                                                          */
/*  Copyright (c) 2000 - 2006, Broadcom Corporation, All Rights Reserved.   */
/*                                                                          */
/****************************************************************************/

#include "stdafx.h"
#include "resource.h"
#include "btifClasses.h"
#include "PrintingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const int   WAKEUP_TIMEOUT = 500;           // 500ms

static char type_string_text[]   = "text/plain";
static char type_string_pcl[]    = "application/vnd.hp-PCL:3C";
static char type_string_xhtml[]  = "application/vnd.pwg-xhtml-print+xml:1.0";
static char type_string_pdf[]    = "application/PDF";
static char type_string_jpeg[]   = "image/jpeg";
static char type_string_gif[]    = "image/gif:89A";

/////////////////////////////////////////////////////////////////////////////
// CPrintingDlg dialog

BEGIN_MESSAGE_MAP(CPrintingDlg, CDialog)
    //{{AFX_MSG_MAP(CPrintingDlg)
    ON_WM_TIMER()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

CPrintingDlg::CPrintingDlg(CPrintClient::ePRINT_PROFILE  ProfileToUse, BD_ADDR bd_addr, LPCTSTR pszFileToPrint, CWnd* pParent /*=NULL*/):
    m_sFileToPrint(pszFileToPrint),
    CDialog(CPrintingDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CPrintingDlg)
    //}}AFX_DATA_INIT

    ASSERT(pszFileToPrint != NULL);

    m_bIsPrintActive  = FALSE;
    m_PrintProfile    = ProfileToUse;
	memcpy(m_bdaddr, bd_addr, BD_ADDR_LEN);
    
}

CPrintingDlg::~CPrintingDlg()
{
    if (m_pPrintClient)
    {
        m_pPrintClient->Cancel();
        delete m_pPrintClient;
        m_pPrintClient = NULL;
    }
}

void CPrintingDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPrintingDlg)
    DDX_Control(pDX, IDC_CANCEL_STATIC, m_cCancelStatic);
    DDX_Control(pDX, IDCANCEL, m_cCancelBtn);
    //}}AFX_DATA_MAP
}

BOOL CPrintingDlg::OnInitDialog() 
{
    CDialog::OnInitDialog();

    // Make sure the cancel static & button are invisible
    m_cCancelStatic.ShowWindow(SW_HIDE);
    m_cCancelBtn.ShowWindow(SW_HIDE);

    // Give some time for the dlg to show up.
    SetTimer(1, 100, NULL);

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPrintingDlg::StartPrinting()
{
    TCHAR           szMsg[1024];
	CPrintClient::ePRINT_ERROR    Err;
    char            *p_type = NULL;

    // Then start printing the file.
    _stprintf(szMsg, _T("Printing %s..."), m_sFileToPrint);
    SetDlgItemText(IDC_STATUS, szMsg);

    m_pPrintClient = new CPrintClient;
	

    // For BPP, we need tos et the file type
    p_type = type_string_text;
    
	BTPRINTSTRUCT pntStruct;

	pntStruct.pszType = (LPCTSTR)p_type;
	pntStruct.mask = BPSF_TYPE;
	pntStruct.dwSize = sizeof(BTPRINTSTRUCT);
	
	Err = m_pPrintClient->Start (m_bdaddr, m_PrintProfile, m_sFileToPrint, &pntStruct);

    if (Err != CPrintClient::PRINT_RC_OK)
    {
        TCHAR   *pErrMsg;

        m_pPrintClient->GetLastError(&pErrMsg);
        wsprintf (szMsg, "Could not start printer, error (%d): %s",  Err, pErrMsg);
        AfxMessageBox(szMsg);
        EndDialog(IDABORT);
        return;
    }

    m_bIsPrintActive = TRUE;

    // Show the Cancel static & button.
    m_cCancelStatic.ShowWindow(SW_SHOW);
    m_cCancelBtn.ShowWindow(SW_SHOW);

    // Finally, set a timer to monitor the printing.
    SetTimer(2, WAKEUP_TIMEOUT, NULL);

    return;
}

void CPrintingDlg::MonitorPrinting()
{
    // Sow how many bytes were printed
    TCHAR           szBuffer[128];
	CPrintClient::ePRINT_STATE    state;
	CPrintClient::ePRINT_ERROR    Err;
    TCHAR           *pErrMsg;

    state = m_pPrintClient->GetState();

    switch (state)
    {
    case CPrintClient::PRINT_STATE_IDLE:
        SetDlgItemText(IDC_BYTE_COUNT, "Idle");
        break;

    case CPrintClient::PRINT_STATE_CONNECTING: 
        SetDlgItemText(IDC_BYTE_COUNT, "Connecting");
        break;

    case CPrintClient::PRINT_STATE_PRINTING:
        _stprintf(szBuffer, _T("Sent %u bytes of %u"), m_pPrintClient->GetBytesSent(), m_pPrintClient->GetBytesTotal());
        SetDlgItemText(IDC_BYTE_COUNT, szBuffer);
        break;

    case CPrintClient::PRINT_STATE_FLOW_CONTROLLED:
        m_bIsPrintActive = FALSE;
        if (AfxMessageBox(_T("The printer is not responding. Do you want to wait a few more seconds?"), MB_ICONQUESTION | MB_YESNO) == IDNO)
            OnCancel();
        else
            m_bIsPrintActive = TRUE;

    case CPrintClient::PRINT_STATE_DISCONNECTING:
        SetDlgItemText(IDC_BYTE_COUNT, "Disconnecting");
        break;

    case CPrintClient::PRINT_STATE_DONE:
        m_bIsPrintActive = FALSE;

        Err = m_pPrintClient->GetLastError(&pErrMsg);

        if (Err == CPrintClient::PRINT_RC_FILE_PRINTED_OK)
            AfxMessageBox(_T("Print Job finished OK\n"));
        else
        {
            wsprintf (szBuffer, "Print failed, error (%d): %s",  Err, pErrMsg);
            AfxMessageBox(szBuffer);
        }
        OnOK();
        break;
    }
}


/////////////////////////////////////////////////////////////////////////////
// CPrintingDlg message handlers

void CPrintingDlg::OnTimer(UINT_PTR nIDEvent) 
{
    switch (nIDEvent)
    {
    case 1:
        KillTimer(1);
        StartPrinting();
        break;

    case 2:
        if (m_bIsPrintActive)
            MonitorPrinting();
        break;

    default:
        // Pass to default.
        CDialog::OnTimer(nIDEvent);
    }
}

void CPrintingDlg::OnOK() 
{
    m_bIsPrintActive = FALSE;

    if (m_pPrintClient)
    {
        m_pPrintClient->Cancel();
        delete m_pPrintClient;
        m_pPrintClient = NULL;
    }

    KillTimer(2);
    CDialog::OnOK();
}

void CPrintingDlg::OnCancel() 
{
    m_bIsPrintActive = FALSE;

    if (m_pPrintClient)
    {
        m_pPrintClient->Cancel();
        delete m_pPrintClient;
        m_pPrintClient = NULL;
    }

    KillTimer(2);
    CDialog::OnCancel();
}


