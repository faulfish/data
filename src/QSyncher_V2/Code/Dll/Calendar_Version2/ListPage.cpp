/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   ListPage.cpp  $
*  $Revision:   1.172  $
*  $Date:   Mar 27 2008 16:06:26  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Calendar_Version2/ListPage.cpp.-arc  $
   
      Rev 1.172   Mar 27 2008 16:06:26   Alan Tu
   update
   
      Rev 1.171   Mar 27 2008 14:07:22   Alan Tu
   Disable button when disconnect
   
      Rev 1.170   Mar 20 2008 17:33:54   Jame Tsai
   update
   
      Rev 1.169   Mar 19 2008 14:37:32   Alan Tu
   remove unused function
   
      Rev 1.168   Mar 19 2008 13:59:28   Jame Tsai
   fix error
   
      Rev 1.167   Mar 18 2008 21:53:44   Jame Tsai
   update
   
      Rev 1.166   Mar 18 2008 15:52:54   Jame Tsai
   update
   
      Rev 1.165   Mar 13 2008 20:38:34   Jame Tsai
   update radio btn
   
      Rev 1.164   Mar 13 2008 16:28:54   Jame Tsai
   remove function SetUI
   
      Rev 1.163   Mar 13 2008 16:15:46   Jame Tsai
   add radio button
   
      Rev 1.162   Mar 13 2008 13:59:38   Jame Tsai
   Add SyncOutlookDlg
   
      Rev 1.161   Mar 05 2008 16:16:26   Alan Tu
   update
   
      Rev 1.160   Mar 05 2008 15:10:50   Alan Tu
   update
   
      Rev 1.159   Mar 05 2008 11:33:28   Alan Tu
   update
   
      Rev 1.158   Mar 04 2008 17:26:42   Alan Tu
   add remark
   
      Rev 1.157   Mar 04 2008 17:23:26   Alan Tu
   check for delete fail
   
      Rev 1.156   Mar 03 2008 09:57:36   Mei Yang
   fix error 
   
      Rev 1.155   Feb 27 2008 13:49:10   Mei Yang
   change SetUI -> SetXMLUI
   
      Rev 1.154   Feb 27 2008 11:37:26   Mei Yang
   mark save
   
      Rev 1.153   Feb 27 2008 08:53:52   Mei Yang
   update SetXMLUI
   
      Rev 1.152   Feb 26 2008 10:36:22   Alan Tu
   m_bIsEditing = false;
   
      Rev 1.151   Feb 25 2008 09:56:44   Jame Tsai
   change string
   
      Rev 1.150   Feb 22 2008 09:35:38   Alan Tu
   update for disconnect case
   
      Rev 1.149   Feb 19 2008 17:09:44   Alan Tu
   update for disconnect
   
      Rev 1.148   Feb 18 2008 13:24:12   Jame Tsai
   Donothing while dragging file from handset to handset
   
      Rev 1.147   Feb 18 2008 09:57:16   Alan Tu
   update parent of messagebox
   
      Rev 1.146   Feb 12 2008 10:01:22   Jame Tsai
   fix error
   
      Rev 1.145   Feb 04 2008 11:27:46   Jame Tsai
   update
   
      Rev 1.144   Jan 31 2008 14:45:02   Mei Yang
   add INotify
   
      Rev 1.143   Jan 30 2008 17:55:58   Mei Yang
   update SetXMLUI
   
      Rev 1.142   Jan 29 2008 10:29:08   Mei Yang
   add #include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
   
      Rev 1.141   Jan 29 2008 10:26:40   Mei Yang
   add SetXMLUI
   
      Rev 1.140   Jan 28 2008 14:05:50   Jame Tsai
   Add XML ID description and refactory
   
      Rev 1.139   Jan 25 2008 11:43:58   Alan Tu
   update
   
      Rev 1.138   Jan 18 2008 09:43:06   Jame Tsai
   recover to ver1.136
  
*
*
***************************************************************************/


#include "stdafx.h"
#include "resource.h"       // main symbols
#include "ListPage.h"
#include "..\..\Components\Configuration\QConfigure\QMobileConfigure.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "..\..\CommonUtil\CursorUtility.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

using namespace QSYNC_LIB;

// CListPage dialog
CListPage* g_pThis = NULL;

IMPLEMENT_DYNAMIC(CListPage, Win32DoubleBufferDialogWidge)

