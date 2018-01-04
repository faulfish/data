#include "StdAfx.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "..\..\CommonUtil\unicodeutility.h"
#include "EFs.h"



CEFs::CEFs(int nCOMPort):CInterfaceBase(nCOMPort)
{
}

CEFs::~CEFs(void)
{
}

bool CEFs::EFSFileOpen(const char *strFile, const long oflag, const long mode, long& fd,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	// To determine the length of the request packet
	CRequestPtr ptRequest = new CEFsOpenFileRequest(strFile, oflag, mode);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char *)ptBufferReceived->GetData();

		fd = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
		iErrorCode = pBuffer[8] + (pBuffer[9] << 8) + (pBuffer[10] << 16) + (pBuffer[11] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}
	return bRes;

}
bool CEFs::EFSFileClose(const long fd,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CEFsCloseFileRequest(fd);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char *)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;

}

bool CEFs::EfsReadFile(const char *sFileSrc, const char *sFileDst, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	long fd, mode, oflag;

	mode=S_IRUSR | S_IRGRP | S_IROTH;
	oflag=O_RDONLY;

	bRes = CEFs::EFSFileOpen(sFileSrc,oflag, mode, fd, iErrorCode);

	if(bRes)
	{
		FILE *pStream;
		pStream = fopen((const char*)sFileDst, "wb");

		if(pStream == NULL)
		{
			EFSFileClose(fd,iErrorCode);
			bRes = false;
		}

		if(bRes)
		{
			DWORD nbyte, tot_bytes_read=0;
			DWORD iBytes_Read = 0;

			tot_bytes_read = 0;
			do
			{
				nbyte = 0x0200;
				CRequestPtr ptRequest = new CEFsReadFileRequest(fd, nbyte, tot_bytes_read);	
				bRes = Execute(ptRequest);
	
				if(bRes)
				{
					CBufferPtr ptBufferReceived;
					ptRequest->GetReceiveData(ptBufferReceived);
					unsigned char *pBuffer = (unsigned char*)ptBufferReceived->GetData();

					DWORD ifd_rsp = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
					DWORD iOffset_rsp = pBuffer[8] + (pBuffer[9] << 8) + (pBuffer[10] << 16) + (pBuffer[11] << 24);
					iBytes_Read = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[15] << 24);
					iErrorCode = pBuffer[16] + (pBuffer[17] << 8) + (pBuffer[18] << 16) + (pBuffer[19] << 24);

					if((iErrorCode != 0 ) || (ifd_rsp != fd) || (iBytes_Read < 0) || (iOffset_rsp != tot_bytes_read))
						bRes = false;
				
					if(bRes)
					{
						DWORD nWritten = 0;
						nWritten=fwrite(pBuffer+20, 1, iBytes_Read, pStream);

						tot_bytes_read += iBytes_Read;
					}
				}
			}while( (iBytes_Read == nbyte) && bRes);

			fclose(pStream);
			EFSFileClose(fd, iErrorCode);
		}
	}

	return bRes;
	
}

bool CEFs::EfsWriteFile(const char *sFileSrc, const char *sFileDst,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	EfsFileAttributes attrib = {0,0,0};
	
	bool bRes = PerformEFS2WriteOp(sFileDst, 0, attrib, iErrorCode,sFileSrc,0,0);
	return (bRes );
}

bool CEFs::EfsWriteFileFromMem(const char *sData, const DWORD iSize, const char *sFileDst,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	EfsFileAttributes attrib = {0,0,0};
	
	bool bRes = PerformEFS2WriteOp(sFileDst, 0, attrib, iErrorCode,sData, 1, iSize );

	return (bRes );
}

