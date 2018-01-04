#pragma once
#include "..\..\lib\requestutility\interfacebase.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\RequestManager\Request\EFsRequest.h"

#ifdef QISDA_DOWNLOAD_MODULE_EXPORTS
#define QISDA_DOWNLOAD_API extern "C" __declspec(dllexport)
#define QISDA_DOWNLOAD_CLASS_API __declspec(dllexport)
#else
#define QISDA_DOWNLOAD_API extern "C" __declspec(dllimport)
#define QISDA_DOWNLOAD_CLASS_API __declspec(dllimport)
#endif

/******************************************************************************
						Definitions for EFS operations
*******************************************************************************/

class QISDA_DOWNLOAD_CLASS_API CDownload : public CInterfaceBase
{
public:
	CDownload(int nCOMPort);
	bool GetDLBootLoaderRevocation(int& nStatus,int& nVersion);
	bool GetDLPhoneCodeRevocation(int& nStatus,int& nVersion);
public:
	virtual ~CDownload(void);
};
