#include "stdafx.h"
#include "GPRSMdmSetupPropertyPage5.h"
#include "GPRSMdmSetupPropertyPages.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CGPRSMdmSetupPropertyPage5, CPropertyPage)

BEGIN_MESSAGE_MAP(CGPRSMdmSetupPropertyPage5, CPropertyPage)
    //{{AFX_MSG_MAP(CGPRSMdmSetupPropertyPage5)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage5 property page
/////////////////////////////////////////////////////////////////////////////

CGPRSMdmSetupPropertyPage5::CGPRSMdmSetupPropertyPage5() : CPropertyPage(CGPRSMdmSetupPropertyPage5::IDD)
{
    //{{AFX_DATA_INIT(CGPRSMdmSetupPropertyPage5)
    //}}AFX_DATA_INIT

    //--------------------------- Steven generated codes --------------------------
    //(Disable the "Help" button.)
    m_psp.dwFlags &= ~PSP_HASHELP;

    //(*Set up the PropertyPage Font according to the Locale of the current Windows dynamically.)
    switch( PRIMARYLANGID(LANGIDFROMLCID(::GetUserDefaultLCID())) )
    {
    case LANG_ENGLISH:
        {
            m_szPageFontName	= _T("Verdana");
            m_nPageFontSize	= 10;

            break;
        }
    case LANG_CHINESE:
        {
            if(SUBLANG_CHINESE_TRADITIONAL == SUBLANGID(LANGIDFROMLCID(::GetUserDefaultLCID())))
            {
                m_szPageFontName	= _T("·s²Ó©úÅé");
                m_nPageFontSize	= 11;
            }
            else if(SUBLANG_CHINESE_SIMPLIFIED == SUBLANGID(LANGIDFROMLCID(::GetUserDefaultLCID())))
            {
                m_szPageFontName	= _T("SimSun");  //(§ºÊ^)
                m_nPageFontSize	= 10;
            }
            else
            {
                m_szPageFontName	= _T("MS Shell Dlg");
                m_nPageFontSize	= 9;
            }

            break;
        }
    case LANG_JAPANESE:
        {
            m_szPageFontName	= _T("MS UI Gothic");
            m_nPageFontSize	= 10;

            break;
        }
    case LANG_KOREAN:
        {
            m_szPageFontName	= _T("Gulim");
            m_nPageFontSize	= 10;

            break;
        }
    default:
        {
            m_szPageFontName	= _T("MS Shell Dlg");
            m_nPageFontSize	= 9;

            break;
        }
    }

    //(*You have the only chance to set up the Property Page Caption in his Create method.)
    m_szPageCaptionText = LOADSTRING(IDS_GPRSMDM_PROPERTY_PAGE5_CAPTION);
    m_psp.pszTitle = m_szPageCaptionText;
    m_psp.dwFlags |= PSP_USETITLE;

    //(Set up the PropertyPage Look and Feel.)
    m_EditTextColor = RGB(95,95,95);
    m_EditBkBrush.CreateSolidBrush( RGB(219,215,190) );
    m_PageBkBrush.CreateSolidBrush( RGB(245,245,245) );
    //--------------------------- Steven generated codes --------------------------
}


CGPRSMdmSetupPropertyPage5::~CGPRSMdmSetupPropertyPage5()
{
    ;
}


void CGPRSMdmSetupPropertyPage5::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGPRSMdmSetupPropertyPage5)
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE_PIC5, m_PagePicStatic);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE5_TEXT_LINE1, m_PageTextLine1Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE5_TEXT_LINE2, m_PageTextLine2Static);
    DDX_Control(pDX, IDC_RADIO_PREPERTY_PAGE5_REBOOT_NOW, m_RebootNowRadio);
    DDX_Control(pDX, IDC_RADIO_PREPERTY_PAGE5_REBOOT_LATER, m_RebootLaterRadio);
    //}}AFX_DATA_MAP
}

