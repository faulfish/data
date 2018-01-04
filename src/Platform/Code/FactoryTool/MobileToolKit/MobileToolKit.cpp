// MobileToolKit.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MobileToolKit.h"
#include "../../Modules/QisdaNVModule/NVItem.h"
#include "../../Modules\ODMCoachModule\ODM.h"
#include "../../Modules\QisdaMiscellaneousModule\Wireless.h"
#include "../../Modules\QisdaMiscellaneousModule\QisdaMiscellaneousModule.h"
#include "../../Modules\QisdaSimLockModule\SimLockStore.h"
//#include "../../Modules\QisdaQualcommDownloadModule\Download.h"
#include "../../odmtma_api/DownloadDLL/NV_ItemHolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}

	return nRetCode;
}

MOBILETOOLKIT_API bool EnableLog(bool bEnableLog)
{
	bool bRes = true;
	if(bEnableLog)
	{
		char szTemp[] = {"enableLOG"};
		CCloud::Put(_T("LogPolicy"),sizeof(szTemp),szTemp);
	}
	else
	{
		char szTemp[] = {"disableLOG"};
		CCloud::Put(_T("LogPolicy"),sizeof(szTemp),szTemp);
	}
	return bRes;
}

void AddLog(int nPort,LPCTSTR szMsg)
{
#ifndef DISABLE_LOG
	CString strFormat;
	strFormat.Format(_T("Test_COM%02d.txt"),nPort);

	LogPolicy::Log(QSYNC_LIB::GetAnsi(szMsg).c_str(),strFormat);
#endif//#ifndef DISABLE_LOG
}

CCriticalSection	m_LockObj;

MOBILETOOLKIT_API bool GetBTAddr(int nPort,char* outBTAddrBuffer,int nBufferSize)
{	
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CNVItem nv(nPort);
	string strBTAddr = nv.GetBTAddr();
	if(static_cast<int>(strBTAddr.length()) < nBufferSize)
	{
		::memcpy(outBTAddrBuffer,strBTAddr.c_str(),strBTAddr.length());
		bRes = true;
	}
	return bRes;
}

MOBILETOOLKIT_API bool GetIMEI(int nPort,char* outIMEI) 
{	
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	string strIMEI = nv.GetIMEI();
	memcpy(outIMEI, strIMEI.c_str(), strIMEI.length());
	return true;
}

MOBILETOOLKIT_API bool WriteIMEI(int nPort,const char* inIMEI) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.WriteIMEI(inIMEI);
}

MOBILETOOLKIT_API bool LockSPC(int nPort)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.LockSPC();
}

MOBILETOOLKIT_API bool IsSPCLocked(int nPort,long& nStatus)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.IsSPCLocked(nStatus);
}

MOBILETOOLKIT_API bool UnlockSPC(int nPort,const char* szLock, bool& bUnlockOK)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.UnlockSPC(szLock,bUnlockOK);
}

MOBILETOOLKIT_API bool ProgramSPC(int nPort,const char* szLock)
{
	CNVItem nv(nPort);
	return nv.ProgramSPC(szLock);
}

/*
Return true means the command had send to handset.
Return false means timeout to get the response from handset

The return value can't promise the BT pair is ok or not.
*/
MOBILETOOLKIT_API bool WriteHeadsetBTAddress(const char* strBTAddr, int nPort) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nPort);
	bool bRes = wireless.WriteHeadsetBTAddress(strBTAddr, 3000);
	return bRes;
}

/*
The return value can't promise the BT pair is ok or not.
他只能說明增經友配對成功過
Only one it can descript is that the handset had even paired succeed.
*/
MOBILETOOLKIT_API bool CheckBTHeadsetState(const char *szBTAddr,int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.QueryBTPairCommand(szBTAddr);
}

MOBILETOOLKIT_API bool ClearActivationData(int nPort) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.ClearActivationData();
}

bool CreateCOMPort(int nPort)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	TCHAR szCOM[50] = {0};
	if ( nPort > 9 )// COM port number larger than 9, add prefix string.
		::sprintf_s(szCOM, _T("\\\\.\\COM%d"),nPort);
	else	
		::sprintf_s(szCOM, _T("COM%d"),nPort);
	HANDLE hFile = ::CreateFile(szCOM,
		GENERIC_READ|GENERIC_WRITE|MAXIMUM_ALLOWED,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		::CloseHandle(hFile);
		bRes = true;
	}

	return bRes;
}

