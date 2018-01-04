/*************************************************
   THIS CODE AND INFORMATION IS PROVIDED 'AS IS' 
   WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
   OR IMPLIED, INCLUDING BUT NOT LIMITED TO
   THE IMPLIED WARRANTIES OF MERCHANTABILITY 
   AND/OR FITNESS FOR A PARTICULAR PURPOSE.
   Author: Barretto VN  7/2002
***************************************************/

// ShellClass.cpp : implementation file
//

#include "stdafx.h"
#include "ShellClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CShellClass

CShellClass::CShellClass()
{
}

CShellClass::~CShellClass()
{
}

/*
BEGIN_MESSAGE_MAP(CShellClass, CWnd)
	//{{AFX_MSG_MAP(CShellClass)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
*/

/////////////////////////////////////////////////////////////////////////////
// CShellClass message handlers

/*******************************************************
*  Function : GetImageList(BOOL)
*  
*  Parameters : True if small images list is required
*
*  Purpose  : Get Handle of the Shells Image List and
*             send it to the caller
* 
*  Comments : 
*
*  Return : HIMAGELIST
********************************************************/
HIMAGELIST CShellClass::GetImageList(BOOL bSmall)
{

	SHFILEINFO sfi;
	HIMAGELIST himl;
	if(bSmall) 
        himl = (HIMAGELIST)SHGetFileInfo(
        _T(""), 0,
        &sfi, 
        sizeof(SHFILEINFO), 
        SHGFI_SYSICONINDEX |
		SHGFI_SMALLICON);
	else
		himl = (HIMAGELIST)SHGetFileInfo( _T(""), 
        0,
        &sfi, 
        sizeof(SHFILEINFO), 
        SHGFI_SYSICONINDEX |
		SHGFI_LARGEICON);

	return himl;
}

/*******************************************************
*  Function : CopyItemID(LPMALLOC, LPITEMIDLIST)
*  
*  Parameters : Shell Allocated Memory , ItemIDList
*
*  Purpose  : To return a Copy of a ItemIDList
* 
*  Comments : this is necessary since we cannot use
*             something like itemid = olditemid
*
*  Return : LPITEMIDLIST
********************************************************/
LPITEMIDLIST CShellClass::CopyItemID(LPMALLOC g_pMalloc , LPITEMIDLIST pidl) 
   { 

	// Get the size of the specified item identifier. 
   int cb = pidl->mkid.cb; 

   // Allocate a new item identifier list. 
   LPITEMIDLIST pidlNew = (LPITEMIDLIST) 
   g_pMalloc->Alloc(cb + sizeof(USHORT)); 
   if (pidlNew == NULL) 
      return NULL; 

   // Copy the specified item identifier. 
   memcpy(pidlNew, pidl, cb); 

   // Append a terminating zero. 
   *((USHORT *) (((LPBYTE) pidlNew) + cb)) = 0; 

   return pidlNew; 
} 

/*******************************************************
*  Function : GetNormalAndSelectedIcons(LPITEMIDLIST, LPTV_ITME)
*  
*  Parameters : ITEMIDLIST, Tree Item Structure
*
*  Purpose  : To return the index of the Selected and
*             Normal Icons useful in a Tree Control
* 
*  Comments : returns a referance for the images indexes
*             in the Tree item structure passed
*
*  Return : none
********************************************************/
void CShellClass::GetNormalAndSelectedIcons(LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	// Don't check the return value here. 
	// If GetIcon() fails, you're in big trouble.

	lptvitem->iImage = GetIcon (lpifq, 
								SHGFI_PIDL | 
								CSIDL_DESKTOP |
								SHGFI_ICON |
								SHGFI_SYSICONINDEX | 
								SHGFI_SMALLICON);

	lptvitem->iSelectedImage = GetIcon (lpifq, 
										SHGFI_PIDL | 
										SHGFI_SYSICONINDEX | 
										SHGFI_SMALLICON | 
										SHGFI_OPENICON);

	return;
}

int CShellClass::GetIcon (LPITEMIDLIST lpi, UINT uFlags)
{
	SHFILEINFO sfi;
	SHGetFileInfo ((LPCTSTR)lpi, 0, &sfi, sizeof (SHFILEINFO), uFlags);
	return sfi.iIcon;
} 

/*******************************************************
*  Function : GetName(LPSHELLFOLDER, 
*                     LPITEMIDLIST,
*                     DWORD,
*                     LPSTR)
*  
*  Parameters : ShellFolder , ItemIDLIst, Flags
*               , returned String
*
*  Purpose  : To return a user-friendly name from 
*             ITEMIDLIST
* 
*  Comments : returns FALSE if failed 
*
*  Return : BOOL
********************************************************/
BOOL CShellClass::GetName (LPSHELLFOLDER lpsf, LPITEMIDLIST lpi,
						   DWORD dwFlags, LPSTR lpFriendlyName)
{

	
	BOOL bSuccess = TRUE;
	STRRET str;

	if (NOERROR == lpsf->GetDisplayNameOf (lpi, dwFlags, &str))
	{

	
		switch (str.uType)
		{
			case STRRET_WSTR:
			WideCharToMultiByte (CP_ACP, // code page
									0, // dwFlags
									str.pOleStr, // lpWideCharStr
									-1, // cchWideChar
									lpFriendlyName, // lpMultiByteStr
							//		sizeof (lpFriendlyName), // cchMultiByte
							        sizeof(str), 
									NULL, // lpDefaultChar
									NULL); // lpUsedDefaultChar
			break;

			case STRRET_OFFSET:
				lstrcpy ((LPTSTR)lpFriendlyName, (LPTSTR)lpi + str.uOffset);
				break;

			case STRRET_CSTR:
				lstrcpy ((LPTSTR)lpFriendlyName, (LPTSTR) str.cStr);
				break;

			default:
				bSuccess = FALSE;
				break;
		}
	}
	else
		bSuccess = FALSE;

return bSuccess;
} 
 
LPITEMIDLIST CShellClass::Concatenate(LPMALLOC lpMalloc ,LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2) 
{ 

	LPITEMIDLIST   pidlNew; 
	UINT           cb1 = 0,	cb2 = 0;  //are both of these NULL? 
	if(!pidl1 && !pidl2)    
		return NULL;  
	//if pidl1 is NULL, just return a copy of pidl2 

	if(!pidl1)    
	{    
		pidlNew = Copy(lpMalloc , pidl2);     
		return pidlNew;    
	}  
	//if pidl2 is NULL, just return a copy of pidl1 

	if(!pidl2)    
	{    
		pidlNew = Copy(lpMalloc, pidl1);     
		return pidlNew;    
	}  
	cb1 = GetSize(pidl1) - sizeof(ITEMIDLIST);  
	cb2 = GetSize(pidl2);  
	//create the new PIDL 
	pidlNew = (LPITEMIDLIST)lpMalloc->Alloc(cb1 + cb2);  
	if(pidlNew)    
	{    
		//copy the first PIDL    
		memcpy(pidlNew, pidl1, cb1);       
		//copy the second PIDL    
		memcpy(((LPBYTE)pidlNew) + cb1, pidl2, cb2);    
	}  
	return pidlNew; 
}  
 
UINT CShellClass::GetSize(LPCITEMIDLIST pidl) 
{ 
	UINT cbTotal = 0; 
	LPITEMIDLIST pidlTemp = (LPITEMIDLIST) pidl;  
	if(pidlTemp)    
	{    
		while(pidlTemp->mkid.cb)       
		{       
			cbTotal += pidlTemp->mkid.cb;       
			pidlTemp = GetNextItem(pidlTemp);       
		}      
		//add the size of the NULL terminating ITEMIDLIST    
		cbTotal += sizeof(ITEMIDLIST);   
	 } 
	return (cbTotal); 
}  

LPITEMIDLIST CShellClass::GetNextItem(LPCITEMIDLIST pidl) 
{ 
	if(pidl)    
		return (LPITEMIDLIST)(LPBYTE)(((LPBYTE)pidl) + pidl->mkid.cb); 
	else    
		return 	(NULL); 
}  

