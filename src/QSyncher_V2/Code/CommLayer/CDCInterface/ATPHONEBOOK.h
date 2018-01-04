#pragma once

#ifndef _ATPHONEBOOK_H_
#define _ATPHONEBOOK_H_

#include "..\MobileInterface\Interface_Include\IPhonebook.h"
#include "Request.h"
#include "..\..\CommonUtil\NotifyBase.h"

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

CDCINTERFACE_CLASS_DLLEXT IPHONEBOOK* CreateATPHONEBOOK();
CDCINTERFACE_CLASS_DLLEXT void DeleteATPHONEBOOK();

#ifdef _CDCINTERFACEDLL
#include "RequestFunctionBase.h"

class CATPHONEBOOK : public IPHONEBOOK , public RequestFunctionBase
{
public:
	CATPHONEBOOK(void):RequestFunctionBase(/*INFINITE*/10000){}
	virtual ~CATPHONEBOOK(void){}

	//interface IPHONEBOOK
	void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut) { m_nTimeOut = nTimeOut; };
	bool STDMETHODCALLTYPE IsPhonebookTypeSupport(ePhonebookType eType) { return true; };
    bool STDMETHODCALLTYPE GetPhonebookRange(ePhonebookType eType, UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount);
	bool STDMETHODCALLTYPE ListPhonebookIndex(ePhonebookType eType, std::vector<UINT> *vIdxList);
    int  STDMETHODCALLTYPE ReadVCardbyIndex(ePhonebookType eType, UINT nIdx, char *cVCardbuf, int nBufMaxLen, int &nVcardLen);
    int  STDMETHODCALLTYPE WriteVCardbyIndex(ePhonebookType eType, UINT& nIdx, int& nRetError, char *cVCardbuf, int nBufLen);
    int  STDMETHODCALLTYPE RemoveVCardbyIndex(ePhonebookType eType, UINT nIdx);
};

#endif //#ifdef _CDCINTERFACEDLL

#endif