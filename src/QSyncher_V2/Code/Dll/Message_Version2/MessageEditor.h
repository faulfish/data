/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageEditor.h  $
 *  $Revision:   1.31  $
 *  $Date:   Mar 27 2008 16:09:14  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageEditor.h.-arc  $
 * 
 *    Rev 1.31   Mar 27 2008 16:09:14   Alan Tu
 * update
 * 
 *    Rev 1.30   Mar 27 2008 15:36:22   Alan Tu
 * Disappear when disconnect
 * 
 *    Rev 1.29   Mar 21 2008 13:27:16   Alan Tu
 * remove unused
 * 
 *    Rev 1.28   Feb 29 2008 17:20:12   Alan Tu
 * update
 * 
 *    Rev 1.27   Feb 27 2008 20:41:56   Mei Yang
 * change SetUI->SetXMLUI
 * 
 *    Rev 1.26   Feb 12 2008 14:59:16   Eddy Guo
 * fix bug
 * ATHB1A_AthensB1A SW_Issue  ATHB1A-00402
 * 
 *    Rev 1.25   Feb 12 2008 09:33:48   Eddy Guo
 * update for new behavior for add contacts
 * 
 *    Rev 1.24   Feb 01 2008 10:21:12   Jame Tsai
 * add IDS_Message_Button_AddPhoneNo
 * 
 *    Rev 1.23   Jan 30 2008 19:46:14   Eddy Guo
 * update
 * 
 *    Rev 1.22   Jan 28 2008 08:18:52   Eddy Guo
 * Add anonymous contact dialog
 * 
 *    Rev 1.21   Jan 04 2008 17:31:52   Jame Tsai
 * add word count widget
 * 
 *    Rev 1.20   Jan 02 2008 15:03:30   Jame Tsai
 * update
 * 
 *    Rev 1.19   Jan 01 2008 17:08:44   Jame Tsai
 * Set buttons
 * 
 *    Rev 1.18   Dec 31 2007 17:45:34   Jame Tsai
 * Set type
 * 
 *    Rev 1.17   Dec 27 2007 16:41:10   Eddy Guo
 * update
 * 
 *    Rev 1.16   Dec 20 2007 08:44:04   Eddy Guo
 * update
 * 
 *    Rev 1.15   Dec 18 2007 14:26:16   Eddy Guo
 * update
 * 
 *    Rev 1.14   Dec 17 2007 10:05:56   Eddy Guo
 * add new view for design
 * 
 *    Rev 1.13   Nov 09 2007 13:02:42   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageEditor.h
    @brief {brief description of MessageEditor.h}

    {detail description of MessageEditor.h}
*/
#ifndef __MESSAGE_EDITOR_H__
#define __MESSAGE_EDITOR_H__
 
#pragma once
#include "../../CommonUtil/NotifyBase.h"
#include "../../Components/Localization/ilocalization.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include <map>
#include "afxcmn.h"
#include "afxdtctl.h"
#include "afxwin.h"
#include "Resource.h"
#include "AnonymousContactDialog.h"
 
using namespace QSYNC_LIB;
using namespace std;
class CMessageEditor : public Win32DoubleBufferQDialogWidge,public QSYNC_LIB::CINotifyBase
{
    DECLARE_DYNAMIC(CMessageEditor)
 
public:
    CMessageEditor(CWnd* pParent = NULL);   // standard constructor
    virtual ~CMessageEditor();
	void Event(const TSTRING& strEvent, long nParam);

    enum { IDD = IDD_MESSAGE_BOX };
    //CTreeCtrl m_ContactListTreeCtrl;    // Contact list tree control
    CListCtrl m_ContactListCtrl;        // Contact list control for showing phone book
    CListCtrl m_ContactListRight;       // contact list control for right position (read for sending a message)    
    CEdit m_ContactListFilter;          // Select the contact list (key in then find all)
    CEdit m_ContentEdit;                // Content text edit control
    CString* m_pStrOutputContactList;   // for output contact list
    CString* m_pStrOutputContentText;   // for output content text
    bool* m_pBSaveToSim;                // for output save to sim
    CString m_strTitle;                 // title for dialog box
    CWnd* m_pPostMessageWnd;            // for remembering parent
    CString m_strOldContactList;        // for recording the correct contact list    
    ILocalization* m_pLocalization;     // localization
    map<TSTRING, TSTRING>* m_pMapLeftPB;// phone book
    map<TSTRING, TSTRING> m_MapRightPB; // phone book
	
	WIN32LabelWidge* m_WordCountWidget;	// Show the number of words in short message
	WIN32IconButton* m_pWinCloseButton;
	WIN32IconButton* m_pAddBtn;
	WIN32IconButton* m_pRemoveBtn;
	WIN32IconButton* m_pAddPhoneNoBtn;
	WIN32IconButton* m_pBtnCancel;
	WIN32IconButton* m_pBtnSave;
	WIN32IconButton* m_pBtnSend; 
	WIN32IconButton* m_pBtnCheck;
	CAnonymousContactDialog m_acd;

    
	//TSTRING m_strWordCount;				// Number of words in short message
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    DECLARE_MESSAGE_MAP()
private:
    CString GetString(const TSTRING& strID){return m_pLocalization->GetString(strID).c_str();}
    bool CheckAllASCIIString(BYTE* szInputString, int nLength);
    void SetPhonebook(map<TSTRING, TSTRING>* pMapPB);
    void SetContactList(map<TSTRING, TSTRING> pMapPB);
    void AddAnonymousContact(vector<CString>* vecContacts);
public:
    void SetContactList(CString strContactList);
    void SetContentText(CString strContentText);
    void GetContactList(CString& strContactList);
    void GetContentText(CString& strContentText);
	void SetXMLUI();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DoModal(CString& strContactList, CString& strContentText, bool &bSaveToSim, map<TSTRING, TSTRING>* mapPB);
    void OnEnChangeWordCountExtracted(CString strTest, bool bAllASCII, int &nPageCount);
	void UpdateWordCount(const TSTRING&);	
	virtual void Disconnect(const TSTRING& strTitle,const TSTRING& strContent = _T("IDS_Cal_Disconnect"),bool bShowMessage = true);
private:
	void SetBackground();	
	void RegetContacts();

	//Button Function:
	void FnWinClose();
	void OnBtnClickSave();

    bool GetMessageEditorInformation(bool bSend);

	void OnBtnClickSend();
	void OnBtnClickAdd();
	void OnBtnClickRemove();
	void OnBnClickSaveToSim();

private:
    afx_msg void OnBnClickedMsgDlgBtnReceiver();
    afx_msg void OnBnClickedMsgDlgBtnSend();
    afx_msg void OnBnClickedMsgDlgBtnSave();
    afx_msg void OnBnClickedMsgDlgBtnCancle();
    afx_msg void OnEnChangeWordCount();
    afx_msg void OnEnChangeContactList();
    afx_msg void OnBnClickedCheckSaveToSim();
    afx_msg void OnBnClickedMsgDlgBtnRemove();
    afx_msg void OnEnChangeMsgDlgEditSelectPhonebook();
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
    afx_msg void OnLvnItemchangedMsgDlgPhonebookList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickMsgDlgPhonebookList(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMDblclkMsgDlgPhonebookList(NMHDR *pNMHDR, LRESULT *pResult);
public:
    afx_msg void OnBnClickedBtnAddAnonymousContact();
};
 
#endif
