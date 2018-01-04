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
		
		virtual CMultiDownloadDlg::~CMultiDownloadDlg()
		{
			if(m_pLayoutManager)
				delete m_pLayoutManager;
		}

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
        afx_msg void OnLbnSelchangeList1();
        afx_msg void OnSelchangecom9();
        afx_msg void OnDestroy();
		LRESULT OnDLNotify(WPARAM wParam, LPARAM lParam);
	public:
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
		CString			m_strFolderPath;

		UINT		m_uiDownloadMask;

		CDownloadFunc	*m_DownloadFunc[8];
		int			m_DownloadProgress[8];
		CString		m_sz1FF;
	private:
		void SetXMLUI(void);

		bool UpdateComportCombo(LPCTSTR lpszComport, bool bAdd = true);
		
		void Browse1FFFile(void);
		void DownloadFunc(unsigned int unDownloadButtonMask);
		CallBackInfo m_pCallBackFunc[8];
		bool EnableStartButton(int nStartButtonIndex,bool bEnable);
	public:

		bool LogFn(int nIndex,const char* szMsg, int nStep, int nProgress);
		static bool CallBack1(const char* szMsg, int nStep, int nProgress);
		static bool CallBack2(const char* szMsg, int nStep, int nProgress);
		static bool CallBack3(const char* szMsg, int nStep, int nProgress);
		static bool CallBack4(const char* szMsg, int nStep, int nProgress);
		static bool CallBack5(const char* szMsg, int nStep, int nProgress);
		static bool CallBack6(const char* szMsg, int nStep, int nProgress);
		static bool CallBack7(const char* szMsg, int nStep, int nProgress);
		static bool CallBack8(const char* szMsg, int nStep, int nProgress);
	};
}