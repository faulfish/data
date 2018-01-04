/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   MainFrm.cpp  $
*  $Revision:   1.82  $
*  $Date:   Mar 18 2008 09:53:46  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/QSyncer2/MainFrm.cpp.-arc  $
   
      Rev 1.82   Mar 18 2008 09:53:46   Alan Tu
   refactory
   
      Rev 1.81   Mar 18 2008 09:48:06   Alan Tu
   remove pictures2
   
      Rev 1.80   Mar 04 2008 22:38:38   Alan Tu
   update
   
      Rev 1.79   Mar 04 2008 21:45:22   Alan Tu
   update
   
      Rev 1.78   Mar 03 2008 13:51:34   Alan Tu
   remove log
   
      Rev 1.77   Feb 20 2008 10:13:06   Alan Tu
   update caption
   
      Rev 1.76   Feb 18 2008 09:40:36   Alan Tu
   Add parent to messagebox
   
      Rev 1.75   Feb 04 2008 17:16:10   Jame Tsai
   fix error
   
      Rev 1.74   Feb 01 2008 17:53:48   Alan Tu
   update
   
      Rev 1.73   Jan 31 2008 13:59:02   Alan Tu
   update
   
      Rev 1.72   Jan 31 2008 13:49:42   Alan Tu
   update
   
      Rev 1.71   Jan 31 2008 10:31:26   Alan Tu
   update
   
      Rev 1.70   Jan 29 2008 16:53:12   Alan Tu
   update
   
      Rev 1.69   Jan 29 2008 15:57:00   Alan Tu
   update
   
      Rev 1.68   Jan 23 2008 15:51:14   Mei Yang
   // m_pLM->LoadXML(strPath);
   Add header
*
*
***************************************************************************/

#include "stdafx.h"
#include "QSyncer2.h"

#include "MainFrm.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "../../Components/UIFramework/interface/QControlNotify.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "..\..\CommonUtil\LogPolicy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
namespace QSYNC_LIB
{
    // CMainFrame

    IMPLEMENT_DYNAMIC(CMainFrame, Win32DoubleBufferMDIFrameWndWidge)

        BEGIN_MESSAGE_MAP(CMainFrame, Win32DoubleBufferMDIFrameWndWidge)
            ON_WM_CREATE()
            ON_WM_TIMER()
            ON_WM_ACTIVATE()
            ON_WM_CLOSE()
			ON_WM_SIZE()
        END_MESSAGE_MAP()

        static UINT indicators[] =
    {
        ID_SEPARATOR,           // status line indicator
        ID_INDICATOR_CAPS,
        ID_INDICATOR_NUM,
        ID_INDICATOR_SCRL,
    };


    // CMainFrame construction/destruction
	CMainFrame* g_pThis = NULL;

    CMainFrame::CMainFrame()
        :m_FunctionBar(static_cast<IMainframe*>(this))
        ,m_pLayout(NULL)
        ,m_pSplitLayout(NULL)
        ,m_pMainLayout(NULL)
		,m_pSplitLogoFunctionLayout(NULL)
		,m_pSplitLogoLayout(NULL)
		,m_pSplitButtonLayout(NULL)
        ,m_IConfigure(CreateIConfigure(_T("Mainframe")))
        ,m_nX(m_IConfigure,_T("X"),0)
        ,m_nY(m_IConfigure,_T("Y"),0)
        ,m_nW(m_IConfigure,_T("W"),nMIN_WIDTH_MAINFRAME)
        ,m_nH(m_IConfigure,_T("H"),nMIN_HEIGHT_MAINFRAME)
        ,m_pStatusWidget(new WIN32LabelWidge(this, L"",Left,L"Arial", 9))
        ,m_bCreated(false)
        ,m_pMainFrmRgn1(NULL)
        ,m_pMainFrmRgn2(NULL)
        ,m_pMainFrmRgn3(NULL)
        ,m_pMainFrmRgn4(NULL)
        ,m_pMainFrmRgn5(NULL)
        ,m_pMainFrmRgn6(NULL)
        ,m_pMainFrmRgn7(NULL)
        ,m_pMainFrmRgn8(NULL)
        ,m_pMainFrmRgn9(NULL)
        ,m_nFrameStyle(0)
        ,m_dwFlags(0)
		,m_pLanguageButton(NULL)
		,m_pAboutButton(NULL)
		,m_pLM(new CLayoutManager)
    {
		TRACE_FILE_LINE_INFO();

		g_pThis = this;

        if(m_IConfigure)
			m_IConfigure->Load(STORAGE_INIFILE);

        SetFramePictures();
		CreateILocalization(&m_pILocalization);
    }

    CMainFrame::~CMainFrame()
    {
		TRACE_FILE_LINE_INFO();

        if(m_IConfigure)
        {
            m_IConfigure->Save(STORAGE_INIFILE);
            DeleteIConfigure(_T("Mainframe"));
        }
        delete m_pMainFrmRgn1;
        delete m_pMainFrmRgn2;
        delete m_pMainFrmRgn3;
        delete m_pMainFrmRgn4;
        delete m_pMainFrmRgn5;
        delete m_pMainFrmRgn6;
        delete m_pMainFrmRgn7;
        delete m_pMainFrmRgn8;
        delete m_pMainFrmRgn9;
		if(m_pLM)
			delete m_pLM;
    }

