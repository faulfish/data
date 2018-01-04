// NVItemTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "NVItemUI.h"
#include "NVItemTreeView.h"
#include "NVItemDataView.h"
#include "ChildFrm.h"
#include "NVItemUIDoc.h"

// CNVItemTreeView

IMPLEMENT_DYNCREATE(CNVItemTreeView, CTreeView)

CNVItemTreeView::CNVItemTreeView()
{
	myTreeCtrl = &GetTreeCtrl();
	mNVItemId =_T("");
}

CNVItemTreeView::~CNVItemTreeView()
{
}

BEGIN_MESSAGE_MAP(CNVItemTreeView, CTreeView)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CNVItemTreeView::OnTvnSelchanged)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CNVItemTreeView::OnNMRclick)
	ON_COMMAND(ID_SORTBYID_SORTBYID, &CNVItemTreeView::OnSortAscbyid)
	ON_COMMAND(ID_SORT_SORTDESBYID, &CNVItemTreeView::OnSortSortdesbyid)
	ON_COMMAND(ID_GETNVITEM_GETNVITEMFROMPHONE, &CNVItemTreeView::OnGetnvitemGetnvitemfromphone)
END_MESSAGE_MAP()

void CNVItemTreeView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();
	DWORD style = GetTreeCtrl().GetStyle();
	style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;
	GetTreeCtrl().ModifyStyle(0,style);


	image.Create(16, 16, ILC_COLOR32 , 4, 0);//(IDB_IMAGELIST,30,0,RGB(255,0,255));
	CBitmap bitmap;

	bitmap.LoadBitmap(IDB_BITMAP1);
	image.Add(&bitmap,COLOR_WINDOW);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_BITMAP2);
	image.Add(&bitmap,COLOR_WINDOW);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_BITMAP4);
	image.Add(&bitmap,COLOR_WINDOW);
	bitmap.DeleteObject();

	bitmap.LoadBitmap(IDB_BITMAP3);
	image.Add(&bitmap,COLOR_WINDOW);
	bitmap.DeleteObject();


	myTreeCtrl->SetImageList(&image,TVSIL_NORMAL);

}

// CNVItemTreeView diagnostics

#ifdef _DEBUG
void CNVItemTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CNVItemTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CNVItemTreeView message handlers

void CNVItemTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{

	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here

	CChildFrame * frame  = (CChildFrame * )this->GetParentFrame();
	CNVItemDataView * pDateView= (CNVItemDataView*) frame->m_wndSplitter2.GetPane(0,0);

	HTREEITEM selected = GetTreeCtrl().GetSelectedItem();
	CNVItemUIDoc *doc = (CNVItemUIDoc *)this->GetDocument();
	//pDateView->GetEditCtrl().DeleteTempMap();

	mNVItemId = GetTreeCtrl().GetItemText(selected);
	mNVItemId = mNVItemId.Left(mNVItemId.Find(_T(")")));
	mNVItemId = mNVItemId.Right(mNVItemId.GetLength()-mNVItemId.Find(_T("="))-1);
	if(mNVItemId!=_T(""))
	{
		doc->GetNVItem(atoi(mNVItemId));
		pDateView->GetHexEdit().SetData(atoi(mNVItemId),doc->GetNVItemData(),doc->GetNVItemLen());
		//pDateView->GetHexEdit().OnPaint();
		pDateView->GetMobileEdit().SetData(0,NULL,0);
	}else 
	{
		pDateView->GetHexEdit().SetData(0,NULL,0);
		pDateView->GetMobileEdit().SetData(0,NULL,0);
	}
	*pResult = 0;
}

void CNVItemTreeView::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	//*pResult = 0;
  

	DWORD  dw  = GetMessagePos(); 
	CPoint pt(LOWORD(dw),HIWORD(dw));
	myTreeCtrl->ScreenToClient(&pt); 
	HTREEITEM hItem = myTreeCtrl->HitTest(pt); 
	myTreeCtrl->SelectItem(hItem);
 
	CMenu menu ,* pSubMenu;
	 menu.LoadMenu(IDR_MENU1);
	 if(mNVItemId==_T(""))
	 {
		pSubMenu = menu.GetSubMenu(0);
	 }else {
		pSubMenu = menu.GetSubMenu(1);
	 }
	 pSubMenu->TrackPopupMenu(TPM_LEFTALIGN, LOWORD(dw), HIWORD(dw), this);
}


void CNVItemTreeView::OnSortAscbyid()
{
	// TODO: Add your command handler code here
	CNVItemUIDoc *doc = (CNVItemUIDoc *)this->GetDocument();
	doc->SortAscNVItemArray();
}

void CNVItemTreeView::OnSortSortdesbyid()
{
	// TODO: Add your command handler code here
	CNVItemUIDoc *doc = (CNVItemUIDoc *)this->GetDocument();
	doc->SortDesNVItemArray();
}

void CNVItemTreeView::OnGetnvitemGetnvitemfromphone()
{
	// TODO: Add your command handler code here
	CChildFrame * frame  = (CChildFrame * )this->GetParentFrame();
	CNVItemDataView * pDateView= (CNVItemDataView*) frame->m_wndSplitter2.GetPane(0,0);
	CNVItemUIDoc *doc = (CNVItemUIDoc *)this->GetDocument();
	pDateView->GetMobileEdit().SetIsEdit(FALSE);
	if(doc->GetMoblieNVItemByID()>=0)
		pDateView->GetMobileEdit().SetData(atoi(mNVItemId),doc->GetMobileNVItem()->GetItemContent(),doc->GetMobileNVItem()->GetNVContentLen());
	else 
		pDateView->GetMobileEdit().SetData(0,NULL,0);
}
