#ifndef _CVCalCtrl_HPP_
#define _CVCalCtrl_HPP_

#ifdef		PIMDB_DLL
#define		PIMDB_DLL_DECLSPEC		__declspec(dllexport)
#else
#define		PIMDB_DLL_DECLSPEC		__declspec(dllimport)
#endif

#pragma once
#include "../../Components/Localization/ilocalization.h"
#include "..\..\CommonUtil\PIMStringUtility.h"
#include "../../Components/PIMDB/SyncMLDevice.h"
#include "../../Components/PIMDB/PIMRecord.h"
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"

#include <string>
#include <cstringt.h>

using namespace std;

namespace QSYNC_LIB 
{

	/* Merlin Format - Interface */ 
#define					KEYWORD_SUBJECT							"VEVENT:SUMMARY"
#define                 KEYWORD_LOCATION						"VEVENT:LOCATION"	
#define					KEYWORD_CATEGORIES						"VEVENT:CATEGORIES"
#define					KEYWORD_DTSTART							"VEVENT:DTSTART"
#define					KEYWORD_DTEND							"VEVENT:DTEND"
#define					KEYWORD_RECURRENCE						"VEVENT:RRULE"
#define					KEYWORD_DALARM							"VEVENT:DALARM:0"
#define					KEYWORD_DESCRIPTION						"VEVENT:DESCRIPTION"
#define					KEYWORD_ATTENDEE						"VEVENT:ATTENDEE"
#define					KEYWORD_EXDATE							"VEVENT:EXDATE"

#define					KEYWORD_DALARM_1						"VEVENT:DALARM:1"
#define					KEYWORD_DALARM_2						"VEVENT:DALARM:2"

#define					KEYWORD_VEVENT							_T("VEVENT")


#define					RRULE_TYPE_DAILY						_T("D1")
#define					RRULE_TYPE_WEEKLY						_T("W1")
#define					RRULE_TYPE_MONTHLY						_T("MD1")
#define					RRULE_TYPE_YEARLY						_T("YM1")

#define					CATEGORIES_APPOINTMENT                  _T("APPOINTMENT")
#define					CATEGORIES_BUSINESS						_T("BUSINESS")
#define					CATEGORIES_EDUCATION					_T("EDUCATION")
#define					CATEGORIES_HOLIDAY						_T("HOLIDAY")
#define					CATEGORIES_MEETING						_T("MEETING")
#define					CATEGORIES_MISC							_T("MISC")
#define					CATEGORIES_PERSONAL						_T("PERSONAL")
#define					CATEGORIES_PHONE						_T("PHONE")
#define					CATEGORIES_SICKDAY						_T("SICKDAY")
#define					CATEGORIES_SPECIAL						_T("SPECIAL")
#define					CATEGORIES_TRAVEL						_T("TRAVEL")
#define					CATEGORIES_VACATION						_T("VACATION")
#define					CATEGORIES_ANNIVERSARY					_T("ANNIVERSARY")

	const TSTRING RRULE_WEEK[7]	= {_T("SU"), _T("MO"), _T("TU"), _T("WE"), _T("TH"), _T("FR"), _T("SA")};

	class PIMDB_DLL_DECLSPEC CVCalCtrl	
	{
	public:
		QSYNC_LIB::ILocalization*	m_pILocale;
	public:
		CVCalCtrl(IPIMObjPt PIMObjPt);
		virtual ~CVCalCtrl(void);

		bool GetCmnStrData(const char* cpKey, CString& strCmnData);
		bool GetCmnIntData( const char * szName, long& nInt );         
		bool SetCmnStrData(const char * szName, const TCHAR* szData);
		bool SetCmnIntData(const char * szName, int nData);

		bool SetCmnWStrData(const char * szName, const wchar_t* szData);
		std::wstring GetWString( const char * szName );

		bool SetEmptyIPIMObjPt(void);
		bool SaveCmnData(const char* cpKey, const CString& strValue);	

		bool GetLimitation(const TSTRING& strKey, TSTRING& strLimitation);
		int  GetLimitation(const TSTRING& strKey);
	private:
		IPIMObjPt m_PIMObjPt;
		IPIMCONFIG* m_pIPIMConfig;
	private:
		bool SmartAddPIMEntity(const char* cpKey, const CString& strValue);
		bool AddPIMProptyName(const char* cpKey);
		bool AddPIMOtherValues(const char* cpKey);
		bool AddPIMParas(const char* cpKey, bool bEncoding, bool bCharset);

		bool SmartDeletePIMEntity(const char* cpKey);
		bool DeletePIMParas(const char* cpKey);
		bool DeletePIMValues(const char* cpKey);
		bool DeletePIMProptyName(const char* cpKey);

		bool CheckLimitation(const std::string& cpKey, TSTRING& strValue);
		bool IsInConfigLimitation(const TSTRING& strKey, TSTRING& strValue, bool bUTF8);
		bool IsKey(const std::string strKey, const std::string strOrg, TSTRING& tstrKey);
		bool IsUTF8(const std::string& strOrgKey);
		bool TrimValue(int nLimitation, TSTRING& strValue, bool bUTF8);
	};

} //end namespace QSYNC_LIB

#endif // _CVCalCtrl_HPP_