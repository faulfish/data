/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2005 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   QDateTimeCtrl.cpp  $
 *  $Revision:   1.1  $
 *  $Date:   Apr 25 2007 09:47:28  $ 
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 * 
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 ***************************************************************************/
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Calendar_Version2/QDateTimeCtrl.cpp.-arc  $
   
      Rev 1.1   Apr 25 2007 09:47:28   Evelyn Chen
   update
   
      Rev 1.0   Jan 12 2007 08:26:36   Evelyn Chen
   Initial revision.
   
      Rev 1.0   Oct 31 2006 17:34:48   Jane Yu
   Initial revision.
   
      Rev 1.1   Dec 01 2005 16:37:32   Sky Yang
   Force to get new date since it may be changed by WM_DATE_CHANGED.
   
      Rev 1.0   Oct 20 2005 15:33:54   Jane Yu
   Initial revision.
  */
 
 /**************************************************************************/
// QDateTimeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "QDateTimeCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQDateTimeCtrl

CQDateTimeCtrl::CQDateTimeCtrl()
{
}

CQDateTimeCtrl::~CQDateTimeCtrl()
{
}


BEGIN_MESSAGE_MAP(CQDateTimeCtrl, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CQDateTimeCtrl)
//	ON_NOTIFY_REFLECT(DTN_DATETIMECHANGE, OnDatetimechange)
//	ON_NOTIFY_REFLECT(DTN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQDateTimeCtrl message handlers

//void CQDateTimeCtrl::OnDatetimechange(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	NMDATETIMECHANGE* pChange = (NMDATETIMECHANGE*) pNMHDR;
//	*pResult = 0;
//
//	if(pChange != NULL)
//	{
//		if((pChange->st.wDay != m_stCurTime.wDay) || (pChange->st.wMonth != m_stCurTime.wMonth) || (pChange->st.wYear != m_stCurTime.wYear))
//		{
//			// calcuate the relative time.
//			COleDateTime t1(pChange->st.wYear, pChange->st.wMonth, pChange->st.wDay, 0, 0, 0);
//			COleDateTime t2(m_stCurTime.wYear, m_stCurTime.wMonth, m_stCurTime.wDay, 0, 0, 0);
//
//			COleDateTimeSpan* pSpan = new COleDateTimeSpan;
//			ASSERT(pSpan != NULL);
//
//			if(pSpan != NULL)
//			{
//				*pSpan = (t1-t2);
//				::SendMessage(GetParent()->GetSafeHwnd(), WM_DATE_CHANGED, (DWORD)pSpan, (DWORD)GetDlgCtrlID());
//			}
//
//			// update the current selected date.
//			GetTime(&m_stCurTime);
//		}
//	}
//}

//void CQDateTimeCtrl::InitDate()
//{
//	GetTime(&m_stCurTime);
//}
//
//void CQDateTimeCtrl::OnCloseup(NMHDR* pNMHDR, LRESULT* pResult) 
//{
//	// TODO: Add your control notification handler code here
//	
//	*pResult = 0;
//}
