#pragma once
#ifndef _OUTLOOKACCESS_OLCONTACTACCESS_HPP__
#define _OUTLOOKACCESS_OLCONTACTACCESS_HPP__

#include "IOutlookAccess.h"
#include "..\..\Components\PIMDB\VCardCtrl.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include <fstream>
#include <iostream>


namespace QSYNC_LIB
{
	class CContactLocation;

	class COLContactAccess : public COutlookAccess
	{
	public:
		COLContactAccess(void);
		virtual ~COLContactAccess(void);
		virtual bool New(IPIMObjPt PimObjPt);
		virtual bool Modify(IPIMObjPt PimObjPt);
		virtual bool Delete(IPIMObjPt PimObjPt);
		virtual bool StartToFetch();
		virtual bool Abort();
		virtual bool GetByIndex(int nIndex, IPIMObjPt& PIMObjPt);
		virtual int GetCount();

	private:
		bool				m_bAbort;
		bool				m_bFinish;
		CCriticalSection	m_CritSection;
		vector<IPIMObjPt> m_pOLContactList;
		CFolderLocation m_FolderLocation;
	private:
		bool SetOLContact(IPIMObjPt PimObjPt, Outlook::_ContactItemPtr pContact);
		bool GetOLContact(IPIMObjPt PIMObjPt, Outlook::_ContactItemPtr pContact);
		bool SetPIMObj(IPIMObjPt PIMObjPt, _bstr_t* vContactItem); // used by GetOLContact
		bool GetPIMObj(IPIMObjPt PIMObjPt, vector<_bstr_t>& vContactItem); // used by SetOLContact
		bool GetDefaultContacts();
		bool SaveContactToList(CContactLocation* ContactLocation, Outlook::_ContactItemPtr pContact);
		bool SearchInList(CContactLocation* pContactLocation, int& nIndex);	
	};


	class CContactLocation
	{
	public:
		CContactLocation(IPIMObjPt PIMObjPt)
		{
			CommonDataPt sptemData = new CommonData;
			if((size_t)(-1) != PIMObjPt->GetByName(OUTLOOK_CONTACTENTRYID, sptemData))
				m_bstrContactEntryID = sptemData->GetWString();
			if((size_t)(-1) != PIMObjPt->GetByName(OUTLOOK_FOLDERENTRYID, sptemData))
				m_bstrFolderStoreID = sptemData->GetWString();
		}
		CContactLocation(_bstr_t bstrFolderStoreID, _bstr_t bstrContactEntryID):
		m_bstrFolderStoreID(bstrFolderStoreID),m_bstrContactEntryID(bstrContactEntryID){}
		CContactLocation(Outlook::MAPIFolderPtr pFolder,Outlook::_ContactItemPtr pContact)
		{
			pFolder->get_StoreID(m_bstrFolderStoreID.GetAddress());
			pContact->get_EntryID(m_bstrContactEntryID.GetAddress());
		}
		virtual ~CContactLocation(){}

		CContactLocation(CContactLocation* ContactLoc)
		{
			m_bstrFolderStoreID = ContactLoc->GetFolderStoreID();
			m_bstrContactEntryID = ContactLoc->GetContactEntryID();
		}
		bool IsTheSame(CContactLocation* pContactLoc) const
		{
			return (m_bstrFolderStoreID == pContactLoc->GetFolderStoreID())&&(m_bstrContactEntryID == pContactLoc->GetContactEntryID());
		}

		bool SetFolderStoreID(Outlook::MAPIFolderPtr pFolder)
		{
			return (S_OK == pFolder->get_StoreID(m_bstrFolderStoreID.GetAddress()));
		}
		bool SetContactEntryID(Outlook::_ContactItemPtr pContact)
		{
			return (S_OK == pContact->get_EntryID(m_bstrContactEntryID.GetAddress()));
		}
		_bstr_t GetFolderStoreID(){return m_bstrFolderStoreID;}
		_bstr_t GetContactEntryID(){return m_bstrContactEntryID;}
		bool IsEmpty(){return ((m_bstrFolderStoreID == _bstr_t(""))||(m_bstrContactEntryID == _bstr_t(""))); }
	private:
		_bstr_t m_bstrFolderStoreID;
		_bstr_t m_bstrContactEntryID;
	};
}//namespace QSYNC_LIB

#endif //_OUTLOOKACCESS_OLCONTACTACCESS_HPP__