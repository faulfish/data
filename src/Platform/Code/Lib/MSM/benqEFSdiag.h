#ifndef _BENQEFSDIAG_H_
#define _BENQEFSDIAG_H_

#include "comdef.h"

/* Operation status values */
/* Do not move these values around as they must be in sysn with diag */
typedef enum {
  FS_OKAY_S,                /* Good status                                 */
  FS_FAIL_S,                /* Low-level operation failed                  */
  FS_BUSY_S,                /* Operation is queued or in progress          */
  FS_FILE_NOT_OPEN_S,       /* File needs to be opened for this operation  */
  FS_FILE_OPEN_S,           /* File needs to be closed for this operation  */
  FS_FILE_ALREADY_EXISTS_S, /* File already exists                         */
  FS_NONEXISTENT_FILE_S,    /* File doesn't exist                          */
  FS_DIR_ALREADY_EXISTS_S,  /* User directory already exists               */
  FS_NONEXISTENT_DIR_S,     /* User directory doesn't exist                */
  FS_DIR_NOT_EMPTY_S,       /* User directory not empty for remove         */
  FS_UNKNOWN_OPERATION_S,   /* Client specified an unknown operation       */
  FS_ILLEGAL_OPERATION_S,   /* Client specified an invalid operation       */
  FS_PARAMETER_ERROR_S,     /* Client supplied a bad parameter value       */
  FS_BAD_FILE_NAME_S,       /* Client specified invalid file/directory name*/
  FS_BAD_FILE_HANDLE_S,     /* Client specified invalid file handle        */
  FS_BAD_SEEK_POS_S,        /* Client specified an invalid SEEK position   */
  FS_BAD_TRUNCATE_POS_S,    /* Client supplied an invalid truncate position*/
  FS_FILE_IS_REMOTE_S,      /* Operation invalid for remote files          */
  FS_INVALID_CONF_CODE_S,   /* Invalid confirmation code specified         */
  FS_EOF_S,                 /* End of file reached                         */
  FS_MORE_S,                /* More records exist to be processed          */
  FS_GC_IN_PROGRESS_S,      /* Garbage Collection in progress              */
  FS_SPACE_EXHAUSTED_S,     /* Out of file system space                    */
  FS_BLK_FULL_S,            /* File block is full                          */
  FS_DIR_SPACE_EXHAUSTED_S, /* Out of Master Directory space               */
  FS_FBL_POOL_EXHAUSTED_S,  /* Out of File Block List free pool space      */
  FS_OPEN_TABLE_FULL_S,     /* Out of open-file table slots                */
  FS_INVALID_ITERATOR_S,    /* Invalid enumeration iterator control struct */
  FS_ENUM_DONE_S,           /* Enumeration operation completed             */
  FS_RMLINK_EXHAUSTED_S,    /* Maximum number of remote links reached      */
  FS_NO_MEDIA_S,            /* Removable media not inserted                */
  FS_MEDIA_CHANGED_S        /* Media has been changed since opened         */
} fs_status_type;

/**************************************************************************************
 *   File System                                                                      *   
 **************************************************************************************/
/*
 *	STATUS ¡V Status of specified operation
         0 ¡V Success
         1 ¡V FS device failure
         2 ¡V Reserved
         3 ¡V Reserved
         4 ¡V File open
         5 ¡V File already exists
         6 ¡V File does not exist
         7 ¡V Directory already exists
         8 ¡V Directory does not exist
         9 ¡V Directory not empty
        10 ¡V Unknown operation
        11 ¡V Illegal operation
        12 ¡V Parameter error
        13 ¡V Bad file name
        14 ¡V Reserved
        15 ¡V Reserved
        16 ¡V Reserved
        17 ¡V Invalid operation for remote file
        18 ¡V Reserved
        19 ¡V Reserved
        20 ¡V Reserved
        21 ¡V Reserved
        22 ¡V Out of file system space
        23 ¡V Block full
        24 ¡V Out of master directory space
        25 ¡V Out of file block list (similar to I-nodes) space
 */

#define BENQDIAG_MAX_FILENAME_LEN       60 /* Specified by FS              */
#define BENQDIAG_MAX_FILE_BLOCK_SIZE   256 /* Limited by req buffer of 256 */

