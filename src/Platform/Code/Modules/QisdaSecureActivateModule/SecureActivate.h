#pragma once
#include "..\..\Lib\RequestUtility\InterfaceBase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\CommonUtil\RefObject.h"
#include "..\..\CommonUtil\ErrorCode.h"

#ifdef QISDA_SECURE_ACTIVATE_EXPORTS
#define SECURE_API extern "C" __declspec(dllexport)
#define SECURE_CLASS_API __declspec(dllexport)
#else
#define SECURE_API extern "C" __declspec(dllimport)
#define SECURE_CLASS_API __declspec(dllimport)
#endif

const int g_nDEFAULT_OTP_RELATED = 60000;

class SECURE_CLASS_API CSecureActivate: public CInterfaceBase
{	
public:	
	BOOL SecureActivate(BYTE iSlotId, BYTE iSIM_AppIdLen, BYTE* piSIM_AppId, DWORD *pStatus, int nTimeout = g_nDEFAULT_OTP_RELATED);	// Freeze OTP
	BOOL IsOTPFrozen(int& nLastError, int nTimeout = g_nDEFAULT_OTP_RELATED);

public:
	CSecureActivate(int nCOMPort);
	~CSecureActivate(void);
};


class SECURE_CLASS_API CCFCOTPRelated: public CSecureActivate, public CRefObject, public CLastError
{
public:
	CCFCOTPRelated(int nCOMPort);
	~CCFCOTPRelated();
public:
	bool FreezeOTP(int nTimeout = g_nDEFAULT_OTP_RELATED);
	bool IsOTPFrozen(int nTimeout = g_nDEFAULT_OTP_RELATED); // return false when OTP isn't frozen or command fail. (add error coad later)

};

typedef CRefObjectPtr<CCFCOTPRelated> CCFCOTPRelatedPtr;