#ifndef  _BENQ_MOBILEPHONE_COMM_ICALENDAR_H_
#define _BENQ_MOBILEPHONE_COMM_ICALENDAR_H_

#pragma once

#include "interface_include\ICommMobile.h"
#include "CommInterface.h"

class CCommICalendar 	: public ICALENDAR, public CCommInterface
{
public:
	CCommICalendar(void);
public:
	virtual ~CCommICalendar(void);

public:
    //ICALENDAR
  virtual bool STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
  virtual bool STDMETHODCALLTYPE GetRange(UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount);
  virtual bool STDMETHODCALLTYPE ListIndex(std::vector<UINT> *vIdxList);
  virtual bool STDMETHODCALLTYPE ReadByIndex(UINT nIdx, char *szbuf, int nBufMaxLen, int &nVcardLen);
  virtual bool STDMETHODCALLTYPE WriteByIndex(UINT& nIdx, char *szbuf, int nBufLen);
  virtual bool STDMETHODCALLTYPE RemoveByIndex(UINT nIdx);
};

#endif //_BENQ_MOBILEPHONE_COMM_ICALENDAR_H_