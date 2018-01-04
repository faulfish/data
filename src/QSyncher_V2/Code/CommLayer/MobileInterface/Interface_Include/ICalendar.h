#ifndef  _BENQ_MOBILEPHONE_API_ICALENDAR_H_
#define _BENQ_MOBILEPHONE_API_ICALENDAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

/***************************************************************************/
/* ICALENDAR Interface                                                    */
/***************************************************************************/

interface ICALENDAR
{
  virtual bool STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut) = 0;
  virtual bool STDMETHODCALLTYPE GetRange(UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount) = 0;
  virtual bool STDMETHODCALLTYPE ListIndex(std::vector<UINT> *vIdxList) = 0;
  virtual bool STDMETHODCALLTYPE ReadByIndex(UINT nIdx, char *szbuf, int nBufMaxLen, int &nVcardLen) = 0;
  virtual bool STDMETHODCALLTYPE WriteByIndex(UINT& nIdx, char *szbuf, int nBufLen) = 0;
  virtual bool STDMETHODCALLTYPE RemoveByIndex(UINT nIdx) = 0;
};

#endif //_BENQ_MOBILEPHONE_API_ICALENDAR_H_