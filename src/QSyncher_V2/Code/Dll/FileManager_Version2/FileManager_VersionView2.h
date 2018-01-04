// FileManager_VersionView.h : interface of the CMy22View class
//

#if !defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__FileManager_VersionView2__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__FileManager_VersionView2__INCLUDED_

#pragma once


#include "../../Components/UIFramework/Porting/QPortingTreeUI.h"
#include "../../Components/UIFramework/interface/CQCommonTreeCtrl.h"
#include "../../Components/UIFramework/interface/CommonUIParameter.h"
#include "../../Components/UIFramework/interface/QCommonControlBase.h"
#include "../../components/uiframework/porting/qportingtreeui.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32Widge.h"
#include "..\..\Components\UIFramework\Porting\QListCtrl.h"
#include "..\..\Components\UIFramework\interface\IQCommListCtrl.h"
#include "..\..\Components\UIFramework\interface\IQListCtrlUI.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "ShellCtrlClasses.h"
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"

#include "CMEListSubMenu.h"
#include "CQMEFileListDataSource.h"
#include "CQMEFileTreeDataSource.h"
#include "FileSubMenu.h"
#include "ProgressBar.h"
#include "afxcmn.h"
#include "resource.h"
#include "InputFolderDlg.h"

namespace QSYNC_LIB
{
	enum FileManager_ListWnd
	{ 
		None,
		METree,
		MEList,
		PCTree,
		PCList
	};

