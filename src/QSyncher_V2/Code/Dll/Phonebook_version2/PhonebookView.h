/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   PhonebookView.h  $
 *  $Revision:   1.41  $
 *  $Date:   Mar 28 2008 10:57:50  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhonebookView.h.-arc  $
 * 
 *    Rev 1.41   Mar 28 2008 10:57:50   Jame Tsai
 * Update
 * 
 *    Rev 1.40   Mar 27 2008 15:40:24   Alan Tu
 * disappear dialog when disconnect 
 * 
 *    Rev 1.39   Mar 21 2008 17:46:58   Jame Tsai
 * refactory and fix error
 * 
 *    Rev 1.38   Mar 20 2008 15:29:20   Jame Tsai
 * remove error step
 * 
 *    Rev 1.37   Feb 27 2008 18:35:12   Jame Tsai
 * add SetXMLUI
 * 
 *    Rev 1.36   Feb 15 2008 10:24:12   Mei Yang
 * add IsOverMSLimit
 * 
 *    Rev 1.35   Feb 13 2008 14:52:42   Mei Yang
 * Update ExistSameName
 * 
 *    Rev 1.34   Feb 12 2008 20:15:56   Mei Yang
 * add judgetment that user can't drop the contact with the same name and transfer right contact name to SIM
 * 
 *    Rev 1.33   Feb 12 2008 18:25:50   Mei Yang
 * add  bool bShowMessage = true to IsOverGroupLimit to show message
 * 
 *    Rev 1.32   Feb 05 2008 12:03:08   Mei Yang
 * add IsOverGroupLimit
 * 
 *    Rev 1.31   Jan 18 2008 22:42:50   Alan Tu
 * update
 * 
 *    Rev 1.30   Jan 11 2008 23:11:08   Alan Tu
 * update
 * 
 *    Rev 1.29   Jan 02 2008 16:43:42   Alan Tu
 * update
 * 
 *    Rev 1.28   Dec 27 2007 10:48:22   Jame Tsai
 * refactory
 * 
 *    Rev 1.27   Dec 24 2007 13:59:28   Alan Tu
 * update
 * 
 *    Rev 1.26   Dec 21 2007 14:50:08   Alan Tu
 * update
 * 
 *    Rev 1.25   Dec 18 2007 22:33:40   Alan Tu
 * update
 * 
 *    Rev 1.24   Dec 18 2007 21:23:22   Alan Tu
 * update
 * 
 *    Rev 1.23   Dec 18 2007 17:14:14   Alan Tu
 * update
 * 
 *    Rev 1.22   Dec 18 2007 10:30:22   Alan Tu
 * update
 * 
 *    Rev 1.21   Dec 18 2007 08:42:54   Alan Tu
 * update
 * 
 *    Rev 1.20   Dec 16 2007 16:18:38   Alan Tu
 * update
 * 
 *    Rev 1.19   Dec 14 2007 20:53:58   Alan Tu
 * update
 * 
 *    Rev 1.18   Dec 14 2007 11:44:10   Alan Tu
 * update
 * 
 *    Rev 1.17   Dec 10 2007 19:44:12   Mei Yang
 * update and add CPhoneBookSIMContactDlg			m_SIMContactEditor;
 * 
 *    Rev 1.16   Nov 29 2007 16:22:46   Alan Tu
 * remove multisync
 * 
 *    Rev 1.15   Nov 21 2007 20:46:30   Alan Tu
 * update for user data
 * 
 *    Rev 1.14   Nov 21 2007 13:34:54   Alan Tu
 * update for remote device
 * 
 *    Rev 1.13   Nov 19 2007 19:46:34   Alan Tu
 * Update for VCF
 * 
 *    Rev 1.12   Nov 15 2007 17:50:50   Alan Tu
 * remove outlook control
 * 
 *    Rev 1.11   Nov 15 2007 16:55:22   Alan Tu
 * edit item from list
 * 
 *    Rev 1.10   Nov 14 2007 18:35:34   Alan Tu
 * update
 * 
 *    Rev 1.9   Nov 14 2007 13:51:08   Alan Tu
 * Add list datasource files
 * 
 *    Rev 1.8   Nov 13 2007 20:20:36   Alan Tu
 * update UI
 * 
 *    Rev 1.7   Nov 13 2007 10:05:52   Alan Tu
 * update UI
 * 
 *    Rev 1.6   Nov 12 2007 20:46:24   Alan Tu
 * update UI
 *
 *
 ***************************************************************************/

