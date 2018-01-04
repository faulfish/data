#ifndef _SHOW_MESSAGE_H_
#define _SHOW_MESSAGE_H_
#pragma  once
#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

#include "../CommUIExportAPI.h"
#include "../Porting/QMessageBox.h"

namespace QSYNC_LIB
{
    static HHOOK g_hHook;
    static WNDPROC ShowMessageOldProc;
    static HWND g_hParentHwnd;

    __forceinline static void MoveDialogToParentCenter(HWND hParent, HWND hChild) {
        RECT rectParent = {0};
        RECT rectChild = {0};
        RECT rectDesktop = {0};
        HWND hQSyncher = ::GetParent(hParent);
        if(hQSyncher == NULL) return;
        HWND hDesktop = ::GetDesktopWindow();
        if(hDesktop == NULL) return;
        ::GetWindowRect(hChild,  &rectChild);
        ::GetWindowRect(hQSyncher, &rectParent);
        ::GetWindowRect(hDesktop, &rectDesktop);
        int nParentCenterX = (rectParent.right + rectParent.left) / 2;
        int nParentCenterY = (rectParent.bottom + rectParent.top) / 2;
        int nX = nParentCenterX - (rectChild.right - rectChild.left) / 2;
        int nY = nParentCenterY - (rectChild.bottom - rectChild.top) / 2;
        if(nX < 0) nX = 0;
        if(nY < 0) nY = 0;
        if(nX + (rectChild.right - rectChild.left) > rectDesktop.right)
            nX = rectDesktop.right - (rectChild.right - rectChild.left);
        if((nY + (rectChild.bottom - rectChild.top)) > rectDesktop.bottom)
            nY = rectDesktop.bottom - (rectChild.bottom - rectChild.top);
        ::MoveWindow(hChild, nX, nY, rectChild.right - rectChild.left,
            rectChild.bottom - rectChild.top, TRUE);
    }

	static LRESULT CALLBACK HookWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        //Let OS to perform basic operation
        LRESULT rc = CallWindowProc(ShowMessageOldProc, hWnd, uMsg, wParam, lParam);

        //If MessageBox wants to load, do something
        if(uMsg == WM_INITDIALOG) {
            MoveDialogToParentCenter(g_hParentHwnd, hWnd);
        }
        if(uMsg == WM_NCDESTROY) {
            //On exit Uninstall the hook procedure
            BOOL retVal = UnhookWindowsHookEx(g_hHook);
            ASSERT(retVal);
        }
        return rc;
    }
    static LRESULT CALLBACK HookProcess(int nCode, WPARAM wParam, LPARAM lParam) {
        if (nCode==HC_ACTION) {
            CWPSTRUCT* pwp = (CWPSTRUCT*)lParam;
            if (pwp->message == WM_INITDIALOG)
                ShowMessageOldProc = (WNDPROC)SetWindowLong(pwp->hwnd, GWL_WNDPROC, (LONG)HookWndProc); 
        }
        return CallNextHookEx(g_hHook, nCode, wParam, lParam);
    }


    //enum MESSAGERETURN	
    //{
    //    QIDOK		=	1,		//   The OK button was selected.
    //    QIDCANCEL	=	2,		//   The Cancel button was selected.
    //    QIDABORT	=	3,		//   The Abort button was selected.
    //    QIDRETRY	=	4,		//   The Retry button was selected.
    //    QIDIGNORE	=	5,		//   The Ignore button was selected.
    //    QIDYES		=	6,		//   The Yes button was selected.
    //    QIDNO		=	7		//   The No button was selected.
    //};

    //enum MESSAGETYPE
    //{		
    //    QMB_OK					=	0x00000000L,//   The message box contains one pushbutton: OK.
    //    QMB_OKCANCEL,							//   The message box contains two pushbuttons: OK and Cancel.
    //    QMB_ABORTRETRYIGNORE,					//   The message box contains three pushbuttons: Abort, Retry, and Ignore.		
    //    QMB_YESNOCANCEL,						//   The message box contains three pushbuttons: Yes, No, and Cancel.
    //    QMB_YESNO,								//   The message box contains two pushbuttons: Yes and No.
    //    QMB_RETRYCANCEL							//   The message box contains two pushbuttons: Retry and Cancel.				
    //};


    static long GetIconTypeByMessageType(MESSAGETYPE nID, long nNewID) {
        switch(nID) {
        case QMB_OK:
        case QMB_OKCANCEL:
            nNewID = nID | MB_ICONASTERISK;
            break;
        case QMB_ABORTRETRYIGNORE:
        case QMB_RETRYCANCEL:
            nNewID = nID | MB_ICONEXCLAMATION;
            break;
        case QMB_YESNOCANCEL:
        case QMB_YESNO:
            nNewID = nID | MB_ICONQUESTION;
            break;
        }
        return nNewID;
    }
    COMMONUI_API MESSAGERETURN ShowMessage(HWND hWnd, const TSTRING& strMessage, const TSTRING& strTitle, MESSAGETYPE nID = QMB_OK);
    COMMONUI_API MESSAGERETURN ShowMessage(HWND hWnd, LPCTSTR strMessage, LPCTSTR strTitle, MESSAGETYPE nID = QMB_OK);
}

#pragma warning(pop)

#endif //_SHOW_MESSAGE_H_