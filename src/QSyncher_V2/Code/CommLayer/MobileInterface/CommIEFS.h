#ifndef _BENQ_MOBILEPHONE_COMM_IEFS_H_
#define _BENQ_MOBILEPHONE_COMM_IEFS_H_

#pragma once

#include "interface_include\ICommMobile.h"
#include "CommInterface.h"

class CCommIEFS :
	public IEFS, public CCommInterface
{
public:
	CCommIEFS(void);
public:
	virtual ~CCommIEFS(void);


public:
    //IEFS
	virtual BOOL STDMETHODCALLTYPE CanCancel();
	virtual BOOL STDMETHODCALLTYPE Cancel();
	virtual BOOL STDMETHODCALLTYPE QueryCapability(IEFSCapability eIEFSCapability);
    virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
    virtual int  STDMETHODCALLTYPE GetFileASync( int nOperationID, LPCTSTR strPath, int& nFileSize );
    virtual int  STDMETHODCALLTYPE GetFileData(  int nOperationID, BYTE **ppFileData, DWORD& nSize, BOOL &bLastData );   
    virtual int  STDMETHODCALLTYPE PutFileAsync( int nOperationID, LPCTSTR strPath,  int nFileSize );
    virtual int  STDMETHODCALLTYPE PutFileData(  int nOperationID, BYTE *pFileData, DWORD nSize, BOOL bLastData );
    virtual int  STDMETHODCALLTYPE DeleteFile( LPCTSTR strPath );
    virtual int  STDMETHODCALLTYPE GetFolderEntries( LPCTSTR strPath, vector<WIN32_FIND_DATA> *vEntryList );
	virtual int  STDMETHODCALLTYPE CreateFolder( LPCTSTR strPath );
    virtual int  STDMETHODCALLTYPE RemoveFolder( LPCTSTR strPath );
	virtual BOOL STDMETHODCALLTYPE Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath );
	
	virtual int  STDMETHODCALLTYPE RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
    virtual int  STDMETHODCALLTYPE UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
};

#endif