#ifndef _SIM_ENCORDER_API_H_
#define _SIM_ENCORDER_API_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/**
 * Program: SimEncorder
 *
 * \file
 *  SimEncorderApi.h
 * \brief
 *  Defines DLL interfaces provided by SimEncorder DLL library.
 * \version
 *  1.4
 *   \code
 *    2006-11-02 1.0 Sky Yang
 *     - Initial version.
 *
 *    2006-11-03 1.1 Sky Yang
 *     - Update ISimEnc_SetSimLockFile function parameters; using szSimFilePath to specify file path 
 *       directly.
 *
 *    2006-11-06 1.2 Sky Yang
 *     - Update T_SIM_UNLOCK_CODE structure; add tReserved structure member for furture usage.
 *     - Update ISimEnc_SetSimLockFile function remark section; add notice about 256 bytes limitation
 *       for full file path.
 *
 *    2006-11-10 1.3 Sky Yang
 *     - Add ISimEnc_EnableTextLog function to enable text log function for debugging.
 *     - Remove T_SINGLE_UNLOCK_CODE type definition.
 *     - Update T_SIM_UNLOCK_CODE structure member; replace T_SINGLE_UNLOCK_CODE with char* type.
 *     - Update ISimEnc_SetUnlockCodes function remark section; add notice about all unlock codes
 *       must equal to MAX_SIM_CODE_LENGTH.
 *     - Update ISimEnc_SetSimLockFile function remark section; right now the function supports full
 *       file path that more than 256 bytes.
 *
 *    2007-01-04 1.4 Sky Yang
 *     - Add ISimEnc_CheckSimLockData function to check if the specific SIM data is correct.
 *     - Update ISimEnc_SetFlashSerialNumber function:
 *       a. Update input FSN format: from 16 bytes binary data to 32 bytes hex string.
 *       b. Update parameters: from "unsigned char *puFsn" to "char *szFsn".
 *     - Update text log format.
 *
 *    2007-01-29 1.4.1 Karen Chi
 *     - Corrected a typo in servtables.c line 46
 *
 *    2008-07-03 1.4.2 Jae Liao
 *     - Add a new encoder algorithm for project Sagem H21
 *     - Add a new encoder algorithm for project AthensBT
 *     - Modify Log mechanism; to enable log or not is depending on the enable log flag and log file path information
 *       retrieved from registry first.
 *          [HKEY_CURRENT_USER\Software\Mobile Utility\QisdaSimEncoder]
 *              \EnableLog: 0/1 value
 *              \LogPath: the log file path
 *     - Modify object allocation mechanism: allocate a new encoder object no matter what the sim type id is
 *     - Add Retry Limit and Interval features for each Lock Type
 *
 *    2008-09-09 1.4.3 Jae Liao
 *     - Add a new encoder algorithm for project Lavernock
 *     - Add a limitation: the USim code(master code) can't not be the same with the others.
 *
 *   \endcode
 * \date
 *  Jan-2007
 * \author
 *  Sky Yang
 */

/*--------------------------------------------------------------------------*/
/* CONSTANT DECLARATION                                                     */
/*--------------------------------------------------------------------------*/

#define MAX_IMEI_LENGTH						15		///< Maximum length of IMEI.
#define MAX_FSN_LENGTH						16		///< Maximum length of FSN (Flash Serial Number).
#define MAX_SIM_CODE_LENGTH					8		///< Maximum length of SIM unlock codes.

/*--------------------------------------------------------------------------*/
/* TYPE DEFINITION                                                          */
/*--------------------------------------------------------------------------*/

