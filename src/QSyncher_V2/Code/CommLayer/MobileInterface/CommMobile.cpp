// CommMobile.cpp : The implementation file of ICommMobile interface
// Author: Saffin Liao @ BenQ Corp. Date: 15/Nov,2006   

#include "stdafx.h"
#include "MobileInterface.h"
#include "CommIEFS.h"
#include "CommDevice.h"
#include "CommAdapter.h"
#include "CommProxy.h"
#include "..\..\Components\Configuration\QConfigure\QMobileConfigure.h"
#include "..\..\CommonUtil\LogPolicy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern CApp theApp;
		
int CommOpenMobileInterface( LPCTSTR strApp ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	if(pNewMobile)
		pNewMobile->m_registerApp = strApp;
	return eCommRetSuccess;
}

int CommSetDetectMode( LPCTSTR strApp, CommDetectMode eConnectMode ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return eCommRetSuccess;
}

int CommGetDetectMode(  LPCTSTR strApp, CommDetectMode &eConnectMode ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return eCommRetSuccess;
}


int CommSetDetectPorts( LPCTSTR strApp, CommTransDevice eTransDevice, LPCTSTR strPorts ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return eCommRetSuccess;
}

int CommGetDetectPorts( LPCTSTR strApp, CommTransDevice eTransDevice, LPTSTR strPorts ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return eCommRetSuccess;
}

int CommConnectMobile(  LPCTSTR strApp, CommTransDevice eTransDevice, LPCTSTR strPort, LPCTSTR strMobileModel ,int& nIndex)
{
	TRACE_FILE_LINE_INFO();

	CMobileInterface *pNewMobile = theApp.GetMobileInterface(nIndex);
	if(pNewMobile == NULL)
	{
		pNewMobile = new CMobileInterface;
		nIndex = theApp.AddMobileInterface(pNewMobile);
	}

	LockMobileInterface lock;

	ICommDevice *pDevice;
	BOOL bContinue = TRUE;
	int  nRes = S_OK;

	if ( pNewMobile->m_ConnectState == eCommConnected )
	{
		assert(0);
		bContinue = FALSE;
		nRes = eCommErrDeviceAlreadyConnect;
	}

	if ( CString(strPort) == _T("") ||  CString(strMobileModel) == _T("") )
	{
		assert(0);
		bContinue = FALSE;
		nRes = eCommErrParamEmpty;
	}

	ASSERT ( pNewMobile->m_ConnectState != eCommConnecting );

	if ( bContinue )
	{
		pNewMobile->m_ConnectState = eCommConnecting;
		//First Attach all device belong to the MobileModel
		bContinue = pNewMobile->m_CommProxy->AttachDevice( strMobileModel );
	}	

	if ( bContinue ) //Second,Get MainDevice from all attached device to connect
		bContinue = pNewMobile->m_CommProxy->GetMainDevice( &pDevice );
	else if( nRes == S_OK ) 
		nRes = eCommErrAttachDevice;

	if ( bContinue && nRes==S_OK ) //Connect Main Device
		nRes = pDevice->ConnectDevice( strMobileModel, strPort );
	else if( nRes == S_OK )
		nRes = eCommErrAttachDevice;

	if ( bContinue && nRes == S_OK ) // Connect Success
	{
		pNewMobile->m_ConnectState = eCommConnected;
		pNewMobile->m_ConnectDevModel = pDevice->GetModelName().c_str();
		pNewMobile->m_ConnectDevIMEI  = pDevice->GetIMEI().c_str();
		pNewMobile->m_ConnectDevSoftwareVer = pDevice->GetSoftwareVer().c_str();
		pNewMobile->m_ConnectDevDisplayName = pDevice->GetModelName().c_str();
		pNewMobile->m_ConnectPort = pDevice->GetPort().c_str();
		pDevice->GetFDNStatus(pNewMobile->m_bIsFDNMode);
	}
	else if( bContinue )
	{
		pNewMobile->m_CommProxy->DetachAllDevice(); // Connect Device fail, Need to DetachAllDevice
		pNewMobile->m_ConnectState = eCommDisConnected;
	}

	return nRes;
}