typedef enum {
	BENQDIAG_FS_MK_DIR       = 0,      /* Create directory                */
	BENQDIAG_FS_RM_DIR       = 1,      /* Remove Directory                */
	BENQDIAG_FS_DISP_DIRS    = 2,      /* Display directories list        */
	BENQDIAG_FS_DISP_FILES   = 3,      /* Display file list               */
									  /* NOTE: Display ops are temporary *
									   * implementations                 */ 
	BENQDIAG_FS_READ_FILE    = 4,      /* Read a file in FS               */
	BENQDIAG_FS_WRITE_FILE    = 5,      /* Write a file in FS              */  
	BENQDIAG_FS_REMOVE_FILE  = 6,      /* Remove a file from FS           */  
	BENQDIAG_FS_GET_ATTRIB   = 7,      /* Get file attributes             */
	BENQDIAG_FS_SET_ATTRIB    = 8,      /* Set file attributes             */
	BENQDIAG_FS_REMOTE_LINK  = 9,      /* Create a remote file link       */
	BENQDIAG_FS_ITER_DIRS     = 10,     /* Iterative display directories   */
	BENQDIAG_FS_ITER_FILES    = 11,     /* Iterative display files         */
	BENQDIAG_FS_SPACE_AVAIL  = 12,     /* Amount of free space in EFS     */
	BENQDIAG_FS_LAST_OP,               /* Last OP.  For range checking.   */
}benqdiag_fs_operation_e_type;

/* File System header type */
typedef struct 
{
	uint8     command_code;
	uint8     subsys_id;
	uint16    subsys_cmd_code;
	uint8	  fs_operation;
}benqdiag_fs_hdr_type;

typedef struct
{
	byte   len;                                      /* Length of null-terminated filename string( NULL 
													 character included), maximum of 60 bytes        */
	char   name[BENQDIAG_MAX_FILENAME_LEN];  /* Null-terminated ASCII string containing the 
                                                absolute name of the file or directory being
                                                specified(unless otherwise specified)
                                                Valid data len == filename_len+1              */
}benqdiag_fs_filename_type;

typedef struct
{
	word   len;                                      /* Length of data block, maximum of 256 bytes */
	byte   data[BENQDIAG_MAX_FILE_BLOCK_SIZE];  /* Data for this block  */
}benqdiag_fs_data_block_type;

/* File attribute mask */
typedef enum 
{
	BENQDIAG_FA_UNRESTRICTED     =0x00FF,  /* No file access restrictions           */
	BENQDIAG_FA_PERMANENT        =0x007F,  /* File can't be removed nor truncated   */
	BENQDIAG_FA_READONLY          =0x00BF,  /* File can't be opened for update       */
	BENQDIAG_FA_SYS_PERMANENT   =0x0037,  /* File persists across system reformats */
	BENQDIAG_FA_REMOTE            =0x001F   /* Remote file (resides outside file   *
												  * system address space)          */
}benqdiag_fs_attribute_mask_type;     

/* File OPEN operation buffering options */
typedef enum 
{
	BENQDIAG_DIAGPKT__FS_OB_PROHIBIT,    /* Prohibit file buffering */
	BENQDIAG_DIAGPKT__FS_OB_ALLOW      /* Allow file buffering    */
}benqdiag_buffering_option_type;

/* File OPEN operation clean-up options */
typedef enum 
{
	BENQDIAG_DIAGPKT__FS_OC_CLOSE       = 0x00, /* Close file as-is                           */
	BENQDIAG_DIAGPKT__FS_OC_DISCARD     = 0x01, /* Delete file and remove it from directory   */
	BENQDIAG_DIAGPKT__FS_OC_TRUNCATE    = 0x02, /* Truncate file from specified position      */
	BENQDIAG_DIAGPKT__FS_OC_REVERT      = 0x03  /* Revert to last checkpointed version        */
}benqdiag_cleanup_option_type;

typedef struct
{
	word    attribute_mask;    /* Use fsdiag_attribute_mask_type   */
	byte    buffering_option;  /* Use fsdiag_buffering_option_type */
	byte    cleanup_option;    /* Use fsdiag_cleanup_option_type   */
}benqdiag_fs_attributes_type;