MOBILETOOLKIT_API bool StandbyCurrentTest(int nPort) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CNVItem nv(nPort);

	if(nv.StandbyCurrentTest() == true)
	{
		int nMaxRetry = 10;
		while(CreateCOMPort(nPort) && nMaxRetry-- > 0)
			nv.StandbyCurrentTest();
		bRes = nMaxRetry > 0;
	}

	return bRes;
}

MOBILETOOLKIT_API bool MasterClear(int nPort) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.MasterClear();
}

MOBILETOOLKIT_API bool WriteQFuse(int nPort, unsigned int chain, unsigned long int Config_Register[5]) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.WriteQFuse(chain, Config_Register[0], Config_Register[1], Config_Register[2], Config_Register[3], Config_Register[4]);
}

MOBILETOOLKIT_API bool ReadQFuseState(int nComport, unsigned long& reg1, unsigned long& reg2, bool& bSuccess) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.ReadQFuseState(reg1, reg2, bSuccess);
}

MOBILETOOLKIT_API bool BTHeadsetChargingPower(int nPort, bool IsOn) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.BTHeadsetChargingPower(IsOn);
}

MOBILETOOLKIT_API bool CheckSIMCardExistence(int nPort, int& nType) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nPort);
	return nv.CheckSIMCardExistence(nType);
}

MOBILETOOLKIT_API bool ReadFactoryAreaNew(int nComport, WORD nNVItem, unsigned char* szFactoryArea128, int& nState, int nTimeOut) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.ReadFactoryAreaNew(szFactoryArea128, nState, nTimeOut);
}

MOBILETOOLKIT_API bool OnLineMode(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.OnLineMode();
}

MOBILETOOLKIT_API bool ChangeMode(int nComport,const unsigned int mode)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CControlModeSet ControlMode(nComport);
	return ControlMode.DiagControlMode(mode);
}

MOBILETOOLKIT_API bool SetOnline(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.SetOnline();
}

MOBILETOOLKIT_API bool SetOffline(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.SetOffline();
}

MOBILETOOLKIT_API bool ResetHandset(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.ResetHandset();
}

MOBILETOOLKIT_API bool BTHeadsetVoltageReading(int nComport, long& nVoltage, int nTimeout) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.BTHandsetVoltageReading(nVoltage, nTimeout);
}

MOBILETOOLKIT_API bool ChangeOnline(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.ChangeOnline();
}

MOBILETOOLKIT_API bool FTMMode(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	return ChangeMode(nComport,MODE_FTM_F);
}

MOBILETOOLKIT_API bool ChangeFTM(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CNVItem nv(nComport);
    return nv.ChangeFTM();
}

MOBILETOOLKIT_API bool CheckFTM(int nComport, int& nFTMValue) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CNVItem nv(nComport);
    return nv.CheckFTM(nFTMValue);
}

MOBILETOOLKIT_API bool EnableQualcommDebugEvent(int nComport,bool bEnable)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CNVItem nv(nComport);
    return nv.EnableDiagEvent(bEnable);
}

MOBILETOOLKIT_API bool WriteSIMLockFileNameToNV(int nComport,const std::string& strSIMLockFileName)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CNVItem nv(nComport);
    return nv.WriteSIMLockFileNameToNV(strSIMLockFileName);
}

MOBILETOOLKIT_API bool WriteSIMLockFileNameToNV_VC6(int nComport,const char* strSIMLockFileName)
{
	return WriteSIMLockFileNameToNV(nComport,strSIMLockFileName);
}

MOBILETOOLKIT_API bool ReadSIMLockFileNameFromNV(int nComport,std::string& strSIMLockFileName)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CNVItem nv(nComport);
    return nv.ReadSIMLockFileNameFromNV(strSIMLockFileName);
}

MOBILETOOLKIT_API bool WriteSimData(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CODM odm(nComport);
    return odm.WriteSimData(pSimDataBuf,nSimDataBufLen);
}

MOBILETOOLKIT_API bool WriteSimDataV2(long nComport, BYTE *pSimDataBuf, int nSimDataBufLen)
{
	BYTE ucSimAppLen = 16;
	BYTE ucSimApp[16] = {0};		
	DWORD nCmdStatus = 0, nStatus = 0;
	CSimLockStore simStore(nComport);

	if (simStore.StoreSimData(1, ucSimAppLen, ucSimApp, nSimDataBufLen, pSimDataBuf, &nCmdStatus, &nStatus))
		return (nCmdStatus == 0 && nStatus == 0);
	else
		return false;
}

