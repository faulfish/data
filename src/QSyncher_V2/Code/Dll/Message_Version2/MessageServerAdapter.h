/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageServerAdapter.h  $
 *  $Revision:   1.15  $
 *  $Date:   Mar 03 2008 20:50:30  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageServerAdapter.h.-arc  $
 * 
 *    Rev 1.15   Mar 03 2008 20:50:30   Jame Tsai
 * add function
 * 
 *    Rev 1.14   Jan 23 2008 17:17:52   Eddy Guo
 * update for standard at command
 * 
 *    Rev 1.13   Nov 09 2007 13:02:42   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageServerAdapter.h
    @brief {brief description of MessageServerAdapter.h}

    {detail description of MessageServerAdapter.h}
*/
#ifndef __QSYNC_LIB_MESSAGESERVERADAPTER_H__
#define __QSYNC_LIB_MESSAGESERVERADAPTER_H__
 
#include "../../CommonUtil/NotifyBase.h"
#include "../../Components/SMSServer/ISMSServer.h"
#include "../../Components/UIFramework/interface/IListDataSource.h"
#include "../../Components/UIFramework/interface/IQTreeDataSource.h"
#include "MessageListDataSource.h"
#include "../../Components/UIFramework/interface/IQListCtrlUI.h"
#include "../../Components/UIFramework/Porting/QPortingTreeUI.h"
#include "../../Components/UIFramework/Porting/QListCtrl.h"
#include "../../Components/Localization/ilocalization.h"
#include "MessageTreeDataSource.h"
#include "../../CommLayer/ProtocolCodec/Global_Def.h"
#include "../../Components/Configuration/QConfigure/QMobileConfigure.h"

using namespace QSYNC_LIB;
 
class CMessageServerAdapter : public CINotifyBase
{
public:
    CMessageServerAdapter();
    CMessageServerAdapter(ISMSServer* pSMSServer, IQListDataSource* pListDataSrc, IQTreeDataSource* pTreeDataSrc, CQPortingTreeUI* pTreeCtrl, CQListCtrl* pListCtrl);
    virtual ~CMessageServerAdapter();
 
    virtual bool Register(INotify* pNotify, const TSTRING& strEvent) { return CINotifyBase::Register(pNotify, strEvent); }
    virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent) { return CINotifyBase::Register(pEventFunc, strEvent); }
    virtual bool Unregister(INotify* pNotify, const TSTRING& strEvent) { return CINotifyBase::Unregister(pNotify, strEvent); }
    virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) { return CINotifyBase::Unregister(pEventFunc, strEvent); }
    virtual void Event(const TSTRING& strEvent,long nParam);
    void OnEventListFolderOK(long nParam);
    void OnEventUpdateTreeControl();
    void OnEventSMSServerReadMessage(long nParam);
    void OnEventUpdateTreeControl(long nParam);
    virtual void DefineNotify();
    virtual void AddInstance(ISMSServer* pISMSServer, IQListDataSource* pIListDataSrc, IQTreeDataSource* pITreeDataSrc);
    void SetCurrentFolderID(UINT nID) { m_nCurrentFolderID = nID; }
	UINT GetCurrentFolderID() const {return m_nCurrentFolderID;}
    enumAtCmdSet GetATCommandSet();
 
private:
    void ConvertCShortMessageToListData(CShortMessage* pCSM, CQListData& listData);
    void OnEventSMSServerSaveMessage(long nParam);
    CString GetString(const TSTRING& strID) { return m_pLocalization->GetString(strID).c_str(); }
    void UpdateMessageCountOnTreeCtrl(int nUnread, int nInbox, int nDraft, int nOutbox, int nSent);
    void UpdateTreeNodeString(LPCTSTR strIndexName, LPCTSTR strLocalizationID, CString& strCount);
 
private:
    ILocalization* m_pLocalization;
    IQListDataSource* m_pQListDataSource;
    IQTreeDataSource* m_pQTreeDataSource;
    ISMSServer* m_pSMSServer;
    CQPortingTreeUI* m_pTreeCtrl;
    CQListCtrl* m_pListCtrl;
    UINT m_nCurrentFolderID;
    enumAtCmdSet m_ATCommandSet;
};
 
#endif
