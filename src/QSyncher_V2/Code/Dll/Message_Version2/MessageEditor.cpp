/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   MessageEditor.cpp  $
 *  $Revision:   1.50  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageEditor.cpp.-arc  $
   
      Rev 1.50   Mar 27 2008 16:09:14   Alan Tu
   update
   
      Rev 1.49   Mar 27 2008 15:36:20   Alan Tu
   Disappear when disconnect
   
      Rev 1.48   Mar 21 2008 13:27:16   Alan Tu
   remove unused
   
      Rev 1.47   Mar 20 2008 16:00:26   Alan Tu
   update
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageEditor.cpp
    @brief {brief description of MessageEditor.cpp}

    {detail description of MessageEditor.cpp}
*/
#include "StdAfx.h"
#include "MessageEditor.h"
#include "..\..\Components\UIFramework\CreateRgnFromFile.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"

using namespace QSYNC_LIB;
IMPLEMENT_DYNAMIC(CMessageEditor, CQDialog)

CMessageEditor::CMessageEditor(CWnd* pParent /*=NULL*/)
: Win32DoubleBufferQDialogWidge(CMessageEditor::IDD, pParent),m_WordCountWidget(NULL)
,m_pAddBtn(NULL)
,m_pRemoveBtn(NULL)
,m_pAddPhoneNoBtn(NULL)
,m_pBtnCancel(NULL)
,m_pBtnSave(NULL)
,m_pBtnSend(NULL)
,m_pBtnCheck(NULL)
{
}

CMessageEditor::~CMessageEditor()
{
}

void CMessageEditor::DoDataExchange(CDataExchange* pDX)
{
    CQDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_MSG_DLG_PHONEBOOK_LIST, m_ContactListCtrl);
    DDX_Control(pDX, IDC_CONTACT_RIGHT_LIST, m_ContactListRight);
    DDX_Control(pDX, IDC_MSG_DLG_EDIT_FILTER_PHONEBOOK, m_ContactListFilter);
    DDX_Control(pDX, IDC_CONTENT_TEXT_EDIT, m_ContentEdit);
}

BEGIN_MESSAGE_MAP(CMessageEditor, CQDialog)    
    ON_EN_CHANGE(IDC_CONTENT_TEXT_EDIT, &CMessageEditor::OnEnChangeWordCount)
    ON_EN_CHANGE(IDC_MSG_DLG_EDIT_FILTER_PHONEBOOK, &CMessageEditor::OnEnChangeMsgDlgEditSelectPhonebook)
    ON_WM_SIZING()
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_MSG_DLG_PHONEBOOK_LIST, &CMessageEditor::OnLvnItemchangedMsgDlgPhonebookList)
    ON_NOTIFY(NM_CLICK, IDC_MSG_DLG_PHONEBOOK_LIST, &CMessageEditor::OnNMClickMsgDlgPhonebookList)
    ON_NOTIFY(NM_DBLCLK, IDC_MSG_DLG_PHONEBOOK_LIST, &CMessageEditor::OnNMDblclkMsgDlgPhonebookList)
END_MESSAGE_MAP()

BOOL CMessageEditor::OnInitDialog()
{
    CQDialog::OnInitDialog();
    CreateILocalization(&m_pLocalization);
    SetWindowText(m_strTitle);
    
    m_ContactListCtrl.InsertColumn(0, GetString(_T("IDS_PHONEBOOK_NAME")));
    m_ContactListCtrl.InsertColumn(1, GetString(_T("IDS_PHONEBOOK_PRIVATENUMBER")));
    m_ContactListCtrl.SetColumnWidth(0, 120);
    m_ContactListCtrl.SetColumnWidth(1, 150);
    m_ContactListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);

    m_ContactListRight.InsertColumn(0, GetString(_T("IDS_PHONEBOOK_NAME")));
    m_ContactListRight.InsertColumn(1, GetString(_T("IDS_PHONEBOOK_PRIVATENUMBER")));
    m_ContactListRight.SetColumnWidth(0, 120);
    m_ContactListRight.SetColumnWidth(1, 150);
    m_ContactListRight.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	SetXMLUI();
	OnEnChangeWordCount();
    SetContactList(*m_pStrOutputContactList);
    SetContentText(*m_pStrOutputContentText);
    SetPhonebook(m_pMapLeftPB);
    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

