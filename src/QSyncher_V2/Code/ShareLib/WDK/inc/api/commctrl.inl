/* Copyright (c) 2001-2006, Microsoft Corp. All rights reserved. */

#if _MSC_VER > 1000
#pragma once
#endif

#if defined(__cplusplus)
extern "C" {
#endif


#if !defined(RC_INVOKED) /* RC complains about long symbols in #ifs */
#if defined(ISOLATION_AWARE_ENABLED) && (ISOLATION_AWARE_ENABLED != 0)


#if !defined(ISOLATION_AWARE_USE_STATIC_LIBRARY)
#define ISOLATION_AWARE_USE_STATIC_LIBRARY 0
#endif

#if !defined(ISOLATION_AWARE_BUILD_STATIC_LIBRARY)
#define ISOLATION_AWARE_BUILD_STATIC_LIBRARY 0
#endif

#if !defined(ISOLATION_AWARE_INLINE)
#if ISOLATION_AWARE_BUILD_STATIC_LIBRARY
#define ISOLATION_AWARE_INLINE /* nothing */
#else
#if defined(__cplusplus)
#define ISOLATION_AWARE_INLINE inline
#else
#define ISOLATION_AWARE_INLINE __inline
#endif
#endif
#endif

#if !ISOLATION_AWARE_USE_STATIC_LIBRARY
FARPROC IsolationAwarePrivatenCv CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY(LPCSTR pszProcName);

#endif /* ISOLATION_AWARE_USE_STATIC_LIBRARY */

#if !defined(ISOLATIONAWARECOMMCTRL_LPSTREAM)
#if !defined(__IStream_INTERFACE_DEFINED__)
  #if defined(interface)
    interface IStream; typedef interface IStream IStream;
  #else
    struct IStream; typedef struct IStream IStream;
  #endif
#endif
typedef IStream *ISOLATIONAWARECOMMCTRL_LPSTREAM;
#define ISOLATIONAWARECOMMCTRL_LPSTREAM ISOLATIONAWARECOMMCTRL_LPSTREAM
#endif
void IsolationAwarePrivatenCv IsolationAwareInitCommonControls(void);
BOOL IsolationAwarePrivatenCv IsolationAwareInitCommonControlsEx(const INITCOMMONCONTROLSEX*picce);
HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Create(int cx,int cy,UINT flags,int cInitial,int cGrow);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Destroy(HIMAGELIST himl);
int IsolationAwarePrivatenCv IsolationAwareImageList_GetImageCount(HIMAGELIST himl);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetImageCount(HIMAGELIST himl,UINT uNewCount);
int IsolationAwarePrivatenCv IsolationAwareImageList_Add(HIMAGELIST himl,HBITMAP hbmImage,HBITMAP hbmMask);
int IsolationAwarePrivatenCv IsolationAwareImageList_ReplaceIcon(HIMAGELIST himl,int i,HICON hicon);
COLORREF IsolationAwarePrivatenCv IsolationAwareImageList_SetBkColor(HIMAGELIST himl,COLORREF clrBk);
COLORREF IsolationAwarePrivatenCv IsolationAwareImageList_GetBkColor(HIMAGELIST himl);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetOverlayImage(HIMAGELIST himl,int iImage,int iOverlay);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Draw(HIMAGELIST himl,int i,HDC hdcDst,int x,int y,UINT fStyle);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Replace(HIMAGELIST himl,int i,HBITMAP hbmImage,HBITMAP hbmMask);
int IsolationAwarePrivatenCv IsolationAwareImageList_AddMasked(HIMAGELIST himl,HBITMAP hbmImage,COLORREF crMask);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DrawEx(HIMAGELIST himl,int i,HDC hdcDst,int x,int y,int dx,int dy,COLORREF rgbBk,COLORREF rgbFg,UINT fStyle);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DrawIndirect(IMAGELISTDRAWPARAMS*pimldp);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Remove(HIMAGELIST himl,int i);
HICON IsolationAwarePrivatenCv IsolationAwareImageList_GetIcon(HIMAGELIST himl,int i,UINT flags);
HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_LoadImageA(HINSTANCE hi,LPCSTR lpbmp,int cx,int cGrow,COLORREF crMask,UINT uType,UINT uFlags);
HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_LoadImageW(HINSTANCE hi,LPCWSTR lpbmp,int cx,int cGrow,COLORREF crMask,UINT uType,UINT uFlags);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Copy(HIMAGELIST himlDst,int iDst,HIMAGELIST himlSrc,int iSrc,UINT uFlags);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_BeginDrag(HIMAGELIST himlTrack,int iTrack,int dxHotspot,int dyHotspot);
void IsolationAwarePrivatenCv IsolationAwareImageList_EndDrag(void);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragEnter(HWND hwndLock,int x,int y);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragLeave(HWND hwndLock);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragMove(int x,int y);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetDragCursorImage(HIMAGELIST himlDrag,int iDrag,int dxHotspot,int dyHotspot);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragShowNolock(BOOL fShow);
HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_GetDragImage(POINT*ppt,POINT*pptHotspot);
HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Read(ISOLATIONAWARECOMMCTRL_LPSTREAM pstm);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Write(HIMAGELIST himl,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm);
#if (_WIN32_WINNT >= 0x0501)
HRESULT IsolationAwarePrivatenCv IsolationAwareImageList_ReadEx(DWORD dwFlags,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm,REFIID riid,PVOID*ppv);
HRESULT IsolationAwarePrivatenCv IsolationAwareImageList_WriteEx(HIMAGELIST himl,DWORD dwFlags,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm);
#endif /* (_WIN32_WINNT >= 0x0501) */
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_GetIconSize(HIMAGELIST himl,int*cx,int*cy);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetIconSize(HIMAGELIST himl,int cx,int cy);
BOOL IsolationAwarePrivatenCv IsolationAwareImageList_GetImageInfo(HIMAGELIST himl,int i,IMAGEINFO*pImageInfo);
HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Merge(HIMAGELIST himl1,int i1,HIMAGELIST himl2,int i2,int dx,int dy);
HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Duplicate(HIMAGELIST himl);
HRESULT IsolationAwarePrivatenCv IsolationAwareHIMAGELIST_QueryInterface(HIMAGELIST himl,REFIID riid,void**ppv);
HWND IsolationAwarePrivatenCv IsolationAwareCreateToolbarEx(HWND hwnd,DWORD ws,UINT wID,int nBitmaps,HINSTANCE hBMInst,UINT_PTR wBMID,LPCTBBUTTON lpButtons,int iNumButtons,int dxButton,int dyButton,int dxBitmap,int dyBitmap,UINT uStructSize);
HBITMAP IsolationAwarePrivatenCv IsolationAwareCreateMappedBitmap(HINSTANCE hInstance,INT_PTR idBitmap,UINT wFlags,__in_opt LPCOLORMAP lpColorMap,int iNumMaps);
void IsolationAwarePrivatenCv IsolationAwareDrawStatusTextA(HDC hDC,LPCRECT lprc,LPCSTR pszText,UINT uFlags);
void IsolationAwarePrivatenCv IsolationAwareDrawStatusTextW(HDC hDC,LPCRECT lprc,LPCWSTR pszText,UINT uFlags);
HWND IsolationAwarePrivatenCv IsolationAwareCreateStatusWindowA(long style,LPCSTR lpszText,HWND hwndParent,UINT wID);
HWND IsolationAwarePrivatenCv IsolationAwareCreateStatusWindowW(long style,LPCWSTR lpszText,HWND hwndParent,UINT wID);
void IsolationAwarePrivatenCv IsolationAwareMenuHelp(UINT uMsg,WPARAM wParam,LPARAM lParam,HMENU hMainMenu,HINSTANCE hInst,HWND hwndStatus,__in_ecount(4) UINT*lpwIDs);
BOOL IsolationAwarePrivatenCv IsolationAwareShowHideMenuCtl(HWND hWnd,UINT_PTR uFlags,__in __nullterminated LPINT lpInfo);
void IsolationAwarePrivatenCv IsolationAwareGetEffectiveClientRect(HWND hWnd,LPRECT lprc,__in __nullterminated const int*lpInfo);
BOOL IsolationAwarePrivatenCv IsolationAwareMakeDragList(HWND hLB);
void IsolationAwarePrivatenCv IsolationAwareDrawInsert(HWND handParent,HWND hLB,int nItem);
int IsolationAwarePrivatenCv IsolationAwareLBItemFromPt(HWND hLB,POINT pt,BOOL bAutoScroll);
HWND IsolationAwarePrivatenCv IsolationAwareCreateUpDownControl(DWORD dwStyle,int x,int y,int cx,int cy,HWND hParent,int nID,HINSTANCE hInst,HWND hBuddy,int nUpper,int nLower,int nPos);
HRESULT IsolationAwarePrivatenCv IsolationAwareTaskDialogIndirect(const TASKDIALOGCONFIG*pTaskConfig,__out_opt int*pnButton,__out_opt int*pnRadioButton,__out_opt BOOL*pfVerificationFlagChecked);
HRESULT IsolationAwarePrivatenCv IsolationAwareTaskDialog(__in_opt HWND hwndParent,__in_opt HINSTANCE hInstance,__in_opt PCWSTR pszWindowTitle,__in_opt PCWSTR pszMainInstruction,__in_opt PCWSTR pszContent,TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons,__in_opt PCWSTR pszIcon,__out_opt int*pnButton);
void IsolationAwarePrivatenCv IsolationAwareInitMUILanguage(LANGID uiLang);
LANGID IsolationAwarePrivatenCv IsolationAwareGetMUILanguage(void);
HDSA IsolationAwarePrivatenCv IsolationAwareDSA_Create(int cbItem,int cItemGrow);
BOOL IsolationAwarePrivatenCv IsolationAwareDSA_Destroy(__in HDSA hdsa);
void IsolationAwarePrivatenCv IsolationAwareDSA_DestroyCallback(__in HDSA hdsa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
BOOL IsolationAwarePrivatenCv IsolationAwareDSA_DeleteItem(__in HDSA hdsa,__in int i);
BOOL IsolationAwarePrivatenCv IsolationAwareDSA_DeleteAllItems(__in HDSA hdsa);
void IsolationAwarePrivatenCv IsolationAwareDSA_EnumCallback(__in HDSA hdsa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
int IsolationAwarePrivatenCv IsolationAwareDSA_InsertItem(__in HDSA hdsa,__in int i,__in const void*pitem);
PVOID IsolationAwarePrivatenCv IsolationAwareDSA_GetItemPtr(__in HDSA hdsa,__in int i);
BOOL IsolationAwarePrivatenCv IsolationAwareDSA_GetItem(__in HDSA hdsa,__in int i,__out_xcount(pdsa->cbItem) void*pitem);
BOOL IsolationAwarePrivatenCv IsolationAwareDSA_SetItem(__in HDSA hdsa,__in int i,__in const void*pitem);
HDSA IsolationAwarePrivatenCv IsolationAwareDSA_Clone(__in HDSA hdsa);
ULONGLONG IsolationAwarePrivatenCv IsolationAwareDSA_GetSize(__in HDSA hdsa);
BOOL IsolationAwarePrivatenCv IsolationAwareDSA_Sort(__in HDSA pdsa,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam);
HDPA IsolationAwarePrivatenCv IsolationAwareDPA_Create(int cItemGrow);
HDPA IsolationAwarePrivatenCv IsolationAwareDPA_CreateEx(__in int cpGrow,__in_opt HANDLE hheap);
HDPA IsolationAwarePrivatenCv IsolationAwareDPA_Clone(__in const HDPA hdpa,__in HDPA hdpaNew);
BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Destroy(__in HDPA hdpa);
void IsolationAwarePrivatenCv IsolationAwareDPA_DestroyCallback(__in HDPA hdpa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
PVOID IsolationAwarePrivatenCv IsolationAwareDPA_DeletePtr(__in HDPA hdpa,__in int i);
BOOL IsolationAwarePrivatenCv IsolationAwareDPA_DeleteAllPtrs(__in HDPA hdpa);
void IsolationAwarePrivatenCv IsolationAwareDPA_EnumCallback(__in HDPA hdpa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Grow(__in HDPA pdpa,__in int cp);
int IsolationAwarePrivatenCv IsolationAwareDPA_InsertPtr(__in HDPA hdpa,__in int i,__in_opt void*p);
BOOL IsolationAwarePrivatenCv IsolationAwareDPA_SetPtr(__in HDPA hdpa,__in int i,__in_opt void*p);
PVOID IsolationAwarePrivatenCv IsolationAwareDPA_GetPtr(__in HDPA hdpa,__in INT_PTR i);
int IsolationAwarePrivatenCv IsolationAwareDPA_GetPtrIndex(__in HDPA hdpa,__in const void*p);
ULONGLONG IsolationAwarePrivatenCv IsolationAwareDPA_GetSize(__in HDPA hdpa);
BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Sort(__in HDPA hdpa,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam);
HRESULT IsolationAwarePrivatenCv IsolationAwareDPA_LoadStream(__out HDPA*phdpa,__in PFNDPASTREAM pfn,__in struct IStream*pstream,__in_opt void*pvInstData);
HRESULT IsolationAwarePrivatenCv IsolationAwareDPA_SaveStream(__in HDPA hdpa,__in PFNDPASTREAM pfn,__in struct IStream*pstream,__in_opt void*pvInstData);
BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Merge(__in HDPA hdpaDest,__in HDPA hdpaSrc,__in DWORD dwFlags,__in PFNDACOMPARE pfnCompare,__in PFNDPAMERGE pfnMerge,__in LPARAM lParam);
int IsolationAwarePrivatenCv IsolationAwareDPA_Search(__in HDPA hdpa,__in_opt void*pFind,__in int iStart,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam,__in UINT options);
BOOL IsolationAwarePrivatenCv IsolationAwareStr_SetPtrW(__deref_inout_opt LPWSTR*ppsz,LPCWSTR psz);
#if !defined(NOTRACKMOUSEEVENT)
BOOL IsolationAwarePrivatenCv IsolationAware_TrackMouseEvent(__inout LPTRACKMOUSEEVENT lpEventTrack);
#endif /* !defined(NOTRACKMOUSEEVENT) */
BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_EnableScrollBar(HWND unnamed1,int unnamed2,UINT unnamed3);
BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_ShowScrollBar(HWND unnamed1,int code,BOOL unnamed2);
BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollRange(HWND unnamed1,int code,LPINT unnamed2,LPINT unnamed3);
BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollInfo(HWND unnamed1,int code,LPSCROLLINFO unnamed2);
int IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollPos(HWND unnamed1,int code);
BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollProp(HWND unnamed1,int propIndex,LPINT unnamed2);
BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollPropPtr(HWND unnamed1,int propIndex,PINT_PTR unnamed2);
int IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollPos(HWND unnamed1,int code,int pos,BOOL fRedraw);
int IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollInfo(HWND unnamed1,int code,LPSCROLLINFO psi,BOOL fRedraw);
int IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollRange(HWND unnamed1,int code,int min,int max,BOOL fRedraw);
BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollProp(HWND unnamed1,UINT index,INT_PTR newValue,BOOL unnamed2);
BOOL IsolationAwarePrivatenCv IsolationAwareInitializeFlatSB(HWND unnamed1);
HRESULT IsolationAwarePrivatenCv IsolationAwareUninitializeFlatSB(HWND unnamed1);
#if (_WIN32_IE >= 0x560)
BOOL IsolationAwarePrivatenCv IsolationAwareSetWindowSubclass(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass,DWORD_PTR dwRefData);
BOOL IsolationAwarePrivatenCv IsolationAwareGetWindowSubclass(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass,DWORD_PTR*pdwRefData);
BOOL IsolationAwarePrivatenCv IsolationAwareRemoveWindowSubclass(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass);
#endif /* (_WIN32_IE >= 0x560) */
LRESULT IsolationAwarePrivatenCv IsolationAwareDefSubclassProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
HRESULT IsolationAwarePrivatenCv IsolationAwareLoadIconMetric(HINSTANCE hinst,PCWSTR pszName,int lims,__out HICON*phico);
HRESULT IsolationAwarePrivatenCv IsolationAwareLoadIconWithScaleDown(HINSTANCE hinst,PCWSTR pszName,int cx,int cy,__out HICON*phico);
int IsolationAwarePrivatenCv IsolationAwareDrawShadowText(HDC hdc,__in_ecount(cch) LPCWSTR pszText,UINT cch,RECT*prc,DWORD dwFlags,COLORREF crText,COLORREF crShadow,int ixOffset,int iyOffset);

ISOLATION_AWARE_INLINE HRESULT CommctrlIsolationAwarePrivateJVaQPGbueRfhYg(void)
{
    DWORD dwLastError = GetLastError();
    if (dwLastError == NO_ERROR)
        dwLastError = ERROR_INTERNAL_ERROR;
    return HRESULT_FROM_WIN32(dwLastError);
}

#if defined(UNICODE)

#define IsolationAwareCreateStatusWindow IsolationAwareCreateStatusWindowW
#define IsolationAwareDrawStatusText IsolationAwareDrawStatusTextW
#define IsolationAwareImageList_LoadImage IsolationAwareImageList_LoadImageW

#else /* UNICODE */

#define IsolationAwareCreateStatusWindow IsolationAwareCreateStatusWindowA
#define IsolationAwareDrawStatusText IsolationAwareDrawStatusTextA
#define IsolationAwareImageList_LoadImage IsolationAwareImageList_LoadImageA

#endif /* UNICODE */

#if !ISOLATION_AWARE_USE_STATIC_LIBRARY
ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareInitCommonControls(void)
{
    typedef void (WINAPI* PFN)(void);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("InitCommonControls");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn();
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareInitCommonControlsEx(const INITCOMMONCONTROLSEX*picce)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(const INITCOMMONCONTROLSEX*picce);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("InitCommonControlsEx");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(picce);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Create(int cx,int cy,UINT flags,int cInitial,int cGrow)
{
    HIMAGELIST imagelistResult = NULL;
    typedef HIMAGELIST (WINAPI* PFN)(int cx,int cy,UINT flags,int cInitial,int cGrow);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return imagelistResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Create");
            if (s_pfn == NULL)
                __leave;
        }
        imagelistResult = s_pfn(cx,cy,flags,cInitial,cGrow);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (imagelistResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return imagelistResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Destroy(HIMAGELIST himl)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Destroy");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareImageList_GetImageCount(HIMAGELIST himl)
{
    int nResult = 0 ;
    typedef int (WINAPI* PFN)(HIMAGELIST himl);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_GetImageCount");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(himl);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetImageCount(HIMAGELIST himl,UINT uNewCount)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,UINT uNewCount);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_SetImageCount");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,uNewCount);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareImageList_Add(HIMAGELIST himl,HBITMAP hbmImage,HBITMAP hbmMask)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(HIMAGELIST himl,HBITMAP hbmImage,HBITMAP hbmMask);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Add");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(himl,hbmImage,hbmMask);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareImageList_ReplaceIcon(HIMAGELIST himl,int i,HICON hicon)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(HIMAGELIST himl,int i,HICON hicon);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_ReplaceIcon");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(himl,i,hicon);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE COLORREF IsolationAwarePrivatenCv IsolationAwareImageList_SetBkColor(HIMAGELIST himl,COLORREF clrBk)
{
    COLORREF colorResult = RGB(0,0,0);
    typedef COLORREF (WINAPI* PFN)(HIMAGELIST himl,COLORREF clrBk);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return colorResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_SetBkColor");
            if (s_pfn == NULL)
                __leave;
        }
        colorResult = s_pfn(himl,clrBk);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (colorResult == RGB(0,0,0));
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return colorResult;
}

ISOLATION_AWARE_INLINE COLORREF IsolationAwarePrivatenCv IsolationAwareImageList_GetBkColor(HIMAGELIST himl)
{
    COLORREF colorResult = RGB(0,0,0);
    typedef COLORREF (WINAPI* PFN)(HIMAGELIST himl);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return colorResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_GetBkColor");
            if (s_pfn == NULL)
                __leave;
        }
        colorResult = s_pfn(himl);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (colorResult == RGB(0,0,0));
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return colorResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetOverlayImage(HIMAGELIST himl,int iImage,int iOverlay)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int iImage,int iOverlay);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_SetOverlayImage");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,iImage,iOverlay);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Draw(HIMAGELIST himl,int i,HDC hdcDst,int x,int y,UINT fStyle)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int i,HDC hdcDst,int x,int y,UINT fStyle);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Draw");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,i,hdcDst,x,y,fStyle);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Replace(HIMAGELIST himl,int i,HBITMAP hbmImage,HBITMAP hbmMask)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int i,HBITMAP hbmImage,HBITMAP hbmMask);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Replace");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,i,hbmImage,hbmMask);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareImageList_AddMasked(HIMAGELIST himl,HBITMAP hbmImage,COLORREF crMask)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(HIMAGELIST himl,HBITMAP hbmImage,COLORREF crMask);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_AddMasked");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(himl,hbmImage,crMask);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DrawEx(HIMAGELIST himl,int i,HDC hdcDst,int x,int y,int dx,int dy,COLORREF rgbBk,COLORREF rgbFg,UINT fStyle)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int i,HDC hdcDst,int x,int y,int dx,int dy,COLORREF rgbBk,COLORREF rgbFg,UINT fStyle);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_DrawEx");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,i,hdcDst,x,y,dx,dy,rgbBk,rgbFg,fStyle);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DrawIndirect(IMAGELISTDRAWPARAMS*pimldp)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(IMAGELISTDRAWPARAMS*pimldp);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_DrawIndirect");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(pimldp);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Remove(HIMAGELIST himl,int i)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int i);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Remove");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,i);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HICON IsolationAwarePrivatenCv IsolationAwareImageList_GetIcon(HIMAGELIST himl,int i,UINT flags)
{
    HICON iconResult = NULL;
    typedef HICON (WINAPI* PFN)(HIMAGELIST himl,int i,UINT flags);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return iconResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_GetIcon");
            if (s_pfn == NULL)
                __leave;
        }
        iconResult = s_pfn(himl,i,flags);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (iconResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return iconResult;
}