LPITEMIDLIST CShellClass::Copy(LPMALLOC lpMalloc , LPCITEMIDLIST pidlSource)
{ 

	LPITEMIDLIST pidlTarget = NULL; 
	UINT cbSource = 0;  
	if(NULL == pidlSource)    
		return (NULL);  
	// Allocate the new pidl
	cbSource = GetSize(pidlSource); 
	pidlTarget = (LPITEMIDLIST)lpMalloc->Alloc(cbSource); 
	if(!pidlTarget)    
		return (NULL); 
	// Copy the source to the target 
	memcpy(pidlTarget, pidlSource, cbSource);  
	return pidlTarget; 
}  
 
BOOL CShellClass::GetParentID(LPITEMIDLIST pidl)
{

	BOOL fRemoved = FALSE;

	// Make sure it's a valid PIDL.
    if (pidl == NULL)
        return(FALSE);

    if (pidl->mkid.cb)
    {
		LPITEMIDLIST pidlNext = pidl;
        while (pidlNext)
        {
           pidl = pidlNext;
		   pidlNext = GetNextItemID(pidl);
        }
		// Remove the last one, insert null-terminator.
        pidl->mkid.cb = 0; 
        fRemoved = TRUE;
    }

    return fRemoved;
}


LPITEMIDLIST CShellClass::GetNextItemID(LPCITEMIDLIST pidl) 
   { 
   // Check for valid pidl.
   if(pidl == NULL)
      return NULL;

   // Get the size of the specified item identifier. 
   int cb = pidl->mkid.cb; 

   // If the size is zero, it is the end of the list. 
   if (cb == 0) 
      return NULL; 

   // Add cb to pidl (casting to increment by bytes). 
   pidl = (LPITEMIDLIST) (((LPBYTE) pidl) + cb); 

   // Return NULL if it is null-terminating, or a pidl otherwise. 
   return (pidl->mkid.cb == 0) ? NULL : (LPITEMIDLIST) pidl; 
} 

/*******************************************************
*  Function : InsertTreeItemTE(BOOL, TVINSERTSTRUCT,
*                              char, HTREEITEM,
*                              HTREEITEM, LPSHELLFOLDER,
*                              LPITEMIDLIST, LPITEMIDLIST,
*                              BOOL, CString)
*  
*  Parameters : 
*
*  Purpose  : Most important function for inserting
*             an item in a Tree Control
* 
*  Comments : Call SetTvMask(..) to set mask and signs
*             (+-)
*
*  Return : int
********************************************************/
HTREEITEM CShellClass::InsertTreeItemTE(BOOL bRoot, TVINSERTSTRUCT* tvins, TCHAR szBuff[MAX_PATH],
    HTREEITEM hParent, HTREEITEM hPrev, LPSHELLFOLDER lpsf, LPITEMIDLIST lpifq, LPITEMIDLIST lpi,
    BOOL bChildValid, CString szTitle)
{

	TV_ITEM tvi; // tree view item
	LPENUMIDLIST lpe = NULL;
	LPITEMIDLIST lpiTemp = NULL, lpifqThisItem = NULL;
	UINT uCount = 0;
	HRESULT hr;

	LPMALLOC lpMalloc;
	CoInitialize( NULL );
    hr = SHGetMalloc(&lpMalloc);

	if(FAILED(hr))
		return NULL;

	ULONG ulAttrs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_FILESYSTEM | SFGAO_GHOSTED | SFGAO_LINK | SFGAO_SHARE;

	// Determine what type of object you have.

	//if(!(lpi) || (lpi->mkid.cb >= 82))
	//		return NULL;

	lpsf->GetAttributesOf(	1, (const struct _ITEMIDLIST **)&lpi, &ulAttrs);

	SetTvMask(ulAttrs , &tvi, bChildValid);

		// You need this next if statement to 
		// avoid adding objects that are not real 
		// folders to the tree. Some objects can 
		// have subfolders but aren't real folders.
//		if (ulAttrs & SFGAO_FOLDER | SFGAO_FILESYSTEM)
		{

			// Get some memory for the ITEMDATA structure.

			LPTVITEMDATA* lptvid = NULL;

			lptvid = (LPTVITEMDATA*) lpMalloc->Alloc (sizeof (LPTVITEMDATA));
			if (! lptvid)
			{
				goto Done; // Error - could not allocate memory
				return NULL;
			}

			// Now get the friendly name to 
			// put in the tree view control.
            char szName[MAX_PATH];
			if (! GetName (lpsf, lpi, SHGDN_NORMAL, szName))
			{
	//				goto Done; // Error - could not get friendly name
			}
            MultiByteToWideChar(CP_ACP, 0, szName, -1, szBuff, MAX_PATH);

            if(_tcscmp(szTitle, _T("")) !=0 )
                _tcscpy(szBuff , szTitle);

			tvi.pszText = szBuff;
			tvi.cchTextMax = MAX_PATH;

	//			lpifqThisItem = Concatenate (lpMalloc, lpifq, lpi);

			// Now make a copy of the ITEMIDLIST.
			lptvid->lpi = CopyItemID (lpMalloc, lpi);
	//			GetNormalAndSelectedIcons (lpifqThisItem, &tvi);

			lptvid->lpsfParent = lpsf; // pointer to parent folder
			lpsf->AddRef ();

            lptvid->bRoot = bRoot;
			lptvid->lpifq =  Concatenate(lpMalloc, lpifq, lpi);
			GetNormalAndSelectedIcons (lptvid->lpifq, &tvi);
			tvi.lParam = (LPARAM)lptvid;

			// Populate the tree view insert structure.
			// The item is the one filled above.
			// Insert it after the last item inserted at this level,
			// and indicate that this is a root entry.
			tvins->item = tvi;
			tvins->hInsertAfter = hPrev;
			tvins->hParent = hParent; //NULL; //hParent;

			// Add the item to the tree.
//			hPrev = mTree->InsertItem (&tvins);

		}
	//	}
	
Done:
	lpMalloc->Release();
//	lpsf->Release();
    CoUninitialize();
	return hPrev;
}

