#ifndef _DEVICEDETECT_H_
#define _DEVICEDETECT_H_

#pragma once

#include "..\..\CommonUtil\NotifyBase.h"
#include <dbt.h>
#include <map>
#include "..\..\ShareLib\loki-0.1.5\include\loki\SmartPtr.h"
#include "..\..\ShareLib\loki-0.1.5\include\loki\Singleton.h"
#include "..\..\CommonUtil\SerialPort.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include <initguid.h>
#include <winioctl.h>
#include "..\..\ShareLib\WinDDK\inc\wxp\usbioctl.h"
#include <devguid.h>
#include <Setupapi.h>
#include <cfgmgr32.h>
#include <shlwapi.h>
#include "MobileFinderInterface.h"
#include <process.h>
#include "..\..\CommonUtil\Win32ThreadUtility.h"
#include <afxmt.h>

namespace QSYNC_LIB
{
	using namespace std;
	using namespace Loki;

	
	typedef pair <TSTRING,DeviceDataPt>					DeviceMapPair;
	typedef map<TSTRING,DeviceDataPt>::const_iterator	DeviceMapCIter;
	typedef map<TSTRING,DeviceDataPt>::iterator			DeviceMapIter;

	class AutoHKEY
	{
	public:
		AutoHKEY(HKEY hKey,  LPCTSTR lpSubKey,  DWORD ulOptions,  REGSAM samDesired):m_hKey(NULL)
		{
			LONG lRet = ::RegOpenKeyEx( hKey,lpSubKey,ulOptions, samDesired, &m_hKey );	
		}
		~AutoHKEY()
		{
			if(m_hKey)
				::RegCloseKey( m_hKey );
		}
		operator const HKEY(){return m_hKey;}
		TSTRING GetSubKey(int nIndex)
		{
			TSTRING strKey;

			if(m_hKey)
			{
#define MAX_KEY_LENGTH 255
				TCHAR szBuffer[MAX_KEY_LENGTH] = {0};
				DWORD dwBufLen=MAX_KEY_LENGTH*sizeof(TCHAR);
				FILETIME ftLastWriteTime;
				if(::RegEnumKeyEx(m_hKey, nIndex,szBuffer,&dwBufLen,NULL,NULL,NULL,&ftLastWriteTime) == ERROR_SUCCESS)
					strKey = szBuffer;
			}

			return strKey;
		}
		TSTRING GetValue(const TSTRING& strField)
		{
			TSTRING strValue;

			if(m_hKey)
			{
#define MAX_VALUE_LENGTH 16383
				TCHAR szBuffer[MAX_VALUE_LENGTH] = {0};
				DWORD dwBufLen=MAX_VALUE_LENGTH*sizeof(TCHAR);
				LONG lRet = ::RegQueryValueEx( m_hKey, strField.c_str(), NULL, NULL,(LPBYTE) szBuffer, &dwBufLen);
				if( (lRet == ERROR_SUCCESS) && (dwBufLen < MAX_VALUE_LENGTH*sizeof(TCHAR)) )
					strValue = szBuffer;
			}

			return strValue;
		}
	private:
		HKEY m_hKey;
	};
	class CDectectDevice :public CNotifyBase
	{
	public:
		void Start() 
		{

			m_bEnable = true;
			m_DetectThreadEventAbort.ResetEvent();
			m_DetectThreadEventFinish.ResetEvent();
			m_hDetectThread = (HANDLE)_beginthread(DetectThreadEntry,0,(LPVOID)this);
		}

		void Stop()	
		{
			m_bEnable = false;
			DeviceMapIter Iter = m_Devices.begin();
			while (Iter != m_Devices.end())
			{
				RemoveDeviceFromMap(Iter);
				Iter = m_Devices.begin();
			}
            m_DetectThreadEventAbort.SetEvent();
            m_DetectThreadEventFinish.SetEvent();
		}


		const size_t GetDeviceCount() 
		{
			CSingleLock singleLock(static_cast<CSyncObject *>(&m_CritSection),TRUE);
			return m_Devices.size();
		}
		const DeviceDataPt GetDevice(const TSTRING& strID)
		{
			CSingleLock singleLock(static_cast<CSyncObject *>(&m_CritSection),TRUE);
			DeviceDataPt ptDeviceData;
			using namespace std;
			map<TSTRING,DeviceDataPt>::const_iterator cIter = m_Devices.find(strID);
			if(cIter != m_Devices.end())
				ptDeviceData = cIter->second;
			return ptDeviceData;
		}
		bool GetDevice(const int nIndex,DeviceDataPt& ptdeviceData)
		{
			bool bRes = false;
			if(m_Devices.size() > static_cast<size_t>(nIndex))
			{
				std::map<TSTRING,DeviceDataPt>::const_iterator cIter = m_Devices.begin();				
				advance(cIter,nIndex);
				ptdeviceData = cIter->second;
				bRes = true;
			}
			return bRes;
		}
	public:
		CDectectDevice()
			:m_bEnable(false),m_hWnd(NULL),m_strClassName(_T("DectectDevice")),m_strTitleName(_T("Ghost"))
			,m_DetectThreadEventAbort(FALSE,TRUE,_T("DetectThreadEventAbort"))
			,m_DetectThreadEventFinish(FALSE,TRUE,_T("DetectThreadEventFinish"))
			,m_hDetectThread(NULL)
			,m_hDeviceConnectionStatusEvent(FALSE,TRUE,_T("MobileDeviceConnectionEvent"))
		{
			m_hDeviceConnectionStatusEvent.ResetEvent();
			DefineNotify(DEVICE_CONNECT);
			DefineNotify(DEVICE_DISCONNECT);
			DefineNotify(DEVICE_DIAG_CONNECT);
			DefineNotify(DEVICE_DIAG_DISCONNECT);
			CreateGhostWnd();
			RegisterNotification();
		}
		virtual ~CDectectDevice()
		{
			WaitForDetectThread();
			DeviceMapIter Iter = m_Devices.begin();
			while(Iter != m_Devices.end())
			{
				RemoveDeviceFromMap(Iter);
				Iter = m_Devices.begin();
			};
			if(IsWindow(m_hWnd))
				DestroyWindow();
		}

