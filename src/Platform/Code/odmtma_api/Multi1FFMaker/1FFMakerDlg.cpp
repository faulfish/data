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
	DDX_Control(pDX, IDC_Cust_Model, m_Cust_ModelName);

	DDX_Control(pDX, IDC_SWLoadPath, m_SWLoadPath);
	DDX_Control(pDX, IDC_BtnSWLoadPath, m_BtnSWLoadPath);
	DDX_Control(pDX, IDC_VariantPath, m_VariantPath);
	DDX_Control(pDX, IDC_BtnVariantPath, m_BtnVariantPath);
	DDX_Control(pDX, IDC_LockFilesPath, m_LockFilesPath);
	DDX_Control(pDX, IDC_BtnLockFilesPath, m_BtnLockFilesPath);
	DDX_Control(pDX, IDC_OutputFolder, m_OutputFolder);
	DDX_Control(pDX, IDC_BtnOutputFolder, m_BtnOutputFolder);
	DDX_Control(pDX, IDC_Status, m_StatusText);

	DDX_Control(pDX, IDOK, m_BtnMake);
	DDX_Control(pDX, IDCLOSE, m_BtnClose);
}

BEGIN_MESSAGE_MAP(CMy1FFMakerDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BtnSWLoadPath, &CMy1FFMakerDlg::OnBnClickedSWLoadPath)
	ON_BN_CLICKED(IDC_BtnVariantPath, &CMy1FFMakerDlg::OnBnClickedVariantPath)
	ON_BN_CLICKED(IDC_BtnLockFilesPath, &CMy1FFMakerDlg::OnBnClickedLockFilesPath)
	ON_BN_CLICKED(IDC_BtnOutputFolder, &CMy1FFMakerDlg::OnBnClickedOutputFolder)

	ON_BN_CLICKED(IDC_SetLockFile, &CMy1FFMakerDlg::OnBnClickedSetLockFile)
	ON_BN_CLICKED(IDOK, &CMy1FFMakerDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCLOSE, &CMy1FFMakerDlg::OnBnClickedClose)
	//ON_BN_CLICKED(IDC_BtnZipPath, &CMy1FFMakerDlg::OnBnClickedBtnzippath)
END_MESSAGE_MAP()

// CMy1FFMakerDlg message handlers

BOOL CMy1FFMakerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	UpdateStatusText(_T("Program start."));

#ifdef _DEBUG
	m_strIniFilePath = _T("..\\debug");
#else
	m_strIniFilePath = _T(".\\");
#endif

#ifdef Lavernock
	m_strIniFilePath += _T("\\Multi1FFMakerSetting_Lavernock.ini");
#endif

#ifdef Melos
	m_strIniFilePath += _T("\\Multi1FFMakerSetting_Melos.ini");
	GetDlgItem(IDC_SetLockFile)->EnableWindow(FALSE);
	GetDlgItem(IDC_LockFilesPath)->EnableWindow(FALSE);
	GetDlgItem(IDC_BtnLockFilesPath)->EnableWindow(FALSE);
	m_BtnMake.EnableWindow();
#endif

	m_NameMaker.SetNamingRuleINIFilePath(m_strIniFilePath);

	//記錄zip.exe的路徑
	TCHAR szPath[MAX_PATH];
	::GetCurrentDirectory(MAX_PATH, szPath); 
	m_strtZipExeFullPath = szPath;
#ifdef _DEBUG
	m_strtZipExeFullPath += _T("\\..\\debug");
