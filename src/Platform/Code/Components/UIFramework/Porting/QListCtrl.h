///////////////////////////////////////////////////////////
//  CQListBox.h
//  Implementation of the Class CQListCtrl
//  Created on:      08-Feb-2007 08:08:14
//  Original author: Eddy Guo
///////////////////////////////////////////////////////////

#if !defined(BENQ_EA9A2A33_53A0_45e4_A1F8_09876F272925__INCLUDED_)
#define BENQ_EA9A2A33_53A0_45e4_A1F8_09876F272925__INCLUDED_

#include "../../../CommonUtil/CommonUtility.h"
#include "../../../CommonUtil/NotifyBase.h"
#include "../../ConfigureTool/IConfigure.h"
#include "../CommUIExportAPI.h"
#include "../interface/IQCommListCtrl.h"
#include "../interface/IQListCtrlUI.h"
#include "../interface/QControlNotify.h"
#include "..\..\..\Components\Localization\ilocalization.h"
#include "QUIDropTraget.h"
#include "UIOleDataSource.h"

namespace QSYNC_LIB {

#define WM_USER_RBUTTONDOWN WM_USER + 1001

	class COMMONUI_API CQListCtrl : public CListCtrl, public INotify, public CNotifyBase, public IQListCtrlUI 
	{
		friend class CQCommListCtrl;
		DECLARE_DYNAMIC(CQListCtrl);
	public:
		CQListCtrl();
        //CQListCtrl(CString strAppViewName);
		virtual ~CQListCtrl();

		IQListCtrlUI* GetIQListCtrlUI() 
		{
			//for vptr mapping to base interface
			return static_cast<IQListCtrlUI*>(this);
		}	
        //virtual bool SetAppViewName(LPCTSTR strAppViewName);

	protected:
		// 插入一個row
		virtual int InsertDataByIndex(CQListData* data, int nRowIndex = -1);
		// 插入一個cell
		virtual bool InsertDataByRowColumn(CBasicData* data, int nRow, int nCol);
		// 更新一個row
		virtual bool UpdateRowByIndex(int nRow, CQListData* data);
		// 更新一個cell
		virtual bool UpdateDataByRowColumn(CBasicData* data, int nRow, int nCol);
		// 刪除一個row
		virtual bool DeleteRowByIndex(int nRow);
		// 取得現在的row count
		virtual int GetRowCount();
		// 排序
		virtual bool Order(int nColumn, int sortType);

		virtual bool Register(EventFunc* pEventFunc, const TSTRING &strEvent) 
		{
			return CNotifyBase::Register(pEventFunc, strEvent);
		}
		virtual bool Register(INotify* pINotify, const TSTRING &strEvent) 
		{
			return CNotifyBase::Register(pINotify, strEvent);
		}
		virtual bool Unregister(INotify* pINotify, const TSTRING &strEvent) 
		{
			return CNotifyBase::Unregister(pINotify, strEvent);
		}
		virtual bool Unregister(EventFunc* pEventFunc, const TSTRING &strEvent) 
		{
			return CNotifyBase::Unregister(pEventFunc, strEvent);
		}

		virtual void Event(const TSTRING& strEvent, long nParam);
		// 插入一個column
		virtual bool InsertColumn(int nIndex, ColumnData* pColumnData);
		// 刪除指定index的column
		virtual bool DeleteColumnByIndex(int nIndex);
		virtual bool DeleteColumnByKey(const TSTRING& strKey);
		virtual bool UpdateColumnByIndex(int nIndex, ColumnData* pColumnData);
		virtual bool UpdateColumnByKey(const TSTRING& strKey, const TSTRING& vlaue);
		virtual bool GetColumnByIndex(int nIndex, ColumnData* pColumnData);
		virtual bool GetColumnByKey(const TSTRING& strKey, ColumnData* pColumnData);
		// 取得目前UI的column count
		virtual int GetColumnCount();

		// 使用者(任何view)設定list元件是否可以drop
		virtual long SetDropable(bool bIsDropable);

		// 設定UI的風格
		virtual DWORD SetExtendedStyle(DWORD dwStyle);

		virtual bool GetSelectedItem(vector<fdp>& vectSelected);
		virtual void RefreshClear();

	private:
		bool CreateLVITEMFromBasicData(CBasicData* data, LVITEM* item);
		bool CreateLVCOLUMNFromColumnData(ColumnData* data, LVCOLUMN* item,TSTRING& strDisplay);
		bool GetRowByIndex(int nRowIndex,CQListData** ppListData);
		bool GetColumnIndexByKey(const TSTRING& key, int& nColIndex);
		void DrawSubItem(Graphics& graphics,const CRect& rcItem,CBasicData* pBasicData,bool bSelected, bool bFocus);
		void DrawItemBackground(Graphics& graphics,const CRect& rcItem,bool bSelected, bool bFocus);
		int  DrawImage(Graphics& graphics,const CRect& rcSubItem,CBasicData *pBasicData);
		CRect GetSubItemRectEx(int nItemIndex,int nSubItemIndex);
		Font* GetDefaultFont(Graphics& graphics);
		std::wstring GetPicturePath();

	private:
		typedef	std::vector<ColumnData*> ColumnArray;
		ColumnArray         m_mapheader;
		CUIDropTarget       m_DropTarget;
		CUIOleDataSource    m_DataSrc;		
		ILocalization*      m_pILocalization;
		COMMONCONTROL::GlobalCreater<COMMONCONTROL::_GdiplusCreaterPolicy> m_GDIPLUS;
        //CString m_strAppViewName;
        //IConfigure* m_pCfgTool;

	public:
		DECLARE_MESSAGE_MAP()
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		afx_msg void OnHdnItemclick(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnDestroy();
		afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnLvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnKeyDown( UINT, UINT, UINT );
		afx_msg LRESULT OnUSER_RBUTTONDOWN(WPARAM, LPARAM);
		afx_msg void OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg void OnKillFocus( CWnd* );
		afx_msg void OnSetFocus( CWnd* );

	protected:
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void PreSubclassWindow();
		virtual void DrawItem(LPDRAWITEMSTRUCT);
public:
    afx_msg void OnSize(UINT nType, int cx, int cy);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};

}
#endif // !defined(BENQ_EA9A2A33_53A0_45e4_A1F8_09876F272925__INCLUDED_)
