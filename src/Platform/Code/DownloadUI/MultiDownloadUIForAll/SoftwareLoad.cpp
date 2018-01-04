#include "StdAfx.h"
#include "SoftwareLoad.h"

//////////////////////////////////////////////////////////////////////////
// This class id for 6245
//////////////////////////////////////////////////////////////////////////
SoftwareLoad6245::SoftwareLoad6245(CString strIniFile) : SoftwareLoad(strIniFile)
{
    m_imgHEX.nSize = 0;
    m_imgPBL.nSize = 0;
    m_imgPartition.nSize = 0;
    m_imgQCSBL.nSize = 0;
    m_imgQCSBLHD.nSize = 0;
    m_imgOEMSBL.nSize = 0;
    m_imgOEMSBLHD.nSize = 0;
    m_imgAMSS.nSize = 0;
    m_imgAMSSHD.nSize = 0;
    m_imgCEFS.nSize = 0;
    m_imgDU.nSize = 0;

    m_imgHEX.pData = 0;
    m_imgPBL.pData = 0;
    m_imgPartition.pData = 0;
    m_imgQCSBL.pData = 0;
    m_imgQCSBLHD.pData = 0;
    m_imgOEMSBL.pData = 0;
    m_imgOEMSBLHD.pData = 0;
    m_imgAMSS.pData = 0;
    m_imgAMSSHD.pData = 0;
    m_imgCEFS.pData = 0;
    m_imgDU.pData = 0;
    
}

SoftwareLoad6245::~SoftwareLoad6245() {
    if(m_imgHEX.pData != NULL)
        delete m_imgHEX.pData;
    if(m_imgPBL.pData != NULL)
        delete m_imgPBL.pData;
    if(m_imgPartition.pData != NULL)
        delete m_imgPartition.pData;
    if(m_imgQCSBL.pData != NULL)
        delete m_imgQCSBL.pData;
    if(m_imgQCSBLHD.pData != NULL)
        delete m_imgQCSBLHD.pData;
    if(m_imgOEMSBL.pData != NULL)
        delete m_imgOEMSBL.pData;
    if(m_imgOEMSBLHD.pData != NULL)
        delete m_imgOEMSBLHD.pData;
    if(m_imgAMSS.pData != NULL)
        delete m_imgAMSS.pData;
    if(m_imgAMSSHD.pData != NULL)
        delete m_imgAMSSHD.pData;
    if(m_imgCEFS.pData != NULL)
        delete m_imgCEFS.pData;
    if(m_imgDU.pData != NULL)
        delete m_imgDU.pData;
}

CString SoftwareLoad6245::GetVariantVersion() {
    return _T("");
}

Platform SoftwareLoad6245::GetSupportedPlatform() {
    return PLATFORM_6245;
}

void SoftwareLoad6245::GetImageFileName() {
    m_strHEXPath = GetLoadPathFromIni(_T("hex"));
    m_strPartitionPath = GetLoadPathFromIni(_T("partition"));
    m_strQCSBLHDPath = GetLoadPathFromIni(_T("qcsblhd"));
    m_strQCSBLPath = GetLoadPathFromIni(_T("qcsbl"));
    m_strOEMSBLHDPath = GetLoadPathFromIni(_T("oemsblhd"));
    m_strOEMSBLPath = GetLoadPathFromIni(_T("oemsbl"));
    m_strAMSSHDPath = GetLoadPathFromIni(_T("amsshd"));
    m_strAMSSPath = GetLoadPathFromIni(_T("amss"));
    m_strCEFSPath = GetLoadPathFromIni(_T("cefs"));
    m_strDUPath = GetLoadPathFromIni(_T("du"));
}

void SoftwareLoad6245::GetImageContent() {
    static CCriticalSection cs;
    CSingleLock sl(&cs);
    sl.Lock();
    OpenImage(&m_imgHEX, m_strHEXPath);
    OpenImage(&m_imgPartition, m_strPartitionPath);
    OpenImage(&m_imgQCSBLHD, m_strQCSBLHDPath);
    OpenImage(&m_imgQCSBL, m_strQCSBLPath);
    OpenImage(&m_imgOEMSBLHD, m_strOEMSBLHDPath);
    OpenImage(&m_imgOEMSBL, m_strOEMSBLPath);
    OpenImage(&m_imgAMSSHD, m_strAMSSHDPath);
    OpenImage(&m_imgAMSS, m_strAMSSPath);
    OpenImage(&m_imgCEFS, m_strCEFSPath);
    OpenImage(&m_imgDU, m_strDUPath);
    m_bIsLoadOpened = true;
    TRACE("[SoftwareLoad6245]All Images Opened...\n");
    sl.Unlock();
}

