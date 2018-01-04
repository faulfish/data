/*********************************************************************/
/** @file:	QPortingTreeUI.h

Defines the CQPortingTreeUI class.

$Original Author:   Steven CC Chen  $
$Version:	1.0 $
$Date:   Feb 25 2008 22:06:04  $
*/
/*********************************************************************/

#pragma once

#include "..\CommUIExportAPI.h"
#include "..\interface\IQTreeUI.h"
#include "QUIDropTraget.h"
#include "..\interface\CQCommonTreeCtrl.h"
#include "../../../CommonUtil/NotifyBase.h"
#include "UIOleDataSource.h"
#include "../../../CommonUtil/CommonUtility.h"
#include "..\..\..\Components\Localization\ilocalization.h"

namespace QSYNC_LIB
{

#ifndef TVIS_FOCUSED
#define TVIS_FOCUSED 0x0001
#endif

#ifndef TVIS_CHECKED
#define TVIS_CHECKED 0x2000
#endif

#define WM_USER_RBUTTONDOWN WM_USER + 1001

	typedef CMap<CQTreeNode*,CQTreeNode*,HTREEITEM,HTREEITEM> TreeNodeAssocMap;  //(<Key,Value> = <Data TreeNode Pointer,UI TreeNode Handle>; supports up to max 4 bytes entries.)

	class COMMONUI_API CQPortingTreeUI : public CTreeCtrl, public IQTreeUI, public CNotifyBase,  public INotify
	{
		friend class CQCommonTreeCtrl;
		DECLARE_DYNAMIC(CQPortingTreeUI)

	public:
		CQPortingTreeUI();
		virtual ~CQPortingTreeUI();
		virtual CQTreeNode* GetCurrentNode();

		// 使用者(任何view)設定list元件是否可以drop
		virtual long SetDropable(bool bIsDropable);

	protected:
		DECLARE_MESSAGE_MAP()

		// Class Private Methods and Operations
	private:
		bool ExpandBranch(HTREEITEM hHeadTreeItem,bool bExpand);
		BOOL DeleteSelectedItem(void);
		void HasChile(HTREEITEM hTreeItem,bool bHasChild);
		void DrawItems( CDC* pDC );
		void DrawBackGround( CDC* pDC );
		void DrawButton( CDC* pDC, bool bOpen,const CRect& rcButton);
		void DrawIcon( CDC* pDC, bool bSelected,CQTreeNode* pNode,const CRect& rcIcon);
		void DrawString( CDC* pDC, bool bSelected, bool bFocus,const CRect& rcItem , const CString& strTemp);
		void	HandleDragDropEvent(long nParam);
		std::wstring GetPicturePath();
		void RecursiveUpdate(CQTreeNode*);

		// Class Public Methods and Operations
	protected:
		// To implemtnet IQTreeUI interface
		virtual bool CreateNode(CQTreeNode* pParentNode, CQTreeNode* pNewNode);
		virtual bool DeleteNode(CQTreeNode* pNode);
		virtual bool ExpandNode(CQTreeNode* pNode, bool bExpandFlag);
		virtual CQTreeNode* GetRoot();
		virtual bool UpdateNode(CQTreeNode* pNode);
		// To implemtnet INotifyBase interface
		virtual bool Register(EventFunc* pEventFunc, const TSTRING& strEvent);
		virtual bool Register(INotify* pINotify, const TSTRING& strEvent);
		virtual bool Unregister(INotify* pINotify, const TSTRING& strEvent);
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING& strEvent);
		virtual void Event(const TSTRING& strEvent, long nParam);
		virtual void RefreshClear();		

		// Windows Override Functions
	protected:
		virtual void PreSubclassWindow();
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		// Windiows Message Handlers
	public:
		afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnLButtonUp( UINT, CPoint );
		afx_msg void OnMouseMove( UINT, CPoint );
		afx_msg LRESULT OnUSER_RBUTTONDOWN(WPARAM wpara, LPARAM lpara);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnPaint();
		afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnKillFocus( CWnd* );
		afx_msg void OnSetFocus( CWnd* );

	public:
		//	DECLARE_MESSAGE_MAP()

		// Class Private Data Members
	private:
		TreeNodeAssocMap	m_nodeMap;
		CUIDropTarget		m_DropTarget;
		CUIOleDataSource	m_DataSrc;	
		COMMONCONTROL::GlobalCreater<COMMONCONTROL::_GdiplusCreaterPolicy> m_GDIPLUS;
		CRect				m_rect;						// The client rect when drawing
		int					m_h_offset;					// 0... -x (scroll offset)
		int					m_h_size;						// width of unclipped window
		int					m_v_offset;					// 0... -y (scroll offset)
		int					m_v_size;						// height of unclipped window
		CImageList*			m_pImageList;
		ILocalization*			m_pILocalization;
	};

}