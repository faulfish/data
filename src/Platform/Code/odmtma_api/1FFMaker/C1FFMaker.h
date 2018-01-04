#pragma once

#include <string>
#include <vector>
#include "../../CommonUtil/PIMStringUtility.h"
#include <io.h>


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

class C1FFNameMaker
{
public:
	C1FFNameMaker(void);
	~C1FFNameMaker(void);

	// Encode to .mot
	bool GetMotName(TSTRING& strMotName);

	void AddFileName(const TSTRING& strName) { m_vstrFiles.push_back(strName); }
	void ClearFileNames(){ m_vstrFiles.clear(); }
	void SetMarketName(const TSTRING& strMarketName){ m_strMarketName = strMarketName; }

	// Decode .mot
	bool SetMotName(const TSTRING& strMotName); // trim .mot and keep file name
	bool CompareSWV(TSTRING strSWV);
	bool CompareFlexVersion(TSTRING strFlexVersion);

	int GetActiveFileCount(){return m_nActiveCount;} // at least 1 , or otherwise never set this number before.

	bool GetAMSS(TSTRING& strAMSSHD, TSTRING& strAMSS);
	bool GetPartition(TSTRING& strPartition);
	bool GetBootLoader(TSTRING& strQCSBLHD, TSTRING& strQCSBL, TSTRING& strOEMHD, TSTRING& strOEM);
	bool GetCsv(TSTRING& strCsv);
	bool GetCEFS(TSTRING& strCEFS);
	bool GetHex(TSTRING& strHex);
	bool GetDU(TSTRING& strDu);
	
	void SetDefaultFolder(const TSTRING& strFolder){m_strDefaultFolder = strFolder; }
	TSTRING GetDefaultFolder(){return m_strDefaultFolder;}

	bool GetLoc(TSTRING& strLock);

private:
	// Encode
	bool GetSWVersion();
	bool GetLang();
	bool GetSIMLock();	
	
	// Decode
	TSTRING GetMbn();
	TSTRING GetLP_Res(const TSTRING& strSubName);
	
private:
	bool m_bSetMotName;
	TSTRING m_strMotName;
	int m_nActiveCount;
	TSTRING m_strDefaultFolder;

	vector<TSTRING> m_vstrFiles;  // for encode
	vector<TSTRING> m_vstrNaming; // for decode
	TSTRING m_strMarketName;
	TSTRING m_strSIMFile;
	TSTRING m_strMainLoad;
	TSTRING m_strLang;
};

class CFileGetter {
public:

    CFileGetter();
    virtual ~CFileGetter();
    virtual bool SetMot(const TSTRING& strMot);
    virtual bool SetFileName(bool bFindAll = true) = 0;
    virtual void SetMotExtractFolder(const TSTRING& strFolderPath);
    virtual bool GetFolderPath(TSTRING& strFolderPath);
    virtual bool GetImageFilePath(TSTRING& strOutFileName, TSTRING& strImageFileName);

    virtual bool GetAMSS(TSTRING& strAMSS) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetAMSSHD(TSTRING& strAMSSHD) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetPartition(TSTRING& strPartition) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetQCSBLHD(TSTRING& strQCSBLHD) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetQCSBL(TSTRING& strQCSBL) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetOEMSBL(TSTRING& strOemsbl) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetOEMSBLHD(TSTRING& strOemsblhd) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetCEFS(TSTRING& strCEFS) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetHex(TSTRING& strHex) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetDU(TSTRING& strDu) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetLoc(TSTRING& strLoc) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetOSBL(TSTRING& strOSBL) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetDBL(TSTRING& strDBL) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetFSBL(TSTRING& strFSBL) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };
    virtual bool GetPBL(TSTRING& strDBL) {
        ASSERT(0 && _T("Don't use this directly!!"));
        return false;
    };

protected:
    virtual bool FindFile(const TSTRING& strSearchFile, TSTRING& strFileName);
public:
    TSTRING m_strFolderPath;
    TSTRING m_strMot;
};

class CFileGetterFor6245 : public CFileGetter
{
public:
	CFileGetterFor6245();
	virtual ~CFileGetterFor6245();

	virtual bool SetFileName(bool bFindAll = true);

	bool GetAMSS(TSTRING& strAMSS);
	bool GetAMSSHD(TSTRING& strAMSSHD);
	bool GetPartition(TSTRING& strPartition);
	bool GetQCSBLHD(TSTRING& strQCSBLHD);
	bool GetQCSBL(TSTRING& strQCSBL);
	bool GetOEMSBL(TSTRING& strOemsbl);
	bool GetOEMSBLHD(TSTRING& strOemsblhd);
	bool GetCEFS(TSTRING& strCEFS);
	bool GetHex(TSTRING& strHex);
	bool GetDU(TSTRING& strDu);
	bool GetLoc(TSTRING& strLoc);

	bool GetOSBL(TSTRING& strOSBL);
	bool GetDBL(TSTRING& strDBL);
	bool GetFSBL(TSTRING& strFSBL);

//protected:
//	bool FindFile(const TSTRING& strSearchFile, TSTRING& strFileName);
protected:

	TSTRING m_strLoc;
	TSTRING m_strPartition;
	TSTRING m_strHex;
	TSTRING m_strQcsbl;
	TSTRING m_strQcsblhd;
	TSTRING m_strOemsbl;
	TSTRING m_strOemsblhd;
	TSTRING m_strAmss;
	TSTRING m_strAmsshd;
	TSTRING m_strCEFS;
	TSTRING m_strDU;

	TSTRING m_strOSBL;
	TSTRING m_strDBL;
	TSTRING m_strFSBL;
};

class CFileGetterFor6240 : public CFileGetter
{
public:
    CFileGetterFor6240();
    virtual ~CFileGetterFor6240();

    virtual bool SetFileName(bool bFindAll = true);

    bool GetHex(TSTRING& strHex);
    bool GetPBL(TSTRING& strDBL);
    bool GetPartition(TSTRING& strPartition);
    bool GetDBL(TSTRING& strDBL);
    bool GetFSBL(TSTRING& strFSBL);
    bool GetOSBL(TSTRING& strOSBL);
    bool GetAMSS(TSTRING& strAMSS);
    bool GetCEFS(TSTRING& strCEFS);

//protected:
//    bool FindFile(const TSTRING& strSearchFile, TSTRING& strFileName);
protected:

    TSTRING m_strHEX;
    TSTRING m_strPBL;
    TSTRING m_strPartition;
    TSTRING m_strDBL;
    TSTRING m_strFSBL;
    TSTRING m_strOSBL;
    TSTRING m_strAMSS;
    TSTRING m_strCEFS;
};