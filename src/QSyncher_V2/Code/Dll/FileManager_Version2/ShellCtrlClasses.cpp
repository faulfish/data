/************************************************
THIS CODE AND INFORMATION IS PROVIDED 'AS IS' 
WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED 
OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY 
AND/OR FITNESS FOR A PARTICULAR PURPOSE.
Author: Barretto VN  7/2002
*************************************************/


// ShellCtrlClasses.cpp : implementation file
//

#include "stdafx.h"
#include "ShellCtrlClasses.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
// These key is CLSID for special folder
const CString My_Computer           = _T("{20D04FE0-3AEA-1069-A2D8-08002B30309D}");
const CString Control_Panel         = _T("{21EC2020-3AEA-1069-A2DD-08002B30309D}");
const CString My_Documents          = _T("{450D8FBA-AD25-11D0-98A8-0800361B1103}");
const CString Recycle_Bin           = _T("{645FF040-5081-101B-9F08-00AA002F954E}");
const CString Default_Navigator     = _T("{871C5380-42A0-1069-A2EA-08002B30309D}");
const CString Network_Neighbourhood = _T("{208D2C60-3AEA-1069-A2D7-08002B30309D}");

/////////////////////////////////////////////////////////////////////////////
// CShellCtrlClasses

CShellCtrlClasses::CShellCtrlClasses()
{
}

CShellCtrlClasses::~CShellCtrlClasses()
{
}


BEGIN_MESSAGE_MAP(CShellCtrlClasses, CWnd)
    //{{AFX_MSG_MAP(CShellCtrlClasses)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CShellCtrlClasses message handlers


/******************************************
*    List Control Class (CShellListCtrl)
*
*
*
*
*
*******************************************/

/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl

CShellListCtrl::CShellListCtrl()
{
    giCtr = 0;
    // Allocate Shell Memory at start-up 
    // for use in various functions throught
    // the application
    HRESULT hr = SHGetMalloc(&m_pMalloc);
    if(FAILED(hr)) {
        AfxMessageBox(IDS_MEMORY_ERROR);
    }
    m_pCfgTool = CreateIConfigure(_T("FileManagerShellListCtrl"));
    m_pCfgTool->Load(STORAGE_INIFILE);
    DefineNotify(_T("TNotify_MEListSubMenu_DownloadNode"));
}

CShellListCtrl::~CShellListCtrl()
{
    m_pCfgTool->Save();
    DeleteIConfigure(_T("FileManagerShellListCtrl"));
    m_DropTarget.Unregister(this, EVENT_DROP);
    m_DropTarget.Unregister(this, EVENT_DRAG_ENTER);
    m_DropTarget.Unregister(this, EVENT_DRAG_OVER);
    m_DropTarget.Unregister(this, EVENT_DRAG_LEAVE);
}


BEGIN_MESSAGE_MAP(CShellListCtrl, CListCtrl)
    //{{AFX_MSG_MAP(CShellListCtrl)
    ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
    ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
    ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
    ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, &CShellListCtrl::OnLvnItemchanged)
    ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CShellListCtrl::OnLvnBegindrag)
    ON_NOTIFY(HDN_ENDTRACKA, 0, &CShellListCtrl::OnHdnEndtrack)
    ON_NOTIFY(HDN_ENDTRACKW, 0, &CShellListCtrl::OnHdnEndtrack)
    ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, &CShellListCtrl::OnHdnDividerdblclick)
    ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, &CShellListCtrl::OnHdnDividerdblclick)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellListCtrl message handlers


void CShellListCtrl::PreSubclassWindow()
{
    // TODO: Add your specialized code here and/or call the base class
    long nStyle = ::GetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE);
    ::SetWindowLong(this->GetSafeHwnd(), GWL_EXSTYLE, nStyle | WS_EX_ACCEPTFILES);
    m_DropTarget.Register(this);
    m_DropTarget.Register(this, EVENT_DROP);
    m_DropTarget.Register(this, EVENT_DRAG_ENTER);
    m_DropTarget.Register(this, EVENT_DRAG_OVER);
    m_DropTarget.Register(this, EVENT_DRAG_LEAVE);
    m_DropTarget.SetDropable(true);
    __super::PreSubclassWindow();
}


BOOL CShellListCtrl::SubclassDlgItem(UINT nID, CWnd* pParent)
{
    return CListCtrl::SubclassDlgItem(nID, pParent);
}

