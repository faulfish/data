#include "StdAfx.h"
#include "CommIEFS.h"
#include "MobileInterface.h"

CCommIEFS::CCommIEFS(void)
{

}

CCommIEFS::~CCommIEFS(void)
{

}

BOOL CCommIEFS::CanCancel()
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->CanCancel();
	}
	else
		return FALSE;
}

BOOL CCommIEFS::Cancel()
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->Cancel();
	}
	else
		return FALSE;
}

BOOL CCommIEFS::QueryCapability(IEFSCapability eIEFSCapability)
{
	return TRUE;
}

void CCommIEFS::SetOperationTimeOut(int nTimeOut)
{

}

int CCommIEFS::GetFileASync( int nOperationID, LPCTSTR strPath, int& nFileSize )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->GetFileASync( nOperationID, strPath, nFileSize );
	}
	else
		return eCommErrInterfaceLocked;
}


int CCommIEFS::GetFileData( int nOperationID, BYTE **ppFileData, DWORD& nSize, BOOL &bLastData )
{
	//Don't lock it , because GetFileData is called by GetFileASync
	//LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->GetFileData( nOperationID, ppFileData, nSize, bLastData );
	}
	else
		return eCommErrInterfaceLocked;
}

int CCommIEFS::PutFileAsync( int nOperationID, LPCTSTR strPath,  int nFileSize )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->PutFileAsync( nOperationID, strPath, nFileSize);
	}
	else
		return eCommErrInterfaceLocked;
}

int CCommIEFS::PutFileData( int nOperationID, BYTE *pFileData, DWORD nSize, BOOL bLastData )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->PutFileData(nOperationID, pFileData, nSize, bLastData);
	}
	else
		return eCommErrInterfaceLocked;
}

int CCommIEFS::DeleteFile( LPCTSTR strPath )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->DeleteFile( strPath );
	}
	else
		return eCommErrInterfaceLocked;
}

int CCommIEFS::GetFolderEntries( LPCTSTR strPath, vector<WIN32_FIND_DATA> *vEntryList )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->GetFolderEntries( strPath, vEntryList );
	}
	else
		return eCommErrInterfaceLocked;
}

int CCommIEFS::CreateFolder( LPCTSTR strPath )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->CreateFolder( strPath );
	}
	else
		return eCommErrInterfaceLocked;
}

int CCommIEFS::RemoveFolder( LPCTSTR strPath )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->RemoveFolder( strPath );
	}
	else
		return eCommErrInterfaceLocked;
}

BOOL CCommIEFS::Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath )
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->Rename( strOldFullPath,strNewFullPath );
	}
	else
		return FALSE;
}

int CCommIEFS::RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->RegisterNotify( pNotify, strEvent );
	}
	else
		return eCommErrInterfaceLocked;

}

int CCommIEFS::UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{
	LockMobileInterface lock;

	int    eInterface = -1;
	LPVOID pInterface;
	IEFS   *pIEFS;

	// interface is not locked
	if (GetInterface( eInterface, pInterface ) && eInterface == eIEFS) 
	{
		pIEFS = (IEFS*)pInterface;
		return pIEFS->UnRegisterNotify( pNotify, strEvent );
	}
	else
		return eCommErrInterfaceLocked;
}