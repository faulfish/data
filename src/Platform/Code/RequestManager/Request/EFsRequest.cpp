#include "stdafx.h"
#include <stdio.h>
#include <tchar.h>
#include "EFsRequest.h"



namespace PLATFORM_TOOL
{
	CEFsRequest::CEFsRequest()
	{
	}

	CEFsRequest::~CEFsRequest()
	{
	}

	CEFsOpenFileRequest::CEFsOpenFileRequest(const TCHAR* sFileName, const long iFlag, const long iMode)
	{	
		TCHAR szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_OPEN & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_OPEN >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFlag & 0xFF);
		szBuf[5] = (BYTE)((iFlag >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFlag >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFlag >> 24) & 0xFF);
		szBuf[8] = (BYTE)((iMode) & 0xFF);
		szBuf[9] = (BYTE)((iMode >> 8) & 0xFF);
		szBuf[10] = (BYTE)((iMode >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iMode >> 24) & 0xFF);

		size_t iNameSize = _tcslen(sFileName);
		memcpy(&szBuf[12], sFileName, iNameSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iNameSize + 12 + 1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsOpenFileRequest::~CEFsOpenFileRequest()
	{
	}

	CEFsCloseFileRequest::CEFsCloseFileRequest(const DWORD iFd)
	{	
		TCHAR szBuf[20] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_CLOSE & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_CLOSE >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFd & 0xFF);
		szBuf[5] = (BYTE)((iFd >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFd >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFd >> 24) & 0xFf);

		CBufferPtr ptBufferSend;
		Encode(szBuf,8, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsCloseFileRequest::~CEFsCloseFileRequest()
	{
	}

	CEFsReadFileRequest::CEFsReadFileRequest(const DWORD iFb, const DWORD iBytes, const DWORD iOffset)
	{
		TCHAR szBuf[20] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_READ & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_READ >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFb & 0xFF);
		szBuf[5] = (BYTE)((iFb >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFb >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFb >> 24) & 0xFF);
		szBuf[8] = (BYTE)((iBytes) & 0xFF);
		szBuf[9] = (BYTE)((iBytes >> 8) & 0xFF);
		szBuf[10] = (BYTE)((iBytes >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iBytes >> 24) & 0xFF);
		szBuf[12] = (BYTE)((iOffset) & 0xFF);
		szBuf[13] = (BYTE)((iOffset >> 8) & 0xFF);
		szBuf[14] = (BYTE)((iOffset >> 16) & 0xFF);
		szBuf[15] = (BYTE)((iOffset >> 24) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 16, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsReadFileRequest::~CEFsReadFileRequest()
	{
	}

	CEFsWriteFileRequest::CEFsWriteFileRequest(const DWORD iFd, const DWORD iOffset, const TCHAR* sData, const DWORD iSize)
	{
		TCHAR szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_WRITE & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_WRITE >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iFd & 0xFF);
		szBuf[5] = (BYTE)((iFd >> 8)& 0xFF);
		szBuf[6] = (BYTE)((iFd >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iFd >> 24) & 0xFF);
		szBuf[8] = (BYTE)((iOffset) & 0xFF);
		szBuf[9] = (BYTE)((iOffset >> 8) & 0xFF);
		szBuf[10] = (BYTE)((iOffset >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iOffset >> 24) & 0xFF);

		memcpy(&szBuf[12], sData, iSize);

		CBufferPtr ptBufferSend;
		Encode(szBuf,iSize+12, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsWriteFileRequest::~CEFsWriteFileRequest()
	{
	}

	CEFsDeleteFileRequest::CEFsDeleteFileRequest(const TCHAR *sFilename)
	{
		TCHAR szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_UNLINK & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_UNLINK >> 8) & 0xFF);

		size_t iNameSize = _tcslen(sFilename);
		memcpy(&szBuf[4], sFilename, iNameSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iNameSize + 4 + 1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsDeleteFileRequest::~CEFsDeleteFileRequest()
	{
	}

	CEFsMakeDirRequest::CEFsMakeDirRequest(const WORD iMode, const TCHAR* sFolderPath)
	{
		TCHAR szBuf[540] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_MKDIR & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_MKDIR >> 8) & 0xFF);
		szBuf[4] = (BYTE)(iMode & 0xFF);
		szBuf[5] = (BYTE)((iMode >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFolderPath);
		memcpy(&szBuf[6], sFolderPath, iPathSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf,(int)iPathSize + 6 + 1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsMakeDirRequest:: ~CEFsMakeDirRequest()
	{
	}

	CEFsCreateLinkRequest::CEFsCreateLinkRequest(const TCHAR* sFilePath, const TCHAR *sPointedFilePath)
	{
		TCHAR szBuf[1000] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_SYMLINK & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_SYMLINK >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		size_t iPointedPathSize = strlen(sPointedFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize +1);
		memcpy(&szBuf[4]+iPathSize+1, sPointedFilePath, iPointedPathSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize+ (int)iPointedPathSize+4+2, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsCreateLinkRequest::~CEFsCreateLinkRequest()
	{
	}

	CEFsReadLinkRequest::CEFsReadLinkRequest(const TCHAR* sFilePath)
	{
		TCHAR szBuf[4096] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_READLINK & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_READLINK >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf,(int)iPathSize+4+1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsReadLinkRequest::~CEFsReadLinkRequest()
	{
	}

	CEFsOpenDirRequest::CEFsOpenDirRequest(const TCHAR* sFolderPath)
	{
		TCHAR szBuf[4096] = {0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_OPENDIR & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_OPENDIR >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFolderPath);
		memcpy(&szBuf[4], sFolderPath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf,(int)iPathSize+4+1, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsOpenDirRequest:: ~CEFsOpenDirRequest()
	{
	}
	
	CEFsCloseDirRequest::CEFsCloseDirRequest(const DWORD iDirp)
	{

		TCHAR szBuf[20] = {0};
		szBuf[0] = (BYTE)DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE)DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE)(_EFS2_DIAG_CLOSEDIR & 0xFF);
		szBuf[3] = (BYTE)((_EFS2_DIAG_CLOSEDIR >>8 ) & 0xFF);
		szBuf[4] = (BYTE)((iDirp) & 0xFF);
		szBuf[5] = (BYTE)((iDirp >> 8) & 0xFF);
		szBuf[6] = (BYTE)((iDirp >> 16) & 0xFF);
		szBuf[7] = (BYTE)((iDirp >> 24) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 8, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsCloseDirRequest::~CEFsCloseDirRequest()
	{
	}

	CEFsFileSystemStatusRequest::CEFsFileSystemStatusRequest(const TCHAR * sFilePath)
	{
		TCHAR szBuf[4096] = {0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_STATFS & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_STATFS >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize +1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 4 +1, ptBufferSend);
		SetSendData(ptBufferSend);

	}
	CEFsFileSystemStatusRequest::~CEFsFileSystemStatusRequest()
	{
	}

	CEFsFlashDeviceInfoRequest::CEFsFlashDeviceInfoRequest(void)
	{
		TCHAR szBuf[10] = {0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_NAND_DEV_INFO & 0xFF);
		szBuf[3]=(BYTE)((_EFS2_DIAG_NAND_DEV_INFO >> 8) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf, 4, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsFlashDeviceInfoRequest::~CEFsFlashDeviceInfoRequest()
	{
	}

	CEFsCheckAccessAttrRequest::CEFsCheckAccessAttrRequest(const WORD iPermissionBits, const TCHAR* sFilePath)
	{
		TCHAR szBuf[4096]={0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_ACCESS & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_ACCESS >> 8) & 0xFF);
		szBuf[4]=(BYTE)(iPermissionBits & 0xFF);
		szBuf[5]=(BYTE)((iPermissionBits >>8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[6], sFilePath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 6 + 1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsCheckAccessAttrRequest::~CEFsCheckAccessAttrRequest()
	{
	}

	CEFsChomdRequest::CEFsChomdRequest(const DWORD iMode, const TCHAR* sFilePath)
	{
		TCHAR szBuf[4096]={0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_CHMOD & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_CHMOD >> 8) & 0xFF);
		szBuf[4]=(BYTE)(iMode &0xFF);
		szBuf[5]=(BYTE)((iMode >> 8) & 0xFF);
		szBuf[6]=(BYTE)((iMode >>16) & 0xFF);
		szBuf[7]=(BYTE)((iMode >>24) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[8], sFilePath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 8 +1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsChomdRequest::~CEFsChomdRequest()
	{
	}

	CEFsQueryFileStatusRequest::CEFsQueryFileStatusRequest(const TCHAR* sFilePath)
	{
		TCHAR szBuf[4096]={0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE) (_EFS2_DIAG_STAT & 0xFF);
		szBuf[3] = (BYTE) ((_EFS2_DIAG_STAT >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFilePath);
		memcpy(&szBuf[4], sFilePath, iPathSize+1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize+ 4 + 1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsQueryFileStatusRequest::~CEFsQueryFileStatusRequest()
	{
	}

	CEFsRenameRequest::CEFsRenameRequest(const TCHAR* sOldPathName,const TCHAR* sNewPathName)
	{
		TCHAR szBuf[4096]={0};
		szBuf[0]=(BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1]=(BYTE) DIAG_SUBSYS_EFS;
		szBuf[2]=(BYTE) (_EFS2_DIAG_RENAME & 0xFF);
		szBuf[3]=(BYTE) ((_EFS2_DIAG_RENAME >> 8 )& 0xFF);

		size_t iOldPathSize = _tcslen(sOldPathName);
		size_t iNewPathSize = _tcslen(sNewPathName);
		memcpy(&szBuf[4], sOldPathName, iOldPathSize +1);
		memcpy(&szBuf[4]+iOldPathSize+1, sNewPathName, iNewPathSize);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iOldPathSize+(int)iNewPathSize + 4 + 2, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsRenameRequest::~CEFsRenameRequest()
	{
	}

	CEFsRemoveFolderRequest::CEFsRemoveFolderRequest(const TCHAR* sFolderPath)
	{
		TCHAR szBuf[4096]={0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE) (_EFS2_DIAG_RMDIR & 0xFF);
		szBuf[3] = (BYTE) ((_EFS2_DIAG_RMDIR >> 8) & 0xFF);

		size_t iPathSize = _tcslen(sFolderPath);
		memcpy(&szBuf[4], sFolderPath, iPathSize + 1);

		CBufferPtr ptBufferSend;
		Encode(szBuf, (int)iPathSize + 4 + 1, ptBufferSend);
		SetSendData(ptBufferSend);

	}

	CEFsRemoveFolderRequest::~CEFsRemoveFolderRequest()
	{
	}

	CEFsReadDirRequest::CEFsReadDirRequest(const DWORD dirp, const int iseqno)
	{
		TCHAR szBuf[20]={0};
		szBuf[0] = (BYTE) DIAG_SUBSYS_CMD_F;
		szBuf[1] = (BYTE) DIAG_SUBSYS_EFS;
		szBuf[2] = (BYTE) (_EFS2_DIAG_READDIR & 0xFF);
		szBuf[3] = (BYTE) ((_EFS2_DIAG_READDIR >> 8) & 0xFF);

		szBuf[4] = (BYTE)(dirp & 0xFF);
		szBuf[5] = (BYTE)((dirp>>8) & 0xFF);
		szBuf[6] = (BYTE)((dirp>>16)&0xFF);
		szBuf[7] = (BYTE)((dirp>>24)&0xFF);
		szBuf[8] = (BYTE)(iseqno&0xFF);
		szBuf[9] = (BYTE)((iseqno>>8)&0xFF);
		szBuf[10] = (BYTE)((iseqno >> 16) & 0xFF);
		szBuf[11] = (BYTE)((iseqno >> 24) & 0xFF);

		CBufferPtr ptBufferSend;
		Encode(szBuf,12, ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CEFsReadDirRequest::~CEFsReadDirRequest()
	{
	}
	
	CGetMicroSDInfoRequest::CGetMicroSDInfoRequest(int& nFilesystemType,int& nBlockSize,int& nTotalBlockCount,int& nFreeBlockCount,
		int& nAvailableBlockCount,int& nTotalFileNumber,int& nFreeFileNumber,int& nFileSystemID
		,int& nMaximumFilename,BYTE& bStatus)
		:CDiagRequestBase("CGetMicroSDInfoRequest")
		,m_nFilesystemType(nFilesystemType)
		,m_nBlockSize(nBlockSize)
		,m_nTotalBlockCount(nTotalBlockCount)
		,m_nFreeBlockCount(nFreeBlockCount)
		,m_nAvailableBlockCount(nAvailableBlockCount)
		,m_nTotalFileNumber(nTotalFileNumber)
		,m_nFreeFileNumber(nFreeFileNumber)
		,m_nFileSystemID(nFileSystemID)
		,m_nMaximumFilename(nMaximumFilename)
		,m_bStatus(bStatus)
	{
		BYTE szRAWCommand[] = {75, 19,255,  0};

		CBufferPtr ptBufferSend;
		Encode((const char*)szRAWCommand,sizeof(szRAWCommand),ptBufferSend);
		SetSendData(ptBufferSend);
	}

	CGetMicroSDInfoRequest::~CGetMicroSDInfoRequest()
	{
	}

	bool CGetMicroSDInfoRequest::IsCompletePacket(CBufferPtr& ptBufferReceived)
	{
		return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
	}

	void CGetMicroSDInfoRequest::OnDataReceiver(CBufferPtr& ptBufferReceived)
	{	
#pragma pack(push, 1)
		typedef struct  
		{
			BYTE command_code;
			BYTE subsys_id;
			short mobile_id;
			int dwAvail;
			int nFilesystemType;
			int nBlockSize;
			int nTotalBlockCount;
			int nFreeBlockCount;
			int nAvailableBlockCount;
			int nTotalFileNumber;
			int nFreeFileNumber;
			int nFileSystemID;
			int nMaximumFilename;
			BYTE nStatus;
		} diag_get_miscro_SD_rsp_type;
#pragma pack(pop)
		diag_get_miscro_SD_rsp_type resp = {0};
		int nSize = sizeof(diag_get_miscro_SD_rsp_type);
		assert(sizeof(diag_get_miscro_SD_rsp_type) <= ptBufferReceived->GetLength());
		memcpy(&resp,ptBufferReceived->GetData(),nSize);

		m_nFilesystemType = resp.nFilesystemType;
		m_nBlockSize = resp.nBlockSize;
		m_nTotalBlockCount = resp.nTotalBlockCount;
		m_nFreeBlockCount = resp.nFreeBlockCount;
		m_nAvailableBlockCount = resp.nAvailableBlockCount;
		m_nTotalFileNumber = resp.nTotalFileNumber;
		m_nFreeFileNumber = resp.nFreeFileNumber;
		m_nFileSystemID = resp.nFileSystemID;
		m_nMaximumFilename = resp.nMaximumFilename;
		m_bStatus = resp.nStatus;

		CDiagRequestBase::OnDataReceiver(ptBufferReceived);
	}
}
