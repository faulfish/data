#ifndef  _DEFINE_QCalendar_Device_H_
#define _DEFINE_QCalendar_Device_H_

#pragma once

#include "..\..\Components\PIMDB\CPIMDB.h"
#include "..\..\Components\PIMDB\PIMObjectbase.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\ICommMobile.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\ICalendar.h"

namespace QSYNC_LIB
{
	class QCalendarDevice : public DeviceBase
	{
		class CalendarKey
		{
		public:
			CalendarKey(){}
			CalendarKey(GUID guid,UINT nID = 0):m_guid(guid),m_nID(nID){}
			~CalendarKey(){}
			bool operator<(const CalendarKey& rhs) const 
			{
				bool bRes = false;
				if(m_guid.Data1 != rhs.m_guid.Data1)
					bRes = m_guid.Data1 < rhs.m_guid.Data1;
				else if(m_guid.Data2 != rhs.m_guid.Data2)
					bRes = m_guid.Data2 < rhs.m_guid.Data2;
				else if(m_guid.Data3 != rhs.m_guid.Data3)
					bRes = m_guid.Data3 < rhs.m_guid.Data3;
				else
				{
					//unsigned char  Data4[ 8 ];
					//for performance issue, it almost to be equal if Data1, Data2 and Data3 are the same
					//bRes = false;
				}
				return bRes;
			}
			bool operator==(const CalendarKey& rhs) const 
			{
				bool bRes = m_guid.Data1 == rhs.m_guid.Data1;
				if(bRes == true)
					bRes = m_guid.Data2 == rhs.m_guid.Data2;
				else if(bRes == true)
					bRes = m_guid.Data3 == rhs.m_guid.Data3;
				else if(bRes == true)
					bRes = m_guid.Data4 == rhs.m_guid.Data4;
				return bRes;
			}
			
			GUID							m_guid;
			UINT							m_nID;
		};

	public:
		QCalendarDevice(ICALENDAR* pICALENDAR,const TSTRING& strDisplayName)
			:DeviceBase(_T("QCalendar"))
			,m_pICALENDAR(pICALENDAR)
		{
			SetDisplayName(strDisplayName);
		}
		virtual ~QCalendarDevice()
		{
			ReleaseInterface();
		}
		virtual bool CanSync() const
		{
			return true;
		}
		virtual bool Sync(SyncType syncType)
		{
			bool bRes = true;
			m_syncType = syncType;
			CreateThread(NULL,0,ThreadProc,this,syncType,NULL);
			return TRUE;
		}
		virtual bool	InsertItem(IPIMObjPt& ptIPIMObj)
		{
			TSTRING strRaw;
			ptIPIMObj->encode(strRaw);
			
			bool bRes = false;
			string strAnsiCode = widetoansi(strRaw,CP_ACP);
			strAnsiCode += "\r\n";
			UINT nRetID = 0;
			if(m_pICALENDAR->WriteByIndex(nRetID,const_cast<char*>(strAnsiCode.c_str()),static_cast<int>(strAnsiCode.size())))
			{
				bRes = (nRetID != -1);
			}

			if(bRes)
			{
				bRes = Internal_InsertItem(ptIPIMObj);				
				CalendarKey key(ptIPIMObj->GetGUID(),nRetID);
				m_CalMappingData.insert(std::pair<CalendarKey,CalendarKey>(key,key));
			}

			return bRes;
		}
		virtual bool RemoveItem(IPIMObjPt& ptIPIMObj)
		{
			return DeletefromMS(ptIPIMObj->GetGUID());
		}
		virtual bool UpdateItem(IPIMObjPt& ptIPIMObj)
		{
			TSTRING strRaw;
			ptIPIMObj->encode(strRaw);
			return UpdatefromMS(ptIPIMObj->GetGUID(),strRaw);
		}
	protected:
		static DWORD WINAPI ThreadProc( LPVOID lpParameter)
		{
			QCalendarDevice* pThis = (QCalendarDevice*)lpParameter;
			switch(pThis->m_syncType)
			{
			case OneWayFromClient:
			case OneWayFromServer:
				break;
			case TwoWay:
				pThis->FetchAllData();
				break;
			case Slow:
				assert(0);
				break;
			default:
				assert(0);
				break;
			};
			return 0;
		}
		void SyncFromChange()
		{
			if(GetDatabase() != NULL)
			{
				//0. Get interface first
				GetInterface();

				//1. Sync status
				int nCount = 0;
				GetDatabase()->GetChangeCount(nCount);
				for(int i=0;i<nCount;i++)
				{
					ChangeLogData logData;
					GetDatabase()->GetChangeByIndex(i,logData); 
					
					TSTRING strRAW;
					switch(logData.m_ChangeLogStatus)
					{
					case ChangeLogStatus_Update:
						assert(0);
						break;
					case ChangeLogStatus_Insert:
						assert(0);
						break;
					case ChangeLogStatus_Delete:
						DeletefromMS(logData.m_Guid);
						break;
					case ChangeLogStatus_Replace:
						assert(0);//Need to implement
						break;
					default:
						assert(0);
						break;
					};
				}
			}
		}
		void FetchAllData()
		{			
			if(GetDatabase() != NULL)
			{
				//Get all data from mobile
				GetDatabase()->BeginTransaction();
				GetDatabase()->SetStatus(Transport);
				
				bool bRes = Extract();

				GetDatabase()->SetStatus(Ready);
				GetDatabase()->EndTransaction();
				GetDatabase()->Fire(g_strNotifyDBFinish,bRes ? 1 : 0);
			}
		}
		bool Extract()
		{
			bool bRes = true;
			if(m_pICALENDAR)
			{
				UINT m_nMinIndex = 0;
				UINT m_nMaxIndex = 0;
				UINT m_nMaxCount = 0;
				std::vector<UINT> vIdxList;
				m_pICALENDAR->GetRange(m_nMinIndex,m_nMaxIndex,m_nMaxCount);
				m_pICALENDAR->ListIndex(&vIdxList);
				for(std::vector<UINT>::iterator Iter = vIdxList.begin();Iter != vIdxList.end();Iter++)
				{
					const int nMaxLength = 4096*2;
					int nRetLen = 0;
					char szBuffer[nMaxLength] = {0};
					if(m_pICALENDAR->ReadByIndex(*Iter,szBuffer,nMaxLength,nRetLen))
					{
#ifdef _UNICODE
						size_t nAnsiLen = strlen(szBuffer);
						wchar_t* wszTemp = new wchar_t[nAnsiLen+1];
						memset(wszTemp,0,(nAnsiLen+1)*sizeof(wchar_t));
						AnsiToUnicodeBuffer(szBuffer,wszTemp,static_cast<int>((nAnsiLen+1)*sizeof(wchar_t)));
						InsertIPIM(wszTemp,*Iter);
						delete [] wszTemp;
#else
						InsertIPIM(szBuffer);
#endif							
					}
					else
					{
						bRes = false;
						break;
					}
				}
			}
			return bRes;
		}
	private:
		bool InsertIPIM(const TCHAR* szPIMRAW,UINT nPBID)
		{
			bool bRes = false;

			IPIMObjPt pimObjPt;
			GetDatabase()->CreatePIMObject(pimObjPt);
			if(pimObjPt->decode(szPIMRAW))
			{
#ifdef _DEBUG
				TSTRING strRaw;
				pimObjPt->encode(strRaw);
#endif
				Internal_InsertItem(pimObjPt,true);
				CalendarKey key(pimObjPt->GetGUID(),nPBID);
				m_CalMappingData.insert(std::pair<CalendarKey,CalendarKey>(key,key));
				bRes = true;
			}
			assert(bRes);

			return bRes;
		}

