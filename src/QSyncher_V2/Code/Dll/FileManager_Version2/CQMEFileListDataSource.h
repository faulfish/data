/*$Log:



*/


///////////////////////////////////////////////////////////
//  CQMEFileListDataSource.h
//  Implementation of the Class CQMEFileListDataSource
//  Created on:      07-Feb-2007 15:14:57
//  Original author: YC Liang
///////////////////////////////////////////////////////////

#if !defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CQMEFileListDataSource__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CQMEFileListDataSource__INCLUDED_


#pragma once

#include "../../Components/UIFramework/interface/IListDataSourceBase.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../../Components/UIFramework/interface/IQTreeDataSource.h"
#include "CMEFileNode.h"

namespace QSYNC_LIB
{
	class CQMEFileListDataSource:  public INotify, public CIListDatasourceBase
	{
	public:

		CQMEFileListDataSource();
		virtual ~CQMEFileListDataSource();

		//IDataSource
		virtual bool SortByColumn(const CString& strKey);
		virtual bool GetOrderInfo(CString& strKey, bool& nOrder);

		bool DisplayFileNodeInfo(CMEFileNode* pNode);
		bool Clear(void);

	private:
		//目前的排序column
		TSTRING m_strCurrentOrder;		
	protected:
		virtual void Event(const TSTRING& strEvent,long nParam);
	};//class end

}

#endif//!defined(EA_DAD34693_43B2_44e0_A65A_QTreeDataSource_2007_Feb_12__CQMEFileListDataSource__INCLUDED_)


