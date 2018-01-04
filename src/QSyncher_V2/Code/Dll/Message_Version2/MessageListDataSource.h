/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageListDataSource.h  $
 *  $Revision:   1.11  $
 *  $Date:   Nov 09 2007 13:02:42  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageListDataSource.h.-arc  $
 * 
 *    Rev 1.11   Nov 09 2007 13:02:42   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageListDataSource.h
    @brief {brief description of MessageListDataSource.h}

    {detail description of MessageListDataSource.h}
*/
#ifndef __QSYNC_LIB_MESSAGELISTDATASOURCE_H__
#define __QSYNC_LIB_MESSAGELISTDATASOURCE_H__
 
#pragma once
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../../Components/UIFramework/interface/IListDatasourceBase.h"
#include "../../Components/UIFramework/interface/IQTreeDataSource.h"
#include "../../CommonUtil/UnicodeUtility.h"
 
 
using namespace QSYNC_LIB;
using namespace std;
 
class CMessageListDataSource : public INotify, public CIListDatasourceBase
{
public:
    CMessageListDataSource(void);
    virtual ~CMessageListDataSource(void);
 
    // 取得指定index的資料
    virtual bool SortByColumn(const CString& strKey);
    virtual bool GetOrderInfo(CString& strKey, bool& bOrder) {
        strKey = m_strCurrentOrder.c_str();
        bOrder = m_SortType[m_strCurrentOrder];
        return true;
    }
 
    bool GetAllMessage();
    bool GetMessagesByFolderID(int nID);
    bool SyncDataToListCtrl();
    void DeleteAllDataSource()  {
        while(m_ListDatas.size())
            this->DeleteDataByIndex(0);
    }
protected:
    virtual void Event(const TSTRING& strEvent,long nParam);
public:
    UINT m_nCurrentMessageID;
 
private:
    TSTRING			m_strCurrentOrder;
};
 
class SortFunc {
private:
    CString m_Factor;
    bool	m_bDsc;
public:
    SortFunc ( const CString& _Val,bool	bDsc ) : m_Factor ( _Val ),m_bDsc( bDsc ) {}
    bool operator () ( CQListData* elem1, CQListData* elem2 ) const
    {
        bool bRes = false;
 
        CString strFirst, strSecond = _T("");
        strFirst = CString(elem1->GetValue((LPCTSTR)m_Factor)->m_strText.c_str());
        strSecond = CString(elem2->GetValue((LPCTSTR)m_Factor)->m_strText.c_str());
        if(m_bDsc)
            bRes = strFirst > strSecond;
        else
            bRes = strFirst < strSecond;
        return bRes;
    }
};
 
#endif
