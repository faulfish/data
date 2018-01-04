#ifndef _DEFINE_SyncML_Device_H_
#define _DEFINE_SyncML_Device_H_

#pragma once

#include <Winbase.h>
#include "..\..\Components\PIMDB\CPIMDB.h"
#include "..\..\Components\PIMDB\SyncDevice.h"
#include "..\..\Components\PIMDB\PIMObjectbase.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\Components\SyncML\SyncMLSession.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\ICommMobile.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\ISYNCML.h"

namespace QSYNC_LIB
{
	class SyncMLDevice : public DeviceBase , public ISyncMLCallback
	{
	public:		
		SyncMLDevice(const TSTRING& strType,ISYNCML* pISyncML,const TSTRING& strDisplayName)
			:DeviceBase(Name())
			,m_strType(strType)
			,m_pISyncML(pISyncML)
		{
			SetDisplayName(strDisplayName);
		}
		virtual ~SyncMLDevice()
		{
			if(m_pISyncML)
			{
				CommDeleteInterface(m_pISyncML);
				m_pISyncML = NULL;
			}
		}
		virtual bool CanSync() const
		{
			bool bRes = true;//for test only
			return bRes;
		}
		virtual bool Sync(SyncType syncType)
		{
			bool bRes = true;
			m_syncType = syncType;
			CreateThread(NULL,0,ThreadProc,this,NULL,NULL);
			return TRUE;
		}
		static TSTRING Name() {return _T("SyncMLDevice");}
	protected:
		static DWORD WINAPI ThreadProc( LPVOID lpParameter)
		{
			SyncMLDevice* pThis = (SyncMLDevice*)lpParameter;
			if(pThis)
			{
				IDevice* pIDevice = static_cast<IDevice*>(pThis);
				ISyncMLCallback* pISyncMLCallback = static_cast<ISyncMLCallback*>(pThis);
				if(pIDevice->GetDatabase() != NULL)
				{
					pThis->GetDatabase()->BeginTransaction();
					pThis->GetDatabase()->SetStatus(Transport);
				
					ISYNCML* pISyncML = pThis->m_pISyncML;
					ASSERT(pISyncML);
					if(pISyncML)
					{
						int nSyncType = 206;
						switch(pThis->m_syncType)
						{
						case TwoWay:nSyncType = 200;break;
						case Slow:nSyncType = 201;break;
						case OneWayFromClient:nSyncType = 202;break;
						case RefreshFromClient:nSyncType = 203;break;
						case OneWayFromServer:nSyncType = 204;break;
						case RefreshFromServer:nSyncType = 205;break;
						case TwoWayByServer:nSyncType = 206;break;
						case OneWayFromClientByServer:nSyncType = 207;break;
						case RefreshFromClientByServer:nSyncType = 208;break;
						case OneWayFromServerByServer:nSyncType = 209;break;
						case RefreshFromServerByServer:nSyncType = 210;break;
						default:assert(0);break;
						};

						CSyncMLApplication syncMLApp(pThis->m_strType,nSyncType,pISyncML,pISyncMLCallback,pThis->GetUniqueName().c_str(),pThis->GetDatabase()->GetAnchor(),pThis->GetDatabase()->GetAnchor()+1);
						syncMLApp.Run();

						pThis->GetDatabase()->Save();
					}

					pThis->GetDatabase()->SetStatus(Ready);
					pThis->GetDatabase()->EndTransaction();
					pThis->GetDatabase()->Fire(g_strNotifyDBFinish, 1);				
				}
			}
			return 0;
		}
		//interface ISyncMLCallback
		virtual bool SetCount(int& nCount)
		{
			GetDatabase()->SetCount(nCount);
			return true;
		}
		virtual bool Delete(const TCHAR* strSource)
		{
			bool bRes = true;
			int nLuid = GetSourceID(strSource);
			GUID guid = GetDatabase()->FindGUIDByLUID(nLuid);
			if(guid != GUID_NULL)
				bRes = GetDatabase()->RemoveItemByGUID(guid);
			return bRes;
		}
		virtual bool Replace(const TCHAR* szData,const TCHAR* strSource,const TCHAR* strType)
		{
			bool bRes = false;

			int nLuid = GetSourceID(strSource);
			GUID guid = GetDatabase()->FindGUIDByLUID(nLuid);
			if(guid != GUID_NULL)
				GetDatabase()->RemoveItemByGUID(guid);
			
			IPIMObjPt pimObjPt;
			GetDatabase()->CreatePIMObject(pimObjPt);
			if(pimObjPt->decode(szData))
			{
				if(guid != GUID_NULL)
					pimObjPt->SetGUID(guid);
				bRes = GetDatabase()->InsertItem(pimObjPt);
			}
			assert(bRes);

			return bRes;
		}
		virtual bool Add(const TCHAR* szData,const TCHAR* strSource,const TCHAR* strType)
		{
			return Replace(szData,strSource,strType);
		}
		virtual int	 GetChangesCount()
		{
			int nCount = 0;
			GetDatabase()->GetChangeCount(nCount);
			return nCount;
		}
		virtual bool GetChangeByIndex(int nIndex,ChangeLogData& syncActionPara)
		{
			return GetDatabase()->GetChangeByIndex(nIndex,syncActionPara);
		}
		virtual void ResetChange(int nIndex)
		{
			GetDatabase()->ResetChange(nIndex);
		}
		virtual void EnableLog(bool bEnable)
		{
			GetDatabase()->EnableLog(bEnable);
		}
		virtual bool GetDataByGUID(const GUID& guid,TSTRING& strRAW)
		{
			IPIMObjPt ptIPIMObj;
			bool bRes = GetDatabase()->GetItemByGUID(guid,ptIPIMObj);
			if(bRes)
				bRes = ptIPIMObj->encode(strRAW);
			return bRes;
		}
		virtual bool SetAnchor(int nLast,int nNext)
		{
			GetDatabase()->SetAnchor(nNext);
			return true;
		}
		virtual int GetAnchor()
		{
			return GetDatabase()->GetAnchor();
		}
		virtual bool Map(const TCHAR* strTarget, const TCHAR* strSource)
		{
			return GetDatabase()->SetLUIDByLUID(GetSourceID(strTarget),GetSourceID(strSource));
		}
		int GetSourceID(const TCHAR* strSource)
		{
			TSTRING strLUID = strSource;
			TSTRING::size_type nIndex = strLUID.find_last_of(_T("\\/"));
			if(nIndex != TSTRING::npos)
				strLUID = strLUID.substr(nIndex+1);
			return ::_ttoi(strLUID.c_str());
		}
	private:
		TSTRING			m_strType;
		SyncType			m_syncType;
		ISYNCML*			m_pISyncML;
	};

}
#endif //#define _DEFINE_SyncML_Device_H_