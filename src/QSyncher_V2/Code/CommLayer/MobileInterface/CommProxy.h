#ifndef _BENQ_MOBILEPHONE_COMM_PROXY_H_
#define _BENQ_MOBILEPHONE_COMM_PROXY_H_

#pragma once

#include "EmptyDevice.h"
#include "CommDevice.h"

class CCommProxy
{
	typedef std::vector<ICommDevice*> CommDeviceList;
public:
	CCommProxy();
public:
	~CCommProxy();

private:
	CommDeviceList  m_DeviceList;
	BOOL            m_isDevAttached;
	ICommDevice*    m_MainDevice;   

public:
	LPVOID				ProxyGetInterface ( CommInterface eInterface );
	BOOL				AttachDevice ( LPCTSTR  strModel);
	BOOL				DetachAllDevice ();
	BOOL				GetAttachDevices ( CommDeviceList *pDeviceList );
	BOOL				GetMainDevice ( ICommDevice **pDevice );

private:
	void				RemoveAttachedDev();
	void				SetDeviceToEmpty();
	ICommDevice*		LoadDeviceFomDll(LPCTSTR strDll);
};

#endif