bool CEFs::PerformEFS2WriteOp(const char *sFileDst, const BYTE mode, const EfsFileAttributes attrib,long &iErrorCode, const char *pUnkStream, 
						const int mem, const unsigned long iSize)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	long fd;
	DWORD tot_data_size;

	FILE *pStream;

	char *pData = new char[EFS2_MAX_DATA_SIZE+1];
	assert(pData != NULL);

	if(mem == 0)
	{
		using namespace QSYNC_LIB;
		TSTRING strFile = GetTSTRING(pUnkStream);

		pStream = fopen((const char*)strFile.c_str(), "rb");

		if(pStream == NULL)
		{
			fclose(pStream);
		}
		else
		{	fseek(pStream, 0, SEEK_END);
			tot_data_size = ftell(pStream);
			rewind(pStream);
			bRes = true;
		}
	}
	else
	{
		bRes = true;
		tot_data_size= iSize;
	}

	if(bRes)
	{
		DWORD oflag=O_WRONLY | O_CREAT | O_TRUNC;
		const long efs2Mode = DEFFILEMODE;
		bRes = EFSFileOpen(sFileDst,oflag, efs2Mode, fd, iErrorCode);
	}

	if(bRes)
	{
		DWORD tot_data_written = 0;
		while((tot_data_written < tot_data_size) && bRes)
		{
			long nRead = EFS2_MAX_DATA_SIZE;
			if(mem == 0) //From file
			{
//				InFile.Seek(tot_data_written, CFile::begin);
				nRead = fread(pData,sizeof(char), EFS2_MAX_DATA_SIZE,pStream);
			}
			else
			{
				if(nRead > (tot_data_size - tot_data_written))
				{
					nRead = tot_data_size - tot_data_written;
				}

				memcpy(pData, pUnkStream + tot_data_written, nRead);
			}

			CRequestPtr ptRequest = new CEFsWriteFileRequest(fd, tot_data_written, pData, nRead);	
			bRes = Execute(ptRequest);

			if(bRes)
			{
				CBufferPtr ptBufferReceived;
				ptRequest->GetReceiveData(ptBufferReceived);
				unsigned char *pBuffer = (unsigned char*)ptBufferReceived->GetData();

				DWORD ifd_rsp = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
				DWORD iOffset_rsp = pBuffer[8] + (pBuffer[9] << 8) + (pBuffer[10] << 16) + (pBuffer[11] << 24);
				DWORD iBytes_Write = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[15] << 24);
				iErrorCode = pBuffer[16] + (pBuffer[17] << 8) + (pBuffer[18] << 16) + (pBuffer[19] << 24);
				
				if((iErrorCode != 0) || (iBytes_Write == 0) || (ifd_rsp != fd) || (iOffset_rsp != tot_data_written))
				{
					EFSFileClose(fd, iErrorCode);
					bRes = false;
				}

				tot_data_written += iBytes_Write;
			}
			else
				EFSFileClose(fd, iErrorCode);
		}
		if(bRes)
			bRes = EFSFileClose(fd, iErrorCode);

		fclose(pStream);
	}

	delete [] pData;

	return bRes;

}

bool CEFs::EfsDeleteFile(const char *sEFS_File,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	// To determine the length of the request packet
	CRequestPtr ptRequest = new CEFsDeleteFileRequest(sEFS_File);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;
}

bool CEFs::EfsCreateLink(const char* sFilePath, const char* sPointedFilePath,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CEFsCreateLinkRequest(sFilePath, sPointedFilePath);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;
}

bool CEFs::EfsReadLink(const char* sFilePath , char* sPointedFilePath,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CEFsReadLinkRequest(sFilePath);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char*)ptBufferReceived->GetData();
		int nLen = ptBufferReceived->GetLength();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);

		if(iErrorCode != 0)
			bRes = false;

		::_tcscpy_s(sPointedFilePath, nLen - 8,&pBuffer[8]);
	}
	return bRes;
}

bool CEFs::EfsMakeFolder(const char* strDir,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CEFsMakeDirRequest(0x01FF, strDir);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char*)ptBufferReceived->GetData();

		iErrorCode = *(pBuffer+4) + (*(pBuffer+5) << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;

}

bool CEFs::EFSDirOpen(const char *strPath, long& dirp,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	if(_tcslen(strPath) == 0)
		return false;

	CRequestPtr ptRequest = new CEFsOpenDirRequest(strPath);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);

		char *pBuffer = (char*)ptBufferReceived->GetData();

		dirp = pBuffer[4]+(pBuffer[5]<<8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
		iErrorCode =  pBuffer[8]+(pBuffer[9]<<8) + (pBuffer[10] << 16) + (pBuffer[11] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}
	return bRes;
}

bool CEFs::EFSDirClose(const long dirp,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CEFsCloseDirRequest(dirp);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] <<8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;
}