int CommDisConnectMobile( LPCTSTR strApp, CommTransDevice eTransDevice, LPCTSTR strPort, LPCTSTR strMobileModel ,int nIndex)
{
	TRACE_FILE_LINE_INFO();
	
	int  nRes = S_OK; 

	LockMobileInterface lock;
	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	
	if(pNewMobile)
	{
		BOOL bContinue = TRUE;		
		ICommDevice *pDevice;

		if ( pNewMobile->m_ConnectState == eCommDisConnected || 
			pNewMobile->m_ConnectState == eCommNoConnection )
		{
			nRes = eCommErrDeviceNotConnect;
			bContinue = FALSE;
			assert(0);
		}

		if (bContinue)
		{
			pNewMobile->m_ConnectState = eCommConnecting;
			//Get MainDevice from all attached device to disconnect
			bContinue = pNewMobile->m_CommProxy->GetMainDevice( &pDevice );
		}

		if (bContinue) //DisConnect Main Device
			nRes = pDevice->DisconnectDevice( strMobileModel, strPort ); 
		else if( nRes == S_OK )
			nRes = eCommErrAttachDevice;

		if ( bContinue && nRes == S_OK) // DisConnect Success
		{
			pNewMobile->m_ConnectState = eCommNoConnection;
			//Reget Main device because original devices are dettached
			pNewMobile->m_CommProxy->GetMainDevice( &pDevice );
			pNewMobile->m_ConnectDevType = pDevice->GetTransDevice();

			//It will set it to empty device
			pNewMobile->m_CommProxy->DetachAllDevice();			
		}
		//theApp.RemoveMobileInterface(nIndex);
	}
	if(pNewMobile)
		pNewMobile->m_CommAdapter->AdaptAllinterface();
	return nRes;
}

int CommGetConnectionState( CommConnectionState &eConnectionState ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	if(pNewMobile)
		eConnectionState = pNewMobile->m_ConnectState;
	return eCommRetSuccess;
}

int CommGetConnectPort( CommTransDevice &eTransDevice, LPTSTR strPort  ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	if ( strPort == NULL )
		return eCommErrParamNull;  

	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	if(pNewMobile)
	{
		eTransDevice = pNewMobile->m_ConnectDevType;
		_tcscpy_s( strPort, MAX_RET_STRING_LENGTH, pNewMobile->m_ConnectPort.GetBuffer());
	}

	return eCommRetSuccess;
}

int CommGetMobileInfo(  LPCTSTR strApp, LPTSTR pMobileModel, LPTSTR pIMEI, LPTSTR pDisplayName,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	int nRet = eCommRetSuccess;

	if ( pMobileModel==NULL || pIMEI==NULL || pDisplayName == NULL )
	{
		nRet = eCommErrParamNull;   
	}

	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	if(pNewMobile)
	{
		_tcscpy_s( pMobileModel, MAX_RET_STRING_LENGTH, pNewMobile->m_ConnectDevModel.GetBuffer());
		_tcscpy_s( pIMEI, MAX_RET_STRING_LENGTH, pNewMobile->m_ConnectDevIMEI.GetBuffer());
		_tcscpy_s( pDisplayName, MAX_RET_STRING_LENGTH, pNewMobile->m_ConnectDevDisplayName.GetBuffer());
	}

	return nRet;
}

int CommSetDisplayName( LPCTSTR strApp, LPTSTR strIMEI, LPTSTR strDisplayName,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return eCommRetSuccess;
}

int CommGetMobileSoftwareVer( LPTSTR strSoftwareVer ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	int nRet = eCommRetSuccess;

	if ( strSoftwareVer == NULL )
	{
		nRet = eCommErrParamNull;  
	}

	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	if(pNewMobile)
	{
		if (pNewMobile->m_ConnectState != eCommConnected)
		{
			nRet = eCommErrDeviceNotConnect;
		}
	}

	_tcscpy_s( strSoftwareVer, MAX_RET_STRING_LENGTH, pNewMobile->m_ConnectDevSoftwareVer.GetBuffer());

	return nRet;
}

int CommIsConnectionAlive( int nTimeOut, LPCTSTR strIMEI ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return eCommRetSuccess;
}

BOOL CommGetMobileCapability( CommInterface eInterface ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return FALSE;
}

