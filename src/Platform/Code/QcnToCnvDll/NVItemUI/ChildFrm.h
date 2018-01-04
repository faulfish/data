// ChildFrm.h : interface of the CChildFrame class
//


#pragma once
#include "..\..\QcnToCnvDll\QcnToCnvDll\QcnToCnvDll.h"

class CChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CChildFrame)
public:
	CChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
protected:
	CToolBar    m_wndToolBar;
	CComboBox   m_ComboCom;
	CEdit       m_comPort;
// Generated message map functions
protected:
	BOOL		 OnCreateClient(LPCREATESTRUCT lpCreateStruct, CCreateContext* pContext);
	void		 ActivateFrame(int nCmdShow); 
	void		 OnComboChange(void);
	DECLARE_MESSAGE_MAP()

public:
	CSplitterWnd m_wndSplitter1;
	CSplitterWnd m_wndSplitter2;
	NVArray		 m_nVArray;
	CString		 m_fileType;
	CString		 m_filePath; 
};