void CMessageEditor::Event(const TSTRING& strEvent, long nParam)
{
	if(strEvent == EVENT_BUTTON_CLICK)
	{
		if(m_pWinCloseButton->GetID() == nParam || m_pBtnCancel->GetID() == nParam)
			FnWinClose();
		else if(m_pAddBtn->GetID() == nParam)
			OnBtnClickAdd();
		else if(m_pRemoveBtn->GetID() == nParam)
			OnBtnClickRemove();
		else if(m_pAddPhoneNoBtn->GetID() == nParam)
			OnBnClickedBtnAddAnonymousContact();
		else if(m_pBtnSave->GetID() == nParam)
			OnBtnClickSave();
		else if(m_pBtnSend->GetID() == nParam)
			OnBnClickedMsgDlgBtnSend();
		else if(m_pBtnCheck->GetID() == nParam)
			OnBnClickedCheckSaveToSim();
	}
}
void CMessageEditor::SetXMLUI()
{
	SetBackground();
	CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
	Widge<HDC>* pWidget = pLayoutManager->Load(_T("MessageEditorID"));
	m_WordCountWidget = (WIN32LabelWidge *)pLayoutManager->GetWidgetByID(_T("Win32Label_5"));
	m_pWinCloseButton = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_0"));
	m_pBtnCheck = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_4"));
	m_pAddBtn = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_1"));
	m_pRemoveBtn = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_3"));
	m_pAddPhoneNoBtn = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_2"));
	m_pBtnCancel = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_5"));
	m_pBtnSave = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_6"));
	m_pBtnSend = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_7")); 

	SetLayout(pWidget);
	delete pLayoutManager;

}

void CMessageEditor::SetPhonebook(map<TSTRING, TSTRING>* pMapPB) {
    m_ContactListCtrl.DeleteAllItems();
    for(map<TSTRING, TSTRING>::iterator iter = pMapPB->begin(); iter != pMapPB->end(); iter++) {
        if(iter->second != _T("")) 
		{
            int nInsertItem = m_ContactListCtrl.InsertItem(0, CString(iter->first.c_str()));
            BOOL bSetItemText = m_ContactListCtrl.SetItemText(nInsertItem, 1, CString(iter->second.c_str()));
            ASSERT(bSetItemText);
            if(m_MapRightPB.find(iter->first) != m_MapRightPB.end())
                m_ContactListCtrl.SetItemState(nInsertItem, LVIS_CUT , LVIS_CUT);
        }
    }
}

void CMessageEditor::SetContactList(map<TSTRING, TSTRING> pMapPB) {
    m_ContactListRight.DeleteAllItems();
    if(pMapPB.empty()) return;
    int i = 0;
    for(map<TSTRING, TSTRING>::iterator iter = pMapPB.begin(); iter != pMapPB.end(); iter++) {
        int nInsertItem = m_ContactListRight.InsertItem(i, CString(iter->first.c_str()));
        BOOL bSetItemText = m_ContactListRight.SetItemText(nInsertItem, 1, CString(iter->second.c_str()));
        ++i;
    }
}

// add contacts to right
// not use now...
void CMessageEditor::OnBnClickedMsgDlgBtnReceiver()
{
}
// remove a contact at the caret position
// not use now...
void CMessageEditor::OnBnClickedMsgDlgBtnRemove()
{
}

void CMessageEditor::SetContactList(CString strContactList) {
    //TCHAR* szContactList = new TCHAR[strContactList.GetLength()+1];
    //memset(szContactList, 0, strContactList.GetLength()+1);
    //_tcscpy_s(szContactList, strContactList.GetLength()+1, strContactList);
    RegetContacts();
    //delete szContactList;
	/*
    for(map<TSTRING, TSTRING>::iterator iter = m_pMapLeftPB->begin(); iter != m_pMapLeftPB->end(); iter++) {
        if(iter->second == strContactList.GetString()) {
            m_MapRightPB[iter->first] = iter->second;
            SetContactList(m_MapRightPB);
            return;
        }
    }
	*/
    if(strContactList != _T(""))
        m_MapRightPB[_T("Anonymous")] = strContactList.GetString();
    SetContactList(m_MapRightPB);
}

