#pragma once

#include "HexEdit.h"
#include "Resource.h"

// CNVItemDataView form view

class CNVItemDataView : public CFormView
{
	DECLARE_DYNCREATE(CNVItemDataView)

protected:
	CNVItemDataView();           // protected constructor used by dynamic creation
	virtual ~CNVItemDataView();

public:
	enum { IDD = IDD_HEX_EDIT };
	CHexEdit m_hexEdit;
	CHexEdit m_mobileEdit;
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CHexEdit &   GetHexEdit(){return m_hexEdit;}
	CHexEdit &   GetMobileEdit(){return m_mobileEdit;}
	virtual void OnInitialUpdate();
	void         SaveDataChage(int itemId,BYTE* itemContent);
	void		 setIsEdit(bool isModify);
};


