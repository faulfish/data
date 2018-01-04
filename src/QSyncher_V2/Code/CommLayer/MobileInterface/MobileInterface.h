// MobileInterface.h : main header file for the MobileInterface DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"							// main symbols
#include "Interface_Include/ICommMobile.h"	
#include <vector>
#include "CommProxy.h"
#include "CommAdapter.h"
#include <map>

class CCommAdapter;
class CCommProxy;

class LockMobileInterface
{
public:
	LockMobileInterface():m_CritSection(TRUE,TRUE,_T("MobileInterface"))
	{
		::WaitForSingleObject(m_CritSection,INFINITE);
		m_CritSection.ResetEvent();
	}
	~LockMobileInterface()
	{
		m_CritSection.SetEvent();
	}
private:
	CEvent m_CritSection;
};

class CMobileInterface
{
public:
	CMobileInterface()
		:m_CommAdapter(new CCommAdapter)
		,m_CommProxy(new CCommProxy)
		,m_ConnectState(eCommNoConnection)
		,m_ConnectDevType(eCommEmptyDev)
	{
		m_CommAdapter->SetProxy( m_CommProxy );
	}
	~CMobileInterface()
	{
		if(m_CommAdapter)
		{
			delete m_CommAdapter;
			m_CommAdapter = NULL;
		}
		if(m_CommProxy)
		{
			delete m_CommProxy;
			m_CommProxy = NULL;
		}
	}

//Members
public:
	CCommAdapter*				m_CommAdapter;
    CCommProxy*					m_CommProxy;
	CString								m_registerApp;
	CommDetectMode			m_detectMode;

//Connect Device Information
public:
	CommConnectionState	m_ConnectState;
	CommTransDevice			m_ConnectDevType;
    CString								m_ConnectDevModel;
	CString								m_ConnectDevIMEI;
    CString								m_ConnectDevSoftwareVer;
	CString								m_ConnectPort;
    CString								m_ConnectDevDisplayName;	
	bool								m_bIsFDNMode;
    bool								m_bIsOnline;
};

class CApp : public CWinApp
{
public:
	CApp();
	int AddMobileInterface(CMobileInterface*);
	CMobileInterface* GetMobileInterface(int nID = 0);
	CMobileInterface* GetMobileInterfaceByIndex(int nIndex = 0);
	int GetMobileInterfaceCount();
	void RemoveMobileInterface(int nID);

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int  ExitInstance();	
	DECLARE_MESSAGE_MAP()

protected:
	std::map<int,CMobileInterface*> m_MobileInterfaces;
	int													m_nIndex;
};

