/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2005 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   xSkinButton.h  $
 *  $Revision:   1.1  $
 *  $Date:   Apr 01 2009 14:17:28  $ 
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
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Components/UIFramework/Porting/xSkinButton.h.-arc  $
 * 
 *    Rev 1.1   Apr 01 2009 14:17:28   Kevin Tai
 * no change
 * 
 *    Rev 1.0   Apr 22 2008 20:54:24   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.0   Mar 27 2007 20:02:56   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.0   Mar 02 2005 18:59:44   Neko Hsieh
 * Initial revision.
  */
 
 /**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */
#if !defined(AFX_MYB_H__3832DDEF_0C12_11D5_B6BE_00E07D8144D0__INCLUDED_)
#define AFX_MYB_H__3832DDEF_0C12_11D5_B6BE_00E07D8144D0__INCLUDED_
 
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "..\CommUIExportAPI.h"
 
#define WM_CXSHADE_RADIO WM_USER+0x100
/////////////////////////////////////////////////////////////////////////////
// CxSkinButton window
class COMMONUI_API CxSkinButton : public CButton
{
	DECLARE_DYNAMIC(CxSkinButton)
 
// Construction
public:
	CxSkinButton();
 
// Attributes
private:
 
// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CxSkinButton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL
// Implementation
public:
	void SetToolTipText(CString s);
	COLORREF SetTextColor(COLORREF new_color);
	void SetSkin(UINT normal,UINT down, UINT over=0, UINT disabled=0, UINT focus=0,UINT mask=0,
				short drawmode=1,short border=1,short margin=4);
	virtual ~CxSkinButton();
	// Generated message map functions
protected:
	bool	m_Checked;					//radio & check buttons
	DWORD	m_Style;					//radio & check buttons
    bool m_tracking;
    bool m_button_down;
	void RelayEvent(UINT message, WPARAM wParam, LPARAM lParam);
	CToolTipCtrl m_tooltip;
	CBitmap m_bNormal,m_bDown,m_bDisabled,m_bMask,m_bOver,m_bFocus; //skin bitmaps
	short	m_FocusRectMargin;		//dotted margin offset
	COLORREF m_TextColor;			//button text color
	HRGN	hClipRgn;				//clipping region
	BOOL	m_Border;				//0=flat; 1=3D;
	short	m_DrawMode;				//0=normal; 1=stretch; 2=tiled;
	HRGN	CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);
	void	FillWithBitmap(CDC* dc, HBITMAP hbmp, RECT r);
	void	DrawBitmap(CDC* dc, HBITMAP hbmp, RECT r, int DrawMode);
	int		GetBitmapWidth (HBITMAP hBitmap);
	int		GetBitmapHeight (HBITMAP hBitmap);
	//{{AFX_MSG(CxSkinButton)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnClicked();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	afx_msg LRESULT OnMouseLeave(WPARAM, LPARAM);
	afx_msg LRESULT OnRadioInfo(WPARAM, LPARAM);
	afx_msg LRESULT OnBMSetCheck(WPARAM, LPARAM);
	afx_msg LRESULT OnBMGetCheck(WPARAM, LPARAM);
	DECLARE_MESSAGE_MAP()
};
/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
#endif // !defined(AFX_MYB_H__3832DDEF_0C12_11D5_B6BE_00E07D8144D0__INCLUDED_)
