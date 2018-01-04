// QMIVIEW.cpp : implementation file
//

#include "stdafx.h"
#include "QMIVIEW.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"

namespace QSYNC_LIB
{
	// CQMIVIEW

	IMPLEMENT_DYNCREATE(CQMIVIEW, CView)

		CQMIVIEW::CQMIVIEW()
	{

	}

	CQMIVIEW::~CQMIVIEW()
	{
	}

	BEGIN_MESSAGE_MAP(CQMIVIEW, CView)
	END_MESSAGE_MAP()


	// CQMIVIEW drawing

	void CQMIVIEW::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: add draw code here
	}


	// CQMIVIEW diagnostics

#ifdef _DEBUG
	void CQMIVIEW::AssertValid() const
	{
		CView::AssertValid();
	}

#ifndef _WIN32_WCE
	void CQMIVIEW::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
#endif
#endif //_DEBUG
}

// CQMIVIEW message handlers

void QSYNC_LIB::CQMIVIEW::OnInitialUpdate()
{
	CQMIVIEW* pView = this;
	Win32DoubleBufferFormCWndWidge<CView>::OnInitialUpdate();

	StackLayout<int,HDC>* pStackLayout = new StackLayout<int,HDC>;
	pStackLayout->AddWidget(1,new WIN32ApplicationWidge(GetSafeHwnd(),_T("QMI Connection Manager"),_T("#32770"),_T("D:\\Prjbase3\\QSyncher_V2\\Other\\HM11-VB633-3A_1.2.00_2005\\ConnectionManager\\Debug\\QMICM.exe")));
	pStackLayout->Switch(1);

	SetLayout(pStackLayout);
	Widge* pLayout = GetLayout();
}