CListPage::CListPage() 
: Win32DoubleBufferDialogWidge(IDD_List_Page)
,m_pListCtrl(NULL)
,m_bIsConnectSuccess(false)
,m_bConnected(false)
,m_IConfigure(QSYNC_LIB::CreateIConfigure(DEF_Configure_Name))
,m_SyncFromOutlookDlg(this)
,m_strUserTargetDevice(m_IConfigure,_T("UseTargetDevice"))
,m_strUserSourceDevice(m_IConfigure,_T("UseSourceDevice"))
,m_pNewButton(NULL)
,m_pDeleteButton(NULL)
,m_pEditButton(NULL)
,m_pTwoWaySyncButton(NULL)
,m_SyncProcess(&m_QDataCtrl)
,m_fWidthOfText_All(0)
,m_pSplitAllAndDuration(NULL)
,m_bShowAll(true)
,m_pLayoutManager(NULL)
,m_bIsEditing(false)
{
	g_pThis = this;
	if(m_IConfigure)
		m_IConfigure->Load(QSYNC_LIB::STORAGE_INIFILE);

	CreateILocalization(&m_pILocale);
	m_pUISelData = new vector<fdp>(); 
}

CListPage::~CListPage()
{
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_LBTNDOWN);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_RBTNDOWN);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_LDBCLICK);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_RDBCLICK);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_LCLICK);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_RCLICK);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_DROP);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_DRAG_ENTER);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_DRAG_OVER);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_DRAG_LEAVE); 
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_HEADER_BUTTON_HIT);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_ITEMCHANGED);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_SELECT_ROW);
	m_pListCtrl->Unregister(this, EVENT_LIST_UI_DELETE_ROW);
	m_pILocale->Unregister(this, TNotify_Language_Change);

	if(m_pListCtrl)
		delete m_pListCtrl;

	if(m_pUISelData)
		delete m_pUISelData;

	if(m_IConfigure)
		m_IConfigure->Save();
	
	if(m_pLayoutManager)
	{
		delete m_pLayoutManager;
		m_pLayoutManager = NULL;
	}

	DeleteIConfigure(DEF_Configure_Name);
}

void CListPage::DoDataExchange(CDataExchange* pDX)
{
	Win32DoubleBufferDialogWidge::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Calendar_ListCtr, m_CalendarListCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_DataStartCtrl);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_DataEndCtrl);
	DDX_Control(pDX, IDC_COMBO_DEVICE, m_ComboTarget);
	DDX_Control(pDX, IDC_COMBO_SOURCE_DEVICE, m_ComboSource);
	DDX_Control(pDX, IDC_DURATION_ALL_CHECK, m_CheckDuration);
}


BEGIN_MESSAGE_MAP(CListPage, Win32DoubleBufferDialogWidge)
	ON_WM_CREATE()
	ON_WM_DEADCHAR()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CListPage::OnChangeDuration)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER2, &CListPage::OnChangeDuration)
END_MESSAGE_MAP()


// CListPage message handlers

BOOL CListPage::OnInitDialog()
{
	Win32DoubleBufferDialogWidge::OnInitDialog();

	IQListCtrlUI* pIQListCtrlUI = m_CalendarListCtrl.GetIQListCtrlUI();
	m_pListCtrl = new CQCommListCtrl(&m_QDataCtrl, pIQListCtrlUI);
	m_pListCtrl->Register(this, EVENT_LIST_UI_LBTNDOWN);
	m_pListCtrl->Register(this, EVENT_LIST_UI_RBTNDOWN);
	m_pListCtrl->Register(this, EVENT_LIST_UI_LDBCLICK);
	m_pListCtrl->Register(this, EVENT_LIST_UI_RDBCLICK);
	m_pListCtrl->Register(this, EVENT_LIST_UI_LCLICK);
	m_pListCtrl->Register(this, EVENT_LIST_UI_RCLICK);
	m_pListCtrl->Register(this, EVENT_LIST_UI_DROP);
	m_pListCtrl->Register(this, EVENT_LIST_UI_DRAG_ENTER);
	m_pListCtrl->Register(this, EVENT_LIST_UI_DRAG_OVER);
	m_pListCtrl->Register(this, EVENT_LIST_UI_DRAG_LEAVE);
	m_pListCtrl->Register(this, EVENT_LIST_UI_HEADER_BUTTON_HIT);
	m_pListCtrl->Register(this, EVENT_LIST_UI_ITEMCHANGED);
	m_pListCtrl->Register(this, EVENT_LIST_UI_SELECT_ROW);
	m_pListCtrl->Register(this, EVENT_LIST_UI_DELETE_ROW);
	m_pListCtrl->Register(this, EVENT_LIST_UI_DRAG_BEGIN);

	m_pILocale->Register(this, TNotify_Language_Change);
	m_QDataCtrl.SetParentHWND(GetSafeHwnd());

	InsertColumnData();
	SetXMLUI();

	TSTRING strAll = m_pILocale->GetString(L"IDS_Cal_All");
	m_fWidthOfText_All = CountStringLength(strAll);
	m_pSplitAllAndDuration->SetFixedValue(26+13+(int)m_fWidthOfText_All+20);


	//m_CheckDuration.SetCheck(BST_CHECKED);
	m_pBtn_Radio_SelectAll->SetButtonCheck(true);
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(false);
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER1))->SetMonthCalColor(MCSC_TITLEBK,RGB(20,80,100));
	((CDateTimeCtrl*)GetDlgItem(IDC_DATETIMEPICKER2))->SetMonthCalColor(MCSC_TITLEBK,RGB(20,80,100));

	SetButton(FALSE,FALSE,FALSE);

	return TRUE;
}

