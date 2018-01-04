// OptionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DownloadUIForIthaca.h"
#include "OptionDlg.h"
#include "../../../code/Components/UIFramework/interface/QControlNotify.h"
#include "../../../code/CommonUtil\Win32SHELLUtility.h"
#include "../../../code/CommonUtil\Win32ThreadUtility.h"
#include <fstream>

static TSTRING g_strIniPath = _T("Download.ini");
namespace QSYNC_LIB
{
	// COptionDlg dialog

	IMPLEMENT_DYNAMIC(COptionDlg, Win32DoubleBufferQDialogWidge)

	COptionDlg::COptionDlg(CString szPath,CWnd* pParent /*=NULL*/)
		:Win32DoubleBufferQDialogWidge(COptionDlg::IDD, pParent)
		,m_sz1FFPath(_T(""))
		,m_szHexPath(_T(""))
		,m_szPartitionPath(_T(""))
		,m_szQCSBLPath(_T(""))
		,m_szQCSBLHDPath( _T(""))
		,m_szOEMSBLPath(_T(""))
		,m_szOEMSBLHDPath(_T(""))
		,m_szAMSSPath(_T(""))
		,m_szAMSSHDPath(_T(""))
		,m_szCEFSPath(_T(""))
		,m_szDUPath(_T(""))
		,m_bBackupChecked(false)
		,m_bVersionChecked(false)
	{
		m_szPath = szPath;
		m_pLayoutManager = NULL;
		m_pMainLayout = NULL;
	}
	COptionDlg::~COptionDlg()
	{
	}

