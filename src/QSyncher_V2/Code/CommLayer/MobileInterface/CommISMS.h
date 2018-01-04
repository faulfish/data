#ifndef _BENQ_MOBILEPHONE_COMM_ISMS_H_
#define _BENQ_MOBILEPHONE_COMM_ISMS_H_

#pragma once

#include "interface_include\ICommMobile.h"
#include "CommInterface.h"

class CCommISMS : public ISMS, public CCommInterface {
public:
    CCommISMS(void);
public:
    virtual ~CCommISMS(void);

public:
    //ISMS
    // get service center address
    HRESULT STDMETHODCALLTYPE GetServiceCenterAddress(CString* pcstrSCA);
    // set PDU Mode
    HRESULT STDMETHODCALLTYPE SetPDUMode();
    // set preferred storage (for standard)
    HRESULT STDMETHODCALLTYPE SetMessageStorage(enumStorage eMsgStorage, MemUsed& used);
    // query message space
    HRESULT STDMETHODCALLTYPE QueryMessageSpace(enumStorage eMsgStorage, UINT* puiUsed, UINT* puiTotal);
    // list message
    HRESULT STDMETHODCALLTYPE ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>* vecSM = 0, std::vector<CShortMessage>* vecShortMessage = 0);
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
};

#endif