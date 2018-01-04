// NVItemUIView.h : interface of the CNVItemUIView class
//


#pragma once


class CNVItemUIView : public CView
{
protected: // create from serialization only
	CNVItemUIView();
	DECLARE_DYNCREATE(CNVItemUIView)

// Attributes
public:
	CNVItemUIDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CNVItemUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in NVItemUIView.cpp
inline CNVItemUIDoc* CNVItemUIView::GetDocument() const
   { return reinterpret_cast<CNVItemUIDoc*>(m_pDocument); }
#endif

