/*$Log:

*/

///////////////////////////////////////////////////////////
//  CQCommonTreeCtrl.cpp
//  Implementation of the Class CQCommonTreeCtrl
//  Created on:      08-Feb-2007 16:04:51
//  Original author: YC Liang
///////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../interface/CQCommonTreeCtrl.h"
#include "../interface/QControlNotify.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB
{

	CQCommonTreeCtrl::CQCommonTreeCtrl(IQTreeUI* pIQTreeUI,IQTreeDataSource* pITreeDataSource)
		:m_pIQTreeUI(pIQTreeUI),m_pITreeDataSource(pITreeDataSource)
	{
		DefineNotify(EVENT_TREE_INSERT_NODE);	
		DefineNotify(EVENT_TREE_DELETE_NODE);	
		DefineNotify(EVENT_TREE_UPDATE_NODE);
		DefineNotify(EVENT_TREE_RBUTTONDOWN_NODE);
		DefineNotify(EVENT_TREE_SELECT_NODE);
		DefineNotify(EVENT_TREE_UI_DROP);
		DefineNotify(EVENT_DRAG_ENTER);
		DefineNotify(EVENT_DRAG_OVER);
		DefineNotify(EVENT_DRAG_LEAVE);
		DefineNotify(EVENT_TREE_LBUTTONDOWN_NODE);	
		DefineNotify(EVENT_TREE_EXPAND_NODE);
		DefineNotify(EVENT_TREE_DBCLICK_NODE);
		DefineNotify(EVENT_TREE_ITEM_BUTTONDOWN_NODE);

		DefineNotify(EVENT_TREE_DATA_DELETE_NODE);
		DefineNotify(EVENT_TREE_DATA_INSERT_NODE);
		DefineNotify(EVENT_TREE_DATA_UPDATE_NODE);
		DefineNotify(EVENT_TREE_DATA_EXPAND_NODE);		
		DefineNotify(EVENT_TREE_UI_EDIT_BEGIN);
		DefineNotify(EVENT_TREE_UI_EDIT_END);
		DefineNotify(EVENT_TREE_UI_DRAG_BEGIN);

		DefineNotify(EVENT_TREE_UI_KILLFOCUS);
		DefineNotify(EVENT_TREE_UI_SETFOCUS);

		AssignInstance(m_pIQTreeUI,m_pITreeDataSource);
	}


	CQCommonTreeCtrl::~CQCommonTreeCtrl()
	{
		m_pIQTreeUI->Unregister(this, EVENT_TREE_DELETE_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_INSERT_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_UPDATE_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_SELECT_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_EXPAND_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_DBCLICK_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_LBUTTONDOWN_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_RBUTTONDOWN_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_ITEM_BUTTONDOWN_NODE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_UI_DROP);
		m_pIQTreeUI->Unregister(this, EVENT_DRAG_ENTER);
		m_pIQTreeUI->Unregister(this, EVENT_DRAG_OVER);
		m_pIQTreeUI->Unregister(this, EVENT_DRAG_LEAVE);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_UI_EDIT_BEGIN);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_UI_EDIT_END);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_UI_DRAG_BEGIN);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_UI_KILLFOCUS);
		m_pIQTreeUI->Unregister(this, EVENT_TREE_UI_SETFOCUS);

		m_pITreeDataSource->Unregister(this, EVENT_TREE_DATA_DELETE_NODE);
		m_pITreeDataSource->Unregister(this, EVENT_TREE_DATA_INSERT_NODE);
		m_pITreeDataSource->Unregister(this, EVENT_TREE_DATA_UPDATE_NODE);
		m_pITreeDataSource->Unregister(this, EVENT_TREE_DATA_EXPAND_NODE);
	}

	bool CQCommonTreeCtrl::AssignInstance(IQTreeUI* pIQTreeUI, IQTreeDataSource* pITreeDataSource)
	{
		//  Tree UI incoming message
		m_pIQTreeUI->Register(this, EVENT_TREE_DELETE_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_INSERT_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_UPDATE_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_SELECT_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_EXPAND_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_DBCLICK_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_LBUTTONDOWN_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_RBUTTONDOWN_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_ITEM_BUTTONDOWN_NODE);
		m_pIQTreeUI->Register(this, EVENT_TREE_UI_DROP);
		m_pIQTreeUI->Register(this, EVENT_DRAG_ENTER);
		m_pIQTreeUI->Register(this, EVENT_DRAG_OVER);
		m_pIQTreeUI->Register(this, EVENT_DRAG_LEAVE);
		m_pIQTreeUI->Register(this, EVENT_TREE_UI_EDIT_BEGIN);
		m_pIQTreeUI->Register(this, EVENT_TREE_UI_EDIT_END);
		m_pIQTreeUI->Register(this, EVENT_TREE_UI_DRAG_BEGIN);
		m_pIQTreeUI->Register(this, EVENT_TREE_UI_KILLFOCUS);
		m_pIQTreeUI->Register(this, EVENT_TREE_UI_SETFOCUS);

		//  DataSource incoming message
		m_pITreeDataSource->Register(this, EVENT_TREE_DATA_DELETE_NODE);
		m_pITreeDataSource->Register(this, EVENT_TREE_DATA_INSERT_NODE);
		m_pITreeDataSource->Register(this, EVENT_TREE_DATA_UPDATE_NODE);
		m_pITreeDataSource->Register(this, EVENT_TREE_DATA_EXPAND_NODE);

		return true;
	}

	bool CQCommonTreeCtrl::Register(EventFunc* pEventFunc, const TSTRING& strEvent) 
	{
		return CNotifyBase::Register(pEventFunc,strEvent);
	}

	bool CQCommonTreeCtrl::Register(INotify* pINotify, const TSTRING& strEvent) 
	{
		return CNotifyBase::Register(pINotify,strEvent);
	}

	bool CQCommonTreeCtrl::Unregister(INotify* pINotify, const TSTRING& strEvent) 
	{
		return CNotifyBase::Unregister(pINotify,strEvent);
	}

	bool CQCommonTreeCtrl::Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) 
	{
		return CNotifyBase::Unregister(pEventFunc,strEvent);
	}

	void CQCommonTreeCtrl::ExpandUINode(CQTreeNode* pNode)
	{
		if(m_pITreeDataSource != NULL)
			m_pIQTreeUI->ExpandNode(pNode, true);
	}

	void CQCommonTreeCtrl::ExpandDatasourceNode(CQTreeNode* pNode)
	{
		if(m_pITreeDataSource)
			m_pITreeDataSource->ExpandNode(pNode);
	}

	void CQCommonTreeCtrl::Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent == EVENT_TREE_EXPAND_NODE)
		{
			ExpandDatasourceNode(GetAddr(nParam));
		}
		else if(strEvent == EVENT_TREE_ITEM_BUTTONDOWN_NODE)
		{
			ExpandDatasourceNode(GetAddr(nParam));
		}
		else if(strEvent == EVENT_TREE_DATA_DELETE_NODE)
		{
			if(m_pITreeDataSource != NULL && nParam != NULL)
				m_pIQTreeUI->DeleteNode(GetAddr(nParam));
		}
		else if(strEvent == EVENT_TREE_DATA_INSERT_NODE)
		{
			if(m_pITreeDataSource != NULL && nParam != NULL)
			{
				CQTreeNode* pNode = GetAddr(nParam);
				if(pNode)
					m_pIQTreeUI->CreateNode(const_cast<CQTreeNode*>(pNode->GetParent()), GetAddr(nParam));
			}
		}
		else if(strEvent == EVENT_TREE_DATA_UPDATE_NODE)
		{
			if(m_pITreeDataSource != NULL && nParam != NULL)
				m_pIQTreeUI->UpdateNode(GetAddr(nParam));
		}
		else if(strEvent == EVENT_TREE_DATA_EXPAND_NODE)
		{
			ExpandUINode(GetAddr(nParam));
		}
		else Fire(strEvent,nParam);

	}

	CQTreeNode* CQCommonTreeCtrl::GetAddr(long nParam)
	{
		return (CQTreeNode*)nParam;
	}


}

#pragma warning(pop)