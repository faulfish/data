/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage5 dialog
/////////////////////////////////////////////////////////////////////////////
#ifndef __GPRSMdmSetupPropertyPage5_H__
#define __GPRSMdmSetupPropertyPage5_H__

#pragma once

#include "..\..\Components\UIFramework\Porting\SyncStatic.h"
#include "resource.h"

class CGPRSMdmSetupPropertyPage5 : public CPropertyPage
{
    DECLARE_DYNCREATE(CGPRSMdmSetupPropertyPage5)

    // Construction
public:
    CGPRSMdmSetupPropertyPage5();
    ~CGPRSMdmSetupPropertyPage5();

    // Dialog Data
    //{{AFX_DATA(CGPRSMdmSetupPropertyPage5)
    enum { IDD = IDD_PROPPAGE5 };
    CStatic				m_PagePicStatic;
    CSyncStatic			m_PageTextLine1Static;
    CSyncStatic			m_PageTextLine2Static;
    CButton				m_RebootNowRadio;
    CButton				m_RebootLaterRadio;
    //}}AFX_DATA
    CBitmap				m_PagePicBitmap;
    CString				m_szPageFontName;
    int					m_nPageFontSize;
    CString				m_szPageCaptionText;
    COLORREF			m_EditTextColor;
    CBrush				m_EditBkBrush;
    CBrush				m_PageBkBrush;
    CToolTipCtrl		m_Tooltip;

    // Member method
private:
    bool				ToDo(void);

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CGPRSMdmSetupPropertyPage5)
public:
    virtual BOOL		OnSetActive();
    virtual BOOL		OnWizardFinish();
    virtual BOOL		PreTranslateMessage(MSG* pMsg);
protected:
    virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CGPRSMdmSetupPropertyPage5)
    virtual BOOL		OnInitDialog();
    afx_msg HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif