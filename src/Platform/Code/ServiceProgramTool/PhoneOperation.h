#if !defined(_WINDOWS_)
#include "windows.h"
#endif

#include "simencorderapi.h"
#include "ServiceProgramTool.h"

namespace PHONEOP{

bool FreezeOTP(long nComPort);

/* ******************************************************************
*    Get IMEI stored in the phone
*    [i] nComPort - the phone com port
*    [o] imei - the imei value setted in the phone, null-terminated string
*    [i] size - the size of imei buffer
*
*    return true if success; otherwise return false
****************************************************************** */
bool GetIMEI(long nComPort, BYTE *imei, const size_t size);
bool SetIMEI(long nComPort, const BYTE *imei);

/* ******************************************************************
*    Get FSN stored in the phone
*    [i] nComPort - the phone com port
*    [o] fsn - the imei value setted in the phone, null-terminated string
*    [i] size - the size of fsn buffer
*
*    return 0 if success; otherwise return 1
****************************************************************** */
bool GetFSN(long nComPort, BYTE *fsn, const size_t size, const BYTE *SPC=NULL);
bool SetFSN(long nComPort, const BYTE *fsn, const BYTE *SPC=NULL);

bool GetBTAddress(long nComPort, BYTE *addr, const size_t size);

/* ******************************************************************
*    Generate Encrypted Sim Data
*    [i] iSimTypeID - the sim encoder algorithm
*    [i] szImei - IMEI
*    [i] szFsn - Flash Serial Number
*    [i] szMasterCode - lock code
*    [i] szSubsidyCode - lock code
*    [i] szLockPath - sim lock file path, an absolute file path
*    [o] simEncryptedDataBuf - the output data, encrypted sim data
*    [o] simEncryptedDataSize - the size of the encrypted sim data
*
*    return 0 if success; otherwise see error code
****************************************************************** */
int GenSimFile(int iSimTypeID, char *szImei, char *szFsn, char *szMasterCode, char *szSubsidyCode,
				char *szLockPath, BYTE **simEncryptedData, int *simEncryptedDataSize);

/* ******************************************************************
*    Write the encoded sim data to the phone
*    [i] nComPort - the phone com port
*    [i] simdata - the encoded sim data
*    [i] simDataSize - the size of the encoded data
*
*    return 0 if success; otherwise return 1
****************************************************************** */
bool WriteSimData(long nComPort, BYTE *simData, const size_t simDataSize, DWORD *cmdStatus, DWORD *status);

bool WriteSimLockFileName(long nComPort, char *pFileName);

/* ******************************************************************
*    Check if the encrypted sim data is programmed into the mobile
*    phone.
****************************************************************** */
bool CheckSimDataIsProgrammed(long nComPort);

/* ******************************************************************
*    Unlock SPC
*    [i] nComPort - the phone com port
*    [i] SPC - six digits, service programming code
*
*    return true, if the spc is correct; otherwise, return false
****************************************************************** */
bool QuerySPC(long nComPort, const BYTE *SPC);

bool SetSPC(long nComPort, const BYTE *SPC);

/* ******************************************************************
*    Query Lock Status
*    [i] nComPort - the phone com port
*    [i-o] nw, sp, ns, cp, sim, master - lock status
****************************************************************** */
bool QueryLockStatus(long nComPort, bool *nwStatus=NULL, bool *spStatus=NULL, bool *nsStatus=NULL, 
					 bool *cpStatus=NULL, bool *simStatus=NULL, bool *masterStatus=NULL);

/* ******************************************************************
*    Unlock Sim Data
*    [i] nComPort - the phone com port
*    [i] unlockType - the options to unlock include nw, sp, ns, cp, sim, master
*		GSDIDIAG_PERSO_NW        = 0x00000000,	// Network Personalization
*		GSDIDIAG_PERSO_NS        = 0x00000001,	// Network Subset Personalization
*		GSDIDIAG_PERSO_SP        = 0x00000002,	// Service Provider Personalization
*		GSDIDIAG_PERSO_CP        = 0x00000003,	// Corporate Personalization
*		GSDIDIAG_PERSO_SIM       = 0x00000004,	// SIM Personalization
*		GSDIDIAG_PERSO_PROP1	 = 0x00000010	// Antitheft Personalization
*
*    [i] szCode - unlock code
*    [o] status - 0 if unlock successfully; otherwise, return non-zero value
****************************************************************** */
bool UnLockSimData(long nComPort, int unlockType, char *szCode, DWORD *status);

bool UnLockPhoneMasterCode(long nComPort, char *szCode, DWORD *status);

void RestartPhone(long nComPort);

/* ******************************************************************
*    Enable/Disable Qualcomn event log
****************************************************************** */
bool EnableQualcommEventLog(long nComPort, bool bEnable);

/* ******************************************************************
*    Change to Online/FTM mode
*    It is more stable to read/set imei(fsn, and etc) in FTM mode 
*    than in Online mode.
****************************************************************** */
bool ChangeToOnlineMode(long nComPort);

bool ChangeToFTMMode(long nComPort);

MOBILEMODEL GetCurrentMobileModel(long nComPort);

}	// namespace PHONEOP