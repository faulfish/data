#ifndef _BENQ_MOBILEPHONE_COMM_ISYNCML_H_
#define _BENQ_MOBILEPHONE_COMM_ISYNCML_H_

#pragma once

#include "interface_include\ICommMobile.h"
#include "CommInterface.h"

class CCommISyncML :
	public ISYNCML, public CCommInterface
{
public:
	CCommISyncML(void);
public:
	virtual ~CCommISyncML(void);


public:
    //ISyncML
	void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
    int	 STDMETHODCALLTYPE Get(BYTE **ppFileData, DWORD& nSize);
    int	 STDMETHODCALLTYPE Put(BYTE *pFileData, DWORD nSize);
	int	 STDMETHODCALLTYPE Open();
	int	 STDMETHODCALLTYPE Close();
};

#endif