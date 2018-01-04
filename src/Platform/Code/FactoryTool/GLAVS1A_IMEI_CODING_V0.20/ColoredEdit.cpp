// ColoredEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "ColoredComboBox.h"
#include "ColoredEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredEdit

CColoredEdit::CColoredEdit()
{
	m_Brush.CreateSolidBrush( RGB( 250,0,0) );
}

CColoredEdit::~CColoredEdit()
{
}


BEGIN_MESSAGE_MAP(CColoredEdit, CEdit)
	//{{AFX_MSG_MAP(CColoredEdit)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredEdit message handlers

HBRUSH CColoredEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
   pDC->SetBkColor( m_clrBk );
	pDC->SetTextColor(  m_clrText );
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called
	return m_Brush;
}
