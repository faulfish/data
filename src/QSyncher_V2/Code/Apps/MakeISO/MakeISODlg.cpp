// MakeISODlg.cpp : implementation file
//

#include "stdafx.h"
#include "MakeISO.h"
#include "MakeISODlg.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include "..\..\CommonUtil\unicodeutility.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMakeISODlg dialog




CMakeISODlg::CMakeISODlg(CWnd* pParent /*=NULL*/)
: CDialog(CMakeISODlg::IDD, pParent),m_IConfigure(QSYNC_LIB::CreateIConfigure(_T("MakeISO")))
,m_strSource(m_IConfigure,_T("SourceFolder"))
,m_strTarget(m_IConfigure,_T("TargetFolder"))
,m_strCodeName(m_IConfigure,_T("CodeName"))
,m_strProjectName(m_IConfigure,_T("ProjectName"))
,m_strProjectVersion(m_IConfigure,_T("ProjectVersion"))
{
	m_IConfigure->Load(QSYNC_LIB::STORAGE_INIFILE);
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMakeISODlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMakeISODlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SameFolder_CHECK, &CMakeISODlg::OnBnClickedSamefolderCheck)
	ON_CONTROL_RANGE(BN_CLICKED,IDC_Folder_BUTTON,IDC_Folder_BUTTON2,&CMakeISODlg::OnBnClickedFolderButton)
	ON_EN_CHANGE(IDC_Format_EDIT, &CMakeISODlg::MakeSample)
	ON_BN_CLICKED(IDC_Exec_BUTTON, &CMakeISODlg::OnBnClickedExecButton)
	ON_BN_CLICKED(IDC_Rename_BUTTON, &CMakeISODlg::OnBnClickedRenameButton)
	ON_EN_CHANGE(IDC_SoureFolder_EDIT, &CMakeISODlg::OnEnChangeSourefolderEdit)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_TargetFolder_EDIT, &CMakeISODlg::OnEnChangeTargetfolderEdit)
	ON_EN_CHANGE(IDC_CodeName_EDIT, &CMakeISODlg::OnEnChangeCodenameEdit)
	ON_EN_CHANGE(IDC_ProjectName_EDIT, &CMakeISODlg::OnEnChangeProjectnameEdit)
	ON_EN_CHANGE(IDC_ProjectVersion_EDIT, &CMakeISODlg::OnEnChangeProjectversionEdit)
END_MESSAGE_MAP()


// CMakeISODlg message handlers

BOOL CMakeISODlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here	
	SetString(IDC_SoureFolder_EDIT,m_strSource.Get().c_str());
	SetString(IDC_TargetFolder_EDIT,m_strTarget.Get().c_str());
	SetString(IDC_CodeName_EDIT,m_strCodeName.Get().c_str());
	SetString(IDC_ProjectName_EDIT,m_strProjectName.Get().c_str());
	SetString(IDC_ProjectVersion_EDIT,m_strProjectVersion.Get().c_str());

	SetString(IDC_Format_EDIT,_T("%c_%p%d_%v_%s.ISO"));
	SetCheck(IDC_RemoveOriginal_CHECK,true);

	SetCheck(IDC_SameFolder_CHECK,true);
	OnBnClickedSamefolderCheck();

	DeleteTempISOFirst();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMakeISODlg::OnPaint()
{
		CDialog::OnPaint();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMakeISODlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMakeISODlg::OnBnClickedSamefolderCheck()
{	
	GetDlgItem(IDC_TargetFolder_EDIT)->EnableWindow(IsCheck(IDC_SameFolder_CHECK) == false);
	GetDlgItem(IDC_Folder_BUTTON2)->EnableWindow(IsCheck(IDC_SameFolder_CHECK) == false);
}

bool CMakeISODlg::IsCheck(UINT ID)
{
	CButton* pButton = (CButton*)GetDlgItem(ID);
	return pButton->GetCheck() == BST_CHECKED;
}

void CMakeISODlg::SetCheck(UINT ID,bool bCheck)
{
	CButton* pButton = (CButton*)GetDlgItem(ID);
	return pButton->SetCheck(bCheck ? BST_CHECKED : BST_UNCHECKED);
}

void CMakeISODlg::SetString(UINT ID,const CString& strBuffer)
{
	CButton* pButton = (CButton*)GetDlgItem(ID);
	pButton->SetWindowText(strBuffer);
}

CString CMakeISODlg::GetString(UINT ID)
{
	CString strTemp;	
	CButton* pButton = (CButton*)GetDlgItem(ID);
	pButton->GetWindowText(strTemp);
	return strTemp;
}

void CMakeISODlg::OnBnClickedFolderButton(UINT ID)
{
	TCHAR szFolder[MAX_PATH] = {0};

	CString strFolder = GetString(ID);
	if(strFolder.IsEmpty())
		Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T(""));
	else
		::_tcscpy(szFolder,strFolder);

	TSTRING strFolderPath;
	if(Win32Utility::GetFolder(strFolderPath, _T("Select folder"), m_hWnd, NULL, szFolder))
		SetString(ID+1000,strFolderPath.c_str());
}

