// AnonymousContactDialog.cpp : implementation file
//

#include "stdafx.h"
#include "AnonymousContactDialog.h"


// CAnonymousContactDialog dialog

IMPLEMENT_DYNAMIC(CAnonymousContactDialog, CQDialog)

CAnonymousContactDialog* g_pThis = NULL;

CAnonymousContactDialog::CAnonymousContactDialog(CWnd* pParent /*=NULL*/)
: Win32DoubleBufferQDialogWidge(CAnonymousContactDialog::IDD, pParent),
m_pStrContact1(NULL),
m_pStrContact2(NULL),
m_pStrContact3(NULL),
m_pStrContact4(NULL),
m_pStrContact5(NULL), 
m_pIPIMConfig(NULL)
{
	g_pThis = this;
}

CAnonymousContactDialog::~CAnonymousContactDialog()
{
	m_pIPIMConfig = NULL;
}
BOOL CAnonymousContactDialog::OnInitDialog()
{
	CQDialog::OnInitDialog();
	//SetUI();
	SetXMLUI();
	SetPhoneCheckingWords();
	return TRUE;
}
void CAnonymousContactDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTACT_EDIT1, m_QEditCtrlContact1	);
	DDX_Control(pDX, IDC_CONTACT_EDIT2, m_QEditCtrlContact2	);
	DDX_Control(pDX, IDC_CONTACT_EDIT3, m_QEditCtrlContact3	);
	DDX_Control(pDX, IDC_CONTACT_EDIT4, m_QEditCtrlContact4	);
	DDX_Control(pDX, IDC_CONTACT_EDIT5, m_QEditCtrlContact5	);
}

BEGIN_MESSAGE_MAP(CAnonymousContactDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CAnonymousContactDialog::OnBnClickedOk)
	ON_WM_SIZING()
	ON_EN_CHANGE(IDC_CONTACT_EDIT1,  &CQEditCtrl::OnEnChange)
	ON_EN_CHANGE(IDC_CONTACT_EDIT2, &CQEditCtrl::OnEnChange)
	ON_EN_CHANGE(IDC_CONTACT_EDIT3,	  &CQEditCtrl::OnEnChange)
	ON_EN_CHANGE(IDC_CONTACT_EDIT4,   &CQEditCtrl::OnEnChange)
	ON_EN_CHANGE(IDC_CONTACT_EDIT5,     &CQEditCtrl::OnEnChange)
END_MESSAGE_MAP()

void CAnonymousContactDialog::Event(const TSTRING& strEvent, long nParam)
{
	if(strEvent == EVENT_BUTTON_CLICK)
	{
		if(m_pBtnOK->GetID() == nParam)
			OnBnClickedOk();
		else if (m_pBtnCancel->GetID() == nParam)
			OnBnClickedCancel();
		else if (m_pWinCloseButton->GetID() == nParam)
			FnWinClose();
	}
}
void CAnonymousContactDialog::SetXMLUI()
{
	SetBackground();
	CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
	Widge<HDC>* pWidget = pLayoutManager->Load(_T("SMS_AnonymousContactDlgID"));
	m_pWinCloseButton = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_0"));
	m_pBtnCancel = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_1"));
	m_pBtnOK = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_2"));
	SetLayout(pWidget);
	delete pLayoutManager;
}

void CAnonymousContactDialog::SetUI()
{
	SetBackground();
	SplitLayout<HDC>* pMainLayout = new SplitLayout<HDC>(false);

	SplitLayout<HDC>* pCaptionLayout = CreateCaptionLayout();
	SplitLayout<HDC>* pClientLayout = CreateClientLayout();

	pMainLayout->AddWidget(pCaptionLayout, pClientLayout);
	pMainLayout->SetFixedValue(29);
	pMainLayout->SetSpacing(2);

	SetLayout(pMainLayout);

#ifdef _DEBUG
	CLayoutManager LayoutManager(this, this);
	bool bSave = LayoutManager.Save(pMainLayout, _T("SMS_AnonymousContactDlg"), _T("SMS_AnonymousContactDlgID"));
#endif
}
void CAnonymousContactDialog::SetBackground()
{
	std::wstring strBkImage = L"Message\\Background\\Bk_SMSEdit_NewNUM.png";
	SetBKImage(strBkImage,BK_Fixed);

	const int nContactDlgWIDTH = GetBKImage()->GetWidth(); 
	const int nContactDlgHEIGHT = GetBKImage()->GetHeight() ;
	SetMinWidth(nContactDlgWIDTH);
	SetMinHeight(nContactDlgHEIGHT);
	SetMaxWidth(nContactDlgWIDTH);
	SetMaxHeight(nContactDlgHEIGHT);
}
void CAnonymousContactDialog::SetPhoneCheckingWords()
{
	TSTRING strCheckWords;
	GetLimitation(_T("PhoneLimitWords"), strCheckWords);
	if(strCheckWords.empty())
		strCheckWords = _T("0123456789P*#+");

	m_QEditCtrlContact1.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
	m_QEditCtrlContact2.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
	m_QEditCtrlContact3.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
	m_QEditCtrlContact4.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
	m_QEditCtrlContact5.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
}
bool CAnonymousContactDialog::GetLimitation(const TSTRING& strKey, TSTRING& strLimitation)
{
	bool bRes = true;
	if(!m_pIPIMConfig)
		m_pIPIMConfig = (IPIMCONFIG*) CommCreateInterface(_T("QSycner"), _T("Phonebook"), eIPIMConfigure);

	if(m_pIPIMConfig)
	{
		m_pIPIMConfig->GetProperty(_T("PhonebookConfig"), strKey, strLimitation);
		bRes = (!strLimitation.empty());
	}
	else
	{
		bRes = false;
	}
	return bRes;
}

