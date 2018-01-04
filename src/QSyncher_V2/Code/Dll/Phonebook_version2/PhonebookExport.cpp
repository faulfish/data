#include "StdAfx.h"
#include "PhonebookExport.h"
#include "../../Components/PIMDB/CPIMDB.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../../Components/PIMDB/PIMRecord.h"
#include "..\..\Components\PIMDB\VCardCtrl.h"

namespace QSYNC_LIB
{
	class NotifyClass : public INotify
	{
	public:
		NotifyClass():m_hEvent(::CreateEvent(NULL,TRUE,FALSE,NULL))
		{
		}
		~NotifyClass()
		{
			::CloseHandle(m_hEvent);
		}
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(strEvent == g_strNotifyDBFinish)
			{
				::SetEvent(m_hEvent);
			}
		}
		HANDLE m_hEvent;
	};

	void GetMobileData(TSTRING& strMobileModel,TSTRING& strIMEI,TSTRING& strDisplayName)
	{
		LPTSTR	szApp = new TCHAR[MAX_RET_STRING_LENGTH];
		LPTSTR	szMobileModel = new TCHAR[MAX_RET_STRING_LENGTH];
		LPTSTR	szIMEI = new TCHAR[MAX_RET_STRING_LENGTH];
		LPTSTR	szDisplayName = new TCHAR[MAX_RET_STRING_LENGTH];

		::ZeroMemory(szApp, MAX_RET_STRING_LENGTH);
		::ZeroMemory(szMobileModel, MAX_RET_STRING_LENGTH);
		::ZeroMemory(szIMEI, MAX_RET_STRING_LENGTH);
		::ZeroMemory(szDisplayName, MAX_RET_STRING_LENGTH);		

		CommGetMobileInfo(szApp, szMobileModel, szIMEI, szDisplayName);
		strMobileModel = szMobileModel;
		strIMEI = szIMEI;
		strDisplayName = szDisplayName;

		delete [] szApp;
		delete [] szMobileModel;
		delete [] szIMEI;
		delete [] szDisplayName;
	}

	bool GetPIMString(const char *szName,IPIMObjPt& ptPIM,TSTRING& strValue)
	{
		bool bRes = false;
		CommonDataPt spData;
		size_t nNotFoundIndex = -1;
		if(ptPIM->GetByName(szName,spData) != nNotFoundIndex && spData)
		{
#ifdef _UNICODE
			bRes = spData->GetType() == WString;
			if(bRes)
				strValue = spData->GetWString();
#else
			bRes = spData->GetType() == String;
			if(bRes)
				strValue = spData->GetString();
#endif
		}
		return bRes;
	}

	bool GetPIMSpecificString(TSTRING& strValue, IPIMObjPt& ptPIM, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount = 0)
	{
		bool bRes = true;
		CVCardCtrl VCardCtrl(ptPIM);
		strValue = VCardCtrl.GetSpecificString(cpKey, strParameter, nPropertyCount, nValueCount);
		return bRes;
	}

	extern "C" PHONEBOOK_DLL_DECLSPEC bool GetPhonebook(std::map<TSTRING,TSTRING>& mapPB)
	{
		bool bRes = false;

		CommConnectionState eConnectionState;
		CommGetConnectionState(eConnectionState);
		if(eCommConnected == eConnectionState)
		{
			IPIMPt ptIPIM = new PIMFactory<CPIMRecord>;	

			TSTRING strMobileModel;
			TSTRING strIMEI;
			TSTRING strDisplayName;
			GetMobileData(strMobileModel,strIMEI,strDisplayName);
			TSTRING strNormalizeDatabaseName = _T("ID:") + strIMEI + _T(",MODULE:") + strMobileModel + _T(",TYPE:vcard");

			IPIMDBPt spIPIMDB;
			if(CreatePIMDB(strNormalizeDatabaseName,ptIPIM,spIPIMDB))
			{
				IDevicePt defaultDevice;
				spIPIMDB->GetDefaultDevice(defaultDevice);
				if(defaultDevice)
				{				
					if(spIPIMDB->GetCount() == 0)
					{
						NotifyClass notify;
						spIPIMDB->Register(&notify,g_strNotifyDBFinish);
						defaultDevice->Sync(TwoWay);

						::WaitForSingleObject(notify.m_hEvent,INFINITE);
						spIPIMDB->Unregister(&notify,g_strNotifyDBFinish);
					}

					bRes = true;
					long nCount = spIPIMDB->GetCount();
					for(int i=0;i<nCount;i++)
					{
						IPIMObjPt pimObjPt;
						if(spIPIMDB->BeginTransaction(i,pimObjPt))
						{
							TSTRING strName1,strName2;
							TSTRING strTel;
							GetPIMString("VCARD:N:0",pimObjPt,strName1);
							GetPIMString("VCARD:N:1",pimObjPt,strName2);
							GetPIMSpecificString(strTel, pimObjPt,"TEL",_T("HOME;VOICE"),0);

							mapPB.insert(std::pair<TSTRING,TSTRING>(strName1 + _T(" ") + strName2,strTel));
							spIPIMDB->EndTransaction();
						}
					}
				}
			}			
		}

		return bRes;
	}	

	extern "C" PHONEBOOK_DLL_DECLSPEC bool PutPhonebook(IPIMObjPts& spIPIMs)
	{
		bool bRes = false;

		CommConnectionState eConnectionState;
		CommGetConnectionState(eConnectionState);
		if(eCommConnected == eConnectionState)
		{
			IPIMPt ptIPIM = new PIMFactory<CPIMRecord>;	

			TSTRING strMobileModel;
			TSTRING strIMEI;
			TSTRING strDisplayName;
			GetMobileData(strMobileModel,strIMEI,strDisplayName);
			TSTRING strNormalizeDatabaseName = _T("ID:") + strIMEI + _T(",MODULE:") + strMobileModel + _T(",TYPE:vcard");

			IPIMDBPt spIPIMDB;
			if(CreatePIMDB(strNormalizeDatabaseName,ptIPIM,spIPIMDB))
			{
				IDevicePt defaultDevice;
				spIPIMDB->GetDefaultDevice(defaultDevice);
				if(defaultDevice)
				{				
					//1. Get orginal items
					if(spIPIMDB->GetCount() == 0)
					{
						NotifyClass notify;
						spIPIMDB->Register(&notify,g_strNotifyDBFinish);
						defaultDevice->Sync(TwoWay);

						::WaitForSingleObject(notify.m_hEvent,INFINITE);
						spIPIMDB->Unregister(&notify,g_strNotifyDBFinish);
					}

					//2.Remove all items first
					bRes = spIPIMDB->RemoveAll();

					//3. Insert all items
					if(bRes)
					{
						for(IPIMObjPts::iterator Iter = spIPIMs.begin();Iter != spIPIMs.end();Iter)
						{
							if(spIPIMDB->InsertItem(*Iter) == false)
							{
								bRes = false;
								break;
							}
						}
					}
				}
			}			
		}

		return bRes;
	}	
}