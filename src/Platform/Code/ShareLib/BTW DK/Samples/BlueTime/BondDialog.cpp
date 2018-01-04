// BondDialog.cpp : implementation file
//

#include "stdafx.h"
#include "bluetime.h"
#include "BondDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBondDialog dialog


CBondDialog::CBondDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CBondDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBondDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBondDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBondDialog)
	DDX_Control(pDX, IDC_UNBOND_BUTTON, m_unbond_button);
	DDX_Control(pDX, IDC_BOND, m_bond_button);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBondDialog, CDialog)
	//{{AFX_MSG_MAP(CBondDialog)
	ON_BN_CLICKED(IDC_BONDEX_BUTTON, OnBondExButton)
	ON_BN_CLICKED(IDC_UNBOND_BUTTON, OnUnbondButton)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SEND, OnBnClickedSend)
	ON_BN_CLICKED(IDC_CANCEL_SEND, OnBnClickedCancelSend)
	ON_BN_CLICKED(IDC_BOND, OnBnClickedBond)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBondDialog message handlers
void BondCb(CBtIf::eBOND_CB_EVENT event, void * user_data, UINT32 event_data)
{
	if (user_data)
		((CBondDialog*)(user_data))->BondCb(event, event_data);
}

BOOL CBondDialog::BondCb(CBtIf::eBOND_CB_EVENT event, UINT32 event_data)
{
	CString str;
	switch (event)
	{
		case CBtIf::BOND_EVT_CONFIRM_REQ:
			str.Format("Confirm pin code: %d", event_data);
			m_BtIf.BondReply(
				(IDOK == MessageBox(str, "Confirm", MB_OKCANCEL)) ? 
					CBtIf::BOND_CONFIRM_ALLOW : CBtIf::BOND_CONFIRM_DISALLOW);
			SetDlgItemText(IDC_BOND_STATUS, "Bond Confirm Sent");
			break;

		case CBtIf::BOND_EVT_PASSKEY_REQ:
			str.Format("Enter this pin code on KB: %d", event_data);
			SetDlgItemText(IDC_BOND_STATUS, str);
			break;

		case CBtIf::BOND_EVT_PIN_CODE_REQ:
			SetDlgItemText(IDC_BOND_STATUS, "Enter Pin Code");
			GetDlgItem(IDC_PIN_EDIT)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEND)->EnableWindow(TRUE);
			GetDlgItem(IDC_SEND)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_CANCEL_SEND)->EnableWindow(TRUE);
			GetDlgItem(IDC_CANCEL_SEND)->ShowWindow(SW_NORMAL);
			GetDlgItem(IDC_PIN_EDIT)->SetFocus();
			break;
	}

	return TRUE;
}

UINT BondExThread(LPVOID p)
{
	CBondDialog * pDlg = (CBondDialog*)p;
	CBtIf::BOND_RETURN_CODE rc ;

	if (pDlg->IsDlgButtonChecked(IDC_BTW_UI))
		rc = pDlg->m_BtIf.BondEx(pDlg->m_BdAddr, NULL, NULL);
	else
		rc = pDlg->m_BtIf.BondEx(pDlg->m_BdAddr, ::BondCb, pDlg);

	switch (rc)
	{
	case CBtIf::SUCCESS:
		pDlg->SetDlgItemText(IDC_BOND_STATUS, _T("Bond Success"));
		pDlg->m_bond_button.EnableWindow( FALSE );
		pDlg->GetDlgItem(IDC_BONDEX_BUTTON)->EnableWindow(FALSE);
		pDlg->m_unbond_button.EnableWindow( TRUE );
		break;
	case CBtIf::BAD_PARAMETER:
		pDlg->SetDlgItemText(IDC_BOND_STATUS, _T("Bad Parameter (length must be 1 - 16 bytes)"));
		pDlg->GetDlgItem(IDC_BOND)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_BONDEX_BUTTON)->EnableWindow(TRUE);
		break;
	case CBtIf::FAIL:
		pDlg->SetDlgItemText(IDC_BOND_STATUS, _T("Bond Failed"));
		pDlg->GetDlgItem(IDC_BOND)->EnableWindow(TRUE);
		pDlg->GetDlgItem(IDC_BONDEX_BUTTON)->EnableWindow(TRUE);
		break;
	}
	pDlg->GetDlgItem(IDC_PIN_EDIT)->EnableWindow(TRUE);
	
	pDlg->GetDlgItem(IDC_SEND)->ShowWindow(SW_HIDE);
	pDlg->GetDlgItem(IDC_CANCEL_SEND)->ShowWindow(SW_HIDE);
	return 0;
}

