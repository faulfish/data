/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage1 dialog
/////////////////////////////////////////////////////////////////////////////
#ifndef __GPRSMdmSetupPropertyPage1_H__
#define __GPRSMdmSetupPropertyPage1_H__

#pragma once

#include "..\..\Components\UIFramework\Porting\SyncStatic.h"
#include "resource.h"
class CGPRSMdmSetupPropertyPage1 : public CPropertyPage
{
    DECLARE_DYNCREATE(CGPRSMdmSetupPropertyPage1)

    // Construction
public:
    CGPRSMdmSetupPropertyPage1();
    ~CGPRSMdmSetupPropertyPage1();

    // Dialog Data
    //{{AFX_DATA(CGPRSMdmSetupPropertyPage1)
    enum { IDD = IDD_PROPPAGE1 };
    CStatic				m_PagePicStatic;
    CSyncStatic			m_PageTextLine1Static;
    CSyncStatic			m_PageTextLine2Static;
    CSyncStatic			m_PageTextLine3Static;
    CSyncStatic			m_PageTextLine4Static;
    CSyncStatic			m_PageTextLine5Static;
    //}}AFX_DATA
    CBitmap				m_PagePicBitmap;
    CString				m_szPageFontName;
    int					m_nPageFontSize;
    CString				m_szPageCaptionText;
    COLORREF			m_EditTextColor;
    CBrush				m_EditBkBrush;
    CBrush				m_PageBkBrush;
    CToolTipCtrl		m_Tooltip;

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CGPRSMdmSetupPropertyPage1)
public:
    virtual BOOL		OnSetActive();
    virtual LRESULT		OnWizardNext();
    virtual BOOL		PreTranslateMessage(MSG* pMsg);
protected:
    virtual void		DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CGPRSMdmSetupPropertyPage1)
    virtual BOOL		OnInitDialog();
    afx_msg HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif