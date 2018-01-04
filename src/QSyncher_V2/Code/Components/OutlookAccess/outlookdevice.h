#ifndef _DEFINE_Outlook_Device_H_
#define _DEFINE_Outlook_Device_H_

#pragma once

#include "..\..\Components\PIMDB\CPIMDB.h"
#include "..\..\Components\PIMDB\PIMObjectbase.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\Components\OutlookAccess\IOutlookCtrl.h"
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"

#ifdef _DEBUG
#pragma comment(lib, "..\\..\\Apps\\Debug\\OutlookAccess.lib")
#else
#pragma comment(lib, "..\\..\\Apps\\release\\OutlookAccess.lib")
#endif

namespace QSYNC_LIB
{	
	const char	OUTLOOK_CONTACTENTRYID[]		 =    "ContactEntryID";
	const char	OUTLOOK_CALENTRYID[]			 =    "CalEntryID";
	const char	OUTLOOK_FOLDERSTOREID[]			 =    "FolderStoreID";
	const char	OUTLOOK_FOLDERENTRYID[]			 =    "FolderEntryID";
	const char	OUTLOOK_FOLDERNAME[]			 =    "FolderName";

	class OutlookDevice : public DeviceBase , public INotify
	{
	public:
		OutlookDevice(const TSTRING& strType):DeviceBase(_T("OutlookDevice")),m_pOutlookCtrl(NULL),m_strType(strType), m_pIPIMConfig(NULL)
		{
			assert(m_strType == KEYWORD_CONTACT || m_strType == KEYWORD_CALENDAR);
			SetDisplayName(_T("Microsoft Outllook"));
		}
		virtual ~OutlookDevice()
		{
			DeleteCtrl();
			m_pIPIMConfig = NULL;
		}

		//interface of IDevice
		virtual bool CanSync() const
		{
			return true;
		}

		virtual bool Sync(SyncType syncType)
		{//Because m_pOutlookCtrl->StartToFetch() will create thread, so sync() can call it directly
			bool bRes = true;
			//0. Initial value
			m_syncType = syncType;			
			switch(m_syncType)
			{
			case OneWayFromClient:
				FetchFromOutlook();
				break;
			case OneWayFromServer:
				FetchFromOutlook();
				break;
			default:
				bRes = false;
				assert(0);//Not define, please check
				break;
			};
			return bRes;
		}

	protected:

		void FetchFromOutlook()
		{
			//1. Delete first
			DeleteCtrl();

			//2. Create new one to make sure is create by m_strType
			if(m_pOutlookCtrl == NULL)
			{
				if(m_strType == KEYWORD_CONTACT || m_strType == KEYWORD_CALENDAR)
					CreateOutlookAccess(m_strType.c_str(), &m_pOutlookCtrl);

				if(m_pOutlookCtrl)
				{
					m_pOutlookCtrl->Register(this,EVENT_CONTACT_FETCHING_FINISH);
					m_pOutlookCtrl->Register(this,EVENT_CALENDAR_FETCHING_FINISH);
					m_pOutlookCtrl->Register(this,EVENT_CONTACT_FETCHING_FAIL);
					m_pOutlookCtrl->Register(this,EVENT_CALENDAR_FETCHING_FAIL);
					m_pOutlookCtrl->Register(this,EVENT_ACCESS_ESTABLISH_FAIL);

					m_pOutlookCtrl->StartToFetch();
				}
			}
		}

		bool Func_OneWayFromClient()
		{//From Outlook to handset
			bool bRes = true;

			int nSize = m_pOutlookCtrl->GetCount();
			for(int i=0;i<nSize && bRes;i++)
			{				
				IPIMObjPt ptTempPIMObj;
				m_pOutlookCtrl->GetByIndex(i,ptTempPIMObj);

				if(ExistInDatabaseAndReplaceGUID(ptTempPIMObj) == false)
				{
#ifdef _DEBUG
					bool bTest = ExistInDatabaseAndReplaceGUID(ptTempPIMObj);
#endif
					bRes = InsertToDefaultDevice(ptTempPIMObj);
				}
				else
				{//Because have full information to judge if the same one	
					IPIMObjPt ptOrgPIMObj;
					if(BeginTransaction(ptTempPIMObj->GetGUID(),ptOrgPIMObj))
					{
						ptOrgPIMObj->Assign(*ptTempPIMObj);
						EndDatabaseTransaction();
					}
					bRes = UpdateToDefaultDevice(ptTempPIMObj);
				}
			}		
			return bRes;
		}