void CBondDialog::OnBondExButton() 
{
    GetDlgItemText(IDC_PIN_EDIT,  m_pin_code);

	SetDlgItemText(IDC_BOND_STATUS, _T("Bond Attempt"));
	GetDlgItem(IDC_BOND)->EnableWindow(FALSE);
	GetDlgItem(IDC_BONDEX_BUTTON)->EnableWindow(FALSE);
	//CWaitCursor w;
	GetDlgItem(IDC_PIN_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CANCEL_SEND)->ShowWindow(SW_HIDE);

	AfxBeginThread(BondExThread, this);
	
}

void CBondDialog::OnUnbondButton() 
{
	// TODO: Add your control notification handler code here
	if (m_BtIf.UnBond(m_BdAddr))
	{
		m_bond_button.EnableWindow( TRUE );
		GetDlgItem(IDC_BONDEX_BUTTON)->EnableWindow(TRUE);
		m_unbond_button.EnableWindow( FALSE );
		SetDlgItemText(IDC_BOND_STATUS, _T("UnBond Requested"));
	}
	else
		SetDlgItemText(IDC_BOND_STATUS, _T("UnBond Could Not Be Requested"));
}

void CBondDialog::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	
}

BOOL CBondDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetDlgItemText(IDC_DEVICE, m_Name);
	SetDlgItemText(IDC_PIN_EDIT, _T(""));
	if (m_BtIf.BondQuery(m_BdAddr))
	{
		SetDlgItemText(IDC_BOND_STATUS, _T("Devices Paired"));
		m_bond_button.EnableWindow( FALSE );
		m_unbond_button.EnableWindow( TRUE );
		GetDlgItem(IDC_BONDEX_BUTTON)->EnableWindow(FALSE);
	}
	else
	{
		SetDlgItemText(IDC_BOND_STATUS, _T("Devices Not Paired"));
		m_bond_button.EnableWindow( TRUE );
		m_unbond_button.EnableWindow( FALSE );
	}
	
	//GetDlgItem(IDC_PIN_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CANCEL_SEND)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CBondDialog::OnBnClickedSend()
{
	SetDlgItemText(IDC_BOND_STATUS, "Pin Code Sent...");
	//GetDlgItem(IDC_PIN_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CANCEL_SEND)->EnableWindow(FALSE);

	PIN_CODE pin;
	GetDlgItemText(IDC_PIN_EDIT, (char*)pin, PIN_CODE_LEN-1);
	m_BtIf.BondReply(CBtIf::BOND_PIN_ALLOW, (UINT32)strlen((char*)pin),pin);
}

void CBondDialog::OnBnClickedCancelSend()
{
	SetDlgItemText(IDC_BOND_STATUS, "Pin Code Cancel Sent...");
	GetDlgItem(IDC_PIN_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_SEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_CANCEL_SEND)->EnableWindow(FALSE);
	m_BtIf.BondReply(CBtIf::BOND_PIN_DISALLOW, NULL, 0);
}

void CBondDialog::OnBnClickedBond()
{
	CBtIf::BOND_RETURN_CODE rc;

	GetDlgItemText(IDC_PIN_EDIT,  m_pin_code);

	SetDlgItemText(IDC_BOND_STATUS, _T("Bond Attempt"));
	CWaitCursor w;

	// TODO: Add your control notification handler code here
	rc = m_BtIf.Bond(m_BdAddr, m_pin_code.GetBuffer(100));
	switch (rc)
	{
	case CBtIf::SUCCESS:
		SetDlgItemText(IDC_BOND_STATUS, _T("Bond Success"));
		m_bond_button.EnableWindow( FALSE );
		GetDlgItem(IDC_BONDEX_BUTTON)->EnableWindow(FALSE);
		m_unbond_button.EnableWindow( TRUE );
		break;
	case CBtIf::BAD_PARAMETER:
		SetDlgItemText(IDC_BOND_STATUS, _T("Bad Parameter (length must be 1 - 16 bytes)"));
		break;
	case CBtIf::FAIL:
		SetDlgItemText(IDC_BOND_STATUS, _T("Bond Failed"));
		break;
	}
}
