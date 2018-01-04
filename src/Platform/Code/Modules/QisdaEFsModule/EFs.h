#pragma once
#include "..\..\lib\requestutility\interfacebase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\RequestManager\Request\EFsRequest.h"

#ifdef QISDA_EFS_MODULE_EXPORTS
#define QISDA_EFS_API extern "C" __declspec(dllexport)
#define QISDA_EFS_CLASS_API __declspec(dllexport)
#else
#define QISDA_EFS_API extern "C" __declspec(dllimport)
#define QISDA_EFS_CLASS_API __declspec(dllimport)
#endif

/******************************************************************************
						Definitions for EFS operations
*******************************************************************************/

/* "User" permissions. */
#define S_IRUSR         0400
#define S_IWUSR         0200
#define S_IXUSR         0100

/* "Group" permissions. */
#define S_IRGRP         0040
#define S_IWGRP         0020
#define S_IXGRP         0010

/* "Other" permissions. */
#define S_IROTH         0004
#define S_IWOTH         0002
#define S_IXOTH         0001

/* BSD definitions. */
#undef S_IREAD
#define S_IREAD         S_IRUSR


#undef S_IWRITE
#define S_IWRITE        S_IWUSR

#undef S_IEXEC
#define S_IEXEC         S_IXUSR

/* Common combinations of bits. */
#define ACCESSPERMS     0777
#define ALLPERMS        0777
#define DEFFILEMODE     0666

//! EFS2 Enumerate directory
#define ENUM_DIRS 1

//! EFS2 Enumerate files
#define ENUM_FILES 0

//! EFS2 Enumerate links
#define ENUM_LINKS 2

#define O_ACCMODE          0003
#define O_RDONLY             00
#define O_WRONLY             01
#define O_RDWR               02
#define O_CREAT            0100 /* not fcntl */
#define O_EXCL             0200 /* not fcntl */
#define O_NOCTTY           0400 /* not fcntl */
#define O_TRUNC           01000 /* not fcntl */
#define O_APPEND          02000
#define O_NONBLOCK        04000
#define O_NDELAY        O_NONBLOCK
#define O_SYNC           010000
#define FASYNC           020000 /* fcntl, for BSD compatibility */
#define O_DIRECT         040000 /* direct disk access hint */
#define O_LARGEFILE     0100000
#define O_DIRECTORY     0200000 /* must be a directory */
#define O_NOFOLLOW      0400000 /* don't follow links */
#define O_ITEMFILE     01000000 /* Create special ITEM file. */
#define O_AUTODIR      02000000 /* Allow auto-creation of directories */

# define SEEK_SET       0       /* Seek from beginning of file.  */
# define SEEK_CUR       1       /* Seek from current position.  */
# define SEEK_END       2       /* Seek from end of file.  */

#define EFS_IOFBF       0       /* Full buffering.    */
#define EFS_IOLBF       1       /* Line buffering.    */
#define EFS_IONBF       2       /* No buffering.      */

#define DIAG_FS_MAX_FILE_BLOCK_SIZE  256 /* Limited by req buffer of 256 */
#define FPOS( type, field ) \
    /*lint -e545 */   ( (DWORD) &(( type near *) 0)-> field )   /*lint +e545 */

#define NO_TRANSMISSION_RETRIES 5
#define EFS2_WRITE_PACKET_SIZE 524
#define EFS2_READ_PACKET_SIZE 536 
#define EFS2_MAX_DATA_SIZE 0x200

#define DIAG_FS_MAX_FILENAME_LEN      80 /* Specified by EFS2              */
#define DIAG_FS_MAX_PATHNAME_LEN     128 /* Specified by EFS2              */

#pragma pack(push, 1)

/**
	File system read request
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD	fd;					//!<' File descriptor
	DWORD	nbyte;				//!<' Number of bytes to read
	DWORD offset;				//!<' Offset in bytes from the origin
} efs2_diag_read_req_type;


/**
	File system open response
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	int fd;				//!<' File descriptor if successful, -1 otherwise
	int errnum;			//!<' Error code if error, 0 otherwise
	
} efs2_diag_open_rsp_type;


typedef struct
{
	WORD attribute_mask;
	BYTE buffering_option;
	BYTE cleanup_option;
}EfsFileAttributes;


/**
	Open File Packet

	Used to open a named file.

	The following oflag values are valid:
	0x0000: O_RDONLY (open for reading mode)
	0x0001: O_WRONLY (open for writing mode)
	0x0002: O_RDWR   (open for reading and writing)
	0x0002: O_TRUNC  (if successfully opened, truncate length to 0)
	0x0100: O_CREAT  (create file if it does not exist)

	The O_CREAT flag can be orred with the other flags.
	The mode field is ignored unless the O_CREAT flag is specified.
	If O_CREAT is specified, the mode is a three-digit octal number with
	each octet representing read, write and execute permissions for owner,
	group and other.

	Note: If a request is received to open an already open file, the file
	is closed and reopened.
*/


typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD oflag;					//!<' Open flags
	DWORD mode;						//!<' Mode
	char  path[1];					//!<' Pathname (null-terminated string)
} efs2_diag_open_req_type;



/**
	File system read response
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD fd;					//!<' File descriptor
	DWORD offset;				//!<' Requested offset in bytes from the origin
	DWORD bytes_read;			//!<' bytes read if successful, -1 otherwise
	DWORD   errnum;				//!<' Error code if error, 0 otherwise
	char   data[1];			//!<' The data read out
} efs2_diag_read_rsp_type;


/**
	Close File Packet request 

	Used to close a file descriptor.
	Note: If a request is received to close an already closed file, nothing
	is done but an error is not reported.
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD fd;                //!<' File descriptor
} efs2_diag_close_req_type;

/**
	Close File Packet response

	Used to close a file descriptor.
	Note: If a request is received to close an already closed file, nothing
	is done but an error is not reported.
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD errnum;             //!<' Error code if error, 0 otherwise
} efs2_diag_close_rsp_type;


/**
	Write File Packet request
	
	Used to write data into a given offset in an open file. If the offset is
	past the current end of the file, the file is zero-filled until the offset
	and then the data is written to the file. Note that the request packet
	does not explicitly contain the number of bytes to be written, since this
	can be computed from the packet size. Unless an error occurs, all the
	bytes specified in the data field of the request packet are written to
	the file.
	
	NOTE 1: The fd is part of the response packet because we want to allow for
	the possibility of more than one file being open for writing. The fd allows
	the tool to match a response with a request.
	
	NOTE 2: This is one of the packets that support windowing. Thus there can
	be several requests outstanding before a response is sent.
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD fd;			//!<' File descriptor
	DWORD offset;		//!<' Offset in bytes from the origin
	char  data[1];	//!<' The data to be written
} efs2_diag_write_req_type;

/**
	Write file response
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD fd;					//!<' File descriptor
	DWORD offset;				//!<' Requested offset in bytes from the origin
	DWORD bytes_written;		//!<' The number of bytes written
	int errnum;					//!<' Error code if error, 0 otherwise
} efs2_diag_write_rsp_type;

/**
	delete file request
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;					
	char  path[1];					//!<' Pathname (null-terminated string)
} efs2_diag_delete_req_type;

/**
	delete File response
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	int errnum;			//!<' Error code if error, 0 otherwise
} efs2_diag_delete_rsp_type;

typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	char  path[1];				//!<' Pathname (null-terminated string)
} efs2_diag_opendir_req_type;

typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD dirp;				//!<' Directory pointer. NULL if error
	DWORD errnum;				//!<' Error code if error, 0 otherwise
} efs2_diag_opendir_rsp_type;

/**
	Read Directory Packet request

	Used to read the next entry from an open directory. Returns the type
	of the entry and its name. The directory pointer passed in is returned
	in the response packet so that the tool can match it to the request
	packet. The request packet contains a sequence number so that the tool
	can request to retrieve an entry again if the response packet does not
	get through.

	NOTE: This is one of the packets that support windowing. Thus there can
	be several requests outstanding before a response is sent.
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD dirp;					//!<' Directory pointer.
	DWORD seqno;					//!<' Sequence number of directory entry to read
} efs2_diag_readdir_req_type;

/**
	Read Directory Packet response
*/
typedef struct 
{
  diagpkt_subsys_hdr_type hdr;
  DWORD dirp;					//!<' Directory pointer.
  DWORD seqno;					//!<' Sequence number of directory entry
  DWORD errnum;					//!<' Error code if error, 0 otherwise
  DWORD entry_type;				//!<' 0 if file, 1 if directory, 2 if symlink
  DWORD mode;					//!<' File mode
  DWORD size;					//!<' File size in bytes
  DWORD atime;					//!<' Time of last access
  DWORD mtime;					//!<' Time of last modification
  DWORD ctime;					//!<' Time of last status change
  char   entry_name[1];			//!<' Name of directory entry (not full pathname)
} efs2_diag_readdir_rsp_type;

/**
	Close Directory Packet request

	Used to close an open directory.

	Note: If a request is received to close an already closed directory,
	nothing is done but an error is not returned.
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	DWORD dirp;					//!<' Directory pointer.
} efs2_diag_closedir_req_type;

/**
	Close Directory Packet response
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr;
	int errnum;					//!<' Error code if error, 0 otherwise             */
} efs2_diag_closedir_rsp_type;


/**
	Make Directory Packet request

	Used to create a directory.

	Note: If request is received to create an existing directory, the
	directory will be removed if possible and recreated. If directory
	removal fails (because it is not empty, typically) then an error is
	returned.
*/
typedef struct 
{
  diagpkt_subsys_hdr_type hdr;
  WORD mode;				//!<' The creation mode
  char  path[1];			//!<' Pathname (null-terminated string)
} efs2_diag_mkdir_req_type;

/**
	Make Directory Packet response
*/
typedef struct 
{
  diagpkt_subsys_hdr_type hdr;
  int errnum;				//!<' Error code if error, 0 otherwise
} efs2_diag_mkdir_rsp_type;

