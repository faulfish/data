/*****************************************************************************/
/** @file:		Message.h
  This is the header file that contains the Message class.

  $Author:   Eddy Guo  $
  $Version:		1.0 $
  $Date:   Jan 23 2008 17:19:22  $
  */
/*****************************************************************************/
#ifndef __MESSAGE_H__
#define __MESSAGE_H__
#pragma once
#include "..\MobileInterface\Interface_Include\ISMS.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "Request.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "MessageRequest.h"
#include "RequestManager.h"

#ifdef _CDCINTERFACEDLL
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllexport)
#else
	#define		CDCINTERFACE_CLASS_DLLEXT	__declspec(dllimport)
#endif

#ifdef _CDCINTERFACEDLL 
/*! @class		CMessage
 *  @brief		interface to message functions with AT command
 *  @author		Eddy Guo
 *  @date		2007/02/08
 */
class CMessage : public ISMS , public QSYNC_LIB::INotify
{
    // This enum is used for GetResult
    // After g_RequestManager.EnqueueRequest, call GetCommandResult
    // put this enum tag into GetCommandResult as parameter

public:
	CMessage();
	~CMessage();

	// get service center address
	HRESULT STDMETHODCALLTYPE GetServiceCenterAddress(CString* pcstrSCA);

	// set PDU Mode
	HRESULT STDMETHODCALLTYPE SetPDUMode();

	// set preferred storage (for standard)
	HRESULT STDMETHODCALLTYPE SetMessageStorage(enumStorage eMsgStorage, MemUsed& used);

	// query message space
	HRESULT STDMETHODCALLTYPE QueryMessageSpace(enumStorage eMsgStorage, UINT* puiUsed, UINT* puiTotal);

	// list message
    HRESULT STDMETHODCALLTYPE ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>* pVecSM = 0, std::vector<CShortMessage>* pVecShortMessage = 0);

	// send message
	HRESULT STDMETHODCALLTYPE SendMessage(BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID);

	// send message in storage
	HRESULT STDMETHODCALLTYPE SendMessageInStorage(UINT uiMsgID, CString cstrAddress);

	// write message
	HRESULT STDMETHODCALLTYPE WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID);
    HRESULT STDMETHODCALLTYPE WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID, char* szTime);

	// read message
	HRESULT STDMETHODCALLTYPE ReadMessage(UINT uiMsgID, BYTE* pMsgData, UINT* puiMsgDataSize);
    HRESULT STDMETHODCALLTYPE ReadMessage(UINT uiMsgID, CShortMessage& csm);

	// delete message
	HRESULT STDMETHODCALLTYPE DeleteMessage(UINT uiMsgID);

	// get sub folders by folder ID
   HRESULT STDMETHODCALLTYPE GetFolderEntries(BOOL bGetRootFolder, UINT nParentFolderID, vector<CMessageFolderInfo> *vEntryList);
   
   // create or rename message sub folders by folder ID
   HRESULT STDMETHODCALLTYPE WriteFolderEntries(BOOL bCreate, BOOL bCreateRootFolder, UINT nParentFolderID, UINT nRenameFolderID, CString strDiaplayName, UINT &nNewID);
   
   // delete sub folders by folder ID
   HRESULT STDMETHODCALLTYPE DeleteFolderEntry( UINT nFolderID );

   // update message state
   HRESULT STDMETHODCALLTYPE UpdateMessageState(UINT uiMsgID, UINT nState);

   // INotify, this class only receives a message "Received".
   // Reset the event to notify this command is received.
   virtual void Event(const TSTRING& strEvent,long nParam)
   {
	   if(strEvent == _T("Received"))
		   m_ReceiveEvent.SetEvent();
   }

public:
    enumAtCmdSet QueryAtCommandSet();           ///< 詢問手機目前是何種AT Command
    void SetAtCommandSet(enumAtCmdSet set);     ///< 設定為何種AT Command

private:
    template<class SpType> HRESULT EnqueueRequest(HRESULT hr, SpType& spRequest);
    void ReadEachMessageByID(CString* pcstrMsgIDList, std::vector<CShortMessage>* pVecShortMessage);

protected:
	int Execute(CRequestPtr& ptRequest,int nTimeOut);

private:
	CEvent m_ReceiveEvent;
	int m_nTimeOut;
    CQMobileConfigure* m_pConfigure;
    enumAtCmdSet m_eCurAtCmdSet;
};
#endif

CDCINTERFACE_CLASS_DLLEXT ISMS* CreateISMS();
CDCINTERFACE_CLASS_DLLEXT void DeleteISMS();

#endif
