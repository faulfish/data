#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "stdio.h"


#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

enum sub_cmd_code{
    _EFS2_DIAG_HELLO    = 0,        //!<' Parameter negotiation packet
    _EFS2_DIAG_QUERY    = 1,        //!<' Send information about EFS2 params
    _EFS2_DIAG_OPEN     = 2,        //!<' Open a file
    _EFS2_DIAG_CLOSE    = 3,        //!<' Close a file
    _EFS2_DIAG_READ     = 4,        //!<' Read a file
    _EFS2_DIAG_WRITE    = 5,        //!<' Write a file
    _EFS2_DIAG_SYMLINK  = 6,        //!<' Create a symbolic link
    _EFS2_DIAG_READLINK = 7,        //!<' Read a symbolic link
    _EFS2_DIAG_UNLINK   = 8,        //!<' Remove a symbolic link or file
    _EFS2_DIAG_MKDIR    = 9,        //!<' Create a directory
    _EFS2_DIAG_RMDIR    = 10,       //!<' Remove a directory
    _EFS2_DIAG_OPENDIR  = 11,       //!<' Open a directory for reading               */
    _EFS2_DIAG_READDIR  = 12,       //!<' Read a directory                           */
    _EFS2_DIAG_CLOSEDIR = 13,       //!<' Close an open directory                    */
    _EFS2_DIAG_RENAME   = 14,       //!<' Rename a file or directory                 */
    _EFS2_DIAG_STAT     = 15,       //!<' Obtain information about a named file      */
    _EFS2_DIAG_LSTAT    = 16,       //!<' Obtain information about a symbolic link   */
    _EFS2_DIAG_FSTAT    = 17,       //!<' Obtain information about a file descriptor */
    _EFS2_DIAG_CHMOD    = 18,       //!<' Change file permissions                    */
    _EFS2_DIAG_STATFS   = 19,       //!<' Obtain file system information             */
    _EFS2_DIAG_ACCESS   = 20,       //!<' Check a named file for accessibility       */
    _EFS2_DIAG_NAND_DEV_INFO    = 21,   //!<' Get NAND device info */
    _EFS2_DIAG_FACT_IMAGE_START = 22,   //!<' Start EFS2 data output for Factory Image */
    _EFS2_DIAG_FACT_IMAGE_READ  = 23,   //!<' Get EFS2 data for Factory Image */
    _EFS2_DIAG_FACT_IMAGE_END   = 24,   //!<' End EFS2 data output for Factory Image */
    _EFS2_DIAG_PREP_FACT_IMAGE  = 25,   //!<' Prepare file system for image dump */
    _EFS2_DIAG_PUT      = 26,           //!<' 
    _EFS2_DIAG_GET      = 27,           //!<' 
    _EFS2_DIAG_ERROR    = 28,           //!<' 
    _EFS2_DIAG_EXTENDED_INFO    = 29    //!<' Get Extra information. */
 };

namespace PLATFORM_TOOL
{

