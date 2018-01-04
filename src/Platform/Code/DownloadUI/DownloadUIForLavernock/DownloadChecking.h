#pragma once

typedef bool (*pFnOnlineMode)(int nPort);
typedef bool (*pFnSetOffline)(int nPort);
typedef bool (*pFnResetHandset)(int nPort);
typedef bool (*pFnFTMMode)(int nPort);
typedef bool (*pFnReadQFuse)(int, unsigned long&, unsigned long&, bool&);
typedef bool (*pFnChangeFTM)(int nPort);
typedef bool (*pFnChangeOnline)(int nPort);
typedef bool (*pFnCheckFTM)(int nPort, int& nFTMValue);


class CDownloadChecking {
public:
    CDownloadChecking() : hDll(NULL) {
        hDll = LoadLibrary(_T("QisdaNVModule_StaticM.dll"));
        ASSERT(hDll);
    }
    virtual ~CDownloadChecking() {
        if(hDll != NULL)
            FreeLibrary(hDll);
        hDll = NULL;
    }
    //////////////////////////////////////////////////////////////////////////
    // This function is used for checking the load in handset.
    //////////////////////////////////////////////////////////////////////////
    bool CheckVersion() {
        
    }
    //////////////////////////////////////////////////////////////////////////
    // This function is used for checking fused or not.
    //////////////////////////////////////////////////////////////////////////
    bool CheckQFuse() {

    }
    //////////////////////////////////////////////////////////////////////////
    // This function is used for check software load is signed or unsigned.
    // There are two kinds of methods to check the code is signed or not.
    // Find the string "QUALCOMM Attestation CA"
    //////////////////////////////////////////////////////////////////////////
    bool CheckSignedCodeByString(CString strFile, unsigned long nCheckLength = -1) {
        CFile f;
        BOOL bOpened = f.Open(strFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone);
        if(!bOpened) return false;
        // 1. Check String QUALCOMM Attestation CA
        char* szFileContent = NULL;
        if((-1 == nCheckLength) && bOpened) {
            nCheckLength = f.GetLength();   //< Get all file content
        }
        szFileContent = new char[nCheckLength];
        int nReadLength = f.Read(szFileContent, nCheckLength);
        ASSERT(nReadLength == nCheckLength);
        const char* szAttestation = "QUALCOMM Attestation CA";
        bool bRet = false;
        for(int i = 0; i <  nCheckLength - 23; ++i) {
            int n = memcmp(&(szFileContent[i]), szAttestation, 23);
            if(n == 0) {
                bRet = true;
                break;
            }
        }
        f.Close();
        delete [] szFileContent;
        return bRet;    //< If the value is true, the code is signed. Otherwise, false
    }
    //////////////////////////////////////////////////////////////////////////
    // Or Check the string: O'zapft is!000;Athens;0.24;ATHB1A_0.24;U;0;
    //                      O'zbplt is!000;Athens;0.231;ATHB1A_0.231;U;1;
    // O'zbplt is!000;Athens;0.231;ATHB1A_0.231;U;1; => last 1 is the version #
    //////////////////////////////////////////////////////////////////////////
    bool CheckSignedCodeByLabel(CString strFile, const char* szCheckString) {
        CFile f;
        BOOL bOpened = f.Open(strFile, CFile::modeRead | CFile::typeBinary | CFile::shareDenyNone);
        if(!bOpened) return false;
        char* szFileContent = new char[f.GetLength()];
        int nReadLength = f.Read(szFileContent, f.GetLength());
        ASSERT(nReadLength == f.GetLength());
        int nPosition = 0;
        for(int i = 0; i < f.GetLength() - 7; ++i) {
            int n = memcmp(&(szFileContent[i]), szFileContent, 7);
            if(n == 0) {
                nPosition = i;
                break;
            }
        }
        char* szRevocation = new char[100]; //< allocate 100 byte to use. if the length is not enough, it should be add more
        memcpy(szRevocation, &(szFileContent[nPosition]), 100);
        delete [] szFileContent;
        for(int i = 0; i < 100; ++i) {
            
        }
    }
    //////////////////////////////////////////////////////////////////////////
    // This function is used for check software load security version
    // O'zbplt is!000;Athens;0.231;ATHB1A_0.231;U;1; => unsigned load
    // O'zbplt is!000;Athens;0.231;ATHB1A_0.231;P;1; => signed load
    //////////////////////////////////////////////////////////////////////////
    bool CheckLoadSecurityVersion(CString strFile) {
        
    }

    // private variables
private:
    HMODULE hDll;

    // public variables
public:
    CString m_strHexFile;
    CString m_strPartitionFile;
    CString m_strAmssFile;
    CString m_strAmssHdFile;
    CString m_strCEFSFile;
    CString m_strDUFile;
    CString m_strOemsblFile;
    CString m_strOemsblHdFile;
    CString m_strQcsblFile;
    CString m_strQcsblHdFile;
};
