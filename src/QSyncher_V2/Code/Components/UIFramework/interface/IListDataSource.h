///////////////////////////////////////////////////////////
//  IListDataSource.h
//  Implementation of the Interface IListDataSource
//  Created on:      08-Feb-2007 08:14:57
//  Original author: Evelyn Chen
///////////////////////////////////////////////////////////

#if !defined(BENQ_2D565BC3_B5B7_41a0_90EB_216B787A7DE1__INCLUDED_)
#define BENQ_2D565BC3_B5B7_41a0_90EB_216B787A7DE1__INCLUDED_
#include "IQListCtrlUI.h"

#ifndef IQListDataSource
#define IQListDataSource IDataSource
#endif

namespace QSYNC_LIB
{
	interface IDataSource
	{
		virtual bool InsertDataByIndex(CQListData* item, int nRow = -1) =0;
		virtual bool UpdateDataByIndex(CQListData* newItem, int nRow) =0;
		virtual bool DeleteDataByIndex(int nRow) =0;
		virtual int  GetColumnCount() =0;
		virtual int  GetRowCount() =0;
		virtual bool GetColumnByIndex(int nIndex, ColumnData* pColumnData) =0;
		virtual bool InsertColumn(ColumnData* pColumnData, int nRow = -1) =0;
		virtual bool DeleteColumnByIndex(int nIndex) =0; 
		virtual bool GetDataByIndex(int nIndex, CQListData** item) =0; 
		virtual bool GetColumnByKey(const CString& strKey, ColumnData* pColumnData) =0;
		virtual bool DeleteColumnByKey(const CString& strKey) =0;
		virtual bool UpdateColumnByIndex(int nIndex, ColumnData* pColumnData) =0;
		virtual bool UpdateColumnByKey(const CString& strKey, const CString& vlaue) =0;
		virtual bool SortByColumn(const CString& strKey) =0;
		virtual bool GetOrderInfo(CString& strKey, bool& nOrder) = 0;
		virtual bool Register(INotify* pNotify, const TSTRING& strEvent) =0;
		virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent) =0;
		virtual bool Unregister(INotify* pNotify, const TSTRING& strEvent) =0;
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent) =0;
		virtual void RefreshClear() = 0;

	};
}
#endif // !defined(BENQ_2D565BC3_B5B7_41a0_90EB_216B787A7DE1__INCLUDED_)