ISOLATION_AWARE_INLINE HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_LoadImageA(HINSTANCE hi,LPCSTR lpbmp,int cx,int cGrow,COLORREF crMask,UINT uType,UINT uFlags)
{
    HIMAGELIST imagelistResult = NULL;
    typedef HIMAGELIST (WINAPI* PFN)(HINSTANCE hi,LPCSTR lpbmp,int cx,int cGrow,COLORREF crMask,UINT uType,UINT uFlags);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return imagelistResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_LoadImageA");
            if (s_pfn == NULL)
                __leave;
        }
        imagelistResult = s_pfn(hi,lpbmp,cx,cGrow,crMask,uType,uFlags);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (imagelistResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return imagelistResult;
}

ISOLATION_AWARE_INLINE HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_LoadImageW(HINSTANCE hi,LPCWSTR lpbmp,int cx,int cGrow,COLORREF crMask,UINT uType,UINT uFlags)
{
    HIMAGELIST imagelistResult = NULL;
    typedef HIMAGELIST (WINAPI* PFN)(HINSTANCE hi,LPCWSTR lpbmp,int cx,int cGrow,COLORREF crMask,UINT uType,UINT uFlags);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return imagelistResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_LoadImageW");
            if (s_pfn == NULL)
                __leave;
        }
        imagelistResult = s_pfn(hi,lpbmp,cx,cGrow,crMask,uType,uFlags);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (imagelistResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return imagelistResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Copy(HIMAGELIST himlDst,int iDst,HIMAGELIST himlSrc,int iSrc,UINT uFlags)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himlDst,int iDst,HIMAGELIST himlSrc,int iSrc,UINT uFlags);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Copy");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himlDst,iDst,himlSrc,iSrc,uFlags);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_BeginDrag(HIMAGELIST himlTrack,int iTrack,int dxHotspot,int dyHotspot)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himlTrack,int iTrack,int dxHotspot,int dyHotspot);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_BeginDrag");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himlTrack,iTrack,dxHotspot,dyHotspot);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareImageList_EndDrag(void)
{
    typedef void (WINAPI* PFN)(void);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_EndDrag");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn();
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragEnter(HWND hwndLock,int x,int y)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND hwndLock,int x,int y);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_DragEnter");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hwndLock,x,y);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragLeave(HWND hwndLock)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND hwndLock);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_DragLeave");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hwndLock);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragMove(int x,int y)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(int x,int y);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_DragMove");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(x,y);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetDragCursorImage(HIMAGELIST himlDrag,int iDrag,int dxHotspot,int dyHotspot)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himlDrag,int iDrag,int dxHotspot,int dyHotspot);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_SetDragCursorImage");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himlDrag,iDrag,dxHotspot,dyHotspot);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_DragShowNolock(BOOL fShow)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(BOOL fShow);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_DragShowNolock");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(fShow);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_GetDragImage(POINT*ppt,POINT*pptHotspot)
{
    HIMAGELIST imagelistResult = NULL;
    typedef HIMAGELIST (WINAPI* PFN)(POINT*ppt,POINT*pptHotspot);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return imagelistResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_GetDragImage");
            if (s_pfn == NULL)
                __leave;
        }
        imagelistResult = s_pfn(ppt,pptHotspot);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (imagelistResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return imagelistResult;
}

