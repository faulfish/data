#ifndef _BENQ_MOBILEPHONE_COMM_IPHONEBOOK_H_
#define _BENQ_MOBILEPHONE_COMM_IPHONEBOOK_H_

#pragma once

#include "interface_include\ICommMobile.h"
#include "CommInterface.h"

class CCommIPhonebook :
	public IPHONEBOOK, public CCommInterface
{
public:
	CCommIPhonebook(void);
public:
	virtual ~CCommIPhonebook(void);


public:
    //IPHONEBOOK
	void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
    bool STDMETHODCALLTYPE IsPhonebookTypeSupport(ePhonebookType eType);
    bool STDMETHODCALLTYPE GetPhonebookRange(ePhonebookType eType, UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount);
    bool STDMETHODCALLTYPE ListPhonebookIndex(ePhonebookType eType, std::vector<UINT> *vIdxList);
    int  STDMETHODCALLTYPE ReadVCardbyIndex(ePhonebookType eType, UINT nIdx, char *cVCardbuf, int nBufMaxLen, int &nVcardLen);
    int  STDMETHODCALLTYPE WriteVCardbyIndex(ePhonebookType eType, UINT& nIdx,int& nRetError, char *cVCardbuf, int nBufLen);
    int  STDMETHODCALLTYPE RemoveVCardbyIndex(ePhonebookType eType, UINT nIdx);
};

#endif