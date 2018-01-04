#if !defined(AFX_COLOREDEDITINCOMBO_H__D6A45732_DFF0_4EFB_AECC_8D10030D27CF__INCLUDED_)
#define AFX_COLOREDEDITINCOMBO_H__D6A45732_DFF0_4EFB_AECC_8D10030D27CF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColoredEditInCombo.h : header file
//

#include "ColoredEdit.h"

class		CColoredCombo;

/////////////////////////////////////////////////////////////////////////////
// CColoredEditInCombo window

class CColoredEditInCombo : public CColoredEdit
{
// Construction
public:
	CColoredEditInCombo(CColoredCombo* pOwner);

	void		SetDropListStyle(bool bDropList = true)
	{
		//udate the flag, and then use it in the OnCtlClr, when subclassing the m_Edit
		m_bDropDownListStyle = bDropList;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredEditInCombo)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColoredEditInCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredEditInCombo)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	bool				m_bDropDownListStyle;
	CColoredCombo*	m_pOwner;

	DECLARE_MESSAGE_MAP()

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDEDITINCOMBO_H__D6A45732_DFF0_4EFB_AECC_8D10030D27CF__INCLUDED_)