void CMessageEditor::SetContentText(CString strContentText) {
    TCHAR* szContentText = new TCHAR[strContentText.GetLength()+1];
    memset(szContentText, 0, strContentText.GetLength()+1);
    _tcscpy_s(szContentText, strContentText.GetLength()+1, strContentText);
    this->m_ContentEdit.SetWindowText(szContentText);
    delete szContentText;
}

// Cancel
void CMessageEditor::OnBnClickedMsgDlgBtnCancle()
{
    CQDialog::EndDialog(IDC_MSG_DLG_BTN_CANCEL);
}

// Send Button
void CMessageEditor::OnBnClickedMsgDlgBtnSend()
{
	RegetContacts();
    m_ContentEdit.GetWindowText(*m_pStrOutputContentText);
    if(m_pStrOutputContentText->GetLength() == 0) {
        ShowMessage(this->GetSafeHwnd(), GetString(_T("IDS_MESSAGE_INPUT_ERROR")), GetString(_T("IDS_MESSAGE_FUNCTION")), QMB_OK);
        return;
    }
    CQDialog::EndDialog(IDC_MSG_DLG_BTN_SEND);
}

// Save Button
void CMessageEditor::OnBnClickedMsgDlgBtnSave()
{
    RegetContacts();
    m_ContentEdit.GetWindowText(*m_pStrOutputContentText);
    if(m_pStrOutputContentText->GetLength() == 0) {
        ShowMessage(this->GetSafeHwnd(), GetString(_T("IDS_MESSAGE_INPUT_ERROR")), GetString(_T("IDS_MESSAGE_FUNCTION")), QMB_OK);
        return;
    }
    CQDialog::EndDialog(IDC_MSG_DLG_BTN_SAVE);
}

INT_PTR CMessageEditor::DoModal(CString& strContactList, CString& strContentText, bool& bSaveToSim, map<TSTRING, TSTRING>* mapPB)
{
    m_pStrOutputContactList = &strContactList;
    m_strOldContactList = strContactList;
    m_pStrOutputContentText = &strContentText;
    m_pBSaveToSim = &bSaveToSim;
    m_pMapLeftPB = mapPB;
    return CDialog::DoModal();
}

bool CMessageEditor::CheckAllASCIIString(BYTE* szInputString, int nLength) {
    for(int i = 0; i < nLength; ++i) {
        if(szInputString[i] > 127) {
            return false;
        }
    }
    return true;
}

void CMessageEditor::OnEnChangeWordCountExtracted(CString strTest, bool bAllASCII, int &nPageCount)
{
    if(bAllASCII) {
        m_ContentEdit.SetLimitText(755);
        if(strTest.GetLength() > 160) {
            nPageCount = (strTest.GetLength() / 152);
            if(strTest.GetLength() % 152 != 0) nPageCount++;
        }
    } else {
        m_ContentEdit.SetLimitText(330);
        if(strTest.GetLength() > 70) {
            nPageCount = (strTest.GetLength() / 66);
            if(strTest.GetLength() % 66 != 0) nPageCount++;
        }
    }

	CString strPages;
	strPages.Format(GetString(_T("IDS_Message_Page_Count")),strTest.GetLength(),nPageCount);
	UpdateWordCount((LPCTSTR)strPages);
}
void CMessageEditor::OnEnChangeWordCount()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the __super::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.
    // TODO:  Add your control notification handler code here
    CString strTest;
    m_ContentEdit.GetWindowText(strTest);
    DWORD dwNum = WideCharToMultiByte(CP_ACP, NULL, strTest, -1, NULL, 0, NULL, FALSE);
    char* psText = new char[dwNum];
    memset(psText, 0, dwNum);
    WideCharToMultiByte(CP_ACP, NULL, strTest, -1, psText, dwNum, NULL, FALSE);
    bool bAllASCII = CheckAllASCIIString((BYTE*)psText, dwNum-1);
    delete [] psText;
    int nPageCount = 1;
    OnEnChangeWordCountExtracted(strTest, bAllASCII, nPageCount);
    CString strPageNumber(GetString(_T("IDS_Message_Page_Count")));
    strPageNumber.Format((LPCTSTR)strPageNumber, strTest.GetLength(), nPageCount);
    //GetDlgItem(IDC_WORD_COUNT)->SetWindowText(strPageNumber);
}

