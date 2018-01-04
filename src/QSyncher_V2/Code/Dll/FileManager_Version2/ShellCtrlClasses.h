/************************************************
THIS CODE AND INFORMATION IS PROVIDED 'AS IS' 
WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY 
AND/OR FITNESS FOR A PARTICULAR PURPOSE.
Author: Barretto VN  7/2002
*************************************************/


#if !defined(AFX_SHELLCTRLCLASSES_H__C043DE78_0AB9_4B97_85E0_E6D29C979DFC__INCLUDED_)
#define AFX_SHELLCTRLCLASSES_H__C043DE78_0AB9_4B97_85E0_E6D29C979DFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellCtrlClasses.h : header file
//

#include "shellclass.h"
#include "../../Components/Localization/ilocalization.h"
#include "../../Components/ConfigureTool/IConfigure.h"
#include "../../CommonUtil/NotifyBase.h"
#include "../../Components/UIFramework/Porting/QUIDropTraget.h"
#include "../../Components/UIFramework/Porting/UIOleDataSource.h"
#include "../../Components/UIFramework/interface/IQListCtrlUI.h"


#define IDS_MEMORY_ERROR _T("Error Allocating Shell Memory")

#define ID_COL_NAME   0
#define ID_COL_TYPE   1
#define ID_COL_SIZE   2
#define ID_COL_DATE   3

using namespace QSYNC_LIB;

// This static function is used for CShellTreeCtrl and CShellListCtrl
static wstring GetLocalizationString(const TSTRING& strID) {
    QSYNC_LIB::ILocalization* pil;
    CreateILocalization(&pil);
    return pil->GetString(strID);
}

typedef struct 
{
    BOOL bRoot;
    LPITEMIDLIST lpi;
    LPSHELLFOLDER lpsfParent;
    LPITEMIDLIST lpifq;
} LPTVITEMDATA;

//---------------------------------------------------------------------------
// CShellCtrlClasses window
//---------------------------------------------------------------------------
class CShellListCtrl;
class CShellTreeCtrl;

//---------------------------------------------------------------------------
// CShellListCtrl window
//---------------------------------------------------------------------------
class CShellListCtrl : public CListCtrl, public INotify, public CNotifyBase
{
    // Construction
public:
    CShellListCtrl();
    virtual ~CShellListCtrl();
    // Attributes
public:
    CImageList       m_pImageListL;
    CImageList       m_pImageListS;
    int              giCtr;
    CShellTreeCtrl*  m_pShellTreeCtrl;
    LPMALLOC         m_pMalloc;
    CString          m_CurrentSelectedFilePath;
    IConfigure*      m_pCfgTool;
    CUIDropTarget    m_DropTarget;
    CUIOleDataSource m_DropSource;
    // Operations
public:
    BOOL SubclassDlgItem(UINT nID, CWnd* pParent);
    int InitilizeCtrl();
    void SetupImageLists();
    void LVPopulateFiles(LPTVITEMDATA* lptvid);
    void MyDeleteAllItems();
    BOOL InsertListViewItem(LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, LPITEMIDLIST lpifq);
    void ShowStdMenu(BOOL bShowMenu ,  LPTVITEMDATA* lptvid);
    void virtual Event(const TSTRING& strEvent,long nParam);
    void SetCurrentFilePath(CString strFilePath) { m_CurrentSelectedFilePath = strFilePath; }
    CString GetCurrentFilePath() { return m_CurrentSelectedFilePath; }
    void RefreshList();
    virtual bool Register(INotify* pINotify,const TSTRING& strEvent) { return CNotifyBase::Register(pINotify,strEvent); }
    virtual bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) { return CNotifyBase::Register(pEventFunc,strEvent); }
    virtual bool Unregister(INotify* pINotify,const TSTRING& strEvent) { return CNotifyBase::Unregister(pINotify,strEvent); }
    virtual bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) { return CNotifyBase::Unregister(pEventFunc,strEvent); }
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CShellListCtrl)
    //}}AFX_VIRTUAL
    // Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
    //{{AFX_MSG(CShellListCtrl)
    afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnRclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    virtual void PreSubclassWindow();
};


//---------------------------------------------------------------------------
// CShellTreeCtrl window
//---------------------------------------------------------------------------
class CShellTreeCtrl : public CTreeCtrl, public QSYNC_LIB::INotify
{
    // Construction
public:
    CShellTreeCtrl();
    virtual ~CShellTreeCtrl();
    // Attributes
public:
    CImageList      m_pImageList;
    HTREEITEM       m_pSelectedItem;
    LPMALLOC        m_pMalloc;
    CShellListCtrl* m_pShellListCtrl;
    CShellClass     m_cShell;
    HTREEITEM       m_hParent;
    CString         m_CurrentSelectedFolderPath;
    IConfigure*     m_pCfgTool;
    // Operations
public:
    BOOL SubclassDlgItem(UINT nID, CWnd* pParent);
    void InitializeCtrl();
    HTREEITEM InsertDesktopItem(LPSHELLFOLDER lpsf);
    void SetupImages();
    BOOL PopulateTree(NMHDR* pNMHDR);
    UINT DeleteChildren(HTREEITEM hItem);
    void SelectThisItem(LPCTSTR szBuff);
    void virtual Event(const TSTRING& strEvent,long nParam) {}
    void SetCurrentFolderPath(CString strFolderPath) {
        m_CurrentSelectedFolderPath = strFolderPath;
    }
    CString GetCurrentFolderPath() {
        return m_CurrentSelectedFolderPath;
    }
    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CShellTreeCtrl)
    //}}AFX_VIRTUAL

    // Generated message map functions
protected:
    DECLARE_MESSAGE_MAP()
    //{{AFX_MSG(CShellTreeCtrl)
    afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
};


//---------------------------------------------------------------------------
// Context Menu Class (CContextMenuClass)
//---------------------------------------------------------------------------
class CShellContextMenuClass
{
public:
    CShellContextMenuClass(); 

public:
    void ShowMenu(LPTVITEMDATA *lptvid, BOOL bShowMenu, HWND m_hWnd);
    void ShowViewStyleMenu(CListCtrl& mListCtrl);

public:

public:
    virtual ~CShellContextMenuClass();
};




class CShellCtrlClasses : public CWnd
{
    // Construction
public:
    CShellCtrlClasses();

    // Attributes
public:

    // Operations
public:

    // Overrides
    // ClassWizard generated virtual function overrides
    //{{AFX_VIRTUAL(CShellCtrlClasses)
    //}}AFX_VIRTUAL

    // Implementation
public:
    virtual ~CShellCtrlClasses();

    // Generated message map functions
protected:
    //{{AFX_MSG(CShellCtrlClasses)
    // NOTE - the ClassWizard will add and remove member functions here.
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLCTRLCLASSES_H__C043DE78_0AB9_4B97_85E0_E6D29C979DFC__INCLUDED_)