BOOL CGPRSMdmSetupPropertyPage5::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_MOUSEMOVE:
        {
            m_Tooltip.RelayEvent(pMsg);
            break;
        }
    default:
        {
            //Do nothing.
            break;
        }
    }

    return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CGPRSMdmSetupPropertyPage5::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    //--------------------------- Steven generated codes --------------------------
    //To-do: Set up the associated Property Page Picture.
    if( m_PagePicBitmap.LoadBitmap(IDB_BITMAP_GPRSMDMPIC5) )
    {
        m_PagePicStatic.SetBitmap( (HBITMAP)m_PagePicBitmap );
    }else;

    //To-do: Set up my CSyncStatic controls...............
    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE5_TEXT_LINE1, LOADSTRING(IDS_STATIC_PREPERTY_PAGE5_TEXT_LINE1));
    m_PageTextLine1Static.set_FontName		( m_szPageFontName );
    m_PageTextLine1Static.set_FontSize		( m_nPageFontSize );
    m_PageTextLine1Static.set_FontWeight		( FW_BOLD );
    m_PageTextLine1Static.set_Italic			( FALSE );
    m_PageTextLine1Static.set_Underline		( FALSE );
    m_PageTextLine1Static.set_TextFaceColor	( RGB(85,0,0) );
    m_PageTextLine1Static.set_3DText			( TRUE, RGB(150,150,150) );
    m_PageTextLine1Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE5_TEXT_LINE2, LOADSTRING(IDS_STATIC_PREPERTY_PAGE5_TEXT_LINE2));
    m_PageTextLine2Static.set_FontName		( m_szPageFontName );
    m_PageTextLine2Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine2Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine2Static.set_Italic			( FALSE );
    m_PageTextLine2Static.set_Underline		( FALSE );
    m_PageTextLine2Static.set_TextFaceColor	( RGB(60,60,60) );
    m_PageTextLine2Static.set_3DText			( FALSE );
    m_PageTextLine2Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)
    //......................................................

    //To-do: Add the ToolTips...............................
    m_Tooltip.Create(this, 0x00000040);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)MAKELONG(4500,0));  //(4500ms.)
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPTEXTCOLOR, (WPARAM)RGB(100,100,100), 0);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPBKCOLOR, (WPARAM)RGB(231,231,231), 0);
    m_Tooltip.Activate(TRUE);
    //......................................................

    //(Set up the Radio Buttons.)
    ::SetDlgItemText(m_hWnd, IDC_RADIO_PREPERTY_PAGE5_REBOOT_NOW, LOADSTRING(IDS_RADIO_PREPERTY_PAGE5_REBOOT_NOW));
    ::SetDlgItemText(m_hWnd, IDC_RADIO_PREPERTY_PAGE5_REBOOT_LATER, LOADSTRING(IDS_RADIO_PREPERTY_PAGE5_REBOOT_LATER));
    m_RebootNowRadio.SetCheck(1);  //(default to choose the "Re-boot Now".)
    //--------------------------- Steven generated codes --------------------------


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGPRSMdmSetupPropertyPage5::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //--------------------------- Steven generated codes --------------------------
    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);


    switch( nCtlColor )
    {
    case CTLCOLOR_DLG:
        {
            pDC->SetBkMode(TRANSPARENT);
            hbr = (HBRUSH) m_PageBkBrush;

            break;
        }
    case CTLCOLOR_EDIT:
        {
            if( ::IsWindowEnabled(pWnd->m_hWnd) )
            {
                pDC->SetTextColor( m_EditTextColor );
                pDC->SetBkColor( RGB(219,215,190) );
                hbr = (HBRUSH) m_EditBkBrush;
            }
            else
            {
                //(use the Windows default settings.)
                ;
            }

            break;
        }
    case CTLCOLOR_LISTBOX:
        {
            if( ::IsWindowEnabled(pWnd->m_hWnd) )
            {
                pDC->SetTextColor( m_EditTextColor );
                pDC->SetBkColor( RGB(219,215,190) );
                hbr = (HBRUSH) m_EditBkBrush;
            }
            else
            {
                //(use the Windows default settings.)
                ;
            }

            break;
        }
    default:
        {
            if( ::IsWindowEnabled(pWnd->m_hWnd) )
            {
                pDC->SetBkMode(TRANSPARENT);
                hbr = (HBRUSH) m_PageBkBrush;
            }
            else
            {
                //(set up the Disable State Color for each control.)
                ;
            }

            break;
        }
    }


    return hbr;
    //--------------------------- Steven generated codes --------------------------
}

