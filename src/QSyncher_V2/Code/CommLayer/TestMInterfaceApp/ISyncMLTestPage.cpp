// ISyncMLTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "TestMInterfaceApp.h"
#include "ISyncMLTestPage.h"


// CISyncMLTestPage dialog

IMPLEMENT_DYNAMIC(CISyncMLTestPage, CDialog)

enum{
ePut = 0,
eGet
};

CISyncMLTestPage::CISyncMLTestPage(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CISyncMLTestPage::IDD, pParent)
{
    m_pISyncML = NULL;
}

CISyncMLTestPage::~CISyncMLTestPage()
{
}

void CISyncMLTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nISyncML_Function);
	DDV_MinMaxInt(pDX, m_nISyncML_Function, 0, 1);
}


BEGIN_MESSAGE_MAP(CISyncMLTestPage, CDialog)
	ON_BN_CLICKED(IDC_NEW_ISyncML, &CISyncMLTestPage::OnBnClickedNewIsyncml)
	ON_BN_CLICKED(IDC_DELETE_INTERFACE, &CISyncMLTestPage::OnBnClickedDeleteInterface)
	ON_BN_CLICKED(IDC_EXEC_FUNCTION, &CISyncMLTestPage::OnBnClickedExecFunction)
END_MESSAGE_MAP()


// CISyncMLTestPage message handlers

void CISyncMLTestPage::OnBnClickedNewIsyncml()
{
	// TODO: Add your control notification handler code here
	if ( m_pISyncML == NULL )
	{
	  m_pISyncML = (ISYNCML*)CommCreateInterface(theAppName,_T("Test ISyncML"),eISyncML);
	  if (m_pISyncML)
      AfxMessageBox( _T("ISyncML interface Successfully Created."), MB_OK|MB_ICONINFORMATION );
	  else
      AfxMessageBox( _T("ISyncML interface Create Fail."));
	}
	else
	AfxMessageBox( _T("ISyncML interface Already Created !"));
}

void CISyncMLTestPage::OnBnClickedDeleteInterface()
{
	// TODO: Add your control notification handler code here
	if ( m_pISyncML )
	{
	  CommDeleteInterface(m_pISyncML);
	  m_pISyncML = NULL;
	  AfxMessageBox( _T("ISyncML interface Successfully Deleted."), MB_OK|MB_ICONINFORMATION );
	}
	else
      AfxMessageBox( _T("No ISyncML interface has been Created."));
}

void CISyncMLTestPage::OnBnClickedExecFunction()
{
	// TODO: Add your control notification handler code here
    UpdateData();
	
    if ( m_pISyncML == NULL)
	{
	  AfxMessageBox(_T("Please create Interface first."));
	  return;
	}

    int     nResult = -1;
	BYTE*   pData = (BYTE*)"Test";
    DWORD   nlen = 5;
	
	switch ( m_nISyncML_Function )
	{
	case ePut:
		nResult = m_pISyncML->Put( pData, nlen );
		break;
	case eGet:
		nResult = m_pISyncML->Get( &pData, nlen );
		break;
	}

}
