/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2005 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   QDateTimeCtrl.h  $
 *  $Revision:   1.1  $
 *  $Date:   Apr 25 2007 09:46:52  $ 
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
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Calendar_Version2/QDateTimeCtrl.h.-arc  $
 * 
 *    Rev 1.1   Apr 25 2007 09:46:52   Evelyn Chen
 * update
 * 
 *    Rev 1.0   Jan 12 2007 08:26:36   Evelyn Chen
 * Initial revision.
 * 
 *    Rev 1.0   Oct 31 2006 17:34:50   Jane Yu
 * Initial revision.
 * 
 *    Rev 1.0   Oct 20 2005 15:33:56   Jane Yu
 * Initial revision.
  */
 
 /**************************************************************************/
#if !defined(AFX_QDATETIMECTRL_H__7F033D0D_EE23_46C1_8811_E30965FF6541__INCLUDED_)
#define AFX_QDATETIMECTRL_H__7F033D0D_EE23_46C1_8811_E30965FF6541__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////////////////////////////////////////////////
// Constants

// message identification.
#define WM_DATE_CHANGED		WM_USER + 0x441

// QDateTimeCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CQDateTimeCtrl window

class CQDateTimeCtrl : public CDateTimeCtrl
{
// Construction
public:
	CQDateTimeCtrl();

// Attributes
public:
//	void InitDate();

// Operations
protected:
	SYSTEMTIME m_stCurTime;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQDateTimeCtrl)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CQDateTimeCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CQDateTimeCtrl)
//	afx_msg void OnDatetimechange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QDATETIMECTRL_H__7F033D0D_EE23_46C1_8811_E30965FF6541__INCLUDED_)
