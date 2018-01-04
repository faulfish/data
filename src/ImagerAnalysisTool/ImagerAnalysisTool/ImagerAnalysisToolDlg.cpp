// CDAutoRunDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ImagerAnalysisTool.h"
#include "ImagerAnalysisToolDlg.h"
#include "../../../code/Components/UIFramework/interface/QControlNotify.h"
#include "../../QLib_Export/QLib_export.h"
#include "../../../code/CommonUtil\Win32SHELLUtility.h"
#include "ImageBlockLocalization\ImageParser.h"
#include "CalculateFunction.h"
#include "../../../code/CommonUtil\Win32ThreadUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CImagerAnalysisToolDlg dialog


namespace QSYNC_LIB
{
	CImagerAnalysisToolDlg* g_pThis = NULL;

	CImagerAnalysisToolDlg::CImagerAnalysisToolDlg(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(CImagerAnalysisToolDlg::IDD, pParent)
		,m_pIConfigure(NULL)
		,m_AutoCaptureEvent(FALSE,TRUE)
		,m_CloseAppEvent(FALSE,TRUE)
		,m_ThreadCloseEvent(FALSE,TRUE)
	{
		m_AutoCaptureEvent.ResetEvent();
		m_ThreadCloseEvent.SetEvent();
		m_CloseAppEvent.ResetEvent();
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		CreateILocalization(&m_pILocalization);
		g_pThis = this;
		m_pIConfigure = CreateIConfigure(_T("AnalysisParameter"));

#ifdef _DEBUG
		m_strFunction_INIFile_Path = _T("..\\Debug\\BtnFunction\\BtnFunction.ini");
#else
		m_strFunction_INIFile_Path = _T("..\\BtnFunction\\BtnFunction.ini");
#endif	

		TCHAR szPath[MAX_PATH] = {0};
		bool bRes = Win32Utility::GetShellFolder(szPath,CSIDL_PERSONAL,_T("ImagerAnalysisTool"));
		m_strCapturedFile = szPath;
		m_strCapturedFile += _T("Sensor0.jpg");
	}

	void CImagerAnalysisToolDlg::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
	}

	BEGIN_MESSAGE_MAP(CImagerAnalysisToolDlg, Win32DoubleBufferQDialogWidge)
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		ON_WM_SIZING()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()


	// CImagerAnalysisToolDlg message handlers

	BOOL CImagerAnalysisToolDlg::OnInitDialog()
	{
		CQDialog::OnInitDialog();

		SetUI();

		//Initial parameter
		CCfgStr strCfgChart_Width = CCfgStr(m_pIConfigure,_T("Chart_Width"),_T(""));
		CCfgStr strCfgChart_Heigh = CCfgStr(m_pIConfigure,_T("Chart_Heigh"),_T(""));
		CCfgStr strCfgChart_Distance = CCfgStr(m_pIConfigure,_T("Chart_Distance"),_T(""));
		CCfgInt nCfgSensor = CCfgInt(m_pIConfigure,_T("Sensor"),0);
		CCfgInt nCfgResolution = CCfgInt(m_pIConfigure,_T("Resolution"),0);
		m_pIConfigure->Load(STORAGE_INIFILE);

		GetDlgItem(IDC_ChartWidth)->SetWindowText(strCfgChart_Width.Get().c_str());
		GetDlgItem(IDC_ChartHeigh)->SetWindowText(strCfgChart_Heigh.Get().c_str());
		GetDlgItem(IDC_Distance)->SetWindowText(strCfgChart_Distance.Get().c_str());
		nCfgSensor.Get() == 0 ? m_pBtnSensor0->SetButtonCheck(true) : m_pBtnSensor1->SetButtonCheck(true);
		m_ResolutionSubmenu.m_nCurrentResolution = nCfgResolution.Get();
		m_pCurrentResolution->SetDisplayString((TSTRING)(m_ResolutionSubmenu.GetCurrentResolutionByString()));
		m_pCurrentResolution->Update();

		::AfxBeginThread(TaskWorkerThreadProc,NULL);

		return TRUE;  // return TRUE  unless you set the focus to a control
	}

