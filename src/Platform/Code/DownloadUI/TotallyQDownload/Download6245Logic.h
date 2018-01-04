#ifndef __DOWNLOAD6245_LOGIC__
#define __DOWNLOAD6245_LOGIC__

#include "DownloadInterface.h"
#include "../../Modules/QisdaQualcommDownloadModule/Download.h"

class CDownload6245Logic : public DownloadInterface {
public:
    CDownload6245Logic(long nPort): m_download(nPort) {
        m_imgHEX.pData = NULL; m_imgHEX.nSize = 0;
        m_imgPartition.pData = NULL; m_imgPartition.nSize = 0;
        m_imgQCSBL.pData = NULL; m_imgQCSBL.nSize = 0;
        m_imgQCSBLHD.pData = NULL; m_imgQCSBLHD.nSize = 0;
        m_imgOEMSBL.pData = NULL; m_imgOEMSBL.nSize = 0;
        m_imgOEMSBLHD.pData = NULL; m_imgOEMSBLHD.nSize = 0;
        m_imgAMSS.pData = NULL; m_imgAMSS.nSize = 0;
        m_imgAMSSHD.pData = NULL; m_imgAMSSHD.nSize = 0;
        m_imgCEFS.pData = NULL; m_imgCEFS.nSize = 0;
        m_imgDU.pData = NULL; m_imgDU.nSize = 0;
        OpenAllImage();
    }
    virtual ~CDownload6245Logic() {
        if(m_imgHEX.pData != NULL)
            delete m_imgHEX.pData;
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
        TRACE("CDownload6245Logic Destructor...\n");
    }

    void GetVariantStringFromIni() {
        TCHAR szVariant[512] = {0};
        DWORD lRetValue = ::GetPrivateProfileString(_T("Path"), _T("cefs"), NULL, szVariant, 512, m_strIni);
        CString strVariantString = szVariant;
        int nFound = strVariantString.ReverseFind('\\');
        strVariantString = strVariantString.Mid(nFound+1, strVariantString.GetLength() - nFound);
        strVariantString.Replace(_T(".qvcefs"), _T(""));
        strVariantString.Replace(_T("_"), _T(" "));
        Fire(EVENT_TOTALLY_GET_VARIANT_STR, (long)(LPCTSTR)strVariantString);
    }

    void GetSoftwareIsSigned() {
        // Need implement
        ASSERT(0);
    }

    virtual bool PreDownload() {
        GetVariantStringFromIni();
        return false;
    }
    virtual bool PostDownload() {
        return false;
    }
    bool StartDownload() {
        m_download.Register((INotify_ANSI*)this, EVENT_DL_PROGRESS);
        m_download.Register((INotify_ANSI*)this, EVENT_DL_PARTITION_DIF);
        m_download.SetResetMode(false);
        bool bRes = m_download.Download();
        if(!bRes && m_bDifferentPartition) {
            m_download.SetIsOveridePartition(true);
            m_download.SetResetMode(true);
        }
        m_download.Unregister((INotify_ANSI*)this, EVENT_DL_PROGRESS);
        m_download.Unregister((INotify_ANSI*)this, EVENT_DL_PARTITION_DIF);
        return bRes;
    }
    bool SetImageParameters() {
        if(m_imgHEX.pData != NULL)
            m_download.SetHex(m_imgHEX.pData, m_imgHEX.nSize);
        if(m_imgPartition.pData != NULL)
            m_download.SetPartition(m_imgPartition.pData, m_imgPartition.nSize);
        if(m_imgQCSBL.pData != NULL)
            m_download.SetQCSBL(m_imgQCSBL.pData, m_imgQCSBL.nSize);
        if(m_imgQCSBLHD.pData != NULL) 
            m_download.SetQCSBLHD(m_imgQCSBLHD.pData, m_imgQCSBLHD.nSize);
        if(m_imgOEMSBL.pData != NULL)
            m_download.SetOEMSBL(m_imgOEMSBL.pData, m_imgOEMSBL.nSize);
        if(m_imgOEMSBLHD.pData != NULL)
            m_download.SetOEMSBLHD(m_imgOEMSBLHD.pData, m_imgOEMSBLHD.nSize);
        if(m_imgAMSS.pData != NULL)
            m_download.SetAMSS(m_imgAMSS.pData, m_imgAMSS.nSize);;
        if(m_imgAMSSHD.pData != NULL)
            m_download.SetAMSSHD(m_imgAMSSHD.pData, m_imgAMSSHD.nSize);
        if(m_imgCEFS.pData != NULL)
            m_download.SetCEFS(m_imgCEFS.pData, m_imgCEFS.nSize);
        return true;
    }
    int GetStepLight(CStringA& strMsg) {
        int nStep = 0;
        if(strMsg.Find("DMSSOpenAndWriteHexFile success") != -1) {
            nStep = 1; // the 1st light
        } else if(strMsg.Find("StreamingPartition success") != -1) {
            nStep = 2; // the 2nd light
        } else if(strMsg.Find("********** [Image] QCSBL **********") != -1) {
            nStep = 3;
        } else if(strMsg.Find("********** [Image] OEMSBLHD/OEMSBL **********") != -1) {
            nStep = 4;
        } else if(strMsg.Find("********** [Image] (AMSSHD)/AMSS **********") != -1) {
            nStep = 5;
        } else if(strMsg.Find("********** [Image] CEFS **********") != -1) {
            nStep = 6;
        } else if(strMsg.Find("********** [Image] DU **********") != -1) {
            nStep = 7;
        } else if(strMsg.Find("StreamingReset success") != -1) {
            nStep = 8;
        }
        return nStep;
    }
private:

    bool OpenAllImage() {
        bool bAMSSExist = OpenImage(m_imgAMSS, _T("amss"));
        bool bAMSSEHDxist = OpenImage(m_imgAMSSHD, _T("amsshd"));
        bool bCEFSExist = OpenImage(m_imgCEFS, _T("cefs"));
        bool bHEXExist = OpenImage(m_imgHEX, _T("hex"));
        bool bQCSBLExist = OpenImage(m_imgQCSBL, _T("qcsbl"));
        bool bQCSBLHDExist = OpenImage(m_imgQCSBLHD, _T("qcsblhd"));
        bool bOEMSBLExist = OpenImage(m_imgOEMSBL, _T("oemsbl"));
        bool bOEMSBLHDExist = OpenImage(m_imgOEMSBLHD, _T("oemsblhd"));
        bool bPartitionExist = OpenImage(m_imgPartition, _T("partition"));
        return bAMSSExist & bAMSSEHDxist & bCEFSExist & bHEXExist &
            bQCSBLExist & bQCSBLHDExist & bOEMSBLExist & bOEMSBLHDExist &
            bPartitionExist;
    }
public:
    CShareDownload6245 m_download;
    IMAGE m_imgHEX;
    IMAGE m_imgPartition;
    IMAGE m_imgQCSBL;
    IMAGE m_imgQCSBLHD;
    IMAGE m_imgOEMSBL;
    IMAGE m_imgOEMSBLHD;
    IMAGE m_imgAMSS;
    IMAGE m_imgAMSSHD;
    IMAGE m_imgCEFS;
    IMAGE m_imgDU;
};



#endif