/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   ListPage.h  $
 *  $Revision:   1.80  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Calendar_Version2/ListPage.h.-arc  $
 * 
 *    Rev 1.80   Mar 27 2008 16:06:26   Alan Tu
 * update
 * 
 *    Rev 1.79   Mar 20 2008 17:33:54   Jame Tsai
 * update
 * 
 *    Rev 1.78   Mar 19 2008 14:37:32   Alan Tu
 * remove unused function
 * 
 *    Rev 1.77   Mar 18 2008 21:54:04   Jame Tsai
 * update
 * 
 *    Rev 1.76   Mar 14 2008 08:56:52   Jame Tsai
 * update
 * 
 *    Rev 1.75   Mar 13 2008 16:28:54   Jame Tsai
 * remove function SetUI
 * 
 *    Rev 1.74   Mar 13 2008 13:59:38   Jame Tsai
 * Add SyncOutlookDlg
 * 
 *    Rev 1.73   Mar 04 2008 17:23:26   Alan Tu
 * check for delete fail
 * 
 *    Rev 1.72   Feb 22 2008 09:35:38   Alan Tu
 * update for disconnect case
 * 
 *    Rev 1.71   Feb 18 2008 13:24:12   Jame Tsai
 * Donothing while dragging file from handset to handset
 * 
 *    Rev 1.70   Feb 18 2008 09:57:14   Alan Tu
 * update parent of messagebox
 * 
 *    Rev 1.69   Feb 04 2008 11:27:46   Jame Tsai
 * update
 * 
 *    Rev 1.68   Jan 30 2008 17:55:58   Mei Yang
 * update SetXMLUI
 * 
 *    Rev 1.67   Jan 29 2008 10:26:22   Mei Yang
 * add SetXMLUI
 * 
 *    Rev 1.66   Jan 28 2008 14:05:48   Jame Tsai
 * Add XML ID description and refactory
 * 
 *    Rev 1.65   Jan 25 2008 11:43:58   Alan Tu
 * update
 * 
 *    Rev 1.64   Jan 15 2008 11:27:32   Alan Tu
 * update
 * 
 *
 *
 ***************************************************************************/
#pragma once
#include "AppointmentEditor.h"
#include "SyncOutlookDlg.h"
#include "QDataCtrl.h"
#include "..\..\Components\UIFramework\Porting\QListCtrl.h"
#include "..\..\Components\UIFramework\interface\IQCommListCtrl.h"
#include "CalSubMenu.h"
#include "..\..\Components\UIFramework\interface\QCommonControlBase.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\GridLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"
#include "..\..\Components\PIMDB\SyncProcess.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"

#define		EVENT_LIST_CALSUBMENU_APPOINTMENTADD		_T("TNotify_APPOINTMENTAdd")
#define		EVENT_LIST_CALSUBMENU_APPOINTMENTEDIT		_T("TNotify_APPOINTMENTEdit")
#define		EVENT_LIST_CALSUBMENU_APPOINTMENTDELETE		_T("TNotify_APPOINTMENTDelete")
#define		DEF_Configure_Name _T("ListPage")
using namespace QSYNC_LIB;

class CListPage : public Win32DoubleBufferDialogWidge,public QSYNC_LIB::CINotifyBase
{
	DECLARE_DYNAMIC(CListPage)

public:
	CListPage();
	virtual ~CListPage();
	void SetConnect(bool bConnected);
	void Event(const TSTRING& strEvent, long nParam);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnBnClickedBtnnew();
	afx_msg void OnBnClickedBtndelete();
	afx_msg void OnBnClickedBtnedit();	
	afx_msg void OnChangeDuration(NMHDR *pNMHDR, LRESULT *pResult);

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	void 				InsertColumnData();
	void 				UpdateColumnData();
	bool 				AddNewAppointment();
	void 				EditAppointment();
	bool 				DeleteAppointment(vector<fdp>* DeleteData);
	void 				PopSubMenu(long nParam);
	void 				SetButton(BOOL bNew, BOOL bEdit, BOOL bDelete);
	void					OnBnClickedBtngetperiod(bool bDisplayAll);

	CString				GetString(const TSTRING& strID){return m_pILocale->GetString(strID).c_str();}

	CString				GetString(UINT ID);
	void 				SetString(UINT ID,const CString& strBuffer);
	void 				SetCheck(UINT ID,bool bCheck);
	bool 				IsCheck(UINT ID);
	void				SetXMLUI(void);
	void 				SetRemotePIMDevice();
	static void			TwowaySync();
	void						OnRefresh();
	void						OnSyncToOutlook();
	void						OnSyncFromOutlook();
	void						OnBtn_SelectAll();
	void						OnBtn_StartBetween();
	void						Sync(SyncType synctype);
	void				SyncWithDevice(UINT nID);	
	TSTRING				SaveTempICS(CQPObjListData* pListData);
	bool				IsICSFile(const TSTRING& strICSFile) const;
	void				HandleDragContent(CDragDropFireParam* pdropFireParam);
	void				GetAnsiFileContent(const TSTRING& strFile,string& strAnsi);
	void				AddItemByRaw(const wstring& strRaw);
	void				AddItemFromICSFile(const TSTRING& strICSFile);
	void				AddDeviceToCombobox(UINT nComboID,IDevicePt ptDevice);
	REAL				CountStringLength(const TSTRING&);
	bool				IsDroppedFileFromHandset(CDragDropFireParam* pdropFireParam);

public:	
	int									m_nCurrentDataIndex; 
	bool								m_bIsConnectSuccess;
	REAL								m_fWidthOfText_All;

	// IConfigure
	IConfigure*							m_IConfigure;
	CCfgStr								m_strUserTargetDevice;
	CCfgStr								m_strUserSourceDevice;

	CQDataCtrl							m_QDataCtrl;
	CQListCtrl							m_CalendarListCtrl;
	CQCommListCtrl*						m_pListCtrl;
	ILocalization*						m_pILocale;
	CalSubMenu							m_MySubMenu;
	QCommonControlBase					m_SubmenuBase;
	CAppointmentEditor					m_AppEditor;
	CSyncOutlookDlg						m_SyncFromOutlookDlg;
	CSyncOutlookDlg						m_SyncToOutlookDlg;
	bool								m_bIsEditing;
	vector<fdp>*						m_pUISelData;
	bool								m_bConnected;


	//UI control
	CQDateTimeCtrl						m_DataStartCtrl;
	CQDateTimeCtrl						m_DataEndCtrl;

	CButton								m_CheckDuration;
	CComboBox							m_ComboTarget;
	CComboBox							m_ComboSource;

	CLayoutManager*						m_pLayoutManager;
	WIN32ButtonWidge*					m_pNewButton;				
	WIN32ButtonWidge*					m_pDeleteButton;			
	WIN32ButtonWidge*					m_pEditButton;				
	WIN32ButtonWidge*					m_pRefreshButton;			
	WIN32ButtonWidge*					m_pSyncToOutlookButton;		
	WIN32ButtonWidge*					m_pSyncFromOutlookButton;	
	WIN32ButtonWidge*					m_pTwoWaySyncButton;

	SplitLayout<HDC>*					m_pMainSplitLayout;			
	SplitLayout<HDC>*					m_pSplitAllAndDuration;		
	WIN32IconButton*					m_pBtn_Radio_SelectAll;
	WIN32IconButton*					m_pBtn_Radio_StartBetween;
	BoxLayout<HDC>*						m_pDurationBox;				

	SyncProcess<CQDataCtrl>				m_SyncProcess;
private:
	bool				m_bShowAll;
};