bool CEFs::EFsQueryFileSystemStatus(const char * sFilePath, \
									DWORD &iFS_I, \
									DWORD &iFS_Type, \
									DWORD &iBlockSize, \
									DWORD &iTotalBlocks, \
									DWORD &iAvailBlocks, \
									DWORD &iFreeBlocks, \
									DWORD &iMaxFileSize, \
									DWORD &iNfiles, \
									DWORD &iMaxNfiles, \
									long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CEFsFileSystemStatusRequest(sFilePath);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		unsigned char *pBuffer = (unsigned char*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
		iFS_I = pBuffer[8] + (pBuffer[9] << 8) + (pBuffer[10] << 16) + (pBuffer[7] << 24);
		iFS_Type = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[17] << 24);
		iBlockSize = pBuffer[16] + (pBuffer[17] << 8) + (pBuffer[18] << 16) + (pBuffer[19] << 24);
		iTotalBlocks = pBuffer[20] + (pBuffer[21] << 8) + (pBuffer[22] << 16) + (pBuffer[23] << 24);
		iAvailBlocks = pBuffer[24] + (pBuffer[25] << 8) + (pBuffer[26] << 16) + (pBuffer[27] << 24);
		iFreeBlocks = pBuffer[28] + (pBuffer[29] << 8) + (pBuffer[30] << 16) + (pBuffer[31] << 24);
		iMaxFileSize = pBuffer[32] + (pBuffer[33] << 8) + (pBuffer[34] << 16) + (pBuffer[35] << 24);
		iNfiles = pBuffer[36] + (pBuffer[37] << 8) + (pBuffer[38] << 16) + (pBuffer[39] << 24);
		iMaxNfiles = pBuffer[40] + (pBuffer[41] << 8) + (pBuffer[42] << 16) + (pBuffer[43] << 24);

		if(iErrorCode != 0)
			bRes = false;

	}

	return bRes;
}

bool CEFs::EFsQueryFlashDeviceInfo(DWORD& iTotalNumOfBlocks,\
									DWORD& iNumPagesPerBlock,\
									DWORD& iPageSize,\
									DWORD& iTotalPageSize,\
									DWORD& iMakerId,\
									DWORD& iDeviceId,\
									BYTE& iDeviceType,\
									char* psDeviceName, \
									long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CBufferPtr ptBufferReceived;
	
	CRequestPtr ptRequest = new CEFsFlashDeviceInfoRequest();	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
		iTotalNumOfBlocks = pBuffer[8] + (pBuffer[9] << 8) + (pBuffer[10] << 16) + (pBuffer[7] << 24);
		iNumPagesPerBlock = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[17] << 24);
		iPageSize = pBuffer[16] + (pBuffer[17] << 8) + (pBuffer[18] << 16) + (pBuffer[19] << 24);
		iTotalPageSize = pBuffer[20] + (pBuffer[21] << 8) + (pBuffer[22] >> 16) + (pBuffer[23] << 24);
		iMakerId = pBuffer[24] + (pBuffer[25] << 8) + (pBuffer[26] << 16) + (pBuffer[27] << 24);
		iDeviceId = pBuffer[28] + (pBuffer[29] << 8) + (pBuffer[30] << 16) + (pBuffer[31] << 24);
		iDeviceType = pBuffer[32];// + (pBuffer[33] << 8) + (pBuffer[34] << 16) + (pBuffer[35] << 24);

		int nLen = ptBufferReceived->GetLength();
		strcpy_s(psDeviceName, nLen - 33, (char*)pBuffer+33);
//		strcpy(psDeviceName, pBuffer+33);

//		strcpy(psDeviceName, pBuffer+36);

	if(iErrorCode != 0)
			bRes = false;

	}

	return bRes;
}

bool CEFs::EFsCheckAccessAttribute(const unsigned short iPermissionBits, const char* sFilePath, long& iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CEFsCheckAccessAttrRequest(iPermissionBits, sFilePath);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		char *pBuffer = (char*)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] <<8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);	
		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;
}

bool CEFs::EFsChomd(const DWORD iMode, const char* sFilePath,long& iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CEFsChomdRequest(iMode, sFilePath);	
	bRes = Execute(ptRequest);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		unsigned char *pBuffer = (unsigned char *)ptBufferReceived->GetData();

		iErrorCode = pBuffer[4] + (pBuffer[5] <<8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);	
		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;
}