		bool Func_OneWayFromServer_Contact()
		{//From handset to Outlook
			bool bRes = true;


			//逐項上傳資料
			for(int nIndex = 0;nIndex < GetDatabaseCount() && bRes;nIndex++)
			{
				IPIMObjPt ptTempPIMObj;
				if(BeginTransaction(nIndex,ptTempPIMObj))
				{
					if(ExistInDevice(ptTempPIMObj) == false) 
					{
						if(m_pOutlookCtrl)
							bRes = m_pOutlookCtrl->Insert(ptTempPIMObj);
					}
					else
					{//Because have full information to judge if the same one
						if(m_pOutlookCtrl)
							bRes = m_pOutlookCtrl->Update(ptTempPIMObj);
					}
					EndDatabaseTransaction();
				}
				else assert(0);
			}
			return bRes;
		}

		bool Func_OneWayFromServer_Calendar()
		{//From handset to Outlook
			bool bRes = true;

			//從硬碟裡讀出時間限制條件,後面filter需用到
			IConfigure* CfgToolSyncOL = CreateIConfigure(_T("SyncOutlookSetting"));    //建立ConfigTool,以便從檔案讀取資料,用完要刪除
			assert(CfgToolSyncOL);
			CCfgInt nCfgSyncWay = CCfgInt(CfgToolSyncOL,_T("SyncWay"),0);						//表示是從手機寫入OL,或是從OL寫入手機
			CCfgInt bCfgSyncAll = CCfgInt(CfgToolSyncOL,_T("ShowAll"),1);						//是否要全部上傳
			CCfgInt nCfgStartAfterYear = CCfgInt(CfgToolSyncOL,_T("StartAfterYear"),1);		
			CCfgInt nCfgStartAfterMonth = CCfgInt(CfgToolSyncOL,_T("StartAfterMonth"),1);
			CCfgInt nCfgStartAfterDate = CCfgInt(CfgToolSyncOL,_T("StartAfterDate"),1);
			CCfgInt nCfgStartBeforeYear = CCfgInt(CfgToolSyncOL,_T("StartBeforeYear"),3000);
			CCfgInt nCfgStartBeforeMonth = CCfgInt(CfgToolSyncOL,_T("StartBeforeMonth"),12);
			CCfgInt nCfgStartBeforeDate = CCfgInt(CfgToolSyncOL,_T("StartBeforeDate"),31);
			CfgToolSyncOL->Load(STORAGE_INIFILE);//從檔案讀出資料
			COleDateTime FilterTimeStartAfter , FilterTimeStartBefore;
			FilterTimeStartAfter.SetDateTime(nCfgStartAfterYear, nCfgStartAfterMonth, nCfgStartAfterDate, 0, 0, 0);
			FilterTimeStartBefore.SetDateTime(nCfgStartBeforeYear, nCfgStartBeforeMonth, nCfgStartBeforeDate, 23, 59, 59);
			assert(FilterTimeStartAfter <= FilterTimeStartBefore);

			//逐項上傳資料
			for(int nIndex = 0;nIndex < GetDatabaseCount() && bRes;nIndex++)
			{
				IPIMObjPt ptTempPIMObj;
				if(BeginTransaction(nIndex,ptTempPIMObj))
				{
					if(nCfgSyncWay == 0/* 0 means SyncToOutlook,1 means SyncFromOutlook*/ && bCfgSyncAll == false)
						if(!CalendarFitFileter(ptTempPIMObj,FilterTimeStartAfter,FilterTimeStartBefore))//Filter: 判斷是否符合限制條件
						{
							EndDatabaseTransaction();
							continue;
						}
					if(ExistInDevice(ptTempPIMObj) == false)
					{
						if(m_pOutlookCtrl)
							bRes = m_pOutlookCtrl->Insert(ptTempPIMObj);
					}
					else
					{//Because have full information to judge if the same one
						if(m_pOutlookCtrl)
							bRes = m_pOutlookCtrl->Update(ptTempPIMObj);
					}
					EndDatabaseTransaction();
				}
				else assert(0);
			}
			DeleteIConfigure(_T("SyncOutlookSetting"));     //用完ConfigTool後要記得刪除
			return bRes;
		}
		bool CalendarFitFileter(const IPIMObjPt& ptTempPIMObj,const COleDateTime& FilterTimeStartAfter,const COleDateTime& FilterTimeStartBefore)
		{
			//從ptTempPIMObj裡抓出手機上的時間資料
			bool bRes = true;
			TSTRING strDateStart/*,strDateEnd*/;
			GetCmnData(ptTempPIMObj,"VEVENT:DTSTART",strDateStart);
			int nYear, nMonth, nDay, nHour, nMinute, nSecond =0;
			if(16 == strDateStart.size() || 15 == strDateStart.size())
			{
				//拆解字串
				CString CStrDateStart = strDateStart.c_str();
				nYear = _ttoi(CStrDateStart.Mid(0,4));
				nMonth = _ttoi(CStrDateStart.Mid(4,2));
				nDay = _ttoi(CStrDateStart.Mid(6,2));
				nHour = _ttoi(CStrDateStart.Mid(9,2));
				nMinute = _ttoi(CStrDateStart.Mid(11,2));
				nSecond = _ttoi(CStrDateStart.Mid(13,2));
			}
			COleDateTime DataTime;
			DataTime.SetDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond);