BOOL CListPage::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_ESCAPE:
				{
					return TRUE;  //(to disable <Esc>-Key-Exit.)
				}
				break;
			case VK_RETURN:
				return TRUE;
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return __super::PreTranslateMessage(pMsg);
}

void CListPage::SetRemotePIMDevice()
{
	//Clear first
	CComboBox* pTargetDevice = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE);
	CComboBox* pSourceDevice = (CComboBox*)GetDlgItem(IDC_COMBO_SOURCE_DEVICE);
	while(pTargetDevice->GetCount() > 0)
		pTargetDevice->DeleteString(0);
	while(pSourceDevice->GetCount() > 0)
		pSourceDevice->DeleteString(0);

	//Assign device to UI
	IDevicePt defaultDevice;
	if(m_QDataCtrl.GetDefaultDevice(defaultDevice) && defaultDevice)
		AddDeviceToCombobox(IDC_COMBO_SOURCE_DEVICE,defaultDevice);

	IDevicePts devices;
	m_QDataCtrl.GetRemoteDevices(devices);
	for(IDevicePts::iterator Iter = devices.begin();Iter != devices.end();Iter++)
		AddDeviceToCombobox(IDC_COMBO_DEVICE,*Iter);

	//Set default value
	pTargetDevice->SetCurSel(0);
	pTargetDevice->SelectString(0,m_strUserTargetDevice.Get().c_str());
	pSourceDevice->SetCurSel(0);
	pSourceDevice->SelectString(0,m_strUserSourceDevice.Get().c_str());
}

void CListPage::AddDeviceToCombobox(UINT nComboID,IDevicePt ptDevice)
{	
	CComboBox* pComBo = (CComboBox*)GetDlgItem(nComboID);
	TSTRING strName =ptDevice->GetDisplayName();
	int nIndex = pComBo->AddString(GetString(strName));
	pComBo->SetItemDataPtr(nIndex,(void*)ptDevice.GetPointer(ptDevice));
}

void CListPage::TwowaySync()
{
	g_pThis->Sync(TwoWay);
}

void	CListPage::OnRefresh()
{
	if(m_bIsConnectSuccess == true)
	{
		if(m_pListCtrl)
			m_pListCtrl->RefreshClear();

		SyncWithDevice(IDC_COMBO_SOURCE_DEVICE);
		Sync(TwoWay);
		OnBnClickedBtngetperiod(m_pBtn_Radio_SelectAll->GetButtonCheck());
		SetButton(TRUE,FALSE,FALSE);
	}
}

void	CListPage::OnSyncToOutlook()
{
	//OneWayFromServer	
	if(m_bIsConnectSuccess == false)
		return;
	CTime PeriodStart, PeriodEnd;
	m_DataStartCtrl.GetTime(PeriodStart);
	m_DataEndCtrl.GetTime(PeriodEnd);

	if (m_SyncToOutlookDlg.DoModal(SyncToOL,m_bShowAll,&PeriodStart,&PeriodEnd) == IDOK)
	{
		SyncWithDevice(IDC_COMBO_DEVICE);
		Sync(OneWayFromServer);
	}
	SetButton(TRUE,FALSE,FALSE);
}

void	CListPage::OnSyncFromOutlook()
{
	//OneWayFromClient

	if(m_bIsConnectSuccess == false)
		return;

	CTime PeriodStart, PeriodEnd;
	m_DataStartCtrl.GetTime(PeriodStart);
	m_DataEndCtrl.GetTime(PeriodEnd);

	if (m_SyncFromOutlookDlg.DoModal(SyncFromOL,m_bShowAll,&PeriodStart,&PeriodEnd) == IDOK)
	{
		SyncWithDevice(IDC_COMBO_DEVICE);
		Sync(OneWayFromClient);
		OnBnClickedBtngetperiod(m_pBtn_Radio_SelectAll->GetButtonCheck());
	}
	SetButton(TRUE,FALSE,FALSE);
}

