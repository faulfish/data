#ifndef _BENQ_MOBILEPHONE_API_IPHONEBOOK_H_
#define _BENQ_MOBILEPHONE_API_IPHONEBOOK_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

//Phonebook Type Definition
typedef enum PhonebookTypeTag
{
  ePhonebookSIM = 0,
  ePhonebookME,
  ePhonebookAll
}ePhonebookType; 

//phonebook retrun value 
typedef enum IPhonebookRetTag
{
  eSuccess = 0,
  eMemoryNotEnough,
  eIndexError,
  eInvalidData,
  eInternalError
}ePhonebookRetValue;

/***************************************************************************/
/* IPHONEBOOK Interface                                                    */
/***************************************************************************/

interface IPHONEBOOK
{
  virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut) = 0;
  virtual bool STDMETHODCALLTYPE IsPhonebookTypeSupport(ePhonebookType eType) = 0;
  virtual bool STDMETHODCALLTYPE GetPhonebookRange(ePhonebookType eType, UINT &nMinIdx, UINT &nMaxIdx, UINT &nMaxCount) = 0;
  virtual bool STDMETHODCALLTYPE ListPhonebookIndex(ePhonebookType eType, std::vector<UINT> *vIdxList) = 0;
  virtual int  STDMETHODCALLTYPE ReadVCardbyIndex(ePhonebookType eType, UINT nIdx, char *cVCardbuf, int nBufMaxLen, int &nVcardLen) = 0;
  virtual int  STDMETHODCALLTYPE WriteVCardbyIndex(ePhonebookType eType, UINT& nIdx, int& RetError, char *cVCardbuf, int nBufLen) = 0;
  virtual int  STDMETHODCALLTYPE RemoveVCardbyIndex(ePhonebookType eType, UINT nIdx) = 0;
};

#endif