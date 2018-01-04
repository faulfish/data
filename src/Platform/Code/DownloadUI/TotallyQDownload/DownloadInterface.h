#ifndef __DOWNLOAD_INTERFACE__
#define __DOWNLOAD_INTERFACE__

#include "../../CommonUtil/NotifyBase.h"

using namespace std;
using namespace QSYNC_LIB;

const TSTRING EVENT_TOTALLY_CALLBACK        = _T("TOTALLY_DOWNLOAD_CALL_BACK");
const TSTRING EVENT_TOTALLY_PROGRESS        = _T("TOTALLY_DOWNLOAD_PROGRESS");
const TSTRING EVENT_TOTALLY_STEP_LIGHT      = _T("TOTALLY_DOWNLOAD_STEP_LIGHT");
const TSTRING EVENT_TOTALLY_GET_VARIANT_STR = _T("TOTALLY_DOWNLOAD_VARIANT_STRING");

typedef enum _Platform {
    NPRG6240,
    NPRG6245,
    NPRG6260,
    NPRG6270,
    UNKNOWN
} Platform;


class DownloadInterface : public CNotifyBase, public INotify_ANSI {

public:
    DownloadInterface(): m_bDifferentPartition(false) {
        DefineNotify(EVENT_TOTALLY_CALLBACK);
        DefineNotify(EVENT_TOTALLY_PROGRESS);
        DefineNotify(EVENT_TOTALLY_STEP_LIGHT);
        DefineNotify(EVENT_TOTALLY_GET_VARIANT_STR);

        //////////////////////////////////////////////////////////////////////////
        // Get the Download.ini file path
        TCHAR szCurrentFolder[MAX_PATH] = {0};
        ::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
        m_strIni = CString(szCurrentFolder) + _T("\\Download.ini");
        ASSERT(_taccess(m_strIni.GetString(), 0) == 0);
    }

    virtual ~DownloadInterface() {}

    bool Register(INotify* pNotify,const TSTRING& strEvent) {
        return CNotifyBase::Register(pNotify, strEvent);
    }
    bool Register(EventFunc* pEventFunc,const TSTRING& strEvent) {
        return CNotifyBase::Register(pEventFunc, strEvent);
    }
    bool Unregister(INotify* pNotify,const TSTRING& strEvent) {
        return CNotifyBase::Unregister(pNotify, strEvent);
    }
    bool Unregister(EventFunc* pEventFunc,const TSTRING& strEvent) {
        return CNotifyBase::Unregister(pEventFunc, strEvent);
    }
    void DefineNotify(const TSTRING& strNewEvent) {
        CNotifyBase::DefineNotify(strNewEvent);
    }
    void Fire(const TSTRING& strEvent,long nParam) {
        CNotifyBase::Fire(strEvent, nParam);
    }

    void Event(const std::string& strEvent,long nParam) {
        if(strEvent == EVENT_DL_PROGRESS) {
            int nPrecentage = ((DL_PROGRESS*)nParam)->nProgress;
            CStringA strPercent;
            strPercent.Format("%s %d%%",((DL_PROGRESS*)nParam)->szMsg, nPrecentage);
            DWORD dwNum = MultiByteToWideChar (CP_ACP, 0, strPercent.GetString(), -1, NULL, 0);
            wchar_t *pwText = new wchar_t[dwNum];
            MultiByteToWideChar(CP_ACP, 0, strPercent.GetString(), -1, pwText, dwNum);
            int nStep = GetStepLight(strPercent);
            if(nStep != 0)
                Fire(EVENT_TOTALLY_STEP_LIGHT, nStep);  // transfer step light
            Fire(EVENT_TOTALLY_CALLBACK, (long)pwText); // transfer message to UI
            Fire(EVENT_TOTALLY_PROGRESS, nPrecentage);  // transfer percentage
            delete[] pwText;
        } else if(strEvent == EVENT_DL_PARTITION_DIF) {
            m_bDifferentPartition = true;
        }
    }
    bool OpenImage(IMAGE & imgLoad, CString strLoadName) {
        TCHAR szCurrentFolder[MAX_PATH] = {0};
        ::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
        TCHAR szLoadPath[MAX_PATH] = {0};
        GetPrivateProfileString(_T("path"), strLoadName.GetString(), _T(""), szLoadPath, MAX_PATH, m_strIni.GetString());
        CString strLoadFileName = CString(szCurrentFolder) + CString(_T("\\")) + CString(szLoadPath);
        bool bLoadExist = (_taccess(strLoadFileName.GetString(), 0) == 0);
        if(!bLoadExist) return false;
        CFile fLoad;
        BOOL bOpened = fLoad.Open(strLoadFileName.GetString(), CFile::modeRead);
        if(imgLoad.pData == NULL && bOpened != FALSE) {
            UINT nLength = (UINT)fLoad.GetLength();
            imgLoad.pData = new BYTE[nLength+2];
            memset(imgLoad.pData, 0, nLength+2);
            imgLoad.nSize = nLength;
            UINT nReadSize = fLoad.Read(imgLoad.pData, nLength);
            ASSERT(nReadSize == nLength);
        }
        return bLoadExist;
    }

    static Platform GetPlatform() {
        TCHAR szCurrentFolder[MAX_PATH] = {0};
        ::GetCurrentDirectory(MAX_PATH, szCurrentFolder);
        CString strFindFile = CString(szCurrentFolder) + CString(_T("\\*.hex"));
        CFileFind ff;
        ff.FindFile(strFindFile);
        BOOL bWorking = ff.FindNextFile();
        CString strHex = ff.GetFileName();
        Platform platform = UNKNOWN;
        if(strHex.CompareNoCase(_T("NPRG6240.hex")) == 0) {
            platform = NPRG6240;
        } else if(strHex.CompareNoCase(_T("NPRG6245.hex")) == 0) {
            platform = NPRG6245;
        } else if(strHex.CompareNoCase(_T("NPRG6260.hex")) == 0) {
            platform = NPRG6260;
        } else if(strHex.CompareNoCase(_T("NPRG6270.hex")) == 0) {
            platform = NPRG6270;
        } else {
            ASSERT(0);
        }
        return platform;
    }


public:
    bool m_bDifferentPartition;
    CString m_strIni;

    ///////////////////////////////////////////////////////////////////////////////////////////////
    // These functions should be implemented
    virtual bool PreDownload() = 0;                 ///< doing something before downloading
    virtual bool PostDownload() = 0;                ///< doing something after downloading
    virtual bool StartDownload() = 0;               ///< implement by sub-class
    virtual bool SetImageParameters() = 0;          ///< implement by sub-class
    virtual int GetStepLight(CStringA& strMsg) = 0; ///< return the step index
    ///////////////////////////////////////////////////////////////////////////////////////////////
};

#endif