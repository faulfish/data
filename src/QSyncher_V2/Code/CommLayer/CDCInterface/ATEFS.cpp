/*****************************************************************************/
/** @file:		ATEFS.cpp
This is the file that contains the functions related to the ATEFS class.

$Author:   Jame Tsai  $
$Version:		1.0 $
$Date:   Mar 11 2008 13:29:24  $
*/
/*****************************************************************************/
#include "StdAfx.h"
#include "ATEFS.h"
#include "RequestManager.h"
#include "ATFSRequest.h"
#include "..\..\CommonUtil\UnicodeUtility.h"


#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

CATEFS *g_pSingleton = NULL;

IEFS* CreateATIEFS()
{
	if(g_pSingleton == NULL)
		g_pSingleton = new CATEFS();
	return g_pSingleton;
}

void DeleteATIEFS()
{
	if(g_pSingleton)
	{
		delete g_pSingleton;
		g_pSingleton = NULL;
	}
}

const bool IsOK(const CBufferPtr& ptBuffer)
{
	bool bRes = false;
	BYTE szATEnd[6] = {0x0d,0x0a,'O','K',0x0d,0x0a};
	if(ptBuffer->GetLength() == 6)
	{
		BYTE* pTemp = (BYTE*)ptBuffer->GetData();
		bRes = memcmp(pTemp,szATEnd,6) == 0;
	}
	return bRes;
}

CATEFS::CATEFS() : 
m_nTimeOut(3000),
m_dwFileSize(0),
m_nOperationID(-1),
m_DataModeEvent(FALSE,TRUE)
{
	DefineNotify(NOTIFY_GET_FILE_DATA);
	DefineNotify(NOTIFY_ABORT_GET_FILE_DATA);
}

CATEFS::~CATEFS()
{
}


int  CATEFS::RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{ 
	return (QSYNC_LIB::CNotifyBase::Register( pNotify, strEvent ) == true)?eEFSRetSuccess:eEFSRetFail; 
}

int  CATEFS::UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent)
{ 
	return (QSYNC_LIB::CNotifyBase::Unregister( pNotify, strEvent ) == true)?eEFSRetSuccess:eEFSRetFail; 
}

BOOL STDMETHODCALLTYPE CATEFS::QueryCapability(IEFSCapability eIEFSCapability)
{
	return TRUE;
}

void STDMETHODCALLTYPE CATEFS::SetOperationTimeOut(int nTimeOut)
{
	m_nTimeOut = nTimeOut;
}

/*IEFS function description

Process of use get a file asynchronous

0. Regiser "NOTIFY_GET_FILE_DATA" event
1. User should call PutFileAsync() first and wait for "NOTIFY_GET_FILE_DATA" event
2. When receiver the "NOTIFY_GET_FILE_DATA" event, call GetFileData() to get the partial data.

*/
int STDMETHODCALLTYPE CATEFS::GetFileASync(int nOperationID, LPCTSTR szPath, int& nFileSize)
{
	//0. Check if get file finished
	int nRet = eEFSRetFail;

		//2. Reset counter
		m_nHadReceiverFileSize = 0;
		m_nFileSize = 0;

		//3. Clear file buffer first
		{
			CSingleLock singleLock(&m_Critsection,TRUE);
			m_vectBuffer.clear();
		}

		//4. Send get file command
		BOOL bIsDBB = (szPath[0] != _T('/')) ? FALSE : TRUE;

		/*
		File system AT command error codes
		File system will automatically send out an error code of the form “$FSERROR:<error-code>” if it encounters some error when accessing the flash ROM. The error codes are provided by XPIFS, and they are listed as below:
		0        Successful
		-1       File system fails to initialize
		-2       File system has been initialized
		-3       Specified file does not exist
		-4       Checksum of data is wrong
		-5       Size of given buffer is larger
		-6       Size of given buffer is smaller
		-7       Free flash space is not enough
		-8       Unknown parameters
		-9       Specified record does not exist
		-10      Can not handle this file type
		-11      Open too many files/directories
		-12      File/directory already exists
		-13      Directory doesn't exist
		-14      No next item
		-15      Flash program error
		-16      Flash erase error
		-17      Delete split file error
		-18      One or some split file`s segment not found
		-19      Too many objects in the directory
		-20      Directory not empty
		-21      Result of Close_An_Opened_Segment
		-22      Object already be opened
		-23      Whole file too large (> 60KB)
		-24      File can't be rewritten but read only 
		-25      File should not be deleted 
		-26      Destination already be programmed
		-27      Invalid access to specified place 
		-28      Path too deep (>10)
		-29      too many files/directories in root directory (based on volume capacity)
		-30      unsupported volume
		-32767   Unknown error
		-32768   Timeout
		*/
		int nATFileErrCode = 0;
		CRequestPtr	spGetFileRequest = new CRequestATFSGetFile(szPath, bIsDBB,m_nTimeOut,m_nFileSize,nATFileErrCode);
		spGetFileRequest->Register(this,NOTIFY_GET_AT_FILE_DATA);
		nRet = Execute(spGetFileRequest,true,true);
		if(nATFileErrCode == 0)
		{
			nRet = nRet == eEFSRetSuccess ? eEFSRetSuccessContinue : nRet;
			nFileSize = m_nFileSize;
		}
		else
		{
			spGetFileRequest->FreeToken();
		}
	return nRet;
}

