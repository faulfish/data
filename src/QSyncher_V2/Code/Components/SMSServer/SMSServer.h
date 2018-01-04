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
// ������UI�h�Ω��h�����U���O��SERVER, �䤤�]�t���\�঳�s�W, �R��, �ǰe, Ū��
// �h��, �ƥ�, �٭�
// �|�O���@���w�Ѷ}������Messages, �Τ@�����Ѷ}��Messages, �Q��DecodedMessage,
// ��SM, BQSM��@, �䤤Util��������Message�ާ@, �Ѧ�CUtil.h, CUtil.cpp
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
 
    // ���wLocation��, ���o�Ҧ��b�oLocation��messages, �i�঳�h��message,
    // �o��message�|�s��bMessages��
    virtual bool GetMessagesByLocation(int loc) sealed;
 
    // �d�ߥثe�bloc�o��folder�̦��h��messages
    virtual UINT GetMessagesCountByLocation(enumMsgLoc loc);
 
    // �h�����w��mesasge����w��folder(�h��), �p�GMMI���䴩, �N�i�H��@
    //virtual bool MoveFolder(UINT nIDs[], UINT count, UINT fromFolder, UINT toFolder, long timeout);
 
    // �ƥ���ƨϥ�, �g�J�ɮ�, �p�G�ɮצs�b, �h�ק��ɦW, �ƥ�SM, ME,
    // �`�N���ƪ�message id
    virtual bool Backup(LPCTSTR szFilename);
 
    // �٭��ƨϥ�, �����g�J, �n���R������b������Y�����, �ӥB���wME, �DSM
    virtual bool Restore(LPCTSTR szFilename);
 
    // �߰ݥثefolder ID�U�٦��h��folders, �^�Ǫ�uint vector�i�H���m_MapFolderMessageID
    //virtual bool QueryFolder(bool bIsFolder, UINT nCurrentFolderID, VecUINT& vecFolderID);
 
    // �߰ݩҦ���folder, �ҥ�QueryFolder�Ӱݨ�Ҧ���folder, �n��recursive
    //virtual bool QueryAllFolders();
 
    // �߰ݥثefolder ID�U, ���h��message ID (���ӭn�^�Ǥ@��list)
    //virtual bool QueryMessageByFolderID(UINT nCurrentFolderID, VecUINT& vecMessageID);
 
    // �Ĥ@���z����s�u��, load�W�өҦ���message, �|�b�����ɵo�eEvent��view
    virtual bool ReloadAllMessageFromMobile();

    size_t ListMessageByLocation(UINT nStart, UINT nEnd, size_t nTotalMessage, enumAtCmdSet cmdSet);

    // �R��folder
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
    // ��CList��message pool, ��Ҧ���message����b�o���pool
    // �䤤DecodedMessages�O�w�Ѷ}��, ��ShortMessages�O���Ѷ}��
    // �O�o�nRemoveAll�ɭn����delete�@��, �A�I�sRemoveAll
    CList<CDecodedMessage*> DecodedMessages;
 
    // ��CList��Standard AT Command(��l��message, pdu�O�@��bytes 16�i��r��)
    // ��message list
    CList<CMessageObject*> ShortMessages;
 
    // ��CList��BenQ AT Command(��l��message, ��ƬO�@��bytes)��message list
    CList<CMessageObject*> BQShortMessages;
    // message ID -> message (ID����message)
    map<UINT, CDecodedMessage> m_MapIDMessage;
    // �@��folder�U�٦��X��subfolder, �o���O�@��tree���c, �u�O�@�ӯª�map
    map<UINT, VecUINT*> m_MapFolderMessageID;
    // folder ID -> folder name (ID����folder name)
    map<UINT, CString> m_MapFolderName;
    // CShortMessage
    vector<CShortMessage> m_vecShortMessage;
    //vector<MsgFolder> m_vecFolder;
    vector<CMessageFolderInfo> m_vecFolder;
    // map folder name and total count
 
private:
 
    // �q���Ū���Ҧ���message, �䤤�ʧ@���UAT+CMGL���O, �n�s�JCShortMessage��CBQShortMessage
    // �Q����Ӥ��P���O����, �M��s�줣�P��List���Y, CShortMessage��CBQShortMessage
    // Ū������n�D�ѽX�A�`�NDecodedMessages���Y�񪺳��OCString Unicode���T��
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
