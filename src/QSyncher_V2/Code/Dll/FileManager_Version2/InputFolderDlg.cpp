// InputFolderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "InputFolderDlg.h"
#include "../../Components/UIFramework/LayoutManager/Win32Widge.h"

// CInputFolderDlg dialog
namespace QSYNC_LIB
{

	IMPLEMENT_DYNAMIC(CInputFolderDlg, CQDialog)

		CInputFolderDlg* g_pThis = NULL;

	CInputFolderDlg::CInputFolderDlg(CQTreeNode* pFolderNode, CWnd* pParent /*=NULL*/)
		:Win32DoubleBufferQDialogWidge(CInputFolderDlg::IDD, pParent)
		,m_pFolderNode(pFolderNode)
		,m_pWinCloseButton(NULL)
		,m_pBtnCancel(NULL)
		,m_pBtnOK(NULL)
	{
		CreateILocalization(&m_pILocale);
		g_pThis = this;
	}

	CInputFolderDlg::~CInputFolderDlg()
	{
	}

	void CInputFolderDlg::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
	}


	BEGIN_MESSAGE_MAP(CInputFolderDlg, CQDialog)
		ON_EN_CHANGE(IDC_EDIT_INPUT_FILE_NAME, &CInputFolderDlg::OnEnChangeEditInputFileName)
		ON_BN_CLICKED(IDOK_INPUT_FILE_NAME, &CInputFolderDlg::OnBnClickedInputFileName)
		ON_BN_CLICKED(IDOK, &CInputFolderDlg::OnBnClickedOk)
		ON_WM_SIZING()
	END_MESSAGE_MAP()



	BOOL CInputFolderDlg::OnInitDialog()
	{
		CQDialog::OnInitDialog();
		//SetUI();
		SetXMLUI();
		if(!m_strInitialText.IsEmpty())
		{
		CEdit *pEditFolder = (CEdit*)(GetDlgItem(IDC_EDIT_INPUT_FILE_NAME));
		pEditFolder->SetWindowTextW(m_strInitialText);
		}
		UpdateData(FALSE);
		return TRUE;
	}

	// CInputFolderDlg message handlers

	void CInputFolderDlg::OnEnChangeEditInputFileName()
	{
		// TODO:  If this is a RICHEDIT control, the control will not
		// send this notification unless you override the CQDialog::OnInitDialog()
		// function and call CRichEditCtrl().SetEventMask()
		// with the ENM_CHANGE flag ORed into the mask.


	}

	void CInputFolderDlg::OnBnClickedInputFileName()
	{




	}

	CString CInputFolderDlg::GetInputString(void) const
	{
		return m_strInText;
	}
	void CInputFolderDlg::SetInitialString(const CString& strInitialString)
	{
		m_strInitialText = strInitialString;
	}

	void CInputFolderDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}

	void CInputFolderDlg::Event(const TSTRING& strEvent, long nParam)
	{
		if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pBtnOK->GetID() == nParam)
				g_pThis->OnBnClickedOk();
			else if(m_pWinCloseButton->GetID() == nParam || m_pBtnCancel->GetID() == nParam)
				FnWinClose();
		}
	}
	void CInputFolderDlg::SetXMLUI()
	{
		SetBackground();
		CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
		Widge<HDC>* pWidget = pLayoutManager->Load(_T("FileManager_InputFolderDlgID"));
		m_pWinCloseButton = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_0"));
		m_pBtnCancel = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_1"));
		m_pBtnOK = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_2"));

		SetLayout(pWidget);
		delete pLayoutManager;
	}
	void CInputFolderDlg::SetUI()
	{
		//RC檔只是大致畫出輪廓,按鈕都是用自己的Widget實作的
		SetBackground();
		SplitLayout<HDC>* pMainLayout = new SplitLayout<HDC>(false);

		SplitLayout<HDC>* pCaptionLayout = CreateCaptionLayout();
		SplitLayout<HDC>* pClientLayout = CreateClientLayout();

		pMainLayout->AddWidget(pCaptionLayout, pClientLayout);
		pMainLayout->SetFixedValue(29);
		pMainLayout->SetSpacing(2);

		SetLayout(pMainLayout);
		CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
		bool bSave = pLayoutManager->Save(pMainLayout, _T("FileManager_RenameFileDlg"), _T("FileManager_RenameFileDlgID"));
		delete pLayoutManager;
	}

	CString CInputFolderDlg::GetString(const TSTRING& strID)
	{
		return m_pILocale->GetString(strID).c_str();
	}
	std::wstring CInputFolderDlg::GetWString(const TSTRING& strID)
	{
#ifdef _UNICODE
		return m_pILocale->GetString(strID).c_str();
#else
		return ansitowide(m_pILocale->GetString(strID));

#endif
	}
	bool CInputFolderDlg::ExistSameNameFolder(const CString& strFolderName) const
	{
		bool bRes = false;
		if(m_pFolderNode)
		{
			for (int nIndex = 0 ; nIndex < m_pFolderNode->GetChildSize() ; ++nIndex)
			{
				bRes |= m_pFolderNode->GetChild(nIndex)->GetName().c_str() == strFolderName;
			}
		}
		return bRes;
	}

	void CInputFolderDlg::SetBackground()
	{
		std::wstring strBkImage = L"FileManager\\Background\\Bk_CreatNewFolder.png";
		SetBKImage(strBkImage , BK_Fixed);

		const int nMIN_WIDTH_MAINFRAME = GetBKImage()->GetWidth();
		const int nMIN_HEIGHT_MAINFRAME = GetBKImage()->GetHeight();
		SetMinWidth(nMIN_WIDTH_MAINFRAME);
		SetMinHeight(nMIN_HEIGHT_MAINFRAME);
		SetMaxWidth(nMIN_WIDTH_MAINFRAME);
		SetMaxHeight(nMIN_HEIGHT_MAINFRAME);
	}

	SplitLayout<HDC>* CInputFolderDlg::CreateCaptionLayout()
	{
		SplitLayout<HDC>* pCaptionLayout = new SplitLayout<HDC>;
		WIN32LabelWidge* pCaptionText = new WIN32LabelWidge(this,_T("IDS_FILEMANAGER_FUNCTION"),Left|VCenter,L"Arial",9);
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
	SplitLayout<HDC>* CInputFolderDlg::CreateClientLayout()
	{
		SplitLayout<HDC>* pClientLayout = new SplitLayout<HDC>(false);
		pClientLayout->AddWidget( CreateSubWindowLayout() , CreateButtonLayout() );
		pClientLayout->SetFixedValue(184-138,false);
		pClientLayout->SetVariouslyMargin(0,0,0,10,false);

		return pClientLayout;
	}
	BoxLayout<HDC>* CInputFolderDlg::CreateButtonLayout()
	{
		BoxLayout<HDC>* pButtonLayout = new BoxLayout<HDC>(RightToLeft);
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
	SplitLayout<HDC>* CInputFolderDlg::CreateSubWindowLayout()
	{
		SplitLayout<HDC>* pSubWindowLayout = new SplitLayout<HDC>(false);

		WIN32LabelWidge* pCaptionText = new WIN32LabelWidge(this, GetWString(_T("IDS_FileMgr_Input_FolderName")),Left,L"Arial", 9);
		pCaptionText->SetTextColor(255,255,255,255);

		pSubWindowLayout->AddWidget(pCaptionText,0);
		pSubWindowLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_EDIT_INPUT_FILE_NAME)),VCenter|HCenter,false);
		pSubWindowLayout->SetFixedValue(50);
		pSubWindowLayout->SetVariouslyMargin(44-30,1,22,3,true);
		pSubWindowLayout->SetVariouslyMargin(98-80,136-118,82,399-317,false);
		//m_pLayout->AddWidget(new WIN32WindowWidge(GetDlgItem(IDC_INPUT_FILE_NAME_MSG)),0);

		return pSubWindowLayout;
	}

	//Button functions:
	void CInputFolderDlg::WinClose()
	{
		g_pThis->FnWinClose();
	}
	void CInputFolderDlg::FnWinClose()
	{
		PostMessage(WM_CLOSE);
	}
	void CInputFolderDlg::Cancel()
	{
		g_pThis->FnWinClose();
	}
	void CInputFolderDlg::OK()
	{
		g_pThis->OnBnClickedOk();
	}
	void CInputFolderDlg::OnBnClickedOk()
	{
		m_strInText.Empty();
		CEdit *pEditFolder = (CEdit*)(GetDlgItem(IDC_EDIT_INPUT_FILE_NAME));
		pEditFolder->GetWindowTextW(m_strInText);

		if(ExistSameNameFolder(m_strInText))
			ShowMessage(GetSafeHwnd(),_T("IDS_FileMgr_MsgSameName"),_T("IDS_PHONEBOOK_FUNCTION"));
		else if(m_strInText.IsEmpty())
			ShowMessage(GetSafeHwnd(),_T("IDS_FileMgr_MsgEmptyFileName"),_T("IDS_PHONEBOOK_FUNCTION"));
		else
			OnOK();
	}

