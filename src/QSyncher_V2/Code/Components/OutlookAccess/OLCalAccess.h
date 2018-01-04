#pragma once
#ifndef _OUTLOOKACCESS_OLAPPOINTMENTACCESS_HPP__
#define _OUTLOOKACCESS_OLAPPOINTMENTACCESS_HPP__
#include "IOutlookAccess.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include "..\..\Components\PIMDB\VCalCtrl.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"

namespace QSYNC_LIB
{
	class CAppointmentLocation;

	class COLCalAccess : public COutlookAccess
	{
	public:
		COLCalAccess(void);
		virtual ~COLCalAccess(void);
		virtual bool New(IPIMObjPt PimObjPt);
		virtual bool Modify(IPIMObjPt PimObjPt);
		virtual bool Delete(IPIMObjPt PimObjPt);
		virtual bool StartToFetch();
		virtual bool Abort();
		virtual bool GetByIndex(int nIndex, IPIMObjPt& PIMObjPt);
		virtual int GetCount();
	private:
		bool				m_bAbort;
		bool				m_bFinish;
		bool				m_bUseRepeat;
		CCriticalSection	m_CritSection;
		vector<IPIMObjPt>	m_pOLCalList;
		CFolderLocation		m_FolderLocation;
	private:
		bool SearchInList(CAppointmentLocation* pCalLocation, int& nIndex);

		bool GetDefaultCalendar();
		bool GetDefaultCalendarByFilter(
			int nStartAfterYear,int nStartAfterMonth, int nStartAfterDate,
			int nStartBeforeYear,int nStartBeforeMonth, int nStartBeforeDate);
		bool SaveAppointmentToList(CAppointmentLocation* pLocation, Outlook::_AppointmentItemPtr pAppointment);
		bool GetOLAppointment(IPIMObjPt PIMObjPt, Outlook::_AppointmentItemPtr pAppointment);
		bool SetOLAppointment(IPIMObjPt PIMObjPt, Outlook::_AppointmentItemPtr pAppointment);

		bool GetDateTime(CVCalCtrl* pVCalCtrl, const char* cpKey, COleDateTime& oleLocalDateTime);
		TSTRING TransferDateTime(const DATE& date);
		TSTRING TransferDateTime(const DATE& date, const DATE& time);
		bool CheckDTEnd(DATE& EndDT);
		bool GetAlarm(Outlook::_AppointmentItemPtr pAppointment, TSTRING& strAlarm , const DATE& StartDate, long nAlarm);
		long SetAlarm(CVCalCtrl* pVCalCtrl, const COleDateTime& oleLocalStart);

		bool SetOLRecurrence(IPIMObjPt PIMObjPt,Outlook::_AppointmentItemPtr pAppointment);
		bool GetOLRecurrence(IPIMObjPt PIMObjPt,Outlook::RecurrencePatternPtr pRecurrence);
		bool GetPrefix(const TSTRING& strTitle, Outlook::RecurrencePatternPtr pRecurrencePattern,TSTRING& strPrefix);
		bool GetDayOfWeekMask(Outlook::RecurrencePatternPtr pRecurrencePattern, TSTRING& strWeek);
		bool GetMonthly( Outlook::RecurrencePatternPtr pRecurrencePattern, Outlook::OlRecurrenceType nRecurrenceTypeEnum, TSTRING& strMonthly);
		bool GetYearly( Outlook::RecurrencePatternPtr pRecurrencePattern, Outlook::OlRecurrenceType nRecurrenceTypeEnum, TSTRING& strYearly);
		bool GetRepeat( Outlook::RecurrencePatternPtr pRecurrencePattern, TSTRING& strRepeat );

		bool GetException(CVCalCtrl VCalCtrl, Outlook::RecurrencePatternPtr pRecurrencePattern);
		bool GetDate(LCID lcid, int nYear, int nMonth, int nDay, LPCTSTR lpFormat, CString& strDate, DWORD dwflags = DATE_SHORTDATE);
		bool GetTime(LCID lcid, int nHour, int nMinute, int nSecond, LPCTSTR lpFormat, CString& strTime, DWORD dwflags = TIME_NOSECONDS);

