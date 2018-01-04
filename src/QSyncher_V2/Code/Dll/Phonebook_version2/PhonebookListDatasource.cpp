/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   PhonebookListDatasource.cpp  $
*  $Revision:   1.8  $
*  $Date:   Feb 14 2008 09:46:06  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhonebookListDatasource.cpp.-arc  $
   
      Rev 1.8   Feb 14 2008 09:46:06   Jame Tsai
   add sort for display name
   
      Rev 1.7   Feb 13 2008 19:49:14   Mei Yang
   change node GetName to GetDisplayName
   
      Rev 1.6   Feb 05 2008 18:22:14   Jame Tsai
   Add sort in data list
   
      Rev 1.5   Dec 06 2007 16:43:48   Alan Tu
   update
   
      Rev 1.4   Nov 28 2007 14:45:08   Alan Tu
   update
   
      Rev 1.3   Nov 27 2007 10:31:20   Alan Tu
   update picture path
   
      Rev 1.2   Nov 15 2007 15:02:42   Alan Tu
   update
   
      Rev 1.1   Nov 14 2007 18:35:32   Alan Tu
   update
   
      Rev 1.0   Nov 14 2007 13:47:54   Alan Tu
   Initial revision.
*
*
***************************************************************************/
#include "stdafx.h"
#include "PhonebookListDatasource.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB
{
	CPhonebookListDatasource::CPhonebookListDatasource()
	{
	}

	CPhonebookListDatasource::~CPhonebookListDatasource()
	{
	}
	class SortFunc
	{
	private:
		TSTRING m_Factor;
		bool	m_bDsc;
	public:
		SortFunc ( const TSTRING& _Val,bool	bDsc ) : m_Factor ( _Val ),m_bDsc( bDsc ) {
		}

		bool operator () ( CQListData* elem1, CQListData* elem2 ) const
		{
			bool bRes = false;
			if(m_Factor == strPHONEBOOK_PRIVATENUMBER_LISTKEY)
			{
				int nVal1 = _ttoi(elem1->GetValue(m_Factor)->m_strText.c_str());
				int nVal2 = _ttoi(elem2->GetValue(m_Factor)->m_strText.c_str());
				bRes = m_bDsc ? nVal1 > nVal2 : nVal1 < nVal2;
			}
			else if(m_Factor == strPHONEBOOK_DISPLAY_LISTKEY)
			{
				CString strFirst, strSecond;
				strFirst = elem1->GetValue(strPHONEBOOK_DISPLAY_LISTKEY)->GetTextString().c_str();
				strSecond = elem2->GetValue(strPHONEBOOK_DISPLAY_LISTKEY)->GetTextString().c_str();
				if(m_bDsc)
					bRes = strFirst > strSecond;
				else
					bRes = strFirst < strSecond;
			}
			else if(m_Factor == strPHONEBOOK_NAME_LISTKEY)
			{
				CString strFirst, strSecond;
				strFirst = elem1->GetValue(strPHONEBOOK_NAME_LISTKEY)->GetTextString().c_str();
				strSecond = elem2->GetValue(strPHONEBOOK_NAME_LISTKEY)->GetTextString().c_str();
				if(m_bDsc)
					bRes = strFirst > strSecond;
				else
					bRes = strFirst < strSecond;
			}
			return bRes;
		}
	};
	//INotify
	void CPhonebookListDatasource::Event(const TSTRING& strEvent,long nParam)
	{
	}

	//IDataSource
	bool CPhonebookListDatasource::SortByColumn(const CString& strKey)
	{
		CSingleLock singleLock(&m_CritSection,TRUE);

		m_strCurrentOrder = strKey;		
		sort( m_ListDatas.begin(), m_ListDatas.end(), SortFunc(LPCTSTR(strKey),m_SortType[m_strCurrentOrder]));
		m_SortType[m_strCurrentOrder] = !m_SortType[m_strCurrentOrder];
		Fire(EVENT_LIST_COLUMN_SORTOK, (long)&m_ListDatas);
		return true;
	}

	bool CPhonebookListDatasource::GetOrderInfo(CString& strKey, bool& nOrder)
	{
		return true;
	}

	bool CPhonebookListDatasource::Clear()
	{
		int nRowSize = GetRowCount();
		for(int i = 0; i < nRowSize; i++)
			DeleteDataByIndex(0);
		return true;
	}
	
	bool CPhonebookListDatasource::AddItem(CGUIDTreeNode* pNode)
	{		
		bool bRet = false;
		if(pNode)
		{
			CQListData* pListRowData = new CQListData;
			if(pListRowData)
			{
				pListRowData->SetLPARAM((long)pNode);

				CBasicData basicItemFrom;
				int nPBType = pNode->GetPBType();
				if(nPBType == ePhonebookSIM)
					basicItemFrom.m_strImagePath = GetPicturePath(_T("sim_contact.png"));
				else if(nPBType == ePhonebookME)
					basicItemFrom.m_strImagePath = GetPicturePath(_T("phone_contact.PNG"));

				bRet = pListRowData->SetData(str_STORAGE_LISTKEY, basicItemFrom);
				bRet = pListRowData->SetData(str_NAME_LISTKEY, CBasicData(pNode->GetDisplayName()));
				bRet = pListRowData->SetData(str_PHONE_LISTKEY, CBasicData(pNode->GetPhone()));
				bRet = InsertDataByIndex( pListRowData);
			}
		}
		return bRet;
	}

	TSTRING CPhonebookListDatasource::GetPicturePath(const TSTRING& strImage)
	{
		return _T("PhoneBook\\") + strImage;
	}

	bool CPhonebookListDatasource::UpdateDataByIndex(CQListData* pNewItem, int nRow)
	{
		return CIListDatasourceBase::UpdateDataByIndex(pNewItem,nRow);
	}
}

#pragma warning(pop)