#include "stdafx.h"
#include "GPRSMdmSetupPropertyPage4.h"
#include "GPRSMdmSetupPropertyPages.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CGPRSMdmSetupPropertyPage4, CPropertyPage)

BEGIN_MESSAGE_MAP(CGPRSMdmSetupPropertyPage4, CPropertyPage)
    //{{AFX_MSG_MAP(CGPRSMdmSetupPropertyPage4)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage4 property page
/////////////////////////////////////////////////////////////////////////////

CGPRSMdmSetupPropertyPage4::CGPRSMdmSetupPropertyPage4() : CPropertyPage(CGPRSMdmSetupPropertyPage4::IDD)
{
    //{{AFX_DATA_INIT(CGPRSMdmSetupPropertyPage4)
    m_szDialupConnectionName = _T("");
    m_szDialNumber = _T("");
    m_szDialupUsername = _T("");
    m_szDialupPassword = _T("");
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
    m_szPageCaptionText = LOADSTRING(IDS_GPRSMDM_PROPERTY_PAGE4_CAPTION);
    m_psp.pszTitle = m_szPageCaptionText;
    m_psp.dwFlags |= PSP_USETITLE;

    //(Set up the PropertyPage Look and Feel.)
    m_EditTextColor = RGB(95,95,95);
    m_EditBkBrush.CreateSolidBrush( RGB(219,215,190) );
    m_PageBkBrush.CreateSolidBrush( RGB(245,245,245) );
    //--------------------------- Steven generated codes --------------------------
}


CGPRSMdmSetupPropertyPage4::~CGPRSMdmSetupPropertyPage4()
{
    ;
}


void CGPRSMdmSetupPropertyPage4::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGPRSMdmSetupPropertyPage4)
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE_PIC4, m_PagePicStatic);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE1, m_PageTextLine1Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE2, m_PageTextLine2Static);
    DDX_Text(pDX, IDC_EDIT_PREPERTY_PAGE4_DIALUP_CONNECTION_NAME, m_szDialupConnectionName);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE4, m_PageTextLine4Static);
    DDX_Text(pDX, IDC_EDIT_PREPERTY_PAGE4_DIALUP_NUMBER, m_szDialNumber);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE5, m_PageTextLine5Static);
    DDX_Text(pDX, IDC_EDIT_PREPERTY_PAGE4_DIALUP_USERNAME, m_szDialupUsername);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE6, m_PageTextLine6Static);
    DDX_Text(pDX, IDC_EDIT_PREPERTY_PAGE4_DIALUP_PASSWORD, m_szDialupPassword);
    //}}AFX_DATA_MAP
}

BOOL CGPRSMdmSetupPropertyPage4::PreTranslateMessage(MSG* pMsg)
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

