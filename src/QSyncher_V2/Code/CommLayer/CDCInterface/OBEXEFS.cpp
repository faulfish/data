#include "StdAfx.h"
#include "OBEXEFS.h"
#include "RequestManager.h"
#include "GeneralRequest.h"
#include "FolderListingCodec.h"
#include "GeneralRequest.h"

#pragma warning(push)
#pragma warning(disable:4311)

COBEXEFS *g_pSingleton = NULL;

CDCINTERFACE_CLASS_DLLEXT IEFS* CreateOBEXIEFS()
{
	if(g_pSingleton == NULL)
		g_pSingleton = new COBEXEFS();
	return g_pSingleton;
}

CDCINTERFACE_CLASS_DLLEXT void DeleteOBEXIEFS()
{
	if(g_pSingleton)
	{
		delete g_pSingleton;
		g_pSingleton = NULL;
	}
}

COBEXEFS::COBEXEFS(void):m_nTimeOut(5000),m_ptReceiver(new CBuffer),m_hThreadHandle(NULL),m_AbortEvent(FALSE,TRUE,_T("EFS_ABOUT_EVENT")),m_bCanCancel(true),m_bFinishGetFile(false)
{
	DefineNotify(NOTIFY_GET_FILE_DATA);
	DefineNotify(NOTIFY_ABORT_GET_FILE_DATA);	
}

COBEXEFS::~COBEXEFS(void)
{
}

void COBEXEFS::Event(const TSTRING& strEvent,long nParam)
{
	if(strEvent == _T("Received"))
		m_ReceiveEvent.SetEvent();
}

BOOL COBEXEFS::CanCancel()
{
	return m_bCanCancel == true;
}

BOOL COBEXEFS::Cancel()
{
	BOOL bRes = false;	
	if(::WaitForSingleObject(m_AbortEvent,INFINITE) == WAIT_OBJECT_0 && m_bFinishGetFile == false)
	{
		CRequestPtr ptRequest = new COBEXFS_Abort;
		bRes = Execute(ptRequest) == eEFSRetSuccess;
	}
	return bRes;
}

BOOL STDMETHODCALLTYPE COBEXEFS::QueryCapability(IEFSCapability eIEFSCapability)
{
	return TRUE;
}

void STDMETHODCALLTYPE COBEXEFS::SetOperationTimeOut(int nTimeOut)
{
	m_nTimeOut = nTimeOut;
}

/*IEFS function description

Process of use get a file asynchronous

0. Regiser "NOTIFY_GET_FILE_DATA" event
1. User should call PutFileAsync() first and wait for "NOTIFY_GET_FILE_DATA" event
2. When receiver the "NOTIFY_GET_FILE_DATA" event, call GetFileData() to get the partial data.

*/
int  STDMETHODCALLTYPE COBEXEFS::GetFileASync( int nOperationID, LPCTSTR strPath, int& nFileSize )
{
	int nRet = eEFSRetFail;

	if(m_hThreadHandle == NULL)
	{
		TSTRING strOrgPath = strPath;
		TSTRING::size_type nFindindex;
		nFindindex = strOrgPath.find_last_of(_T("/\\"));
		TSTRING strPurePath = strOrgPath.substr(0,nFindindex);
		TSTRING strFileName = strOrgPath.substr(nFindindex+1);

		m_vectBuffer.clear();
		if(SetPath(strPurePath.c_str()))
		{
			CRequestPtr ptRequest = new COBEXFS_GetFirst(strFileName);
			nRet = Execute(ptRequest);
			nFileSize = -1;//please get it from GetFolderEntries
		}
		if(nRet == eEFSRetSuccessContinue)
		{
			CObexCodecPtr ptObex = new CObexCodec;
			ptObex->DecodePackage(m_ptReceiver,CObexCodec::oreqGet);
			CBufferPtr ptFileBuffer = new CBuffer;
			ptFileBuffer = ptObex->GetBody();
			{
				CSingleLock singleLock(&m_Critsection,TRUE);
				m_vectBuffer.push_back(ptFileBuffer);
			}

			DWORD dwThreadId = 0;
			m_nOperationID = nOperationID;
			m_AbortEvent.ResetEvent();
			m_hThreadHandle = ::CreateThread(NULL, 0, GetFileThread, (LPVOID)this, 0, &dwThreadId);
		}
	}

	return nRet;
}