#endif
	m_strtZipExeFullPath += _T("\\zip.exe");

	TCHAR cpModelName[32];
	DWORD lRetValue = ::GetPrivateProfileString(_T("Name"),_T("ProjectName_ModelName"),NULL,cpModelName,32,m_strIniFilePath);
	ASSERT(lRetValue != 0);
	m_strProjectName_ModelName = cpModelName;
	m_Cust_ModelName.SetWindowText(m_strProjectName_ModelName);
	GetINISetting(m_strIniFilePath, _T("Path"), &m_mapPath);
	InitialLog();
	m_SWLoadPath.SetWindowText(m_mapPath[_T("SWFolder")].c_str());
	m_VariantPath.SetWindowText(m_mapPath[_T("VariantFolder")].c_str());
	m_LockFilesPath.SetWindowText(m_mapPath[_T("LockFolder")].c_str());
	m_OutputFolder.SetWindowText(m_mapPath[_T("OutputPath")].c_str());
	GetINISetting(m_strIniFilePath, _T("SWHeader"), &m_mapSWHeader);
	GetINISetting(m_strIniFilePath, _T("SWExtension"), &m_mapSWExtension);
	GetINISetting(m_strIniFilePath, _T("VariantExtension"), &m_mapVariantExtension);
#ifdef Lavernock
	GetINISetting(m_strIniFilePath, _T("LockExtension"), &m_mapLockExtension);
#endif
	GetINISetting(m_strIniFilePath, _T("CheckNameExtensionNumber"), &m_mapCheckNameExtensionNumber);

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMy1FFMakerDlg::UpdateStatusText(const CString& strStatusText)
{
	m_StatusText.SetWindowText(strStatusText);
}

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
{ //將所有檔案名稱寫入m_NameMaker, 以便生成mot檔名
	bool bRes = true;
	TSTRING strDirPath = strFolderPath + _T("\\*"); // searching all files

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strDirPath.c_str(),&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		bRes = false;

	map<TSTRING, int> mapNameExtensionNumber;
	bool bSearch = true;
	m_NameMaker.ClearFileNames();
	while(bSearch && bRes) 
	{
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) 
				continue;
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) 
				AfxMessageBox(_T("This folder contains another folder.\nWe'll skip it!"));
			else
			{
				m_NameMaker.AddFileName(FindFileData.cFileName);

				CString strFileFullName = FindFileData.cFileName;
				TSTRING strFileNameExtension = strFileFullName.Mid(strFileFullName.ReverseFind(_T('.')) +1 );
				if(mapNameExtensionNumber.find(strFileNameExtension) != mapNameExtensionNumber.end())
					mapNameExtensionNumber[strFileNameExtension] += 1;
				else
					mapNameExtensionNumber[strFileNameExtension] = 1;
			}
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
	
	if(bRes)
	{
		for(map<TSTRING, int>::iterator iter = m_mapCheckNameExtensionNumber.begin(); bRes && iter != m_mapCheckNameExtensionNumber.end(); ++iter)
		{
				bRes = mapNameExtensionNumber[iter->first] == iter->second;
				if(bRes == false)
				{
					CString strLog = _T("The number of ");
					strLog += iter->first.c_str();
					strLog +=  _T(" files is not match the [CheckNameExtensionNumber] setting in Multi1FFMakerSetting.ini.");
					WriteLog(strLog);
				}
		}
	}
	return bRes;
}

bool CMy1FFMakerDlg::IsDots(const TCHAR* str) 
{
	bool bRes = true;
	if(_tcscmp(str,_T(".")) && _tcscmp(str,_T(".."))) 
		bRes = false;
	return bRes;
}

//void CMy1FFMakerDlg::OnBnClickedButton1() //Select Folder
//{
//	m_BtnSelectFolder.EnableWindow(FALSE);
//	m_BtnMake.EnableWindow(FALSE);
//	m_BtnClose.EnableWindow(FALSE);
//	TSTRING strPath;
//	if (GetFolder(strPath, _T("Please select a folder"))) 
//	{
//		m_FolderPath.SetWindowText(strPath.c_str());
//	}
//	m_BtnSelectFolder.EnableWindow(TRUE);
//	m_BtnMake.EnableWindow(TRUE);
//	m_BtnClose.EnableWindow(TRUE);
//}
//
void CMy1FFMakerDlg::OnBnClickedSWLoadPath() //Select Folder
{
	TSTRING strPath;
	if (GetFolder(strPath, _T("Please select a folder"))) 
	{
		m_SWLoadPath.SetWindowText(strPath.c_str());
	}
}

void CMy1FFMakerDlg::OnBnClickedVariantPath() //Select Folder
{
	TSTRING strPath;
	if (GetFolder(strPath, _T("Please select a folder"))) 
		m_VariantPath.SetWindowText(strPath.c_str());
}

