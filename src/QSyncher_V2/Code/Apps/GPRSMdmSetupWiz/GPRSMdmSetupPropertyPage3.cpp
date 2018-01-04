#include "stdafx.h"
#include "GPRSMdmSetupPropertyPage3.h"
#include "GPRSMdmSetupPropertyPages.h"
#include "GPRSUtil.h"
#include "resource.h"

IMPLEMENT_DYNCREATE(CGPRSMdmSetupPropertyPage3, CPropertyPage)

BEGIN_MESSAGE_MAP(CGPRSMdmSetupPropertyPage3, CPropertyPage)
    //{{AFX_MSG_MAP(CGPRSMdmSetupPropertyPage3)
    ON_WM_CTLCOLOR()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertyPage3 property page
/////////////////////////////////////////////////////////////////////////////

CGPRSMdmSetupPropertyPage3::CGPRSMdmSetupPropertyPage3() : CPropertyPage(CGPRSMdmSetupPropertyPage3::IDD)
{
    //{{AFX_DATA_INIT(CGPRSMdmSetupPropertyPage3)
    m_szAPN = _T("");
    //}}AFX_DATA_INIT

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
    m_szPageCaptionText = LOADSTRING(IDS_GPRSMDM_PROPERTY_PAGE3_CAPTION);
    m_psp.pszTitle = m_szPageCaptionText;
    m_psp.dwFlags |= PSP_USETITLE;

    //(Set up the PropertyPage Look and Feel.)
    m_EditTextColor = RGB(95,95,95);
    m_EditBkBrush.CreateSolidBrush( RGB(219,215,190) );
    m_PageBkBrush.CreateSolidBrush( RGB(245,245,245) );
}


CGPRSMdmSetupPropertyPage3::~CGPRSMdmSetupPropertyPage3()
{
    ;
}


void CGPRSMdmSetupPropertyPage3::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CGPRSMdmSetupPropertyPage3)
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE_PIC3, m_PagePicStatic);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE3_TEXT_LINE1, m_PageTextLine1Static);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE3_TEXT_LINE2, m_PageTextLine2Static);
    DDX_Control(pDX, IDC_COMBO_PREPERTY_PAGE3_COMPORT, m_COMPortCombo);
    DDX_Control(pDX, IDC_STATIC_PREPERTY_PAGE3_TEXT_LINE3, m_PageTextLine3Static);
    DDX_Text(pDX, IDC_EDIT_PREPERTY_PAGE3_APN, m_szAPN);
    //}}AFX_DATA_MAP
}

BOOL CGPRSMdmSetupPropertyPage3::PreTranslateMessage(MSG* pMsg)
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

