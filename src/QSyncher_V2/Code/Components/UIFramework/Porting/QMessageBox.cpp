// QMessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "QMessageBox.h"


// QMessageBox dialog
using namespace QSYNC_LIB;

QMessageBox* g_pThis = NULL;

IMPLEMENT_DYNAMIC(QMessageBox, CQDialog)

QMessageBox::QMessageBox(CWnd* pParent /*=NULL*/)
	: Win32DoubleBufferQDialogWidge(IDD_QMESSAGEBOX_DIALOG, pParent)
	,m_strWarningImage(L"General\\Dialog\\Dialog_Warning.png")
	,m_nID(QMB_OK)
	,m_nNumberOfTextLines(0)
	,m_pILocalization(NULL)
{
	g_pThis = this;
	CreateILocalization(&m_pILocalization);
}
QMessageBox::QMessageBox(const std::wstring& strMessage, const TSTRING& strTitle, MESSAGETYPE nID,CWnd* pParent /*=NULL*/)
: Win32DoubleBufferQDialogWidge(IDD_QMESSAGEBOX_DIALOG, pParent)
,m_nNumberOfTextLines(0)
,m_pILocalization(NULL)
{
	g_pThis = this;
	m_strWarningText = strMessage;
	SetMessageType(nID);
	m_strTitleText = strTitle;
	CreateILocalization(&m_pILocalization);

	//SetWindowText(strTitle.c_str());
}

QMessageBox::~QMessageBox()
{
}

void QMessageBox::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
void QMessageBox::OnSizing(UINT fwSide, LPRECT pRect)
{
	GeometryRect rcClient = GetGeometry();
	CQDialog::SetWindowRegion(pRect, rcClient);
}


BEGIN_MESSAGE_MAP(QMessageBox, CQDialog)
	ON_WM_SIZING()
END_MESSAGE_MAP()

BOOL QMessageBox::OnInitDialog()
{
	CQDialog::OnInitDialog();
	SetUI();
	return TRUE;
}

bool QMessageBox::SetUI()
{
	SplitLayout<HDC>* pMainLayout = new SplitLayout<HDC>(false);

	SplitLayout<HDC>* pCaptionLayout = CreateCaptionLayout();
	SplitLayout<HDC>* pClientLayout = CreateClientLayout();

	pMainLayout->AddWidget(pCaptionLayout, pClientLayout);
	pMainLayout->SetFixedValue(29);
	pMainLayout->SetSpacing(2);

	bool bRes = true;
	bRes = SetLayout(pMainLayout);
	return bRes;
}
void QMessageBox::SetWarningImage()
{
	switch(m_nID) 
	{
	case QMB_OK:
		m_strWarningImage=_T("General\\Dialog\\Dialog_Warning.png"); // !
		break;
	case QMB_OKCANCEL:
		m_strWarningImage=_T("General\\Dialog\\Dialog_QuestionMark.png"); // ?		
		break;
	case QMB_ABORTRETRYIGNORE:
	case QMB_RETRYCANCEL:
		m_strWarningImage=_T("General\\Dialog\\Dialog_Warning.png"); // !
		break;
	case QMB_YESNOCANCEL:
	case QMB_YESNO:
		m_strWarningImage=_T("General\\Dialog\\Dialog_QuestionMark.png"); // ?
		break;
	}
}

