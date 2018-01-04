// SyncOutlookDlg.cpp : implementation file
//


#include "stdafx.h"
#include "SyncOutlookDlg.h"


// CSyncOutlookDlg dialog
namespace QSYNC_LIB
{
	IMPLEMENT_DYNAMIC(CSyncOutlookDlg, Win32DoubleBufferQDialogWidge)

		CSyncOutlookDlg::CSyncOutlookDlg(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(CSyncOutlookDlg::IDD, pParent)
		,m_pPeriodStartAfter(NULL)
		,m_pPeriodStartBefore(NULL)
		,m_bShowAll(false)
		,m_pWinCloseButton(NULL)
		,m_pBtnCancel(NULL)
		,m_pBtnOK(NULL)
		,m_pBtnRadioAll(NULL)
		,m_pBtnRadioStartBetween(NULL)
		,m_SyncWay(SyncFromOL)
		,m_SubcaptionText(NULL)
		,m_ItemNumberText(NULL)
	{	

	}

	CSyncOutlookDlg::~CSyncOutlookDlg()
	{

	}

	void CSyncOutlookDlg::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
	}
	INT_PTR CSyncOutlookDlg::DoModal(SyncWay nSyncWay, bool bShowAll,CTime* pPeriodStart,CTime* pPeriodEnd)
	{
		m_pPeriodStartAfter = pPeriodStart;
		m_pPeriodStartBefore = pPeriodEnd;
		m_bShowAll = bShowAll;
		m_SyncWay = nSyncWay;
		INT_PTR nRet = CQDialog::DoModal();
		return nRet;
	}
	BOOL CSyncOutlookDlg::OnInitDialog()
	{
		BOOL nRet = CQDialog::OnInitDialog();
		SetXMLUI();

		//設定兩個Ctrl的顏色
		((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTAFTER))->SetMonthCalColor(MCSC_TITLEBK,RGB(20,80,100));
		((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTBEFORE))->SetMonthCalColor(MCSC_TITLEBK,RGB(20,80,100));

		//設定子視窗文字
		if(m_SyncWay == SyncFromOL)
			SetCaptionText(_T("IDS_Cal_SyncFromOL"));
		else;//初始值已是IDS_Cal_SyncToOL,故對SyncToOL的情況不做設定

		//設定按鈕初始狀態
		if(m_bShowAll)
		{
			m_pBtnRadioAll->SetButtonCheck(true);
			GetDlgItem(IDC_DATETIMEPICKER_STARTAFTER)->EnableWindow(false);
			GetDlgItem(IDC_DATETIMEPICKER_STARTBEFORE)->EnableWindow(false);
		}
		else
			m_pBtnRadioStartBetween->SetButtonCheck(true);
		if (nRet && m_pPeriodStartAfter)
			nRet = ((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTAFTER))->SetTime(m_pPeriodStartAfter);
		if (nRet && m_pPeriodStartBefore)
			nRet = ((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTBEFORE))->SetTime(m_pPeriodStartBefore);

		return nRet;
	}