void CMessageEditor::OnBnClickedCheckSaveToSim()
{
}

// when the text is changed, filter the contact list for the LIST Control to show
// not use now...
void CMessageEditor::OnEnChangeMsgDlgEditSelectPhonebook()
{
    // TODO:  If this is a RICHEDIT control, the control will not
    // send this notification unless you override the Win32DoubleBufferFormCWndWidge<CDialog>::OnInitDialog()
    // function and call CRichEditCtrl().SetEventMask()
    // with the ENM_CHANGE flag ORed into the mask.

    // TODO:  Add your control notification handler code here
    CString strFilter;
    m_ContactListFilter.GetWindowText(strFilter);
    if(strFilter.IsEmpty()) {
        SetPhonebook(m_pMapLeftPB);
        return;
    }
    map<TSTRING, TSTRING> pFilterPB;
    map<TSTRING, TSTRING>::iterator iter;
    for(iter = m_pMapLeftPB->begin(); iter != m_pMapLeftPB->end(); iter++) {
        size_t nPos1 = iter->first.find((LPTSTR)(LPCTSTR)strFilter);
        size_t nPos2 = iter->second.find((LPTSTR)(LPCTSTR)strFilter);
        if(nPos1 != TSTRING::npos || nPos2 != TSTRING::npos)
            pFilterPB[iter->first] = iter->second;
    }
    SetPhonebook(&pFilterPB);
}

void CMessageEditor::OnSizing(UINT fwSide, LPRECT pRect)
{
    //Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
    GeometryRect rcClient = GetGeometry();
    CQDialog::SetWindowRegion(pRect, rcClient);
    // TODO: Add your message handler code here
}

void CMessageEditor::SetBackground()
{
	std::wstring strBkImage = L"Message\\Background\\Bk_SMSEdit.png";
	SetBKImage(strBkImage,BK_Fixed);

	const int nMIN_WIDTH_MAINFRAME = GetBKImage()->GetWidth(); 
	const int nMIN_HEIGHT_MAINFRAME = GetBKImage()->GetHeight();

	SetMinWidth(nMIN_WIDTH_MAINFRAME);
	SetMinHeight(nMIN_HEIGHT_MAINFRAME);
	SetMaxWidth(nMIN_WIDTH_MAINFRAME);
	SetMaxHeight(nMIN_HEIGHT_MAINFRAME);
}

void CMessageEditor::UpdateWordCount(const TSTRING& strWordCount)
{
	if(m_WordCountWidget)
	{
		m_WordCountWidget->SetDisplayString(strWordCount);
		m_WordCountWidget->Update();
	}
}

//Button functions

void CMessageEditor::FnWinClose()
{
	CQDialog::EndDialog(IDC_MSG_DLG_BTN_CANCEL);
}

void CMessageEditor::OnBtnClickSave()
{
    RegetContacts();
    if(!GetMessageEditorInformation(false))
    	return;
    CQDialog::EndDialog(IDC_MSG_DLG_BTN_SAVE);
}

void CMessageEditor::OnBtnClickSend()
{
	//m_ContactListEdit.GetWindowText(*m_pStrOutputContactList);
    if(!GetMessageEditorInformation(true))
        return;
	CQDialog::EndDialog(IDC_MSG_DLG_BTN_SEND);
}

void CMessageEditor::RegetContacts()
{
	m_pStrOutputContactList->Empty();
    for(int i = 0; i < m_ContactListRight.GetItemCount(); ++i) {
        *m_pStrOutputContactList += m_ContactListRight.GetItemText(i, 1) + CString(_T(";"));
    }
}

