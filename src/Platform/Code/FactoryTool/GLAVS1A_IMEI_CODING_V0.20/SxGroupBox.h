#if !defined(__SXGROUPBOX_H)
#define __SXGROUPBOX_H
#pragma once
//
//===================================================================
// SxGroupBox Custom Groupbox Class
//                                         By Toby Norris, 1999
//
// This class provides functions for customizing the font, colors, 
// and style of the GroupBox control.  
//====================================================================
//

// box line styles
#define BS_3D     11
#define BS_RECT   12

#include "SxLogFont.h"

class CSxGroupBox : public CButton
{
public:
	CSxGroupBox();
	virtual ~CSxGroupBox();

	void SetBoxColors(COLORREF color1 = NULL, 
		COLORREF color2 = NULL, bool redraw = false);
	void GetBoxColors(COLORREF& color1, COLORREF& color2);

	void SetText(CString textStr, bool redraw = false);
	CString GetText()
		{return m_txtString;};

	void SetTextColor(COLORREF color, bool redraw = false);
	COLORREF GetTextColor()
		{return m_txtColor;};

	void SetFont(const LOGFONT* lpLogFont = (LOGFONT*)NULL);
	CFont* GetFont()
		{return m_txtFont;};

	void SetTextAlign(int TextAlign);
	int GetTextAlign()
		{return m_txtAlignment;};

	void SetTextOffset(int Voffset, int Hoffset);

	void SetLineStyle(int StyleLine = BS_3D);
	int GetLineStyle()
		{return m_LineStyle;};

	void SetLineThickness(int thickness, bool redraw = false)
		{m_boxThickness=thickness;if(redraw)Invalidate();};
	int GetLineThickness()
		{return m_boxThickness;};

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSxGroupBox)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

private:
	COLORREF m_boxColor1;
	COLORREF m_boxColor2;
	COLORREF m_boxColor1_d;
	COLORREF m_boxColor2_d;
	bool m_StyleBitmap;
	int m_boxThickness;

	COLORREF m_txtColor;
	CFont *m_txtFont;
	CString m_txtString;
	int m_txtAlignment;    // text alignment (BS_LEFT, BS_CENTER, BS_RIGHT)
	int m_txtHOffset;      // horizontal distance from left/right side to beginning/end of text
	int m_txtVOffset;      // vertical distance to move font up
	int m_LineStyle;       // line style (BS_FLAT, BS_3D, BS_RECT)

	CRect m_boxRect;

	//{{AFX_MSG(CSxGroupBox)
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
