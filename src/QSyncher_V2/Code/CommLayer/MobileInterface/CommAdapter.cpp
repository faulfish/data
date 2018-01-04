#include "StdAfx.h"
#include "CommAdapter.h"
#include "CommProxy.h"
#include "CommIEFS.h"
#include "CommISMS.h"
#include "CommISyncML.h"
#include "CommIPhonebook.h"
#include "CommICalendar.h"
#include "CommIPIMConfigure.h"

CCommAdapter::CCommAdapter( ):m_pCommProxy(NULL)
{
}

CCommAdapter::CCommAdapter( CCommProxy* pCommProxy ):m_pCommProxy(pCommProxy)
{
}

CCommAdapter::~CCommAdapter(void)
{
	DeleteAllinterface();
}

void CCommAdapter::SetProxy( CCommProxy* pCommProxy )
{
	m_pCommProxy = pCommProxy; 
}

BOOL CCommAdapter::CreateInterface( CommInterface eInterface, CCommInterface* &pInterface )
{
	BOOL   bInterfaceCreated = FALSE;
	LPVOID iInterface;

	if (m_pCommProxy == NULL) return FALSE;

	iInterface =  m_pCommProxy->ProxyGetInterface(eInterface);

	if ( iInterface != NULL )
	{
		bool bRes = true;
		switch (eInterface)
		{
		case eIEFS:
			pInterface = new CCommIEFS();
			break;
		case eISMS:
			pInterface = new CCommISMS();
			break;
		case eISyncML:
			pInterface = new CCommISyncML();
			break;
		case eIPhonebook:
			pInterface = new CCommIPhonebook();
			break;
		case eICalendar:
			pInterface = new CCommICalendar();
			break;
		case eIPIMConfigure:
			pInterface = new CCommIPIMConfigure();
			break;
		default:
			bRes = false;
			assert(0);
			break;
		}
		if(bRes)
		{
			pInterface->SetInterface( eInterface, iInterface );
			m_vpCommInterface.AddTail ( pInterface );
			bInterfaceCreated = TRUE;
		}
	}
	return bInterfaceCreated;
}

void CCommAdapter::DeleteInterface( CCommInterface *pInterface )
{
	POSITION pos;
	CCommInterface* pCommInterface;

	for( pos = m_vpCommInterface.GetHeadPosition(); pos != NULL; )
	{
		pCommInterface = m_vpCommInterface.GetNext( pos );
		if ( pInterface == pCommInterface )
		{
			m_vpCommInterface.RemoveAt( pos );
			delete pCommInterface;
			break;
		}
	}
}

void CCommAdapter::LockAllInterface( BOOL bLock )
{
	POSITION pos;
	CCommInterface* pCommInterface;

	for( pos = m_vpCommInterface.GetHeadPosition(); pos != NULL; )
	{
		pCommInterface = m_vpCommInterface.GetNext( pos );
		pCommInterface->LockInterface( bLock );
	}
}


BOOL CCommAdapter::AdaptAllinterface()
{
	BOOL bInterfaceAdapted = TRUE;
	POSITION pos;
	CCommInterface* pCommInterface;
	LPVOID iInterface;
	int eInterface;

	if (m_pCommProxy == NULL) return FALSE;

	for( pos = m_vpCommInterface.GetHeadPosition(); pos != NULL; )
	{
		pCommInterface = m_vpCommInterface.GetNext( pos );
		pCommInterface->GetInterface( eInterface, iInterface );
		iInterface =  m_pCommProxy->ProxyGetInterface( (CommInterface)eInterface );

		if ( iInterface != NULL )
		{
			pCommInterface->SetInterface( (CommInterface)eInterface, iInterface );
		}
		else
		{
			bInterfaceAdapted = FALSE;
		}
	} 
	return bInterfaceAdapted;
}

BOOL CCommAdapter::DeleteAllinterface()
{
	POSITION pos;
	CCommInterface* pCommInterface;

	for( pos = m_vpCommInterface.GetHeadPosition(); pos != NULL; )
	{
		pCommInterface = m_vpCommInterface.GetNext( pos );
		delete pCommInterface;
	}

	m_vpCommInterface.RemoveAll(); 

	return TRUE;
}