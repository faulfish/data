#include "StdAfx.h"
#include "OLCalAccess.h"

#define DATE_SHORTDATE            0x00000001  // use short date picture
#define DATE_LONGDATE             0x00000002  // use long date picture
#define DATE_USE_ALT_CALENDAR     0x00000004  // use alternate calendar (if any)

namespace QSYNC_LIB
{

COLCalAccess::COLCalAccess(void):
 m_bAbort(false)
,m_bFinish(false)
,m_bUseRepeat(true)
{
	m_pOLCalList.clear();
}

COLCalAccess::~COLCalAccess(void)
{
	if(!m_pOLCalList.empty())
	{
		m_pOLCalList.clear();
	}
}

bool COLCalAccess::New(IPIMObjPt PIMObjPt)
{
	bool bRes = false;
	if(!m_FolderLocation.IsEmpty() && PIMObjPt)
	{
		Outlook::MAPIFolderPtr pFolder = NULL;
		VARIANT varFolderStoreID;
		VariantInit(&varFolderStoreID);
		varFolderStoreID.vt = VT_BSTR;
		varFolderStoreID.bstrVal = m_FolderLocation.GetFolerStoreID();
		if(SUCCEEDED(m_pNameSpace->GetFolderFromID(m_FolderLocation.GetFolderEntryID(), varFolderStoreID, &pFolder)))
		{
			Outlook::_ItemsPtr pItems = NULL;
			if(SUCCEEDED(pFolder->get_Items(&pItems)))
			{
				VARIANT varType;
				VariantInit(&varType);
				varType.vt = VT_BSTR;
				varType.bstrVal = _bstr_t("IPM.Appointment");
				LPDISPATCH lpDispatch = NULL;
				if(SUCCEEDED(pItems->Add(varType, &lpDispatch)))
				{
					Outlook::_AppointmentItemPtr pAppointment = NULL;
					lpDispatch->QueryInterface(Outlook::IID__AppointmentItem, (void**)&pAppointment);
					SetOLAppointment(PIMObjPt, pAppointment);
					SetOLRecurrence(PIMObjPt, pAppointment);
					bRes = SUCCEEDED(pAppointment->Save());
					if(bRes)
					{
						CAppointmentLocation CalLocation(pFolder,pAppointment);
						SetWStringData(PIMObjPt, OUTLOOK_CONTACTENTRYID, CalLocation.GetAppointmentEntryID());
						SetWStringData(PIMObjPt, OUTLOOK_FOLDERSTOREID, CalLocation.GetFolderStoreID());
						CSingleLock singleLock(&m_CritSection,TRUE);
						m_pOLCalList.push_back(PIMObjPt);
					}
					pAppointment->Release();
				}
				lpDispatch = NULL;
			}
		}
	}
	return bRes;
}
bool COLCalAccess::Modify(IPIMObjPt PIMObjPt)
{
	CSingleLock singleLock(&m_CritSection,TRUE);
	bool bRes = false;
	if(PIMObjPt)
	{
		CAppointmentLocation CalLocation(PIMObjPt);
		if(!CalLocation.IsEmpty())
		{
			Outlook::MAPIFolderPtr pFolder = NULL;
			VARIANT varFolderStoreID;
			VariantInit(&varFolderStoreID);
			varFolderStoreID.vt = VT_BSTR;
			varFolderStoreID.bstrVal = CalLocation.GetFolderStoreID();
			LPDISPATCH lpDispatch;
			if(SUCCEEDED(m_pNameSpace->GetItemFromID(CalLocation.GetAppointmentEntryID(), varFolderStoreID, &lpDispatch)))
			{
				Outlook::_AppointmentItemPtr pAppointment = NULL;
				lpDispatch->QueryInterface(Outlook::IID__AppointmentItem, (void**)&pAppointment);
				SetOLAppointment(PIMObjPt, pAppointment);
				SetOLRecurrence(PIMObjPt, pAppointment);
				bRes = SUCCEEDED(pAppointment->Save());
			}
		}
	}
	return bRes;
}
bool COLCalAccess::Delete(IPIMObjPt PIMObjPt)
{
	bool bRes = false;
	if(PIMObjPt)
	{
		CAppointmentLocation CalLocation(PIMObjPt);
		if(!CalLocation.IsEmpty())
		{
			VARIANT varFolderStoreID;
			varFolderStoreID.vt = VT_BSTR;
			varFolderStoreID.bstrVal = CalLocation.GetFolderStoreID();

			LPDISPATCH lpDispatch = NULL;
			if(SUCCEEDED(m_pNameSpace->GetItemFromID(CalLocation.GetAppointmentEntryID(), varFolderStoreID, &lpDispatch)))
			{
				Outlook::_AppointmentItemPtr pAppointment = NULL;
				lpDispatch->QueryInterface(Outlook::IID__AppointmentItem, (void**)&pAppointment);
				bRes = SUCCEEDED(pAppointment->Delete()) == TRUE;
				assert(bRes);
				pAppointment->Release();

				int nIndex = 0;
				bool bSearch = false;
				CSingleLock singleLock(&m_CritSection,TRUE);
				if(SearchInList(&CalLocation, nIndex) && bRes)
				{
					IterPIMObj iter = m_pOLCalList.begin();
					advance(iter, nIndex);
					m_pOLCalList.erase(iter);
					bSearch = true;
				}
				bRes = bSearch;
			}
			lpDispatch = NULL;
		}
	}
	return bRes;
}
bool COLCalAccess::SearchInList(CAppointmentLocation* pCalLocation, int& nIndex)
{
	bool bRes = false;
	nIndex = -1;
	for(IterPIMObj iter = m_pOLCalList.begin(); iter != m_pOLCalList.end() && !bRes; iter++)
	{
		nIndex++;
		CAppointmentLocation PIMLocation((IPIMObjPt)(*iter));
		if(PIMLocation.IsTheSame(pCalLocation))
		{
			bRes = true;
			break;
		}
	}
	return bRes;
}

bool COLCalAccess::StartToFetch()
{
	bool bRes = false;
	m_bAbort = false;
	m_bFinish = false;

	IConfigure* CfgToolSyncOL = CreateIConfigure(_T("SyncOutlookSetting"));    //建立ConfigTool,以便從檔案讀取資料
	if(CfgToolSyncOL)
	{
		CCfgInt bCfgSyncWay = CCfgInt(CfgToolSyncOL,_T("SyncWay"),0);
		CCfgInt bCfgShowAll = CCfgInt(CfgToolSyncOL,_T("ShowAll"),1);						//是否寫入所有資料
		CCfgInt nCfgStartAfterYear = CCfgInt(CfgToolSyncOL,_T("StartAfterYear"),1);		
		CCfgInt nCfgStartAfterMonth = CCfgInt(CfgToolSyncOL,_T("StartAfterMonth"),1);
		CCfgInt nCfgStartAfterDate = CCfgInt(CfgToolSyncOL,_T("StartAfterDate"),1);
		CCfgInt nCfgStartBeforeYear = CCfgInt(CfgToolSyncOL,_T("StartBeforeYear"),3000);
		CCfgInt nCfgStartBeforeMonth = CCfgInt(CfgToolSyncOL,_T("StartBeforeMonth"),12);
		CCfgInt nCfgStartBeforeDate = CCfgInt(CfgToolSyncOL,_T("StartBeforeDate"),31);
		CfgToolSyncOL->Load(STORAGE_INIFILE);												//從檔案讀出資料
		if (bCfgSyncWay == 0 /* 0 = SyncToOL */)
			bRes = GetDefaultCalendar();
		else if (bCfgShowAll)
			bRes = GetDefaultCalendar();
		else		//若不寫入全部資料,則依據日期篩選出要寫入的資料
			bRes = GetDefaultCalendarByFilter(
			nCfgStartAfterYear,nCfgStartAfterMonth,nCfgStartAfterDate,
			nCfgStartBeforeYear,nCfgStartBeforeMonth,nCfgStartBeforeDate
			);
	}
	else
		bRes = GetDefaultCalendar();
	DeleteIConfigure(_T("SyncOutlookSetting"));     //用完ConfigTool後要記得刪除

	m_bFinish = bRes;
	return bRes;
}

bool COLCalAccess::GetDate(LCID lcid, int nYear, int nMonth, int nDay, LPCTSTR lpFormat, CString& strDate, DWORD dwflags)	
{
	bool bRes = true;
	SYSTEMTIME SystemTime;
	SystemTime.wYear = nYear;
	SystemTime.wMonth = nMonth;
	SystemTime.wDay = nDay;

	LPTSTR lpDate;
	int nLength = ::GetDateFormat(lcid, dwflags, &SystemTime, lpFormat, NULL, 0);
	if(nLength != 0)
	{
		lpDate = new TCHAR[nLength];
		::ZeroMemory(lpDate, nLength);
		::GetDateFormat(lcid, dwflags, &SystemTime, lpFormat, lpDate, nLength);
		strDate = lpDate;
		delete [] lpDate;
	}
	else
	{
		bRes = false;
	}
	return bRes;
}
bool COLCalAccess::GetTime(LCID lcid, int nHour, int nMinute, int nSecond, LPCTSTR lpFormat, CString& strTime, DWORD dwflags)
{
	bool bRes = true;
	SYSTEMTIME SystemTime;
	GetLocalTime(&SystemTime);
	SystemTime.wHour = nHour;
	SystemTime.wMinute = nMinute;
	SystemTime.wSecond = nSecond;

	int nLength = ::GetTimeFormat(lcid, dwflags, &SystemTime, lpFormat, NULL, 0);
	if(nLength != 0)
	{
		LPTSTR lpTime = new TCHAR[nLength];
		::ZeroMemory(lpTime, nLength);
		::GetTimeFormat(lcid, dwflags, &SystemTime, lpFormat, lpTime, nLength);
		strTime = lpTime;
		delete [] lpTime;
	}
	else
	{
		bRes = false;
	}
	return bRes;
}	

bool COLCalAccess::GetDefaultCalendarByFilter(int nStartAfterYear,int nStartAfterMonth, int nStartAfterDate,
											  int nStartBeforeYear,int nStartBeforeMonth, int nStartBeforeDate)
{
	bool bRes = true;
	LCID lcid = GetUserDefaultLCID();//get 區域語言

	//Create StartAfterDatetime
	BSTR bstrOLStartAfterDatetime;
	CString szOLStartAfterDatetime;
	CString szStartAfterTIME;
	GetDate(lcid, nStartAfterYear, nStartAfterMonth, nStartAfterDate, NULL, szOLStartAfterDatetime);
	GetTime(lcid, 0, 0, 0, NULL, szStartAfterTIME); 
	szOLStartAfterDatetime = szOLStartAfterDatetime + _T(" ") + szStartAfterTIME ;
	bstrOLStartAfterDatetime = szOLStartAfterDatetime.AllocSysString();
	//Create StartAfterDatetime end

	//Create StartBeforeDatetime
	BSTR bstrOLStartBeforeDatetime;
	CString szOLStartBeforeDatetime;
	CString szStartBeforeDatetime;
	GetDate(lcid, nStartBeforeYear, nStartBeforeMonth, nStartBeforeDate, NULL, szOLStartBeforeDatetime);
	GetTime(lcid, 23, 59, 59, NULL, szStartBeforeDatetime);  //注意時間
	szOLStartBeforeDatetime = szOLStartBeforeDatetime + _T(" ") + szStartBeforeDatetime ;
	bstrOLStartBeforeDatetime = szOLStartBeforeDatetime.AllocSysString();
	//Create StartBeforeDatetime end

	////Create EndAfterDatetime
	//BSTR bstrOLEndAfterDatetime;
	//CString szOLEndAfterDatetime;
	//CString szEndAfterDatetime;
	//GetDate(lcid, 2000, 1, 1, NULL, szOLEndAfterDatetime);
	//GetTime(lcid, 0, 0, 0, NULL, szEndAfterDatetime); 
	//szOLEndAfterDatetime = szOLEndAfterDatetime + _T(" ") + szEndAfterDatetime ;
	//bstrOLEndAfterDatetime = szOLEndAfterDatetime.AllocSysString();
	////Create EndAfterDatetime end

	////Create EndBeforeDatetime
	//BSTR bstrOLEndBeforeDatetime;
	//CString szOLEndBeforeDatetime;
	//CString szEndBeforeDatetime;
	//GetDate(lcid, 2999, 12, 31, NULL, szOLEndBeforeDatetime);
	//GetTime(lcid, 0, 0, 0, NULL, szEndBeforeDatetime); 
	//szOLEndBeforeDatetime = szOLEndBeforeDatetime + _T(" ") + szEndBeforeDatetime ;
	//bstrOLEndBeforeDatetime = szOLEndBeforeDatetime.AllocSysString();
	////Create EndBeforeDatetime end

	//儲存《"開始日期時間"與"結束日期時間"之邏輯運算結果》.
	//目前僅針對StartDate作篩選,以後若要增加EndDate的篩選機制,就比照前面mark掉的語法寫入後兩個字串參數
	_bstr_t bstrtDatetimeRange = CreateDateTimeRange(bstrOLStartAfterDatetime, bstrOLStartBeforeDatetime, L"", L"");

	Outlook::MAPIFolderPtr pFolder = NULL;
	if(SUCCEEDED(m_pNameSpace->GetDefaultFolder(Outlook::olFolderCalendar, &pFolder)))
	{
		Outlook::_ItemsPtr pItems = NULL;
		m_FolderLocation.Set(pFolder);

		pFolder->get_Items(&pItems);
		LPDISPATCH lpDispatch = NULL;
		long nItemsCount = 0;

		long nIndex = 0;
		HRESULT hr = pItems->Find(bstrtDatetimeRange, &lpDispatch);
		while(SUCCEEDED(hr) && lpDispatch && bRes)
		{
			++nIndex;
			Outlook::_AppointmentItemPtr pAppointment = NULL;
			lpDispatch->QueryInterface(Outlook::IID__AppointmentItem, (void**)&pAppointment);
			if(pAppointment)
			{					
				CAppointmentLocation AppointmentLocation(pFolder, pAppointment);
				bRes = SaveAppointmentToList(&AppointmentLocation, pAppointment);
				if(bRes)
					Fire(EVENT_aCAL_FETCH_SUCCEEDED, nIndex);
			}
			hr = pItems->FindNext(&lpDispatch);
		};
		if(m_bAbort && !m_pOLCalList.empty())
		{		
			CSingleLock singleLock(&m_CritSection,TRUE);
			m_pOLCalList.clear();
			bRes = false;
		}
	}
	return bRes;	
}

bool COLCalAccess::GetDefaultCalendar()
{
	bool bRes = false;
	Outlook::MAPIFolderPtr pFolder = NULL;
	if(SUCCEEDED(m_pNameSpace->GetDefaultFolder(Outlook::olFolderCalendar, &pFolder)))
	{
		Outlook::_ItemsPtr pItems = NULL;
		m_FolderLocation.Set(pFolder);
		pFolder->get_Items(&pItems);
		long nItemsCount = 0;
		pItems->get_Count(&nItemsCount);

		bRes = true;
		for(long nIndex = 1; nIndex < nItemsCount+1 && bRes && !m_bAbort; nIndex++)
		{
			LPDISPATCH lpDispatch = NULL;
			VARIANT varIndex;
			varIndex.vt = VT_I4;
			varIndex.lVal = nIndex;
			pItems->Item(varIndex, &lpDispatch);

			Outlook::_AppointmentItemPtr pAppointment = NULL;
			lpDispatch->QueryInterface(Outlook::IID__AppointmentItem, (void**)&pAppointment);
			if(pAppointment)
			{					
				CAppointmentLocation AppointmentLocation(pFolder, pAppointment);
				bRes = SaveAppointmentToList(&AppointmentLocation, pAppointment);
				if(bRes)
					Fire(EVENT_aCAL_FETCH_SUCCEEDED, nIndex);
			}
		}
		if(m_bAbort && !m_pOLCalList.empty())
		{		
			CSingleLock singleLock(&m_CritSection,TRUE);
			m_pOLCalList.clear();
			bRes = false;
		}
	}
	return bRes;	
}
_bstr_t COLCalAccess::CreateDateTimeRange(BSTR bstrStartAfterDatetime,BSTR bstrStartBeforeDatetime,BSTR bstrEndAfterDatetime,BSTR bstrEndBeforeDatetime)
{
	_bstr_t bstrtDatetimeRange = L"";
	if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime != "") )  //("開始日期時間的範圍" 和 "結束日期時間的範圍" 皆有指定.)
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime == "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime == "") && ((CString)bstrEndBeforeDatetime != "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime == "") && ((CString)bstrEndBeforeDatetime == "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime == "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime != "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime == "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime == "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime == "") && ((CString)bstrEndAfterDatetime == "") && ((CString)bstrEndBeforeDatetime != "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime != "") && ((CString)bstrStartBeforeDatetime == "") && ((CString)bstrEndAfterDatetime == "") && ((CString)bstrEndBeforeDatetime == "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] >= ") + _bstr_t("'") + _bstr_t(bstrStartAfterDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime == "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime != "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime == "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime == "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime == "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime == "") && ((CString)bstrEndBeforeDatetime != "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime == "") && ((CString)bstrStartBeforeDatetime != "") && ((CString)bstrEndAfterDatetime == "") && ((CString)bstrEndBeforeDatetime == "") )
	{
		bstrtDatetimeRange = _bstr_t("[Start] < ") + _bstr_t("'") + _bstr_t(bstrStartBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime == "") && ((CString)bstrStartBeforeDatetime == "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime != "") )
	{
		bstrtDatetimeRange = _bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'") \
			+ _bstr_t(" AND ") + \
			_bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime == "") && ((CString)bstrStartBeforeDatetime == "") && ((CString)bstrEndAfterDatetime != "") && ((CString)bstrEndBeforeDatetime == "") )
	{
		bstrtDatetimeRange = _bstr_t("[End] >= ") + _bstr_t("'") + _bstr_t(bstrEndAfterDatetime) + _bstr_t("'");
	}
	else if( ((CString)bstrStartAfterDatetime == "") && ((CString)bstrStartBeforeDatetime == "") && ((CString)bstrEndAfterDatetime == "") && ((CString)bstrEndBeforeDatetime != "") )
	{
		bstrtDatetimeRange = _bstr_t("[End] < ") + _bstr_t("'") + _bstr_t(bstrEndBeforeDatetime) + _bstr_t("'");
	}
	else;  //("開始日期時間的範圍" 和 "結束日期時間的範圍" 皆未指定.)
	return bstrtDatetimeRange;
}

bool COLCalAccess::SaveAppointmentToList(CAppointmentLocation* pLocation, Outlook::_AppointmentItemPtr pAppointment)
{
	bool bRes = false;
	IPIMObjPt PIMObjPt = m_ptPIMRecordFactory->CreatePIMObject();
	CVCalCtrl VCalCtrl(PIMObjPt);

	if(VCalCtrl.SetEmptyIPIMObjPt() && GetOLAppointment(PIMObjPt, pAppointment))
	{
		bool bGet = true;
		VARIANT_BOOL varbIsRecurring;
		if(SUCCEEDED(pAppointment->get_IsRecurring(&varbIsRecurring)) && varbIsRecurring == VARIANT_TRUE)
		{
			bGet = false;
			Outlook::RecurrencePatternPtr pRecurrencePattern = NULL;
			if(SUCCEEDED(pAppointment->GetRecurrencePattern( &pRecurrencePattern )))
				bGet = GetOLRecurrence(PIMObjPt, pRecurrencePattern);
		}
		if(bGet)
		{
			SetWStringData(PIMObjPt, OUTLOOK_CALENTRYID, pLocation->GetAppointmentEntryID());
			SetWStringData(PIMObjPt, OUTLOOK_FOLDERSTOREID, pLocation->GetFolderStoreID());
			CSingleLock singleLock(&m_CritSection,TRUE);
			m_pOLCalList.push_back(PIMObjPt);

			bRes = true;
		}
	}

	return bRes;
}
bool COLCalAccess::SetOLAppointment(IPIMObjPt PIMObjPt, Outlook::_AppointmentItemPtr pAppointment)
{
	bool bRes = true;
	CVCalCtrl VCalCtrl(PIMObjPt);

	pAppointment->put_Subject(	 (_bstr_t)VCalCtrl.GetWString(KEYWORD_SUBJECT).c_str());
	pAppointment->put_Body(		 (_bstr_t)VCalCtrl.GetWString(KEYWORD_DESCRIPTION).c_str());
	pAppointment->put_Location(	 (_bstr_t)VCalCtrl.GetWString(KEYWORD_LOCATION).c_str());
	pAppointment->put_Categories((_bstr_t)VCalCtrl.GetWString(KEYWORD_CATEGORIES).c_str());
	/*************************************************************************/
	COleDateTime oleLocalStart;
	GetDateTime(&VCalCtrl, KEYWORD_DTSTART, oleLocalStart);
	pAppointment->put_Start(oleLocalStart.m_dt);

	COleDateTime oleLocalEnd;
	GetDateTime(&VCalCtrl, KEYWORD_DTEND, oleLocalEnd);
	pAppointment->put_End(oleLocalEnd.m_dt);
	/*************************************************************************/
	pAppointment->put_ReminderMinutesBeforeStart(SetAlarm(&VCalCtrl, oleLocalStart));

	return bRes;
}
bool COLCalAccess::SetOLRecurrence(IPIMObjPt PIMObjPt,Outlook::_AppointmentItemPtr pAppointment)
{
	using namespace Outlook;
	bool bRes = true;

	CVCalCtrl VCalCtrl(PIMObjPt);
	CString strRecurrence;
	bool bGet = VCalCtrl.GetCmnStrData( KEYWORD_RECURRENCE, strRecurrence);
	if(bGet && !strRecurrence.IsEmpty())
	{
		OlRecurrenceType pRecurrenceType[] = {olRecursDaily, olRecursWeekly, olRecursMonthly,olRecursMonthNth,olRecursYearNth, olRecursYearly};
		CRRULEParser RRULE((TSTRING)strRecurrence);

		RecurrencePatternPtr pRecurrencePattern = NULL;
		pAppointment->GetRecurrencePattern(&pRecurrencePattern);
		if(RRULE.m_Type > -1 && RRULE.m_Type < 6)
			pRecurrencePattern->put_RecurrenceType(pRecurrenceType[RRULE.m_Type]);

		CVCalCtrl VCalCtrl(PIMObjPt);
		COleDateTime oleLocalStart;
		GetDateTime(&VCalCtrl, KEYWORD_DTSTART, oleLocalStart);
		COleDateTime StartDate( oleLocalStart.GetYear(), oleLocalStart.GetMonth(), oleLocalStart.GetDay(),0,0,0);
		pRecurrencePattern->put_PatternStartDate(StartDate.m_dt);
		COleDateTime StartTime(0,0,0,oleLocalStart.GetHour(), oleLocalStart.GetMinute(), oleLocalStart.GetSecond());
		pRecurrencePattern->put_StartTime(StartTime.m_dt);

		if(RRULE.m_Type == 5)
			RRULE.m_nDayOfMonth = oleLocalStart.GetDay();
		//if(RRULE.m_Type == 4)
		//	RRULE.m_nMonthOfYear = oleLocalStart.GetMonth();

		pRecurrencePattern->put_DayOfMonth(RRULE.m_nDayOfMonth);
		pRecurrencePattern->put_MonthOfYear(RRULE.m_nMonthOfYear);
		pRecurrencePattern->put_DayOfWeekMask((Outlook::OlDaysOfWeek)RRULE.m_nDayOfWeek);

		pRecurrencePattern->put_Interval(RRULE.m_nInterval);
		pRecurrencePattern->put_Instance(RRULE.m_nInstance);

		VARIANT_BOOL varbIsNoEndDate = RRULE.m_nOccurence == 0 ? VARIANT_TRUE: VARIANT_FALSE;
		pRecurrencePattern->put_NoEndDate(varbIsNoEndDate);	
		if(varbIsNoEndDate == VARIANT_FALSE)
		{
			if(RRULE.m_bRepeat)
				pRecurrencePattern->put_Occurrences(RRULE.m_nOccurence);
			else
			{
				COleDateTime EndDate( RRULE.m_EndDateTime.GetYear(), RRULE.m_EndDateTime.GetMonth(), RRULE.m_EndDateTime.GetDay(),0,0,0);
				pRecurrencePattern->put_PatternEndDate(EndDate.m_dt);
				COleDateTime EndTime(0,0,0,RRULE.m_EndDateTime.GetHour(), RRULE.m_EndDateTime.GetMinute(), RRULE.m_EndDateTime.GetSecond());
				pRecurrencePattern->put_EndTime(EndTime.m_dt);
			}

		}
	}
	return bRes;
}

bool COLCalAccess::GetDateTime(CVCalCtrl* pVCalCtrl, const char* cpKey, COleDateTime& oleLocalDateTime)
{
	bool bRes = false;
	CString strDateTime; 
	bRes = pVCalCtrl->GetCmnStrData(cpKey, strDateTime);
	COleDateTime oleDateTime;
	StrToTime(strDateTime, oleDateTime);
	ConvertTime(oleDateTime, oleLocalDateTime, true);
	return bRes;
}

long COLCalAccess::SetAlarm(CVCalCtrl* pVCalCtrl, const COleDateTime& oleLocalStart)
{
	long nRes = 0;

	CString strAlarm, strDTStart;
	pVCalCtrl->GetCmnStrData(KEYWORD_DALARM, strAlarm);
	pVCalCtrl->GetCmnStrData(KEYWORD_DTSTART, strDTStart);
	COleDateTime oleAlarm, oleDTStart;
	StrToTime(strAlarm, oleAlarm);
	StrToTime(strDTStart, oleDTStart);

	COleDateTimeSpan SpanTime = oleDTStart - oleAlarm;
	nRes = (long)SpanTime.GetTotalMinutes();
	return nRes;
}

bool COLCalAccess::GetOLAppointment(IPIMObjPt PIMObjPt, Outlook::_AppointmentItemPtr pAppointment)
{
	bool bRes = true;
	CVCalCtrl VCalCtrl(PIMObjPt);

	_bstr_t vAppointment[4];
	bool bSuccess[7]={true};
	bSuccess[0] = SUCCEEDED(pAppointment->get_Subject(	 vAppointment[0].GetAddress()));
	bSuccess[1] = SUCCEEDED(pAppointment->get_Body(		 vAppointment[1].GetAddress()));
	bSuccess[2] = SUCCEEDED(pAppointment->get_Location(	 vAppointment[2].GetAddress()));
	bSuccess[3] = SUCCEEDED(pAppointment->get_Categories(vAppointment[3].GetAddress()));
	DATE StartDate;
	bSuccess[4] = SUCCEEDED(pAppointment->get_Start(&StartDate));
	DATE EndDate;
	bSuccess[5] = SUCCEEDED(pAppointment->get_End(&EndDate));
	long nAlarm = 0;
	bSuccess[6] = SUCCEEDED(pAppointment->get_ReminderMinutesBeforeStart( &nAlarm ));

	for(int i =0; i<7 && bRes; i++)
	{	
		bRes = bRes && bSuccess[i];
	}

	if(bRes)
	{
		VCalCtrl.SetCmnWStrData( KEYWORD_SUBJECT,	 vAppointment[0]);
		VCalCtrl.SetCmnWStrData( KEYWORD_DESCRIPTION,vAppointment[1]);
		VCalCtrl.SetCmnWStrData( KEYWORD_LOCATION,	 vAppointment[2]);
		//if(vAppointment[3].length() !=0)
		VCalCtrl.SetCmnStrData( KEYWORD_CATEGORIES, vAppointment[3]);
		//else
		//	VCalCtrl.SetCmnStrData( KEYWORD_CATEGORIES, _T(""));
		/*************************************************************************/
		VCalCtrl.SetCmnStrData( KEYWORD_DTSTART, TransferDateTime(StartDate).c_str());
		CheckDTEnd(EndDate);
		VCalCtrl.SetCmnStrData( KEYWORD_DTEND, TransferDateTime(EndDate).c_str());
		/*************************************************************************/
		TSTRING strAlarm;
		bRes = GetAlarm(pAppointment, strAlarm, StartDate, nAlarm);
		VCalCtrl.SetCmnStrData( KEYWORD_DALARM, strAlarm.c_str() );
		/*************************************************************************/
	}

	return bRes;
}
bool COLCalAccess::CheckDTEnd(DATE& EndDT)
{
	bool bRes = true;
	COleDateTime OleEndDT(EndDT);
	if( 0 == OleEndDT.GetHour() && 0 == OleEndDT.GetMinute() && 0 == OleEndDT.GetSecond())
	{
		COleDateTimeSpan spanTime(0,0,1,0);
		OleEndDT = OleEndDT-spanTime;
		EndDT = OleEndDT.m_dt;
	}
	return bRes;
}

bool COLCalAccess::GetAlarm(Outlook::_AppointmentItemPtr pAppointment, TSTRING& strAlarm, const DATE& StartDate, long nAlarm)
{
	bool bRes = true;

	COleDateTime oleDateTime(StartDate);
	COleDateTime oleUTCDateTime;
	bRes = ConvertTime(oleDateTime, oleUTCDateTime, false);
	COleDateTimeSpan SpanTime(0, 0, (int)nAlarm, 0);
	COleDateTime oleAlarm = oleUTCDateTime - SpanTime;
	strAlarm = oleAlarm.Format(_T("%Y%m%dT%H%M%SZ"));

	return bRes;
}

TSTRING COLCalAccess::TransferDateTime(const DATE& date)
{
	COleDateTime oleDateTime(date);
	COleDateTime oleUTCDateTime;
	ConvertTime(oleDateTime, oleUTCDateTime, false);
	TSTRING strDateTime;
	if((oleUTCDateTime.GetStatus() != COleDateTime::null) && (oleUTCDateTime.GetStatus() != COleDateTime::invalid))
		strDateTime = oleUTCDateTime.Format(_T("%Y%m%dT%H%M%SZ"));
	return strDateTime;
}
TSTRING COLCalAccess::TransferDateTime(const DATE& date, const DATE& time)
{
	TSTRING strDateTime;

	COleDateTime oleDate(date);
	COleDateTime oleUTCDate;
	ConvertTime(oleDate, oleUTCDate, false);

	COleDateTime oleTime(time);
	COleDateTime oleUTCTime;
	ConvertTime(oleTime, oleUTCTime, false);

	TSTRING strDate, strTime;
	if((oleUTCDate.GetStatus() != COleDateTime::null) && (oleUTCDate.GetStatus() != COleDateTime::invalid))
		strDate = oleUTCDate.Format(_T("%Y%m%dT"));
	if((oleUTCTime.GetStatus() != COleDateTime::null) && (oleUTCTime.GetStatus() != COleDateTime::invalid))
		strTime = oleUTCTime.Format(_T("%H%M%SZ"));

	if(!strDate.empty() && !strTime.empty())
		strDateTime = strDate + strTime;

	return strDateTime;
}

bool COLCalAccess::GetOLRecurrence(IPIMObjPt PIMObjPt,Outlook::RecurrencePatternPtr pRecurrencePattern)
{
	using namespace Outlook;
	bool bRes = true;
	bool bSuccess[6] = {true};
	TSTRING strRule;
	CVCalCtrl VCalCtrl(PIMObjPt);

	DATE dtStartDate, dtStartTime, dtEndDate, dtEndTime;
	bSuccess[0] = SUCCEEDED(pRecurrencePattern->get_PatternStartDate(&dtStartDate));
	bSuccess[1] = SUCCEEDED(pRecurrencePattern->get_StartTime(&dtStartTime));
	bSuccess[2] = SUCCEEDED(pRecurrencePattern->get_PatternEndDate(&dtEndDate));
	bSuccess[3] = SUCCEEDED(pRecurrencePattern->get_EndTime(&dtEndTime));
	OlRecurrenceType nRecurrenceTypeEnum;
	bSuccess[4] = SUCCEEDED(pRecurrencePattern->get_RecurrenceType( &nRecurrenceTypeEnum ));

	for(int i = 0; i<5 && bRes; i++)
		bRes = bRes & bSuccess[i];

	if(bRes)
	{
		//VCalCtrl.SetCmnStrData( KEYWORD_DTSTART, TransferDateTime(dtStartDate, dtStartTime).c_str());
		//VCalCtrl.SetCmnStrData( KEYWORD_DTEND, TransferDateTime(dtEndDate, dtEndTime).c_str());
		TSTRING strType, strPrefix, strWeek, strMonthly, strYearly;
		if(nRecurrenceTypeEnum == olRecursDaily && GetPrefix(_T("D"),pRecurrencePattern, strPrefix))
			strType = strPrefix;
		else if(nRecurrenceTypeEnum == olRecursWeekly && GetPrefix(_T("W"), pRecurrencePattern, strPrefix) && GetDayOfWeekMask(pRecurrencePattern, strWeek))
			strType = strPrefix  + strWeek;	
		else if( (nRecurrenceTypeEnum == olRecursMonthly || nRecurrenceTypeEnum == olRecursMonthNth) && GetMonthly(pRecurrencePattern,nRecurrenceTypeEnum, strMonthly))
			strType = strMonthly;
		else if( (nRecurrenceTypeEnum == olRecursYearly || nRecurrenceTypeEnum == olRecursYearNth) && GetYearly(pRecurrencePattern, nRecurrenceTypeEnum, strYearly))
			strType = strYearly;
		else 
			bRes = false;
		if(bRes)
		{
			TSTRING strRepeat;
			GetRepeat(pRecurrencePattern, strRepeat);
			strType = strType + _T("  ") + strRepeat;
			VCalCtrl.SetCmnStrData( KEYWORD_RECURRENCE, strType.c_str());
			bRes = GetException(VCalCtrl, pRecurrencePattern);
		}
	}
	return bRes;
}
bool COLCalAccess::GetRepeat( Outlook::RecurrencePatternPtr pRecurrencePattern, TSTRING& strRepeat )
{
	bool bRes = true;
	if(m_bUseRepeat)
	{
		strRepeat = _T("#");
		long nOccurence = 0; //重複次數
		bRes = SUCCEEDED(pRecurrencePattern->get_Occurrences( &nOccurence ));

		VARIANT_BOOL varbIsNoEndDate; // true的話 occurrence = 0 -> forever
		bRes = bRes && SUCCEEDED(pRecurrencePattern->get_NoEndDate( &varbIsNoEndDate ));
		if(bRes)
		{
			nOccurence = varbIsNoEndDate == VARIANT_TRUE ? 0 : nOccurence;
			TSTRING strOcuurence;
			iToTStr<TSTRING>((int)nOccurence, strOcuurence);
			strRepeat += strOcuurence;
		}
	}
	else
	{
		DATE dtEndDate, dtEndTime;
		bool bEndDate = SUCCEEDED(pRecurrencePattern->get_PatternEndDate(&dtEndDate));
		bool bEndTime = SUCCEEDED(pRecurrencePattern->get_EndTime(&dtEndTime));
		strRepeat = TransferDateTime(dtEndDate,dtEndTime);
		bRes = bEndDate && bEndTime;
		m_bUseRepeat = true; // Reset
	}

	return bRes;
}

bool COLCalAccess::GetPrefix(const TSTRING& strTitle, Outlook::RecurrencePatternPtr pRecurrencePattern, TSTRING& strPrefix)
{
	bool bRes = true;
	strPrefix = strTitle;

	long nInterval;
	bRes = SUCCEEDED(pRecurrencePattern->get_Interval( &nInterval ));
	if(bRes)
	{
		TSTRING strInterval;
		if(nInterval < 1)
		{
			nInterval = 1;
			m_bUseRepeat = false;
		}
		iToTStr<TSTRING>((int)nInterval, strInterval);
		strPrefix += strInterval;
	}
	return bRes;
}

bool COLCalAccess::GetDayOfWeekMask(Outlook::RecurrencePatternPtr pRecurrencePattern, TSTRING& strWeek)
{
	bool bRes = true;
	Outlook::OlDaysOfWeek nDayOfWeekMaskEnum;	//(get <DayOfWeekMask>)
	bRes = SUCCEEDED(pRecurrencePattern->get_DayOfWeekMask( &nDayOfWeekMaskEnum ));

	if(nDayOfWeekMaskEnum != 0 && bRes)
	{
		char cpWeek[] = {1,2,4,8,16,32,64};
		for(int i = 0; i<7; i++)
			if(cpWeek[i] & nDayOfWeekMaskEnum)
				strWeek += _T(" ") + RRULE_WEEK[i];
	}			
	return bRes;
}
bool COLCalAccess::GetMonthly( Outlook::RecurrencePatternPtr pRecurrencePattern, Outlook::OlRecurrenceType nRecurrenceTypeEnum, TSTRING& strMonthly)
{
	bool bRes = false;
	if(nRecurrenceTypeEnum == Outlook::olRecursMonthly && GetPrefix( _T("MD"), pRecurrencePattern, strMonthly)) // MDn Day
	{
		long nDayOfMonth;
		bRes = SUCCEEDED(pRecurrencePattern->get_DayOfMonth( &nDayOfMonth )); 
		if(bRes)
		{
			TSTRING strDayOfMonth;
			iToTStr<TSTRING>((int)nDayOfMonth, strDayOfMonth);
			strMonthly += _T(" ") + strDayOfMonth;
		}
	}
	else if(nRecurrenceTypeEnum == Outlook::olRecursMonthNth && GetPrefix( _T("MP"), pRecurrencePattern, strMonthly)) // MPn i+ Week
	{
		long nInstance;
		bRes = SUCCEEDED(pRecurrencePattern->get_Instance( & nInstance));
		if(bRes)
		{
			TSTRING strInstance;
			iToTStr<TSTRING>((int)nInstance, strInstance);
			TSTRING strWeek;
			if(nInstance < 5 && GetDayOfWeekMask(pRecurrencePattern, strWeek))
				strMonthly += _T(" ") + strInstance + _T("+") + strWeek;
			else if(nInstance == 5 && GetDayOfWeekMask(pRecurrencePattern, strWeek))
				strMonthly += _T(" 1-") + strWeek;
			else
				bRes = false;
		}
	}
	return bRes;
}
bool COLCalAccess::GetYearly(Outlook::RecurrencePatternPtr pRecurrencePattern, Outlook::OlRecurrenceType nRecurrenceTypeEnum, TSTRING& strYearly)
{
	using namespace Outlook;
	bool bRes = false;
	if((nRecurrenceTypeEnum == olRecursYearly /*|| nRecurrenceTypeEnum == olRecursYearNth*/) /*&& GetPrefix( _T("YM"), pRecurrencePattern , strYearly)*/)
	{
		long nMonthOfYear;
		bRes = SUCCEEDED(pRecurrencePattern->get_MonthOfYear( &nMonthOfYear )); 
		if(bRes)
		{
			strYearly = _T("YM1");
			TSTRING strMonthOfYear;
			iToTStr<TSTRING>((int)nMonthOfYear, strMonthOfYear);
			strYearly += _T(" ") + strMonthOfYear;
		}
	}
	//if(nRecurrenceTypeEnum == olRecursYearly /*&& GetPrefix( _T("YD"), pRecurrencePattern , strYearly)*/)
	/*{
	strYearly = _T("YD1");
	long nMonthOfYear, nDayOfMonth;
	bRes = SUCCEEDED(pRecurrencePattern->get_MonthOfYear( &nMonthOfYear ));
	bRes = SUCCEEDED(pRecurrencePattern->get_DayOfMonth( &nDayOfMonth )); 
	if(bRes)
	{
	TSTRING strTotalDay;
	COleDateTime ole0101, oleDate;
	oleDate.SetDate(2000,nMonthOfYear, nDayOfMonth);
	ole0101.SetDate(2000,1,1);
	COleDateTimeSpan spanTime = oleDate - ole0101;
	int nTotalDay = spanTime.GetDays()+1;
	iToTStr<TSTRING>(nTotalDay, strTotalDay);
	strYearly += _T(" ") + strTotalDay;
	}


	}*/
	else if(nRecurrenceTypeEnum == olRecursYearNth)
	{	
		bRes = GetMonthly(pRecurrencePattern, Outlook::olRecursMonthNth, strYearly);
	}

	return bRes;
}

bool COLCalAccess::GetException(CVCalCtrl VCalCtrl, Outlook::RecurrencePatternPtr pRecurrencePattern)
{
	bool bRes = true;
	Outlook::ExceptionsPtr pExceptions = NULL;
	Outlook::ExceptionPtr pException = NULL;
	bRes = SUCCEEDED(pRecurrencePattern->get_Exceptions( &pExceptions ));

	long nExceptionCount;
	if(bRes)
		bRes = SUCCEEDED(pExceptions->get_Count( &nExceptionCount ));
	vector<TSTRING> vExceptionDate;
	for(long i=0; i < nExceptionCount && bRes; i++)
	{
		VARIANT varIndex;
		VariantInit(&varIndex);
		varIndex.vt = VT_I4;
		varIndex.lVal = (long)i;
		pExceptions->Item(varIndex, &pException);

		if(pException)
		{
			VARIANT_BOOL varbIsDeleted;
			bRes = SUCCEEDED(pException->get_Deleted( &varbIsDeleted ));
			if(varbIsDeleted == VARIANT_TRUE)
			{
				DATE dtOriginalDate;
				bRes = SUCCEEDED(pException->get_OriginalDate( &dtOriginalDate ));
				vExceptionDate.push_back(TransferDateTime(dtOriginalDate));
			}
		}
	}
	if(	!vExceptionDate.empty()&& bRes)
	{
		TSTRING strExdate = StrConnect<TSTRING>(_T(";"), vExceptionDate, false);
		VCalCtrl.SetCmnStrData(KEYWORD_EXDATE, strExdate.c_str());
	}
	return bRes;
}

bool COLCalAccess::GetByIndex(int nIndex, IPIMObjPt& PIMObjPt)
{
	bool bRes = false;
	assert(m_bFinish);
	if(m_bFinish)
	{
		if(m_pOLCalList.size() > (UINT)0 && m_pOLCalList.size() > (UINT)nIndex && nIndex > -1)
		{
			PIMObjPt = m_pOLCalList[nIndex];
			bRes = true;
		}
	}
	return bRes;
}
int COLCalAccess::GetCount()
{
	int nCount = (int)m_pOLCalList.size();
	return nCount;
}

bool COLCalAccess::Abort()
{	
	m_bAbort = true;
	return true;
}

CRRULEParser::CRRULEParser(const TSTRING& strRRULE):
m_nOccurence(0),m_nInstance(0),m_nInterval(0),m_nDayOfWeek(0),m_nDayOfMonth(0),m_nMonthOfYear(0),m_bRepeat(true)
{
	vector<TSTRING> vToken;
	Tokenize<TSTRING>(_T(" "), (TSTRING)strRRULE, vToken, false);
	if( vToken.size() >0 && SetType(vToken[0]) )
		vToken.erase(vToken.begin());
	if( vToken.size() > 0 && SetRepeat(vToken[vToken.size()-1]))
		vToken.erase(vToken.end() -1 );
	if(vToken.size() > 0)
	{
		if(m_Type == Week || m_Type == MonthNth)
			SetDayOfWeek(vToken);
		if(m_Type == Month)
			SetMonthOrDay(vToken, m_nDayOfMonth);
		if(m_Type ==Year)
			SetMonthOrDay(vToken, m_nDayOfMonth);
		//SetDayofYear(vToken);
	}
}

bool CRRULEParser::SetType(const TSTRING& strPrefix)
{
	bool bRes = true;
	TSTRING strType = strPrefix;
	switch (strType[0])
	{
	case _T('D'):
		m_Type = Day;
		strType.erase(0,1);
		break;
	case _T('W'):
		m_Type = Week;
		strType.erase(0,1);
		break;
	case _T('M'):
		if(strType[1] == _T('P'))
			m_Type = MonthNth;
		else if(strType[1] == _T('D'))
			m_Type = Month;
		strType.erase(0,2);
		break;
	case _T('Y'):
		if(strType[1] == _T('M'))
			m_Type = Year;
		else 
			bRes = false;
		strType.erase(0,2);
		break;
	default:
		bRes = false;
	}
	m_nInterval = _tstol(strType.c_str());
	return bRes;
}
bool CRRULEParser::SetRepeat(const TSTRING& strOcurrence)
{
	bool bRes = false;
	TSTRING strRepeat = strOcurrence;
	if(strRepeat[0]==_T('#'))
	{
		strRepeat.erase(0,1);
		m_nOccurence = _tstol(strRepeat.c_str());
		m_bRepeat = true;
		bRes = true;
	}
	else
	{
		bRes = StrToTime(strRepeat.c_str(), m_EndDateTime);
		m_bRepeat = false;
	}
	return bRes;
}


bool CRRULEParser::SetDayOfWeek(vector<TSTRING>& vToken)
{
	bool bRes = true;
	bool bDayOfWeek = false;
	int npWeek[] = {1,2,4,8,16,32,64};
	m_nDayOfWeek = 0;
	for(vector<TSTRING>::iterator iter = vToken.begin(); iter!=vToken.end(); iter++)
	{
		for(int i=0; i<7; i++)
		{
			if(RRULE_WEEK[i] == (*iter))
			{
				m_nDayOfWeek = m_nDayOfWeek | npWeek[i];
				bDayOfWeek = true;
			}
		}
		if(!bDayOfWeek)
		{
			if((*iter)[(*iter).size()-1] == _T('+'))
			{
				(*iter).erase((*iter).end());
				m_nInstance = _tstol((*iter).c_str());
			}
			else if((*iter)[(*iter).size()-1] == _T('-'))
			{
				m_nInstance = 5;
			}
			else
			{
				bRes = false;
				assert(bRes);
			}
		}
		bDayOfWeek = false;
	}
	/*if(m_nInterval == 12) // for transfering 12 months to 1 year.
	{
	m_Type = YearNth;
	m_nInterval = 1;
	}*/
	return bRes;
}

bool CRRULEParser::SetMonthOrDay(vector<TSTRING>& vToken, long& nSet)
{
	bool bRes = false;
	if( vToken.size() > 0 )
		nSet =  _tstol(vToken[0].c_str());
	return bRes;
}
bool CRRULEParser::SetDayofYear(vector<TSTRING>& vToken)
{
	bool bRes = false;
	long nDay =0;
	if( vToken.size() > 0 )
		nDay =  _tstol(vToken[0].c_str());
	COleDateTime oleDate(2000,1,1,0,0,0);
	COleDateTimeSpan TimeSpan(nDay-1,0,0,0);
	oleDate += TimeSpan;
	m_nMonthOfYear = oleDate.GetMonth();
	m_nDayOfMonth = oleDate.GetDay();
	m_nInterval = 12;

	return bRes;
}

}//namespace QSYNC_LIB
