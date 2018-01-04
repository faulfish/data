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
#include <fstream>
#include "../../../Code\Modules\QisdaNVModule\NVItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4996)

// CImagerAnalysisToolDlg dialog


namespace QSYNC_LIB
{
	CImagerAnalysisToolDlg* g_pThis = NULL;

	CImagerAnalysisToolDlg::CImagerAnalysisToolDlg(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(CImagerAnalysisToolDlg::IDD, pParent)
		,m_pIConfigure(CreateIConfigure(_T("AnalysisParameter")))
		,m_nCfgResolution(m_pIConfigure,_T("Resolution"),0)
		,m_strCfgChart_Width(m_pIConfigure,_T("Chart_Width"),_T(""))
		,m_strCfgChart_Heigh(m_pIConfigure,_T("Chart_Heigh"),_T(""))
		,m_strCfgChart_Distance(m_pIConfigure,_T("Chart_Distance"),_T(""))
		,m_nCfgSensor(m_pIConfigure,_T("Sensor"),0)
		,m_nCfgThreshold(m_pIConfigure,_T("Threshold"),25)
		,m_CfgRotationUpperBound(m_pIConfigure,_T("RotationUpperBound"), _T("3.0")),m_dwRotationUpperBound(0)
		,m_CfgRotationLowerBound(m_pIConfigure,_T("RotationLowerBound"), _T("-3.0")),m_dwRotationLowerBound(0)
		,m_CfgTiltXUpperBound(m_pIConfigure,_T("TiltXUpperBound"), _T("4.5")),m_dwTiltXUpperBound(0)
		,m_CfgTiltXLowerBound(m_pIConfigure,_T("TiltXLowerBound"), _T("-4.5")),m_dwTiltXLowerBound(0)
		,m_CfgTiltYUpperBound(m_pIConfigure,_T("TiltYUpperBound"), _T("4.5")),m_dwTiltYUpperBound(0)
		,m_CfgTiltYLowerBound(m_pIConfigure,_T("TiltYLowerBound"), _T("-4.5")),m_dwTiltYLowerBound(0)
		,m_AutoCaptureEvent(FALSE,TRUE)
		,m_CloseAppEvent(FALSE,TRUE)
		,m_ThreadCloseEvent(FALSE,TRUE)
		,m_bFirstTimeToSaveResult(true)
		,m_fpWidth(0)
		,m_fpHeight(0)
		,m_fpDistance(0)

	{
		if(m_pIConfigure)
			m_pIConfigure->Load(STORAGE_INIFILE);
		_stscanf( m_CfgRotationUpperBound.Get().c_str(), _T("%f"), &m_dwRotationUpperBound ); // C4996
		_stscanf( m_CfgRotationLowerBound.Get().c_str(), _T("%f"), &m_dwRotationLowerBound ); // C4996
		_stscanf( m_CfgTiltXUpperBound.Get().c_str(), _T("%f"), &m_dwTiltXUpperBound ); // C4996
		_stscanf( m_CfgTiltXLowerBound.Get().c_str(), _T("%f"), &m_dwTiltXLowerBound ); // C4996
		_stscanf( m_CfgTiltYUpperBound.Get().c_str(), _T("%f"), &m_dwTiltYUpperBound ); // C4996
		_stscanf( m_CfgTiltYLowerBound.Get().c_str(), _T("%f"), &m_dwTiltYLowerBound ); // C4996

		m_AutoCaptureEvent.ResetEvent();
		m_ThreadCloseEvent.SetEvent();
		m_CloseAppEvent.ResetEvent();
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		CreateILocalization(&m_pILocalization);
		g_pThis = this;

#ifdef _DEBUG
		m_strFunction_INIFile_Path = _T("..\\Debug\\BtnFunction\\BtnFunction.ini");
#else
		m_strFunction_INIFile_Path = _T("..\\BtnFunction\\BtnFunction.ini");
#endif	

		TCHAR szPath[MAX_PATH] = {0};
		bool bRes = Win32Utility::GetShellFolder(szPath,CSIDL_PERSONAL,_T("ImagerAnalysisTool"));
		m_strCapturedFilePath = szPath;
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

		m_DropTarget.Register(this);
		m_DropTarget.SetDropable(true);

		m_DropTarget.Register(this, EVENT_DROP);
		m_DropTarget.Register(this, EVENT_DRAG_ENTER);
		m_DropTarget.Register(this, EVENT_DRAG_OVER);
		m_DropTarget.Register(this, EVENT_DRAG_LEAVE);

		SetUI();
		CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
		CString strInitialString_CHT = m_pILocalization->GetString(_T("IDS_InitialString_CHT")).c_str();
		CString strInitialString_ENG = m_pILocalization->GetString(_T("IDS_InitialString_ENG")).c_str();
		pListBox->InsertString(0,strInitialString_CHT);
		pListBox->InsertString(0,strInitialString_ENG);

		//Initial parameter
		GetDlgItem(IDC_ChartWidth)->SetWindowText(m_strCfgChart_Width.Get().c_str());
		GetDlgItem(IDC_ChartHeigh)->SetWindowText(m_strCfgChart_Heigh.Get().c_str());
		GetDlgItem(IDC_Distance)->SetWindowText(m_strCfgChart_Distance.Get().c_str());
		m_nCfgSensor == 0 ? m_pBtnSensor0->SetButtonCheck(true) : m_pBtnSensor1->SetButtonCheck(true);
		m_ResolutionSubmenu.m_nCurrentResolution = m_nCfgResolution;
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

	void CImagerAnalysisToolDlg::SetViewSize()
	{
		float nMax = 400;

		Image image(GetUnicode(m_strCurrentCapturedFileFullPath).c_str());
		float nX = (float)image.GetWidth();
		float nY = (float)image.GetHeight();

		if(nX > nMax || nY > nMax)
		{
			nX = (float)m_ResolutionSubmenu.GetCurrentResolution().m_nPosX;
			nY = (float)m_ResolutionSubmenu.GetCurrentResolution().m_nPosY;
			float nRatio = nX / nY;

			if(nRatio >= 1)
			{
				nX = nMax;
				nY = nMax / nRatio;
			}
			else
			{
				nX = nMax * nRatio;
				nY= nMax;
			}
		}

		m_pViewer->SetFixedWidth((int)nX);
		m_pViewer->SetFixedHeight((int)nY);
	}

	void CImagerAnalysisToolDlg::Event(const TSTRING& strEvent,long nParam)
	{

		if (strEvent == EVENT_DROP)
		{
			CDragDropFireParam* pdropFireParam = (CDragDropFireParam*)nParam;
			for(int i = 0; i< pdropFireParam->GetFileCount(); i++)
				AttendToCertainPhoto(pdropFireParam->GetByIndex(i));
		}
		else if(strEvent == EVENT_DRAG_ENTER)
		{
		}
		else if(strEvent == EVENT_DRAG_OVER)
		{
		}
		else if(strEvent == EVENT_DRAG_LEAVE)
		{
		}
		else
		{
			CString cstrEvent = strEvent.c_str();
			if (cstrEvent.Left(11) == _T("Resolution_"))
			{
				m_ResolutionSubmenu.m_nCurrentResolution = _tstoi(cstrEvent.Mid(11));
				m_nCfgResolution = m_ResolutionSubmenu.m_nCurrentResolution;
				m_pCurrentResolution->SetDisplayString((TSTRING)(m_ResolutionSubmenu.GetCurrentResolutionByString()));			
				m_pCurrentResolution->Update();
			}
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
		pMainLayout->SetFixedValue(490);
		pSplitButtonAndPictureLayout->SetFixedValue(400,false);
		pMainLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_LIST1)),Bottom,false);

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
		m_pSaveImageBtn = new WIN32IconButton(
			this
			,_T("")
			,strCheckBoxOriginal
			,strCheckBoxMouseOver
			,strCheckBoxMouseDown
			,strCheckBoxDisable
			,SaveImageFunc
			);
		pText_AutoCapture->SetFixedWidth(30);
		m_pSaveImageBtn->SetWidthHeightByImage();
		m_pSaveImageBtn->BeToggleButten();
		m_pSaveImageBtn->SetFourImagesOfButtonDown(strCheckBoxOriginal2,strCheckBoxMouseOver2,strCheckBoxMouseDown2,strCheckBoxDisable2);
		WIN32LabelWidge* pText_SaveImage = new WIN32LabelWidge(this,_T("IDS_Save"),Left|VCenter,L"Arial",9);

		BoxLayout<HDC>* pCaptureBoxLayout = new BoxLayout<HDC>(LeftToRight);
		pCaptureBoxLayout->AddWidget(m_pCaptureBtn);
		pCaptureBoxLayout->AddWidget(m_pAutoCaptureBtn);
		pCaptureBoxLayout->AddWidget(pText_AutoCapture);
		pCaptureBoxLayout->AddWidget(m_pSaveImageBtn);
		pCaptureBoxLayout->AddWidget(pText_SaveImage);

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

		//11.Clear log
		WIN32IconButton* pClearLogBtn = new WIN32IconButton(this,_T("IDS_ClearLog"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,ClearLog);
		GreenButtonLocalSetting(pClearLogBtn);
		pClearLogBtn->BeToggleButten();
		pClearLogBtn->SetFourImagesOfButtonDown(strGreenClickImage,strGreenClickImage,strGreenClickImage,strGreenDisableImage,L"MobileMode");

		SplitLayout<HDC>* pSplit_ClearLog =new SplitLayout<HDC>(false);
		pSplit_Distance->AddWidget(pSplit_ClearLog,0,false);
		pSplit_ClearLog->SetFixedValue(26);
		pSplit_ClearLog->AddWidget(pClearLogBtn,Left);

		//7.Quit
		WIN32IconButton* pWinCloseBtn = new WIN32IconButton(this,_T("IDS_QUIT"),strGreenBtnBKImage,strGreenHighLightImage,strGreenClickImage,strGreenDisableImage,WinClose);
		GreenButtonLocalSetting(pWinCloseBtn);

		SplitLayout<HDC>* pSplit_Quit =new SplitLayout<HDC>(false);
		pSplit_ClearLog->AddWidget(pSplit_Quit,0,false);
		pSplit_Quit->SetFixedValue(26);
		pSplit_Quit->AddWidget(pWinCloseBtn,Left);

		//Button list end
		pSplitButtonAndPictureLayout->AddWidget(pSplit_Capture,Top|Left);

		//Set Viewer
		m_pViewer = new WIN32PictureViewerWidge(this,m_strCurrentCapturedFileFullPath,Stretch);

		SetViewSize();

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

		UpdateData(TRUE);
		//Save parameter to an ini file.
		CString cstrChart_Width, cstrChart_Heigh, cstrChart_Distance;
		GetDlgItem(IDC_ChartWidth)->GetWindowText(cstrChart_Width);
		GetDlgItem(IDC_ChartHeigh)->GetWindowText(cstrChart_Heigh);
		GetDlgItem(IDC_Distance)->GetWindowText(cstrChart_Distance);
		m_strCfgChart_Width = (TSTRING)cstrChart_Width;
		m_strCfgChart_Heigh = (TSTRING)cstrChart_Heigh;
		m_strCfgChart_Distance = (TSTRING)cstrChart_Distance;
		m_nCfgSensor = m_pBtnSensor1->GetButtonCheck() ? 1 : 0;

		if(m_pIConfigure)
			m_pIConfigure->Save(STORAGE_INIFILE);

		DeleteIConfigure(_T("AnalysisParameter"));		
		m_DropTarget.Unregister(this, EVENT_DROP);
		m_DropTarget.Unregister(this, EVENT_DRAG_ENTER);
		m_DropTarget.Unregister(this, EVENT_DRAG_OVER);
		m_DropTarget.Unregister(this, EVENT_DRAG_LEAVE);

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
	void	CImagerAnalysisToolDlg::SaveImageFunc()
	{

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

	void CImagerAnalysisToolDlg::ClearLog()
	{
		g_pThis->FnClearLog();
	}

	void CImagerAnalysisToolDlg::FnClearLog()
	{
		CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
		pListBox->ResetContent();
	}

	bool CImagerAnalysisToolDlg::GetInputNumber()
	{
		CString cstrChart_Width, cstrChart_Heigh, cstrChart_Distance;
		GetDlgItem(IDC_ChartWidth)->GetWindowText(cstrChart_Width);
		if(cstrChart_Width.IsEmpty())
			return false;
		GetDlgItem(IDC_ChartHeigh)->GetWindowText(cstrChart_Heigh);
		if(cstrChart_Heigh.IsEmpty())
			return false;
		GetDlgItem(IDC_Distance)->GetWindowText(cstrChart_Distance);
		if(cstrChart_Heigh.IsEmpty())
			return false;

		_stscanf( (LPCTSTR)cstrChart_Width, _T("%f"), &m_fpWidth );
		_stscanf( (LPCTSTR)cstrChart_Heigh, _T("%f"), &m_fpHeight );
		_stscanf( (LPCTSTR)cstrChart_Distance, _T("%f"), &m_fpDistance );
		return true;
	}
	void CImagerAnalysisToolDlg::FnCapture()
	{
		CWaitCursor wait;

		CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
		if(GetInputNumber() == false)
		{
			pListBox->InsertString(0,_T("Please input the chart width, chart height,and distance from target to lens."));
			return;
		}

		unsigned short nPorts = 10;
		unsigned short Ports[10] = {0};
		if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
		{
			int nSensorID = m_pBtnSensor1->GetButtonCheck() ? 1 : 0;
			QPoint CurrentResolution = m_ResolutionSubmenu.GetCurrentResolution();

			int nCurrentImageNumber = 0;
			int nTest = 0;
			m_strCurrentMobileID = GetMobileID(Ports);
			if(m_strCurrentMobileID.empty())
			{
				CString strReport = _T("Get Mobile ID fail!");
				pListBox->InsertString(0,strReport);
				return;
			}
			if(m_pSaveImageBtn->GetButtonCheck())
				while(nTest == 0)
				{
					nCurrentImageNumber++;

					CString cstrFileName;
					cstrFileName.Format(_T("%s_%sSensor_[%d %d]_%05d.png"),
						m_strCurrentMobileID.c_str(),nSensorID == 0 ? _T("Main") : _T("Sub"),CurrentResolution.m_nPosX,CurrentResolution.m_nPosY,nCurrentImageNumber);

					m_strCurrentCapturedFileFullPath = m_strCapturedFilePath + (TSTRING)cstrFileName;
					nTest = _taccess(m_strCurrentCapturedFileFullPath.c_str(),0) ;
				}
			else
			{
				nCurrentImageNumber = -1;
				m_strCurrentCapturedFileFullPath = m_strCapturedFilePath + _T("TestImage.png");
			}


			if(QL_STL_CapturePicture(nSensorID,CurrentResolution.m_nPosX,CurrentResolution.m_nPosY,GetAnsi(g_pThis->m_strCurrentCapturedFileFullPath),Ports[0]))			
			{
				g_pThis->RotateImageFile(GetUnicode(g_pThis->m_strCurrentCapturedFileFullPath).c_str(),Rotate270FlipNone);
				
				SetViewSize();
				SetLayout();

				double dwRotation(0), dwTiltX(0), dwTiltY(0);
				bool bAnalysisSucceed = AnalysisPhoto(m_strCurrentCapturedFileFullPath, dwRotation, dwTiltX, dwTiltY);

				bool bRotation = (dwRotation <= m_dwRotationUpperBound && dwRotation >= m_dwRotationLowerBound);
				bool bTiltX = (dwTiltX <= m_dwTiltXUpperBound && dwTiltX >= m_dwTiltXLowerBound);
				bool bTiltY = (dwTiltY <= m_dwTiltYUpperBound && dwTiltY >= m_dwTiltYLowerBound);
				CString strReport;
				strReport.Format(_T("NO. %d, Rotation %0.02f, TiltX %0.02f,TiltY %0.02f,  "),nCurrentImageNumber,dwRotation,dwTiltX,dwTiltY);
				if(bAnalysisSucceed == false)
					strReport += _T("Analysis fail !");
				else if(bRotation && bTiltX && bTiltY)
					strReport += _T("Pass");
				else
				{
					if(bRotation == false)
						strReport += _T("Rotation ");
					if(bTiltX == false)
						strReport += _T("TiltX ");
					if(bTiltY == false)
						strReport += _T("TiltY ");
					strReport += _T("fail !!");
				}
				pListBox->InsertString(0,strReport);

				if(m_pSaveImageBtn->GetButtonCheck())
					SaveResult(nCurrentImageNumber, nSensorID, dwRotation, dwTiltX, dwTiltY);
			}
			else
			{//Capture fail
				AfxMessageBox(_T("Capture fail, please check it."));
			}
		}
	}

	void CImagerAnalysisToolDlg::AttendToCertainPhoto(const TSTRING& strFileFullPath)
	{
		CWaitCursor wait;
		CListBox* pListBox = (CListBox*)GetDlgItem(IDC_LIST1);
		if(GetInputNumber() == false)
		{
			pListBox->InsertString(0,_T("Please input the chart width, chart height,and distance from target to lens."));
			return;
		}

		CString cstrFileFullPath = strFileFullPath.c_str();
		CString strFileName = PathFindFileName(cstrFileFullPath);

		m_strCurrentCapturedFileFullPath = strFileFullPath;
		SetViewSize();
		SetLayout();

		m_strCurrentCapturedFileFullPath = strFileFullPath;
		double dwRotation(0), dwTiltX(0), dwTiltY(0);
		bool bAnalysisSucceed = AnalysisPhoto(strFileFullPath, dwRotation, dwTiltX, dwTiltY);

		bool bRotation = (dwRotation <= m_dwRotationUpperBound && dwRotation >= m_dwRotationLowerBound);
		bool bTiltX = (dwTiltX <= m_dwTiltXUpperBound && dwTiltX >= m_dwTiltXLowerBound);
		bool bTiltY = (dwTiltY <= m_dwTiltYUpperBound && dwTiltY >= m_dwTiltYLowerBound);
		CString strReport;
		strReport.Format(_T("FileName %s, Rotation %0.02f, TiltX %0.02f,TiltY %0.02f,  "),strFileName,dwRotation,dwTiltX,dwTiltY);
		if(bAnalysisSucceed == false)
			strReport += _T("Analysis fail !");
		else if(bRotation && bTiltX && bTiltY)
			strReport += _T("Pass");
		else
		{
			if(bRotation == false)
				strReport += _T("Rotation ");
			if(bTiltX == false)
				strReport += _T("TiltX ");
			if(bTiltY == false)
				strReport += _T("TiltY ");
			strReport += _T("fail !!");
		}
		pListBox->InsertString(0,strReport);

		if(m_pSaveImageBtn->GetButtonCheck())
			SaveResult(cstrFileFullPath, dwRotation, dwTiltX, dwTiltY);
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

		//注意, 手機版本0.26給的圖檔有唯讀屬性, 以jpeg格式儲存的話會有問題
		//所以要轉成png格式
		CLSID pngClsid;
		GetEncoderClsid(L"image/png", &pngClsid);
		Status status = image.Save(filename, &pngClsid, NULL);
	}

	bool CImagerAnalysisToolDlg::AnalysisPhoto(const TSTRING& strFileFullPath, double& dwRotation, double& dwTiltX, double& dwTiltY)
	{
		g_pThis->m_pViewer->SetImagePath(strFileFullPath);
		g_pThis->m_pViewer->Update();

		FPOINT ptBlocks[5] = {0};
		if(m_pIConfigure)
			m_pIConfigure->Load(STORAGE_INIFILE);
		int nThreshold = m_nCfgThreshold.Get();
		ImageParser(GetAnsi(strFileFullPath).c_str(),ptBlocks,5,nThreshold);

		g_pThis->m_pViewer->RemoveTrack();

		Image image(GetUnicode(strFileFullPath).c_str());
		float nImageW = (float)image.GetWidth();
		float nImageH = (float)image.GetHeight();

		float nViewerW = (float)g_pThis->m_pViewer->GetMaxWidth();
		float nViewerH = (float)g_pThis->m_pViewer->GetMaxHeight();

		float nRationW = nViewerW / nImageW;
		float nRationH = nViewerH / nImageH;

		for(int i=0;i<5;i++)
			g_pThis->m_pViewer->AddTrack(ptBlocks[i].x * nRationW,ptBlocks[i].y * nRationH);

		bool bCalclateRotation = CalclateRotation(ptBlocks,dwRotation);
		if (bCalclateRotation == false)
		{
			ArrangePointAgain(ptBlocks);
			bCalclateRotation = CalclateRotation(ptBlocks,dwRotation);
		}
		if(bCalclateRotation == false)
			return false;

		//Get param:
		const double dw_Ideal_X_Center = nImageW / 2;
		const double dw_Ideal_Y_Center = nImageH / 2;

		const double dw_Chart_Width = (double)m_fpWidth;
		const double dw_Chart_Height = (double)m_fpHeight;
		const double dw_Distance = (double)m_fpDistance;
		bool bCalclateTilt = CalclateTilt(ptBlocks,dw_Ideal_X_Center,dw_Ideal_Y_Center,dw_Chart_Width,dw_Chart_Height,dw_Distance,dwTiltX,dwTiltY);
		if(bCalclateTilt == false)
			return false;
		return true;
	}

	void CImagerAnalysisToolDlg::WriteFile(const CString& strFileFullPath, const CString& strContent)
	{
		wofstream outfile ((LPCTSTR)strFileFullPath, wofstream::app);
		long nStringSize = strContent.GetLength();
		outfile.write ((LPTSTR)(LPCTSTR)strContent,nStringSize);
		outfile.close();
	}

	void CImagerAnalysisToolDlg::SaveResult(int nCurrentImageNumber,int nSensorID,double dwRotation,double dwTiltX,double dwTiltY)
	{
		if(m_bFirstTimeToSaveResult)
		{
			int nTest(0), nCSVFileNumber(0);
			CString cstrFileName ;
			while(nTest == 0)
			{
				++nCSVFileNumber;

				cstrFileName.Format(_T("ImagerTest%03d.csv"),nCSVFileNumber);
				m_cstrCSVFileFullPath = m_strCapturedFilePath.c_str() + cstrFileName;
				nTest = _taccess(m_cstrCSVFileFullPath,0) ;
			}
			CString strInitialText = _T("Item,Sensor type,Rotation,Tilt X,Tilt Y,Mobile ID,Chart_Width,Chart_Height,Distance from target to lens,Threshold\n");
			WriteFile(m_cstrCSVFileFullPath, strInitialText);

			m_bFirstTimeToSaveResult = false;
		}

		CString strAnalysisResult;
		strAnalysisResult.Format(_T("%d,%s,%0.02f,%0.02f,%0.02f,")
			, nCurrentImageNumber, nSensorID==0 ? _T("MainSensor") : _T("SubSensor"), dwRotation, dwTiltX, dwTiltY);
		strAnalysisResult += m_strCurrentMobileID.c_str();
		CString strTemp;
		strTemp.Format(_T(",%0.02f,%0.02f,%0.02f,%d"),m_fpWidth, m_fpHeight, m_fpDistance,m_nCfgThreshold.Get());
		strAnalysisResult += strTemp;
		strAnalysisResult += _T("\n");
		WriteFile(m_cstrCSVFileFullPath, strAnalysisResult);
	}
	void CImagerAnalysisToolDlg::SaveResult(const CString& strFileFullPath,double dwRotation,double dwTiltX,double dwTiltY)
	{
		if(m_bFirstTimeToSaveResult)
		{
			int nTest(0), nCSVFileNumber(0);
			CString cstrFileName ;
			while(nTest == 0)
			{
				++nCSVFileNumber;

				cstrFileName.Format(_T("ImagerTest%03d.csv"),nCSVFileNumber);
				m_cstrCSVFileFullPath = m_strCapturedFilePath.c_str() + cstrFileName;
				nTest = _taccess(m_cstrCSVFileFullPath,0) ;
			}
			CString strInitialText = _T("Item,Sensor type,Rotation,Tilt X,Tilt Y,Mobile ID,Chart_Width,Chart_Height,Distance from target to lens,Threshold\n");
			WriteFile(m_cstrCSVFileFullPath, strInitialText);

			m_bFirstTimeToSaveResult = false;
		}

		CString strImagerName = PathFindFileName(strFileFullPath);
		CString strAnalysisResult;
		strAnalysisResult.Format(_T("%s,,%0.02f,%0.02f,%0.02f,,%0.02f,%0.02f,%0.02f,%d"), strImagerName, dwRotation, dwTiltX, dwTiltY,m_fpWidth, m_fpHeight, m_fpDistance,m_nCfgThreshold.Get());
		strAnalysisResult += _T("\n");
		WriteFile(m_cstrCSVFileFullPath, strAnalysisResult);
	}

	TSTRING CImagerAnalysisToolDlg::GetMobileID(unsigned short* pPortList)
	{
		TSTRING strMobileID;

		if(pPortList)
		{
			CNVItem NVItem(pPortList[0]);

			//Get factory data
			DIAG_NVITEM_PACKET_F_type ReadResp = {0};
			if(NVItem.ReadNVItem(NV_FACTORY_DATA_1_I,ReadResp))
			{
				//Get Mobile ID    
				/*
				45  11 used used used used used mobile ID line number
				46   used used used used used  
				47   used used(0x00) used(0x00) used(0x00) used(0x00)  
				48   used used(0x00) used(0x00) used(0x00) used(0x00)  adapter
				49   used used used used used  DUT number
				50   used used used used used  day
				51   used used used used used  month
				52   used used used used used  year
				53   used used used used used  hour
				54   used used used used used  minute
				55   used used used used used  seconds
				*/    
				int nLine1 = ReadResp.item_data[45];
				int nLine2 = ReadResp.item_data[46];
				int nLine3 = ReadResp.item_data[47];
				int nLine4 = ReadResp.item_data[48];
				int nDUT = ReadResp.item_data[49];
				int nDate = ReadResp.item_data[50];
				int nMonth = ReadResp.item_data[51];
				int nYear = ReadResp.item_data[52];
				int nHour = ReadResp.item_data[53];
				int nMinute = ReadResp.item_data[54];
				int nSeconds = ReadResp.item_data[55];

				TCHAR szMibileID[512] = {0};
				_stprintf(szMibileID,_T("%01d%01d%01d%01d%02d%02d%02d%02d%02d%02d%02d"),nLine1,nLine2,nLine3,nLine4,nDUT,nYear,nMonth,nDate,nHour,nMinute,nSeconds);
				strMobileID = szMibileID;
			}
		}
		return strMobileID;
	}
}

#pragma warning(pop)