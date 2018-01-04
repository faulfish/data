#ifndef _CITreeDatasourceBase_H_
#define _CITreeDatasourceBase_H_

#pragma once

#include "../../Components/UIFramework/interface/IQTreeDataSource.h"
#include "../../CommonUtil/NotifyBase.h"
#include "../../Components\Localization\Ilocalization.h"

namespace QSYNC_LIB 
{
	class CITreeDatasourceBase : public CTNotifyBase<IQTreeDataSource>
	{
	protected:
		CITreeDatasourceBase(void):m_pRootNode(NULL),m_pILocalization(NULL)
		{			
			DefineNotify(EVENT_TREE_DATA_DELETE_NODE);
			DefineNotify(EVENT_TREE_DATA_INSERT_NODE);
			DefineNotify(EVENT_TREE_DATA_UPDATE_NODE);
			DefineNotify(EVENT_TREE_DATA_EXPAND_NODE);

			CreateILocalization(&m_pILocalization);
		}
		virtual ~CITreeDatasourceBase(void)
		{
			RefreshClear();
		}
	public:
		virtual bool Register(INotify* pINotify, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQTreeDataSource>::Register(pINotify,strEvent);
		}
		virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQTreeDataSource>::Register(pEventFunc,strEvent);
		}
		virtual bool Unregister(INotify* pINotify, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQTreeDataSource>::Unregister(pINotify,strEvent);
		}
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent)
		{
			return CTNotifyBase<IQTreeDataSource>::Unregister(pEventFunc,strEvent);
		}
		virtual CQTreeNode* GetRoot(){return m_pRootNode;}
		void SetRootNode(CQTreeNode* pNode){	m_pRootNode = pNode;}
		TSTRING GetString(const TSTRING& strID)
		{
			TSTRING strRes = strID;
			if(m_pILocalization)
				strRes = m_pILocalization->GetString(strID);
			return strRes;
		}
		virtual void RefreshClear()
		{
			RecursiveDeleteNode(m_pRootNode);
			m_pRootNode = NULL;
		}
	protected:
		bool RecursiveDeleteNode(CQTreeNode* pNode)
		{
			bool bRet = false;
			if(pNode)
			{
				bool bFlag = false;
				while(pNode->GetChildSize() > 0)
				{
					CQTreeNode *pChileNode = pNode->GetChild(0);
					if(pChileNode)
						bFlag = RecursiveDeleteNode(pChileNode);
					else break;
				};//while(vctChildNode.size() > 0)
				delete pNode;
				pNode = NULL;
				bRet = true;
			}
			return bRet;
		}
	private:
		CQTreeNode*							m_pRootNode;
		QSYNC_LIB::ILocalization*		m_pILocalization;
	};
}

#endif //#ifndef _CITreeDatasourceBase_H_