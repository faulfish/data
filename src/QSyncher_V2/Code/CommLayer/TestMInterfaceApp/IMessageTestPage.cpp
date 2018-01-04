// IMessageTestPage.cpp : implementation file
//

#include "stdafx.h"
#include "TestMInterfaceApp.h"
#include "IMessageTestPage.h"


// CIMessageTestPage dialog

enum{
eGetServiceCenterAddress,
eSetPDUMode,
eSetMessageStorage,
eQueryMessageSpace,
eListMessageTi,
eListMessageStd,
eSendMessage,
eSendMessageInStorage,
eWriteMessage,
eReadMessage,
eDeleteMessage
};

IMPLEMENT_DYNAMIC(CIMessageTestPage, CDialog)

CIMessageTestPage::CIMessageTestPage(CWnd* pParent /*=NULL*/)
	: CTabPageSSL(CIMessageTestPage::IDD, pParent)
	, m_Param1(_T(""))
	, m_param2(_T(""))
	, m_param3(_T(""))
	, m_param4(_T(""))
{
   m_pISMS = NULL;
}

CIMessageTestPage::~CIMessageTestPage()
{
}

void CIMessageTestPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO1, m_nISMS_Function);
	DDV_MinMaxInt(pDX, m_nISMS_Function, 0, 12);
	DDX_Text(pDX, IDC_EDIT_Param1, m_Param1);
	DDX_Text(pDX, IDC_EDIT_Param2, m_param2);
	DDX_Text(pDX, IDC_EDIT_Param3, m_param3);
	DDX_Text(pDX, IDC_EDIT_Param4, m_param4);
	DDX_Control(pDX, IDC_LIST_INFO, m_result_list);
	DDX_Control(pDX, IDC_EDIT_Result, m_result_edit);
}


BEGIN_MESSAGE_MAP(CIMessageTestPage, CDialog)
	ON_BN_CLICKED(IDC_NEW_INTERFACE, &CIMessageTestPage::OnBnClickedNewInterface)
	ON_BN_CLICKED(IDC_DELETE_INTERFACE, &CIMessageTestPage::OnBnClickedDeleteInterface)
	ON_BN_CLICKED(IDC_EXEC_FUNCTION, &CIMessageTestPage::OnBnClickedExecFunction)
END_MESSAGE_MAP()


// CIMessageTestPage message handlers

void CIMessageTestPage::OnBnClickedNewInterface()
{
	// TODO: Add your control notification handler code here
	if ( m_pISMS == NULL )
	{
	  m_pISMS = (ISMS*)CommCreateInterface(theAppName,_T("Test IEFS"),eISMS);
	  if (m_pISMS)
      AfxMessageBox( _T("ISMS interface Successfully Created."), MB_OK|MB_ICONINFORMATION );
	  else
      AfxMessageBox( _T("ISMS interface Create Fail."));
	}
	else
	AfxMessageBox( _T("ISMS interface Already Created !"));
}

void CIMessageTestPage::OnBnClickedDeleteInterface()
{
	// TODO: Add your control notification handler code here
	if ( m_pISMS )
	{
	  CommDeleteInterface(m_pISMS);
	  m_pISMS = NULL;
	  AfxMessageBox( _T("ISMS interface Successfully Deleted."), MB_OK|MB_ICONINFORMATION );
	}
	else
      AfxMessageBox( _T("No ISMS interface has been Created."));
}

void CIMessageTestPage::OnBnClickedExecFunction()
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
    if ( m_pISMS == NULL)
	{
	  AfxMessageBox(_T("Please create Interface first."));
	  return;
	}
    
	int		  nResult = -1 ;
	CString   strResult;
	CString   strParam1,strParam2;
	UINT      nUsedSpace =0 , nTotalSpace = 0;
	byte      pData [1024];   UINT nLen = 0 , nMsgID = 0;
	CStringA  strTestSMS = "Test BenQ SMS";
	
	m_result_edit.SetWindowTextW(_T(""));
	m_result_list.ResetContent();
	
	switch ( m_nISMS_Function )
	{
	case eGetServiceCenterAddress:
		 nResult = m_pISMS->GetServiceCenterAddress( &strParam1 );
		 break;
	case eSetPDUMode:
		 nResult = m_pISMS->SetPDUMode();
         break;
	case eSetMessageStorage:
		 nResult = m_pISMS->SetMessageStorage( (enumStorage)(_ttoi(m_Param1.GetBuffer())) );
         break;
	case eQueryMessageSpace:
		 nResult = m_pISMS->QueryMessageSpace
			       ((enumStorage)(_ttoi(m_Param1.GetBuffer())), &nUsedSpace, &nTotalSpace );
         break; 
	case eListMessageTi:
		 nResult = m_pISMS->ListMessageTi((enumMsgLoc)(_ttoi(m_Param1.GetBuffer())), &strParam2);
         break;
	case eListMessageStd:
		 nResult = m_pISMS->ListMessageStd();
         break; 
	case eSendMessage:
		 nResult = m_pISMS->SendMessage((BYTE*)strTestSMS.GetBuffer(),strTestSMS.GetLength(), &nMsgID);
         break;
	case eSendMessageInStorage:
		 nResult = m_pISMS->SendMessageInStorage(_ttoi(m_Param1.GetBuffer()), m_param2);
         break;
	case eWriteMessage:
		 nResult = m_pISMS->WriteMessage((enumMsgLoc)(_ttoi(m_Param1.GetBuffer())), 
			      (BYTE*)strTestSMS.GetBuffer(),strTestSMS.GetLength(), &nMsgID);
         break;
	case eReadMessage:
		 nResult = m_pISMS->ReadMessage( _ttoi(m_Param1.GetBuffer()), pData , &nLen );
         break;
	case eDeleteMessage:
		 nResult = m_pISMS->DeleteMessage( _ttoi(m_Param1.GetBuffer()) );
         break;  
	}
	
	strResult.Format(_T("%d"),nResult);
	m_result_edit.SetWindowTextW( strResult );
}
