#include "StdAfx.h"
#include "OutlookCtrl.h"
#include "..\..\CommonUtil\Win32ThreadUtility.h"

namespace QSYNC_LIB
{

	OUTLOOKACCESS_DLL_DECLSPEC bool CreateOutlookAccess(LPCTSTR cpName, IOutlookCtrl** ppOutlookCtrl)
	{
		*ppOutlookCtrl = new COutlookCtrl(cpName);
		return true;
	}
	OUTLOOKACCESS_DLL_DECLSPEC bool DeleteOutlookAccess(IOutlookCtrl* pOutlookCtrl)
	{
		if(pOutlookCtrl)
		{
			delete pOutlookCtrl;
			pOutlookCtrl = NULL;
		}
		return true;
	}

	COutlookCtrl::COutlookCtrl(LPCTSTR cpName):m_bAbort(false),m_pOLAccess(NULL),m_ThreadFinishEvent(TRUE,TRUE)
	{
		if(0 == _tcscmp(cpName,KEYWORD_CONTACT))
		{
			m_pOLAccess = new COLContactAccess;
			m_strCtrlName = KEYWORD_CONTACT;
			//m_pOLAccess->Register(this, EVENT_aCONTACT_FETCH_SUCCEEDED);
		}
		else if(0 == _tcscmp(cpName,KEYWORD_CALENDAR) )
		{
			m_pOLAccess = new COLCalAccess;
			m_strCtrlName = KEYWORD_CALENDAR;
			//m_pOLAccess->Register(this, EVENT_aCAL_FETCH_SUCCEEDED);
		}

		DefineNotify(EVENT_CONTACT_FETCHING_FINISH);
		DefineNotify(EVENT_CALENDAR_FETCHING_FINISH);
		DefineNotify(EVENT_CONTACT_FETCHING_FAIL);
		DefineNotify(EVENT_CALENDAR_FETCHING_FAIL);
		DefineNotify(EVENT_ACCESS_ESTABLISH_FAIL);

	}

	COutlookCtrl::~COutlookCtrl()
	{
		WaitForThreadFinish();

		if(m_pOLAccess)
		{
			delete m_pOLAccess;
			m_pOLAccess = NULL;
		}
	}

	void COutlookCtrl::WaitForThreadFinish()
	{
		QSYNC_LIB::MessageLoop(&m_ThreadFinishEvent.m_hObject,1);
	}

	bool COutlookCtrl::StartToFetch()
	{
		WaitForThreadFinish();

		AfxBeginThread(this->TaskWorkerThreadProc, (LPVOID)this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
		return true;
	}

	UINT COutlookCtrl::TaskWorkerThreadProc(LPVOID pParam)
	{
		COutlookCtrl* pThis = (COutlookCtrl*)pParam;

		pThis->m_ThreadFinishEvent.ResetEvent();

		BOOL bRes = FALSE;
		bool bEstablsih = false;
		if(pThis->m_pOLAccess)
		{	
			bEstablsih = pThis->m_pOLAccess->EstablishSession();
			if(bEstablsih)
			{
				bRes = pThis->m_pOLAccess->StartToFetch();
				pThis->m_pOLAccess->DestroySession();

				if(!pThis->m_bAbort && bRes && pThis->GetCtrlName() == KEYWORD_CONTACT)
					pThis->Fire(EVENT_CONTACT_FETCHING_FINISH);
				else if(!pThis->m_bAbort && bRes && pThis->GetCtrlName() == KEYWORD_CALENDAR)
					pThis->Fire(EVENT_CALENDAR_FETCHING_FINISH);
				else if((pThis->m_bAbort || !bRes) && pThis->GetCtrlName() == KEYWORD_CONTACT)
					pThis->Fire(EVENT_CONTACT_FETCHING_FAIL);
				else if((pThis->m_bAbort || !bRes) && pThis->GetCtrlName() == KEYWORD_CALENDAR)
					pThis->Fire(EVENT_CALENDAR_FETCHING_FAIL);
				pThis->m_bAbort = false;
			}
			else
			{
				pThis->Fire(EVENT_ACCESS_ESTABLISH_FAIL);
			}
		}
		pThis->m_ThreadFinishEvent.SetEvent();
		return bRes;
	}

	bool COutlookCtrl::Abort()
	{
		bool bRes = false;
		if(m_pOLAccess)
		{
			m_bAbort = true;
			bRes = m_pOLAccess->Abort();
		}
		return bRes;
	}

	bool COutlookCtrl::GetByIndex(int nIndex, IPIMObjPt& PIMObjPt)
	{
		bool bRes = false;
		if(m_pOLAccess)
			bRes = m_pOLAccess->GetByIndex(nIndex, PIMObjPt);
		return bRes;
	}
	int COutlookCtrl::GetCount()
	{
		int nCount = 0;
		if(m_pOLAccess)
			nCount = m_pOLAccess->GetCount();
		return nCount;
	}
	bool COutlookCtrl::Insert(IPIMObjPt PIMObjPt)
	{
		bool bRes = false;
		if(m_pOLAccess && PIMObjPt)
		{
			m_pOLAccess->EstablishSession();
			bRes = m_pOLAccess->New(PIMObjPt);
			m_pOLAccess->DestroySession();
		}
		return bRes;
	}
	bool COutlookCtrl::Update(IPIMObjPt PIMObjPt)
	{
		bool bRes = false;
		if(m_pOLAccess && PIMObjPt)
		{
			m_pOLAccess->EstablishSession();
			bRes = m_pOLAccess->Modify(PIMObjPt);
			m_pOLAccess->DestroySession();
		}
		return bRes;
	}
	bool COutlookCtrl::Delete(IPIMObjPt PIMObjPt)
	{
		bool bRes = false;
		if(m_pOLAccess && PIMObjPt)
		{
			m_pOLAccess->EstablishSession();
			bRes = m_pOLAccess->Delete(PIMObjPt);
			m_pOLAccess->DestroySession();
		}
		return bRes;
	}
	void COutlookCtrl::Event(const TSTRING& strEvent, long nParam)
	{
	}

}//namespace QSYNC_LIB