/*******************************************************
*  Function : InitilizeCtrl()
*
*  Parameters : none
*
*  Purpose  : Setup ListView Styles and Add columns 
*             and call to setup ImageList
*
*  Comments : Calls InsertListViewItem(...) to insert
*             individual items
*
*  Return : int
********************************************************/
int CShellListCtrl::InitilizeCtrl() 
{
    ModifyStyle(NULL, LVS_REPORT | LVS_SHAREIMAGELISTS, 0);
    //SetExtendedStyle(GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    SetupImageLists();
    InsertColumn(ID_COL_NAME, GetLocalizationString(_T("IDS_FileMgr_List_FileName")).c_str(), LVCFMT_LEFT  , m_pCfgTool->GetCfgVar(_T("Column0"), 140));
    InsertColumn(ID_COL_TYPE, GetLocalizationString(_T("IDS_FileMgr_List_FileAttr")).c_str(), LVCFMT_LEFT  , m_pCfgTool->GetCfgVar(_T("Column1"), 140));
    InsertColumn(ID_COL_SIZE, GetLocalizationString(_T("IDS_FileMgr_List_FileSize")).c_str(), LVCFMT_LEFT , m_pCfgTool->GetCfgVar(_T("Column2"), 140));
    InsertColumn(ID_COL_DATE, GetLocalizationString(_T("IDS_FileMgr_List_FileMDate")).c_str(), LVCFMT_LEFT  , m_pCfgTool->GetCfgVar(_T("Column3"), 140));
    return 0;
}

/*******************************************************
*  Function : LVPopulateFiles(LPTVITEMDATA*)
*
*  Parameters : User-defined structure
*
*  Purpose  : To Delete all existing items and
*             insert new items into the List Control
*             from the structure passed by enumerating
*             the passed user-defined structure
*
*  Comments : Calls InsertListViewItem(...) to insert
*             individual items
*
*  Return : none
********************************************************/
void CShellListCtrl::LVPopulateFiles(LPTVITEMDATA* lptvid) 
{
    HRESULT hr;
    ULONG celtFetched;
    LPITEMIDLIST pidlItems = NULL;
    LPENUMIDLIST ppenum = NULL;
    IShellFolder *psfProgFiles = NULL;

    if(lptvid->bRoot)
        psfProgFiles = lptvid->lpsfParent;
    else
    {
        hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, NULL, IID_IShellFolder, (LPVOID *) &psfProgFiles);
        if(FAILED(hr))
            return;
    }

    hr = psfProgFiles->EnumObjects(NULL,SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN, &ppenum);
    if(FAILED(hr))
        return;

    MyDeleteAllItems();
    SetRedraw(FALSE);
    while( hr = ppenum->Next(1,&pidlItems, &celtFetched) == S_OK && (celtFetched) == 1)
    {
        TCHAR szFilePath[MAX_PATH];
        SHGetPathFromIDList(pidlItems, szFilePath);
        CString strFilePath(&szFilePath[2]);
        if(strFilePath == Control_Panel || strFilePath == Recycle_Bin ||
            strFilePath == Default_Navigator || strFilePath == Network_Neighbourhood
            ) continue;
        ULONG uAttr = SFGAO_FOLDER | SFGAO_LINK | SFGAO_HIDDEN;
        hr = psfProgFiles->GetAttributesOf(1, (LPCITEMIDLIST*) &pidlItems, &uAttr);
        if((hr == S_OK) && ((uAttr&SFGAO_FOLDER) != SFGAO_FOLDER) &&
            ((uAttr&SFGAO_LINK) != SFGAO_LINK) && ((uAttr&SFGAO_HIDDEN) !=SFGAO_HIDDEN))
            InsertListViewItem(psfProgFiles , pidlItems, lptvid->lpifq);
    }
    SetRedraw(TRUE);
    return;
}	