ISOLATION_AWARE_INLINE HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Read(ISOLATIONAWARECOMMCTRL_LPSTREAM pstm)
{
    HIMAGELIST imagelistResult = NULL;
    typedef HIMAGELIST (WINAPI* PFN)(ISOLATIONAWARECOMMCTRL_LPSTREAM pstm);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return imagelistResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Read");
            if (s_pfn == NULL)
                __leave;
        }
        imagelistResult = s_pfn(pstm);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (imagelistResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return imagelistResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_Write(HIMAGELIST himl,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Write");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,pstm);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

#if (_WIN32_WINNT >= 0x0501)

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareImageList_ReadEx(DWORD dwFlags,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm,REFIID riid,PVOID*ppv)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(DWORD dwFlags,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm,REFIID riid,PVOID*ppv);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_ReadEx");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(dwFlags,pstm,riid,ppv);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareImageList_WriteEx(HIMAGELIST himl,DWORD dwFlags,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(HIMAGELIST himl,DWORD dwFlags,ISOLATIONAWARECOMMCTRL_LPSTREAM pstm);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_WriteEx");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(himl,dwFlags,pstm);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

#endif /* (_WIN32_WINNT >= 0x0501) */

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_GetIconSize(HIMAGELIST himl,int*cx,int*cy)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int*cx,int*cy);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_GetIconSize");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,cx,cy);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_SetIconSize(HIMAGELIST himl,int cx,int cy)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int cx,int cy);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_SetIconSize");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,cx,cy);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareImageList_GetImageInfo(HIMAGELIST himl,int i,IMAGEINFO*pImageInfo)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HIMAGELIST himl,int i,IMAGEINFO*pImageInfo);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_GetImageInfo");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(himl,i,pImageInfo);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Merge(HIMAGELIST himl1,int i1,HIMAGELIST himl2,int i2,int dx,int dy)
{
    HIMAGELIST imagelistResult = NULL;
    typedef HIMAGELIST (WINAPI* PFN)(HIMAGELIST himl1,int i1,HIMAGELIST himl2,int i2,int dx,int dy);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return imagelistResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Merge");
            if (s_pfn == NULL)
                __leave;
        }
        imagelistResult = s_pfn(himl1,i1,himl2,i2,dx,dy);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (imagelistResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return imagelistResult;
}