bool CMessageEditor::GetMessageEditorInformation(bool bSend) {
    RegetContacts();
    m_pStrOutputContactList->TrimRight(_T(";"));
    m_ContentEdit.GetWindowText(*m_pStrOutputContentText);
    if((bSend && m_pStrOutputContactList->GetLength() == 0)) {
        ShowMessage(this->GetSafeHwnd(), GetString(_T("IDS_MESSAGE_INPUT_ERROR")), GetString(_T("IDS_MESSAGE_FUNCTION")), QMB_OK);
        return false;
    } else if(m_pStrOutputContentText->GetLength() == 0) {
        ShowMessage(this->GetSafeHwnd(), GetString(_T("IDS_MESSAGE_INPUT_ERROR")), GetString(_T("IDS_MESSAGE_FUNCTION")), QMB_OK);
        return false;
    }
    return true;
}

void CMessageEditor::OnBtnClickAdd()
{

    for(int i = 0; i < m_ContactListRight.GetItemCount(); ++i) {
        if(m_ContactListRight.GetItemText(i, 0).Find(_T("Anonymous")) != -1) {
            //mapRight[m_ContactListRight.GetItemText(i, 0).GetString()] = m_ContactListRight.GetItemText(i, 1).GetString();
            m_MapRightPB[m_ContactListRight.GetItemText(i, 0).GetString()] = m_ContactListRight.GetItemText(i, 1).GetString();
        }
    }
    int nTotalCount = m_ContactListCtrl.GetItemCount();
    for(int i = 0; i < nTotalCount; ++i) {
        if(m_ContactListCtrl.GetCheck(i)) {
            UINT nState = m_ContactListCtrl.GetItemState(i,  (UINT)-1);
            m_ContactListCtrl.SetItemState(i, LVIS_CUT , LVIS_CUT);
            CString strItemName = m_ContactListCtrl.GetItemText(i, 0);
            CString strItemPhone = m_ContactListCtrl.GetItemText(i, 1);
            if(strItemPhone.GetLength() != 0)
                m_MapRightPB[strItemName.GetString()] = strItemPhone.GetString();
        }
    }
    SetContactList(m_MapRightPB);
}

void CMessageEditor::OnBtnClickRemove()
{
    map<CString, CString> mapSelected;
    int nTotalCount = m_ContactListCtrl.GetItemCount();
    POSITION pos = m_ContactListRight.GetFirstSelectedItemPosition();
    while(pos != NULL) {
        int nItem = m_ContactListRight.GetNextSelectedItem(pos);
        CString strItemName = m_ContactListRight.GetItemText(nItem, 0);
        CString strItemPhone = m_ContactListRight.GetItemText(nItem, 1);
        mapSelected[strItemName] = strItemPhone;
        for(int i = 0; i < m_ContactListCtrl.GetItemCount(); ++i) {
            if(m_ContactListCtrl.GetItemText(i, 0) == strItemName &&
               m_ContactListCtrl.GetItemText(i, 1) == strItemPhone &&
               m_ContactListCtrl.GetItemState(i, LVIS_CUT) == LVIS_CUT) {
                m_ContactListCtrl.SetItemState(i, 0, LVIS_CUT);
                m_ContactListCtrl.SetCheck(i, FALSE);
                break;
            }
        }
    }
    for(map<CString, CString>::iterator iter = mapSelected.begin(); iter != mapSelected.end(); iter++) {
        for(int j = 0; j < m_ContactListRight.GetItemCount(); ++j) {
            CString strItemName = m_ContactListRight.GetItemText(j, 0);
            CString strItemPhone = m_ContactListRight.GetItemText(j, 1);
            if(iter->first == strItemName && iter->second == strItemPhone) {
                m_ContactListRight.DeleteItem(j);
                break;
            }
        }
    }
    m_MapRightPB.clear();
    for(int i = 0; i < m_ContactListRight.GetItemCount(); ++i) {
        m_MapRightPB[m_ContactListRight.GetItemText(i, 0).GetString()] = m_ContactListRight.GetItemText(i, 1).GetString();
    }
}

void CMessageEditor::OnBnClickSaveToSim()
{
}

