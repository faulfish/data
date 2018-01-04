/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   SMSServer.h  $
 *  $Revision:   1.26  $
 *  $Date:   Feb 19 2008 16:18:04  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/SMSServer/SMSServer.h.-arc  $
 * 
 *    Rev 1.26   Feb 19 2008 16:18:04   Eddy Guo
 * update
 * 
 *    Rev 1.25   Jan 23 2008 17:18:36   Eddy Guo
 * update for standard at command
 * 
 *    Rev 1.24   Jan 16 2008 10:53:44   Eddy Guo
 * update progress bar and add update message state
 * 
 *    Rev 1.23   Nov 23 2007 08:31:32   Eddy Guo
 * update
 * 
 *    Rev 1.22   Nov 22 2007 10:02:54   Eddy Guo
 * update
 * 
 *    Rev 1.21   Nov 20 2007 17:13:54   Eddy Guo
 * update
 * 
 *    Rev 1.20   Nov 09 2007 12:59:08   Eddy Guo
 * Add Header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file SMSServer.h
    @brief {brief description of SMSServer.h}

    {detail description of SMSServer.h}
*/
#ifndef __SMSSERVER_SMSSERVER_H__
#define __SMSSERVER_SMSSERVER_H__
 
// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the SMSSERVER_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// SMSSERVER_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
 
///////////////////////////////////////////////////////////////////////////////
// 中介於UI層及底層對手機下指令的SERVER, 其中包含的功能有新增, 刪除, 傳送, 讀取
// 搬移, 備份, 還原
// 會保有一份已解開完成的Messages, 及一份未解開的Messages, 利用DecodedMessage,
// 及SM, BQSM實作, 其中Util為相關的Message操作, 參考CUtil.h, CUtil.cpp
///////////////////////////////////////////////////////////////////////////////
#ifdef SMSSERVER_EXPORTS
#define SMSSERVER_API __declspec(dllexport)
#else
#define SMSSERVER_API __declspec(dllimport)
#endif
 
#include "../../CommLayer/CDCInterface/MessageUtil.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../../Components/Localization/ilocalization.h"
#include "../../Components/UIFramework/interface/QControlNotify.h"
#include "../../Dll/BackupRestore/IGeneralBackupRestore.h"
#include "..\..\ShareLib\loki-0.1.5\include\loki\Singleton.h"
#include "ISMSServer.h"
#include "MessageStructure.h"
#include "../../CommLayer/ProtocolCodec/Global_Def.h"
#include "../../Components/Configuration/QConfigure/QMobileConfigure.h"
#include "../../Components/Configuration/QConfigure/QConfigure.h"
using namespace std;
using namespace QSYNC_LIB;
// This class is exported from the SMSServer.dll
 
class SMSSERVER_API CSMSServer : public ISMSServer, public CNotifyBase, public INotify, public IGeneralBackupRestore {
public:
    CSMSServer(void);
    virtual ~CSMSServer(void);
    // Attributes
public:
    long            m_lMaxTimeOut;
    UINT            m_nMaxPageCount;
    bool            m_bIsSupportPrivateFolder;
    ISMS*           m_pISMS; ///< ISMS for all operation
    // Operations
public:
 
    // 指定Location後, 取得所有在這Location的messages, 可能有多個message,
    // 這些message會存放在Messages裡
    virtual bool GetMessagesByLocation(int loc) sealed;
 
    // 查詢目前在loc這個folder裡有多少messages
    virtual UINT GetMessagesCountByLocation(enumMsgLoc loc);
 
    // 搬移指定的mesasge到指定的folder(多個), 如果MMI有支援, 就可以實作
    //virtual bool MoveFolder(UINT nIDs[], UINT count, UINT fromFolder, UINT toFolder, long timeout);
 
    // 備份資料使用, 寫入檔案, 如果檔案存在, 則修改檔名, 備份SM, ME,
    // 注意重複的message id
    virtual bool Backup(LPCTSTR szFilename);
 
    // 還原資料使用, 完全寫入, 要先刪除原先在手機裡頭的資料, 而且指定ME, 非SM
    virtual bool Restore(LPCTSTR szFilename);
 
    // 詢問目前folder ID下還有多少folders, 回傳的uint vector可以放到m_MapFolderMessageID
    //virtual bool QueryFolder(bool bIsFolder, UINT nCurrentFolderID, VecUINT& vecFolderID);
 
    // 詢問所有的folder, 例用QueryFolder來問到所有的folder, 要用recursive
    //virtual bool QueryAllFolders();
 
    // 詢問目前folder ID下, 有多少message ID (應該要回傳一個list)
    //virtual bool QueryMessageByFolderID(UINT nCurrentFolderID, VecUINT& vecMessageID);
 
    // 第一次透手機連線時, load上來所有的message, 會在完成時發送Event給view
    virtual bool ReloadAllMessageFromMobile();

    size_t ListMessageByLocation(UINT nStart, UINT nEnd, size_t nTotalMessage, enumAtCmdSet cmdSet);

    // 刪除folder
    virtual bool DeleteFolderByID(UINT nFolderID);
 
    virtual void Event(const TSTRING& strEvent,long nParam);
 
    //-------------------------------------------------------------------------
    // For STD, ListMessage will return all message contents including message
    // id, stat, alpha, length, pdu.
    // For BenQ for TI, ListMessage will return all the message id only.
    // For New Command, ListMessage will return all the message id and its
    // status. This vector is saving decoded messages.
    //-------------------------------------------------------------------------
    virtual bool ListMessage(int nLocation , vector<CShortMessage>& vecShortMessage);
 
    // Read message by id. This function is reading out a message by the message id.
    // It will return a decoded short message.
    virtual bool ReadMessage(UINT nMessageID, CShortMessage& message);
 