		bool GetKeyByGuid(const GUID& guid,CalendarKey& key)
		{
			bool bFound = false;
			CalendarKey keytemp(guid);
			std::map<CalendarKey,CalendarKey>::iterator Iter = m_CalMappingData.find(keytemp);
			if(Iter != m_CalMappingData.end())
			{
				key = Iter->second;
				bFound = true;
			}			
			return bFound;
		}

		bool DeletefromMS(const GUID& guid)
		{
			bool bRes = false;
			bool bFound = false;
			CalendarKey key;
			if(GetKeyByGuid(guid,key))
			{
				bRes = (0 != m_pICALENDAR->RemoveByIndex(key.m_nID));
				if(bRes)
					m_CalMappingData.erase(CalendarKey(guid));
			}
			return bRes;
		}

		bool UpdatefromMS(const GUID& guid,const TSTRING& strRaw)
		{		
			bool bRes = false;
			CalendarKey key;
			if(GetKeyByGuid(guid,key))
			{
				string strAnsiCode = widetoansi(strRaw,CP_ACP);
				strAnsiCode += "\r\n";
				UINT nOrgID = key.m_nID;
				UINT nRetID = key.m_nID;
				if(m_pICALENDAR->WriteByIndex(nRetID,const_cast<char*>(strAnsiCode.c_str()),static_cast<int>(strAnsiCode.size())))
				{
					if(nOrgID != nRetID)
					{
						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						/* Tricke
						//Becasue MMI didn't support "Update" function, AT$BQVCALW always create a item
						//Solution is update it and delete old one
						*/

						//1. Delete old one
						bool bTestRes = m_pICALENDAR->RemoveByIndex(nOrgID);
						assert(bTestRes);
						//2. Mapping to new one
						//2.1 Delete one mapping
						CalendarKey keytemp(guid);
						std::map<CalendarKey,CalendarKey>::iterator IterOld = m_CalMappingData.find(keytemp);
						m_CalMappingData.erase(IterOld);						
						//2.2 Insert new mapping
						CalendarKey key(guid,nRetID);
						m_CalMappingData.insert(std::pair<CalendarKey,CalendarKey>(key,key));
						//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						bRes = true;
					}
					else bRes = (nRetID != -1);
				}
			}
			return bRes;
		}

		ICALENDAR* GetInterface()
		{
			assert(m_pICALENDAR);
			return m_pICALENDAR;
		}
		void ReleaseInterface()
		{			
			if(m_pICALENDAR)
			{
				CommDeleteInterface(m_pICALENDAR);
				m_pICALENDAR = NULL;
			}
		}

		bool GetPIMInteger(const char *szName,IPIMObjPt& ptPIM,int& nVal)
		{
			bool bRes = false;
			CommonDataPt spData;
			size_t nNotFoundIndex = -1;
			if(ptPIM->GetByName(szName,spData) != nNotFoundIndex && spData)
			{
				bRes = spData->GetType() == Integer;
				if(bRes)
					nVal = spData->GetInteger();
			}
			return bRes;
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
	private:
		ICALENDAR*											m_pICALENDAR;
		std::map<CalendarKey,CalendarKey>	m_CalMappingData;
		SyncType													m_syncType;
	};
}

#endif // _DEFINE_QCalendar_Device_H_