    FORCEINLINE void CMainFrame::SetFramePictures() 
    {
		TRACE_FILE_LINE_INFO();

        XMLMANAGER::XMLElement elm;
		if(m_pLM)
			m_pLM->GetElementByID(elm, _T("MainFrameID"));
        m_nFrameStyle = elm.getAttr(_T("FrameStyle"));
        XMLAllElement* vecAllElm = elm.getAllElement();
        if(vecAllElm->size() > 2)
        {
            XMLMANAGER::XMLElement subElm = vecAllElm->at(m_nFrameStyle);
            TSTRING strFirst, strSecond, strThird, strFourth, strFifth, strSixth, strSeventh, strEighth, strNinth;

			CLayoutManager LM;
			TCHAR szBuffer[MAX_PATH] = {0};
			_tcscpy(szBuffer,LM.GetPath().c_str());
			PathRemoveFileSpec(szBuffer);
			PathAddBackslash(szBuffer);
			CString strPicPath = szBuffer;

            switch(m_nFrameStyle) {
            case 0: // normal style
                break;
            case 1: // fixed picture and frame
                subElm.getAttr(_T("FixedPic"),   strFirst);
                m_pMainFrmRgn1 = new CCreateRgnFromFile(strPicPath+strFirst.c_str()  , RGB(0,0,0));
                break;
            case 2: // nine pictures to load
                subElm.getAttr(_T("First"),   strFirst  );
                subElm.getAttr(_T("Second"),  strSecond );
                subElm.getAttr(_T("Third"),   strThird  );
                subElm.getAttr(_T("Fourth"),  strFourth );
                subElm.getAttr(_T("Fifth"),   strFifth  );
                subElm.getAttr(_T("Sixth"),   strSixth  );
                subElm.getAttr(_T("Seventh"), strSeventh);
                subElm.getAttr(_T("Eighth"),  strEighth );
                subElm.getAttr(_T("Ninth"),   strNinth  );
                m_pMainFrmRgn1 = new CCreateRgnFromFile(strPicPath+strFirst.c_str()  , RGB(0,0,0));
                m_pMainFrmRgn2 = new CCreateRgnFromFile(strPicPath+strSecond.c_str() , RGB(0,0,0));
                m_pMainFrmRgn3 = new CCreateRgnFromFile(strPicPath+strThird.c_str()  , RGB(0,0,0));
                m_pMainFrmRgn4 = new CCreateRgnFromFile(strPicPath+strFourth.c_str() , RGB(0,0,0));
                m_pMainFrmRgn5 = new CCreateRgnFromFile(strPicPath+strFifth.c_str()  , RGB(0,0,0));
                m_pMainFrmRgn6 = new CCreateRgnFromFile(strPicPath+strSixth.c_str()  , RGB(0,0,0));
                m_pMainFrmRgn7 = new CCreateRgnFromFile(strPicPath+strSeventh.c_str(), RGB(0,0,0));
                m_pMainFrmRgn8 = new CCreateRgnFromFile(strPicPath+strEighth.c_str() , RGB(0,0,0));
                m_pMainFrmRgn9 = new CCreateRgnFromFile(strPicPath+strNinth.c_str()  , RGB(0,0,0));
                m_handle[0] = (HANDLE)m_event1;
                m_handle[1] = (HANDLE)m_event2;
                m_handle[2] = (HANDLE)m_event3;
                m_handle[3] = (HANDLE)m_event4;
                m_handle[4] = (HANDLE)m_event5;
                m_handle[5] = (HANDLE)m_event6;
                m_handle[6] = (HANDLE)m_event7;
                m_handle[7] = (HANDLE)m_event8;
                m_handle[8] = (HANDLE)m_event9;
                break;
            default:
                break;
            }
        }
    }

    LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
    {
        CPoint pt;
        LRESULT lr = 0;
        lr = Win32DoubleBufferMDIFrameWndWidge::WindowProc(message, wParam, lParam);
        switch(message)
        {
        case WM_SIZE:			
			TRACE_FILE_LINE_INFO();
            if(m_nFrameStyle == 1 || m_nFrameStyle == 2)
                OnSize(static_cast<UINT>(wParam), LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_LBUTTONDOWN:
			TRACE_FILE_LINE_INFO();
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            OnLButtonDown(0, pt);
            break;
        case WM_LBUTTONUP:
			TRACE_FILE_LINE_INFO();
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            OnLButtonUp(0, pt);
            break;
        case WM_MOUSEMOVE:
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            OnMouseMove(0, pt);
            break;
        }
        return lr;
    }

    BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs,CCreateContext* pContext)
    {
		TRACE_FILE_LINE_INFO();

        CreateClient(lpcs, NULL);

        if(m_hWndMDIClient)
            ::ShowWindow(m_hWndMDIClient,SW_HIDE);

        return TRUE;
    }

