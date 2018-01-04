///////////////////////////////////////////////////////////
//  IOutlookAccess.h
//  Implementation of the Interface IOutlookAccess
//  Created on:      20-Aug-2007 15:27:29
///////////////////////////////////////////////////////////

#if !defined(EA_826388C6_224A_42ef_95C6_3BB3D225B964__INCLUDED_)
#define EA_826388C6_224A_42ef_95C6_3BB3D225B964__INCLUDED_
/*************************************************************************
 * [Notice]: Office 2000 uses the 'MSO9.DLL' runtime library, but Office 2003 uses the new
 *           'MSO.DLL' runtime library instead. So,
 *
 *           for {Outlook 2003}, please add the following path
 *           'C:\Program Files\Common Files\Microsoft Shared\OFFICE11'
 *           in <VC++> --- <Tools> --- <Options...> --- <Directories> --- <Lirary files>
 *           to import the "MSO.DLL" when building HMTool.
 *
 *           for {Outlook 2000}, please add the following path
 *           'C:\Program Files\Microsoft Office\Office'
 *           in <VC++> --- <Tools> --- <Options...> --- <Directories> --- <Lirary files>
 *           to import the "MSO9.DLL" when building HMTool.
 *
 *           for other Outlook versions, please find the exact path of the "MSOx.DLL" in
 *           your PC and do the same thing as what I mentioned above.
 *
 *                                                      by  Steven CC Chen
 *************************************************************************/
//#import "C:\Program Files\Common Files\Microsoft Shared\OFFICE11\MSO.DLL" rename_namespace("Office"),named_guids
/*************************************************************************
 * [Notice]: Outlook 2000 uses the 'MSOUTL9.OLB' type library, but Outlook 2003 uses the new
 *           'MSOUTL.OLB' type library instead.
 *                                                      by  Steven CC Chen
 *************************************************************************/
//#import ".\MSOUTL.OLB" rename_namespace("Outlook"), named_guids, raw_interfaces_only, exclude("_IRecipientControl", "_DRecipientControl")
//using namespace Outlook;
#pragma once
#include "mso.tlh"
#include "msoutl.tlh"
#include "..\PIMDB\PIMRecord.h"
#include "..\PIMDB\SyncManager.h"
#include <comutil.h>


namespace QSYNC_LIB
{
	const char	OUTLOOK_CONTACTENTRYID[]		 =    "ContactEntryID";
	const char	OUTLOOK_CALENTRYID[]			 =    "CalEntryID";
	const char	OUTLOOK_FOLDERSTOREID[]			 =    "FolderStoreID";
	const char	OUTLOOK_FOLDERENTRYID[]			 =    "FolderEntryID";
	const char	OUTLOOK_FOLDERNAME[]			 =    "FolderName";
	const LPCTSTR EVENT_aCAL_FETCH_SUCCEEDED	 =	  _T("Fetch one appointment succeeded");
	const LPCTSTR EVENT_aCONTACT_FETCH_SUCCEEDED =	  _T("Fetch one contact succeeded");

	class IOutlookAccess
	{
	public:
		IOutlookAccess(){}
		virtual ~IOutlookAccess(){}

		virtual bool EstablishSession() =0;
		virtual bool DestroySession() =0;
		virtual bool New(IPIMObjPt PimObjPt) =0;
		virtual bool Modify(IPIMObjPt PimObjPt) =0;
		virtual bool Delete(IPIMObjPt PimObjPt) =0;
		virtual bool StartToFetch() =0;
		virtual bool Abort() =0;
		virtual bool GetByIndex(int nIndex, IPIMObjPt& PIMObjPt)=0;
		virtual int GetCount()=0;
		virtual bool Register(INotify* pNotify,const TSTRING& strEvent) = 0;
		virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent) = 0;
	};

	class COutlookAccess : public CTNotifyBase<IOutlookAccess>
	{
		friend class COutlookCtrl;
	public:
		typedef vector<IPIMObjPt>::iterator IterPIMObj;
		Outlook::_NameSpacePtr m_pNameSpace;
		Outlook::_ApplicationPtr m_pApp;
		IPIMPt m_ptPIMRecordFactory;
	public:
		virtual bool EstablishSession()
		{	
			bool bRes = false;
			
			if(SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED)))					
				if(SUCCEEDED(m_pApp.CreateInstance( __uuidof(Outlook::Application) )))			
					if(SUCCEEDED(m_pApp->GetNamespace(_bstr_t("MAPI"), &m_pNameSpace)))
					{						
						bRes = true;
					}
			
			return bRes;
		}
		virtual bool DestroySession()
		{
			
			m_pApp = NULL;
			m_pNameSpace = NULL;
			CoUninitialize();
		
			return true;
		}
	protected:
		COutlookAccess():m_ptPIMRecordFactory(new PIMFactory<CPIMRecord>)
		{
			DefineNotify(EVENT_aCAL_FETCH_SUCCEEDED);
			DefineNotify(EVENT_aCONTACT_FETCH_SUCCEEDED);
		}
		virtual ~COutlookAccess(){}
		void SetWStringData( IPIMObjPt PIMObjPt, const char * szName, const wchar_t* szData)
		{
			CommonDataPt sptemData = new CommonData;
			sptemData->SetName(szName);
			sptemData->SetWString(szData); 
			PIMObjPt->SetByName(szName, sptemData);
		}
		bool GetWStringData( IPIMObjPt PIMObjPt, const char * szName, wstring& szData )
		{
			CommonDataPt sptemData = new CommonData;
			size_t npos = -1;
			size_t nGet = PIMObjPt->GetByName(szName, sptemData);
			bool bRes = false;
			if(nGet != npos)
			{
				const wchar_t* tmp = sptemData->GetWString(); 
				szData = tmp;
				bRes = true;
			}
			return bRes;
		}
	};

	class CFolderLocation
	{		
	public:
		CFolderLocation(){}
		CFolderLocation(_bstr_t bstrFolderStoreID, _bstr_t bstrFolderEntryID, _bstr_t bstrFolderName):
			m_bstrFolderStoreID(bstrFolderStoreID), m_bstrFolderEntryID(bstrFolderEntryID), m_bstrFolderName(bstrFolderName){}
		bool Set(Outlook::MAPIFolderPtr pFolder)
		{
			bool bSuccess1 = pFolder->get_StoreID(m_bstrFolderStoreID.GetAddress()) == S_OK;
			bool bSuccess2 = pFolder->get_EntryID(m_bstrFolderEntryID.GetAddress()) == S_OK;
			bool bSuccess3 = pFolder->get_Name(m_bstrFolderName.GetAddress()) == S_OK;
			return (bSuccess1 && bSuccess2 && bSuccess3);
		}
		_bstr_t GetFolerStoreID(){return m_bstrFolderStoreID;}
		_bstr_t GetFolderEntryID(){return m_bstrFolderEntryID;}
		_bstr_t GetFolderName(){return m_bstrFolderName;}
		bool IsEmpty(){return ((m_bstrFolderStoreID == _bstr_t(""))||(m_bstrFolderEntryID == _bstr_t("")) || (m_bstrFolderName == _bstr_t(""))); }
		virtual ~CFolderLocation(){}
	private:
		_bstr_t m_bstrFolderStoreID;
		_bstr_t m_bstrFolderEntryID;
		_bstr_t m_bstrFolderName;
	};

}//namespace QSYNC_LIB

#endif // !defined(EA_826388C6_224A_42ef_95C6_3BB3D225B964__INCLUDED_)
