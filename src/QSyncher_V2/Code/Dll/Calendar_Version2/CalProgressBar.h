#ifndef _CCALPROGRESSBAR_HPP_
#define _CCALPROGRESSBAR_HPP_
#endif
#pragma once

#include "..\..\Components\UIFramework\interface\IQProgressClient.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

class CalProgressBar : public QSYNC_LIB::QProgressClientBase
{
public :
	CalProgressBar()
		:m_bTaskAbort(FALSE)
		,m_RunningEvent(FALSE,TRUE)
		,m_GetVCalendarEvent(false,true,_T("vcalendar"))
		,m_nPersent(0)
	{
		CreateILocalization(&m_pILocale);
		SetIsAnimate(true);
	}
	virtual ~CalProgressBar(){}
	// To implement Progress UI Manipulation interface:
public:
	virtual CString GetTitle(void);
	virtual CString GetDisplayText(void);
	virtual CString GetCancelButtonCaption(void);
	virtual CString GetProgressCaption(void);
	virtual CString GetProgressCaptionPercentChar(void);
	virtual BOOL GetAutoClose(void){return TRUE; }
	virtual BOOL GetCancelable(void){return FALSE;}

	CEvent m_RunningEvent;
	int m_nPersent;

	// To implement Progress Task Manipulation interface:
public:
	virtual bool StartTask(void);
	virtual bool AbortTask(void);
	virtual bool ResumeTask(void);
	virtual bool SuspendTask(void);

private:
	CString m_strTitle;
	CString m_strDisplayText;
	CString m_strCancelButtonCaption;
	CString m_strProgressCaption;
	CString m_strProgressCaptionPercentChar;

private:
	QSYNC_LIB::ILocalization* m_pILocale;

	bool m_bTaskAbort;	
	CEvent m_GetVCalendarEvent;

};


//**********************Function Implement**********************//
inline bool CalProgressBar::StartTask()
{
	using namespace QSYNC_LIB;
	QProgressNotify qpn;
	qpn.m_strCurrentDisplayText = m_pILocale->GetString(_T("IDS_CALENDAR_PROGRESS_TITLE")).c_str();

	m_GetVCalendarEvent.ResetEvent();

	while(WAIT_OBJECT_0 == ::WaitForSingleObject(m_RunningEvent, 0))
	{
		if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_GetVCalendarEvent,100))
		{
			m_GetVCalendarEvent.ResetEvent(); 
			qpn.m_nCurrentPosition = m_nPersent;
			Fire(QSYNC_LIB::EVENT_PROGRESS_TASK_UPDATE, reinterpret_cast<long>(&qpn));			
		}
	};

	HWND hwnd = NULL;
	Fire(EVENT_PROGRESS_TASK_GETHWMD,(long)&hwnd);
	
	return true;
}

inline bool CalProgressBar::AbortTask()
{
	using namespace QSYNC_LIB;
	HWND hwnd = NULL;
	Fire(EVENT_PROGRESS_TASK_GETHWMD,(long)&hwnd);

	int nCancelable = ShowMessage(hwnd,m_pILocale->GetString(_T("IDS_Sync_Cancel")) ,m_pILocale->GetString(_T("IDS_CALENDAR_FUNCTION")),QMB_YESNO);
	m_bTaskAbort = (nCancelable == QIDYES);	
	return m_bTaskAbort == TRUE;
}

inline bool CalProgressBar::ResumeTask()
{
	//m_RunningEvent.SetEvent();
	return true;
}

inline bool CalProgressBar::SuspendTask()
{
	//m_RunningEvent.ResetEvent();
	return true;
}

inline CString CalProgressBar::GetTitle()
{
	m_strTitle = m_pILocale->GetString(_T("IDS_CALENDAR_FUNCTION")).c_str();
	return m_strTitle;
}

inline CString CalProgressBar::GetDisplayText()
{
	m_strDisplayText = m_pILocale->GetString(_T("IDS_Initiation")).c_str();
	return m_strDisplayText;
}

inline CString CalProgressBar::GetCancelButtonCaption()
{
	return m_pILocale->GetString(_T("IDS_Cancel")).c_str();
}

inline CString CalProgressBar::GetProgressCaption()
{
	return _T("");
}

inline CString CalProgressBar::GetProgressCaptionPercentChar()
{
	return m_strProgressCaptionPercentChar;
}

//********************Function Implement End********************//

#pragma warning(pop)