/*
Get file data

@param		int nOperationID - the operation ID
@param		BYTE** ppFileData - the file path
@param		DWORD& nFileSize - a reference number to deliver the file size
@param		BOOL& bLastData - a reference number to deliver if this is the 
			last package of data
@return		int - the result of file system method
*/
int STDMETHODCALLTYPE CATEFS::GetFileData(int nOperationID, BYTE** ppFileData, DWORD& nSize, BOOL& bLastData)
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

/*---------------------------------------------------------------------------*/
/*!
Request will call this function to notify ATEFS some specificed event occured

@param		const TSTRING& strEvent - the name of event
@param		nParam - parameter passed from request layer
@return		None
*/
void CATEFS::Event(const TSTRING& strEvent,long nParam)
{
	if(strEvent == NOTIFY_GET_AT_FILE_DATA)
	{
		//0. Get new file partial data
		CBuffer* pData = (CBuffer*)nParam;
		CBufferPtr ptrBuf = new CBuffer(pData->GetLength(),pData->GetData());

		//1. push to vector
		{
			CSingleLock singleLock(&m_Critsection,TRUE);
			m_vectBuffer.push_back(ptrBuf);
			pData->PopFront(ptrBuf->GetLength());
		}

		//2. Count the receive size
		m_nHadReceiverFileSize += ptrBuf->GetLength();

		//3. Notify 
		if(m_nFileSize > 0)
		{
			if(m_nHadReceiverFileSize >= m_nFileSize)
			{
				//TRACE(_T("Had receiver size is %d\n"),m_nHadReceiverFileSize);
				//TRACE(_T("File size is %d\n"),m_nFileSize);
				NotifyPara para(m_nOperationID,0);
				Fire(NOTIFY_GET_FILE_DATA,(long)&para);
			}
			else
			{
				NotifyPara para(m_nOperationID,ptrBuf->GetLength());
				Fire(NOTIFY_GET_FILE_DATA,(long)&para);
			}
		}
	}
	else if(strEvent == _T("Received"))
		m_ReceiveEvent.SetEvent();
	else if(strEvent == NOTIFY_DATA_MODE)
		m_DataModeEvent.SetEvent();
}

/*
@param		int nOperationID - the operation ID
@param		LPCTSTR strPath - the file path
@param		int nFileSize - the file size
@return		int - the result of file system method
*/
int STDMETHODCALLTYPE CATEFS::PutFileAsync(int nOperationID, LPCTSTR strPath, int nFileSize)
{
	int nRet = eEFSRetSuccess;

	if( (strPath == NULL) || nFileSize < 0)
		nRet = eEFSRetFail;
	else
	{
		m_nOperationID = nOperationID;
		m_dwTotalBytesSent = 0;
			
		CString strFolderPath = strPath;
		BOOL bIsDBB = strFolderPath.GetAt(0) == _T('/');

		try
		{
			int nErrCode = 0;
			CRequestPtr spRequest = new CRequestATFSPutFile(strFolderPath, bIsDBB,nFileSize,nErrCode,INFINITE);
			if (Execute(spRequest) == eEFSRetSuccess)
			{
				CBufferPtr spBuffer = spRequest->GetReceiver();
				if(IsOK(spBuffer) == false)
					nRet = eEFSRetFail;
			}
			if(nErrCode != 0)
				nRet = eEFSRetFail;
		}
		catch(...)
		{
			nRet = eEFSRetFail;
		}
	}

	return nRet;
}

