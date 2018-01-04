#ifndef _CQZIP_HPP__
#define _CQZIP_HPP__
#pragma once

#include <string>
#include <io.h>
#include <sys/stat.h>


namespace ODMTMA_LIB
{

using namespace std;

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

class CQZip
{
public:
	CQZip();
	~CQZip();

	bool Unzip(const string& strZipFile, const string& strFolderPath = ""); // UnZipExe ZipFile -d FolderPath 
	bool UnzipWithoutExe(const string& strZippedFileName,const string& strUnZipDestFolder);
	bool Zip(const string& strZipFile, const string& strFolderPath);  // ZipExe ZipFile -r FolderPath 

	void SetZipFilePath(const string& strZipFilePath){m_strZipFilePath = strZipFilePath;}
	void GetZipFilePath(string& strZipFilePath){strZipFilePath = m_strZipFilePath;}
	void SetFolderPath(const string& strFolderPath){m_strFolderPath = strFolderPath;}
	void GetFolderPath(string& strFolderPath){strFolderPath = m_strFolderPath;}

	bool SetFolderPathFromMot(char* szMotPath);
	
	void SetZipExePath(const string& strZipExePath){m_strZipExePath = strZipExePath;}
	void GetZipExePath(string& strZipExePath){strZipExePath = m_strZipExePath;}
	void SetUnZipExePath(const string& strUnZipExePath){m_strUnZipExePath = strUnZipExePath;}
	void GetUnZipExePath(string& strUnZipExePath){strUnZipExePath = m_strUnZipExePath;}

	bool DeleteUnzipFiles();

private:
	bool Unzip();
	bool Zip();
	bool DeleteDirectory(const string& strPath);
	bool IsDots(const char* str);
private:
	string m_strZipFilePath;
	string m_strUnZipExePath;
	string m_strZipExePath;
	string m_strFolderPath;
};

} // namespace ODMTMA_LIB

#endif //_CQZIP_HPP__