SplitLayout<HDC>* CAnonymousContactDialog::CreateCaptionLayout()
{
	SplitLayout<HDC>* pCaptionLayout = new SplitLayout<HDC>;
	WIN32LabelWidge* pCaptionText = new WIN32LabelWidge(this,L"IDS_Message_AnonymousDlg_AddPhoneNo",Left|VCenter,L"Arial",9);
	pCaptionText->SetMargin(0,0,13,0);
	pCaptionText->SetTextColor(255,255,255,255);
	pCaptionLayout->AddWidget(pCaptionText,Left,true);

	TSTRING strWinCloseOriginal = _T("General\\Button\\Btn_Close_Original.png");
	TSTRING strWinCloseMouseOver = _T("General\\Button\\Btn_Close_MouseOver.png");
	TSTRING strWinCloseMouseDown = _T("General\\Button\\Btn_Close_MouseDown.png");
	TSTRING strWinCloseDisable = _T("General\\Button\\Btn_Close_Disable.png");
	m_pWinCloseButton = new WIN32IconButton(
		this
		,L""
		,strWinCloseOriginal
		,strWinCloseMouseOver
		,strWinCloseMouseDown
		,strWinCloseDisable
		,WinClose
		);
	m_pWinCloseButton->SetWidthHeightByImage();
	pCaptionLayout->AddWidget(m_pWinCloseButton,Right,false);
	pCaptionLayout->SetVariouslyMargin(3,1,0,15,false);
	return pCaptionLayout;
}
SplitLayout<HDC>* CAnonymousContactDialog::CreateClientLayout()
{
	SplitLayout<HDC>* pClientLayout = new SplitLayout<HDC>(false);
	pClientLayout->AddWidget( CreateSubWindowLayout() , CreateButtonLayout() );
	pClientLayout->SetFixedValue(264-30);
	pClientLayout->SetVariouslyMargin(44-30 , 2 , 10 , 11 , true);
	pClientLayout->SetVariouslyMargin(194-186 , 0 , 0 , 9 , false);

	return pClientLayout;
}

SplitLayout<HDC>* CAnonymousContactDialog::CreateSubclientLayout()
{
	BoxLayout<HDC>* pIndexLayout = new BoxLayout<HDC>(TopToBottom);
	BoxLayout<HDC>* pCtrlLayout = new BoxLayout<HDC>(TopToBottom);
	for(int nIndex = 1;nIndex<=5;++nIndex)
	{
		CString strIndex;
		strIndex.Format(_T("%d."),nIndex);
		pIndexLayout->AddWidget(new WIN32LabelWidge(this,(TSTRING)strIndex,Right|VCenter, L"Arial", 9));

		pCtrlLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_EDIT1 + nIndex -1)));
	}
	pIndexLayout->SetCellHeight(21);
	pCtrlLayout->SetCell(295-34,21);
	pIndexLayout->SetSpacing(10);
	pCtrlLayout->SetSpacing(10);

	SplitLayout<HDC>* pListLayout = new SplitLayout<HDC>;
	pListLayout->AddWidget(pIndexLayout,pCtrlLayout);
	pListLayout->SetFixedValue(25);

	SplitLayout<HDC>* pSubclientLayout = new SplitLayout<HDC>(false);
	pSubclientLayout->AddWidget(new WIN32LabelWidge(this, _T("IDS_Message_AnonymousDlg_InsertPhoneNo"), Left|VCenter, L"Arial", 9),pListLayout);
	pSubclientLayout->SetFixedValue(25);
	pSubclientLayout->SetVariouslyMargin(0,0,8,0,true);
	return pSubclientLayout;
}

