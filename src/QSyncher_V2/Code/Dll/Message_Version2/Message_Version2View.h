/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
*
*                           QSyncher V2 Project
*
*	             Copyright (c) 2005 BENQ Corpration
*
****************************************************************************
* File Description
* ----------------
*
*
***************************************************************************
*
* Revision Details
* ----------------
*
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/Message_Version2View.h.-arc  $
 * 
 *    Rev 1.51   Mar 27 2008 15:26:56   Alan Tu
 * Auto close when disconnect
 * 
 *    Rev 1.50   Mar 24 2008 14:20:20   Alan Tu
 * check self item when drag
* 
*
*
***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file Message_Version2View.h
@brief {brief description of Message_Version2View.h}

{detail description of Message_Version2View.h}
*/
#ifndef __MESSAGE_VERSION2VIEW_H__
#define __MESSAGE_VERSION2VIEW_H__

// Message_Version2View.h : interface of the CMessage_Version2View class
//
#ifdef MESSAGE_VERSION2_EXPORTS
#define MESSAGE_VERSION2 __declspec(dllexport)
#else
#define MESSAGE_VERSION2 __declspec(dllimport)
#endif

#include "../../CommonUtil/NotifyBase.h"
#include "../../CommonUtil/QSyncerFrameworks.h"
#include "../../Components/ConfigureTool/IConfigure.h"
#include "../../Components/Localization/ilocalization.h"
#include "../../Components/SMSServer/SMSServer.h"
#include "../../Components/UIFramework/LayoutManager/Win32FrameworkWidge.h"
#include "../../Components/UIFramework/LayoutManager/Win32Widge.h"
#include "../../Components/UIFramework/Porting/QEditCtrl.h"
#include "../../Components/UIFramework/Porting/QListCtrl.h"
#include "../../Components/UIFramework/Porting/QPortingTreeUI.h"
#include "../../Components/UIFramework/Porting/QProgressServer.h"
#include "../../Components/UIFramework/Porting/QUIDropTraget.h"
#include "../../Components/UIFramework/Porting/UIOleDataSource.h"
#include "../../Components/UIFramework/Porting/xSkinButton.h"
#include "../../Components/UIFramework/interface/CQCommonTreeCtrl.h"
#include "../../Components/UIFramework/interface/IQCommListCtrl.h"
#include "../../Components/UIFramework/interface/IQListCtrlUI.h"
#include "../../Components/UIFramework/interface/QCommonControlBase.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "MessageEditor.h"
#include "MessageListDataSource.h"
#include "MessageProgressBar.h"
#include "MessageServerAdapter.h"
#include "MessageSubMenu.h"
#include "MessageTreeDataSource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "shlobj.h"
#include "resource.h"

#define WM_DELETE_SUCCESS WM_USER   + 1
#define WM_FORWARD_SUCCESS WM_USER  + 2
#define WM_SEND_SUCCESS WM_USER     + 3
#define WM_SAVE_SUCCESS WM_USER     + 4
#define WM_BACKUP_SUCCESS WM_USER   + 5
#define WM_RESTORE_SUCCESS WM_USER  + 6

#define WM_DELETE_FAILED WM_USER    + 7
#define WM_FORWARD_FAILED WM_USER   + 8
#define WM_SEND_FAILED WM_USER      + 9
#define WM_SAVE_FAILED WM_USER      + 10
#define WM_BACKUP_FAILED WM_USER    + 11
#define WM_RESTORE_FAILED WM_USER   + 12

#define WM_REFRESH_SUCCESS WM_USER  + 13
#define WM_REFRESH_FAILED WM_USER   + 14

namespace QSYNC_LIB {