/// Enum to define return value from SIM encorder DLL interfaces.
typedef enum
{
	SIM_RET_SUCCESS,								///< ' 0 = Success.
	SIM_RET_NONE_SUPP_FUNC,							///< ' 1 = The specific function is not supported for specifiec SIM type.
	SIM_RET_NONE_ENOUGH_MEM,						///< ' 2 = No enough memory.
	SIM_RET_INVALID_PARAM,							///< ' 3 = The specific parameter is not valid.
	SIM_RET_INVALID_OBJECT,							///< ' 4 = The specific object handle is not valid.
	SIM_RET_NO_SIMMODE,								///< ' 5 - The sim mode is not set yet
	SIM_RET_NO_IMEI,								///< ' 6 = IMEI is not set yet.
	SIM_RET_NO_FSN,									///< ' 7 = FSN is not set yet.
	SIM_RET_NO_SIM_FILE,							///< ' 8 = SIM file is not set yet.
	SIM_RET_NO_UNLOCK_CODE,							///< ' 9 = Unlock code is not set yet.
	SIM_RET_INVALID_USIM_CODE,						///< ' 10 - USim code is the same with a certain one code
	SIM_RET_FILE_PARSE_FAIL,						///< ' 11 = Parse SIM file fails.
	SIM_RET_BUF_NOT_ENOUGH,							///< ' 12 = The specific output buffer is too small.	
	SIM_RET_RESULT_FAIL,							///< ' 13 = The test result fails.
	SIM_RET_LOCNAME_TOOLONG							///< ' 14 - The loc file name is too long (for Lavernock)
} T_SIM_RET_VALUE;

/// Enum to define SIM type.
typedef enum
{	
	SIM_TYPE_SIRIUS_L55_NEW_CRYPT,					///< ' Sirius L55 solution; extended for new encryption algorithm.
	SIM_TYPE_ATHENS,
	SIM_TYPE_ATHENS_BT,							    ///< ' Athens BT solution
	SIM_TYPE_SAGEM_H21,								///< ' Sagem H21 solution; 
	SIM_TYPE_SIRIUS_L55,
	SIM_TYPE_LAVERNOCK,
	MAX_SIM_TYPE									//   ' Maximum SIM type (internal usage).
} T_SIM_TYPE;

/**
 * Structure to define unlock codes for SIM encorder. Notice that the all code length should be equal 
 * to MAX_SIM_CODE_LENGTH.
 */
typedef struct
{
	char *szNetworkCode;							///< ' Unlock code for Network Lock.
	char *szServiceProviderCode;					///< ' Unlock code for Service Provider Lock.
	char *szCorporateCode;							///< ' Unlock code for Corporate Lock.
	char *szNetworkSubsetCode;						///< ' Unlock code for Network Subset Lock.
	char *szSimCode;								///< ' Unlock code for SIM Lock.
	char *szUsimCode;								///< ' Unblock code for User SIM Lock.
	char *szReserved;								///< ' Reserved code for furture usage.
} T_SIM_UNLOCK_CODE;

/*--------------------------------------------------------------------------*/
/* MACRO DEFINITION                                                         */
/*--------------------------------------------------------------------------*/

#ifdef _SIMENCORDERDLL
	#ifdef __cplusplus
		#define	SIMENCORDER_API	extern "C" __declspec(dllexport)
	#else
		#define	SIMENCORDER_API	__declspec(dllexport)
	#endif
#else
	#ifdef __cplusplus
		#define	SIMENCORDER_API	extern "C" __declspec(dllimport)
	#else
		#define	SIMENCORDER_API	__declspec(dllimport)
	#endif
#endif

/*--------------------------------------------------------------------------*/
/* GLOBAL FUNCTION DECLARATION                                              */
/*--------------------------------------------------------------------------*/

