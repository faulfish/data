#ifndef _BENQ_MOBILEPHONE_COMM_DEVICE_H_
#define _BENQ_MOBILEPHONE_COMM_DEVICE_H_

#pragma once

#include "Interface_Include/ICommMobile.h"
#include <vector>

class CCommDevice : public ICommDevice
{
public:
	CCommDevice(void):m_eConnectState(eCommNoConnection),m_eDeviceType(eCommSerial){}
public:
	virtual ~CCommDevice(void){}

public:
	//Attribute
	CString                 m_strDevName;
	CommConnectionState     m_eConnectState;
	CString                 m_strConnectPort;
	CommTransDevice         m_eDeviceType;
	CString                 m_strModelname;
	CString                 m_strIMEI;
	CString                 m_strSoftwareVer;
};

#endif