BOOL CGPRSMdmSetupPropertyPage4::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    //--------------------------- Steven generated codes --------------------------
    //To-do: Set up the associated Property Page Picture.
    if( m_PagePicBitmap.LoadBitmap(IDB_BITMAP_GPRSMDMPIC4) )
    {
        m_PagePicStatic.SetBitmap( (HBITMAP)m_PagePicBitmap );
    }else;

    //To-do: Set up my CSyncStatic controls...............
    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE1, LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE1));
    m_PageTextLine1Static.set_FontName		( m_szPageFontName );
    m_PageTextLine1Static.set_FontSize		( m_nPageFontSize );
    m_PageTextLine1Static.set_FontWeight		( FW_BOLD );
    m_PageTextLine1Static.set_Italic			( FALSE );
    m_PageTextLine1Static.set_Underline		( FALSE );
    m_PageTextLine1Static.set_TextFaceColor	( RGB(85,0,0) );
    m_PageTextLine1Static.set_3DText			( TRUE, RGB(150,150,150) );
    m_PageTextLine1Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE2, LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE2));
    m_PageTextLine2Static.set_FontName		( m_szPageFontName );
    m_PageTextLine2Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine2Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine2Static.set_Italic			( FALSE );
    m_PageTextLine2Static.set_Underline		( FALSE );
    m_PageTextLine2Static.set_TextFaceColor	( RGB(60,60,60) );
    m_PageTextLine2Static.set_3DText			( FALSE );
    m_PageTextLine2Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE4, LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE4));
    m_PageTextLine4Static.set_FontName		( m_szPageFontName );
    m_PageTextLine4Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine4Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine4Static.set_Italic			( FALSE );
    m_PageTextLine4Static.set_Underline		( FALSE );
    m_PageTextLine4Static.set_TextFaceColor	( RGB(60,60,60) );
    m_PageTextLine4Static.set_3DText			( FALSE );
    m_PageTextLine4Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE5, LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE5));
    m_PageTextLine5Static.set_FontName		( m_szPageFontName );
    m_PageTextLine5Static.set_FontSize		( m_nPageFontSize - 2 );
    m_PageTextLine5Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine5Static.set_Italic			( FALSE );
    m_PageTextLine5Static.set_Underline		( FALSE );
    m_PageTextLine5Static.set_TextFaceColor	( RGB(87,63,88) );
    m_PageTextLine5Static.set_3DText			( FALSE );
    m_PageTextLine5Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE4_TEXT_LINE6, LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE6));
    m_PageTextLine6Static.set_FontName		( m_szPageFontName );
    m_PageTextLine6Static.set_FontSize		( m_nPageFontSize - 2 );
    m_PageTextLine6Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine6Static.set_Italic			( FALSE );
    m_PageTextLine6Static.set_Underline		( FALSE );
    m_PageTextLine6Static.set_TextFaceColor	( RGB(87,63,88) );
    m_PageTextLine6Static.set_3DText			( FALSE );
    m_PageTextLine6Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)
    //......................................................

    //To-do: Add the ToolTips...............................
    m_Tooltip.Create(this, 0x00000040);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)MAKELONG(4500,0));  //(4500ms.)
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPTEXTCOLOR, (WPARAM)RGB(100,100,100), 0);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPBKCOLOR, (WPARAM)RGB(231,231,231), 0);
    m_Tooltip.Activate(TRUE);

    m_Tooltip.AddTool(GetDlgItem(IDC_EDIT_PREPERTY_PAGE4_DIALUP_CONNECTION_NAME), LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE2));
    m_Tooltip.AddTool(GetDlgItem(IDC_EDIT_PREPERTY_PAGE4_DIALUP_NUMBER), LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE4));
    m_Tooltip.AddTool(GetDlgItem(IDC_EDIT_PREPERTY_PAGE4_DIALUP_USERNAME), LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE5));
    m_Tooltip.AddTool(GetDlgItem(IDC_EDIT_PREPERTY_PAGE4_DIALUP_PASSWORD), LOADSTRING(IDS_STATIC_PREPERTY_PAGE4_TEXT_LINE6));
    //......................................................
    //-------------------------------------------------------------------------
    // 加入預設值, 如果有Operator.ini檔, 寫入ini裡定義的值
    // 如果沒有撥按號碼, 就寫入預設 ==> *99#
    // 如果沒有連線名稱, 就寫入Test
    //-------------------------------------------------------------------------
    CString szDirPath;
    GetDir(szDirPath);
    CString strPath = CString(szDirPath) + CString(_T("\\")) + CString(_T("OperatorAPN.ini"));
    CIniReader ini((LPTSTR)(LPCTSTR)strPath);
    CString strDialNumber = ini.ReadString(_T("GPRS_MODEM_SETTING"), _T("PHONE_NUMBER"),_T("*99#"));
    CString strConnectionName = ini.ReadString(_T("GPRS_MODEM_SETTING"), _T("CONNECT_NAME"), _T("Dial up"));
    CString strUserName = ini.ReadString(_T("GPRS_MODEM_SETTING"), _T("USER_NAME"), _T(""));
    CString strPassword = ini.ReadString(_T("GPRS_MODEM_SETTING"), _T("PASSWORD"), _T(""));
    m_szDialNumber = strDialNumber;
    m_szDialupConnectionName = strConnectionName;
    m_szDialupUsername = strUserName;
    m_szDialupPassword = strPassword;
    UpdateData(FALSE);
    return TRUE; // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGPRSMdmSetupPropertyPage4::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CGPRSMdmSetupPropertyPage4::OnSetActive()
{
    //--------------------------- Steven generated codes --------------------------
    CPropertySheet * pParentPropertySheet = reinterpret_cast<CPropertySheet *>(GetParent());

    //To-do: Set up the Wizard buttons.
    if(pParentPropertySheet != NULL)
    {
        pParentPropertySheet->SetWizardButtons(PSWIZB_NEXT);  //(*Can not Back to the previous step, cuz the driver has been installed.)

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
        }
        ::FreeLibrary(hDll);
    }
    //... Slide to next page ...


    return CPropertyPage::OnSetActive();
    //--------------------------- Steven generated codes --------------------------
}

