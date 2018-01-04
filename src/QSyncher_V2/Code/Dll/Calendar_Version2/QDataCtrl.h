#if !defined(BENQ_EE05CBFF_8748_4855_8F99_71871D8D219B__INCLUDED_)
#define BENQ_EE05CBFF_8748_4855_8F99_71871D8D219B__INCLUDED_

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

#pragma once
#include "../../Components/UIFramework/interface/IListDatasourceBase.h"
#include "../../Components/UIFramework/interface/IQListCtrlUI.h"
#include "../../Components/PIMDB/ClientDevice.h"
#include "../../Components/PIMDB/PIMRecord.h"
#include "CalProgressBar.h"
#include "..\..\Components\PIMDB\VCalCtrl.h"
#include "../../Components/UIFramework/Porting/QProgressServer.h"
#include "../../Components/Localization/ilocalization.h"

//whether Switch View or not
const LPCTSTR CALENDAR_V2VIEW_SWITCHVIEW_ON = _T("TNotify_SwitchView_On");
const LPCTSTR CALENDAR_V2VIEW_SWITCHVIEW_OFF = _T("TNotify_SwitchView_Off");

namespace QSYNC_LIB 
{
	using namespace std;

	class CQDataCtrl;

	class CQPObjListData : public CQListData
	{
		friend class CQDataCtrl;
	public:
		CQPObjListData(IPIMObjPt PIMObjPt):m_PIMObjPt(PIMObjPt),m_bShow(true),m_pILocale(NULL)
		{
			CreateILocalization(&m_pILocale);
		}

		virtual ~CQPObjListData()
		{
			for(vector<CBasicData*>::iterator iter = m_vpGarbageBD.begin(); iter != m_vpGarbageBD.end(); iter++)
			{	
				if((*iter))
				{
					delete (*iter);
					(*iter) = NULL;
				}
			}
			m_vpGarbageBD.clear();
		}

		virtual CQPObjListData& operator=(const CQPObjListData& rhs)
		{
			m_PIMObjPt = rhs.m_PIMObjPt;
			m_vpGarbageBD = rhs.m_vpGarbageBD;
			m_bShow = rhs.m_bShow;
			m_pILocale = rhs.m_pILocale;

			return *this;
		}

		int GetRule(CString& strRule);
		int GetCategories(CString& strCate, bool bGetKeyword);
		CString GetAlarm(int nInterval);
		virtual CBasicData* GetValueByKey(const TSTRING& strKey);

		CString GetCommonStrData(CommonDataPt spCommonData);
		bool GetCmnData(const char* cpKey, CString& strCmnData);

		IPIMObjPt GetPIMObj();
		const GUID& GetPIMObjGUID();
		bool SetPIMObj(IPIMObjPt PObjPt);

	protected:		
		bool IsShow() const {return m_bShow;}
		void Show(bool bShow) {m_bShow = bShow;}
		void GarbageCollection(CBasicData* pBasicData);
		CString GetString(const TSTRING& strID){return m_pILocale->GetString(strID).c_str();}
	private:
		IPIMObjPt m_PIMObjPt;
		vector<CBasicData*> m_vpGarbageBD;
		bool m_bShow;
		QSYNC_LIB::ILocalization* m_pILocale;		
	};

	class CQDataCtrl : public INotify, public CTIListDatasourceBase<ColumnData,CQPObjListData> 
	{
	public:
		TSTRING m_szMobileModel;
		TSTRING m_szIMEI;

	public:
		CQDataCtrl();
		virtual ~CQDataCtrl();
		bool Sync(IDevice* pIDevice,SyncType synctype);
		void SetPeriod(const COleDateTime& DTStart, const COleDateTime& DTEnd);
		CQPObjListData* NewPObjListData();
		bool InsertPIMObj(IPIMObjPt PIMObjPt);

		bool RemoveAll(bool bRefresh = false);

		virtual void RefreshClear();
		virtual bool GetDataByIndex(int nIndex, CQListData** ppListData);
		virtual bool DeleteDataByIndex(int nRow);
		virtual bool SortByColumn(const CString& strKey);	
		virtual bool GetOrderInfo(CString& strKey, bool& bOrder)
		{
			strKey = m_strCurrentOrder.c_str();
			bOrder = m_SortType[m_strCurrentOrder];
			return true;
		}
		virtual bool UpdateDataByIndex(CQListData* pNewItem, int nRow);
		bool GetDefaultDevice(IDevicePt& Device);
		bool GetRemoteDevices(IDevicePts& Devices);
		void Connect();
		void DisplayAll();
		void SetParentHWND(HWND hParent){m_hParent = hParent;}

	protected:
		IPIMDBPt m_spIPIMDB;	
		virtual void Event(const TSTRING& strEvent,long nParam);		

	private:
		CEvent m_CalEvent;
		CalProgressBar m_CalProgressBar;
		TSTRING m_strCurrentOrder;		
		QSYNC_LIB::ILocalization* m_pILocale;
		HWND m_hParent;
		bool			m_bSyncSucceed;
	private:
		bool SetListDatas();
		int GetRealIndex(int nRow);
		void InsertByGUID(const GUID& guid);
	};

} //end namespace QSYNC_LIB

#pragma warning(pop)

#endif // !defined(BENQ_EE05CBFF_8748_4855_8F99_71871D8D219B__INCLUDED_)