/**
	Create link Packet request

*/
typedef struct 
{
  diagpkt_subsys_hdr_type hdr;	
  char  path[1];
  char  pointedpath[1];  //!<' Pathname (null-terminated string)
} efs2_diag_createlink_req_type;

/**
	Create link Packet response
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr; 
	int errnum;           
} efs2_diag_createlink_rsp_type;

/**
	Read link Packet request

*/
typedef struct 
{
  diagpkt_subsys_hdr_type hdr;	
  char  path[1];			//!<' Pathname (null-terminated string)
} efs2_diag_readlink_req_type;

/**
	Read link Packet response
*/
typedef struct 
{
	diagpkt_subsys_hdr_type hdr; 
	int errnum;  
	char  pointedpath[1];           
} efs2_diag_readlink_rsp_type;
#pragma pack(pop)


typedef struct 
{
	// to avoid storin too lon paths
	char name[DIAG_FS_MAX_PATHNAME_LEN+1];  //defined in DiagEfsPkt.h
	// 0 - dir, 1 - file, 2 - link
	int isFile;

	//attributes follow below
	unsigned long attributeMask;
	unsigned char bufferingOption;
	unsigned char cleanupOption;
	unsigned long createDate;
	unsigned long fileSize;
} Efs2DirItem;

#include "..\..\CommonUtil\RefObject.h"

class QISDA_EFS_CLASS_API CEFs : public CInterfaceBase
{
public:
	CEFs(int nCOMPort);
	bool EfsReadFile(const char *sFileSrc, const char *sFileDst, long &iErrorCode);
	bool EfsWriteFile(const char *sFileSrc, const char *sFileDst,long &iErrorCode);
	bool EfsWriteFileFromMem(const char *sData, const DWORD iSize, const char *sFileDst,long &iErrorCode);	
	bool EfsDeleteFile(const char *sEFS_File,long &iErrorCode);
	bool EfsCreateLink(const char* sFilePath, const char* sPointedFilePath,long &iErrorCode);
	bool EfsReadLink(const char* sFilePath , char* sPointedFilePath,long &iErrorCode);
	bool EfsMakeFolder(const char* strDir,long &iErrorCode);
	bool EFsQueryFileSystemStatus(const char * sFilePath, DWORD &iFS_I, DWORD &iFS_Type, DWORD &iBlockSize, DWORD &iTotalBlocks, \
									DWORD &iAvailBlocks, DWORD &iFreeBlocks, DWORD &iMaxFileSize, DWORD &iNfiles, DWORD &iMaxNfiles,long &iErrorCode );
	bool EFsQueryFlashDeviceInfo(DWORD& iTotalNumOfBlocks,DWORD& iNumPagesPerBlock,\
									DWORD& iPageSize,DWORD& iTotalPageSize,DWORD& iMakerId,DWORD& iDeviceId,\
									BYTE& iDeviceType,	char* psDeviceName,long &iErrorCode);
	bool EFsCheckAccessAttribute(const unsigned short iPermissionBits, const char* sFilePath, long& iErrorCode);
	bool EFsChomd(const DWORD iMode, const char* sFilePath,long& iErrorCode);
	bool EFsRemoveFolder(const char* sFolderPath, long &iErrorCode);
	bool EFsRename(const char* sOldPathName, const char* sNewPathName, long& iErrorCode);
	bool EFsQueryFStatus(const char* sFilePath, DWORD& iMode, DWORD& iSize, DWORD& iNlink, DWORD& iAtime, DWORD& iMtime, DWORD& iCtime,long &iErrorCode);
	bool EfsDirectoryList(const char *strDir, std::vector<Efs2DirItem> *pDirList, DWORD &iSize, BYTE entryType,long &iErrorCode);
	bool GetMicroSDInfo(int& nFilesystemType,int& nBlockSize,int& nTotalBlockCount,int& nFreeBlockCount,
			int& nAvailableBlockCount,int& nTotalFileNumber,int& nFreeFileNumber,int& nFileSystemID
			,int& nMaximumFilename,BYTE& bStatus);

private:
	bool EFSFileOpen(const char *strFile, const long oflag, const long mode, long& fd,long &iErrorCode);
	bool EFSFileClose(const long fd,long &iErrorCode);
	bool EFSDirOpen(const char *strPath, long& dirp,long &iErrorCode);
	bool EFSDirClose(const long dirp,long &iErrorCode);
	bool PerformEFS2WriteOp(const char *strEfsFile, const BYTE mode, EfsFileAttributes attrib,long &iErrorCode,const char *pUnkStream, 
							const int mem=0, const unsigned long iSize=0);

public:
	virtual ~CEFs(void);
};

class QISDA_EFS_CLASS_API CCFCEFs : public CEFs, public CRefObject
{
public:
	CCFCEFs(int nCOMPort);
	virtual ~CCFCEFs();
public:
	bool GetLocFile(const char* szDstFilePath);
	bool GetLocFileName(char *szFileName);
};

typedef CRefObjectPtr<CCFCEFs> CCFCEFSModulePtr;