CString CMakeISODlg::GetTempISOFileName()
{
	TCHAR szFolder[MAX_PATH] = {0};
	Win32Utility::GetShellFolder(szFolder,CSIDL_PERSONAL,_T("MakeISO"));

	return szFolder + CString(_T("temp.iso"));
}

void CMakeISODlg::DeleteTempISOFirst()
{
	::DeleteFile(GetTempISOFileName());
}

bool CMakeISODlg::IsISOFileExist()
{
	return ::_taccess(GetTempISOFileName(),0) != -1;
}

void CMakeISODlg::OnBnClickedExecButton()
{
	CString strSourceFolder = GetString(IDC_SoureFolder_EDIT);
	if(!strSourceFolder.IsEmpty())
	{
		DeleteTempISOFirst();

		CString strAddPara;
		CFileFind finder;
		BOOL bResult = finder.FindFile(strSourceFolder +  _T("\\*.*"));
		if (bResult)
		{
			while(bResult)
			{
				bResult = finder.FindNextFile();

				if (finder.IsDots())
					continue;

				strAddPara += _T(" -a \"") + finder.GetFilePath() + _T("\"");
			};
			finder.Close();
		}

		CString strPara = _T("\"") + GetTempISOFileName() + _T("\" -be 2 -py ") + strAddPara;
		TRACE(_T("-----------%s-----------\n"),strPara);
		WinExec(_T("C:\\Program Files\\MagicISO\\Miso.exe "),strPara.GetBuffer());

		OnBnClickedRenameButton();
	}
	else AfxMessageBox(_T("Source folder is empty"));
}

CString CMakeISODlg::GetNewFormat()
{
	CString strFormat = GetString(IDC_Format_EDIT);

	CString strCode = GetString(IDC_CodeName_EDIT);
	strFormat.Replace(_T("%c"),strCode);

	CString strProect = GetString(IDC_ProjectName_EDIT);
	strFormat.Replace(_T("%p"),strProect);

	CString strVer = GetString(IDC_ProjectVersion_EDIT);
	strFormat.Replace(_T("%v"),strVer);

	CTime time = CTime::GetCurrentTime();
	CString strDate = time.Format(_T("%m%d"));
	strFormat.Replace(_T("%d"),strDate);

	return strFormat;
}

void CMakeISODlg::MakeSample()
{
	CString strSample = _T("Ex : ");
	SetString(IDC_SAMPLE_STATIC,strSample+GetNewFormat());
}