/*---------------------------------------------------------------------------*/
/*!
put a block of file data

@param		int nOperationID - the operation ID
@param		BYTE* pFileData - the file data buffer
@param		DWORD nSize - the file size
@param		BOOL bLastData - if this is the last package of data
@return		int - the result of file system method
*/
int STDMETHODCALLTYPE CATEFS::PutFileData(int nOperationID, BYTE* pFileData, DWORD nSize, BOOL bLastData)
{
	int nRet = eEFSRetFail;

	if(nOperationID == m_nOperationID)
	{
		try
		{//Without response until latest package
			CRequestPtr spRequest = new CRequestATFSPutFileData(pFileData,nSize,bLastData == TRUE);
			if (Execute(spRequest,bLastData == TRUE) == eEFSRetSuccess)
			{
				if(bLastData == FALSE)
					nRet = eEFSRetSuccess;
				else
				{
					CBufferPtr spBuffer = spRequest->GetReceiver();
					if(IsOK(spBuffer))
						nRet = eEFSRetSuccess;
				}
			}
			else nRet = eEFSRetSuccess;
		}
		catch(...)
		{
			nRet = eEFSRetFail;
		}
	}

	return nRet;
}

/*---------------------------------------------------------------------------*/
/*!
delete a file

@param		LPCTSTR strPath - the file path
@return		int - the result of file system method
*/
int STDMETHODCALLTYPE CATEFS::DeleteFile(LPCTSTR strPath)
{
	return SimpleFunc<CRequestATFSDeleteFile>(strPath);
}

template <class funcClass>
int CATEFS::SimpleFunc(LPCTSTR strPath1,LPCTSTR strPath2,int nTimeOut)
{
	int nRet = eEFSRetFail;

	try
	{
		CRequestPtr spRequest = new funcClass(strPath1,strPath2, strPath1[0] == _T('/'),nTimeOut);
		if (Execute(spRequest) == eEFSRetSuccess)
		{
			CBufferPtr spBuffer = spRequest->GetReceiver();
			if(IsOK(spBuffer))
				nRet = eEFSRetSuccess;
		}
	}
	catch(...)
	{
		nRet = eEFSRetFail;
	}

	return nRet;
}

template <class funcClass>
int CATEFS::SimpleFunc(LPCTSTR strPath,int nTimeOut)
{
	int nRet = eEFSRetFail;	

	try
	{
		CRequestPtr spRequest = new funcClass(strPath, strPath[0] == _T('/'),nTimeOut);
		if (Execute(spRequest) == eEFSRetSuccess)
		{
			CBufferPtr spBuffer = spRequest->GetReceiver();
			if(IsOK(spBuffer))
				nRet = eEFSRetSuccess;
		}
	}
	catch(...)
	{
		nRet = eEFSRetFail;
	}

	return nRet;
}


int STDMETHODCALLTYPE CATEFS::GetFolderEntries(LPCTSTR szPath, vector<WIN32_FIND_DATA> *vEntryList)
{
	int nRet = eEFSRetFail;

	if(szPath)
	{
		BOOL		bIsDBB = (szPath[0] != _T('/')) ? FALSE : TRUE;
		string		strFolders;
		CRequestPtr spRequest = new CRequestATFSListDirectory(szPath, bIsDBB,strFolders);

		if (Execute(spRequest) == eEFSRetSuccess)
		{
            
            size_t findPos1 = strFolders.find("?Continue?");
            if(findPos1 != string::npos)
                strFolders = strFolders.substr(0, findPos1);
            string strFolders2;
            size_t findPos2 = 1;
            while(findPos1 != string::npos) {
                CRequestPtr spRequest = new CRequestATFSListDirectory(szPath, bIsDBB, strFolders2, TRUE);
                if(Execute(spRequest) == eEFSRetSuccess) {
                    if(strFolders2.find("?Continue?") == string::npos) {
                        strFolders += strFolders2;
                        break;
                    } else {
                        strFolders += strFolders2.substr(0, strFolders2.find("?Continue?"));
                    }
                }
            }
			CString sListResult = QSYNC_LIB::UTF8ToUCS2(strFolders);

			WIN32_FIND_DATA fdata;
			while (ParseEntryList(sListResult, fdata))
				vEntryList->push_back(fdata);

			nRet = eEFSRetSuccess;
		}
	}

	return nRet;
}

int STDMETHODCALLTYPE CATEFS::CreateFolder(LPCTSTR strPath)
{
	return SimpleFunc<CRequestATFSCreateFolder>(strPath);
}


int STDMETHODCALLTYPE CATEFS::RemoveFolder(LPCTSTR strPath)
{
	return SimpleFunc<CRequestATFSRemoveDirectory>(strPath);
}

