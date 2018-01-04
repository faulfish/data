
#if !defined(AFX_HEXEDIT_H__D9147501_258D_11D4_8478_00104B0E56C6__INCLUDED_)
#define AFX_HEXEDIT_H__D9147501_258D_11D4_8478_00104B0E56C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HexEdit.h : header file
//

#define		NO_SELECTED				0xffffffff
#define		HEX_EDIT_DATA_SIZE		1000
#define		TOHEX(a, b)				{*b++ = gcHexTable[a >> 4]; *b++ = gcHexTable[a&0xf];}

static bool gbInit = false; 
/////////////////////////////////////////////////////////////////////////////
// CHexEdit window

class CHexEdit : public CEdit
{
	// Construction
public:
	CHexEdit();

	enum EDITMODE{EDIT_NONE, EDIT_ASCII, EDIT_HIGH,	EDIT_LOW} ;
	// Attributes
public:
	CView		*pOwnerView;
	BOOL		m_bDirty;	// memory contents are changed

	LPBYTE		m_pData;			// pointer to data
	int			m_length;			// length of data
	int			m_topindex;			// offset of first visible byte on screen
	int			m_itemId;

	int			m_currentAddress;	// address under cursor
	EDITMODE	m_currentMode;		// current editing mode: address/hex/ascii
	int			m_selStart;			// start address of selection
	int			m_selEnd;			// end address of selection

	int			m_bpr;				// byte per row
	int			m_lpp;				// lines per page
	BOOL		m_bShowAddress;
	BOOL		m_bShowAscii;
	BOOL		m_bShowHex;
	BOOL		m_bAddressIsWide;

	BOOL		m_bNoAddressChange;	// internally used
	BOOL		m_bHalfPage;

	CFont		m_Font;
	int			m_lineHeight;
	int			m_nullWidth;
	BOOL		m_bUpdate;
	BOOL		m_isEdit;     //can be edit

	int			m_offHex,m_offAscii,m_offAddress;

	CPoint		m_editPos;

	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHexEdit)
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

	// Implementation
public:
	void		SetSel(int s, int e);
	void		SetOptions(BOOL bAddress, BOOL bHex, BOOL bAscii, BOOL bWide=FALSE);
	void		SetData(int itemId,LPBYTE p, int len);
	void		SetBPR(int bpr);
	CSize		GetSel(void);
	int			GetData(LPBYTE p, int len);
	virtual		~CHexEdit();

	// Generated message map functions
protected:
	void		UpdateScrollbars();
	void		ScrollIntoView(int p);
	void		RepositionCaret(int p);
	void		NormalizeSel(void);
	void		Move(int x, int y);
	inline BOOL IsSelected(void);
	void		CreateEditCaret(void);
	void		CreateAddressCaret(void);
	CPoint		CalcPos(int x, int y);
	//{{AFX_MSG(CHexEdit)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg UINT OnGetDlgCode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg void OnPaint();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	void          SetIsEdit(bool isModify){ m_isEdit= isModify; };
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEXEDIT_H__D9147501_258D_11D4_8478_00104B0E56C6__INCLUDED_)
