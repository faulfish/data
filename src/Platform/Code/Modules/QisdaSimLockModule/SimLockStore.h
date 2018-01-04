#pragma once
#include "..\..\Lib\RequestUtility\InterfaceBase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\CommonUtil\ErrorCode.h"

#ifdef QISDA_SIM_MODULE_EXPORTS
#define CSIM_API extern "C" __declspec(dllexport)
#define CSIM_CLASS_API __declspec(dllexport)
#else
#define CSIM_API extern "C" __declspec(dllimport)
#define CSIM_CLASS_API __declspec(dllimport)
#endif


const int g_nDEFAULT_SIMLOCK_Related = 60000;

/* ******************************************************************
*    [i] nComPort - the Com Port#
*    [i] nSIM_SecureDataLen - the sim lock data size
*    [i] piSIM_SecureData - the encrypted sim lock data
*    [i] pCmdStatus - the command status returned from the phone
*    [i] pStatus - the status after writing returned from the phone
****************************************************************** */
CSIM_API int WriteSimLockData(UINT nComPort, DWORD nSIM_SecureDataLen, BYTE* piSIM_SecureData, DWORD *pCmdStatus, DWORD *pStatus, int nTimeout=g_nDEFAULT_SIMLOCK_Related);

class CSIM_CLASS_API CSimLockStore: public CInterfaceBase
{
public:
	/* *****************************************************************
	*  [o] pCmdStatus - 0, GSDIDIAG_SUCCESS     
    *                   1, GSDIDIAG_BAD_PARAM
    *                   2, GSDIDIAG_ACCESS_DENIED	// check if spc is unlocked or not
    *                   9, GSDIDIAG_NOT_SUPPORTED
	***************************************************************** */
	virtual BOOL StoreSimData(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE* piSIM_AppId, DWORD iSIM_SecureDataLen, 
		BYTE* piSIM_SecureData, DWORD *pCmdStatus, DWORD *pStatus, int nTimeout = g_nDEFAULT_SIMLOCK_Related); 

private:
	BOOL StoreSimData_512(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE* piSIM_AppId, DWORD iSIM_SecureDataLen, 
		BYTE* piSIM_SecureData, DWORD *pCmdStatus, DWORD *pStatus, int nTimeout = g_nDEFAULT_SIMLOCK_Related);

	BOOL StoreSimData_576(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE* piSIM_AppId, DWORD iSIM_SecureDataLen, 
		BYTE* piSIM_SecureData, DWORD *pCmdStatus, DWORD *pStatus, int nTimeout = g_nDEFAULT_SIMLOCK_Related);

public:
	CSimLockStore(int nCOMPort);
	virtual ~CSimLockStore(void);
};

class CSIM_CLASS_API CSimLockStatus: public CInterfaceBase
{
public:
	bool QueryLockStatus(bool *nwStatus=NULL, bool *spStatus=NULL, bool *nsStatus=NULL, 
				bool *cpStatus=NULL, bool *simStatus=NULL, bool *masterStatus=NULL);
	bool CheckSIMLock(std::string& strSIMLockName, int nTimeout = g_nDEFAULT_SIMLOCK_Related);

public:
	CSimLockStatus(int nCOMPort);
	virtual ~CSimLockStatus(void);
};

class CSIM_CLASS_API CSimUnLockStatus: public CInterfaceBase
{
public:
	/* *****************************************************************
	* Unlock Type
	*   GSDIDIAG_PERSO_NW        = 0x00000000,	// Network Personalization
	*	GSDIDIAG_PERSO_NS        = 0x00000001,	// Network Subset Personalization
	*	GSDIDIAG_PERSO_SP        = 0x00000002,	// Service Provider Personalization
	*	GSDIDIAG_PERSO_CP        = 0x00000003,	// Corporate Personalization
	*	GSDIDIAG_PERSO_SIM       = 0x00000004,	// SIM Personalization
	*	GSDIDIAG_PERSO_PROP1	 = 0x00000010	// Antitheft Personalization
	*
	* sCode - a null-terminated string
	* pStatus - 0, if unlock successfully
	***************************************************************** */
	bool UnlockStatus(int unlockType, char *sCode, DWORD *pStatus, int nTimeout = g_nDEFAULT_SIMLOCK_Related);