void CListPage::SetXMLUI(void)
{
	std::wstring strBkImage = L"MainFrame\\Bk_CalendarView.png";
	SetBKImage(strBkImage,BK_Fixed);
	// Load
	if(!m_pLayoutManager)
		m_pLayoutManager = new CLayoutManager(this, this);
	Widge<HDC>* pWidget = m_pLayoutManager->Load(_T("CalendarID"));
	m_pNewButton =  (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("Win32Button_0"));	
	m_pEditButton =  (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("Win32Button_1"));			
	m_pDeleteButton =  (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("Win32Button_2"));				
	m_pRefreshButton =  (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("Win32Button_5"));			
	m_pSyncToOutlookButton =  (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("Win32Button_3"));		
	m_pSyncFromOutlookButton =  (WIN32ButtonWidge*)m_pLayoutManager->GetWidgetByID(_T("Win32Button_4"));	

	m_pMainSplitLayout =  (SplitLayout<HDC>*)m_pLayoutManager->GetWidgetByID(_T("SplitLayout_0"));			
	m_pSplitAllAndDuration =  (SplitLayout<HDC>*)m_pLayoutManager->GetWidgetByID(_T("SplitLayout_5"));		
	m_pBtn_Radio_SelectAll =  (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("IconBtn_SelectAll"));
	m_pBtn_Radio_StartBetween =  (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("IconBtn_StartBetween"));
	m_pDurationBox = (BoxLayout<HDC>*)m_pLayoutManager->GetWidgetByID(_T("BoxLayout_1"));

	SetLayout(pWidget);
}

void CListPage::Event(const TSTRING& strEvent, long nParam)
{
	if(strEvent == EVENT_LIST_UI_HEADER_BUTTON_HIT)
	{
		SetButton(FALSE,FALSE,FALSE);
		m_QDataCtrl.SortByColumn(((ColumnData*)nParam)->m_strKey.c_str());
		SetButton(TRUE,FALSE,FALSE);
	}
	else if(strEvent == EVENT_LIST_UI_DELETE_ROW)
		DeleteAppointment((vector<fdp>*)nParam);
	else if(strEvent == EVENT_LIST_UI_RBTNDOWN) {
		PopSubMenu(nParam);
	}
	else if(strEvent == EVENT_LIST_UI_LDBCLICK)
	{
		m_nCurrentDataIndex = ((vector<fdp*>*) nParam)->at(0)->nRow;
		EditAppointment();
	} 
	else if(strEvent == EVENT_LIST_CALSUBMENU_APPOINTMENTADD)
		AddNewAppointment();
	else if(strEvent == EVENT_LIST_CALSUBMENU_APPOINTMENTEDIT)
	{
		m_nCurrentDataIndex = m_pUISelData->at(0).nRow;
		EditAppointment();
	}
	else if(strEvent == EVENT_LIST_CALSUBMENU_APPOINTMENTDELETE)
	{
		OnBnClickedBtndelete();
	}
	else if(strEvent == TNotify_Language_Change)
	{
		UpdateColumnData();
	}
	else if(strEvent == EVENT_LIST_UI_ITEMCHANGED)
	{
		if (((vector<fdp>*)nParam)->size() > 1) 
			SetButton(TRUE,FALSE,TRUE);
		else if(((vector<fdp>*)nParam)->size() == 0)
			SetButton(TRUE,FALSE,FALSE);
		else
			SetButton(TRUE,TRUE,TRUE);
	}
	else if(strEvent == EVENT_LIST_UI_DRAG_BEGIN)
	{
		CDragDropFireParam *pFireParam = (CDragDropFireParam*)(nParam);
		pFireParam->SetEffect(DROPEFFECT_COPY);				

		vector<FireListDataParameter> vectSelected;
		m_pListCtrl->GetSelectedItem(vectSelected);
		for(vector<FireListDataParameter>::iterator Iter = vectSelected.begin();Iter != vectSelected.end();Iter++)
		{
			TSTRING strTemp = SaveTempICS((CQPObjListData*)Iter->pListData);
			if(!strTemp.empty())
				pFireParam->Insert(strTemp.c_str());
		}
	}
	else if(strEvent == EVENT_LIST_UI_SELECT_ROW){} 
	else if(strEvent == EVENT_LIST_UI_LBTNDOWN){}
	else if(strEvent == EVENT_LIST_UI_RDBCLICK){} 
	else if(strEvent == EVENT_LIST_UI_LCLICK){} 
	else if(strEvent == EVENT_LIST_UI_RCLICK){}
	else if(strEvent == EVENT_LIST_UI_DROP)
	{
		CCursorWait wait;

		CDragDropFireParam* pdropFireParam = (CDragDropFireParam*)nParam;

		if(IsDroppedFileFromHandset(pdropFireParam) == false)
			for(int i = 0;i< pdropFireParam->GetFileCount();i++)
				AddItemFromICSFile(pdropFireParam->GetByIndex(i));
	}
	else if(strEvent == EVENT_LIST_UI_DRAG_ENTER)
	{
		HandleDragContent((CDragDropFireParam*)nParam);
	} 
	else if(strEvent == EVENT_LIST_UI_DRAG_OVER)
	{
		HandleDragContent((CDragDropFireParam*)nParam);
	}
	else if(strEvent == EVENT_LIST_UI_DRAG_LEAVE)
	{
	}
	else if(strEvent == EVENT_SyncProcess_FINISH)
	{
	}
	else if(strEvent == EVENT_BUTTON_CLICK)
	{
		/*if(m_pTwoWaySyncButton->GetID() == nParam)
			TwowaySync();*/
		if(m_pNewButton->GetID() == nParam)
			OnBnClickedBtnnew();
		else if(m_pDeleteButton->GetID() == nParam)
			OnBnClickedBtndelete();
		else if(m_pEditButton->GetID() == nParam)
			OnBnClickedBtnedit();
		else if(m_pSyncToOutlookButton->GetID() == nParam)
			OnSyncToOutlook();
		else if(m_pSyncFromOutlookButton->GetID() == nParam)
			OnSyncFromOutlook();
		else if(m_pRefreshButton->GetID() == nParam)
			OnRefresh();
		else if(m_pBtn_Radio_SelectAll->GetID() == nParam)
			OnBtn_SelectAll();
		else if(m_pBtn_Radio_StartBetween->GetID() == nParam)
			OnBtn_StartBetween();
	}
	else 
	{
		ASSERT(false);
	}
}