	void CSyncOutlookDlg::SetXMLUI()
	{
		//Set Background
		std::wstring strBkImage = L"Calendar\\Background\\BK_SyncOutlook.png";
		SetBKImage(strBkImage,BK_Fixed);
		const int nMIN_WIDTH_MAINFRAME = GetBKImage()->GetWidth(); 
		const int nMIN_HEIGHT_MAINFRAME = GetBKImage()->GetHeight();
		SetMinWidth(nMIN_WIDTH_MAINFRAME);
		SetMinHeight(nMIN_HEIGHT_MAINFRAME);
		SetMaxWidth(nMIN_WIDTH_MAINFRAME);
		SetMaxHeight(nMIN_HEIGHT_MAINFRAME);
		//end set background

		//Set UI by XML
		CLayoutManager SyncOLDlgLayoutManager(this, this);
		Widge<HDC>* pWidget = SyncOLDlgLayoutManager.Load(_T("Cal_SyncOutlookDlgID"));
		m_pWinCloseButton = (WIN32IconButton *)SyncOLDlgLayoutManager.GetWidgetByID(_T("Btn_WinClose"));
		m_pBtnCancel = (WIN32IconButton *)SyncOLDlgLayoutManager.GetWidgetByID(_T("Btn_Cancel"));
		m_pBtnOK = (WIN32IconButton *)SyncOLDlgLayoutManager.GetWidgetByID(_T("Btn_OK"));
		m_pBtnRadioAll = (WIN32IconButton *)SyncOLDlgLayoutManager.GetWidgetByID(_T("IconBtn_SelectAll"));
		m_pBtnRadioStartBetween = (WIN32IconButton *)SyncOLDlgLayoutManager.GetWidgetByID(_T("IconBtn_StartBetween"));
		m_SubcaptionText = (WIN32LabelWidge *)SyncOLDlgLayoutManager.GetWidgetByID(_T("SubcaptionText"));
		m_ItemNumberText = (WIN32LabelWidge *)SyncOLDlgLayoutManager.GetWidgetByID(_T("ItemNumber"));

		SetLayout(pWidget);
		//End Set UI
	}
	void CSyncOutlookDlg::Event(const TSTRING& strEvent, long nParam)
	{
		if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pBtnOK->GetID() == nParam)
				OnBnClickedOK();
			else if(m_pWinCloseButton->GetID() == nParam || m_pBtnCancel->GetID() == nParam)
				OnBnClickedCancel();
			else if(m_pBtnRadioAll->GetID() == nParam)
				OnBnClickedAll();
			else if(m_pBtnRadioStartBetween->GetID() == nParam)
				OnBnClickedStartBetween();
		}
	}

	void CSyncOutlookDlg::SetCaptionText(const std::wstring& strDisplay)
	{
		if(m_SubcaptionText)
			m_SubcaptionText->SetDisplayString(strDisplay);
	}
	void CSyncOutlookDlg::SetItemNumberText(const std::wstring& strDisplay)
	{
		if(m_ItemNumberText)
			m_ItemNumberText->SetDisplayString(strDisplay);
	}

	//Button function
	void CSyncOutlookDlg::OnBnClickedOK()
	{
		CTime PeriodStartAfter, PeriodStartBefore;
		((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTAFTER))->GetTime(PeriodStartAfter);
		((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER_STARTBEFORE))->GetTime(PeriodStartBefore);
		if(PeriodStartAfter > PeriodStartBefore)
		{
			//swap
			CTime tempTime = PeriodStartAfter;
			PeriodStartAfter = PeriodStartBefore;
			PeriodStartBefore = tempTime;
		}

		//將限制時間存入硬碟,需要使用這資料的人再從硬碟讀出來
		IConfigure* CfgToolSyncOL = CreateIConfigure(_T("SyncOutlookSetting"));
		if(CfgToolSyncOL)
		{
			CCfgInt bCfgSyncWay = CCfgInt(CfgToolSyncOL,_T("SyncWay"),0);
			CCfgInt bCfgShowAll = CCfgInt(CfgToolSyncOL,_T("ShowAll"),1);
			CCfgInt nCfgStartAfterYear = CCfgInt(CfgToolSyncOL,_T("StartAfterYear"),1);
			CCfgInt nCfgStartAfterMonth = CCfgInt(CfgToolSyncOL,_T("StartAfterMonth"),1);
			CCfgInt nCfgStartAfterDate = CCfgInt(CfgToolSyncOL,_T("StartAfterDate"),1);
			CCfgInt nCfgStartBeforeYear = CCfgInt(CfgToolSyncOL,_T("StartBeforeYear"),3000);
			CCfgInt nCfgStartBeforeMonth = CCfgInt(CfgToolSyncOL,_T("StartBeforeMonth"),12);
			CCfgInt nCfgStartBeforeDate = CCfgInt(CfgToolSyncOL,_T("StartBeforeDate"),31);
			CfgToolSyncOL->Load(STORAGE_INIFILE);
			bCfgSyncWay = m_SyncWay;
			bCfgShowAll = m_bShowAll;
			nCfgStartAfterYear =  PeriodStartAfter.GetYear();
			nCfgStartAfterMonth = PeriodStartAfter.GetMonth();
			nCfgStartAfterDate =  PeriodStartAfter.GetDay();
			nCfgStartBeforeYear = PeriodStartBefore.GetYear();
			nCfgStartBeforeMonth = PeriodStartBefore.GetMonth();
			nCfgStartBeforeDate = PeriodStartBefore.GetDay();
			CfgToolSyncOL->Save(STORAGE_INIFILE);
			DeleteIConfigure(_T("SyncOutlookSetting"));
		}

		OnOK();
	}
	void CSyncOutlookDlg::OnBnClickedCancel()
	{
		OnCancel();
	}
	void CSyncOutlookDlg::OnBnClickedAll()
	{
		m_bShowAll = true;
		GetDlgItem(IDC_DATETIMEPICKER_STARTAFTER)->EnableWindow(false);
		GetDlgItem(IDC_DATETIMEPICKER_STARTBEFORE)->EnableWindow(false);
	}
	void CSyncOutlookDlg::OnBnClickedStartBetween()
	{
		m_bShowAll = false;
		GetDlgItem(IDC_DATETIMEPICKER_STARTAFTER)->EnableWindow(true);
		GetDlgItem(IDC_DATETIMEPICKER_STARTBEFORE)->EnableWindow(true);
	}

	BEGIN_MESSAGE_MAP(CSyncOutlookDlg, CQDialog)
		ON_WM_SIZING()
	END_MESSAGE_MAP()

	void CSyncOutlookDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}
}// end namespace QSYNC_LIB
