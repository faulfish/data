#ifndef _IOUTLOOKCTRL_HPP__
#define _IOUTLOOKCTRL_HPP__
#pragma once

#include "..\PIMDB\SyncManager.h"

namespace QSYNC_LIB
{

#ifdef		OUTLOOKACCESS_DLL
	#define		OUTLOOKACCESS_DLL_DECLSPEC		__declspec(dllexport)
#else
	#define		OUTLOOKACCESS_DLL_DECLSPEC		__declspec(dllimport)
#endif


#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif


const LPCTSTR EVENT_CONTACT_FETCHING_FINISH  = _T("Contact Fetching Finish");
const LPCTSTR EVENT_CALENDAR_FETCHING_FINISH = _T("Calendar Fetching Finish");
const LPCTSTR EVENT_CONTACT_FETCHING_FAIL	 = _T("Contact Fetching Fail");
const LPCTSTR EVENT_CALENDAR_FETCHING_FAIL   = _T("Calendar Fetching Fail");
const LPCTSTR EVENT_ACCESS_ESTABLISH_FAIL    = _T("Access Establish Fail");

	class IOutlookCtrl
	{
	public:
		IOutlookCtrl(){}
		virtual ~IOutlookCtrl(){}

		virtual bool StartToFetch() =0;
		virtual bool Abort()=0;
		
		virtual bool GetByIndex(int nIndex, IPIMObjPt& PIMObjPt)=0;
		virtual int GetCount()=0;
		virtual bool Insert(IPIMObjPt PIMObjPt)=0;
		virtual bool Update(IPIMObjPt PIMObjPt)=0;
		virtual bool Delete(IPIMObjPt PIMObjtPt)=0;

		virtual bool Register(INotify* pNotify,const TSTRING& strEvent) = 0;
		virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent) = 0;
	};


const TCHAR KEYWORD_CONTACT[] = _T("Contact");
const TCHAR KEYWORD_CALENDAR[] = _T("Calendar");
	
	OUTLOOKACCESS_DLL_DECLSPEC bool CreateOutlookAccess(LPCTSTR cpName, IOutlookCtrl** ppOutlookCtrl);
	OUTLOOKACCESS_DLL_DECLSPEC bool DeleteOutlookAccess(IOutlookCtrl* pOutlookCtrl);
	
}//namespace QSYNC_LIB
#endif //_IOUTLOOKCTRL_HPP__