bool CEFs::EFsRemoveFolder(const char* sFolderPath, long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	
	CRequestPtr ptRequest = new CEFsRemoveFolderRequest(sFolderPath);
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);

		unsigned char* pBuffer = (unsigned char*)ptBufferReceived->GetData();

		ptBufferReceived->Release();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
		if(iErrorCode != 0)
			bRes = false;
	}

	return bRes;
}

bool CEFs::EFsRename(const char* sOldPathName, const char* sNewPathName, long& iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	CBufferPtr ptBufferReceived;

	CRequestPtr ptRequest = new CEFsRenameRequest(sOldPathName, sNewPathName);	
	bRes = Execute(ptRequest);
	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);

		unsigned char* pBuffer = (unsigned char*)ptBufferReceived->GetData();

		ptBufferReceived->Release();

		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
		if(iErrorCode != 0)
			bRes = false;
	}


	return bRes;
}

bool CEFs::EFsQueryFStatus(const char* sFilePath, DWORD& iMode, DWORD& iSize, DWORD& iNlink, DWORD& iAtime, DWORD& iMtime, DWORD& iCtime,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;
	
	CRequestPtr ptRequest = new CEFsQueryFileStatusRequest(sFilePath);	
	bRes = Execute(ptRequest, 10000);

	if(bRes)
	{
		CBufferPtr ptBufferReceived;
		ptRequest->GetReceiveData(ptBufferReceived);
		unsigned char *pBuffer = (unsigned char*)ptBufferReceived->GetData();
		
		iErrorCode = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
		iMode = pBuffer[8] + (pBuffer[9] << 8) + (pBuffer[10] << 16) + (pBuffer[11] << 24);
		iSize = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[15] << 24);
		iNlink = pBuffer[16] + (pBuffer[17] << 8) + (pBuffer[18] << 16) + (pBuffer[19] << 24);
		iAtime = pBuffer[20] + (pBuffer[21] << 8) + (pBuffer[22] << 16) + (pBuffer[23] << 24);
		iMtime = pBuffer[24] + (pBuffer[25] << 8) + (pBuffer[26] << 16) + (pBuffer[27] << 24);
		iCtime = pBuffer[28] + (pBuffer[29] << 8) + (pBuffer[30] << 16) + (pBuffer[31] << 24);

		if(iErrorCode != 0)
			bRes = false;
	}


	return bRes;
}

bool CEFs::EfsDirectoryList(const char *strDir, std::vector<Efs2DirItem> *pDirList, DWORD &iSize, BYTE entryType,long &iErrorCode)
{
	using namespace PLATFORM_TOOL;

	long dirp, seqno = 1;
	bool bRes = false;

	bRes = EFSDirOpen(strDir, dirp, iErrorCode);

	if(bRes)
	{
		seqno=1;
		// Read the directory
		while(bRes)
		{
			CRequestPtr ptRequest = new CEFsReadDirRequest(dirp,seqno);	
			bRes = Execute(ptRequest, 10000);
			if(bRes)
			{
				CBufferPtr ptBufferReceived;
				ptRequest->GetReceiveData(ptBufferReceived);
				unsigned char *pBuffer = (unsigned char*)ptBufferReceived->GetData();

				DWORD iDirp_rsp = pBuffer[4] + (pBuffer[5] << 8) + (pBuffer[6] << 16) + (pBuffer[7] << 24);
				DWORD iSeqno_rsp = pBuffer[8] + (pBuffer[9] << 8) + (pBuffer[10] << 16) + (pBuffer[11] << 24);
				iErrorCode = pBuffer[12] + (pBuffer[13] << 8) + (pBuffer[14] << 16) + (pBuffer[15] << 24);
				DWORD iEntryType = pBuffer[16] + (pBuffer[17] << 8) + (pBuffer[18] << 16) + (pBuffer[19] << 24);
				DWORD iMode = pBuffer[20] + (pBuffer[21] << 8) + (pBuffer[22] << 16) + (pBuffer[23] << 24);
				DWORD iSize = pBuffer[24] + (pBuffer[25] << 8) + (pBuffer[26] << 16) + (pBuffer[27] << 24);
				DWORD iAtime = pBuffer[28] + (pBuffer[29] << 8) + (pBuffer[30] << 16) + (pBuffer[31] << 24);
				DWORD iMtime = pBuffer[32] + (pBuffer[33] << 8) + (pBuffer[34] << 16) + (pBuffer[35] << 24);
				DWORD iCtime = pBuffer[36] + (pBuffer[37] << 8) + (pBuffer[38] << 16) + (pBuffer[39] << 24);

				if((iErrorCode != 0) || (iDirp_rsp != dirp))
				{
					bRes = false;
					continue;
				}

				//If end of directory break out of loop
				if( pBuffer[40] == '\0' )
					break;

				// Store the name only if the file type is the same as that
				// for which iteration in requested
				if( (entryType == (BYTE)-1) || (entryType == iEntryType ))
				{
					char szName[256]={0};
					strcpy(szName, strDir);
					if(*(pBuffer+40) != '/')
						if(strcmp(strDir,"/") != 0)
							strcat(szName, "/");
					strcat(szName,(const char*)pBuffer+40);

					Efs2DirItem dir_item;
					strcpy(dir_item.name,szName);
					dir_item.isFile = (int)iEntryType;
					dir_item.fileSize = iSize;
					dir_item.createDate =iCtime;
					pDirList->push_back(dir_item);
				}
				//Increment the sequence number
				seqno++;
			}
		}
	}

	EFSDirClose(dirp, iErrorCode);

	//update the size if asked for
	if(bRes)
	{
		iSize = pDirList->size();
	}

	return bRes;
}

