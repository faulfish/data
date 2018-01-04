// NVItemListView.cpp : implementation file
//

#include "stdafx.h"
#include "NVItemUI.h"
#include "NVItemListView.h"


// CNVItemListView

IMPLEMENT_DYNCREATE(CNVItemListView, CListView)

CNVItemListView::CNVItemListView()
{
	m_list= &GetListCtrl();
}

CNVItemListView::~CNVItemListView()
{
}

void CNVItemListView::OnInitialUpdate(void)
{
	CListView::OnInitialUpdate();

	DWORD style = m_list->GetStyle();
	//style |= LVS_EDITLABELS| WS_CAPTION; /*ctlTree.GetStyle()*/;
	//m_list->ModifyStyle(0,style);
	CRect rect;
	GetClientRect(&rect);
	m_list->ModifyStyle(LVS_TYPEMASK,LVS_REPORT&LVS_TYPEMASK);
	m_list->InsertColumn(0,_T("Step Info"),LVCFMT_LEFT,rect.Width());
	m_list->InsertColumn(1,_T("Result"),LVCFMT_LEFT,rect.Width());
}
BEGIN_MESSAGE_MAP(CNVItemListView, CListView)
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, &CNVItemListView::OnNMCustomdraw)
END_MESSAGE_MAP()


// CNVItemListView diagnostics

#ifdef _DEBUG
void CNVItemListView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CNVItemListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNVItemListView message handlers

void CNVItemListView::ShowMesInfo(const char* szInfo,int flag)
{

	int nInsertPos = m_list->GetItemCount();

	int iItem = m_list->InsertItem(nInsertPos,_T(""));

	//m_list->SetItemText(nInsertPos,0,m_Step++); 
	m_list->SetItemText(nInsertPos,0,szInfo);
 	if(flag<0)
	{
		m_list->SetItemText(nInsertPos,1,_T("ERROR"));
		SetItemColor(nInsertPos, RGB(224, 0, 0));
	}else
	{
		m_list->SetItemText(nInsertPos,1,_T("SUCCESS"));
		SetItemColor(nInsertPos, RGB(128, 128, 128));
	}
	m_list->EnsureVisible(iItem, TRUE);
}

void CNVItemListView::SetItemColor(DWORD iItem, COLORREF color)
{

	MapItemColor.SetAt(iItem, color);
	m_list->RedrawItems(iItem, iItem);
	UpdateWindow();

}
void CNVItemListView::OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = CDRF_DODEFAULT;
	NMLVCUSTOMDRAW * lplvdr=(NMLVCUSTOMDRAW*)pNMHDR;
	NMCUSTOMDRAW &nmcd = lplvdr->nmcd;

	switch(lplvdr->nmcd.dwDrawStage)
	{
	case CDDS_PREPAINT:

		*pResult = CDRF_NOTIFYITEMDRAW;
		break;

	case CDDS_ITEMPREPAINT:
		{
			COLORREF ItemColor;
			if( MapItemColor.Lookup(nmcd.dwItemSpec, ItemColor))
			{
				lplvdr->clrText = ItemColor;
				*pResult = CDRF_DODEFAULT;
			}
		}
		break;


	}
}
