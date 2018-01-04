// NVItemDataView.cpp : implementation file
//

#include "stdafx.h"
#include "NVItemUI.h"
#include "NVItemDataView.h"
#include "NVItemUIDoc.h"


// CNVItemDataView

IMPLEMENT_DYNCREATE(CNVItemDataView, CFormView)

CNVItemDataView::CNVItemDataView()
	: CFormView(CNVItemDataView::IDD)
{

}

CNVItemDataView::~CNVItemDataView()
{
	theApp.m_bIsNVItemViewOpen=FALSE;
	theApp.m_bIsLoadFileOpen = TRUE;
}

void CNVItemDataView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HEX_EDIT, m_hexEdit);
	DDX_Control(pDX, IDC_MOBILE_EDIT, m_mobileEdit);
}

BEGIN_MESSAGE_MAP(CNVItemDataView, CFormView)
END_MESSAGE_MAP()


// CNVItemDataView diagnostics

#ifdef _DEBUG
void CNVItemDataView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CNVItemDataView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNVItemDataView message handlers

void CNVItemDataView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	if(m_hexEdit.pOwnerView == NULL)
	{
		m_hexEdit.pOwnerView = this;
	}

	if(m_mobileEdit.pOwnerView == NULL)
	{
		m_mobileEdit.pOwnerView = this;
	}
	// TODO: Add your specialized code here and/or call the base class
}

void CNVItemDataView::SaveDataChage(int itemId,BYTE* itemContent)
{
	CNVItemUIDoc *doc = (CNVItemUIDoc *)this->GetDocument();
	doc->SaveDataChange(itemId,itemContent);
}

void CNVItemDataView::setIsEdit(bool isModify)
{
	m_hexEdit.SetIsEdit(isModify);
}
