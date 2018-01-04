// QisdaMiscellaneousModule.h : main header file for the QisdaMiscellaneousModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "../../RequestManager/Request/QualcommRequest.h"
#include "..\..\Lib\RequestUtility\InterfaceBase.h"
#include "..\..\CommonUtil\RefObject.h"
#include "..\..\RequestManager\Request\ODMRequest.h"

#ifdef QISDA_MISC_MODULE_EXPORTS
#define QMM_API __declspec(dllexport)
#else
#define QMM_API __declspec(dllimport)
#endif
// CQisdaMiscellaneousModuleApp
// See QisdaMiscellaneousModule.cpp for the implementation of this class
//

class QMM_API CQisdaMiscellaneousModule : public CInterfaceBase, public CRefObject
{
public:
	CQisdaMiscellaneousModule(int nCOMPort);
	bool GetSWHWVersion(LPCSTR sSW, LPCSTR sHW);
	//for CFC
	bool GetHDVersion(int& nPhoneCodeVersion, int& nBootloaderVersion, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT); // same as isOTPFrozen command
	bool GetReleaseDate(char* szDate12, char* szTime9, int nTimeOut=g_nDEFAULT_CMD_TIME_OUT);

public:
	virtual ~CQisdaMiscellaneousModule(void);
};
typedef CRefObjectPtr<CQisdaMiscellaneousModule> CQisdaMiscellaneousModulePtr;
