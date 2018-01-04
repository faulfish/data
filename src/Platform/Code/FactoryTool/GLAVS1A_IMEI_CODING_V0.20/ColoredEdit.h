#if !defined(AFX_COLOREDEDIT_H__B84ED3D1_049C_4C29_BD23_510B53E0A64A__INCLUDED_)
#define AFX_COLOREDEDIT_H__B84ED3D1_049C_4C29_BD23_510B53E0A64A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColoredEdit.h : header file
//

/****************************************************************
	this class is colored editBox, and it is good for those who
	needs just a simple edit box with colors.
****************************************************************/


/////////////////////////////////////////////////////////////////////////////
// CColoredEdit window

class CColoredEdit : public CEdit
{
// Construction
public:
	CColoredEdit();

// Attributes
public:

// Operations
public:
	void		SetBkColor(COLORREF clrBk)
	{	m_clrBk = clrBk;	}

	void		SetTextColor(COLORREF clrText)
	{	m_clrText = clrText;	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColoredEdit)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColoredEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColoredEdit)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
	CBrush	m_Brush;
	COLORREF m_clrBk;
	COLORREF m_clrText;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLOREDEDIT_H__B84ED3D1_049C_4C29_BD23_510B53E0A64A__INCLUDED_)