void  CListPage::AddItemFromICSFile(const TSTRING& strICSFile)
{
	if(IsICSFile(strICSFile))
	{
		string strAnsi;
		GetAnsiFileContent(strICSFile,strAnsi);
		AddItemByRaw(ansitowide(strAnsi));
	}
}

void CListPage::AddItemByRaw(const wstring& strRaw)
{				
	CQPObjListData* pListData = m_QDataCtrl.NewPObjListData();
	if(pListData)
	{
		pListData->GetPIMObj()->decode(strRaw);
		m_QDataCtrl.InsertPIMObj(pListData->GetPIMObj());
	}
}

void CListPage::GetAnsiFileContent(const TSTRING& strFile,string& strAnsi)
{
	CFile file(strFile.c_str(),CFile::modeRead | CFile::typeBinary);
	char ch;
	while(file.Read(&ch,1))
		strAnsi += ch;				
	file.Close();
}
void CListPage::HandleDragContent(CDragDropFireParam* pdropFireParam)
{
	bool bExistICS = false;
	for(int i = 0;i< pdropFireParam->GetFileCount();i++)
	{
		if(IsICSFile(pdropFireParam->GetByIndex(i)))
		{
			bExistICS = true;
			break;
		}
	}
	DROPEFFECT effect = DROPEFFECT_NONE;
	if(bExistICS == false)
	{
		pdropFireParam->SetEffect(DROPEFFECT_NONE);
		effect = pdropFireParam->GetEffect();
	}
}

bool CListPage::IsICSFile(const TSTRING& strICSFile) const
{
	return strICSFile.find(_T(".vcs")) != TSTRING::npos;
}

TSTRING CListPage::SaveTempICS(CQPObjListData* pListData)
{	
	TSTRING strTemp;
	TCHAR szFolder[MAX_PATH] = {0};
	if(Win32Utility::GetShellFolder(szFolder,CSIDL_COMMON_TEMPLATES,_T("QSyncer")))
	{			
		IPIMObjPt PIMObjPt = pListData->GetPIMObj();			

		CString strSubject;
		if(pListData->GetCmnData(KEYWORD_SUBJECT,strSubject))
		{
			strTemp = szFolder + Win32Utility::GetCompatiableWin32FileName( strSubject + _T(".vcs"));
		}

		CFile file;
		if(file.Open(strTemp.c_str(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))
		{
			TSTRING strCode;
			PIMObjPt->encode(strCode);
			string strAnsiCode = widetoansi(strCode,CP_ACP);
			file.Write(strAnsiCode.c_str(),static_cast<UINT>(strAnsiCode.size()));
			file.Close();
		}
		else
		{
			strTemp.clear();
		}
	}
	return strTemp;
}

int CListPage::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	using namespace QSYNC_LIB;
	m_SubmenuBase.SetSubMenu(&m_MySubMenu);
	for(int i =0;i<m_MySubMenu.GetMenuItemCount();i++)
	{
		TSTRING strDisplay,strNotifyID;
		m_MySubMenu.GetMenuItem(i,strDisplay,strNotifyID);
		m_SubmenuBase.Register(this,strNotifyID);
	}
	return 0;
}


