/*********************************************************************/
/** @file:	QProgressServer.cpp

Implements the CQProgressServer and its associated classes.

$Original Author:   Steven CC Chen  $
$Version:	 $
$Date:   Mar 19 2008 17:23:06  $
*/
/*********************************************************************/

#include "stdafx.h"
#include "QProgressServer.h"
#include "..\..\..\CommonUtil\CalculateFunctionTime.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB
{
	//////////////////////////////////////////////////////////////////////////////////////
	// CQProgressServerDialog

	IMPLEMENT_DYNCREATE(CQProgressServerDialog, CQDialog);

	CQProgressServerDialog::CQProgressServerDialog(IQProgressClient* pQProgressClient /*=NULL*/, CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(IDD_QPROGRESSSERVER_DIALOG, pParent)
		, m_pQProgressClient(pQProgressClient)
		, m_bIsModal(FALSE)
		, m_bCanAutoCloseAfterTaskComplete(TRUE)
		, m_pTaskWorkerThread(NULL)
		, m_bEnableCancelButton(TRUE)
		, m_pProgress(NULL)
		, m_pTextNearBar(NULL)
		, m_strPercentage(_T("0%"))
		, m_pCaptionText(NULL)
	{
		DefineNotify(EVENT_PROGRESS_TASK_FINISH);
		DefineNotify(EVENT_PROGRESS_TASK_ABORT);
		DefineNotify(EVENT_PROGRESS_TASK_UPDATE);
		DefineNotify(EVENT_PROGRESS_TASK_GETHWMD);
		// To register the IQProgressClient Events
		if(m_pQProgressClient != NULL)
		{
			m_pQProgressClient->Register(this, EVENT_PROGRESS_TASK_UPDATE);
			m_pQProgressClient->Register(this, EVENT_PROGRESS_TASK_GETHWMD);
		}
	}

	CQProgressServerDialog::~CQProgressServerDialog()
	{
		if(m_pQProgressClient)
		{
			m_pQProgressClient->Unregister(this, EVENT_PROGRESS_TASK_UPDATE);
			m_pQProgressClient->Unregister(this, EVENT_PROGRESS_TASK_GETHWMD);
		}
	}

	BOOL CQProgressServerDialog::OnInitDialog()
	{
		CQDialog::OnInitDialog();
		SetXMLUI();
		//-----------------------------------------
		// Set up its properties:
		if(m_pQProgressClient != NULL)
			SetCanAutoCloseAfterTaskComplete( m_pQProgressClient->GetAutoClose() );
		//-----------------------------------------

		//-----------------------------------------
		// Set up its UI styles:
		assert(m_pQProgressClient != NULL);
		if(m_pQProgressClient != NULL)
		{
			SetProgressDialogCaption( LPCTSTR(m_pQProgressClient->GetTitle()) );
			SetProgressDialogText( LPCTSTR(m_pQProgressClient->GetDisplayText()) );
			SetCancelButtonCaption( LPCTSTR(m_pQProgressClient->GetCancelButtonCaption()) );

			bool bCancelable = m_pQProgressClient->GetCancelable() == TRUE;
			EnableCancelButton(bCancelable);
			if(m_pCancelButton)
				m_pCancelButton->Show(bCancelable);
		}
		//-----------------------------------------

		//-----------------------------------------
		// Set up and create its ToolTips:
		m_Tooltip.Create(this, 0x40);
		::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)QPROGRESS_TOOLTIP_DEFAULT_DELAYTIME);
		::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPTEXTCOLOR, (WPARAM)QPROGRESS_TOOLTIP_DEFAULT_TIPTEXTCOLOR, 0);
		::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPBKCOLOR, (WPARAM)QPROGRESS_TOOLTIP_DEFAULT_TIPBKCOLOR, 0);
		m_Tooltip.Activate(TRUE);
		//-----------------------------------------

		//-----------------------------------------
		// Start the (background) Task Worker Thread immediately:
		if( TaskWorkerThreadBeginSuspended() )
			TaskWorkerThreadResume();  //(*把 Task Worker Thread 跑起來之後就放著不管, 只被動地等 QProgressClient 的通知來更新 progress 進度*)
		//-----------------------------------------	

		return TRUE;
	}

	void CQProgressServerDialog::SetXMLUI()
	{
		SetUIBackground();
		CLayoutManager	LayoutManager(this, this);
		
		Widge<HDC>* pWidget;
		if(m_pQProgressClient && !m_pQProgressClient->GetIsAnimate())
		{
			pWidget = LayoutManager.Load(_T("QProgressServerID"));
			m_pTextNearBar = static_cast<WIN32LabelWidge*>(LayoutManager.GetWidgetByID(_T("Win32Label_2")));
		}
		else if(m_pQProgressClient)
			pWidget = LayoutManager.Load(_T("QProgressServer_AnimateID"));
		m_pText =static_cast<WIN32LabelWidge*>(LayoutManager.GetWidgetByID(_T("Win32Label_1")));		
		m_pProgress =static_cast<Win32ProgressBarWidge*>(LayoutManager.GetWidgetByID(_T("Win32ProgressBar_0")));
		if(m_pQProgressClient && m_pQProgressClient->GetIsAnimate())
			m_pProgress->AnimateStart(this);
		m_pCaptionText = static_cast<WIN32LabelWidge*>(LayoutManager.GetWidgetByID(_T("Win32Label_0")));
		m_pCancelButton = static_cast<WIN32IconButton*>(LayoutManager.GetWidgetByID(_T("Win32IconButton_0")));			

		SetLayout(pWidget);
	}

	void CQProgressServerDialog::OwnerDestructor()
	{
		// To un-register the IQProgressClient Events:
		if(m_pQProgressClient != NULL)
			m_pQProgressClient->Unregister(this, EVENT_PROGRESS_TASK_UPDATE);
	}


	void CQProgressServerDialog::OwnerCloseDialog()
	{
		if(m_bIsModal)
		{
			// for Modal Dialog:
			EndDialog(IDCLOSE);
		}
		else
		{
			// for Modeless Dialog:
			::DestroyWindow(GetSafeHwnd());
		}
	}


	UINT CQProgressServerDialog::TaskWorkerThreadProc(LPVOID pParam) //(#)
	{
		UINT nRet = 1;  //(failure)

		CQProgressServerDialog* pQProgressServerDialog = (CQProgressServerDialog*)pParam;
		if(pQProgressServerDialog != NULL)
		{
			if(pQProgressServerDialog->m_pQProgressClient != NULL)
			{
				pQProgressServerDialog->m_pQProgressClient->StartTask();
				pQProgressServerDialog->Fire(EVENT_PROGRESS_TASK_FINISH);
				if( pQProgressServerDialog->GetCanAutoCloseAfterTaskComplete() )
					::PostMessage(pQProgressServerDialog->GetSafeHwnd(), WM_CLOSE, 0, 0);
			}
		}				

		return nRet;
	}


	BOOL CQProgressServerDialog::TaskWorkerThreadBeginSuspended() //(#)
	{
		BOOL bRet = FALSE;

		//(*create only once*)
		if(m_pTaskWorkerThread == NULL)
		{
			m_pTaskWorkerThread = AfxBeginThread(TaskWorkerThreadProc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, CREATE_SUSPENDED, NULL);
			if(m_pTaskWorkerThread != NULL)
			{
				m_pTaskWorkerThread->m_bAutoDelete = TRUE;
				bRet = TRUE;
			}
		}

		return bRet;
	}


	BOOL CQProgressServerDialog::TaskWorkerThreadResume() //(#)
	{
		BOOL bRet = FALSE;

		if(m_pTaskWorkerThread != NULL)
		{
			if(m_pTaskWorkerThread->ResumeThread() <= 1)
			{
				bRet = TRUE;
			}
		}

		return bRet;
	}


	BOOL CQProgressServerDialog::TaskWorkerThreadSuspend() //(#)
	{
		BOOL bRet = FALSE;

		if(m_pTaskWorkerThread != NULL)
			bRet = m_pTaskWorkerThread->SuspendThread() != 0xFFFFFFFF;

		return bRet;
	}


	CString CQProgressServerDialog::GetProgressDialogText()
	{
		CString strResult = m_strProgressDialogText.c_str();
		return strResult;
	}


	BOOL CQProgressServerDialog::SetProgressDialogText(LPCTSTR pszText)
	{
		BOOL bRes = TRUE;
		m_strProgressDialogText = pszText;
		if(m_pProgress)
			m_pProgress->SetDisplayString(m_strProgressDialogText);
		if(m_pText)
		{
			m_pText->SetDisplayString(m_strProgressDialogText);
			m_pText->Update();
		}
		return bRes;
	}

	TSTRING CQProgressServerDialog::GetProgressDialogCaption()
	{
		TSTRING strRes;
		if(m_pCaptionText)
			strRes = m_pCaptionText->GetDisplayString();		
		return strRes;
	}

	void CQProgressServerDialog::SetProgressDialogCaption(LPCTSTR pszText)
	{
		if(m_pCaptionText)
			m_pCaptionText->SetDisplayString(pszText);
	}

	CString CQProgressServerDialog::GetCancelButtonCaption()
	{
		CString strResult = m_strCancelText.c_str();
		return strResult;
	}

	BOOL CQProgressServerDialog::SetCancelButtonCaption(LPCTSTR pszText)
	{
		m_strCancelText = pszText;
		if(m_pCancelButton)
			m_pCancelButton->SetDisplayString(m_strCancelText);
		return TRUE;
	}

	BOOL CQProgressServerDialog::GetCanAutoCloseAfterTaskComplete()
	{
		return m_bCanAutoCloseAfterTaskComplete;
	}

	void CQProgressServerDialog::SetCanAutoCloseAfterTaskComplete(BOOL bAutoClose)
	{
		m_bCanAutoCloseAfterTaskComplete = bAutoClose;
	}

	LRESULT CQProgressServerDialog::OnUserUpdate(WPARAM wParam, LPARAM lParam) 
	{		
		if( m_pQProgressClient != NULL)
		{
			SetProgressDialogText( LPCTSTR(m_strCurrentDisplayText) );
			iToTStr(m_nCurrentPosition,m_strPercentage);
			m_strPercentage += _T("%");
			if(m_pProgress && m_pTextNearBar)
			{
				m_pProgress->UpdatePercent(m_nCurrentPosition);
				m_pTextNearBar->SetDisplayString(m_strPercentage);
			}
		}
		return 0;
	}
	// To process the IQProgressClient Events:
	void CQProgressServerDialog::Event(const TSTRING& strEvent, long nParam)
	{
		QProgressNotify* pQProgressNotify = NULL;

		if(strEvent == EVENT_PROGRESS_TASK_UPDATE)
		{	
			pQProgressNotify = (QProgressNotify*)nParam;
			if(pQProgressNotify)
			{
				if(m_nCurrentPosition != pQProgressNotify->m_nCurrentPosition || m_strCurrentDisplayText != pQProgressNotify->m_strCurrentDisplayText)
				{
					m_strCurrentDisplayText = pQProgressNotify->m_strCurrentDisplayText;
					m_nCurrentPosition = pQProgressNotify->m_nCurrentPosition;
					PostMessage(WM_USER_UPDATE,0,0);
				}
			}
		}
		else if(strEvent == EVENT_PROGRESS_TASK_GETHWMD)
		{
			HWND* pHwnd = (HWND*)nParam;
			*pHwnd = GetSafeHwnd();
		}
		else if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pCancelButton && m_pCancelButton->GetID() == nParam)
				OnBnClickedCancel();
		}
		else
		{
			//Need to implement
			//const LPCTSTR EVENT_PROGRESS_TASK_FINISH			= _T("TNotify_TASK_Finish");
			//const LPCTSTR EVENT_PROGRESS_TASK_ABORT			= _T("TNotify_TASK_Abort");
			assert(0);
		}
	}

	void CQProgressServerDialog::EnableCancelButton(bool bEnable)
	{
		if(m_bEnableCancelButton != (bEnable == true))
		{
			m_bEnableCancelButton = bEnable;
			if(m_pCancelButton)
				m_pCancelButton->SetEnable(bEnable);
		}
	}

	void CQProgressServerDialog::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
	}

	BEGIN_MESSAGE_MAP(CQProgressServerDialog, CQDialog)
		ON_WM_CLOSE()
		ON_WM_SETCURSOR()
		ON_WM_LBUTTONDOWN()
		ON_WM_MOUSEMOVE()
		ON_WM_LBUTTONUP()
		ON_WM_SIZING()
		ON_MESSAGE(WM_USER_UPDATE , &CQProgressServerDialog::OnUserUpdate)
	END_MESSAGE_MAP()

	BOOL CQProgressServerDialog::PreTranslateMessage(MSG* pMsg)
	{
		switch(pMsg->message)
		{
		case WM_MOUSEMOVE:
			{
				m_Tooltip.RelayEvent(pMsg);
			}
			break;
		case WM_SYSKEYDOWN:
			if(pMsg->wParam == VK_F4)
				return TRUE;
			break;
		case WM_KEYDOWN:
			{
				switch(pMsg->wParam)
				{
				case VK_ESCAPE:
					{
						return TRUE;  //(to disable <Esc>-Key-Exit.)
					}
					break;
				case VK_RETURN:
					{
						return TRUE;  //(to disable <ENTER>-Key-Exit.)
					}
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		}

		return __super::PreTranslateMessage(pMsg);
	}

	void CQProgressServerDialog::PostNcDestroy()
	{
		OwnerDestructor();
		//(*we have to explicitly delete this dialog when it is dynamically created*)
		if( !m_bIsModal )
			delete this;

		CDialog::PostNcDestroy();
	}

	INT_PTR CQProgressServerDialog::DoModal()
	{
		m_bIsModal = TRUE;
		return CDialog::DoModal();
	}

	void CQProgressServerDialog::OnBnClickedCancel()
	{
		if(m_pQProgressClient != NULL)
		{
			if( m_pQProgressClient->SuspendTask() )
			{
				m_pQProgressClient->AbortTask();
				m_pQProgressClient->ResumeTask();
			}
		}
	}

	void CQProgressServerDialog::OnClose()
	{
		m_Tooltip.Activate(FALSE);
		m_Tooltip.DestroyWindow();
		OwnerCloseDialog();
	}

	void CQProgressServerDialog::OnSizing(UINT fwSide, LPRECT pRect)
	{
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
	}
	void CQProgressServerDialog::SetUIBackground()
	{
		TSTRING strBkImage = L"ProgressBar\\Bk_ProgressBar.png";
		SetBKImage(strBkImage,BK_Fixed);
		int nDeltaWidth = GetWidthDifferenceOfWindowAndClient();
		int nDeltaHeight = GetHeightDifferenceOfWindowAndClient();
		const int nContactDlgWIDTH = 360 ;
		const int nContactDlgHEIGHT = 174 ;
		SetWindowPos(NULL, 0, 0, nContactDlgWIDTH + nDeltaWidth, nContactDlgHEIGHT + nDeltaHeight, SWP_NOMOVE);
	}

	int CQProgressServerDialog::GetHeightDifferenceOfWindowAndClient()
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		return rcWindow.Height() - rcClient.Height();
	}

	int CQProgressServerDialog::GetWidthDifferenceOfWindowAndClient()
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		return rcWindow.Width() - rcClient.Width();
	}

	void CQProgressServerDialog::MouseDown(int nCX,int nCY)
	{
		Win32DoubleBufferQDialogWidge::MouseDown(nCX,nCY);

		CPoint point(nCX,nCY);
		PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
    }

	//////////////////////////////////////////////////////////////////////////////////////
	// CQProgressServer

	CQProgressServer::CQProgressServer()
	{
	}

	CQProgressServer::~CQProgressServer()
	{
		//it will delete by itself, please see "void CQProgressServerDialog::PostNcDestroy()"
		/*
		if(GetServerDialog())
		{
			delete GetServerDialog();
			GetServerDialog() = NULL;
		}
		*/
	}

	BOOL CQProgressServer::Exec(IQProgressClient* pQProgressClient,CWnd* pParentWnd)
	{
		BOOL bRet = FALSE;

		if(pQProgressClient != NULL)
		{
			if(pQProgressClient->IsModalless() == FALSE)
			{
				CQProgressServerDialog progressDiag(pQProgressClient,pParentWnd);
				progressDiag.DoModal();
				bRet = TRUE;
			}
			else
			{
				if(GetServerDialog() == NULL)
					GetServerDialog() = new CQProgressServerDialog(pQProgressClient,pParentWnd);

				if(GetServerDialog() != NULL)
				{
					BOOL bRet = GetServerDialog()->Create(IDD_QPROGRESSSERVER_DIALOG);
					if(bRet)
					{
						GetServerDialog()->ShowWindow(SW_NORMAL);
						GetServerDialog()->UpdateWindow();  //(*We have to call UpdateWindow() explicitly, or the dialog will show empty*)
						bRet = TRUE;
					}
				}
			}
		}
		return bRet;
	}
}

#pragma warning(pop)