	// If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.

	void CImagerAnalysisToolDlg::OnPaint()
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
	HCURSOR CImagerAnalysisToolDlg::OnQueryDragIcon()
	{
		return static_cast<HCURSOR>(m_hIcon);
	}

	void CImagerAnalysisToolDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}
	void CImagerAnalysisToolDlg::Event(const TSTRING& strEvent,long nParam)
	{
		CString cstrEvent = strEvent.c_str();
		if (cstrEvent.Left(11) == _T("Resolution_"))
		{
			m_ResolutionSubmenu.m_nCurrentResolution = _tstoi(cstrEvent.Mid(11));
			m_pCurrentResolution->SetDisplayString((TSTRING)(m_ResolutionSubmenu.GetCurrentResolutionByString()));
			m_pCurrentResolution->Update();
		}
	}

	void CImagerAnalysisToolDlg::SetUI()
	{
		/*
		------------------------------------------------------
		| -------------- ------             ---------------  |
		| |  Capture   | |Auto|	     		|			  |	 |
		| -------------- ------             |             |  |
		| --------------         			|			  |  |
		| |  Sensor0   |    				|	Picture   |	 |
		| --------------      				|			  |	 |
		| --------------					|			  |	 |
		| |  Sensor1   |					|			  |	 |
		| --------------					---------------	 |
		| --------------                                     |
		| | Resolution |                                     | 
		| --------------									 |  
		| --------------								     |  
		| |  FTN mode  |									 |  
		| --------------									 |  
		| --------------								     |  
		| |On line mode|									 |  
		| --------------									 |  
		| --------------								     |  
		| |   Quit     |									 |  
		| --------------									 |  
		|									                 |
		|	---------------------------------------------    |
		|	|								            |    |
		|	|	          	Log   List					|    |
		|   |                                           |    |
		|	|											|	 |
		|	---------------------------------------------    |
		|													 |
		------------------------------------------------------
		*/

		SetBackground();
		SplitLayout<HDC>* pMainLayout = new SplitLayout<HDC>(false);
		SplitLayout<HDC>* pSplitButtonAndPictureLayout = new SplitLayout<HDC>(true);
		pMainLayout->AddWidget(pSplitButtonAndPictureLayout,Top);
		pMainLayout->SetMargin(10,10,10,10);
		pMainLayout->SetFixedValue(460);
		pSplitButtonAndPictureLayout->SetFixedValue(400,false);
		pMainLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_LIST1)),Bottom,false);

		CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
		pListBox->AddString(_T("Rotate,TiltX,TiltY"));

		//Set button list
		TSTRING strGreenBtnBKImage = _T("FunctionBtn\\Btn_DeepGreen_Original.png");
		TSTRING strGreenHighLightImage = _T("FunctionBtn\\Btn_DeepGreen_MouseOver.png");
		TSTRING strGreenClickImage = _T("FunctionBtn\\Btn_DeepGreen_MouseDown.png");
		TSTRING strGreenDisableImage = _T("FunctionBtn\\Btn_DeepGreen_Disable.png");
		TSTRING strCheckBoxOriginal = _T("Btn_Check_Original.png");
		TSTRING strCheckBoxMouseOver = _T("Btn_Check_MouseOver.png");
		TSTRING strCheckBoxMouseDown = _T("Btn_Check_MouseDown.png");
		TSTRING strCheckBoxDisable = _T("Btn_Check_Disable.png");
		TSTRING strCheckBoxOriginal2 = _T("Btn_Check_Original_2.png");
		TSTRING strCheckBoxMouseOver2 = _T("Btn_Check_MouseOver_2.png");
		TSTRING strCheckBoxMouseDown2 = _T("Btn_Check_MouseDown_2.png");
		TSTRING strCheckBoxDisable2 = _T("Btn_Check_Disable_2.png");

		//1.Capture
		m_pCaptureBtn = new WIN32IconButton(this,_T("IDS_CAPTURE"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,CaptureFunc);
		GreenButtonLocalSetting(m_pCaptureBtn);
		m_pAutoCaptureBtn = new WIN32IconButton(
			this
			,_T("")
			,strCheckBoxOriginal
			,strCheckBoxMouseOver
			,strCheckBoxMouseDown
			,strCheckBoxDisable
			,AutoCaptureFunc
			);
		m_pAutoCaptureBtn->SetWidthHeightByImage();
		m_pAutoCaptureBtn->BeToggleButten();
		m_pAutoCaptureBtn->SetFourImagesOfButtonDown(strCheckBoxOriginal2,strCheckBoxMouseOver2,strCheckBoxMouseDown2,strCheckBoxDisable2);
		WIN32LabelWidge* pText_AutoCapture = new WIN32LabelWidge(this,_T("IDS_Auto"),Left|VCenter,L"Arial",9);
		BoxLayout<HDC>* pCaptureBoxLayout = new BoxLayout<HDC>(LeftToRight);
		pCaptureBoxLayout->AddWidget(m_pCaptureBtn);
		pCaptureBoxLayout->AddWidget(m_pAutoCaptureBtn);
		pCaptureBoxLayout->AddWidget(pText_AutoCapture);

		SplitLayout<HDC>* pSplit_Capture =new SplitLayout<HDC>(false);
		pSplit_Capture->SetFixedValue(26);
		pSplit_Capture->AddWidget(pCaptureBoxLayout,Left);

		//2.Sensor0
		m_pBtnSensor0 = new WIN32IconButton(this,_T("IDS_Sensor0"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,Sensor0);
		GreenButtonLocalSetting(m_pBtnSensor0);
		m_pBtnSensor0->BeToggleButten();
		m_pBtnSensor0->SetFourImagesOfButtonDown(strGreenClickImage,strGreenClickImage,strGreenClickImage,strGreenDisableImage,L"Sensor");

		SplitLayout<HDC>* pSplit_Sensor0 =new SplitLayout<HDC>(false);
		pSplit_Capture->AddWidget(pSplit_Sensor0,Left,false);
		pSplit_Sensor0->SetFixedValue(26);
		pSplit_Sensor0->AddWidget(m_pBtnSensor0,Left);

		//3.Sensor1
		m_pBtnSensor1 = new WIN32IconButton(this,_T("IDS_Sensor1"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,Sensor1);
		GreenButtonLocalSetting(m_pBtnSensor1);
		m_pBtnSensor1->BeToggleButten();
		m_pBtnSensor1->SetFourImagesOfButtonDown(strGreenClickImage,strGreenClickImage,strGreenClickImage,strGreenDisableImage,L"Sensor");

		SplitLayout<HDC>* pSplit_Sensor1 =new SplitLayout<HDC>(false);
		pSplit_Sensor0->AddWidget(pSplit_Sensor1,0,false);
		pSplit_Sensor1->SetFixedValue(26);
		pSplit_Sensor1->AddWidget(m_pBtnSensor1,Left);

		//4.Resolution
		m_pBtnResolution = new WIN32IconButton(this,_T("IDS_Resolution"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,Resolution);
		GreenButtonLocalSetting(m_pBtnResolution);
		m_pCurrentResolution = new WIN32LabelWidge(this,_T(""),Left|VCenter,L"Arial",9);

		SplitLayout<HDC>* pResolutionSplit = new SplitLayout<HDC>(true);
		pResolutionSplit->SetFixedValue(100);
		pResolutionSplit->AddWidget(m_pBtnResolution,m_pCurrentResolution,Left,Left);

		SplitLayout<HDC>* pSplit_Resolution =new SplitLayout<HDC>(false);
		pSplit_Sensor1->AddWidget(pSplit_Resolution,0,false);
		pSplit_Resolution->SetFixedValue(26);
		pSplit_Resolution->AddWidget(pResolutionSplit,Left);

		//5.To FTN mode
		m_pBtnFTMMode = new WIN32IconButton(this,_T("IDS_FTNMode"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,FTNMode);
		GreenButtonLocalSetting(m_pBtnFTMMode);
		m_pBtnFTMMode->BeToggleButten();
		m_pBtnFTMMode->SetFourImagesOfButtonDown(strGreenClickImage,strGreenClickImage,strGreenClickImage,strGreenDisableImage,L"MobileMode");

		SplitLayout<HDC>* pSplit_FTNMode =new SplitLayout<HDC>(false);
		pSplit_Resolution->AddWidget(pSplit_FTNMode,0,false);
		pSplit_FTNMode->SetFixedValue(26);
		pSplit_FTNMode->AddWidget(m_pBtnFTMMode,Left);

		//6.For on line mode
		m_pBtnOnLineMode = new WIN32IconButton(this,_T("IDS_OnLineMode"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,OnLineMode);
		GreenButtonLocalSetting(m_pBtnOnLineMode);
		m_pBtnOnLineMode->BeToggleButten();
		m_pBtnOnLineMode->SetFourImagesOfButtonDown(strGreenClickImage,strGreenClickImage,strGreenClickImage,strGreenDisableImage,L"MobileMode");

		SplitLayout<HDC>* pSplit_OnLineMode =new SplitLayout<HDC>(false);
		pSplit_FTNMode->AddWidget(pSplit_OnLineMode,0,false);
		pSplit_OnLineMode->SetFixedValue(26);
		pSplit_OnLineMode->AddWidget(m_pBtnOnLineMode,Left);

		//8.Input Chart Width
		const int nStringLength = 190;
		WIN32LabelWidge* pTextInputChartWidth = new WIN32LabelWidge(this,_T("IDS_InputChartWidth"),Left|VCenter,L"Arial",9);
		SplitLayout<HDC>* pChartWidthLayout = new SplitLayout<HDC>(true);
		m_pEditbox_ChartWidth = new WIN32WindowWidge(GetDlgItem(IDC_ChartWidth));
		m_pEditbox_ChartWidth->SetMaxHeight(20);
		pChartWidthLayout->AddWidget(pTextInputChartWidth,m_pEditbox_ChartWidth,Left|VCenter,Left|VCenter);
		pChartWidthLayout->SetFixedValue(nStringLength);

		SplitLayout<HDC>* pSplit_ChartWidth =new SplitLayout<HDC>(false);
		pSplit_OnLineMode->AddWidget(pSplit_ChartWidth,0,false);
		pSplit_ChartWidth->SetFixedValue(26);
		pSplit_ChartWidth->AddWidget(pChartWidthLayout,Left);

		//9.Input Chart Height
		WIN32LabelWidge* pTextInputChartHeigh = new WIN32LabelWidge(this,_T("IDS_InputChartHeigh"),Left|VCenter,L"Arial",9);
		SplitLayout<HDC>* pChartHeighLayout = new SplitLayout<HDC>(true);
		m_pEditbox_ChartHeigh = new WIN32WindowWidge(GetDlgItem(IDC_ChartHeigh));
		m_pEditbox_ChartHeigh->SetMaxHeight(20);
		pChartHeighLayout->AddWidget(pTextInputChartHeigh,m_pEditbox_ChartHeigh,Left|VCenter,Left|VCenter);
		pChartHeighLayout->SetFixedValue(nStringLength);

		SplitLayout<HDC>* pSplit_ChartHeigh =new SplitLayout<HDC>(false);
		pSplit_ChartWidth->AddWidget(pSplit_ChartHeigh,0,false);
		pSplit_ChartHeigh->SetFixedValue(26);
		pSplit_ChartHeigh->AddWidget(pChartHeighLayout,Left);

		//10.Input Distance
		WIN32LabelWidge* pTextInputChartDistance = new WIN32LabelWidge(this,_T("IDS_InputDistance"),Left|VCenter,L"Arial",9);
		SplitLayout<HDC>* pChartDistanceLayout = new SplitLayout<HDC>(true);
		m_pEditbox_ChartDistance = new WIN32WindowWidge(GetDlgItem(IDC_Distance));
		m_pEditbox_ChartDistance->SetMaxHeight(20);
		pChartDistanceLayout->AddWidget(pTextInputChartDistance,m_pEditbox_ChartDistance,Left|VCenter,Left|VCenter);
		pChartDistanceLayout->SetFixedValue(nStringLength);

		SplitLayout<HDC>* pSplit_Distance =new SplitLayout<HDC>(false);
		pSplit_ChartHeigh->AddWidget(pSplit_Distance,0,false);
		pSplit_Distance->SetFixedValue(26);
		pSplit_Distance->AddWidget(pChartDistanceLayout,Left);

		//7.Quit
		WIN32IconButton* pWinCloseBtn = new WIN32IconButton(this,_T("IDS_QUIT"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,WinClose);
		GreenButtonLocalSetting(pWinCloseBtn);

		SplitLayout<HDC>* pSplit_Quit =new SplitLayout<HDC>(false);
		pSplit_Distance->AddWidget(pSplit_Quit,0,false);
		pSplit_Quit->SetFixedValue(26);
		pSplit_Quit->AddWidget(pWinCloseBtn,Left);

		//Button list end
		pSplitButtonAndPictureLayout->AddWidget(pSplit_Capture,Top|Left);

		//Set Viewer
		m_pViewer = new WIN32PictureViewerWidge(this,m_strCapturedFile,Fixed);
		m_pViewer->SetFixedWidth(300);
		m_pViewer->SetFixedHeight(400);		
		pSplitButtonAndPictureLayout->AddWidget(m_pViewer,Top|Right,false);


		SetLayout(pMainLayout);
	}
	void CImagerAnalysisToolDlg::SetBackground()
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
	bool CImagerAnalysisToolDlg::GreenButtonLocalSetting(WIN32IconButton* pButton)
	{
		if (pButton == NULL)
			return false;
		pButton->SetWidthHeightByImage();
		pButton->SetTextColor(255,255,255,255,255, 15, 85, 105);
		pButton->SetFontSize(9);
		return true;
	}
	SplitLayout<HDC>* CImagerAnalysisToolDlg::CreateFunctionLayoutFromINI()
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
	int CImagerAnalysisToolDlg::GetNumberOfFunctions()
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

	void	CImagerAnalysisToolDlg::WinClose()
	{
		static bool bReentry = false;
		if(bReentry == false)
		{
			bReentry = true;
			g_pThis->FnWinClose();
		}
	}
	void	CImagerAnalysisToolDlg::FnWinClose()
	{		
		CWaitCursor wait;

		//Wait for thread finish
		m_AutoCaptureEvent.ResetEvent();
		m_CloseAppEvent.SetEvent();		
		QSYNC_LIB::MessageLoop(&m_ThreadCloseEvent.m_hObject,1,INFINITE);

		//Save parameter to an ini file.
		CString cstrChart_Width, cstrChart_Heigh, cstrChart_Distance;
		GetDlgItem(IDC_ChartWidth)->GetWindowText(cstrChart_Width);
		GetDlgItem(IDC_ChartHeigh)->GetWindowText(cstrChart_Heigh);
		GetDlgItem(IDC_Distance)->GetWindowText(cstrChart_Distance);
		CCfgStr strCfgChart_Width = CCfgStr(m_pIConfigure,_T("Chart_Width"),_T(""));
		strCfgChart_Width = (TSTRING)cstrChart_Width;
		CCfgStr strCfgChart_Heigh = CCfgStr(m_pIConfigure,_T("Chart_Heigh"),_T(""));
		strCfgChart_Heigh = (TSTRING)cstrChart_Heigh;
		CCfgStr strCfgChart_Distance = CCfgStr(m_pIConfigure,_T("Chart_Distance"),_T(""));
		strCfgChart_Distance = (TSTRING)cstrChart_Distance;
		CCfgInt nCfgSensor = CCfgInt(m_pIConfigure,_T("Sensor"),0);
		nCfgSensor = m_pBtnSensor1->GetButtonCheck() ? 1 : 0;
		CCfgInt nCfgResolution = CCfgInt(m_pIConfigure,_T("Resolution"),0);
		nCfgResolution = m_ResolutionSubmenu.m_nCurrentResolution;
		m_pIConfigure->Save(STORAGE_INIFILE);

		DeleteIConfigure(_T("AnalysisParameter"));		

		PostMessage(WM_CLOSE);		
	}
	void CImagerAnalysisToolDlg::INIBtnFunction(UINT nIndex)
	{
		g_pThis->FnINIBtnFunction(nIndex);
	}
	void	CImagerAnalysisToolDlg::CaptureFunc()
	{
		g_pThis->FnCapture();
	}

	void CImagerAnalysisToolDlg::AutoCaptureFunc()
	{
		if(g_pThis->m_pAutoCaptureBtn->GetButtonCheck())
		{
			g_pThis->m_AutoCaptureEvent.SetEvent();		
		}
		else
		{
			g_pThis->m_AutoCaptureEvent.ResetEvent();
		}
		g_pThis->m_pCaptureBtn->SetEnable(!g_pThis->m_pAutoCaptureBtn->GetButtonCheck());
		g_pThis->m_pBtnFTMMode->SetEnable(!g_pThis->m_pAutoCaptureBtn->GetButtonCheck());
		g_pThis->m_pBtnOnLineMode->SetEnable(!g_pThis->m_pAutoCaptureBtn->GetButtonCheck());			
	}

	UINT CImagerAnalysisToolDlg::TaskWorkerThreadProc(LPVOID pParam)
	{
		g_pThis->m_ThreadCloseEvent.ResetEvent();
		HANDLE handles[] = {g_pThis->m_CloseAppEvent,g_pThis->m_AutoCaptureEvent};		
		do
		{
			if(::WaitForMultipleObjects(2,handles,FALSE,INFINITE) == (WAIT_OBJECT_0 + 1))
				CaptureFunc();
			else
				break;
		}
		while(1);
		g_pThis->m_ThreadCloseEvent.SetEvent();
		return 0;
	}

	void CImagerAnalysisToolDlg::Sensor0()
	{
	}
	void CImagerAnalysisToolDlg::Sensor1()
	{
	}
	void CImagerAnalysisToolDlg::Resolution()
	{
		g_pThis->FnResolution();
	}
	void CImagerAnalysisToolDlg::FnResolution()
	{
		m_QCommonControlBase_Resolution.SetSubMenu(&m_ResolutionSubmenu);
		for(int i =0;i < m_ResolutionSubmenu.GetMenuItemCount();i++)
		{
			TSTRING strDisplay,strNotifyID;
			m_ResolutionSubmenu.GetMenuItem(i,strDisplay,strNotifyID);
			m_QCommonControlBase_Resolution.Register(this,strNotifyID);
		}

		//Set position of sub menu.
		GeometryRect rcResolutionBtn = m_pBtnResolution->GetGeometry();
		RECT rcMainFrame = {0};
		GetClientRect(&rcMainFrame);
		ClientToScreen(&rcMainFrame);
		QPoint ptPoint;
		ptPoint.m_nPosX = rcMainFrame.left + rcResolutionBtn.m_nX;
		ptPoint.m_nPosY = rcMainFrame.top + rcResolutionBtn.m_nY + rcResolutionBtn.m_nHeight;
		m_QCommonControlBase_Resolution.Event(EVENT_SUBMENU,(long)&ptPoint);
	}
	void CImagerAnalysisToolDlg::FTNMode()
	{
		CWaitCursor wait;

		unsigned short nPorts = 10;
		unsigned short Ports[10] = {0};
		if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
			QL_SetTestMode(Ports[0]);
	}
	void CImagerAnalysisToolDlg::OnLineMode()
	{
		CWaitCursor wait;

		unsigned short nPorts = 10;
		unsigned short Ports[10] = {0};
		if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
			QL_SetOnlineMode(Ports[0]);
	}

	void CImagerAnalysisToolDlg::FnINIBtnFunction(UINT nIndex)
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
	void CImagerAnalysisToolDlg::FnCapture()
	{
		//bAuto即代表是否要auto capture, 目前還沒被使用, 將來作auto capture時會用到這個值
		bool bAuto = m_pAutoCaptureBtn->GetButtonCheck();

		CWaitCursor wait;

		unsigned short nPorts = 10;
		unsigned short Ports[10] = {0};
		if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
		{
			int nSensorID = m_pBtnSensor1->GetButtonCheck() ? 1 : 0;
			QPoint CurrentResolution = m_ResolutionSubmenu.GetCurrentResolution();
			if(QL_STL_CapturePicture(nSensorID,CurrentResolution.m_nPosX,CurrentResolution.m_nPosY,GetAnsi(g_pThis->m_strCapturedFile),Ports[0]))
			{
				g_pThis->RotateImageFile(GetUnicode(g_pThis->m_strCapturedFile).c_str(),Rotate270FlipNone);
				g_pThis->m_pViewer->Update();

				FPOINT ptBlocks[5] = {0};
				CCfgInt nCfgThreshold = CCfgInt(m_pIConfigure,_T("Threshold"),25);
				ImageParser(GetAnsi(g_pThis->m_strCapturedFile).c_str(),ptBlocks,5,nCfgThreshold.Get());

				g_pThis->m_pViewer->RemoveTrack();
				for(int i=0;i<5;i++)
					g_pThis->m_pViewer->AddTrack(ptBlocks[i].x,ptBlocks[i].y);

				double dwRotation = 0;
				CalclateRotation(ptBlocks,dwRotation);

				//Get param:
				const double dw_Ideal_X_Center = m_ResolutionSubmenu.GetCurrentResolution().m_nPosX / 2;
				const double dw_Ideal_Y_Center = m_ResolutionSubmenu.GetCurrentResolution().m_nPosY / 2;

				CString cstrChart_Width, cstrChart_Heigh, cstrChart_Distance;
				GetDlgItem(IDC_ChartWidth)->GetWindowText(cstrChart_Width);
				if(cstrChart_Width.IsEmpty())
					return;
				GetDlgItem(IDC_ChartHeigh)->GetWindowText(cstrChart_Heigh);
				if(cstrChart_Heigh.IsEmpty())
					return;
				GetDlgItem(IDC_Distance)->GetWindowText(cstrChart_Distance);
				if(cstrChart_Heigh.IsEmpty())
					return;
			
				float fpWidth, fpHeigh, fpDistance;
				_stscanf( (LPCTSTR)cstrChart_Width, _T("%f"), &fpWidth );
				_stscanf( (LPCTSTR)cstrChart_Heigh, _T("%f"), &fpHeigh );
				_stscanf( (LPCTSTR)cstrChart_Distance, _T("%f"), &fpDistance );

				const double dw_Chart_Width = (double)fpWidth;
				const double dw_Chart_Height = (double)fpHeigh;
				const double dw_Distance = (double)fpDistance;
				double dwTiltX = 0;
				double dwTiltY = 0;
				CalclateTilt(ptBlocks,dw_Ideal_X_Center,dw_Ideal_Y_Center,dw_Chart_Width,dw_Chart_Height,dw_Distance,dwTiltX,dwTiltY);

				CString strReport;
				strReport.Format(_T("%0.02f,%0.02f,%0.02f"),dwRotation,dwTiltX,dwTiltY);

				CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
				pListBox->AddString(strReport);
			}
		}
	}
	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
	{
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes

		ImageCodecInfo* pImageCodecInfo = NULL;

		GetImageEncodersSize(&num, &size);
		if(size == 0)
			return -1;  // Failure

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
		if(pImageCodecInfo == NULL)
			return -1;  // Failure

		GetImageEncoders(num, size, pImageCodecInfo);

		for(UINT j = 0; j < num; ++j)
		{
			if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )
			{
				*pClsid = pImageCodecInfo[j].Clsid;
				free(pImageCodecInfo);
				return j;  // Success
			}    
		}

		free(pImageCodecInfo);
		return -1;  // Failure
	}

	void CImagerAnalysisToolDlg::RotateImageFile(const WCHAR *filename,RotateFlipType rotateFlipType)
	{
		Image image(filename);
		image.RotateFlip(rotateFlipType);
		CLSID pngClsid;
		GetEncoderClsid(L"image/jpeg", &pngClsid);
		image.Save(filename, &pngClsid, NULL);
	}
}