void CListPage::InsertColumnData()
{ 
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_DTSTART),     GetString(_T("IDS_Cal_StartDT")));
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_DTEND),		GetString(_T("IDS_Cal_EndDT")));
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_SUBJECT),     GetString(_T("IDS_Cal_Subject"))); 
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_DESCRIPTION), GetString(_T("IDS_Cal_Descript")));
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_DALARM),      GetString(_T("IDS_Cal_Alarm")));
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_RECURRENCE),  GetString(_T("IDS_Cal_Recur"))); 
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_CATEGORIES),  GetString(_T("IDS_Cal_Categories")));
	m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_LOCATION),    GetString(_T("IDS_Cal_Location")));	
	//m_QDataCtrl.InsertColumnKeyAndName(_T(KEYWORD_ATTENDEE),    GetString(_T("IDS_Cal_Attende")));
	
}
void CListPage::UpdateColumnData()
{ 
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_DTSTART),     GetString(_T("IDS_Cal_StartDT")));
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_DTEND),       GetString(_T("IDS_Cal_EndDT")));
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_SUBJECT),     GetString(_T("IDS_Cal_Subject")));
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_DESCRIPTION), GetString(_T("IDS_Cal_Descript")));
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_DALARM),      GetString(_T("IDS_Cal_Alarm")));
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_RECURRENCE),  GetString(_T("IDS_Cal_Recur"))); 
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_LOCATION),    GetString(_T("IDS_Cal_Location")));	
	m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_CATEGORIES),    GetString(_T("IDS_Cal_Categories")));	
	//m_QDataCtrl.UpdateColumnByKey(_T(KEYWORD_ATTENDEE),    GetString(_T("IDS_Cal_Attende")));
	
	TSTRING strAll = m_pILocale->GetString(L"IDS_Cal_All");
	m_fWidthOfText_All = CountStringLength(strAll);
	m_pSplitAllAndDuration->SetFixedValue(26+13+(int)m_fWidthOfText_All+20);
	m_pSplitAllAndDuration->SetGeometry(m_pSplitAllAndDuration->GetGeometry());
	int nStartDTLabelLength = (int)CountStringLength(m_pILocale->GetString(L"IDS_Cal_StartDTBetween"));
	m_pDurationBox->SetCellWidth(1,nStartDTLabelLength+10);
	int nTildeLabelLength = (int)CountStringLength(m_pILocale->GetString(L"IDS_Cal_Tilde"));
	m_pDurationBox->SetCellWidth(3,nTildeLabelLength+10);

	m_pSplitAllAndDuration->SetLayout();
	Update();
}


void CListPage::PopSubMenu(long nParam)
{
	vector<fdp*>* pVpfdp = (vector<fdp*>*)nParam;
	vector<fdp*>::iterator iter;

	if(!m_pUISelData->empty())
		m_pUISelData->clear();

	for( iter = pVpfdp->begin(); iter != pVpfdp->end(); iter++)
	{
		fdp temp_fdp = *(*iter);
		m_pUISelData->push_back(temp_fdp);
	}

	for(iter = pVpfdp->begin(); iter != pVpfdp->end(); iter++)
	{
		CPoint point = (*iter)->point;
		UINT nFlags = (*iter)->nFlags;
		bool isFocused = (*iter)->isFocused;

		if(isFocused)
		{
			CRect rect;
			m_CalendarListCtrl.GetWindowRect(&rect);

			point.x += rect.left;
			point.y += rect.top;
			QPoint ptPoint;
			ptPoint.m_nPosX = point.x;
			ptPoint.m_nPosY = point.y;	

			m_SubmenuBase.Event(EVENT_SUBMENU,(long)&ptPoint);	
			break;
		}
	}
}

bool CListPage::AddNewAppointment()
{
	bool bRet = false;
	CQPObjListData* pListData = m_QDataCtrl.NewPObjListData();
	if(pListData)
	{
		m_bIsEditing = true;
		if(m_AppEditor.DoModal(pListData, true) == IDOK && m_bConnected)
		{
			m_bIsEditing = false;
			bRet = m_QDataCtrl.InsertPIMObj(pListData->GetPIMObj());			
			if(!bRet)
				ShowMessage(GetSafeHwnd(),GetString(_T("IDS_InsertFail")), GetString(_T("IDS_CALENDAR_FUNCTION")));
			else
				OnBnClickedBtngetperiod(m_pBtn_Radio_SelectAll->GetButtonCheck());
		}
		else
		{
			delete pListData;
		}
		m_bIsEditing = false;
	}
	return bRet;
}

void CListPage::EditAppointment()
{
	CQPObjListData* pListData = NULL;
	m_QDataCtrl.GetDataByIndex(m_nCurrentDataIndex,(CQListData**)(&pListData));

	m_bIsEditing = true;
	if(m_AppEditor.DoModal(pListData, false) == IDOK && m_bConnected)
	{
		m_bIsEditing = false;
		m_QDataCtrl.UpdateDataByIndex(static_cast<CQListData*>(pListData), m_nCurrentDataIndex);
		OnBnClickedBtngetperiod(m_pBtn_Radio_SelectAll->GetButtonCheck());
	}
	m_bIsEditing = false;
}

void CListPage::SetConnect(bool bConnected)
{
	if(m_bConnected != bConnected)
		m_bConnected = bConnected;

	if(m_bConnected == false)
		SetButton(m_bConnected,m_bConnected,m_bConnected);

	if(bConnected)
	{
		m_QDataCtrl.Connect();
		SetRemotePIMDevice();
		OnRefresh();
	}
	else
	{
		if(m_bIsEditing)
			m_AppEditor.Disconnect(_T("IDS_CALENDAR_FUNCTION"));

		m_QDataCtrl.RefreshClear();
		m_CalendarListCtrl.DeleteAllItems();
	}
}