/************************************************
*  Virtual Function : OnGetdispinfo(....)
*
*  Purpose : Display Text and Icons "On-Demand",
*            this function is called by the
*            system framework when it needs to
*            display items in the ListView
*
*  Comment : 
*
*************************************************/
void CShellListCtrl::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
    LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;


    LPTVITEMDATA* lptvid = NULL;
    lptvid = (LPTVITEMDATA*) m_pMalloc->Alloc (sizeof (LPTVITEMDATA));
    lptvid = (LPTVITEMDATA*)pDispInfo->item.lParam;
    CShellClass csc;
    char szBuff[MAX_PATH];

    DWORD dwStyles = SHGFI_PIDL|SHGFI_TYPENAME;
    SHFILEINFO sfi;
    TCHAR szFilePath[MAX_PATH] = {0};
    SHGetFileInfo ((LPCTSTR)lptvid->lpi, 0, &sfi, sizeof (SHFILEINFO), dwStyles);

    WIN32_FIND_DATA fd;
    SHGetDataFromIDList(lptvid->lpsfParent , lptvid->lpi, SHGDFIL_FINDDATA , (WIN32_FIND_DATA*)&fd , sizeof(fd));

    if (pDispInfo->item.mask & LVIF_IMAGE)
    {
        pDispInfo->item.iImage = csc.GetNormalIcon(lptvid->lpifq);

        ULONG uAttr = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_FILESYSTEM | SFGAO_GHOSTED | SFGAO_LINK | SFGAO_SHARE;
        lptvid->lpsfParent->GetAttributesOf(1, (LPCITEMIDLIST *) &lptvid->lpi, &uAttr);
        if (uAttr & SFGAO_GHOSTED)
        {
            pDispInfo->item.mask |= LVIF_STATE;
            pDispInfo->item.stateMask = LVIS_CUT;
            pDispInfo->item.state = LVIS_CUT;
        }
        if (uAttr & SFGAO_LINK)
        {
            pDispInfo->item.mask |= LVIF_STATE;
            pDispInfo->item.stateMask = LVIS_OVERLAYMASK;
            pDispInfo->item.state = INDEXTOOVERLAYMASK(2);
        }
        if (uAttr & SFGAO_SHARE)
        {
            pDispInfo->item.mask |= LVIF_STATE;
            pDispInfo->item.stateMask = LVIS_OVERLAYMASK;
            pDispInfo->item.state = INDEXTOOVERLAYMASK(1);
        }

    }

    if (pDispInfo->item.mask & LVIF_TEXT)
    {
        switch(pDispInfo->item.iSubItem)
        {

        case ID_COL_NAME:
            {
                csc.GetName(lptvid->lpsfParent, lptvid->lpi, SHGDN_NORMAL, szBuff);
                MultiByteToWideChar(CP_ACP, 0, szBuff, -1, pDispInfo->item.pszText, MAX_PATH);
                //_tcscpy(pDispInfo->item.pszText, (LPTSTR)szBuff);
                break;
            }
        case ID_COL_TYPE:
            {
                SHFILEINFO fileInfo;
                SHGetFileInfo((LPTSTR)lptvid->lpi, NULL, &fileInfo, sizeof(fileInfo), SHGFI_PIDL|SHGFI_TYPENAME);
                //MultiByteToWideChar(CP_ACP, 0, fileInfo.szTypeName, -1, pDispInfo->item.pszText, MAX_PATH);
                _tcscpy(pDispInfo->item.pszText,fileInfo.szTypeName);
                break;
            }
        case ID_COL_SIZE:
            {
                if(fd.dwFileAttributes != 3435973836)
                {
                    char sNumBuff[30];
                    if(fd.nFileSizeLow)
                    {
                        if(fd.nFileSizeLow > 1024)
                        {
                            _ltoa((long)fd.nFileSizeLow/1024,sNumBuff,10);
                            strcat(sNumBuff, " KB");
                        }
                        else
                            _ltoa((long)fd.nFileSizeLow,sNumBuff,10);
                    }
                    else
                        strcpy(sNumBuff,"");
                    MultiByteToWideChar(CP_ACP, 0, sNumBuff, -1, pDispInfo->item.pszText, MAX_PATH);
                    //_tcscpy(pDispInfo->item.pszText,(LPCTSTR)sNumBuff);
                }
                break;
            }

        case ID_COL_DATE:
            {
                if(fd.dwFileAttributes != 3435973836)
                {
                    TCHAR refTime[512] = {0};
                    SYSTEMTIME st;
                    FileTimeToSystemTime(&fd.ftLastWriteTime, &st);
                    wsprintf(refTime, _T("%04u/%02u/%02u %02u:%02u:%02u"), st.wYear, st.wMonth, st.wDay,
                        st.wHour, st.wMinute, st.wSecond); 
                    _tcscpy(pDispInfo->item.pszText, refTime);
                }
                break;
            }
        }
    }

    *pResult = 0;
}
/************************************************
*  Virtual Function : OnDblclk(....)
*
*  Purpose : Called by the Framework when the user
*            double-click on a the List Control area
*
*  Comment : if the clicked item is a folder, 
*            SelectThisItem(..) function in the 
*            CShellTreeView class is called to
*            expand the the Treeview after searching
*            for the passed character string
*
*************************************************/
void CShellListCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{

    // User has double-clicked, get the clicked Item
    // depending on the clicked point
    LVHITTESTINFO lvhInf;
    GetCursorPos(&lvhInf.pt);
    ScreenToClient(&lvhInf.pt);
    int item = ListView_HitTest(m_hWnd, &lvhInf);
    if((LVHT_ONITEMLABEL & lvhInf.flags ) || (LVHT_ONITEMICON & lvhInf.flags))
    {

        LPTVITEMDATA* lptvid = NULL;
        lptvid = (LPTVITEMDATA*) m_pMalloc->Alloc (sizeof (LPTVITEMDATA));

        LVITEM lvi;
        lvi.mask = LVIF_PARAM;
        lvi.iItem = lvhInf.iItem;
        ListView_GetItem(m_hWnd, &lvi);
        lptvid = (LPTVITEMDATA*)lvi.lParam;

        ULONG uAttr = SFGAO_FOLDER;
        lptvid->lpsfParent->GetAttributesOf(1, (LPCITEMIDLIST *) &lptvid->lpi, &uAttr);

        // is the item a Folder

        if(uAttr & SFGAO_FOLDER)
        {
            char szBuff[MAX_PATH];
            CShellClass csc;
            csc.GetName(lptvid->lpsfParent , lptvid->lpi , SHGDN_NORMAL, szBuff);
            //		   this->m_pShellTreeCtrl->SelectThisItem(szBuff);
        }
        else
            // display a popup-menu
            ShowStdMenu(FALSE, lptvid);

    }

    *pResult = 0;
}