		//取得時間範圍(以Outlook::_ItemsPtr可用來搜尋的格式)
		_bstr_t CreateDateTimeRange(BSTR bstrOLStartAfterDatetime,BSTR bstrOLStartBeforeDatetime,BSTR bstrOLEndAfterDatetime,BSTR bstrOLEndBeforeDatetime);

	};

	class CAppointmentLocation
	{
	public:
		CAppointmentLocation(IPIMObjPt PIMObjPt)
		{
			CommonDataPt sptemData = new CommonData;
			if((size_t)(-1) != PIMObjPt->GetByName(OUTLOOK_CALENTRYID, sptemData))
				m_bstrAppointmentEntryID = sptemData->GetWString();
			if((size_t)(-1) != PIMObjPt->GetByName(OUTLOOK_FOLDERENTRYID, sptemData))
				m_bstrFolderStoreID = sptemData->GetWString();
		}
		CAppointmentLocation(_bstr_t bstrFolderStoreID, _bstr_t bstrAppointmentEntryID):
		m_bstrFolderStoreID(bstrFolderStoreID),m_bstrAppointmentEntryID(bstrAppointmentEntryID){}
		CAppointmentLocation(Outlook::MAPIFolderPtr pFolder,Outlook::_AppointmentItemPtr pAppointment)
		{
			pFolder->get_StoreID(m_bstrFolderStoreID.GetAddress());
			pAppointment->get_EntryID(m_bstrAppointmentEntryID.GetAddress());
		}
		virtual ~CAppointmentLocation(){}

		CAppointmentLocation(CAppointmentLocation* pAppointmentLoc)
		{
			m_bstrFolderStoreID = pAppointmentLoc->GetFolderStoreID();
			m_bstrAppointmentEntryID = pAppointmentLoc->GetAppointmentEntryID();
		}
		bool IsTheSame(CAppointmentLocation* pAppointmentLoc) const
		{
			return (m_bstrFolderStoreID == pAppointmentLoc->GetFolderStoreID())&&(m_bstrAppointmentEntryID == pAppointmentLoc->GetAppointmentEntryID());
		}

		bool SetFolderStoreID(Outlook::MAPIFolderPtr pFolder)
		{
			return (S_OK == pFolder->get_StoreID(m_bstrFolderStoreID.GetAddress()));
		}
		bool SetAppointmentEntryID(Outlook::_ContactItemPtr pContact)
		{
			return (S_OK == pContact->get_EntryID(m_bstrAppointmentEntryID.GetAddress()));
		}
		_bstr_t GetFolderStoreID(){return m_bstrFolderStoreID;}
		_bstr_t GetAppointmentEntryID(){return m_bstrAppointmentEntryID;}
		bool IsEmpty(){return ((m_bstrFolderStoreID == _bstr_t(""))||(m_bstrAppointmentEntryID == _bstr_t(""))); }
	private:
		_bstr_t m_bstrFolderStoreID;
		_bstr_t m_bstrAppointmentEntryID;
	};


	// Parse RRULE string to useful information for setting OL.
	class CRRULEParser
	{
	public:
		enum RecurrenceType
		{
			Day,
			Week,
			Month,
			MonthNth,
			YearNth,
			Year
		};
		RecurrenceType m_Type;
		long m_nOccurence;
		long m_nInstance;
		long m_nInterval;
		COleDateTime m_EndDateTime;
		int m_nDayOfWeek;
		long m_nDayOfMonth;
		long m_nMonthOfYear;
		bool m_bRepeat;
	public:
		CRRULEParser(const TSTRING& strRRULE);
		~CRRULEParser(){}
	private:
		bool SetType(const TSTRING& strPrefix);
		bool SetRepeat(const TSTRING& strOcurrence);
		bool SetDayOfWeek(vector<TSTRING>& vToken);
		bool SetMonthOrDay(vector<TSTRING>& vToken, long& nSet);
		bool SetDayofYear(vector<TSTRING>& vToken);
	};

}//namespace QSYNC_LIB


#endif //OLAPPOINTMENTACCESS