/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageListDataSource.cpp  $
 *  $Revision:   1.12  $
 *  $Date:   Jan 16 2008 19:23:18  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageListDataSource.cpp.-arc  $
   
      Rev 1.12   Jan 16 2008 19:23:18   Eddy Guo
   remove warning
   
      Rev 1.11   Nov 09 2007 13:02:42   Eddy Guo
   add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageListDataSource.cpp
    @brief {brief description of MessageListDataSource.cpp}

    {detail description of MessageListDataSource.cpp}
*/
#include "StdAfx.h"
#include "MessageListDataSource.h"
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
CMessageListDataSource::CMessageListDataSource(void)
{
    TRACE("CMessageListDataSource::CMessageListDataSource(void)\n");
}
 
CMessageListDataSource::~CMessageListDataSource(void)
{
}
 
void CMessageListDataSource::Event(const TSTRING& strEvent,long nParam) {
    if(strEvent == EVENT_LIST_UI_HEADER_BUTTON_HIT) {
        SortByColumn(((ColumnData*)nParam)->m_strKey.c_str());
    }
}
 
bool CMessageListDataSource::SortByColumn(const CString& strKey) {
    CSingleLock singleLock(&m_CritSection,TRUE);
    m_strCurrentOrder = strKey;
    sort( m_ListDatas.begin(), m_ListDatas.end(), SortFunc(strKey,m_SortType[m_strCurrentOrder]) );
    m_SortType[m_strCurrentOrder] = !m_SortType[m_strCurrentOrder];
    Fire(EVENT_LIST_COLUMN_SORTOK, (long)&m_ListDatas);
    return true;
}
 
bool CMessageListDataSource::GetAllMessage() {
    return false;
}
 
bool CMessageListDataSource::GetMessagesByFolderID(int nID) {
    return false;
}
 
bool CMessageListDataSource::SyncDataToListCtrl() {
    return false;
}

#pragma warning(pop)