#include "StdAfx.h"
#include "MobileFinderInterface.h"
#include "DetectDevice.h"

namespace QSYNC_LIB
{

	class MobileFinderInterface : public IDeviceFinder
	{
	public:
		MobileFinderInterface(void){}
		virtual ~MobileFinderInterface(void){}
	public:	
		virtual bool Register(INotify* pNotify)
		{
			bool bRes = false;
			if(pNotify)
			{
				bRes = m_DectectDevice.Register(pNotify,DEVICE_CONNECT);
				bRes = bRes && m_DectectDevice.Register(pNotify,DEVICE_DISCONNECT);
			}
			return bRes;
		}
		virtual bool Unregister(INotify* pNotify)
		{
			bool bRes = false;
			if(pNotify)
			{
				bRes = m_DectectDevice.Unregister(pNotify,DEVICE_CONNECT);
				bRes = bRes && m_DectectDevice.Unregister(pNotify,DEVICE_DISCONNECT);
			}
			return bRes;
		}
		virtual void StartMonitor()
		{
			m_DectectDevice.Start();
		}
		virtual void StopMonitor()
		{
			m_DectectDevice.Stop();
		}
		virtual const size_t GetDeviceCount () 
		{
			return m_DectectDevice.GetDeviceCount();
		}
		virtual bool GetDevice(const int nIndex,DeviceDataPt& ptdeviceData)
		{
			return m_DectectDevice.GetDevice(nIndex,ptdeviceData);
		}
		virtual const DeviceDataPt GetDevice(const TSTRING& strID)
		{
			return m_DectectDevice.GetDevice(strID);
		}
		void WaitForDetectThread()
		{
			m_DectectDevice.WaitForDetectThread();
		}
	private:
		CDectectDevice m_DectectDevice;
	};

	MobileFinderInterface* g_pMobileFinderInterface = NULL;
	//typedef SingletonHolder<MobileFinderInterface> MobileFinderInterfaceHolder;

	CDCINTERFACE_DLLEXT IDeviceFinder* GetDeviceFinder()
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if(g_pMobileFinderInterface == NULL)
			g_pMobileFinderInterface = new MobileFinderInterface;
		return static_cast<IDeviceFinder*>(g_pMobileFinderInterface);
	}

	CDCINTERFACE_DLLEXT void DeleteDeviceFinder()
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());
		if(g_pMobileFinderInterface)
		{
			g_pMobileFinderInterface->WaitForDetectThread();
			delete g_pMobileFinderInterface;
			g_pMobileFinderInterface = NULL;
		}
	}
}