// QAboutBox.cpp : implementation file
//

#include "stdafx.h"
#include "QSyncer2.h"
#include "QAboutBox.h"
#include "../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "HPVERsionInfo.h"

// CQAboutBox dialog
namespace QSYNC_LIB
{

	IMPLEMENT_DYNAMIC(CQAboutBox, Win32DoubleBufferQDialogWidge)

		CQAboutBox::CQAboutBox(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(CQAboutBox::IDD, pParent),m_pButtonOK(NULL)
	{
	}
	CQAboutBox::~CQAboutBox()
	{
	}
	void CQAboutBox::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
	}
	BOOL CQAboutBox::OnInitDialog()
	{
		SetXMLUI();
		return TRUE;
	}
	void CQAboutBox::Event(const TSTRING& strEvent, long nParam)
	{
		if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pButtonOK->GetID() == nParam)
				OnBtnOK();
		}
	}
	void CQAboutBox::SetXMLUI()
	{
		SetBackground();
		CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
		Widge<HDC>* pWidget = pLayoutManager->Load(_T("QAboutBoxID"));
		
		CVersionInfo version(::AfxGetApp()->m_hInstance);
		WIN32LabelWidge* pVersion = (WIN32LabelWidge *)pLayoutManager->GetWidgetByID(_T("Win32Label_1"));
		if(pVersion)
			pVersion->SetDisplayString(version.AsString());
		WIN32LabelWidge* pCompany = (WIN32LabelWidge *)pLayoutManager->GetWidgetByID(_T("Win32Label_2"));		
		if(pCompany)
			pCompany->SetDisplayString(version.GetCompanyName());

		m_pButtonOK = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_0"));
		SetLayout(pWidget);
		delete pLayoutManager;
	}

	BEGIN_MESSAGE_MAP(CQAboutBox, CQDialog)
		ON_WM_SIZING()
	END_MESSAGE_MAP()

	void CQAboutBox::SetBackground()
	{
		TSTRING strBkImage = L"MainFrame\\BK_Dialog_About.png";
		SetBKImage(strBkImage,BK_Fixed);

		const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
		const int nHeight_AboutBox = GetBKImage()->GetHeight();

		SetMinWidth(nWIDTH_AboutBox);
		SetMinHeight(nHeight_AboutBox);
		SetMaxWidth(nWIDTH_AboutBox);
		SetMaxHeight(nHeight_AboutBox);
	}

	void CQAboutBox::OnSizing(UINT fwSide, LPRECT pRect)
	{
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
	}
	void CQAboutBox::OnBtnOK()
	{
		PostMessage(WM_CLOSE);
	}

}
// CQAboutBox message handlers
