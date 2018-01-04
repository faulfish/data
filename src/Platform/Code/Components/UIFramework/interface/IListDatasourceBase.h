#ifndef _CIDatasourceBase_H_
#define _CIDatasourceBase_H_

#pragma once

#include "../../Components/UIFramework/interface/IListDataSource.h"
#include "../../CommonUtil/NotifyBase.h"

#pragma warning(push)
#pragma warning(disable:4311)

namespace QSYNC_LIB 
{
	template <class ColumnClass = ColumnData,class ListClass = CQListData>
	class CTIListDatasourceBase : public CTNotifyBase<IDataSource>
	{
	protected:
		CTIListDatasourceBase(void):m_nWidth(100)
		{
			DefineNotify(EVENT_LIST_DATA_DELETE_NODE  );
			DefineNotify(EVENT_LIST_DATA_INSERT_NODE  );
			DefineNotify(EVENT_LIST_DATA_UPDATE_NODE  );
			DefineNotify(EVENT_LIST_COLUMN_DELETE_NODE);
			DefineNotify(EVENT_LIST_COLUMN_INSERT_NODE);
			DefineNotify(EVENT_LIST_COLUMN_UPDATE_NODE);
			DefineNotify(EVENT_LIST_COLUMN_SORTOK     );
		}
		virtual ~CTIListDatasourceBase(void)
		{			
			for(ColumnDataVec::iterator treeIter = m_ColumnDatas.begin();treeIter != m_ColumnDatas.end();treeIter++)
			{
				delete *treeIter;
			}		
			for(ListDataVec::iterator listIter = m_ListDatas.begin();listIter != m_ListDatas.end();listIter++)
			{
				delete *listIter;
			}
		}
	public:
		virtual bool Register(INotify* pINotify, const TSTRING& strEvent)
		{
			return CTNotifyBase<IDataSource>::Register(pINotify,strEvent);
		}
		virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent)
		{
			return CTNotifyBase<IDataSource>::Register(pEventFunc,strEvent);
		}
		virtual bool Unregister(INotify* pINotify, const TSTRING& strEvent)
		{
			return CTNotifyBase<IDataSource>::Unregister(pINotify,strEvent);
		}
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent)
		{
			return CTNotifyBase<IDataSource>::Unregister(pEventFunc,strEvent);
		}
		virtual int GetColumnCount(){return static_cast<int>(m_ColumnDatas.size());}
		virtual int GetRowCount(){return static_cast<int>(m_ListDatas.size());}

		virtual bool GetColumnByIndex(int nIndex, ColumnData* pColumnData) 
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(!m_ColumnDatas.empty() ) 
			{
				*pColumnData = *m_ColumnDatas.at(nIndex);
				bRes = true;
			}
			return bRes;
		}

		virtual bool GetColumnByKey(const CString& strKey, ColumnData* pColumnData)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(!m_ColumnDatas.empty() ) 
			{
				ColumnDataVec::iterator Iter;
				for(Iter = m_ColumnDatas.begin();Iter != m_ColumnDatas.end();Iter++)
				{
					if((*Iter)->m_strKey == LPCTSTR(strKey))
					{
						pColumnData->m_strDisplay = (*Iter)->m_strDisplay;
						pColumnData->m_strKey = (*Iter)->m_strKey;
						bRes = true;
						break;
					}
				}
			}
			return bRes;
		}

		virtual bool InsertColumn(ColumnData* pColumnData, int nRow = -1) 
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			if(InsertColumnEx(pColumnData,nRow))
			{
				FireColumnDataParameter columnparam;
				columnparam.pColumnData = pColumnData;
				Fire(EVENT_LIST_COLUMN_INSERT_NODE,(long)&columnparam);
			}
			return true;
		} 

		virtual bool DeleteColumnByIndex(int nIndex)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(DeleteColumnByIndexEx(nIndex))
			{
				FireListDataParameter dataparam;
				dataparam.nRow = nIndex;
				Fire(EVENT_LIST_COLUMN_DELETE_NODE,(long)&dataparam);
				bRes = true;
			}
			return bRes;
		}

		virtual bool DeleteColumnByKey(const CString& strKey)
		{		
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(DeleteColumnByKeyEx(strKey))
			{
				FireColumnDataParameter columnparam;
				columnparam.pColumnData = NULL;
				Fire(EVENT_LIST_COLUMN_DELETE_NODE,(long)&columnparam);
			}
			return bRes;
		}
		
		virtual bool UpdateColumnByIndex(int nIndex, ColumnData* pColumnData)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(nIndex >= 0 && nIndex < GetColumnCount())
			{
				ColumnDataVec::iterator Iter = m_ColumnDatas.begin();	
				advance ( Iter , nIndex );

				m_ColumnDatas.erase(Iter);
				m_ColumnDatas.insert(Iter, pColumnData);

				FireColumnDataParameter columnparam;
				columnparam.pColumnData = pColumnData;
				Fire(EVENT_LIST_COLUMN_UPDATE_NODE,(long)&columnparam);

				bRes = true;
			}
			return bRes;
		}

		virtual bool UpdateColumnByKey(const CString& strKey, const CString& strValue)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			for(ColumnDataVec::iterator Iter = m_ColumnDatas.begin();Iter != m_ColumnDatas.end();Iter++)
			{
				if((*Iter)->m_strKey == LPCTSTR(strKey))
				{
					(*Iter)->m_strDisplay = strValue;
					(*Iter)->m_nWidth = GetColumnWidth();
					FireColumnDataParameter columnparam;
					columnparam.pColumnData = (*Iter);
					Fire(EVENT_LIST_COLUMN_UPDATE_NODE,(long)&columnparam);

					bRes = true;
					break;
				}
			}
			return bRes;
		}

		virtual bool InsertDataByIndex(CQListData* pItem, int nRow = -1) 
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(InsertDataByIndexEx(static_cast<ListClass*>(pItem),nRow)) 
			{
				FireListDataParameter dataparam;
				dataparam.nRow = nRow;
				dataparam.pListData = pItem;
				Fire(EVENT_LIST_DATA_INSERT_NODE,(long)&dataparam);
				bRes = true;
			}
			return bRes;
		}

		virtual bool DeleteDataByIndex(int nRow) 
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(DeleteDataByIndexEx(nRow)) 
			{
				FireListDataParameter dataparam;
				dataparam.nRow = nRow;
				Fire(EVENT_LIST_DATA_DELETE_NODE,(long)&dataparam);
				bRes = true;
			}
			return bRes;
		}

		virtual bool UpdateDataByIndex(CQListData* pNewItem, int nRow) 
		{
			CSingleLock singleLock(&m_CritSection,TRUE);

			bool bRes = false;
			if(nRow >= 0 && nRow < GetRowCount()) 
			{
				ListDataVec::iterator Iter = m_ListDatas.begin(); 
				advance(Iter, nRow);
				CQListData* pOriginalData = *Iter;
				*pOriginalData = *pNewItem;

				FireListDataParameter dataparam;
				dataparam.nRow = nRow;
				dataparam.pListData = pNewItem;
				Fire(EVENT_LIST_DATA_UPDATE_NODE,(long)&dataparam);
				bRes = true;
			}
			return bRes;
		}

		virtual bool GetDataByIndex(int nIndex, CQListData** ppListData) 
		{
			CSingleLock singleLock(&m_CritSection,TRUE);
			return GetDataByIndexEx(nIndex,(ListClass**)(ppListData));
		}

		bool GetDataByIndexEx(int nIndex, ListClass** ppListData) 
		{
			bool bRes= false;
			if(nIndex >= 0 && nIndex < GetRowCount() && ppListData)
			{
				*ppListData = m_ListDatas[nIndex];
				bRes = true;
			}
			return bRes;
		}

		bool InsertDataByIndexEx(ListClass* item, int nRow) 
		{
			if(nRow == -1)
				m_ListDatas.push_back(item);
			else
			{
				ListDataVec::iterator Iter = m_ListDatas.begin();
				advance ( Iter , nRow );
				m_ListDatas.insert(Iter, item);
			}
			return true;
		}

		bool DeleteDataByIndexEx(int nRow) 
		{
			bool bRes = false;
			if(nRow >= 0 && nRow < GetRowCount())
			{
				ListDataVec::iterator Iter = m_ListDatas.begin();
				advance ( Iter , nRow );
				delete *Iter;
				m_ListDatas.erase(Iter);
				bRes = true;
			}
			return bRes;
		}

		bool InsertColumnEx(ColumnClass* pColumnData, int nRow = -1) 
		{
			if(nRow == -1)
				m_ColumnDatas.push_back(pColumnData);
			else
			{
				vector<ColumnClass*>::iterator Iter = m_ColumnDatas.begin();
				advance ( Iter , nRow );
				m_ColumnDatas.insert(Iter, pColumnData);				
			}
			m_SortType.insert(std::pair<TSTRING,bool>((pColumnData->m_strKey),false));
			return true;
		} 

		bool DeleteColumnByKeyEx(const CString& strKey)
		{
			bool bRes = false;
			for(ColumnDataVec::iterator Iter = m_ColumnDatas.begin();Iter != m_ColumnDatas.end();Iter++)
			{
				if((*Iter)->m_strKey == LPCTSTR(strKey))
				{
					SortKeys::iterator sortIter = m_SortType.find(LPCTSTR(strKey));
					if(sortIter != m_SortType.end())
						m_SortType.erase(sortIter);
					m_ColumnDatas.erase(Iter);
					bRes = true;
					break;
				}
			}
			return bRes;
		}

		bool DeleteColumnByIndexEx(int nIndex)
		{
			bool bRes = false;
			if(nIndex >= 0 && nIndex < GetColumnCount())
			{
				ColumnDataVec::iterator Iter = m_ColumnDatas.begin();
				advance ( Iter , nIndex );
				m_ColumnDatas.erase(Iter);
				bRes = true;
			}
			return bRes;
		}

		bool InsertColumnKeyAndName(const CString& strKey,const CString& colName, int nWidth = 100)
		{
			ColumnClass* pColumnData = new ColumnClass;
			pColumnData->m_strKey = strKey;
			pColumnData->m_strDisplay = colName;
			pColumnData->m_nWidth = nWidth;
			return InsertColumn(pColumnData);
		}
		bool SetColumnWidth(int nWidth)
		{
			bool bRes = true;
			m_nWidth = nWidth;
			return bRes;
		}
		int GetColumnWidth(){return m_nWidth;}

		virtual void RefreshClear()
		{
			CSingleLock singleLock(&m_CritSection,TRUE);
			
			for(ListDataVec::iterator listIter = m_ListDatas.begin();listIter != m_ListDatas.end();listIter++)
				delete *listIter;
			m_ListDatas.clear();
		}
	protected:		
		typedef	std::vector<ColumnClass*>	ColumnDataVec;
		typedef	std::vector<ListClass*>		ListDataVec;
		typedef std::map<TSTRING,bool>		SortKeys;

		ColumnDataVec						m_ColumnDatas;
		ListDataVec							m_ListDatas;
		SortKeys							m_SortType;
		int									m_nWidth;

		CCriticalSection					m_CritSection;
	};

	typedef CTIListDatasourceBase<ColumnData,CQListData> CIListDatasourceBase;
}

#pragma warning(pop)
#endif //#ifndef _CIDatasourceBase_H_