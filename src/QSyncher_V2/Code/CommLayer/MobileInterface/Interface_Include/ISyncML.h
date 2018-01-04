
#ifndef _BENQ_MOBILEPHONE_API_ISYNCML_H_
#define _BENQ_MOBILEPHONE_API_ISYNCML_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IEFS Return Code Definition
typedef enum ISYNCMLRetCode
{
  eSYNCMLSRetFail = 0,
  eSYNCMLRetSuccess = 1,
  eSYNCMLRetSuccessContinue = 2
}ISYNCMLRetCode; 

/***************************************************************************/
/* ISYNCML Interface                                                          */
/***************************************************************************/

interface ISYNCML
{
  virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut) = 0;
  virtual int  STDMETHODCALLTYPE Get(BYTE **ppFileData, DWORD& nSize) = 0;
  virtual int  STDMETHODCALLTYPE Put(BYTE *pFileData, DWORD nSize) = 0;
  virtual int  STDMETHODCALLTYPE Open() = 0;
  virtual int  STDMETHODCALLTYPE Close() = 0;
};

#endif