void CMy1FFMakerDlg::OnBnClickedLockFilesPath() //Select Folder
{
	TSTRING strPath;
	if (GetFolder(strPath, _T("Please select a folder"))) 
		m_LockFilesPath.SetWindowText(strPath.c_str());
}

void CMy1FFMakerDlg::OnBnClickedOutputFolder() //Select Folder
{
	TSTRING strPath;
	if (GetFolder(strPath, _T("Please select a folder"))) 
		m_OutputFolder.SetWindowText(strPath.c_str());
}

void CMy1FFMakerDlg::OnBnClickedOk() //Make
{
	CWaitCursor wait;
	SaveINISetting()	;

	if(_taccess_s(m_strtZipExeFullPath.c_str(),0) != 0)
	{
		WriteLog(_T("The file zip.exe is not found."));
		AfxMessageBox(_T("Please check the zip execution file."));
	}
	else
	{	//建立一個temp folder, 把SW load放進去
		//然後把variant, lock files放進去, zip
		//再delete variant, lock files, 換下一組, loop
		//全部完成後再清空此folder
		WriteLog(_T("\r\n"));
		WriteLog(_T("Create Mot start."));

		m_Cust_ModelName.GetWindowText(m_strProjectName_ModelName);

		TSTRING strTempFolderPath;

		if(m_mapPath[_T("TempFolder")].empty() == false)
			strTempFolderPath = m_mapPath[_T("TempFolder")];
		else
		{
			TCHAR szTempPath[MAX_PATH] = {0};
			SHGetFolderPath(GetSafeHwnd(), CSIDL_COMMON_TEMPLATES, NULL, NULL, szTempPath);
			strTempFolderPath = szTempPath;
			strTempFolderPath += _T("\\MotFiles");
		}
		bool bClearFolder = ClearFolder(strTempFolderPath);
		ASSERT(bClearFolder);

		bool bCopySWLoad = CopySWLoad(strTempFolderPath.c_str());
		if(bCopySWLoad == false)
			AfxMessageBox(_T("Copy SW load failed, see the Log.txt.\n If the error number is 3, please set the temp folder in Multi1FFMakerSetting.ini."));
		else
		{
			bool bCreateMotFile = true;

#ifdef Lavernock // Lavernock 要特別處理Loc檔
			for (map<TSTRING, TSTRING>::iterator iterVariantToLock = m_mapVariantToLockFile.begin(); bCreateMotFile && iterVariantToLock != m_mapVariantToLockFile.end(); ++iterVariantToLock)
			{
				bCreateMotFile = CreateOneMotFile(strTempFolderPath, iterVariantToLock->first, iterVariantToLock->second);
			}
#endif

#ifdef Melos //Melos 沒有loc檔
			const vector<CString> vecVariantFileName = m_SetLockFileDlg.GetVariantFiles(m_mapPath[_T("VariantFolder")], m_mapVariantExtension);
			for (int nIndex = 0; bCreateMotFile && nIndex < vecVariantFileName.size(); ++nIndex)
			{
				bCreateMotFile = CreateOneMotFile(strTempFolderPath, (TSTRING)(vecVariantFileName.at(nIndex)), _T("No loc"));
			}
#endif

			if (bCreateMotFile)
				AfxMessageBox(_T("Create Mot files succeeded."));
			else
				AfxMessageBox(_T("Create Mot files failed.\n Please see Log.txt"));
		}
		WriteLog(_T("Create Mot end."));
	}	
}
void CMy1FFMakerDlg::SaveINISetting()
{
	m_Cust_ModelName.GetWindowText(m_strProjectName_ModelName);
	WritePrivateProfileString(_T("Name"), _T("ProjectName_ModelName"), m_strProjectName_ModelName, m_strIniFilePath);

	//Save path:
	CString strSWLoadPath;
	m_SWLoadPath.GetWindowText(strSWLoadPath);
	 WritePrivateProfileString(_T("Path"), _T("SWFolder"), strSWLoadPath, m_strIniFilePath);
	 CString strVariantPath;
	 m_VariantPath.GetWindowText(strVariantPath);
	 WritePrivateProfileString(_T("Path"), _T("VariantFolder"), strVariantPath, m_strIniFilePath);
	 CString strLockFilesPath;
	 m_LockFilesPath.GetWindowText(strLockFilesPath);
	 WritePrivateProfileString(_T("Path"), _T("LockFolder"), strLockFilesPath, m_strIniFilePath);
	 CString strOutputFolder;
	 m_OutputFolder.GetWindowText(strOutputFolder);
	 WritePrivateProfileString(_T("Path"), _T("OutputPath"), strOutputFolder, m_strIniFilePath);

	 //還有很多要save
}
bool CMy1FFMakerDlg::IsVariantNameLegal(const CString& strVariantFileName)
{
	//抓副檔名
	int nDotPos = strVariantFileName.ReverseFind(_T('.'));
	if(nDotPos == -1)
		return false; //沒有副檔名

	bool bRes = false;
	TSTRING strVariantFileNameExtension = (TSTRING)strVariantFileName.Mid(nDotPos+1);
	for (map<TSTRING, TSTRING>::iterator iter = m_mapSWExtension.begin(); !bRes && iter != m_mapSWExtension.end(); ++iter)
	{
		if(bRes = strVariantFileNameExtension == iter->second) //副檔名符合
			bRes = m_mapSWHeader[iter->first] == (TSTRING)strVariantFileName.Left((int)m_mapSWHeader[iter->first].length());//檢查檔頭
	}

	return bRes;
}