bool CEFs::GetMicroSDInfo(int& nFilesystemType,int& nBlockSize,int& nTotalBlockCount,int& nFreeBlockCount,
			int& nAvailableBlockCount,int& nTotalFileNumber,int& nFreeFileNumber,int& nFileSystemID
			,int& nMaximumFilename,BYTE& bStatus)
{
	using namespace PLATFORM_TOOL;

	CRequestPtr ptRequest = new CGetMicroSDInfoRequest(nFilesystemType,nBlockSize,nTotalBlockCount,nFreeBlockCount
		,nAvailableBlockCount,nTotalFileNumber,nFreeFileNumber,nFileSystemID	,nMaximumFilename,bStatus);	
	return Execute(ptRequest);
}


CCFCEFs::CCFCEFs(int nCOMPort): CEFs(nCOMPort)
{}
CCFCEFs::~CCFCEFs(){}
bool CCFCEFs::GetLocFile(const char *szDstFilePath)
{
	bool bRes = false;	

	std::vector<Efs2DirItem> DirList;
	DWORD nSize = 0;
	BYTE entryType = 0;
	long iErrorCode;
	bool bListFile = EfsDirectoryList("/", &DirList, nSize,entryType,iErrorCode);
	if(bListFile)
	{
		for(std::vector<Efs2DirItem>::iterator Iter = DirList.begin(); !bRes && Iter != DirList.end(); ++Iter)
		{
			CStringA strFileName = Iter->name;
			strFileName.MakeLower();
			int nSlashPos = strFileName.ReverseFind('/');
			CStringA strTmp = strFileName.Mid(nSlashPos + 1); // remove "/"

			int nDotPos = strFileName.ReverseFind('.');
			if (nDotPos != -1 && strFileName.Mid(nDotPos) == ".loc")
			{
				bRes = EfsReadFile(Iter->name, szDstFilePath, iErrorCode);
			}
		}
	}
	return bRes;
}

bool CCFCEFs::GetLocFileName(char *szFileName)
{
	bool bRes = false;	
	
	std::vector<Efs2DirItem> DirList;
	DWORD nSize = 0;
	BYTE entryType = 0;
	long iErrorCode;
	bool bListFile = EfsDirectoryList("/", &DirList, nSize,entryType,iErrorCode);
	if(bListFile)
	{
		for(std::vector<Efs2DirItem>::iterator Iter = DirList.begin(); !bRes && Iter != DirList.end(); ++Iter)
		{
			CStringA strFileName = Iter->name;
			strFileName.MakeLower();
			int nSlashPos = strFileName.ReverseFind('/');
			CStringA strTmp = strFileName.Mid(nSlashPos + 1); // remove "/"

			int nDotPos = strFileName.ReverseFind('.');
			if (nDotPos != -1 && strFileName.Mid(nDotPos) == ".loc")
			{
				bRes = true;
				memcpy(szFileName, Iter->name + nSlashPos +1, strTmp.GetLength()+1); //Don't change the return value to lowercase letter, it will effect SIM Encode
				strTmp.ReleaseBuffer();
			}
		}
	}
	return bRes;
}