SplitLayout<HDC>* CAnonymousContactDialog::CreateSubWindowLayout()
{
	SplitLayout<HDC>* pSubWindowLayout = new SplitLayout<HDC>(false);

	WIN32LabelWidge* pSubcaptionText = new WIN32LabelWidge(this, _T("IDS_Message_AnonymousDlg_PhoneNo"), Left, L"Arial", 9);
	pSubcaptionText->SetTextColor(255,255,255,255);
	pSubcaptionText->SetMargin(0,0,10,0);
	SplitLayout<HDC>* pSubclientLayout = CreateSubclientLayout();
	pSubWindowLayout->AddWidget(pSubcaptionText,pSubclientLayout);
	pSubWindowLayout->SetFixedValue(80-44);
	return pSubWindowLayout;
}

BoxLayout<HDC>* CAnonymousContactDialog::CreateButtonLayout()
{
	BoxLayout<HDC>* pButtonLayout = new BoxLayout<HDC>(RightToLeft);
	pButtonLayout->SetCell(-1,-1);
	pButtonLayout->SetSpacing(5);
	TSTRING strLanguageBKImage = _T("MainFrame\\Button\\Btn_Language_Original.png");
	TSTRING strLanguageHighLightImage = _T("MainFrame\\Button\\Btn_Language_MouseOver.png");
	TSTRING strLanguageClickImage = _T("MainFrame\\Button\\Btn_Language_MouseDown.png");
	TSTRING strLanguageDisableImage = _T("MainFrame\\Button\\Btn_Language_Disable.png");
	m_pBtnCancel = new WIN32IconButton(this,_T("IDS_Cancel"),strLanguageBKImage,strLanguageHighLightImage,strLanguageClickImage,strLanguageDisableImage,Cancel);
	m_pBtnOK = new WIN32IconButton(this,_T("IDS_OK"),strLanguageBKImage,strLanguageHighLightImage,strLanguageClickImage,strLanguageDisableImage,OK);
	m_pBtnCancel->SetWidthHeightByImage();
	m_pBtnCancel->SetTextColor(255,255,255,255,255,15,85,105);
	m_pBtnCancel->SetFontSize(9);
	m_pBtnOK->SetWidthHeightByImage();
	m_pBtnOK->SetTextColor(255,255,255,255,255,15,85,105);
	m_pBtnOK->SetFontSize(9);

	pButtonLayout->AddWidget(m_pBtnCancel);
	pButtonLayout->AddWidget(m_pBtnOK);

	return pButtonLayout;
}





void CAnonymousContactDialog::SetExtractStrings(CString& strContact1, CString& strContact2, CString& strContact3, CString& strContact4, CString& strContact5) {
    m_pStrContact1 = &strContact1;
    m_pStrContact2 = &strContact2;
    m_pStrContact3 = &strContact3;
    m_pStrContact4 = &strContact4;
    m_pStrContact5 = &strContact5;
}

// CAnonymousContactDialog message handlers

void CAnonymousContactDialog::OnBnClickedOk()
{
    // TODO: Add your control notification handler code here
    if(m_pStrContact1 != NULL && m_pStrContact2 != NULL && m_pStrContact3 != NULL && m_pStrContact4 != NULL && m_pStrContact5 != NULL) {
        ((CEdit*)GetDlgItem(IDC_CONTACT_EDIT1))->GetWindowText(*m_pStrContact1);
        ((CEdit*)GetDlgItem(IDC_CONTACT_EDIT2))->GetWindowText(*m_pStrContact2);
        ((CEdit*)GetDlgItem(IDC_CONTACT_EDIT3))->GetWindowText(*m_pStrContact3);
        ((CEdit*)GetDlgItem(IDC_CONTACT_EDIT4))->GetWindowText(*m_pStrContact4);
        ((CEdit*)GetDlgItem(IDC_CONTACT_EDIT5))->GetWindowText(*m_pStrContact5);
    }
    EndDialog(0);
}
void CAnonymousContactDialog::WinClose()
{
	g_pThis->FnWinClose();
}

void CAnonymousContactDialog::FnWinClose()
{
	PostMessage(WM_CLOSE);
}
void CAnonymousContactDialog::Cancel()
{
	g_pThis->OnBnClickedCancel();
}

void CAnonymousContactDialog::OnBnClickedCancel()
{
	PostMessage(WM_CLOSE);
}

void CAnonymousContactDialog::OK()
{
	g_pThis->OnBnClickedOk();
}

void CAnonymousContactDialog::OnSizing(UINT fwSide, LPRECT pRect)
{
	//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
	GeometryRect rcClient = GetGeometry();
	CQDialog::SetWindowRegion(pRect, rcClient);
	// TODO: Add your message handler code here
}