DWORD WINAPI COBEXEFS::GetFileThread(LPVOID pCOBEXEFS)
{
	COBEXEFS* pThis = (COBEXEFS*)pCOBEXEFS;
	
	{
		CSingleLock singleLock(&pThis->m_Critsection,TRUE);		
		pThis->m_bFinishGetFile = false;
	}
	CRequestPtr ptRequest = new COBEXFS_GetData();
	int nRet = pThis->Execute(ptRequest);
	while(nRet == eEFSRetSuccessContinue)
	{
		if(::WaitForSingleObject(pThis->m_AbortEvent,0) == WAIT_OBJECT_0)
		{
			if(pThis->Cancel())
			{
				pThis->Fire(NOTIFY_ABORT_GET_FILE_DATA);
				break;
			}
		}
		CObexCodecPtr ptObex = new CObexCodec;
		ptObex->DecodePackage(pThis->m_ptReceiver,CObexCodec::oreqGet);
		CBufferPtr ptFileBuffer = ptObex->GetBody();
		{
			CSingleLock singleLock(&pThis->m_Critsection,TRUE);
			pThis->m_vectBuffer.push_back(ptFileBuffer);
		}
		NotifyPara para(pThis->m_nOperationID,ptFileBuffer->GetLength());
		pThis->Fire(NOTIFY_GET_FILE_DATA,(long)&para);
		nRet = pThis->Execute(ptRequest);
	}
	if(nRet == eEFSRetSuccess)
	{
		NotifyPara para(pThis->m_nOperationID,0);
		pThis->Fire(NOTIFY_GET_FILE_DATA,(long)&para);
		pThis->m_bFinishGetFile = true;
	}
 
	pThis->m_hThreadHandle = NULL;
	return 0;
}

int  STDMETHODCALLTYPE COBEXEFS::GetFileData( int nOperationID, BYTE **ppFileData, DWORD& nSize, BOOL &bLastData )
{
	CSingleLock singleLock(&m_Critsection,TRUE);
	if(m_nOperationID == nOperationID)
	{
		if(ppFileData == NULL)
		{
			nSize = 0;
			for(std::vector<CBufferPtr>::iterator Iter = m_vectBuffer.begin();Iter != m_vectBuffer.end();Iter++)
			{
				nSize += (*Iter)->GetLength();
			}
		}
		else
		{
			CBufferPtr ptFileBuffer = new CBuffer;
			if(m_vectBuffer.size() > 0)
			{
				ptFileBuffer = *(m_vectBuffer.begin());
				m_vectBuffer.erase(m_vectBuffer.begin());		
				nSize = ptFileBuffer->GetLength();
				if(nSize > 0)
				{
					*ppFileData = new BYTE[nSize];
					memcpy(*ppFileData,ptFileBuffer->GetData(),nSize);
				}
			}
			bLastData = (m_vectBuffer.size() == 0);
		}
	}
	return m_nOperationID == nOperationID;
}

int  STDMETHODCALLTYPE COBEXEFS::PutFileAsync( int nOperationID, LPCTSTR strPath,  int nFileSize )
{
	//DeleteFile(strPath);
	m_nOperationID = nOperationID;
	int nRet = eEFSRetFail;
	TSTRING strOrgPath = strPath;
	TSTRING::size_type nFindindex;
	nFindindex = strOrgPath.find_last_of(_T("/\\"));
	TSTRING strPurePath = strOrgPath.substr(0,nFindindex);
	TSTRING strFileName = strOrgPath.substr(nFindindex+1);

	if(SetPath(strPurePath.c_str()))
	{
		m_bFirstFlag = true;
		m_strPath = strFileName;
		m_nFileSize = nFileSize;
		nRet = eEFSRetSuccess;
	}
	return nRet;
}

int  STDMETHODCALLTYPE COBEXEFS::PutFileData(  int nOperationID, BYTE *pFileData, DWORD nSize, BOOL bLastData )
{
	int nRet = eEFSRetFail;
	if(m_nOperationID == nOperationID)
	{
		CRequestPtr ptRequest;
		CBufferPtr ptFileBuffer = new CBuffer(nSize,(const char*)pFileData);
		if(m_bFirstFlag)
		{
			m_bFirstFlag = false;
			ptRequest = new COBEXFS_PutFirst(m_strPath,m_nFileSize,ptFileBuffer,bLastData == TRUE);			
		}
		else
		{
			ptRequest = new COBEXFS_PutData(ptFileBuffer,bLastData == TRUE);
		}
		nRet = Execute(ptRequest);
	}
	return nRet;
}

int  STDMETHODCALLTYPE COBEXEFS::DeleteFile( LPCTSTR strPath )
{
	int nRet = eEFSRetFail;	

	//1.Set Path
	TSTRING strParentPath = strPath;
	TSTRING strDeleteFile;
	TSTRING::size_type nFindIndex = strParentPath.find_last_of(_T("/\\"));
	if(nFindIndex != TSTRING::npos)
	{
		strDeleteFile = strParentPath.substr(nFindIndex+1);		
		strParentPath = strParentPath.substr(0,nFindIndex);
	}
	bool bRes = SetPath(strParentPath.c_str());

	//2. Delete File
	assert(bRes);
	if(bRes)
	{
		CRequestPtr ptRequest = new COBEXFS_DeleteFile(strDeleteFile);
		nRet = Execute(ptRequest);
	}
	return nRet;
}