    int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
    {
		TRACE_FILE_LINE_INFO();

        // init to hide the window, show the window on OnTimer
        AfxGetApp()->m_nCmdShow = SW_HIDE;
        if (Win32DoubleBufferMDIFrameWndWidge::OnCreate(lpCreateStruct) == -1)
            return -1;

        //Set shadow window
        XMLMANAGER::XMLElement elm;
		if(m_pLM)
			m_pLM->GetElementByID(elm, _T("MainFrameID"));
        m_nEnableShadow = elm.getAttr(_T("EnableShadow"));
        XMLAllElement* vecAllElm = elm.getAllElement();
        if(m_nEnableShadow)
        {
            if(vecAllElm->size() != 0) {
                XMLMANAGER::XMLElement subElm = vecAllElm->at(0);
                int nX = subElm.getAttr(_T("PositionX"));
                int nY = subElm.getAttr(_T("PositionY"));
                int nSize = subElm.getAttr(_T("Size"));
				int nSharp = subElm.getAttr(_T("Sharpness"));
				int nDark = subElm.getAttr(_T("Darkness"));
				int nR = subElm.getAttr(_T("ColorR"));
				int nG = subElm.getAttr(_T("ColorG"));
				int nB = subElm.getAttr(_T("ColorB"));
                m_Shadow.Create(GetSafeHwnd());
                m_Shadow.SetPosition(nX,nY);
                m_Shadow.SetSize(nSize);
				m_Shadow.SetColor(RGB(nR, nG, nB));
				m_Shadow.SetDarkness(nDark);
				m_Shadow.SetSharpness(nSharp);
            }
        }

		m_FunctionBar.Create(IDD_FUNCTION_DLG,this);
		SetUI();
		CQSyncer2App* pApp = (CQSyncer2App*)::AfxGetApp();	
		if(pApp)
		{
			pApp->Register(&m_FunctionBar,DEVICE_CONNECT);
			pApp->Register(&m_FunctionBar,DEVICE_DISCONNECT);
		}

		if(m_nW.Get() >= GetSystemMetrics(SM_CXMAXIMIZED) ||
			m_nH.Get() >= GetSystemMetrics(SM_CYMAXIMIZED)) {
				AfxGetMainWnd()->ShowWindow(SW_SHOWMAXIMIZED);
		} else {
			MoveWindow(
				m_nX.Get() >= 0 ? m_nX.Get() : 0,
				m_nY.Get() >= 0 ? m_nY.Get() : 0,
				m_nW.Get() > nMIN_WIDTH_MAINFRAME ? m_nW.Get() : nMIN_WIDTH_MAINFRAME,
				m_nH.Get() > nMIN_HEIGHT_MAINFRAME ? m_nH.Get() : nMIN_HEIGHT_MAINFRAME
				);
			AfxGetMainWnd()->ShowWindow(SW_SHOW);
		}

        SetTimer(1,1,NULL);
        return 0;
    }

	void CMainFrame::OnTimer(UINT_PTR nIDEvent)
	{
		TRACE_FILE_LINE_INFO();

		static bool bFlag = false;
		if(nIDEvent == 1)
		{
			KillTimer(nIDEvent);			
			if(bFlag)
			{
				TRACE_FILE_LINE_INFO();
				return;
			}
			bFlag = true;

			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			try{
			m_FunctionBar.FindViews();
			}
			catch(...)
			{
				TRACE_FILE_LINE_INFO();
			}

			GetDeviceFinder()->StartMonitor();
			/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		}

		Win32DoubleBufferMDIFrameWndWidge::OnTimer(nIDEvent);
		m_bCreated = true;
	}

    bool CMainFrame::AddWidget(QSyncerFramework* pQSyncerFramework,Widge<HDC>* pIWidget)
    {
		TRACE_FILE_LINE_INFO();

        bool bRes = false;
        if(m_pLayout)
        {
            bRes = m_pLayout->AddWidget(pQSyncerFramework,pIWidget);
            SetLayout();
        }
        return bRes;
    }

    bool CMainFrame::SwitchWidget(QSyncerFramework* pQSyncerFramework)
    {
		TRACE_FILE_LINE_INFO();

        bool bRes = false;
        if(m_pLayout)
        {
            bRes = m_pLayout->Switch(pQSyncerFramework);
            SetLayout();
        }
		else assert(0);//Make sure the status is correct
        return bRes;
    }

    BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
    {
        if( !CMDIFrameWnd::PreCreateWindow(cs) )
            return FALSE;
        // TODO: Modify the Window class or styles here by modifying
        //  the CREATESTRUCT cs

        return TRUE;
    }

    // CMainFrame diagnostics

#ifdef _DEBUG
    void CMainFrame::AssertValid() const
    {
        Win32DoubleBufferMDIFrameWndWidge::AssertValid();
    }

    void CMainFrame::Dump(CDumpContext& dc) const
    {
        Win32DoubleBufferMDIFrameWndWidge::Dump(dc);
    }

#endif //_DEBUG