bool CListPage::DeleteAppointment(vector<fdp>* DeleteData)
{
	CCursorWait wait;

	bool bRes = true;
	int nCount = 0;
	int nDeleteable = ShowMessage(GetSafeHwnd(),GetString(_T("IDS_Cal_Delete_PROMPT")),GetString(_T("IDS_CALENDAR_FUNCTION")),QMB_YESNO);
	if(nDeleteable == QIDYES){

		for(vector<fdp>::iterator iter = DeleteData->begin(); iter != DeleteData->end(); iter++)
		{
			int nRow = iter->nRow;
			nRow = nRow-nCount;			
			bool bResDelete = m_QDataCtrl.DeleteDataByIndex(nRow);

			////////////////////////////////////////////////////////////////////////////////////////////////////////
			//2008/3/4 by alan tu
			//This a tricky for MMI too busy to delete event.
			//It should be retry and wait for one second.
			int nRetryTimes = 10;
			while(bResDelete == false && nRetryTimes-- > 0)
			{
				Sleep(2000);
				bResDelete = m_QDataCtrl.DeleteDataByIndex(nRow);
			};
			////////////////////////////////////////////////////////////////////////////////////////////////////////

			if(bResDelete == false)
			{
				bRes = false;
				break;
			}
			nCount++;
		}
		return bRes;
	}

	//Reset button
	int nSelectedCount = m_CalendarListCtrl.GetSelectedCount();
	SetButton(TRUE,nSelectedCount == 1,nSelectedCount > 0);
	return bRes;
}

void CListPage::OnBnClickedBtnnew()
{
	AddNewAppointment();
}

void CListPage::OnBnClickedBtndelete()
{
	vector<fdp> vfdpRowData;
	m_pListCtrl->GetSelectedItem(vfdpRowData);
	bool bRes = DeleteAppointment(&vfdpRowData);
	if(bRes == false)
		ShowMessage(GetSafeHwnd(),GetString(_T("IDS_Cal_Delete_Fail")), GetString(_T("IDS_CALENDAR_FUNCTION")));
}

void CListPage::OnBnClickedBtnedit()
{
	vector<fdp> vfdp;
	m_pListCtrl->GetSelectedItem(vfdp);
	m_nCurrentDataIndex = vfdp.at(0).nRow;
	EditAppointment();
}

void CListPage::OnChangeDuration(NMHDR *pNMHDR, LRESULT *pResult)
{
	OnBnClickedBtngetperiod(false);
}

void CListPage::OnBnClickedBtngetperiod(bool bDisplayAll)
{
	if(bDisplayAll && !m_bShowAll)
	{
		m_QDataCtrl.DisplayAll();
	}
	else if(!bDisplayAll)
	{
		COleDateTime PeriodStart, PeriodEnd;
		m_DataStartCtrl.GetTime(PeriodStart);
		m_DataEndCtrl.GetTime(PeriodEnd);

		if (PeriodStart > PeriodEnd)
		{
			//swap
			COleDateTime tempTime = PeriodStart;
			PeriodStart = PeriodEnd;
			PeriodEnd = tempTime;
		}

		PeriodStart.SetDateTime(PeriodStart.GetYear(),PeriodStart.GetMonth(), PeriodStart.GetDay(),0,0,0);
		PeriodEnd.SetDateTime(PeriodEnd.GetYear(),PeriodEnd.GetMonth(), PeriodEnd.GetDay(),23,59,59);
		m_QDataCtrl.SetPeriod(PeriodStart, PeriodEnd);
	}
}

void CListPage::Sync(SyncType synctype)
{
	if(m_bIsConnectSuccess == true)
	{
		using namespace QSYNC_LIB;
		m_SyncProcess.Register(this,EVENT_SyncProcess_FINISH);
		m_SyncProcess.start(synctype);
	}
}

void CListPage::SyncWithDevice(UINT nID)
{
	IDevice* pIDevice = NULL;
	CComboBox* pDevice = (CComboBox*)GetDlgItem(nID);	
	if(pDevice && ::IsWindow(pDevice->GetSafeHwnd()))
	{
		if(pDevice->GetCount() > 0)
			pIDevice = (IDevice*)pDevice->GetItemDataPtr(pDevice->GetCurSel());
	}

	if(pIDevice)
		m_SyncProcess.push(pIDevice);
}

