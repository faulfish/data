// 1FFMakerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "1FFMakerUI.h"
#include "1FFMakerDlg.h"
#include "../../CommonUtil/UnicodeUtility.h"
#include "../QFlash/QZip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMy1FFMakerDlg dialog




CMy1FFMakerDlg::CMy1FFMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy1FFMakerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy1FFMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Cust_Model, m_Cust_ModelPath);
	DDX_Control(pDX, IDC_EDIT1, m_FolderPath);
	DDX_Control(pDX, IDC_EDITZipPath, m_ZipPath);
	DDX_Control(pDX, IDC_BUTTON1, m_BtnSelectFolder);
	DDX_Control(pDX, IDOK, m_BtnMake);
	DDX_Control(pDX, IDCLOSE, m_BtnClose);
	DDX_Control(pDX, IDC_BtnZipPath, m_BtnZipExe);
}

BEGIN_MESSAGE_MAP(CMy1FFMakerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CMy1FFMakerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CMy1FFMakerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCLOSE, &CMy1FFMakerDlg::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BtnZipPath, &CMy1FFMakerDlg::OnBnClickedBtnzippath)
END_MESSAGE_MAP()


// CMy1FFMakerDlg message handlers

BOOL CMy1FFMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Cust_ModelPath.SetWindowText(_T("WellFleet_ATHB1A"));

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy1FFMakerDlg::OnPaint()
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
HCURSOR CMy1FFMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




bool CMy1FFMakerDlg::GetFolder(TSTRING& strFolderPath, const TCHAR* szCaption, HWND hOwner)
{
	bool retVal = false;

	// The BROWSEINFO struct tells the shell 
	// how it should display the dialog.
	BROWSEINFO bi;
	memset(&bi, 0, sizeof(bi));

	bi.ulFlags   = BIF_USENEWUI;
	bi.hwndOwner = hOwner;
	bi.lpszTitle = szCaption;

	// must call this if using BIF_USENEWUI
	::OleInitialize(NULL);

	// Show the dialog and get the itemIDList for the selected folder.
	LPITEMIDLIST pIDL = ::SHBrowseForFolder(&bi);

	if(pIDL != NULL)
	{
		// Create a buffer to store the path, then get the path.
		TCHAR buffer[_MAX_PATH] = {'\0'};
		if(::SHGetPathFromIDList(pIDL, buffer) != 0)
		{
			// Set the string value.
			strFolderPath = buffer;
			retVal = true;
		}		

		// free the item id list
		CoTaskMemFree(pIDL);
	}

	::OleUninitialize();

	return retVal;
}



bool CMy1FFMakerDlg::FindFiles(const TSTRING& strFolderPath)
{
	bool bRes = true;
	TSTRING strDirPath = strFolderPath + _T("\\*"); // searching all files

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strDirPath.c_str(),&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		bRes = false;

	bool bSearch = true;
	m_NameMaker.ClearFileNames();
	while(bSearch && bRes) 
	{
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) 
				continue;
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //recursively
				AfxMessageBox(_T("This folder contains another folder.\nWe'll skip it!"));
			else
				m_NameMaker.AddFileName(FindFileData.cFileName);
		}
		else 
		{	
			if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
				bSearch = false;
			else 
				bRes = false;
		}
	}
	FindClose(hFind);
	ASSERT(bRes);
	return bRes;
}

bool CMy1FFMakerDlg::IsDots(const TCHAR* str) 
{
	bool bRes = true;
	if(_tcscmp(str,_T(".")) && _tcscmp(str,_T(".."))) 
		bRes = false;
	return bRes;
}

void CMy1FFMakerDlg::OnBnClickedButton1() //Select Folder
{
	m_BtnSelectFolder.EnableWindow(FALSE);
	m_BtnMake.EnableWindow(FALSE);
	m_BtnClose.EnableWindow(FALSE);
	m_BtnZipExe.EnableWindow(FALSE);
	TSTRING strPath;
	if (GetFolder(strPath, _T("Please select a folder"))) 
	{
		m_FolderPath.SetWindowText(strPath.c_str());
	}
	m_BtnZipExe.EnableWindow(TRUE);
	m_BtnSelectFolder.EnableWindow(TRUE);
	m_BtnMake.EnableWindow(TRUE);
	m_BtnClose.EnableWindow(TRUE);
}

void CMy1FFMakerDlg::OnBnClickedOk() //Make
{
	CString strFolderPath, strZipExePath, strCustModel;
	m_FolderPath.GetWindowText(strFolderPath);
	m_ZipPath.GetWindowText(strZipExePath);
	m_Cust_ModelPath.GetWindowText(strCustModel);
	if (strFolderPath.IsEmpty() || strZipExePath.IsEmpty())
	{
		AfxMessageBox(_T("Please input a folder path."));
	}
	else
	{
		TSTRING strFilePath;
		if(FindFiles((TSTRING)strFolderPath))
		{
			m_NameMaker.SetMarketName((TSTRING)strCustModel);
			if(m_NameMaker.GetMotName(strFilePath))
			{
				ODMTMA_LIB::CQZip QZip;

				QZip.SetZipExePath((TSTRING)strZipExePath);
				bool bZip = QZip.Zip(strFilePath,(TSTRING)strFolderPath);
				//OnOK();
			}
			

			//QZip.Unzip(strFilePath);
			
		}
	}
}

void CMy1FFMakerDlg::OnBnClickedClose()
{
	OnCancel();
}

void CMy1FFMakerDlg::OnBnClickedBtnzippath()
{
	m_BtnSelectFolder.EnableWindow(FALSE);
	m_BtnMake.EnableWindow(FALSE);
	m_BtnClose.EnableWindow(FALSE);
	m_BtnZipExe.EnableWindow(FALSE);
	CFileDialog FileDlg(TRUE);
	TSTRING strZipPath;
	if(IDOK == FileDlg.DoModal())
	{
		m_ZipPath.SetWindowText(FileDlg.GetPathName());
	}
	m_BtnZipExe.EnableWindow(TRUE);
	m_BtnSelectFolder.EnableWindow(TRUE);
	m_BtnMake.EnableWindow(TRUE);
	m_BtnClose.EnableWindow(TRUE);

}
