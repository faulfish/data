/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MainFrm.h  $
 *  $Revision:   1.30  $
 *  $Date:   Jan 09 2008 15:07:52  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/QSyncer2/MainFrm.h.-arc  $
 * 
 *    Rev 1.30   Jan 09 2008 15:07:52   Jame Tsai
 * add AboutBox
 * 
 *    Rev 1.29   Jan 08 2008 16:36:14   Jame Tsai
 * add submenu
 * 
 *    Rev 1.28   Dec 28 2007 14:59:02   Alan Tu
 * update
 * 
 *    Rev 1.27   Dec 27 2007 12:21:54   Eddy Guo
 * update
 * 
 *    Rev 1.26   Dec 26 2007 17:53:02   Alan Tu
 * update
 * 
 *    Rev 1.25   Dec 26 2007 17:01:08   Alan Tu
 * Add window button
 * 
 *    Rev 1.24   Dec 26 2007 09:53:02   Eddy Guo
 * update
 * 
 *    Rev 1.23   Dec 24 2007 15:14:36   Jame Tsai
 * add button
 * 
 *    Rev 1.22   Dec 21 2007 16:02:38   Jame Tsai
 * change size
 * 
 *    Rev 1.21   Dec 13 2007 16:34:52   Jame Tsai
 * Change FunctionBar Seat
 * 
 *    Rev 1.20   Dec 11 2007 14:08:56   Eddy Guo
 * update for main frame using xml
 * 
 *    Rev 1.19   Nov 30 2007 14:30:56   Eddy Guo
 * update
 * 
 *    Rev 1.18   Nov 29 2007 16:33:04   Eddy Guo
 * update
 * 
 *    Rev 1.17   Nov 14 2007 09:46:34   Alan Tu
 * Add header
 *
 *
 ***************************************************************************/

#include "FunctionBar.h"
#include "../../Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "../../Components/UIFramework/LayoutManager/StackLayout.h"
#include "../../Components/UIFramework/LayoutManager/SplitLayout.h"
#include "../../Components/UIFramework/LayoutManager/BoxLayout.h"
#include "../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"
#include "WndShadow.h"
#include "../../Components/UIFramework/CreateRgnFromFile.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "../../Components/UIFramework/interface/QCommonControlBase.h"
#include "LanguageSubmenu.h"
#include "AboutSubmenu.h"
#include "QAboutBox.h"
#pragma once

#ifndef DRAGGING
#define DRAGGING 0x01
#endif

namespace QSYNC_LIB
{

	typedef Win32DoubleBufferFormCWndWidge<CMDIFrameWnd>			Win32DoubleBufferMDIFrameWndWidge;

	const int nMIN_WIDTH_MAINFRAME = 720;
	const int nMIN_HEIGHT_MAINFRAME = 540;

	class CMainFrame : public Win32DoubleBufferMDIFrameWndWidge , public IMainframe , public INotify
	{
		DECLARE_DYNAMIC(CMainFrame)
	public:
		CMainFrame();

		// Attributes
	public:

		// Operations
	public:
		//IMainframe
		virtual bool IsConnected(TSTRING& strName, TSTRING& strIMEI);

		// Overrides
	public:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs,CCreateContext* pContext);
		virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
		virtual void Event(const TSTRING& strEvent,long nParam);


		void SetStatusText(const CString& strText);
		bool AddWidget(QSyncerFramework* pQSyncerFramework,Widge<HDC>* pIWidget);
		bool SwitchWidget(QSyncerFramework* pQSyncerFramework);

		// Implementation
	public:
		virtual ~CMainFrame();
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

	protected:
		void ShowFunctionBar();
		void FindAllViews();
		void LoadViewFomDll(LPCTSTR strDll);
        void SetFramePictures();
		void SetUI();
		static void	fnLanguage();
		static void	WinClose();
		static void	WinMin();
		static void	About();
		void	FnLanguage();
		void	FnWinClose();
		void	FnWinMin();
		void	FnAbout();
		
    private:
        void OnSize(UINT nType, int cx, int cy);
        struct ThreadParam {
            HRGN* pHRgn;
            CCreateRgnFromFile* pCrff;
            CEvent* pEvent;
            int width;
            int height;
            int offX;
            int offY;
        };
        static UINT GetRgnThread(LPVOID thParam);
		virtual void MouseDown(int nCX,int nCY);
	protected:  // control bar embedded members
		CFunctionBar				m_FunctionBar;
		IConfigure*						m_IConfigure;
		CCfgInt							m_nX;
		CCfgInt							m_nY;
		CCfgInt							m_nW;
		CCfgInt							m_nH;

		WIN32LabelWidge* 	    m_pStatusWidget;
		SplitLayout<HDC>*	    m_pSplitLayout;
		SplitLayout<HDC>*		m_pMainLayout;
		SplitLayout<HDC>*       m_pSplitLogoFunctionLayout;
		SplitLayout<HDC>*       m_pSplitLogoLayout;
		SplitLayout<HDC>*       m_pSplitButtonLayout;

		StackLayout<QSyncerFramework*,HDC>* m_pLayout;
        CCreateRgnFromFile* m_pMainFrmRgn1; // top left
        CCreateRgnFromFile* m_pMainFrmRgn2; // top
        CCreateRgnFromFile* m_pMainFrmRgn3; // top right
        CCreateRgnFromFile* m_pMainFrmRgn4; // left
        CCreateRgnFromFile* m_pMainFrmRgn5; // center
        CCreateRgnFromFile* m_pMainFrmRgn6; // right
        CCreateRgnFromFile* m_pMainFrmRgn7; // bottom left
        CCreateRgnFromFile* m_pMainFrmRgn8; // bottom
        CCreateRgnFromFile* m_pMainFrmRgn9; // bottom right
        bool m_bCreated;
        HANDLE m_handle[9]; // control the following event
        CEvent m_event1;
        CEvent m_event2;
        CEvent m_event3;
        CEvent m_event4;
        CEvent m_event5;
        CEvent m_event6;
        CEvent m_event7;
        CEvent m_event8;
        CEvent m_event9;

		// Generated message map functions
	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnClose();
		DECLARE_MESSAGE_MAP()	

	private:
		std::vector<CWnd*> m_AllView;
		CWndShadow			m_Shadow;
        CLayoutManager*    m_pLM;
        int m_nEnableShadow; // 1 means true, 0 means false
        int m_nFrameStyle;
        DWORD m_dwFlags;     // used for drag to move the window        
		int			m_nOffsetX;
		int			m_nOffsetY;

		ILocalization*					m_pILocalization;
		QCommonControlBase    m_QCommonControlBase_Language;
		LanguageSubmenu m_LanguageSubmenu;
		QCommonControlBase    m_QCommonControlBase_About;
		AboutBoxSubmenu m_AboutSubmenu;
		WIN32IconButton* m_pLanguageButton;
		WIN32IconButton* m_pAboutButton;
	};

}