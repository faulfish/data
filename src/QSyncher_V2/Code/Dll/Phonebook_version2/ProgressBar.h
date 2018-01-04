#ifndef _PROGRESSBAR_HPP_
#define _PROGRESSBAR_HPP_

#pragma once

#include "..\..\Components\UIFramework\interface\IQProgressClient.h"
#include "..\..\Components\Localization\ilocalization.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

class ProgressBar : public QSYNC_LIB::QProgressClientBase
{
public :
	ProgressBar()
		:m_bTaskAbort(FALSE)
		,m_RunningEvent(FALSE,TRUE)
		,m_GetEvent(false,true,_T("vcard"))
		,m_nPersent(0)
		,m_hDeviceConnectionStatusEvent(FALSE,TRUE,_T("MobileDeviceConnectionEvent"))
	{
		CreateILocalization(&m_pILocale);
		SetIsAnimate(true);
	}

	// To implement Progress UI Manipulation interface:
public:
	virtual CString GetTitle()
	{
		return m_pILocale->GetString(_T("IDS_PHONEBOOK_FUNCTION")).c_str();
	}
	virtual CString GetDisplayText()
	{
		return m_pILocale->GetString(_T("IDS_Initiation")).c_str();
	}
	virtual CString GetCancelButtonCaption()
	{
		return m_pILocale->GetString(_T("IDS_Cancel")).c_str();
	}
	virtual CString GetProgressCaption()
	{
		return _T("");
	}
	virtual CString GetProgressCaptionPercentChar(void)
	{
		return m_strProgressCaptionPercentChar;
	}
	virtual BOOL GetAutoClose(void){return TRUE; }
	virtual BOOL GetCancelable(void){return FALSE;}

	CEvent m_RunningEvent;
	int m_nPersent;

	// To implement Progress Task Manipulation interface:
public:
	virtual bool StartTask()
	{
		using namespace QSYNC_LIB;
		QProgressNotify qpn;
		qpn.m_strCurrentDisplayText = m_pILocale->GetString(_T("IDS_PHONEBOOK_PROGRESS_TITLE")).c_str();

		m_GetEvent.ResetEvent();

		while(WAIT_OBJECT_0 == ::WaitForSingleObject(m_RunningEvent, 0))
		{
			if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_hDeviceConnectionStatusEvent,0))
				break;

			if(WAIT_OBJECT_0 == ::WaitForSingleObject(m_GetEvent,100))
			{
				m_GetEvent.ResetEvent(); 
				qpn.m_nCurrentPosition = m_nPersent;
				Fire(QSYNC_LIB::EVENT_PROGRESS_TASK_UPDATE, reinterpret_cast<long>(&qpn));			
			}
		};

		HWND hwnd = NULL;
		Fire(EVENT_PROGRESS_TASK_GETHWMD,(long)&hwnd);

		return true;
	}

	virtual bool AbortTask()
	{
		using namespace QSYNC_LIB;
		HWND hwnd = NULL;
		Fire(EVENT_PROGRESS_TASK_GETHWMD,(long)&hwnd);

		int nCancelable = ShowMessage(hwnd,m_pILocale->GetString(_T("IDS_Sync_Cancel")) ,m_pILocale->GetString(_T("IDS_PHONEBOOK_FUNCTION")),QMB_YESNO);
		m_bTaskAbort = (nCancelable == QIDYES);	
		return m_bTaskAbort == TRUE;
	}

	virtual bool ResumeTask(void){return TRUE;}
	virtual bool SuspendTask(void){return TRUE;}

private:
	CString m_strProgressCaptionPercentChar;

private:
	QSYNC_LIB::ILocalization* m_pILocale;

	bool m_bTaskAbort;	
	CEvent m_GetEvent;
	CEvent	m_hDeviceConnectionStatusEvent;
};

#pragma warning(pop)

#endif // _PROGRESSBAR_HPP_