ISOLATION_AWARE_INLINE HIMAGELIST IsolationAwarePrivatenCv IsolationAwareImageList_Duplicate(HIMAGELIST himl)
{
    HIMAGELIST imagelistResult = NULL;
    typedef HIMAGELIST (WINAPI* PFN)(HIMAGELIST himl);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return imagelistResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ImageList_Duplicate");
            if (s_pfn == NULL)
                __leave;
        }
        imagelistResult = s_pfn(himl);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (imagelistResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return imagelistResult;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareHIMAGELIST_QueryInterface(HIMAGELIST himl,REFIID riid,void**ppv)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(HIMAGELIST himl,REFIID riid,void**ppv);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("HIMAGELIST_QueryInterface");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(himl,riid,ppv);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HWND IsolationAwarePrivatenCv IsolationAwareCreateToolbarEx(HWND hwnd,DWORD ws,UINT wID,int nBitmaps,HINSTANCE hBMInst,UINT_PTR wBMID,LPCTBBUTTON lpButtons,int iNumButtons,int dxButton,int dyButton,int dxBitmap,int dyBitmap,UINT uStructSize)
{
    HWND windowResult = NULL;
    typedef HWND (WINAPI* PFN)(HWND hwnd,DWORD ws,UINT wID,int nBitmaps,HINSTANCE hBMInst,UINT_PTR wBMID,LPCTBBUTTON lpButtons,int iNumButtons,int dxButton,int dyButton,int dxBitmap,int dyBitmap,UINT uStructSize);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return windowResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("CreateToolbarEx");
            if (s_pfn == NULL)
                __leave;
        }
        windowResult = s_pfn(hwnd,ws,wID,nBitmaps,hBMInst,wBMID,lpButtons,iNumButtons,dxButton,dyButton,dxBitmap,dyBitmap,uStructSize);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (windowResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return windowResult;
}

ISOLATION_AWARE_INLINE HBITMAP IsolationAwarePrivatenCv IsolationAwareCreateMappedBitmap(HINSTANCE hInstance,INT_PTR idBitmap,UINT wFlags,__in_opt LPCOLORMAP lpColorMap,int iNumMaps)
{
    HBITMAP bitmapResult = NULL;
    typedef HBITMAP (WINAPI* PFN)(HINSTANCE hInstance,INT_PTR idBitmap,UINT wFlags,__in_opt LPCOLORMAP lpColorMap,int iNumMaps);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return bitmapResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("CreateMappedBitmap");
            if (s_pfn == NULL)
                __leave;
        }
        bitmapResult = s_pfn(hInstance,idBitmap,wFlags,lpColorMap,iNumMaps);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (bitmapResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return bitmapResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareDrawStatusTextA(HDC hDC,LPCRECT lprc,LPCSTR pszText,UINT uFlags)
{
    typedef void (WINAPI* PFN)(HDC hDC,LPCRECT lprc,LPCSTR pszText,UINT uFlags);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DrawStatusTextA");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(hDC,lprc,pszText,uFlags);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareDrawStatusTextW(HDC hDC,LPCRECT lprc,LPCWSTR pszText,UINT uFlags)
{
    typedef void (WINAPI* PFN)(HDC hDC,LPCRECT lprc,LPCWSTR pszText,UINT uFlags);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DrawStatusTextW");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(hDC,lprc,pszText,uFlags);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE HWND IsolationAwarePrivatenCv IsolationAwareCreateStatusWindowA(long style,LPCSTR lpszText,HWND hwndParent,UINT wID)
{
    HWND windowResult = NULL;
    typedef HWND (WINAPI* PFN)(long style,LPCSTR lpszText,HWND hwndParent,UINT wID);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return windowResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("CreateStatusWindowA");
            if (s_pfn == NULL)
                __leave;
        }
        windowResult = s_pfn(style,lpszText,hwndParent,wID);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (windowResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return windowResult;
}

ISOLATION_AWARE_INLINE HWND IsolationAwarePrivatenCv IsolationAwareCreateStatusWindowW(long style,LPCWSTR lpszText,HWND hwndParent,UINT wID)
{
    HWND windowResult = NULL;
    typedef HWND (WINAPI* PFN)(long style,LPCWSTR lpszText,HWND hwndParent,UINT wID);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return windowResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("CreateStatusWindowW");
            if (s_pfn == NULL)
                __leave;
        }
        windowResult = s_pfn(style,lpszText,hwndParent,wID);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (windowResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return windowResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareMenuHelp(UINT uMsg,WPARAM wParam,LPARAM lParam,HMENU hMainMenu,HINSTANCE hInst,HWND hwndStatus,__in_ecount(4) UINT*lpwIDs)
{
    typedef void (WINAPI* PFN)(UINT uMsg,WPARAM wParam,LPARAM lParam,HMENU hMainMenu,HINSTANCE hInst,HWND hwndStatus,__in_ecount(4) UINT*lpwIDs);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("MenuHelp");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(uMsg,wParam,lParam,hMainMenu,hInst,hwndStatus,lpwIDs);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareShowHideMenuCtl(HWND hWnd,UINT_PTR uFlags,__in __nullterminated LPINT lpInfo)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND hWnd,UINT_PTR uFlags,__in __nullterminated LPINT lpInfo);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("ShowHideMenuCtl");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hWnd,uFlags,lpInfo);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareGetEffectiveClientRect(HWND hWnd,LPRECT lprc,__in __nullterminated const int*lpInfo)
{
    typedef void (WINAPI* PFN)(HWND hWnd,LPRECT lprc,__in __nullterminated const int*lpInfo);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("GetEffectiveClientRect");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(hWnd,lprc,lpInfo);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareMakeDragList(HWND hLB)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND hLB);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("MakeDragList");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hLB);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareDrawInsert(HWND handParent,HWND hLB,int nItem)
{
    typedef void (WINAPI* PFN)(HWND handParent,HWND hLB,int nItem);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DrawInsert");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(handParent,hLB,nItem);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareLBItemFromPt(HWND hLB,POINT pt,BOOL bAutoScroll)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(HWND hLB,POINT pt,BOOL bAutoScroll);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("LBItemFromPt");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(hLB,pt,bAutoScroll);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE HWND IsolationAwarePrivatenCv IsolationAwareCreateUpDownControl(DWORD dwStyle,int x,int y,int cx,int cy,HWND hParent,int nID,HINSTANCE hInst,HWND hBuddy,int nUpper,int nLower,int nPos)
{
    HWND windowResult = NULL;
    typedef HWND (WINAPI* PFN)(DWORD dwStyle,int x,int y,int cx,int cy,HWND hParent,int nID,HINSTANCE hInst,HWND hBuddy,int nUpper,int nLower,int nPos);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return windowResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("CreateUpDownControl");
            if (s_pfn == NULL)
                __leave;
        }
        windowResult = s_pfn(dwStyle,x,y,cx,cy,hParent,nID,hInst,hBuddy,nUpper,nLower,nPos);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (windowResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return windowResult;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareTaskDialogIndirect(const TASKDIALOGCONFIG*pTaskConfig,__out_opt int*pnButton,__out_opt int*pnRadioButton,__out_opt BOOL*pfVerificationFlagChecked)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(const TASKDIALOGCONFIG*pTaskConfig,__out_opt int*pnButton,__out_opt int*pnRadioButton,__out_opt BOOL*pfVerificationFlagChecked);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("TaskDialogIndirect");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(pTaskConfig,pnButton,pnRadioButton,pfVerificationFlagChecked);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareTaskDialog(__in_opt HWND hwndParent,__in_opt HINSTANCE hInstance,__in_opt PCWSTR pszWindowTitle,__in_opt PCWSTR pszMainInstruction,__in_opt PCWSTR pszContent,TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons,__in_opt PCWSTR pszIcon,__out_opt int*pnButton)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(__in_opt HWND hwndParent,__in_opt HINSTANCE hInstance,__in_opt PCWSTR pszWindowTitle,__in_opt PCWSTR pszMainInstruction,__in_opt PCWSTR pszContent,TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons,__in_opt PCWSTR pszIcon,__out_opt int*pnButton);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("TaskDialog");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(hwndParent,hInstance,pszWindowTitle,pszMainInstruction,pszContent,dwCommonButtons,pszIcon,pnButton);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareInitMUILanguage(LANGID uiLang)
{
    typedef void (WINAPI* PFN)(LANGID uiLang);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("InitMUILanguage");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(uiLang);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE LANGID IsolationAwarePrivatenCv IsolationAwareGetMUILanguage(void)
{
    LANGID languageIdResult = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
    typedef LANGID (WINAPI* PFN)(void);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return languageIdResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("GetMUILanguage");
            if (s_pfn == NULL)
                __leave;
        }
        languageIdResult = s_pfn();
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (languageIdResult == MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return languageIdResult;
}

ISOLATION_AWARE_INLINE HDSA IsolationAwarePrivatenCv IsolationAwareDSA_Create(int cbItem,int cItemGrow)
{
    HDSA result = NULL;
    typedef HDSA (WINAPI* PFN)(int cbItem,int cItemGrow);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_Create");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(cbItem,cItemGrow);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDSA_Destroy(__in HDSA hdsa)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDSA hdsa);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_Destroy");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdsa);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareDSA_DestroyCallback(__in HDSA hdsa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData)
{
    typedef void (WINAPI* PFN)(__in HDSA hdsa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_DestroyCallback");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(hdsa,pfnCB,pData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDSA_DeleteItem(__in HDSA hdsa,__in int i)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDSA hdsa,__in int i);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_DeleteItem");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdsa,i);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDSA_DeleteAllItems(__in HDSA hdsa)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDSA hdsa);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_DeleteAllItems");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdsa);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareDSA_EnumCallback(__in HDSA hdsa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData)
{
    typedef void (WINAPI* PFN)(__in HDSA hdsa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_EnumCallback");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(hdsa,pfnCB,pData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareDSA_InsertItem(__in HDSA hdsa,__in int i,__in const void*pitem)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(__in HDSA hdsa,__in int i,__in const void*pitem);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_InsertItem");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(hdsa,i,pitem);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE PVOID IsolationAwarePrivatenCv IsolationAwareDSA_GetItemPtr(__in HDSA hdsa,__in int i)
{
    PVOID vResult = NULL;
    typedef PVOID (WINAPI* PFN)(__in HDSA hdsa,__in int i);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return vResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_GetItemPtr");
            if (s_pfn == NULL)
                __leave;
        }
        vResult = s_pfn(hdsa,i);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (vResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return vResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDSA_GetItem(__in HDSA hdsa,__in int i,__out_xcount(pdsa->cbItem) void*pitem)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDSA hdsa,__in int i,__out_xcount(pdsa->cbItem) void*pitem);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_GetItem");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdsa,i,pitem);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDSA_SetItem(__in HDSA hdsa,__in int i,__in const void*pitem)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDSA hdsa,__in int i,__in const void*pitem);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_SetItem");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdsa,i,pitem);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HDSA IsolationAwarePrivatenCv IsolationAwareDSA_Clone(__in HDSA hdsa)
{
    HDSA result = NULL;
    typedef HDSA (WINAPI* PFN)(__in HDSA hdsa);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_Clone");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(hdsa);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE ULONGLONG IsolationAwarePrivatenCv IsolationAwareDSA_GetSize(__in HDSA hdsa)
{
    ULONGLONG result = 0 ;
    typedef ULONGLONG (WINAPI* PFN)(__in HDSA hdsa);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_GetSize");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(hdsa);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDSA_Sort(__in HDSA pdsa,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDSA pdsa,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DSA_Sort");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(pdsa,pfnCompare,lParam);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HDPA IsolationAwarePrivatenCv IsolationAwareDPA_Create(int cItemGrow)
{
    HDPA result = NULL;
    typedef HDPA (WINAPI* PFN)(int cItemGrow);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_Create");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(cItemGrow);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HDPA IsolationAwarePrivatenCv IsolationAwareDPA_CreateEx(__in int cpGrow,__in_opt HANDLE hheap)
{
    HDPA result = NULL;
    typedef HDPA (WINAPI* PFN)(__in int cpGrow,__in_opt HANDLE hheap);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_CreateEx");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(cpGrow,hheap);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HDPA IsolationAwarePrivatenCv IsolationAwareDPA_Clone(__in const HDPA hdpa,__in HDPA hdpaNew)
{
    HDPA result = NULL;
    typedef HDPA (WINAPI* PFN)(__in const HDPA hdpa,__in HDPA hdpaNew);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_Clone");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(hdpa,hdpaNew);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Destroy(__in HDPA hdpa)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDPA hdpa);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_Destroy");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdpa);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareDPA_DestroyCallback(__in HDPA hdpa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData)
{
    typedef void (WINAPI* PFN)(__in HDPA hdpa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_DestroyCallback");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(hdpa,pfnCB,pData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE PVOID IsolationAwarePrivatenCv IsolationAwareDPA_DeletePtr(__in HDPA hdpa,__in int i)
{
    PVOID vResult = NULL;
    typedef PVOID (WINAPI* PFN)(__in HDPA hdpa,__in int i);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return vResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_DeletePtr");
            if (s_pfn == NULL)
                __leave;
        }
        vResult = s_pfn(hdpa,i);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (vResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return vResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDPA_DeleteAllPtrs(__in HDPA hdpa)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDPA hdpa);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_DeleteAllPtrs");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdpa);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE void IsolationAwarePrivatenCv IsolationAwareDPA_EnumCallback(__in HDPA hdpa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData)
{
    typedef void (WINAPI* PFN)(__in HDPA hdpa,__in PFNDAENUMCALLBACK pfnCB,__in_opt void*pData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_EnumCallback");
            if (s_pfn == NULL)
                __leave;
        }
        s_pfn(hdpa,pfnCB,pData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Grow(__in HDPA pdpa,__in int cp)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDPA pdpa,__in int cp);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_Grow");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(pdpa,cp);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareDPA_InsertPtr(__in HDPA hdpa,__in int i,__in_opt void*p)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(__in HDPA hdpa,__in int i,__in_opt void*p);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_InsertPtr");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(hdpa,i,p);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDPA_SetPtr(__in HDPA hdpa,__in int i,__in_opt void*p)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDPA hdpa,__in int i,__in_opt void*p);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_SetPtr");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdpa,i,p);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE PVOID IsolationAwarePrivatenCv IsolationAwareDPA_GetPtr(__in HDPA hdpa,__in INT_PTR i)
{
    PVOID vResult = NULL;
    typedef PVOID (WINAPI* PFN)(__in HDPA hdpa,__in INT_PTR i);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return vResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_GetPtr");
            if (s_pfn == NULL)
                __leave;
        }
        vResult = s_pfn(hdpa,i);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (vResult == NULL);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return vResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareDPA_GetPtrIndex(__in HDPA hdpa,__in const void*p)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(__in HDPA hdpa,__in const void*p);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_GetPtrIndex");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(hdpa,p);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE ULONGLONG IsolationAwarePrivatenCv IsolationAwareDPA_GetSize(__in HDPA hdpa)
{
    ULONGLONG result = 0 ;
    typedef ULONGLONG (WINAPI* PFN)(__in HDPA hdpa);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_GetSize");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(hdpa);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Sort(__in HDPA hdpa,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDPA hdpa,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_Sort");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdpa,pfnCompare,lParam);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareDPA_LoadStream(__out HDPA*phdpa,__in PFNDPASTREAM pfn,__in struct IStream*pstream,__in_opt void*pvInstData)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(__out HDPA*phdpa,__in PFNDPASTREAM pfn,__in struct IStream*pstream,__in_opt void*pvInstData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_LoadStream");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(phdpa,pfn,pstream,pvInstData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareDPA_SaveStream(__in HDPA hdpa,__in PFNDPASTREAM pfn,__in struct IStream*pstream,__in_opt void*pvInstData)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(__in HDPA hdpa,__in PFNDPASTREAM pfn,__in struct IStream*pstream,__in_opt void*pvInstData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_SaveStream");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(hdpa,pfn,pstream,pvInstData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareDPA_Merge(__in HDPA hdpaDest,__in HDPA hdpaSrc,__in DWORD dwFlags,__in PFNDACOMPARE pfnCompare,__in PFNDPAMERGE pfnMerge,__in LPARAM lParam)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__in HDPA hdpaDest,__in HDPA hdpaSrc,__in DWORD dwFlags,__in PFNDACOMPARE pfnCompare,__in PFNDPAMERGE pfnMerge,__in LPARAM lParam);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_Merge");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hdpaDest,hdpaSrc,dwFlags,pfnCompare,pfnMerge,lParam);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareDPA_Search(__in HDPA hdpa,__in_opt void*pFind,__in int iStart,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam,__in UINT options)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(__in HDPA hdpa,__in_opt void*pFind,__in int iStart,__in PFNDACOMPARE pfnCompare,__in LPARAM lParam,__in UINT options);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DPA_Search");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(hdpa,pFind,iStart,pfnCompare,lParam,options);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareStr_SetPtrW(__deref_inout_opt LPWSTR*ppsz,LPCWSTR psz)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__deref_inout_opt LPWSTR*ppsz,LPCWSTR psz);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("Str_SetPtrW");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(ppsz,psz);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

#if !defined(NOTRACKMOUSEEVENT)

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAware_TrackMouseEvent(__inout LPTRACKMOUSEEVENT lpEventTrack)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(__inout LPTRACKMOUSEEVENT lpEventTrack);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("_TrackMouseEvent");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(lpEventTrack);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

#endif /* !defined(NOTRACKMOUSEEVENT) */

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_EnableScrollBar(HWND unnamed1,int unnamed2,UINT unnamed3)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1,int unnamed2,UINT unnamed3);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_EnableScrollBar");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1,unnamed2,unnamed3);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_ShowScrollBar(HWND unnamed1,int code,BOOL unnamed2)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1,int code,BOOL unnamed2);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_ShowScrollBar");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1,code,unnamed2);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollRange(HWND unnamed1,int code,LPINT unnamed2,LPINT unnamed3)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1,int code,LPINT unnamed2,LPINT unnamed3);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_GetScrollRange");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1,code,unnamed2,unnamed3);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollInfo(HWND unnamed1,int code,LPSCROLLINFO unnamed2)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1,int code,LPSCROLLINFO unnamed2);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_GetScrollInfo");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1,code,unnamed2);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollPos(HWND unnamed1,int code)
{
    int nResult = 0 ;
    typedef int (WINAPI* PFN)(HWND unnamed1,int code);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_GetScrollPos");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(unnamed1,code);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollProp(HWND unnamed1,int propIndex,LPINT unnamed2)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1,int propIndex,LPINT unnamed2);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_GetScrollProp");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1,propIndex,unnamed2);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_GetScrollPropPtr(HWND unnamed1,int propIndex,PINT_PTR unnamed2)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1,int propIndex,PINT_PTR unnamed2);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY(
#ifdef _WIN64
            "FlatSB_GetScrollPropPtr"
#else
            "FlatSB_GetScrollProp"
#endif
            );
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1,propIndex,unnamed2);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollPos(HWND unnamed1,int code,int pos,BOOL fRedraw)
{
    int nResult = 0 ;
    typedef int (WINAPI* PFN)(HWND unnamed1,int code,int pos,BOOL fRedraw);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_SetScrollPos");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(unnamed1,code,pos,fRedraw);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollInfo(HWND unnamed1,int code,LPSCROLLINFO psi,BOOL fRedraw)
{
    int nResult = 0 ;
    typedef int (WINAPI* PFN)(HWND unnamed1,int code,LPSCROLLINFO psi,BOOL fRedraw);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_SetScrollInfo");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(unnamed1,code,psi,fRedraw);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollRange(HWND unnamed1,int code,int min,int max,BOOL fRedraw)
{
    int nResult = 0 ;
    typedef int (WINAPI* PFN)(HWND unnamed1,int code,int min,int max,BOOL fRedraw);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_SetScrollRange");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(unnamed1,code,min,max,fRedraw);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareFlatSB_SetScrollProp(HWND unnamed1,UINT index,INT_PTR newValue,BOOL unnamed2)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1,UINT index,INT_PTR newValue,BOOL unnamed2);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("FlatSB_SetScrollProp");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1,index,newValue,unnamed2);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareInitializeFlatSB(HWND unnamed1)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND unnamed1);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("InitializeFlatSB");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(unnamed1);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareUninitializeFlatSB(HWND unnamed1)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(HWND unnamed1);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("UninitializeFlatSB");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(unnamed1);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

#if (_WIN32_IE >= 0x560)

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareSetWindowSubclass(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass,DWORD_PTR dwRefData)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass,DWORD_PTR dwRefData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("SetWindowSubclass");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hWnd,pfnSubclass,uIdSubclass,dwRefData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareGetWindowSubclass(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass,DWORD_PTR*pdwRefData)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass,DWORD_PTR*pdwRefData);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("GetWindowSubclass");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hWnd,pfnSubclass,uIdSubclass,pdwRefData);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

ISOLATION_AWARE_INLINE BOOL IsolationAwarePrivatenCv IsolationAwareRemoveWindowSubclass(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass)
{
    BOOL fResult = FALSE;
    typedef BOOL (WINAPI* PFN)(HWND hWnd,SUBCLASSPROC pfnSubclass,UINT_PTR uIdSubclass);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return fResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("RemoveWindowSubclass");
            if (s_pfn == NULL)
                __leave;
        }
        fResult = s_pfn(hWnd,pfnSubclass,uIdSubclass);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (fResult == FALSE);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return fResult;
}