	class NotifyReceiver : public INotify
	{
	public:
		NotifyReceiver(INotify *pINotify = NULL):m_pINotify(pINotify){}
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(m_pINotify)
				m_pINotify->Event(strEvent,nParam);
		}
		QSYNC_LIB::INotify* m_pINotify;
	};

	class CFMLIstInfo
	{
	public:
		CString strFileName;
		int         nRow;

		CFMLIstInfo(){ nRow = -1; }
		virtual ~CFMLIstInfo() { nRow = -1; strFileName.Empty();  }
	};

	class CFileManager_VersionView2 : public Win32DoubleBufferFormViewWidge, public INotify
	{		
		class EnableExistNotify : public INotify
		{
		public:
			EnableExistNotify(CFileManager_VersionView2 *pThis):m_pThis(pThis){}
			~EnableExistNotify()
			{
				m_pThis->m_Progress.Unregister(this,EVENT_PROGRESS_TASK_FINISH);
			}
			virtual void Event(const TSTRING& strEvent,long nParam)
			{
				m_pThis->EnableExit(true);
				m_pThis->DisplayNodeInfoToList(m_pThis->m_pCurrentNode,true);
                m_pThis->m_PCListCtrl.RefreshList();
			}
		protected:
			CFileManager_VersionView2* m_pThis;
		};

        // this class is used for notify by list and tree control on ME
        class CFileManagerEventMEHandler: public INotify {  ///< ME list and tree event handler
        private:
            CFileManagerEventMEHandler() {}
        public:
            CFileManagerEventMEHandler(CFileManager_VersionView2* pView): m_pView(pView) {}
            ~CFileManagerEventMEHandler() {}
            virtual void Event(const TSTRING& strEvent, long nParam);
        private:
            CFileManager_VersionView2* m_pView;
        };

        // this class is used for notify by list and tree control on PC
        class CFileManagerEventPCHandler: public INotify {  ///< PC list and tree event handler
        private:
            CFileManagerEventPCHandler() {}
        public:
            CFileManagerEventPCHandler(CFileManager_VersionView2* pView): m_pView(pView) {}
            ~CFileManagerEventPCHandler() {}
            virtual void Event(const TSTRING& strEvent, long nParam);
        private:
            CFileManager_VersionView2* m_pView;
        };

	protected: // create from serialization only
		CFileManager_VersionView2();
		DECLARE_DYNCREATE(CFileManager_VersionView2)
	public:
		enum{ IDD = IDD_FILEMANAGER_VERSION2_FORM };
		
	protected:
        virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnInitialUpdate(); // called first time after construct

		// Implementation
	public:
		virtual ~CFileManager_VersionView2();
#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

	protected:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnConnect();

	protected:
        virtual void Event(const TSTRING& strEvent,long nParam);
        bool NewMEFolder(CQTreeNode* pNode);
        bool DelMETreeFolder(CQTreeNode* pNode);
        bool RefreshMEFolder(CQTreeNode* pNode);
		bool IsFolderDeleteable(CCommonFileNode* pNode) const;

		bool DelMEListSelectedFile();

		bool InitFileMgrList(void);
		bool DisplayFileTreeChildNodeToList(CMEFileNode* pNode);
		bool DisplayNodeInfoToList(CQTreeNode* pNode,bool bForce = false);
		void UploadFilesToME(CQTreeNode *pNode,CDragDropFireParam *pParam);
		bool DownloadFilesIntoPC(const CString& strPCFolder, vector<CString> &vct_strMEFilePath);
		CMEFileNode* ConvertListIndexToTreeNode(int nListIndex);
		CString Getfolder();
		bool RenameListFile();
		bool OKRename(const CString& strFolder,CMEFileNode *pNode,const CString& strNewFileName);
		
        void OnEventListUIDrop(long nParam);
        void OnEventListRButtonDown(long nParam);
        void OnEventTreeRButtonDown(long nParam);
        void OnEventTreeDeleteNode(long nParam);
        void OnEventTreeInsertNode(long nParam);
        void OnEventTreeSelectNode(long nParam);
        void OnEventTreeUpdateNode(long nParam);
        void OnEventListHeaderButtonHit(long nParam);
        void OnEventTreeUIDrop(long nParam);
        void OnEventAfterViewSwitch(long nParam);
        void OnEventBeforeViewSwitch(long &nParam);
        void OnEventClose(long nParam);
        void OnEventDeviceConnect();
        void OnEventDeviceDisconnect();
        void OnRefreshUILocalization();
	public:
		void EnableExit(bool bCanExit){	m_bCanExit = bCanExit;}
		template <class T>
		bool ProcSubMenu(FireUIParam<T>* pNode);
		bool IsSourceAndTargetFolderTheSame(CDragDropFireParam* pSourceParam,CQTreeNode* pTargetNode);
		bool IsDragging() const {return m_bIsDragging;}
		void SetDragging(bool IsDragging){m_bIsDragging = IsDragging;}
		void SetCurrentWnd(FileManager_ListWnd CurrentWnd) {m_CurrentWnd = CurrentWnd;}
		FileManager_ListWnd GetCurrentWnd() const {return m_CurrentWnd;}
		HWND GetCtrlHwnd(FileManager_ListWnd nCtrl) const
		{
			if (nCtrl == METree)
				return m_METreeCtrl.GetSafeHwnd();
			else if (nCtrl == MEList)
				return m_MEListCtrl.GetSafeHwnd();
			else if (nCtrl == PCTree)
				return m_PCTreeCtrl.GetSafeHwnd();
			else if (nCtrl == PCList)
				return m_PCListCtrl.GetSafeHwnd();
			else
				return 0;
		}

	public:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	private:
		void SetXMLUI();
		void FuncDelete();
		void FuncRefresh();

	private:
        ILocalization*					m_pILocalization;
		CQTreeNode*						m_pCurrentNode;
		IPIMCONFIG*						m_pIPIMConfig;

		//While submenu of ME Tree pop up, save the selected item
		CQTreeNode*						m_pTempSubmenuMETreeNode;

        // ME Tree 
        CQPortingTreeUI				    m_METreeCtrl;
        CQMEFileTreeDataSource			m_METreeDataSrc;
        CQCommonTreeCtrl*				m_pMETreeLogical;
        QCommonControlBase				m_QCommonControlBase_METree;	
        CFileSubMenu					m_METreeSubMenu;

        // ME List
        CQListCtrl						m_MEListCtrl;
        CQMEFileListDataSource			m_MEListDataSrc;
        CQCommListCtrl*					m_pMEListLogical;
        QCommonControlBase				m_QCommonControlBase_MEList;
        CMEListSubMenu					m_MEListSubMenu;

        // PC Tree 
        CShellTreeCtrl   				m_PCTreeCtrl;
        CQMEFileTreeDataSource			m_PCTreeDataSrc;
        CQCommonTreeCtrl*				m_pPCTreeLogical;
        QCommonControlBase				m_QCommonControlBase_PCTree;	
        CFileSubMenu					m_PCTreeSubMenu;

        // PC List
        CShellListCtrl  				m_PCListCtrl;
        CQMEFileListDataSource			m_PCListDataSrc;
        CQCommListCtrl*					m_pPCListLogical;
        QCommonControlBase				m_QCommonControlBase_PCList;
        CMEListSubMenu					m_PCListSubMenu;

        CMEFileNode*                    m_pCurrentFileNode;
        ProgressBar						m_Progress;

        bool							m_bShow;
        bool							m_bConnected;
        bool							m_bCanExit;
		bool							m_bIsDragging;

        EnableExistNotify				m_EnableExistNotify;

		WIN32ButtonWidge*	m_pDeleteButton;                ///< Button
		WIN32ButtonWidge*	m_pRefreshButton;               ///< Button
		CLayoutManager*		m_pLayoutManager;

        CFileManagerEventMEHandler* m_pMEEventHandler;
        CFileManagerEventPCHandler* m_pPCEventHandler;

		FileManager_ListWnd m_CurrentWnd;
		CInputFolderDlg*	m_pInputFolderDlg;
		CRenameDlg*		m_pRenameFileDlg;
	};	

}//namespace end
/*
#ifndef _DEBUG  // debug version in CFileManager_VersionView2.cpp
inline CMy22Doc* CFileManager_VersionView2::GetDocument() const
{ return reinterpret_cast<CMy22Doc*>(m_pDocument); }
#endif
*/
#endif