    // Send a message. Input short message and a vector of Recipients.
    // Notice: After sending a message, write this message into mobile (outbox/sent).
    virtual bool SendMessage(CString cstrMessage, vector<CString>& vecRecipient);
 
    // Send a message. before send message, this message will write into mobile.
    // This write function is used for View
    virtual bool SendMessage(CString strContactList, CString strContent);
 
    // Send a message by indicating ID.
    // Because the message may be a Concatenated message, this function will read from UI message
    // to send multi page messages.
    // Notice: After sending a message, write this message into mobile (outbox/sent).
    virtual bool SendMessage(UINT nMessageID, CString cstrAddress);
 
    // Write a message. Write a message into mobile.
    virtual bool WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID);
 
    // Write a message. Write a message into mobile. Default will write into DRAFT (UNSENT(2)).
    // This write function is used for View
    virtual bool WriteMessage(CString strContactList, CString strContent, int nLoc = 2);
 
    // Write message for New BenQ AT Command...
    //
    virtual bool WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* nRetID, char* szTime);

    // Write drop file message, check the type is correct or not
    virtual bool WriteDropMessage(CString strSender, CString strMsg, CString strState, CString strLoc, CString strStorage);
 
    // Delete a message. Delete a message by indicating message id.
    virtual bool DeleteMessage(UINT nMessageID);
    virtual bool DeleteMessage(CString strIDList);
 
    // List All Folder from the indicated folder id
    // save the folder into vecFolder
    virtual bool ListFolder(UINT nFolderID, vector<CMessageFolderInfo>& vecFolder);
 
    virtual bool QueryMessageSpace(enumStorage eStorage, UINT& nRemnant);

    virtual bool UpdateMessageState(UINT nIndex, UINT nState);
 
    // For backup/restore
    int GetNumberOfPDU(LPCTSTR pFilename);
 
    virtual void GeneralBackup(LPCTSTR szFileName) {
        this->Backup(_T("Test.txt"));
    }

    virtual void GeneralRestore(LPCTSTR szFileName) {
    }

    typedef Loki::SingletonHolder<CSMSServer> SMSServerSingleton;

    static IGeneralBackupRestore* CreateBackupRestoreInstance() {
        return &SMSServerSingleton::Instance();
    }

    virtual bool CreateISMS() {
        bool bRes = false;
        if(m_pISMS == NULL) {
            m_pISMS = (ISMS*)CommCreateInterface(theAppName, _T("ISMS"), eISMS);
            return m_pISMS == NULL? false : true;
        }
        return bRes;
    }

    virtual bool Register(INotify* pNotify,const TSTRING& strEvent) {
        return CNotifyBase::Register(pNotify, strEvent);
    }
    virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) {
        return CNotifyBase::Register(pEventFunc, strEvent);
    }
    virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent) {
        return CNotifyBase::Unregister(pNotify, strEvent);
    }
    virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) {
        return CNotifyBase::Unregister(pEventFunc, strEvent);
    }


public:
    // 用CList當成message pool, 把所有的message都放在這兩個pool
    // 其中DecodedMessages是已解開的, 而ShortMessages是未解開的
    // 記得要RemoveAll時要全部delete一次, 再呼叫RemoveAll
    CList<CDecodedMessage*> DecodedMessages;
 
    // 用CList當成Standard AT Command(原始的message, pdu是一堆bytes 16進位字串)
    // 的message list
    CList<CMessageObject*> ShortMessages;
 
    // 用CList當成BenQ AT Command(原始的message, 資料是一堆bytes)的message list
    CList<CMessageObject*> BQShortMessages;
    // message ID -> message (ID對應message)
    map<UINT, CDecodedMessage> m_MapIDMessage;
    // 一個folder下還有幾個subfolder, 這不是一個tree結構, 只是一個純的map
    map<UINT, VecUINT*> m_MapFolderMessageID;
    // folder ID -> folder name (ID對應folder name)
    map<UINT, CString> m_MapFolderName;
    // CShortMessage
    vector<CShortMessage> m_vecShortMessage;
    //vector<MsgFolder> m_vecFolder;
    vector<CMessageFolderInfo> m_vecFolder;
    // map folder name and total count
 
private:
 
    // 從手機讀取所有的message, 其中動作有下AT+CMGL指令, 要存入CShortMessage及CBQShortMessage
    // 利換兩個不同的記憶體, 然後存到不同的List裡頭, CShortMessage及CBQShortMessage
    // 讀完之後要求解碼，注意DecodedMessages裡頭放的都是CString Unicode的訊息
    UINT ReadAllMessagesFromMobile();
    //UINT ReadMessagesFromFolder(UINT nFolderID);
    //UINT ListFolders(UINT nCurrentFolderID, VecUINT* vecFolderList);
    UINT CreateFolder(UINT nCurrentFolderID, CString strFolderName, UINT& nOutFolderID);
    void DefineNotify();
    void GetServiceCenterAddress(CString& strSCA);
    void BackupWriteToFile(enumStorage storageSetting, CStdioFile& file);
    enumAtCmdSet GetATCommandSet();

    bool SendMessageByUsingNewMechanism(CString strContactList, CString strContent);
    bool SendMessageByUsingStandardMethod(CString strContactList, CString strContent);
    bool WriteMessageByUsingNewMechanism(CString strContactList, CString strContent, int nLoc);
    bool WriteMessageByUsingStandardMethod(CString strContactList, CString strContent, int nLoc);

private:
    enumAtCmdSet m_ATCommandSet;
};
 
//extern SMSSERVER_API UINT nSMSServer;
 
//SMSSERVER_API UINT fnSMSServer(void);
 
 
#endif // __SMSSERVER_SMSSERVER_H__
