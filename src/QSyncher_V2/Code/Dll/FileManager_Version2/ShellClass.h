/*************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' 
   WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY 
   AND/OR FITNESS FOR A PARTICULAR PURPOSE.
   Author: Barretto VN  7/2002
***************************************************/

#if !defined(AFX_SHELLCLASS_H__9F21EF51_7AEA_40D7_B684_EAD4E045DD51__INCLUDED_)
#define AFX_SHELLCLASS_H__9F21EF51_7AEA_40D7_B684_EAD4E045DD51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShellClass.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShellClass window

/*
typedef struct tagTVID
{
   LPSHELLFOLDER lpsfParent;
   LPITEMIDLIST  lpi;
   LPITEMIDLIST  lpifq;
   LPARAM		 lParam;		
} TVITEMDATA, *LPTVITEMDATA;
*/

class CShellClass  // : public CWnd
{
// Construction
public:
	CShellClass();

// Attributes
public:
	typedef struct {
		   BOOL bRoot;
		   LPITEMIDLIST lpi;
		   LPSHELLFOLDER lpsfParent;
		   LPITEMIDLIST lpifq;
		}LPTVITEMDATA;

// Operations
public:
	void SetTvMask(ULONG ulAttrs , TVITEM *tvi, BOOL bChildValid);

	int GetNormalIcon(LPITEMIDLIST lpifq);
	LPITEMIDLIST GetNextItem(LPCITEMIDLIST pidl);
	UINT GetSize(LPCITEMIDLIST pidl); 
	LPITEMIDLIST Concatenate(LPMALLOC lpMalloc ,LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2); 
	BOOL GetName (LPSHELLFOLDER lpsf, LPITEMIDLIST lpi,DWORD dwFlags, LPSTR lpFriendlyName);
	int GetIcon (LPITEMIDLIST lpi, UINT uFlags);
	void GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem);
	LPITEMIDLIST CopyItemID(LPMALLOC g_pMalloc , LPITEMIDLIST pidl); 
	HIMAGELIST GetImageList(BOOL bSmall = TRUE);
	LPITEMIDLIST Copy(LPMALLOC lpMalloc , LPCITEMIDLIST pidlSource);
	LPITEMIDLIST GetNextItemID(LPCITEMIDLIST pidl); 
	BOOL GetParentID(LPITEMIDLIST pidl);
	HTREEITEM InsertTreeItemTE(BOOL bRoot, TVINSERTSTRUCT* tvins, TCHAR szBuff[MAX_PATH], HTREEITEM hParent, HTREEITEM hPrev, LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, LPITEMIDLIST lpi, BOOL bChildValid, CString szTitle);


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShellClass)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CShellClass();

	// Generated message map functions
protected:
	//{{AFX_MSG(CShellClass)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
//	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHELLCLASS_H__9F21EF51_7AEA_40D7_B684_EAD4E045DD51__INCLUDED_)