/*******************************************************
*  Function : InsertListViewItem(LPSHELLFOLDER,
*                                LPITEMIDLIST,
*                                LPITEMIDLIST)
*
*  Parameters : Shellfolder, ItemIDList of the selected
*               item , and Fully Qualified ItemIDList of
*               the selected item
*
*  Purpose  : Create a structure and insert the
*               item in the ListView for later retreival
*               and update in OnGetdispinfo Virtual
*               function
* Comments : This function passes the LPARAM to 
*            the ListView but does not itself 
*            display the item, Icons and Text 
*            are inserted and displayed on-demand 
*            in the function OnGetdispinfo() below
*
*  Return : UINT
********************************************************/
BOOL CShellListCtrl::InsertListViewItem(LPSHELLFOLDER lpsf, 
                                        LPITEMIDLIST lpi,
                                        LPITEMIDLIST lpifq)
{

    UINT uFlags;
    LV_ITEM lvi;
    lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
    CShellClass csc;
    char szBuff[MAX_PATH];
    csc.GetName (lpsf, lpi, SHGDN_NORMAL, szBuff);

    lvi.iItem = giCtr++;
    lvi.iSubItem = ID_COL_NAME;
    lvi.pszText =  LPSTR_TEXTCALLBACK;
    TCHAR szwBuf[MAX_PATH];
    MultiByteToWideChar(CP_ACP, 0, szBuff, -1, szwBuf, MAX_PATH);
    //lvi.pszText = szwBuf; 
    uFlags = SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON;
    LPTVITEMDATA* lptvid = NULL;
    lptvid = (LPTVITEMDATA*) m_pMalloc->Alloc (sizeof (LPTVITEMDATA));

    lvi.iImage = I_IMAGECALLBACK; 
    lptvid->lpsfParent = lpsf;
    lptvid->lpi = csc.CopyItemID(m_pMalloc, lpi);
    lptvid->lpifq = csc.Concatenate(m_pMalloc, lpifq,lpi); 
    lvi.lParam = (LPARAM)lptvid;
    // This function passes the LPARAM to the ListView
    // but does not itself display the item
    // Icons and Text are inserted and displayed
    // on-demand in the function OnGetdispinfo() below
    int iItem = InsertItem (&lvi);

    return TRUE;
}

void CShellListCtrl::MyDeleteAllItems()
{
    DeleteAllItems();
    giCtr = 0;
}


/*******************************************************
*  Function : SetupImageLists()
*  
*  Parameters : none
*
*  Purpose  : Get ImageLists of 3 type 
*             (Normal / Small / State)
*             from the CShellClass and
*             add to the ListView
* 
*  Comments : 
*
*  Return : void
********************************************************/
void CShellListCtrl::SetupImageLists()
{
    CShellClass csc;
    HIMAGELIST himlSmall, himlLarge;
    himlSmall = csc.GetImageList(TRUE);
    himlLarge = csc.GetImageList(FALSE);
    SetImageList(m_pImageListL.FromHandle(himlLarge), LVSIL_NORMAL); 
    SetImageList(m_pImageListS.FromHandle(himlSmall), LVSIL_SMALL); 
    SetImageList(m_pImageListS.FromHandle(himlSmall), LVSIL_STATE); 
}

/*******************************************************
*  Function : ShowStdMenu(BOOL, LPTVITEMDATA*)
*
*  Parameters : True if menu should be displayed, User-defined
*               structure
*
*  Purpose  :  Call CShellContextMenuClass class to
*              display the Shells Context-menu depending
*              on the parameter 1
*
*  Comments : 
*
*  Return : none
********************************************************/
void CShellListCtrl::ShowStdMenu(BOOL bShowMenu ,  LPTVITEMDATA* lptvid)
{
    CShellContextMenuClass cmc;
    cmc.ShowMenu(lptvid, bShowMenu, m_hWnd);
    return;
}

/************************************************
*  Virtual Function : OnRclick(....)
*
*  Purpose : Called by the Framework when the user
*            right-click on the client area
*
*  Comment : Calls CShellContextMenuClass class to 
*            display a pop-menu menu to select 
*            ListView Styles like Small Icons, Large 
*            Icons etc...
*
*************************************************/
void CShellListCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
    //POSITION pos = GetFirstSelectedItemPosition();
    //if(pos == NULL)
    //{
    //    CShellContextMenuClass cmc;
    //    cmc.ShowViewStyleMenu(*this);
    //}

    *pResult = 0;
}

void CShellListCtrl::OnLvnItemchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    LPTVITEMDATA* lptvid = NULL;
    lptvid = (LPTVITEMDATA*)pNMLV->lParam;
    // Get the absolute path this file , include file name
    TCHAR szFilePath[MAX_PATH] = {0};
    SHGetPathFromIDList(lptvid->lpifq, szFilePath);
    SetCurrentFilePath(szFilePath);

    // TODO: Add your control notification handler code here
    *pResult = 0;
}

void CShellListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    int nSelectCount = GetSelectedCount();
    POSITION pos = GetFirstSelectedItemPosition();
    CDragDropFireParam cddfp;
    cddfp.SetEffect(DROPEFFECT_COPY);
    while (pos) {
        int nItem = GetNextSelectedItem(pos);
        LVITEM lvitem = {0};
        lvitem.iItem = nItem;
        lvitem.mask = LVIF_PARAM;
        GetItem(&lvitem);
        LPTVITEMDATA* lptvid = (LPTVITEMDATA*)lvitem.lParam;
        TCHAR szFilePath[MAX_PATH] = {0};
        SHGetPathFromIDList(lptvid->lpifq, szFilePath);
        cddfp.Insert(szFilePath);
    }
    m_DropSource.BeginDrag(cddfp);
    *pResult = 0;
}


void CShellListCtrl::OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
    CString strColNum;
    strColNum.Format(_T("Column%d"), phdr->iItem);
    m_pCfgTool->SetCfgVar((LPTSTR)(LPCTSTR)strColNum, phdr->pitem->cxy);
    m_pCfgTool->Save(STORAGE_INIFILE);
    *pResult = 0;
}

void CShellListCtrl::OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: Add your control notification handler code here
    CString strColNum;
    strColNum.Format(_T("Column%d"), phdr->iItem);
    int nColumnWidth = GetColumnWidth(phdr->iItem);
    m_pCfgTool->SetCfgVar((LPTSTR)(LPCTSTR)strColNum, nColumnWidth);
    m_pCfgTool->Save(STORAGE_INIFILE);
    *pResult = 0;
}


void CShellListCtrl::RefreshList() {
    HTREEITEM hItem = m_pShellTreeCtrl->GetSelectedItem();
    TVITEM tvItem = {0};
    tvItem.cchTextMax = MAX_PATH;
    TCHAR szFolderName[MAX_PATH];
    tvItem.pszText = szFolderName;
    tvItem.hItem = hItem;
    tvItem.mask = TVIF_PARAM | TVIF_TEXT;
    m_pShellTreeCtrl->GetItem(&tvItem);
    LPTVITEMDATA* lptvid = (LPTVITEMDATA*)tvItem.lParam;
    LVPopulateFiles(lptvid);
}

void CShellListCtrl::Event(const TSTRING& strEvent,long nParam)
{
    if(strEvent == EVENT_DROP) {
        TRACE("EVENT DROP!!!\n");
        CDragDropFireParam* cddpf = (CDragDropFireParam*)nParam;
        Fire(_T("TNotify_MEListSubMenu_DownloadNode"));
    } else if(strEvent == EVENT_DRAG_ENTER) {
        TRACE("EVENT_DRAG_ENTER!!!\n");
    } else if(strEvent == EVENT_DRAG_OVER ) {
        TRACE("EVENT_DRAG_OVER!!!\n");
    } else if(strEvent == EVENT_DRAG_LEAVE) {
        TRACE("EVENT_DRAG_LEAVE!!!\n");
    } else {
    }
}





/******************************************
*    Tree Control Class (CShellTreeCtrl)
*
*
*
*
*
*******************************************/



/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl

CShellTreeCtrl::CShellTreeCtrl()
{
    m_pShellListCtrl = NULL;
    m_pSelectedItem = NULL;
    m_hParent = NULL;
    HRESULT hr = SHGetMalloc(&m_pMalloc);
    if(FAILED(hr)) {
        AfxMessageBox(IDS_MEMORY_ERROR);
    }
    m_pCfgTool = CreateIConfigure(_T("FileManagerShellTreeCtrl"));
    m_pCfgTool->Load(STORAGE_INIFILE);
}

CShellTreeCtrl::~CShellTreeCtrl()
{
    m_pCfgTool->Save();
    DeleteIConfigure(_T("FileManagerShellTreeCtrl"));
}


BEGIN_MESSAGE_MAP(CShellTreeCtrl, CTreeCtrl)
    //{{AFX_MSG_MAP(CShellTreeCtrl)
    ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
    //ON_NOTIFY_REFLECT(TVN_SELCHANGING, OnSelchanging)
    ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CShellTreeCtrl message handlers


BOOL CShellTreeCtrl::SubclassDlgItem(UINT nID, CWnd* pParent)
{
    return CTreeCtrl::SubclassDlgItem(nID, pParent);
}

/************************************************
*  Function : InitilizeCtrl()
*
*  Purpose : Set the initial Style of the Tree
*            control , create and setup ImageList
*            and Insert the Desktop Item
*
*  Comment : Calls SetImageLists() to setup
*            ImageList for display and the calls
*            InsertDesktopItem(...) to insert the 
*            Desktop Folder Item
*
*************************************************/
void CShellTreeCtrl::InitializeCtrl() 
{

    ModifyStyle(NULL,  TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT, 0);

    SetupImages();

    LPSHELLFOLDER lpsf = NULL;
    HRESULT hr;

    HTREEITEM hParent = NULL;
    hr = SHGetDesktopFolder (&lpsf);
    if (SUCCEEDED (hr))
    {
        hParent = InsertDesktopItem(lpsf);
        lpsf->Release ();
    }
}

