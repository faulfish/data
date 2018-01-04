/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage3 dialog
/////////////////////////////////////////////////////////////////////////////
#ifndef __GPRSMdmSetupPropertyPage3_H__
#define __GPRSMdmSetupPropertyPage3_H__

#pragma once

#include "..\..\Components\UIFramework\Porting\SyncStatic.h"
#include "resource.h"

class CGPRSMdmSetupPropertyPage3 : public CPropertyPage
{
    DECLARE_DYNCREATE(CGPRSMdmSetupPropertyPage3)

    // Construction
public:
    CGPRSMdmSetupPropertyPage3();
    ~CGPRSMdmSetupPropertyPage3();

    // Dialog Data
    //{{AFX_DATA(CGPRSMdmSetupPropertyPage3)
    enum { IDD = IDD_PROPPAGE3 };
    CStatic				m_PagePicStatic;
    CSyncStatic			m_PageTextLine1Static;
    CSyncStatic			m_PageTextLine2Static;
    CComboBox			m_COMPortCombo;
    CSyncStatic			m_PageTextLine3Static;
    CString				m_szAPN;
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
    //{{AFX_VIRTUAL(CGPRSMdmSetupPropertyPage3)
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
    //{{AFX_MSG(CGPRSMdmSetupPropertyPage3)
    virtual BOOL		OnInitDialog();
    afx_msg HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif