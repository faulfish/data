/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   ISMSServer.h  $
 *  $Revision:   1.21  $
 *  $Date:   Jan 16 2008 10:53:44  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/SMSServer/ISMSServer.h.-arc  $
 * 
 *    Rev 1.21   Jan 16 2008 10:53:44   Eddy Guo
 * update progress bar and add update message state
 * 
 *    Rev 1.20   Nov 23 2007 08:31:32   Eddy Guo
 * update
 * 
 *    Rev 1.19   Nov 22 2007 10:02:54   Eddy Guo
 * update
 * 
 *    Rev 1.18   Nov 20 2007 17:13:52   Eddy Guo
 * update
 * 
 *    Rev 1.17   Nov 09 2007 12:59:08   Eddy Guo
 * Add Header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file ISMSServer.h
    @brief {brief description of ISMSServer.h}

    {detail description of ISMSServer.h}
*/
#ifndef __SMSSERVER_ISMSSERVER_H__
#define __SMSSERVER_ISMSSERVER_H__
 
#include <vector>
#include "../../CommonUtil/NotifyBase.h"
#include "MessageStructure.h"
 
using namespace QSYNC_LIB;
using namespace std;
typedef vector<UINT> VecUINT;
 
const CString theAppName = _T("SMS Server Dll");
 
const TCHAR SMSSERVER_LIST_ONE_FOLDER    [] = _T("Server_List_One_Folder");
const TCHAR SMSSERVER_LIST_FOLDER_OK     [] = _T("Server_List_Folder_OK");
const TCHAR SMSSERVER_READ_ONE_MESSAGE   [] = _T("Server_Read_One_Message");
const TCHAR SMSSERVER_DELETE_ONE_MESSAGE [] = _T("Server_Del_One_Message");
const TCHAR SMSSERVER_WRITE_ONE_MESSAGE  [] = _T("Server_Write_One_Message");
const TCHAR SMSSERVER_SAVE_ONE_MESSAGE   [] = _T("Server_Save_One_Message");
 
const TCHAR SMSSERVER_BEGIN_PROGRESS_BAR [] = _T("Server_Begin_Progress_Bar");
const TCHAR SMSSERVER_END_PROGRESS_BAR   [] = _T("Server_End_Progress_Bar");
 
const TCHAR SMSSERVER_BACKUP_ONE_MESSAGE [] = _T("Server_Backup_One_Message");
const TCHAR SMSSERVER_RESTORE_ONE_MESSAGE[] = _T("Server_Restore_One_Message");
const TCHAR SMSSERVER_GET_MESSAGE_BY_LOC [] = _T("Server_Get_Message_By_Loc");
 
const TCHAR SMSSERVER_UPDATE_TREE_CONTROL[] = _T("Server_Update_Tree_Control");
const TCHAR SMSSERVER_UPDATE_LIST_CONTROL[] = _T("Server_Update_List_Control");
 
const TCHAR SMSSERVER_SEND_ONE_PAGE_MSG  [] = _T("Server_Send_One_Page_Message");
 
// SMS Pure Virtual Functions
// For Implementation, Message View
interface ISMSServer {
 
    // 指定Location後, 取得所有在這Location的messages, 可能有多個message,
    // 這些message會存放在Messages裡
    virtual bool GetMessagesByLocation(int loc) = 0;
 
    // 查詢目前在loc這個folder裡有多少messages
    virtual UINT GetMessagesCountByLocation(enumMsgLoc loc) = 0;
 
    // 搬移指定的mesasge到指定的folder(多個), 如果MMI有支援, 就可以實作
    //virtual bool MoveFolder(UINT nIDs[], UINT count, UINT fromFolder, UINT toFolder, long timeout) = 0;
 
    // 備份資料使用, 寫入檔案, 如果檔案存在, 則修改檔名, 備份SM, ME,
    // 注意重複的message id
    virtual bool Backup(LPCTSTR szFilename) = 0;
 
    // 還原資料使用, 完全寫入, 要先刪除原先在手機裡頭的資料, 而且指定ME, 非SM
    virtual bool Restore(LPCTSTR szFilename) = 0;
 
    // 詢問目前folder ID下還有多少folders
    //virtual bool QueryFolder(bool bIsRoot, UINT nCurrentFolderID, VecUINT& vecFolderID) = 0;
 
    // 詢問所有的folder, 例用QueryFolder來問到所有的folder
    //virtual bool QueryAllFolders() = 0;
 
    // 詢問目前folder ID下, 有多少message ID (應該要回傳一個list)
    //virtual bool QueryMessageByFolderID(UINT nCurrentFolderID, VecUINT& vecMessageID) = 0;
 
    // 第一次透手機連線時, load上來所有的message, 會在完成時發送Event給view
    virtual bool ReloadAllMessageFromMobile() = 0;
 
    // 刪除folder
    virtual bool DeleteFolderByID(UINT nFolderID) = 0;
 
    virtual bool CreateISMS() = 0;
 
    virtual bool Register(INotify* pNotify,const TSTRING& strEvent) = 0;
    virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) = 0;
    virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent) = 0;
    virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) = 0;
 
    // For STD, ListMessage will return all message contents including message id, stat, alpha, length, pdu
    // For BenQ for TI, ListMessage will return all the message id only
    // For New Command, ListMessage will return all the message id and its status
    // This vector is saving decoded messages
    virtual bool ListMessage(int nLocation , vector<CShortMessage>& vecShortMessage) = 0;
 
    // Read message by id. This function is reading out a message by the message id.
    // It will return a decoded short message.
    virtual bool ReadMessage(UINT nMessageID, CShortMessage& message) = 0;
 
    // Send a message. Input short message and a vector of Recipients.
    virtual bool SendMessage(CString cstrMessage, vector<CString>& vecRecipient) = 0;
 
    // Send a message. before send message, this message will write into mobile.
    // This write function is used for View
    virtual bool SendMessage(CString strContactList, CString strContent) = 0;
 
    // Send a message by indicating ID.
    // Because the message may be a Concatenated message, this function will read from UI message
    // to send multi page messages.
    virtual bool SendMessage(UINT nMessageID, CString cstrAddress) = 0;
 
    // Write a message. Write a message into mobile.
    virtual bool WriteMessage(enumMsgLoc eMsgLoc, BYTE* pMsgData, UINT uiMsgDataSize, UINT* puiMsgID) = 0;
    virtual bool WriteMessage(CString strContactList, CString strContent, int nLoc) = 0;
    virtual bool WriteMessage(enumStorageTag eMsgStorage, UINT nFolderID, UINT nTag, BYTE* pMsgData, UINT uiMsgDataSize, UINT* nRetID, char* szTime) = 0;
    virtual bool WriteDropMessage(CString strSender, CString strMsg, CString strState, CString strLoc, CString strStorage) = 0;
 
    // Delete a message. Delete a message by indicating message id.
    virtual bool DeleteMessage(UINT nMessageID) = 0;
    virtual bool DeleteMessage(CString strIDList) = 0;
 
    // List All Folder from the indicated folder id
    // save the folder into vecFolder
    virtual bool ListFolder(UINT nFolderID, vector<CMessageFolderInfo>& vecFolder) = 0;
 
    // Query Message Space
    // This function is used for query remnant space to write
    // The remnant space is counted by message page
    virtual bool QueryMessageSpace(enumStorage eStorage, UINT& nRemnant) = 0;
 

    // Update Message State
    // This function is used for update a message from read to unread (message state)
    // Or update the message from unread to read
    virtual bool UpdateMessageState(UINT nIndex, UINT nState) = 0;
 
};
 
#endif // __SMSSERVER_ISMSSERVER_H__
