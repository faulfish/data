#pragma once

#ifndef _ATCALENDAR_H_
#define _ATCALENDAR_H_

#include "..\MobileInterface\Interface_Include\ICalendar.h"
#include "Request.h"
#include "..\..\CommonUtil\NotifyBase.h"

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

CDCINTERFACE_CLASS_DLLEXT ICALENDAR* CreateATCALENDAR();
CDCINTERFACE_CLASS_DLLEXT void DeleteATCALENDAR();

#ifdef _CDCINTERFACEDLL
#include "RequestFunctionBase.h"

class CATCALENDAR : public ICALENDAR , public RequestFunctionBase
{
public:
	CATCALENDAR(void):RequestFunctionBase(/*INFINITE*/10000){}
	virtual ~CATCALENDAR(void){}

	//interface ICALENDAR
  virtual bool STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
  virtual bool STDMETHODCALLTYPE GetRange(UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount);
  virtual bool STDMETHODCALLTYPE ListIndex(std::vector<UINT> *vIdxList);
  virtual bool STDMETHODCALLTYPE ReadByIndex(UINT nIdx, char *szbuf, int nBufMaxLen, int &nVcardLen);
  virtual bool STDMETHODCALLTYPE WriteByIndex(UINT& nIdx, char *szbuf, int nBufLen);
  virtual bool STDMETHODCALLTYPE RemoveByIndex(UINT nIdx);
};

#endif //#ifdef _CDCINTERFACEDLL

#endif //_ATCALENDAR_H_