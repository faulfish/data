/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   BackupRestoreView.h  $
 *  $Revision:   1.10  $
 *  $Date:   Nov 09 2007 13:04:30  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/BackupRestore/BackupRestoreView.h.-arc  $
 * 
 *    Rev 1.10   Nov 09 2007 13:04:30   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file BackupRestoreView.h
    @brief {brief description of BackupRestoreView.h}

    {detail description of BackupRestoreView.h}
*/
// BackupRestoreView.h : interface of the CBackupRestoreView class
//
 
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "resource.h"
 
#include "../../Components/Localization/ilocalization.h"
#include "../../Components/SMSServer/SMSServer.h"
#include "../../Components/PIMDB/ClientDevice.h"
#include "../../Components/PIMDB/PIMRecord.h"
#include "../../Components/PIMDB/SyncMLDevice.h"
#include "../../Components/ConfigureTool/IConfigure.h"
#include "../../CommonUtil/QSyncerFrameworks.h"
#include "../../Components/UIFramework/LayoutManager/Win32FrameworkWidge.h"
#include "../../Components/UIFramework/LayoutManager/BoxLayout.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "..\../CommLayer/MobileFinder/MobileFinderInterface.h"
#include "IGeneralBackupRestore.h"
 
namespace QSYNC_LIB {
 
class NotifyReceiver : public QSYNC_LIB::INotify
{
public:
    NotifyReceiver():m_pINotify(NULL){}
    virtual void Event(const TSTRING& strEvent,long nParam)
    {
        if(m_pINotify)
            m_pINotify->Event(strEvent,nParam);
    }
    QSYNC_LIB::INotify* m_pINotify;
};
 
class CBackupRestoreView : public Win32DoubleBufferFormViewWidge, public QSYNC_LIB::CINotifyBase
{
protected: // create from serialization only
	CBackupRestoreView();
    virtual ~CBackupRestoreView();
	DECLARE_DYNCREATE(CBackupRestoreView)
 
public:
	enum{ IDD = IDD_BACKUP_RESTORE_FORM };
 
// Operations
public:
 
    virtual bool Register(INotify* pNotify,const TSTRING& strEvent) { return CINotifyBase::Register(pNotify, strEvent);}
    virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) { return CINotifyBase::Register(pEventFunc, strEvent);}
    virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent) { return CINotifyBase::Unregister(pNotify, strEvent);}
    virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) { return CINotifyBase::Unregister(pEventFunc, strEvent);}
// Overrides
 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
    virtual void Event(const std::wstring &strEvent, long nParam);
 
private:
	void SetUI();
    void GenerateIndexFile();
    void UpdateButtonStatus(bool bCheckVCaneldar, bool bCheckVCard, bool bCheckPhonebook, bool bCheckSMS, bool bCheckJava, bool bBtnBackup, bool bBtnRestore);
    void UpdateUiLocalization();
    CString GetString(LPCTSTR szIDString) { return m_pLocalization->GetString(szIDString).c_str(); };
    void OnEventAfterSwitchView(long nParam);
    void GetMobileInfo();
 
    void BackupVCalendarPIMDB();
    void BackupVCarPIMDB();
 
    void RestoreVCalendarPIMDB();
    void RestoreVCarPIMDB();
 
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
 
public:
    CButton m_checkBoxVCalendar;
    CButton m_checkBoxPhonebook;
    CButton m_checkBoxSMS;
    CButton m_checkBoxJava;
    CButton m_btnBackup;
    CButton m_btnRestore;
    TSTRING m_szMobileModel;
    TSTRING m_szIMEI;
 
private:
    ILocalization* m_pLocalization;
    IConfigure* m_pCfgTool;
	BoxLayout<HDC>* m_pBoxLayout;
 
    IDevicePt m_spDevice;
    IPIMDBPt m_spIPIMDB;
    CEvent m_Event;
// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
    afx_msg void OnBackupButtonClick();
    afx_msg void OnRestoreButtonClick();
public:
    virtual BOOL DestroyWindow();
    afx_msg void OnDestroy();
};
 
} // end of namespace
