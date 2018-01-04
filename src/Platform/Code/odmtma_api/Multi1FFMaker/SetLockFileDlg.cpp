// SetLockFileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SetLockFileDlg.h"
#include <set>

// CSetLockFileDlg dialog

IMPLEMENT_DYNAMIC(CSetLockFileDlg, CDialog)

CSetLockFileDlg::CSetLockFileDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetLockFileDlg::IDD, pParent)
{

}

CSetLockFileDlg::~CSetLockFileDlg()
{
}

void CSetLockFileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VariantToLockList, m_VariantToLockListCtrl);
	DDX_Control(pDX, IDOK, m_BtnOK);
}

BOOL CSetLockFileDlg::OnInitDialog()
{
	BOOL BRet = CDialog::OnInitDialog();

	GetAllVariantFiles();
	GetAllLockFiles();

	m_LockFilesListDlg.SetListCtrl(m_vecLockFileName);
//#ifdef _DEBUG
//	m_strVariantToLockIniFilePath = _T("..\\debug\\VariantToLock.ini");
//#else
//	m_strVariantToLockIniFilePath = _T(".\\VariantToLock.ini");
//#endif

	m_VariantToLockListCtrl.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_VariantToLockListCtrl.InsertColumn(0, _T("Variant"), LVCFMT_LEFT, 400);
	m_VariantToLockListCtrl.InsertColumn(1, _T("LockFile"), LVCFMT_LEFT,400);

	m_VariantToLockListCtrl.InsertItem(0, _T("Default Lock File"));
    m_VariantToLockListCtrl.SetItemText(0, 1, _T("Non defined"));	

	m_VariantToLockListCtrl.InsertItem(1, _T("")); // 空行,無意義
	
	for (size_t nRowNumber = 0; nRowNumber < m_vecVariantFileName.size() ; ++nRowNumber)
	{
		m_VariantToLockListCtrl.InsertItem((int)nRowNumber + 2, m_vecVariantFileName.at(nRowNumber));
	}

	return BRet;
}

bool IsDots(const TCHAR* str) 
{
	bool bRes = true;
	if(_tcscmp(str,_T(".")) && _tcscmp(str,_T(".."))) 
		bRes = false;
	return bRes;
}

