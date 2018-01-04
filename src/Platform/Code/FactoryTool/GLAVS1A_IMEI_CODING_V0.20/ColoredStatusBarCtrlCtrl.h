#if !defined(AFX_COLOREDSTATUSBARCTRLCTRL_H__728F9FD1_CA39_435A_9C7D_0F7C8E3FA75E__INCLUDED_)
#define AFX_COLOREDSTATUSBARCTRLCTRL_H__728F9FD1_CA39_435A_9C7D_0F7C8E3FA75E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColoredStatusBarCtrlCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColoredStatusBarCtrlCtrl window

class CColoredStatusBarCtrlCtrl : public CStatusBarCtrl
{
// Construction
public:
	CColoredStatusBarCtrlCtrl();
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredStatusBarCtrlCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColoredStatusBarCtrlCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredStatusBarCtrlCtrl)
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDSTATUSBARCTRLCTRL_H__728F9FD1_CA39_435A_9C7D_0F7C8E3FA75E__INCLUDED_)
