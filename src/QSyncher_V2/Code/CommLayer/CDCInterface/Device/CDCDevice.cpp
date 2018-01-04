#include "StdAfx.h"
#include "CDCDevice.h"
#include "../CDCInterface/OBEXEFS.h"
#include "../CDCInterface/ATEFS.h"
#include "../CDCInterface/ATPHONEBOOK.h"
#include "../CDCInterface/ATCALENDAR.h"
#include "../CDCInterface/Message.h"
#include "../CDCInterface/Request/OBEXSYNCML.h"
#include "../CDCInterface/PIMConfigure.h"

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

extern "C" CDCINTERFACE_CLASS_DLLEXT ICommDevice* GetDevice()
{
	/*
	Garbage collection by mobile interface
	*/
	return new CCDCDevice();
}

CCDCDevice::CCDCDevice(void):
m_pISMS(NULL),
m_pIGeneral(NULL),
m_pIEFS(NULL),
m_pISyncML(NULL),
m_pIPhonebook(NULL),
m_pICalendar(NULL),
m_Configure(NULL),
m_pIPIMConfigure(NULL)
{
	m_Configure = GetMobileConFig();
}

CCDCDevice::~CCDCDevice(void)
{
	DetachAllInterface ();
}

BOOL CCDCDevice::DeviceGetInterface( CommInterface eInterface, LPVOID *ppInterface )
{
	BOOL bInterfaceGet = FALSE;

	if ( !IsInterfaceAttached(eInterface) )
		AttachInterface(eInterface);

	if ( IsInterfaceAttached(eInterface) )
	{
		switch ( eInterface )
		{
		case eIEFS: 
			*ppInterface = m_pIEFS;
			bInterfaceGet = TRUE;
			break;
		case eISMS: 
			*ppInterface = m_pISMS;
			bInterfaceGet = TRUE;
			break;
		case eISyncML: 
			*ppInterface = m_pISyncML;
			bInterfaceGet = TRUE;
			break;
		case eIPhonebook: 
			*ppInterface = m_pIPhonebook;
			bInterfaceGet = TRUE;
			break;
		case eICalendar: 
			*ppInterface = m_pICalendar;
			bInterfaceGet = TRUE;
			break;
		case eIPIMConfigure: 
			*ppInterface = m_pIPIMConfigure;
			bInterfaceGet = TRUE;
			break;
		default:
			assert(0);
			break;
		}
	}
	return bInterfaceGet;
}

TSTRING CCDCDevice::GetInterfaceName(CommInterface eInterface)
{
	TSTRING strIBase;
	switch ( eInterface )
	{
	case eIEFS: 
		strIBase = _T("IEFS");
		break;
	case eISMS: 
		strIBase = _T("ISMS");
		break;
	case eISyncML: 
		strIBase = _T("ISyncML");
		break;
	case eIPhonebook: 
		strIBase = _T("IPhonebook");
		break;
	case eICalendar: 
		strIBase = _T("ICalendar");
		break;
	case eIPIMConfigure: 
		strIBase = _T("IPIMConfigure");
		break;
	default:
		assert(0);
		break;
	}
	return strIBase;
}

BOOL CCDCDevice::AttachInterface(CommInterface eInterface)
{
	BOOL bRes = FALSE;
	
	TSTRING strIBase = GetInterfaceName(eInterface);

	CQSetting oMobSetting;
	if(m_Configure && m_Configure->GetMobileSettings( m_strModel, oMobSetting ))
	{
		CQSetting oISetting; 
		if(m_Configure->GetSettings( _T("Interfaces"), oISetting))
		{
			for (int i=0; i<oISetting.Size(); i++)
			{
				//TRACE(_T("Find interface (%s == %s)\n"),strIBase.c_str(),oISetting.GetName(i).c_str());
				if(strIBase == oISetting.GetName(i))
				{
					bRes = CreateInterface( eInterface, oISetting.GetValue(i) );
					break;
				}
			}
		}
	}
	return bRes;
}

BOOL CCDCDevice::CreateInterface(CommInterface eInterface, const TSTRING& strIClass)
{
	bool bRes = false;
	switch ( eInterface )
	{
	case eIEFS: 
		if ( strIClass == _T("OBEXEFS") )
		{
			m_pIEFS = CreateOBEXIEFS();
			bRes = true;
		}
		else if ( strIClass == _T("ATEFS") )
		{
			m_pIEFS = CreateATIEFS();
			bRes = true;
		}
		else assert(0);
		break;
	case eISMS:
		if ( strIClass == _T("ISMS") )
		{
			m_pISMS  = CreateISMS();
			bRes = true;
		}
		else assert(0);
		break;
	case eISyncML: 
		if ( strIClass == _T("OBEXSyncML") )
		{
			m_pISyncML  = CreateOBEXISYNCML();
			bRes = true;
		}
		else assert(0);
		break;
	case eIPhonebook: 
		if ( strIClass == _T("IPhonebook") )
		{
			m_pIPhonebook  = CreateATPHONEBOOK();
			bRes = true;
		}
		else assert(0);
		break;
	case eICalendar:
		if ( strIClass == _T("ICalendar") )
		{
			m_pICalendar  = CreateATCALENDAR();
			bRes = true;
		}
		else assert(0);
		break;
	case eIPIMConfigure: 
		if ( strIClass == _T("IPIMConfigure") )
		{
			m_pIPIMConfigure  = CreatePIMConfigure(m_strModel);
			bRes = true;
		}
		else assert(0);
		break;
	default:
		assert(0);
		break;
	}

	return bRes;
}