//////////////////////////////////////////////////////////////////////////
// This class id for 6240
//////////////////////////////////////////////////////////////////////////
SoftwareLoad6240::SoftwareLoad6240(CString strIniFile) : SoftwareLoad(strIniFile)
{
    m_imgHEX.nSize = 0;
    m_imgPBL.nSize = 0;
    m_imgPartition.nSize = 0;
    m_imgDBL.nSize = 0;
    m_imgFSBL.nSize = 0;
    m_imgOSBL.nSize = 0;
    m_imgAMSS.nSize = 0;
    m_imgCEFS.nSize = 0;

    m_imgHEX.pData = 0;
    m_imgPBL.pData = 0;
    m_imgPartition.pData = 0;
    m_imgDBL.pData = 0;
    m_imgFSBL.pData = 0;
    m_imgOSBL.pData = 0;
    m_imgAMSS.pData = 0;
    m_imgCEFS.pData = 0;

}

SoftwareLoad6240::~SoftwareLoad6240() {
    if(m_imgHEX.pData != 0)
        delete m_imgHEX.pData;
    if(m_imgPBL.pData != 0)
        delete m_imgPBL.pData;
    if(m_imgPartition.pData != 0)
        delete m_imgPartition.pData;
    if(m_imgDBL.pData != 0)
        delete m_imgDBL.pData;
    if(m_imgFSBL.pData != 0)
        delete m_imgFSBL.pData;
    if(m_imgOSBL.pData != 0)
        delete m_imgOSBL.pData;
    if(m_imgAMSS.pData != 0)
        delete m_imgAMSS.pData;
    if(m_imgCEFS.pData != 0)
        delete m_imgCEFS.pData;
}

CString SoftwareLoad6240::GetVariantVersion() {
    return _T("");
}

Platform SoftwareLoad6240::GetSupportedPlatform() {
    return PLATFORM_6240;
}

void SoftwareLoad6240::GetImageFileName() {
    m_strHEXPath = GetLoadPathFromIni(_T("hex"));
    m_strPBLPath = GetLoadPathFromIni(_T("pbl"));
    m_strPartitionPath = GetLoadPathFromIni(_T("partition"));
    m_strDBLPath = GetLoadPathFromIni(_T("dbl"));
    m_strFSBLPath = GetLoadPathFromIni(_T("fsbl"));
    m_strOSBLPath = GetLoadPathFromIni(_T("qsbl"));
    m_strAMSSPath = GetLoadPathFromIni(_T("amss"));
    m_strCEFSPath = GetLoadPathFromIni(_T("cefs"));
}

void SoftwareLoad6240::GetImageContent() {
    static CCriticalSection cs;
    CSingleLock sl(&cs);
    sl.Lock();
    OpenImage(&m_imgHEX, m_strHEXPath);
    OpenImage(&m_imgPBL, m_strPBLPath);
    OpenImage(&m_imgPartition, m_strPartitionPath);
    OpenImage(&m_imgDBL, m_strDBLPath);
    OpenImage(&m_imgFSBL, m_strFSBLPath);
    OpenImage(&m_imgOSBL, m_strOSBLPath);
    OpenImage(&m_imgAMSS, m_strAMSSPath);
    OpenImage(&m_imgCEFS, m_strCEFSPath);
    m_bIsLoadOpened = true;
    TRACE("[SoftwareLoad6240]All Images Opened...\n");
    sl.Unlock();
}

//////////////////////////////////////////////////////////////////////////
// This class id for 6270
//////////////////////////////////////////////////////////////////////////
SoftwareLoad6270::SoftwareLoad6270(CString strIniFile) : SoftwareLoad6240(strIniFile)
{

}

SoftwareLoad6270::~SoftwareLoad6270() {

}

CString SoftwareLoad6270::GetVariantVersion() {
    return _T("");
}

Platform SoftwareLoad6270::GetSupportedPlatform() {
    return PLATFORM_6240;
}