#endif /* (_WIN32_IE >= 0x560) */

ISOLATION_AWARE_INLINE LRESULT IsolationAwarePrivatenCv IsolationAwareDefSubclassProc(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
    LRESULT result = 0 ;
    typedef LRESULT (WINAPI* PFN)(HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return result;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DefSubclassProc");
            if (s_pfn == NULL)
                __leave;
        }
        result = s_pfn(hWnd,uMsg,wParam,lParam);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (result == 0 );
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareLoadIconMetric(HINSTANCE hinst,PCWSTR pszName,int lims,__out HICON*phico)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(HINSTANCE hinst,PCWSTR pszName,int lims,__out HICON*phico);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("LoadIconMetric");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(hinst,pszName,lims,phico);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE HRESULT IsolationAwarePrivatenCv IsolationAwareLoadIconWithScaleDown(HINSTANCE hinst,PCWSTR pszName,int cx,int cy,__out HICON*phico)
{
    HRESULT result = S_OK;
    typedef HRESULT (WINAPI* PFN)(HINSTANCE hinst,PCWSTR pszName,int cx,int cy,__out HICON*phico);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("LoadIconWithScaleDown");
            if (s_pfn == NULL)
            {
                result = CommctrlIsolationAwarePrivateJVaQPGbueRfhYg();
                __leave;
            }
        }
        result = s_pfn(hinst,pszName,cx,cy,phico);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
        }
    }
    return result;
}

