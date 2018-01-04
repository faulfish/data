// CDAutoRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CDAutoRun.h"
#include "CDAutoRunDlg.h"
#include "../../../Components/UIFramework/interface/QControlNotify.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCDAutoRunDlg dialog


namespace QSYNC_LIB
{
	CCDAutoRunDlg* g_pThis = NULL;

	CCDAutoRunDlg::CCDAutoRunDlg(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(CCDAutoRunDlg::IDD, pParent)
	{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		CreateILocalization(&m_pILocalization);
		g_pThis = this;
#ifdef _DEBUG
		m_strFunction_INIFile_Path = _T("..\\Debug\\BtnFunction\\BtnFunction.ini");
#else
		m_strFunction_INIFile_Path = _T("..\\BtnFunction\\BtnFunction.ini");
#endif	
	}

	void CCDAutoRunDlg::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
	}

	BEGIN_MESSAGE_MAP(CCDAutoRunDlg, Win32DoubleBufferQDialogWidge)
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		ON_WM_SIZING()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()


	// CCDAutoRunDlg message handlers

	BOOL CCDAutoRunDlg::OnInitDialog()
	{
		CQDialog::OnInitDialog();

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetUI();
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO: Add extra initialization here

		return TRUE;  // return TRUE  unless you set the focus to a control
	}

	// If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.