int  STDMETHODCALLTYPE COBEXEFS::GetFolderEntries( LPCTSTR strPath, vector<WIN32_FIND_DATA> *vEntryList )
{
	int nRet = eEFSRetFail;
	if(SetPath(strPath))
	{
		CRequestPtr ptRequest = new COBEXFS_GetObject("x-obex/folder-listing",CObexCodec::oreqGetFolderListing);
		nRet = Execute(ptRequest) == eEFSRetSuccess;
		if(nRet)
		{
			CFolderListingCodec FolderListing;
			FolderListing.DecodeBuffer(ptRequest->GetReceiver());
			for(DWORD dwIndex=0;dwIndex<FolderListing.GetNoOfEntries();dwIndex++)
			{
				WIN32_FIND_DATA findData;
				memset(&findData,0,sizeof(WIN32_FIND_DATA));
				_tcscpy_s(findData.cFileName,MAX_PATH,FolderListing.GetName(dwIndex));
				findData.nFileSizeLow = FolderListing.GetFileSize(dwIndex);
				findData.dwFileAttributes = FolderListing.GetAttributeDirectory(dwIndex) ? FILE_ATTRIBUTE_DIRECTORY : 0;
				if(FolderListing.GetAttributeHidden(dwIndex))
					findData.dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
				if(FolderListing.GetAttributeReadonly(dwIndex))
					findData.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
				if(FolderListing.GetAttributeSystem(dwIndex))
					findData.dwFileAttributes |= FILE_ATTRIBUTE_SYSTEM;
				if(FolderListing.GetAttributeArchive(dwIndex))
					findData.dwFileAttributes |= FILE_ATTRIBUTE_ARCHIVE;
				findData.ftCreationTime = GetFileTimeFromOleTime(FolderListing.GetCreationTime(dwIndex));
				findData.ftLastAccessTime = GetFileTimeFromOleTime(FolderListing.GetLastAccessTime(dwIndex));
				findData.ftLastWriteTime = GetFileTimeFromOleTime(FolderListing.GetLastWriteTime(dwIndex));				
				vEntryList->push_back(findData);
			}
		}
	}
	return nRet;
}

BOOL STDMETHODCALLTYPE COBEXEFS::Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath )
{
	CRequestPtr ptRequest = new COBEXFS_MoveFile(strOldFullPath,strNewFullPath,INFINITE);
	return Execute(ptRequest,INFINITE);
}

int  STDMETHODCALLTYPE COBEXEFS::CreateFolder( LPCTSTR strPath )
{	
	CRequestPtr ptRequest = new COBEXFS_SetFullPath(strPath,true,INFINITE);
	//CRequestPtr ptRequest = new COBEXFS_CreateFolder(strPath);	
	return Execute(ptRequest);
}

int  STDMETHODCALLTYPE COBEXEFS::RemoveFolder( LPCTSTR strPath )
{
	return DeleteFile(strPath);
}

int COBEXEFS::Execute(CRequestPtr& ptRequest,int nTimeOut)
{
	m_ptReceiver = new CBuffer;

	ptRequest->Register(this,_T("Received"));	
	m_ReceiveEvent.ResetEvent();
	if (g_RequestManager.EnqueueRequest(ptRequest) == S_OK)
	{
		if(nTimeOut == 0)
			nTimeOut = m_nTimeOut;
		::WaitForSingleObject(m_ReceiveEvent,nTimeOut);
		m_ptReceiver = ptRequest->GetReceiver();
	}
	ptRequest->Unregister(this,_T("Received"));
	
	int nRet = ptRequest->GetReceiverResult(m_ptReceiver);
	
	if(nRet == 0xA0)
		nRet = eEFSRetSuccess;
	else if(nRet == 0x90)
		nRet = eEFSRetSuccessContinue;
	else nRet = eEFSRetFail;

	return nRet;
}

FILETIME COBEXEFS::GetFileTimeFromOleTime(COleDateTime oleDateTime)
{
	FILETIME fileTime;
	memset(&fileTime,0,sizeof(FILETIME));
	if(oleDateTime != 0)
	{
		CTime time(oleDateTime.GetYear(),oleDateTime.GetMonth(),oleDateTime.GetDay(),oleDateTime.GetHour(),oleDateTime.GetMinute(),oleDateTime.GetSecond());
		SYSTEMTIME timeDest;
		time.GetAsSystemTime(timeDest);
		::SystemTimeToFileTime(&timeDest, &fileTime);
	}
	return fileTime;
}

int COBEXEFS::RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{
	return (Register( pNotify, strEvent ) == true)?eEFSRetSuccess:eEFSRetFail;
}


int COBEXEFS::UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{
	return (Unregister( pNotify, strEvent ) == true)?eEFSRetSuccess:eEFSRetFail;
}

bool COBEXEFS::SetPath(LPCTSTR strPath)
{
	bool bRes = false;
	if(m_strCurrentPath != strPath)
	{
		CRequestPtr ptRequest = new COBEXFS_SetFullPath(strPath);
		int nRet = Execute(ptRequest);
		//work around for setpath fail
		if(nRet != eEFSRetSuccess)
		{
			CString strIMEI;
			CRequestPtr spATRequest = new CRequestGetPhoneModel(&strIMEI);
			Execute(spATRequest);
			nRet = Execute(ptRequest);
		}
		bRes = nRet == eEFSRetSuccess;
		assert(bRes);
		if(bRes)
			m_strCurrentPath = strPath;
	}
	else bRes = true;
	return bRes;
}

#pragma warning(pop)