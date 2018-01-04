#include "StdAfx.h"
#include "OLContactAccess.h"

namespace QSYNC_LIB
{
COLContactAccess::COLContactAccess(void):m_bAbort(false), m_bFinish(false)
{
	m_pOLContactList.clear();
}

COLContactAccess::~COLContactAccess(void)
{
	if(!m_pOLContactList.empty())
	{
		m_pOLContactList.clear();
	}
}
bool COLContactAccess::New(IPIMObjPt PIMObjPt)
{
	bool bRes = false;
	if(!m_FolderLocation.IsEmpty() && PIMObjPt)
	{
		Outlook::MAPIFolderPtr pFolder = NULL;
		VARIANT varFolderStoreID;
		VariantInit(&varFolderStoreID);
		varFolderStoreID.vt = VT_BSTR;
		varFolderStoreID.bstrVal = m_FolderLocation.GetFolerStoreID();
		if(SUCCEEDED(m_pNameSpace->GetFolderFromID(m_FolderLocation.GetFolderEntryID(), varFolderStoreID, &pFolder)))
		{
			Outlook::_ItemsPtr pItems = NULL;
			if(SUCCEEDED(pFolder->get_Items(&pItems)))
			{
				VARIANT varType;
				VariantInit(&varType);
				varType.vt = VT_BSTR;
				varType.bstrVal = _bstr_t("IPM.Contact");
				LPDISPATCH lpDispatch = NULL;
				if(SUCCEEDED(pItems->Add(varType, &lpDispatch)))
				{
					Outlook::_ContactItemPtr pContact = NULL;
					lpDispatch->QueryInterface(Outlook::IID__ContactItem, (void**)&pContact);
					bRes = SetOLContact(PIMObjPt, pContact) && SUCCEEDED(pContact->Save());
					assert(bRes);
					if(bRes)
					{
						CContactLocation ContactLocation(pFolder,pContact);
						SetWStringData(PIMObjPt, OUTLOOK_CONTACTENTRYID, ContactLocation.GetContactEntryID());
						SetWStringData(PIMObjPt, OUTLOOK_FOLDERSTOREID, ContactLocation.GetFolderStoreID());
						CSingleLock singleLock(&m_CritSection,TRUE);
						m_pOLContactList.push_back(PIMObjPt);
					}
					pContact->Release();
				}
				lpDispatch = NULL;
			}
		}
	}
	return bRes;
}


bool COLContactAccess::Modify(IPIMObjPt PIMObjPt) //doesn't change ContactList
{
	CSingleLock singleLock(&m_CritSection,TRUE);
	bool bRes = false;
	if(PIMObjPt)
	{
		CContactLocation ContactLocation(PIMObjPt);
		if(!ContactLocation.IsEmpty())
		{
			Outlook::MAPIFolderPtr pFolder = NULL;
			VARIANT varFolderStoreID;
			VariantInit(&varFolderStoreID);
			varFolderStoreID.vt = VT_BSTR;
			varFolderStoreID.bstrVal = ContactLocation.GetFolderStoreID();
			LPDISPATCH lpDispatch;
			if(SUCCEEDED(m_pNameSpace->GetItemFromID(ContactLocation.GetContactEntryID(), varFolderStoreID, &lpDispatch)))
			{
				Outlook::_ContactItemPtr pContact = NULL;
				lpDispatch->QueryInterface(Outlook::IID__ContactItem, (void**)&pContact);
				bRes = SetOLContact(PIMObjPt, pContact);
				if(bRes)
					bRes = SUCCEEDED(pContact->Save());
			}
		}
	}
	return bRes;

}
bool COLContactAccess::Delete(IPIMObjPt PIMObjPt)
{
	bool bRes = false;
	if(PIMObjPt)
	{
		CContactLocation ContactLocation(PIMObjPt);
		if(!ContactLocation.IsEmpty())
		{
			VARIANT varFolderStoreID;
			varFolderStoreID.vt = VT_BSTR;
			varFolderStoreID.bstrVal = ContactLocation.GetFolderStoreID();
			
			LPDISPATCH lpDispatch = NULL;
			if(SUCCEEDED(m_pNameSpace->GetItemFromID(ContactLocation.GetContactEntryID(), varFolderStoreID, &lpDispatch)))
			{
				Outlook::_ContactItemPtr pContact = NULL;
				lpDispatch->QueryInterface(Outlook::IID__ContactItem, (void**)&pContact);
				bRes = SUCCEEDED(pContact->Delete()) == TRUE;
				assert(bRes);
				pContact->Release();

				int nIndex = 0;
				bool bSearch = false;
				CSingleLock singleLock(&m_CritSection,TRUE);
				if(SearchInList(&ContactLocation, nIndex) && bRes)
				{
					IterPIMObj iter = m_pOLContactList.begin();
					advance(iter, nIndex);
					m_pOLContactList.erase(iter);
					bSearch = true;
				}
				bRes = bSearch;
			}
			lpDispatch = NULL;
		}
	}
	return bRes;
}
bool COLContactAccess::SearchInList(CContactLocation* pContactLocation, int& nIndex)
{
	bool bRes = false;
	nIndex = -1;
	for(IterPIMObj iter = m_pOLContactList.begin(); iter != m_pOLContactList.end() && !bRes; iter++)
	{
		nIndex++;
		CContactLocation PIMLocation((IPIMObjPt)(*iter));
		if(PIMLocation.IsTheSame(pContactLocation))
		{
			bRes = true;
			break;
		}
	}
	return bRes;
}
bool COLContactAccess::StartToFetch()
{
	bool bRes = true;
	m_bAbort = false;
	m_bFinish = false;
	bRes = GetDefaultContacts();
	m_bFinish = bRes;
	return bRes;
}
bool COLContactAccess::GetDefaultContacts()
{
	bool bRes = false;
	Outlook::MAPIFolderPtr pFolder = NULL;
	if(SUCCEEDED(m_pNameSpace->GetDefaultFolder(Outlook::olFolderContacts, &pFolder)))
	{
		Outlook::_ItemsPtr pItems = NULL;
		m_FolderLocation.Set(pFolder);
		pFolder->get_Items(&pItems);
		long nItemsCount = 0;
		pItems->get_Count(&nItemsCount);
		
		bRes = true;
		for(long nIndex = 1; nIndex < nItemsCount+1 && bRes && !m_bAbort; nIndex++)
		{
			LPDISPATCH lpDispatch = NULL;
			VARIANT varIndex;
			varIndex.vt = VT_I4;
			varIndex.lVal = nIndex;
			pItems->Item(varIndex, &lpDispatch);

			Outlook::_ContactItemPtr pContact = NULL;
			try
			{
				if(lpDispatch)
					lpDispatch->QueryInterface(Outlook::IID__ContactItem, (void**)&pContact);
			}
			catch(...)
			{
				lpDispatch = NULL;
				pContact = NULL;
			}
			if(pContact)
			{					
				CContactLocation ContactLocation(pFolder, pContact);
				bRes = SaveContactToList(&ContactLocation, pContact);
				if(bRes)
					Fire(EVENT_aCONTACT_FETCH_SUCCEEDED, nIndex);
			}
		}
		if(m_bAbort && !m_pOLContactList.empty())
		{		
			CSingleLock singleLock(&m_CritSection,TRUE);
			m_pOLContactList.clear();
			bRes = false;
		}
	}
	return bRes;
}

bool COLContactAccess::SaveContactToList(CContactLocation* pContactLocation,Outlook::_ContactItemPtr pContact)
{
	bool bRes = false;	
	
	IPIMObjPt PIMObjPt = m_ptPIMRecordFactory->CreatePIMObject();
	CVCardCtrl VCardCtrl(PIMObjPt);
	if(VCardCtrl.SetEmptyIPIMObjPt())
	{
		if(GetOLContact(PIMObjPt, pContact))
		{
			SetWStringData(PIMObjPt, OUTLOOK_CONTACTENTRYID, pContactLocation->GetContactEntryID());
			SetWStringData(PIMObjPt, OUTLOOK_FOLDERSTOREID, pContactLocation->GetFolderStoreID());
			CSingleLock singleLock(&m_CritSection,TRUE);
			m_pOLContactList.push_back(PIMObjPt);
			bRes = true;
		}
	}

	return bRes;
}

bool COLContactAccess::Abort()
{
	m_bAbort = true;
	return true;
}

bool COLContactAccess::GetByIndex(int nIndex, IPIMObjPt& PIMObjPt)
{
	bool bRes = false;
	assert(m_bFinish);
	if(m_bFinish)
	{
		if(m_pOLContactList.size() > (UINT)0 && m_pOLContactList.size() > (UINT)nIndex && nIndex > -1)
		{
			PIMObjPt = m_pOLContactList[nIndex];
			bRes = true;
		}
	}
	return bRes;
}
int COLContactAccess::GetCount()
{
	int nCount = (int)m_pOLContactList.size();
	return nCount;
}

bool COLContactAccess::SetOLContact(IPIMObjPt PIMObjPt, Outlook::_ContactItemPtr pContact)
{
	bool bRes = true;
	vector<_bstr_t> vContactItem;
	GetPIMObj(PIMObjPt, vContactItem);

	bool bPut[36];
	bPut[0] = SUCCEEDED(pContact->put_LastName(		vContactItem[0]));
	bPut[1] = SUCCEEDED(pContact->put_FirstName(	vContactItem[1]));
	bPut[2] = SUCCEEDED(pContact->put_MiddleName(	vContactItem[2]));
	bPut[3] = SUCCEEDED(pContact->put_Title(		vContactItem[3]));
	bPut[4] = SUCCEEDED(pContact->put_Suffix(		vContactItem[4]));
	bPut[5] = true;//SUCCEEDED(pContact->put_FullName(		vContactItem[5]));

	bPut[6] = SUCCEEDED(pContact->put_HomeTelephoneNumber(			vContactItem[6]	));
	bPut[7] = SUCCEEDED(pContact->put_BusinessTelephoneNumber(		vContactItem[7]	));
	bPut[8] = SUCCEEDED(pContact->put_MobileTelephoneNumber(		vContactItem[8]	));
	bPut[9] = SUCCEEDED(pContact->put_HomeFaxNumber(				vContactItem[9]	));
	bPut[10] = SUCCEEDED(pContact->put_BusinessFaxNumber(			vContactItem[10]));
	bPut[11] = SUCCEEDED(pContact->put_PrimaryTelephoneNumber(		vContactItem[11]));
	bPut[12] = SUCCEEDED(pContact->put_CompanyMainTelephoneNumber(	vContactItem[12]));
	bPut[13] = SUCCEEDED(pContact->put_Email1Address(				vContactItem[13]));
	bPut[14] = SUCCEEDED(pContact->put_Email2Address(				vContactItem[14]));
	bPut[15] = SUCCEEDED(pContact->put_Email3Address(				vContactItem[15]));
																														   
	bPut[16] = SUCCEEDED(pContact->put_BusinessAddressPostOfficeBox(vContactItem[16]));
	bPut[17] = SUCCEEDED(pContact->put_BusinessAddress(				vContactItem[17]));
	bPut[18] = SUCCEEDED(pContact->put_BusinessAddressStreet(		vContactItem[18]));
	bPut[19] = SUCCEEDED(pContact->put_BusinessAddressCity(			vContactItem[19]));
	bPut[20] = SUCCEEDED(pContact->put_BusinessAddressState(		vContactItem[20]));
	bPut[21] = SUCCEEDED(pContact->put_BusinessAddressPostalCode(	vContactItem[21]));
	bPut[22] = SUCCEEDED(pContact->put_BusinessAddressCountry(		vContactItem[22]));
	
	bPut[23] = SUCCEEDED(pContact->put_HomeAddressPostOfficeBox(vContactItem[23]));
	bPut[24] = SUCCEEDED(pContact->put_HomeAddress(				vContactItem[24]));
	bPut[25] = SUCCEEDED(pContact->put_HomeAddressStreet(		vContactItem[25]));
	bPut[26] = SUCCEEDED(pContact->put_HomeAddressCity(			vContactItem[26]));
	bPut[27] = SUCCEEDED(pContact->put_HomeAddressState(		vContactItem[27]));
	bPut[28] = SUCCEEDED(pContact->put_HomeAddressPostalCode(	vContactItem[28]));
	bPut[29] = SUCCEEDED(pContact->put_HomeAddressCountry(		vContactItem[29]));
																	
	bPut[30] = SUCCEEDED(pContact->put_BusinessHomePage(	vContactItem[30]));
	bPut[31] = SUCCEEDED(pContact->put_PersonalHomePage(	vContactItem[31]));
	bPut[32] = SUCCEEDED(pContact->put_CompanyName(	   		vContactItem[32]));
	bPut[33] = SUCCEEDED(pContact->put_JobTitle(		   	vContactItem[33]));
	bPut[34] = SUCCEEDED(pContact->put_Profession(	   		vContactItem[34]));
																														   
	TSTRING strBDay;
	CVCardCtrl VCtrl(PIMObjPt);
	VCtrl.GetCmnStrData("VCARD:BDAY",strBDay);
	bPut[35] = true;
	if(strBDay!=_T(""))
	{
		strBDay += _T("T000000Z");
		COleDateTime oleDateTime;
		StrToTime(strBDay.c_str(), oleDateTime);
		bPut[35] = SUCCEEDED(pContact->put_Birthday(oleDateTime.m_dt));
	}
	
	for(int i =0 ; i<36 && bRes; i++)
		bRes = bRes && bPut[i];
	return bRes;
}
bool COLContactAccess::GetPIMObj(IPIMObjPt PIMObjPt, vector<_bstr_t>& vContactItem)
{
	bool bRes = true;

	CVCardCtrl VCtrl(PIMObjPt);
	vContactItem.push_back(VCtrl.GetWString("VCARD:N:0").c_str());
	vContactItem.push_back(VCtrl.GetWString("VCARD:N:1").c_str());
	vContactItem.push_back(VCtrl.GetWString("VCARD:N:2").c_str());
	vContactItem.push_back(VCtrl.GetWString("VCARD:N:3").c_str());
	vContactItem.push_back(VCtrl.GetWString("VCARD:N:4").c_str());

	std::wstring strFullName = L"";//GetUnicode(VCtrl.GetOfficialDisplayName());
	vContactItem.push_back(strFullName.c_str());	
	
	vContactItem.push_back(VCtrl.GetSpecificWString("TEL",_T("HOME;VOICE"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("TEL",_T("WORK;VOICE"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("TEL",_T("HOME;CELL"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("TEL",_T("HOME;FAX"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("TEL",_T("WORK;FAX"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("TEL",_T("HOME;PREF"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("TEL",_T("WORK;PREF"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("EMAIL",_T("HOME"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("EMAIL",_T("HOME"),1).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("EMAIL",_T("WORK"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("WORK"),0,0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("WORK"),0,1).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("WORK"),0,2).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("WORK"),0,3).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("WORK"),0,4).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("WORK"),0,5).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("WORK"),0,6).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("HOME"),0,0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("HOME"),0,1).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("HOME"),0,2).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("HOME"),0,3).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("HOME"),0,4).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("HOME"),0,5).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("ADR",_T("HOME"),0,6).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("URL",_T("WORK"),0).c_str());
	vContactItem.push_back(VCtrl.GetSpecificWString("URL",_T("HOME"),0).c_str());
	vContactItem.push_back(VCtrl.GetWString("VCARD:ORG").c_str());
	vContactItem.push_back(VCtrl.GetWString("VCARD:TITLE").c_str());
	vContactItem.push_back(VCtrl.GetWString("VCARD:ROLE").c_str());

	return bRes;
}


bool COLContactAccess::GetOLContact(IPIMObjPt PIMObjPt, Outlook::_ContactItemPtr pContact)
{
	bool bRes = true;
	_bstr_t vContactItem[36];
	bool bGet[36] = {true};
	bGet[0] = SUCCEEDED(pContact->get_LastName(						vContactItem[0].GetAddress()));
	bGet[1] = SUCCEEDED(pContact->get_FirstName(					vContactItem[1].GetAddress()));
	bGet[2] = SUCCEEDED(pContact->get_MiddleName(					vContactItem[2].GetAddress()));
	bGet[3] = SUCCEEDED(pContact->get_Title(						vContactItem[3].GetAddress()));
	bGet[4] = SUCCEEDED(pContact->get_Suffix(						vContactItem[4].GetAddress()));
	bGet[5] = SUCCEEDED(pContact->get_FullName(						vContactItem[5].GetAddress()));
	
	bGet[6] = SUCCEEDED(pContact->get_BusinessAddressPostOfficeBox( vContactItem[6].GetAddress()));
	bGet[7] = SUCCEEDED(pContact->get_BusinessAddress(				vContactItem[7].GetAddress()));
	bGet[8] = SUCCEEDED(pContact->get_BusinessAddressStreet(		vContactItem[8].GetAddress()));
	bGet[9] = SUCCEEDED(pContact->get_BusinessAddressCity(			vContactItem[9].GetAddress()));
	bGet[10] = SUCCEEDED(pContact->get_BusinessAddressState(		vContactItem[10].GetAddress()));
	bGet[11] = SUCCEEDED(pContact->get_BusinessAddressPostalCode(	vContactItem[11].GetAddress()));
	bGet[12] = SUCCEEDED(pContact->get_BusinessAddressCountry(		vContactItem[12].GetAddress()));
	
	bGet[13] = SUCCEEDED(pContact->get_HomeAddressPostOfficeBox(	vContactItem[13].GetAddress()));
	bGet[14] = SUCCEEDED(pContact->get_HomeAddress(					vContactItem[14].GetAddress()));
	bGet[15] = SUCCEEDED(pContact->get_HomeAddressStreet(			vContactItem[15].GetAddress()));
	bGet[16] = SUCCEEDED(pContact->get_HomeAddressCity(				vContactItem[16].GetAddress()));
	bGet[17] = SUCCEEDED(pContact->get_HomeAddressState(			vContactItem[17].GetAddress()));
	bGet[18] = SUCCEEDED(pContact->get_HomeAddressPostalCode(		vContactItem[18].GetAddress()));
	bGet[19] = SUCCEEDED(pContact->get_HomeAddressCountry(			vContactItem[19].GetAddress()));

	bGet[20] = SUCCEEDED(pContact->get_HomeTelephoneNumber(			vContactItem[20].GetAddress()));
	bGet[21] = SUCCEEDED(pContact->get_BusinessTelephoneNumber(		vContactItem[21].GetAddress()));
	bGet[22] = SUCCEEDED(pContact->get_MobileTelephoneNumber(		vContactItem[22].GetAddress()));
	bGet[23] = SUCCEEDED(pContact->get_HomeFaxNumber(				vContactItem[23].GetAddress()));
	bGet[24] = SUCCEEDED(pContact->get_BusinessFaxNumber(			vContactItem[24].GetAddress()));
	bGet[25] = SUCCEEDED(pContact->get_PrimaryTelephoneNumber(		vContactItem[25].GetAddress()));
	bGet[26] = SUCCEEDED(pContact->get_CompanyMainTelephoneNumber(	vContactItem[26].GetAddress()));
	bGet[27] = SUCCEEDED(pContact->get_Email1Address(				vContactItem[27].GetAddress()));
	bGet[28] = SUCCEEDED(pContact->get_Email2Address(				vContactItem[28].GetAddress()));
	bGet[29] = SUCCEEDED(pContact->get_Email3Address(				vContactItem[29].GetAddress()));
	bGet[30] = SUCCEEDED(pContact->get_BusinessHomePage(			vContactItem[30].GetAddress()));
	bGet[31] = SUCCEEDED(pContact->get_PersonalHomePage(			vContactItem[31].GetAddress()));
	bGet[32] = SUCCEEDED(pContact->get_CompanyName(					vContactItem[32].GetAddress()));
	bGet[33] = SUCCEEDED(pContact->get_JobTitle(					vContactItem[33].GetAddress()));
	bGet[34] = SUCCEEDED(pContact->get_Profession(					vContactItem[34].GetAddress()));
	DATE date;
	bGet[35] = SUCCEEDED(pContact->get_Birthday(&date));

	COleDateTime oleDateTime(date);

	TSTRING strLimitation, strFirst, strSecond;
	CVCardCtrl VCtrl(PIMObjPt);
	VCtrl.GetLimitation(_T(KEYWORD_BDAY), strLimitation);
	Split<TSTRING>(_T("-"), strLimitation, strFirst, strSecond);
	COleDateTime oleFirst, oleSecond;
	StrToTime(strFirst.c_str(), oleFirst);
	StrToTime(strSecond.c_str(), oleSecond);

	if(oleDateTime >= oleFirst && oleDateTime <= oleSecond)
	{
#ifdef _UNICODE
		vContactItem[35] = oleDateTime.Format(_T("%Y%m%d"));
#else
		vContactItem[35] = CharToWideChar(oleDateTime.Format(_T("%Y%m%d")), CP_UTF8);
#endif
	}
	else
	{
		vContactItem[35] = L"";
	}

	for(int i =0 ; i<36 && bRes; i++)
		bRes = bRes && bGet[i];
	if(bRes)
		bRes = SetPIMObj(PIMObjPt, vContactItem);
	return bRes;
}

bool COLContactAccess::SetPIMObj(IPIMObjPt PIMObjPt, _bstr_t* vContactItem) //Default: There is only one VCard.
{
	bool bRes = true;
	bool bSet[36];
	CVCardCtrl VCardCtrl(PIMObjPt);
	bSet[0] = VCardCtrl.SetCmnWStrData( "VCARD:N:0", vContactItem[0]);
	bSet[1] = VCardCtrl.SetCmnWStrData( "VCARD:N:1", vContactItem[1]);
	bSet[2] = VCardCtrl.SetCmnWStrData( "VCARD:N:2", vContactItem[2]);
	bSet[3] = VCardCtrl.SetCmnWStrData( "VCARD:N:3", vContactItem[3]);
	bSet[4] = VCardCtrl.SetCmnWStrData( "VCARD:N:4", vContactItem[4]);
	bSet[5] = VCardCtrl.SetCmnWStrData( "VCARD:FN", vContactItem[5]);

	//bSet[6] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:0", vContactItem[6]);
	bSet[6] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:0", L"");
	//bSet[7] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:1", vContactItem[7]);
	bSet[7] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:1",  L"");
	bSet[8] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:2", vContactItem[8]);
	bSet[9] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:3", vContactItem[9]);
	bSet[10] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:4", vContactItem[10]);
	bSet[11] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:5", vContactItem[11]);
	bSet[12] = VCardCtrl.SetCmnWStrData( "VCARD:ADR:6", vContactItem[12]);

	//bSet[13] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:0", vContactItem[13]);
	bSet[13] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:0", L"");
	//bSet[14] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:1", vContactItem[14]);
	bSet[14] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:1", L"");
	bSet[15] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:2", vContactItem[15]);
	bSet[16] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:3", vContactItem[16]);
	bSet[17] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:4", vContactItem[17]);
	bSet[18] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:5", vContactItem[18]);
	bSet[19] = VCardCtrl.SetCmnWStrData( "VCARD:ADR.1:6", vContactItem[19]);

	bSet[20] = VCardCtrl.SetCmnWStrData( "VCARD:TEL", vContactItem[20]);
	bSet[21] = VCardCtrl.SetCmnWStrData( "VCARD:TEL.1", vContactItem[21]);
	bSet[22] = VCardCtrl.SetCmnWStrData( "VCARD:TEL.2", vContactItem[22]);
	bSet[23] = VCardCtrl.SetCmnWStrData( "VCARD:TEL.4", vContactItem[23]);
	bSet[24] = VCardCtrl.SetCmnWStrData( "VCARD:TEL.5", vContactItem[24]);
	bSet[25] = VCardCtrl.SetCmnWStrData( "VCARD:TEL.6", vContactItem[25]);
	bSet[26] = VCardCtrl.SetCmnWStrData( "VCARD:TEL.7", vContactItem[26]);
	bSet[27] = VCardCtrl.SetCmnWStrData( "VCARD:EMAIL", vContactItem[27]);
	bSet[28] = VCardCtrl.SetCmnWStrData( "VCARD:EMAIL.1", vContactItem[28]);
	bSet[29] = VCardCtrl.SetCmnWStrData( "VCARD:EMAIL.2", vContactItem[29]);
	bSet[30] = VCardCtrl.SetCmnWStrData( "VCARD:URL", vContactItem[30]);
	bSet[31] = VCardCtrl.SetCmnWStrData( "VCARD:URL.1", vContactItem[31]);
	bSet[32] = VCardCtrl.SetCmnWStrData( "VCARD:ORG", vContactItem[32]);
	bSet[33] = VCardCtrl.SetCmnWStrData( "VCARD:TITLE", vContactItem[33]);
	bSet[34] = VCardCtrl.SetCmnWStrData( "VCARD:ROLE", vContactItem[34]);
	bSet[35] = VCardCtrl.SetCmnWStrData( "VCARD:BDAY", vContactItem[35]);

	for(int i = 0 ; i<36 && bRes; i++)
		bRes = bRes && bSet[i];
	return bRes;
}

}//namespace QSYNC_LIB