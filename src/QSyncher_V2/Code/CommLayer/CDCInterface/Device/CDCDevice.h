#ifndef _BENQ_MOBILEPHONE_CDC_DEVICE_H_
#define _BENQ_MOBILEPHONE_CDC_DEVICE_H_

#pragma once
#include "../MobileInterface/Interface_Include/ICommMobile.h"
#include "../CDCInterface/General.h"
#include "../../Components/Configuration/QConfigure/QMobileConfigure.h"
#include "../MobileInterface/Interface_Include/IPIMConfig.h"

using namespace QSYNC_LIB;

class CCDCDevice : public ICommDevice
{
public:
	CCDCDevice(void);
public:
	~CCDCDevice(void);

	//device interfaces
private:
	CGeneral*				m_pIGeneral;

	IEFS*						m_pIEFS;
	ISMS*						m_pISMS;
	ISYNCML*				m_pISyncML;
	IPHONEBOOK*		m_pIPhonebook;
	ICALENDAR*		m_pICalendar;
	IPIMCONFIG*		m_pIPIMConfigure;

	CQMobileConfigure*		m_Configure;
	TSTRING				m_strModel;
	TSTRING				m_strIMEI;
	TSTRING				m_strSoftwareVer;
	TSTRING				m_strConnectPort;
	bool							m_bIsFDN;
    bool							m_bIsOnline;
	
	CString                 m_strDevName;
	CommConnectionState     m_eConnectState;	
	CommTransDevice         m_eDeviceType;

public:
	virtual BOOL  DeviceGetInterface( CommInterface eInterface, LPVOID *ppInterface ); 
	virtual BOOL  DetachAllInterface ();
	virtual int   ConnectDevice( LPCTSTR strModel,  LPCTSTR port);
	virtual int   DisconnectDevice( LPCTSTR strModel,  LPCTSTR port);
	virtual BOOL  IsInterfaceAttached( CommInterface eInterface );
	virtual TSTRING	GetModelName() const {return m_strModel;}
	virtual TSTRING	GetIMEI() const {return m_strIMEI;}
	virtual TSTRING	GetSoftwareVer() const {return m_strSoftwareVer;}
	virtual TSTRING GetPort() const {return m_strConnectPort;}
	virtual CommTransDevice GetTransDevice() const {return m_eDeviceType;}
	virtual BOOL GetFDNStatus(bool& bIsFDN){bIsFDN = m_bIsFDN;return TRUE;}
    virtual BOOL GetConnectivity(bool& bIsOnline);
private:
	BOOL	CreateInterface(CommInterface eInterface, const TSTRING& strIClass);
	TSTRING GetInterfaceName(CommInterface eInterface);
	BOOL	AttachInterface(CommInterface eInterface);

};

#endif