LPVOID CommCreateInterface( LPCTSTR strApp, LPCTSTR strClient, CommInterface eInterface ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	CCommInterface* pInterface = NULL;
	BOOL            bInterfaceCreated = FALSE;
	LPVOID          pAppInterface = NULL;

    int nPos = theApp.GetMobileInterfaceCount() - 1;
    CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nPos);
	if(pNewMobile)
		bInterfaceCreated = pNewMobile->m_CommAdapter->CreateInterface( eInterface, pInterface );

	if ( bInterfaceCreated && pInterface)
	{
		ASSERT( eInterface == pInterface->GetInterfaceEnum() );

		pInterface->SetRegisterApp( strApp );
		pInterface->SetRegisterClient( strClient );

		switch ( eInterface )
		{
		case eIEFS:
			//CCommInterface* can be dynamic cast to IEFS if pInterface points to a CCommIEFS object 
			pAppInterface =  dynamic_cast< IEFS* >(pInterface); 
			break;  
		case eISMS:
			//CCommInterface* can be dynamic cast to ISMS if pInterface points to a CCommSMS object 
			pAppInterface =  dynamic_cast< ISMS* >(pInterface); 
			break;    
		case eISyncML:
			//CCommInterface* can be dynamic cast to ISYNCML if pInterface points to a CCommSyncML object 
			pAppInterface =  dynamic_cast< ISYNCML* >(pInterface); 
			break; 
		case eIPhonebook:
			//CCommInterface* can be dynamic cast to IPHONEBOOK if pInterface points to a CCommIPhonebook object 
			pAppInterface =  dynamic_cast< IPHONEBOOK* >(pInterface); 
			break; 
		case eICalendar:
			//CCommInterface* can be dynamic cast to IPHONEBOOK if pInterface points to a CCommIPhonebook object 
			pAppInterface =  dynamic_cast< ICALENDAR* >(pInterface); 
			break; 
		case eIPIMConfigure:
			//CCommInterface* can be dynamic cast to IPIMCONFIG if pInterface points to a CCommIPIMConfigure object 
			pAppInterface =  dynamic_cast< IPIMCONFIG* >(pInterface); 
			break;
		default:
			assert(0);//Not define , please check it
			break;
		}
	}

	return pAppInterface;
}

void CommDeleteInterface( LPVOID pInterface ,int nIndex)     
{
	TRACE_FILE_LINE_INFO();

	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	if(pNewMobile)
		pNewMobile->m_CommAdapter->DeleteInterface( (CCommInterface*)pInterface );
}

int CommCloseMobileInterface( LPCTSTR strApp ,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	return eCommRetSuccess;
}

 int CommGetMobileInterfaceCount()
{
	TRACE_FILE_LINE_INFO();

	return theApp.GetMobileInterfaceCount();
}

void CommGetPIMDevice(LPCTSTR strApp,LPCTSTR strType,/*Out*/LPTSTR strPIMDEviceName,int nIndex)
{
	TRACE_FILE_LINE_INFO();

	TCHAR szMobileModel[MAX_RET_STRING_LENGTH] = {0};
	TCHAR szIMEI[MAX_RET_STRING_LENGTH] = {0};
	TCHAR szDisplayName[MAX_RET_STRING_LENGTH] = {0};

	CommGetMobileInfo(strApp, szMobileModel, szIMEI, szDisplayName,nIndex);

	using namespace QSYNC_LIB;
	bool bRes = false;
	CQSetting oSetting;
	CQMobileConfigure* pConfigure = GetMobileConFig();
	if(pConfigure)
	{
		if(pConfigure->LoadDefaultXML())
		{
			CQSetting oMobSetting;
			if(pConfigure->GetMobileSettings( szMobileModel, oMobSetting ))
				bRes = pConfigure->GetSettings( _T("PIMDevices"), oSetting);
		}

		if(bRes)
		{
			for(int i=0;i<oSetting.Size();i++)
			{
				TSTRING strName = oSetting.GetName(i);
				if(strName.find(strType) == 0)
					_tcscpy_s( strPIMDEviceName, MAX_RET_STRING_LENGTH, oSetting.GetValue(i).c_str());
			}
		}
	}
}

int CommGetFDNStatus(bool& bIsFDN,int nIndex)
{
	CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
	if(pNewMobile)
		bIsFDN = pNewMobile->m_bIsFDNMode;
	return true;
}

int CommGetConnectivity(bool& bIsOnline,int nIndex) {
    
    CMobileInterface *pNewMobile = theApp.GetMobileInterfaceByIndex(nIndex);
    ICommDevice *pDevice = NULL;
    if(pNewMobile)
        pNewMobile->m_CommProxy->GetMainDevice(&pDevice);
    if(pDevice)
        pDevice->GetConnectivity(bIsOnline);
    return true;
}
