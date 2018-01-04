/*****************************************************************************/
/** @file:		General.h
  This is the header file that contains the General class.

  $Author:   Eddy Guo  $
  $Version:		1.0 $
  $Date:   Feb 19 2008 16:12:28  $
  */
/*****************************************************************************/
#ifndef __GENERAL_H__
#define __GENERAL_H__

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

#include "RequestFunctionBase.h"
class CDCINTERFACE_CLASS_DLLEXT CGeneral : public RequestFunctionBase
{
public:
	CGeneral();
	~CGeneral();

	// set/reset port
	HRESULT SetPort(CString cstrPort);
	HRESULT ResetPort();

	// phone information
	HRESULT GetPhoneModel(CString* pstrModel);
	HRESULT GetIMEI(CString* pstrIMEI);
	HRESULT GetSoftwareVer(CString* pstrSoftwareVer);
	HRESULT GetManufacturer(CString* pstrManufacturer);
	HRESULT GetMobileInfo(CString* pstrModel, CString* pstrIMEI, CString* pstrSoftwareVer);
	HRESULT GetFDNStatus(bool& bIsFDN);
    HRESULT GetConnectivity(bool& bIsOnline);
	// check password
	HRESULT CheckPassword(CString strPassword, BOOL *pbPasswordValid);

    // enable echo or not
    HRESULT SetCommandEchoOff(bool bIsEcho = false);
};

#endif