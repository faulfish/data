#include "stdafx.h"
#include "GPRSMdmSetupPropertyPage2.h"
#include "GPRSMdmSetupPropertyPages.h"
#include "resource.h"

#pragma warning(push)
#pragma warning(disable:4312)

IMPLEMENT_DYNCREATE(CGPRSMdmSetupPropertyPage2, CPropertyPage)

BEGIN_MESSAGE_MAP(CGPRSMdmSetupPropertyPage2, CPropertyPage)
    //{{AFX_MSG_MAP(CGPRSMdmSetupPropertyPage2)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage2 property page
/////////////////////////////////////////////////////////////////////////////

CGPRSMdmSetupPropertyPage2::CGPRSMdmSetupPropertyPage2() : CPropertyPage(CGPRSMdmSetupPropertyPage2::IDD),m_ModemChangeEvent(FALSE,TRUE,_T("DetectPort"))
{
    //(Disable the "Help" button.)
    m_psp.dwFlags &= ~PSP_HASHELP;

    //(*Set up the PropertyPage Font according to the Locale of the current Windows dynamically.)
    switch( PRIMARYLANGID(LANGIDFROMLCID(::GetUserDefaultLCID())) )
    {
    case LANG_ENGLISH:
        m_szPageFontName	= _T("Verdana");
        m_nPageFontSize	= 10;

        break;
    case LANG_CHINESE:
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
    case LANG_JAPANESE:
        m_szPageFontName	= _T("MS UI Gothic");
        m_nPageFontSize	= 10;

        break;
    case LANG_KOREAN:
        m_szPageFontName	= _T("Gulim");
        m_nPageFontSize	= 10;

        break;
    default:
        m_szPageFontName	= _T("MS Shell Dlg");
        m_nPageFontSize	= 9;

        break;
    }

    //(*You have the only chance to set up the Property Page Caption in his Create method.)
    m_szPageCaptionText = LOADSTRING(IDS_GPRSMDM_PROPERTY_PAGE2_CAPTION);
    m_psp.pszTitle = m_szPageCaptionText;
    m_psp.dwFlags |= PSP_USETITLE;

    //(Set up the PropertyPage Look and Feel.)
    m_EditTextColor = RGB(95,95,95);
    m_EditBkBrush.CreateSolidBrush( RGB(219,215,190) );
    m_PageBkBrush.CreateSolidBrush( RGB(245,245,245) );
}


CGPRSMdmSetupPropertyPage2::~CGPRSMdmSetupPropertyPage2()
{
    ;
}
void CGPRSMdmSetupPropertyPage2::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGPRSMdmSetupPropertyPage2)
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE_PIC2, m_PagePicStatic);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE2_TEXT_LINE1, m_PageTextLine1Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE2_TEXT_LINE2, m_PageTextLine2Static);
    //}}AFX_DATA_MAP
}


BOOL CGPRSMdmSetupPropertyPage2::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message)
    {
    case WM_MOUSEMOVE:
        m_Tooltip.RelayEvent(pMsg);
        break;
    default:
        //Do nothing.
        break;
    }

    return CPropertyPage::PreTranslateMessage(pMsg);
}

BOOL CGPRSMdmSetupPropertyPage2::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    //--------------------------- Steven generated codes --------------------------
    //To-do: Set up the associated Property Page Picture.
    if( m_PagePicBitmap.LoadBitmap(IDB_BITMAP_GPRSMDMPIC2) )
    {
        m_PagePicStatic.SetBitmap( (HBITMAP)m_PagePicBitmap );
    }

    //To-do: Set up my CSyncStatic controls...............
    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE2_TEXT_LINE1, LOADSTRING(IDS_STATIC_PREPERTY_PAGE2_TEXT_LINE1));
    m_PageTextLine1Static.set_FontName		( m_szPageFontName );
    m_PageTextLine1Static.set_FontSize		( m_nPageFontSize );
    m_PageTextLine1Static.set_FontWeight		( FW_BOLD );
    m_PageTextLine1Static.set_Italic			( FALSE );
    m_PageTextLine1Static.set_Underline		( FALSE );
    m_PageTextLine1Static.set_TextFaceColor	( RGB(85,0,0) );
    m_PageTextLine1Static.set_3DText			( TRUE, RGB(150,150,150) );
    m_PageTextLine1Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE2_TEXT_LINE2, LOADSTRING(IDS_STATIC_PREPERTY_PAGE2_TEXT_LINE2));
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
    //--------------------------- Steven generated codes --------------------------


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGPRSMdmSetupPropertyPage2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
    //--------------------------- Steven generated codes --------------------------
    HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);


    switch( nCtlColor )
    {
    case CTLCOLOR_DLG:
        pDC->SetBkMode(TRANSPARENT);
        hbr = (HBRUSH) m_PageBkBrush;

        break;
    case CTLCOLOR_EDIT:
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
    case CTLCOLOR_LISTBOX:
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
    default:
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

    return hbr;
}

BOOL CGPRSMdmSetupPropertyPage2::OnSetActive()
{
    CPropertySheet * pParentPropertySheet = reinterpret_cast<CPropertySheet *>(GetParent());

    //To-do: Set up the Wizard buttons.
    if(pParentPropertySheet != NULL)
    {
        pParentPropertySheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

        CButton * pCancelButton = (CButton*) pParentPropertySheet->GetDlgItem(IDCANCEL);
        if(pCancelButton != NULL)
        {
            pCancelButton->EnableWindow(TRUE);  //(enable the "Cancel" button.)
            pCancelButton->ShowWindow(SW_SHOW);
        }
    }

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
}

void CGPRSMdmSetupPropertyPage2::Event(const TSTRING& strEvent,long nParam)
{
    TRACE(strEvent.c_str());
    TRACE(_T(".....................................\n"));
    using namespace QSYNC_LIB;
    if(strEvent == DEVICE_CONNECT)
    {
        DeviceDataPt* pspDeviceData = (DeviceDataPt*)nParam;

        theApp.m_szCOMPortID = (*pspDeviceData)->ID().c_str();
        theApp.m_szModelName = (*pspDeviceData)->GetExt().c_str();
        m_ModemChangeEvent.SetEvent();
    }
    else if(strEvent == DEVICE_DISCONNECT)
    {
        DeviceDataPt* pspDeviceData = (DeviceDataPt*)nParam;
    }
}

LRESULT CGPRSMdmSetupPropertyPage2::OnWizardNext() 
{
    LRESULT nRes = 1;
    using namespace QSYNC_LIB;

    HCURSOR OldCursor = ::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_WAIT));

    m_ModemChangeEvent.ResetEvent();
    GetDeviceFinder()->Register(this);
    GetDeviceFinder()->StartMonitor();

    CPropertySheet *pParentPropertySheet = reinterpret_cast < CPropertySheet * > (GetParent());
    pParentPropertySheet->SetWizardButtons(PSWIZB_BACK);

    if(MessageLoop(&m_ModemChangeEvent.m_hObject,1,10000) == WAIT_OBJECT_0)
        nRes = CPropertyPage::OnWizardNext();
    else
        nRes = 1;

    ::SetCursor(OldCursor);

    if(nRes == 1)
    {
        CString szErrMsg = LOADSTRING(IDS_GPRSMDM_NO_MOBILE_PHONE_DETECTED);
        ::MessageBox(NULL, (LPCTSTR)szErrMsg, AfxGetApp()->m_pszAppName, MB_ICONERROR | MB_OK);
    }

    pParentPropertySheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
    return nRes;
}

#pragma warning(pop)