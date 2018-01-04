#pragma once

#include "../resource.h"
#include "../CommUIExportAPI.h"
#include "../LayoutManager/TWidge.h"
#include "../../../Components/Localization/Ilocalization.h"
#include "../../../CommonUtil/UnicodeUtility.h"
// CQDialog dialog


namespace QSYNC_LIB {

class COMMONUI_API CQDialog : public CDialog
{
	DECLARE_DYNAMIC(CQDialog)

protected:
	CQDialog(CWnd* pParent = NULL);   // standard constructor
    CQDialog(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CQDialog();
	std::wstring GetWString(const TSTRING& strID);

// Dialog Data
	enum { IDD = IDD_QDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual void SetWindowRegion(LPRECT& pRect, GeometryRect& rcClient);

	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL SetWindowText(LPCTSTR lpszString);
	virtual void GetWindowText(CString& strText);
    afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnLButtonDown( UINT, CPoint );
	void SetDlgMoveable(bool);	
	virtual void OnBnClickedOK(){assert(0);/*Need to be overwrited.*/}
	virtual void Disconnect(const TSTRING& strTitle,const TSTRING& strContent = _T("IDS_Cal_Disconnect"),bool bShowMessage = true);
private:
	TSTRING m_strWindowText;
	ILocalization* m_pLocale;
	bool m_DlgMoveable;
};

} // end of QSYNC_LIBz