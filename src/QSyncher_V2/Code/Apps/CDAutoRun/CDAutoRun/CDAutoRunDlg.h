// CDAutoRunDlg.h : header file
//
#include "..\..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "../../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../Components/UIFramework/interface/QCommonControlBase.h"

#include "LanguageSubmenu.h"

#pragma once
namespace QSYNC_LIB
{
	// CCDAutoRunDlg dialog
	class CCDAutoRunDlg : public Win32DoubleBufferQDialogWidge, public INotify
	{
		// Construction
	public:
		CCDAutoRunDlg(CWnd* pParent = NULL);	// standard constructor

		// Dialog Data
		enum { IDD = IDD_CDAUTORUN_DIALOG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


		// Implementation
	protected:
		HICON m_hIcon;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
		afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	public:
		virtual void Event(const TSTRING& strEvent,long nParam);

	private:
		void SetUI();
		void SetBackground();
		SplitLayout<HDC>* CreateFunctionLayoutFromINI();
		int GetNumberOfFunctions();

		//Button functions:
		static void	Language();
		void	FnLanguage();
		static void	WinClose();
		void	FnWinClose();
		static void	INIBtnFunction(UINT);
		void	FnINIBtnFunction(UINT);

	private:
		ILocalization*		m_pILocalization;
		QCommonControlBase    m_QCommonControlBase_Language;
		LanguageSubmenu m_LanguageSubmenu;
		WIN32IconButton* m_pLanguageBtn;
		CString m_strFunction_INIFile_Path;

		std::vector<WIN32IconButton*>	m_vecFunctionBtn;
	};
}