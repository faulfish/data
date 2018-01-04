// LockFilesListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LockFilesListDlg.h"


// CLockFilesListDlg dialog

IMPLEMENT_DYNAMIC(CLockFilesListDlg, CDialog)

CLockFilesListDlg::CLockFilesListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLockFilesListDlg::IDD, pParent)
	, m_nSelectedRow(-1)
{

}

CLockFilesListDlg::~CLockFilesListDlg()
{
}
BOOL CLockFilesListDlg::OnInitDialog()
{
	BOOL BRet = CDialog::OnInitDialog();

	m_LockFilesListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_LockFilesListCtrl.InsertColumn(0, _T("Lock files"), LVCFMT_LEFT, 400);

	for (size_t nRowNumber = 0; nRowNumber < m_vecLockFilesList.size() ; ++nRowNumber)
	{
		m_LockFilesListCtrl.InsertItem((int)nRowNumber, m_vecLockFilesList.at(nRowNumber));
	}

	return BRet;
}

void CLockFilesListDlg::SetListCtrl(const vector<CString>& vecLockFilesList)
{
	m_vecLockFilesList = vecLockFilesList;
}

void CLockFilesListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LOCKFILESLIST, m_LockFilesListCtrl);
}


BEGIN_MESSAGE_MAP(CLockFilesListDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_LOCKFILESLIST, &CLockFilesListDlg::OnNMDblclkLockfileslist)
END_MESSAGE_MAP()


// CLockFilesListDlg message handlers

void CLockFilesListDlg::OnNMDblclkLockfileslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	m_nSelectedRow = m_LockFilesListCtrl.GetSelectionMark();
	EndDialog(0);
}
int CLockFilesListDlg::GetSelectedRow() const
{
	return m_nSelectedRow;
}
