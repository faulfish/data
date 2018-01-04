#pragma once

#include "resource.h"
#include "../../Components/UIFramework/LayoutManager/Win32FrameworkWidge.h"
#include "../../Components/UIFramework/LayoutManager/BoxLayout.h"
#include "../../Components/UIFramework/LayoutManager/Splitlayout.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "../../Components/UIFramework/interface/IQTreeUI.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"

// CInputFolderDlg dialog
namespace QSYNC_LIB
{

	class CInputFolderDlg : public QSYNC_LIB::Win32DoubleBufferQDialogWidge,public QSYNC_LIB::CINotifyBase
	{
		DECLARE_DYNAMIC(CInputFolderDlg)

	public:
		CInputFolderDlg(CQTreeNode* pFolderNode, CWnd* pParent = NULL);   // standard constructor
		virtual ~CInputFolderDlg();
		void Event(const TSTRING& strEvent, long nParam);

		// Dialog Data
		enum { IDD = IDD_DIALOG_INPUT_FILE_NAME };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		CString m_strInText;
		CString m_strInitialText;
		virtual BOOL	OnInitDialog();
		void SetBackground();

		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnEnChangeEditInputFileName();
		afx_msg void OnBnClickedInputFileName();
		virtual afx_msg void OnBnClickedOk();
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	public:
		CString GetInputString(void) const;
		void SetInitialString(const CString&);

	private:
		void	SetUI();
		virtual void SetXMLUI();
		CString GetString(const TSTRING& strID);
		std::wstring CInputFolderDlg::GetWString(const TSTRING& strID);

		//SetUI functions
		SplitLayout<HDC>* CreateCaptionLayout();
		SplitLayout<HDC>* CreateClientLayout();
		BoxLayout<HDC>* CreateButtonLayout();
		SplitLayout<HDC>* CreateSubWindowLayout();
		//Button functions
	protected:
		virtual	bool ExistSameNameFolder(const CString& strFolderName)const;
		static void WinClose();
		void FnWinClose();
		static void Cancel();
		static void OK();

	protected:
		WIN32IconButton* m_pWinCloseButton;
		WIN32IconButton* m_pBtnCancel;
		WIN32IconButton* m_pBtnOK;

		ILocalization*  m_pILocale;
		CQTreeNode* m_pFolderNode;
		//QSYNC_LIB::BoxLayout<HDC>* m_pLayout;
	};

	class CRenameDlg:public CInputFolderDlg
	{
	public:
		CRenameDlg(CQTreeNode* pFolderNode,const CString& strOldFileName, CWnd* pParent = NULL);
	private:
		virtual void SetXMLUI();
		//virtual	bool ExistSameNameFolder(const CString& strFolderName)const;
		virtual void OnBnClickedOk();

	protected:
		CString m_strOldFileName;
	};
}