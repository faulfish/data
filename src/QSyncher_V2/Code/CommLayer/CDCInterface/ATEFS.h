/*****************************************************************************/
/** @file:		ATEFS.h
  This is the header file that contains the ATEFS class.

  $Author:   Alan Tu  $
  $Version:		1.0 $
  $Date:   Dec 17 2007 20:02:30  $
  */
/*****************************************************************************/
#ifndef __ATEFS_H__
#define __ATEFS_H__

#include "..\MobileInterface\Interface_Include\IEFS.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\CommonUtil\Buffer.h"
#include "Request.h"

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

/*! @class		CATEFS
 *  @brief		interface to file system functions with AT command
 *  @author		Eddy Guo
 *  @date		2007/01/30
 */
class CDCINTERFACE_CLASS_DLLEXT CATEFS : public IEFS, 
	                                     public QSYNC_LIB::INotify,
										 public QSYNC_LIB::CNotifyBase
{
public:
	CATEFS();
	virtual ~CATEFS();

	virtual BOOL STDMETHODCALLTYPE CanCancel(){return FALSE;}
	virtual BOOL STDMETHODCALLTYPE Cancel(){return FALSE;}
	virtual BOOL STDMETHODCALLTYPE QueryCapability(IEFSCapability eIEFSCapability);
	virtual void STDMETHODCALLTYPE SetOperationTimeOut(int nTimeOut);
	virtual int  STDMETHODCALLTYPE GetFileASync(int nOperationID, LPCTSTR strPath, int& nFileSize);
	virtual int  STDMETHODCALLTYPE GetFileData( int nOperationID, BYTE** ppFileData, DWORD& nSize, BOOL& bLastData);
	virtual int  STDMETHODCALLTYPE PutFileAsync(int nOperationID, LPCTSTR strPath,  int nFileSize);
	virtual int  STDMETHODCALLTYPE PutFileData( int nOperationID, BYTE* pFileData, DWORD nSize, BOOL bLastData);
	virtual int  STDMETHODCALLTYPE DeleteFile(LPCTSTR strPath);
	virtual int  STDMETHODCALLTYPE GetFolderEntries(LPCTSTR strPath, vector<WIN32_FIND_DATA>* vEntryList);
	virtual int  STDMETHODCALLTYPE CreateFolder(LPCTSTR strPath);
	virtual int  STDMETHODCALLTYPE RemoveFolder(LPCTSTR strPath);
	virtual BOOL STDMETHODCALLTYPE Rename( LPCTSTR strOldFullPath,LPCTSTR strNewFullPath );
	virtual int  STDMETHODCALLTYPE RegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
	virtual int  STDMETHODCALLTYPE UnRegisterNotify(QSYNC_LIB::INotify* pNotify, LPCTSTR strEvent);
    
public:
	void Event(const TSTRING& strEvent,long nParam); 
	int Execute(CRequestPtr& ptRequest,bool bWithResponse = true,bool bWithDataMode = false);

private:
	void ParseItem(const CString& strList,CStringArray& Tokens);
    BOOL ParseEntryList(CString& strList,WIN32_FIND_DATA& fdata);
	template <class funcClass> int SimpleFunc(LPCTSTR strPath,int nTimeOut = INFINITE);
	template <class funcClass> int SimpleFunc(LPCTSTR strPath1,LPCTSTR strPath2,int nTimeOut = INFINITE);
	
private:
	CEvent			 m_ReceiveEvent;
	CEvent			 m_DataModeEvent;

	CString          m_strFileName;

	int				 m_nTimeOut;
	DWORD			 m_dwFileSize;
	DWORD            m_dwTotalBytesRead;
    DWORD            m_dwTotalBytesSent;
	int              m_nOperationID;

	int								m_nHadReceiverFileSize;
	int								m_nFileSize;
	CCriticalSection				m_Critsection;
	std::vector<CBufferPtr>			m_vectBuffer;
};

CDCINTERFACE_CLASS_DLLEXT IEFS* CreateATIEFS();
CDCINTERFACE_CLASS_DLLEXT void DeleteATIEFS();

#endif