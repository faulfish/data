#include "stdafx.h"
#include "SxGroupBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CSxGroupBox::CSxGroupBox()
{
	// set disabled box colors
	m_boxColor1_d = ::GetSysColor(COLOR_3DSHADOW);
	m_boxColor2_d = ::GetSysColor(COLOR_3DHILIGHT);

	// set default box colors
	m_boxColor1 = m_boxColor1_d;
	m_boxColor2 = m_boxColor2_d;

	// default text color is black
	m_txtColor = RGB(0,0,0);

	// default font
	CSxLogFont lf1;	
	m_txtFont = new CFont;
	m_txtFont->CreatePointFontIndirect(&lf1);

	// default text
	m_txtString.Empty();

	// default text alignment
	m_txtAlignment = BS_LEFT;

	// default text offset (pixels)
	m_txtHOffset = 8;
	m_txtVOffset = 0;

	// default 3D style
	m_LineStyle = BS_3D;

	// default bitmap style
	m_StyleBitmap = false;

	// default box line thickness
	m_boxThickness = 1;

}

CSxGroupBox::~CSxGroupBox()
{
    if(m_txtFont) delete m_txtFont;
}

BEGIN_MESSAGE_MAP(CSxGroupBox, CButton)
	//{{AFX_MSG_MAP(CSxGroupBox)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