ISOLATION_AWARE_INLINE int IsolationAwarePrivatenCv IsolationAwareDrawShadowText(HDC hdc,__in_ecount(cch) LPCWSTR pszText,UINT cch,RECT*prc,DWORD dwFlags,COLORREF crText,COLORREF crShadow,int ixOffset,int iyOffset)
{
    int nResult = -1;
    typedef int (WINAPI* PFN)(HDC hdc,__in_ecount(cch) LPCWSTR pszText,UINT cch,RECT*prc,DWORD dwFlags,COLORREF crText,COLORREF crShadow,int ixOffset,int iyOffset);
    static PFN s_pfn;
    ULONG_PTR  ulpCookie = 0;
    const BOOL fActivateActCtxSuccess =
#ifdef _M_IX86
        IsolationAwarePrivateT_SqbjaYRiRY ||
#endif
        IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
    if (!fActivateActCtxSuccess)
        return nResult;
    __try
    {
        if (s_pfn == NULL)
        {
            s_pfn = (PFN)CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY("DrawShadowText");
            if (s_pfn == NULL)
                __leave;
        }
        nResult = s_pfn(hdc,pszText,cch,prc,dwFlags,crText,crShadow,ixOffset,iyOffset);
    }
    __finally
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            const BOOL fPreserveLastError = (nResult == -1);
            const DWORD dwLastError = fPreserveLastError ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (fPreserveLastError)
                SetLastError(dwLastError);
        }
    }
    return nResult;
}