MOBILETOOLKIT_API bool WriteSimLockFile(long nComport, const std::string& strSIMLockFileName)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    bool bRes = false;
    if(::_access(strSIMLockFileName.c_str(),0) != -1)
    {
        FILE* hFile = NULL;
        ::fopen_s(&hFile,strSIMLockFileName.c_str(),"rb");
        if(hFile)
        {
            const int nSIMLockMaxSize = 512;
            int nReadLen = 0;
            BYTE pBuffer[nSIMLockMaxSize+1] = {0};
            BYTE *pIndex = pBuffer;
            while(::fread(pIndex++,1,1,hFile) == 1)
            {
                nReadLen++;
                if(nReadLen >= nSIMLockMaxSize)
                    break;
            }
            ::fclose(hFile);
            if(nReadLen <= nSIMLockMaxSize)
                bRes = WriteSimData(nComport,pBuffer,nReadLen);
        }		
    }
    return bRes;
}

#include "../../odmtma_api\DownloadDLL\NV_ItemHolder.h"

bool LineToNVItem(long nComport,const CString& strLine, std::vector<CNV_ItemHolder>& vNVItem, CNVItem& nv)
{
    bool bRes = false;

    if(!strLine.IsEmpty())
    {		
        int npos = 0;

        // Find the NV name using the delimiter ";"
        CString strToken = strLine.Tokenize(_T(";"),npos);
        if ( -1 != npos)
        {
            CNV_ItemHolder objCurNV;
            objCurNV.SetName(strToken);

            // Find the NV ID using the delimiter ";"
            strToken = strLine.Tokenize(_T(";"),npos);
            if ( -1 != npos)
            {
                objCurNV.SetNV_ID(strToken);
                WORD wNVid = atoi(strToken.GetBuffer());
                DIAG_NVITEM_PACKET_F_type ReadResp;
                if(nv.ReadNVItem(wNVid,ReadResp) && ReadResp.nv_stat == 0)
                {
                    objCurNV.SetData(ReadResp.item_data, 128);
                    vNVItem.push_back(objCurNV);
                    bRes = true;
                }

                if(bRes == false)
                {
                    CString strFormat;
                    strFormat.Format(_T("ReadNVItem Fail : NV ITEM == %d, State == %d"),wNVid,ReadResp.nv_stat);
                    AddLog(nComport,strFormat);
#ifdef _DEBUG
                    bRes = true;// Record the ID and keep going
#endif
                }
            }
        }
    }
    return bRes;
}

MOBILETOOLKIT_API bool BackupNV2BRT_VC6(long nComport,const char* strOutputBRTFileName,const char* strInputCSVFile)
{
	return BackupNV2BRT(nComport, strOutputBRTFileName, strInputCSVFile);
}

MOBILETOOLKIT_API bool BackupNV2BRT(long nComport,const std::string& strOutputBRTFileName,const std::string& strInputCSVFile)
{
	CSingleLock singleLock(&m_LockObj,TRUE);
    CStdioFile csvFile;
	UINT nSuccess = 0;
	bool bRes = false;
    if( TRUE == csvFile.Open(strInputCSVFile.c_str(),CFile::shareDenyNone | CFile::modeRead) )
    {		
        CStringA strLine;
        csvFile.ReadString(strLine);

        std::vector<CNV_ItemHolder> vNVItem;
        CNVItem nv(nComport);
        while ( csvFile.ReadString(strLine) )
        {
            strLine.Trim();
            if(!strLine.IsEmpty() )
            {
                if(LineToNVItem(nComport,strLine, vNVItem, nv))
					nSuccess++;                
            }
        }
        csvFile.Close();

        CFile oFile;
        if (vNVItem.size()>0 && TRUE == oFile.Open(strOutputBRTFileName.c_str(),CFile::modeCreate | CFile::modeWrite))
        {
            int nNVCount = static_cast<int>(vNVItem.size());
            oFile.Write(&nNVCount,sizeof(int));
            for (std::vector<CNV_ItemHolder>::iterator iter = vNVItem.begin(); iter!=vNVItem.end(); iter++)
            {
                int iVal = iter->GetNV_ID();
                oFile.Write(&iVal,sizeof(int)); //nv id
                iVal = iter->GetSize();
                oFile.Write(&iVal,sizeof(int)); //item size
                oFile.Write( iter->GetData(), iter->GetSize() ); //item data
            }
            oFile.Close();
            bRes = true;
        }
    }

    return bRes;
}

