#ifndef _BENQ_MOBILEPHONE_API_IIRMC_H_
#define _BENQ_MOBILEPHONE_API_IIRMC_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IIRMCS Return Code Definition
typedef enum IIRMCRetCodeTag
{
  eIRMCSRetFail = 0,
  eIRMCRetSuccess = 1,
}IIRMCRetCode; 

/***************************************************************************/
/* IIRMC Interface                                                         */
/***************************************************************************/

interface IIRMC
{
  virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut) = 0;
  virtual int  STDMETHODCALLTYPE Open(int nIrMCLevel) = 0;
  virtual int  STDMETHODCALLTYPE Get(CString strPath, BYTE *pData, DWORD nDataMaxSize, DWORD &nDataSize) = 0;
  virtual int  STDMETHODCALLTYPE Put(CString strPath, BYTE *pFileData, DWORD nDataSize, CString &strLUID, int &nCounter) = 0;
  virtual int  STDMETHODCALLTYPE Close(int nIrMCLevel) = 0;
};

#endif