ISOLATION_AWARE_INLINE FARPROC IsolationAwarePrivatenCv CommctrlIsolationAwarePrivatetRgCebPnQQeRff_pbZPgYQP_QYY(LPCSTR pszProcName)
/* This function is shared by the other stubs in this header. */
{
    FARPROC proc = NULL;
    static HMODULE s_module;
    BOOL fActivateActCtxSuccess = FALSE;
    ULONG_PTR ulpCookie = 0;
#ifndef _M_CEE_PURE
    const static IsolationAwarePrivatepBAFGnAG_zBqHyr_vAsB
        c = { IsolationAwarePrivatezlybNQyVOeNelJ, L"Comctl32.dll"
#ifdef _M_IX86
             , IsolationAwarePrivatezlybNQyVOeNeln, "Comctl32.dll"
#endif
    };
#else
    static IsolationAwarePrivatepBAFGnAG_zBqHyr_vAsB c;
    c.WinbaseIsolationAwarePrivateybNQJ = IsolationAwarePrivatezlybNQyVOeNelJ;
    c.WinbaseIsolationAwarePrivateANZRJ = L"Comctl32.dll";
#ifdef _M_IX86
    c.WinbaseIsolationAwarePrivateybNQn = IsolationAwarePrivatezlybNQyVOeNeln;
    c.WinbaseIsolationAwarePrivateANZRn = "Comctl32.dll";
#endif
#endif
    static IsolationAwarePrivatezHGnoyr_zBqHyr_vAsB m;

    __try
    {
#ifdef _M_IX86
        if (!IsolationAwarePrivateT_SqbjaYRiRY)
#endif
        {
            fActivateActCtxSuccess = IsolationAwarePrivatenPgViNgRzlnPgpgk(&ulpCookie);
            if (!fActivateActCtxSuccess)
                __leave;
        }
        proc = IsolationAwarePrivatezltRgCebPnQQeRff(&c, &m, pszProcName);
    }
    __finally
    {
        if (
#ifdef _M_IX86
            !IsolationAwarePrivateT_SqbjaYRiRY &&
#endif
            fActivateActCtxSuccess)
        {
            const DWORD dwLastError = (proc == NULL) ? GetLastError() : NO_ERROR;
            (void)IsolationAwareDeactivateActCtx(0, ulpCookie);
            if (proc == NULL)
                SetLastError(dwLastError);
        }
    }
    return proc;
}

#endif /* ISOLATION_AWARE_USE_STATIC_LIBRARY */

#define CreateMappedBitmap IsolationAwareCreateMappedBitmap
#define CreateStatusWindowA IsolationAwareCreateStatusWindowA
#define CreateStatusWindowW IsolationAwareCreateStatusWindowW
#define CreateToolbarEx IsolationAwareCreateToolbarEx
#define CreateUpDownControl IsolationAwareCreateUpDownControl
#define DPA_Clone IsolationAwareDPA_Clone
#define DPA_Create IsolationAwareDPA_Create
#define DPA_CreateEx IsolationAwareDPA_CreateEx
#define DPA_DeleteAllPtrs IsolationAwareDPA_DeleteAllPtrs
#define DPA_DeletePtr IsolationAwareDPA_DeletePtr
#define DPA_Destroy IsolationAwareDPA_Destroy
#define DPA_DestroyCallback IsolationAwareDPA_DestroyCallback
#define DPA_EnumCallback IsolationAwareDPA_EnumCallback
#define DPA_GetPtr IsolationAwareDPA_GetPtr
#define DPA_GetPtrIndex IsolationAwareDPA_GetPtrIndex
#define DPA_GetSize IsolationAwareDPA_GetSize
#define DPA_Grow IsolationAwareDPA_Grow
#define DPA_InsertPtr IsolationAwareDPA_InsertPtr
#define DPA_LoadStream IsolationAwareDPA_LoadStream
#define DPA_Merge IsolationAwareDPA_Merge
#define DPA_SaveStream IsolationAwareDPA_SaveStream
#define DPA_Search IsolationAwareDPA_Search
#define DPA_SetPtr IsolationAwareDPA_SetPtr
#define DPA_Sort IsolationAwareDPA_Sort
#define DSA_Clone IsolationAwareDSA_Clone
#define DSA_Create IsolationAwareDSA_Create
#define DSA_DeleteAllItems IsolationAwareDSA_DeleteAllItems
#define DSA_DeleteItem IsolationAwareDSA_DeleteItem
#define DSA_Destroy IsolationAwareDSA_Destroy
#define DSA_DestroyCallback IsolationAwareDSA_DestroyCallback
#define DSA_EnumCallback IsolationAwareDSA_EnumCallback
#define DSA_GetItem IsolationAwareDSA_GetItem
#define DSA_GetItemPtr IsolationAwareDSA_GetItemPtr
#define DSA_GetSize IsolationAwareDSA_GetSize
#define DSA_InsertItem IsolationAwareDSA_InsertItem
#define DSA_SetItem IsolationAwareDSA_SetItem
#define DSA_Sort IsolationAwareDSA_Sort
#define DefSubclassProc IsolationAwareDefSubclassProc
#define DrawInsert IsolationAwareDrawInsert
#define DrawShadowText IsolationAwareDrawShadowText
#define DrawStatusTextA IsolationAwareDrawStatusTextA
#define DrawStatusTextW IsolationAwareDrawStatusTextW
#define FlatSB_EnableScrollBar IsolationAwareFlatSB_EnableScrollBar
#define FlatSB_GetScrollInfo IsolationAwareFlatSB_GetScrollInfo
#define FlatSB_GetScrollPos IsolationAwareFlatSB_GetScrollPos
#define FlatSB_GetScrollProp IsolationAwareFlatSB_GetScrollProp
#if defined(FlatSB_GetScrollPropPtr)
#undef FlatSB_GetScrollPropPtr
#endif
#define FlatSB_GetScrollPropPtr IsolationAwareFlatSB_GetScrollPropPtr
#define FlatSB_GetScrollRange IsolationAwareFlatSB_GetScrollRange
#define FlatSB_SetScrollInfo IsolationAwareFlatSB_SetScrollInfo
#define FlatSB_SetScrollPos IsolationAwareFlatSB_SetScrollPos
#define FlatSB_SetScrollProp IsolationAwareFlatSB_SetScrollProp
#define FlatSB_SetScrollRange IsolationAwareFlatSB_SetScrollRange
#define FlatSB_ShowScrollBar IsolationAwareFlatSB_ShowScrollBar
#define GetEffectiveClientRect IsolationAwareGetEffectiveClientRect
#define GetMUILanguage IsolationAwareGetMUILanguage
#define GetWindowSubclass IsolationAwareGetWindowSubclass
#define HIMAGELIST_QueryInterface IsolationAwareHIMAGELIST_QueryInterface
#define ImageList_Add IsolationAwareImageList_Add
#define ImageList_AddMasked IsolationAwareImageList_AddMasked
#define ImageList_BeginDrag IsolationAwareImageList_BeginDrag
#define ImageList_Copy IsolationAwareImageList_Copy
#define ImageList_Create IsolationAwareImageList_Create
#define ImageList_Destroy IsolationAwareImageList_Destroy
#define ImageList_DragEnter IsolationAwareImageList_DragEnter
#define ImageList_DragLeave IsolationAwareImageList_DragLeave
#define ImageList_DragMove IsolationAwareImageList_DragMove
#define ImageList_DragShowNolock IsolationAwareImageList_DragShowNolock
#define ImageList_Draw IsolationAwareImageList_Draw
#define ImageList_DrawEx IsolationAwareImageList_DrawEx
#define ImageList_DrawIndirect IsolationAwareImageList_DrawIndirect
#define ImageList_Duplicate IsolationAwareImageList_Duplicate
#define ImageList_EndDrag IsolationAwareImageList_EndDrag
#define ImageList_GetBkColor IsolationAwareImageList_GetBkColor
#define ImageList_GetDragImage IsolationAwareImageList_GetDragImage
#define ImageList_GetIcon IsolationAwareImageList_GetIcon
#define ImageList_GetIconSize IsolationAwareImageList_GetIconSize
#define ImageList_GetImageCount IsolationAwareImageList_GetImageCount
#define ImageList_GetImageInfo IsolationAwareImageList_GetImageInfo
#define ImageList_LoadImageA IsolationAwareImageList_LoadImageA
#define ImageList_LoadImageW IsolationAwareImageList_LoadImageW
#define ImageList_Merge IsolationAwareImageList_Merge
#define ImageList_Read IsolationAwareImageList_Read
#define ImageList_ReadEx IsolationAwareImageList_ReadEx
#define ImageList_Remove IsolationAwareImageList_Remove
#define ImageList_Replace IsolationAwareImageList_Replace
#define ImageList_ReplaceIcon IsolationAwareImageList_ReplaceIcon
#define ImageList_SetBkColor IsolationAwareImageList_SetBkColor
#define ImageList_SetDragCursorImage IsolationAwareImageList_SetDragCursorImage
#define ImageList_SetIconSize IsolationAwareImageList_SetIconSize
#define ImageList_SetImageCount IsolationAwareImageList_SetImageCount
#define ImageList_SetOverlayImage IsolationAwareImageList_SetOverlayImage
#define ImageList_Write IsolationAwareImageList_Write
#define ImageList_WriteEx IsolationAwareImageList_WriteEx
#define InitCommonControls IsolationAwareInitCommonControls
#define InitCommonControlsEx IsolationAwareInitCommonControlsEx
#define InitMUILanguage IsolationAwareInitMUILanguage
#define InitializeFlatSB IsolationAwareInitializeFlatSB
#define LBItemFromPt IsolationAwareLBItemFromPt
#define LoadIconMetric IsolationAwareLoadIconMetric
#define LoadIconWithScaleDown IsolationAwareLoadIconWithScaleDown
#define MakeDragList IsolationAwareMakeDragList
#define MenuHelp IsolationAwareMenuHelp
#define RemoveWindowSubclass IsolationAwareRemoveWindowSubclass
#define SetWindowSubclass IsolationAwareSetWindowSubclass
#define ShowHideMenuCtl IsolationAwareShowHideMenuCtl
#define Str_SetPtrW IsolationAwareStr_SetPtrW
#define TaskDialog IsolationAwareTaskDialog
#define TaskDialogIndirect IsolationAwareTaskDialogIndirect
#define UninitializeFlatSB IsolationAwareUninitializeFlatSB
#define _TrackMouseEvent IsolationAware_TrackMouseEvent

#endif /* ISOLATION_AWARE_ENABLED */
#endif /* RC */


#if defined(__cplusplus)
} /* __cplusplus */
#endif