	void COptionDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}


	BEGIN_MESSAGE_MAP(COptionDlg, Win32DoubleBufferQDialogWidge)
		ON_WM_SIZING()
	END_MESSAGE_MAP()


	// COptionDlg message handlers
	void COptionDlg::Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(this->m_pBtn1FF->GetID() == nParam)
				SearchFile(TYPE_1FF);
			else if(this->m_pBtnHex->GetID() == nParam)
				SearchFile(TYPE_HEX);
			else if(this->m_pBtnPartition->GetID() == nParam)
				SearchFile(TYPE_PARTITION);
			else if(this->m_pBtnQCSBL->GetID() == nParam)
				SearchFile(TYPE_QCSBL);
			else if(this->m_pBtnQCSBLHD->GetID() == nParam)
				SearchFile(TYPE_QCSBLHD);
			else if(this->m_pBtnOEMSBL->GetID() == nParam)
				SearchFile(TYPE_OEMSBL);
			else if(this->m_pBtnOEMSBLHD->GetID() == nParam)
				SearchFile(TYPE_OEMSBLHD);
			else if(this->m_pBtnAMSS->GetID() == nParam)
				SearchFile(TYPE_AMSS);
			else if(this->m_pBtnAMSSHD->GetID() == nParam)
				SearchFile(TYPE_AMSSHD);
			else if(this->m_pBtnCEFS->GetID() == nParam)
				SearchFile(TYPE_CEFS);
			else if(this->m_pBtnDU->GetID() == nParam)
				SearchFile(TYPE_DU);
			else if(this->m_pBtnOK->GetID() == nParam)
				this->OnOKButton();
			else if(this->m_pBtnCancel->GetID() == nParam)
				this->OnCancel();
			else
				;
		}
		else
		{
		}
	}

	void COptionDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		GeometryRect rcClient = GetGeometry();
		CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}

	BOOL COptionDlg::OnInitDialog()
	{
		__super::OnInitDialog();

		// TODO:  Add extra initialization here
		this->SetXMLUI();
		WIN32IconButton *pTemp;

		std::wstring strIniFile = m_szPath.GetBuffer(0);
		strIniFile += _T("Download.ini");

		GetDLPathFromIni(strIniFile, _T("1ff"), m_sz1FFPath);
		if(m_sz1FFPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_1FF, m_sz1FFPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("1FFCheck"));
			pTemp->SetButtonCheck(true);

		}

		GetDLPathFromIni(strIniFile, _T("hex"), m_szHexPath);
		if(m_szHexPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_HEX, m_szHexPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("HexCheck"));
			pTemp->SetButtonCheck(true);

		}
		
		GetDLPathFromIni(strIniFile, _T("partition"), m_szPartitionPath);
		if(m_szPartitionPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_PARTITION, m_szPartitionPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("PartitionCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("qcsbl"), m_szQCSBLPath);
		if(m_szQCSBLPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_QCSBL, m_szQCSBLPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("QCSBLCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("qcsblhd"), m_szQCSBLHDPath);
		if(m_szQCSBLHDPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_QCSBLHD, m_szQCSBLHDPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("QCSBLHDCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("oemsbl"), m_szOEMSBLPath);
		if(m_szOEMSBLPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_OEMSBL, m_szOEMSBLPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("OEMSBLCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("oemsblhd"), m_szOEMSBLHDPath);
		if(m_szOEMSBLHDPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_OEMSBLHD, m_szOEMSBLHDPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("OEMSBLHDCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("amss"), m_szAMSSPath);
		if(m_szAMSSPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_AMSS, m_szAMSSPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("AMSSCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("amsshd"), m_szAMSSHDPath);
		if(m_szAMSSHDPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_AMSSHD, m_szAMSSHDPath.c_str());
			pTemp =(WIN32IconButton *) m_pLayoutManager->GetWidgetByID(_T("AMSSHDCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("cefs"), m_szCEFSPath);
		if(m_szCEFSPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_CEFS, m_szCEFSPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("CEFSCheck"));
			pTemp->SetButtonCheck(true);
		}

		GetDLPathFromIni(strIniFile, _T("du"), m_szDUPath);
		if(m_szDUPath != _T(""))
		{
			SetDlgItemText(IDC_PATH_DU,m_szDUPath.c_str());
			pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("DUCheck"));
			pTemp->SetButtonCheck(true);
		}


		m_bBackupChecked = (1 == ::GetPrivateProfileInt(_T("Setting"), _T("Backup"), 0, strIniFile.c_str()));
		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("IconBtn_Backup"));
		m_bBackupChecked == 0? pTemp->SetButtonCheck(false):pTemp->SetButtonCheck(true);

		m_bVersionChecked = (1 == ::GetPrivateProfileInt(_T("Setting"), _T("CheckVersion"), 0, strIniFile.c_str()));
		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("IconBtn_CheckVer"));
		m_bVersionChecked == 0? pTemp->SetButtonCheck(false):pTemp->SetButtonCheck(true);

		return TRUE;  // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	}

	void COptionDlg::SetXMLUI(void)
	{
		if(m_pLayoutManager == NULL)
		{
			SetBKImage(_T("Bk_Option.png"),BK_Fixed);
			const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
			const int nHeight_AboutBox = GetBKImage()->GetHeight();

			SetMinWidth(nWIDTH_AboutBox);
			SetMinHeight(nHeight_AboutBox);
			SetMaxWidth(nWIDTH_AboutBox);
			SetMaxHeight(nHeight_AboutBox);

			m_pLayoutManager = new CLayoutManager(this, this, _T("OptionLayout.xml"));
		}

		m_pMainLayout = (SplitLayout<HDC>*)m_pLayoutManager->Load(_T("COptionDlgID"));

		m_pBtn1FF = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("1FFButton"));
		m_pBtnHex = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("HexButton"));
		m_pBtnPartition = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("PartitionButton"));
		m_pBtnQCSBL = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("QCSBLButton"));
		m_pBtnQCSBLHD = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("QCSBLHDButton"));
		m_pBtnOEMSBL = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("OEMSBLButton"));
		m_pBtnOEMSBLHD = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("OEMSBLHDButton"));
		m_pBtnAMSS = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("AMSSButton"));
		m_pBtnAMSSHD = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("AMSSHDButton"));
		m_pBtnCEFS = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("CEFSButton"));
		m_pBtnDU = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("DUButton"));
		m_pBtnOK = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("Btn_OK"));
		m_pBtnCancel = (WIN32IconButton*) m_pLayoutManager->GetWidgetByID(_T("Btn_CANCEL"));

		SetLayout(m_pMainLayout);

	}

	void COptionDlg::OnOKButton()
	{
		CString szTemp;
		WIN32IconButton *pTemp;
		std::wstring strIniFile = m_szPath.GetBuffer(0);
		strIniFile += _T("Download.ini");

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("1FFCheck"));
		if(!pTemp->GetButtonCheck())
			m_sz1FFPath = L"";
		SetDLPathToIni(strIniFile, _T("1ff"), m_sz1FFPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("HexCheck"));
		if(!pTemp->GetButtonCheck())
			m_szHexPath = L"";
		SetDLPathToIni(strIniFile, _T("hex"), m_szHexPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("PartitionCheck"));
		if(!pTemp->GetButtonCheck())
			m_szPartitionPath = L"";
		SetDLPathToIni(strIniFile, _T("partition"), m_szPartitionPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("QCSBLCheck"));
		if(!pTemp->GetButtonCheck())
			m_szQCSBLPath = L"";
		SetDLPathToIni(strIniFile, _T("qcsbl"), m_szQCSBLPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("QCSBLHDCheck"));
		if(!pTemp->GetButtonCheck())
			m_szQCSBLHDPath = L"";
		SetDLPathToIni(strIniFile, _T("qcsblhd"), m_szQCSBLHDPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("OEMSBLCheck"));
		if(!pTemp->GetButtonCheck())
			m_szOEMSBLPath = L"";
		SetDLPathToIni(strIniFile, _T("oemsbl"), m_szOEMSBLPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("OEMSBLHDCheck"));
		if(!pTemp->GetButtonCheck())
			m_szOEMSBLHDPath = L"";
		SetDLPathToIni(strIniFile, _T("oemsblhd"), m_szOEMSBLHDPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("AMSSCheck"));
		if(!pTemp->GetButtonCheck())
			m_szAMSSPath = L"";
		SetDLPathToIni(strIniFile, _T("amss"), m_szAMSSPath);

		pTemp =(WIN32IconButton *) m_pLayoutManager->GetWidgetByID(_T("AMSSHDCheck"));
		if(!pTemp->GetButtonCheck())
			m_szAMSSHDPath = L"";
		SetDLPathToIni(strIniFile, _T("amsshd"), m_szAMSSHDPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("CEFSCheck"));
		if(!pTemp->GetButtonCheck())
			m_szCEFSPath = L"";
		SetDLPathToIni(strIniFile, _T("cefs"), m_szCEFSPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("DUCheck"));
		if(!pTemp->GetButtonCheck())
			m_szDUPath = L"";
		SetDLPathToIni(strIniFile, _T("du"), m_szDUPath);

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("IconBtn_Backup"));
		m_bBackupChecked = pTemp->GetButtonCheck();
		szTemp.Format(_T("%d"), m_bBackupChecked);
		::WritePrivateProfileString(_T("Setting"), _T("Backup"), szTemp.GetBuffer(0), strIniFile.c_str());

		pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("IconBtn_CheckVer"));
		m_bVersionChecked = pTemp->GetButtonCheck();
		szTemp.Format(_T("%d"), m_bVersionChecked);
		::WritePrivateProfileString(_T("Setting"), _T("CheckVersion"), szTemp.GetBuffer(0), strIniFile.c_str());

		this->OnOK();
	}

	void COptionDlg::SearchFile(int nFileType)
	{
		CString szFilter, szDefaultName;

		switch(nFileType){
			case TYPE_1FF:
				szFilter = _T("1FF Files(*.mot)|*.mot|All Files(*.*)|*.*||");
				szDefaultName = m_sz1FFPath.c_str();
				break;
			case TYPE_HEX:
				szFilter = _T("HEX Files(*.hex)|*.hex|All Files(*.*)|*.*||");
				szDefaultName = m_szHexPath.c_str();
				break;
			case TYPE_PARTITION:
				szFilter = _T("Partition Files(*.mbn)|*.mbn|All Files(*.*)|*.*||");
				szDefaultName = m_szPartitionPath.c_str();
				break;
			case TYPE_QCSBL:
				szFilter = _T("QCSBL Files(*.mbn)|*.mbn|All Files(*.*)|*.*||");
				szDefaultName = m_szQCSBLPath.c_str();
				break;
			case TYPE_QCSBLHD:
				szFilter = _T("QCSBLHD Files(*.mbn)|*.mbn|All Files(*.*)|*.*||");
				szDefaultName = m_szQCSBLHDPath.c_str();
				break;
			case TYPE_OEMSBL:
				szFilter = _T("OEMSBL Files(*.mbn)|*.mbn|All Files(*.*)|*.*||");
				szDefaultName = m_szOEMSBLPath.c_str();
				break;
			case TYPE_OEMSBLHD:
				szFilter = _T("OEMSBLHD Files(*.mbn)|*.mbn|All Files(*.*)|*.*||");
				szDefaultName = m_szOEMSBLHDPath.c_str();
				break;
			case TYPE_AMSS:
				szFilter = _T("AMSS Files(*.mbn)|*.mbn|All Files(*.*)|*.*||");
				szDefaultName = m_szAMSSPath.c_str();
				break;
			case TYPE_AMSSHD:
				szFilter = _T("AMSSHD Files(*.mbn)|*.mbn|All Files(*.*)|*.*||");
				szDefaultName = m_szAMSSHDPath.c_str();
				break;
			case TYPE_CEFS:
				szFilter = _T("CEFS Files(*.qvcefs)|*.qvcefs|All Files(*.*)|*.*||");
				szDefaultName = m_szCEFSPath.c_str();
				break;
			case TYPE_DU:
				szFilter = _T("DU Files(*.du)|*.du|All Files(*.*)|*.*||");
				szDefaultName = m_szDUPath.c_str();
				break;
			default:
				ASSERT(false);
				break;
		}

		TCHAR szPath[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, szPath);
		CFileDialog fd(TRUE, NULL, szDefaultName, OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);

		if(fd.DoModal() == IDOK)
		{
			::SetCurrentDirectory(szPath);

			WIN32IconButton *pTemp;
			switch(nFileType){
				case TYPE_1FF:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("1FFCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_1FF, fd.GetPathName().GetBuffer(0));
					m_sz1FFPath = fd.GetPathName();
					break;
				case TYPE_HEX:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("HexCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_HEX, fd.GetPathName().GetBuffer(0));
					m_szHexPath = fd.GetPathName();
					break;
				case TYPE_PARTITION:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("PartitionCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_PARTITION, fd.GetPathName().GetBuffer(0));
					 m_szPartitionPath = fd.GetPathName();
					 break;
				case TYPE_QCSBL:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("QCSBLCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_QCSBL, fd.GetPathName().GetBuffer(0));
					m_szQCSBLPath = fd.GetPathName();
					 break;
				case TYPE_QCSBLHD:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("QCSBLHDCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_QCSBLHD, fd.GetPathName().GetBuffer(0));
					m_szQCSBLHDPath = fd.GetPathName();
					 break;
				case TYPE_OEMSBL:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("OEMSBLCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_OEMSBL, fd.GetPathName().GetBuffer(0));
					m_szOEMSBLPath = fd.GetPathName();
					 break;
				case TYPE_OEMSBLHD:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("OEMSBLHDCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_OEMSBLHD, fd.GetPathName().GetBuffer(0));
					m_szOEMSBLHDPath = fd.GetPathName();
					 break;
				case TYPE_AMSS:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("AMSSCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_AMSS, fd.GetPathName().GetBuffer(0));
					m_szAMSSPath = fd.GetPathName();
					 break;
				case TYPE_AMSSHD:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("AMSSHDCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_AMSSHD, fd.GetPathName().GetBuffer(0));
					m_szAMSSHDPath = fd.GetPathName();
					 break;
				case TYPE_CEFS:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("CEFSCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_CEFS, fd.GetPathName().GetBuffer(0));
					m_szCEFSPath = fd.GetPathName();
					 break;
				case TYPE_DU:
					pTemp = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("DUCheck"));
					pTemp->SetButtonCheck(true);
					SetDlgItemText(IDC_PATH_DU, fd.GetPathName().GetBuffer(0));
					m_szDUPath = fd.GetPathName();
					 break;
				default:
					ASSERT(false);
					break;
			}
		}
	}

	bool COptionDlg::GetDLPathFromIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath)
	{
		bool bRes = false;
		TCHAR szPath[500] ={0};
		DWORD lRetValue = ::GetPrivateProfileString(_T("Path"),strName.c_str(),NULL,szPath, 500, strIniPath.c_str());
		strPath = szPath;
		if(::_taccess_s(strPath.c_str(),0) == 0)
			bRes = true;
		else
			strPath = _T("");
		return bRes;
	}

	bool COptionDlg::SetDLPathToIni(const TSTRING& strIniPath, const TSTRING& strName, TSTRING& strPath)
	{
		DWORD lRetValue = ::WritePrivateProfileString(_T("Path"), strName.c_str(), strPath.c_str(), strIniPath.c_str());
		return true;
	}

}