/************************************************************************/
/*                    CRenameDlg                                        */
/************************************************************************/


	CRenameDlg::CRenameDlg(CQTreeNode* pFolderNode,const CString& strOldFileName, CWnd* pParent):
	CInputFolderDlg(pFolderNode,pParent),m_strOldFileName(strOldFileName)
	{
		SetInitialString(m_strOldFileName);
	}
	void CRenameDlg::SetXMLUI()
	{
		SetBackground();
		CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
		Widge<HDC>* pWidget = pLayoutManager->Load(_T("FileManager_RenameFileDlgID"));
		m_pWinCloseButton = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Btn_WinClose"));
		m_pBtnCancel = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Btn_Cancel"));
		m_pBtnOK = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Btn_OK"));

		SetLayout(pWidget);
		delete pLayoutManager;
	}

	//bool CRenameDlg::ExistSameNameFolder(const CString& strFolderName) const
	//{
	//	bool bRes = false;
	//	if(m_pFolderNode)
	//	{
	//		for (int nIndex = 0 ; nIndex < m_pFolderNode->GetChildSize() ; ++nIndex)
	//		{
	//			bRes |= m_pFolderNode->GetChild(nIndex)->GetName().c_str() == strFolderName;
	//		}
	//	}
	//	return bRes;
	//}

	void CRenameDlg::OnBnClickedOk()
	{
		m_strInText.Empty();
		CEdit *pEditFolder = (CEdit*)(GetDlgItem(IDC_EDIT_INPUT_FILE_NAME));
		pEditFolder->GetWindowTextW(m_strInText);

		if(m_strInText == m_strOldFileName)
			FnWinClose();
		else if(ExistSameNameFolder(m_strInText))
			ShowMessage(GetSafeHwnd(),_T("IDS_FileMgr_MsgSameName"),_T("IDS_PHONEBOOK_FUNCTION"));
		else if(m_strInText.IsEmpty())
			ShowMessage(GetSafeHwnd(),_T("IDS_FileMgr_MsgEmptyFileName"),_T("IDS_PHONEBOOK_FUNCTION"));
		else
			OnOK();
	}

}