///*-------------------------------------------------------
//
//Definitions of data for specific operations.
//
//-------------------------------------------------------*/
/* Create Directory */
typedef struct 
{
	benqdiag_fs_hdr_type    hdr;
	benqdiag_fs_filename_type        name_info;
}benqdiag_fs_mkdir_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type    hdr;
	byte   fs_status;
}benqdiag_fs_mkdir_rsp_type;

/* Remove Directory */
typedef struct 
{
	benqdiag_fs_hdr_type        hdr;
	benqdiag_fs_filename_type        name_info;
}benqdiag_fs_rmdir_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type        hdr;
	byte     fs_status;
}benqdiag_fs_rmdir_rsp_type;

/* Display Directory List */
typedef struct 
{
	benqdiag_fs_hdr_type     hdr;
	benqdiag_fs_filename_type        name_info;
}benqdiag_fs_disp_dirs_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type     hdr;
	byte        fs_status;
	word    num_dirs;
	benqdiag_fs_data_block_type      dir_list;
}benqdiag_fs_disp_dirs_rsp_type;

/* Display File List */
typedef struct 
{
	benqdiag_fs_hdr_type        hdr;
	benqdiag_fs_filename_type        name_info;
}benqdiag_fs_disp_files_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type     hdr;
	byte                      fs_status;
	word                     num_files;
	benqdiag_fs_data_block_type   file_list;
}benqdiag_fs_disp_files_rsp_type;

/* Read File */
typedef union
{ 
	struct
	{
		dword                        total_length;    /* Total length of this file, specified in first block of read-only */
		benqdiag_fs_data_block_type   block;         /* Data block */
	}begin;
	benqdiag_fs_data_block_type  block; 
}benqdiag_fs_read_block_type;

typedef struct
{
	benqdiag_fs_hdr_type hdr;
	byte                        seq_num;    /* Sequence number; 0 indicates new listing */
	benqdiag_fs_filename_type   name_info;   /* File name info. Used only if seq_num == 0        *
											* Note: Only include this data if requesting          *
											* the first block of a file. If SEQ_NUM is nonzero,    *
											* the field is omitted                             */
}benqdiag_fs_read_req_type;

typedef struct
{
	benqdiag_fs_hdr_type hdr;
	byte                          fs_status;      
	byte                          seq_num;    /* Sequence number for multiple block reads   *
										   * This number is 0 for the first block of a file   */
	byte                          more;       /* Flag to specify if more blocks are to be sent to complete the file transfer *
																					  * 0-No more blocks will follow                                      *
																					  * 1-At least one more block remains                                 */
	benqdiag_fs_read_block_type   read_data;  /* Current portion of the file */
}benqdiag_fs_read_rsp_type;

/* Write File */
typedef enum
{
	BENQDIAG_FS_NO_OVERWRITE = 0,
	BENQDIAG_FS_OVERWRITE    = 1
}benqdiag_fs_write_mode_e_type;

typedef struct
{ 
	byte                          mode;        /* Use benqdiag_fs_write_mode_enum_type */
	dword                        total_length;   /* Total length of this file to be transfered */
	benqdiag_fs_attributes_type     attrib;        /* Attributes for this file */
	union
	{
			benqdiag_fs_filename_type   name_info;     /* File name info                                      *
													  * Note: Only include this data if sending the first block      *
													  * of a file. During subsequent blocks, this field has length 0  */
			byte                       raw_data[ sizeof(benqdiag_fs_filename_type) +
												 sizeof(benqdiag_fs_data_block_type) ];  /* Full possible size of variable *
																					  * length buffer                  */                                                
	}var_len_buf;
}benqdiag_fs_write_begin_req_type;

typedef union
{
	benqdiag_fs_write_begin_req_type  begin;   /* First block of a write           */
	benqdiag_fs_data_block_type       subseq;  /* Subsequent blocks for write      */
}benqdiag_fs_write_block_type;

typedef struct
{
	benqdiag_fs_hdr_type			hdr;

	byte							seq_num;  /* Sequence number for multiple blocks *
												* use 0 to start a new file           */
	byte							more;     /* Flag to specify if more blocks are to be sent to complete the file transfer *
												* 0-No more blocks will follow                                      *
												* 1-At least one more block remains                                 */
	benqdiag_fs_write_block_type	block;    /* Block for this portion of the file */
}benqdiag_fs_write_req_type;

