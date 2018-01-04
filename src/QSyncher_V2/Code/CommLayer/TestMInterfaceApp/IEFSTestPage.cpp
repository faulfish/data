// IEFSTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "TestMInterfaceApp.h"
#include "IEFSTestPage.h"

// CIEFSTestPage dialog

IMPLEMENT_DYNAMIC(CIEFSTestPage, CDialog)

enum{
eGetFileASync = 0,
eGetFileData,
ePutFileAsync,
ePutFileData,
eDeleteFile,
eGetFolderEntries,
eCreateFolder,
eRemoveFolder
};

CIEFSTestPage::CIEFSTestPage(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CIEFSTestPage::IDD, pParent)
	, m_nIEFS_Function(0)
	, m_Param1(_T(""))
	, m_Param2(_T(""))
	, m_Param3(_T(""))
	, m_Param4(_T(""))
{
    m_pIEFS = NULL;
	m_pIEFSNotify = new CIEFSNotify(this);
}

CIEFSTestPage::~CIEFSTestPage()
{
    delete m_pIEFSNotify;
}

void CIEFSTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nIEFS_Function);
	DDV_MinMaxInt(pDX, m_nIEFS_Function, 0, 12);
	DDX_Control(pDX, IDC_EDIT_Result, m_Result_Edit);
	DDX_Text(pDX, IDC_EDIT_Param1, m_Param1);
	DDX_Text(pDX, IDC_EDIT_Param2, m_Param2);
	DDX_Text(pDX, IDC_EDIT_Param3, m_Param3);
	DDX_Text(pDX, IDC_EDIT_Param4, m_Param4);
	DDX_Control(pDX, IDC_LIST_INFO, m_ResList);
}


BEGIN_MESSAGE_MAP(CIEFSTestPage, CDialog)
	ON_BN_CLICKED(IDC_NEW_INTERFACE, &CIEFSTestPage::OnBnClickedNewInterface)
	ON_BN_CLICKED(IDC_EXEC_FUNCTION, &CIEFSTestPage::OnBnClickedExecFunction)
	ON_BN_CLICKED(IDC_DELETE_INTERFACE, &CIEFSTestPage::OnBnClickedDeleteInterface)
END_MESSAGE_MAP()


// CIEFSTestPage message handlers

void CIEFSTestPage::OnBnClickedNewInterface()
{
	// TODO: Add your control notification handler code here
	if ( m_pIEFS == NULL )
	{
	  m_pIEFS = (IEFS*)CommCreateInterface(theAppName,_T("Test IEFS"),eIEFS);
	  if (m_pIEFS)
	  {
		AfxMessageBox( _T("IEFS interface Successfully Created."), MB_OK|MB_ICONINFORMATION );
		m_pIEFS->RegisterNotify( m_pIEFSNotify, NOTIFY_GET_FILE_DATA );
	  }
	  else
      AfxMessageBox( _T("IEFS interface Create Fail."));
	}
	else
	AfxMessageBox( _T("IEFS interface Already Created !"));
}

void CIEFSTestPage::OnBnClickedExecFunction()
{
	// TODO: Add your control notification handler code here
    UpdateData();
	
    if ( m_pIEFS == NULL)
	{
	  AfxMessageBox(_T("Please create Interface first."));
	  return;
	}
    
	vector<WIN32_FIND_DATA> vEntries;
	int		nResult = -1 ;
	CString strResult;
	int     nFileSize = 0;
	DWORD   nPartSize = 0;
	BOOL    bLastData = FALSE;
	BYTE*   pFileData;

	m_Result_Edit.SetWindowTextW(_T(""));
	m_ResList.ResetContent();
	
	switch ( m_nIEFS_Function )
	{
	case eGetFileASync:
		nResult = m_pIEFS->GetFileASync( _ttoi(m_Param1.GetBuffer()), m_Param2, nFileSize );
		break;
	case eGetFileData:
		nResult = m_pIEFS->GetFileData( _ttoi(m_Param1.GetBuffer()), &pFileData, nPartSize, bLastData );
		break;
	case ePutFileAsync:
		nResult = m_pIEFS->PutFileAsync( _ttoi(m_Param1.GetBuffer()), m_Param2, _ttoi(m_Param3.GetBuffer()) );
		break;
	case ePutFileData:
		pFileData = NULL;
		nResult = m_pIEFS->PutFileData( _ttoi(m_Param1.GetBuffer()), pFileData, nPartSize, TRUE );
		break;
	case eDeleteFile:
		nResult = m_pIEFS->DeleteFile( m_Param1 );
		break;
	case eGetFolderEntries:
		nResult = m_pIEFS->GetFolderEntries( m_Param1, &vEntries);
		if (nResult == eEFSRetSuccess)
		for ( UINT i=0; i<vEntries.size(); i++ )
		{
		  m_ResList.AddString( vEntries[i].cFileName );
		}
		break;
	case eCreateFolder:
		nResult = m_pIEFS->CreateFolder( m_Param1 );
		break;
	case eRemoveFolder:
		nResult = m_pIEFS->RemoveFolder( m_Param1 );
		break;
	}
	strResult.Format(_T("%d"),nResult);
	m_Result_Edit.SetWindowTextW( strResult );
}

void CIEFSTestPage::OnBnClickedDeleteInterface()
{
	// TODO: Add your control notification handler code here
    if ( m_pIEFS )
	{
	  CommDeleteInterface(m_pIEFS);
	  m_pIEFS = NULL;
	  AfxMessageBox( _T("IEFS interface Successfully Deleted."), MB_OK|MB_ICONINFORMATION );
	}
	else
      AfxMessageBox( _T("No IEFS interface has been Created."));
}

void CIEFSTestPage::OnIEFSEventNotify(const TSTRING& strEvent,long nParam)
{
    CString strResult;
	NotifyPara* pNotify;
	int nSizeGet = 0;
	int nOpId = 0;
    
	if ( strEvent == NOTIFY_GET_FILE_DATA && nParam != NULL )
	{
	   pNotify = (NotifyPara*)nParam;
	   nSizeGet = pNotify->m_nSize;
	   nOpId    = pNotify->nOperationID;
	}
	//strResult.Format(_T("%d"),nParam);
	//m_ResList.InsertString(0, strResult); 	
}