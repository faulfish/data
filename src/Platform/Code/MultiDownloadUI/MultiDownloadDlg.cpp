// MultiDownloadDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MultiDownload.h"
#include "MultiDownloadDlg.h"

#include "../../../code/Components/UIFramework/interface/QControlNotify.h"
#include "../../../code/components/UIFramework/layoutmanager/Win32ProgressBarWidge.h"
#include "../../../code/CommonUtil\Win32SHELLUtility.h"
#include "../../../code/CommonUtil\Win32ThreadUtility.h"
#include <fstream>
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"
#include "..\..\..\code\odmtma_api\SerCOMLib\SerCOMLibrary.h"
//#include "..\..\..\Code\odmtma_api\QDownload_Export\Download_Export.h"
//#include "..\..\..\Code\Modules\QisdaMiscellaneousModule\QisdaMiscellaneousModule.h"
//#include "..\..\..\Code\Modules\QisdaNVModule\NVItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)


// CMultiDownloadDlg dialog


namespace QSYNC_LIB
{
	CMultiDownloadDlg* g_pThis = NULL;

	UINT DownloadThread(LPVOID pParam)
	{
	//	CMultiDownloadDlg* pDlg = (CMultiDownloadDlg*)pParam;
	//	if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_1)
	//		pDlg->StartDownload1();
	//	else if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_2)
	//		pDlg->StartDownload2();
	//	else if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_3)
	//		pDlg->StartDownload3();
	//	else if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_4)
	//		pDlg->StartDownload4();
	//	else if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_5)
	//		pDlg->StartDownload5();
	//	else if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_6)
	//		pDlg->StartDownload6();
	//	else if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_7)
	//		pDlg->StartDownload7();
	//	else if(pDlg->m_uiDownloadActive == DOWNLOAD_BUTTON_8)
	//		pDlg->StartDownload8();
		((CDownloadFunc *)pParam)->DownloadProcess();

		return 1;
	}
	CMultiDownloadDlg::CMultiDownloadDlg(CWnd* pParent /*=NULL*/)
		:Win32DoubleBufferQDialogWidge(CMultiDownloadDlg::IDD, pParent)
	{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
		m_pLayoutManager = NULL;

		m_unAvailCom = 0;
		m_unTotalCom = 0;

		g_pThis = this;

		for(int i = 0; i < 8; i++)
			m_DownloadFunc[i]= NULL;
	}