bool CGPRSMdmSetupPropertyPage4::ToDo() {
    CString szConnectionName; //(e.g. "BenQ GPRS Dial-up Connection")
    CString szDailNumber; //(default to "*99#".)
    CString szUserName, szPassword; //(default to be not necessary.)
    //~~~~~ Model-dependent Modem Name ~~~~~
    CString szModemName = theApp.m_szModemName; //GetHardwareDeviceID(_T("HardwareID.ini"));

    //if(theApp.m_szModelName == MODEL_NAME_J21) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_J21;
    //} else if(theApp.m_szModelName == MODEL_NAME_J11B2A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_J11B2A;
    //} else if(theApp.m_szModelName == MODEL_NAME_J11B4A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_J11B4A;
    //} else if(theApp.m_szModelName == MODEL_NAME_V3) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_V3;
    //} else if(theApp.m_szModelName == MODEL_NAME_HERMESC1A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_HERMESC1A;
    //} else if(theApp.m_szModelName == MODEL_NAME_HYPERIONB1A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_HYPERIONB1A;
    //} else if(theApp.m_szModelName == MODEL_NAME_PANDORAB2A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_PANDORAB2A;
    //} else if(theApp.m_szModelName == MODEL_NAME_PANDORAB3A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_PANDORAB3A;
    //} else if(theApp.m_szModelName == MODEL_NAME_HERMESB1A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_HERMESB1A;
    //} else if(theApp.m_szModelName == MODEL_NAME_CUPIDS1A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_CUPIDS1A;
    //} else if(theApp.m_szModelName == MODEL_NAME_HERMESC3A) {
    //    szModemName = MODEM_DEVICE_FRIENDLY_NAME_HERMESC3A;
    //} else {
    //    szModemName = _T("BenQ Generic USB Wireless Modem");
    //}
    //~~~~~ Model-dependent Modem Name ~~~~~
    //(get the user input values.)
    UpdateData(TRUE);
    szConnectionName = m_szDialupConnectionName;
    szDailNumber = m_szDialNumber;
    szUserName = m_szDialupUsername;
    szPassword = m_szDialupPassword;
    UpdateData(FALSE);
    if(szConnectionName == "") {
        CString szErrMsg = LOADSTRING(IDS_GPRSMDM_CONNECTIONNAME_EMPTY);
        ::MessageBox(NULL, (LPCTSTR)szErrMsg, AfxGetApp()->m_pszAppName, MB_ICONERROR | MB_OK);
        return false;
    } else if(szDailNumber == "") {
        CString szErrMsg = LOADSTRING(IDS_GPRSMDM_DIALNUMBER_EMPTY);
        ::MessageBox(NULL, (LPCTSTR)szErrMsg, AfxGetApp()->m_pszAppName, MB_ICONERROR | MB_OK);
        return false;
    }
    //(delete the old one first if existed.)
    DeleteDialupSettings(szConnectionName);
    //(then create the new one.)
    return CreateDialupSettings(szConnectionName, szDailNumber, szModemName, szUserName, szPassword);
}

LRESULT CGPRSMdmSetupPropertyPage4::OnWizardNext() {
    //--------------------------- Steven generated codes --------------------------
    CPropertySheet *pParentPropertySheet = reinterpret_cast < CPropertySheet * > (GetParent());
    if(pParentPropertySheet != NULL) {
        //(Disable the "Next" button temporarily.)
        pParentPropertySheet->SetWizardButtons(0);
        //To-do: Create the Dial-up Connection.
        bool bResult = ToDo();
        if(bResult != true) {
            //(Enable the "Next" button again.)
            pParentPropertySheet->SetWizardButtons(PSWIZB_NEXT);
            return 1; //(prevent the page from changing.)
        }
    }
    return CPropertyPage::OnWizardNext();
    //--------------------------- Steven generated codes --------------------------
}