MOBILETOOLKIT_API bool RestoreBRT2NV(long nComport,const std::string& strInBRTFileName)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    bool bRes = false;

    std::vector<CNV_ItemHolder> vNVItem;

    CFile oFile;
    if (oFile.Open(strInBRTFileName.c_str(),CFile::shareDenyNone| CFile::modeRead))
    {		
        int nvCount = 0;
        UINT nRead = oFile.Read(&nvCount,sizeof(int));
        while(nRead > 0)
        {
            CNV_ItemHolder NVHolder;

            int iVal = 0;
            nRead = oFile.Read(&iVal,sizeof(int));//nvid
            NVHolder.SetNV_ID(iVal);
            nRead = oFile.Read(&iVal,sizeof(int));
            NVHolder.SetSize(iVal);
            nRead = oFile.Read(NVHolder.GetData(),iVal);//data

            if(nRead == iVal && nRead > 0)
                vNVItem.push_back(NVHolder);
            else
                break;
        }
        bRes = true;
    }

    if(bRes)
    {
        CNVItem nv(nComport);
        for(std::vector<CNV_ItemHolder>::iterator iter = vNVItem.begin(); iter!=vNVItem.end(); iter++)
        {
            if (iter->GetSize() > 128)
            {
                assert(0);
            }
            else
            {
                DIAG_NVITEM_PACKET_F_type ReadResp = {0};
                if(nv.WriteNVItem(iter->GetNV_ID(),ReadResp,iter->GetData(),iter->GetSize()))
                {
                    if(ReadResp.nv_stat == NV_DONE_S)
                        bRes = true;
                    else if(ReadResp.nv_stat == NV_BUSY_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_BADCMD_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_FULL_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_FAIL_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_NOTACTIVE_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_BADPARM_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_READONLY_S)
                        bRes = false;//Remember unlock SPC
                    else if(ReadResp.nv_stat == NV_BADTG_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_NOMEM_S)
                        bRes = false;
                    else if(ReadResp.nv_stat == NV_NOTALLOC_S)
                        bRes = false;
                    else
                        assert(0);

                    if(bRes == false)
                    {
                        CString strFormat;
                        strFormat.Format(_T("WriteNVItem Fail : NV ITEM == %d, State == %d"),iter->GetNV_ID(),ReadResp.nv_stat);
                        AddLog(nComport,strFormat);
                        bRes = true;
#ifdef _DEBUG
                        bRes = true;// Record the ID and keep going
#endif
                    }
                }
                else
                {
                    bRes = false;

                    CString strFormat;
                    strFormat.Format(_T("WriteNVItem Fail : NV ITEM == %d"),iter->GetNV_ID());
                    AddLog(nComport,strFormat);
                    bRes = true;
#ifdef _DEBUG
                    bRes = true;// Record the ID and keep going
#endif
                }
            }

            if(bRes == false)
                break;
        }
    }

    return bRes;
}

MOBILETOOLKIT_API bool BackupNVPartition(long nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.BackupNVPartition(30000);
}

MOBILETOOLKIT_API bool RestoreNVPartition(long nComport)
{	
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.RestoreNVPartition(30000);
}

MOBILETOOLKIT_API bool CheckNVPartition(long nComport, int& nBackupTimes, CTime& LastBackupTime)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CNVItem nv(nComport);
	return nv.CheckNVPartition(nBackupTimes,LastBackupTime);
}

MOBILETOOLKIT_API bool EnableGAP(long nComport,bool bEnable)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.EnableGAP(bEnable);
}

MOBILETOOLKIT_API bool StartBTPower(long nComport,int nDegree)
{
	CWireless wireless(nComport);
	return wireless.StartBTPower(nDegree);
}

MOBILETOOLKIT_API bool EnterBTTestMode(long nComport,bool bEnable)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.EnterBTTestMode(bEnable);
}

MOBILETOOLKIT_API bool GetSWHWVersion(long nComport,LPCSTR sSW, LPCSTR sHW)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CQisdaMiscellaneousModule Miscellaneous(nComport);
	return Miscellaneous.GetSWHWVersion(sSW, sHW);
}

MOBILETOOLKIT_API bool GetLavernockHWVersion(long nComport,int& nVersion)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComport);
	return odm.GetLavernockHWVersion(nVersion);
}


MOBILETOOLKIT_API bool ReadNVItem(long nComport, const WORD& NVItem, BYTE  item_data[128], WORD&  nv_stat)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CNVItem nv(nComport);

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	if(nv.ReadNVItem(NVItem, ReadResp))
	{
		nv_stat = ReadResp.nv_stat;
		memcpy(item_data, ReadResp.item_data, 128);
		bRes = true;
	}
	else
	{
		nv_stat = ReadResp.nv_stat;
		bRes = false;
	}

	return bRes;
}

