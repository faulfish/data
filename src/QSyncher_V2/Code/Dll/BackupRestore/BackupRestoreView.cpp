/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   BackupRestoreView.cpp  $
 *  $Revision:   1.16  $
 *  $Date:   Dec 11 2007 15:00:22  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/BackupRestore/BackupRestoreView.cpp.-arc  $
   
      Rev 1.16   Dec 11 2007 15:00:22   Alan Tu
   update
   
      Rev 1.15   Nov 09 2007 13:04:30   Eddy Guo
   add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file BackupRestoreView.cpp
    @brief {brief description of BackupRestoreView.cpp}

    {detail description of BackupRestoreView.cpp}
*/
// BackupRestoreView.cpp : implementation of the CBackupRestoreView class
//
 
#include "stdafx.h"
#include "BackupRestoreView.h"
#include "../../Components\UIFramework\LayoutManager\Win32Widge.h"
#include "../../Components/SMSServer/SMSServer.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)

using namespace QSYNC_LIB;
 
extern class NotifyReceiver g_NotifyReceiver;
// CBackupRestoreView
 
IMPLEMENT_DYNCREATE(CBackupRestoreView, Win32DoubleBufferFormViewWidge)
 
BEGIN_MESSAGE_MAP(CBackupRestoreView, Win32DoubleBufferFormViewWidge)
    ON_BN_CLICKED(IDC_BTN_BACKUP, &CBackupRestoreView::OnBackupButtonClick)
    ON_BN_CLICKED(IDC_BTN_RESTORE, &CBackupRestoreView::OnRestoreButtonClick)
    ON_WM_DESTROY()
END_MESSAGE_MAP()
 
// CBackupRestoreView construction/destruction
 
CBackupRestoreView::CBackupRestoreView()
	: Win32DoubleBufferFormViewWidge(CBackupRestoreView::IDD), m_pBoxLayout(NULL)
{
	// TODO: add construction code here
    g_NotifyReceiver.m_pINotify = this;
}
 
CBackupRestoreView::~CBackupRestoreView()
{
 
}
 
void CBackupRestoreView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_CHECK_VCALENDAR, m_checkBoxVCalendar);
    //DDX_Control(pDX, IDC_CHECK_VCARD,     m_checkBoxVCard);
    DDX_Control(pDX, IDC_CHECK_PHONEBOOK, m_checkBoxPhonebook);
    DDX_Control(pDX, IDC_CHECK_SMS,       m_checkBoxSMS);
    DDX_Control(pDX, IDC_CHECK_JAVA,      m_checkBoxJava);
    DDX_Control(pDX, IDC_BTN_BACKUP,      m_btnBackup);
    DDX_Control(pDX, IDC_BTN_RESTORE,     m_btnRestore);
}
 
void CBackupRestoreView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();
    m_checkBoxVCalendar.SetCheck(BST_CHECKED);
    //m_checkBoxVCard.SetCheck(BST_CHECKED);
    m_checkBoxPhonebook.SetCheck(BST_CHECKED);
    m_checkBoxSMS.SetCheck(BST_CHECKED);
    m_checkBoxJava.SetCheck(BST_CHECKED);
    CreateILocalization(&m_pLocalization);
    UpdateUiLocalization();
    UpdateButtonStatus(false, false, false, false, false, false, false);
    m_pLocalization->Register(this, TNotify_Language_Change);
    m_pCfgTool = CreateIConfigure(_T("BackupRestoreView"));
    m_pCfgTool->Load(STORAGE_INIFILE);
	SetUI();
 
}
void CBackupRestoreView::SetUI()
{
	m_pBoxLayout = new BoxLayout<HDC>(TopToBottom);
	m_pBoxLayout->SetMargin(10, 0, 10, 10);
 
	m_pBoxLayout->AddWidget( new WIN32WindowWidge(GetDlgItem(IDC_CHECK_VCALENDAR)), -1, 33);
	m_pBoxLayout->AddWidget( new WIN32WindowWidge(GetDlgItem(IDC_CHECK_PHONEBOOK)), -1, 33);
	m_pBoxLayout->AddWidget( new WIN32WindowWidge(GetDlgItem(IDC_CHECK_SMS)),  -1, 33);
	m_pBoxLayout->AddWidget( new WIN32WindowWidge(GetDlgItem(IDC_CHECK_JAVA)), -1, 33);
		BoxLayout<HDC>* pButtonLayout = new BoxLayout<HDC>;
		pButtonLayout->SetSpacing(5);
		pButtonLayout->AddWidget( new WIN32WindowWidge(GetDlgItem(IDC_BTN_BACKUP)), 75, 23);
		pButtonLayout->AddWidget( new WIN32WindowWidge(GetDlgItem(IDC_BTN_RESTORE)), 75, 23);
	m_pBoxLayout->AddWidget(pButtonLayout, -1 ,23);
 
	SetLayout(m_pBoxLayout);
}
 
