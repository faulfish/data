/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage2 dialog
/////////////////////////////////////////////////////////////////////////////
#ifndef __GPRSMdmSetupPropertyPage2_H__
#define __GPRSMdmSetupPropertyPage2_H__

#pragma once

#include "..\..\Components\UIFramework\Porting\SyncStatic.h"
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "resource.h"

class CGPRSMdmSetupPropertyPage2 : public CPropertyPage , public QSYNC_LIB::INotify
{
    DECLARE_DYNCREATE(CGPRSMdmSetupPropertyPage2)

    // Construction
public:
    CGPRSMdmSetupPropertyPage2();
    ~CGPRSMdmSetupPropertyPage2();

    // Dialog Data
    //{{AFX_DATA(CGPRSMdmSetupPropertyPage2)
    enum { IDD = IDD_PROPPAGE2 };
    CStatic				m_PagePicStatic;
    CSyncStatic			m_PageTextLine1Static;
    CSyncStatic			m_PageTextLine2Static;
    //}}AFX_DATA
    CBitmap				m_PagePicBitmap;
    CString				m_szPageFontName;
    int					m_nPageFontSize;
    CString				m_szPageCaptionText;
    COLORREF			m_EditTextColor;
    CBrush				m_EditBkBrush;
    CBrush				m_PageBkBrush;
    CToolTipCtrl		m_Tooltip;
    CEvent m_ModemChangeEvent;

    // Overrides
    // ClassWizard generate virtual function overrides
    //{{AFX_VIRTUAL(CGPRSMdmSetupPropertyPage2)
public:
    virtual BOOL		OnSetActive();
    virtual LRESULT		OnWizardNext();
    virtual BOOL		PreTranslateMessage(MSG* pMsg);
    virtual void Event(const TSTRING& strEvent,long nParam);
protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    //}}AFX_VIRTUAL

    // Implementation
protected:
    // Generated message map functions
    //{{AFX_MSG(CGPRSMdmSetupPropertyPage2)
    virtual BOOL		OnInitDialog();
    afx_msg HBRUSH		OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

#endif