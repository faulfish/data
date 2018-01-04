// QDialog.cpp : implementation file
//

#include "stdafx.h"
#include "QDialog.h"
#include "../interface/ShowMessage.h"

// CQDialog dialog
namespace QSYNC_LIB
{

	IMPLEMENT_DYNAMIC(CQDialog, CDialog)

		CQDialog::CQDialog(CWnd* pParent /*=NULL*/)
		: CDialog(CQDialog::IDD, pParent),m_pLocale(NULL),m_DlgMoveable(true)
	{

	}

	CQDialog::CQDialog(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
		: CDialog(nIDTemplate, pParent),m_pLocale(NULL),m_DlgMoveable(true)
	{

	}

	CQDialog::~CQDialog()
	{
	}

	void CQDialog::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}


	BEGIN_MESSAGE_MAP(CQDialog, CDialog)
		ON_WM_SIZE()
		ON_WM_LBUTTONDOWN()
	END_MESSAGE_MAP()


	// CQDialog message handlers

	BOOL CQDialog::OnInitDialog()
	{
		CDialog::OnInitDialog();

		CreateILocalization(&m_pLocale);
		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE

	}

	void CQDialog::SetWindowText(LPCTSTR lpszString)
	{
		m_strWindowText = lpszString;
	}

	void CQDialog::GetWindowText(CString& strText) const
	{
		strText = m_strWindowText.c_str();
	}

	void CQDialog::SetWindowRegion(LPRECT& pRect, GeometryRect& rcClient) 
	{
		RECT rcClient2 = {0};
		GetClientRect(&rcClient2);
		ClientToScreen(&rcClient2);
		HRGN frameRgn = CreateRectRgn(0, 0, pRect->right - pRect->left, pRect->bottom - pRect->top);
		HRGN clientRgn = CreateRoundRectRgn(0, 0, rcClient.m_nWidth+1, rcClient.m_nHeight+1, 6, 6);    
		OffsetRgn(clientRgn, rcClient2.left - pRect->left, rcClient2.top - pRect->top);
		CombineRgn(frameRgn, frameRgn, clientRgn, RGN_AND);
		SetWindowRgn(frameRgn, TRUE);
	}

	std::wstring CQDialog::GetWString(const TSTRING& strID)
	{
		std::wstring wstrRes = L"";
		TSTRING strText = m_pLocale->GetString(strID).c_str();
#ifdef _UNICODE
		wstrRes = strText;
#else
		wstrRes = CharToWideChar( wstrRes, CP_UTF8 );
#endif
		return wstrRes;
	}

	void CQDialog::SetDlgMoveable(bool bMoveable)
	{
		m_DlgMoveable = bMoveable;
	}

	void CQDialog::OnLButtonDown( UINT nFlags, CPoint point)
	{
		if(m_DlgMoveable)
			PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	}

	BOOL CQDialog::PreTranslateMessage(MSG* pMsg)
	{
		switch(pMsg->message)
		{
		case WM_SYSKEYDOWN:
			if(pMsg->wParam == VK_F4)
				return TRUE; //(to disable Alt-F4-Exit.)
			break;
		case WM_KEYDOWN:
			{
				switch(pMsg->wParam)
				{
				case VK_ESCAPE:
					return TRUE;  //(to disable <Esc>-Key-Exit.)
					break;
				case VK_RETURN:
					OnBnClickedOK();
					return TRUE;
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

	void CQDialog::Disconnect(const TSTRING& strTitle,const TSTRING& strContent,bool bShowMessage)
	{
		if(::IsWindow(GetSafeHwnd()))
		{
			if(bShowMessage)
				ShowMessage(GetSafeHwnd(),strContent,strTitle);

			PostMessage(WM_CLOSE);
		}
	}

}