	void CMultiDownloadDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}

	BEGIN_MESSAGE_MAP(CMultiDownloadDlg, Win32DoubleBufferQDialogWidge)
		ON_WM_PAINT()
		ON_WM_SIZING()
		ON_WM_QUERYDRAGICON()
		//}}AFX_MSG_MAP
		ON_LBN_SELCHANGE(IDC_LIST1, &CMultiDownloadDlg::OnLbnSelchangeList1)
		ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangecom1)
		ON_CBN_SELCHANGE(IDC_COMBO2, OnSelchangecom2)
		ON_CBN_SELCHANGE(IDC_COMBO3, OnSelchangecom3)
		ON_CBN_SELCHANGE(IDC_COMBO4, OnSelchangecom4)
		ON_CBN_SELCHANGE(IDC_COMBO5, OnSelchangecom5)
		ON_CBN_SELCHANGE(IDC_COMBO6, OnSelchangecom6)
		ON_CBN_SELCHANGE(IDC_COMBO7, OnSelchangecom7)
		ON_CBN_SELCHANGE(IDC_COMBO8, OnSelchangecom8)
		ON_WM_DESTROY()
		ON_MESSAGE(WM_DL_NOTIFY, OnDLNotify)
	END_MESSAGE_MAP()


	// CMultiDownloadDlg message handlers

	BOOL CMultiDownloadDlg::OnInitDialog()
	{
		CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO: Add extra initialization here
		this->SetXMLUI();


		GetDeviceFinder()->StartMonitor();
		GetDeviceFinder()->Register(this);

		m_pWinStart1->SetEnable(false);
		m_pWinStart2->SetEnable(false);
		m_pWinStart3->SetEnable(false);
		m_pWinStart4->SetEnable(false);
		m_pWinStart5->SetEnable(false);
		m_pWinStart6->SetEnable(false);
		m_pWinStart7->SetEnable(false);
		m_pWinStart8->SetEnable(false);
		m_pWinStartAll->SetEnable(false);
	
		//Add the last used 10 1ff files in combo box;
		m_szFileArray.RemoveAll();
		for(int i = 0; i < 10; i++)
		{
			CString szFile;
			CString szKey;
			szKey.Format(_T("1FF %d"), i);
			szFile = AfxGetApp()->GetProfileString(_T("MultiDownload"), szKey.GetBuffer());

			if(szFile != _T(""))
			{
				m_szFileArray.Add(szFile);
				CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO9);
				pCombo->AddString(szFile.GetBuffer());
			}
			else
				break;

		}

		m_szComArray.RemoveAll();

		CString szPath;
		::GetModuleFileName(NULL, szPath.GetBuffer(MAX_PATH), MAX_PATH);
		szPath.ReleaseBuffer();
		szPath = szPath.Left(szPath.ReverseFind(_T('\\'))+1);
		m_strIni = szPath + _T("Download.ini");


		return TRUE;  // return TRUE  unless you set the focus to a control
	}

	// If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.

	void CMultiDownloadDlg::OnPaint()
	{
		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CDialog::OnPaint();
		}
	}

	// The system calls this function to obtain the cursor to display while the user drags
	//  the minimized window.
	HCURSOR CMultiDownloadDlg::OnQueryDragIcon()
	{
		return static_cast<HCURSOR>(m_hIcon);
	}


	void CMultiDownloadDlg::OnLbnSelchangeList1()
	{
		// TODO: Add your control notification handler code here

	}

	void CMultiDownloadDlg::OnSelchangecom1()
	{
		// TODO: Add your control notification handler code here
		CComboBox *pCombox = (CComboBox*)GetDlgItem(IDC_COMBO1);
		int nSelected = pCombox->GetCurSel();
		int nComport = pCombox->GetItemData(nSelected);

		for(int i = 1; i < 8; i++)
		{
			pCombox = (CComboBox*)GetDlgItem(IDC_COMBO1+i);
			int nSel = pCombox->GetCurSel();
			if(nSel != -1)
			{
				int nCom = pCombox->GetItemData(pCombox->GetCurSel());
				if(nCom == nComport)
				{
					CString szTemp;
					szTemp.Format(_T("this com port is selected in %d Combo Box! Please select other!"), i);
					MessageBox(szTemp);
					break;
				}
			}
		}

	}

	void CMultiDownloadDlg::OnSelchangecom2()
	{
		// TODO: Add your control notification handler code here
		CComboBox *pCombox = (CComboBox*)GetDlgItem(IDC_COMBO2);
		int nSelected = pCombox->GetCurSel();
		int nComport = pCombox->GetItemData(nSelected);

		for(int i = 2; i < 8; i++)
		{
			pCombox = (CComboBox*)GetDlgItem(IDC_COMBO1+i);
			int nSel = pCombox->GetCurSel();
			if(nSel != -1)
			{
				int nCom = pCombox->GetItemData(pCombox->GetCurSel());
				if(nCom == nComport)
				{
					CString szTemp;
					szTemp.Format(_T("this com port is selected in %d Combo Box! Please select other!"), i);
					MessageBox(szTemp);
				}
			}
		}
		
	}

	void CMultiDownloadDlg::OnSelchangecom3()
	{
		// TODO: Add your control notification handler code here
	}

	void CMultiDownloadDlg::OnSelchangecom4()
	{
		// TODO: Add your control notification handler code here
	}

	void CMultiDownloadDlg::OnSelchangecom5()
	{
		// TODO: Add your control notification handler code here
	}

	void CMultiDownloadDlg::OnSelchangecom6()
	{
		// TODO: Add your control notification handler code here
	}

	void CMultiDownloadDlg::OnSelchangecom7()
	{
		// TODO: Add your control notification handler code here
	}

	void CMultiDownloadDlg::OnSelchangecom8()
	{
		// TODO: Add your control notification handler code here
	}

	void CMultiDownloadDlg::SetXMLUI(void)
	{
		if(m_pLayoutManager == NULL)
		{
			SetBKImage(_T("BK_Download.png"),BK_Fixed);

			const int nWIDTH_AboutBox = GetBKImage()->GetWidth(); 
			const int nHeight_AboutBox = GetBKImage()->GetHeight();

			SetMinWidth(nWIDTH_AboutBox);
			SetMinHeight(nHeight_AboutBox);
			SetMaxWidth(nWIDTH_AboutBox);
			SetMaxHeight(nHeight_AboutBox);

			m_pLayoutManager = new CLayoutManager(this, this, _T("MultiDownloadLayout.xml"));
		}

		m_pMainLayout = (SplitLayout<HDC>*)m_pLayoutManager->Load(_T("CMultiDownloadDlgID"));

		m_pWinBrowse = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("BrowserButton"));
		m_pWinStartAll = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("StartAll"));
		m_pWinStart1 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start1"));
		m_pWinStart2 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start2"));
		m_pWinStart3 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start3"));
		m_pWinStart4 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start4"));
		m_pWinStart5 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start5"));
		m_pWinStart6 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start6"));
		m_pWinStart7 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start7"));
		m_pWinStart8 = (WIN32IconButton *)m_pLayoutManager->GetWidgetByID(_T("Start8"));

		SetLayout(m_pMainLayout);

	}
	void CMultiDownloadDlg::OnSizing(UINT fwSide, LPRECT pRect)
	{
		//Win32DoubleBufferFormCWndWidge<CQDialog>::OnSizing(fwSide, pRect);
		//GeometryRect rcClient = GetGeometry();
		//CQDialog::SetWindowRegion(pRect, rcClient);
		// TODO: Add your message handler code here
	}

	void CMultiDownloadDlg::Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pWinBrowse->GetID() == nParam) //Search 1FF file
				Browse1FFFile();
			else //Start All/Start 1 ~ Start 8
			{
				m_uiDownloadActive = -1;
				if(m_pWinStart1->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_1;
				else if(m_pWinStart2->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_2;
				else if(m_pWinStart3->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_3;
				else if(m_pWinStart4->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_4;
				else if(m_pWinStart5->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_5;
				else if(m_pWinStart6->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_6;
				else if(m_pWinStart7->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_7;
				else if(m_pWinStart8->GetID() == nParam)
					m_uiDownloadMask = DOWNLOAD_BUTTON_8;
				else
					m_uiDownloadMask = DOWNLOAD_BUTTON_8 + DOWNLOAD_BUTTON_7 + DOWNLOAD_BUTTON_6 + DOWNLOAD_BUTTON_5 + DOWNLOAD_BUTTON_4 + DOWNLOAD_BUTTON_3 + DOWNLOAD_BUTTON_2 + DOWNLOAD_BUTTON_1;
				
				DownloadFunc(m_uiDownloadMask);
			}
		}
		else if(strEvent == EVENT_SUBMENU)
		{
			//m_pILocalization->SetRegion(strEvent);
		}
		else if(strEvent == DEVICE_DIAG_CONNECT)
		{
			DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
			DeviceDataPt spDeviceData = *pspDeviceData; 

			UpdateComportCombo(spDeviceData->ID().c_str());
		}
		else if(strEvent == DEVICE_DIAG_DISCONNECT)
		{
			DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
			DeviceDataPt spDeviceData = *pspDeviceData; 

			UpdateComportCombo(spDeviceData->ID().c_str(), false);
		}
		else
		{
		}
	}

	bool CMultiDownloadDlg::UpdateComportCombo(LPCTSTR lpszComport, bool bAdd) 
	{
		int iCom = _wtoi(&lpszComport[3]);
		UINT unCount = 0;
		if(bAdd) //connect
		{
			m_unTotalCom++;
			m_szComArray.Add(lpszComport);

			switch(m_unTotalCom)
			{
			case 1:
				m_pWinStart1->SetEnable(true);
				break;
			case 2:
				m_pWinStart2->SetEnable(true);
				m_pWinStartAll->SetEnable(true);
				break;
			case 3:
				m_pWinStart3->SetEnable(true);
				break;
			case 4:
				m_pWinStart4->SetEnable(true);
				break;
			case 5:
				m_pWinStart5->SetEnable(true);
				break;
			case 6:
				m_pWinStart6->SetEnable(true);
				break;
			case 7:
				m_pWinStart7->SetEnable(true);
				break;
			case 8:
				m_pWinStart8->SetEnable(true);
				break;
			default:
				break;
			}
			
			UINT nLoop = 8;
			if(m_unTotalCom < 8 )
				nLoop = m_unTotalCom;

			for(UINT i = 0; i < nLoop; i++)
			{
				CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO1+i);
				pComboBox->AddString(lpszComport);
				unCount = pComboBox->GetCount();
				pComboBox->SetItemData(unCount -1, iCom);
				if(m_DownloadFunc[i] != NULL)
				{
					WIN32IconButton *pButton = m_DownloadFunc[i]->ReturnButtonWnd();
					pButton->SetEnable(false);
					pComboBox->SetCurSel(unCount -1);
				}
				int j = 0;
				while(unCount < m_unTotalCom)
				{
					CString szTemp = m_szComArray.GetAt(j);
					int iCom = _wtoi(szTemp.Mid(3).GetBuffer());
					pComboBox->InsertString(j, szTemp);
					pComboBox->SetItemData(j, iCom);
					unCount = pComboBox->GetCount();
					j++;
				}
			}
		}
		else //disconnect
		{
			switch(m_unTotalCom)
			{
			case 1:
				m_pWinStart1->SetEnable(false);
				break;
			case 2:
				m_pWinStart2->SetEnable(false);
				m_pWinStartAll->SetEnable(false);
				break;
			case 3:
				m_pWinStart3->SetEnable(false);
				break;
			case 4:
				m_pWinStart4->SetEnable(false);
				break;
			case 5:
				m_pWinStart5->SetEnable(false);
				break;
			case 6:
				m_pWinStart6->SetEnable(false);
				break;
			case 7:
				m_pWinStart7->SetEnable(false);
				break;
			case 8:
				m_pWinStart8->SetEnable(false);
				break;
			default:
				break;
			}

			UINT nLoop = 8;
			if(m_unTotalCom < 8 )
				nLoop = m_unTotalCom;

			for(int i = 0; i < m_unTotalCom; i++)
			{
				if(m_szComArray.GetAt(i) == lpszComport)
				{
					m_szComArray.RemoveAt(i);
					break;
				}
			}
			m_unTotalCom--;

			for(UINT i = 0; i < nLoop; i++)
			{
				CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO1+i);
				int nIndex = pComboBox->FindString(-1, lpszComport);
				pComboBox->DeleteString(nIndex);

				if((i > 0) && (pComboBox->GetCount() == 1))
					pComboBox->DeleteString(0);
			}
		}

		return false;
	}

	void CMultiDownloadDlg::OnDestroy()
	{
		GetDeviceFinder()->StopMonitor();
		GetDeviceFinder()->Unregister(this);

		for(int i = 0; i < m_szFileArray.GetCount(); i++)
		{
			CString szKey;
			szKey.Format(_T("1FF %d"), i);

			AfxGetApp()->WriteProfileString(_T("MultiDownload"), szKey.GetBuffer(), m_szFileArray.GetAt(i));
		}
		__super::OnDestroy();

		// TODO: Add your message handler code here
	}

	void CMultiDownloadDlg::Browse1FFFile(void)
	{
		CString szFilter = _T("1FF Files(*.mot)|*.mot|All Files(*.*)|*.*||");
		TCHAR szPath[MAX_PATH];
		::GetCurrentDirectory(MAX_PATH, szPath);
		CFileDialog fd(TRUE, NULL, _T(""), OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, szFilter);

		if(fd.DoModal() == IDOK)
		{
			::SetCurrentDirectory(szPath);

			CString szFile = fd.GetPathName();
			m_szFileArray.InsertAt(0, szFile);
			CComboBox* pComBo = (CComboBox*)GetDlgItem(IDC_COMBO9);
			for(int i = 0; i < pComBo->GetCount(); i++)
				pComBo->DeleteString(i);

			for(int i = 0; i < m_szFileArray.GetCount(); i++)
			{
				pComBo->AddString(m_szFileArray.GetAt(i));
				pComBo->SetItemData(i, i);
			}
			pComBo->SetCurSel(0);
			
		}

	}


	void CMultiDownloadDlg::DownloadFunc(unsigned int unDownloadButtonMask)
	{
		CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO9);
		if(pCombo->GetCurSel() == -1)
		{
			MessageBox( _T("Please select the Source code 1FF file first!"), _T("MultiDownlaod"), MB_OK);
			return;
		}

//		::WritePrivateProfileString(_T("Path"), _T("1ff"), m_szFileArray.GetAt(pCombo->GetItemData(pCombo->GetCurSel())), m_strIni);

		m_unButtonEnNum = 0;
		for(int i = 0; i < 8; i++)
		{
			if((unDownloadButtonMask & (DOWNLOAD_BUTTON_1 << i)) == (DOWNLOAD_BUTTON_1 << i))
			{
				Win32ProgressBarWidge *pProgress;
				WIN32IconButton* pButton;
				switch(i){
				case 0:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar1"));
					pButton = m_pWinStart1;
					break;
				case 1:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar2"));
					pButton = m_pWinStart2;
					break;
				case 2:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar3"));
					pButton = m_pWinStart3;
					break;
				case 3:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar4"));
					pButton = m_pWinStart4;
					break;
				case 4:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar5"));
					pButton = m_pWinStart5;
					break;
				case 5:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar6"));
					pButton = m_pWinStart6;
					break;
				case 6:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar7"));
					pButton = m_pWinStart7;
					break;
				case 7:
					pProgress = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar8"));
					pButton = m_pWinStart8;
					break;
				}
				if(pButton->GetEnable())
				{
					CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO1 + i);
					UINT nSelect = pComport->GetCurSel();
					UINT nComport =(UINT) pComport->GetItemData(nSelect);

					pButton->SetEnable(false);
					m_unButtonEnNum++;
					m_DownloadFunc[i] = new CDownloadFunc(nComport, i, m_strIni, m_hWnd, pButton, pProgress);
					if(m_DownloadFunc[i] != NULL)
					{
						m_DownloadFunc[i]->Set1ffFile(m_szFileArray.GetAt(pCombo->GetItemData(pCombo->GetCurSel())));
						AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[i], THREAD_PRIORITY_NORMAL);
					}
					else
						MessageBox(_T("New memory failed!"));
				}
			}
		}

//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_1) == DOWNLOAD_BUTTON_1)
//		{
//			if(m_pWinStart1->GetEnable())
//			{
//				m_pWinStart1->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO1);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar1"));
////				m_DownloadFunc[0] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 1, m_pWinStart1, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[0], THREAD_PRIORITY_NORMAL);
//			}
//		}
//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_2) == DOWNLOAD_BUTTON_2)
//		{
//			if(m_pWinStart2->GetEnable())
//			{
//				m_pWinStart2->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO2);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar2"));
////				m_DownloadFunc[1] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 2, m_pWinStart2, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[1], THREAD_PRIORITY_NORMAL);
//			}
//		}
//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_3) == DOWNLOAD_BUTTON_3)
//		{
//			if(m_pWinStart3->GetEnable())
//			{
//				m_pWinStart3->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO3);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar3"));
////				m_DownloadFunc[2] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 3, m_pWinStart3, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[2], THREAD_PRIORITY_NORMAL);
//			}
//		}
//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_4) == DOWNLOAD_BUTTON_4)
//		{
//			if(m_pWinStart4->GetEnable())
//			{
//				m_pWinStart4->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO4);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar4"));
////				m_DownloadFunc[3] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 4, m_pWinStart4, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[3], THREAD_PRIORITY_NORMAL);
//			}
//		}
//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_5) == DOWNLOAD_BUTTON_5)
//		{
//			if(m_pWinStart5->GetEnable())
//			{
//				m_pWinStart5->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO5);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar5"));
////				m_DownloadFunc[4] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 5, m_pWinStart5, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[4], THREAD_PRIORITY_NORMAL);
//			}
//		}
//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_6) == DOWNLOAD_BUTTON_6)
//		{
//			if(m_pWinStart6->GetEnable())
//			{
//				m_pWinStart6->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO6);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar6"));
////				m_DownloadFunc[5] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 6, m_pWinStart6, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[5], THREAD_PRIORITY_NORMAL);
//			}
//		}
//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_7) == DOWNLOAD_BUTTON_7)
//		{
//			if(m_pWinStart7->GetEnable())
//			{
//				m_pWinStart7->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO7);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar7"));
////				m_DownloadFunc[6] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 7, m_pWinStart7, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[6], THREAD_PRIORITY_NORMAL);
//			}
//		}
//		if((unDownloadButtonMask & DOWNLOAD_BUTTON_8) == DOWNLOAD_BUTTON_8)
//		{
//			unDownloadButtonMask ^= DOWNLOAD_BUTTON_8;
//			if(m_pWinStart8->GetEnable())
//			{
//				m_pWinStart8->SetEnable(false);
//				m_unButtonEnNum++;
//				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO8);
//				Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar8"));
////				m_DownloadFunc[7] = new CDownloadFunc(pComport->GetItemData(pComport->GetCurSel()), 8, m_pWinStart8, pProgressBar, m_strIni.GetBuffer(), this->m_hWnd);
//				AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[7], THREAD_PRIORITY_NORMAL);
//			}
//		}
		if(m_unButtonEnNum > 1)
			m_pWinStartAll->SetEnable(false);
	}

	//bool CMultiDownloadDlg::LogFn1(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	//CFont font;
	//	//font.CreateFont(10,10,0,0,FW_THIN,true,false,false,
	//	//	CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
	//	//	CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
	//	//	FF_MODERN,_T("ËÎÌו"));
	//	//pList->SetFont(&font);


	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount(), szMessage);
	//	pList->SetCurSel(pList->GetCount() -1);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar1"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart1->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack1(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn1(szMsg, nStep, nProgress);

	//	return true;
	//}

	//bool CMultiDownloadDlg::LogFn2(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST2);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount()-1, szMessage);
	//	pList->SetCurSel(pList->GetCount() -1);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar2"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart2->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//	}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack2(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn2(szMsg, nStep, nProgress);

	//	return true;
	//}

	//bool CMultiDownloadDlg::LogFn3(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST3);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount()-1, szMessage);
	//	pList->SetCurSel(pList->GetCount() -1);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar3"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart3->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//	}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack3(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn3(szMsg, nStep, nProgress);

	//	return true;
	//}

	//bool CMultiDownloadDlg::LogFn4(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST4);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount()-1, szMessage);
	//	pList->SetCurSel(pList->GetCount() -1);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar4"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart4->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//	}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack4(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn4(szMsg, nStep, nProgress);

	//	return true;
	//}

	//bool CMultiDownloadDlg::LogFn5(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST5);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount()-1, szMessage);
	//	pList->SetCurSel(pList->GetCount() -1);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar5"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart5->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//	}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack5(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn5(szMsg, nStep, nProgress);

	//	return true;
	//}

	//bool CMultiDownloadDlg::LogFn6(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST6);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount()-1, szMessage);
	//	pList->SetCurSel(pList->GetCount() -1);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar6"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart6->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//	}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack6(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn6(szMsg, nStep, nProgress);

	//	return true;
	//}

	//bool CMultiDownloadDlg::LogFn7(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST7);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount() - 1, szMessage);
	//	pList->SetCurSel(pList->GetCount() -1);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar7"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart7->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//	}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack7(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn7(szMsg, nStep, nProgress);

	//	return true;
	//}

	//bool CMultiDownloadDlg::LogFn8(const char* szMsg, int nStep, int nProgress)
	//{

	//	CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST8);
	//	CString szMessage;
	//	CString szLabelInfo;

	//	TCHAR szUnicodeInfor[4096] = {0};
	//	UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
	//	szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);

	//	pList->InsertString(pList->GetCount(), szMessage);

	//	Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_pLayoutManager->GetWidgetByID(_T("ProgressBar8"));
	//	if(pProgressBar != NULL)
	//		pProgressBar->UpdatePercent(nProgress);

	//	szLabelInfo = szUnicodeInfor;
	//	if(szLabelInfo == _T("Download Success") || szLabelInfo == _T("Download Fail"))
	//	{
	//		m_pWinStart8->SetEnable(true);
	//		if(m_unButtonEnNum > 1)
	//		{
	//			m_unButtonEnNum--;
	//			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
	//		}
	//	}

	//	return true;
	//}

	//bool CMultiDownloadDlg::CallBack8(const char* szMsg, int nStep, int nProgress)
	//{

	//	g_pThis->LogFn8(szMsg, nStep, nProgress);

	//	return true;
	//}

	//void CMultiDownloadDlg::StartDownload1()
	//{
	//	bool bDownloadOK = true;
	//	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST1);
	//	CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO1);
	//	UINT uComport = pComport->GetItemData(pComport->GetCurSel());
	//	CComboBox* pComBo = (CComboBox*)GetDlgItem(IDC_COMBO9);
	//	CString sz1FF = m_szFileArray.GetAt(pComBo->GetItemData(pComBo->GetCurSel()));
	////	bool bSignedCode = CheckSignedCode(pList);   ///< if the value is true, the code is signed
	//	bool bFused = CheckQFuseState(uComport);    ///< if the value is true, the mobile is fused

	//	//if(bSignedCode != bFused) {
	//	//	pList->AddString(_T("The software load and the handset are not matched..."));
	//	//	pList->AddString(_T("Please make sure that the load and handset are all signed or all unsigned..."));
	//	//	return;
	//	//}

	//	DWORD dwStartCount = ::GetTickCount();

	//	if(bDownloadOK)
	//	{
	//		pList->InsertString(pList->GetCount()-1, _T("Get ME info, Please wait......"));
	//		pList->SetCurSel(pList->GetCount() -1);
	//		if(!GetInfoFromME(pList, uComport))
	//		{	pList->InsertString(pList->GetCount()-1,_T("Can not Get Info From ME! Please try again!"));
	//			pList->SetCurSel(pList->GetCount() -1);
	//			bDownloadOK = false;
	//		}
	//	}

	//	if(bDownloadOK)
	//	{
	//		pList->InsertString(pList->GetCount() -1 , _T("Start to download"));
	//		pList->SetCurSel(pList->GetCount() -1);

	//		char cstrPath[MAX_PATH] ={0};
	//		UINT iUTF8CharCount = WideCharToMultiByte(CP_UTF8,FALSE,m_strIni.GetBuffer(0),m_strIni.GetLength(), cstrPath,MAX_PATH,NULL,NULL);	
	//		
	//		bool bDownload = Download(uComport,CallBack1, cstrPath);

	//		if(bDownload)
	//		{
	//			pList->InsertString(pList->GetCount() - 1, _T("Download Success!"));
	//			pList->SetCurSel(pList->GetCount() -1);

	//		}	
	//		else
	//		{
	//			pList->InsertString(pList->GetCount() - 1, _T("Download Failed!"));
	//			pList->SetCurSel(pList->GetCount() -1);

	//		}	
	//	}

	//	DWORD dwEndCount = ::GetTickCount();
	//	CString szTime;
	//	szTime.Format(_T("Download Cost Time %d s"),(dwEndCount - dwStartCount)/1000);
	//	pList->InsertString(pList->GetCount() - 1, szTime);
	//	pList->SetCurSel(pList->GetCount() -1);


	//	if(!bDownloadOK)
	//	{
	//		char *szInfo = "Download Fail";
	//		LogFn1(szInfo, 10, 100);
	//	}

	//}
	//void CMultiDownloadDlg::StartDownload2()
	//{
	//	bool bDownloadOK = true;
	//	CListBox* pList = (CListBox*)GetDlgItem(IDC_LIST2);
	//	CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO2);
	//	UINT uComport = pComport->GetItemData(pComport->GetCurSel());
	//	CComboBox* pComBo = (CComboBox*)GetDlgItem(IDC_COMBO9);
	//	CString sz1FF = m_szFileArray.GetAt(pComBo->GetItemData(pComBo->GetCurSel()));
	//	//	bool bSignedCode = CheckSignedCode(pList);   ///< if the value is true, the code is signed
	//	bool bFused = CheckQFuseState(uComport);    ///< if the value is true, the mobile is fused

	//	//if(bSignedCode != bFused) {
	//	//	pList->AddString(_T("The software load and the handset are not matched..."));
	//	//	pList->AddString(_T("Please make sure that the load and handset are all signed or all unsigned..."));
	//	//	return;
	//	//}

	//	DWORD dwStartCount = ::GetTickCount();

	//	if(bDownloadOK)
	//	{
	//		pList->InsertString(pList->GetCount()-1, _T("Get ME info, Please wait......"));
	//		pList->SetCurSel(pList->GetCount() -1);
	//		if(!GetInfoFromME(pList, uComport))
	//		{	pList->InsertString(pList->GetCount()-1,_T("Can not Get Info From ME! Please try again!"));
	//			pList->SetCurSel(pList->GetCount() -1);
	//			bDownloadOK = false;
	//		}
	//	}

	//	if(bDownloadOK)
	//	{
	//		pList->InsertString(pList->GetCount() -1 , _T("Start to download"));
	//		pList->SetCurSel(pList->GetCount() -1);

	//		char cstrPath[MAX_PATH] ={0};
	//		UINT iUTF8CharCount = WideCharToMultiByte(CP_UTF8,FALSE,m_strIni.GetBuffer(0),m_strIni.GetLength(), cstrPath,MAX_PATH,NULL,NULL);	

	//		bool bDownload = Download(uComport, CallBack2, cstrPath);

	//		if(bDownload)
	//		{
	//			pList->InsertString(pList->GetCount() - 1, _T("Download Success!"));
	//			pList->SetCurSel(pList->GetCount() -1);

	//		}	
	//		else
	//		{
	//			pList->InsertString(pList->GetCount() - 1, _T("Download Failed!"));
	//			pList->SetCurSel(pList->GetCount() -1);

	//		}	
	//	}

	//	DWORD dwEndCount = ::GetTickCount();
	//	CString szTime;
	//	szTime.Format(_T("Download Cost Time %d s"),(dwEndCount - dwStartCount)/1000);
	//	pList->InsertString(pList->GetCount() - 1, szTime);
	//	pList->SetCurSel(pList->GetCount() -1);


	//	if(!bDownloadOK)
	//	{
	//		char *szInfo = "Download Fail";
	//		LogFn1(szInfo, 10, 100);
	//	}
	//}
	//void CMultiDownloadDlg::StartDownload3()
	//{
	//}
	//void CMultiDownloadDlg::StartDownload4()
	//{
	//}
	//void CMultiDownloadDlg::StartDownload5()
	//{
	//}
	//void CMultiDownloadDlg::StartDownload6()
	//{
	//}
	//void CMultiDownloadDlg::StartDownload7()
	//{
	//}
	//void CMultiDownloadDlg::StartDownload8()
	//{
	//}

	//void CMultiDownloadDlg::CheckSignedCode(CListBox &List ) 
	//{
	//	//InsertStringList(_T("Check Signed Code..."), _T(""));
	//	//bool bRes = false;
	//	//TSTRING strQcsbl, strAmss, strOemsbl;
	//	//GetDLPathFromIni(m_strIni.GetString(), _T("amss"), strAmss);
	//	//GetDLPathFromIni(m_strIni.GetString(), _T("oemsbl"), strOemsbl);
	//	////GetDLPathFromIni(m_strIni.GetString(), _T("qcsbl"), strQcsbl);
	//	//if(strAmss.length() == 0 || strOemsbl.length() == 0)// || strQcsbl.length() == 0)
	//	//	return false;
	//
	//	//CFile famss(strAmss.c_str(), CFile::modeRead | CFile::typeBinary);
	//	//CFile foemsbl(strOemsbl.c_str(), CFile::modeRead | CFile::typeBinary);
	//	////CFile fqcsbl(strQcsbl.c_str(), CFile::modeRead | CFile::typeBinary);
	//	//BYTE* szAmssBuf = new BYTE[4096];
	//	//BYTE* szOemsblBuf = new BYTE[foemsbl.GetLength()];
	//	////BYTE* szQcsblBuf = new BYTE[fqcsbl.GetLength()];
	//
	//	//const char* szAttestation = "QUALCOMM Attestation CA";
	//	//int nLength = famss.Read(szAmssBuf, 4096);
	//	//nLength = foemsbl.Read(szOemsblBuf, foemsbl.GetLength());
	//	////nLength = fqcsbl.Read(szQcsblBuf, fqcsbl.GetLength());
	//
	//	//bool bAmss = false, bOemsbl = false;//, bQcsbl = false;
	//	//for(int i = 0; i <  4096 - 23; ++i) {
	//	//	int n = memcmp(&(szAmssBuf[i]), szAttestation, 23);
	//	//	if(n == 0) {
	//	//		bAmss = true;
	//	//		InsertStringList(_T("Check Signed Code--amss..."), _T("Signed"));
	//	//		break;
	//	//	}
	//	//}
	//	//if(!bAmss)
	//	//	InsertStringList(_T("Check Signed Code--amss..."), _T("Unsigned"));
	//	//for(int i = 0; i <  foemsbl.GetLength() - 23; ++i) {
	//	//	int n = memcmp(&(szOemsblBuf[i]), szAttestation, 23);
	//	//	if(n == 0) {
	//	//		bOemsbl = true;
	//	//		InsertStringList(_T("Check Signed Code--oemsbl..."), _T("Signed"));
	//	//		break;
	//	//	}
	//	//}
	//	//if(!bOemsbl)
	//	//	InsertStringList(_T("Check Signed Code--oemsbl..."), _T("Unsigned"));
	//	////for(int i = 0; i <  fqcsbl.GetLength() - 23; ++i) {
	//	////    int n = memcmp(&(szQcsblBuf[i]), szAttestation, 23);
	//	////    if(n == 0) {
	//	////        bQcsbl = true;
	//	////        InsertStringList(_T("Check Signed Code--qcsbl..."), _T("Signed"));
	//	////        break;
	//	////    }
	//	////}
	//	////InsertStringList(_T("Check Signed Code--qcsbl..."), _T("Unsigned"));
	//	//delete[] szAmssBuf;
	//	//delete[] szOemsblBuf;
	//	////delete[] szQcsblBuf;
	//	//return bAmss && bOemsbl;// && bQcsbl;
	//}

	//bool CMultiDownloadDlg::CheckQFuseState(UINT uComport) 
	//{
	//	typedef bool (*pFnOnlineMode)(int nPort);
	//	typedef bool (*pFnFTMMode)(int nPort);
	//	HMODULE hDll = LoadLibrary(_T("MobileToolKit.dll"));
	//	if(hDll == NULL) return false;
	//	pFnOnlineMode Online = (pFnOnlineMode)GetProcAddress(hDll, "OnLineMode");
	//	Online(uComport);
	//	pFnFTMMode FTM = (pFnFTMMode)GetProcAddress(hDll, "FTMMode");
	//	FTM(uComport);
	//	Sleep(1000);
	//	unsigned long reg1 = 0, reg2 = 0;
	//	typedef bool (*pFnReadQFuse)(int, unsigned long&, unsigned long&);
	//	pFnReadQFuse ReadQFuse = (pFnReadQFuse)GetProcAddress(hDll, "ReadQFuseState");
	//	ReadQFuse(uComport, reg1, reg2);
	//	FreeLibrary(hDll);
	//	return !(reg1 == 6 && reg2 == 0);
	//}

	//bool CMultiDownloadDlg::GetInfoFromME(CListBox* pList, UINT uComport)
	//{
	//	CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(uComport);

	//	char szSW[100] ={0};
	//	char szHW[100] ={0};

	//	if(pMsModule->GetSWHWVersion(szSW, szHW))
	//	{
	//		TCHAR szUnicodeInfor[4096] = {0};
	//		UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szHW,-1,szUnicodeInfor,4096);	
	//		CString szString;
	//		szString.Format( _T("Your Phone HW: %s"), szUnicodeInfor);
	//		pList->InsertString(pList->GetCount() - 1, szString);
	//		pList->SetCurSel(pList->GetCount() -1);
	//		iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szSW,-1,szUnicodeInfor,4096);	
	//		szString.Format(_T("Your Phone SW: SVN %s"), szUnicodeInfor);
	//		pList->InsertString(pList->GetCount() -1 ,szString);
	//		pList->SetCurSel(pList->GetCount() -1);

	//	}
	//	else
	//	{
	//		pList->InsertString(pList->GetCount() -1, _T("Can not Get SWHW Version from ME"));
	//		pList->SetCurSel(pList->GetCount() -1);

	//	}

	//	delete pMsModule;

	//	DIAG_NVITEM_PACKET_F_type NVResponse;
	//	CNVItem *pNVModule = new CNVItem(uComport);
	//	if(pNVModule->ReadNVItem(3497, NVResponse))
	//	{
	//		TCHAR szUnicodeInfor[4096] = {0};
	//		UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)NVResponse.item_data,-1,szUnicodeInfor,4096);	
	//		CString szString;
	//		szString.Format(_T("Your Phone Variant: %s"), szUnicodeInfor);
	//		pList->InsertString(pList->GetCount() -1 , szString);
	//		pList->SetCurSel(pList->GetCount() -1);

	//	}
	//	else
	//	{
	//		pList->InsertString(pList->GetCount() -1 ,  _T("Can not get Variant version from ME"));
	//		pList->SetCurSel(pList->GetCount() -1);

	//	}

	//	delete pNVModule;
	//	return true;
	//}

	LRESULT CMultiDownloadDlg::OnDLNotify(WPARAM wParam, LPARAM lParam)
	{
		UINT	nPercentage		= 0;
		UINT	nStep			= 0;
		WCHAR	szErrorMsg[200] = {0};

		m_DownloadFunc[(UINT)lParam]->GetDLTaskInfo((UINT)wParam, szErrorMsg, nStep, nPercentage);

		CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1 + ((UINT)lParam));
		CString szMessage;
		CString szLabelInfo;

		//CFont font;
		//font.CreateFont(10,10,0,0,FW_THIN,true,false,false,
		//	CHINESEBIG5_CHARSET,OUT_CHARACTER_PRECIS,
		//	CLIP_CHARACTER_PRECIS,DEFAULT_QUALITY,
		//	FF_MODERN,_T("ËÎÌו"));
		//pList->SetFont(&font);

		szMessage.Format(_T("%s, Percentage: %d"), szErrorMsg, nPercentage);

		pList->InsertString(pList->GetCount(), szMessage);
		pList->SetCurSel(pList->GetCount() -1);

		//Win32ProgressBarWidge *pProgressBar = (Win32ProgressBarWidge*)m_DownloadFunc[(UINT)lParam ]->ReturnProgressWnd();
		//if(pProgressBar != NULL)
		//	pProgressBar->UpdatePercent(nPercentage);

		szLabelInfo = szErrorMsg;
		if(szLabelInfo == _T("Download Successful!") || szLabelInfo == _T("Download Failed!"))
		{
			WIN32IconButton *pButton = (WIN32IconButton*)m_DownloadFunc[(UINT)lParam]->ReturnButtonWnd();
			pButton->SetEnable(true);
			delete m_DownloadFunc[(UINT)lParam];
			m_DownloadFunc[(UINT)lParam] = NULL;
			if(m_unButtonEnNum > 1)
			{
				m_unButtonEnNum--;
				m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
			}
		}

		return 0;
	}
}