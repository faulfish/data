#ifndef _BENQ_MOBILEPHONE_COMM_IPIMCONFIG_H_
#define _BENQ_MOBILEPHONE_COMM_IPIMCONFIG_H_

#pragma once

#include "interface_include\ICommMobile.h"
#include "CommInterface.h"

#include "MobileInterface.h"
extern CApp theApp;

class CCommIPIMConfigure :
	public IPIMCONFIG, public CCommInterface
{
public:
	CCommIPIMConfigure(void){}
public:
	virtual ~CCommIPIMConfigure(void){}

public:
	//interface IPIMCONFIG
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut)
	{
		LockMobileInterface lock;

		int		  eInterface = -1;
		LPVOID     pInterface;
		IPIMCONFIG *pIPIMCONFIG = NULL;

		// interface is not locked
		if (GetInterface( eInterface, pInterface ) && eInterface == eIPIMConfigure) 
		{
			pIPIMCONFIG = (IPIMCONFIG*)pInterface;
			pIPIMCONFIG->SetOperationTimeOut(nTimeOut);
		}
	}
	virtual int  STDMETHODCALLTYPE ListPIMGroup(ePIMType eType, vector<CPIMGroupInfo> *vGroupList)
	{
		LockMobileInterface lock;

		int		  eInterface = -1;
		LPVOID     pInterface;
		IPIMCONFIG *pIPIMCONFIG = NULL;

		// interface is not locked
		if (GetInterface( eInterface, pInterface ) && eInterface == eIPIMConfigure) 
		{
			pIPIMCONFIG = (IPIMCONFIG*)pInterface;
			return pIPIMCONFIG->ListPIMGroup(eType,vGroupList);
		}
		else
			return false;
	}
	virtual int  STDMETHODCALLTYPE SetPIMGroup(ePIMType eType, int nGroupID, CPIMGroupInfo &setGroup)
	{
		LockMobileInterface lock;

		int		  eInterface = -1;
		LPVOID     pInterface;
		IPIMCONFIG *pIPIMCONFIG = NULL;

		// interface is not locked
		if (GetInterface( eInterface, pInterface ) && eInterface == eIPIMConfigure) 
		{
			pIPIMCONFIG = (IPIMCONFIG*)pInterface;
			return pIPIMCONFIG->SetPIMGroup(eType,nGroupID,setGroup);
		}
		else
			return false;
	}
	virtual int  STDMETHODCALLTYPE CreatePIMGroup(CPIMGroupInfo &newGroup, int &nNewID)
	{
		LockMobileInterface lock;

		int		  eInterface = -1;
		LPVOID     pInterface;
		IPIMCONFIG *pIPIMCONFIG = NULL;

		// interface is not locked
		if (GetInterface( eInterface, pInterface ) && eInterface == eIPIMConfigure) 
		{
			pIPIMCONFIG = (IPIMCONFIG*)pInterface;
			return pIPIMCONFIG->CreatePIMGroup(newGroup,nNewID);
		}
		else
			return false;
	}
	virtual int  STDMETHODCALLTYPE DeletePIMGroup(int nDeleteID)
	{
		LockMobileInterface lock;

		int		  eInterface = -1;
		LPVOID     pInterface;
		IPIMCONFIG *pIPIMCONFIG = NULL;

		// interface is not locked
		if (GetInterface( eInterface, pInterface ) && eInterface == eIPIMConfigure) 
		{
			pIPIMCONFIG = (IPIMCONFIG*)pInterface;
			return pIPIMCONFIG->DeletePIMGroup(nDeleteID);
		}
		else
			return false;
	}

	virtual int		STDMETHODCALLTYPE GetProperty(const TSTRING& strGroup,const TSTRING& strName,TSTRING& strValue)
	{
		TRACE_FILE_LINE_INFO();

		LockMobileInterface lock;

		int		  eInterface = -1;
		LPVOID     pInterface = NULL;
		IPIMCONFIG *pIPIMCONFIG = NULL;

		int nRes = false;
		// interface is not locked
		LogPolicy::Log(_T("Enter GetInterface();"));
		if (GetInterface( eInterface, pInterface ) && eInterface == eIPIMConfigure) 
		{
			pIPIMCONFIG = (IPIMCONFIG*)pInterface;
			if(pIPIMCONFIG)
				nRes = pIPIMCONFIG->GetProperty(strGroup,strName,strValue);
		}
		LogPolicy::Log(_T("Leave GetInterface();"));
		return nRes;
	}

	virtual int		STDMETHODCALLTYPE GetIntegerProperty(const TSTRING& strGroup,const TSTRING& strName,int& nValue)
	{
		LockMobileInterface lock;

		int		  eInterface = -1;
		LPVOID     pInterface;
		IPIMCONFIG *pIPIMCONFIG = NULL;

		// interface is not locked
		if (GetInterface( eInterface, pInterface ) && eInterface == eIPIMConfigure) 
		{
			pIPIMCONFIG = (IPIMCONFIG*)pInterface;
			return pIPIMCONFIG->GetIntegerProperty(strGroup,strName,nValue);
		}
		else
			return false;
	}
};

#endif //_BENQ_MOBILEPHONE_COMM_IPIMCONFIG_H_