#ifndef _BENQ_MOBILEPHONE_API_ISMS_H_
#define _BENQ_MOBILEPHONE_API_ISMS_H_

#pragma once
#include "../../Components/SMSServer/MessageStructure.h"
#include <vector>

using namespace std;

interface ISMS
{
    // get service center address
    virtual HRESULT STDMETHODCALLTYPE GetServiceCenterAddress(CString* pcstrSCA)=0;
    // set PDU Mode
    virtual HRESULT STDMETHODCALLTYPE SetPDUMode()=0;
    // set preferred storage (for standard)
    virtual HRESULT STDMETHODCALLTYPE SetMessageStorage(enumStorage eMsgStorage, MemUsed& used)=0;
    // query message space
    virtual HRESULT STDMETHODCALLTYPE QueryMessageSpace(enumStorage eMsgStorage, UINT* puiUsed, UINT* puiTotal)=0;
    // list message
    virtual HRESULT STDMETHODCALLTYPE ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>* pVecSM = 0, std::vector<CShortMessage>* pVecShortMessage = 0)=0;
    //virtual HRESULT STDMETHODCALLTYPE ListMessage(enumMsgLoc eMsgLoc, CString* pcstrMsgIDList, std::vector<SM>& vecSM) = 0;
    // send messaage
    virtual HRESULT STDMETHODCALLTYPE SendMessage(BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID)=0;
    // send message in storage
    virtual HRESULT STDMETHODCALLTYPE SendMessageInStorage(UINT uiMsgID, CString cstrAddress)=0;
    // write message
    virtual HRESULT STDMETHODCALLTYPE WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID)=0;
    virtual HRESULT STDMETHODCALLTYPE WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID, char* szTime)=0;
    // read message
    virtual HRESULT STDMETHODCALLTYPE ReadMessage(UINT uiMsgID, BYTE* pMsgData, UINT* puiMsgDataSize)=0;
    virtual HRESULT STDMETHODCALLTYPE ReadMessage(UINT uiMsgID, CShortMessage& csm)=0;
    // delete message
    virtual HRESULT STDMETHODCALLTYPE DeleteMessage(UINT uiMsgID)=0;
    // get message sub folders by folder ID
    virtual HRESULT STDMETHODCALLTYPE GetFolderEntries(BOOL bGetRootFolder, UINT nParentFolderID, vector<CMessageFolderInfo> *vEntryList)=0;
    // create or rename message sub folders by folder ID
    virtual HRESULT STDMETHODCALLTYPE WriteFolderEntries(BOOL bCreate, BOOL bCreateRootFolder, UINT nParentFolderID, UINT nRenameFolderID, CString strDiaplayName, UINT &nNewID)=0;
    // delete sub folders by folder ID
    virtual HRESULT STDMETHODCALLTYPE DeleteFolderEntry( UINT nFolderID )=0;
    // update message state
    virtual HRESULT STDMETHODCALLTYPE UpdateMessageState(UINT uiMsgID, UINT nState = 0)=0;
};

#endif