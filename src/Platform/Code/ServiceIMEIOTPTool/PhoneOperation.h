#if !defined(_WINDOWS_)
#include "windows.h"
#endif

namespace PHONEOP{

BOOL FreezeOTP(const UINT nComPort);

/* ******************************************************************
*    Get IMEI stored in the phone
*    [i] nComPort - the phone com port
*    [o] imei - the imei value setted in the phone, null-terminated string
*    [i] size - the size of imei buffer
*
*    return true if success; otherwise return false
****************************************************************** */
BOOL GetIMEI(const UINT nComPort, BYTE *imei, const size_t size);
BOOL SetIMEI(const UINT nComPort, const BYTE *imei);

/* ******************************************************************
*    Get FSN stored in the phone
*    [i] nComPort - the phone com port
*    [o] fsn - the imei value setted in the phone, null-terminated string
*    [i] size - the size of fsn buffer
*
*    return 0 if success; otherwise return 1
****************************************************************** */
BOOL GetFSN(const UINT nComPort, BYTE *fsn, const size_t size, const BYTE *SPC=NULL);
BOOL SetFSN(const UINT nComPort, const BYTE *fsn, const BYTE *SPC=NULL);

BOOL SetBTAddress(const UINT nComPort, BYTE *addr);
BOOL GetBTAddress(const UINT nComPort, BYTE *addr, const size_t size);

/* ******************************************************************
*    Unlock SPC
*    [i] nComPort - the phone com port
*    [i] SPC - six digits, service programming code
*
*    return true, if the spc is correct; otherwise, return false
****************************************************************** */
BOOL QuerySPC(const UINT nComPort, const BYTE *SPC);

BOOL SetSPC(const UINT nComPort, const BYTE *SPC);

BOOL RestartPhone(const UINT nComPort, const UINT nSleepTime);

/* ******************************************************************
*    Change to Online/FTM mode
*    It is more stable to read/set imei(fsn, and etc) in FTM mode 
*    than in Online mode.
****************************************************************** */
BOOL ChangeToOnlineMode(const UINT nComPort);

BOOL ChangeToFTMMode(const UINT nComPort);

BOOL BackNV(const UINT nComPort);

BOOL RestoreBRT(const UINT nComPort, LPCTSTR pBRTFilePath);

/* ******************************************************************
*    Write QFuse
*    Use MobileToolkit to write QFuse, there are 2 parameters. 
*    Chack HW version and Select parameters, Write QFuse, Check QFuse
****************************************************************** */

BOOL CheckHWVersion(long nPort, unsigned long reg[5]);

BOOL WriteQFuse(long nPort, long nChaig, unsigned long reg[5]);

BOOL CheckQFuse(long nPort, unsigned long reg[5]);

}	// namespace PHONEOP