			//比較是否符合條件
			if (FilterTimeStartAfter <= DataTime && DataTime <= FilterTimeStartBefore)
				bRes = true;
			else
				bRes = false;
			return bRes;
		}

		//interface of INotify
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(EVENT_CONTACT_FETCHING_FINISH == strEvent)
			{
				bool bSuccess = true;
				if(m_syncType == OneWayFromClient)
				{
					//依據INI檔案得知Outlook資料要下載到SIM卡裡還是手機裡,跑一個迴圈將此資訊寫入PIMObj
					int nSize = m_pOutlookCtrl->GetCount();
					int nPBType = GetSyncTagByConfigTool();
					if (nPBType != ePhonebookSIM && nPBType != ePhonebookME)
						nPBType = ePhonebookME;
					for(int nIndex=0 ; nIndex < nSize ; ++nIndex)
					{				
						IPIMObjPt ptTempPIMObj;
						m_pOutlookCtrl->GetByIndex(nIndex,ptTempPIMObj);
						CommonDataPt ptType = new CommonData("PBTYPE");
						ptType->SetInteger(nPBType);
						ptTempPIMObj->SetByName(ptType->GetName(),ptType);
					}

					bSuccess = Func_OneWayFromClient();
				}
				else if(m_syncType == OneWayFromServer)
					bSuccess = Func_OneWayFromServer_Contact();
				else assert(0);// Not define

				long nFire = bSuccess ? 1 : 0;
				DatabaseFire(g_strNotifyDBFinish, nFire);
			}
			else if(EVENT_CALENDAR_FETCHING_FINISH == strEvent)
			{
				bool bSuccess = true;
				if(m_syncType == OneWayFromClient)
					bSuccess = Func_OneWayFromClient();
				else if(m_syncType == OneWayFromServer)
					bSuccess = Func_OneWayFromServer_Calendar();
				else assert(0);// Not define
				
				long nFire = bSuccess ? 1 : 0;
				DatabaseFire(g_strNotifyDBFinish, nFire);
			}
			else if(EVENT_CONTACT_FETCHING_FAIL == strEvent || EVENT_CALENDAR_FETCHING_FAIL == strEvent)
			{
				DatabaseFire(g_strNotifyDBFinish, 0);
			}
			else if(EVENT_ACCESS_ESTABLISH_FAIL == strEvent)
			{
				DatabaseFire(g_strNotifyDBFinish, 0);
			}
		}

		void DeleteCtrl()
		{
			if(m_pOutlookCtrl)
			{
				m_pOutlookCtrl->Unregister(this,EVENT_CONTACT_FETCHING_FINISH);
				m_pOutlookCtrl->Unregister(this,EVENT_CALENDAR_FETCHING_FINISH);
				m_pOutlookCtrl->Unregister(this,EVENT_CONTACT_FETCHING_FAIL);
				m_pOutlookCtrl->Unregister(this,EVENT_CALENDAR_FETCHING_FAIL);
				m_pOutlookCtrl->Unregister(this,EVENT_ACCESS_ESTABLISH_FAIL);
				DeleteOutlookAccess(m_pOutlookCtrl);
				m_pOutlookCtrl = NULL;
			}
		}

		bool IsOutlookPIM(IPIMObjPt PIMObjPt)
		{
			bool bRes = false;
			CommonDataPt spData = NULL; 
			size_t npos = -1;   
			if( (npos != PIMObjPt->GetByName(OUTLOOK_CONTACTENTRYID, spData) || npos != PIMObjPt->GetByName(OUTLOOK_CALENTRYID, spData)) && 
				npos != PIMObjPt->GetByName(OUTLOOK_FOLDERSTOREID, spData))
			{
				bRes=true;
			}
			return bRes;
		}

		bool IsSame(IPIMObjPt& PIMObjPt1,IPIMObjPt& PIMObjPt2)
		{
			bool bRes = true;
			if(!m_pIPIMConfig)
				m_pIPIMConfig = (IPIMCONFIG*) CommCreateInterface(_T("QSycner"), _T("Phonebook"), eIPIMConfigure);
			vector<TSTRING> vstrLimitation;
			if(m_pIPIMConfig)
			{
				TSTRING strLimitation;
				m_pIPIMConfig->GetProperty(_T("OutlookConfig"), m_strType, strLimitation);
				Tokenize<TSTRING>(_T("|"), strLimitation, vstrLimitation, false);
			}
			if(vstrLimitation.size() == 0)
			{ // default
				if(m_strType == KEYWORD_CONTACT)
				{
					vstrLimitation.push_back(_T("VCARD:N:0"));
					vstrLimitation.push_back(_T("VCARD:N:1"));
				}
				else if(m_strType == KEYWORD_CALENDAR)
				{
					vstrLimitation.push_back(_T("VEVENT:SUMMARY"));
					vstrLimitation.push_back(_T("VEVENT:DTSTART"));
				}
			}
		
			for(vector<TSTRING>::iterator iter = vstrLimitation.begin(); iter!= vstrLimitation.end(); ++iter)
			{
#ifdef _UNICODE
				bRes &= CompareValue(widetoansi((*iter)),PIMObjPt1,PIMObjPt2);
#else
				bRes &= CompareValue((*iter), PIMObjPt1,PIMObjPt2);
#endif
				if(bRes == false)
					break;
			}
			if(bRes) // add OL Location
			{
				if(IsOutlookPIM(PIMObjPt1) && !IsOutlookPIM(PIMObjPt2))
				{
					TSTRING strENTRYID, strFOLDERSTOREID;
					if(m_strType == KEYWORD_CONTACT)
					{
						GetCmnData(PIMObjPt1, OUTLOOK_CONTACTENTRYID, strENTRYID);	
						SetCmnStrData(PIMObjPt2,OUTLOOK_CONTACTENTRYID, strENTRYID.c_str());
					}
					else if(m_strType == KEYWORD_CALENDAR)
					{
						GetCmnData(PIMObjPt1, OUTLOOK_CALENTRYID, strENTRYID);	
						SetCmnStrData(PIMObjPt2,OUTLOOK_CALENTRYID, strENTRYID.c_str());
					}
					GetCmnData(PIMObjPt1, OUTLOOK_FOLDERSTOREID, strFOLDERSTOREID);	
					SetCmnStrData(PIMObjPt2,OUTLOOK_FOLDERSTOREID, strFOLDERSTOREID.c_str());
				}	
				else if(!IsOutlookPIM(PIMObjPt1) && IsOutlookPIM(PIMObjPt2))
				{
					TSTRING strENTRYID, strFOLDERSTOREID;
					if(m_strType == KEYWORD_CONTACT)
					{
						GetCmnData(PIMObjPt2, OUTLOOK_CONTACTENTRYID, strENTRYID);	
						SetCmnStrData(PIMObjPt1,OUTLOOK_CONTACTENTRYID, strENTRYID.c_str());
					}
					else if(m_strType == KEYWORD_CALENDAR)
					{
						GetCmnData(PIMObjPt2, OUTLOOK_CALENTRYID, strENTRYID);	
						SetCmnStrData(PIMObjPt1,OUTLOOK_CALENTRYID, strENTRYID.c_str());
					}
					GetCmnData(PIMObjPt2, OUTLOOK_FOLDERSTOREID, strFOLDERSTOREID);	
					SetCmnStrData(PIMObjPt1,OUTLOOK_FOLDERSTOREID, strFOLDERSTOREID.c_str());
				}
			}
			
			return bRes;
		}
	private:
		bool InsertToDefaultDevice(IPIMObjPt& ptPIMObj)
		{	
			bool bRes = true;
			if(BeginTransaction())
			{
				bRes = InsertItemToDefaultDevice(ptPIMObj);
				EndDatabaseTransaction();
			}
			return bRes;
		}
		bool UpdateToDefaultDevice(IPIMObjPt& ptPIMObj)
		{
			bool bRes = true;
			if(BeginTransaction())
			{
				bRes = UpdateItemToDefaultDevice(ptPIMObj);
				EndDatabaseTransaction();
			}
			return bRes;
		}
		bool ExistInDevice(IPIMObjPt& ptTestPIMObj)
		{
			bool bExist = false;
			int nSize = m_pOutlookCtrl->GetCount();
			for(int i=0;i<nSize;i++)
			{
				IPIMObjPt ptTempPIMObj;
				m_pOutlookCtrl->GetByIndex(i,ptTempPIMObj);
				if(IsSame(ptTestPIMObj,ptTempPIMObj))
				{
					bExist = true;
					break;
				}
			}
			return bExist;
		}

		bool ExistInDatabaseAndReplaceGUID(IPIMObjPt& ptTestPIMObj)
		{					
			bool bExist = false;
			for(int nIndex = 0;nIndex < GetDatabaseCount();nIndex++)
			{
				IPIMObjPt ptTempPIMObj;
				if(BeginTransaction(nIndex,ptTempPIMObj))
				{
					EndDatabaseTransaction();
					if(IsSame(ptTestPIMObj,ptTempPIMObj))
					{
						//If ptTestPIMObj == ptTempPIMObj, it's GUID should be the same
						CommonDataPt spData; 
						//if(ptTempPIMObj->GetByName("VEVENT:UID",spData) > 0)
						//	ptTestPIMObj->SetByName("VEVENT:UID",spData);
						if(ptTempPIMObj->GetByName("LUID",spData) > 0)
							ptTestPIMObj->SetByName("LUID",spData);

						ptTestPIMObj->SetGUID(	ptTempPIMObj->GetGUID());
						bExist = true;
						break;
					}
				}
			}
			return bExist;
		}

		bool CompareValue(const string& strKey,IPIMObjPt& PIMObjPt1,IPIMObjPt& PIMObjPt2)
		{
			bool bRes = true;

			TSTRING strTemp1;
			GetCmnData(PIMObjPt1,strKey.c_str(), strTemp1);
			TSTRING strTemp2;
			GetCmnData(PIMObjPt2,strKey.c_str(), strTemp2);

			if(strKey == "VCARD:N:0" || strKey == "VCARD:N:1")
			{
				CString cstrTemp1 = strTemp1.c_str();
				CString cstrTemp2 = strTemp2.c_str();
				strTemp1 = cstrTemp1.MakeLower();
				strTemp2 = cstrTemp2.MakeLower();
			}
			//return !strTemp1.empty() && !strTemp2.empty() && (strTemp1 == strTemp2);
			return  (strTemp1 == strTemp2);
		}
		bool GetCmnData(IPIMObjPt PIMObjPt,const char* cpKey, TSTRING& strCmnData)
		{
			bool bRet = false;
			CommonDataPt spData = NULL;
			size_t npos = -1;
			size_t nGet = PIMObjPt->GetByName(cpKey, spData);
			if(nGet != npos)
			{
				strCmnData = GetCommonStrData(spData);
				bRet = true;
			}
			else
			{
				strCmnData = _T("");
			}
			return bRet;
		}
		
		TSTRING GetCommonStrData(CommonDataPt spCommonData)
		{
			CommonDataType datatype = spCommonData->GetType();
			TSTRING strRet;
			if(datatype == WString)
			{
#ifdef _UNICODE
				strRet = spCommonData->GetWString();
#endif
			}
			else if(datatype == String)
			{
#ifndef _UNICODE
				strRet = spCommonData->GetString();
#endif
			}
			return strRet;
		}

		bool SetCmnStrData(IPIMObjPt PIMObjPt, const char * szName, const TCHAR* szData)
		{	
			bool bRes = false;
			size_t npos = -1;
			TSTRING strData;
			if(szData)
				strData = szData;

			CommonDataPt sptemData = new CommonData;
			sptemData->SetName(szName);

#ifdef _UNICODE
			sptemData->SetWString(strData.c_str()); 
#else
			sptemData->SetString(strData.c_str()); 
#endif
			bRes = npos != PIMObjPt->SetByName(szName, sptemData);

			return bRes;
		}
		//virtual IPIMDB* GetDatabase(){assert(0);return NULL;/*Should not use the function in OutlookDevice*/}


		virtual int GetDatabaseCount()
		{
			int nRet = 0;
			int nSyncTag = GetSyncTagByConfigTool();
			if(nSyncTag == ePhonebookSIM || nSyncTag == ePhonebookME)
			{
				for(int nIndex = 0;nIndex < GetDatabase()->GetCount() ; ++nIndex)
				{
					IPIMObjPt ptTempPIMObj;
					if(GetDatabase()->BeginTransaction(nIndex,ptTempPIMObj))
					{
						GetDatabase()->EndTransaction();
						CommonDataPt spData;
						size_t nNotFoundIndex = -1;
						if(ptTempPIMObj->GetByName("PBTYPE",spData) != nNotFoundIndex && spData && spData->GetType() == Integer)
						{
							if(spData->GetInteger() == ePhonebookSIM)
								++nRet;
						}
						else assert(0);
					}
					else assert(0);
				}
			}
			else
				nRet = GetDatabase()->GetCount();

			return nRet;
		}
		virtual bool BeginTransaction(unsigned long dwMilliseconds = 0)
		{
			return GetDatabase()->BeginTransaction(dwMilliseconds);
		}
		virtual bool BeginTransaction(const GUID& guid,IPIMObjPt& pimObjPt,unsigned long dwMilliseconds = 0)
		{
			return GetDatabase()->BeginTransaction(guid,pimObjPt,dwMilliseconds);
		}
		virtual int BeginTransaction(const long nFilterIndex,IPIMObjPt& pimObjPt)
		{
			assert(nFilterIndex < GetDatabaseCount());
			long nOriginalIndex = 0; //Filter by SIM or ME
			int nSyncTag = GetSyncTagByConfigTool();
			if(nSyncTag == ePhonebookSIM || nSyncTag == ePhonebookME)
			{
				long nTemp = -1;
				for(nOriginalIndex = 0 ; nOriginalIndex < GetDatabase()->GetCount() && nTemp < nFilterIndex ; ++nOriginalIndex)
				{
					IPIMObjPt ptTempPIMObj;
					if(GetDatabase()->BeginTransaction(nOriginalIndex,ptTempPIMObj))
					{
						GetDatabase()->EndTransaction();
						CommonDataPt spData;
						size_t nNotFoundIndex = -1;
						if(ptTempPIMObj->GetByName("PBTYPE",spData) != nNotFoundIndex && spData && spData->GetType() == Integer)
						{
							if(spData->GetInteger() == nSyncTag)
								++nTemp;
							else;
						}
						else assert(0);
					}
					else assert(0);
				}// end for
				--nOriginalIndex;
			}
			else
				nOriginalIndex = nFilterIndex;

			assert(nOriginalIndex >= 0);
			return GetDatabase()->BeginTransaction(nOriginalIndex,pimObjPt);
		}

		int GetSyncTagByConfigTool()
		{
			int nRet = ePhonebookAll;
			if(m_strType == KEYWORD_CONTACT) 
			{
				IConfigure* CfgToolSyncOL = CreateIConfigure(_T("SyncOutlookSetting"));    //建立ConfigTool,以便從檔案讀取資料,用完要刪除
				assert(CfgToolSyncOL);
				CCfgInt nCfgPhonebookTypeTag = CCfgInt(CfgToolSyncOL, _T("PhonebookTypeTag"), ePhonebookAll);
				CfgToolSyncOL->Load(STORAGE_INIFILE);		//從檔案讀出資料
				if (nCfgPhonebookTypeTag == ePhonebookME || nCfgPhonebookTypeTag == ePhonebookSIM)
					nRet = nCfgPhonebookTypeTag;
			}
			else if (m_strType == KEYWORD_CALENDAR)
			{
				nRet = ePhonebookAll;
			}
			return nRet;
		}
	private:		
		IOutlookCtrl*						m_pOutlookCtrl;
		TSTRING								m_strType;
		SyncType							m_syncType;
		IPIMCONFIG*							m_pIPIMConfig;
	};
}//End for namespace QSYNC_LIB

#endif //_DEFINE_Outlook_Device_H_