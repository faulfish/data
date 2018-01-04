#ifndef __DOWNLOAD6240_LOGIC__
#define __DOWNLOAD6240_LOGIC__

#include "DownloadInterface.h"
#include "../../Modules/QisdaQualcommDownloadModule/Download.h"

class CDownload6240Logic : public DownloadInterface {
public:
    CDownload6240Logic(long nPort): m_download(nPort) {
        m_imgHEX.pData = NULL; m_imgHEX.nSize = 0; 
        m_imgPBL.pData = NULL; m_imgPBL.nSize = 0;
        m_imgPartition.pData = NULL; m_imgPartition.nSize = 0;
        m_imgDBL.pData = NULL; m_imgDBL.nSize = 0;
        m_imgFSBL.pData = NULL; m_imgFSBL.nSize = 0;
        m_imgOSBL.pData = NULL; m_imgOSBL.nSize = 0;
        m_imgAMSS.pData = NULL; m_imgAMSS.nSize = 0;
        m_imgCEFS.pData = NULL; m_imgCEFS.nSize = 0;
        OpenAllImage();
    }
    virtual ~CDownload6240Logic() {
        if(m_imgHEX.pData)
            delete m_imgHEX.pData;
        if(m_imgPBL.pData)
            delete m_imgPBL.pData;
        if(m_imgPartition.pData)
            delete m_imgPartition.pData;
        if(m_imgDBL.pData)
            delete m_imgDBL.pData;
        if(m_imgFSBL.pData)
            delete m_imgFSBL.pData;
        if(m_imgOSBL.pData)
            delete m_imgOSBL.pData;
        if(m_imgAMSS.pData)
            delete m_imgAMSS.pData;
        if(m_imgCEFS.pData)
            delete m_imgCEFS.pData;
        TRACE("CDownload6240Logic Destructor...\n");
    }

    virtual bool PreDownload() {
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
            bRes = m_download.StreamingDLFromPartition(true);
        }
        m_download.Unregister((INotify_ANSI*)this, EVENT_DL_PROGRESS);
        m_download.Unregister((INotify_ANSI*)this, EVENT_DL_PARTITION_DIF);
        return bRes;
    }
    // until now, set all parameters.
    bool SetImageParameters() {
        bool bRes = true;
        if(m_imgAMSS.pData != NULL)
            bRes = m_download.SetAMSS(m_imgAMSS.pData, m_imgAMSS.nSize);
        if(m_imgCEFS.pData != NULL)
            bRes = m_download.SetCEFS(m_imgCEFS.pData, m_imgCEFS.nSize);
        if(m_imgDBL.pData != NULL)
            bRes = m_download.SetDBL(m_imgDBL.pData, m_imgDBL.nSize);
        if(m_imgFSBL.pData != NULL)
            bRes = m_download.SetFSBL(m_imgFSBL.pData, m_imgFSBL.nSize);
        if(m_imgHEX.pData != NULL)
            bRes = m_download.SetHex(m_imgHEX.pData, m_imgHEX.nSize);
        if(m_imgOSBL.pData != NULL)
            bRes = m_download.SetOSBL(m_imgOSBL.pData, m_imgOSBL.nSize);
        if(m_imgPartition.pData != NULL)
            bRes = m_download.SetPartition(m_imgPartition.pData, m_imgPartition.nSize);
        if(m_imgPBL.pData != NULL)
            bRes = m_download.SetPBL(m_imgPBL.pData, m_imgPBL.nSize);
        return true;
    }
private:

    bool OpenAllImage() {
        bool bAMSSExist = OpenImage(m_imgAMSS, _T("amss"));
        bool bCEFSExist = OpenImage(m_imgCEFS, _T("cefs"));
        bool bDBLExist = OpenImage(m_imgDBL, _T("dbl"));
        bool bFSBLExist = OpenImage(m_imgFSBL, _T("fsbl"));
        bool bOSBLExist = OpenImage(m_imgOSBL, _T("osbl"));
        bool bPartitionExist = OpenImage(m_imgPartition, _T("partition"));
        bool bHEXExist = OpenImage(m_imgHEX, _T("hex"));
        return bAMSSExist & bCEFSExist & bDBLExist & bFSBLExist & bOSBLExist & bPartitionExist & bHEXExist;
    }

    inline int GetStepLight(CStringA& strMsg) {
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

public:
    CShareDownload6240 m_download;
    IMAGE m_imgHEX;
    IMAGE m_imgPBL;
    IMAGE m_imgPartition;
    IMAGE m_imgDBL;
    IMAGE m_imgFSBL;
    IMAGE m_imgOSBL;
    IMAGE m_imgAMSS;
    IMAGE m_imgCEFS;

};


#endif