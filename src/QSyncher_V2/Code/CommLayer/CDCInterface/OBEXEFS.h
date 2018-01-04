#ifndef __OBEXEFS_H__
#define __OBEXEFS_H__

#pragma once

#include "..\MobileInterface\Interface_Include\IEFS.h"

#ifndef CDCINTERFACE_CLASS_DLLEXT
	#ifdef _CDCINTERFACEDLL
		#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
	#else
		#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
	#endif
#endif

#ifdef _CDCINTERFACEDLL

#include "..\..\CommonUtil\NotifyBase.h"
#include "OBEXRequest.h"
class COBEXEFS : public IEFS, public QSYNC_LIB::INotify , public QSYNC_LIB::CNotifyBase
{
public:
	COBEXEFS(void);
public:
	~COBEXEFS(void);
public:
	////////////////////////////////////////////////////////////////
	//IEFS	
	virtual BOOL STDMETHODCALLTYPE CanCancel();
	virtual BOOL STDMETHODCALLTYPE Cancel();
	virtual BOOL STDMETHODCALLTYPE QueryCapability(IEFSCapability eIEFSCapability);
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
	virtual int  STDMETHODCALLTYPE GetFileASync( int nOperationID, LPCTSTR strPath, int& nFileSize );
	virtual int  STDMETHODCALLTYPE GetFileData(  int nOperationID, BYTE **ppFileData, DWORD& nSize, BOOL &bLastData );
	virtual int  STDMETHODCALLTYPE PutFileAsync( int nOperationID, LPCTSTR strPath,  int nFileSize );
	virtual int  STDMETHODCALLTYPE PutFileData(  int nOperationID, BYTE *pFileData, DWORD nSize, BOOL bLastData);
	virtual int  STDMETHODCALLTYPE DeleteFile( LPCTSTR strPath );
	virtual int  STDMETHODCALLTYPE GetFolderEntries( LPCTSTR strPath, vector<WIN32_FIND_DATA> *vEntryList );
	virtual int  STDMETHODCALLTYPE CreateFolder( LPCTSTR strPath );
	virtual BOOL STDMETHODCALLTYPE Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath );
	virtual int  STDMETHODCALLTYPE RemoveFolder( LPCTSTR strPath );
	virtual int  STDMETHODCALLTYPE RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
    virtual int  STDMETHODCALLTYPE UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
public:
	////////////////////////////////////////////////////////////////
	//INotify
	virtual void Event(const TSTRING& strEvent,long nParam);
protected:
	int Execute(CRequestPtr& ptRequest,int nTimeOut = 0);
	FILETIME GetFileTimeFromOleTime(COleDateTime oleDateTime);
	static DWORD WINAPI GetFileThread(LPVOID pRequestManager);
	bool SetPath(LPCTSTR strPath);
private:
	CEvent			m_ReceiveEvent;
	int				m_nTimeOut;
	CBufferPtr		m_ptReceiver;
	HANDLE			m_hThreadHandle;
	std::vector<CBufferPtr> m_vectBuffer;
	CCriticalSection m_Critsection;
	CEvent			m_AbortEvent;
	bool			m_bFinishGetFile;
	int				m_nOperationID;
	bool			m_bFirstFlag;
	TSTRING			m_strPath;
	int				m_nFileSize;
	bool			m_bCanCancel;
	TSTRING			m_strCurrentPath;
};

#endif

CDCINTERFACE_CLASS_DLLEXT IEFS* CreateOBEXIEFS();
CDCINTERFACE_CLASS_DLLEXT void DeleteOBEXIEFS();

#endif//__OBEXEFS_H__