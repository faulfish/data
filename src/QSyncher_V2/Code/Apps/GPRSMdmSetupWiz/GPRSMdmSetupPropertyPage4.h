/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage4 dialog
/////////////////////////////////////////////////////////////////////////////
#ifndef __GPRSMdmSetupPropertyPage4_H__
#define __GPRSMdmSetupPropertyPage4_H__

#pragma once

#include "..\..\Components\UIFramework\Porting\SyncStatic.h"
#include "resource.h"

class CGPRSMdmSetupPropertyPage4 : public CPropertyPage
{
    DECLARE_DYNCREATE(CGPRSMdmSetupPropertyPage4)

    // Construction
public:
    CGPRSMdmSetupPropertyPage4();
    ~CGPRSMdmSetupPropertyPage4();

    // Dialog Data
    //{{AFX_DATA(CGPRSMdmSetupPropertyPage4)
    enum { IDD = IDD_PROPPAGE4 };
    CStatic				m_PagePicStatic;
    CSyncStatic			m_PageTextLine1Static;
    CSyncStatic			m_PageTextLine2Static;
    CString				m_szDialupConnectionName;
    CSyncStatic			m_PageTextLine4Static;
    CString				m_szDialNumber;
    CSyncStatic			m_PageTextLine5Static;
    CString				m_szDialupUsername;
    CSyncStatic			m_PageTextLine6Static;
    CString				m_szDialupPassword;
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
    //{{AFX_VIRTUAL(CGPRSMdmSetupPropertyPage4)
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
    //{{AFX_MSG(CGPRSMdmSetupPropertyPage4)
    virtual BOOL		OnInitDialog();
    afx_msg HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif