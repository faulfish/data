// ColoredCombo.cpp : implementation file
//

/****************************************************************
its not that trivial to color the comboBox.... 
hereby, i show how to do it easily and effectively.
*****************************************************************/

#include "stdafx.h"
//#include "ColoredComboBox.h"
#include "ColoredCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CColoredCombo

CColoredCombo::CColoredCombo()
:	m_bDropDownListStyle(false),
	m_bIsButtonClick(false),
	m_clrText(BLACK),
	m_clrBk(WHITE),
	m_Edit(this)
{
	m_Brush.CreateSolidBrush( m_clrBk );
}

CColoredCombo::~CColoredCombo()
{
}


BEGIN_MESSAGE_MAP(CColoredCombo, CComboBox)
	//{{AFX_MSG_MAP(CColoredCombo)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CColoredCombo message handlers

HBRUSH CColoredCombo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
//	if (nCtlColor == CTLCOLOR_LISTBOX)
//   {
//      if( ! m_ListBox.GetSafeHwnd() )
//		{
//         VERIFY( m_ListBox.SubclassWindow(pWnd->GetSafeHwnd()) );
//		}
//   }

	if(nCtlColor == CTLCOLOR_EDIT || nCtlColor == CTLCOLOR_LISTBOX)
   {
      pDC->SetBkColor( m_clrBk );
		pDC->SetTextColor( m_clrText );
      return m_Brush;
   }

	HBRUSH hbr = CComboBox::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
}

void CColoredCombo::OnPaint() 
{
	static int nCounter(0);
	TRACE("%d. OnPaint()\n", nCounter++);
	
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CRect ClientRect;
	GetClientRect( &ClientRect );
	
	//draw the 2- 3D Rects around the combo
	dc.Draw3dRect(&ClientRect,
		::GetSysColor(COLOR_3DSHADOW),
		WHITE );
	ClientRect.DeflateRect(1,1);
	dc.Draw3dRect(&ClientRect,
		::GetSysColor(COLOR_3DDKSHADOW),
		::GetSysColor(COLOR_3DLIGHT) );
	ClientRect.DeflateRect(1,1);

	//fill the rect in the brush's color
	CBrush* pOldBrush = dc.SelectObject( &m_Brush );
	dc.FillRect(ClientRect, &m_Brush);

	//since the flags are not expon. (1,2,4,8...), i need to do like this:
	DWORD dwComboStyle = GetStyle();
	BYTE byteComboStyle = (BYTE)dwComboStyle;
	//in SIMPLE style, there is no button
	if( byteComboStyle == CBS_SIMPLE )
	{
		dc.SelectObject( pOldBrush );
		return;
	}

	/*******		only when Button is needed:	*****************/

	int nButtonWidth = ::GetSystemMetrics(SM_CYSIZE);

	//resize the ClientRect to the size of the buttom:
	DWORD dwExStyles = GetExStyle();
	if(dwExStyles & WS_EX_RIGHT)	//the button is on the left, WS_EX_RIGHT
	{
		ClientRect.right = ClientRect.left + nButtonWidth;
	} 
	else	//the button is on the right. WS_EX_LEFT
	{
		ClientRect.left = ClientRect.right - nButtonWidth + 1;	//+1 very important - dont lose a pixel
	}


	//paint the ****ing arrow:
	CPoint CenterPoint( ClientRect.CenterPoint() );
	++CenterPoint.y;
	CPoint UpperPoint(CenterPoint.x, CenterPoint.y - 4);
	CPen Pen(PS_SOLID, 1, m_clrText);
	CPen* pOldPen = dc.SelectObject(&Pen);
	for(int i = 0; i < 4; ++i)
	{
		dc.MoveTo(CenterPoint);
		VERIFY( dc.LineTo(UpperPoint) );

		--CenterPoint.x;
		--CenterPoint.y;
		--UpperPoint.x;
	}
	
	CenterPoint = ClientRect.CenterPoint();
	++CenterPoint.y;
	UpperPoint = CenterPoint;
	UpperPoint.y -= 4;
	for(int i = 0; i < 4; ++i)
	{
		dc.MoveTo(CenterPoint);
		VERIFY( dc.LineTo(UpperPoint) );

		++CenterPoint.x;
		--CenterPoint.y;
		++UpperPoint.x;
	}
	//until here - painting the arrow:

	//draw the rect of the button:
	if( ! m_bIsButtonClick )	//regular state - btn NOT pressed
	{
		dc.Draw3dRect(&ClientRect,
			::GetSysColor(COLOR_3DLIGHT),
			::GetSysColor(COLOR_3DDKSHADOW) );
		ClientRect.DeflateRect(1,1);
		dc.Draw3dRect(&ClientRect,
			WHITE,
			::GetSysColor(COLOR_3DSHADOW) );
	}
	else		//btn is pressed
	{
		m_bIsButtonClick = false;

		dc.Draw3dRect(&ClientRect,
			::GetSysColor(COLOR_3DSHADOW),
			::GetSysColor(COLOR_3DSHADOW) );
	}

	dc.SelectObject( pOldBrush );
	dc.SelectObject( pOldPen );
	
	// Do not call CComboBox::OnPaint() for painting messages
}

void CColoredCombo::PreSubclassWindow() 
{
	HWND hEdit = ::GetTopWindow(m_hWnd);
   VERIFY( m_Edit.SubclassWindow( hEdit ) );
	m_Edit.SetReadOnly( m_bDropDownListStyle );
   
	/*********************************************************************/
	//in case of "drop List", there is no EditBox, but Static, and window
	//paints the static AFTER my OnPaint, so i cannot paint it. so i do NOT allow
	//to work in this mode, but i edit it myself (ASSERT...)
	
	//since the flags are not expon. (1,2,4,8...), i need to do like this:
	DWORD dwComboStyle = GetStyle();
	BYTE byteComboStyle = (BYTE)dwComboStyle;
	if( byteComboStyle == CBS_DROPDOWNLIST )
	{
		//can NOT change the ctrl's style to drop down, so this code will NOT work:
//		ModifyStyle(CBS_DROPDOWNLIST, CBS_DROPDOWN);
//		//dwComboStyle = GetStyle();		//make sure... :)
		//so instead, :
		ASSERT( !"cannot paint DropDown List !!!");
	}
	
	CComboBox::PreSubclassWindow();
}

void CColoredCombo::SetBkColor(COLORREF clrBk)
{	
	m_clrBk = clrBk;
	m_Brush.Detach();
	VERIFY( m_Brush.CreateSolidBrush( m_clrBk ) );
	m_Edit.SetBkColor( clrBk ); 
}

/****************************************************************/

//the combo gets this message when its Button is pressed
void CColoredCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//the combo gets this message when its Button is pressed
	m_bIsButtonClick = true;
	
	CComboBox::OnLButtonDown(nFlags, point);
}
