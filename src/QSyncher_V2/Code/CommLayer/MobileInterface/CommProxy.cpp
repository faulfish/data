#include "StdAfx.h"
#include "CommProxy.h"
#include "../../Components/Configuration/QConfigure/QMobileConfigure.h"

CCommProxy::CCommProxy():m_isDevAttached(FALSE)
{
	//Device List  contains only an Empty Device.
	SetDeviceToEmpty();
}

CCommProxy::~CCommProxy()
{
	RemoveAttachedDev();
}

LPVOID CCommProxy::ProxyGetInterface ( CommInterface eInterface )
{
	LPVOID pInterface = NULL;

	m_MainDevice->DeviceGetInterface(eInterface, &pInterface);

	return pInterface;
}

ICommDevice* CCommProxy::LoadDeviceFomDll(LPCTSTR strDll) 
{
	ICommDevice* pDevice = NULL;

	HINSTANCE hDLL		  = NULL;     	  
	hDLL = ::LoadLibrary(strDll);
	if(hDLL)
	{		
		LPDeviceFunc lpfnDllFunc = (LPDeviceFunc)::GetProcAddress(hDLL,"GetDevice");
		if (lpfnDllFunc)
		{
			pDevice = lpfnDllFunc();
		}
		else
		{
			::FreeLibrary(hDLL);
		}
	}
	return pDevice;
}

BOOL CCommProxy::AttachDevice ( LPCTSTR  strModel )
{
	using namespace QSYNC_LIB;

	CQMobileConfigure* m_Configure = GetMobileConFig();
	if(m_Configure)
		m_Configure->LoadDefaultXML();

	CQSetting oMobSetting;
	if(m_Configure && m_Configure->GetMobileSettings( strModel, oMobSetting ))
	{		
		TSTRING strDeviceDLL = oMobSetting.Get(_T("Devices"));
		ICommDevice* pCDCDev = LoadDeviceFomDll(strDeviceDLL.c_str());
		if(pCDCDev)
		{
			m_MainDevice = pCDCDev;
			m_DeviceList.push_back( pCDCDev );
			m_isDevAttached = TRUE; 
		}
		else assert(0);
	}	

	return TRUE;
}

BOOL CCommProxy::DetachAllDevice ()
{
	//Remove current attached device objects in Device List
	RemoveAttachedDev();
	//Device List  contains only an Empty Device.
	SetDeviceToEmpty();
	m_isDevAttached = FALSE;

	return TRUE;
}

BOOL CCommProxy::GetAttachDevices ( CommDeviceList *pDeviceList )
{
	return m_isDevAttached;
}

BOOL CCommProxy::GetMainDevice ( ICommDevice **pDevice )
{
	*pDevice = m_MainDevice; 
	return TRUE;
}

bool delFunc(ICommDevice* pDev)
{
	bool bRes = true;
	pDev->DetachAllInterface();
	delete pDev;
	return bRes;
}

void CCommProxy::RemoveAttachedDev()
{
	std::vector<ICommDevice*>::iterator Iter = remove_if (m_DeviceList.begin( ), m_DeviceList.end( ), delFunc );
	m_DeviceList.clear();
}

void CCommProxy::SetDeviceToEmpty()
{
	CEmptyDevice* pEmptyDev = new CEmptyDevice();
	m_DeviceList.push_back( pEmptyDev );
	m_MainDevice = pEmptyDev;
}