MOBILETOOLKIT_API bool WriteNVItem(long nComport,const WORD& NVItem, const BYTE *WriteData, const size_t szWriteData,WORD&  nv_stat)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CNVItem nv(nComport);

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	if(nv.WriteNVItem(NVItem, ReadResp,WriteData, szWriteData))
	{
        if(ReadResp.nv_stat == 5)
            nv.WriteNVItem(NVItem, ReadResp,WriteData, szWriteData);
		nv_stat = ReadResp.nv_stat;
		bRes = true;
	}

	return bRes;
}

MOBILETOOLKIT_API bool TurnOnOffEM(long nComport,bool bOpen)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CNVItem nv(nComport);

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	BYTE item_value = bOpen ? 1 : 0;
	bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_MFG_TEST_I,ReadResp,&item_value,1);
	if(bRes)
		bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_TEXT_LABEL_TEST_I,ReadResp,&item_value,1);
	if(bRes)
		bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_LCD_TEST_I,ReadResp,&item_value,1);
	if(bRes)
		bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_SLT_ALT_TEST_I,ReadResp,&item_value,1);
	if(bRes)
		bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_EFEM_I,ReadResp,&item_value,1);
	return bRes;
}

MOBILETOOLKIT_API bool WriteFailureByte(int nComport,BYTE nFailureByte)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	bool bRes = false;
	CODM odm(nComport);
	bRes = odm.WriteFailureByte(nFailureByte);
	return bRes;
}

MOBILETOOLKIT_API bool SearchBTDevice(int nComport) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    bool bRes = false;
    CNVItem nv(nComport);
    if((bRes = nv.SearchBTDevice())) {
        Sleep(10000);
        bRes = nv.QuerySearchBTResult();
    }
    return bRes;
}

MOBILETOOLKIT_API bool ReadBatteryTemperature(int nComportm, int& nTemperature)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CNVItem nv(nComportm);
    return nv.ReadBatteryTemperature(nTemperature);
}

MOBILETOOLKIT_API bool ReadBatteryVoltage(int nComportm, int& nVoltage) 
{
	CSingleLock singleLock(&m_LockObj,TRUE);

    CNVItem nv(nComportm);
    return nv.ReadBatteryVoltage(nVoltage);
}

MOBILETOOLKIT_API bool WriteDefaultPhoneCode(int nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComport);
	return odm.WriteDefaultPhoneCode();
}


MOBILETOOLKIT_API bool FreezeOTP(long nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComport);
	return odm.FreezeOTP();
}

MOBILETOOLKIT_API bool IsOTPFrozen(long nComport, bool& bFrozen,int& nLastError)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComport);
	bFrozen = odm.IsOTPFrozen(nLastError);
	return nLastError == DIAG_ERROR_SUCCESS || nLastError == DIAG_ERROR_EMPTY_OTP;
}

MOBILETOOLKIT_API bool ReplaceEFSFile(int nComPort, const std::string& strEFSFilePath, const std::string& strNewLocalFilePath)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComPort);
	return odm.QLIB_ReplaceFile(strEFSFilePath,strNewLocalFilePath);
}

MOBILETOOLKIT_API bool IsGSensorWorkingV2(int nComPort,bool& bIsGSensorWork,bool& bIsUpWork)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComPort);
	return odm.IsGSensorWork(bIsGSensorWork,bIsUpWork);
}

MOBILETOOLKIT_API bool IsGSensorWorkingV3(int nComPort,int& nX,int& nY,int& nZ)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComPort);
	return odm.IsGSensorWork(nX,nY,nZ);
}

MOBILETOOLKIT_API bool IsGSensorWorking(int nComPort)
{
	bool bIsGSensorWork = false;
	bool bIsUpWork = false;
	return IsGSensorWorkingV2(nComPort,bIsGSensorWork,bIsUpWork);
}

MOBILETOOLKIT_API bool CheckSIMLock(int nComPort,std::string& strSIMLockName)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CSimLockStatus simlock(nComPort);
	return simlock.CheckSIMLock(strSIMLockName);
}

