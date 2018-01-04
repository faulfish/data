// ColoredEditInCombo.cpp : implementation file
//

#include "stdafx.h"
#include "ColoredEditInCombo.h"
#include "ColoredCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColoredEditInCombo

CColoredEditInCombo::CColoredEditInCombo(CColoredCombo* pOwner)
:	m_pOwner(pOwner),
	m_bDropDownListStyle(false)
{
}

CColoredEditInCombo::~CColoredEditInCombo()
{
}


BEGIN_MESSAGE_MAP(CColoredEditInCombo, CColoredEdit)
	//{{AFX_MSG_MAP(CColoredEditInCombo)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColoredEditInCombo message handlers

void CColoredEditInCombo::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//enable context menu only in Drop Down, not in LIST style
	if( m_bDropDownListStyle )
		return;
	
	CColoredEdit::OnRButtonDown(nFlags, point);
}

void CColoredEditInCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//clicking on the edit box in drop down LIST should open the list:
	if( m_bDropDownListStyle )
	{
		m_pOwner->ShowDropDown();
		return;
	}
	
	CColoredEdit::OnLButtonDown(nFlags, point);
}

void CColoredEditInCombo::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	//when the list is open, enter should make the selection:
	if(m_pOwner->GetDroppedState())
	{
		if (nChar == VK_RETURN)
			m_pOwner->ShowDropDown( FALSE );
	}
	
	CColoredEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}