bool CSetLockFileDlg::GetAllVariantFiles()
{
	bool bRes = true;

	CString strSearchFolderPath = m_strVariantPath + _T("\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strSearchFolderPath, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		bRes = false;

	//取得Variant Folder下所有檔案, 但子目錄跳過
	set<CString> setFileName; 
	bool bSearch = true;
	while(bSearch && bRes)
	{
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) 
				continue;
			else if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			else
			{
				CString strFileName = FindFileData.cFileName;
				int nDotPos = strFileName.ReverseFind(_T('.'));
				if (nDotPos == -1)
					setFileName.insert(strFileName);
				else
					setFileName.insert(strFileName.Left(nDotPos));
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
	//取得Variant Folder下所有檔案結束

	//檢查副檔名, 刪除不合法的檔案
	m_vecVariantFileName.clear();
	for(set<CString>::iterator setIter = setFileName.begin(); setIter != setFileName.end() ; ++setIter)
	{
		bool bIsLegalVariantFile = true;
		for(map<TSTRING, TSTRING>::iterator mapIter = m_mapVariantExtension.begin();bIsLegalVariantFile && mapIter != m_mapVariantExtension.end() ; ++mapIter)
		{
			CString strFileFullName = m_strVariantPath + _T("\\") +*setIter + _T(".") + mapIter->second.c_str();
			bIsLegalVariantFile = (_taccess(strFileFullName,0) != -1);
		}
		if(bIsLegalVariantFile)
			m_vecVariantFileName.push_back(*setIter);
	}
	//檢查副檔名結束

	return bRes;
}

const vector<CString>& CSetLockFileDlg::GetVariantFiles(const TSTRING& strVariantPath, const map<TSTRING, TSTRING>& mapVariantExtension)
{
	m_strVariantPath = strVariantPath.c_str();
	m_mapVariantExtension = mapVariantExtension;
	GetAllVariantFiles();
	return m_vecVariantFileName;
}

bool CSetLockFileDlg::GetAllLockFiles()
{
	bool bRes = true;

	CString strSearchFolderPath = m_strLockFilePath + _T("\\*");
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strSearchFolderPath, &FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		bRes = false;

	//取得LockFile Folder下所有檔案, 但子目錄跳過
	set<CString> setFileName; 
	bool bSearch = true;
	while(bSearch && bRes)
	{
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) 
				continue;
			else if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				continue;
			else
			{
				CString strFileName = FindFileData.cFileName;
				int nDotPos = strFileName.ReverseFind(_T('.'));
				if (nDotPos == -1)
					setFileName.insert(strFileName);
				else
					setFileName.insert(strFileName.Left(nDotPos));
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
	//取得LockFile Folder下所有檔案結束

	//檢查副檔名, 刪除不合法的檔案
	m_vecLockFileName.clear();
	m_vecLockFileName.push_back(_T("No loc"));
	for(set<CString>::iterator setIter = setFileName.begin(); setIter != setFileName.end() ; ++setIter)
	{
		bool bIsLegalLockFile = true;
		for(map<TSTRING, TSTRING>::iterator mapIter = m_mapLockExtension.begin();bIsLegalLockFile && mapIter != m_mapLockExtension.end() ; ++mapIter)
		{
			CString strFileFullName = m_strLockFilePath + _T("\\") +*setIter + _T(".") + mapIter->second.c_str();
			bIsLegalLockFile = (_taccess(strFileFullName,0) != -1);
		}
		if(bIsLegalLockFile)
			m_vecLockFileName.push_back(*setIter);
	}
	//檢查副檔名結束

	return bRes;
}

INT_PTR CSetLockFileDlg::DoModal(const CString& strVariantPath, const CString& strLockFilePath, const map<TSTRING, TSTRING>& mapVariantExtension, const map<TSTRING, TSTRING>& mapLockExtension)
{
	m_strVariantPath = strVariantPath;
	m_mapVariantExtension = mapVariantExtension;
	m_strLockFilePath = strLockFilePath;
	m_mapLockExtension = mapLockExtension;

	INT_PTR nRet = CDialog::DoModal();
	return nRet;
}
BEGIN_MESSAGE_MAP(CSetLockFileDlg, CDialog)
	ON_NOTIFY(NM_DBLCLK, IDC_VariantToLockList, &CSetLockFileDlg::OnNMDblclkVarianttolocklist)
	ON_BN_CLICKED(IDOK, &CSetLockFileDlg::OnBnClickedOk)
END_MESSAGE_MAP()

void CSetLockFileDlg::OnNMDblclkVarianttolocklist(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	int nSelectedRow = m_VariantToLockListCtrl.GetSelectionMark();
	if(nSelectedRow == -1 || nSelectedRow == 1)
		return; // 無意義的點擊

	m_LockFilesListDlg.DoModal();
	int nSelectedLockFileNO = m_LockFilesListDlg.GetSelectedRow();
	if(nSelectedLockFileNO == -1)
		m_VariantToLockListCtrl.SetItemText(nSelectedRow, 1, _T(""));	
	else
		m_VariantToLockListCtrl.SetItemText(nSelectedRow, 1, m_vecLockFileName.at(nSelectedLockFileNO));	

	if(nSelectedRow == 0)
	{
		CString strDefaultLockFile = m_VariantToLockListCtrl.GetItemText(0, 1);
		if(strDefaultLockFile != _T("Non defined"))
			m_BtnOK.EnableWindow();
	}
}

void CSetLockFileDlg::OnBnClickedOk()
{
	m_mapVariantToLockFile.clear();
	CString strDefaultLockFile = m_VariantToLockListCtrl.GetItemText(0, 1);
	if(strDefaultLockFile == _T("Non defined"))
	{
		AfxMessageBox("Default lock file is not defined.");
		return;
	}

	for(size_t nIndex = 0; nIndex < m_vecVariantFileName.size(); ++nIndex)
	{
		CString strLockFileName = m_VariantToLockListCtrl.GetItemText((int)nIndex + 2, 1);
		if(strLockFileName.IsEmpty())
			m_mapVariantToLockFile[(TSTRING)(m_vecVariantFileName.at(nIndex))] = strDefaultLockFile;
		else
			m_mapVariantToLockFile[(TSTRING)(m_vecVariantFileName.at(nIndex))] = strLockFileName;
	}
	OnOK();
}