    // CMainFrame message handlers
    void CMainFrame::ShowFunctionBar() 
    {
		TRACE_FILE_LINE_INFO();
    }

	void CMainFrame::SetUI()
	{
		TRACE_FILE_LINE_INFO();

		TSTRING strBkImage = L"MainFrame\\BK_Main.png";
		SetBKImage(strBkImage,BK_Fixed);

		using namespace QSYNC_LIB;
		if(m_pLayout == NULL)
			m_pLayout = new StackLayout<QSyncerFramework*,HDC>;
		if(m_pSplitLayout == NULL)
			m_pSplitLayout = new SplitLayout<HDC>(false);
		if(m_pMainLayout == NULL)
			m_pMainLayout = new SplitLayout<HDC>(false);
		if(m_pSplitLogoFunctionLayout == NULL)
			m_pSplitLogoFunctionLayout = new SplitLayout<HDC>(false);
		if(m_pSplitLogoLayout == NULL)
			m_pSplitLogoLayout = new SplitLayout<HDC>(true);
		if(m_pSplitButtonLayout == NULL)
			m_pSplitButtonLayout = new SplitLayout<HDC>(false);

		m_pMainLayout->SetFixedValue(487);

		BoxLayout<HDC>* pWindowControl = new BoxLayout<HDC>(RightToLeft); //擺放Help,最小化和關閉按鈕
		pWindowControl->SetCell(26,25);
		pWindowControl->SetSpacing(1);

		TSTRING strEmpty ;

		TSTRING strWinCloseOriginal = _T("General\\Button\\Btn_Close_Original.png");
		TSTRING strWinCloseMouseOver = _T("General\\Button\\Btn_Close_MouseOver.png");
		TSTRING strWinCloseMouseDown = _T("General\\Button\\Btn_Close_MouseDown.png");
		TSTRING strWinCloseDisable = _T("General\\Button\\Btn_Close_Disable.png");
		pWindowControl->AddWidget(new WIN32ButtonWidge(this,strEmpty,strWinCloseOriginal,strWinCloseMouseOver,strWinCloseMouseDown,strWinCloseDisable,WinClose));

		TSTRING strWinMinOriginal = _T("General\\Button\\Btn_Minimize_Original.png");
		TSTRING strWinMinMouseOver = _T("General\\Button\\Btn_Minimize_MouseOver.png");
		TSTRING strWinMinMouseDown = _T("General\\Button\\Btn_Minimize_MouseDown.png");
		TSTRING strWinMinDisable = _T("General\\Button\\Btn_Minimize_Disable.png");
		pWindowControl->AddWidget(new WIN32ButtonWidge(this,strEmpty,strWinMinOriginal,strWinMinMouseOver,strWinMinMouseDown,strWinMinDisable,WinMin));

		TSTRING strAboutOriginal = _T("General\\Button\\Btn_About_Original.png");
		TSTRING strAboutMouseOver = _T("General\\Button\\Btn_About_MouseOver.png");
		TSTRING strAboutMouseDown = _T("General\\Button\\Btn_About_MouseDown.png");
		TSTRING strAboutDisable = _T("General\\Button\\Btn_About_Disable.png");
		m_pAboutButton = new WIN32IconButton(this,strEmpty,strAboutOriginal,strAboutMouseOver,strAboutMouseDown,strAboutDisable,About);
		pWindowControl->AddWidget(m_pAboutButton);

		m_pSplitButtonLayout->AddWidget(pWindowControl,Right,true);
		m_pSplitButtonLayout->SetVariouslyMargin(3,1,0,7,true);

		TSTRING strDisplay = _T("IDS_LANGUAGE");
		TSTRING strLanguageBKImage = _T("MainFrame\\Button\\Btn_Language_Original.png");
		TSTRING strLanguageHighLightImage = _T("MainFrame\\Button\\Btn_Language_MouseOver.png");
		TSTRING strLanguageClickImage = _T("MainFrame\\Button\\Btn_Language_MouseDown.png");
		TSTRING strLanguageDisableImage = _T("MainFrame\\Button\\Btn_Language_Disable.png");
		m_pLanguageButton = new WIN32IconButton(this,strDisplay,strLanguageBKImage,strLanguageHighLightImage,strLanguageClickImage,strLanguageDisableImage,fnLanguage);
		m_pLanguageButton->SetFixedWidth(98);
		m_pLanguageButton->SetFixedHeight(26);
		m_pLanguageButton->SetTextColor(255,255,255,255,255, 15, 85, 105);
		m_pLanguageButton->SetFontSize(9);
		m_pSplitButtonLayout->AddWidget(m_pLanguageButton,Right|VCenter,false);

		m_pSplitButtonLayout->SetFixedValue(29);
		m_pSplitButtonLayout->SetSpacing(2);

		std::wstring strLogoImage = _T("MainFrame\\LOGO.png");
		m_pSplitLogoLayout->AddWidget(new WIN32DrawerWidge(this,strLogoImage,Fixed),VCenter,true);
		m_pSplitLogoLayout->AddWidget(m_pSplitButtonLayout,0,false);
		m_pSplitLogoLayout->SetSpacing(9);
		m_pSplitLogoLayout->SetFixedValue(168);

		m_pSplitLogoFunctionLayout->AddWidget(m_pSplitLogoLayout,0,true);
		m_pSplitLogoFunctionLayout->AddWidget(new WIN32WindowWidge(&m_FunctionBar),0,false);
		//m_pSplitLogoFunctionLayout->AddWidget(new WIN32WindowWidge(&m_FunctionBar),425,60,VCenter|HCenter,false);
		//m_pSplitLogoFunctionLayout->SetSpacing(10);
		m_pSplitLogoFunctionLayout->SetFixedValue(67);
		//m_pSplitLogoFunction->/*GetWidget(false)->*/SetMargin(5,0,50,0);

		m_pSplitLayout->AddWidget(m_pSplitLogoFunctionLayout,0,true);
		m_pSplitLayout->AddWidget(m_pLayout,0,false);
		m_pSplitLayout->SetFixedValue(126,true);


		m_pMainLayout->AddWidget(m_pSplitLayout);
		TSTRING strStatusBkImage = L"MainFrame\\BK_StatusBar.png";
		m_pStatusWidget->SetBkImage(strStatusBkImage);
		m_pStatusWidget->SetTextColor(255,255,255,255);
		m_pStatusWidget->SetMargin(0,0,7,0);

		m_pMainLayout->AddWidget(m_pStatusWidget, HCenter|VCenter, false);
		m_pMainLayout->SetVariouslyMargin(0,0,10,9,true);
		m_pMainLayout->GetWidget(false)->SetFixedWidth(705);
		m_pMainLayout->GetWidget(false)->SetFixedHeight(31);
		m_pMainLayout->SetVariouslyMargin(0,0,0,0,false);

		SetMinWidth(nMIN_WIDTH_MAINFRAME);
		SetMinHeight(nMIN_HEIGHT_MAINFRAME);

		SetMaxWidth(nMIN_WIDTH_MAINFRAME);
		SetMaxHeight(nMIN_HEIGHT_MAINFRAME);
		SetLayout(m_pMainLayout);
	}

