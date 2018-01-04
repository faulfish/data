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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
#pragma warning(disable:4996)


// CMultiDownloadDlg dialog

static int nLoopCount = 200;

namespace QSYNC_LIB
{
	CMultiDownloadDlg* g_pThis = NULL;

	UINT DownloadThread(LPVOID pParam)
	{
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
		m_unButtonEnNum = 0;

		m_sz1FF = _T("");

		g_pThis = this;

		for(int i = 0; i < 8; i++)
		{
			m_DownloadFunc[i]= NULL;
			m_DownloadProgress[i] = 0;
		}	

		m_pCallBackFunc[0] = CallBack1;
		m_pCallBackFunc[1] = CallBack2;
		m_pCallBackFunc[2] = CallBack3;
		m_pCallBackFunc[3] = CallBack4;
		m_pCallBackFunc[4] = CallBack5;
		m_pCallBackFunc[5] = CallBack6;
		m_pCallBackFunc[6] = CallBack7;
		m_pCallBackFunc[7] = CallBack8;

		m_strFolderPath = _T("");
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
		ON_CBN_SELCHANGE(IDC_COMBO9, OnSelchangecom9)
		ON_LBN_SELCHANGE(IDC_LIST1, &CMultiDownloadDlg::OnLbnSelchangeList1)		
		ON_WM_DESTROY()
		ON_MESSAGE(WM_DL_NOTIFY, OnDLNotify)
	END_MESSAGE_MAP()

	void CMultiDownloadDlg::OnSelchangecom9()
	{
		CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO9);
		CString szTemp = m_szFileArray.GetAt(pComboBox->GetCurSel());
		if((szTemp != m_sz1FF))
		{
			CDownloadFunc *pDownloadFunc = new CDownloadFunc(0, 0, m_strIni, m_hWnd, NULL, NULL);
			if(pDownloadFunc != NULL)
			{
				pDownloadFunc->Set1ffFile(szTemp);
				CString szTemp1 = pDownloadFunc->CheckLoad();
				if(szTemp1 == _T(""))
				{
					pComboBox->DeleteString(pComboBox->GetCurSel());
					m_szFileArray.RemoveAt(pComboBox->GetCurSel());

					int nIndex = pComboBox->FindString(-1, m_sz1FF);
					if(nIndex != -1)
						pComboBox->SetCurSel(nIndex);

					MessageBox(_T("Can not Unzip the 1FF file, please check the 1FF file is OK!"));
				}
				else
				{
					m_sz1FF = szTemp;
					m_strFolderPath = szTemp1;
				}

				delete pDownloadFunc;
			}
		}
	}

	BOOL CMultiDownloadDlg::OnInitDialog()
	{
		CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO: Add extra initialization here
		SetXMLUI();


		GetDeviceFinder()->StartMonitor();
		GetDeviceFinder()->Register(this);

		m_pWinStartAll->SetEnable(false);

		for(int i = 0; i < 8; i++)
		{
			CComboBox *pCombox = (CComboBox*)GetDlgItem(IDC_COMBO1+i);
			pCombox->EnableWindow(false);
			EnableStartButton(i,false);
		}
	
		//Add the last used 10 1ff files in combo box;
		m_szFileArray.RemoveAll();
		int nCount = 0;
		for(int i = 0; i < 10; i++)
		{
			CString szFile;
			CString szKey;
			szKey.Format(_T("1FF %d"), i);
			szFile = AfxGetApp()->GetProfileString(_T("MultiDownload"), szKey.GetBuffer());

			if(szFile != _T(""))
			{
				int j = 0;
				for(j = 0; j < m_szFileArray.GetCount(); j++)
					if(m_szFileArray.GetAt(j) == szFile)
						break;

				if(j == m_szFileArray.GetCount())
				{
					m_szFileArray.InsertAt(nCount, szFile);
					CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO9);
					pCombo->InsertString(nCount, szFile);
					pCombo->SetItemData(nCount, nCount);
					nCount++;
				}
			}
			else
				break;

		}

		m_szComArray.RemoveAll();