BOOL CGPRSMdmSetupPropertyPage3::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    //To-do: Set up the associated Property Page Picture.
    if( m_PagePicBitmap.LoadBitmap(IDB_BITMAP_GPRSMDMPIC3) )
    {
        m_PagePicStatic.SetBitmap( (HBITMAP)m_PagePicBitmap );
    }

    //To-do: Set up my CSyncStatic controls...............
    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE3_TEXT_LINE1, LOADSTRING(IDS_STATIC_PREPERTY_PAGE3_TEXT_LINE1));
    m_PageTextLine1Static.set_FontName		( m_szPageFontName );
    m_PageTextLine1Static.set_FontSize		( m_nPageFontSize );
    m_PageTextLine1Static.set_FontWeight		( FW_BOLD );
    m_PageTextLine1Static.set_Italic			( FALSE );
    m_PageTextLine1Static.set_Underline		( FALSE );
    m_PageTextLine1Static.set_TextFaceColor	( RGB(85,0,0) );
    m_PageTextLine1Static.set_3DText			( TRUE, RGB(150,150,150) );
    m_PageTextLine1Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE3_TEXT_LINE2, LOADSTRING(IDS_STATIC_PREPERTY_PAGE3_TEXT_LINE2));
    m_PageTextLine2Static.set_FontName		( m_szPageFontName );
    m_PageTextLine2Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine2Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine2Static.set_Italic			( FALSE );
    m_PageTextLine2Static.set_Underline		( FALSE );
    m_PageTextLine2Static.set_TextFaceColor	( RGB(60,60,60) );
    m_PageTextLine2Static.set_3DText			( FALSE );
    m_PageTextLine2Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)

    ::SetDlgItemText(m_hWnd, IDC_STATIC_PREPERTY_PAGE3_TEXT_LINE3, LOADSTRING(IDS_STATIC_PREPERTY_PAGE3_TEXT_LINE3));
    m_PageTextLine3Static.set_FontName		( m_szPageFontName );
    m_PageTextLine3Static.set_FontSize		( m_nPageFontSize - 1 );
    m_PageTextLine3Static.set_FontWeight		( FW_NORMAL );
    m_PageTextLine3Static.set_Italic			( FALSE );
    m_PageTextLine3Static.set_Underline		( FALSE );
    m_PageTextLine3Static.set_TextFaceColor	( RGB(60,60,60) );
    m_PageTextLine3Static.set_3DText			( FALSE );
    m_PageTextLine3Static.RedrawText();  //(*Dont forget to call this method to make the above changes take effect.)
    //......................................................

    //To-do: Add the ToolTips...............................
    m_Tooltip.Create(this, 0x00000040);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETDELAYTIME, TTDT_AUTOPOP, (LPARAM)MAKELONG(4500,0));  //(4500ms.)
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPTEXTCOLOR, (WPARAM)RGB(100,100,100), 0);
    ::SendMessage(m_Tooltip.GetSafeHwnd(), TTM_SETTIPBKCOLOR, (WPARAM)RGB(231,231,231), 0);
    m_Tooltip.Activate(TRUE);

    m_Tooltip.AddTool(GetDlgItem(IDC_EDIT_PREPERTY_PAGE3_APN), LOADSTRING(IDS_STATIC_PREPERTY_PAGE3_TEXT_LINE3));
    //......................................................

    //(generate the ComboBox item.)
    DWORD dwSize = 0;
    DWORD dwNumOfDevices = 0;
    DWORD dwRV = RasEnumDevices(NULL, &dwSize, &dwNumOfDevices);    // Get total devices' count

    RASDEVINFO *lpRdi = new RASDEVINFO[dwNumOfDevices];
    lpRdi->dwSize = sizeof(*lpRdi);
    dwRV = RasEnumDevices(lpRdi, &dwSize, &dwNumOfDevices);
    if(dwRV == 0)
    {
        CString strType;
        int j = 0;
        for(int i = 0; i < (int)dwNumOfDevices; i++)
        {
            strType = lpRdi[i].szDeviceType;
            if(strType.CompareNoCase(_T("MODEM")) == 0)  // = "RASDT_Modem";
            {
                m_COMPortCombo.AddString(lpRdi[i].szDeviceName);
                j++;
            } 
        }
        delete []lpRdi; 
    }

    if(!theApp.m_szCOMPortID.IsEmpty() && m_COMPortCombo.GetCount())
    {
        m_COMPortCombo.SelectString(0, _T("BenQ"));
    }
    //-------------------------------------------------------------------------
    // 加入預設值, 如果有Operator.ini檔, 寫入ini裡定義的值
    // 如果沒有, 就寫入預設 ==> AT+CGDCONT=1,"IP","Internet","",0,0
    //-------------------------------------------------------------------------
    CString szDirPath;
    GetDir(szDirPath);
    CString strPath = CString(szDirPath) + CString(_T("\\ModemSetupSetting\\")) + CString(_T("OperatorAPN.ini"));
    CIniReader ini((LPTSTR)(LPCTSTR)strPath);
    CString strAPN = ini.ReadString(_T("GPRS_MODEM_SETTING"), _T("SETTING"), _T("Internet"));
    m_szAPN = strAPN;
    UpdateData(FALSE);
    return TRUE; // return TRUE unless you set the focus to a control
    // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CGPRSMdmSetupPropertyPage3::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

BOOL CGPRSMdmSetupPropertyPage3::OnSetActive()
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
        }

        ::FreeLibrary(hDll);
    }
    //... Slide to next page ...

    return CPropertyPage::OnSetActive();
}