	void	CMainFrame::fnLanguage()
	{
		TRACE_FILE_LINE_INFO();

		g_pThis->FnLanguage();
	}

	void	CMainFrame::WinClose()
	{
		TRACE_FILE_LINE_INFO();

		g_pThis->FnWinClose();
	}

	void	CMainFrame::WinMin()
	{
		g_pThis->FnWinMin();
	}

	void	CMainFrame::About()
	{
		TRACE_FILE_LINE_INFO();

		g_pThis->FnAbout();
	}

	void	CMainFrame::FnLanguage()
	{	
		TRACE_FILE_LINE_INFO();

		m_QCommonControlBase_Language.SetSubMenu(&m_LanguageSubmenu);
		for(int i =0;i < m_LanguageSubmenu.GetMenuItemCount();i++)
		{
			TSTRING strDisplay,strNotifyID;
			m_LanguageSubmenu.GetMenuItem(i,strDisplay,strNotifyID);
			m_QCommonControlBase_Language.Register(this,strNotifyID);
		}

		GeometryRect rcLangBtn = m_pLanguageButton->GetGeometry();
		RECT rcMainFrame = {0};
		GetClientRect(&rcMainFrame);
		ClientToScreen(&rcMainFrame);

		QPoint ptPoint;
		ptPoint.m_nPosX = rcMainFrame.left + rcLangBtn.m_nX;
		ptPoint.m_nPosY = rcMainFrame.top + rcLangBtn.m_nY + rcLangBtn.m_nHeight;
		m_QCommonControlBase_Language.Event(EVENT_SUBMENU,(long)&ptPoint);
	}

	void	CMainFrame::FnWinClose()
	{
		TRACE_FILE_LINE_INFO();

		PostMessage(WM_CLOSE);
	}

	void	CMainFrame::FnWinMin()
	{
		TRACE_FILE_LINE_INFO();

		ShowWindow(SW_MINIMIZE);
	}

	void	CMainFrame::FnAbout()
	{
		TRACE_FILE_LINE_INFO();

		m_QCommonControlBase_About.SetSubMenu(&m_AboutSubmenu);
		int nMenuItemCount = m_AboutSubmenu.GetMenuItemCount();
		for(int i =0;i < nMenuItemCount;i++)
		{
			TSTRING strDisplay,strNotifyID;
			m_AboutSubmenu.GetMenuItem(i,strDisplay,strNotifyID);
			m_QCommonControlBase_About.Register(this,strNotifyID);
		}

		GeometryRect rcAboutBtn = m_pAboutButton->GetGeometry();
		RECT rcMainFrame = {0};
		GetClientRect(&rcMainFrame);
		ClientToScreen(&rcMainFrame);

		QPoint ptPoint;
		ptPoint.m_nPosX = rcMainFrame.left + rcAboutBtn.m_nX;
		ptPoint.m_nPosY = rcMainFrame.top + rcAboutBtn.m_nY + rcAboutBtn.m_nHeight;
		m_QCommonControlBase_About.Event(EVENT_SUBMENU,(long)&ptPoint);

		//assert(0);//need to implement
	}

