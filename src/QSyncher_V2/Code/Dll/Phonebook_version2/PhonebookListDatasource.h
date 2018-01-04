/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   PhonebookListDatasource.h  $
 *  $Revision:   1.6  $
 *  $Date:   Feb 13 2008 19:17:32  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhonebookListDatasource.h.-arc  $
 * 
 *    Rev 1.6   Feb 13 2008 19:17:32   Mei Yang
 * change Name to DisplayName 
 * 
 *    Rev 1.5   Dec 10 2007 10:15:46   Mei Yang
 * change IDS
 * 
 *    Rev 1.4   Nov 28 2007 14:45:08   Alan Tu
 * update
 * 
 *    Rev 1.3   Nov 15 2007 16:55:22   Alan Tu
 * edit item from list
 * 
 *    Rev 1.2   Nov 14 2007 18:35:32   Alan Tu
 * update
 * 
 *    Rev 1.1   Nov 14 2007 14:22:00   Alan Tu
 * update
 * 
 *    Rev 1.0   Nov 14 2007 13:47:52   Alan Tu
 * Initial revision.
 *
 *
 ***************************************************************************/
#if !defined(EA_DAD34693_43B2_44e0_A65A_QListTreeDataSource_2007_Feb_12__CQMEFileListDataSource__INCLUDED_)
#define EA_DAD34693_43B2_44e0_A65A_QListTreeDataSource_2007_Feb_12__CQMEFileListDataSource__INCLUDED_

#pragma once

#include "../../Components/UIFramework/interface/IListDataSourceBase.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../../Components/UIFramework/interface/IQTreeDataSource.h"
#include "PhonebookDatasource.h"

namespace QSYNC_LIB
{	
	const LPCTSTR				str_STORAGE_LISTKEY		= _T("IDS_PHONEBOOK_Storage");
	const LPCTSTR				str_NAME_LISTKEY		= _T("IDS_PHONEBOOK_DISPLAY");
	const LPCTSTR				str_PHONE_LISTKEY		= _T("IDS_PHONEBOOK_PRIVATENUMBER");

	class CPhonebookListDatasource:  public INotify, public CIListDatasourceBase
	{
	public:

		CPhonebookListDatasource();
		virtual ~CPhonebookListDatasource();

		//INotify
		virtual void		Event(const TSTRING& strEvent,long nParam);

		//IDataSource
		virtual bool		SortByColumn(const CString& strKey);
		virtual bool		GetOrderInfo(CString& strKey, bool& nOrder);
		virtual bool		UpdateDataByIndex(CQListData* pNewItem, int nRow);

		bool					Clear();
		bool					AddItem(CGUIDTreeNode* pChildNode);

	protected:
		TSTRING		GetPicturePath(const TSTRING& strImage);

	private:
		TSTRING m_strCurrentOrder;
	};//class end

}

#endif//!defined(EA_DAD34693_43B2_44e0_A65A_QListTreeDataSource_2007_Feb_12__CQMEFileListDataSource__INCLUDED_)