bool CGPRSMdmSetupPropertyPage3::ToDo()
{
    CString szCOMPortID = theApp.m_szCOMPortID;  //(e.g. "COM3")
    CString szAPN;  //(e.g. "emome")
    CString szINFFileName               = MODEM_INF_FILE_NAME;				//(must be the same with the INF given.)
    CString szWin16InstallerFileName    = WIN16_MODEM_INSTALLER_FILE_NAME;	//(must be the same with the EXE given.)
    CString szWin32InstallerFileName    = WIN32_MODEM_INSTALLER_FILE_NAME;	//(must be the same with the EXE given.)

    //~~~~~ Model-dependent Modem HardwareID ~~~~~
    // Get Hardware ID like USB\Vid_04a5&Pid_4045&Rev_0000&MI_00
    CString szModemHardwareID = GetHardwareDeviceID(_T("HardwareID.ini"));

    //(get the user input values.)
    UpdateData(TRUE);
    //m_COMPortCombo.GetLBText(m_COMPortCombo.GetCurSel(), szCOMPortID);
    szAPN = m_szAPN;
    UpdateData(FALSE);
    m_COMPortCombo.GetLBText(this->m_COMPortCombo.GetCurSel(), theApp.m_szModemName);

    //(Enable the "Back" and "Next" buttons.)
    CPropertySheet * pParentPropertySheet = reinterpret_cast<CPropertySheet *>(GetParent());
    pParentPropertySheet->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);

    //(validate the necessary parameters first.)
    if(szCOMPortID == "") {
        CString szErrMsg = LOADSTRING(IDS_GPRSMDM_COMPORTID_EMPTY);
        ::MessageBox(NULL, (LPCTSTR)szErrMsg, AfxGetApp()->m_pszAppName, MB_ICONERROR | MB_OK);
        return false;
    } else if(szAPN == "") {
        CString szErrMsg = LOADSTRING(IDS_GPRSMDM_APN_EMPTY);
        ::MessageBox(NULL, (LPCTSTR)szErrMsg, AfxGetApp()->m_pszAppName, MB_ICONERROR | MB_OK);
        return false;
    }
    // 在這裡完成APN字串之後寫入裝置管理員, 新增一個modem...
    // 有必要在這裡辨別出輸入的是什麼樣的字串, 以有沒有AT+為開頭做為辨別
    // 如果有AT+就當成全輸入, 如果沒有就當成只有APN
    if(szAPN.Find(_T("AT+")) ==  - 1) {
        // 找不到AT開始的，所以把他當成只有APN
        szAPN = CString(_T("AT+CGDCONT=1,\"IP\",\"")) + szAPN + CString(_T("\",,,"));
    }

    //(invoke the specific installer.)
    CGPRSUtil::ChangeDeviceProperty(szModemHardwareID, TAG_MODEM_CALSS_USERINIT, szAPN);
    //OSVERSIONINFO ovi;
    //ZeroMemory(&ovi, sizeof(OSVERSIONINFO));
    //ovi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    //::GetVersionEx(&ovi);
    //CString szExePath, szCmdLineParams, szDirPath;
    //GetDir(szDirPath);
    //szExePath = CString(szDirPath) + CString(_T("\\")) + CString(szWin32InstallerFileName);
    //szCmdLineParams = CString(szINFFileName) + CString(_T(";")) + szCOMPortID + CString(_T(";")) + szAPN + CString(_T(";")) + szModemHardwareID;
    //if(szExePath.IsEmpty() || szCmdLineParams.IsEmpty()) {
    //    TRACE("[CGPRSMdmSetupPropertyPage3::ToDo]: Invalid Shell command.\n");
    //    return false;
    //} else {
    //    STARTUPINFO si;
    //    ZeroMemory(&si, sizeof(si));
    //    si.cb = sizeof(si);
    //    si.dwFlags = STARTF_USESHOWWINDOW;
    //    si.wShowWindow = SW_HIDE;
    //    PROCESS_INFORMATION pi;
    //    ZeroMemory(&pi, sizeof(pi));
    //    if(::CreateProcess((LPTSTR)LPCTSTR(szExePath), (LPTSTR)LPCTSTR(szCmdLineParams), NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
    //        ::CloseHandle(pi.hThread);
    //        ::WaitForSingleObject(pi.hProcess, 600000); //(只等十分鐘或'INFINITE'亦可.)
    //        DWORD dwExitCode;
    //        ::GetExitCodeProcess(pi.hProcess, &dwExitCode);
    //        ::CloseHandle(pi.hProcess);
    //    } else {
    //        DWORD dwErrorCode = ::GetLastError();
    //        CString szErrorMsg;
    //        CString szSystemErrorMsg;
    //        if(GetErrorMsg(szSystemErrorMsg, dwErrorCode)) {
    //            szErrorMsg.Format(_T("%s"), szSystemErrorMsg);
    //        } else {
    //            szErrorMsg.Format(_T("0x%8.8lX"), dwErrorCode);
    //        }
    //        TRACE("[CGPRSMdmSetupPropertyPage3::ToDo]: CreateProcess error: %s.\n", szErrorMsg);
    //        return false; //failure.
    //    }
    //}
    return true;
}

LRESULT CGPRSMdmSetupPropertyPage3::OnWizardNext() {
    CPropertySheet *pParentPropertySheet = reinterpret_cast < CPropertySheet * > (GetParent());
    if(pParentPropertySheet != NULL) {
        //(Disable the "Next" button temporarily.)
        pParentPropertySheet->SetWizardButtons(0);
        //To-do: Install the modem driver.
        bool bResult = ToDo();
        if(bResult != true) {
            return 1; //(prevent the page from changing.)
        }
    }
    return CPropertyPage::OnWizardNext();
}