BOOL CGPRSMdmSetupPropertyPage5::OnSetActive()
{
    //--------------------------- Steven generated codes --------------------------
    CPropertySheet * pParentPropertySheet = reinterpret_cast<CPropertySheet *>(GetParent());

    //To-do: Set up the Wizard buttons.
    if(pParentPropertySheet != NULL)
    {
        pParentPropertySheet->SetWizardButtons(PSWIZB_FINISH);  //(Show the "Finish" button only.)

        CButton * pCancelButton = (CButton*) pParentPropertySheet->GetDlgItem(IDCANCEL);
        if(pCancelButton != NULL)
        {
            pCancelButton->EnableWindow(FALSE);  //(disable the "Cancel" button.)
            pCancelButton->ShowWindow(SW_SHOW);
        }else;
    }else;

    //... Slide in this page ...
    HMODULE hDll = ::LoadLibrary( _T("user32") );
    if(hDll != NULL)
    {
        typedef UINT (WINAPI *FPAnimateWindow)(HWND, DWORD, DWORD);

        FPAnimateWindow fpAnimateWindow = (FPAnimateWindow) ::GetProcAddress(hDll, "AnimateWindow");
        if(fpAnimateWindow != NULL)
        {
            fpAnimateWindow(GetSafeHwnd(), 1000, 0x00040000 | 0x00000001);
        }else;

        ::FreeLibrary(hDll);
    }else;
    //... Slide to next page ...


    return CPropertyPage::OnSetActive();
    //--------------------------- Steven generated codes --------------------------
}

bool CGPRSMdmSetupPropertyPage5::ToDo() {
    OSVERSIONINFO ovi;
    ZeroMemory(&ovi, sizeof(OSVERSIONINFO));
    ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    ::GetVersionEx(&ovi);
    if(m_RebootNowRadio.GetCheck() == 1)
    { //("Reboot Now")
        if(ovi.dwPlatformId == VER_PLATFORM_WIN32_NT) {
            HANDLE hToken;
            TOKEN_PRIVILEGES tp;
            TOKEN_PRIVILEGES tpold;
            DWORD dwReturnLength;
            ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
            ::LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tp.Privileges[0].Luid);
            tp.PrivilegeCount = 1;
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
            ::AdjustTokenPrivileges(hToken, FALSE, &tp, sizeof(TOKEN_PRIVILEGES), &tpold, &dwReturnLength);
        } else
            ;
        ::ExitWindowsEx(EWX_REBOOT, 0);
    } else
        ;
    return true;
}

BOOL CGPRSMdmSetupPropertyPage5::OnWizardFinish() {
    //--------------------------- Steven generated codes --------------------------
    CPropertySheet *pParentPropertySheet = reinterpret_cast < CPropertySheet * > (GetParent());
    //... Fade out this page ...
    HMODULE hDll = ::LoadLibrary(_T("user32"));
    if(hDll != NULL) {
        typedef UINT(WINAPI *FPAnimateWindow)(HWND, DWORD, DWORD);
        FPAnimateWindow fpAnimateWindow = (FPAnimateWindow)::GetProcAddress(hDll, "AnimateWindow");
        if(fpAnimateWindow != NULL) {
            fpAnimateWindow(pParentPropertySheet->GetSafeHwnd(), 700, 0x00040000 | 0x00000010 | 0x00010000);
        } else
            ;
        ::FreeLibrary(hDll);
    } else
        ;
    //... Fade out this page ...
    ToDo();
    return CPropertyPage::OnWizardFinish(); //(just Finish it.)
    //--------------------------- Steven generated codes --------------------------
}