		CString strINIFolder;
		TCHAR pszPath[MAX_PATH] = {0};
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, pszPath))) 
		{
			strINIFolder = CString(pszPath) + CString(_T("\\MultiDownload\\"));
			::CreateDirectory(m_strFolderPath, NULL);
		}
		else
		{
			assert(0);
		}
		m_strIni = strINIFolder + _T("Download.ini");


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

	//Update the com port status
	bool CMultiDownloadDlg::UpdateComportCombo(LPCTSTR lpszComport, bool bAdd) 
	{
		int iCom = _wtoi(&lpszComport[3]);
		UINT unCount = 0;

		//Check if the slot is downloading, if yes, return false
		for(int i = 0; i < 8; i++)
		{
			if(m_DownloadFunc[i] != NULL && iCom == m_DownloadFunc[i]->ReturnComPort())  //download port do nothing
				return false;
		}

		if(bAdd) //connect
		{			
			for(int i = 0; i < static_cast<int>(m_unTotalCom); i++)
			{
				if(m_szComArray.GetAt(i) == lpszComport) //already exist
					return false;
			}

			m_unTotalCom++;
			m_szComArray.Add(lpszComport);

			UINT nLoop = 8;
			if(m_unTotalCom < 8 )
				nLoop = m_unTotalCom;

			for(UINT i = 0; i < nLoop; i++)
			{
				CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO1+i);
				unCount = pComboBox->GetCount();
				pComboBox->InsertString(unCount,lpszComport);
				pComboBox->SetItemData(unCount, iCom);

				if(unCount == 0)
				{	
					pComboBox->EnableWindow(TRUE);
					pComboBox->SetCurSel(unCount);
				}

				//if(m_DownloadFunc[i] != NULL)
				//{
				//	WIN32IconButton *pButton = m_DownloadFunc[i]->ReturnButtonWnd();
				//	pButton->SetEnable(false);
				//	if(m_DownloadFunc[i]->ReturnComPort() == iCom)
				//		pComboBox->SetCurSel(unCount);
				//}
				int j = 0;
				unCount++;
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
			if(m_unTotalCom < 9)
			{
				if(!m_pWinStart1->GetEnable() && (m_DownloadFunc[0] == NULL))
					m_pWinStart1->SetEnable(true);
				else if(!m_pWinStart2->GetEnable() && (m_DownloadFunc[1] == NULL))
					m_pWinStart2->SetEnable(true);
				else if(!m_pWinStart3->GetEnable() && (m_DownloadFunc[2] == NULL))
					m_pWinStart3->SetEnable(true);
				else if(!m_pWinStart4->GetEnable() && (m_DownloadFunc[3] == NULL))
					m_pWinStart4->SetEnable(true);
				else if(!m_pWinStart5->GetEnable() && (m_DownloadFunc[4] == NULL))
					m_pWinStart5->SetEnable(true);
				else if(!m_pWinStart6->GetEnable() && (m_DownloadFunc[5] == NULL))
					m_pWinStart6->SetEnable(true);
				else if(!m_pWinStart7->GetEnable() && (m_DownloadFunc[6] == NULL))
					m_pWinStart7->SetEnable(true);
				else if(!m_pWinStart8->GetEnable() && (m_DownloadFunc[7] == NULL))
					m_pWinStart8->SetEnable(true);
				else
					ASSERT(0);
			}

			if((m_unButtonEnNum == 0) && (m_unTotalCom > 1))
				m_pWinStartAll->SetEnable(true);

		}
		else //disconnect
		{
			UINT nLoop = 8;
			if(m_unTotalCom < 8 )
				nLoop = m_unTotalCom;

			for(int i = 0; i < static_cast<int>(m_unTotalCom); i++)
			{
				if(m_szComArray.GetAt(i) == lpszComport)
				{
					m_szComArray.RemoveAt(i);
					m_unTotalCom--;
					break;
				}
			}			

			for(UINT i = 0; i < nLoop; i++)
			{
				CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO1+i);
				int nIndex = pComboBox->FindString(-1, lpszComport);
				int nCurSel = pComboBox->GetCurSel();
				pComboBox->DeleteString(nIndex);

				if(nCurSel == nIndex && m_unTotalCom <8)
				{
					pComboBox->EnableWindow(false);
					while(pComboBox->GetCount() > 0)
						pComboBox->DeleteString(0);
					EnableStartButton(i,false);
				}
				else if(nCurSel == nIndex)
				{
					int nCount = pComboBox->GetCount();
					nIndex = 0;
					while(nIndex < nCount)
					{
						int nCom = static_cast<int>(pComboBox->GetItemData(nIndex));
						int j = 0;
						for(j = 0 ; j < 8; j++)
						{
							CComboBox *pTempComboBox = (CComboBox*)GetDlgItem(IDC_COMBO1 + j);
							if(pTempComboBox->GetCount() != -1)
							{
								int nCom2 = static_cast<int>(pTempComboBox->GetItemData(pTempComboBox->GetCurSel()));
								if(nCom2 == nCom)
									break;
							}
						}
						if(j < 8)
							nIndex++;
						else
						{
							pComboBox->SetCurSel(nIndex);
							break;
						}	
					}
				}
			}
			if((m_unButtonEnNum == 0) && (m_unTotalCom > 1))
				m_pWinStartAll->SetEnable(true);
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

		CDownloadFunc *pDownloadFunc = new CDownloadFunc(0, 0, m_strIni, m_hWnd, NULL, NULL);
		if(pDownloadFunc != NULL)
		{
			pDownloadFunc->Set1ffFile(m_sz1FF);
			pDownloadFunc->DeleteUnzippedFolder(m_strFolderPath.GetBuffer(0));
			delete pDownloadFunc;
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

			CComboBox *pComboBox = (CComboBox*) GetDlgItem(IDC_COMBO9);
			if((szFile != m_sz1FF))
			{
				CDownloadFunc *pDownloadFunc = new CDownloadFunc(0, 0, m_strIni, m_hWnd, NULL, NULL);
				if(pDownloadFunc != NULL)
				{
					pDownloadFunc->Set1ffFile(szFile);
					CString szTemp = pDownloadFunc->CheckLoad();
					if(szTemp == _T(""))
					{
						int nIndex = pComboBox->FindString(-1, m_sz1FF);
						if(nIndex != -1)
							pComboBox->SetCurSel(nIndex);

						MessageBox(_T("Can not Unzip the 1FF file, please check the 1FF file is OK!"));
					}
					else
					{
						int nIndex = pComboBox->FindString(-1, szFile);
						while(nIndex != -1){
							pComboBox->DeleteString(nIndex);
							m_szFileArray.RemoveAt(nIndex);
							nIndex = pComboBox->FindString(-1, szFile);
						}

						m_szFileArray.InsertAt(0, szFile);
						pComboBox->InsertString(0, szFile);
						pComboBox->SetCurSel(0);
						if(pComboBox->GetCount() > 10)
							pComboBox->DeleteString(10);
						m_sz1FF = szFile;
						m_strFolderPath = szTemp;
					}
					delete pDownloadFunc;
				}
			}
		}
	}


	void CMultiDownloadDlg::DownloadFunc(unsigned int unDownloadButtonMask)
	{
		if(m_sz1FF == _T(""))
		//CComboBox* pCombo = (CComboBox*)GetDlgItem(IDC_COMBO9);
		//if(pCombo->GetCurSel() == -1)
		{
			MessageBox( _T("Please select the Source code 1FF file first!"), _T("MultiDownlaod"), MB_OK);
			return;
		}

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
				if(pButton->GetEnable() && (m_DownloadFunc[i] == NULL))
				{
					CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO1 + i);
					UINT nSelect = pComport->GetCurSel();
					if(nSelect == -1)
					{
						MessageBox(_T("Please select the com port first!"));
					}
					else
					{
						UINT nComport =(UINT) pComport->GetItemData(nSelect);
						bool bContinue = true;

						for(int j = 0; j < 8; j++)
						{
							if(j == i)
								continue;
							if(m_DownloadFunc[j] != NULL)
							{
								if(m_DownloadFunc[j]->ReturnComPort() == nComport)
								{
									MessageBox(_T("The comport is used by other thread!"));
									bContinue = false;
									break;
								}
							}
						}

						if(bContinue)
						{
							m_DownloadFunc[i] = new CDownloadFunc(nComport, i, m_strIni, m_hWnd, pButton, pProgress);
							if(m_DownloadFunc[i] != NULL)
							{
								pButton->SetEnable(false);
								pComport->EnableWindow(false);
//								CString szTemp = m_szFileArray.GetAt(pCombo->GetItemData(pCombo->GetCurSel()));
//								m_DownloadFunc[i]->Set1ffFile(szTemp);
								m_DownloadFunc[i]->Set1ffFile(m_sz1FF);
								m_DownloadFunc[i]->SetCallBackFunc(m_pCallBackFunc[i]);
								//if((szTemp != m_sz1FF) || (m_unButtonEnNum == 0))
								//{
								//	m_DownloadFunc[i]->CheckLoad();
								//	m_sz1FF = szTemp;
								//}
								m_unButtonEnNum++;
								AfxBeginThread(DownloadThread, (LPVOID)m_DownloadFunc[i], THREAD_PRIORITY_NORMAL);
							}
							else
								MessageBox(_T("New memory failed!"));
						}
						//if(m_unButtonEnNum == m_unTotalCom)
						//	m_pWinStartAll->SetEnable(false);
					}
				}
			}
		}

		if((m_unButtonEnNum > 6) || ((m_unTotalCom - m_unButtonEnNum) < 2))
			m_pWinStartAll->SetEnable(false);
	}

	bool CMultiDownloadDlg::LogFn(int nIndex,const char* szMsg, int nStep, int nProgress)
	{
		CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1 + nIndex);
		if(pList)
		{
			TCHAR szUnicodeInfor[4096] = {0};
			UINT iUCS2CharCount = MultiByteToWideChar(CP_UTF8,FALSE,(char *)szMsg,-1,szUnicodeInfor,4096);	
			CString szMessage;
			szMessage.Format(_T("%s, Percentage: %d"), szUnicodeInfor, nProgress);
			pList->InsertString(pList->GetCount(), szMessage);
			pList->SetCurSel(pList->GetCount() -1);
			
			if(m_DownloadProgress[nIndex]  != nProgress)
			{
				Win32ProgressBarWidge *pProgressBar = m_DownloadFunc[nIndex]->ReturnProgressWnd();
				if(pProgressBar != NULL)
					pProgressBar->UpdatePercent(nProgress);
				m_DownloadProgress[nIndex] = nProgress;
			}

			CString szLabelInfo = szUnicodeInfor;
			if(szLabelInfo == _T("Download Successful!") || szLabelInfo == _T("Download Failed!"))
			{
				WIN32IconButton *pButton = (WIN32IconButton*)m_DownloadFunc[nIndex]->ReturnButtonWnd();
				pButton->SetEnable(true);
				CComboBox* pComport = (CComboBox*)GetDlgItem(IDC_COMBO1 + nIndex);
				pComport->EnableWindow(true);

				m_unButtonEnNum--;
				m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);

				//delete m_DownloadFunc[nIndex];
				//m_DownloadFunc[nIndex] = NULL;
			}
		}
		return true;
	}

	bool CMultiDownloadDlg::CallBack1(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(0,szMsg, nStep, nProgress);
	}

	bool CMultiDownloadDlg::CallBack2(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(1,szMsg, nStep, nProgress);
	}

	bool CMultiDownloadDlg::CallBack3(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(2,szMsg, nStep, nProgress);
	}

	bool CMultiDownloadDlg::CallBack4(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(3,szMsg, nStep, nProgress);
	}

	bool CMultiDownloadDlg::CallBack5(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(4,szMsg, nStep, nProgress);
	}

	bool CMultiDownloadDlg::CallBack6(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(5,szMsg, nStep, nProgress);
	}

	bool CMultiDownloadDlg::CallBack7(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(6,szMsg, nStep, nProgress);
	}

	bool CMultiDownloadDlg::CallBack8(const char* szMsg, int nStep, int nProgress)
	{
		return g_pThis->LogFn(7,szMsg, nStep, nProgress);
	}

	LRESULT CMultiDownloadDlg::OnDLNotify(WPARAM wParam, LPARAM lParam)
	{
		UINT	nPercentage		= 0;
		UINT	nStep			= 0;
		WCHAR	szErrorMsg[200] = {0};

		m_DownloadFunc[(UINT)lParam]->GetDLTaskInfo((UINT)wParam, szErrorMsg, nStep, nPercentage);

		CListBox *pList = (CListBox*)GetDlgItem(IDC_LIST1 + ((UINT)lParam));
		CString szMessage;
		CString szLabelInfo;

		szMessage.Format(_T("%s, Percentage: %d"), szErrorMsg, nPercentage);

		pList->InsertString(pList->GetCount(), szMessage);
		pList->SetCurSel(pList->GetCount() -1);

		szLabelInfo = szErrorMsg;
		if(szLabelInfo == _T("Download Successful!") || szLabelInfo == _T("Download Failed!"))
		{
			WIN32IconButton *pButton = (WIN32IconButton*)m_DownloadFunc[(UINT)lParam]->ReturnButtonWnd();
			pButton->SetEnable(true);
			m_unButtonEnNum--;
			m_pWinStartAll->SetEnable(m_unButtonEnNum == 0);
			delete m_DownloadFunc[(UINT)lParam];
			m_DownloadFunc[(UINT)lParam] = NULL;
		}

		return 0;
	}

	bool CMultiDownloadDlg::EnableStartButton(int nStartButtonIndex,bool bEnable)
	{
		bool bRes = true;
		switch(nStartButtonIndex)
		{
		case 0:
			m_pWinStart1->SetEnable(false);
			break;
		case 1:
			m_pWinStart2->SetEnable(false);
			break;
		case 2:
			m_pWinStart3->SetEnable(false);
			break;
		case 3:
			m_pWinStart4->SetEnable(false);
			break;
		case 4:
			m_pWinStart5->SetEnable(false);
			break;
		case 5:
			m_pWinStart6->SetEnable(false);
			break;
		case 6:
			m_pWinStart7->SetEnable(false);
			break;
		case 7:
			m_pWinStart8->SetEnable(false);
			break;
		default:
			ASSERT(false);
			bRes = false;
			break;
		}
		return bRes;
	}
}