	class RM_CLASS_API CEFsRequest  : public CDiagRequestBase
	{
	public:
		CEFsRequest();
		virtual ~CEFsRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsOpenFileRequest: public CDiagRequestBase
	{
	public:
		CEFsOpenFileRequest(const TCHAR* sFileName, const long iFlag, const long iMode);
		virtual ~CEFsOpenFileRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	
	class RM_CLASS_API CEFsCloseFileRequest: public CDiagRequestBase
	{
	public:
		CEFsCloseFileRequest(const DWORD iFd);
		virtual ~CEFsCloseFileRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsReadFileRequest: public CDiagRequestBase
	{
	public:
		CEFsReadFileRequest(const DWORD iFb, const DWORD iBytes, const DWORD iOffset);
		virtual ~CEFsReadFileRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsWriteFileRequest: public CDiagRequestBase
	{
	public:
		CEFsWriteFileRequest(const DWORD iFd, const DWORD iOffset, const TCHAR* sData, const DWORD iSize);
		virtual ~CEFsWriteFileRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsDeleteFileRequest: public CDiagRequestBase
	{
	public:
		CEFsDeleteFileRequest(const TCHAR *sFilename);
		virtual ~CEFsDeleteFileRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	
	class RM_CLASS_API CEFsMakeDirRequest: public CDiagRequestBase
	{
	public:
		CEFsMakeDirRequest(const WORD iMode, const TCHAR* sFolderPath);
		virtual ~CEFsMakeDirRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsCreateLinkRequest: public CDiagRequestBase
	{
	public:
		CEFsCreateLinkRequest(const TCHAR* sFilePath, const TCHAR *sPointedFilePath);
		virtual ~CEFsCreateLinkRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsReadLinkRequest: public CDiagRequestBase
	{
	public:
		CEFsReadLinkRequest(const TCHAR* sFilePath);
		virtual ~CEFsReadLinkRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	
	class RM_CLASS_API CEFsOpenDirRequest: public CDiagRequestBase
	{
	public:
		CEFsOpenDirRequest(const TCHAR* sFolderPath);
		virtual ~CEFsOpenDirRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsCloseDirRequest: public CDiagRequestBase
	{
	public:
		CEFsCloseDirRequest(const DWORD iDirp);
		virtual ~CEFsCloseDirRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsFileSystemStatusRequest  : public CDiagRequestBase
	{
	public:
		CEFsFileSystemStatusRequest(const TCHAR * sFilePath);
		virtual ~CEFsFileSystemStatusRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsFlashDeviceInfoRequest  : public CDiagRequestBase
	{
	public:
		CEFsFlashDeviceInfoRequest(void);
		virtual ~CEFsFlashDeviceInfoRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsCheckAccessAttrRequest  : public CDiagRequestBase
	{
	public:
		CEFsCheckAccessAttrRequest(const WORD iPermissionBits, const TCHAR* sFilePath);
		virtual ~CEFsCheckAccessAttrRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsChomdRequest  : public CDiagRequestBase
	{
	public:
		CEFsChomdRequest(const DWORD iMode, const TCHAR* sFilePath);
		virtual ~CEFsChomdRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CEFsQueryFileStatusRequest  : public CDiagRequestBase
	{
	public:
		CEFsQueryFileStatusRequest(const TCHAR* sFilePath);
		virtual ~CEFsQueryFileStatusRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};



	class RM_CLASS_API CEFsRenameRequest  : public CDiagRequestBase
	{
	public:
		CEFsRenameRequest(const TCHAR* sOldPathName, const TCHAR* sNewPathName);
		virtual ~CEFsRenameRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	class RM_CLASS_API CEFsRemoveFolderRequest  : public CDiagRequestBase
	{
	public:
		CEFsRemoveFolderRequest(const TCHAR* sFolderPath);
		virtual ~CEFsRemoveFolderRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	class RM_CLASS_API CEFsReadDirRequest  : public CDiagRequestBase
	{
	public:
		CEFsReadDirRequest(const DWORD dirp, const int iseqno);
		virtual ~CEFsReadDirRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CGetMicroSDInfoRequest  : public CDiagRequestBase
	{
	public:
		CGetMicroSDInfoRequest(int& nFilesystemType,int& nBlockSize,int& nTotalBlockCount,int& nFreeBlockCount,
			int& nAvailableBlockCount,int& nTotalFileNumber,int& nFreeFileNumber,int& nFileSystemID
			,int& nMaximumFilename,BYTE& bStatus);
		virtual ~CGetMicroSDInfoRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nFilesystemType;
		int& m_nBlockSize;
		int& m_nTotalBlockCount;
		int& m_nFreeBlockCount;

		int& m_nAvailableBlockCount;
		int& m_nTotalFileNumber;
		int& m_nFreeFileNumber;
		int& m_nFileSystemID;
		int& m_nMaximumFilename;
		BYTE& m_bStatus;
		/*
		f_type	4	Type of filesystem
		F_bsize	4	Filesystem blocksize
		f_blocks	4	Total data blocks in filesystem
		F_bfree	4	Free blocks in filesystem
		f_bavail	4	Free blocks available to user use
		F_files	4	Total number of file nodes in FS
		f_ffree	4	Free file nodes in FS
		F_fsid	4	File system ID
		f_namelen	4	Maximum filename length
		fs_stat	1	File system status
		*/
	};
}