BOOL CATEFS::ParseEntryList(CString& strList, WIN32_FIND_DATA& fdata)
{
	//FSDIR=15
	//<"Themes","R-",0><"Games","R-",0><"Applications","R-",0><"Miscellaneous","R-",0><"Pictures","R-",0><"Sounds","R-",0><"Videos","R-",0>EOF

	//FSDIR=6
	//<Games,"R-",0><Applications,"R-",0><Miscellaneous,"R-",0><Pictures,"R-",0><Sounds,"R-",0><Videos,"R-",0>EOF

	//|Postcard.3gp,"RW",693431,2007/09/07,00:55:39,/brew/mod/cmgr/cs/sys/vid/usr/Postcard.3gp||Romantic period.3gp,"RW",1071923,2007/09/07,00:55:39,/brew/mod/cmgr/cs/sys/vid/usr/Romantic period.3gp||Video ringtone Ukulele.3gp,"RW",265333,2007/09/07,00:55:39,/brew/mod/cmgr/cs/sys/vid/usr/Videoringtone Ukulele.3gp|EOF

	if (strList.IsEmpty() || (strList == _T("EOF")))
		return FALSE;

	memset(&fdata,0,sizeof(WIN32_FIND_DATA));

	TCHAR Tag = strList.GetAt(0);

	switch (Tag)
	{
	case _T('|'): //Whole file.
	case _T('*'): // Split file
		fdata.dwFileAttributes = 0;
		break;
	case _T('<'): // Directory.
		fdata.dwFileAttributes = FILE_ATTRIBUTE_DIRECTORY;
		Tag = _T('>');
		break;
	case _T('?'): // Extern Parameter
		break;
	default:
		Tag = 0;
		break;
	};

	CStringArray Tokens;
	int nFound = -1;
	if(Tag != 0)
		nFound = strList.Find(Tag,1);
	if(nFound != -1)
	{
		CString sDataInfo = strList.Mid(1,nFound - 1);
		strList = strList.Mid(nFound + 1);
		ParseItem(sDataInfo,Tokens);

		if(fdata.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			_tcsncpy(fdata.cFileName,(LPCTSTR)Tokens.GetAt(0),MAX_PATH - 1);
		else
		{
			assert(Tokens.GetCount() >= 6);
			if(Tokens.GetCount() >= 6)
			{
				if(Tokens.GetAt(5).IsEmpty())
					_tcsncpy(fdata.cFileName,(LPCTSTR)Tokens.GetAt(0),MAX_PATH - 1);
				else
					_tcsncpy(fdata.cFileName,(LPCTSTR)Tokens.GetAt(5),MAX_PATH - 1);
			}
		}

		CString sAttr = Tokens.GetAt(1);
		sAttr.TrimLeft(_T('\"'));
		sAttr.TrimRight(_T('\"')); 
		if(sAttr == _T("RW"))
			fdata.dwFileAttributes |= FILE_ATTRIBUTE_NORMAL;
		else if(sAttr == _T("-W"))
			fdata.dwFileAttributes |= FILE_ATTRIBUTE_HIDDEN;
		else if(sAttr == _T("R-"))
			fdata.dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
		else
			fdata.dwFileAttributes |= (FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_READONLY);

		fdata.nFileSizeLow = _ttoi((LPCTSTR)Tokens.GetAt(2));
	}

	if(Tag == _T('|') || Tag == _T('*'))
	{
		CString sDate = Tokens.GetAt(3);
		sDate.TrimLeft(_T('\"'));
		sDate.TrimRight(_T('\"')); 
		CStringArray datePart;
		while((nFound = sDate.Find(_T('/'))) != -1)
		{
			datePart.Add(sDate.Left(nFound));
			sDate = sDate.Mid(nFound + 1);
		}
		datePart.Add(sDate);
		ASSERT(datePart.GetCount() == 3);

		CString sTime = Tokens.GetAt(4);
		sTime.TrimLeft(_T('\"'));
		sTime.TrimRight(_T('\"')); 
		CStringArray timePart;
		while((nFound = sTime.Find(_T(':'))) != -1)
		{
			timePart.Add(sTime.Left(nFound));
			sTime = sTime.Mid(nFound + 1);
		}
		timePart.Add(sTime);
		ASSERT(timePart.GetCount() == 3);

		CTime localTime(_ttoi(datePart.GetAt(0)),
			_ttoi(datePart.GetAt(1)),
			_ttoi(datePart.GetAt(2)),
			_ttoi(timePart.GetAt(0)),
			_ttoi(timePart.GetAt(1)),
			_ttoi(timePart.GetAt(2)));
		SYSTEMTIME sysTm;
		localTime.GetAsSystemTime(sysTm);
		FILETIME  fTm;
		::SystemTimeToFileTime(&sysTm,&fTm);
		fdata.ftCreationTime = fTm;
		fdata.ftLastWriteTime = fTm;
		fdata.ftLastAccessTime = fTm;
	}

	return TRUE;
}

void CATEFS::ParseItem(const CString& sDataInfo,CStringArray& Tokens)
{
	//處理資料夾時字串格式為 "檔名","屬性",0 舉例如下
	//"Applications","R-",0

	// 若處理檔案時則字串格式為 "檔名","屬性",大小,日期,時間,路徑 舉例如下
	// "檔名有,的檔案.txt","RW",12,2007/01/02,12:16:22,/brew/user/Themes/檔名有,的檔案.txt

	bool bStatrtDoubleQuotes = false;
	CString strToken;
	int nCommaNumber = 0;

	int nIndex = -1;
	while(sDataInfo.GetLength() > nIndex)
	{
		nIndex++;
		switch(sDataInfo.GetAt(nIndex))
		{
		case '\"':
			bStatrtDoubleQuotes = !bStatrtDoubleQuotes;
			nCommaNumber = 0;
			if(bStatrtDoubleQuotes == false)
			{
				Tokens.Add(strToken);
				strToken.Empty();
			}
			break;
		case ',':
			++nCommaNumber;
			if(bStatrtDoubleQuotes)
				strToken += sDataInfo.GetAt(nIndex);
			else if(strToken.IsEmpty() == false)
				if(nCommaNumber <= 4)//資料都是以逗號隔開,而最後一項(即DoubleQuotes之後的第5項)為檔案路徑,可能會帶有逗號,所以要特別區隔開來
				{
					Tokens.Add(strToken);
					strToken.Empty();
				}
				else
					strToken += sDataInfo.GetAt(nIndex);
			break;
		default:
			strToken += sDataInfo.GetAt(nIndex);
			break;
		};
	};
	Tokens.Add(strToken);

	//以下是舊版,當檔名有逗號時會有問題

	//bool bStatrtToken = false;
	//CString strToken;
	//TCHAR cPreviousTag = 0;
	//while(sDataInfo.GetLength() > nIndex)
	//{
	//	nIndex++;
	//	switch(sDataInfo.GetAt(nIndex))
	//	{
	//	case '\"':
	//		if(bStatrtToken)
	//		{
	//			if(!(cPreviousTag == ',' || cPreviousTag == 0))
	//			{
	//				Tokens.Add(strToken);
	//				strToken.Empty();
	//				bStatrtToken = false;
	//			}
	//		}
	//		else
	//		{
	//			bStatrtToken = true;
	//		}
	//		break;
	//	case ',':
	//		if(bStatrtToken)
	//		{
	//				Tokens.Add(strToken);
	//				strToken.Empty();
	//		}
	//		else
	//		{
	//			bStatrtToken = true;
	//		}
	//		break;
	//	default:
	//		strToken += sDataInfo.GetAt(nIndex);
	//		break;
	//	};
	//	cPreviousTag = sDataInfo.GetAt(nIndex);
	//};

	//if(bStatrtToken)
	//{
	//	Tokens.Add(strToken);
	//	strToken.Empty();
	//}
}

int CATEFS::Execute(CRequestPtr& ptRequest,bool bWithResponse,bool bWithDataMode)
{
	int nRet = eEFSRetFail;

	if(bWithResponse)
	{
		ptRequest->Register(this,_T("Received"));
		m_ReceiveEvent.ResetEvent();

		if(bWithDataMode)
		{
			ptRequest->Register(this,NOTIFY_DATA_MODE);
			m_DataModeEvent.ResetEvent();
		}
	}

	if (g_RequestManager.EnqueueRequest(ptRequest,bWithDataMode == false) == S_OK)
	{
		if(bWithResponse)
		{
			if(bWithDataMode == false)
			{
				ptRequest->Unregister(this,_T("Received"));
				if(ptRequest->GetReceiverResult(ptRequest->GetReceiver()))
					nRet = eEFSRetSuccess;
			}
			else nRet = eEFSRetSuccess;
		}
		else nRet = eEFSRetSuccess;
	}

	return nRet;
}

BOOL STDMETHODCALLTYPE CATEFS::Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath )
{
	return SimpleFunc<CRequestATFSRenameFile>(strNewFullPath,strOldFullPath);
}

#pragma warning(pop)