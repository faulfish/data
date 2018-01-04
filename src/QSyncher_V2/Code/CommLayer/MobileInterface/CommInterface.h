#ifndef _BENQ_MOBILEPHONE_COMM_INTERFACE_H_
#define _BENQ_MOBILEPHONE_COMM_INTERFACE_H_

#pragma once

#include "Interface_Include/ICommMobile.h"
#include "..\..\CommonUtil\LogPolicy.h"

class CCommInterface : public CObject
{
public:
	CCommInterface(void);
public:
	virtual ~CCommInterface(void);

protected:
	long          m_nTimeoutSec;
	CString		  m_strRegApp;
	CString		  m_strRegClient;

private:  
	LPVOID		  m_pPxyInterface;
	BOOL          m_bInterLock;
	CommInterface m_eInterface;

public:
	void    SetRegisterApp( LPCTSTR strApp ) { m_strRegApp = strApp; }
	void    GetRegisterApp( LPTSTR strApp ){ strApp = m_strRegApp.GetBuffer(); }
	void    SetRegisterClient( LPCTSTR strClient){ m_strRegClient = strClient; }
	void    GetRegisterClient( LPTSTR strClient){ strClient = m_strRegClient.GetBuffer(); }
	void    SetInterface( CommInterface eInterface, LPVOID pInterface)
	{ 
		m_eInterface = eInterface;
		m_pPxyInterface = pInterface; 
	}
	void    LockInterface( BOOL bLock) { m_bInterLock = bLock; }
	int     GetInterfaceEnum(){ return m_eInterface; } 
	BOOL    GetInterface( int &eInterface, LPVOID &pInterface ) 
	{ 
		TRACE_FILE_LINE_INFO();

		BOOL bRes = FALSE;
		if (!m_bInterLock && m_pPxyInterface)
		{
			eInterface = m_eInterface;
			pInterface = m_pPxyInterface;
			bRes  = TRUE;
		}
		return bRes;
	}
};

typedef CTypedPtrList<CObList, CCommInterface*> CommInterfaceList;

#endif