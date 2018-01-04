#pragma once


// CNVItemListView view

class CNVItemListView : public CListView
{
	DECLARE_DYNCREATE(CNVItemListView)

protected:
	CNVItemListView();           // protected constructor used by dynamic creation
	virtual ~CNVItemListView();
private:
	CListCtrl * m_list;
	CMap<DWORD , DWORD& , COLORREF , COLORREF&> MapItemColor;
public:
	void SetItemColor(DWORD iItem, COLORREF color);
	void ShowMesInfo(const char* szInfo,int flag);
	void OnInitialUpdate(void);
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnNMCustomdraw(NMHDR *pNMHDR, LRESULT *pResult);
};


