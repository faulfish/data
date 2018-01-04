#include "StdAfx.h"
#include "EmptyDevice.h"

CEmptyDevice::CEmptyDevice()
:m_eConnectState(eCommNoConnection)
,m_pIEFS(NULL)
,m_pISMS(NULL)
,m_pIPhonebook(NULL)
,m_pICalendar(NULL)
,m_pIPIMConfigure(NULL)
{
}

CEmptyDevice::~CEmptyDevice()
{
}

BOOL CEmptyDevice::DeviceGetInterface( CommInterface eInterface, LPVOID *ppInterface )
{	
	BOOL bRes = FALSE;
	switch ( eInterface )
	{
	case eIEFS: 
		if(m_pIEFS == NULL)
			m_pIEFS = new CEmptyIEFS;
		assert(m_pIEFS);
		if(m_pIEFS)
			*ppInterface = m_pIEFS;
		break;
	case eISMS:
		if(m_pISMS == NULL)
			m_pISMS = new CEmptyISMS;
		assert(m_pISMS);
		if(m_pISMS)
			*ppInterface = m_pISMS;
		break;
	case eIPhonebook: 
		if(m_pIPhonebook == NULL)
			m_pIPhonebook = new CEmptyIPHONEBOOK;
		assert(m_pIPhonebook);
		if(m_pIPhonebook)
			*ppInterface = m_pIPhonebook;		
		break;
	case eICalendar:
		if(m_pICalendar == NULL)
			m_pICalendar = new CEmptyICALENDAR;
		assert(m_pICalendar);
		if(m_pICalendar)
			*ppInterface = m_pICalendar;		
		break;
	case eIPIMConfigure:
		if(m_pIPIMConfigure == NULL)
			m_pIPIMConfigure = new CEmptyIPIMCONFIG;
		assert(m_pIPIMConfigure);
		if(m_pIPIMConfigure)
			*ppInterface = m_pIPIMConfigure;		
		break;
	default:
		assert(0);
		break;
	}
	return FALSE;
}

BOOL CEmptyDevice::AttachAllInterface ( LPCTSTR  strModel)
{
	return FALSE;
}

BOOL CEmptyDevice::DetachAllInterface ()
{	
	if(m_pIEFS)
	{
		delete m_pIEFS;
		m_pIEFS = NULL;
	}
	if(m_pISMS)
	{
		delete m_pISMS;
		m_pISMS = NULL;
	}
	if(m_pIPhonebook)
	{
		delete m_pIPhonebook;
		m_pIPhonebook = NULL;
	}
	if(m_pICalendar)
	{
		delete m_pICalendar;
		m_pICalendar = NULL;
	}
	if(m_pIPIMConfigure)
	{
		delete m_pIPIMConfigure;
		m_pIPIMConfigure = NULL;
	}
	return FALSE;
}

int CEmptyDevice::ConnectDevice( LPCTSTR strModel,  LPCTSTR port)
{
	HRESULT hr = S_OK;
	m_strConnectPort = port;
	m_strModel = strModel;
	m_strIMEI = _T("1234567890");
	m_strSoftwareVer = _T("EmptyDevice");	

	assert(m_eConnectState != eCommConnected);
	if(m_eConnectState != eCommConnected)
		m_eConnectState = eCommConnected;
	return hr;
}

int CEmptyDevice::DisconnectDevice( LPCTSTR strModel,  LPCTSTR port)
{
	HRESULT hr = S_OK;
	if(m_eConnectState == eCommConnected)
		m_eConnectState = eCommNoConnection;
	return hr;
}

BOOL CEmptyDevice::IsInterfaceAttached( CommInterface eInterface )
{
	return FALSE;
}


BOOL CEmptyIEFS::QueryCapability(IEFSCapability eIEFSCapability)
{
	return FALSE;
}

void CEmptyIEFS::SetOperationTimeOut(int nTimeOut)
{

}

int CEmptyIEFS::GetFileASync( int nOperationID, LPCTSTR strPath, int& nFileSize )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::GetFileData( int nOperationID, BYTE **ppFileData, DWORD& nSize, BOOL &bLastData )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::PutFileAsync( int nOperationID, LPCTSTR strPath,  int nFileSize )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS:: PutFileData( int nOperationID, BYTE *pFileData, DWORD nSize, BOOL bLastData )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::DeleteFile( LPCTSTR strPath )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::GetFolderEntries( LPCTSTR strPath, vector<WIN32_FIND_DATA> *vEntryList )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::CreateFolder( LPCTSTR strPath )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::RemoveFolder( LPCTSTR strPath )
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{
	return eEFSDeviceEmpty;
}

int CEmptyIEFS::UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{
	return eEFSDeviceEmpty;
}

BOOL CEmptyIEFS::Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath )
{
	return FALSE;
}