
/*********************************************************************/
/** @file:	QPortingTreeUI.cpp

Implements the CQPortingTreeUI class.

$Original Author:   Steven CC Chen  $
$Version:	1.0 $
$Date:   Apr 01 2009 14:17:26  $
*/
/*********************************************************************/

// QPortingTreeUI.cpp : implementation file
//

#include "stdafx.h"
#include "QPortingTreeUI.h"
#include "../Interface/CommonUIParameter.h"
#include "../../../CommonUtil/CommonDrawer.h"
#include "../resource.h"

#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)

namespace QSYNC_LIB
{
	std::wstring CQPortingTreeUI::GetPicturePath()
	{
#ifdef _DEBUG
		std::wstring strImagePath = L"..\\Debug\\Pictures2\\";
#else
		std::wstring strImagePath = L"Pictures2\\";
#endif
		return strImagePath;
	}

	IMPLEMENT_DYNAMIC(CQPortingTreeUI, CTreeCtrl)

		BEGIN_MESSAGE_MAP(CQPortingTreeUI, CTreeCtrl)
			ON_NOTIFY_REFLECT(TVN_DELETEITEM, &CQPortingTreeUI::OnTvnDeleteitem)
			ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CQPortingTreeUI::OnTvnSelchanged)
			ON_WM_LBUTTONDOWN()
			ON_WM_LBUTTONUP()
			ON_WM_RBUTTONDOWN()
			ON_WM_LBUTTONDBLCLK()
			ON_WM_MOUSEMOVE()
			ON_WM_MOVE()
			ON_WM_KILLFOCUS()
			ON_WM_SETFOCUS()
			ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CQPortingTreeUI::OnBegindrag)
			ON_MESSAGE( WM_USER_RBUTTONDOWN, OnUSER_RBUTTONDOWN )
			ON_WM_ERASEBKGND()
			ON_WM_PAINT()
			ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
			ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
		END_MESSAGE_MAP()



		CQPortingTreeUI::CQPortingTreeUI()
		:m_pImageList(NULL)
		,m_pILocalization(NULL)
		,m_pCfgDefaultColor(CreateIConfigure(_T("DefaultColor")))
		,m_NormalItemColor(0,0,0)
		,m_SelectedItemColor(0,128,192)
		,m_SelectedUnfocusColor(128,128,128)
		,m_pCfgDefaultFont(CreateIConfigure(_T("DefaultFont")))
		,m_bNormalItemBold(false)
		,m_bSelectedItemBold(false)
		,m_bSelectedUnfocusItemBold(false)
	{
		DefineNotify(EVENT_TREE_DELETE_NODE);
		DefineNotify(EVENT_TREE_INSERT_NODE);
		DefineNotify(EVENT_TREE_UPDATE_NODE);
		DefineNotify(EVENT_TREE_SELECT_NODE);
		DefineNotify(EVENT_TREE_EXPAND_NODE);
		DefineNotify(EVENT_TREE_DBCLICK_NODE);
		DefineNotify(EVENT_TREE_LBUTTONDOWN_NODE);
		DefineNotify(EVENT_TREE_RBUTTONDOWN_NODE);
		DefineNotify(EVENT_TREE_ITEM_BUTTONDOWN_NODE);
		DefineNotify(EVENT_TREE_UI_EDIT_BEGIN);
		DefineNotify(EVENT_TREE_UI_EDIT_END);
		DefineNotify(EVENT_TREE_UI_DRAG_BEGIN);	

		DefineNotify(EVENT_TREE_UI_DROP);
		DefineNotify(EVENT_DRAG_ENTER);
		DefineNotify(EVENT_DRAG_OVER);
		DefineNotify(EVENT_DRAG_LEAVE);

		DefineNotify(EVENT_TREE_UI_KILLFOCUS);
		DefineNotify(EVENT_TREE_UI_SETFOCUS);

		CreateILocalization(&m_pILocalization);
		m_pILocalization->Register(this,TNotify_Language_Change);

		if(m_pCfgDefaultColor)
		{
			CCfgInt m_nCfgTreeFontNormalR(m_pCfgDefaultColor,_T("TreeFontNormalR"),0);
			CCfgInt m_nCfgTreeFontNormalG(m_pCfgDefaultColor,_T("TreeFontNormalG"),0);
			CCfgInt m_nCfgTreeFontNormalB(m_pCfgDefaultColor,_T("TreeFontNormalB"),0);
			CCfgInt m_nCfgTreeFontFocusR(m_pCfgDefaultColor,_T("TreeFontFocusR"),0);
			CCfgInt m_nCfgTreeFontFocusG(m_pCfgDefaultColor,_T("TreeFontFocusG"),128);
			CCfgInt m_nCfgTreeFontFocusB(m_pCfgDefaultColor,_T("TreeFontFocusB"),192);
			CCfgInt m_nCfgTreeFontUnfocusR(m_pCfgDefaultColor,_T("TreeFontUnfocusR"),128);
			CCfgInt m_nCfgTreeFontUnfocusG(m_pCfgDefaultColor,_T("TreeFontUnfocusG"),128);
			CCfgInt m_nCfgTreeFontUnfocusB(m_pCfgDefaultColor,_T("TreeFontUnfocusB"),128);
			m_pCfgDefaultColor->Load(STORAGE_INIFILE);
			m_NormalItemColor.SetFromCOLORREF(RGB(m_nCfgTreeFontNormalR,m_nCfgTreeFontNormalG,m_nCfgTreeFontNormalB));
			m_SelectedItemColor.SetFromCOLORREF(RGB(m_nCfgTreeFontFocusR,m_nCfgTreeFontFocusG,m_nCfgTreeFontFocusB));
			m_SelectedUnfocusColor.SetFromCOLORREF(RGB(m_nCfgTreeFontUnfocusR,m_nCfgTreeFontUnfocusG,m_nCfgTreeFontUnfocusB));
		}
		if(m_pCfgDefaultFont)
		{
			m_pCfgDefaultFont->Load(STORAGE_INIFILE);
			m_bNormalItemBold = CCfgInt(m_pCfgDefaultFont,_T("TreeNormalItemBold"),0).Get() == 1;
			m_bSelectedItemBold = CCfgInt(m_pCfgDefaultFont,_T("TreeSelectedItemBold"),1).Get() == 1;
			m_bSelectedUnfocusItemBold = CCfgInt(m_pCfgDefaultFont,_T("TreeSelectedUnfocusItemBold"),0).Get() == 1;
		}
	}


	CQPortingTreeUI::~CQPortingTreeUI()
	{
		if(m_pImageList)
		{
			m_pImageList->DeleteImageList();
			delete m_pImageList;
			m_pImageList = NULL;
		}

		if(m_pILocalization)
			m_pILocalization->Unregister(this,TNotify_Language_Change);

		if(m_pCfgDefaultColor)
			DeleteIConfigure(_T("DefaultColor"));
		if(m_pCfgDefaultFont)
			DeleteIConfigure(_T("DefaultFont"));
	}

	void CQPortingTreeUI::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
	{
		TV_DISPINFO* pDispInfo = (TV_DISPINFO*)pNMHDR;
		CTreeEditParam editParam;
		editParam.SetNode((CQTreeNode*)GetItemData(pDispInfo->item.hItem));
		Fire(EVENT_TREE_UI_EDIT_BEGIN,(long)&editParam);
		*pResult = editParam.GetEditable() ? 0 : 1;
	}

	void CQPortingTreeUI::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
	{
		TV_DISPINFO* pDispInfo = (TV_DISPINFO*)pNMHDR;

		if(pDispInfo->item.pszText)
		{
			CTreeEditParam editParam;
			editParam.SetNode((CQTreeNode*)GetItemData(pDispInfo->item.hItem));
			editParam.SetEditString(pDispInfo->item.pszText);		
			Fire(EVENT_TREE_UI_EDIT_END,(long)&editParam);
			*pResult = editParam.GetFinishEdit() ? 1 : 0;
		}
	}

	bool CQPortingTreeUI::ExpandBranch(HTREEITEM hHeadTreeItem,bool bExpand)
	{
		bool bRet = false;

		if( hHeadTreeItem && ItemHasChildren(hHeadTreeItem) )
		{
			this->Expand(hHeadTreeItem, bExpand ? TVE_EXPAND : TVE_COLLAPSE);

			hHeadTreeItem = GetChildItem(hHeadTreeItem);
			do
			{
				ExpandBranch(hHeadTreeItem,bExpand);
			}
			while( (hHeadTreeItem = GetNextSiblingItem(hHeadTreeItem)) != NULL );

			bRet = true;
		}

		return bRet;
	}


	BOOL CQPortingTreeUI::DeleteSelectedItem(void)
	{
		// (**To prevent from deleting the Tree node directly, and fire the "EVENT_TREE_DELETE_NODE" event that includes the info about the Tree node to be deleted instead; here using the "CQTreeNode*" pointer as the info.)
		// (*Here using the HTREEITEM handle to map Tree UI nodes with Tree Data nodes.)
		BOOL bRet = FALSE;

		HTREEITEM hSeletcedTreeItem = this->GetSelectedItem();
		if(hSeletcedTreeItem != NULL)
		{
			CQTreeNode* pQTreeNode = (CQTreeNode*)GetItemData(hSeletcedTreeItem);
			if(pQTreeNode)
			{
				Fire(EVENT_TREE_DELETE_NODE, reinterpret_cast<long>(pQTreeNode));
				bRet = TRUE;
			}
		}
		return bRet;
	}


	bool CQPortingTreeUI::CreateNode(CQTreeNode* pParentNode, CQTreeNode* pNewNode)
	{
		bool bRet = false;
		HTREEITEM hNewTreeItem = NULL;

		if(pParentNode == pNewNode || pNewNode == NULL)
		{
			TRACE("[CQPortingTreeUI::CreateNode]: Can not insert a new node to itself or new node is invaild.\n");
		}
		else
		{
			HTREEITEM hParentNode		= NULL;
			if(pParentNode != NULL)
			{
				HTREEITEM hParentTreeItem = NULL;
				if( m_nodeMap.Lookup(pParentNode,hParentTreeItem) )
				{
					HasChile(hParentTreeItem,true);
					hParentNode = hParentTreeItem;
				}
			}
			hNewTreeItem = InsertItem(0, hParentNode, NULL);

			if(hNewTreeItem != NULL)
			{
				HasChile(hNewTreeItem,pNewNode->m_bHasChild);
				m_nodeMap.SetAt( pNewNode, hNewTreeItem );
				bRet = UpdateNode(pNewNode);
			}
		}

		return bRet;
	}

	void CQPortingTreeUI::HasChile(HTREEITEM hTreeItem,bool bHasChild)
	{
		TVITEM item;
		memset(&item,0,sizeof(TVITEM));
		item.hItem = hTreeItem;
		item.mask = TVIF_CHILDREN;
		GetItem(&item);
		if(item.cChildren != (bHasChild ? 1 : 0))
		{
			item.cChildren = bHasChild ? 1 : 0;
			SetItem(&item);
		}
	}

	bool CQPortingTreeUI::DeleteNode(CQTreeNode* pNode)
	{
		bool bRet = false;
		if(pNode == NULL)
		{
			if(TRUE == this->DeleteAllItems())
			{
				m_nodeMap.RemoveAll();
				bRet = true;
			}
		}
		else
		{
			HTREEITEM hHeadTreeItem = NULL;
			if( m_nodeMap.Lookup(pNode,hHeadTreeItem) )
			{
				m_nodeMap.RemoveKey(pNode);
				HTREEITEM hParentItem = GetParentItem( hHeadTreeItem );
				if(TRUE == DeleteItem(hHeadTreeItem))
				{
					if(hParentItem)
					{
						HTREEITEM hCurSel = GetNextItem(hParentItem,TVGN_CHILD);
						HasChile(hParentItem,hCurSel != NULL);
					}
					bRet = true;
				}
			}
			else
			{
				//delete pNode;
				//  02/07 Jame Tai : It is not the jurisdiction of this function.
				assert(0); 
			}
		}
		return bRet;
	}


	bool CQPortingTreeUI::ExpandNode(CQTreeNode* pNode, bool bExpandFlag)
	{
		HTREEITEM hHeadTreeItem = TVGN_ROOT;

		if(pNode)
			m_nodeMap.Lookup(pNode,hHeadTreeItem);

		return ExpandBranch(hHeadTreeItem,bExpandFlag);
	}


	CQTreeNode* CQPortingTreeUI::GetRoot()
	{
		HTREEITEM hRootTreeItem = NULL;
		CQTreeNode* pQTreeNode = NULL;	//(Key)		

		hRootTreeItem = GetRootItem();
		if(hRootTreeItem)
			pQTreeNode = (CQTreeNode*)GetItemData(hRootTreeItem);

		return pQTreeNode;
	}


	bool CQPortingTreeUI::UpdateNode(CQTreeNode* pNode)
	{
		bool bRet = false;

		if(pNode != NULL)
		{			
			TSTRING strDisplay = pNode->GetDisplayName().c_str();
			if(m_pILocalization)
				m_pILocalization->GetString(strDisplay,strDisplay);

			UINT nMask					= (TVIF_HANDLE | TVIF_TEXT | TVIF_STATE | TVIF_PARAM);  //(TVIF_CHILDREN not used here.)
			LPCTSTR lpszItemText		= strDisplay.c_str();
			UINT nStateMask				= (TVIS_FOCUSED | TVIS_SELECTED | TVIS_CHECKED | TVIS_CUT | TVIS_DROPHILITED | TVIS_EXPANDED | TVIS_EXPANDEDONCE | TVIS_EXPANDPARTIAL);
			UINT nState					= 0;
			LPARAM lParam				= (LPARAM)pNode;
			int nImage					= pNode->m_nUnselectedImageID;
			int nSelectedImage			= pNode->m_nSelectedImageID;

			HTREEITEM hTreeItem			= NULL;
			if( m_nodeMap.Lookup(pNode,hTreeItem) && (hTreeItem != NULL) )
			{
				if(pNode->m_bFocus)
					nState |= TVIS_FOCUSED;
				else
					nState &= ~TVIS_FOCUSED;
				if(pNode->m_bSelected)
					nState |= TVIS_SELECTED;
				else
					nState &= ~TVIS_SELECTED;
				if(pNode->m_bChecked)
					nState |= TVIS_CHECKED;
				else
					nState &= ~TVIS_CHECKED;

				if(pNode->m_strImagePath.size() > 0)
				{
					nMask |= TVIF_IMAGE;
					nMask |= TVIF_SELECTEDIMAGE;
					CImageList* pImageList = GetImageList(TVSIL_NORMAL);
					if(pImageList == NULL)
					{
						pImageList = new CImageList;
						pImageList->Create(16, 16, ILC_COLOR, 0, 4);
						HICON hIcon = AfxGetApp()->LoadIcon(IDI_DEFAULT);
						assert(hIcon);
						int nRet = pImageList->Add(hIcon);
						assert(nRet != -1);
						SetImageList(pImageList,TVSIL_NORMAL);
						m_pImageList = pImageList;
					}
				}

				if(TRUE == SetItem(hTreeItem, nMask, lpszItemText, nImage, nSelectedImage, nState, nStateMask, lParam) )
					bRet = true;
			}
		}
		return bRet;
	}


	bool CQPortingTreeUI::Register(EventFunc* pEventFunc, const TSTRING& strEvent)
	{
		return CNotifyBase::Register(pEventFunc, strEvent);
	}

	bool CQPortingTreeUI::Register(INotify* pINotify, const TSTRING& strEvent)
	{
		return CNotifyBase::Register(pINotify, strEvent);
	}

	bool CQPortingTreeUI::Unregister(INotify* pINotify, const TSTRING& strEvent)
	{
		return CNotifyBase::Unregister(pINotify, strEvent);
	}

	bool CQPortingTreeUI::Unregister(EventFunc* pEventFunc, const TSTRING& strEvent)
	{
		return CNotifyBase::Unregister(pEventFunc, strEvent);
	}

	void CQPortingTreeUI::PreSubclassWindow()
	{
		BOOL bRet = m_DropTarget.Register(this);
		if(!bRet) {
			TRACE("[CQPortingTreeUI::PreSubclassWindow]: To register itself as a Drop Target fail.\n");
		}
		__super::PreSubclassWindow();
	}

	BOOL CQPortingTreeUI::PreTranslateMessage(MSG* pMsg)
	{
		switch(pMsg->message)
		{
		case WM_KEYDOWN:
			{
				switch(pMsg->wParam)
				{
				case VK_DELETE:
					if( (m_hWnd != NULL) && ::IsWindowEnabled(m_hWnd) )
						DeleteSelectedItem();
					break;
				default:
					break;
				}
			}
			break;
		default:
			break;
		};

		return __super::PreTranslateMessage(pMsg);
	}

	long CQPortingTreeUI::SetDropable(bool bIsDropable) {

		long nRet = 0;
		long nStyle = ::GetWindowLong(m_hWnd, GWL_EXSTYLE);
		if(bIsDropable) 
		{                
			nStyle |= WS_EX_ACCEPTFILES;
			m_DropTarget.Register(this, EVENT_DROP);
            m_DropTarget.Register(this, EVENT_DRAG_ENTER);
            m_DropTarget.Register(this, EVENT_DRAG_OVER);
            m_DropTarget.Register(this, EVENT_DRAG_LEAVE);
		} 
		else 
		{
			nStyle &= ~WS_EX_ACCEPTFILES;
			m_DropTarget.Unregister(this, EVENT_DROP);
            m_DropTarget.Unregister(this, EVENT_DRAG_ENTER);
            m_DropTarget.Unregister(this, EVENT_DRAG_OVER);
            m_DropTarget.Unregister(this, EVENT_DRAG_LEAVE);
		}
		nRet = ::SetWindowLong(m_hWnd, GWL_EXSTYLE, nStyle);
		m_DropTarget.SetDropable(bIsDropable);
		return nRet;
	}

	void CQPortingTreeUI::OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult)
	{
		// (To fire the "EVENT_TREE_DELETE_NODE" event that includes the info about the Tree node to be deleted instead; here using the "CQTreeNode*" pointer as the info.)
		// (*Here using the HTREEITEM handle to map Tree UI nodes with Tree Data nodes.)
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		HTREEITEM hDeleteTreeItem = pNMTreeView->itemNew.hItem;		
		*pResult = 0;
	}

	void CQPortingTreeUI::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
	{
		// (To fire the "EVENT_TREE_SELECT_NODE" event that includes the info about the selected Tree node; here using the "CQTreeNode*" pointer as the info.)
		// (*Here using the HTREEITEM handle to map Tree UI nodes with Tree Data nodes.)
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		HTREEITEM hSelectedTreeItem = pNMTreeView->itemNew.hItem;
		if(hSelectedTreeItem != NULL)
		{
			CQTreeNode* pQTreeNode = NULL;	//(Key)
			pQTreeNode = (CQTreeNode*)GetItemData(hSelectedTreeItem);
			if(pQTreeNode)
				Fire(EVENT_TREE_SELECT_NODE, reinterpret_cast<long>(pQTreeNode));
		}
		*pResult = 0;
	}

	void CQPortingTreeUI::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// (To fire the "EVENT_TREE_LBUTTONDOWN_NODE" event that includes the info about the clicked Tree node; here using the "CQTreeNode*" pointer as the info.)
		// (*Here using the HTREEITEM handle to map Tree UI nodes with Tree Data nodes.)
		UINT uFlags;
		HTREEITEM hLBDownTreeItem = HitTest(point, &uFlags);
		if((hLBDownTreeItem != NULL) && (uFlags & TVHT_ONITEMBUTTON))
		{
			CQTreeNode* pQTreeNode = NULL;	//(Key)
			pQTreeNode = (CQTreeNode*)GetItemData(hLBDownTreeItem);
			if(pQTreeNode)
				Fire(EVENT_TREE_ITEM_BUTTONDOWN_NODE, reinterpret_cast<long>(pQTreeNode));
		}
		else if((hLBDownTreeItem != NULL) && (uFlags & TVHT_ONITEMLABEL))
		{
			CQTreeNode* pQTreeNode = NULL;	//(Key)
			pQTreeNode = (CQTreeNode*)GetItemData(hLBDownTreeItem);
			if(pQTreeNode)
				Fire(EVENT_TREE_LBUTTONDOWN_NODE, reinterpret_cast<long>(pQTreeNode));
		}

		__super::OnLButtonDblClk(nFlags, point);
	}

	void CQPortingTreeUI::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		UINT uFlags;
		HTREEITEM hDblclkTreeItem = HitTest(point, &uFlags);
		CQTreeNode* pQTreeNode = NULL;
		if(hDblclkTreeItem)
			pQTreeNode = (CQTreeNode*)GetItemData(hDblclkTreeItem);
		if(pQTreeNode)
			Fire(EVENT_TREE_DBCLICK_NODE, reinterpret_cast<long>(pQTreeNode));

		__super::OnLButtonDblClk(nFlags, point);
	}

	void CQPortingTreeUI::OnRButtonDown(UINT nFlags, CPoint point)
	{
		PostMessage(WM_USER_RBUTTONDOWN,point.x,point.y);
		__super::OnRButtonDown(nFlags, point);
	}

	LRESULT  CQPortingTreeUI::OnUSER_RBUTTONDOWN(WPARAM wpara, LPARAM lpara)
	{
		CPoint point(static_cast<int>(wpara), static_cast<int>(lpara));
		UINT nFlags;
		HTREEITEM hLBDownTreeItem = HitTest(point, &nFlags);

		//While right-clicking, let the item get selected
		CTreeCtrl::Select(hLBDownTreeItem, TVGN_CARET);
		
		if((hLBDownTreeItem != NULL))
		{
			CQTreeNode* pQTreeNode = (CQTreeNode*)GetItemData(hLBDownTreeItem);
			if(pQTreeNode)
			{
				FireUIParam<CQTreeNode> UIParm;
				UIParm.m_nFlags = nFlags;
				UIParm.m_Point  = point;
				UIParm.m_pData  = pQTreeNode;

				Fire(EVENT_TREE_RBUTTONDOWN_NODE, reinterpret_cast<long>(&UIParm));
			}
		}
		return 1;
	}

	void CQPortingTreeUI::Event(const TSTRING& strEvent, long nParam)
	{
		if(strEvent == EVENT_DROP)
		{
			HandleDragDropEvent(nParam);
			Fire(EVENT_TREE_UI_DROP, nParam);			
		}
		else if(strEvent == EVENT_DRAG_ENTER) 
		{
			HandleDragDropEvent(nParam);
            Fire(EVENT_DRAG_ENTER, nParam);
        } 
		else if(strEvent == EVENT_DRAG_OVER) 
		{
			HandleDragDropEvent(nParam);
            Fire(EVENT_DRAG_OVER, nParam);
        } 
		else if(strEvent == EVENT_DRAG_LEAVE) 
		{
			HandleDragDropEvent(nParam);
            Fire(EVENT_DRAG_LEAVE, nParam);
        }
		else if(TNotify_Language_Change == strEvent)
		{
			CQTreeNode* pNode = GetRoot();

			//cycle
			RecursiveUpdate(pNode);
		}
	}

	void CQPortingTreeUI::RecursiveUpdate(CQTreeNode* pNode)
	{
		//cycle
		if(pNode)
		{
			//1. Update self
			UpdateNode(pNode);

			//2. update all children
			int nSize = pNode->GetChildSize();
			for(int nIndex = 0;nIndex < nSize;nIndex++)
			{
				CQTreeNode* pChildNode = pNode->GetChild(nIndex);
				if(pChildNode)
					RecursiveUpdate(pChildNode);
			}
		}
	}

	void CQPortingTreeUI::HandleDragDropEvent(long nParam)
	{
		if(nParam)
		{
			CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
			UINT uFlags;
			HTREEITEM hLBDownTreeItem = HitTest(pParam->GetPoint(), &uFlags);
			if(hLBDownTreeItem)
			{
				CQTreeNode* pQTreeNode = (CQTreeNode*)GetItemData(hLBDownTreeItem);
				pParam->SetPara((void*)pQTreeNode);			
			}
		}
	}

	CQTreeNode* CQPortingTreeUI::GetCurrentNode()
	{
		CQTreeNode* pNode = NULL;
		HTREEITEM hSelectedTreeItem = GetSelectedItem();
		if(hSelectedTreeItem)
			pNode = (CQTreeNode*)GetItemData(hSelectedTreeItem);
		return pNode;
	}

	void CQPortingTreeUI::OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)	
	{
		LPNMTREEVIEW lpnmtv = (LPNMTREEVIEW) pNMHDR;

		CDragDropFireParam dragFireParam;
		Fire(EVENT_TREE_UI_DRAG_BEGIN,(long)&dragFireParam);
		*pResult = m_DataSrc.BeginDrag(dragFireParam);
	}

	void CQPortingTreeUI::OnLButtonUp( UINT, CPoint ptPoint)
	{
	}

	void CQPortingTreeUI::OnMouseMove( UINT, CPoint ptPoint)
	{
	}
	void CQPortingTreeUI::OnKillFocus( CWnd* CUnfocusWnd)
	{
		CTreeCtrl::OnKillFocus(CUnfocusWnd);
		Fire(EVENT_TREE_UI_KILLFOCUS,(long)CUnfocusWnd->GetSafeHwnd());
	}
	void CQPortingTreeUI::OnSetFocus( CWnd* CFocusWnd)
	{
		CTreeCtrl::OnSetFocus(CFocusWnd);
		Fire(EVENT_TREE_UI_SETFOCUS,(long)CFocusWnd->GetSafeHwnd());
	}


	BOOL CQPortingTreeUI::OnEraseBkgnd(CDC* pDC)
	{
		// nothing to do here -- see OnPaint
		return TRUE;
	}

	void CQPortingTreeUI::OnPaint()
	{
		CPaintDC dc(this);	// Device context for painting

		CDC dc_ff;			// Memory base device context for flicker free painting
		CBitmap bm_ff;		// The bitmap we paint into
		CBitmap *bm_old;
		CFont *font, *old_font;
		CFont fontDC;
		int old_mode;

		GetClientRect(&m_rect);
		SCROLLINFO scroll_info;
		// Determine window portal to draw into taking into account
		// scrolling position
		if ( GetScrollInfo( SB_HORZ, &scroll_info, SIF_POS | SIF_RANGE ) )
		{
			m_h_offset = -scroll_info.nPos;
			m_h_size = max( scroll_info.nMax+1, m_rect.Width());
		}
		else
		{
			m_h_offset = m_rect.left;
			m_h_size = m_rect.Width();
		}
		if ( GetScrollInfo( SB_VERT, &scroll_info, SIF_POS | SIF_RANGE ) )
		{
			if ( scroll_info.nMin == 0 && scroll_info.nMax == 100) 
				scroll_info.nMax = 0;
			m_v_offset = -scroll_info.nPos * GetItemHeight();
			m_v_size = max( (scroll_info.nMax+2)*((int)GetItemHeight()+1), m_rect.Height() );
		}
		else
		{
			m_v_offset = m_rect.top;
			m_v_size = m_rect.Height();
		}

		// Create an offscreen dc to paint with (prevents flicker issues)
		dc_ff.CreateCompatibleDC( &dc );
		bm_ff.CreateCompatibleBitmap( &dc, m_rect.Width(), m_rect.Height() );
		// Select the bitmap into the off-screen DC.
		bm_old = (CBitmap *)dc_ff.SelectObject( &bm_ff );
		// Default font in the DC is not the font used by 
		// the tree control, so grab it and select it in.
		font = GetFont();
		old_font = dc_ff.SelectObject( font );
		// We're going to draw text transparently
		old_mode = dc_ff.SetBkMode( TRANSPARENT );

		DrawBackGround( &dc_ff );
		DrawItems( &dc_ff );

		// Now Blt the changes to the real device context - this prevents flicker.
		dc.BitBlt( m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(), &dc_ff, 0, 0, SRCCOPY);

		dc_ff.SelectObject( old_font );
		dc_ff.SetBkMode( old_mode );
		dc_ff.SelectObject( bm_old );		
	}

	// Draw TreeCtrl Items
	void CQPortingTreeUI::DrawItems( CDC *pDC )
	{
		int nCount = GetCount();
		HTREEITEM show_item = GetFirstVisibleItem();
		if ( show_item == NULL )
			return;

		DWORD tree_style = ::GetWindowLong( m_hWnd, GWL_STYLE ); 

		do
		{
			int state = GetItemState( show_item, TVIF_STATE );
			CQTreeNode* pQTreeNode = (CQTreeNode*)GetItemData(show_item);
			if(pQTreeNode != NULL)
			{
				HTREEITEM parent = GetParentItem( show_item );
				bool has_children = ItemHasChildren( show_item ) || parent == NULL;
				bool bSelected = (state & TVIS_SELECTED) && (tree_style & TVS_SHOWSELALWAYS);
				bool bFocus = (this == GetFocus());

				CRect rc_item;
				if ( GetItemRect( show_item, rc_item, TRUE ) )
				{
					if ( has_children )
					{
						CRect rcButton(rc_item.left - 18, rc_item.top,rc_item.left - 2, rc_item.bottom);
						DrawButton(pDC,(state & TVIS_EXPANDED) != 0,rcButton);
					}				

					CRect rcICON(rc_item.left - 18 , rc_item.top,rc_item.left - 2 , rc_item.top + 16);
					DrawIcon(pDC,bSelected,pQTreeNode,rcICON);				

					CString strName = GetItemText( show_item );
					rc_item.DeflateRect( 0,1,0,1 );

					//For measure string width					
					int nNeedWidth = MeasureString(pDC,bSelected,bFocus,rc_item,strName);
					if(rc_item.Width() < nNeedWidth)
						rc_item.right = rc_item.left + nNeedWidth;

					DrawString(pDC,bSelected,bFocus,rc_item,strName);
				}
			}
		} while ( (show_item = GetNextVisibleItem( show_item )) != NULL );
	}

	void CQPortingTreeUI::DrawIcon( CDC* pDC, bool bSelected,CQTreeNode* pNode,const CRect& rcIcon)
	{
		using namespace COMMONCONTROL;

		//Prepare Graphics
		_SingleBufferPolicy buffer;
		Graphics graphics(buffer.BeginHDC(pDC->GetSafeHdc(),rcIcon));

		std::wstring strImage;
		if(bSelected)
			strImage = pNode->m_strSelectedImagePath;
		else			
			strImage = pNode->m_strImagePath;

		if(strImage.size() > 0)
		{
			std::wstring strImagePath = GetPicturePath() + strImage;
			Image image(strImagePath.c_str());
			Unit srcunit = UnitPixel;
			graphics.DrawImage(&image, rcIcon.left, rcIcon.top);
		}
		else
		{
			CImageList* pImageList = GetImageList(TVSIL_NORMAL);
			if(pImageList)
			{
				CPoint myPoint(rcIcon.left, rcIcon.top);
				int nImageID = bSelected ? pNode->m_nSelectedImageID : pNode->m_nUnselectedImageID;
				pImageList->Draw(pDC, nImageID, myPoint, ILD_NORMAL);
			}
		}
	}

	int CQPortingTreeUI::MeasureString(CDC* pDC,bool bSelected, bool bFocus,const CRect& rcItem,const CString& strTemp)
	{
		int nNeedWidth = 0;

		Graphics graphics(pDC->GetSafeHdc());

		// Create SolidBrush object.
		SolidBrush NormalBrush(m_NormalItemColor);
		SolidBrush FocusBrush(m_SelectedItemColor);
		SolidBrush UnfocusBrush(m_SelectedUnfocusColor);

		// Initialize arguments.
		Font* pmyFont = NULL;
		Font* pmyBoldFont = NULL;

		CFont* font = GetFont();
		assert(font);
		if (font)
		{
			LOGFONT lf;
			font->GetLogFont(&lf);
			HDC hDC = graphics.GetHDC();
			pmyFont = new Font(hDC,&lf);

			FontFamily  fontFamily(lf.lfFaceName);
			LONG nFontSize = lf.lfHeight > 0 ? lf.lfHeight : lf.lfHeight * (-1);
			assert(nFontSize > 0);
			pmyBoldFont = new Font(&fontFamily, (REAL)nFontSize, FontStyleBold, UnitPixel);

			graphics.ReleaseHDC(hDC);
		}

		if(pmyFont && pmyBoldFont)
		{
			RectF layoutRect(static_cast<float>(rcItem.left),static_cast<float>(rcItem.top),static_cast<float>(rcItem.Width()) ,static_cast<float>(rcItem.Height()));

			StringFormat stringFormat;
			stringFormat.SetAlignment(StringAlignmentNear);

			RectF rcBound;
			
			INT codepointsFitted;
			INT linesFilled;
			if(bSelected)
				if(bFocus)
					graphics.MeasureString(strTemp,strTemp.GetLength(),m_bSelectedItemBold ? pmyBoldFont : pmyFont,layoutRect,&stringFormat,&rcBound,&codepointsFitted,&linesFilled);
				else
					graphics.MeasureString(strTemp,strTemp.GetLength(),m_bSelectedUnfocusItemBold ? pmyBoldFont : pmyFont,layoutRect,&stringFormat,&rcBound,&codepointsFitted,&linesFilled);
			else
				graphics.MeasureString(strTemp,strTemp.GetLength(),m_bNormalItemBold ? pmyBoldFont : pmyFont,layoutRect,&stringFormat,&rcBound,&codepointsFitted,&linesFilled);

			nNeedWidth = static_cast<int>(rcBound.Width) + 1;

			//Tricky for measue seems not the real value 
			//Add 4 pixcels buffer for each word
			nNeedWidth += strTemp.GetLength() * 4;
		}

		return nNeedWidth;
	}

	void CQPortingTreeUI::DrawString( CDC* pDC, bool bSelected, bool bFocus,const CRect& rcItem , const CString& strTemp)
	{
		using namespace COMMONCONTROL;

		//Prepare Graphics
		_SingleBufferPolicy buffer;
		Graphics graphics(buffer.BeginHDC(pDC->GetSafeHdc(),rcItem));

		// Create SolidBrush object.
		SolidBrush NormalBrush(m_NormalItemColor);
		SolidBrush FocusBrush(m_SelectedItemColor);
		SolidBrush UnfocusBrush(m_SelectedUnfocusColor);

		// Initialize arguments.
		Font* pmyFont = NULL;
		Font* pmyBoldFont = NULL;

		CFont* font = GetFont();
		assert(font);
		if (font)
		{
			LOGFONT lf;
			font->GetLogFont(&lf);
			HDC hDC = graphics.GetHDC();
			pmyFont = new Font(hDC,&lf);

			FontFamily  fontFamily(lf.lfFaceName);
			LONG nFontSize = lf.lfHeight > 0 ? lf.lfHeight : lf.lfHeight * (-1);
			assert(nFontSize > 0);
			pmyBoldFont = new Font(&fontFamily, (REAL)nFontSize, FontStyleBold, UnitPixel);

			graphics.ReleaseHDC(hDC);
		}

		if(pmyFont && pmyBoldFont)
		{
			RectF layoutRect(static_cast<float>(rcItem.left),static_cast<float>(rcItem.top),static_cast<float>(rcItem.Width()) ,static_cast<float>(rcItem.Height()));

			StringFormat stringFormat;
			stringFormat.SetAlignment(StringAlignmentNear);

			if(bSelected)
				if(bFocus)
					graphics.DrawString(strTemp,-1,m_bSelectedItemBold ? pmyBoldFont : pmyFont,layoutRect,&stringFormat,&FocusBrush);
				else
					graphics.DrawString(strTemp,-1,m_bSelectedUnfocusItemBold ? pmyBoldFont : pmyFont,layoutRect,&stringFormat,&UnfocusBrush);
			else
				graphics.DrawString(strTemp,-1,m_bNormalItemBold ? pmyBoldFont : pmyFont,layoutRect,&stringFormat,&NormalBrush);

			delete pmyFont;
			delete pmyBoldFont;
		}
	}

	void CQPortingTreeUI::DrawButton( CDC* pDC, bool bOpen,const CRect& rcButton)
	{
		using namespace COMMONCONTROL;

		//Prepare Graphics
		_SingleBufferPolicy buffer;
		Graphics graphics(buffer.BeginHDC(pDC->GetSafeHdc(),rcButton));

		std::wstring strImage;
		if(bOpen)
			strImage = L"General\\Tree\\Tree_Open.png";
		else			
			strImage = L"General\\Tree\\Tree_Close.PNG";

		std::wstring strImagePath = GetPicturePath() + strImage;
		Image image(strImagePath.c_str());
		Unit srcunit = UnitPixel;
		graphics.DrawImage(&image, rcButton.left, rcButton.top);
	}

	void CQPortingTreeUI::DrawBackGround( CDC* pDC )
	{
		using namespace COMMONCONTROL;

		//Prepare Graphics
		_DoubleBufferPolicy doubleBuffer;
		Graphics graphics(doubleBuffer.BeginHDC(pDC->GetSafeHdc(),m_rect));

		SolidBrush bgBrush(Color(255,255,255,255));
		graphics.FillRectangle(&bgBrush,m_rect.left,m_rect.top,m_rect.Width(),m_rect.Height());
	}

	void CQPortingTreeUI::RefreshClear()
	{
		LockWindowUpdate();
		DeleteAllItems();
        m_nodeMap.RemoveAll();
		UnlockWindowUpdate();
	}
}// END OF NAMESPACE

#pragma warning(pop)