bool CMy1FFMakerDlg::CopySWLoad(const CString& strTargetFolderPath)
{
	bool bRes = true;
	CString SWLoadPath;
	m_SWLoadPath.GetWindowText(SWLoadPath);

	CString strSearchFolderPath = SWLoadPath; // searching all files
	strSearchFolderPath += _T("\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strSearchFolderPath, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
	{
		bRes = false;
		WriteLog(_T("SW load path error"));
	}

	bool bSearch = true;
	while(bSearch && bRes)
	{
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) 
				continue;
			else if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //recursively
			{
				AfxMessageBox(_T("The SWLoad folder contains a subfolder.\nWe'll skip it."));
			}				
			else if (IsVariantNameLegal(FindFileData.cFileName) == false) // 檢查檔頭與副檔名
				continue;
			else
			{
				CString strExistingFileFullPath = SWLoadPath + _T("\\") + FindFileData.cFileName;
				CString strNewFileFullPath = strTargetFolderPath + _T("\\") + FindFileData.cFileName;
				BOOL bCopyFile = CopyFile(strExistingFileFullPath, strNewFileFullPath, TRUE);
				if(bCopyFile == 0)
				{
					bRes = false;
					DWORD swLastError = GetLastError();
					CString strLog;
					strLog.Format(_T("SW load path error, error number: %d"), swLastError);
					WriteLog(strLog);
					ASSERT(swLastError);
				}
				else
				{//去掉唯讀屬性
					DWORD dwFileAttributes = GetFileAttributes(strNewFileFullPath);
					if(dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					{
						bRes = SetFileAttributes(strNewFileFullPath, dwFileAttributes - FILE_ATTRIBUTE_READONLY) != 0;
						CString strLog;
						strLog.Format(_T("The file %s is ReadOnly, we remove this attribute."), FindFileData.cFileName);
						WriteLog(strLog);
					}
				}
			}
		}
		else 
		{	
			if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
				bSearch = false;
			else 
			{
				bRes = false;
				WriteLog(_T("Unknow error while copying SW load."));
			}
		}
	}
	if (bRes)
		WriteLog(_T("Copy SW load succeeded."));
	else
		WriteLog(_T("Copy SW load failed."));
	return bRes;
}