void CBackupRestoreView::UpdateButtonStatus(bool bCheckVCaneldar, bool bCheckVCard,
                                            bool bCheckPhonebook, bool bCheckSMS,
                                            bool bCheckJava, bool bBtnBackup, bool bBtnRestore)
{
    m_checkBoxVCalendar.EnableWindow(bCheckVCaneldar);
    //m_checkBoxVCard.EnableWindow(bCheckVCard);
    m_checkBoxPhonebook.EnableWindow(bCheckPhonebook);
    m_checkBoxSMS.EnableWindow(bCheckSMS);
    m_checkBoxJava.EnableWindow(bCheckJava);
    m_btnBackup.EnableWindow(bBtnBackup);
    m_btnRestore.EnableWindow(bBtnRestore);
}
 
void CBackupRestoreView::UpdateUiLocalization() {
    ASSERT(m_pLocalization);
    GetDlgItem(IDC_GROUP_BACKUP_RESTORE)->SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_FUNCTION")));
    m_checkBoxVCalendar.SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_CHECKBOX_VCALENDAR")));
    //m_checkBoxVCard.SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_CHECKBOX_VCARD")));
    m_checkBoxPhonebook.SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_CHECKBOX_PHONEBOOK")));
    m_checkBoxSMS.SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_CHECKBOX_SMS")));
    m_checkBoxJava.SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_CHECKBOX_JAVA")));
    m_btnBackup.SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_BTN_BACKUP")));
    m_btnRestore.SetWindowText(GetString(_T("IDS_BACKUP_RESTORE_BTN_RESTORE")));
}
 
void CBackupRestoreView::OnEventAfterSwitchView(long nParam)
{
    TSTRING strMobileModel, strIMEI;
    SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
    if(view_switch)
        if(view_switch->m_pIMainframe->IsConnected(strMobileModel, strIMEI))
            UpdateButtonStatus(true, true, true, true, true, true, true);
}
 
void CBackupRestoreView::GetMobileInfo() {
    TCHAR strApp[256] = {0};
    TCHAR pMobileModel[256] = {0};
    TCHAR pIMEI[256] = {0};
    TCHAR pDisplayName[256] = {0};
    ::ZeroMemory(pDisplayName, 256);
    ::ZeroMemory(strApp, 256);
    ::ZeroMemory(pMobileModel, 256);
    ::ZeroMemory(pIMEI, 256);
    CommGetMobileInfo(strApp, pMobileModel, pIMEI, pDisplayName);
    m_szMobileModel = pMobileModel;
    m_szIMEI = pIMEI;
}
 
void CBackupRestoreView::Event(const std::wstring &strEvent, long nParam) {
    if(strEvent == _T("TNotify_Connect")) {
        UpdateButtonStatus(true, true, true, true, true, true, true);
        //GetMobileInfo();
    } else if(strEvent ==  _T("TNotify_Disconnect")) {
        UpdateButtonStatus(false, false, false, false, false, false, false);
    } else if(strEvent ==  _T("TNotify_Before_Switch")) {
    } else if(strEvent ==  _T("TNotify_After_Switch")) {
        OnEventAfterSwitchView(nParam);
    } else if(strEvent == TNotify_Language_Change) {
        UpdateUiLocalization();
    } else 	if(strEvent == g_strNotifyDBFinish) {
        // Sync finished at here, notice that save to file
        wstring test = m_spDevice->GetName();
        m_spIPIMDB->Save(_T("BackupRestore_VCalendar"));
        m_Event.SetEvent();
    } else if(strEvent == g_strNotifyDBInsert) {
 
    }
}
 
