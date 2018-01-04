/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2004-2006 BenQ Corp.
 *
 ***************************************************************************
 */

#if !defined(AFX_SYNCSTATIC_H__1B613718_F8FB_4E1E_B215_D5F7D5CF825F__INCLUDED_)
#define AFX_SYNCSTATIC_H__1B613718_F8FB_4E1E_B215_D5F7D5CF825F__INCLUDED_


#include	<math.h>
#include "..\CommUIExportAPI.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SyncStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSyncStatic window

//------------------------- Steven-generated codes -------------------------
class COMMONUI_API CSyncStatic : public CStatic
{
// Construction
public:
	CSyncStatic(CString szFontName = _T(""),
					int nLogicFontSize = 9,
					LONG nFontWeight = FW_NORMAL,
					BOOL bIfItalic = FALSE,
					BOOL bIfUnderline = FALSE,
					BOOL bIf3D = FALSE,
					COLORREF TextFaceColor = RGB(0,0,0),
					COLORREF TextShadowColor = GetSysColor(COLOR_3DSHADOW),
					BOOL bIfTransparentBk = TRUE);
	
// Attributes
public:
	CString			m_szFontName;
	int				m_nLogicFontSize;
	LONG			m_nFontWeight;  //(FW_DONTCARE, FW_THIN, FW_NORMAL, FW_SEMIBOLD, FW_BOLD, FW_EXTRABOLD, FW_HEAVY)
	BOOL			m_bIfItalic;
	BOOL			m_bIfUnderline;
	BOOL			m_bIf3D;
	COLORREF		m_TextFaceColor;
	COLORREF		m_TextShadowColor;
	BOOL			m_bIfTransparentBk;
	
// Operations
public:
	void			set_FontName(CString szFontName);
	void			set_FontSize(int nLogicFontSize);
	void			set_FontWeight(LONG nFontWeight);
	void			set_Italic(BOOL bIfItalic);
	void			set_Underline(BOOL bIfUnderline);
	void			set_3DText(BOOL bIf3D, COLORREF TextShadowColor = ::GetSysColor(COLOR_3DSHADOW));
	void			set_TextFaceColor(COLORREF TextFaceColor);
	void			set_TransparentBk(BOOL bIfTransparentBk);
	void			RedrawText(void);
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSyncStatic)
	public:
	virtual void	OnFinalRelease();
	protected:
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL
	
// Implementation
public:
	virtual	~CSyncStatic();
	
protected:
	// Generated message map functions
	//{{AFX_MSG(CSyncStatic)
	afx_msg void	OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
protected:
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CSyncStatic)
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	
protected:
	DECLARE_INTERFACE_MAP()
};
//------------------------- Steven-generated codes -------------------------


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif // !defined(AFX_SYNCSTATIC_H__1B613718_F8FB_4E1E_B215_D5F7D5CF825F__INCLUDED_)