bool CMy1FFMakerDlg::ClearFolder(const TSTRING& strFolderPath) //recursively delete files
{//若有唯讀屬性的檔案, 則移除會失敗
	bool bRes = true;

	CString strSearchFolderPath = strFolderPath.c_str(); // searching all files
	strSearchFolderPath += _T("\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strSearchFolderPath, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		bRes = false;

	vector<TSTRING> vecFileName;
	bool bSearch = true;
	while(bSearch && bRes)
	{
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) 
				continue;
			else if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				TSTRING strSubFolderPath = strFolderPath + _T("\\") + FindFileData.cFileName;
				bRes = ClearFolder(strSubFolderPath);
				ASSERT(bRes);
				if(bRes)
					bRes = (RemoveDirectory(strSubFolderPath.c_str()) != 0);
				ASSERT(bRes);
			}				
			else
			{
				TSTRING strFileFullName = strFolderPath + _T("\\") + FindFileData.cFileName;
				vecFileName.push_back(strFileFullName);
			}
		}
		else 
		{	
			if(GetLastError() == ERROR_NO_MORE_FILES) // no more files there
				bSearch = false;
			else 
				bRes = false;
		}
	} // end while

	//delete files
	for(size_t nIndex = 0; bRes && nIndex < vecFileName.size(); ++nIndex)
		bRes = (DeleteFile(vecFileName.at(nIndex).c_str()) != 0);
	
	FindClose(hFind);
	ASSERT(bRes);
	return bRes;
}

bool CMy1FFMakerDlg::CreateOneMotFile(const TSTRING& strFolderPath,const TSTRING& strVariantName, const TSTRING& strLockFileName)
{
	bool bRes = true;
	for(map<TSTRING, TSTRING>::iterator iterExten = m_mapVariantExtension.begin(); bRes && iterExten!= m_mapVariantExtension.end(); ++iterExten) {
		CString strNewFileFullPath = (strFolderPath + _T("\\") + strVariantName + _T(".") + iterExten->second).c_str();
		bRes = CopyFile(
		(m_mapPath[_T("VariantFolder")] + _T("\\") + strVariantName + _T(".") + iterExten->second).c_str()
		, strNewFileFullPath
		, TRUE) != 0;
		if(bRes)
		{//去掉唯讀屬性
			DWORD dwFileAttributes = GetFileAttributes(strNewFileFullPath);
			if(dwFileAttributes & FILE_ATTRIBUTE_READONLY)
			{
				SetFileAttributes(strNewFileFullPath, dwFileAttributes - FILE_ATTRIBUTE_READONLY);
				CString strLog;
				strLog.Format(_T("The file %s is ReadOnly, we remove this attribute."), (strVariantName + _T(".") + iterExten->second).c_str());
				WriteLog(strLog);
			}
		}
	}
	if (strLockFileName != _T("No loc"))
	{
		for(map<TSTRING, TSTRING>::iterator iterExten = m_mapLockExtension.begin(); bRes && iterExten!= m_mapLockExtension.end(); ++iterExten) {
			CString strNewFileFullPath = (strFolderPath + _T("\\") + strLockFileName + _T(".") + iterExten->second).c_str();
			bRes = CopyFile(
				(m_mapPath[_T("LockFolder")] + _T("\\") + strLockFileName + _T(".") + iterExten->second).c_str()
				, strNewFileFullPath
				, TRUE) != 0;
			if(bRes)
			{//去掉唯讀屬性
				DWORD dwFileAttributes = GetFileAttributes(strNewFileFullPath);
				if(dwFileAttributes & FILE_ATTRIBUTE_READONLY)
				{
					SetFileAttributes(strNewFileFullPath, dwFileAttributes - FILE_ATTRIBUTE_READONLY);
					CString strLog;
					strLog.Format(_T("The file %s is ReadOnly, we remove this attribute."), (strLockFileName + _T(".") + iterExten->second).c_str());
					WriteLog(strLog);
				}
			}
		}
	}
	ASSERT(bRes);

	TSTRING strMotFileFullName;
	if(bRes)
		bRes = FindFiles((TSTRING)strFolderPath);	 //確認現在有哪些檔案

	if(bRes)
	{
		m_NameMaker.SetMarketName((TSTRING)m_strProjectName_ModelName);
		if(m_NameMaker.GetMotName_3(strMotFileFullName))
		{
			CString strLog;
			strLog.Format(_T("Create mot file %s start."), strMotFileFullName.c_str());
			WriteLog(strLog);
			ODMTMA_LIB::CQZip QZip;

			TCHAR szPath[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH, szPath); //記錄目前的預設路徑
			QZip.SetZipExePath(m_strtZipExeFullPath);
			bool bZip = QZip.Zip(strMotFileFullName,(TSTRING)strFolderPath);
			::SetCurrentDirectory(szPath); //把預設路徑存回去，這樣做是因為CQZip會改變預設路徑
			if(bZip)
			{
				CString MotFullPath = (strFolderPath + _T("\\") + strMotFileFullName).c_str();
				int n = 0;
				while(_taccess_s(MotFullPath,0) != 0 && bRes) 
				{
					Sleep(500);
					n += 500;
					bRes =  n < 15000; //十秒還壓不完, 一定有問題
				}
				if(bRes)
					bRes = MoveFile(
					MotFullPath
					, (m_mapPath[_T("OutputPath")] + _T("\\") + strMotFileFullName).c_str()
					) != 0;
				else	WriteLog(_T("Zip mot file fail."));
			}
			else	WriteLog(_T("Zip mot file fail."));
		}
		else	{WriteLog(_T("Get mot file name fail, please check the naming rule.")); bRes = false;}
	}

	//清除temp資料夾下的 Variant files , lock files 和 mot file.
	for(map<TSTRING, TSTRING>::iterator iterExten = m_mapVariantExtension.begin(); bRes && iterExten!= m_mapVariantExtension.end(); ++iterExten)
		bRes = DeleteFile((strFolderPath + _T("\\") + strVariantName + _T(".") + iterExten->second).c_str()) != 0;
	if (strLockFileName != _T("No loc"))
		for(map<TSTRING, TSTRING>::iterator iterExten = m_mapLockExtension.begin(); bRes && iterExten!= m_mapLockExtension.end(); ++iterExten)
			bRes = DeleteFile((strFolderPath + _T("\\") + strLockFileName + _T(".") + iterExten->second).c_str()) != 0;

	CString strLog;
	if (bRes)
		strLog.Format(_T("Create file %s succeeded."), strMotFileFullName.c_str());
	else
		strLog.Format(_T("Create file %s failed."), strMotFileFullName.c_str());
	WriteLog(strLog);
	return bRes;
}