typedef struct
{
	benqdiag_fs_hdr_type	hdr;
	byte					fs_status;        
	byte					seq_num;  /* Sequence number of transfer */
}benqdiag_fs_write_rsp_type;

/* Remove File */
typedef struct 
{
	benqdiag_fs_hdr_type		hdr;
	benqdiag_fs_filename_type	name_info;
}benqdiag_fs_rmfile_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type	hdr;
	byte					fs_status;
}benqdiag_fs_rmfile_rsp_type;

/* Get File Attributes */
typedef struct 
{
	benqdiag_fs_hdr_type			hdr;
	benqdiag_fs_filename_type		name_info;
}benqdiag_fs_get_attrib_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type			hdr;
	byte							fs_status;
	benqdiag_fs_attributes_type		attrib;         /* Attributes */
	dword							creation_date;  /* File creation date */
	dword							size;           /* File size */
}benqdiag_fs_get_attrib_rsp_type;

/* Set File Attributes */
typedef struct
{
	benqdiag_fs_hdr_type            hdr;
	benqdiag_fs_attributes_type     attribs;
	dword							creation_date;
	benqdiag_fs_filename_type		name_info;
}benqdiag_fs_set_attrib_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type        hdr;
	byte     fs_status;
}benqdiag_fs_set_attrib_rsp_type;

/* Remote File Link */
typedef struct

{
	benqdiag_fs_hdr_type     hdr;
	dword                       base_address;
	dword                       length;
	benqdiag_fs_filename_type    name_info;
}benqdiag_fs_remote_link_req_type;

typedef struct 
{
	benqdiag_fs_hdr_type      hdr;
	byte    fs_status;
}benqdiag_fs_remote_link_rsp_type;

/* Iterate Directories */
/* Iterate files      */
typedef struct
{
	benqdiag_fs_hdr_type      hdr;
	dword                       seq_num;
	benqdiag_fs_filename_type    name_info;
}benqdiag_fs_iter_req_type;

typedef struct
{
	benqdiag_fs_hdr_type     hdr;
	byte                          fs_status;      
	dword                        seq_num;
	benqdiag_fs_attributes_type     attrib;          /* Attributes */
	dword                        creation_date;   /* File creation date */
	dword                        logical_size;    /* Size of data */
	dword                        physical_size;   /* Physical space on device */
	byte                          dirname_length;  /* Length of directory portion of file name */                                                  
	benqdiag_fs_filename_type     item_name;
} benqdiag_fs_iter_rsp_type;

/* Space available */
typedef struct
{
	benqdiag_fs_hdr_type hdr;
} benqdiag_fs_space_avail_req_type;

typedef struct
{
	benqdiag_fs_hdr_type hdr;
	byte fs_status;
	dword    space_avail;  /* Number of bytes available in the dile system */
} benqdiag_fs_space_avail_rsp_type;

/**************************************************************************************
 *   File System 2 Operations                                                           *   
 **************************************************************************************/
#define  PIM_EFS_FSTYPE_LEN              8
#define  PIM_MAX_READ_DATA_LEN           512
#define  PIM_MAX_WRITE_DATA_LEN          PIM_MAX_READ_DATA_LEN
#define  PIM_BENQ_FILE_EXT_HDR_TITLE_LEN				70
#define  BENQDIAG_FS_MAX_FILENAME_LEN    128 /* Specified by FS              */
#define  PIM_FS_FILENAME_MAX_LENGTH_P    BENQDIAG_FS_MAX_FILENAME_LEN

enum {
    DIAG_FS_STATFS,                   /* Init pt service book module */
    DIAG_FS_WRITE,                    /* Add new record or modify one record */    
    DIAG_FS_READ,                     /* Read one record by location */
    DIAG_FS_REMOVE,                   
    DIAG_FS_OPEN,                     /* Update Call Group */
	DIAG_FS_CLOSE,
	DIAG_FS_STAT,                     /* Query state of a file or a directory */
	DIAG_FS_ITER_FTITLE,
	DIAG_FS_GET_FTITLE,
	DIAG_FS_SET_FTITLE,
	DIAG_FS_MKDIR,
	DIAG_FS_RMDIR,
    DIAG_NUMFSOPERATIONS,              /* number of option events  */
	DIAG_FS_MAX = 0xFF
};

