#ifndef _DEFINE_CLIENT_Device_H_
#define _DEFINE_CLIENT_Device_H_

#pragma once

#include "..\..\Components\PIMDB\CPIMDB.h"
#include "..\..\Components\PIMDB\PIMObjectbase.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\CommLayer\MobileInterface\Interface_Include\ICommMobile.h"
#include "..\..\Components\PIMDB\SyncDevice.h"

namespace QSYNC_LIB
{
	template <class CClientDevicePolicy,class IPIMClass>
	class CClientDevice : public DeviceBase , public CINotifyBase
	{
	public:
		CClientDevice(void):DeviceBase(CClientDevicePolicy::DeviceName())
		{
			DefineNotify(g_strNotifyDBInsert);
			DefineNotify(g_strNotifyDBFinish);
			DefineNotify(g_strNotifyDBRemove);
			DefineNotify(g_strNotifyDBUpdate);
			SetDisplayName(CClientDevicePolicy::DeviceDisplayName());
		}
		virtual ~CClientDevice(void)
		{
			Disconnect();
		}

		virtual IPIMDBPt& Connect(const TSTRING& strModule,const TSTRING& strID)
		{
			if(!GetDatabase())
			{
				IPIMPt ptIPIM = new PIMFactory<IPIMClass>;		
				TSTRING strNormalizeDatabaseName = _T("ID:") + strID + _T(",MODULE:") + strModule + _T(",TYPE:") + CClientDevicePolicy::DeviceType();
				IPIMDBPt spIPIMDB;
				if(CreatePIMDB(strNormalizeDatabaseName,ptIPIM,spIPIMDB))
				{
					SetDatabase(spIPIMDB);
					GetDatabase()->Register(this,g_strNotifyDBInsert);
					GetDatabase()->Register(this,g_strNotifyDBFinish);
					GetDatabase()->Register(this,g_strNotifyDBRemove);
					GetDatabase()->Register(this,g_strNotifyDBUpdate);
				}
			}
			return GetDatabase();
		}

		virtual void Disconnect()
		{
			if(GetDatabase())
			{
				GetDatabase()->Unregister(this,g_strNotifyDBInsert);
				GetDatabase()->Unregister(this,g_strNotifyDBFinish);
				GetDatabase()->Unregister(this,g_strNotifyDBRemove);
				GetDatabase()->Unregister(this,g_strNotifyDBUpdate);
			}
		}

		virtual bool CanSync() const
		{
			bool bRes = true;
			return bRes;
		}

		virtual bool Sync(SyncType syncType)
		{
			bool bRes = false;
			if(GetDatabase())
			{
				IDevicePts vectDevices;
				if(GetDatabase()->EnumRemoteDevice(vectDevices))
				{
					for(IDevicePts::const_iterator cIter = vectDevices.begin();cIter != vectDevices.end();cIter++)
					{
						if((*cIter)->GetUniqueName() != CClientDevicePolicy::DeviceName())
						if((*cIter)->CanSync())
						{
							bRes = (*cIter)->Sync(syncType);
							break;
						}
					}
				}
			}
			return bRes;
		}

		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			Fire(strEvent,nParam);
		}
	};

	class VCalendarPolicy
	{
	public:
		static TSTRING DeviceName(){return _T("CalendarDevice");}
		static TSTRING DeviceDisplayName(){return _T("IDS_Local");}
		static TSTRING DeviceType(){return _T("vcalendar");}
	};

	class VCardPolicy
	{
	public:
		static TSTRING DeviceName(){return _T("PhonebookDevice");}
		static TSTRING DeviceDisplayName(){return _T("IDS_Local");}
		static TSTRING DeviceType(){return _T("vcard");}
	};
}


#endif // _DEFINE_CLIENT_Device_H_