void CMy1FFMakerDlg::OnBnClickedClose()
{
	OnCancel();
}
void CMy1FFMakerDlg::SavePathMapByEditBox()
{
	CString strSWLoadPath;
	m_SWLoadPath.GetWindowText(strSWLoadPath);
	m_mapPath[_T("SWFolder")] = (TSTRING)strSWLoadPath;
	CString strVariantPath;
	m_VariantPath.GetWindowText(strVariantPath);
	m_mapPath[_T("VariantFolder")] = (TSTRING)strVariantPath;
	CString strLockFilesPath;
	m_LockFilesPath.GetWindowText(strLockFilesPath);
	m_mapPath[_T("LockFolder")] = (TSTRING)strLockFilesPath;
	CString strOutputFolder;
	m_OutputFolder.GetWindowText(strOutputFolder);
	m_mapPath[_T("OutputPath")] = (TSTRING)strOutputFolder;
}

void CMy1FFMakerDlg::OnBnClickedSetLockFile()
{
	SavePathMapByEditBox();
	INT_PTR nDlgRet = m_SetLockFileDlg.DoModal(m_mapPath[_T("VariantFolder")].c_str(), m_mapPath[_T("LockFolder")].c_str(), m_mapVariantExtension, m_mapLockExtension);
	if(nDlgRet == 1)
	{
		m_mapVariantToLockFile = m_SetLockFileDlg.GetVariantToLockFileMap();
		m_BtnMake.EnableWindow( );
	}
}
//void CMy1FFMakerDlg::OnBnClickedBtnzippath()
//{
//	m_BtnSelectFolder.EnableWindow(FALSE);
//	m_BtnMake.EnableWindow(FALSE);
//	m_BtnClose.EnableWindow(FALSE);
//	//m_BtnZipExe.EnableWindow(FALSE);
//	CFileDialog FileDlg(TRUE);
//	TSTRING strZipPath;
//	if(IDOK == FileDlg.DoModal())
//	{
//		m_ZipPath.SetWindowText(FileDlg.GetPathName());
//	}
//	//m_BtnZipExe.EnableWindow(TRUE);
//	m_BtnSelectFolder.EnableWindow(TRUE);
//	m_BtnMake.EnableWindow(TRUE);
//	m_BtnClose.EnableWindow(TRUE);
//}
//
void CMy1FFMakerDlg::GetINISetting(const CString& strINIFileName,const TSTRING& strSection, map<TSTRING, TSTRING>* pMapSectionContent)
{
	pMapSectionContent->clear();
	const DWORD dwBufferSize = 10240;
	TCHAR ac_Result[dwBufferSize] = {0};  //change size depending on needs	
	DWORD lRetValue = ::GetPrivateProfileSection(strSection.c_str(), ac_Result, dwBufferSize,strINIFileName);
	if (lRetValue == 0)
	{
		ASSERT(0);
	}
	if(lRetValue == (dwBufferSize-2))
	{
		ASSERT(0);//need to expand buffer size
	}

	TSTRING strKey;
	TSTRING strValue;
	bool bKeyFlag = true;
	for(DWORD i=0; i<lRetValue; ++i)
	{
		if(ac_Result[i] != _T('\0')) 
		{
			if (ac_Result[i] == _T('='))
			{
				bKeyFlag = false;
				continue;
			}
			(bKeyFlag ? strKey : strValue) += ac_Result[i] ;
		} 
		else 
		{
			bKeyFlag = true;
			pMapSectionContent->insert( pair<TSTRING, TSTRING>(strKey, strValue) );
			strKey.clear();
			strValue.clear();
		}
	}
}

