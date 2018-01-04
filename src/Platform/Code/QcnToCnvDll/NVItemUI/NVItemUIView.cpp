// NVItemUIView.cpp : implementation of the CNVItemUIView class
//

#include "stdafx.h"
#include "NVItemUI.h"

#include "NVItemUIDoc.h"
#include "NVItemUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNVItemUIView

IMPLEMENT_DYNCREATE(CNVItemUIView, CView)

BEGIN_MESSAGE_MAP(CNVItemUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// CNVItemUIView construction/destruction

CNVItemUIView::CNVItemUIView()
{
	// TODO: add construction code here

}

CNVItemUIView::~CNVItemUIView()
{
}

BOOL CNVItemUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CNVItemUIView drawing

void CNVItemUIView::OnDraw(CDC* /*pDC*/)
{
	CNVItemUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CNVItemUIView printing

BOOL CNVItemUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CNVItemUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CNVItemUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CNVItemUIView diagnostics

#ifdef _DEBUG
void CNVItemUIView::AssertValid() const
{
	CView::AssertValid();
}

void CNVItemUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CNVItemUIDoc* CNVItemUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNVItemUIDoc)));
	return (CNVItemUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CNVItemUIView message handlers