/*******************************************************
*  Function : SetupImageList()
*  
*  Parameters : none
*
*  Purpose  : Get ImageList and set it for 
*             use by the Tree when displaying Images
* 
*  Comments : Calls CShellClass to retreive a handle
*             to the Imagelists, we get 2 different
*             Image List one for normal and selected 
*             images and other for State Images (masked
*             type)
*
*  Return : void
********************************************************/
void CShellTreeCtrl::SetupImages()
{
    CShellClass csc;
    HIMAGELIST himagelist;
    himagelist = csc.GetImageList();
    SetImageList(m_pImageList.FromHandle(himagelist), TVSIL_NORMAL); 
    SetImageList(m_pImageList.FromHandle(himagelist), TVSIL_STATE); 
}

/*******************************************************
*  Function : InsertDesktopItem(LPSHELLFOLDER)
*  
*  Parameters : Initial Desktop Shell folder
*
*  Purpose  : To add the Desktop folder to the Tree
* 
*  Comments : Expand the Tree after adding the Desktop
*             item so as to force the Framework to 
*             call OnItemexpanding(..) to add further
*             items.
*
*  Return : HTREEITEM
********************************************************/
HTREEITEM CShellTreeCtrl::InsertDesktopItem(LPSHELLFOLDER lpsf)
{
    LPITEMIDLIST lpi = NULL;
    HTREEITEM hParent = NULL;
    CShellClass csc;
    TCHAR szBuff [MAX_PATH];
    TVINSERTSTRUCT tvins;
    SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP, &lpi);
    csc.InsertTreeItemTE(TRUE, &tvins, szBuff, NULL, NULL , lpsf , NULL, lpi , TRUE, _T(""));
    m_hParent = InsertItem(&tvins);

    SelectItem(m_hParent);
    TVITEM tvitem = {0};
    tvitem.hItem = m_hParent;
    tvitem.mask = TVIF_PARAM | TVIF_HANDLE;
    GetItem(&tvitem);
    //Expand(m_hParent, TVE_EXPAND);
    LPTVITEMDATA* lptvid = (LPTVITEMDATA*)tvitem.lParam;
    if(m_pShellListCtrl != NULL)
        m_pShellListCtrl->LVPopulateFiles(lptvid);
    TCHAR szPath[MAX_PATH] = {0};
    SHGetPathFromIDList(lptvid->lpifq, szPath);
    CString strFolderPath(szPath);
    if(szPath != _T("")) SetCurrentFolderPath(szPath);
    return m_hParent;
}

/*******************************************************
*  Function : PopulateTree(NMHDR*)
*  
*  Parameters : Header of the selected Tree Item,
*               called by virtual function 
*               OnItemexpanding(..)
*
*  Purpose  : To delete all child item of the sent
*             item populate with new items
* 
*  Comments : If we are using a ListView, the items
*             in the Listview are first deleted and
*             new added depening on the LPARAM of the
*             user-defined structure (LPTVITEMDATA*)
*
*  Return : BOOL
********************************************************/
BOOL CShellTreeCtrl::PopulateTree(NMHDR *pNMHDR)
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hItem = pNMTreeView->itemNew.hItem;
    LPTVITEMDATA* lptvid = NULL;

    HRESULT hr;
    lptvid = (LPTVITEMDATA*) m_pMalloc->Alloc (sizeof (LPTVITEMDATA));
    if (! lptvid)
    {
        AfxMessageBox(IDS_MEMORY_ERROR);
        return FALSE; // Error - could not allocate memory
    }

    lptvid = (LPTVITEMDATA*)pNMTreeView->itemNew.lParam;
    if(lptvid == NULL)
        return FALSE;

    LPENUMIDLIST lpe = NULL;	
    ULONG celtFetched;
    LPITEMIDLIST pidlItems = NULL;
    LPENUMIDLIST ppenum = NULL;
    IShellFolder *psfProgFiles = NULL;

    CWaitCursor wait;

    if(lptvid->bRoot)
        psfProgFiles = lptvid->lpsfParent;
    else
    {
        hr = lptvid->lpsfParent->BindToObject(lptvid->lpi, NULL, IID_IShellFolder, (LPVOID *) &psfProgFiles);
        if(psfProgFiles == NULL)
            return FALSE;
    }

    hr = psfProgFiles->EnumObjects(NULL,SHCONTF_FOLDERS | SHCONTF_NONFOLDERS | SHCONTF_INCLUDEHIDDEN , &ppenum);
    if(ppenum == NULL)
        return FALSE;

    DeleteChildren(hItem);

    if(m_pShellListCtrl != NULL)
        m_pShellListCtrl->MyDeleteAllItems();

    LPMALLOC pMalloc = NULL;
    hr = SHGetMalloc(&pMalloc);	if(hr != S_OK) return FALSE;

    this->BeginWaitCursor();
    UINT uiIndex = 1;
    BOOL bHasChildrenItem = FALSE;
    // Get the SUB ITEM from ppenum
    while( hr = ppenum->Next(1,&pidlItems, &celtFetched) == S_OK && (celtFetched) == 1)
    {
        TCHAR szBuff[MAX_PATH] = {0};
        TVINSERTSTRUCT tvins;
        ULONG uAttr = SFGAO_FOLDER | SFGAO_HASPROPSHEET | SFGAO_CANDELETE | SFGAO_CANLINK |
            SFGAO_SHARE | SFGAO_CANRENAME | SFGAO_CANMOVE | SFGAO_CANCOPY | SFGAO_LINK |
            SFGAO_FILESYSTEM | SFGAO_BROWSABLE | SFGAO_HIDDEN;
        psfProgFiles->GetAttributesOf(1, (LPCITEMIDLIST *) &pidlItems, &uAttr);
        CShellClass csc;

        TCHAR szPath[MAX_PATH] = {0};
        SHGetPathFromIDList(pidlItems, szPath);
        CString strAbsPath(&szPath[2]);
        // filter Control panel, Recycle bin, IE, Network neighborhood
        if(strAbsPath == Control_Panel || strAbsPath == Recycle_Bin ||
            strAbsPath == Default_Navigator || strAbsPath == Network_Neighbourhood
            ) continue;
        // filter hidden file
        if((uAttr & SFGAO_HIDDEN)) continue;
        // filter control panel
        if(!(uAttr & SFGAO_FILESYSTEM) && strAbsPath != My_Computer) continue;

        if(m_pShellListCtrl == NULL)
        {
            csc.InsertTreeItemTE(FALSE, &tvins, szBuff, hItem, NULL , psfProgFiles , lptvid->lpifq, pidlItems , TRUE, _T(""));
            HTREEITEM hPrev = InsertItem(&tvins);
        }
        else
        {
            // if the item is not a folder, filter it
            if(uAttr & SFGAO_FOLDER)
            {
                csc.InsertTreeItemTE(FALSE, &tvins, szBuff, hItem, NULL , psfProgFiles , lptvid->lpifq, pidlItems , TRUE, _T(""));
                HTREEITEM hPrev = InsertItem(&tvins);
            }
        }
    }

    return TRUE;
}

