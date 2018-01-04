/*$Log:

*/

///////////////////////////////////////////////////////////
//  CQCommonTreeCtrl.h
//  Implementation of the Class CQCommonTreeCtrl
//  Created on:      08-Feb-2007 16:04:51
//  Original author: YC Liang
///////////////////////////////////////////////////////////

#if !defined(EA_7C23E195_8342_40d9_8105_08EA36D90F88__INCLUDED_)
#define EA_7C23E195_8342_40d9_8105_08EA36D90F88__INCLUDED_

#include "IQTreeUI.h"
#include "IQTreeDataSource.h"
#include "../CommUIExportAPI.h"
#include "../../../CommonUtil/NotifyBase.h"

namespace QSYNC_LIB
{
	class COMMONUI_API CQCommonTreeCtrl : public QSYNC_LIB:: CNotifyBase, public QSYNC_LIB::INotify
	{
	public:
		CQCommonTreeCtrl(IQTreeUI* pIQTreeUI,IQTreeDataSource* pITreeDataSource);
		virtual ~CQCommonTreeCtrl();
		//interface INotify,CNotifyBase
		virtual void		Event(const TSTRING& strEvent,long nParam) ;
		virtual bool		Register(EventFunc* pEventFunc, const TSTRING& strEvent);
		virtual bool		Register(INotify* pINotify, const TSTRING& strEvent);
		virtual bool		Unregister(INotify* pINotify, const TSTRING& strEvent);
		virtual bool		Unregister(EventFunc* pEventFunc, const TSTRING& strEvent);
		
		virtual CQTreeNode* GetCurrentNode()
		{
			CQTreeNode* pNode = NULL;
			if(m_pIQTreeUI)
				pNode = m_pIQTreeUI->GetCurrentNode();
			return pNode;
		}
		void RefreshClear()
		{
			if(m_pIQTreeUI)
				m_pIQTreeUI->RefreshClear();
            if(m_pITreeDataSource)
                m_pITreeDataSource->RefreshClear();
		}
	protected:
		bool				AssignInstance(IQTreeUI* pIQTreeUI, IQTreeDataSource* pITreeDataSource);
		bool				InsertNode(CQTreeNode* pNode);
		CQTreeNode*			GetAddr(long nParam);
		void				ExpandUINode(CQTreeNode* pNode);
		void				ExpandDatasourceNode(CQTreeNode* pNode);
	private:		
		IQTreeUI*			m_pIQTreeUI;
		IQTreeDataSource*	m_pITreeDataSource;

	};
}
#endif // !defined(EA_7C23E195_8342_40d9_8105_08EA36D90F88__INCLUDED_)
