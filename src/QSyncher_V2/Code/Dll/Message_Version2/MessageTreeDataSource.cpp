/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageTreeDataSource.cpp  $
 *  $Revision:   1.8  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageTreeDataSource.cpp.-arc  $
   
      Rev 1.8   Jan 16 2008 19:23:18   Eddy Guo
   remove warning
   
      Rev 1.7   Nov 09 2007 13:02:42   Eddy Guo
   add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageTreeDataSource.cpp
    @brief {brief description of MessageTreeDataSource.cpp}

    {detail description of MessageTreeDataSource.cpp}
*/
#include "StdAfx.h"
#include "MessageTreeDataSource.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

CMessageTreeDataSource::CMessageTreeDataSource(void): m_pTreeRoot(NULL)
{
    DefineNotify(EVENT_TREE_DATA_DELETE_NODE);
    DefineNotify(EVENT_TREE_DATA_INSERT_NODE);
    DefineNotify(EVENT_TREE_DATA_UPDATE_NODE);
    DefineNotify(EVENT_TREE_DATA_EXPAND_NODE);
}
 
CMessageTreeDataSource::~CMessageTreeDataSource(void)
{
    RecursiveDelNode(GetRoot(), true);
}
 
 
void CMessageTreeDataSource::Event(const TSTRING& strEvent,long nParam) {
}
 
bool CMessageTreeDataSource::CreateNode(CQTreeNode* pNode) {
    bool bRet = false;
    if(pNode == NULL) {
        ASSERT(0);
        return false;
    }
    if(m_pTreeRoot == NULL && pNode->GetParent() == NULL) { // Root
        bRet = InsertSingleNode(NULL, pNode);
        if(bRet) Fire(EVENT_TREE_DATA_INSERT_NODE, (long)pNode);
    } else if(m_pTreeRoot != NULL && pNode->GetParent() != NULL) { //Normal node
        bRet = InsertSingleNode(const_cast<CQTreeNode*>(pNode->GetParent()), pNode);
        if(bRet) Fire(EVENT_TREE_DATA_INSERT_NODE, (long)pNode);
    }
    ASSERT(bRet);
    return bRet;
}
 
bool CMessageTreeDataSource::InsertSingleNode(CQTreeNode* pParent, CQTreeNode* pNode) {
    bool bRet = false;
    if(pNode == NULL) return false;
    if(pParent == NULL) { //insert root
        if(m_pTreeRoot == NULL) {
            m_pTreeRoot = pNode;
            bRet = true;
        } else {
            ASSERT(0);
            bRet = false;
        }
    } else { //Normal Node
        bRet = true;
    }
    ASSERT(bRet);
    return bRet;
}
 
 
CQTreeNode* CMessageTreeDataSource::GetRoot() {
    return  m_pTreeRoot;
}
 
bool CMessageTreeDataSource::ExpandNode(CQTreeNode* pNode) {
    Fire(EVENT_TREE_DATA_EXPAND_NODE, (long)pNode);
    return false;
}
 
bool CMessageTreeDataSource::UpdateNode(CQTreeNode* pNode) {
    Fire(EVENT_TREE_DATA_UPDATE_NODE, (long)pNode);
    return false;
}
 
void CMessageTreeDataSource::DelAllNode(void) {
    if(m_pTreeRoot) {
        RecursiveDelNode(m_pTreeRoot, false);
    }
    m_pTreeRoot = NULL;
}
bool CMessageTreeDataSource::DeleteNode(CQTreeNode* pNode) {
    Fire(EVENT_TREE_DATA_DELETE_NODE, (long)pNode);
    return RecursiveDelNode(pNode, true);
}
 
 
bool CMessageTreeDataSource::RecursiveDelNode(CQTreeNode* pNode, bool bDelFlag) {
    bool bRet = false;
    if(pNode)
    {
        bool bFlag = false;
        CQTreeNode* pChileNode = NULL;
        UINT nSize = pNode->GetChildSize();
        if(pNode->GetChildSize() > 0) {
            for(UINT i = 0; i < nSize; i++) {
                pChileNode = pNode->GetChild(0);
                bFlag = RecursiveDelNode(pChileNode,bDelFlag);
            } //for loop end
        }
        delete pNode;
        if(pNode == m_pTreeRoot)
            m_pTreeRoot = NULL;
        pNode = NULL;
        bRet = true;
    }
    return bRet;
}

#pragma warning(pop)