void CMy1FFMakerDlg::GetINISetting(const CString& strINIFileName,const TSTRING& strSection, map<TSTRING, int>* pMapSectionContent)
{
	map<TSTRING, TSTRING> mapTemp;
	GetINISetting(strINIFileName, strSection, &mapTemp);
	for(map<TSTRING, TSTRING>::iterator iter = mapTemp.begin(); iter != mapTemp.end(); ++iter)
		pMapSectionContent->insert( pair<TSTRING, int>(iter->first, _ttoi(iter->second.c_str())) );
}


void CMy1FFMakerDlg::InitialLog()
{
	if (_taccess_s(m_mapPath[_T("Log")].c_str(),0) == 0)
	{
		BOOL bDelete = DeleteFile(m_mapPath[_T("Log")].c_str());
		ASSERT(bDelete);
	}
	CFile cfile(m_mapPath[_T("Log")].c_str(), CFile::modeCreate | CFile::modeWrite);
	COleDateTime oleCurDT(COleDateTime::GetCurrentTime());
	CString strOutput;
	strOutput.Format(_T("[%d/%d %d:%d:%d] Program start.\r\n"), oleCurDT.GetMonth(), oleCurDT.GetDay(), oleCurDT.GetHour(), oleCurDT.GetMinute(), oleCurDT.GetSecond());
	cfile.Write((LPVOID)(strOutput.GetString()), strOutput.GetLength() );
	cfile.Flush();
	cfile.Close();
}
void CMy1FFMakerDlg::WriteLog(const CString& strLog)
{
	COleDateTime oleCurDT(COleDateTime::GetCurrentTime());

	CString strOutput;
	strOutput.Format(_T("[%d/%d %d:%d:%d] %s\r\n"), oleCurDT.GetMonth(), oleCurDT.GetDay(), oleCurDT.GetHour(), oleCurDT.GetMinute(), oleCurDT.GetSecond(), strLog);

	CFile cfile(m_mapPath[_T("Log")].c_str(), CFile::modeWrite);
	cfile.SeekToEnd();
	cfile.Write((LPVOID)(strOutput.GetString()), strOutput.GetLength() );
	cfile.Flush();
	cfile.Close();
}