/*******************************************************
*  Function : SetTvMask(ULONG, TVITEM, BOOL)
*  
*  Parameters : 
*
*  Purpose  : To set Mask for a Tree Item , this is a
*             helper function
* 
*  Comments : 
*
*  Return : none
********************************************************/
void 	CShellClass::SetTvMask(ULONG ulAttrs , TVITEM *tvi , BOOL bChildValid)
{
	tvi->mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	if (ulAttrs & SFGAO_HASSUBFOLDER)
	{
		// This item has subfolders, so put a plus sign in the 
		// tree view control. The first time the user clicks 
		// the item, you should populate the subfolders.
		if(bChildValid)
		{
			tvi->cChildren = 1;
			tvi->mask |= TVIF_CHILDREN;
		}
	}

	if (ulAttrs & SFGAO_GHOSTED)
	{
		tvi->mask |= LVIF_STATE;
		tvi->stateMask = LVIS_CUT;
		tvi->state = LVIS_CUT;
	}
	if (ulAttrs & SFGAO_LINK)
	{
		tvi->mask |= LVIF_STATE;
		tvi->stateMask = LVIS_OVERLAYMASK;
		tvi->state = INDEXTOOVERLAYMASK(2);
	}
	if (ulAttrs & SFGAO_SHARE)
	{
		tvi->mask |= LVIF_STATE;
		tvi->stateMask = LVIS_OVERLAYMASK;
		tvi->state = INDEXTOOVERLAYMASK(1);
	}
}

/*******************************************************
*  Function : GetNormalIcon(LPITEMIDLIST)
*  
*  Parameters : fully qualified Item Id List
*
*  Purpose  : To return index of the passed ItemIDList
*             generally used in ListView;
* 
*  Comments : 
*
*  Return : int
********************************************************/
int CShellClass::GetNormalIcon(LPITEMIDLIST lpifq)
{
	int nIconIndex;
	nIconIndex = GetIcon (lpifq, 
				 		SHGFI_PIDL | 
						CSIDL_DESKTOP |
						SHGFI_ICON |
						SHGFI_SYSICONINDEX | 
						SHGFI_SMALLICON);

	return nIconIndex;
}