void CBackupRestoreView::BackupVCalendarPIMDB() {
    m_spDevice = new CClientDevice<QSYNC_LIB::VCalendarPolicy,CPIMRecord>;
    m_spIPIMDB = m_spDevice->Connect(m_szMobileModel, m_szIMEI, m_spDevice);
    bool bRes = false;
    if(m_spIPIMDB) {
        m_spIPIMDB->Register(this,g_strNotifyDBFinish);
        m_spIPIMDB->Register(this,g_strNotifyDBInsert);
        m_spIPIMDB->Register(this,g_strNotifyDBRemove);
        IDevicePts Devices;
        if(m_spIPIMDB->EnumRemoteDevice(Devices)) {
            for(IDevicePts::const_iterator cIter = Devices.begin();cIter != Devices.end();cIter++) {
                if((*cIter)->GetUniqueName() != m_spDevice->GetUniqueName()) {
                    if((*cIter)->CanSync()) {
                        (*cIter)->Sync(TwoWay);
                        //CQProgressServer::Exec( &(this->m_CalProgressBar) );  //Show progressing bar
                        //Fire(CALENDAR_V2VIEW_SWITCHVIEW_OFF);
                        bRes = true;
                        break;
                    }
                }
            }
        }
    }
}
 
void CBackupRestoreView::BackupVCarPIMDB() {
}
 
void CBackupRestoreView::RestoreVCalendarPIMDB() {
 
}
void CBackupRestoreView::RestoreVCarPIMDB() {
 
}
 
void CBackupRestoreView::OnBackupButtonClick() {
    if(m_checkBoxVCalendar.GetCheck() == BST_CHECKED) {
        GetMobileInfo();
        BackupVCalendarPIMDB();
        WaitForSingleObject(m_Event, INFINITE);
    }
 
    if(m_checkBoxPhonebook.GetCheck() == BST_CHECKED) {
        GetMobileInfo();
        BackupVCarPIMDB();
        WaitForSingleObject(m_Event, INFINITE);
    }
    if(m_checkBoxSMS.GetCheck() == BST_CHECKED) {
        // backup sms without WaitForSingleObject
        IGeneralBackupRestore* IGBR = CSMSServer::CreateBackupRestoreInstance();
        IGBR->GeneralBackup(_T("test"));
    }
    if(m_checkBoxJava.GetCheck() == BST_CHECKED) {
        // use file system to read these files for backuping up java
    }
    GenerateIndexFile();
}
 
void CBackupRestoreView::OnRestoreButtonClick() {
    if(m_checkBoxVCalendar.GetCheck() == BST_CHECKED) {
        GetMobileInfo();
        RestoreVCalendarPIMDB();
        WaitForSingleObject(m_Event, INFINITE);
    }
 
    if(m_checkBoxPhonebook.GetCheck() == BST_CHECKED) {
        GetMobileInfo();
        RestoreVCarPIMDB();
        WaitForSingleObject(m_Event, INFINITE);
    }
    if(m_checkBoxSMS.GetCheck() == BST_CHECKED) {
        // backup sms without WaitForSingleObject
        IGeneralBackupRestore* IGBR = CSMSServer::CreateBackupRestoreInstance();
        IGBR->GeneralRestore(_T("test"));
    }
    if(m_checkBoxJava.GetCheck() == BST_CHECKED) {
        // use file system to read these files for restoring up java
    }
    GenerateIndexFile();
}
 
void CBackupRestoreView::GenerateIndexFile() {
}
 
#ifdef _DEBUG
void CBackupRestoreView::AssertValid() const {
	CFormView::AssertValid();
}
 
void CBackupRestoreView::Dump(CDumpContext& dc) const {
	CFormView::Dump(dc);
}
 
#endif //_DEBUG
BOOL QSYNC_LIB::CBackupRestoreView::DestroyWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    m_pCfgTool->Save(STORAGE_INIFILE);
    DeleteIConfigure(_T("BackupRestoreView"));
    return __super::DestroyWindow();
}
 
void QSYNC_LIB::CBackupRestoreView::OnDestroy()
{
    __super::OnDestroy();
    // TODO: Add your message handler code here
    m_pCfgTool->Save(STORAGE_INIFILE);
    DeleteIConfigure(_T("BackupRestoreView"));
}

#pragma warning(pop)