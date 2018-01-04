/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
*
*                           QSyncher V2 Project
*
*	             Copyright (c) 2005 BENQ Corpration
*
***************************************************************************
*
*  $Workfile:   MessageProgressBar.h  $
*  $Revision:   1.9  $
*  $Date:   Mar 17 2008 22:35:24  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/MessageProgressBar.h.-arc  $
 * 
 *    Rev 1.9   Mar 17 2008 22:35:24   Alan Tu
 * update
 * 
 *    Rev 1.8   Jan 16 2008 19:23:18   Eddy Guo
 * remove warning
 * 
 *    Rev 1.7   Jan 16 2008 10:55:16   Eddy Guo
 * update progress bar
 * 
 *    Rev 1.6   Jan 04 2008 19:36:22   Mei Yang
 * add SetIsAnimate(true);
 * 
 *    Rev 1.5   Dec 25 2007 15:58:38   Alan Tu
 * refactory
* 
*    Rev 1.4   Nov 09 2007 13:02:42   Eddy Guo
* add header
*
*
***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file MessageProgressBar.h
@brief {brief description of MessageProgressBar.h}

{detail description of MessageProgressBar.h}
*/
#ifndef __MESSAGE_PROGRESS_BAR_H__
#define __MESSAGE_PROGRESS_BAR_H__

#pragma once
#pragma warning(push)
#pragma warning(disable:4311)
#include "..\..\Components\UIFramework\interface\IQProgressClient.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"

class CMessageProgressBar : public QSYNC_LIB::QProgressClientBase {
public :
	CMessageProgressBar()
		: m_pILocalization(NULL)
		, m_bAutoClose(TRUE)
		, m_bCancelable(TRUE)
		, m_bTaskAbort(FALSE)
		, m_RunningEvent(TRUE, 1)
		, m_GetMessageEvent(false, true, _T("Message"))
		, m_bFinish(false)
		, m_nPersent(0)
	{
		CreateILocalization(&m_pILocalization);
		// NotifyBase Definitions:
		DefineNotify(QSYNC_LIB::EVENT_PROGRESS_TASK_UPDATE);
		SetIsAnimate(true);
	}

	virtual ~CMessageProgressBar() {
	}
	// To implement Progress UI Manipulation interface:
public:
	virtual CString GetTitle(void);
	virtual CString GetDisplayText(void);
    virtual void SetDiaplayText(std::wstring strDisplayText);
	virtual CString GetCancelButtonCaption(void);
	virtual CString GetProgressCaption(void);
	virtual CString GetProgressCaptionPercentChar(void);
	virtual BOOL GetAutoClose(void);
	virtual BOOL GetCancelable(void);



	// To implement Progress Task Manipulation interface:
	virtual bool StartTask(void);
	virtual bool AbortTask(void);
	virtual bool ResumeTask(void);
	virtual bool SuspendTask(void);

public:
	bool m_bFinish;
	int m_nPersent;
    CEvent m_RunningEvent;
    CEvent m_GetMessageEvent;
    CString m_strTitle;
    CString m_strDisplayText;
    CString m_strCancelButtonCaption;
    CString m_strProgressCaption;
    CString m_strProgressCaptionPercentChar;
    BOOL m_bAutoClose;
    BOOL m_bCancelable;
    QSYNC_LIB::ILocalization* m_pILocalization;
    bool m_bTaskAbort;
};


//**********************Function Implement**********************//
inline bool CMessageProgressBar::StartTask()
{
	using namespace QSYNC_LIB;
	QProgressNotify qpn;
	qpn.m_strCurrentDisplayText = m_pILocalization->GetString(_T("IDS_ProgressBar_MsgTransfer")).c_str();
	// Please write your Task Execution Logic here =============

	//(To update the Progress when necessary)
	m_GetMessageEvent.ResetEvent();
	//::WaitForSingleObject(m_RunningEvent, INFINITE);
	while(WAIT_OBJECT_0 == ::WaitForSingleObject(m_RunningEvent, 0)) 
	{
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_GetMessageEvent, 100)) {
			m_GetMessageEvent.ResetEvent();
			qpn.m_nCurrentPosition = m_nPersent;
			Fire(QSYNC_LIB::EVENT_PROGRESS_TASK_UPDATE, reinterpret_cast<long>(&qpn));
		}
	}
    //HWND hwnd = NULL;
    //Fire(EVENT_PROGRESS_TASK_GETHWMD,(long)&hwnd);
    //ShowMessage(hwnd,m_pILocalization->GetString(_T("IDS_Sync_Finish")), m_pILocalization->GetString(_T("IDS_PHONEBOOK_FUNCTION")));
	/*qpn.m_strCurrentDisplayText = this->m_strDisplayText;
	qpn.m_nCurrentPosition = (int) ceil((double)i / 3565536);
	this->Fire(QSYNC_LIB::EVENT_PROGRESS_TASK_UPDATE, reinterpret_cast<long>(&qpn));*/

	return true;
	// Please write your Task Execution Logic here =============
}

inline bool CMessageProgressBar::AbortTask()
{
	//int nCancelable = AfxMessageBox(m_pILocalization->GetString(_T("IDS_ProgressBar_MsgCancelTransfer")).c_str(), MB_ICONINFORMATION | MB_OK);
	int nCancelable = ShowMessage(NULL, _T(""), m_pILocalization->GetString(_T("IDS_ProgressBar_MsgCancelTransfer")).c_str(), QMB_OK);
	if( nCancelable) {
		this->m_bTaskAbort = TRUE;
		return true;
	} else {
		this->m_bTaskAbort = FALSE;
		return false;
	}
}

inline bool CMessageProgressBar::ResumeTask() {
	::SetEvent(this->m_RunningEvent);
	return true;
}

inline bool CMessageProgressBar::SuspendTask() {
	::ResetEvent(this->m_RunningEvent);
	return true;
}

inline CString CMessageProgressBar::GetTitle() {
    return m_pILocalization->GetString(_T("IDS_MESSAGE_FUNCTION")).c_str();
}

inline void CMessageProgressBar::SetDiaplayText(std::wstring strDisplayText) {
    //m_strDisplayText = strDisplayText.c_str();
    m_strDisplayText = m_pILocalization->GetString(strDisplayText).c_str();
}

inline CString CMessageProgressBar::GetDisplayText() {
    return m_strDisplayText;
}

inline CString CMessageProgressBar::GetCancelButtonCaption() {
    return m_pILocalization->GetString(_T("IDS_Cancel")).c_str();
}

inline CString CMessageProgressBar::GetProgressCaption() {
    return _T("");
	//return this->m_strProgressCaption;
}

inline CString CMessageProgressBar::GetProgressCaptionPercentChar() {
	return this->m_strProgressCaptionPercentChar;
}

inline BOOL CMessageProgressBar::GetAutoClose() {
	return TRUE;
}

inline BOOL CMessageProgressBar::GetCancelable() {
	return FALSE;
}

//********************Function Implement End********************//
#pragma warning(pop)

#endif
