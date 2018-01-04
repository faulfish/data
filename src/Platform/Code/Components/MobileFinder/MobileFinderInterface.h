#ifndef __MOBILEFINDER_INTERFACE_H__
#define __MOBILEFINDER_INTERFACE_H__

#pragma once
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\ShareLib\loki-0.1.5\include\loki\SmartPtr.h"
#include <dbt.h>

#ifdef _MOBILEFINDERDLL
#define		CDCINTERFACE_DLLEXT	__declspec(dllexport)
#else
#define		CDCINTERFACE_DLLEXT	__declspec(dllimport)
#endif

namespace QSYNC_LIB
{
	using namespace std;
	using namespace Loki;

	const LPCTSTR DEVICE_CONNECT = _T("TNotify_Connect");			//parameter is point of DeviceData
	const LPCTSTR DEVICE_DISCONNECT = _T("TNotify_Disconnect");		//parameter is point of DeviceData

	const LPCTSTR DEVICE_DIAG_CONNECT = _T("TNotify_Diag_Connect");			//parameter is point of DeviceData
	const LPCTSTR DEVICE_DIAG_DISCONNECT = _T("TNotify_Diag_Disconnect");			//parameter is point of DeviceData

	class CDectectDevice;
	class DeviceData
	{
		friend class CDectectDevice;
	public:
		DeviceData(const DWORD& deviceType,const TSTRING& strID)
			:m_deviceType(deviceType)
			,m_hPortEvent(NULL)
		{
			SetID(strID);
		}
		virtual ~DeviceData()
		{
		}	
		const DWORD& GetDeviceType() const {return m_deviceType;}
		const TSTRING ID() const
		{
			TSTRING strID;
			switch(m_deviceType)
			{
			case DBT_DEVTYP_PORT:
				strID = m_strPort;
				break;
			case DBT_DEVTYP_VOLUME:
				strID = m_strDriver;
				break;
			default:
				break;
			};
			return strID;
		}
		const TSTRING& GetExt(){return m_strExtName;}
		//protected:
		TSTRING			m_strExtName;
		TSTRING			m_strPort;
		TSTRING			m_strDriver;	
		DWORD				m_deviceType;	
		TSTRING			m_strDeviceClass;		
	protected:
		void SetID(const TSTRING& strID)
		{
			switch(m_deviceType)
			{
			case DBT_DEVTYP_PORT:
				m_strPort = strID;
				break;
			case DBT_DEVTYP_VOLUME:
				m_strDriver = strID;
				break;
			default:
				break;
			};
		}
		void SetEvent(bool bEnable)
		{
			if(m_hPortEvent == NULL)
				m_hPortEvent = ::CreateEvent(NULL,TRUE,FALSE,ID().c_str());
			if(bEnable)
				::SetEvent(m_hPortEvent);
			else
				::ResetEvent(m_hPortEvent);
		}
	private:
		HANDLE m_hPortEvent;
	};

	typedef SmartPtr<DeviceData> DeviceDataPt;

	interface IDeviceFinder
	{
		virtual bool Register(INotify* pNotify) = 0;
		virtual bool Unregister(INotify* pNotify) = 0;
		virtual void StartMonitor() = 0;
		virtual void StopMonitor() = 0;
		virtual const size_t GetDeviceCount () = 0;
		virtual bool GetDevice(const int nIndex,DeviceDataPt& ptdeviceData) = 0;
		virtual const DeviceDataPt GetDevice(const TSTRING& strID) = 0;
	};

	CDCINTERFACE_DLLEXT IDeviceFinder* GetDeviceFinder();
	CDCINTERFACE_DLLEXT void DeleteDeviceFinder();
}

#endif //__MOBILEFINDER_INTERFACE_H__