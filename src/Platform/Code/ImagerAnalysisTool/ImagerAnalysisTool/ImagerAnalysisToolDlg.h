// CDAutoRunDlg.h : header file
//
#include "..\..\..\code\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\..\code\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "../../../code\Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../../code\Components/UIFramework/interface/QCommonControlBase.h"
#include "..\..\..\code\Components\ConfigureTool\IConfigure.h"
#include "LanguageSubmenu.h"
#include "../../../code\Components\UIFramework\Porting\QUIDropTraget.h"
#include "../../../code\Components\UIFramework\Porting\UIOleDataSource.h"

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
		bool GetInputNumber();

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
		static void	SaveImageFunc();
		static void ClearLog();
		void FnClearLog();

		void SetViewSize();
		bool AnalysisPhoto(const TSTRING& strFileFullPath, double& dwRotation, double& dwTiltX, double& dwTiltY);
		void WriteFile(const CString& strFileFullPath, const CString& strContent);
		void SaveResult(int nCurrentImageNumber,int nSensorID,double dwRotation,double dwTiltX,double dwTiltY);
		void SaveResult(const CString& strFileFullPath,double dwRotation,double dwTiltX,double dwTiltY);
		TSTRING GetMobileID(unsigned short* pPortList);
		void AttendToCertainPhoto(const TSTRING& strFileFullPath);

	private:
		ILocalization*		m_pILocalization;
		QCommonControlBase    m_QCommonControlBase_Resolution;
		ResolutionSubmenu m_ResolutionSubmenu;
		WIN32PictureViewerWidge* m_pViewer;
		CString m_strFunction_INIFile_Path;
		CUIDropTarget       m_DropTarget;

		//Button:
		WIN32IconButton* m_pCaptureBtn;
		WIN32IconButton* m_pAutoCaptureBtn;
		WIN32IconButton* m_pSaveImageBtn;
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
		TSTRING		m_strCapturedFilePath;

		TSTRING		m_strCapturedFileName;
		TSTRING		m_strCurrentCapturedFileFullPath;
		TSTRING		m_strCurrentMobileID;

		CString m_cstrCSVFileFullPath;
		bool  m_bFirstTimeToSaveResult;

		CEvent	m_AutoCaptureEvent;
		CEvent	m_CloseAppEvent;
		CEvent	m_ThreadCloseEvent;
		
		//Configure
		IConfigure*	m_pIConfigure;
		CCfgInt   m_nCfgResolution;
		CCfgStr m_strCfgChart_Width;
		CCfgStr m_strCfgChart_Heigh;
		CCfgStr m_strCfgChart_Distance;
		CCfgInt m_nCfgSensor;
		CCfgInt m_nCfgThreshold;

		CCfgStr m_CfgRotationUpperBound;
		CCfgStr m_CfgRotationLowerBound;
		CCfgStr m_CfgTiltXUpperBound;
		CCfgStr m_CfgTiltXLowerBound;
		CCfgStr m_CfgTiltYUpperBound;
		CCfgStr m_CfgTiltYLowerBound;

		float m_dwRotationUpperBound;
		float m_dwRotationLowerBound;
		float m_dwTiltXUpperBound;
		float m_dwTiltXLowerBound;
		float m_dwTiltYUpperBound;
		float m_dwTiltYLowerBound;

		float m_fpWidth, m_fpHeight, m_fpDistance;
	};
}