BoxLayout<HDC>* QMessageBox::SetButtonLayout()
{
	BoxLayout<HDC>* pButton= new BoxLayout<HDC>;
	pButton->SetCell(98,26);
	pButton->SetSpacing(5);
	TSTRING strBKImage = _T("MainFrame\\Button\\Btn_Language_Original.png");
	TSTRING strHighLightImage = _T("MainFrame\\Button\\Btn_Language_MouseOver.png");
	TSTRING strClickImage = _T("MainFrame\\Button\\Btn_Language_MouseDown.png");
	TSTRING strDisableImage = _T("MainFrame\\Button\\Btn_Language_Disable.png");

	switch(m_nID) 
	{
	case QMB_OK:
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_OK"),strBKImage,strHighLightImage,strClickImage,strDisableImage,OK));
		break;
	case QMB_OKCANCEL:
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_OK"),strBKImage,strHighLightImage,strClickImage,strDisableImage,OK));
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Cancel"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Cancel));
		break;
	case QMB_ABORTRETRYIGNORE:
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Abort"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Abort));
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Retry"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Retry));
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Ignore"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Ignore));
		break;
	case QMB_RETRYCANCEL:
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Retry"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Retry));
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Cancel"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Cancel));
		break;
	case QMB_YESNOCANCEL:
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Yes"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Yes));
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_No"),strBKImage,strHighLightImage,strClickImage,strDisableImage,No));
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Cancel"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Cancel));
		break;
	case QMB_YESNO:
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_Yes"),strBKImage,strHighLightImage,strClickImage,strDisableImage,Yes));
		pButton->AddWidget(new WIN32ButtonWidge(this,_T("IDS_No"),strBKImage,strHighLightImage,strClickImage,strDisableImage,No));
		break;
	}
	pButton->SetAlignment(HCenter);
	return pButton;
}
//UI setting functions
void QMessageBox::SetBackground(int nBtnCount)
{
	HWND hTemp = GetSafeHwnd();
	Graphics graphics(hTemp);
	Font myFont(L"Arial", 9);
	StringFormat format;
	SizeF  szLayoutSize( (nBtnCount < 3) ? 303-89 : 353-89 , 100-48 );
	SizeF  szBound;
	INT codepointsFitted;
	graphics.MeasureString(m_pILocalization->GetString(m_strWarningText).c_str(),m_pILocalization->GetString(m_strWarningText).length(),&myFont,szLayoutSize,&format,&szBound,&codepointsFitted,&m_nNumberOfTextLines);

	//TSTRING strBkImage = (nBtnCount < 3) ? L"General\\Dialog\\BK_Dialog_2Btn.png" : L"General\\Dialog\\BK_Dialog_3Btn.png";
	TSTRING strBkImage = _T("General\\Dialog\\BK_Dialog_");
	strBkImage = strBkImage + ((m_nNumberOfTextLines<4) ? _T("") : _T("5L") ) + ((nBtnCount < 3) ? _T("2Btn") : _T("3Btn") ) + _T(".png");

	SetBKImage(strBkImage , BK_Fixed);

	const int nMIN_WIDTH_MAINFRAME = GetBKImage()->GetWidth();
	const int nMIN_HEIGHT_MAINFRAME = GetBKImage()->GetHeight();
	SetMinWidth(nMIN_WIDTH_MAINFRAME);
	SetMinHeight(nMIN_HEIGHT_MAINFRAME);
	SetMaxWidth(nMIN_WIDTH_MAINFRAME);
	SetMaxHeight(nMIN_HEIGHT_MAINFRAME);
}
SplitLayout<HDC>* QMessageBox::CreateCaptionLayout()
{
	SplitLayout<HDC>* pCaptionLayout = new SplitLayout<HDC>;
	WIN32LabelWidge* pCaptionText = new WIN32LabelWidge(this,m_strTitleText,Left|VCenter,L"Arial",9);
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
	pCaptionLayout->SetFixedValue(41,false);

	return pCaptionLayout;
}
SplitLayout<HDC>* QMessageBox::CreateClientLayout()
{
	SplitLayout<HDC>* pClientLayout = new SplitLayout<HDC>(false);

	BoxLayout<HDC>* pButtonLayout = SetButtonLayout();
	pClientLayout->AddWidget(pButtonLayout,Top,false);
	pClientLayout->SetFixedValue(159-121,false);

	int nBtnCount = pButtonLayout->GetCount();
	SetBackground(nBtnCount);

	SplitLayout<HDC>* pMessageLayout = new SplitLayout<HDC>;
	pMessageLayout->AddWidget(new WIN32DrawerWidge(this, m_strWarningImage,Fixed),Left|Top);
	int nAlignment = (m_nNumberOfTextLines == 1) ? VCenter|HCenter : VCenter|Left;
	WIN32LabelWidge* pWarningText = new WIN32LabelWidge(this, m_strWarningText, nAlignment, L"Arial", 9);
	pWarningText->SetTextColor(255,255,255,255);
	pMessageLayout->AddWidget(pWarningText,Left|Top,false);
	pMessageLayout->SetVariouslyMargin(4,21,0,0,false);
	pMessageLayout->SetSpacing(13);
	pMessageLayout->SetFixedValue(60);
	pClientLayout->AddWidget(pMessageLayout,Left);


	pClientLayout->SetVariouslyMargin(14,0,17,17,true);
	return pClientLayout;
}


//Button functions
void QMessageBox::OK()
{
	g_pThis->EndDialog(QIDOK);
}
void QMessageBox::Cancel()
{
	g_pThis->EndDialog(QIDCANCEL);
}
void QMessageBox::Retry()
{
	g_pThis->EndDialog(QIDRETRY);
}
void QMessageBox::Yes()
{
	g_pThis->EndDialog(QIDYES);
}
void QMessageBox::No()
{
	g_pThis->EndDialog(QIDNO);
}
void QMessageBox::Abort()
{
	g_pThis->EndDialog(QIDABORT);
}
void QMessageBox::Ignore()
{
	g_pThis->EndDialog(QIDIGNORE);
}

void QMessageBox::WinClose()
{
	g_pThis->FnWinClose();
}
void QMessageBox::FnWinClose()
{
	PostMessage(WM_CLOSE);
}
// QMessageBox message handlers

BOOL QMessageBox::PreTranslateMessage(MSG* pMsg)
{
	switch(pMsg->message)
	{
	case WM_SYSKEYDOWN:
		if(pMsg->wParam == VK_F4)
			return TRUE;
		break;
	case WM_KEYDOWN:
		{
			switch(pMsg->wParam)
			{
			case VK_ESCAPE:
				{
					return TRUE;  //(to disable <Esc>-Key-Exit.)
				}
				break;
			case VK_RETURN:
				if(m_nID == QMB_OK || QMB_OKCANCEL)
					OK();
				else if(m_nID == QMB_YESNOCANCEL || m_nID == QMB_YESNO)
					Yes();
				else
					Cancel();
				return TRUE;
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	return __super::PreTranslateMessage(pMsg);
}
