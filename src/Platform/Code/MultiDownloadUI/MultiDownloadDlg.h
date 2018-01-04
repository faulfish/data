// MultiDownloadDlg.h : header file
//
#include <afxole.h>

#include "..\..\..\code\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/interface/QCommonControlBase.h"
#include "..\..\..\code\Components\ConfigureTool\IConfigure.h"
#include "../../../code\Components\UIFramework\Porting\QUIDropTraget.h"
#include "../../../code\Components\UIFramework\Porting\UIOleDataSource.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\LayoutManager.h"
//#include "LanguageSubmenu.h"
//#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"
//#include "..\..\lib\requestutility\interfacebase.h"
//#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "DownloadFunc.h"

#define DOWNLOAD_BUTTON_1 0x1
#define DOWNLOAD_BUTTON_2 0x2
#define DOWNLOAD_BUTTON_3 0x4
#define DOWNLOAD_BUTTON_4 0x8
#define DOWNLOAD_BUTTON_5 0x10
#define DOWNLOAD_BUTTON_6 0x20
#define DOWNLOAD_BUTTON_7 0x40
#define DOWNLOAD_BUTTON_8 0x80

#pragma once

namespace QSYNC_LIB
{


	// CMultiDownloadDlg dialog
	class CMultiDownloadDlg : public Win32DoubleBufferQDialogWidge, public INotify
	{
	// Construction
	public:
		CMultiDownloadDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
		enum { IDD = IDD_MULTIDOWNLOAD_DIALOG };

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
		LRESULT OnDLNotify(WPARAM wParam, LPARAM lParam);
	public:
		afx_msg void OnLbnSelchangeList1();
		afx_msg void OnSelchangecom1();
		afx_msg void OnSelchangecom2();
		afx_msg void OnSelchangecom3();
		afx_msg void OnSelchangecom4();
		afx_msg void OnSelchangecom5();
		afx_msg void OnSelchangecom6();
		afx_msg void OnSelchangecom7();
		afx_msg void OnSelchangecom8();
		virtual void Event(const TSTRING& strEvent,long nParam);
		UINT		m_uiDownloadActive;

		void StartDownload1();
		void StartDownload2();
		void StartDownload3();
		void StartDownload4();
		void StartDownload5();
		void StartDownload6();
		void StartDownload7();
		void StartDownload8();

	private:
		SplitLayout<HDC>* m_pMainLayout;
		CLayoutManager* m_pLayoutManager;

		WIN32IconButton* m_pWinBrowse;
		WIN32IconButton* m_pWinStartAll;
		WIN32IconButton* m_pWinStart1;
		WIN32IconButton* m_pWinStart2;
		WIN32IconButton* m_pWinStart3;
		WIN32IconButton* m_pWinStart4;
		WIN32IconButton* m_pWinStart5;
		WIN32IconButton* m_pWinStart6;
		WIN32IconButton* m_pWinStart7;
		WIN32IconButton* m_pWinStart8;

		UINT	m_unAvailCom;
		UINT	m_unTotalCom;
		UINT	m_unButtonEnNum;

		CStringArray	m_szFileArray;
		CStringArray	m_szComArray;
		CString         m_strIni;

		UINT		m_uiDownloadMask;

		CDownloadFunc	*m_DownloadFunc[8];
	private:
		void	SetXMLUI(void);

		bool UpdateComportCombo(LPCTSTR lpszComport, bool bAdd = true);
		afx_msg void OnDestroy();
		void Browse1FFFile(void);
		void DownloadFunc(unsigned int unDownloadButtonMask);
		bool GetInfoFromME(CListBox* pList, UINT uComport);
		bool CheckQFuseState(UINT uComport);
	public:


		static bool CallBack1(const char* szMsg, int nStep, int nProgress);
		bool LogFn1(const char* szMsg, int nStep, int nProgress);
		static bool CallBack2(const char* szMsg, int nStep, int nProgress);
		bool LogFn2(const char* szMsg, int nStep, int nProgress);
		static bool CallBack3(const char* szMsg, int nStep, int nProgress);
		bool LogFn3(const char* szMsg, int nStep, int nProgress);
		static bool CallBack4(const char* szMsg, int nStep, int nProgress);
		bool LogFn4(const char* szMsg, int nStep, int nProgress);
		static bool CallBack5(const char* szMsg, int nStep, int nProgress);
		bool LogFn5(const char* szMsg, int nStep, int nProgress);
		static bool CallBack6(const char* szMsg, int nStep, int nProgress);
		bool LogFn6(const char* szMsg, int nStep, int nProgress);
		static bool CallBack7(const char* szMsg, int nStep, int nProgress);
		bool LogFn7(const char* szMsg, int nStep, int nProgress);
		static bool CallBack8(const char* szMsg, int nStep, int nProgress);
		bool LogFn8(const char* szMsg, int nStep, int nProgress);
	};
}