//
// store groupbox members after creation
//
void CSxGroupBox::PreSubclassWindow() 
{
	// store the current text (from resource)
	GetWindowText(m_txtString);

	// get current style parameters (from resource definition)
	DWORD dwStyle = GetStyle();
	DWORD dwExStyle = GetExStyle();

	// store the current text alignment
	if((dwStyle & BS_CENTER) == BS_RIGHT)
		m_txtAlignment = BS_RIGHT;
	else if((!(dwStyle & BS_CENTER)) || ((dwStyle & BS_CENTER) == BS_LEFT))
		m_txtAlignment = BS_LEFT;
	else if((dwStyle & BS_CENTER) == BS_CENTER)
		m_txtAlignment = BS_CENTER;

	// store the box rectangle
	GetWindowRect(&m_boxRect);
	ScreenToClient(&m_boxRect);

	// handle the extended window styles
	if(dwExStyle & WS_EX_DLGMODALFRAME)
	{
		m_boxRect.DeflateRect(2*GetSystemMetrics(SM_CXEDGE), 2*GetSystemMetrics(SM_CYEDGE));
		m_boxRect.left--;
	}
	if(dwExStyle & WS_EX_CLIENTEDGE)
	{
		m_boxRect.DeflateRect(GetSystemMetrics(SM_CXEDGE)+1, GetSystemMetrics(SM_CYEDGE)+1);
		m_boxRect.left--;
		if (dwExStyle & WS_EX_DLGMODALFRAME)
			m_boxRect.right++;
	}	
	if(dwExStyle & WS_EX_STATICEDGE)
	{
		m_boxRect.DeflateRect(2*GetSystemMetrics(SM_CXBORDER), 2*GetSystemMetrics(SM_CYBORDER));
		m_boxRect.left--;
	}

	// flat or 3D
	if(dwStyle & BS_FLAT)
		m_LineStyle = 0;
	else
		m_LineStyle = 1;

	// text or bitmap/icon
	if(dwExStyle & (BS_ICON | BS_BITMAP))
		m_StyleBitmap = true;
	else
		m_StyleBitmap = false;

	CButton::PreSubclassWindow();
}
//
// draw the control with custom colors and font
//
void CSxGroupBox::OnPaint() 
{
	CPoint ptStart, ptEnd;
	CPen pnFrmDark, pnFrmLight, *ppnOldPen;
	CBrush brFrmDark, brFrmLight;
	LOGBRUSH LbrFrmDark, LbrFrmLight;

	CPaintDC dc(this);
	int nSavedDC = dc.SaveDC();

	// add a blank on each side of text
	CString sText;
	sText.Format(" %s ", m_txtString);

	// get the text size used by resource editor
	CSize seText0 = dc.GetTextExtent(sText);

	// set the font to draw
	CFont *oldFont = dc.SelectObject(m_txtFont);

	// get the drawn text size
	CSize seText = dc.GetTextExtent(sText);
//
// handle text alignment (BS_CENTER == BS_LEFT|BS_RIGHT)
//
	// set the box top
	ptStart.y = ptEnd.y = m_boxRect.top + seText0.cy / 2 - 1;

	// set the horizontal points for the text gap
	if(m_txtAlignment == BS_RIGHT)
	{
		ptEnd.x = m_boxRect.right - m_txtHOffset;
		ptStart.x = ptEnd.x - seText.cx;
	}
	else if(m_txtAlignment == BS_LEFT)
	{
		ptStart.x = m_boxRect.left + m_txtHOffset;
		ptEnd.x = ptStart.x + seText.cx;
	}
	else if(m_txtAlignment == BS_CENTER)
	{
		ptStart.x = (m_boxRect.Width() - seText.cx) / 2;
		ptEnd.x = ptStart.x + seText.cx;
	}
//
// create pens to draw with
//
	int PenStyle = PS_GEOMETRIC | PS_SOLID | PS_ENDCAP_SQUARE;
	if(IsWindowEnabled())
	{
		// create brushes
		brFrmDark.CreateSolidBrush(m_boxColor1);
		brFrmDark.GetLogBrush(&LbrFrmDark);
		brFrmLight.CreateSolidBrush(m_boxColor2);
		brFrmLight.GetLogBrush(&LbrFrmLight);
		// create pens
		pnFrmDark.CreatePen(PenStyle, m_boxThickness, &LbrFrmDark);
		pnFrmLight.CreatePen(PenStyle, m_boxThickness, &LbrFrmLight);
	}
	else
	{
		// create brushes
		brFrmDark.CreateSolidBrush(m_boxColor1_d);
		brFrmDark.GetLogBrush(&LbrFrmDark);
		brFrmLight.CreateSolidBrush(m_boxColor2_d);
		brFrmLight.GetLogBrush(&LbrFrmLight);
		// create pens
		pnFrmDark.CreatePen(PenStyle, m_boxThickness, &LbrFrmDark);
		pnFrmLight.CreatePen(PenStyle, m_boxThickness, &LbrFrmLight);
	}
//
// draw the box with text gap
//
	// flat frame style
	if(m_LineStyle == BS_FLAT)
	{
		// draw the box
		ppnOldPen = dc.SelectObject(&pnFrmDark);
		dc.MoveTo(ptStart);
		dc.LineTo(m_boxRect.left,   ptStart.y);
		dc.LineTo(m_boxRect.left,   m_boxRect.bottom);
		dc.LineTo(m_boxRect.right,  m_boxRect.bottom);
		dc.LineTo(m_boxRect.right,  ptEnd.y);
		dc.LineTo(ptEnd);
	}
	// inscribed rectangles
	else if(m_LineStyle == BS_RECT)
	{
		// draw the first box (inner)
		ppnOldPen = dc.SelectObject(&pnFrmDark);
		dc.MoveTo(ptStart);
		dc.LineTo(m_boxRect.left,   ptStart.y);
		dc.LineTo(m_boxRect.left,   m_boxRect.bottom);
		dc.LineTo(m_boxRect.right,  m_boxRect.bottom);
		dc.LineTo(m_boxRect.right,  ptEnd.y);
		dc.LineTo(ptEnd);

		// draw the second box (outer)
		dc.SelectObject(&pnFrmLight);
		dc.MoveTo(ptStart.x,                        ptStart.y - m_boxThickness);
		dc.LineTo(m_boxRect.left - m_boxThickness,  ptStart.y - m_boxThickness);
		dc.LineTo(m_boxRect.left - m_boxThickness,  m_boxRect.bottom + m_boxThickness);
		dc.LineTo(m_boxRect.right + m_boxThickness, m_boxRect.bottom + m_boxThickness);
		dc.LineTo(m_boxRect.right + m_boxThickness, ptEnd.y - m_boxThickness);
		dc.LineTo(ptEnd.x,                          ptEnd.y - m_boxThickness);	
	}
	// 3D hilite
	else
	{
		// draw the first box
		ppnOldPen = dc.SelectObject(&pnFrmDark);
		dc.MoveTo(ptStart);
		dc.LineTo(m_boxRect.left,                   ptStart.y);
		dc.LineTo(m_boxRect.left,                   m_boxRect.bottom - m_boxThickness);
		dc.LineTo(m_boxRect.right - m_boxThickness, m_boxRect.bottom - m_boxThickness);
		dc.LineTo(m_boxRect.right - m_boxThickness, ptEnd.y);
		dc.LineTo(ptEnd);

		// draw the second box (hilite)
		dc.SelectObject(&pnFrmLight);
		dc.MoveTo(ptStart.x,                        ptStart.y + m_boxThickness);
		dc.LineTo(m_boxRect.left + m_boxThickness,  ptStart.y + m_boxThickness);
		dc.LineTo(m_boxRect.left + m_boxThickness,  m_boxRect.bottom - 2*m_boxThickness);
		dc.MoveTo(m_boxRect.left + m_boxThickness,  m_boxRect.bottom);
		dc.LineTo(m_boxRect.right,                  m_boxRect.bottom);
		dc.LineTo(m_boxRect.right,                  ptEnd.y + m_boxThickness);
		dc.MoveTo(m_boxRect.right - 2*m_boxThickness, ptEnd.y + m_boxThickness);
		dc.LineTo(ptEnd.x,                          ptEnd.y + m_boxThickness);	
	}
//
// draw text (if any)
//
	// set color and drawing mode
	COLORREF oldColor = dc.GetTextColor();
	dc.SetTextColor(m_txtColor);
	dc.SetBkMode(TRANSPARENT);

	if(!sText.IsEmpty() && !m_StyleBitmap)
	{
		// draw disabled text
		if(!IsWindowEnabled())
		{
			ptStart.y -= seText.cy / 2;
			dc.DrawState(ptStart, seText, sText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}

		// draw enabled text
		else
		{
			// get top of text box
			ptStart.y -= m_txtVOffset;

			dc.DrawText(sText, CRect(ptStart, ptEnd), 
				DT_VCENTER | DT_LEFT | DT_SINGLELINE | DT_NOCLIP);
		}
	}

	// cleanup
	dc.SelectObject(oldFont);
	dc.SetTextColor(oldColor);
	dc.SelectObject(ppnOldPen);
	dc.RestoreDC(nSavedDC);
}
//
// sets the box colors
//
void CSxGroupBox::SetBoxColors
(
	COLORREF color1,   // primary color
	COLORREF color2,   // highlight color
	bool redraw        // redraw flag
) 
{ 
	if(color1)
		m_boxColor1 = color1;
	else
		m_boxColor1 = m_boxColor1_d;

	if(color2)
		m_boxColor2 = color2;
	else
		m_boxColor2 = m_boxColor2_d;

	if(redraw) Invalidate(false);
}
//
// gets the box colors
//
void CSxGroupBox::GetBoxColors
(
	COLORREF& color1,
	COLORREF& color2
) 
{
	color1 = m_boxColor1;
	color2 = m_boxColor2;
}
//
// sets the text color
//
void CSxGroupBox::SetTextColor(COLORREF color, bool redraw) 
{ 
	m_txtColor = color;
	if(redraw)
		Invalidate();
}
//
// sets the text font
//
void CSxGroupBox::SetFont(const LOGFONT* lpLogFont)
{
	// create the font
	if(!lpLogFont)
	{
		CSxLogFont lf0;
		lpLogFont = &lf0;
	}

	// delete current font
	if(m_txtFont)
		delete m_txtFont;

	// create new font on heap
	m_txtFont = new CFont;
	m_txtFont->CreatePointFontIndirect(lpLogFont);
}
//
// sets the text
//
void CSxGroupBox::SetText(CString textStr, bool redraw)
{
	m_txtString = textStr;
	SetWindowText(m_txtString);	

	if(redraw)
	{
		Invalidate(true);
	}
}
//
// sets the text alignment
//
void CSxGroupBox::SetTextAlign(int TextAlign)
{
	switch(TextAlign)
	{
		case BS_LEFT:
		case BS_CENTER:
		case BS_RIGHT:
		{
			m_txtAlignment = TextAlign;
			break;
		}
		
		default:
			m_txtAlignment = BS_LEFT;
	}
}
//
// sets the horizontal and vertical text offsets
//
void CSxGroupBox::SetTextOffset(int Hoffset, int Voffset)
{
	m_txtHOffset = Hoffset;
	m_txtVOffset = Voffset;
}
//
// sets the box line style
//
void CSxGroupBox::SetLineStyle(int StyleLine)
{
	if( (StyleLine != BS_FLAT) && 
		 (StyleLine != BS_3D) &&
		 (StyleLine != BS_RECT))
	StyleLine = BS_3D;

	m_LineStyle = StyleLine;

}
//
// handle the tab key
//
HBRUSH CSxGroupBox::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	GetParent()->Invalidate();
	return NULL;
}