    void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
    {
		TRACE_FILE_LINE_INFO();

        Win32DoubleBufferMDIFrameWndWidge::OnActivate(nState, pWndOther, bMinimized);

        if((nState == WA_ACTIVE) || (nState == WA_CLICKACTIVE))
        {
            if(pWndOther)
            {
                CWnd* pWnd = pWndOther;
            }
        }
    }

    void CMainFrame::SetStatusText(const CString& strText)
    {
		TRACE_FILE_LINE_INFO();

        if(m_pStatusWidget)
            m_pStatusWidget->SetDisplayString((LPCTSTR)strText);
		Update();
    }

    void CMainFrame::OnClose()
    {
		TRACE_FILE_LINE_INFO();

        CQSyncer2App* pApp = (CQSyncer2App*)::AfxGetApp();	
        if(pApp && pApp->CloseAllView())
        {
            CRect rcWindow;
            GetWindowRect(&rcWindow);
            m_nX = rcWindow.left > 0 ? rcWindow.left : 0;
            m_nY = rcWindow.top > 0 ? rcWindow.top : 0;
            m_nW = rcWindow.Width() > nMIN_WIDTH_MAINFRAME ? rcWindow.Width() : m_nW;
            m_nH = rcWindow.Height() > nMIN_HEIGHT_MAINFRAME ? rcWindow.Height() : m_nH;

            CQSyncer2App* pApp = (CQSyncer2App*)::AfxGetApp();	
            if(pApp)
            {
                pApp->Unregister(&m_FunctionBar,DEVICE_CONNECT);
                pApp->Unregister(&m_FunctionBar,DEVICE_DISCONNECT);
            }
            Win32DoubleBufferMDIFrameWndWidge::OnClose();
        }
    }

    bool CMainFrame::IsConnected(TSTRING& strName, TSTRING& strIMEI)
    {
		TRACE_FILE_LINE_INFO();

        bool bRes = false;
        CQSyncer2App* pApp = (CQSyncer2App*)::AfxGetApp();	
        if(pApp)
            bRes = pApp->IsConnected(strName,strIMEI);
        return bRes;
    }

