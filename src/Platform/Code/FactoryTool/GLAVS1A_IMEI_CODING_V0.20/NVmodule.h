// NVmodule.h: interface for the CNVmodule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NVMODULE_H__85D5E3C3_29E3_41A1_AFAC_89D09E170B76__INCLUDED_)
#define AFX_NVMODULE_H__85D5E3C3_29E3_41A1_AFAC_89D09E170B76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CNVmodule  
{
public:
//	int Phone_BackupNV(int port,int timeout);
	CNVmodule();
	virtual ~CNVmodule();
	HMODULE hDll;

};

#endif // !defined(AFX_NVMODULE_H__85D5E3C3_29E3_41A1_AFAC_89D09E170B76__INCLUDED_)