	void CCDAutoRunDlg::OnPaint()
	{
		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CQDialog::OnPaint();
		}
	}

	// The system calls this function to obtain the cursor to display while the user drags
	//  the minimized window.
	HCURSOR CCDAutoRunDlg::OnQueryDragIcon()
	{
		return static_cast<HCURSOR>(m_hIcon);
	}

	void CCDAutoRunDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}
	void CCDAutoRunDlg::Event(const TSTRING& strEvent,long nParam)
	{
		m_pILocalization->SetRegion(strEvent);
	}

	void CCDAutoRunDlg::SetUI()
	{
		SetBackground();
		BoxLayout<HDC>* pMainLayout =new BoxLayout<HDC>(TopToBottom);

		TSTRING strWinCloseOriginal = _T("Btn_Close_Original.png");
		TSTRING strWinCloseMouseOver = _T("Btn_Close_MouseOver.png");
		TSTRING strWinCloseMouseDown = _T("Btn_Close_MouseDown.png");
		TSTRING strWinCloseDisable = _T("Btn_Close_Disable.png");
		WIN32IconButton* pWinCloseBtn = new WIN32IconButton(this,L"",strWinCloseOriginal,strWinCloseMouseOver,strWinCloseMouseDown,strWinCloseDisable,WinClose);
		pWinCloseBtn->SetWidthHeightByImage();
		pMainLayout->AddWidget(pWinCloseBtn);

		TSTRING strDisplay = _T("IDS_LANGUAGE");
		TSTRING strLanguageBKImage = _T("FunctionBtn\\Btn_DeepGreen_Original.png");
		TSTRING strLanguageHighLightImage = _T("FunctionBtn\\Btn_DeepGreen_MouseOver.png");
		TSTRING strLanguageClickImage = _T("FunctionBtn\\Btn_DeepGreen_MouseDown.png");
		TSTRING strLanguageDisableImage = _T("FunctionBtn\\Btn_DeepGreen_Disable.png");
		m_pLanguageBtn = new WIN32IconButton(this,strDisplay,strLanguageBKImage,strLanguageHighLightImage,strLanguageClickImage,strLanguageDisableImage,Language);
		m_pLanguageBtn->SetWidthHeightByImage();
		m_pLanguageBtn->SetTextColor(255,255,255,255,255, 15, 85, 105);
		m_pLanguageBtn->SetFontSize(9);
		pMainLayout->AddWidget(m_pLanguageBtn);
		pMainLayout->AddWidget(CreateFunctionLayoutFromINI());		

		SetLayout(pMainLayout);
	}
	void CCDAutoRunDlg::SetBackground()
	{
		TSTRING strBkImage = L"BK_CDAutoRun.png";
		SetBKImage(strBkImage,BK_Fixed);

		const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
		const int nHeight_AboutBox = GetBKImage()->GetHeight();

		SetMinWidth(nWIDTH_AboutBox);
		SetMinHeight(nHeight_AboutBox);
		SetMaxWidth(nWIDTH_AboutBox);
		SetMaxHeight(nHeight_AboutBox);
	}
	SplitLayout<HDC>* CCDAutoRunDlg::CreateFunctionLayoutFromINI()
	{
		BoxLayout<HDC>* pTextLayout = new BoxLayout<HDC>(TopToBottom);
		BoxLayout<HDC>* pFunBtnLayout = new BoxLayout<HDC>(TopToBottom);

		int nNumberOfFunction = GetNumberOfFunctions();
		for (int nIndex = 0; nIndex<nNumberOfFunction;++nIndex)
		{
			CString strSection;
			strSection.Format(_T("Function%d"),nIndex);

			//TSTRING strImgOriginal = _T("FunctionBtn\\Btn_White_Original.png");
			//TSTRING strImgMouseOver = _T("FunctionBtn\\Btn_White_MouseOver.png");
			//TSTRING strImgMouseDown = _T("FunctionBtn\\Btn_White_MouseDown.png");
			//TSTRING strImgDisable = _T("FunctionBtn\\Btn_White_Disable.png");
			TSTRING strPrefix = _T("FunctionBtn\\");
			const DWORD dwBufferSize = 127;
			TCHAR strDisplay[dwBufferSize] = {0};
			TCHAR strImgOriginal[dwBufferSize] = {0};
			TCHAR strImgMouseOver[dwBufferSize] = {0};
			TCHAR strImgMouseDown[dwBufferSize] = {0};
			TCHAR strImgDisable[dwBufferSize] = {0};
			GetPrivateProfileString (strSection,_T("FN_Description"),NULL, strDisplay, dwBufferSize,m_strFunction_INIFile_Path);
			GetPrivateProfileString (strSection,_T("FN_BtnImg_Original"),NULL, strImgOriginal, dwBufferSize,m_strFunction_INIFile_Path);
			GetPrivateProfileString (strSection,_T("FN_BtnImg_MouseOver"),NULL, strImgMouseOver, dwBufferSize,m_strFunction_INIFile_Path);
			GetPrivateProfileString (strSection,_T("FN_BtnImg_MouseDown"),NULL, strImgMouseDown, dwBufferSize,m_strFunction_INIFile_Path);
			GetPrivateProfileString (strSection,_T("FN_BtnImg_Disable"),NULL, strImgDisable, dwBufferSize,m_strFunction_INIFile_Path);

			WIN32IconButton* pFnBtn = new WIN32IconButton(
				this
				,_T("")
				,strPrefix + strImgOriginal
				,strPrefix + strImgMouseOver
				,strPrefix + strImgMouseDown
				,strPrefix + strImgDisable
				,INIBtnFunction
				,nIndex);
			pFnBtn->SetWidthHeightByImage();
			pFnBtn->SetFontSize(9);
			pFunBtnLayout->AddWidget(pFnBtn);
			pTextLayout->AddWidget(new WIN32LabelWidge(this,strDisplay,Left),-1,pFnBtn->GetMaxHeight());
			m_vecFunctionBtn.push_back(pFnBtn);
		}

		SplitLayout<HDC>* pReturnLayout = new SplitLayout<HDC>;
		pReturnLayout->AddWidget(pTextLayout,pFunBtnLayout,Right|Top,Left|Top);
		return pReturnLayout;
	}
	int CCDAutoRunDlg::GetNumberOfFunctions()
	{
		int nNumberOfFunction = 0;
		CString strSectionName;
		while(1)
		{
			strSectionName.Format(_T("Function%d"),nNumberOfFunction);
			if(GetPrivateProfileInt(strSectionName,_T("FN_Description"),300,m_strFunction_INIFile_Path)!=300)
				++nNumberOfFunction;
			else
				break;
		}
		return nNumberOfFunction;
	}


	//Button functions:
	void	CCDAutoRunDlg::Language()
	{
		g_pThis->FnLanguage();
	}
	void	CCDAutoRunDlg::FnLanguage()
	{	
		m_QCommonControlBase_Language.SetSubMenu(&m_LanguageSubmenu);
		for(int i =0;i < m_LanguageSubmenu.GetMenuItemCount();i++)
		{
			TSTRING strDisplay,strNotifyID;
			m_LanguageSubmenu.GetMenuItem(i,strDisplay,strNotifyID);
			m_QCommonControlBase_Language.Register(this,strNotifyID);
		}
		GeometryRect rcLangBtn = m_pLanguageBtn->GetGeometry();
		RECT rcMainFrame = {0};
		GetClientRect(&rcMainFrame);
		ClientToScreen(&rcMainFrame);

		QPoint ptPoint;
		ptPoint.m_nPosX = rcMainFrame.left + rcLangBtn.m_nX;
		ptPoint.m_nPosY = rcMainFrame.top + rcLangBtn.m_nY + rcLangBtn.m_nHeight;
		m_QCommonControlBase_Language.Event(EVENT_SUBMENU,(long)&ptPoint);
	}
	void	CCDAutoRunDlg::WinClose()
	{
		g_pThis->FnWinClose();
	}
	void	CCDAutoRunDlg::FnWinClose()
	{
		PostMessage(WM_CLOSE);
	}
	void CCDAutoRunDlg::INIBtnFunction(UINT nIndex)
	{
		g_pThis->FnINIBtnFunction(nIndex);
	}
	void CCDAutoRunDlg::FnINIBtnFunction(UINT nIndex)
	{
		CString strSection;
		strSection.Format(_T("Function%d"),nIndex);

#ifdef _DEBUG
		TSTRING strPrefix = L"..\\Debug\\";
#else
		TSTRING strPrefix = L"";
#endif

		const DWORD dwBufferSize = 16;
		TCHAR strFileName[dwBufferSize] = {0};  //change size depending on needs	
		DWORD nFileNameReturn = GetPrivateProfileString (strSection,_T("FN_OpenFile_Path"),NULL, strFileName, dwBufferSize,m_strFunction_INIFile_Path);
		TCHAR pstrOpenParameter[dwBufferSize] = {0};
		DWORD nParameterReturn = GetPrivateProfileString (strSection,_T("FN_OpenFile_Parameter"),NULL, pstrOpenParameter, dwBufferSize,m_strFunction_INIFile_Path);
		TSTRING strFilePath = strPrefix + strFileName;
		if((int)ShellExecute(NULL,_T("open"),strFilePath.c_str(),pstrOpenParameter,NULL,SW_SHOWNORMAL) <= 32)//if ShellExecute fail, it's return value <= 32
		{//open fail
			assert(0);
		}
	}

}