typedef struct 
{
    BYTE                 command_code;
    BYTE                 subsys_id;
    WORD                 subsys_cmd_code;
	BYTE                 fs_operation;
}benqdiag_efs2_hdr_type;

/*
 * Parameter Negotiation Packet
 *
 * Used by the tool and the target to negotiate parameter values. Typically
 * sent just once at the start of a session.
 *
 * Window size parameters: The tool and the target should both set their
 * internal window size parameters to the smaller of the sizes specified
 * in the request and response packets. For instance, if the tool's
 * value for the targ_pkt_window parameter is 8 and the target's value for
 * this parameter is 4, the tool will specify 8 as the value of this field
 * in the EFS2_DIAG_HELLO request packet. The target will respond with a
 * packet in which this field is set to 4. Once the request and response
 * packets have been exchanged, both the tool and the target will store a
 * value of 4 for the targ_pkt_window parameter.
 *
 * Version fields: The tool and the target use these fields to determine
 * whether their protocol version numbers are compatible.
 *
 * feature_bits parameter: At the end of the exchange, the tool and the
 * target should perform a logical and on the values of this parameter
 * specified in the request and response packets and use the resulting
 * value as the feature bit mask.
 *
 * NOTE: When the tool sends this packet, all open files and directories
 * are closed.
 */
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD targ_pkt_window;  /* Window size in packets for sends from phone  */
  DWORD targ_byte_window; /* Window size in bytes for sends from phone    */
  DWORD host_pkt_window;  /* Window size in packets for sends from host   */
  DWORD host_byte_window; /* Window size in bytes for sends from host     */
  DWORD iter_pkt_window;  /* Window size in packets for dir iteration     */
  DWORD iter_byte_window; /* Window size in bytes for dir iteration       */
  DWORD version;          /* Protocol version number                      */
  DWORD min_version;      /* Smallest supported protocol version number   */
  DWORD max_version;      /* Highest supported protocol version number    */
  DWORD feature_bits;     /* Feature bit mask; one bit per feature        */
} benqdiag_efs2_diag_hello_req_type;

 
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD targ_pkt_window;  /* Window size in packets for sends from phone  */
  DWORD targ_byte_window; /* Window size in bytes for sends from phone    */
  DWORD host_pkt_window;  /* Window size in packets for sends from host   */
  DWORD host_byte_window; /* Window size in bytes for sends from host     */
  DWORD iter_pkt_window;  /* Window size in packets for dir iteration     */
  DWORD iter_byte_window; /* Window size in bytes for dir iteration       */
  DWORD version;          /* Protocol version number                      */
  DWORD min_version;      /* Smallest supported protocol version number   */
  DWORD max_version;      /* Highest supported protocol version number    */
  DWORD feature_bits;     /* Feature bit mask; one bit per feature        */
} benqdiag_efs2_diag_hello_rsp_type;


/*
 * Statfs Packet
 *
 * Used to retrieve information about a filesystem. Note that the request
 * packet must specify a path to a file or directory. Information is returned
 * about the file system containing the named file or directory.
 */
 
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  char path[PIM_FS_FILENAME_MAX_LENGTH_P + 1];         /* Pathname (null-terminated string)               */
} benqdiag_efs2_diag_statfs_req_type;
 
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  DWORD err;             /* Error code if error, 0 otherwise             */
  DWORD fs_id;             /* Filesystem ID                                */
  BYTE  fs_type[PIM_EFS_FSTYPE_LEN];  /* Filesystem type                       */
  DWORD block_size;        /* Filesystem block size                        */
  DWORD total_blocks;      /* Filesystem size in blocks                    */
  DWORD avail_blocks;      /* Blocks available in filesystem               */
  DWORD free_blocks;       /* Blocks free in filesystem                    */
  DWORD max_file_size;     /* Maximum size of a file in this filesystem    */
  DWORD nfiles;            /* Current number of files in this filesystem   */
  DWORD max_nfiles;        /* Maximum number of files in this filesystem   */
} benqdiag_efs2_diag_statfs_rsp_type;

