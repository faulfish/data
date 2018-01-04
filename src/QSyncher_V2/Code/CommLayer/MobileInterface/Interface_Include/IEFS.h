
#ifndef _BENQ_MOBILEPHONE_API_IEFS_H_
#define _BENQ_MOBILEPHONE_API_IEFS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "..\..\..\CommonUtil\NotifyBase.h"
using namespace std;

/***************************************************************************/
/* IEFS Enum Definitions                                                   */
/***************************************************************************/

// IEFS Capability Definition
typedef enum IEFSCapability
{
	eCopyFileSupport,
	ePredefineFileSupport,   
	eAttributeSupport,
	eCapacitySupport
}IEFSCapability; 

// IEFS Return Code Definition
typedef enum IEFSRetCode
{
	eEFSRetFail = 0,
	eEFSRetSuccess = 1,
	eEFSRetSuccessContinue = 2,
	eEFSFileNotExist = 3,
	eEFSDeviceEmpty = 4
}IEFSRetCode; 

/***************************************************************************/
/* IEFS Interface                                                          */
/***************************************************************************/

class NotifyPara
{
public:
	NotifyPara(int nOp,int nSize):m_nSize(nSize),nOperationID(nOp){}
	~NotifyPara(){}
	int m_nSize;
	int nOperationID;
};

const LPCTSTR NOTIFY_GET_FILE_DATA = _T("GET");
const LPCTSTR NOTIFY_ABORT_GET_FILE_DATA = _T("ABORT_GET");

interface IEFS
{
	virtual BOOL STDMETHODCALLTYPE CanCancel() = 0;
	virtual BOOL STDMETHODCALLTYPE Cancel() = 0;
	virtual BOOL STDMETHODCALLTYPE QueryCapability(IEFSCapability eIEFSCapability) = 0;
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut) = 0;
	virtual int  STDMETHODCALLTYPE GetFileASync( int nOperationID, LPCTSTR strPath, int& nFileSize ) = 0;
	virtual int  STDMETHODCALLTYPE GetFileData(  int nOperationID, BYTE **ppFileData, DWORD& nSize, BOOL &bLastData ) = 0;   
	virtual int  STDMETHODCALLTYPE PutFileAsync( int nOperationID, LPCTSTR strPath,  int nFileSize ) = 0;
	virtual int  STDMETHODCALLTYPE PutFileData(  int nOperationID, BYTE *pFileData, DWORD nSize, BOOL bLastData ) = 0;
	virtual int  STDMETHODCALLTYPE DeleteFile( LPCTSTR strPath ) = 0;
	virtual int  STDMETHODCALLTYPE GetFolderEntries( LPCTSTR strPath, vector<WIN32_FIND_DATA> *vEntryList ) = 0;
	virtual int  STDMETHODCALLTYPE CreateFolder( LPCTSTR strPath ) = 0;
	virtual int  STDMETHODCALLTYPE RemoveFolder( LPCTSTR strPath ) = 0;
	virtual BOOL STDMETHODCALLTYPE Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath ) = 0;

	virtual int  STDMETHODCALLTYPE RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent) = 0;
	virtual int  STDMETHODCALLTYPE UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent) = 0;
};

#endif