/**
 * The \b ISimEnc_AllocSimEncObj function allocate the SimEncorder object. The function returns a handle 
 * that can be used to access the object.
 * 
 * \param
 *  nSimTypeId		[in] An integer used to identified the specific SIM type (e.g. SIM lock solution).
 *   \code
 *    SIM_TYPE_SIRIUS_L55			' Sirius L55 solution.
 *    SIM_TYPE_SIRIUS_L55_NEW_CRYPT	' Sirius L55 solution; extended for new encryption.
 *   \endcode
 * \param
 *  hSimEncHandle	[out] Reference to a variable that recrieve the handle of the SimEncorder object.
 *					\b ISimEnc_AllocSimEncObj sets this value to zero before doing any work or error 
 *					checking; and then set this value to nonzero if function resturns SIM_RET_SUCCESS.
 * 
 * \remark
 *  Use ::ISimEnc_FreeSimEncObj function to free the SimEncorder object returned by \b ISimEnc_AllocSimEncObj.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_ENOUGH_MEM  ' No enough memory to allocate SimEncorder object.
 *     SIM_RET_INVALID_PARAM    ' The specific parameter is not valid (e.g. nSimtypeId is invalid).
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_AllocSimEncObj(int nSimTypeId, void *&hSimEncHandle);
int Lib_ISimEnc_AllocSimEncObj(int nSimTypeId, void *&hSimEncHandle);

/**
 * The \b ISimEnc_FreeSimEncObj function free the allocated SimEncorder object.
 * 
 * \param
 *  hSimEncHandle	[in] Handle to an SimEncorder object.
 * 
 * \remark
 *  Use \b ISimEnc_FreeSimEncObj to close handles returned by calls to the ::ISimEnc_AllocSimEncObj function. 
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_INVALID_OBJECT   ' The specific object handle is not valid.
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_FreeSimEncObj(const void *hSimEncHandle);
int Lib_ISimEnc_FreeSimEncObj(const void *hSimEncHandle);

/**
 * The \b ISimEnc_SetImei function set the IMEI (e.g. one SimEncorder input).
 * 
 * \param
 *  hSimEncHandle	[in] Handle to an SimEncorder object to be set.
 * \param
 *  szImei			[in] Pointer to the buffer containing the IMEI to be set SimEncorder object.
 * \param
 *  nImeiLen		[in] Number of bytes of the buffer pointed by \b szImei.
 * 
 * \remark
 *  The \b nImeiLen must be equal to ::MAX_IMEI_LENGTH; otherwise the function fails.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_SUPP_FUNC   ' The specific function is not supported for specifiec SIM type.
 *     SIM_RET_INVALID_PARAM    ' The specific parameter is not valid (e.g. szImei is invalid or 
 *                                nImeiLen is not equal to MAX_IMEI_LENGTH).
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_SetImei(const void *hSimEncHandle, char *szImei, int nImeiLen);
int Lib_ISimEnc_SetImei(const void *hSimEncHandle, char *szImei, int nImeiLen);

/**
 * The \b ISimEnc_SetFlashSerialNumber function set the FSN (e.g. one SimEncorder input).
 * 
 * \param
 *  hSimEncHandle	[in] Handle to an SimEncorder object to be set.
 * \param
 *  szFsn			[in] Pointer to the buffer containing the FSN to be set SimEncorder object.
 * \param
 *  nFsnLen			[in] Number of bytes of the buffer pointed by \b szFsn.
 * 
 * \remark
 *  The specific FSN data is represented as HEX string; that is, the "12" hex string would be translated as 
 *  0x12 binary byte. Thus the \b nFsnLen must be equal to ::MAX_FSN_LENGTH*2; otherwise the function fails.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_SUPP_FUNC	' The specific function is not supported for specifiec SIM type.
 *     SIM_RET_INVALID_PARAM	' The specific parameter is not valid (e.g. pcFsn is invalid or
 *                                nFsnLen is not equal to MAX_FSN_LENGTH).
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_SetFlashSerialNumber(const void *hSimEncHandle, char *szFsn, int nFsnLen);
int Lib_ISimEnc_SetFlashSerialNumber(const void *hSimEncHandle, char *szFsn, int nFsnLen);

/**
 * The \b ISimEnc_SetSimLockFile function set the SIM lock file (e.g. one SimEncorder input).
 * 
 * \param
 *  hSimEncHandle	[in] Handle to an SimEncorder object to be set.
 * \param
 *  szSimFilePath	[in] Pointer to the null-terminated string that specifies the full file path of SIM 
 *                  lock file.
 *
 * \remark
 *  For SIM lock generation, \b szSimFilePath must specify the SIM lock file with full file path and must
 *  get the read access right for file parsing; otherwise the function fails.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_SUPP_FUNC   ' The specific function is not supported for specifiec SIM type.
 *     SIM_RET_INVALID_PARAM    ' The specific parameter is not valid (e.g. szSimFilePath is invalid
 *                                or no READ access right for the specific file path).
 *     SIM_RET_LOCNAME_TOOLONG  ' The loc file name is too long. The length must <= 63.
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_SetSimLockFile(const void *hSimEncHandle, char *szSimFilePath);
int Lib_ISimEnc_SetSimLockFile(const void *hSimEncHandle, char *szSimFilePath);

/**
 * The \b ISimEnc_SetUnlockCodes function set the unlock codes (e.g. one SimEncorder input).
 * 
 * \param
 *  hSimEncHandle	[in] Handle to an SimEncorder object to be set.
 * \param
 *  ptSimUnlockCode	[in] Pointer to the T_SIM_UNLOCK_CODE that specifies the unlock codes to be set 
 *                  SimEncorder object.
 *
 * \remark
 *  The \b T_SIM_UNLOCK_CODE is used to prepared all unlock codes for SimEncorder object and notice
 *  that all unlock code lenght must equal to MAX_SIM_CODE_LENGTH; otherwise function fails.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_SUPP_FUNC   ' The specific function is not supported for specifiec SIM type.
 *     SIM_RET_INVALID_USIM_CODE' The USim code can't be the same with the others.
 *     SIM_RET_INVALID_PARAM    ' The specific parameter is not valid (e.g. ptSimUnlockCode is
 *                                invalid or any one of the unlock code length is not equal to 
 *                                MAX_SIM_CODE_LENGTH).
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_SetUnlockCodes(const void *hSimEncHandle, T_SIM_UNLOCK_CODE *ptSimUnlockCode);
int Lib_ISimEnc_SetUnlockCodes(const void *hSimEncHandle, T_SIM_UNLOCK_CODE *ptSimUnlockCode);

/**
 * The \b ISimEnc_GenSimLockData function get the SIM lock data according to all SimEncorder inputs.
 * Thus all SimEncorder inputs must be already set before calling this function.
 * 
 * \param
 *  hSimEncHandle		[in] Handle to an SimEncorder object to be set.
 * \param
 *  pcSimLockData		[out] Pointer to the buffer that receives the SIM lock data created by SimEncorder.
 * \param
 *  pnSimLockDataLen	[in, out] Pointer to a variable containing number of bytes of the buffer pointed by
 *                      \b pcSimFileData and then receives number of bytes of the SIM lock data that filled
 *                      into the buffer.
 *
 * \remark
 *  The caller must prepare the buffer pointed by \b pcSimLockData and records the number of buffer size in \b
 *  pnSimLockDataLen before calling \b ISimEnc_GenSimLockData function. After calling \b ISimEnc_GenSimLockData
 *  function, the SIM lock data will be filled into buffer and the actual number of bytes of SIM lock data will
 *  be stored into the varaible pointed by pnSimLockDataLen.
 *
 * \remark
 *  The buffer size should be larger than SIM lock data; otherwise the function fails.
 *
 * \remark
 *  Before creating SIM lock data, function will store a temporary file in local disk. Thus function may
 *  fail if caller doesn't have enough access right for local disk.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_SUPP_FUNC   ' The specific function is not supported for specifiec SIM type.
 *     SIM_RET_INVALID_PARAM    ' The specific parameter is not valid (e.g. pcSimLockData is invalid
 *                                or pnSimLockDataLen is invalid).
 *     SIM_RET_NO_IMEI          ' The IMEI is not set yet.
 *     SIM_RET_NO_FSN           ' The FSN is not set yet.
 *     SIM_RET_NO_SIM_FILE      ' The SIM lock file is not set yet.
 *     SIM_RET_NO_UNLOCK_CODE   ' The unlock code is not set yet.
 *     SIM_RET_BUF_NOT_ENOUGH   ' The buffer size is not large enough to fill the SIM lock data.
 *     SIM_RET_FILE_PARSE_FAIL  ' Can't parse the SIM lock file.
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_GenSimLockData(const void *hSimEncHandle, unsigned char *pcSimLockData, int *pnSimLockDataLen);
int Lib_ISimEnc_GenSimLockData(const void *hSimEncHandle, unsigned char *pcSimLockData, int *pnSimLockDataLen);

/**
 * The \b ISimEnc_CheckSimLockData function checks the specific SIM lock data according to all
 * SimEncorder inputs. Thus all SimEncorder inputs must be already set before calling this function.
 * 
 * \param
 *  hSimEncHandle		[in] Handle to an SimEncorder object to be set.
 * \param
 *  pcSimLockData		[in] Pointer to the buffer that stores the SIM lock data needs to be checked.
 * \param
 *  nSimLockDataLen		[in] Number of bytes of the buffer pointed by \b pcSimLockData.
 *
 * \remark
 *  Before calling \b ISimEnc_CheckSimLockData function, all SimEncorder inputs must be set. The SIM data
 *  length must be fit according to solution type. For example, the ::SIM_TYPE_SIRIUS_L55 solution must
 *  be 512 bytes; otherwise the function fails.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_SUPP_FUNC   ' The specific function is not supported for specifiec SIM type.
 *     SIM_RET_INVALID_PARAM    ' The specific parameter is not valid (e.g. pcSimLockData is invalid
 *                                or pnSimLockDataLen is invalid).
 *     SIM_RET_NO_IMEI          ' The IMEI is not set yet.
 *     SIM_RET_NO_FSN           ' The FSN is not set yet.
 *     SIM_RET_NO_SIM_FILE      ' The SIM lock file is not set yet.
 *     SIM_RET_NO_UNLOCK_CODE   ' The unlock code is not set yet.
 *     SIM_RET_BUF_NOT_ENOUGH   ' The buffer size is not large enough to fill the SIM lock data.
 *     SIM_RET_FILE_PARSE_FAIL  ' Can't parse the SIM lock file.
 *     SIM_RET_RESULT_FAIL      ' The specific SIM data is invalid.
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_CheckSimLockData(const void *hSimEncHandle, unsigned char *pcSimLockData, int nSimLockDataLen);
int Lib_ISimEnc_CheckSimLockData(const void *hSimEncHandle, unsigned char *pcSimLockData, int nSimLockDataLen);

/**
 * The \b ISimEnc_EnableTextLog function enable the text log and output as the text log file for debugging.
 * 
 * \param
 *  hSimEncHandle		[in] Handle to an SimEncorder object to be set.
 * \param
 *  bEnableTextLog		[in] Specifies whether the SimEncorder object output the text log file.
 * \param
 *  szTextLogFilePath	[in] Pointer to the null-terminated string that specifies the full file path of text 
 *                      log file if the \b bEnableTextLog parameter is \b true. If the \b bEnableTextlog 
 *                      parameter is \b false, this parameter is ignored.
 *
 * \remark
 *  For text log function, \b szTextLogFilePath must specify the text log file with full file path if the
 *  \b bEnableTextLog parameter is \b true. If the file doesn't exist, it would be created; and if the file
 *  exists, its contents are destroyed.
 *
 * \return
 *   If the function success, the return value is zero; otherwise the return value is nonzero listed below.
 *    \code
 *     SIM_RET_NONE_SUPP_FUNC   ' The specific function is not supported for specifiec SIM type.
 *     SIM_RET_INVALID_PARAM    ' The specific parameter is not valid (e.g. szTextLogFilePath is 
 *                                invalid).
 *    \endcode
 */

SIMENCORDER_API int __stdcall ISimEnc_EnableTextLog(const void *hSimEncHandle, bool bEnableTextLog, char *szTextLogFilePath);
int Lib_ISimEnc_EnableTextLog(const void *hSimEncHandle, bool bEnableTextLog, char *szTextLogFilePath);

/* ******************************************************************
*	Generate encrypted simlock data (only for Athens)
*	
*	return 0, if success. 
*   return SIM_RET_INVALID_PARAM, if a certain input parameter is invalid
* 
****************************************************************** */
SIMENCORDER_API int __stdcall ISimEnc_GenSimFile(
	const BYTE *szImei, 
	const BYTE *szFsn,
	const BYTE *szMasterCode, 
	const BYTE *szSubsidyCode, 
	const BYTE *szLockPath,
	BYTE *simEncryptedDataBuf, 
	const int simEncryptedDataBufLen);

int Lib_ISimEnc_GenSimFile(
	const BYTE *szImei, 
	const BYTE *szFsn,
	const BYTE *szMasterCode, 
	const BYTE *szSubsidyCode, 
	const BYTE *szLockPath,
	BYTE *simEncryptedDataBuf, 
	const int simEncryptedDataBufLen);


#endif // _SIM_ENCORDER_API_H_