/*
 * Open File Packet
 *
 * Used to open a named file.
 *
 * The following oflag values are valid:
 * 0x0000: O_RDONLY (open for reading mode)
 * 0x0001: O_WRONLY (open for writing mode)
 * 0x0002: O_RDWR   (open for reading and writing)
 * 0x0002: O_TRUNC  (if successfully opened, truncate length to 0)
 * 0x0100: O_CREAT  (create file if it does not exist)
 *
 * The O_CREAT flag can be orred with the other flags.
 * The mode field is ignored unless the O_CREAT flag is specified.
 * If O_CREAT is specified, the mode is a three-digit octal number with
 * each octet representing read, write and execute permissions for owner,
 * group and other.
 *
 * Note: If a request is received to open an already open file, the file
 * is closed and reopened.
 */
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD oflag;             /* Open flags                                   */
  DWORD mode;              /* Mode                                         */
  char  path[PIM_FS_FILENAME_MAX_LENGTH_P + 1];           /* Pathname (null-terminated string)            */
} benqdiag_efs2_diag_open_req_type;
 
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD fd;                /* File descriptor if successful, -1 otherwise  */
  DWORD err;             /* Error code if error, 0 otherwise             */
} benqdiag_efs2_diag_open_rsp_type;


/*
 * Close File Packet
 *
 * Used to close a file descriptor.
 *
 * Note: If a request is received to close an already closed file, nothing
 * is done but an error is not reported.
 */
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD                  fd;                /* File descriptor                              */
} benqdiag_efs2_diag_close_req_type;
 
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD err;             /* Error code if error, 0 otherwise             */
} benqdiag_efs2_diag_close_rsp_type;
 
/*
 * Read File Packet
 *
 * Used to read the contents of an open file. Note that the response packet
 * does not explicitly contain the number of bytes read, since this can be
 * computed from the packet size. The number of bytes actually read may be
 * less than the number specified in the request packet if EOF is encountered
 * (this is not an error).
 *
 * NOTE 1: The fd is part of the response packet because we want to allow for
 * the possibility of more than one file being open for reading. The fd
 * allows the tool to match a response with a request.
 *
 * NOTE 2: This is one of the packets that support windowing. Thus there can
 * be several requests outstanding before a response is sent.
 */
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD  fd;               /* File descriptor                              */
  DWORD  nbyte;            /* Number of bytes to read                      */
  DWORD  offset;           /* Offset in bytes from the origin              */
} benqdiag_efs2_diag_read_req_type;

 
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD  fd;               /* File descriptor                              */
  DWORD  offset;           /* Requested offset in bytes from the origin    */
  DWORD  bytes_read;       /* bytes read if successful, -1 otherwise       */
  DWORD  err;            /* Error code if error, 0 otherwise             */
  char   data[PIM_MAX_READ_DATA_LEN];          /* The data read out        */
} benqdiag_efs2_diag_read_rsp_type;

/*
 * Write File Packet
 *
 * Used to write data into a given offset in an open file. If the offset is
 * past the current end of the file, the file is zero-filled until the offset
 * and then the data is written to the file. Note that the request packet
 * does not explicitly contain the number of bytes to be written, since this
 * can be computed from the packet size. Unless an error occurs, all the
 * bytes specified in the data field of the request packet are written to
 * the file.
 *
 * NOTE 1: The fd is part of the response packet because we want to allow for
 * the possibility of more than one file being open for writing. The fd allows
 * the tool to match a response with a request.
 *
 * NOTE 2: This is one of the packets that support windowing. Thus there can
 * be several requests outstanding before a response is sent.
 */
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD fd;                /* File descriptor                              */
  DWORD offset;            /* Offset in bytes from the origin              */
  DWORD nbyte;
  char  data[PIM_MAX_WRITE_DATA_LEN];           /* The data to be written                       */
} benqdiag_efs2_diag_write_req_type;

 
typedef struct 
{
  benqdiag_efs2_hdr_type hdr;
  DWORD fd;                /* File descriptor                              */
  DWORD offset;           /* Requested offset in bytes from the origin    */
  DWORD bytes_written;     /* The number of bytes written                  */
  DWORD err;             /* Error code if error, 0 otherwise             */
} benqdiag_efs2_diag_write_rsp_type;


typedef struct
{
  BYTE len;                            /* Length of filename string 
                                          including NULL terminating char   */
  char name[BENQDIAG_FS_MAX_FILENAME_LEN]; /* Filename string.  NULL terminated.
                                          Valid data len == filename_len+1  */
} benqdiag_fs2_filename_type;


