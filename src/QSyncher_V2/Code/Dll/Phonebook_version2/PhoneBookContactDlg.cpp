/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   PhoneBookContactDlg.cpp  $
*  $Revision:   1.54  $
*  $Date:   Mar 21 2008 14:50:46  $
*
***************************************************************************
*
* File Description
* ----------------
*
*
***************************************************************************
*
* Revision Details
* ----------------
*
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhoneBookContactDlg.cpp.-arc  $
   
      Rev 1.54   Mar 21 2008 14:50:46   Jame Tsai
   update
   
      Rev 1.53   Mar 14 2008 09:38:16   Jame Tsai
   update
   
      Rev 1.52   Mar 11 2008 09:06:42   Jame Tsai
   change message box
   
      Rev 1.51   Mar 10 2008 12:33:44   Jame Tsai
   change limitation
   
      Rev 1.50   Feb 27 2008 19:35:54   Mei Yang
   fix warning
   
      Rev 1.49   Feb 27 2008 19:33:00   Mei Yang
   change 30 to strCheckWords.size()
   
      Rev 1.48   Feb 18 2008 09:41:18   Alan Tu
   Add parent to messagebox
   
      Rev 1.47   Feb 04 2008 17:18:36   Mei Yang
   add ExistSameDisplayName and fix bug
   
      Rev 1.46   Feb 04 2008 13:58:42   Mei Yang
   add ExistSameDisplayName
   
      Rev 1.45   Jan 29 2008 11:16:50   Jame Tsai
   fix string error
   
      Rev 1.44   Jan 28 2008 16:35:38   Mei Yang
   fix SetBDAY's SetRange timing
   
      Rev 1.43   Jan 24 2008 15:51:34   Jame Tsai
   Replace new layout
   
      Rev 1.42   Jan 21 2008 15:39:04   Eddy Guo
   update
   
      Rev 1.41   Jan 11 2008 11:19:12   Jame Tsai
   Set Dlg color
   
      Rev 1.40   Jan 09 2008 16:05:56   Jame Tsai
   change font size
   
      Rev 1.39   Jan 09 2008 09:35:50   Mei Yang
   fix bug 
   
      Rev 1.38   Jan 08 2008 15:53:02   Mei Yang
   add PhoneCheckingWords from Configuration
   
      Rev 1.37   Jan 08 2008 15:35:36   Mei Yang
   add Phone Keyin Limitation
   
      Rev 1.36   Jan 07 2008 16:17:06   Mei Yang
   add "Can't be all empty" Limitation
   
      Rev 1.34   Jan 04 2008 13:17:22   Mei Yang
   add m_bIsBDAYSet
   
      Rev 1.33   Jan 02 2008 14:52:52   Jame Tsai
   update caption text
   
      Rev 1.32   Jan 02 2008 13:39:02   Jame Tsai
   change CDialog to CQDialog
   
      Rev 1.31   Jan 02 2008 11:35:06   Alan Tu
   Fix crash bug of without group setting
   
      Rev 1.30   Dec 29 2007 17:04:06   Jame Tsai
   change buttons
   
      Rev 1.29   Dec 28 2007 22:24:16   Jame Tsai
   Update
   
      Rev 1.28   Dec 28 2007 21:44:46   Jame Tsai
   set type
   
      Rev 1.27   Dec 27 2007 21:37:24   Jame Tsai
   set type
   
      Rev 1.26   Dec 26 2007 20:11:54   Mei Yang
   add None into Group
   
      Rev 1.25   Dec 26 2007 20:00:04   Mei Yang
   add BDAY Range Limitation
   
      Rev 1.24   Dec 26 2007 19:06:42   Mei Yang
   add SetCallerID and SetBDAY
   
      Rev 1.23   Dec 26 2007 17:41:00   Mei Yang
   add Name UI Limitation
   
      Rev 1.22   Dec 26 2007 17:03:50   Mei Yang
   add one more field
   
      Rev 1.21   Dec 25 2007 21:53:42   Mei Yang
   add RestrictWords
   
      Rev 1.20   Dec 25 2007 14:13:56   Jame Tsai
   change background
   
      Rev 1.19   Dec 20 2007 17:27:28   Mei Yang
   add UI Limitation
   
      Rev 1.18   Dec 18 2007 14:24:38   Mei Yang
   fix error
   
      Rev 1.17   Dec 17 2007 15:22:40   Mei Yang
   update changing CallerID
   
      Rev 1.16   Dec 17 2007 15:12:40   Mei Yang
   show CallerID ok
   
      Rev 1.15   Dec 10 2007 10:17:42   Mei Yang
   remove save type
   
      Rev 1.14   Nov 30 2007 20:15:00   Alan Tu
   keep PIMDB policy
   
      Rev 1.13   Nov 29 2007 13:58:26   Alan Tu
   add return value
   
      Rev 1.12   Nov 28 2007 11:33:40   Alan Tu
   parser qpcode
   
      Rev 1.11   Nov 13 2007 10:05:50   Alan Tu
   update UI
*
*
***************************************************************************/
#include "stdafx.h"
#include "PhoneBookContactDlg.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"

// CPhoneBookContactDlg dialog

namespace QSYNC_LIB
{

	IMPLEMENT_DYNAMIC(CPhoneBookContactDlg, Win32DoubleBufferQDialogWidge)

		CPhoneBookContactDlg* g_pThis = NULL;