/*******************************************************
*  Function : DeleteChildren(HTREEITEM)
*  
*  Parameters : selected Tree Item
*
*  Purpose  : Delete all child Items of this Item
* 
*  Comments : 
*
*  Return : UINT
********************************************************/
UINT CShellTreeCtrl::DeleteChildren(HTREEITEM hItem)
{

    UINT nCount = 0;
    HTREEITEM hChild = GetChildItem (hItem);

    while (hChild != NULL) {
        HTREEITEM hNextItem = GetNextSiblingItem (hChild);
        DeleteItem (hChild);
        hChild = hNextItem;
        nCount++;
    }
    return nCount;
}

/************************************************
*  Virtual Function : OnItemexpanding(....)
*
*  Purpose : Populate the Tree Item (if Expanding)
*            also populate the ListView depending
*            on the Tree Item clicked
*
*  Comment : 
*
*************************************************/
void CShellTreeCtrl::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    if(PopulateTree(pNMHDR))
        *pResult = 0;
    else
        *pResult = 1;
    if(m_pShellListCtrl) {
        HTREEITEM hItem = GetSelectedItem();
        if(hItem != NULL) {
            TCHAR szItemText[MAX_PATH];
            TVITEM tvItem = {0};
            tvItem.hItem = hItem;
            tvItem.cchTextMax = MAX_PATH;
            tvItem.pszText = szItemText;
            tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_STATE;
            GetItem(&tvItem);
            LPTVITEMDATA* lptvid = NULL;
            lptvid = (LPTVITEMDATA*)m_pMalloc->Alloc (sizeof (LPTVITEMDATA));
            lptvid = (LPTVITEMDATA*)tvItem.lParam;
            m_pShellListCtrl->LVPopulateFiles(lptvid);
            *pResult = 0;
        }
    }
}

/************************************************
*  Virtual Function : OnSelchanging(....)
*
*  Purpose : User clicked on the Text of a Tree item
*            populate the ListView if it exists else
*            show the Shells pop-up menu for the 
*            clicked item
*
*  Comment : 
*
*************************************************/
void CShellTreeCtrl::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
    NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    HTREEITEM hItem = pNMTreeView->itemNew.hItem;

    if(!(pNMTreeView->action))
        return;

    LPTVITEMDATA* lptvid = NULL;
    m_pSelectedItem = hItem;

    lptvid = (LPTVITEMDATA*) m_pMalloc->Alloc (sizeof (LPTVITEMDATA));
    if (! lptvid)
    {
        AfxMessageBox(IDS_MEMORY_ERROR);
        return; // Error - could not allocate memory
    }

    lptvid = (LPTVITEMDATA*)pNMTreeView->itemNew.lParam;
    if(lptvid == NULL)
        return;
    TCHAR szPath[MAX_PATH] = {0};
    SHGetPathFromIDList(lptvid->lpifq, szPath);
    CString strFolderPath(szPath);
    if(szPath != _T("")) SetCurrentFolderPath(szPath);
    if(pNMTreeView->action != 4096) {
        if(m_pShellListCtrl != NULL)
            m_pShellListCtrl->LVPopulateFiles(lptvid);
        else
        {
            ULONG uAttr = SFGAO_FOLDER;
            lptvid->lpsfParent->GetAttributesOf(1, (LPCITEMIDLIST *) &lptvid->lpi, &uAttr);
            if(!(uAttr & SFGAO_FOLDER))
            {
                CShellContextMenuClass cmc;
                cmc.ShowMenu(lptvid, FALSE, m_hWnd);
            }
        }
    }

    *pResult = 0;
}