void CMessageEditor::OnLvnItemchangedMsgDlgPhonebookList(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    //// TODO: Add your control notification handler code here
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int nItem = pNMListView->iItem;

    BOOL b = m_ContactListCtrl.GetCheck(nItem);
    int nState = m_ContactListCtrl.GetItemState(nItem, LVIS_STATEIMAGEMASK);
    int nCut = m_ContactListCtrl.GetItemState(nItem, LVIS_CUT);
    if(nCut == LVIS_CUT) {
        m_ContactListCtrl.SetCheck(nItem);
    }

    *pResult = 0;
}

void CMessageEditor::OnNMClickMsgDlgPhonebookList(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int nItem = pNMListView->iItem;
    int nCut = m_ContactListCtrl.GetItemState(nItem, LVIS_CUT);
    int nImaeState = m_ContactListCtrl.GetItemState(nItem, LVIS_STATEIMAGEMASK);
    int nIndex = nImaeState >> 12;
    if(nCut == LVIS_CUT) return;
    if(m_ContactListCtrl.GetCheck(nItem)) {
        m_ContactListCtrl.SetCheck(nItem, FALSE);
    } else {
        m_ContactListCtrl.SetCheck(nItem);
    }
    *pResult = 0;
}

void CMessageEditor::OnNMDblclkMsgDlgPhonebookList(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    int nItem = pNMListView->iItem;
    int nCut = m_ContactListCtrl.GetItemState(nItem, LVIS_CUT);
    int nImaeState = m_ContactListCtrl.GetItemState(nItem, LVIS_STATEIMAGEMASK);
    int nIndex = nImaeState >> 12;
    if(nCut == LVIS_CUT) return;
    if(m_ContactListCtrl.GetCheck(nItem)) {
        m_ContactListCtrl.SetCheck(nItem, FALSE);
    } else {
        m_ContactListCtrl.SetCheck(nItem);
    }
    *pResult = 0;
}

void CMessageEditor::OnBnClickedBtnAddAnonymousContact()
{
    // TODO: Add your control notification handler code here
    CString str1st, str2nd, str3rd, str4th, str5th;    
    m_acd.SetExtractStrings(str1st, str2nd, str3rd, str4th, str5th);
    vector<CString> vecPhoneNumber;
    INT_PTR nRetCode = m_acd.DoModal();
    switch(nRetCode) {
        case 0:
            vecPhoneNumber.push_back(str1st);
            vecPhoneNumber.push_back(str2nd);
            vecPhoneNumber.push_back(str3rd);
            vecPhoneNumber.push_back(str4th);
            vecPhoneNumber.push_back(str5th);
            AddAnonymousContact(&vecPhoneNumber);
            break;
        default:
            break;
    }
}

void CMessageEditor::Disconnect(const TSTRING& strTitle,const TSTRING& strContent,bool bShowMessage)
{	
	m_acd.Disconnect(_T("IDS_MESSAGE_FUNCTION"),_T("IDS_MESSAGE_FUNCTION"),false);
	CQDialog::Disconnect(_T("IDS_MESSAGE_FUNCTION"));
}

void CMessageEditor::AddAnonymousContact(vector<CString>* vecContacts) {
    map<TSTRING, TSTRING> mapPhoneNumber;
    int nItenCount = m_ContactListRight.GetItemCount();
    for(int i = 0; i < nItenCount; ++i) {
        CString strName = m_ContactListRight.GetItemText(i, 0);
        CString strNumber = m_ContactListRight.GetItemText(i, 1);
        mapPhoneNumber[strName.GetString()] = strNumber.GetString();
    }
    static int index = 0;
    for(size_t i = 0; i < vecContacts->size(); ++i) {
        if(vecContacts->at(i).GetLength() != 0) {
            CString strAnonymous = GetString(_T("IDS_MESSAGE_ANONYMOUS"));
            CString strCount;
            strCount.Format(_T(" %d"), index++);
            strAnonymous += strCount;
            mapPhoneNumber[strAnonymous.GetString()] = vecContacts->at(i).GetString();
        }
    }
    SetContactList(mapPhoneNumber);
}