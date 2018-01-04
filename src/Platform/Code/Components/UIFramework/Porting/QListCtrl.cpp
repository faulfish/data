///////////////////////////////////////////////////////////
//  CQListBox.cpp
//  Implementation of the Class CQListCtrl
//  Created on:      08-Feb-2007 08:08:14
//  Original author: Eddy Guo
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "QListCtrl.h"
#include "../../../CommonUtil/CommonDrawer.h"

#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)

namespace QSYNC_LIB {


	std::wstring CQListCtrl::GetPicturePath()
	{
#ifdef _DEBUG
		std::wstring strImagePath = L"..\\Debug\\Pictures2\\";
#else
		std::wstring strImagePath = L"Pictures2\\";
#endif
		return strImagePath;
	}

    IMPLEMENT_DYNAMIC(CQListCtrl, CListCtrl);

	CQListCtrl::CQListCtrl():m_pILocalization(NULL)
    {
        DefineNotify(EVENT_LIST_UI_DELETE_ROW);
        DefineNotify(EVENT_LIST_UI_INSERT_ROW);
        DefineNotify(EVENT_LIST_UI_UPDATE_ROW);
        DefineNotify(EVENT_LIST_UI_UPDATE_DATA);
        DefineNotify(EVENT_LIST_UI_SELECT_ROW);
        DefineNotify(EVENT_LIST_UI_LDBCLICK);
        DefineNotify(EVENT_LIST_UI_RDBCLICK);
        DefineNotify(EVENT_LIST_UI_LCLICK);
        DefineNotify(EVENT_LIST_UI_RCLICK);
        DefineNotify(EVENT_LIST_UI_LBTNDOWN);
        DefineNotify(EVENT_LIST_UI_RBTNDOWN);
        DefineNotify(EVENT_LIST_UI_DROP);
        DefineNotify(EVENT_LIST_UI_DRAG_ENTER);
        DefineNotify(EVENT_LIST_UI_DRAG_OVER);
        DefineNotify(EVENT_LIST_UI_DRAG_LEAVE);
        DefineNotify(EVENT_DROP);
        DefineNotify(EVENT_DRAG_ENTER);
        DefineNotify(EVENT_DRAG_OVER);
        DefineNotify(EVENT_DRAG_LEAVE);
        DefineNotify(EVENT_DRAG_BEGIN);
        DefineNotify(EVENT_LIST_UI_ITEMCHANGED);
        DefineNotify(EVENT_LIST_UI_DRAG_BEGIN);
		DefineNotify(EVENT_DRAG_END);
        DefineNotify(EVENT_LIST_UI_HEADER_BUTTON_HIT);
		DefineNotify(EVENT_LIST_UI_EDIT_BEGIN);
		DefineNotify(EVENT_LIST_UI_EDIT_END);
        DefineNotify(EVENT_LIST_UI_ONSIZE);
        DefineNotify(EVENT_LIST_UI_ONSIZING);
		DefineNotify(EVENT_LIST_UI_KILLFOCUS);
		DefineNotify(EVENT_LIST_UI_SETFOCUS);
		CreateILocalization(&m_pILocalization);
		m_pILocalization->Register(this,TNotify_Language_Change);
    }

    CQListCtrl::~CQListCtrl() 
    {
        ColumnArray::iterator iter1;
        for(iter1 = m_mapheader.begin(); iter1 != m_mapheader.end(); iter1++)
            delete *iter1;
		m_pILocalization->Unregister(this,TNotify_Language_Change);
    }

