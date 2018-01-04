#if !defined(AFX_COLOREDCOMBO_H__83F389F9_2241_40EF_889B_7F7B857279EA__INCLUDED_)
#define AFX_COLOREDCOMBO_H__83F389F9_2241_40EF_889B_7F7B857279EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColoredCombo.h : header file
//

/****************************************************************
	If you ever tried, you must have noticed that coloring the
	ComboBox is not that trivial at all. hereby, i show how to
	do it easily and effectively.
****************************************************************/

#include "ColoredEditInCombo.h"
//#include "ColoredListBox.h"
  
#define RED		RGB(255,0,0) 
#define WHITE	RGB(255,255,255)
#define BLACK	RGB(0,0,0)


/////////////////////////////////////////////////////////////////////////////
// CColoredCombo window

class CColoredCombo : public CComboBox
{
// Construction
public:
	CColoredCombo();
	virtual ~CColoredCombo();

// Attributes
public:

// Operations
public:
	void		SetBkColor(COLORREF clrBk);

	void		SetTextColor(COLORREF clrText)
	{
		m_clrText = clrText;
		m_Edit.SetTextColor( clrText );
	}

	void		SetDropListStyle(bool bDropList = true)
	{
		//udate the flag, and then use it in the OnCtlClr, when subclassing the m_Edit
		m_bDropDownListStyle = bDropList;
		m_Edit.SetReadOnly( m_bDropDownListStyle );
		m_Edit.SetDropListStyle( bDropList );
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredCombo)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredCombo)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CColoredEditInCombo	m_Edit;	//the edit box of the combo. i use it ALWAYS becoz
											//i do not allow the Drop List style, bcoz the
											//static cannot be colored when focus lost.
//	CColoredListBox	m_ListBox;
	bool				m_bDropDownListStyle;
	
	CBrush			m_Brush;
	bool				m_bIsButtonClick;

	COLORREF m_clrBk;
	COLORREF m_clrText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDCOMBO_H__83F389F9_2241_40EF_889B_7F7B857279EA__INCLUDED_)