BOOL CCDCDevice::DetachAllInterface ()
{
	if (m_pIGeneral) 
	{  
		delete m_pIGeneral;
		m_pIGeneral = NULL;
	}

	if(m_pIEFS)
	{
		DeleteATIEFS();
		DeleteOBEXIEFS();
		m_pIEFS = NULL;
	}
	DeleteISMS();
	m_pISMS = NULL;
	DeleteOBEXISYNCML();
	m_pISyncML = NULL;
	DeleteATPHONEBOOK();
	m_pIPhonebook = NULL;
	DeleteATCALENDAR();
	m_pICalendar = NULL;
	DeletePIMConfigure();
	m_pIPIMConfigure = NULL;

	return TRUE; 
}

int CCDCDevice::ConnectDevice( LPCTSTR strModel,  LPCTSTR port)
{
	HRESULT hr = S_OK;
	
	if(m_pIGeneral == NULL)
		m_pIGeneral = new CGeneral();	
	m_Configure->LoadDefaultXML();

	CString strDevModel,strDevIMEI,strDevSoftwareVer;

	if ( m_eConnectState == eCommConnected )
		hr = eCommErrDeviceAlreadyConnect;

	if(hr == S_OK)
	{
		if (m_pIGeneral)  //General Interface is attached
		{
			hr = m_pIGeneral->SetPort( port );
			assert(hr == S_OK);
		}
		else 
		{
			hr = eCommErrDeviceNotInit;  //Some internal errors
			m_pIGeneral->ResetPort();
		}
	}

	if ( hr == S_OK ) //Successfully set port and connect device
	{
		m_strConnectPort = port;
		Sleep(2000); //Sleep a while before actually write data to CDC Device
		hr = m_pIGeneral->GetMobileInfo(&strDevModel, &strDevIMEI, &strDevSoftwareVer);		
		if(hr == S_OK)
			hr = m_pIGeneral->GetFDNStatus(m_bIsFDN);
	}  

	if ( hr == S_OK ) //Successfully get Mobile Information
	{
		if ( strDevModel == strModel )//Found Device Model Name equals to the specified Model name
		{
			m_strModel = LPCTSTR(strDevModel);
			m_strIMEI = LPCTSTR(strDevIMEI);
			m_strSoftwareVer = LPCTSTR(strDevSoftwareVer);
		}
		else
		{
			hr = eCommErrDevModelNameNotMatch;
		}
	}

    if (hr == S_OK && m_pIGeneral)  //General Interface is attached
	{
        hr = m_pIGeneral->SetCommandEchoOff(false);
	}

	if ( hr == S_OK || hr == eCommErrDevModelNameNotMatch )
	{
		m_eConnectState = eCommConnected;
	}

	return hr;  
} 

int CCDCDevice::DisconnectDevice( LPCTSTR strModel,  LPCTSTR port)
{
	HRESULT hr = S_OK;

	if (  m_eConnectState == eCommNoConnection ||
		m_eConnectState == eCommDisConnected   )
		return hr;

	if (m_pIGeneral)  //General Interface is attached
	{
		hr = m_pIGeneral->ResetPort();
	}
	else 
	{
		hr = eCommErrDeviceNotInit;  //Some internal errors
	}

	if ( hr == S_OK ) //Successfully Reset port and disconnect device
	{
		m_strIMEI.clear();
		m_strSoftwareVer.clear();
		m_strConnectPort.clear();
		m_eConnectState = eCommDisConnected;
	} 

	return hr;  
}

BOOL CCDCDevice::IsInterfaceAttached( CommInterface eInterface )
{
	BOOL isAttached = FALSE;

	switch (eInterface)
	{
	case eIEFS:
		if ( m_pIEFS  ) isAttached = TRUE;
		break;
	case eISMS:
		if ( m_pISMS  ) isAttached = TRUE;
		break;
	case eISyncML:
		if ( m_pISyncML  ) isAttached = TRUE;
		break;
	case eIPhonebook:
		if ( m_pIPhonebook  ) isAttached = TRUE;
		break;
	case eICalendar:
		if ( m_pICalendar  ) isAttached = TRUE;
		break;
	case eIPIMConfigure:
		if ( m_pIPIMConfigure  ) isAttached = TRUE;
		break;
	default:
		assert(0);
		break;
	}

	return isAttached;   
}

BOOL CCDCDevice::GetConnectivity(bool& bIsOnline) {
    m_pIGeneral->GetConnectivity(bIsOnline);
    return TRUE;
}