    BEGIN_MESSAGE_MAP(CQListCtrl, CListCtrl)
        ON_WM_RBUTTONDOWN()
        ON_WM_LBUTTONDBLCLK()
        ON_WM_DESTROY()
        ON_NOTIFY(HDN_ITEMCLICK, 0, &CQListCtrl::OnHdnItemclick)
        ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CQListCtrl::OnLvnBegindrag)
        ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CQListCtrl::OnLvnItemChanged)		
        ON_WM_KEYDOWN()
        ON_MESSAGE( WM_USER_RBUTTONDOWN, OnUSER_RBUTTONDOWN )		
		ON_NOTIFY_REFLECT(LVN_BEGINLABELEDIT, OnBeginlabeledit)
		ON_NOTIFY_REFLECT(LVN_ENDLABELEDIT, OnEndlabeledit)
		ON_WM_ERASEBKGND()
		ON_WM_KILLFOCUS()
		ON_WM_SETFOCUS()
        ON_WM_SIZE()
        ON_WM_SIZING()
    END_MESSAGE_MAP()

	void CQListCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
	{
		LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
		CEditParam editParam;
		editParam.SetItemIndex(pDispInfo->item.iItem);
		Fire(EVENT_LIST_UI_EDIT_BEGIN,(long)&editParam);
		*pResult = editParam.GetEditable() ? 0 : 1;
	}

	void CQListCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult)
	{
		LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

		if(pDispInfo->item.pszText)
		{
			CEditParam editParam;
			editParam.SetItemIndex(pDispInfo->item.iItem);
			editParam.SetEditString(pDispInfo->item.pszText);	
			Fire(EVENT_LIST_UI_EDIT_END,(long)&editParam);
			*pResult = editParam.GetFinishEdit() ? 1 : 0;
		}
	}
    //-------------------------------------------------------------------------
    // Event Handler for Notify String
    //-------------------------------------------------------------------------
    void CQListCtrl::Event(const TSTRING &strEvent, long nParam) 
	{
        if(strEvent == EVENT_DROP) 
		{
            Fire(EVENT_LIST_UI_DROP, nParam);
        } 
		else if(strEvent == EVENT_DRAG_ENTER) 
		{
            Fire(EVENT_LIST_UI_DRAG_ENTER, nParam);
        } 
		else if(strEvent == EVENT_DRAG_OVER) 
		{
            Fire(EVENT_LIST_UI_DRAG_OVER, nParam);
        } 
		else if(strEvent == EVENT_DRAG_LEAVE) 
		{
            Fire(EVENT_LIST_UI_DRAG_LEAVE, nParam);
        }
		else if(strEvent == EVENT_DRAG_END)
		{
			Fire(EVENT_DRAG_END,nParam);
		}
		else if(TNotify_Language_Change == strEvent)
		{
			int i = 0;
			for(ColumnArray::iterator Iter = m_mapheader.begin();Iter != m_mapheader.end();Iter++)
				UpdateColumnByIndex(i++,*Iter);
		}		
    }

    bool CQListCtrl::GetRowByIndex(int nRowIndex,CQListData** ppListData) 
    {
        bool bRes = false;
        LVITEM lvItem = {0};
        lvItem.iItem = nRowIndex;
        lvItem.mask = LVIF_PARAM;
        bRes = CListCtrl::GetItem(&lvItem) == TRUE;
        assert(lvItem.lParam != NULL);
        *ppListData = (CQListData*)lvItem.lParam;
        return bRes;
    }

    //-------------------------------------------------------------------------
    // Private method for Create LVITEM from CBAsicData
    //-------------------------------------------------------------------------
    bool CQListCtrl::CreateLVITEMFromBasicData(CBasicData* pBasicData, LVITEM* item) 
    {
        bool bRetVal = false;
        memset(item, 0, sizeof(LVITEM));
        if(pBasicData != NULL && item != NULL) 
        {
            if(pBasicData->m_strImagePath.size() !=  - 1)
                item->mask |= LVIF_IMAGE;
            item->mask |= LVIF_TEXT | LVIF_PARAM ;
			item->pszText = (LPTSTR)pBasicData->GetTextString().c_str();
            item->lParam = pBasicData->GetPARAM();
            bRetVal = true;
        }
        return bRetVal;
    }

    //-------------------------------------------------------------------------
    // Private method for Create LVCOLUMN from ColumnData
    //-------------------------------------------------------------------------
    bool CQListCtrl::CreateLVCOLUMNFromColumnData(ColumnData* pColumnData, LVCOLUMN* item,TSTRING& strDisplay)
    {
        bool bRetVal = false;

        SizeF stringSize;

        if(pColumnData && pColumnData->m_nWidth == 0)
        {
            CDC* pDC = GetDC();
            if(pDC)
            {
                Graphics graphics(pDC->GetSafeHdc());

                //Get Default font
                CFont* font = GetFont();
                if (font)
                {
                    LOGFONT lf;
                    font->GetLogFont(&lf);

                    Font font(pDC->GetSafeHdc(),&lf);
                    SizeF layoutSize(2000, 50);
                    StringFormat format;
                    format.SetAlignment(StringAlignmentFar);			
                    // Measure the string.
                    graphics.MeasureString(pColumnData->m_strDisplay.c_str(), static_cast<INT>(pColumnData->m_strDisplay.size()), &font, layoutSize, &format, &stringSize);
                }			

                ReleaseDC(pDC);
            }
        }
        memset(item, 0, sizeof(LVCOLUMN));
        if(pColumnData != NULL && item != NULL) 
        {
            item->mask |= (LVCF_TEXT | LVCF_WIDTH | LVCF_FMT | LVCF_ORDER);
            if(pColumnData->m_nWidth != 0)
                item->cx = pColumnData->m_nWidth;
            else
                item->cx = static_cast<int>(stringSize.Width + 12);

			strDisplay = pColumnData->m_strDisplay;
			if(m_pILocalization)
				strDisplay = m_pILocalization->GetString(pColumnData->m_strDisplay);			
			item->pszText = (LPTSTR)strDisplay.c_str();

			//Always add new column to lastest position
            item->iOrder = static_cast<int>(m_mapheader.size());
            bRetVal = true;
        }
        return bRetVal;
    }

    bool CQListCtrl::GetColumnIndexByKey(const TSTRING& key, int &nColIndex) 
    {		
        nColIndex = -1;
        for(ColumnArray::iterator iter = m_mapheader.begin(); iter != m_mapheader.end(); iter++) {			
            if((*iter)->m_strKey == key)
            {
                nColIndex = static_cast<int>(iter - m_mapheader.begin());
                break;			
            }
        }
        return nColIndex != -1;
    }

    int CQListCtrl::InsertDataByIndex(CQListData* pListData, int nRowIndex) 
    {
        if(pListData)
        {
            LVITEM lvItem = {0};
            lvItem.iItem = nRowIndex == -1 ? CListCtrl::GetItemCount() : nRowIndex;
            lvItem.lParam = (long)pListData;
            lvItem.mask = LVIF_PARAM;
			lvItem.state = 	pListData->GetState();

            int nNewIndex = CListCtrl::InsertItem(&lvItem);
            assert(nNewIndex != -1);
            if(nNewIndex != -1)
                UpdateRowByIndex(nNewIndex,pListData);
        }
        return CListCtrl::GetItemCount();
    }

    bool CQListCtrl::InsertDataByRowColumn(CBasicData* pBasicData, int nRow, int nCol) 
    {
        return UpdateDataByRowColumn(pBasicData,nRow,nCol);
    }

    bool CQListCtrl::UpdateRowByIndex(int nRow, CQListData* pListData) 
    {
        bool bRes = false;
        if(pListData && nRow >=0 && nRow < GetRowCount())
        {
            for(int i = 0;i<GetColumnCount();i++)
            {				
                ColumnData columnData;
                if(GetColumnByIndex(i,&columnData))
                {
                    if(pListData->Find(columnData.m_strKey))
                        InsertDataByRowColumn(pListData->GetValue(columnData.m_strKey),nRow,i);
                }
            }
            bRes = true;
        }
        return bRes;
    }

    bool CQListCtrl::UpdateDataByRowColumn(CBasicData* pBasicData, int nRow, int nCol) 
    {
        bool bRes = false;
        if(pBasicData)
        {
            UINT nMask = LVIF_TEXT;
            if(pBasicData->m_strImagePath.size() > 0)
                nMask |= LVIF_IMAGE;
            bRes = TRUE == CListCtrl::SetItem(nRow,nCol,nMask,pBasicData->GetTextString().c_str(),-1,0,0,NULL);
        }
        return bRes;
    }

    bool CQListCtrl::DeleteRowByIndex(int nRow) 
    {
        return CListCtrl::DeleteItem(nRow) == TRUE;
    }

    int CQListCtrl::GetColumnCount() 
	{
        return static_cast<int>(m_mapheader.size());
    }

    int CQListCtrl::GetRowCount() 
	{
        return CListCtrl::GetItemCount();
    }
	
	void CQListCtrl::RefreshClear()
	{
		CListCtrl::DeleteAllItems();
	}

    //-------------------------------------------------------------------------
    // Order, Data Source Will Do This
    //-------------------------------------------------------------------------
    bool CQListCtrl::Order(int nColumn, int sortType) 
	{
        return false;
    }

    bool CQListCtrl::InsertColumn(int nIndex, ColumnData* pColumnData) 
	{
        ColumnData* pcolData = new ColumnData();
        *pcolData = *pColumnData;

		TSTRING strDisplay;
        LVCOLUMNW Col;
        CreateLVCOLUMNFromColumnData(pcolData, &Col,strDisplay);
        int nItem = CListCtrl::InsertColumn(nIndex, &Col);

        if(nItem !=  - 1) 
		{
            ColumnArray::iterator Iter = m_mapheader.begin();
            advance(Iter,nIndex);
            m_mapheader.insert(Iter, pcolData);
        }    
        return nItem !=  - 1;
    }

    bool CQListCtrl::DeleteColumnByIndex(int nIndex) 
	{
        if(DeleteColumn(nIndex)) 
		{
            ColumnArray::iterator Iter = m_mapheader.begin();		
            advance(Iter,nIndex);
            m_mapheader.erase(Iter);
        }	
        return true;
    }

    bool CQListCtrl::DeleteColumnByKey(const TSTRING &strKey) 
	{
        bool bRes = false;
        int nIndex =  - 1;
        for(ColumnArray::iterator Iter = m_mapheader.begin(); Iter != m_mapheader.end(); Iter++) {
            nIndex++;
            if((*Iter)->m_strKey == strKey) {
                bRes = DeleteColumnByIndex(nIndex);
                break;
            }
        }	
        return bRes;
    }

	/*
	For Now, it will change text only
	*/
    bool CQListCtrl::UpdateColumnByIndex(int nIndex, ColumnData* pColumnData) 
	{
        ColumnData* pcd = m_mapheader[nIndex];
        pcd->m_nWidth = pColumnData->m_nWidth;
        pcd->m_strDisplay = pColumnData->m_strDisplay;
		bool bRes = false;
        LVCOLUMN column;
        memset(&column, 0, sizeof(LVCOLUMN));
        column.mask = LVCF_IMAGE | LVCF_TEXT | LVCF_WIDTH;
        if(CListCtrl::GetColumn(nIndex, &column)) 
		{
			TSTRING strDisplay = pColumnData->m_strDisplay;
			if(m_pILocalization)
				strDisplay = m_pILocalization->GetString(strDisplay);
			column.pszText = (LPTSTR)strDisplay.c_str();
            CListCtrl::SetColumn(nIndex, &column);
            bRes = true;
        }
        return bRes;
    }

    bool CQListCtrl::UpdateColumnByKey(const TSTRING &strKey, const TSTRING &value)
    {
        int nColIndex = 0;
        for(ColumnArray::iterator Iter = m_mapheader.begin(); Iter != m_mapheader.end(); Iter++) {
            if((*Iter)->m_strKey == (strKey)) {
                (*Iter)->m_strDisplay = value;
                LVCOLUMN column;
                memset(&column, 0, sizeof(LVCOLUMN));
                column.mask = LVCF_IMAGE | LVCF_TEXT | LVCF_WIDTH;
                if(CListCtrl::GetColumn(nColIndex, &column)) 
				{
					//column.cx = static_cast<int>(value.size());
					if((*Iter)->m_nWidth > 0)
						column.cx = (*Iter)->m_nWidth;
					else
						column.cx = static_cast<int>(value.size()+12);
					column.pszText = (LPTSTR)value.c_str();
                    CListCtrl::SetColumn(nColIndex, &column);
                    return true;
                }
            }
            nColIndex++;
        }
        return false;
    }

    bool CQListCtrl::GetColumnByIndex(int nIndex, ColumnData* pColumnData) 
	{
        if(nIndex < 0 || pColumnData == NULL)
            return false;
        ColumnArray::iterator iter = m_mapheader.begin();
        advance(iter,nIndex);
        if(pColumnData != NULL && iter != m_mapheader.end()) 
		{
            pColumnData->m_nOrder = nIndex;
            pColumnData->m_strDisplay = (*iter)->m_strDisplay;
            pColumnData->m_strKey = (*iter)->m_strKey;
            return true;
        }
        return false;
    }

    bool CQListCtrl::GetColumnByKey(const TSTRING &strKey, ColumnData* pColumnData)
    {
        bool bRes = false;
        if(pColumnData != NULL) 
		{
            for(ColumnArray::iterator iter = m_mapheader.begin(); iter != m_mapheader.end(); iter++) 
			{
                if((*iter)->m_strKey == (strKey)) 
				{
                    pColumnData->m_strDisplay = (*iter)->m_strDisplay;
                    pColumnData->m_strKey = (*iter)->m_strKey;
                    bRes = true;
                    break;
                }
            }
        }
        return bRes;
    }

    DWORD CQListCtrl::SetExtendedStyle(DWORD dwStyle) 
	{
        dwStyle |= GetExtendedStyle();
        return CListCtrl::SetExtendedStyle(dwStyle);
    }

    long CQListCtrl::SetDropable(bool bIsDropable) 
    {
        long result = 0;
        long nStyle = ::GetWindowLong(this->m_hWnd, GWL_EXSTYLE);
        if(bIsDropable) 
        {
            result = ::SetWindowLong(this->m_hWnd, GWL_EXSTYLE, nStyle | WS_EX_ACCEPTFILES);
            m_DropTarget.Register(this, EVENT_DROP);
            m_DropTarget.Register(this, EVENT_DRAG_ENTER);
            m_DropTarget.Register(this, EVENT_DRAG_OVER);
            m_DropTarget.Register(this, EVENT_DRAG_LEAVE);
        } 
        else 
        {
            result = ::SetWindowLong(this->m_hWnd, GWL_EXSTYLE, nStyle & ~WS_EX_ACCEPTFILES);
            m_DropTarget.Unregister(this, EVENT_DROP);
            m_DropTarget.Unregister(this, EVENT_DRAG_ENTER);
            m_DropTarget.Unregister(this, EVENT_DRAG_OVER);
            m_DropTarget.Unregister(this, EVENT_DRAG_LEAVE);
        }
        m_DropTarget.SetDropable(bIsDropable);
        return result;
    }

    void CQListCtrl::OnDestroy()
    {
        //if(m_strAppViewName.GetLength() == 0) return;
        //for(UINT i = 0; i < m_mapheader.size(); ++i) {
        //    m_pCfgTool->SetCfgVar(m_mapheader.at(i)->m_strKey, m_mapheader.at(i)->m_nWidth);
        //}

        //m_pCfgTool->Save(STORAGE_INIFILE);
        //DeleteIConfigure(m_strAppViewName);
        //DeleteIConfigure(_T("CQListCtrl"));
    }

    bool CQListCtrl::GetSelectedItem(vector<fdp>& vectfdp)
    {
        POSITION pos = GetFirstSelectedItemPosition();	
        while(pos) 
        {
            fdp para;
            int nSelItem = GetNextSelectedItem(pos);
            para.nRow = nSelItem;
            CQListData* pListData = NULL;
            BOOL bRes = GetRowByIndex(nSelItem,&pListData);
            assert(bRes);
            para.pListData = pListData;
            vectfdp.push_back(para);			
        }
        return true;
    }

    //bool CQListCtrl::SetAppViewName(LPCTSTR strAppViewName) {
    //    m_strAppViewName = strAppViewName;
    //    m_pCfgTool = CreateIConfigure(m_strAppViewName);
    //    m_pCfgTool->Load(STORAGE_INIFILE);
    //    return true;
    //}

    void CQListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
    {
        PostMessage(WM_USER_RBUTTONDOWN,point.x,point.y);
        __super::OnRButtonDown(nFlags, point);
    }

    void CQListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
    {
        vector <fdp*> firVec;
        fdp rowData;
        int nItem = HitTest(point);
        SetItemState(nItem, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        if(nItem >= 0) {
            CQListData* pListData = NULL;
            GetRowByIndex(nItem,&pListData);
            rowData.nRow = nItem;
            rowData.pListData = pListData;
            rowData.nFlags = nFlags;
            firVec.push_back(&rowData);
            Fire(EVENT_LIST_UI_LDBCLICK, (long)&firVec);
        }
        __super::OnLButtonDblClk(nFlags, point);
    }

    BOOL CQListCtrl::PreTranslateMessage(MSG* pMsg) 
    {
        return __super::PreTranslateMessage(pMsg);
    }

    void CQListCtrl::PreSubclassWindow() 
    {
        SetDropable(true);
        SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT);

        m_DropTarget.Register(this);
		m_DropTarget.Register(this,EVENT_DROP);
		m_DropTarget.Register(this,EVENT_DRAG_ENTER);
		m_DropTarget.Register(this,EVENT_DRAG_OVER);
		m_DropTarget.Register(this,EVENT_DRAG_LEAVE);
        __super::PreSubclassWindow();
    }

    void CQListCtrl::OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult) 
    {
        LPNMHEADER phdr = reinterpret_cast < LPNMHEADER > (pNMHDR);
        LPNMLISTVIEW pLV = reinterpret_cast < LPNMLISTVIEW > (pNMHDR);
        ColumnData columnData;
        GetColumnByIndex(phdr->iItem, &columnData);
        Fire(EVENT_LIST_UI_HEADER_BUTTON_HIT, (long)&columnData);
        *pResult = 0;
    }

    void CQListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
    {
        LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
        CDragDropFireParam dragFireParam;
        Fire(EVENT_LIST_UI_DRAG_BEGIN,(long)&dragFireParam);
        ///* Sample code
        //dragFireParam.Insert(_T("c:\\test.txt"));
        //dragFireParam.SetEffect(DROPEFFECT_COPY);
        //*/
		m_DataSrc.Register(this,EVENT_DRAG_END);
        *pResult = m_DataSrc.BeginDrag(dragFireParam);
		//Fire(EVENT_DRAG_END,*pResult);
    }

    void CQListCtrl::OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags)
    {
        vector<fdp> vectfdp;
        switch(nChar) 
        {
        case VK_DELETE:
            GetSelectedItem(vectfdp);
            Fire(EVENT_LIST_UI_DELETE_ROW, (long)&vectfdp);
            break;
        };

		CListCtrl::OnKeyDown(nChar,nRepCnt,nFlags);
    }

	void CQListCtrl::DrawItemBackground(Graphics& graphics,const CRect& rcItem,bool bSelected, bool bFocus)
	{
		if(bSelected)
        {
            if(bFocus)
            {
                SolidBrush bgBrush(Color(255,0,128,192));
                graphics.FillRectangle(&bgBrush,rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height());
            }
            else
            {
                SolidBrush bgBrush(Color(255,128,128,128));
                graphics.FillRectangle(&bgBrush,rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height());
            }
        }
        else
        {
            SolidBrush bgBrush(Color(255, 255, 255, 255));
            graphics.FillRectangle(&bgBrush,rcItem.left,rcItem.top,rcItem.Width(),rcItem.Height());
        }
	}

	CRect CQListCtrl::GetSubItemRectEx(int nItemIndex,int nSubItemIndex)
	{
		CRect rcSubItem;
		BOOL bRes = GetSubItemRect(nItemIndex,nSubItemIndex,LVIR_BOUNDS,rcSubItem);
		ASSERT(bRes);
		if(bRes)
		{
			//For microsoft's bug on first bound
			if(nSubItemIndex == 0)
			{
				if(GetColumnCount() > 1)
				{
					//Get Second item' area
					CRect rcSecondItem;
					bRes = GetSubItemRect(nItemIndex,1,LVIR_BOUNDS,rcSecondItem);
					ASSERT(bRes);
					//Reset first item's area
					rcSubItem.right = rcSecondItem.left;
				}
			}
		}
		return rcSubItem;
	}

	Font* CQListCtrl::GetDefaultFont(Graphics& graphics)
	{
		Font* pmyFont = NULL;
		CFont* font = GetFont();
        assert(font);
        if (font)
        {
            LOGFONT lf;
            font->GetLogFont(&lf);
            HDC hDC = graphics.GetHDC();
            pmyFont = new Font(hDC,&lf);
            graphics.ReleaseHDC(hDC);
        }
		return pmyFont;
	}

	int CQListCtrl::DrawImage(Graphics& graphics,const CRect& rcSubItem,CBasicData *pBasicData)
	{
		int nImageWidth = 0;
		int x = rcSubItem.left;
        int y = rcSubItem.top;
        int width = rcSubItem.Width();
        int height = rcSubItem.Height();

		if(pBasicData && pBasicData->m_strImagePath.size() > 0)
        {
			nImageWidth = pBasicData->m_nImageWidth;
            // Create an Image object.			
            std::wstring strImagePath = GetPicturePath() + pBasicData->m_strImagePath;
            Image image(strImagePath.c_str());			
            Unit srcunit = UnitPixel;

            if(pBasicData->m_bAutoSize)
            {
                graphics.DrawImage(&image, x, y, height, height);
                nImageWidth = height;
            }
            else
            {
                if(nImageWidth == 0)
                {				
                    graphics.DrawImage(&image, x, y);
                    nImageWidth = image.GetWidth();
                }
                else				
                    graphics.DrawImage(&image, x, y, 0, 0, nImageWidth, height, srcunit);
            }
        }
		return nImageWidth;
	}

    void CQListCtrl::DrawItem(LPDRAWITEMSTRUCT pDIS)
    {
        using namespace COMMONCONTROL;

		int nLeftOffset = 0;
		CRect rcItem = pDIS->rcItem;
		if(rcItem.left < 0)
			nLeftOffset = rcItem.left * -1;
		rcItem.MoveToXY(0,0);

        //Prepare Graphics
        _DoubleBufferPolicy doubleBuffer;
        Graphics graphics(doubleBuffer.BeginHDC(pDIS->hDC,pDIS->rcItem));

		bool bFucus = false;
		CWnd* pFocusWnd = GetFocus();
		if(pFocusWnd)
			bFucus = pFocusWnd->GetSafeHwnd() == GetSafeHwnd();

		DrawItemBackground(graphics,rcItem,(pDIS->itemState & ODS_SELECTED) != 0,bFucus);

        CQListData* pListData = NULL;
        GetRowByIndex(pDIS->itemID,&pListData);
        if(pDIS && pListData != NULL)
        {
            for(int nSubItemIndex = 0;nSubItemIndex < GetColumnCount();nSubItemIndex++)			
            {
                CRect rcSubItem = GetSubItemRectEx(pDIS->itemID,nSubItemIndex);
				rcSubItem.OffsetRect(nLeftOffset,0);
				rcSubItem.MoveToY(0);

                ColumnData columnData;
                GetColumnByIndex(nSubItemIndex,&columnData);
                CBasicData* pData = pListData->GetValueByKey(columnData.m_strKey);
                if(pData)                    
                    DrawSubItem(graphics,rcSubItem,pData,(pDIS->itemState & ODS_SELECTED) != 0, bFucus);
            }
        }
    }

    void CQListCtrl::DrawSubItem(Graphics& graphics,const CRect& rcSubItem,CBasicData* pBasicData,bool bSelected, bool bFocus)
    {	
        assert(pBasicData);
        if(pBasicData == NULL) return;

        int nImageWidth = DrawImage(graphics,rcSubItem,pBasicData);

        // Create a SolidBrush object.
        SolidBrush fontBrush(
			bSelected ? Color(255,255,255,255) :
			Color(pBasicData->m_nclrA,pBasicData->m_nclrR,pBasicData->m_nclrG,pBasicData->m_nclrB));

        // Initialize arguments.
        Font* pmyFont = NULL;
        if(pBasicData->m_strFont.size() == 0)
			pmyFont = GetDefaultFont(graphics);
        else
		{
			Gdiplus::REAL rSize(static_cast<float>(pBasicData->m_nFontSize));
            pmyFont = new Font(pBasicData->m_strFont.c_str(),rSize );
		}

        if(pmyFont)
        {
            RectF layoutRect(static_cast<float>(rcSubItem.left + nImageWidth), static_cast<float>(rcSubItem.top), static_cast<float>(rcSubItem.Width() - nImageWidth), static_cast<float>(rcSubItem.Height()));

            StringFormat stringFormat;
            stringFormat.SetTrimming(StringTrimmingEllipsisWord);
            switch(pBasicData->GetAlignment())
            {
            case CBasicData::Alignment_Left:
                stringFormat.SetAlignment(StringAlignmentNear);
                break;
            case CBasicData::Alignment_Center:
                stringFormat.SetAlignment(StringAlignmentCenter);
                break;
            case CBasicData::Alignment_Right:
                stringFormat.SetAlignment(StringAlignmentFar);
                break;
            };		

            // Draw string.
            graphics.DrawString(pBasicData->GetTextString().c_str(),-1,pmyFont,layoutRect,&stringFormat,&fontBrush);

            delete pmyFont;
        }
    }

    LRESULT  CQListCtrl::OnUSER_RBUTTONDOWN(WPARAM wpara, LPARAM lpara)
    {
        CPoint point(static_cast<int>(wpara), static_cast<int>(lpara));
        int nItem = HitTest(point);
        vector<fdp> vectFDP;
        GetSelectedItem(vectFDP);
        vector <fdp*> firVec;

        for(vector<fdp>::iterator Iter = vectFDP.begin();Iter != vectFDP.end();Iter++)
        {
            fdp* pfdp = &(*Iter);
            Iter->isFocused = true;
            Iter->point = point;
            firVec.push_back(pfdp);
        }

        Fire(EVENT_LIST_UI_RBTNDOWN, (long)&firVec);
        return 0;
    }

    void CQListCtrl::OnLvnItemChanged(NMHDR *pNMHDR, LRESULT *pResult)
    {
        vector<fdp> vectFDP;
        GetSelectedItem(vectFDP);
        Fire(EVENT_LIST_UI_ITEMCHANGED,(long)&vectFDP);
        *pResult = 0;
    }

	BOOL CQListCtrl::OnEraseBkgnd(CDC* pDC)
	{
		Graphics graphics(pDC->GetSafeHdc());

		CRect rcView;
		GetClientRect(&rcView);
		
		CRgn hRgn;
		hRgn.CreateRectRgnIndirect(rcView);

		CRect rcVisualItem;
		int nTotal = GetItemCount();
		int nTopIndex = GetTopIndex();
		int nLast = (nTopIndex + GetCountPerPage()) > (nTotal - 1) ? (nTotal - 1) : (nTopIndex + GetCountPerPage());
		if(nTotal > 0)
		{
			CRect rcTopItem;
			GetItemRect(nTopIndex,&rcTopItem,LVIR_BOUNDS);
			CRect rcLastItem;
			GetItemRect(nLast,&rcLastItem,LVIR_BOUNDS);

			rcVisualItem.left = rcTopItem.left;
			rcVisualItem.top = rcTopItem.top;
			rcVisualItem.right = rcTopItem.right;
			rcVisualItem.bottom = rcLastItem.bottom;

			CRgn hVaildRgn;
			hVaildRgn.CreateRectRgnIndirect(rcVisualItem);

			int nCombineResult = hRgn.CombineRgn( &hRgn, &hVaildRgn, RGN_DIFF );
			assert(nCombineResult != ERROR || nCombineResult != NULLREGION  );

			graphics.SetClip(hRgn);
		}

		DWORD dwResult = ::GetSysColor(COLOR_WINDOW); 
		SolidBrush whiteBrush(Color(255, GetRValue(dwResult), GetGValue(dwResult), GetBValue(dwResult)));			
		graphics.FillRectangle(&whiteBrush, 0,0,rcView.Width(),rcView.Height());

		return TRUE;
	}

	void CQListCtrl::OnSize(UINT nType, int cx, int cy)
	{
		__super::OnSize(nType, cx, cy);
		FireOnSizeParam fireOnSizeParam;
		fireOnSizeParam.SetParam(nType, cx, cy);
		Fire(EVENT_LIST_UI_ONSIZE, (long)&fireOnSizeParam);
		// TODO: Add your message handler code here
	}


	void CQListCtrl::OnSizing(UINT fwSide, LPRECT pRect)
	{
		__super::OnSizing(fwSide, pRect);
		FireOnSizingParam fireOnSizingParam;
		fireOnSizingParam.SetParam(fwSide, pRect);
		Fire(EVENT_LIST_UI_ONSIZING, (long)&fireOnSizingParam);
		// TODO: Add your message handler code here
	}
	void CQListCtrl::OnKillFocus( CWnd* CUnfocusWnd)
	{
		CListCtrl::OnKillFocus(CUnfocusWnd);
		Fire(EVENT_LIST_UI_KILLFOCUS,(long)CUnfocusWnd->GetSafeHwnd());
	}
	void CQListCtrl::OnSetFocus( CWnd* CFocusWnd)
	{
		CListCtrl::OnSetFocus(CFocusWnd);
		Fire(EVENT_LIST_UI_SETFOCUS,(long)CFocusWnd->GetSafeHwnd());
	}


} // end of namespace

#pragma warning(pop)