typedef struct
{
    benqdiag_efs2_hdr_type hdr;
    benqdiag_fs2_filename_type fullname;
} benqdiag_fs2_get_ftitle_req_type;


typedef struct
{
    benqdiag_efs2_hdr_type hdr;
    char ftitle[PIM_BENQ_FILE_EXT_HDR_TITLE_LEN+2];
} benqdiag_fs2_get_ftitle_rsp_type;


typedef struct
{
    benqdiag_efs2_hdr_type hdr;
    benqdiag_fs2_filename_type fullname;
    char file_title[PIM_BENQ_FILE_EXT_HDR_TITLE_LEN+2];
} benqdiag_fs2_set_ftitle_req_type;


typedef struct
{
    benqdiag_efs2_hdr_type hdr;
    signed char            result;
} benqdiag_fs2_set_ftitle_rsp_type;


typedef struct
{
    BYTE len;                                   /* Length of filename string 
                                                   including NULL terminating char     */
    char ftitle[PIM_BENQ_FILE_EXT_HDR_TITLE_LEN+2]; /* Title name string.  NULL terminated */
} benqdiag_fs2_ftitle_type;


/* "Iterate Directories" operation */
typedef struct
{
    benqdiag_efs2_hdr_type hdr;
    DWORD                  seq_num;
    benqdiag_fs2_filename_type dir_name;
} benqdiag_fs2_iter_req_type;


/* "Iterate Directories" and "Iterate Files" operation */
typedef struct
{
  benqdiag_efs2_hdr_type hdr;
  
  BYTE  fs_status;               /* Status of operation - values in  
                                          fs_status_enum_type    */
  DWORD seq_num;
  benqdiag_fs2_filename_type  item_name;
  char title_name[PIM_BENQ_FILE_EXT_HDR_TITLE_LEN+2];
} benqdiag_fs2_iter_rsp_type;


/*
 * Stat Packet
 *
 * Used to retrieve information about a named file or directory.
 */
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  char path[PIM_FS_FILENAME_MAX_LENGTH_P + 1];         /* Pathname (null-terminated string)               */
} benqdiag_efs2_diag_stat_req_type;
 
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  DWORD err;             /* Error code if error, 0 otherwise             */
  DWORD mode;              /* File mode                                    */
  DWORD size;              /* File size in bytes                           */
  DWORD nlink;             /* Number of links                              */
  DWORD atime;             /* Time of last access                          */
  DWORD mtime;             /* Time of last modification                    */
  DWORD ctime;             /* Time of last status change                   */
} benqdiag_efs2_diag_stat_rsp_type;


/*
 * Make Directory Packet
 *
 * Used to create a directory.
 *
 * Note: If request is received to create an existing directory, the
 * directory will be removed if possible and recreated. If directory
 * removal fails (because it is not empty, typically) then an error is
 * returned.
 */
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  WORD mode;              /* The creation mode                            */
  char  path[PIM_FS_FILENAME_MAX_LENGTH_P + 1];           /* Pathname (null-terminated string)            */
} benqdiag_efs2_diag_mkdir_req_type;
 
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  DWORD err;             /* Error code if error, 0 otherwise             */
} benqdiag_efs2_diag_mkdir_rsp_type;
 
/*
 * Remove Directory Packet
 *
 * Used to delete a directory. The deletion will fail if the directory is
 * not empty.
 *
 * Note: If a request is received to remove an already removed directory,
 * nothing is done but an error is not reported.
 */
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  char  path[PIM_FS_FILENAME_MAX_LENGTH_P + 1];           /* Pathname (null-terminated string)            */
} benqdiag_efs2_diag_rmdir_req_type;
 
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  int32 err;             /* Error code if error, 0 otherwise             */
} benqdiag_efs2_diag_rmdir_rsp_type;

/*
 * Unlink Packet
 *
 * Used to remove a file or a symbolic link. The file is physically deleted
 * only if there are no more links to it.
 */
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  char  path[PIM_FS_FILENAME_MAX_LENGTH_P + 1];           /* Pathname (null-terminated string)            */
} benqdiag_efs2_diag_unlink_req_type;
 
typedef struct {
  benqdiag_efs2_hdr_type hdr;
  DWORD err;             /* Error code if error, 0 otherwise             */
} benqdiag_efs2_diag_unlink_rsp_type;

#endif //_BENQEFSDIAG_H_
