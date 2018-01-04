#include "StdAfx.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "CQMEFileTreeDataSource.h"
#include "CMEFileNode.h"
#include "FileManager_CommonLib.h"
#include "..\..\CommonUtil\CursorUtility.h"
#include "Shlwapi.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "FileManager_VersionView2.h"

#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)

using namespace QSYNC_LIB;

const CString theAppName = _T("QSyncher");

CQMEFileTreeDataSource::CQMEFileTreeDataSource()
:m_pIEFS(NULL)
,m_nUploadFileBlocks(0)
,m_nUploadedFileBlocks(0)
,m_pILocalization(NULL)
,m_DownloadAvailableEvent(FALSE,TRUE)
,m_nTotalDownloadSize(0)
,m_nDownloadedSize(0)
,m_bFileOpen(false)
,m_hParent(NULL)
,m_nTempProgress(0)
{	
	CreateILocalization(&m_pILocalization);

	m_DownloadAvailableEvent.SetEvent();

	DefineNotify(EVENT_TREE_DATA_DELETE_NODE);
	DefineNotify(EVENT_TREE_DATA_INSERT_NODE);
	DefineNotify(EVENT_TREE_DATA_UPDATE_NODE);
	DefineNotify(EVENT_TREE_DATA_EXPAND_NODE);
	DefineNotify(EVENT_PROGRESS_TASK_UPDATE);
}

CQMEFileTreeDataSource::~CQMEFileTreeDataSource(void)
{
	DeleteFileManagerInterface();
}

bool CQMEFileTreeDataSource::CreateNode(CQTreeNode* pNode)
{
	bool	bRet = false;
	if(pNode != NULL)
	{
		if(CITreeDatasourceBase::GetRoot() == NULL) //Root
			SetRootNode(pNode);

		CMEFileNode* pFNode = static_cast<CMEFileNode*>(pNode);
		if( (pFNode && pFNode->IsFolder()) || pFNode->IsRoot())
			Fire(EVENT_TREE_DATA_INSERT_NODE, (long)pNode);

		bRet = true;		
	}
	return bRet;
}

bool CQMEFileTreeDataSource::ExpandNode(CQTreeNode* pNode)
{
	bool bRes = false;    
	GetIEFS();
	CMEFileNode* pFNode = static_cast<CMEFileNode*>(pNode);
	if( (pFNode && pFNode->IsFolder()) || pFNode->IsRoot())
		bRes = ExpandTwoLevel(pFNode);
	return bRes;
}