void CMakeISODlg::OnBnClickedRenameButton()
{
	CString strTargetISOFileName = GetNewFormat();
	if(IsISOFileExist())
	{
		HANDLE hFile; 

		hFile = ::CreateFile(GetTempISOFileName(),    // file to open
			GENERIC_READ,          // open for reading
			FILE_SHARE_READ,       // share for reading
			NULL,                  // default security
			OPEN_EXISTING,         // existing file only
			FILE_ATTRIBUTE_NORMAL, // normal file
			NULL);                 // no attr. template

		DWORD dwLowLen = 0;
		DWORD dwHighLen = 0;
		dwLowLen = ::GetFileSize(hFile,&dwHighLen);

		::CloseHandle(hFile);

		CString strLen;
		strLen.Format(_T("%d"),dwLowLen);
		strTargetISOFileName.Replace(_T("%s"),strLen);

		CString strTargetFolder;
		if(IsCheck(IDC_SameFolder_CHECK))
			strTargetFolder = GetString(IDC_SoureFolder_EDIT);
		else
			strTargetFolder = GetString(IDC_TargetFolder_EDIT);

		if(IsCheck(IDC_RemoveOriginal_CHECK))
		{
			CString strPara = _T(" /Q /S \"") + GetString(IDC_SoureFolder_EDIT) + _T("\"");
			SystemCommand(_T("rmdir"),strPara.GetBuffer());

			CString strPara2 = _T(" \"") + GetString(IDC_SoureFolder_EDIT) + _T("\"");
			SystemCommand(_T("mkdir"),strPara2.GetBuffer());
		}

		if(!strTargetFolder.IsEmpty())
		{
			CString strPara = _T(" /Y \"") + GetTempISOFileName() + _T("\" \"") + strTargetFolder + _T("\\") + strTargetISOFileName + _T("\"");			
			SystemCommand(_T("move"),strPara.GetBuffer());

			::ShellExecute(NULL,_T("open"),strTargetFolder,NULL,NULL,SW_SHOW);
		}
		else AfxMessageBox(_T("Target folder is empty"));
	}
}

void CMakeISODlg::SystemCommand(LPCTSTR lpApplicationName,LPTSTR lpCommandLine)
{
	TRACE(_T("-----------%s %s-----------\n"),lpApplicationName,lpCommandLine);
	
	CString strExename = lpApplicationName;
	CString strPara = lpCommandLine;
	CString strExecString = strExename + _T(" ") + strPara;
	
	::system(QSYNC_LIB::UnicodetoASCII(strExecString).GetBuffer());
}

void CMakeISODlg::WinExec(LPCTSTR lpApplicationName,LPTSTR lpCommandLine)
{
	TSTRING strExename = lpApplicationName;
	TSTRING strPara = lpCommandLine;
	TSTRING strExecString = strExename + _T(" ") + strPara;

	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi = {0};

    si.cb = sizeof(si);

    // Start the child process. 
    if( !CreateProcess( NULL,   // No module name (use command line). 
        const_cast<TCHAR*>(strExecString.c_str()), // Command line. 
        NULL,             // Process handle not inheritable. 
        NULL,             // Thread handle not inheritable. 
        FALSE,            // Set handle inheritance to FALSE. 
        CREATE_NEW_CONSOLE,                // No creation flags. 
        NULL,             // Use parent's environment block. 
        NULL,             // Use parent's starting directory. 
        &si,              // Pointer to STARTUPINFO structure.
        &pi )             // Pointer to PROCESS_INFORMATION structure.
    ) 
    {
        printf( "CreateProcess failed (%d).\n", GetLastError() );
        return;
    }

    // Wait until child process exits.
    WaitForSingleObject( pi.hProcess, INFINITE );

    // Close process and thread handles. 
    CloseHandle( pi.hProcess );
    CloseHandle( pi.hThread );
}
void CMakeISODlg::OnEnChangeSourefolderEdit()
{
	m_strSource = GetString(IDC_SoureFolder_EDIT).GetBuffer();
}

void CMakeISODlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_IConfigure->Save(QSYNC_LIB::STORAGE_INIFILE);
	QSYNC_LIB::DeleteIConfigure(_T("MakeISO"));
}

void CMakeISODlg::OnEnChangeTargetfolderEdit()
{
	m_strTarget = GetString(IDC_TargetFolder_EDIT).GetBuffer();
}

void CMakeISODlg::OnEnChangeCodenameEdit()
{	
	CString strCode = GetString(IDC_CodeName_EDIT);
	m_strCodeName = strCode.GetBuffer();
	MakeSample();
}

void CMakeISODlg::OnEnChangeProjectnameEdit()
{
	CString strProect = GetString(IDC_ProjectName_EDIT);
	m_strProjectName = strProect.GetBuffer();
	MakeSample();
}

void CMakeISODlg::OnEnChangeProjectversionEdit()
{	
	CString strVer = GetString(IDC_ProjectVersion_EDIT);
	m_strProjectVersion = strVer.GetBuffer();
	MakeSample();
}