	bool UnlockMasterCode(char *sCode, DWORD *pStatus, int nTimeout = g_nDEFAULT_SIMLOCK_Related);

public:
	CSimUnLockStatus(int nCOMPort);
	virtual ~CSimUnLockStatus(void);
};

/* ******************************************************************
*    Check if the encrypted sim data is programmed into the mobile
*    phone.
****************************************************************** */
CSIM_API bool _stdcall CheckSimDataIsProgrammed(long nComPort);




class CSIM_CLASS_API CCFCSimLockRelated :public CSimLockStatus, public CRefObject, public CLastError
{
public:
	CCFCSimLockRelated(int nComPort);
	virtual ~CCFCSimLockRelated();
public:
	bool UnlockNW(char* szLock8, bool& bUnlockOK, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
	bool UnlockNS(char* szLock8, bool& bUnlockOK, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
	bool UnlockSP(char* szLock8, bool& bUnlockOK, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
	bool UnlockCP(char* szLock8, bool& bUnlockOK, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
	bool UnlockSIM(char* szLock8, bool& bUnlockOK, int nTimeout = g_nDEFAULT_SIMLOCK_Related);

	bool UnlockSSCP(char* szLock8, bool& bUnlockOK, int nTimeout = g_nDEFAULT_SIMLOCK_Related); //unlock phone code by using master code
	
	bool IsSSCPLocked(long& nStatus/*0=unlocked, 1=locked*/, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
	bool IsNWSCPLocked(long& nStatus/*0=unlocked, 1=locked*/, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
	bool IsNWSCPLocked(bool& bNWUnlocked, // Network Personalization
					   bool& bNSUnlocked, // Network Subset Personalization
					   bool& bSPUnlocked, // Service Provider Personalization
					   bool& bCPUnlocked, // Corporate Personalization
					   bool& bSIMUnlocked,// SIM Personalization
					   int nTimeOut=g_nDEFAULT_SIMLOCK_Related);
	bool QuerySimDataStatus(bool& bExist, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
	
	bool WriteSimData(BYTE *pSimDataBuf, int nSimDataBufLen, int nTimeout = g_nDEFAULT_SIMLOCK_Related); //[Lavernock,Melos]nSimDataBufLen=576
	bool GetSimDataVersion(std::string& strVersion, int nTimeout = g_nDEFAULT_SIMLOCK_Related);
private:
	bool UnlockNWSCP(BYTE nType, char *szCode, bool& bUnlockOK, int nTimeout);
};

typedef CRefObjectPtr<CCFCSimLockRelated> CCFCSimLockRelatedPtr;

/************************************************************************/
/*  For SPU                                                                                   */
/************************************************************************/
CSIM_API bool IsSSCPLocked_ForSPU(char* szPort, char* szStatus/*return unlocked or locked*/, char* szTimeOut);

CSIM_API bool IsNWSCPLocked_ForSPU(
								   char* szPort,
								   char* szNWStatus/*return unlocked or locked*/, 
								   char* szNSStatus/*return unlocked or locked*/, 
								   char* szSPStatus/*return unlocked or locked*/, 
								   char* szCPStatus/*return unlocked or locked*/, 
								   char* szSIMStatus/*return unlocked or locked*/, 
								   char* szTimeOut);

CSIM_API bool UnlockNW_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut);

CSIM_API bool UnlockNS_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut);

CSIM_API bool UnlockSP_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut);

CSIM_API bool UnlockCP_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut);

CSIM_API bool UnlockSIM_ForSPU(char* szPort, char* szLockCode, char* szUnlockOK, char* szTimeOut);