void CListPage::SetButton(BOOL bNew, BOOL bEdit, BOOL bDelete)
{
	if(m_pNewButton)
		m_pNewButton->SetEnable(bNew && m_bConnected == true);
	if(m_pRefreshButton)
		m_pRefreshButton->SetEnable(bNew && m_bConnected == true);
	if(m_pSyncToOutlookButton)
		if(m_QDataCtrl.GetRowCount() == 0)
			m_pSyncToOutlookButton->SetEnable(false);
		else
			m_pSyncToOutlookButton->SetEnable(bNew && m_bConnected == true);
	if(m_pSyncFromOutlookButton)
		m_pSyncFromOutlookButton->SetEnable(bNew && m_bConnected == true);

	if(m_pDeleteButton)
		m_pDeleteButton->SetEnable(bDelete && m_bConnected == true);
	if(m_pEditButton)
		m_pEditButton->SetEnable(bEdit && m_bConnected == true);

	if(bEdit)
		m_MySubMenu.SetMenuItemCount(3);
	else
		m_MySubMenu.SetMenuItemCount(2);
	m_SubmenuBase.SetSubMenu(&m_MySubMenu);
	for(int i =0;i<m_MySubMenu.GetMenuItemCount();i++)
	{
		TSTRING strDisplay,strNotifyID;
		m_MySubMenu.GetMenuItem(i,strDisplay,strNotifyID);
		m_SubmenuBase.Register(this,strNotifyID);
	}
}

void CListPage::OnBtn_SelectAll()
{	
	if (m_bShowAll)
		return;
	m_bShowAll = true;
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(false);
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(false);
	m_QDataCtrl.DisplayAll();
}
void CListPage::OnBtn_StartBetween()
{
	if(m_bShowAll == false)
		return;
	m_bShowAll = false;
	GetDlgItem(IDC_DATETIMEPICKER1)->EnableWindow(true);
	GetDlgItem(IDC_DATETIMEPICKER2)->EnableWindow(true);
	//從兩個Dlg取出日期格式
	COleDateTime PeriodStart, PeriodEnd;
	m_DataStartCtrl.GetTime(PeriodStart);
	m_DataEndCtrl.GetTime(PeriodEnd);
	PeriodStart.SetDateTime(PeriodStart.GetYear(),PeriodStart.GetMonth(), PeriodStart.GetDay(),0,0,0);
	PeriodEnd.SetDateTime(PeriodEnd.GetYear(),PeriodEnd.GetMonth(), PeriodEnd.GetDay(),23,59,59); //把時間換成23:59:59

	//篩選出符合條件的資料
	PeriodStart < PeriodEnd ? m_QDataCtrl.SetPeriod(PeriodStart, PeriodEnd) : m_QDataCtrl.SetPeriod(PeriodEnd, PeriodStart);
}

bool CListPage::IsCheck(UINT ID)
{
	CButton* pButton = (CButton*)GetDlgItem(ID);
	return pButton->GetCheck() == BST_CHECKED;
}

void CListPage::SetCheck(UINT ID,bool bCheck)
{
	CButton* pButton = (CButton*)GetDlgItem(ID);
	return pButton->SetCheck(bCheck ? BST_CHECKED : BST_UNCHECKED);
}

void CListPage::SetString(UINT ID,const CString& strBuffer)
{
	CButton* pButton = (CButton*)GetDlgItem(ID);
	pButton->SetWindowText(strBuffer);
}

CString CListPage::GetString(UINT ID)
{
	CString strTemp;	
	CButton* pButton = (CButton*)GetDlgItem(ID);
	pButton->GetWindowText(strTemp);
	return strTemp;
}

REAL CListPage::CountStringLength(const TSTRING& strStr)
{
	HWND hTemp = GetSafeHwnd();
	Graphics graphics(hTemp);
	Font myFont(L"Arial", 8);
	StringFormat format;
	SizeF  szLayoutSize( 200 , 211-175 );
	SizeF  szBound;
	graphics.MeasureString(strStr.c_str(),(int)strStr.length(),&myFont,szLayoutSize,&format,&szBound);
	return szBound.Width;
}

bool CListPage::IsDroppedFileFromHandset(CDragDropFireParam* pdropFireParam)
{
	CString strSourceFileFullPath = pdropFireParam->GetByIndex(0).c_str();
	strSourceFileFullPath.MakeLower();
	strSourceFileFullPath.Replace(_T("\\"),_T("/"));
	int nFirstSlash = strSourceFileFullPath.ReverseFind(_T('/'));
	CString strSourceFileFolderPath = strSourceFileFullPath.Left(nFirstSlash);
	int nSecondSlash = strSourceFileFolderPath.ReverseFind(_T('/'));
	CString strSourceFileFolderPathBack = strSourceFileFolderPath.Left(nSecondSlash);

	TCHAR szMy_DocumentPath[MAX_PATH] = {0};
	SHGetFolderPath(GetSafeHwnd(), CSIDL_COMMON_TEMPLATES, NULL, NULL, szMy_DocumentPath);
	CString strTargetFileFolderPath = szMy_DocumentPath;
	strTargetFileFolderPath.MakeLower();
	strTargetFileFolderPath.Replace(_T("\\"),_T("/"));

	return strTargetFileFolderPath == strSourceFileFolderPathBack;
}


#pragma warning(pop)