#ifndef FS_DIAG_ACCESS_H
#define FS_DIAG_ACCESS_H
/*==========================================================================

                   Access validation to directories and files

  Description: Types and function prototypes required to implement access
  validation to directories and files.

Copyright (c) 2003 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                            Edit History

  $Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/fs_diag_access.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $

when         who     what, where, why
----------   ---     --------------------------------------------------------
2003/07/16    gr     Created
===========================================================================*/

#include "comdef.h"
#include "customer.h"

#ifdef FEATURE_EFS2
  #include "fs_sys_types.h"
#else
  #include "fs_parms.h"
#endif

#ifdef FEATURE_DIAG_FS_ACCESS_VALIDATION

/* ------------------------------------------------------------------------
 * EFS Access Validation API
 *   Provides an interface for a client to register an accessor validator
 *   function.  When registered, DIAG will call the function for each of
 *   the file system operations given by fs_diag_op_type.  The callback
 *   returns a value indicating whether the operation can proceed.  If
 *   the call back indicates the operation is not ok, DIAG returns an
 *   ERR packet indicating this is a bad command - otherwise the operation
 *   proceeds as normal.
 * ------------------------------------------------------------------------*/

/* Number of validation clients allowed
 */
#define FS_DIAG_MAX_ACCESS_VALIDATOR_CLIENTS 2

/* Maximum length of a pathname. This varies depending on the filesystem
 * code used.
 */
#ifdef FEATURE_EFS2
  #define FS_DIAG_ACCESS_PATH_MAX FS_PATH_MAX
#else
  #define FS_DIAG_ACCESS_PATH_MAX FS_FILENAME_MAX_LENGTH_P
#endif

/* Enumeration of possible file operations
*/
typedef enum {
  DIAG_FS_CHECK_NONE,
  /* EFS1 operations and operations shared between EFS1 and EFS2
   */
  DIAG_FS_CHECK_MKDIR,       /*  1     */
  DIAG_FS_CHECK_DISP_DIRS,
  DIAG_FS_CHECK_DISP_FILES,
  DIAG_FS_CHECK_RMDIR,
  DIAG_FS_CHECK_GET_ATTRIB, /*   5     */
  DIAG_FS_CHECK_SET_ATTRIB,
  DIAG_FS_CHECK_READ,
  DIAG_FS_CHECK_WRITE,
  DIAG_FS_CHECK_DELETE,
  DIAG_FS_CHECK_REMOTE_LINK,
  DIAG_FS_CHECK_ITERATE,
  /* Operations specific to EFS2
   */
  DIAG_FS_CHECK_OPEN,
  DIAG_FS_CHECK_CLOSE,
  DIAG_FS_CHECK_SYMLINK,
  DIAG_FS_CHECK_READLINK,     /*  15     */
  DIAG_FS_CHECK_UNLINK,
  DIAG_FS_CHECK_OPENDIR,
  DIAG_FS_CHECK_READDIR,
  DIAG_FS_CHECK_CLOSEDIR,
  DIAG_FS_CHECK_RENAME,       /*  20     */
  DIAG_FS_CHECK_STAT,
  DIAG_FS_CHECK_LSTAT,
  DIAG_FS_CHECK_FSTAT,
  DIAG_FS_CHECK_CHMOD,
  DIAG_FS_CHECK_STATFS,       /*  25     */
  DIAG_FS_CHECK_ACCESS,
  DIAG_FS_CHECK_MAX_OP    = 0x7fffffff
} diag_fs_op_type;

/* Macros for access validation based on the path, file handle or directory
 * handle.
 */
extern boolean fs_diag_access_check (const char*, diag_fs_op_type);
#define FS_DIAG_VALIDATE_ACCESS(xx_op, xx_path)\
if (fs_diag_access_check (xx_path, DIAG_FS_CHECK_##xx_op) == FALSE) {\
  return (diagpkt_err_rsp(DIAG_BAD_CMD_F, req, pkt_len));}
#define FS_DIAG_VALIDATE_PATH_ACCESS(xx_op, xx_path)\
if (fs_diag_access_check (xx_path, DIAG_FS_CHECK_##xx_op) == FALSE) {\
  return (diagpkt_err_rsp(DIAG_BAD_CMD_F, req_ptr, pkt_len));}
#define FS_DIAG_VALIDATE_FD_ACCESS(xx_op, xx_fd)\
  FS_DIAG_VALIDATE_PATH_ACCESS (xx_op, fs_diag_get_fdpath (xx_fd))
#define FS_DIAG_VALIDATE_DIRP_ACCESS(xx_op, xx_dirp)\
  FS_DIAG_VALIDATE_PATH_ACCESS (xx_op, fs_diag_get_dirpath (xx_dirp))

/* Callback prototype to registering application
 *   Callback returns TRUE  if the operation is ALLOWED
 *                    FALSE otherwise
 */
typedef boolean (*diag_fs_access_check_fptr_type) (
  char  *file_name_ptr, /* Name of file being validated            */
  uint32 op_type        /* Operation type given by diag_fs_op_type */
);

/*===========================================================================

FUNCTION DIAG_FS_REGISTER_ACCESS_CHECK

DESCRIPTION
   Registers a function to be called for all EFS operations via DIAG for
   the list of directories provided by dir_list_ptr.  If dir_list_ptr
   is NULL, then the callback should be called for ALL access to ANY
   file in the file system accessed via DIAG.

   If dir_list_ptr is not NULL, then dir_list_ptr points to an array
   of dir_list_size NULL terminated strings naming the directories for
   which the access callback should be made.

   DIAG will point to the directory list if it is non NULL, clients must
   not alter this memory after registering with DIAG.
============================================================================*/
void diag_fs_register_access_check (
  const char                    **dir_list_ptr,
  uint32                          dir_list_size,
  diag_fs_access_check_fptr_type  access_cb_fptr);

#else
  #define FS_DIAG_VALIDATE_ACCESS(xx_op, xx_path)
  #define FS_DIAG_VALIDATE_PATH_ACCESS(xx_op, xx_path)
  #define FS_DIAG_VALIDATE_FD_ACCESS(xx_op, xx_fd)
  #define FS_DIAG_VALIDATE_DIRP_ACCESS(xx_op, xx_dirp)
#endif /* FEATURE_DIAG_FS_ACCESS_VALIDATION */

#endif /* FS_DIAG_ACCESS_H */

