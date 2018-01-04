// PhoneBookSIMContactDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PhoneBookSIMContactDlg.h"
#include "../../Components/UIFramework/LayoutManager/WIN32IconButtonWidgeVer2.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"

// CPhoneBookSIMContactDlg dialog
namespace QSYNC_LIB
{
	IMPLEMENT_DYNAMIC(CPhoneBookSIMContactDlg, CQDialog)

	CPhoneBookSIMContactDlg::CPhoneBookSIMContactDlg(CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferQDialogWidge(IDD_PHONEBOOK_SIMCONTACT, pParent), m_pILocale(NULL)
	{
		CreateILocalization(&m_pILocale);
	}

	CPhoneBookSIMContactDlg::~CPhoneBookSIMContactDlg()
	{
	}

	void CPhoneBookSIMContactDlg::DoDataExchange(CDataExchange* pDX)
	{
		CQDialog::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_SIM_PHONE, m_QEditCtrlPhone);
	}


	BEGIN_MESSAGE_MAP(CPhoneBookSIMContactDlg, CQDialog)
		ON_EN_CHANGE(IDC_SIM_PHONE, &CQEditCtrl::OnEnChange)
		ON_WM_SIZING()
	END_MESSAGE_MAP()

	BOOL CPhoneBookSIMContactDlg::OnInitDialog()
	{
		CQDialog::OnInitDialog();
		BOOL bRes = TRUE;
		SetXMLUI();
		SetUILimitation();
		SetPhoneCheckingWords();

		bRes = SetEditBox();	
		return bRes; 
	}
	
