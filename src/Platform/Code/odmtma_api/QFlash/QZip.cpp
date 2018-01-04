#include "stdafx.h"
#include "QZip.h"
#include "assert.h"
#include <direct.h>
#include "shlobj.h"
#include "../../Components/CZipArchive/ZipArchive.h"

namespace ODMTMA_LIB
{

CQZip::CQZip()
{
#ifdef _DEBUG
	m_strZipExePath = "..\\Debug\\zip.exe";
	m_strUnZipExePath = "..\\Debug\\unzip.exe";
#else
	m_strZipExePath = "zip.exe";
	m_strUnZipExePath = "unzip.exe";
#endif
	
	TCHAR szPath[1024] = {0};
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_WINDOWS|CSIDL_FLAG_CREATE, NULL, 0, szPath)))
	{
		m_strFolderPath = szPath;
		m_strFolderPath += "\\Temp\\odmtemp";
	}	
}
CQZip::~CQZip(){}

bool CQZip::Unzip(const string& strZipFile, const string& strFolderPath)
{
	bool bRes = false;
	m_strZipFilePath = strZipFile.size()!=0 ? strZipFile : m_strZipFilePath;
	m_strFolderPath = strFolderPath.size()!=0 ? strFolderPath : m_strFolderPath;
	if(m_strZipFilePath.size() !=0 && m_strFolderPath.size() != 0)
		bRes = Unzip();
	return bRes;
}
bool CQZip::UnzipWithoutExe(const string& strZippedFileName,const string& strUnZipDestFolder)
{
	bool bRes = true;
	//unzip the *.sdq file and store files in strUnZipDestFolder folder
	CZipArchive zip;
	m_strFolderPath = strUnZipDestFolder.size()!=0 ? (string)strUnZipDestFolder : m_strFolderPath;
	//if(zip.Open(strZippedFileName.c_str(), CZipArchive::openReadOnly))
	try
	{
		zip.Open(strZippedFileName.c_str(), CZipArchive::openReadOnly);
		for(WORD i = 0; i< zip.GetNoEntries() && bRes; i++) 
			bRes = zip.ExtractFile(i, m_strFolderPath.c_str());
		zip.Close();
	}
	catch (CException* e)
	{
		bRes = false;
	}

	return bRes;
}

bool CQZip::Zip(const string& strZipFile, const string& strFolderPath)
{
	bool bRes = false;
	m_strZipFilePath = strZipFile.size()!=0 ? strZipFile : m_strZipFilePath;
	m_strFolderPath = strFolderPath.size()!=0 ? strFolderPath : m_strFolderPath;
	if(m_strZipFilePath.size() !=0 && m_strFolderPath.size() != 0)
		bRes = Zip();
	return bRes;
}

bool CQZip::Unzip()
{
	bool bRes = true;
	if(::_access_s(m_strUnZipExePath.c_str(), 0)==-1 || ::_access_s(m_strZipFilePath.c_str(), 0)==-1)
	{
		bRes = false;
		assert(bRes);
	}
	//TCHAR szBufPath[256] = {0};
	//::GetCurrentDirectory(256, szBufPath);
	string strCmd = "\"" + m_strZipFilePath + "\" -d \"" + m_strFolderPath + "\"";
	if(bRes && (int)ShellExecute(NULL,_T("open"),m_strUnZipExePath.c_str(),strCmd.c_str(),NULL,SW_HIDE)<=32)
	{
		bRes = false;
		assert(bRes);
	}
	return bRes;
}
bool CQZip::Zip()
{
	bool bRes = true;
	if(::_access_s(m_strZipExePath.c_str(), 0) == -1)
	{
		bRes = false;
		assert(bRes);
	}
	chdir(m_strFolderPath.c_str());
	string strCmd = "\"" + m_strZipFilePath + "\" *.*";
	string strZipExePath = "\"" + m_strZipExePath + "\"";
	if(bRes && (int)ShellExecute(NULL, "open",strZipExePath.c_str(),strCmd.c_str(),NULL,SW_HIDE)<=32)
	{
		bRes = false;
		assert(bRes);
	}
	//TCHAR szBufPath[256] = {0};
	//::GetCurrentDirectory(256, szBufPath);
	return bRes;
}

bool CQZip::DeleteUnzipFiles()
{
	bool bRes = false;
	bRes = DeleteDirectory(m_strFolderPath);
	return bRes;
}

bool CQZip::DeleteDirectory(const string& strPath)
{	
	bool bRes = true;
	string strDirPath = strPath + "\\*"; // searching all files
	string strFileName = strPath + "\\";

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(strDirPath.c_str(),&FindFileData);
	if(hFind == INVALID_HANDLE_VALUE)
		bRes = false;
	
	strDirPath = strFileName;
	bool bSearch = true;
	while(bSearch && bRes) 
	{    // until we find an entry
		if(FindNextFile(hFind,&FindFileData)) 
		{
			if(IsDots(FindFileData.cFileName)) 
				continue;
			strFileName += FindFileData.cFileName;
			if((FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) //recursively
				bRes = DeleteDirectory(strFileName.c_str());
			else 
			{	if(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					chmod(strFileName.c_str(), _S_IWRITE);
				bRes = DeleteFile(strFileName.c_str()) == TRUE;
			}
			strFileName = strDirPath;
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
		bRes = RemoveDirectory(strPath.c_str()) == TRUE;
	return bRes;  
}
/*
//mfc version delete directory
CFileFind finder;
CString FilePath;
FilePath.Format("%s%s",SOURCE_FOLDER_PATH,FILE_TYPE);
BOOL bWorking = finder.FindFile(FilePath);
while (bWorking)
{
	bWorking = finder.FindNextFile();
	if(finder.IsDirectory())
	{
		if(!finder.IsDots())
		{
		m_lstFileList.ResetContent();
		m_lstFileList.AddString(finder.GetFilePath());
		FolderName2Write = finder.GetFileName();
		FolderPath2Write = finder.GetFilePath();

		///Some processing here

		DeleteDirectory(FolderPath2Write);
		}
	}
}      

DeleteDirectory(LPCTSTR lpszDir)
{
	CFileFind finder;
	CString FF;
	FF.Format("%s\\*.*",lpszDir);
	BOOL bWorking = finder.FindFile(FF);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if(!finder.IsDots() && !finder.IsDirectory())
		DeleteFile(finder.GetFilePath());
	}
	RemoveDirectory(lpszDir);
	finder.Close();
}
*/

bool CQZip::IsDots(const char* str) 
{
	bool bRes = true;
	if(_mbscmp((const unsigned char*)str, (const unsigned char*)".") && _mbscmp((const unsigned char*)str, (const unsigned char*)"..")) 
		bRes = false;
	return bRes;
}

bool CQZip::SetFolderPathFromMot(char* szMotPath)
{
	bool bRes = false;
	if(szMotPath)
	{
		std::string strMotPath = szMotPath;
		std::string::size_type nDot = strMotPath.find_last_of(".");
		m_strFolderPath = strMotPath.substr(0, nDot);
		
		bRes = true;
	}
	return bRes;
}


} // namespace ODMTMA_LIB