    UINT CMainFrame::GetRgnThread(LPVOID thParam) 
	{
		try{
		TRACE_FILE_LINE_INFO();

        ThreadParam* tp= (ThreadParam*)thParam;
        *(tp->pHRgn) = tp->pCrff->GetResizeRgn(tp->width, tp->height);
        OffsetRgn(*(tp->pHRgn), tp->offX, tp->offY);
        tp->pEvent->SetEvent();
        AfxEndThread(0, TRUE);
		}
		catch(...)
		{
			AfxMessageBox(_T("Please contact enginer to solved."));
		}
        return TRUE;
    }
    void CMainFrame::OnSize(UINT nType, int cx, int cy) 
	{
		TRACE_FILE_LINE_INFO();

        if(cx == 0 && cy == 0) return;
        HRGN hRgn = CreateRectRgn(0, 0, 0, 0);
        switch(nType) {
            case SIZE_MINIMIZED:
                return;
                break;
            case SIZE_MAXIMIZED:
                hRgn = CreateRectRgn(0, 0, cx+5, cy+45);
                break;
            case SIZE_RESTORED:
                if(m_nFrameStyle == 2) {
                    HRGN hRgn1;// = m_pMainFrmRgn1->GetResizeRgn(m_pMainFrmRgn1->m_nWidth, m_pMainFrmRgn1->m_nHeight);
                    ThreadParam* tp1 = new ThreadParam;
                    tp1->pHRgn = &hRgn1;
                    tp1->pCrff = m_pMainFrmRgn1;
                    tp1->pEvent = &m_event1;
                    tp1->width = m_pMainFrmRgn1->m_nWidth;
                    tp1->height = m_pMainFrmRgn1->m_nHeight;
                    tp1->offX = 0;
                    tp1->offY = 0;

                    HRGN hRgn2;// = m_pMainFrmRgn2->GetResizeRgn(cx+7 - m_pMainFrmRgn1->m_nWidth - m_pMainFrmRgn3->m_nWidth, m_pMainFrmRgn2->m_nHeight);
                    ThreadParam* tp2 = new ThreadParam;
                    tp2->pHRgn = &hRgn2;
                    tp2->pCrff = m_pMainFrmRgn2;
                    tp2->pEvent = &m_event2;
                    tp2->width = cx+7 - m_pMainFrmRgn1->m_nWidth - m_pMainFrmRgn3->m_nWidth;
                    tp2->height = m_pMainFrmRgn2->m_nHeight;
                    tp2->offX = m_pMainFrmRgn1->m_nWidth;
                    tp2->offY = 0;

                    HRGN hRgn3;// = m_pMainFrmRgn3->GetResizeRgn(m_pMainFrmRgn3->m_nWidth, m_pMainFrmRgn3->m_nHeight);
                    ThreadParam* tp3 = new ThreadParam;
                    tp3->pHRgn = &hRgn3;
                    tp3->pCrff = m_pMainFrmRgn3;
                    tp3->pEvent = &m_event3;
                    tp3->width = m_pMainFrmRgn3->m_nWidth;
                    tp3->height = m_pMainFrmRgn3->m_nHeight;
                    tp3->offX = cx+7-m_pMainFrmRgn3->m_nWidth;
                    tp3->offY = 0;

                    HRGN hRgn4;// = m_pMainFrmRgn4->GetResizeRgn(m_pMainFrmRgn4->m_nWidth, cy+47 - m_pMainFrmRgn1->m_nHeight - m_pMainFrmRgn7->m_nHeight);
                    ThreadParam* tp4 = new ThreadParam;
                    tp4->pHRgn = &hRgn4;
                    tp4->pCrff = m_pMainFrmRgn4;
                    tp4->pEvent = &m_event4;
                    tp4->width = m_pMainFrmRgn4->m_nWidth;
                    tp4->height = cy+47 - m_pMainFrmRgn1->m_nHeight - m_pMainFrmRgn7->m_nHeight;
                    tp4->offX = 0;
                    tp4->offY = m_pMainFrmRgn1->m_nHeight;

                    HRGN hRgn5;// = m_pMainFrmRgn5->GetResizeRgn(cx+7 - m_pMainFrmRgn1->m_nWidth - m_pMainFrmRgn3->m_nWidth, cy+47 - m_pMainFrmRgn1->m_nHeight - m_pMainFrmRgn7->m_nHeight);
                    ThreadParam* tp5 = new ThreadParam;
                    tp5->pHRgn = &hRgn5;
                    tp5->pCrff = m_pMainFrmRgn5;
                    tp5->pEvent = &m_event5;
                    tp5->width = cx+7 - m_pMainFrmRgn1->m_nWidth - m_pMainFrmRgn3->m_nWidth;
                    tp5->height = cy+47 - m_pMainFrmRgn1->m_nHeight - m_pMainFrmRgn7->m_nHeight;
                    tp5->offX = m_pMainFrmRgn1->m_nWidth;
                    tp5->offY = m_pMainFrmRgn1->m_nHeight;

                    HRGN hRgn6;// = m_pMainFrmRgn6->GetResizeRgn(m_pMainFrmRgn6->m_nWidth, cy+47 - m_pMainFrmRgn3->m_nHeight - m_pMainFrmRgn9->m_nHeight);
                    ThreadParam* tp6 = new ThreadParam;
                    tp6->pHRgn = &hRgn6;
                    tp6->pCrff = m_pMainFrmRgn6;
                    tp6->pEvent = &m_event6;
                    tp6->width = m_pMainFrmRgn6->m_nWidth;
                    tp6->height = cy+47 - m_pMainFrmRgn3->m_nHeight - m_pMainFrmRgn9->m_nHeight;
                    tp6->offX = cx+7 - m_pMainFrmRgn6->m_nWidth;
                    tp6->offY = m_pMainFrmRgn3->m_nHeight;

                    HRGN hRgn7;// = m_pMainFrmRgn7->GetResizeRgn(m_pMainFrmRgn7->m_nWidth, m_pMainFrmRgn7->m_nHeight);
                    ThreadParam* tp7 = new ThreadParam;
                    tp7->pHRgn = &hRgn7;
                    tp7->pCrff = m_pMainFrmRgn7;
                    tp7->pEvent = &m_event7;
                    tp7->width = m_pMainFrmRgn7->m_nWidth;
                    tp7->height = m_pMainFrmRgn7->m_nHeight;
                    tp7->offX = 0;
                    tp7->offY = cy+47-m_pMainFrmRgn7->m_nHeight;

                    HRGN hRgn8;// = m_pMainFrmRgn8->GetResizeRgn(cx+7 - m_pMainFrmRgn7->m_nWidth - m_pMainFrmRgn9->m_nWidth, m_pMainFrmRgn8->m_nHeight);
                    ThreadParam* tp8 = new ThreadParam;
                    tp8->pHRgn = &hRgn8;
                    tp8->pCrff = m_pMainFrmRgn8;
                    tp8->pEvent = &m_event8;
                    tp8->width = cx+7 - m_pMainFrmRgn7->m_nWidth - m_pMainFrmRgn9->m_nWidth;
                    tp8->height = m_pMainFrmRgn8->m_nHeight;
                    tp8->offX = m_pMainFrmRgn7->m_nWidth;
                    tp8->offY = cy+47-m_pMainFrmRgn8->m_nHeight;

                    HRGN hRgn9;// = m_pMainFrmRgn9->GetResizeRgn(m_pMainFrmRgn9->m_nWidth, m_pMainFrmRgn9->m_nHeight);
                    ThreadParam* tp9 = new ThreadParam;
                    tp9->pHRgn = &hRgn9;
                    tp9->pCrff = m_pMainFrmRgn9;
                    tp9->pEvent = &m_event9;
                    tp9->width = m_pMainFrmRgn9->m_nWidth;
                    tp9->height = m_pMainFrmRgn9->m_nHeight;
                    tp9->offX = cx+7-m_pMainFrmRgn9->m_nWidth;
                    tp9->offY = cy+47-m_pMainFrmRgn9->m_nHeight;

                    AfxBeginThread(GetRgnThread, tp1);
                    AfxBeginThread(GetRgnThread, tp2);
                    AfxBeginThread(GetRgnThread, tp3);
                    AfxBeginThread(GetRgnThread, tp4);
                    AfxBeginThread(GetRgnThread, tp5);
                    AfxBeginThread(GetRgnThread, tp6);
                    AfxBeginThread(GetRgnThread, tp7);
                    AfxBeginThread(GetRgnThread, tp8);
                    AfxBeginThread(GetRgnThread, tp9);

                    WaitForMultipleObjects(9, m_handle, TRUE, INFINITE);

                    HRGN hRgnC1 = CreateRectRgn(0, 0, cx+7, cy+47);
                    CombineRgn(hRgnC1, hRgnC1, hRgn1, RGN_DIFF);
                    CombineRgn(hRgnC1, hRgnC1, hRgn2, RGN_DIFF);
                    CombineRgn(hRgnC1, hRgnC1, hRgn3, RGN_DIFF);
                    CombineRgn(hRgnC1, hRgnC1, hRgn4, RGN_DIFF);
                    CombineRgn(hRgnC1, hRgnC1, hRgn5, RGN_DIFF);
                    CombineRgn(hRgnC1, hRgnC1, hRgn6, RGN_DIFF);
                    CombineRgn(hRgnC1, hRgnC1, hRgn7, RGN_DIFF);
                    CombineRgn(hRgnC1, hRgnC1, hRgn8, RGN_DIFF);
                    CombineRgn(hRgn, hRgnC1, hRgn9, RGN_DIFF);
                    delete tp1;
                    delete tp2;
                    delete tp3;
                    delete tp4;
                    delete tp5;
                    delete tp6;
                    delete tp7;
                    delete tp8;
                    delete tp9;
                } else if(m_nFrameStyle == 1) {
                    RECT rectMainFrame = {0};
                    GetWindowRect(&rectMainFrame);
                    //HRGN mainFrameRGN = CreateRectRgn(0, 0, rectMainFrame.right - rectMainFrame.left, rectMainFrame.bottom - rectMainFrame.top);
                    RECT rect = {0};
                    GetClientRect(&rect);
                    ClientToScreen(&rect);
                    HRGN ClientRGN = CreateRectRgn(0, 0, m_pMainFrmRgn1->m_nWidth, m_pMainFrmRgn1->m_nHeight);
                    HRGN FixedPicRGN = m_pMainFrmRgn1->CreateRgnFromFile();
                    CombineRgn(hRgn, ClientRGN, FixedPicRGN, RGN_DIFF);
                    OffsetRgn(hRgn, rect.left - rectMainFrame.left, rect.top - rectMainFrame.top);
                    SetWindowRgn(hRgn, TRUE);
                    return;
                }
                break;
        }
        if(hRgn)
            SetWindowRgn(hRgn, TRUE);
    }