bool CQMEFileTreeDataSource::ExpandOneLevel(CMEFileNode* pFNode)
{
	bool	bRet = false;
	if(pFNode && m_pIEFS && pFNode->IsFolder())
	{
		if(pFNode->m_bExpanded == true)
			bRet = true;
		else
		{			
			if(!pFNode->GetFullPath().empty())
			{
				vector<WIN32_FIND_DATA> vEntryList;
				int nRet = m_pIEFS->GetFolderEntries(pFNode->GetFullPath().c_str(), &vEntryList);
				if( nRet == eEFSRetSuccess )//error
				{
					pFNode->m_bExpanded = true;
					bRet = true;

					for(int i = 0; i < static_cast<int>(vEntryList.size()); i++)
					{
						if(vEntryList[i].dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
							continue;

						CMEFileNode* pNewNode = new CMEFileNode(pFNode);

						TSTRING strFileName = vEntryList[i].cFileName;
						if(TSTRING::npos == strFileName.find(pNewNode->GetSeparator()))
						{
							pNewNode->SetDisplayName(vEntryList[i].cFileName);
						}
						else
						{
							TSTRING::size_type nIndex = strFileName.rfind(pNewNode->GetSeparator());
							TSTRING strName = strFileName.substr(nIndex+1);
							pNewNode->SetVirtualPath(strFileName);								
							pNewNode->SetDisplayName(strName.c_str());		
						}
						pNewNode->SetName(pNewNode->GetDisplayName().c_str());

						pNewNode->SetFileSize(vEntryList[i].nFileSizeLow);
						pNewNode->SetFileTime(vEntryList[i].ftLastWriteTime);
						pNewNode->SetFileAttr(vEntryList[i].dwFileAttributes);
						CreateNode(pNewNode);
					}//for end
				}//if end
				else
				{
					ShowMessage(m_hParent,m_pILocalization->GetString(_T("IDS_FileMgr_LIST_FOLDER_FAIL")),m_pILocalization->GetString(_T("IDS_FILEMANAGER_FUNCTION")));
				}
			}
		}
	}
	return bRet;
}

bool CQMEFileTreeDataSource::ExpandTwoLevel(CMEFileNode* pNode)
{
	bool bRet = false;
	if(pNode  && pNode->IsFolder())
	{
		CCursorWait wait;
		if(ExpandOneLevel(pNode))
		{
			pNode->SetNodeHasChild(true);
			int nSize = pNode->GetChildSize();
			if( nSize > 0)
			{
				CMEFileNode* pSubNode = NULL;
				for(int i = 0; i < nSize; i++)
				{
					CMEFileNode* pSubNode = static_cast<CMEFileNode*>(pNode->GetChild(i));
					if(pSubNode != NULL && pSubNode->IsFolder())
					{
						bRet = ExpandOneLevel(pSubNode);
						if(bRet == false)
							break;
					}					
				}//for end
			}//if end
		}
	}
	return bRet;
}

bool CQMEFileTreeDataSource::DeleteNode(CQTreeNode* pNode)
{
	return RecursiveDelNode(pNode, true);
}

bool CQMEFileTreeDataSource::RecursiveDelFile(CQTreeNode* pNode)//移除file時也會移除node
{
	bool bRes = true;
	while(pNode->GetChildSize() > 0 && bRes)
	{
		CQTreeNode *pChileNode = pNode->GetChild(0);
		if(pChileNode)
			bRes = RecursiveDelFile(pChileNode);
		//這會移除pNode裡的m_vct_pChilds裡第一個node,所以pNode->GetChildSize()會確實減少
		else break;
	}

	if(bRes)
	{
		CMEFileNode *pFENode = static_cast<CMEFileNode*>(pNode);
		bRes = DelMEData(pFENode);
	}

	return bRes;
}

bool CQMEFileTreeDataSource::RecursiveDelNode(CQTreeNode* pNode, bool bDelFlag/*true代表檔案與node都要殺,false代表只殺node*/)
{
	bool bRet = false;
	if(pNode)
	{
		bool bIsRoot = pNode->IsRoot();
		if(bDelFlag)
			bRet = RecursiveDelFile(pNode);
		else
			bRet = RecursiveDeleteNode(pNode);
		if(bIsRoot)
			SetRootNode(NULL);
	}
	return bRet;
}

CQTreeNode* CQMEFileTreeDataSource::GetRoot()
{
	GetIEFS();
	if(CITreeDatasourceBase::GetRoot() == NULL)
	{
		CMEFileNode* pRetRoot = new CMEFileNode(NULL);
		pRetRoot->SetDisplayName(_T("IDS_FileMgr_ROOT"));
		pRetRoot->SetName(ME_FILE_PATH_SEPARATOR_STR);
		pRetRoot->SetNodeAsFolder();
		pRetRoot->SetNodeHasChild(true);
		CreateNode(pRetRoot);
	}
	return  CITreeDatasourceBase::GetRoot();
}

bool CQMEFileTreeDataSource::UpdateNode(CQTreeNode* pNode)
{
	bool bRet = false;
	ASSERT(0);
	return bRet;
}

int CQMEFileTreeDataSource::WriteDownloadFileBlock(int nID,BOOL& bLast,bool bFlush)
{
	DWORD nSize = 0;
	BYTE *pFileData = NULL;
	if(m_pIEFS->GetFileData(nID,&pFileData,nSize,bLast))
	{
		if(nSize > 0)
		{			
			if(bFlush)
			{
				CBufferPtr ptFileBuffer = new CBuffer;				
				for(std::vector<CBufferPtr>::iterator Iter = m_vectBuffer.begin();Iter != m_vectBuffer.end();Iter++)
				{
					ptFileBuffer = *Iter;
					m_newFile.Write(ptFileBuffer->GetData(),ptFileBuffer->GetLength());
				}
				m_vectBuffer.clear();
				m_newFile.Write(pFileData,nSize);				
			}
			else
			{
				CBufferPtr ptrBuf = new CBuffer(nSize,(const char *)pFileData);
				m_vectBuffer.push_back(ptrBuf);
			}
			TRACE(_T("Write %d data to  %s for download successful.\n"),nSize,m_strNewFile);
			if(pFileData)
				delete [] pFileData;
		}
	}
	return nSize;
}

void CQMEFileTreeDataSource::Event(const TSTRING& strEvent,long nParam)
{
	if(strEvent == NOTIFY_GET_FILE_DATA)
	{
		assert(m_bFileOpen == true);
		NotifyPara* pPara = (NotifyPara*)nParam;
		if(pPara->m_nSize == 0)
		{//Succeed finish to get file
			BOOL bLast = FALSE;
			while(bLast == FALSE)
			{
				WriteDownloadFileBlock(pPara->nOperationID,bLast,true);
			};
			
			NotifyProgress((LPCTSTR)m_strNewFile,m_nDownloadedSize*100/m_nTotalDownloadSize);
			m_newFile.Close();
			m_bFileOpen = false;
			m_DownloadAvailableEvent.SetEvent();
			TRACE(_T("Close %s for download successful.\n"),m_strNewFile);
		}
		else
		{
			m_nDownloadedSize += pPara->m_nSize;
			int nNewProgress = ((m_nDownloadedSize)*100)/m_nTotalDownloadSize;
			if(m_nTempProgress < nNewProgress && (nNewProgress%1) == 0)
			{
				m_nTempProgress = nNewProgress;
				NotifyProgress((LPCTSTR)m_strNewFile,m_nTempProgress);
			}
		}
	}
	else if(strEvent == NOTIFY_ABORT_GET_FILE_DATA)
	{
		AbortDownload();
	}
}

void CQMEFileTreeDataSource::AbortDownload()
{	
	TRACE(_T("Close %s for download successful.\n"),m_strNewFile);
	if(m_bFileOpen)
	{
		m_newFile.Close();
		DeleteFile(m_strNewFile);
	}
	m_bFileOpen = false;	
	m_DownloadAvailableEvent.SetEvent();
}

bool CQMEFileTreeDataSource::GetDeviceConnection(void)
{
	return GetIEFS() != NULL;
}

bool CQMEFileTreeDataSource::NewFolder(const CString& strFolder)
{
	bool bRet = false;
	if(m_pIEFS && !(strFolder.IsEmpty()))
	{
		CCursorWait wait;

		bRet = (m_pIEFS->CreateFolder(strFolder) == eEFSRetSuccess);
	}
	return  bRet;
}

bool CQMEFileTreeDataSource::DelFolder(CQTreeNode* pNode)
{
	bool bRet = false;
	CMEFileNode* pFNode = (CMEFileNode*)pNode;
	if(pFNode->IsRoot())
		bRet = true;
	else if(m_pIEFS != NULL && pFNode != NULL && pFNode->IsFolder() )
	{
		CCursorWait wait;

		CString strFolder = pFNode->GetFullPath().c_str();

		if(!strFolder.IsEmpty())
			bRet = m_pIEFS->RemoveFolder(strFolder) == eEFSRetSuccess;
	}
	if(bRet)
	{
		Fire(EVENT_TREE_DATA_DELETE_NODE, (long)pNode);
		delete pNode;
	}
	return  bRet;
}

bool CQMEFileTreeDataSource::CanRenameFile(const CString& strFolder,CMEFileNode *pNode)
{
	return !pNode->IsFolder();
}

bool CQMEFileTreeDataSource::RenameFile(const CString& strFolder,CMEFileNode *pNode,const CString& strNewFileName)
{	
	bool bRes = false;
	if(m_pIEFS && !strNewFileName.IsEmpty())
	{
		CCursorWait wait;

		CString strOld = pNode->GetFullPath().c_str();
		CString strNew = strFolder + ME_FILE_PATH_SEPARATOR_STR + strNewFileName;
		bRes = m_pIEFS->Rename(strOld,strNew) == TRUE;
		if(bRes)
		{
			TSTRING strVirtualPath = pNode->GetVirtualPath();
			TSTRING::size_type nIndex = strVirtualPath.rfind(ME_FILE_PATH_SEPARATOR_STR);
			strVirtualPath = strVirtualPath.substr(0,nIndex+1);
			pNode->SetVirtualPath(strVirtualPath + LPCTSTR(strNewFileName));

			pNode->SetName(LPCTSTR(strNewFileName));
			pNode->SetDisplayName(LPCTSTR(strNewFileName));
		}
	}
	return bRes;
}

bool CQMEFileTreeDataSource::CanRenameFolder(const CString& strFolder,CMEFileNode *pNode)
{
	/*
	For Qualcomm issue, rename folder will cause other problems.
	It should rename all subitems on the folder when use BenQ AT EFS command.
	It could be fine to rename when you use OBEX EFS command.
	So maybe we can open it when we can judge what's command use.
	*/
	return false;
}

bool CQMEFileTreeDataSource::RenameFolder(CMEFileNode *pNode,const CString& strNewFolderName)
{
	bool bRes = false;
	if(m_pIEFS && !strNewFolderName.IsEmpty() && !pNode->IsRoot())
	{
		CCursorWait wait;

		CString strOld = pNode->GetFullPath().c_str();
		int nIndex = strOld.ReverseFind(_T('/'));
		CString strNew = strOld.Left(nIndex+1) + strNewFolderName;
		bRes = m_pIEFS->Rename(strOld,strNew) == TRUE;
		if(bRes)
		{
			pNode->SetName(strNewFolderName);
			pNode->SetDisplayName(strNewFolderName);
		}
	}
	return bRes;
}

int CQMEFileTreeDataSource::GetMESize(void)
{
	ASSERT(0);
	return 0;
}

IEFS* CQMEFileTreeDataSource::GetIEFS(void)
{
	if (m_pIEFS == NULL )
		m_pIEFS = (IEFS*)CommCreateInterface(theAppName,_T("FileSystem"),eIEFS);
	return m_pIEFS;
}

bool CQMEFileTreeDataSource::IsIEFSConnecting(void)
{
	return false;
}

void CQMEFileTreeDataSource::NotifyProgress(const TSTRING& strDisplay , int nPos)
{
	QProgressNotify updateNotify;
	updateNotify.m_nCurrentPosition = nPos;
	updateNotify.m_strCurrentDisplayText = strDisplay.c_str();
	Fire(EVENT_PROGRESS_TASK_UPDATE,(long)&updateNotify);
}

CMEFileNode* CQMEFileTreeDataSource::UploadOneFile_PC2ME(CEvent& aboutEvent,CMEFileNode* pNode,CString strSrcPCFilePath, CString strDstMEFolderPath, UINT &nFileSize)
{
	CMEFileNode* pNewNode = NULL;
	bool bRet = false;

	CString strMEFileName;
	if(!(strSrcPCFilePath.IsEmpty() || strDstMEFolderPath.IsEmpty()))//Check file path string
	{
		CString strPCFileName = GetPCFileName(strSrcPCFilePath);
		if(!strPCFileName.IsEmpty())
			strMEFileName = strDstMEFolderPath + ME_FILE_PATH_SEPARATOR_STR + strPCFileName;
	}

	CFile file;
	if(file.Open(strSrcPCFilePath, CFile::modeRead) && m_pIEFS)//check PC file open status
	{
		nFileSize = (UINT)file.GetLength();
		UINT nTransferedSize = 0;
		int nRetFilePut = 0;    

		CFileStatus rFileStatus;
		//2008/2/19 alan tu
		//We need support to upload file with 0 size
		if(file.GetStatus(rFileStatus))
		{
			nRetFilePut = m_pIEFS->PutFileAsync( NULL, strMEFileName, nFileSize);
			if(nRetFilePut == eEFSRetSuccess)
			{
				UINT FilePutTimes = 0;
				if( (nFileSize % MAX_FILE_PACKAGE_SIZE) == 0)
					FilePutTimes = nFileSize / MAX_FILE_PACKAGE_SIZE;
				else
					FilePutTimes = nFileSize / MAX_FILE_PACKAGE_SIZE + 1;

				if(FilePutTimes == 0)
					bRet = true;//Special case, file size is 0

				for(UINT i = 1; i <= FilePutTimes; i++)
				{
					if(::WaitForSingleObject(aboutEvent,0) == WAIT_OBJECT_0)
					{
						if(m_pIEFS->CanCancel())
							m_pIEFS->Cancel();
						bRet = false;
						break;
					}
					//1. Update progress status
					int nFinishPos = static_cast<int>(((m_nUploadedFileBlocks++)*100)/m_nUploadFileBlocks);
					NotifyProgress((LPCTSTR)strSrcPCFilePath,nFinishPos);

					//2. Get data from pc file and then put data on Target
					file.Seek((i-1)*MAX_FILE_PACKAGE_SIZE, CFile::begin);
					BYTE* pFileData = new BYTE[MAX_FILE_PACKAGE_SIZE];
					if(pFileData)
					{
						UINT nFileReadSize = file.Read(pFileData, MAX_FILE_PACKAGE_SIZE);
						nTransferedSize += nFileReadSize;
                        if(m_pIEFS)
						    nRetFilePut = m_pIEFS->PutFileData(NULL, pFileData, nFileReadSize, true );
                        else
                            nRetFilePut = eEFSRetFail;
						delete[] pFileData;
					}

					//3. Get the result of put data to target (2.)
					switch(nRetFilePut)
					{
					case eEFSRetFail:case eEFSFileNotExist:case eEFSDeviceEmpty:
                        //Sleep(3000);
						bRet = false;
						break;
					case eEFSRetSuccessContinue:case eEFSRetSuccess:
						bRet = true;
						break;
					default:
						bRet = false;
						break;
					};//switch end
					if(bRet == false)
						break;//break for loop
				};//for loop end			
			}
		}
		
		file.Close();

		if(bRet)
		{
			CFileFind finder;
			BOOL bResult = finder.FindFile(strSrcPCFilePath);
			if (bResult)
			{
				finder.FindNextFile();
				pNewNode = new CMEFileNode(pNode);
				pNewNode->SetDisplayName(GetPCFileName(strSrcPCFilePath));
				pNewNode->SetName(GetPCFileName(strSrcPCFilePath));
				pNewNode->SetFileSize(nTransferedSize);

				FILETIME refTime;
				finder.GetLastWriteTime(&refTime);

				pNewNode->SetFileTime(refTime);
				//pNewNode->SetFileAttr(vEntryList[i].dwFileAttributes);
			}
		}
	}
	return pNewNode;
}

bool CQMEFileTreeDataSource::UploadFilesIntoME(CEvent& aboutEvent,CQTreeNode *pNode, vector<CString> &vct_strFilePath,HWND hParent)
{
	CCursorWait wait;

	bool bRet = false;
	if(pNode && vct_strFilePath.size() > 0)
	{
		//1. Reset count for progress
		m_nUploadFileBlocks = 0;
		m_nUploadedFileBlocks = 0;

		//2. Calculate total for progress
		CString strMEFolder = ((CMEFileNode*)pNode)->GetFullPath().c_str();
		for(size_t nIndex = 0; nIndex < vct_strFilePath.size(); nIndex++)
		{
			CFile file;
			if(file.Open(vct_strFilePath[nIndex], CFile::modeRead))
			{
				ULONGLONG nFileSize = file.GetLength();
				ULONGLONG nFilePutTimes = 0;
				if( (nFileSize % MAX_FILE_PACKAGE_SIZE) == 0)
					nFilePutTimes = nFileSize / MAX_FILE_PACKAGE_SIZE;
				else
					nFilePutTimes = nFileSize / MAX_FILE_PACKAGE_SIZE + 1;
				m_nUploadFileBlocks += static_cast<int>(nFilePutTimes);

				file.Close();
			}			
		}

		//3. Start to upload file
		CMEFileNode* pFNode = static_cast<CMEFileNode*>(pNode);
		vector<CMEFileNode*> vectNewNodes;
		for(size_t nIndex = 0; nIndex < vct_strFilePath.size(); nIndex++)
		{
			//檢查是否有同名檔案
			bool bExistNamesakeFile = false;
			int nReplaceable = 0;
			int nNamesakeInME = -1;
			for(int i = 0; i < pNode->GetChildSize(); ++i)
			{
				CString strFileName = pNode->GetChild(i)->GetDisplayName().c_str();
				if(PathFindFileName(vct_strFilePath[nIndex]) == strFileName)
				{
					bExistNamesakeFile = true;
					nNamesakeInME = i;
					TSTRING strPrompt = strFileName;
					strPrompt += _T("\n") + m_pILocalization->GetString(_T("IDS_FileMgr_OVERWRITE_FILE_PROMPT"));
					nReplaceable =  ShowMessage(hParent,strPrompt, _T("IDS_FILEMANAGER_FUNCTION"), QMB_YESNOCANCEL);
					break;
				}
			}

			//沒有同名檔案
			if(bExistNamesakeFile == false)  
			{
				UINT nFileSize = 0;			
				CMEFileNode* pNewNode = UploadOneFile_PC2ME(aboutEvent,pFNode,vct_strFilePath[nIndex], strMEFolder ,nFileSize);
				if(pNewNode)
				{
					CreateNode(pNewNode);
					vectNewNodes.push_back(pNewNode);
					bRet = true;
				}
				else
				{//Upload file fail
					ShowMessage(hParent,_T("IDS_FileMgr_UPLOAD_FILE_FAIL"), _T("IDS_FILEMANAGER_FUNCTION"), QMB_OK);
					bRet = false;
					break;  //有錯誤就跳出迴圈
				}
			}

			//有同名檔案:
			else if(nReplaceable == QIDYES)
			{
				if(nNamesakeInME != -1)
					DelFile(pNode->GetChild(nNamesakeInME));
				UINT nFileSize = 0;
				CMEFileNode* pNewNode = UploadOneFile_PC2ME(aboutEvent,pFNode,vct_strFilePath[nIndex], strMEFolder ,nFileSize);
				if(pNewNode)
				{
					CreateNode(pNewNode);
					vectNewNodes.push_back(pNewNode);
					bRet = true;
				}
				else
				{//Upload file fail
					ShowMessage(hParent,_T("IDS_FileMgr_UPLOAD_FILE_FAIL"), _T("IDS_FILEMANAGER_FUNCTION"), QMB_OK);
					bRet = false;
					break;  //有錯誤就跳出迴圈
				}
			}

			else if(nReplaceable == QIDNO) 
			{
				if(nNamesakeInME != -1)
				{
					CFile file;
					TSTRING strNamesakeFilePath = vct_strFilePath[nIndex];
					if(file.Open(strNamesakeFilePath.c_str(), CFile::modeRead))
					{
						ULONGLONG nFileSize = file.GetLength();
						ULONGLONG nFilePutTimes = 0;
						if( (nFileSize % MAX_FILE_PACKAGE_SIZE) == 0)
							nFilePutTimes = nFileSize / MAX_FILE_PACKAGE_SIZE;
						else
							nFilePutTimes = nFileSize / MAX_FILE_PACKAGE_SIZE + 1;
						m_nUploadedFileBlocks += static_cast<int>(nFilePutTimes);

						file.Close();
					}			

					int nFinishPos = static_cast<int>(m_nUploadedFileBlocks*100/m_nUploadFileBlocks);
					NotifyProgress(strNamesakeFilePath.c_str(),nFinishPos);
				}
				continue;
			}
			else if(nReplaceable == QIDCANCEL) 
			{
				break;
			}
		}// End for loop

		for(vector<CMEFileNode*>::iterator Iter = vectNewNodes.begin(); Iter != vectNewNodes.end();Iter++)
		{
			CMEFileNode* pNewNode = *Iter;

			vector<WIN32_FIND_DATA> vEntryList;
			int nRet = m_pIEFS->GetFolderEntries(pFNode->GetFullPath().c_str(), &vEntryList);
			if( nRet == eEFSRetSuccess )
			{
				vector<WIN32_FIND_DATA>::size_type nSize = vEntryList.size();
				for(vector<WIN32_FIND_DATA>::size_type i = 0; i < nSize; i++)
				{
					TSTRING strFileName = vEntryList[i].cFileName;
					if(TSTRING::npos != strFileName.find(pNewNode->GetSeparator()))
					{
						TSTRING::size_type nIndex = strFileName.rfind(pNewNode->GetSeparator());
						TSTRING strName = strFileName.substr(nIndex+1);
						if(_tcsicmp(strName.c_str(),pNewNode->GetName().c_str()) == 0)
						{
							pNewNode->SetVirtualPath(strFileName);
							break;
						}
					}
				}
			}
		}
	}
	return bRet;
}

bool CQMEFileTreeDataSource::DownloadOneFile_ME2PC(CEvent& aboutEvent,const CString& strSrcMEPath, const CString& strPCFolder,HWND hParent)
{
	bool bRes = false;
	int nIndex = strSrcMEPath.ReverseFind(_T('/'));
	if(m_pIEFS && nIndex != -1)
	{
		CString strName = strSrcMEPath.Mid(nIndex+1);
		TSTRING strNewFolder = strPCFolder;
		if(strNewFolder [ strNewFolder.length () - 1] != _T('\\'))
			strNewFolder += _T('\\');
		m_strNewFile = strNewFolder.c_str() + strName;
		
		assert(m_bFileOpen == false);

		bool bExist = false;
		MESSAGERETURN nRet = QIDYES;
		if(::_taccess_s(m_strNewFile,0) != ENOENT)
		{
			TSTRING strPrompt = m_strNewFile;
			strPrompt += _T("\n") + m_pILocalization->GetString(_T("IDS_FileMgr_OVERWRITE_FILE_PROMPT"));
			nRet = ShowMessage(hParent,strPrompt,m_pILocalization->GetString(_T("IDS_FILEMANAGER_FUNCTION")),QMB_YESNO);
			if(nRet == QIDYES)
			{
				bool bOverWrite = m_newFile.Open(m_strNewFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) == TRUE;
				if(bOverWrite == false)
					::DeleteFile(m_strNewFile);
				else
					m_newFile.Close();
			}
		}

		if(nRet == QIDYES)
		{
			if(m_newFile.Open(m_strNewFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
			{
				m_bFileOpen = true;
				TRACE(_T("Open %s for download successful.\n"),m_strNewFile);
				int nSize = 0;
				m_nTempProgress = 0;
				m_pIEFS->RegisterNotify(this,NOTIFY_GET_FILE_DATA);
				m_pIEFS->RegisterNotify(this,NOTIFY_ABORT_GET_FILE_DATA);
				bRes = m_pIEFS->GetFileASync(0,strSrcMEPath,nSize) == eEFSRetSuccessContinue;			
				if(bRes == false)
				{
					m_vectBuffer.clear();
					m_newFile.Close();
					m_bFileOpen = false;
					TSTRING strPrompt = m_strNewFile;
					strPrompt += _T("\n") + m_pILocalization->GetString(_T("IDS_FileMgr_GET_FILE_FAIL"));
					ShowMessage(hParent,strPrompt,m_pILocalization->GetString(_T("IDS_FILEMANAGER_FUNCTION")));
				}
			}
			else
			{				
				TSTRING strPrompt = m_strNewFile;
				strPrompt += _T("\n") + m_pILocalization->GetString(_T("IDS_FileMgr_CREATE_FILE_FAIL"));
				ShowMessage(hParent,strPrompt,m_pILocalization->GetString(_T("IDS_FILEMANAGER_FUNCTION")));	
			}
		}
	}
	return bRes;
}

bool CQMEFileTreeDataSource::DownloadFilesIntoPC(CEvent& aboutEvent,const CString& strPCFolder, vector<CString> &vct_strMEFilePath,HWND hParent)
{	
	bool bRes = false;
	if(_taccess(strPCFolder,0) != -1)
	{	
		m_nDownloadedSize = 0;
		m_nTotalDownloadSize = 0;
		for(vector<CString>::iterator Iter = vct_strMEFilePath.begin();Iter != vct_strMEFilePath.end();Iter++)
		{
			CMEFileNode* pNode = FindNodeByName(*Iter);
			assert(pNode);
			if(pNode)
				m_nTotalDownloadSize += pNode->GetFileSize();
		}
		for(vector<CString>::iterator Iter = vct_strMEFilePath.begin();Iter != vct_strMEFilePath.end();Iter++)
		{
			CMEFileNode* pNode = FindNodeByName(*Iter);
			pNode->GetFileSize();
			if(::WaitForSingleObject(aboutEvent,0) == WAIT_OBJECT_0)
				break;
			//::WaitForSingleObject(m_DownloadAvailableEvent,INFINITE);
			m_DownloadAvailableEvent.ResetEvent();

			CString strRealPath = pNode->GetVirtualPath().c_str();
			if(strRealPath.IsEmpty())
				strRealPath = *Iter;
			if(DownloadOneFile_ME2PC(aboutEvent,strRealPath,strPCFolder,hParent))
				::WaitForSingleObject(m_DownloadAvailableEvent,INFINITE);
		}
		bRes = true;
	}
	//::WaitForSingleObject(m_DownloadAvailableEvent,INFINITE);
	return bRes;
}

bool CQMEFileTreeDataSource::DelFile(CQTreeNode* pNode)
{
	bool bRet = false;
	CMEFileNode* pFNode = (CMEFileNode*)pNode;
	if(pFNode)
	{
		CCursorWait wait;

		CString strFilePath = pFNode->GetFullPath().c_str();
		if(!(strFilePath.IsEmpty()))
			bRet = DelFileByFilePath(strFilePath);
	}
	return bRet;
}


bool CQMEFileTreeDataSource::DelFileByFilePath(const CString& strFileName)
{
	bool bRet = false;	
	if(!(strFileName.IsEmpty()) && m_pIEFS)
		bRet = m_pIEFS->DeleteFile(strFileName) == eEFSRetSuccess;
	if(bRet)
	{
		CMEFileNode* pNode = FindNodeByName(strFileName);
		if(pNode)
			delete pNode;
	}
	return bRet;
}

bool CQMEFileTreeDataSource::DelMEData(CMEFileNode* pFNode)
{
	bool bRet = false;
	if(pFNode /*&& !pFNode->IsRoot()*/)
	{
		if(pFNode->IsFolder())//folder
			bRet= DelFolder(pFNode);
		else//should be file
			bRet = DelFile(pFNode);
	}
	return bRet;
}

CMEFileNode* CQMEFileTreeDataSource::FindNodeByName(const CString& strFileName)
{
	return static_cast<CMEFileNode*>(const_cast<CQTreeNode*>(GetRoot()->GetTreeNodeByFullPath(LPCTSTR(strFileName))));
}

void CQMEFileTreeDataSource::DeleteFileManagerInterface() {
    if (m_pIEFS){
        CommDeleteInterface(m_pIEFS);
        m_pIEFS = NULL;
    }
}

#pragma warning(pop)