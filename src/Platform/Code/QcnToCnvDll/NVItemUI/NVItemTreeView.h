#pragma once


// CNVItemTreeView view


class CNVItemTreeView : public CTreeView
{
	DECLARE_DYNCREATE(CNVItemTreeView)

protected:
	CNVItemTreeView();           // protected constructor used by dynamic creation
	virtual ~CNVItemTreeView();
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);

	void OnInitialUpdate();


private:
	CTreeCtrl * myTreeCtrl;
	CImageList  image;
	CString     mNVItemId;

public:
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSortAscbyid();
	afx_msg void OnSortSortdesbyid();
	afx_msg void OnGetnvitemGetnvitemfromphone();
};