		void WaitForDetectThread()
		{
			if(m_hDetectThread)
			{
				m_DetectThreadEventAbort.SetEvent();
				QSYNC_LIB::MessageLoop(&m_DetectThreadEventFinish.m_hObject,1);
				/*
				while(::WaitForSingleObject(m_DetectThreadEventFinish,1000) != WAIT_OBJECT_0)
				{
					TRACE(_T("WaitForDetectThread().....1 Sec\n"));
				};
				*/
			}
		}

		static void DetectThreadEntry(void* pParam)
		{
			CDectectDevice* pDetectDevice = (CDectectDevice*)pParam;
			pDetectDevice->DetectExistingPorts();
			pDetectDevice->m_DetectThreadEventFinish.SetEvent();
		}

		BOOL CreateGhostWnd();
		HWND GetSafeHwnd(){return IsWindow(m_hWnd) ? m_hWnd : NULL;}
		char FirstDriveFromMask (ULONG unitmask);
		BOOL OnDeviceChange(UINT nEventType,DWORD dwData);
		BOOL RegisterWindow();
		BOOL DestroyWindow(){ return ::DestroyWindow(m_hWnd);}
		LRESULT CALLBACK WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
		void AddDevice(const DeviceDataPt ptdevData,const TSTRING& strEvent);
		void RemoveDevice(const DeviceDataPt ptdevData,const TSTRING& strEvent);
		static CDectectDevice* GetObjectFromWindow(HWND hWnd);
		static LRESULT CALLBACK stWinMsgHandler(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
		int RegisterNotification();
		int UnregisterNotification();
		DWORD LocatePort(HANDLE hUSBHUB, HDEVINFO hDeviceInfoSet, PSP_DEVINFO_DATA pspDeviceInfoData);		
		void GetDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv,UINT nEventType);
		void AddDevice(PDEV_BROADCAST_DEVICEINTERFACE lpbv);
		void RemoveDeviceFromMap(DeviceMapIter Iter);		
		void RemoveDevice(const TSTRING& strDriverClass);
		bool isPortExists(const TSTRING& strPort, vector<TSTRING> &ExistingPortIDs);
		const TSTRING GetDeviceProperty(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoData, DWORD dwProperty);
		const TSTRING GetsSymbolicName(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoDataHub);
		const TSTRING GetCOMPort(HDEVINFO hDeviceInfoSet,SP_DEVINFO_DATA spDeviceInfoData,const TSTRING& sSymbolicName);
		TSTRING GetQualcommPort(const USHORT idProduct,const USHORT idVendor);
		TSTRING GetUSBDeviceInfo(const USHORT idProduct,const USHORT idVendor,const TSTRING& strPara,const TSTRING& strClass,const TSTRING& strSunFolder,const TSTRING& strField);
		bool IsVaildVIDPID(USHORT idVendor,USHORT idProduct);
		void AddPortNotify(const TSTRING& strPort,const TSTRING& strDeviceClass,const TSTRING& strEvent);
		void RemovePortNotify(const TSTRING& strPort,const TSTRING& strEvent);
		void ParserDBCCName(TCHAR dbcc_name[1],USHORT& idVendor,USHORT& idProduct,TSTRING& strPara);

		TSTRING GetSubKey(HKEY hKey,int nIndex);
		const TSTRING GetKeyValue(HKEY hKey,const TSTRING& strName,DWORD& dwType,DWORD& dwRequiredSize);
		const bool IsMODEM(const TSTRING& strCOMPort,TSTRING& strModule);
		const bool IsDiag(const TSTRING& strCOMPort);
		bool IsReallyCOMPort(const TSTRING& strPortName);
		TSTRING GetQualcommPortName(HKEY hKey,const TSTRING& strSubKey);

		bool  DetectExistingPorts();
		bool  GetSerialPortHKEY(HKEY &hkeySERIALCOMM);
		bool  GetExistingPortNumbers(vector<TSTRING> *PortIDs);
	private:
		HDEVNOTIFY									m_hDevNotify;
		HWND												m_hWnd;
		CString												m_strClassName;
		CString												m_strTitleName;
		map<TSTRING,DeviceDataPt>		m_Devices;
		CCriticalSection								m_CritSection;
		bool														m_bEnable;
		CEvent												m_DetectThreadEventFinish;
		CEvent												m_DetectThreadEventAbort;
		HANDLE											m_hDetectThread;
		/*If the device connected, it will be SetEvent or ResetEvent*/
		CEvent												m_hDeviceConnectionStatusEvent;
	};

}//namespace QSYNC_LIB

#endif //_DEVICEDETECT_H_