	class NotifyReceiver : public QSYNC_LIB::INotify
	{
	public:
		NotifyReceiver():m_pINotify(NULL){}
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(m_pINotify)
				m_pINotify->Event(strEvent,nParam);
		}
		QSYNC_LIB::INotify* m_pINotify;
	};


	//class QListCtrl;
	// message version 2 main frame
	class CMessage_Version2View : public Win32DoubleBufferFormViewWidge, public CINotifyBase
	{
	public:
		CMessage_Version2View();
		virtual ~CMessage_Version2View();

	protected: // create from serialization only
		DECLARE_DYNCREATE(CMessage_Version2View)

	public:
		enum{ IDD = IDD_MESSAGE_VERSION2_FORM };

	public:
		virtual void Event(const TSTRING& strEvent, long nParam);
		void BeginProgressBar(long nParam);
		void IncreaseProgressBar();
		void EndProgressBar(TSTRING strMessage = _T(""));
		void OnEventAfterSwitchView(long nParam);

		static UINT ForwardMessageThread(LPVOID lpParam);
		static UINT SendMessageThread(LPVOID lpParam);
		static UINT SaveMessageThread(LPVOID lpParam);
		static UINT SaveMessageThreadNew(LPVOID lpParam);
		static UINT DeleteMessageThread(LPVOID lpParam);
		static UINT RefreshMessageThread(LPVOID lpParam);
		static UINT BackupMessageThread(LPVOID lpParam);
		static UINT RestoreMessageThread(LPVOID lpParam);
		static UINT ListUIDropThread(LPVOID lpParam);
		static UINT ProgressBarThread(LPVOID);
		static wstring SGetString(const TSTRING& strID) {
			ILocalization* pil;
			CreateILocalization(&pil);
			return pil->GetString(strID);
		}
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		virtual void OnInitialUpdate(); // called first time after construct

#ifdef _DEBUG
		virtual void AssertValid() const;
		virtual void Dump(CDumpContext& dc) const;
#endif

	protected:
		void OnEventListUILButtonDown(FireListDataParameter* pFdp);
		void OnEventListUILButtonDoubleClick(FireListDataParameter* pFdp);
		void OnEventTreeUILSelectChange(CQTreeNode* pNode);
		void OnEventListUIRButtonDown(vector<FireListDataParameter*>* vecFPD);
		void OnEventSMSServerListOneFolder();
		void OnEventSMSServerListFolderOK();
		void OnEventSMSServerDeleteOneMessage();
		void OnEventSMSServerWriteOneMessage();
		void OnEventSMSServerSaveOneMessage();
		void OnListUIBeginDrag(long nParam);
		void OnEventUIItemChanged(long nParam);
		void OnEventListUIDrop(long nParam);
		void OnDlgSendMessage(CString strContactList, CString strContentText);
		void OnDlgSaveMessage(CString strContactList, CString strContentText);
		void OnEventOnSize(long nParam);
		void OnEventOnSizing(long nParam);

	public:
		IConfigure*             m_pCfgTool;         // IConfigure
		CQPortingTreeUI         m_folderTreeCtrl;   // 左邊Tree Control
		CQListCtrl              m_msgListCtrl;      // 右邊List Control
		CMessageListDataSource  m_MsgDataSrc;       // Message List Data Source
		CMessageTreeDataSource  m_MsgTreeSrc;       // Folder Tree Data Source
		CSMSServer              m_SMSServer;        // SMS Server
		CQCommListCtrl*         m_pListCtrlLogic;   // Logic層, 須加入ListUI及List Data Source
		CQCommonTreeCtrl*       m_pTreeCtrlLogic;   // Logic層, 須加入TreeUI及Tree Data Source
		CMessageProgressBar     m_ProgressBar;      // Progress bar
		int m_nCurrentProgress;                     // 現在到第幾筆
		int m_nTotalMessageCount;                   // 一共多少筆, 先記下來
		CMessageEditor m_MsgBox;

		// Logic層, 須加入List Data Source, Tree Data Source, 及SMSServer
		CMessageServerAdapter*  m_pServerAdapter;

		ILocalization* m_pLocalization;             // Localization, 在Constructor裡加入
		CQEditCtrl m_cntEdit;                       // Content Edit, 實際簡訊的訊息

		CMessageSubMenu m_MessageSubMenu;           // sub-menu
		QCommonControlBase m_SubmenuBase;           //
		CUIOleDataSource m_DropSource;              // Drop Source
		bool		m_bIsDragging;
		bool		m_bIsSelfDragging;

	private:

		void New();
		void For();
		void Rep();
		void Sen();
		void Del();
		void Ref();

	private:
		struct DeleteThreadParam {
			CSMSServer* pServer;                    ///< SMS server
			CMessageListDataSource* pDataSrc;       ///< dataSource
			vector<FireListDataParameter>* pVecFDP; ///< delete parameters
			CMessage_Version2View* pThis;           ///< this class
		};
		struct WriteThreadParam {
			CSMSServer* pServer;                    ///< SMS Server
			CString strContact;                     ///< contact list
			CString strContent;                     ///< message content
			CMessage_Version2View* pThis;           ///< this class
			int nFolderID;                          ///< folder id
			int nTag;                               ///< tag indicate the message is read or not
			enumStorage eStorage;                   ///< storage SM or ME
		};
		struct SendThreadParam {
			CSMSServer* pServer;                    ///< SMS Server
			CString strContact;                     ///< contact list
			CString strContent;                     ///< message content
			CMessage_Version2View* pThis;           ///< this class
		};
		struct BackupRestoreThreadParam {
			CSMSServer* pServer;                    ///< SMS Server
			CString strFileName;                    ///< restore file name
			CMessage_Version2View* pThis;           ///< this class
		};
		struct RefreshThreadParam {
			CSMSServer* pServer;                    ///< SMS Server
			CMessage_Version2View* pThis;           ///< this class
		};
		struct ListUIDropThreadParam {
			CSMSServer* pServer;                    ///< SMS Server
			CMessage_Version2View* pThis;           ///< this class
			CDragDropFireParam* pDropFileParam;     ///< CDragDropFireParam*
		};
		WIN32ButtonWidge* m_btnNew;                 ///< new a message
		WIN32ButtonWidge* m_btnForward;             ///< forward a message
		WIN32ButtonWidge* m_btnReply;               ///< reply a message
		WIN32ButtonWidge* m_btnSend;                ///< send a message
		WIN32ButtonWidge* m_btnDelete;              ///< delete a message
		WIN32ButtonWidge* m_btnRefresh;             ///< refresh the messages from mobile
		SplitLayout<HDC>* m_pSplitLayout;           ///< layout
		std::map<TSTRING,TSTRING> m_mapPB;          ///< phone book item
		bool m_bIsConnected;                        ///< indicate the mobile is connect or not
		bool m_bIsSynced;                           ///< indicate the messages are synced to view or not
		CWinThread* m_pProgressThread;              ///< pointer to progress bar thread
		bool m_bIsShow;                        ///< indicate the mobile is show or not
		CEvent m_ProgressFinishEvent;
		TSTRING m_strMessage;
	private:
		void Init();                            // 初使化
		void InsertColumn();                    // 之後可能要用問的, 看有哪些column是要show出來的
		void InsertTreeNode();                  // 之後可能要用問的, 看有哪些folder是要list出來, 使用者也可自建folder
		void ConnectMobile();                   // 呼叫SMS Server裡頭的Create ISMS
		void DisconnectMobile();                // 斷線, 應該不用主動斷線
		void RefreshUILocalization();           // 更新畫面的location動作, 用各別的setwindowtext完成
		void ClearEditText();
		void UpdateButtonStatus(                // 改新Button的狀態
			bool btnNew,
			bool btnForward,
			bool btnReply,
			bool btnSend,
			bool btnDelete,
			bool btnRefresh
			);
		// 處理編輯完成後的動作, 呼叫各別的function處理send, save
		void ProcessMessageEditorReturn(INT_PTR nRetCode, CString strReveivers, CString strContent, bool bSaveToSim = false);
		CString GetString(const TSTRING& strID) { return m_pLocalization->GetString(strID).c_str(); }	
		void SetXMLUI();
		void GetPhonebook();

		// Generated message map functions
	protected:
		DECLARE_MESSAGE_MAP()
		// User defined behavior
		afx_msg LRESULT OnThreadBakupMessage  (WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnThreadDeleteMessage (WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnThreadForwardMessage(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnThreadRefreshMessage(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnThreadRestoreMessage(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnThreadSaveMessage   (WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnThreadSendMessage   (WPARAM wParam, LPARAM lParam);

		afx_msg void OnMsgBtnNewClick();
		afx_msg void OnMsgBtnForwardClick();
		afx_msg void OnMsgBtnReplyClick();
		afx_msg void OnMsgBtnSendClick();
		afx_msg void OnMsgBtnDeleteClick();
		afx_msg void OnMsgBtnBackupClick();
		afx_msg void OnMsgBtnRestoreClick();
		afx_msg void OnMsgBtnRefreshClick();
		afx_msg void OnHdnEndtrackList(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnHdnDividerDBLClickList(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnSize(UINT nType, int cx, int cy);
	};

} // end of namespace

#endif
