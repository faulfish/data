#pragma once
#include "interfacebase.h"
#include "..\Lib\msm\nv_items.h"
#include "DiagCommand.h"

#ifdef DIAG_EXPORT_EXPORTS
#define DIAG_API extern "C" __declspec(dllexport)
#define DIAG_CLASS_API __declspec(dllexport)
#else
#define DIAG_API extern "C" __declspec(dllimport)
#define DIAG_CLASS_API __declspec(dllimport)
#endif

class DIAG_CLASS_API CNVItem :	public CInterfaceBase
{
public:
	CNVItem(int nCOMPort);
	virtual ~CNVItem(void);

	bool ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp);//	Read NV Item from the phone
	/*
	bool WriteNVItem();//	Write NV Item to the phone.
	bool ReadNVArray();//	Read NV Array from the phone
	bool WriteNVArray();//	Wrtie NV Array to the phone
	bool ReadIMEI();//	Read IMEI from the phone
	bool WriteIMEI();//	Write IMEI to the phone
	bool ReadFactoryInfo();//	Read factory info block
	bool WrtieFactoryInfo();//	Write factory info block
	bool BackupNV();//	Backup NV to NV partition
	bool RestoreNV();//	Restore NV from NV partition
	*/
};