	void CMainFrame::MouseDown(int nCX,int nCY)
	{
		TRACE_FILE_LINE_INFO();

		Win32DoubleBufferMDIFrameWndWidge::MouseDown(nCX,nCY);

		CPoint point(nCX,nCY);
		PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
    }

	void CMainFrame::Event(const TSTRING& strEvent,long nParam)
	{
		if(_T("Help")==strEvent)
		{
			TRACE_FILE_LINE_INFO();

			TSTRING strCurrentRegion;
			m_pILocalization->GetCurrentRegion(strCurrentRegion);
			std::wstring strPrefix;
#ifdef _DEBUG
			strPrefix = L"..\\Debug\\";
#else
			strPrefix = L"";
#endif

			TSTRING strHelpFilePath = strPrefix + _T("help\\QSyncer_help_")+strCurrentRegion+_T(".pdf");
			if(_taccess(strHelpFilePath.c_str(),0) == -1)
				strHelpFilePath = strPrefix + _T("help\\QSyncer_help_enu.pdf");
			if((int)ShellExecute(NULL,_T("open"),strHelpFilePath.c_str(),NULL,NULL,SW_SHOWNORMAL)<=32)
				ShowMessage(GetSafeHwnd(),_T("IDS_HELPFILE_OPEN_ERROR"),_T("IDS_HELP"));			
		}
		else if(_T("About")==strEvent)
		{
			TRACE_FILE_LINE_INFO();

			CQAboutBox AboutBox(this);
			AboutBox.DoModal();
		}
		else 
		{
			TRACE_FILE_LINE_INFO();

			m_pILocalization->SetRegion(strEvent);
			Update();
		}

	}

}
#pragma warning(pop)