MOBILETOOLKIT_API bool CheckSIMLock_VC6(int nComPort,char* szSIMLockNameBuffer,int nBufferSize)
{
	std::string strSIMLockName = "Empty............................................................";
	bool bRes = CheckSIMLock(nComPort,strSIMLockName);
	if(bRes && nBufferSize >= static_cast<int>(strSIMLockName.size()))
	{
		::strcpy_s(szSIMLockNameBuffer,nBufferSize,strSIMLockName.c_str());
		bRes = true;
	}
	else
		bRes = false;
	return bRes;
}

MOBILETOOLKIT_API bool CheckCEFSFileName(int nComPort,char* szCEFSFileNameBuffer,int nBufferSize)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComPort);	
	return odm.QLIB_GetCEFSFilename(szCEFSFileNameBuffer,nBufferSize);
}

MOBILETOOLKIT_API bool PowerOff(int nComport) {
    CSingleLock singleLock(&m_LockObj,TRUE);
    CNVItem nv(nComport);
    nv.PowerOff();
    return true;
}

MOBILETOOLKIT_API bool DLRAWRequest(int nComport, const char* szCmdInput, int nCmdLength, bool bAddCRC)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComport);
	odm.DLRAWRequest(szCmdInput, nCmdLength, bAddCRC);
	return true;
}

MOBILETOOLKIT_API bool DiagRAWRequest(int nComport, const char* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComport);
	return odm.DiagRAWRequest(szCmdInput, nCmdLength, bAddCRC,bHasReturn);
}

MOBILETOOLKIT_API bool ByPassCellTemperatureCheck(int nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CODM odm(nComport);
	return odm.ByPassCellTemperatureCheckRequest();
}

MOBILETOOLKIT_API bool FMRegistration(int nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMRegistration();
}

MOBILETOOLKIT_API bool FMConnect(int nComport,bool bConnect)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMConnect(bConnect);
}

MOBILETOOLKIT_API bool FMInitialize(int nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMInitialize();
}

MOBILETOOLKIT_API bool FMSetStation(int nComport,WORD nFMFrequence)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMSetStation(nFMFrequence);
}

MOBILETOOLKIT_API bool FMMute(int nComport,bool bMute)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMMute(bMute);
}

MOBILETOOLKIT_API bool FMReceiveRSSI(int nComport,WORD& nRSSI)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMReceiveRSSI(nRSSI);
}

MOBILETOOLKIT_API bool FMSpeaker(int nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMSpeaker();
}

MOBILETOOLKIT_API bool FMMonoHeadset(int nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMMonoHeadset();
}

MOBILETOOLKIT_API bool FMStereoHeadset(int nComport)
{
	CSingleLock singleLock(&m_LockObj,TRUE);

	CWireless wireless(nComport);
	return wireless.FMStereoHeadset();
}

MOBILETOOLKIT_API bool IsInDownloadMode(int nComport) 
{
    CCFCDiagCommand nv(nComport);
    return nv.IsDLMode();
}

MOBILETOOLKIT_API bool WriteMobileID(int nComport, char* szSlot3)
{
	CNVItem nvItem(nComport);
	return nvItem.WriteMobileID(szSlot3);
}

MOBILETOOLKIT_API bool ReadMobileID(int nComport, char* szLine7,char* szAdapter3,char* szSlot3,char* szDay3,char* szMonth3,char* szYear3,char* szHour3,char* szMinute3,char* szSecond3)
{	
	CNVItem nvItem(nComport);
	return nvItem.ReadMobileID(szLine7,szAdapter3,szSlot3,szDay3,szMonth3,szYear3,szHour3,szMinute3,szSecond3);
}

MOBILETOOLKIT_API bool ReadHWVersion_FromFactoryArea(int nComport, char* szHWVersion)
{
	BYTE FactoryBlock[128] = {0};
	WORD nvStatus = 0;
	bool bRes = ReadNVItem(nComport, NV_FACTORY_DATA_1_I, FactoryBlock, nvStatus);
	if (bRes)
	{
		memcpy(szHWVersion, FactoryBlock + 89, 5);
		szHWVersion[5] = 0;
	}
	else;
	return bRes;
}

MOBILETOOLKIT_API bool WriteHWVersion_ToFactoryArea(int nComport, char* szHWVersion)
{
	BYTE FactoryBlock[128] = {0};
	WORD nvStatus = 0;
	ReadNVItem(nComport, NV_FACTORY_DATA_1_I, FactoryBlock, nvStatus);
	memcpy(FactoryBlock + 89, szHWVersion, 5);
	bool bRes = WriteNVItem(nComport, NV_FACTORY_DATA_1_I, FactoryBlock, 128, nvStatus);
	return bRes;
}
