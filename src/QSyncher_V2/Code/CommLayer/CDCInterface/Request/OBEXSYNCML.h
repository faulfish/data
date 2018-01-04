#ifndef __OBEXSYNCML_H__
#define __OBEXSYNCML_H__

#pragma once

#include "..\MobileInterface\Interface_Include\ISyncML.h"
#include "..\..\CommonUtil\NotifyBase.h"

#ifndef CDCINTERFACE_CLASS_DLLEXT
#ifdef _CDCINTERFACEDLL
#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif
#endif

#ifdef _CDCINTERFACEDLL

#include "OBEXSYNCMLRequest.h"

class COBEXSYNCML : public ISYNCML, public QSYNC_LIB::INotify
{
public:
	COBEXSYNCML(void);
public:
	virtual ~COBEXSYNCML(void);
public:
	////////////////////////////////////////////////////////////////
	//ISyncML
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
	virtual int  STDMETHODCALLTYPE Get(BYTE **ppFileData, DWORD& nSize);
	virtual int  STDMETHODCALLTYPE Put(BYTE *pFileData, DWORD nSize);
	virtual int  STDMETHODCALLTYPE Open();
	virtual int  STDMETHODCALLTYPE Close();
public:
	////////////////////////////////////////////////////////////////
	//INotify
	virtual void Event(const TSTRING& strEvent,long nParam);
protected:
	int Execute(CRequestPtr& ptRequest);
	FILETIME GetFileTimeFromOleTime(COleDateTime oleDateTime);
private:
	CEvent			m_ReceiveEvent;
	int				m_nTimeOut;
};

#endif

CDCINTERFACE_CLASS_DLLEXT ISYNCML* CreateOBEXISYNCML();
CDCINTERFACE_CLASS_DLLEXT void DeleteOBEXISYNCML();

#endif//__OBEXSYNCML_H__