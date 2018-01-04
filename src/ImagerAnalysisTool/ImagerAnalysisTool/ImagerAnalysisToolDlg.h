// CDAutoRunDlg.h : header file
//
#include "..\..\..\code\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/interface/QCommonControlBase.h"
#include "..\..\..\code\Components\ConfigureTool\IConfigure.h"
#include "LanguageSubmenu.h"

#pragma once
namespace QSYNC_LIB
{
	// CImagerAnalysisToolDlg dialog
	class CImagerAnalysisToolDlg : public Win32DoubleBufferQDialogWidge, public INotify
	{
		// Construction
	public:
		CImagerAnalysisToolDlg(CWnd* pParent = NULL);	// standard constructor

		// Dialog Data
		enum { IDD = IDD_ImagerAnalysisTool_DIALOG };

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
		bool GreenButtonLocalSetting(WIN32IconButton* pButton);
		void SetBackground();
		SplitLayout<HDC>* CreateFunctionLayoutFromINI();
		int GetNumberOfFunctions();

		void RotateImageFile(const WCHAR *filename,RotateFlipType rotateFlipType);

		//Button functions:
		static void	WinClose();
		void	FnWinClose();
		static void	INIBtnFunction(UINT);
		void	FnINIBtnFunction(UINT);
		static void CaptureFunc();
		void	FnCapture();
		static void AutoCaptureFunc();
		static void Sensor0();
		static void Sensor1();
		static void Resolution();
		void FnResolution();
		static void FTNMode();
		static void OnLineMode();
		static UINT TaskWorkerThreadProc(LPVOID pParam);

	private:
		ILocalization*		m_pILocalization;
		QCommonControlBase    m_QCommonControlBase_Resolution;
		ResolutionSubmenu m_ResolutionSubmenu;
		WIN32PictureViewerWidge* m_pViewer;
		CString m_strFunction_INIFile_Path;

		//Configure
		IConfigure*	m_pIConfigure;

		//Button:
		WIN32IconButton* m_pCaptureBtn;
		WIN32IconButton* m_pAutoCaptureBtn;
		WIN32IconButton* m_pBtnSensor0;
		WIN32IconButton* m_pBtnSensor1;
		WIN32IconButton* m_pBtnResolution;
		WIN32IconButton* m_pBtnOnLineMode;
		WIN32IconButton* m_pBtnFTMMode;

		//Label:
		WIN32LabelWidge* m_pCurrentResolution;

		//WinCtrl:
		WIN32WindowWidge* m_pEditbox_ChartWidth;
		WIN32WindowWidge* m_pEditbox_ChartHeigh;
		WIN32WindowWidge* m_pEditbox_ChartDistance;

		std::vector<WIN32IconButton*>	m_vecFunctionBtn;
		TSTRING										m_strCapturedFile;

		CEvent	m_AutoCaptureEvent;
		CEvent	m_CloseAppEvent;
		CEvent	m_ThreadCloseEvent;
	};
}