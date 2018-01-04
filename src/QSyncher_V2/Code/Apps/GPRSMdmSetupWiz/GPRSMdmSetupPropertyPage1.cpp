#include "stdafx.h"
#include "GPRSMdmSetupPropertyPage1.h"
#include "GPRSMdmSetupPropertyPages.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CGPRSMdmSetupPropertyPage1, CPropertyPage)

BEGIN_MESSAGE_MAP(CGPRSMdmSetupPropertyPage1, CPropertyPage)
    //{{AFX_MSG_MAP(CGPRSMdmSetupPropertyPage1)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage1 property page
/////////////////////////////////////////////////////////////////////////////

CGPRSMdmSetupPropertyPage1::CGPRSMdmSetupPropertyPage1() : CPropertyPage(CGPRSMdmSetupPropertyPage1::IDD)
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
            m_szPageFontName	= _T("新細明體");
            m_nPageFontSize	= 11;
        }
        else if(SUBLANG_CHINESE_SIMPLIFIED == SUBLANGID(LANGIDFROMLCID(::GetUserDefaultLCID())))
        {
            m_szPageFontName	= _T("SimSun");  //(宋体)
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
    m_szPageCaptionText = LOADSTRING(IDS_GPRSMDM_PROPERTY_PAGE1_CAPTION);
    m_psp.pszTitle = m_szPageCaptionText;
    m_psp.dwFlags |= PSP_USETITLE;

    //(Set up the PropertyPage Look and Feel.)
    m_EditTextColor = RGB(95,95,95);
    m_EditBkBrush.CreateSolidBrush( RGB(219,215,190) );
    m_PageBkBrush.CreateSolidBrush( RGB(245,245,245) );
}


CGPRSMdmSetupPropertyPage1::~CGPRSMdmSetupPropertyPage1()
{
}

void CGPRSMdmSetupPropertyPage1::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGPRSMdmSetupPropertyPage1)
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE_PIC1, m_PagePicStatic);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE1, m_PageTextLine1Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE2, m_PageTextLine2Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE3, m_PageTextLine3Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE4, m_PageTextLine4Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE5, m_PageTextLine5Static);
    //}}AFX_DATA_MAP
}

BOOL CGPRSMdmSetupPropertyPage1::PreTranslateMessage(MSG* pMsg)
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

BOOL CGPRSMdmSetupPropertyPage1::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    //To-do: Set up the associated Property Page Picture.
    if(m_PagePicBitmap.LoadBitmap(IDB_BITMAP_GPRSMDMPIC1) )
    {
        m_PagePicStatic.SetBitmap( (HBITMAP)m_PagePicBitmap );
    }

    //To-do: Set up my CSyncStatic controls...............
    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE1, LOADSTRING(IDS_STATIC_PREPERTY_PAGE1_TEXT_LINE1));
    m_PageTextLine1Static.set_FontName		( m_szPageFontName );
    m_PageTextLine1Static.set_FontSize		( m_nPageFontSize );
    m_PageTextLine1Static.set_FontWeight		( FW_BOLD );
    m_PageTextLine1Static.set_Italic			( FALSE );
    m_PageTextLine1Static.set_Underline		( FALSE );
    m_PageTextLine1Static.set_TextFaceColor	( RGB(85,0,0) );
    m_PageTextLine1Static.set_3DText			( TRUE, RGB(150,150,150) );
    m_PageTextLine1Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE2, LOADSTRING(IDS_STATIC_PREPERTY_PAGE1_TEXT_LINE2));
    m_PageTextLine2Static.set_FontName		( m_szPageFontName );
    m_PageTextLine2Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine2Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine2Static.set_Italic			( TRUE );
    m_PageTextLine2Static.set_Underline		( FALSE );
    m_PageTextLine2Static.set_TextFaceColor	( RGB(60,60,60) );
    m_PageTextLine2Static.set_3DText			( FALSE );
    m_PageTextLine2Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE3, LOADSTRING(IDS_STATIC_PREPERTY_PAGE1_TEXT_LINE3));
    m_PageTextLine3Static.set_FontName		( m_szPageFontName );
    m_PageTextLine3Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine3Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine3Static.set_Italic			( FALSE );
    m_PageTextLine3Static.set_Underline		( FALSE );
    m_PageTextLine3Static.set_TextFaceColor	( RGB(80,80,80) );
    m_PageTextLine3Static.set_3DText			( FALSE );
    m_PageTextLine3Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE4, LOADSTRING(IDS_STATIC_PREPERTY_PAGE1_TEXT_LINE4));
    m_PageTextLine4Static.set_FontName		( m_szPageFontName );
    m_PageTextLine4Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine4Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine4Static.set_Italic			( FALSE );
    m_PageTextLine4Static.set_Underline		( FALSE );
    m_PageTextLine4Static.set_TextFaceColor	( RGB(60,60,60) );
    m_PageTextLine4Static.set_3DText			( FALSE );
    m_PageTextLine4Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE1_TEXT_LINE5, LOADSTRING(IDS_STATIC_PREPERTY_PAGE1_TEXT_LINE5));
    m_PageTextLine5Static.set_FontName		( m_szPageFontName );
    m_PageTextLine5Static.set_FontSize		( m_nPageFontSize - 2 );
    m_PageTextLine5Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine5Static.set_Italic			( FALSE );
    m_PageTextLine5Static.set_Underline		( TRUE );
    m_PageTextLine5Static.set_TextFaceColor	( RGB(40,47,68) );
    m_PageTextLine5Static.set_3DText			( FALSE );
    m_PageTextLine5Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)
    //......................................................

    //To-do: Add the ToolTips...............................
    m_Tooltip.Create(this, 0x00000040);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)MAKELONG(4500,0));  //(4500ms.)
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPTEXTCOLOR, (WPARAM)RGB(100,100,100), 0);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPBKCOLOR, (WPARAM)RGB(231,231,231), 0);
    m_Tooltip.Activate(TRUE);
    //......................................................


    return TRUE;  // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGPRSMdmSetupPropertyPage1::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
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
        break;
    case CTLCOLOR_LISTBOX:
        if( ::IsWindowEnabled(pWnd->m_hWnd) )
        {
            pDC->SetTextColor( m_EditTextColor );
            pDC->SetBkColor( RGB(219,215,190) );
            hbr = (HBRUSH) m_EditBkBrush;
        }

        break;
    default:
        if( ::IsWindowEnabled(pWnd->m_hWnd) )
        {
            pDC->SetBkMode(TRANSPARENT);
            hbr = (HBRUSH) m_PageBkBrush;
        }
        break;
    }
    return hbr;
}

BOOL CGPRSMdmSetupPropertyPage1::OnSetActive()
{
    CPropertySheet * pParentPropertySheet = reinterpret_cast<CPropertySheet *>(GetParent());

    //To-do: Set up the Wizard buttons.
    if(pParentPropertySheet != NULL)
    {
        pParentPropertySheet->SetWizardButtons(PSWIZB_NEXT);  //(disable the PSWIZB_BACK.)

        CButton * pCancelButton = (CButton*) pParentPropertySheet->GetDlgItem(IDCANCEL);
        if(pCancelButton != NULL)
        {
            pCancelButton->EnableWindow(TRUE);  //(enable the "Cancel" button.)
            pCancelButton->ShowWindow(SW_SHOW);
        }
    }
    return CPropertyPage::OnSetActive();
}

LRESULT CGPRSMdmSetupPropertyPage1::OnWizardNext()
{
    return CPropertyPage::OnWizardNext(); //(直接進行下一步.)
}