	void CPhoneBookSIMContactDlg::SetPhoneCheckingWords()
	{
		IPIMObjPt PIMObjPt; // Empty PIMObj (cause we won't use it)
		CVCardCtrl VCardCtrl(PIMObjPt);
		TSTRING strCheckWords;
		VCardCtrl.GetLimitation(_T("PhoneLimitWords"), strCheckWords);
		if(strCheckWords.empty())
			strCheckWords = _T("0123456789P*#+");

		m_QEditCtrlPhone.SetCheckingWords((TCHAR*)strCheckWords.c_str(),(UINT)strCheckWords.size());
	}
	void CPhoneBookSIMContactDlg::Event(const TSTRING& strEvent, long nParam)
	{
		if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pBtnOK->GetID() == nParam)
				OnBnClickedOK();
			else if(m_pBtnCancel->GetID() == nParam)
				OnBnClickedCancel();
			else if(m_pWinCloseButton->GetID() == nParam)
				FnWinClose();
		}
	}

	void CPhoneBookSIMContactDlg::SetXMLUI()
	{
		SetBackground();
		CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
		Widge<HDC>* pWidget = pLayoutManager->Load(_T("Phonebook_SIMContactDlgID"));
		m_pLayout = (SplitLayout<HDC>*)pLayoutManager->GetWidgetByID(_T("SplitLayout_0"));
		m_pWinCloseButton = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_0"));
		m_pBtnCancel = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_1"));
		m_pBtnOK = (WIN32IconButton *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_2"));
		SetLayout(pWidget);
		delete pLayoutManager;
	}

	std::wstring CPhoneBookSIMContactDlg::GetWString(const TSTRING& strID)
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
	
	BOOL CPhoneBookSIMContactDlg::SetEditBox()
	{
		BOOL bRes = TRUE;
		if(m_PIMObjPt)
		{
			SetOrgText(IDC_SIM_NAME, KEYWORD_FIRSTNAME);
			SetSpecialOrgText(IDC_SIM_PHONE,	"TEL",_T("HOME;VOICE"),0);
		}
		
		CString cstrOrgDisplayName;
		GetDlgItem(IDC_SIM_NAME)->GetWindowText(cstrOrgDisplayName);
		if(cstrOrgDisplayName.IsEmpty())
			GetDlgItem(IDC_SIM_PHONE)->GetWindowText(cstrOrgDisplayName);

		for(vector<TSTRING>::iterator iter=m_vDisplayString.begin(); iter!= m_vDisplayString.end() && !cstrOrgDisplayName.IsEmpty(); iter++)
		{
			if((*iter) == (TSTRING)cstrOrgDisplayName)
			{			
				m_vDisplayString.erase(iter);
				break;
			}
		}
		return bRes;
	}
	void CPhoneBookSIMContactDlg::SetOrgText(int nID, const char* cpKey)
	{
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		TSTRING strText;
		VCardCtrl.GetCmnStrData(cpKey, strText);
		GetDlgItem(nID)->SetWindowText(strText.c_str());
	}
	void CPhoneBookSIMContactDlg::SetSpecialOrgText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount)
	{
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		TSTRING strText;
		strText = VCardCtrl.GetSpecificString(cpKey, strParameter, nPropertyCount, nValueCount);
		GetDlgItem(nID)->SetWindowText(strText.c_str());
	}

	bool CPhoneBookSIMContactDlg::SaveEditBox()
	{
		bool bRes = false;
		//SaveText(IDC_SIM_NAME, KEYWORD_FIRSTNAME);
		bool bName = SaveText(IDC_SIM_NAME, "N", _T(""), 0, 1);
		bool bPhone = SaveText(IDC_SIM_PHONE,	"TEL",_T("HOME;VOICE"),0);
		bRes = bName && bPhone;
		return bRes;
	}
	bool CPhoneBookSIMContactDlg::SaveText(int nID, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount)
	{
		bool bRes = true;
		CVCardCtrl VCardCtrl(m_PIMObjPt);
		CString cstrText;
		GetDlgItem(nID)->GetWindowText(cstrText);
		TSTRING strText = cstrText;
		bRes = VCardCtrl.SetSpecificString(strText, cpKey, strParameter, nPropertyCount, nValueCount);
		return bRes;
	}
	
	void CPhoneBookSIMContactDlg::SetUILimitation()
	{
		IPIMObjPt PIMObjPt; // Empty PIMObj (cause we won't use it)
		CVCardCtrl VCardCtrl(PIMObjPt);

		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_SIM_NAME)), &VCardCtrl, _T(KEYWORD_FIRSTNAME));
		SetLimit((CQEditCtrl*)(GetDlgItem(IDC_SIM_PHONE)), &VCardCtrl, _T(KEYWORD_TEL));
	}
	void CPhoneBookSIMContactDlg::SetLimit(CQEditCtrl* pEditCtrl,  CVCardCtrl* pVCardCtrl, const TSTRING& strKey)
	{
		int nLimit = pVCardCtrl->GetLimitation(strKey);
		if(nLimit != -1)
			pEditCtrl->SetTextLength(nLimit);
	}
	void CPhoneBookSIMContactDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
	}

	void CPhoneBookSIMContactDlg::SetBackground()
	{
		TSTRING strBkImage = L"PhoneBook\\Dlg\\Bk_SIMEditor.png";
		SetBKImage(strBkImage,BK_Fixed);

		const int nContactDlgWIDTH = GetBKImage()->GetWidth(); 
		const int nContactDlgHEIGHT = GetBKImage()->GetHeight() ;
		SetMinWidth(nContactDlgWIDTH);
		SetMinHeight(nContactDlgHEIGHT);
		SetMaxWidth(nContactDlgWIDTH);
		SetMaxHeight(nContactDlgHEIGHT);
	}

	void CPhoneBookSIMContactDlg::FnWinClose()
	{
		PostMessage(WM_CLOSE);
	}

	void CPhoneBookSIMContactDlg::OnBnClickedCancel()
	{
		PostMessage(WM_CLOSE);
	}

	void CPhoneBookSIMContactDlg::OnBnClickedOK()
	{
		if(!IsBothEmpty() && !ExistSameName())
		{
			SaveEditBox();
			OnOK();
		}	
	}
	
	bool CPhoneBookSIMContactDlg::ExistSameName()
	{
		bool bRes = false;
		TSTRING strDisplay;
		CString cstrName;
		GetDlgItem(IDC_SIM_NAME)->GetWindowText(cstrName);
		if(cstrName.IsEmpty())
		{
			CString cstrPhone;
			GetDlgItem(IDC_SIM_PHONE)->GetWindowText(cstrPhone);
			strDisplay = cstrPhone;
		}
		else
		{
			strDisplay = cstrName;
		}
		for(vector<TSTRING>::iterator iter=m_vDisplayString.begin(); iter!= m_vDisplayString.end(); ++iter)
		{
			if(strDisplay == (*iter))
			{
				bRes = true;
				break;
			}
		}
		if(bRes)
			ShowMessage(GetSafeHwnd(),GetString(_T("IDS_PHONEBOOK_MsgSameName")),GetString(_T("IDS_PHONEBOOK_FUNCTION")));
			
		return bRes;
	}

	bool CPhoneBookSIMContactDlg::IsBothEmpty()
	{
		bool bRes = false;
		CString cstrName;
		GetDlgItem(IDC_SIM_NAME)->GetWindowText(cstrName);
		CString cstrPhone;
		GetDlgItem(IDC_SIM_PHONE)->GetWindowText(cstrPhone);
		if(cstrName.IsEmpty() && cstrPhone.IsEmpty())
		{
			bRes = true;
			ShowMessage(GetSafeHwnd(),GetString(_T("IDS_PHONEBOOK_MsgEmpty")),GetString(_T("IDS_PHONEBOOK_FUNCTION")));
		}
		return bRes;
	}
	CString CPhoneBookSIMContactDlg::GetString(const TSTRING& strID)
	{
		return m_pILocale->GetString(strID).c_str();
	}
}//namespace QSYNC_LIB