/******************************************
*    Context Menu Class (CContextMenuClass)
*
*
*
*
*
*******************************************/

CShellContextMenuClass::CShellContextMenuClass()
{
}

CShellContextMenuClass::~CShellContextMenuClass()
{
}


/*******************************************************
*  Function : ShowMenu(LPTVITEMDATA, BOOL, HWND)
*  
*  Parameters : 1) ItemIDList of the selected item
*               2) True if menu should be displayed 
*				   False otherwise
*				3) Handle of the control (could be
*				   List or Tree control)
* 
*  Purpose : Displays a pop-up menu depending on the
*            parameters passed else executes the command
*
*  Comments : 
*
*  Return : void
********************************************************/

void CShellContextMenuClass::ShowMenu(LPTVITEMDATA *lptvid, 
                                      BOOL bShowMenu,
                                      HWND m_hWnd) 
{

    HRESULT hr;
    HMENU hMenuPopup;
    IContextMenu   *icm;
    IContextMenu2* icm2;
    static POINT point;
    GetCursorPos(&point);

    hr = lptvid->lpsfParent->GetUIObjectOf(m_hWnd, 
        1, 
        (LPCITEMIDLIST*)&lptvid->lpi,
        IID_IContextMenu, 
        NULL, 
        (LPVOID*)&icm);

    if(FAILED(hr))
        return;

    hMenuPopup = CreatePopupMenu();
    if(!hMenuPopup)
        return;

    hr = icm->QueryContextMenu(hMenuPopup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);
    if(FAILED(hr))
        return;

    icm->QueryInterface(IID_IContextMenu2, (LPVOID*)&icm2);
    UINT  id;
    if(bShowMenu)
        id = TrackPopupMenu(hMenuPopup, 
        TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON, 
        point.x,point.y,0,m_hWnd,NULL);
    else
        id = hr;

    if(!id)
        return;

    CMINVOKECOMMANDINFO  ici;
    ici.cbSize = sizeof(CMINVOKECOMMANDINFO);
    ici.fMask = 0;
    ici.hwnd = m_hWnd;
    ici.lpVerb = (LPCSTR)(INT_PTR)(id - 1);
    ici.lpParameters = NULL;
    ici.lpDirectory = NULL;
    ici.nShow = SW_SHOWNORMAL;
    ici.dwHotKey = 0;
    ici.hIcon = NULL;
    hr = icm->InvokeCommand(&ici);

}


/*******************************************************
*  Function : ShowViewStyleMenu(CListCtrl)
*  
*  Parameters : Referance of the ListControl
* 
*  Purpose : Display a popup menu and change the
*            ListView Style depending on the selection
*
*  Comments : 
*
*  Return : void
********************************************************/

void CShellContextMenuClass::ShowViewStyleMenu(CListCtrl& mListCtrl)
{

#define ID_LARGEICONS 5001
#define ID_SMALLICONS 5002
#define ID_LIST       5003
#define ID_DETAILS    5004
#define ID_LINES      5005

    CMenu menu;
    menu.CreatePopupMenu();
    menu.AppendMenu(MF_STRING | MF_ENABLED, ID_LARGEICONS, _T("&Large Icons"));
    menu.AppendMenu(MF_STRING | MF_ENABLED, ID_SMALLICONS, _T("&Small Icons"));
    menu.AppendMenu(MF_STRING | MF_ENABLED, ID_LIST      , _T("L&ist"));
    menu.AppendMenu(MF_STRING | MF_ENABLED, ID_DETAILS   , _T("&Details"));

    POINT pt;
    GetCursorPos(&pt);
    UINT mOptionSelected = menu.TrackPopupMenu(TPM_LEFTALIGN |
        TPM_RIGHTBUTTON | TPM_RETURNCMD, pt.x, pt.y, mListCtrl.FromHandle(mListCtrl.m_hWnd));

    switch(mOptionSelected)
    {
    case ID_LARGEICONS:
        mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_ICON);
        break;
    case ID_SMALLICONS:
        mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_SMALLICON);
        break;
    case ID_LIST:
        mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_LIST);
        break;
    case ID_DETAILS:
        mListCtrl.ModifyStyle(LVS_TYPEMASK, LVS_REPORT);
        break;
    }
}



#pragma warning(pop)