#pragma once

#include "resource.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "PhoneBookContactDlg.h"
#include "PhoneBookSIMContactDlg.h"
#include "../../Components/UIFramework/LayoutManager/Win32Widge.h"
#include "../../Components/UIFramework/interface/CQCommonTreeCtrl.h"
#include "../../components/uiframework/porting/qportingtreeui.h"
#include "PhonebookDatasource.h"
#include "../../Components/UIFramework/interface/QCommonControlBase.h"
#include "PhonebookSubmenu.h"
#include "..\..\Components\UIFramework\Porting\QListCtrl.h"
#include "..\..\Components\UIFramework\interface\IQCommListCtrl.h"
#include "..\..\Components\UIFramework\interface\IQListCtrlUI.h"
#include "PhonebookListDatasource.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"
#include "..\..\Components\PIMDB\SyncProcess.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "SyncOutlookDlg.h"

// CPhonebookView form view
namespace QSYNC_LIB
{
	#define		DEF_Configure_Name _T("Phonebook")

	class NotifyReceiver : public INotify
	{
	public:
		NotifyReceiver(INotify *pINotify = NULL):m_pINotify(pINotify){}
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(m_pINotify)
				m_pINotify->Event(strEvent,nParam);
		}
	public:
		QSYNC_LIB::INotify* m_pINotify;
	};

	enum RefreshType
	{
		RT_Self,
		RT_Default,
		RT_OutlookFrom,
		RT_OutlookTo
	};

	class CPhonebookView : public Win32DoubleBufferFormViewWidge , public INotify
	{
		DECLARE_DYNCREATE(CPhonebookView)

	protected:
		CPhonebookView();           // protected constructor used by dynamic creation
		virtual ~CPhonebookView();

	public:
		virtual void Event(const TSTRING& strEvent,long nParam);

	public:
		enum { IDD = IDD_PHONEBOOK_VIEW };
		
#ifdef _DEBUG
		virtual void AssertValid() const;
#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnInitialUpdate();
		
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedPhonebookBtnNew();
		afx_msg void OnBnClickedPhonebookBtnEdit();
		afx_msg void OnBnClickedPhonebookBtnDelete(bool bForceDelete = false);
		afx_msg void OnCbnSelchangeComboSourceDevice();
		afx_msg void OnCbnSelchangeComboTargetDevice();
		afx_msg void OnDestroy();
	private:
		void				SetXMLUI();
		CString		GetString(const TSTRING& strID) const {return m_pILocale->GetString(strID).c_str();}		
		bool				DisplayNodeInfoToList(CQTreeNode* pNode, bool bForceRefresh = false);
		void				Edit(IPIMObjPt PIMObjPt);
		void				EnableEdit(bool bEnable);
		void				EnableConnectionRelatedButton();
		void				EnableInsert(bool bEnable);
		CGUIDTreeNode*		GetGuidNodeByListdata(CQListData* pListData);
		bool				GetPIMObjByTreeNode(CGUIDTreeNode* pTreeNode,IPIMObjPt& PIMObjPt);
		bool				GetPIMObjByListdata(CQListData* pListData,IPIMObjPt& PIMObjPt);
		TSTRING	SaveTempVCF(CQListData* pListData);
		void				SetRemotePIMDevice();
		void				SetConnect(bool bIsConnect);
		void				EnableSync();
		bool				EnableSyncButton();
		CString		GetComboString(UINT nID);
		void				SaveUserSelected(UINT nID);
		void				Sync(SyncType synctype);
		void				SyncWithDevice(UINT nID);
		void				AddDeviceToCombobox(UINT nComboID,IDevicePt ptDevice);
		ePhonebookType GetPBTypeByNode(CQTreeNode* pNode = NULL);
		bool				UpdateDefaultDevice(CQTreeNode* pNode,IPIMObjPt& PIMObjPt,CQListData* pListData,int nListIndex);
		void				ChangePIMFromFiles(CDragDropFireParam *pParam);
		void				InsertPIMFromFiles(CDragDropFireParam *pParam);
		void				GetPIMObjFromFile(const TSTRING& strVCFFile, IPIMObjPt& PIMObjPt,const TSTRING& strCallerID = _T(""));
		TSTRING		GetPIMObjFullName(IPIMObjPt& PIMObjPt);
		TSTRING		GetPIMObjFirstName(IPIMObjPt& PIMObjPt);
		TSTRING		GetPIMObjLastName(IPIMObjPt& PIMObjPt);
		TSTRING		GetPIMObjDisplayName(IPIMObjPt& PIMObjPt);
		TSTRING		GetPIMObjPhone(IPIMObjPt& PIMObjPt);
		TSTRING		GetPIMObjGroup(IPIMObjPt& PIMObjPt);
		void				SetPIMObjName(IPIMObjPt& PIMObjPt,const TSTRING& strLastName,const TSTRING& strFirstName);
		void				SetPIMObjPhone(IPIMObjPt& PIMObjPt,const TSTRING& strName);		
		void				SetPIMObjGroup(IPIMObjPt& PIMObjPt,const TSTRING& strCallerID);
		void				SetPIMObjDisplayName(IPIMObjPt& PIMObjPt,TSTRING& strDisplayName);
		void				InsertSIMFromFiles(CDragDropFireParam *pParam);
		void				InsertMSFromFiles(CDragDropFireParam *pParam,const TSTRING& strCallerID,CQTreeNode* pNode);
		void				InsertPIMFromFiles(CDragDropFireParam *pParam,CQTreeNode* pNode);
		void				ResetFlag();
		bool				IsDragging();
		bool				IsOverGroupLimit(CQTreeNode* pTreeNode, bool bShowMessage = true);
		bool				IsOverMSLimit(bool bShowMessage = true);
		bool				ExistSameName(bool bSIM, const TSTRING& strName);	

	private:
		bool							m_bIsConnect;
		bool							m_bIsShown;
		ILocalization*					m_pILocale;
		CPhoneBookContactDlg			m_ContactEditor;
		CPhoneBookSIMContactDlg			m_SIMContactEditor;


		//For tree UI
		CQPortingTreeUI					m_TreeCtrl;	
		CPhonebookDatasource			m_TreeDatasource;
		CQCommonTreeCtrl				m_TreeLogic;
		CQTreeNode*						m_pCurrentNode;

		//For list UI		
        CQListCtrl						m_ListCtrl;
        CPhonebookListDatasource		m_ListDatasource;
		CQCommListCtrl					m_ListLogical;

		//For progress dialog
		QCommonControlBase				m_QCommonControlBase_Tree;

		//For submenu
		CPhonebookSubMenu				m_submenu;

		//For Button		
		WIN32ButtonWidge*				m_pInsertButton;
		WIN32ButtonWidge*				m_pEditButton;
		WIN32ButtonWidge*				m_pDeleteButton;
		WIN32ButtonWidge*				m_pSyncToOutlookButton;
		WIN32ButtonWidge*				m_pSyncFromOutlookButton;
		WIN32ButtonWidge*				m_pRefreshFromDefaultButton;
		WIN32ButtonWidge*				m_pTwoWaySyncButton;
		static void						Insert();
		static void						Edit();
		static void						Delete();
		static void						TwowaySync();
		static void						DefaultRefresh();
		static void						SyncFromOutlook();
		static void						SyncToOutlook();		
		void									Func_SyncFromOutlook();
		void									Func_SyncToOutlook();
		void									Func_SyncFromMS(RefreshType nType = RT_Default);

		//For configure
		IConfigure*								m_IConfigure;
		CCfgStr									m_strUserTargetDevice;
		CCfgStr									m_strUserSourceDevice;

		//For sync with Outlook
		CSyncOutlookDlg							m_CSyncFromOutlookDlg;
		CSyncOutlookDlg							m_CSyncToOutlookDlg;

		//For batch sync
		SyncProcess<CPhonebookDatasource>		m_SyncProcess;
		RefreshType								m_nRefresType;

		//For drag information
		bool									m_bDragging;
		bool									m_bDragEnd;
		TSTRING									m_strDragingGroup;
		map<TSTRING, CGUIDTreeNode*>			m_mapDeleteNodes;
		bool									m_bEffectMove;
	};


}