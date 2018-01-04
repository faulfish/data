#include "stdafx.h"
#include "QDataCtrl.h"
#include "..\..\Components\OutlookAccess\OutlookDevice.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB 
{
	void CQPObjListData::GarbageCollection(CBasicData* pBasicData)
	{
		if(m_vpGarbageBD.size() > 100)
		{
			for(vector<CBasicData*>::iterator iter = m_vpGarbageBD.begin(); iter != m_vpGarbageBD.end(); iter++)
			{
				if((*iter))
				{
					delete (*iter);
					*iter = NULL;
				}
			}
			m_vpGarbageBD.clear();
		}
		m_vpGarbageBD.push_back(pBasicData);
	}

	CBasicData* CQPObjListData::GetValueByKey(const TSTRING& strKey)
	{
		size_t npos = -1;
		CBasicData* pBasicData = new CBasicData;
		GarbageCollection(pBasicData);
		CommonDataPt spData, spDTStart = NULL;
		size_t nGet = m_PIMObjPt->GetByName(UnicodetoASCII(strKey.c_str()), spData);
		if(nGet != npos)
		{
			pBasicData->m_strText = GetCommonStrData(spData);
			if(strKey == _T(KEYWORD_DTSTART) || strKey == _T(KEYWORD_DTEND))
			{
				COleDateTime oleUTCTime, oleLocalTime;
				StrToTime(pBasicData->m_strText.c_str(), oleUTCTime);
				ConvertTime(oleUTCTime, oleLocalTime, true);
				pBasicData->m_strText = oleLocalTime.Format(_T("%Y/%m/%d %p %I:%M:%S"));
			}	
			else if(strKey == _T(KEYWORD_DALARM))
			{
				if(npos != m_PIMObjPt->GetByName(KEYWORD_DTSTART,spDTStart))
					pBasicData->m_strText = GetAlarm(MinusTime(GetCommonStrData(spDTStart), pBasicData->m_strText.c_str()));
			}
			else if(strKey == _T(KEYWORD_RECURRENCE))
			{
				CString strTemp = pBasicData->m_strText.c_str();
				GetRule(strTemp);
				pBasicData->m_strText = LPCTSTR(strTemp);
			}
			else if(strKey == _T(KEYWORD_CATEGORIES))
			{
				CString strTemp = pBasicData->m_strText.c_str();
				GetCategories(strTemp, true);
				pBasicData->m_strText = LPCTSTR(strTemp);
			}
		}
		else
		{
			pBasicData->m_strText = GetString(_T("IDS_None"));
		}
		return pBasicData;
	}
	CString CQPObjListData::GetAlarm(int nInterval)
	{	
		CString strRet = _T("");
		switch (nInterval)
		{
		case 0:
			strRet = GetString(_T("IDS_AlarmNone"));
			break;
		case 1:
			strRet = GetString(_T("IDS_AlarmOneMin"));
			break;
		case 2:
			strRet = GetString(_T("IDS_AlarmFiveMin"));
			break;
		case 3:
			strRet = GetString(_T("IDS_AlarmTenMin"));
			break;
		case 4:
			strRet = GetString(_T("IDS_AlarmFifteenMin"));
			break;
		case 5:
			strRet = GetString(_T("IDS_AlarmThirtyMin"));
			break;
		case 6:
			strRet = GetString(_T("IDS_AlarmOneHour"));
			break;
		case 7:
			strRet = GetString(_T("IDS_AlarmOneDay"));
			break;
		default:
			strRet = GetString(_T("IDS_AlarmOther"));
		}
		return strRet;
	}
	int CQPObjListData::GetCategories(CString& strCate, bool bGetKeyword)
	{
		CString arrayStrCate[14][2] = {{_T(""), GetString(_T("IDS_None"))},
		{CATEGORIES_APPOINTMENT, GetString(_T("IDS_CateAppointment"))},
		{CATEGORIES_BUSINESS,    GetString(_T("IDS_CateBusiness"))},
		{CATEGORIES_EDUCATION,   GetString(_T("IDS_CateEducation"))},
		{CATEGORIES_HOLIDAY,     GetString(_T("IDS_CateHoliday"))},
		{CATEGORIES_MEETING,     GetString(_T("IDS_CateMeeting"))},
		{CATEGORIES_MISC,        GetString(_T("IDS_CateMisc"))},
		{CATEGORIES_PERSONAL,    GetString(_T("IDS_CatePersonal"))},
		{CATEGORIES_PHONE,       GetString(_T("IDS_CatePhone"))},
		{CATEGORIES_SICKDAY,     GetString(_T("IDS_CateSickday"))},
		{CATEGORIES_SPECIAL,     GetString(_T("IDS_CateSpecial"))},
		{CATEGORIES_TRAVEL,      GetString(_T("IDS_CateTravel"))},
		{CATEGORIES_VACATION,    GetString(_T("IDS_CateVacation"))},
		{CATEGORIES_ANNIVERSARY, GetString(_T("IDS_CateAnniversary"))}};
		int nKeyword = 0;
		int nLocale = 0;
		int nCategory = 0;
		bGetKeyword == true ? nLocale = 1 : nKeyword = 1;
		
		bool bIsCategory = false;
		for(nCategory = 0; nCategory<14; nCategory++)
		{
			if(arrayStrCate[nCategory][nKeyword] == strCate)
			{
				strCate = LPCTSTR(arrayStrCate[nCategory][nLocale]);
				bIsCategory = true;
				break;
			}
		}
		if(!bIsCategory && !strCate.IsEmpty())
		{
			nCategory = 6;
			strCate = GetString(_T("IDS_CateMisc"));
		}
		return nCategory;
	}
	int CQPObjListData::GetRule(CString& strRule)
	{
		int nRet = 0;
		vector<CString> vstrRule;
		Tokenize<CString>(_T("	 "), strRule, vstrRule, false);
		TSTRING tstrRule;
		if(strRule == _T(""))
		{
			strRule = GetString(_T("IDS_RecurNone"));
			nRet = 0;
		}
		else if(vstrRule.size() == 2 && vstrRule[0]==RRULE_TYPE_DAILY)
		{
			strRule = GetString(_T("IDS_RecurDaily"));
			nRet = 1;
		}
		else if(vstrRule.size() == 3 && vstrRule[0]==RRULE_TYPE_WEEKLY)
		{
			strRule = GetString(_T("IDS_RecurWeekly"));
			nRet = 2;
		}
		else if(vstrRule.size() == 3 && vstrRule[0]==RRULE_TYPE_MONTHLY)
		{
			strRule = GetString(_T("IDS_RecurMonthly"));
			nRet = 3;
		}
		else if(vstrRule.size() == 3 && vstrRule[0]==RRULE_TYPE_YEARLY)
		{
			strRule = GetString(_T("IDS_RecurYearly"));
			nRet = 4;	
		}
		else
		{
			strRule = GetString(_T("IDS_RecurOther"));
			nRet = 5;
		}
		return nRet;
	}

	CString CQPObjListData::GetCommonStrData(CommonDataPt spCommonData)
	{
		CommonDataType datatype = spCommonData->GetType();
		CString strRet = _T("");
		if(datatype == WString)
		{
#ifdef _UNICODE
			strRet = spCommonData->GetWString();
#endif

		}
		else if(datatype == String)
		{
#ifndef _UNICODE
			strRet = spCommonData->GetString();
#endif
		}
		return strRet;
	}
	bool CQPObjListData::GetCmnData(const char* cpKey, CString& strCmnData)
	{
		bool bRet = false;
		CommonDataPt spData = NULL;
		size_t npos = -1;
		size_t nGet = m_PIMObjPt->GetByName(cpKey, spData);
		if(nGet != npos)
		{
			strCmnData = GetCommonStrData(spData);
			bRet = true;
		}
		else
		{
			strCmnData = _T("");
		}
		return bRet;
	}

	const GUID& CQPObjListData::GetPIMObjGUID()
	{
		return m_PIMObjPt->GetGUID();
	}
	IPIMObjPt CQPObjListData::GetPIMObj()
	{
		return m_PIMObjPt;
	}
	bool CQPObjListData::SetPIMObj(IPIMObjPt PIMObjPt)
	{
		bool bRet = false;
		if(PIMObjPt)
		{	
			m_PIMObjPt = PIMObjPt;
			bRet = true;
		}
		return bRet;
	}

	CQDataCtrl::CQDataCtrl()
		:m_CalEvent(false,true,_T("vcalendar"))		
		,m_pILocale(NULL)
		,m_hParent(NULL)
	{
		DefineNotify(CALENDAR_V2VIEW_SWITCHVIEW_ON);
		DefineNotify(CALENDAR_V2VIEW_SWITCHVIEW_OFF);
		CreateILocalization(&m_pILocale);
	}	

	CQDataCtrl::~CQDataCtrl() 
	{
	}

	bool CQDataCtrl::RemoveAll(bool bRefresh)
	{
		bool bRes = false;
		if(m_spIPIMDB)
			bRes = m_spIPIMDB->RemoveAll(bRefresh);
		return bRes;
	}

	void CQDataCtrl::RefreshClear()
	{
		CTIListDatasourceBase<ColumnData,CQPObjListData> ::RefreshClear();
		RemoveAll(true);
	}

	bool CQDataCtrl::GetDataByIndex(int nIndex, CQListData** ppListData)
	{
		int nRealIndex = GetRealIndex(nIndex);
		return CTIListDatasourceBase<ColumnData,CQPObjListData>::GetDataByIndex(nRealIndex, ppListData);
	}
	bool CQDataCtrl::DeleteDataByIndex(int nRow) 
	{
		bool bRes = false;
		CQPObjListData* pListData = NULL;
		if(GetDataByIndex(nRow,(CQListData**)(&pListData)))
		{
			if(m_spIPIMDB->BeginTransaction())
			{
				bRes = m_spIPIMDB->RemoveItemByGUID(pListData->GetPIMObjGUID());
				m_spIPIMDB->EndTransaction();
			}
			if(bRes)
				bRes = CTIListDatasourceBase<ColumnData,CQPObjListData>::DeleteDataByIndex(nRow);
		}
		return bRes;
	}

	int CQDataCtrl::GetRealIndex(int nRow)
	{
		int nShow = -1;
		int nUnshow = 0;
		for(ListDataVec::iterator iter = m_ListDatas.begin(); iter!=m_ListDatas.end() && nShow != nRow;iter++)
		{
			if((*iter)->IsShow() == true)
				nShow++;	
			else
				nUnshow++;
		}
		return (nRow + nUnshow);

	}
	bool CQDataCtrl::Sync(IDevice* pIDevice,SyncType synctype)
	{
		bool bRes = false;
		if(m_spIPIMDB)
		{
			m_spIPIMDB->Register(this,g_strNotifyDBFinish);
			m_spIPIMDB->Register(this,g_strNotifyDBInsert);
			m_spIPIMDB->Register(this,g_strNotifyDBRemove);

			if(pIDevice->CanSync())
			{
				pIDevice->Sync(synctype);
				m_CalProgressBar.m_RunningEvent.SetEvent();
				CWnd* pParentWnd = CWnd::FromHandle(m_hParent);

				m_bSyncSucceed = false;
				CQProgressServer::Exec( &m_CalProgressBar,pParentWnd);
				if(m_bSyncSucceed)
					ShowMessage(m_hParent,m_pILocale->GetString(_T("IDS_Sync_Finish")), m_pILocale->GetString(_T("IDS_CALENDAR_FUNCTION")));
				else
					ShowMessage(m_hParent,m_pILocale->GetString(_T("IDS_Sync_Finish_Fail")), m_pILocale->GetString(_T("IDS_CALENDAR_FUNCTION")));

				bRes = true;
			}
		}
		return bRes;
	}

	void CQDataCtrl::Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent == g_strNotifyDBFinish)
		{				
			m_CalEvent.SetEvent();
			m_CalProgressBar.m_RunningEvent.ResetEvent();
			Fire(CALENDAR_V2VIEW_SWITCHVIEW_ON);
			m_bSyncSucceed  = (nParam == 1);
		}
		else if(strEvent == g_strNotifyDBInsert)
		{
			CSingleLock singleLock(&m_CritSection,TRUE);
			if(m_spIPIMDB)
			{
				long nDBCount = m_spIPIMDB->GetCount();
				long nDBRealCount = m_spIPIMDB->GetRealCount();

				GUID* pGuid = (GUID*)nParam;
				InsertByGUID(*pGuid);

				if(nDBCount != 0)
					m_CalProgressBar.m_nPersent = (nDBRealCount*100)/nDBCount;
			}
			m_CalEvent.SetEvent();
		}
	}

	void CQDataCtrl::InsertByGUID(const GUID& guid)
	{
		IPIMObjPt pimObjPt;
		if(m_spIPIMDB->BeginTransaction(guid,pimObjPt))
		{
			InsertDataByIndex(new CQPObjListData(pimObjPt));
			m_spIPIMDB->EndTransaction();
		}
	}

	bool CQDataCtrl::SetListDatas()
	{
		bool bRet = false;
		IPIMObjPts vPIMObjects;
		if(m_spIPIMDB)
		{
			for(int i=0;i<m_spIPIMDB->GetCount();i++)
			{
				IPIMObjPt pimObjPt;
				if(m_spIPIMDB->BeginTransaction(i,pimObjPt))
				{
					bool bFind = false;
					for(ListDataVec::iterator iterLD = m_ListDatas.begin(); iterLD!=m_ListDatas.end() && !bFind; iterLD++)
					{
						if( pimObjPt->GetGUID() == (*iterLD)->GetPIMObjGUID() )
						{
							(*iterLD)->SetPIMObj(pimObjPt);
							bFind = true;
						}
					}
					if(!bFind)
						InsertDataByIndex(new CQPObjListData(pimObjPt));

					m_spIPIMDB->EndTransaction();
				}
			}
			bRet = true;
		}
		return bRet;
	}
	void CQDataCtrl::DisplayAll()
	{
		Fire(EVENT_LIST_COLUMN_SORTOK, (long)&m_ListDatas);
		for(ListDataVec::iterator iter= m_ListDatas.begin(); iter!=m_ListDatas.end(); iter++)
		{
			(*iter)->Show(true);
		}
	}
	void CQDataCtrl::SetPeriod(const COleDateTime& DTStart, const COleDateTime& DTEnd)
	{
		Fire(EVENT_LIST_COLUMN_SORTOK, (long)&m_ListDatas);
		COleDateTime PeriodStart, PeriodEnd;
		ConvertTime(DTStart, PeriodStart, false);
		ConvertTime(DTEnd, PeriodEnd, false);

		int nRow = 0;
		for(ListDataVec::iterator iter= m_ListDatas.begin(); iter!=m_ListDatas.end(); iter++)
		{
			CString strDTStart, strDTEnd = _T("");
			COleDateTime iterDTStart, iterDTEnd;
			(*iter)->GetCmnData(KEYWORD_DTSTART,strDTStart);
			(*iter)->GetCmnData(KEYWORD_DTEND,strDTEnd);
			StrToTime(strDTStart, iterDTStart);
			StrToTime(strDTEnd, iterDTEnd);

			(*iter)->Show((iterDTStart<=PeriodEnd && iterDTEnd >= PeriodStart ) ? true : false);
			fdp dataparam;
			dataparam.nRow = nRow;
			if(!((*iter)->IsShow()))
			{	
				Fire(EVENT_LIST_DATA_DELETE_NODE,(long)&dataparam);
				nRow--;
			}
			nRow++;
		}
	}

	CQPObjListData* CQDataCtrl::NewPObjListData()
	{	
		CSingleLock singleLock(&m_CritSection,TRUE);
		CQPObjListData* pListData = NULL;
		if(m_spIPIMDB)
		{
			IPIMObjPt PIMObjPt;
			m_spIPIMDB->BeginTransaction();
			m_spIPIMDB->CreatePIMObject(PIMObjPt);
			m_spIPIMDB->EndTransaction();
			pListData = new CQPObjListData(PIMObjPt);
		}
		return pListData;
	}
	bool CQDataCtrl::InsertPIMObj(IPIMObjPt PIMObjPt)
	{
		CSingleLock singleLock(&m_CritSection,TRUE);
		m_spIPIMDB->BeginTransaction();
		bool bRet = m_spIPIMDB->InsertItem(PIMObjPt);
		m_spIPIMDB->EndTransaction();
		return bRet;
	}

	bool CQDataCtrl::UpdateDataByIndex(CQListData* pNewItem, int nRow) 
	{
		CSingleLock singleLock(&m_CritSection,TRUE);

		CQPObjListData* pNewObjListData = static_cast<CQPObjListData*>(pNewItem);

		if(m_spIPIMDB->BeginTransaction())
		{
			m_spIPIMDB->UpdateItem(pNewObjListData->GetPIMObj());			
			m_spIPIMDB->EndTransaction();
		}

		bool bRes = false;
		if(nRow >= 0 && nRow < GetRowCount()) 
		{
			ListDataVec::iterator Iter = m_ListDatas.begin(); 
			advance(Iter, nRow);
			CQPObjListData* pOriginalData = *Iter;
			*pOriginalData = *pNewObjListData;
			FireListDataParameter dataparam;
			dataparam.nRow = nRow;
			dataparam.pListData = pNewItem;
			Fire(EVENT_LIST_DATA_UPDATE_NODE,(long)&dataparam);
			bRes = true;
		}
		return bRes;
	}

	class SortFunc
	{
	private:
		CString m_Factor;
		bool	m_bDsc;
	public:
		SortFunc ( const CString& _Val,bool	bDsc ) : m_Factor ( _Val ),m_bDsc( bDsc ) {}
		bool operator () ( CQPObjListData* elem1, CQPObjListData* elem2 ) const
		{
			bool bRes = false;

			CString strFirst, strSecond = _T("");
			elem1->GetCmnData(QSYNC_LIB::UnicodetoASCII(m_Factor), strFirst);
			elem2->GetCmnData(QSYNC_LIB::UnicodetoASCII(m_Factor), strSecond);
			if(m_bDsc)
				bRes = strFirst > strSecond;
			else
				bRes = strFirst < strSecond;
			return bRes;
		}
	};

	bool CQDataCtrl::SortByColumn(const CString& strKey)
	{
		CSingleLock singleLock(&m_CritSection,TRUE);

		m_strCurrentOrder = strKey;		
		sort( m_ListDatas.begin(), m_ListDatas.end(), SortFunc(strKey,m_SortType[m_strCurrentOrder]) );
		m_SortType[m_strCurrentOrder] = !m_SortType[m_strCurrentOrder];
		Fire(EVENT_LIST_COLUMN_SORTOK, (long)&m_ListDatas);

		int nRow = 0;
		for(ListDataVec::iterator iter= m_ListDatas.begin(); iter!=m_ListDatas.end(); iter++)
		{
			fdp dataparam;
			dataparam.nRow = nRow;
			if(!((*iter)->IsShow()))
			{	
				Fire(EVENT_LIST_DATA_DELETE_NODE,(long)&dataparam);
				nRow--;
			}
			nRow++;
		}
		return true;
	}
	
	bool CQDataCtrl::GetDefaultDevice(IDevicePt& Device)
	{
		if(m_spIPIMDB)
			m_spIPIMDB->GetDefaultDevice(Device);
		return Device;
	}

	bool CQDataCtrl::GetRemoteDevices(IDevicePts& Devices)
	{
		bool bRes = false;
		if(m_spIPIMDB)
			bRes = m_spIPIMDB->EnumRemoteDevice(Devices);
		return bRes;
	}
	void CQDataCtrl::Connect()
	{			
		IPIMPt ptIPIM = new PIMFactory<CPIMRecord>;		
		TSTRING strNormalizeDatabaseName = _T("ID:") + m_szIMEI + _T(",MODULE:") + m_szMobileModel + _T(",TYPE:vcalendar");			
		CreatePIMDB(strNormalizeDatabaseName,ptIPIM,m_spIPIMDB);
		if(m_spIPIMDB)
		{
			IDevicePt ptNewDevice = new OutlookDevice(KEYWORD_CALENDAR);
			m_spIPIMDB->AddRemoteDevice(ptNewDevice,m_spIPIMDB);
		}
	}

} // end of namespace

#pragma warning(pop)