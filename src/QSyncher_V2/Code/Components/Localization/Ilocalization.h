///////////////////////////////////////////////////////////
//  ILocalization.h
//  Implementation of the Interface ILocalization
//  Created on:      06-Feb-2007 11:25:55
///////////////////////////////////////////////////////////

#if !defined(EA_0912E197_4BAE_438a_A9F9_AEF67474B8A2__INCLUDED_)
#define EA_0912E197_4BAE_438a_A9F9_AEF67474B8A2__INCLUDED_

#include <string>
#include "..\..\CommonUtil\notifybase.h"

#ifdef		LOCALIZATION_DLL
#define		LOCALIZATION_DLL_DECLSPEC		__declspec(dllexport)
#else
#define		LOCALIZATION_DLL_DECLSPEC		__declspec(dllimport)
#endif

namespace QSYNC_LIB
{
	#define TNotify_Language_Change	_T("TNotify_Language_Change")	//no parameter

	interface ILocalization
	{
		virtual void GetString(const TSTRING& strID,TSTRING& strRes) =0;
		virtual bool SetRegion(const TSTRING& strRegion) =0;
		virtual int GetRegionNum(void) =0;									    //mei adds
		virtual void GetRegion(int nIndex, TSTRING& strRegion)=0;				//mei adds
		virtual void GetRegionTitle(int nIndex, TSTRING& strRegionTitle)=0;		//mei adds
		virtual void GetCurrentRegion(TSTRING& strRegion)=0;					//mei adds 
		virtual void GetDefaultRegion(TSTRING& strRegion)=0;					//mei adds
		virtual LCID GetCurrentLCID()=0;										//mei adds
		/* 
		 * GetDate:
		 * [FORMAT]
		 * dwflags = LOCALE_NOUSEROVERRIDE 
		 *	         LOCALE_USE_CP_ACP
		 * 			 DATE_SHORTDATE
		 *		 	 DATE_LONGDATE
		 *			 DATE_YEARMONTH
		 *			 DATE_USE_ALT_CALENDAR
		 *			 DATE_LTRREADING
		 *			 DATE_RTLREADING
		 * lpFormat is superior to dwflags
		 **************************************************************************************
		 * If there is any question about the format of dwflags or lpFormat, 
		 * please search GetDateFormat in MSDN for your information.
		 */
		virtual bool GetDate(LCID lcid, int nYear, int nMonth, int nDay, LPCTSTR lpFormat, TSTRING& strDate, DWORD dwflags=DATE_SHORTDATE)=0;	   //mei adds
		/* 
		 * GetTime:
		 * [FORMAT]
		 * dwflags = LOCALE_NOUSEROVERRIDE  
		 *			 LOCALE_USE_CP_ACP
		 *			 TIME_NOMINUTESORSECONDS 
		 *			 TIME_NOSECONDS 
		 *			 TIME_NOTIMEMARKER
		 *			 TIME_FORCE24HOURFORMAT
		 *	lpFormat is superior to dwflags
		 **************************************************************************************
		 * If there is any question about the format of dwflags or lpFormat, 
		 * please search GetTimeFormat in MSDN for your information.
		 */
		virtual bool GetTime(LCID lcid, int nHour, int nMinute, int nSecond, LPCTSTR lpFormat, TSTRING& strTime, DWORD dwflags=TIME_NOSECONDS)=0;//mei adds
		virtual TSTRING GetString(const TSTRING& strID) = 0;
		virtual bool Register(INotify* pNotify,const TSTRING& strEvent) = 0;
		virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) = 0;
		virtual bool Unregister(INotify* pNotify,const TSTRING& strEvent) = 0;
		virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) = 0;
	};

	LOCALIZATION_DLL_DECLSPEC bool CreateILocalization(ILocalization** ppILocalization);
}
#endif // !defined(EA_0912E197_4BAE_438a_A9F9_AEF67474B8A2__INCLUDED_)
