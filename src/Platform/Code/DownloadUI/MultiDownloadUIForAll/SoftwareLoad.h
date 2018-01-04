#ifndef __SOFTWARE_LOAD_H__
#define __SOFTWARE_LOAD_H__

#pragma once
#include "../../Modules/QisdaQualcommDownloadModule/Download.h"

typedef enum _Platform {
    PLATFORM_6240,
    PLATFORM_6245,
    PLATFORM_6270,
    PLATFORM_UKNOWN
} Platform;


class SoftwareLoad
{
public:
    SoftwareLoad(CString strIniFile) : m_strIniFilePath(_T("")), m_bIsLoadOpened(false) {
    }
    virtual ~SoftwareLoad() {
    }

    virtual CString GetVariantVersion() = 0;
    virtual Platform GetSupportedPlatform() = 0;
    virtual void GetImageFileName() = 0;
    virtual void GetImageContent() = 0;

    //************************************
    // Method:    SetIniFilePath
    // FullName:  SoftwareLoad::SetIniFilePath
    // Access:    virtual public 
    // Returns:   bool
    // Parameter: CString strIniFilePath
    //************************************
    virtual bool SetIniFilePath(CString strIniFilePath) {
        if(_taccess(strIniFilePath.GetString(), 0) != 0)
            return false;
        m_strIniFilePath = strIniFilePath;
        return true;
    };

    virtual CString GetIniFilePath() {
        return m_strIniFilePath;
    };

    virtual bool OpenImage(IMAGE* image, CString strFilePath) {
        CFile f;
        BOOL bOpened = f.Open(strFilePath.GetString(), CFile::modeRead);
        if(!bOpened) return false;
        image->nSize = f.GetLength();
        image->pData = new BYTE[f.GetLength()];
        f.Read(image->pData, f.GetLength());
    };

    virtual CString GetLoadPathFromIni(const CString& strLoadName) {
        TCHAR szGetValue[512] = {0};
        DWORD dGetLength = GetPrivateProfileStringW(
            _T("Path"),
            strLoadName.GetString(),
            _T(""),
            szGetValue,
            512,
            m_strIniFilePath.GetString()
            );
        return szGetValue;
    };

public:
    CString m_strIniFilePath;
    bool m_bIsLoadOpened;
};



class SoftwareLoad6245 : public SoftwareLoad {

public:
    SoftwareLoad6245(CString strIniFile);
    virtual ~SoftwareLoad6245();
    CString GetVariantVersion();
    Platform GetSupportedPlatform();
    void GetImageFileName();
    void GetImageContent();
public:
    //////////////////////////////////////////////////////////////////////////
    // For 6245
    //////////////////////////////////////////////////////////////////////////
    IMAGE m_imgHEX;
    IMAGE m_imgPBL;
    IMAGE m_imgPartition;
    IMAGE m_imgQCSBL;
    IMAGE m_imgQCSBLHD;
    IMAGE m_imgOEMSBL;
    IMAGE m_imgOEMSBLHD;
    IMAGE m_imgAMSS;
    IMAGE m_imgAMSSHD;
    IMAGE m_imgCEFS;
    IMAGE m_imgDU;

    CString m_strHEXPath;
    CString m_strPBLPath;
    CString m_strPartitionPath;
    CString m_strQCSBLPath;
    CString m_strQCSBLHDPath;
    CString m_strOEMSBLPath;
    CString m_strOEMSBLHDPath;
    CString m_strAMSSPath;
    CString m_strAMSSHDPath;
    CString m_strCEFSPath;
    CString m_strDUPath;
};


class SoftwareLoad6240 : public SoftwareLoad {
public:
    SoftwareLoad6240(CString strIniFile);
    virtual ~SoftwareLoad6240();
    CString GetVariantVersion();
    Platform GetSupportedPlatform();
    void GetImageFileName();
    void GetImageContent();

public:
    //////////////////////////////////////////////////////////////////////////
    // For 6240
    //////////////////////////////////////////////////////////////////////////
    IMAGE m_imgHEX;
    IMAGE m_imgPBL;
    IMAGE m_imgPartition;
    IMAGE m_imgDBL;
    IMAGE m_imgFSBL;
    IMAGE m_imgOSBL;
    IMAGE m_imgAMSS;
    IMAGE m_imgCEFS;

    CString m_strHEXPath;
    CString m_strPBLPath;
    CString m_strPartitionPath;
    CString m_strDBLPath;
    CString m_strFSBLPath;
    CString m_strOSBLPath;
    CString m_strAMSSPath;
    CString m_strCEFSPath;
};


class SoftwareLoad6270 : public SoftwareLoad6240 {
public:
    SoftwareLoad6270(CString strIniFile);
    virtual ~SoftwareLoad6270();
    CString GetVariantVersion();
    Platform GetSupportedPlatform();
};


#endif