		CPhoneBookContactDlg::CPhoneBookContactDlg(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(IDD_PHONEBOOK_CONTACT, pParent), m_pILocale(NULL), m_FontSize(8), m_bIsBDAYSet(false)
		,m_pTabGeneral_Personal(NULL),m_pTabPrivate(NULL),m_pTabBusiness(NULL),m_pCurrentTabBtn(NULL),m_bIsOfficialFN(false)
	{
		CreateILocalization(&m_pILocale);
		g_pThis = this;
	}

	CPhoneBookContactDlg::~CPhoneBookContactDlg()
	{
	}

	void CPhoneBookContactDlg::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_CONTACT_PRIVATE_NUMBER, m_QEditCtrlPrivatePhone);
		DDX_Control(pDX, IDC_CONTACT_BUSINESS_NUMBER, m_QEditCtrlBusinessPhone);
		DDX_Control(pDX, IDC_CONTACT_PRIVATE_CELL, m_QEditCtrlPrivateCellPhone);
		DDX_Control(pDX, IDC_CONTACT_BUSINESS_CELL, m_QEditCtrlBusinessCellPhone);
		DDX_Control(pDX, IDC_CONTACT_PRIVATE_FAX, m_QEditCtrlPrivateFax);
		DDX_Control(pDX, IDC_CONTACT_BUSINESS_FAX, m_QEditCtrlBusinessFax);
	}

	BEGIN_MESSAGE_MAP(CPhoneBookContactDlg, CQDialog)
		//ON_BN_CLICKED(IDCANCEL, &CPhoneBookContactDlg::OnCancel)
		ON_EN_CHANGE(IDC_CONTACT_PRIVATE_NUMBER, &CQEditCtrl::OnEnChange)
		ON_EN_CHANGE(IDC_CONTACT_BUSINESS_NUMBER, &CQEditCtrl::OnEnChange)
		ON_EN_CHANGE(IDC_CONTACT_PRIVATE_CELL, &CQEditCtrl::OnEnChange)
		ON_EN_CHANGE(IDC_CONTACT_BUSINESS_CELL, &CQEditCtrl::OnEnChange)
		ON_EN_CHANGE(IDC_CONTACT_PRIVATE_FAX, &CQEditCtrl::OnEnChange)
		ON_EN_CHANGE(IDC_CONTACT_BUSINESS_FAX, &CQEditCtrl::OnEnChange)
		ON_NOTIFY(DTN_DATETIMECHANGE, IDC_CONTACT_DATETIMEPICKER_BDAY, &CPhoneBookContactDlg::OnDtnChangeContactBday)
		ON_WM_SIZING()
		//ON_BN_CLICKED(IDOK, &CPhoneBookContactDlg::OnBnClickedOk)
	END_MESSAGE_MAP()
	std::wstring CPhoneBookContactDlg::GetWString(const TSTRING& strID)
	{
		std::wstring wstrRes = L"";
		TSTRING strText = m_pILocale->GetString(strID).c_str();
#ifdef _UNICODE
		wstrRes = strText;
#else
		wstrRes = CharToWideChar( wstrRes, CP_UTF8 );
#endif
		return wstrRes;
	}
	BOOL CPhoneBookContactDlg::OnInitDialog()
	{
		CQDialog::OnInitDialog();
		BOOL bRes = TRUE;
		SetUI();
		m_pTabGeneral_Personal->SetButtonCheck(true);
		m_pCurrentTabBtn = m_pTabGeneral_Personal;

		SetUILimitation();
		SetPhoneCheckingWords();
		
		CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_CONTACT_DATETIMEPICKER_BDAY);
		pDTCtrl->SetFormat(_T("'----/--/--'"));
		pDTCtrl->SetMonthCalColor(MCSC_TITLEBK,RGB(20,80,100));
		m_bIsBDAYSet = false; 

		bRes = SetEditBox();	
		return bRes; 
	}



	void CPhoneBookContactDlg::SetUI()
	{
		SetBackground();

		SplitLayout<HDC>* pDlgLayout = new SplitLayout<HDC>(false);
		pDlgLayout->AddWidget(CreateCaptionLayout(),0,true);
		pDlgLayout->AddWidget(CreateCLientLayout(),0,false);

		pDlgLayout->SetFixedValue(29);
		pDlgLayout->SetSpacing(2);

		SetLayout(pDlgLayout);
	}

	SplitLayout<HDC>* CPhoneBookContactDlg::CreateCaptionLayout()
	{
		SplitLayout<HDC>* pCaptionLayout = new SplitLayout<HDC>;
		WIN32LabelWidge* pCaptionText = new WIN32LabelWidge(this,_T("IDS_PHONEBOOK_FUNCTION"),Left|VCenter,L"Arial",9);
		pCaptionText->SetMargin(0,0,13,0);
		pCaptionText->SetTextColor(255,255,255,255);
		pCaptionLayout->AddWidget(pCaptionText,Left,true);

		TSTRING strWinCloseOriginal = _T("General\\Button\\Btn_Close_Original.png");
		TSTRING strWinCloseMouseOver = _T("General\\Button\\Btn_Close_MouseOver.png");
		TSTRING strWinCloseMouseDown = _T("General\\Button\\Btn_Close_MouseDown.png");
		TSTRING strWinCloseDisable = _T("General\\Button\\Btn_Close_Disable.png");
		WIN32IconButton* pWinCloseButton = new WIN32IconButton(
			this
			,L""
			,strWinCloseOriginal
			,strWinCloseMouseOver
			,strWinCloseMouseDown
			,strWinCloseDisable
			,WinClose
			);
		pWinCloseButton->SetWidthHeightByImage();
		pCaptionLayout->AddWidget(pWinCloseButton,Right,false);
		pCaptionLayout->SetVariouslyMargin(3,1,0,15,false);
		return pCaptionLayout;
	}

	SplitLayout<HDC>* CPhoneBookContactDlg::CreateCLientLayout()
	{
		SplitLayout<HDC>* pClientLayout = new SplitLayout<HDC>(false);
		
		SplitLayout<HDC>* pTextAreaLayout = CreateTextAreaLayout();
		pClientLayout->AddWidget(pTextAreaLayout,Left,true);
		pClientLayout->SetVariouslyMargin(11,2,8,10,true);

		BoxLayout<HDC>* pButtonLayout = CreateButtonLayout();

		pClientLayout->AddWidget(pButtonLayout,Top,false);
		pClientLayout->SetVariouslyMargin(8,2,0,9,false);

		pClientLayout->SetFixedValue(341);
		return pClientLayout;
	}
	SplitLayout<HDC>* CPhoneBookContactDlg::CreateTextAreaLayout()
	{
		m_pIuputStackLayout = new StackLayout<int,HDC>;
		SplitLayout<HDC>* pG_PInfoLayout = new SplitLayout<HDC>;
		pG_PInfoLayout->AddWidget(CreateGerenalInfoLayout(),CreatePersonalInfoLayout());
		m_pIuputStackLayout->AddWidget(General_Personal_Ifo,pG_PInfoLayout);
		m_pIuputStackLayout->AddWidget(Private_Ifo,CreatePrivateInfoLayout());
		m_pIuputStackLayout->AddWidget(Business_Ifo,CreateBusinessInfoLayout());
		m_pIuputStackLayout->Switch(General_Personal_Ifo);

		SplitLayout<HDC>* pTextAreaLayout = new SplitLayout<HDC>(false);
		pTextAreaLayout->AddWidget(CreateTabLayout(),m_pIuputStackLayout,Top|Left);
		pTextAreaLayout->SetFixedValue(40);
		return pTextAreaLayout;
	}
	SplitLayout<HDC>* CPhoneBookContactDlg::CreateGerenalInfoLayout()
	{
		SplitLayout<HDC>* pGeneralInfoLayout = new SplitLayout<HDC>(false);
		WIN32LabelWidge* pCaption = new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_GeneralInfo")), Left|VCenter, L"Arial", m_FontSize);
		pCaption->SetFontBold(true);
		pGeneralInfoLayout->AddWidget(pCaption);
		pGeneralInfoLayout->SetFixedValue(31);
		pGeneralInfoLayout->SetSpacing(2);
		pGeneralInfoLayout->SetVariouslyMargin(0,0,11,0,true);
		pGeneralInfoLayout->SetVariouslyMargin(10,0,11,14,false);

		SplitLayout<HDC>* pSubClientLayout = new SplitLayout<HDC>;

		BoxLayout<HDC>* pLeftText = new BoxLayout<HDC>(TopToBottom);
		BoxLayout<HDC>* pRightInput = new BoxLayout<HDC>(TopToBottom);

		pLeftText->SetCellHeight(20);
		pRightInput->SetCellHeight(20);
		pLeftText->SetSpacing(10);
		pRightInput->SetSpacing(10);

		//General Info

		SplitLayout<HDC>* pContactLayout = new SplitLayout<HDC>;
		pContactLayout->AddWidget(
			new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_FIRSTNAME))
			,new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_LASTNAME))
			);
		pContactLayout->SetSpacing(3);

		pLeftText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_NAME")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(pContactLayout);

		pLeftText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_PRIVATENUMBER")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATE_NUMBER)));
		pLeftText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_BUSINESSNUMBER")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_NUMBER)));
		pLeftText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_PRIVAECELLPHONE")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATE_CELL)));
		pLeftText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_BUSINESSCELLPHONE")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_CELL)));

		pSubClientLayout->AddWidget(pLeftText,pRightInput,0,0);
		pSubClientLayout->SetFixedValue(233,false);
		pGeneralInfoLayout->AddWidget(pSubClientLayout,0,false);
		return pGeneralInfoLayout;
	}
	SplitLayout<HDC>* CPhoneBookContactDlg::CreatePersonalInfoLayout()
	{
		SplitLayout<HDC>* pPersonalInfoLayout = new SplitLayout<HDC>(false);

		WIN32LabelWidge* pCaption = new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_PERSONALINFO")), Left, L"Arial", m_FontSize);
		pCaption->SetFontBold(true);
		pPersonalInfoLayout->AddWidget(pCaption);
		pPersonalInfoLayout->SetFixedValue(31);
		pPersonalInfoLayout->SetSpacing(2);
		pPersonalInfoLayout->SetVariouslyMargin(0,0,17,0,true);
		pPersonalInfoLayout->SetVariouslyMargin(10,0,17,9,false);

		SplitLayout<HDC>* pSubClientLayout = new SplitLayout<HDC>;
		BoxLayout<HDC>* pLeftText = new BoxLayout<HDC>(TopToBottom);
		BoxLayout<HDC>* pRightInput = new BoxLayout<HDC>(TopToBottom);

		pLeftText->SetCellHeight(20);
		pRightInput->SetCellHeight(20);
		pLeftText->SetSpacing(10);
		pRightInput->SetSpacing(10);

		//Add widgets
		pLeftText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_GROUP")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_COMBO_GROUP)));
		pLeftText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_BDAY")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_DATETIMEPICKER_BDAY)));
		pLeftText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_DISPLAY")), Left, L"Arial", m_FontSize));
		pRightInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_DISPLAY)));
		//End Add widgets

		pSubClientLayout->AddWidget(pLeftText,pRightInput,0,0);
		pSubClientLayout->SetFixedValue(233,false);
		pPersonalInfoLayout->AddWidget(pSubClientLayout,0,false);

		return pPersonalInfoLayout;
	}



	SplitLayout<HDC>* CPhoneBookContactDlg::CreateBusinessInfoLayout()
	{

		//Create Left Info area

		SplitLayout<HDC>* pLeftInfoLayout = new SplitLayout<HDC>(false);
		WIN32LabelWidge* pInfoCaption = new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_INFO")), Left|VCenter, L"Arial", m_FontSize);
		pInfoCaption->SetFontBold(true);
		pLeftInfoLayout->AddWidget(pInfoCaption);
		pLeftInfoLayout->SetFixedValue(31);
		pLeftInfoLayout->SetSpacing(2);
		pLeftInfoLayout->SetVariouslyMargin(0,0,11,0,true);
		pLeftInfoLayout->SetVariouslyMargin(10,0,11,14,false);

		SplitLayout<HDC>* pInfoClientLayout = new SplitLayout<HDC>;
		BoxLayout<HDC>* pInfoText = new BoxLayout<HDC>(TopToBottom);
		BoxLayout<HDC>* pInfoInput = new BoxLayout<HDC>(TopToBottom);

		pInfoText->SetCellHeight(20);
		pInfoInput->SetCellHeight(20);
		pInfoText->SetSpacing(10);
		pInfoInput->SetSpacing(10);

		//Add Info widgets
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_EMAIL")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_EMAIL)));
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_EMAIL2")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_EMAIL2)));
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_URL")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_URL)));
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_FAX")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_FAX)));
		pInfoText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_COMPANY")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_COMPANY)));
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_TITLE")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_TITLE)));
		pInfoText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_NOTE")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESS_NOTE)),-1,723-665);
		//End Add Info widgets
		pInfoClientLayout->AddWidget(pInfoText,pInfoInput,0,0);
		pInfoClientLayout->SetFixedValue(233,false);
		pLeftInfoLayout->AddWidget(pInfoClientLayout,0,false);

		//End Create Left Info area


		//Create Right Address area

		SplitLayout<HDC>* pRightAddressLayout = new SplitLayout<HDC>(false);

		WIN32LabelWidge* pAddressCaption = new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_ADDRESS")), Left, L"Arial", m_FontSize);
		pAddressCaption->SetFontBold(true);
		pRightAddressLayout->AddWidget(pAddressCaption);
		pRightAddressLayout->SetFixedValue(31);
		pRightAddressLayout->SetSpacing(2);
		pRightAddressLayout->SetVariouslyMargin(0,0,17,0,true);
		pRightAddressLayout->SetVariouslyMargin(10,0,17,9,false);

		SplitLayout<HDC>* pAddressClientLayout = new SplitLayout<HDC>;
		BoxLayout<HDC>* pAddressText = new BoxLayout<HDC>(TopToBottom);
		BoxLayout<HDC>* pAddressInput = new BoxLayout<HDC>(TopToBottom);

		pAddressText->SetCellHeight(20);
		pAddressInput->SetCellHeight(20);
		pAddressText->SetSpacing(10);
		pAddressInput->SetSpacing(10);

		//Add Address widgets
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_STREET")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_STREET)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_CITY")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_CITY)));
		pAddressText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_POSTALCODE")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_POSTALCODE)));
		pAddressText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_STATE")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_STATE)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_COUNTRY")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_COUNTRY)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_EXTRA")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_EXTRA)));
		//End Add Address widgets
		pAddressClientLayout->AddWidget(pAddressText,pAddressInput,0,0);
		pAddressClientLayout->SetFixedValue(233,false);
		pRightAddressLayout->AddWidget(pAddressClientLayout,0,false);

		//End Create Right Address area


		SplitLayout<HDC>* pBusinessInfoLayout = new SplitLayout<HDC>;
		pBusinessInfoLayout->AddWidget(pLeftInfoLayout,pRightAddressLayout);
		//pBusinessInfoLayout->Show(false);
		return pBusinessInfoLayout;
	}

	SplitLayout<HDC>* CPhoneBookContactDlg::CreatePrivateInfoLayout()
	{

	//Create Left Info area

		SplitLayout<HDC>* pLeftInfoLayout = new SplitLayout<HDC>(false);
		WIN32LabelWidge* pInfoCaption = new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_INFO")), Left|VCenter, L"Arial", m_FontSize);
		pInfoCaption->SetFontBold(true);
		pLeftInfoLayout->AddWidget(pInfoCaption);
		pLeftInfoLayout->SetFixedValue(31);
		pLeftInfoLayout->SetSpacing(2);
		pLeftInfoLayout->SetVariouslyMargin(0,0,11,0,true);
		pLeftInfoLayout->SetVariouslyMargin(10,0,11,14,false);

		SplitLayout<HDC>* pInfoClientLayout = new SplitLayout<HDC>;
		BoxLayout<HDC>* pInfoText = new BoxLayout<HDC>(TopToBottom);
		BoxLayout<HDC>* pInfoInput = new BoxLayout<HDC>(TopToBottom);

		pInfoText->SetCellHeight(20);
		pInfoInput->SetCellHeight(20);
		pInfoText->SetSpacing(10);
		pInfoInput->SetSpacing(10);

		//Add Info widgets
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_EMAIL")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATE_EMAIL)));
		pInfoText->AddWidget(new WIN32LabelWidge(this,GetWString(_T("IDS_PHONEBOOK_EMAIL2")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATE_EMAIL2)));
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_URL")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATE_URL)));
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_FAX")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATE_FAX)));
		pInfoText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_NOTE")), Left, L"Arial", m_FontSize));
		pInfoInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATE_NOTE)),-1,63-5);
		//End Add Info widgets
		pInfoClientLayout->AddWidget(pInfoText,pInfoInput,0,0);
		pInfoClientLayout->SetFixedValue(233,false);
		pLeftInfoLayout->AddWidget(pInfoClientLayout,0,false);

	//End Create Left Info area


	//Create Right Address area

		SplitLayout<HDC>* pRightAddressLayout = new SplitLayout<HDC>(false);

		WIN32LabelWidge* pAddressCaption = new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_ADDRESS")), Left, L"Arial", m_FontSize);
		pAddressCaption->SetFontBold(true);
		pRightAddressLayout->AddWidget(pAddressCaption);
		pRightAddressLayout->SetFixedValue(31);
		pRightAddressLayout->SetSpacing(2);
		pRightAddressLayout->SetVariouslyMargin(0,0,17,0,true);
		pRightAddressLayout->SetVariouslyMargin(10,0,17,9,false);

		SplitLayout<HDC>* pAddressClientLayout = new SplitLayout<HDC>;
		BoxLayout<HDC>* pAddressText = new BoxLayout<HDC>(TopToBottom);
		BoxLayout<HDC>* pAddressInput = new BoxLayout<HDC>(TopToBottom);

		pAddressText->SetCellHeight(20);
		pAddressInput->SetCellHeight(20);
		pAddressText->SetSpacing(10);
		pAddressInput->SetSpacing(10);

		//Add Address widgets
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_STREET")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_STREET)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_CITY")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_CITY)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_POSTALCODE")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_POSTALCODE)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_STATE")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_STATE)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_COUNTRY")), Left, L"Arial", m_FontSize));
		pAddressInput->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_COUNTRY)));
		pAddressText->AddWidget(new WIN32LabelWidge(this, GetWString(_T("IDS_PHONEBOOK_EXTRA")), Left, L"Arial", m_FontSize));
		WIN32WindowWidge* pEXTRA = new WIN32WindowWidge(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_EXTRA));
		pAddressInput->AddWidget(pEXTRA);
		//End Add Address widgets
		pAddressClientLayout->AddWidget(pAddressText,pAddressInput,0,0);
		pAddressClientLayout->SetFixedValue(233,false);
		pRightAddressLayout->AddWidget(pAddressClientLayout,0,false);

	//End Create Right Address area


		SplitLayout<HDC>* pBusinessInfoLayout = new SplitLayout<HDC>;
		pBusinessInfoLayout->AddWidget(pLeftInfoLayout,pRightAddressLayout);
		//pBusinessInfoLayout->Show(false);
		return pBusinessInfoLayout;
	}
	BoxLayout<HDC>* CPhoneBookContactDlg::CreateTabLayout()
	{
		TSTRING strTabOriginal = _T("PhoneBook\\Dlg\\Btn_ContactEditor_Original.png");
		TSTRING strTabMouseOver = _T("PhoneBook\\Dlg\\Btn_ContactEditor_MouseOver.png");
		TSTRING strTabMouseDown = _T("PhoneBook\\Dlg\\Btn_ContactEditor_MouseDown.png");
		TSTRING strTabDisable = _T("PhoneBook\\Dlg\\Btn_ContactEditor_Disable.png");
		TSTRING strTabOriginal2 = _T("PhoneBook\\Dlg\\Btn_ContactEditor_Original_2.png");
		TSTRING strTabMouseOver2 = _T("PhoneBook\\Dlg\\Btn_ContactEditor_MouseOver_2.png");
		TSTRING strTabMouseDown2 = _T("PhoneBook\\Dlg\\Btn_ContactEditor_MouseDown_2.png");
		TSTRING strTabDisable2 = _T("PhoneBook\\Dlg\\Btn_ContactEditor_Disable_2.png");
		const int nTabFontSize = 9;

		TSTRING strGeneral;
		m_pILocale->GetString(_T("IDS_PHONEBOOK_GeneralInfo"),strGeneral);
		TSTRING strPersonal;
		m_pILocale->GetString(_T("IDS_PHONEBOOK_PERSONALINFO"),strPersonal);
		m_pTabGeneral_Personal = new WIN32IconButton(
			this
			,strGeneral + _T("/") + strPersonal
			,strTabOriginal
			,strTabMouseOver
			,strTabMouseDown
			,strTabDisable
			,Tab_GPInfo
			);
		m_pTabGeneral_Personal->BeToggleButten();
		m_pTabGeneral_Personal->SetWidthHeightByImage();
		m_pTabGeneral_Personal->SetFourImagesOfButtonDown(strTabOriginal2,strTabMouseOver2,strTabMouseDown2,strTabDisable2);
		m_pTabGeneral_Personal->SetFontSize(nTabFontSize);
		m_pTabGeneral_Personal->SetTextColor(255,255,255,255,255, 15, 85, 105);

		m_pTabPrivate = new WIN32IconButton(
			this
			,L"IDS_PHONEBOOK_PRIVATEINFO"
			,strTabOriginal
			,strTabMouseOver
			,strTabMouseDown
			,strTabDisable
			,Tab_PrivateInfo
			);
		m_pTabPrivate->BeToggleButten();
		m_pTabPrivate->SetWidthHeightByImage();
		m_pTabPrivate->SetFourImagesOfButtonDown(strTabOriginal2,strTabMouseOver2,strTabMouseDown2,strTabDisable2);
		m_pTabPrivate->SetFontSize(nTabFontSize);
		m_pTabPrivate->SetTextColor(255,255,255,255,255, 15, 85, 105);

		m_pTabBusiness = new WIN32IconButton(
			this
			,L"IDS_PHONEBOOK_BUSINESSINFO"
			,strTabOriginal
			,strTabMouseOver
			,strTabMouseDown
			,strTabDisable
			,Tab_BusinessInfo
			);
		m_pTabBusiness->BeToggleButten();
		m_pTabBusiness->SetWidthHeightByImage();
		m_pTabBusiness->SetFourImagesOfButtonDown(strTabOriginal2,strTabMouseOver2,strTabMouseDown2,strTabDisable2);
		m_pTabBusiness->SetFontSize(nTabFontSize);
		m_pTabBusiness->SetTextColor(255,255,255,255,255, 15, 85, 105);

		BoxLayout<HDC>* pTabLayout = new BoxLayout<HDC>(LeftToRight);
		pTabLayout->AddWidget(m_pTabGeneral_Personal);
		pTabLayout->AddWidget(m_pTabPrivate);
		pTabLayout->AddWidget(m_pTabBusiness);
		pTabLayout->SetSpacing(1);

		return pTabLayout;
	}

	BoxLayout<HDC>* CPhoneBookContactDlg::CreateButtonLayout()
	{
		BoxLayout<HDC>* pButtonLayout = new BoxLayout<HDC>(RightToLeft);
		pButtonLayout->SetCell(-1,-1);
		//pButtonLayout->SetMargin(5,0,0,10);
		pButtonLayout->SetSpacing(5);
		//pButtonLayout->SetAlignment(VCenter);
		TSTRING strLanguageBKImage = _T("MainFrame\\Button\\Btn_Language_Original.png");
		TSTRING strLanguageHighLightImage = _T("MainFrame\\Button\\Btn_Language_MouseOver.png");
		TSTRING strLanguageClickImage = _T("MainFrame\\Button\\Btn_Language_MouseDown.png");
		TSTRING strLanguageDisableImage = _T("MainFrame\\Button\\Btn_Language_Disable.png");
		WIN32IconButton* pBtnCancel = new WIN32IconButton(this,_T("IDS_Cancel"),strLanguageBKImage,strLanguageHighLightImage,strLanguageClickImage,strLanguageDisableImage,Cancel);
		WIN32IconButton* pBtnOK = new WIN32IconButton(this,_T("IDS_OK"),strLanguageBKImage,strLanguageHighLightImage,strLanguageClickImage,strLanguageDisableImage,OK);
		pBtnCancel->SetWidthHeightByImage();
		pBtnCancel->SetTextColor(255,255,255,255,255,15,85,105);
		pBtnCancel->SetFontSize(9);
		pBtnOK->SetWidthHeightByImage();
		pBtnOK->SetTextColor(255,255,255,255,255,15,85,105);
		pBtnOK->SetFontSize(9);

		//pButtonLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDCANCEL)));
		//pButtonLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDOK)));
		pButtonLayout->AddWidget(pBtnCancel);
		pButtonLayout->AddWidget(pBtnOK);

		return pButtonLayout;
	}

	int CPhoneBookContactDlg::GetHeightDifferenceOfWindowAndClient()
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		return rcWindow.Height() - rcClient.Height();
	}
	
	int CPhoneBookContactDlg::GetWidthDifferenceOfWindowAndClient()
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcWindow;
		GetWindowRect(&rcWindow);
		return rcWindow.Width() - rcClient.Width();
	}

	void CPhoneBookContactDlg::SetUILimitation()
	{
		IPIMObjPt PIMObjPt; // Empty PIMObj (cause we won't use it)
		CVCardCtrl VCardCtrl(PIMObjPt);

		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_FIRSTNAME)), &VCardCtrl, _T(KEYWORD_FIRSTNAME));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_LASTNAME)), &VCardCtrl, _T(KEYWORD_LASTNAME));

		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATE_NUMBER)), &VCardCtrl, _T(KEYWORD_TEL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_NUMBER)), &VCardCtrl, _T(KEYWORD_TEL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATE_CELL)), &VCardCtrl, _T(KEYWORD_TEL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_CELL)), &VCardCtrl, _T(KEYWORD_TEL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATE_FAX)), &VCardCtrl, _T(KEYWORD_TEL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_FAX)), &VCardCtrl, _T(KEYWORD_TEL));

		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATE_EMAIL)), &VCardCtrl, _T(KEYWORD_EMAIL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATE_EMAIL2)), &VCardCtrl, _T(KEYWORD_EMAIL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_EMAIL)), &VCardCtrl, _T(KEYWORD_EMAIL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_EMAIL2)), &VCardCtrl, _T(KEYWORD_EMAIL));

		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATE_URL)), &VCardCtrl, _T(KEYWORD_URL));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_URL)), &VCardCtrl, _T(KEYWORD_URL));
		
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATE_NOTE)), &VCardCtrl, _T(KEYWORD_NOTE));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_NOTE)), &VCardCtrl, _T(KEYWORD_NOTE));

		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_DISPLAY)), &VCardCtrl, _T(KEYWORD_DISPLAY));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_COMPANY)), &VCardCtrl, _T(KEYWORD_COMPANY));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESS_TITLE)), &VCardCtrl, _T(KEYWORD_TITLE));

		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_STREET)), &VCardCtrl, _T("VCARD:ADR:2"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_CITY)), &VCardCtrl, _T("VCARD:ADR:3"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_POSTALCODE)), &VCardCtrl, _T("VCARD:ADR:5"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_STATE)), &VCardCtrl, _T("VCARD:ADR:4"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_COUNTRY)), &VCardCtrl, _T("VCARD:ADR:6"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_PRIVATEADDRESS_EXTRA)), &VCardCtrl, _T("VCARD:ADR:1"));
		
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_STREET)), &VCardCtrl, _T("VCARD:ADR:2"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_CITY)), &VCardCtrl, _T("VCARD:ADR:3"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_POSTALCODE)), &VCardCtrl, _T("VCARD:ADR:5"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_STATE)), &VCardCtrl, _T("VCARD:ADR:4"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_COUNTRY)), &VCardCtrl, _T("VCARD:ADR:6"));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_CONTACT_BUSINESSADDRESS_EXTRA)), &VCardCtrl, _T("VCARD:ADR:1"));
	}

	void CPhoneBookContactDlg::SetLimit(CQEditCtrl* pEditCtrl,  CVCardCtrl* pVCardCtrl, const TSTRING& strKey)
	{
		int nLimit = pVCardCtrl->GetLimitation(strKey);
		if(nLimit != -1)
			pEditCtrl->SetTextLength(nLimit);
	}

	void CPhoneBookContactDlg::SetPhoneCheckingWords()
	{
		IPIMObjPt PIMObjPt; // Empty PIMObj (cause we won't use it)
		CVCardCtrl VCardCtrl(PIMObjPt);
		TSTRING strCheckWords;
		VCardCtrl.GetLimitation(_T("PhoneLimitWords"), strCheckWords);
		if(strCheckWords.empty())
			strCheckWords = _T("0123456789P*#+");

		m_QEditCtrlPrivatePhone.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
		m_QEditCtrlBusinessPhone.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
		m_QEditCtrlPrivateCellPhone.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
		m_QEditCtrlBusinessCellPhone.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
		m_QEditCtrlPrivateFax.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
		m_QEditCtrlBusinessFax.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
	}
	BOOL CPhoneBookContactDlg::SetEditBox()
	{
		BOOL bRes = TRUE;
		if(m_PIMObjPt)
		{
			SetOrgText(IDC_CONTACT_FIRSTNAME, KEYWORD_FIRSTNAME);
			SetOrgText(IDC_CONTACT_SECONDNAME, KEYWORD_LASTNAME);
			SetOrgText(IDC_CONTACT_DISPLAY, KEYWORD_DISPLAY);
			GetDlgItem(IDC_CONTACT_DISPLAY)->GetWindowText(m_cstrOrgDisplayName);

			SetSpecialOrgText(IDC_CONTACT_PRIVATE_NUMBER,	"TEL",_T("HOME;VOICE"),0);
			SetSpecialOrgText(IDC_CONTACT_BUSINESS_NUMBER,	"TEL",_T("WORK;VOICE"),0);
			SetSpecialOrgText(IDC_CONTACT_PRIVATE_CELL,		"TEL",_T("HOME;CELL"),0);
			SetSpecialOrgText(IDC_CONTACT_BUSINESS_CELL,	"TEL",_T("WORK;CELL"),0);
			SetSpecialOrgText(IDC_CONTACT_PRIVATE_FAX,		"TEL",_T("HOME;FAX"),0);
			SetSpecialOrgText(IDC_CONTACT_BUSINESS_FAX,		"TEL",_T("WORK;FAX"),0);
			SetSpecialOrgText(IDC_CONTACT_PRIVATE_EMAIL,	"EMAIL",_T("HOME"),0);
			SetSpecialOrgText(IDC_CONTACT_PRIVATE_EMAIL2,	"EMAIL",_T("HOME"),1);
			SetSpecialOrgText(IDC_CONTACT_BUSINESS_EMAIL,	"EMAIL",_T("WORK"),0);
			SetSpecialOrgText(IDC_CONTACT_BUSINESS_EMAIL2,	"EMAIL",_T("WORK"),1);
			SetSpecialOrgText(IDC_CONTACT_PRIVATE_URL,		"URL",_T("HOME"),0);
			SetSpecialOrgText(IDC_CONTACT_BUSINESS_URL,		"URL",_T("WORK"),0);
			//Private Address
			SetSpecialOrgText(IDC_CONTACT_PRIVATEADDRESS_EXTRA,			"ADR",_T("HOME"),0,1);
			SetSpecialOrgText(IDC_CONTACT_PRIVATEADDRESS_STREET,		"ADR",_T("HOME"),0,2);
			SetSpecialOrgText(IDC_CONTACT_PRIVATEADDRESS_CITY,			"ADR",_T("HOME"),0,3);
			SetSpecialOrgText(IDC_CONTACT_PRIVATEADDRESS_STATE,			"ADR",_T("HOME"),0,4);
			SetSpecialOrgText(IDC_CONTACT_PRIVATEADDRESS_POSTALCODE,	"ADR",_T("HOME"),0,5);
			SetSpecialOrgText(IDC_CONTACT_PRIVATEADDRESS_COUNTRY,		"ADR",_T("HOME"),0,6);
			//Business Address
			SetSpecialOrgText(IDC_CONTACT_BUSINESSADDRESS_EXTRA,		"ADR",_T("WORK"),0,1);
			SetSpecialOrgText(IDC_CONTACT_BUSINESSADDRESS_STREET,		"ADR",_T("WORK"),0,2);
			SetSpecialOrgText(IDC_CONTACT_BUSINESSADDRESS_CITY,			"ADR",_T("WORK"),0,3);
			SetSpecialOrgText(IDC_CONTACT_BUSINESSADDRESS_STATE,		"ADR",_T("WORK"),0,4);
			SetSpecialOrgText(IDC_CONTACT_BUSINESSADDRESS_POSTALCODE,	"ADR",_T("WORK"),0,5);
			SetSpecialOrgText(IDC_CONTACT_BUSINESSADDRESS_COUNTRY,		"ADR",_T("WORK"),0,6);

			SetOrgText(IDC_CONTACT_BUSINESS_COMPANY, KEYWORD_COMPANY);
			SetOrgText(IDC_CONTACT_BUSINESS_TITLE, KEYWORD_TITLE);

			SetSpecialOrgText(IDC_CONTACT_PRIVATE_NOTE,					"NOTE",_T("HOME"),0);
			SetSpecialOrgText(IDC_CONTACT_BUSINESS_NOTE,				"NOTE",_T("WORK"),0);

			SetBDAY();
			SetCallerID();
		}
		m_bIsOfficialFN = GetOfficialDisplayName() == (TSTRING)m_cstrOrgDisplayName;
		for(vector<TSTRING>::iterator iter=m_vDisplayString.begin(); iter!= m_vDisplayString.end(); iter++)
		{
			if((*iter) == (TSTRING)m_cstrOrgDisplayName)
			{			
				m_vDisplayString.erase(iter);
				break;
			}
		}
		return bRes;
	}
	void CPhoneBookContactDlg::SetBDAY()
	{
		TSTRING strBDay;
		CVCardCtrl VCtrl(m_PIMObjPt);
		bool bGet = VCtrl.GetCmnStrData(KEYWORD_BDAY,strBDay);

		TSTRING strLimitation, strFirst, strSecond;
		VCtrl.GetLimitation(_T(KEYWORD_BDAY), strLimitation);
		Split<TSTRING>(_T("-"), strLimitation, strFirst, strSecond);
		COleDateTime oleFirst, oleSecond;
		StrToTime(strFirst.c_str(), oleFirst);
		StrToTime(strSecond.c_str(), oleSecond);
		
		CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_CONTACT_DATETIMEPICKER_BDAY);
		pDTCtrl->SetRange(&oleFirst, &oleSecond);

		if(bGet && !strBDay.empty())
		{
			strBDay += _T("T000000Z");
			if(strBDay != _T("45010101T000000Z"))
			{
				pDTCtrl->SetFormat(_T("yyyy/MM/dd"));		
				COleDateTime oleDate;
				StrToTime(strBDay.c_str(), oleDate);
				pDTCtrl->SetTime(oleDate);
			}
		}
		
	}
	void CPhoneBookContactDlg::SetCallerID()
	{
		CVCardCtrl VCtrl(m_PIMObjPt);
		int nCallerID = VCtrl.GetSpecificInt("X-BENQ", _T("CallerID"));

		if(!m_vectPIMGroup.empty())
		{
			int nIndex = 0;
			int nCurSel = (int)m_vectPIMGroup.size();
			bool bSetCurSel = false;
			for(vector<CPIMGroupInfo>::iterator iter= m_vectPIMGroup.begin(); iter!=m_vectPIMGroup.end(); ++iter)
			{
				TSTRING strDisplay;
				QPDecoder(iter->m_strDisplayText, strDisplay, CP_UTF8);
				((CComboBox*)GetDlgItem(IDC_CONTACT_COMBO_GROUP))->AddString(strDisplay.c_str());
				if(nCallerID == (iter->m_nID))
				{
					nCurSel = nIndex;
				}
				++nIndex;
			}
			((CComboBox*)GetDlgItem(IDC_CONTACT_COMBO_GROUP))->AddString(GetString(_T("IDS_None")).c_str());
			((CComboBox*)GetDlgItem(IDC_CONTACT_COMBO_GROUP))->SetCurSel(nCurSel);
		}
	}
	void CPhoneBookContactDlg::SetSpecialOrgText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount)
	{
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		TSTRING strText;
		strText = VCardCtrl.GetSpecificString(cpKey, strParameter, nPropertyCount, nValueCount);
		GetDlgItem(nID)->SetWindowText(strText.c_str());
	}

	void CPhoneBookContactDlg::SetOrgText(int nID, const char* cpKey)
	{
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		TSTRING strText;
		VCardCtrl.GetCmnStrData(cpKey, strText);
		GetDlgItem(nID)->SetWindowText(strText.c_str());
	}
	/*void CPhoneBookContactDlg::SetOrgText(int nID, const char* cpKey1, const char* cpKey2)
	{
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		TSTRING strText1, strText2;
		VCardCtrl.GetCmnStrData(cpKey1, strText1);
		VCardCtrl.GetCmnStrData(cpKey2, strText2);
		GetDlgItem(nID)->SetWindowText((strText1+_T(" ")+strText2).c_str());
	}*/
	void CPhoneBookContactDlg::SaveDisplayName()
	{
		CString cstrDisplayName;
		GetDlgItem(IDC_CONTACT_DISPLAY)->GetWindowText(cstrDisplayName);
		if(cstrDisplayName.IsEmpty() || (m_bIsOfficialFN && m_cstrOrgDisplayName == cstrDisplayName))
		{
			TSTRING strDisplayName = GetOfficialDisplayName();
			CVCardCtrl VCardCtrl(m_PIMObjPt);
			VCardCtrl.SaveCmnData(KEYWORD_DISPLAY, strDisplayName);
		}
		else
		{
			SaveText(IDC_CONTACT_DISPLAY, KEYWORD_DISPLAY);
		}
	}
	bool CPhoneBookContactDlg::SaveEditBox()
	{
		bool bRes = false;
		CVCardCtrl VCardCtrl(m_PIMObjPt);

		CString cstrName;
		//GetDlgItem(IDC_CONTACT_NAME)->GetWindowText(cstrName);
		SaveText(IDC_CONTACT_LASTNAME,	"N",_T(""),0,0);
		SaveText(IDC_CONTACT_FIRSTNAME,	"N",_T(""),0,1);

		//SaveText(IDC_CONTACT_DISPLAY, KEYWORD_DISPLAY); //Display Name
		SaveDisplayName();

		SaveText(IDC_CONTACT_PRIVATE_NUMBER,	"TEL",_T("HOME;VOICE"),0);
		SaveText(IDC_CONTACT_BUSINESS_NUMBER,	"TEL",_T("WORK;VOICE"),0);
		SaveText(IDC_CONTACT_PRIVATE_CELL,		"TEL",_T("HOME;CELL"),0);
		SaveText(IDC_CONTACT_BUSINESS_CELL,		"TEL",_T("WORK;CELL"),0);
		SaveText(IDC_CONTACT_PRIVATE_FAX,		"TEL",_T("HOME;FAX"),0);
		SaveText(IDC_CONTACT_BUSINESS_FAX,		"TEL",_T("WORK;FAX"),0);
		SaveText(IDC_CONTACT_PRIVATE_EMAIL,		"EMAIL",_T("HOME"),0);  
		SaveText(IDC_CONTACT_PRIVATE_EMAIL2,	"EMAIL",_T("HOME"),1);
		SaveText(IDC_CONTACT_BUSINESS_EMAIL,	"EMAIL",_T("WORK"),0);
		SaveText(IDC_CONTACT_BUSINESS_EMAIL2,	"EMAIL",_T("WORK"),1);
		SaveText(IDC_CONTACT_PRIVATE_URL,		"URL",_T("HOME"),0);
		SaveText(IDC_CONTACT_BUSINESS_URL,		"URL",_T("WORK"),0);

		//Private Address
		SaveText(IDC_CONTACT_PRIVATEADDRESS_EXTRA,			"ADR",_T("HOME"),0,1);
		SaveText(IDC_CONTACT_PRIVATEADDRESS_STREET,			"ADR",_T("HOME"),0,2);
		SaveText(IDC_CONTACT_PRIVATEADDRESS_CITY,			"ADR",_T("HOME"),0,3);
		SaveText(IDC_CONTACT_PRIVATEADDRESS_STATE,			"ADR",_T("HOME"),0,4);
		SaveText(IDC_CONTACT_PRIVATEADDRESS_POSTALCODE,		"ADR",_T("HOME"),0,5);
		SaveText(IDC_CONTACT_PRIVATEADDRESS_COUNTRY,		"ADR",_T("HOME"),0,6);

		//Business Address
		SaveText(IDC_CONTACT_BUSINESSADDRESS_EXTRA,			"ADR",_T("WORK"),0,1);
		SaveText(IDC_CONTACT_BUSINESSADDRESS_STREET,		"ADR",_T("WORK"),0,2);
		SaveText(IDC_CONTACT_BUSINESSADDRESS_CITY,			"ADR",_T("WORK"),0,3);
		SaveText(IDC_CONTACT_BUSINESSADDRESS_STATE,			"ADR",_T("WORK"),0,4);
		SaveText(IDC_CONTACT_BUSINESSADDRESS_POSTALCODE,	"ADR",_T("WORK"),0,5);
		SaveText(IDC_CONTACT_BUSINESSADDRESS_COUNTRY,		"ADR",_T("WORK"),0,6);

		SaveText(IDC_CONTACT_BUSINESS_COMPANY, KEYWORD_COMPANY);
		SaveText(IDC_CONTACT_BUSINESS_TITLE, KEYWORD_TITLE); 

		SaveText(IDC_CONTACT_PRIVATE_NOTE,					"NOTE",_T("HOME"),0);
		SaveText(IDC_CONTACT_BUSINESS_NOTE,					"NOTE",_T("WORK"),0);

		
		if(m_bIsBDAYSet)
		{
			CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_CONTACT_DATETIMEPICKER_BDAY);
			COleDateTime oleDate;
			pDTCtrl->GetTime(oleDate);
			VCardCtrl.SetCmnStrData(KEYWORD_BDAY, oleDate.Format(_T("%Y%m%d")));
		}

		CComboBox* pGroup = ((CComboBox*)GetDlgItem(IDC_CONTACT_COMBO_GROUP));
		int nIndex = pGroup->GetCurSel();
		TSTRING strGroup; 
		if((int)m_vectPIMGroup.size() > nIndex)
		{
			if(iToTStr<TSTRING>(m_vectPIMGroup[nIndex].m_nID,strGroup))
				VCardCtrl.SetSpecificString(strGroup, "X-BENQ", _T("CallerID"), 0,0);
		}
		else
		{//Means "None" , without any group setting
			VCardCtrl.SetSpecificString(_T(""), "X-BENQ", _T("CallerID"), 0,0);
		}

		return bRes;
	}

	void CPhoneBookContactDlg::SetBackground()
	{
		TSTRING strBkImage = L"PhoneBook\\Dlg\\Bk_ContactEditor.png";
		SetBKImage(strBkImage,BK_Fixed);
		int nDeltaWidth = GetWidthDifferenceOfWindowAndClient();
		int nDeltaHeight = GetHeightDifferenceOfWindowAndClient();
		const int nContactDlgWIDTH = GetBKImage()->GetWidth(); 
		const int nContactDlgHEIGHT = GetBKImage()->GetHeight() ;
		SetWindowPos(NULL, 0, 0, nContactDlgWIDTH + nDeltaWidth, nContactDlgHEIGHT + nDeltaHeight, SWP_NOMOVE);
	}

	bool CPhoneBookContactDlg::SaveText(int nID, const char* cpKey)
	{
		bool bRes = false;
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		CString cstrText;
		GetDlgItem(nID)->GetWindowText(cstrText);
		TSTRING strText = cstrText;
		bRes = VCardCtrl.SaveCmnData(cpKey, strText);
		return bRes;
	}

	bool CPhoneBookContactDlg::SaveText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount)
	{
		bool bRes = false;
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		CString cstrText;
		GetDlgItem(nID)->GetWindowText(cstrText);
		TSTRING strText = cstrText;
		bRes = VCardCtrl.SetSpecificString(strText, cpKey, strParameter, nPropertyCount, nValueCount);
		return bRes;
	}

	// PhoneBookContactDlg message handlers

	void CPhoneBookContactDlg::OnDtnChangeContactBday(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
		CDateTimeCtrl* pDTCtrl = (CDateTimeCtrl*) GetDlgItem(IDC_CONTACT_DATETIMEPICKER_BDAY);
		pDTCtrl->SetFormat(_T("yyyy/MM/dd"));
		m_bIsBDAYSet = true;
		*pResult = 0;
	}
	void CPhoneBookContactDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}
	//void CPhoneBookContactDlg::OnBnClickedOk()
	//{
	//	SaveEditBox();
	//	OnOK();
	//}

	void CPhoneBookContactDlg::Tab_GPInfo(){ g_pThis->OnGPInfo(); }
	void CPhoneBookContactDlg::OnGPInfo()
	{
		if(m_pCurrentTabBtn != m_pTabGeneral_Personal)
		{
			if(m_pCurrentTabBtn)
				m_pCurrentTabBtn->SetButtonCheck(false);
			m_pCurrentTabBtn = m_pTabGeneral_Personal;
			m_pCurrentTabBtn->SetButtonCheck(true);
			m_pIuputStackLayout->Switch(General_Personal_Ifo);
			SetLayout();
			Update();
		}
		else
			m_pCurrentTabBtn->SetButtonCheck(true);
	}
	void CPhoneBookContactDlg::Tab_PrivateInfo(){ g_pThis->OnPrivateInfo(); }
	void CPhoneBookContactDlg::OnPrivateInfo()
	{
		if(m_pCurrentTabBtn != m_pTabPrivate)
		{
			if(m_pCurrentTabBtn)
				m_pCurrentTabBtn->SetButtonCheck(false);
			m_pCurrentTabBtn = m_pTabPrivate;
			m_pCurrentTabBtn->SetButtonCheck(true);
			m_pIuputStackLayout->Switch(Private_Ifo);
			SetLayout();
			Update();
		}
		else
			m_pCurrentTabBtn->SetButtonCheck(true);
	}
	void CPhoneBookContactDlg::Tab_BusinessInfo(){ g_pThis->OnBusinessInfo(); }
	void CPhoneBookContactDlg::OnBusinessInfo()
	{
		if(m_pCurrentTabBtn != m_pTabBusiness)
		{
			if(m_pCurrentTabBtn)
				m_pCurrentTabBtn->SetButtonCheck(false);
			m_pCurrentTabBtn = m_pTabBusiness;
			m_pCurrentTabBtn->SetButtonCheck(true);
			m_pIuputStackLayout->Switch(Business_Ifo);
			SetLayout();
			Update();
		}
		else
			m_pCurrentTabBtn->SetButtonCheck(true);
	}
	void CPhoneBookContactDlg::WinClose(){ g_pThis->FnWinClose(); }
	void CPhoneBookContactDlg::FnWinClose()
	{
		PostMessage(WM_CLOSE);
	}
	void CPhoneBookContactDlg::Cancel(){ g_pThis->OnBnClickedCancel(); }
	void CPhoneBookContactDlg::OnBnClickedCancel()
	{
		PostMessage(WM_CLOSE);
	}
	void CPhoneBookContactDlg::OK(){ g_pThis->OnBnClickedOK(); }
	void CPhoneBookContactDlg::OnBnClickedOK()
	{
		if(ExistSameDisplayName())
			ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_MsgSameName"),_T("IDS_PHONEBOOK_FUNCTION"));		
		//else if(IsAllEmpty())
		//	ShowMessage(GetSafeHwnd(),GetString(_T("IDS_PHONEBOOK_MsgAllEmpty")),GetString(_T("IDS_PHONEBOOK_FUNCTION")));
		else if (IsDisplayNameEmpty())
			ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_MSGInputDisplayNameField"),_T("IDS_PHONEBOOK_FUNCTION"));
		else
		{
			SaveEditBox();
			OnOK();
		}
	}
	TSTRING CPhoneBookContactDlg::GetOfficialDisplayName()
	{
		TSTRING strRes;
		CString cstrRes;
		if(!IsFieldEmpty(IDC_CONTACT_LASTNAME) || !IsFieldEmpty(IDC_CONTACT_FIRSTNAME))
		{	
			if(!IsFieldEmpty(IDC_CONTACT_LASTNAME) && !IsFieldEmpty(IDC_CONTACT_FIRSTNAME))
			{
				CString cstrLastName, cstrFirstName;
				GetDlgItem(IDC_CONTACT_LASTNAME)->GetWindowText(cstrLastName);
				GetDlgItem(IDC_CONTACT_FIRSTNAME)->GetWindowText(cstrFirstName);
				strRes = cstrLastName + _T(", ") + cstrFirstName;
			}
			else if(!IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME))
			{
				GetDlgItem(IDC_CONTACT_LASTNAME)->GetWindowText(cstrRes);
				strRes = cstrRes;
			}
			else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && !IsFieldEmpty(IDC_CONTACT_FIRSTNAME))
			{
				GetDlgItem(IDC_CONTACT_FIRSTNAME)->GetWindowText(cstrRes);
				strRes = cstrRes;
			}
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
				!IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY))
		{
			GetDlgItem(IDC_CONTACT_BUSINESS_COMPANY)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
				IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && !IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER))
		{
			GetDlgItem(IDC_CONTACT_PRIVATE_NUMBER)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) &&
			!IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER))
		{
			GetDlgItem(IDC_CONTACT_BUSINESS_NUMBER)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) &&
			IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && !IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL))
		{
			GetDlgItem(IDC_CONTACT_PRIVATE_CELL)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) &&
			IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL) && 
			!IsFieldEmpty(IDC_CONTACT_BUSINESS_CELL))
		{
			GetDlgItem(IDC_CONTACT_BUSINESS_CELL)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) &&
			IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_CELL) && !IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL))
		{
			GetDlgItem(IDC_CONTACT_PRIVATE_EMAIL)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) &&
			IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_CELL) && IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL) && 
			!IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL2))
		{
			GetDlgItem(IDC_CONTACT_PRIVATE_EMAIL2)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) &&
			IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_CELL) && IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL) && 
			IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL2) && !IsFieldEmpty(IDC_CONTACT_BUSINESS_EMAIL))
		{
			GetDlgItem(IDC_CONTACT_BUSINESS_EMAIL)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}	
		else if(IsFieldEmpty(IDC_CONTACT_LASTNAME) && IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) && IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) &&
			IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_CELL) && IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL) && 
			IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL2) && IsFieldEmpty(IDC_CONTACT_BUSINESS_EMAIL) && 
			!IsFieldEmpty(IDC_CONTACT_BUSINESS_EMAIL2))
		{
			GetDlgItem(IDC_CONTACT_BUSINESS_EMAIL2)->GetWindowText(cstrRes);
			strRes = cstrRes;
		}

		return strRes;
	}
	bool CPhoneBookContactDlg::ExistSameDisplayName()
	{
		bool bRes = false;
		CString cstrDisplayName;
		GetDlgItem(IDC_CONTACT_DISPLAY)->GetWindowText(cstrDisplayName);
		TSTRING strDisplayName = cstrDisplayName.IsEmpty() || (m_cstrOrgDisplayName == cstrDisplayName && m_bIsOfficialFN) 
									? GetOfficialDisplayName() : (TSTRING)cstrDisplayName;

		for(vector<TSTRING>::iterator iter = m_vDisplayString.begin(); iter!= m_vDisplayString.end(); ++iter)
		{
			if(strDisplayName == (*iter))
			{
				bRes = true;
				break;
			}
		}
		return bRes;
	}
	
	bool CPhoneBookContactDlg::IsAllEmpty()
	{
		return(
		   IsFieldEmpty(IDC_CONTACT_LASTNAME) && 
		   IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
		   IsFieldEmpty(IDC_CONTACT_DISPLAY) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_CELL) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATE_FAX) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_FAX) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL2) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_EMAIL) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_EMAIL2) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATE_URL) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_URL) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATEADDRESS_EXTRA) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATEADDRESS_STREET) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATEADDRESS_CITY) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATEADDRESS_STATE) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATEADDRESS_POSTALCODE) && 
		   IsFieldEmpty(IDC_CONTACT_PRIVATEADDRESS_COUNTRY) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESSADDRESS_EXTRA) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESSADDRESS_STREET) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESSADDRESS_CITY) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESSADDRESS_STATE) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESSADDRESS_POSTALCODE) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESSADDRESS_COUNTRY) && 
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY) &&
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_TITLE) &&
		   IsFieldEmpty(IDC_CONTACT_PRIVATE_NOTE) &&
		   IsFieldEmpty(IDC_CONTACT_BUSINESS_NOTE));
	}
	bool CPhoneBookContactDlg::IsDisplayNameEmpty()
	{
		return(
			IsFieldEmpty(IDC_CONTACT_LASTNAME) && 
			IsFieldEmpty(IDC_CONTACT_FIRSTNAME) && 
			IsFieldEmpty(IDC_CONTACT_PRIVATE_NUMBER) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_NUMBER) && 
			IsFieldEmpty(IDC_CONTACT_PRIVATE_CELL) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_CELL) && 
			IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL) && 
			IsFieldEmpty(IDC_CONTACT_PRIVATE_EMAIL2) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_EMAIL) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_EMAIL2) && 
			IsFieldEmpty(IDC_CONTACT_BUSINESS_COMPANY)
			);
	}

	bool CPhoneBookContactDlg::IsFieldEmpty(int nID)
	{
		bool bRes = true;
		CString cstrText;
		GetDlgItem(nID)->GetWindowText(cstrText);
		bRes = cstrText.IsEmpty();
		return bRes;
	}

}