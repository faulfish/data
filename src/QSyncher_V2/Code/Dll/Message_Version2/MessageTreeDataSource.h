/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageTreeDataSource.h  $
 *  $Revision:   1.11  $
 *  $Date:   Dec 16 2007 15:48:26  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageTreeDataSource.h.-arc  $
 * 
 *    Rev 1.11   Dec 16 2007 15:48:26   Alan Tu
 * update
 * 
 *    Rev 1.10   Nov 09 2007 13:02:42   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageTreeDataSource.h
    @brief {brief description of MessageTreeDataSource.h}

    {detail description of MessageTreeDataSource.h}
*/
#ifndef __QSYNC_LIB_MESSAGE_TREE_DATA_SOURCE_H__
#define __QSYNC_LIB_MESSAGE_TREE_DATA_SOURCE_H__
 
#pragma once
 
#include "../../CommonUtil/NotifyBase.h"
#include "../../Components/UIFramework/interface/ITreeDatasourceBase.h"
#include "../../Components/UIFramework/interface/IQTreeUI.h"
#include <map>
 
using namespace std;
namespace QSYNC_LIB
{
 
	class CMessageTreeDataSource : public INotify, public CITreeDatasourceBase
	{
	public:
		CMessageTreeDataSource(void);
		virtual ~CMessageTreeDataSource(void);
 
		// µù¥U©M¤Ïµù¥Unotify functiion
		virtual void Event(const TSTRING& strEvent,long nParam);
 
		virtual bool CreateNode(CQTreeNode* pNode);
		virtual CQTreeNode* GetRoot();
		virtual bool ExpandNode(CQTreeNode* pNode);
		virtual bool UpdateNode(CQTreeNode* pNode);
 
	private:
		bool InsertSingleNode(CQTreeNode* pParent, CQTreeNode* pNode);
		void DelAllNode();
		bool DeleteNode(CQTreeNode* pNode);
		bool RecursiveDelNode(CQTreeNode* pNode, bool bDelFlag);
 
	private:
		CQTreeNode* m_pTreeRoot;
        map<UINT, CQTreeNode*> m_mapIDTreeNode;
	};
 
	class CMessageFolderNode :public CQTreeNode
	{
	public:
		CMessageFolderNode(CMessageFolderNode* pParent):
		  CQTreeNode(pParent),
			  m_bCanCreateSubFolder(false)
		  {
		  }
		  virtual ~CMessageFolderNode(void) {
		  }
		  void SetData(CString strName, CString strDisplayText, int nFolderID, LPARAM lparam = 0) {
			  SetName(strName);
			  SetDisplayName(strDisplayText);
			  m_nFolderID = nFolderID;
              SetLPARAM(lparam);
		  }
		  int  m_nFolderID;           // real folder id in mobile
		  bool m_bCanCreateSubFolder; // can create subfolder at this folder or not
		  bool m_bCanDelete;          // can be delete or not
	};
 
}
#endif // __MESSAGE_TREE_DATA_SOURCE_H__
