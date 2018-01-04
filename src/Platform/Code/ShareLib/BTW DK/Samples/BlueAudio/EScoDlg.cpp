// ESco.cpp : implementation file
//

#include "stdafx.h"
#include "blueaudio.h"
#include "EScoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEScoDlg dialog


CEScoDlg::CEScoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEScoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEScoDlg)
   
	//}}AFX_DATA_INIT
}


void CEScoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CEScoDlg)
	
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEScoDlg, CDialog)
	//{{AFX_MSG_MAP(CEScoDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEScoDlg message handlers

BOOL CEScoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

    if(m_bChangeEsco)
    {
        GetDlgItem(IDC_FMT)->EnableWindow( FALSE );
        GetDlgItem(IDC_RX)->EnableWindow( FALSE );
        GetDlgItem(IDC_TX)->EnableWindow( FALSE );
    }
    SetDlgItemInt(IDC_RX, m_pEScoParams->rx_bw);
    SetDlgItemInt(IDC_TX, m_pEScoParams->tx_bw);
    SetDlgItemInt(IDC_RETRANS, m_pEScoParams->retrans_effort);
	SetDlgItemInt(IDC_LATENCY, m_pEScoParams->max_latency);
	SetDlgItemInt(IDC_FMT, m_pEScoParams->voice_contfmt);
    SetDlgItemInt(IDC_TYPE, m_pEScoParams->packet_types);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEScoDlg::OnOK() 
{
	// TODO: Add extra validation here
	
   
    m_pEScoParams->rx_bw = GetDlgItemInt(IDC_RX);
    m_pEScoParams->tx_bw = GetDlgItemInt(IDC_TX);
    m_pEScoParams->retrans_effort = GetDlgItemInt(IDC_RETRANS);
	m_pEScoParams->max_latency = GetDlgItemInt(IDC_LATENCY);
	m_pEScoParams->voice_contfmt = GetDlgItemInt(IDC_FMT);
    m_pEScoParams->packet_types = GetDlgItemInt(IDC_TYPE);
	CDialog::OnOK();
}
