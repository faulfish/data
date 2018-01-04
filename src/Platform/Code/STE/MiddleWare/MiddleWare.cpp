// MiddleWare.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "MiddleWare.h"
#include <assert.h>
#include <string>
#include "..\..\Modules\QisdaNVModule\NVItem.h"
#include "..\..\Modules\ODMCoachModule\ODM.h"
#include "..\..\QLib_Export\QLib_export.h"
#include "..\..\odmtma_api\SerCOMLib\SerCOMLibrary.h"
#include "..\..\Modules\QisdaWirelessModule\QisdaWirelessModule.h"
#include "..\..\Modules\QisdaWirelessModule\EnumFile.h"
#include "../../ManualCITTestTool/KeypadCommand.h"
#include "CallbackTypes.h"
#include "findport.h"
#include "FailureByteDlg.h"
#include "GSensorCalibrationDlg.h"
#include "..\..\Modules\QisdaMiscellaneousModule\QisdaMiscellaneousModule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

MIDDLEWARE_API bool AddFailure(HWND hWnd)
{	
    bool bRes = false;

    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};

    if(!(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)) return false;
    
    TCHAR szModel[512] = {0};
    ReadModel(szModel);
    TSTRING strModel = szModel;
    if(strModel == _T("Athens")) {
        CFailureByteDlg dlg(Ports[0], 0);
        dlg.DoModal();
        bRes = true;
    } else if(strModel == _T("Lavernock")) {
        CFailureByteDlg dlg(Ports[0], 1);
        dlg.DoModal();
        bRes = true;
    }
    return bRes;
}

MIDDLEWARE_API bool GSensorCalibration(HWND hWnd) {
    bool bRes = false;

    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};

    if(!(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)) return false;

    TCHAR szModel[512] = {0};
    ReadModel(szModel);
    TSTRING strModel = szModel;
    if(strModel == _T("Lavernock")) {
        CGSensorCalibrationDlg dlg(Ports[0]);
        dlg.DoModal();
        bRes = true;
    }
    return bRes;
}


extern pMessageSender g_pMessageSender;

// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

MIDDLEWARE_API bool SetCallBack(pMessageSender pCallBack) {
    *GetCallbackFunctionPointer() = pCallBack;
    return true;
}


MIDDLEWARE_API bool GetIMEI(TCHAR* szIMEI)
{
	bool bRes = false;
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
	{
		CNVItem nvItem(Ports[0]);
		::_tcscpy(szIMEI,nvItem.GetIMEI().c_str());
		bRes = true;
	}
	return bRes;
}

MIDDLEWARE_API bool GetMotoXPRSLogs(TCHAR* szLogPath)
{
	bool bRes = false;
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
	{
		CODM odm(Ports[0]);
		TCHAR szPath[MAX_PATH] = {0};
		if(odm.GetMotoXprsLogger_Athens(szPath,MAX_PATH))
		{		
			::_tcscpy(szLogPath,szPath);
			bRes = true;
		}
	}
	return bRes;
}

MIDDLEWARE_API bool ReadBootLoaderVersion(TCHAR* outputString)
{
	assert(0);//Need to implement
    return true;
}

MIDDLEWARE_API bool ReadModel(TCHAR* outputString)
{
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};

	if(!QL_GetDiagPort(&nPorts, Ports) || nPorts == 0)
		return false;

	const int iTryMaxCounter = 5;
	int iTryCounter = 0;
	TCHAR cSW[100] = {0};
	TCHAR cHW[100] = {0};

	CQisdaMiscellaneousModule cMiscellaneous(Ports[0]);
	while(!cMiscellaneous.GetSWHWVersion(cSW, cHW) && iTryCounter < iTryMaxCounter)
		iTryCounter++;

	if(iTryCounter == iTryMaxCounter)
		::ZeroMemory(outputString, sizeof(outputString));
	else
		strcpy(outputString, cHW);

	return true;
}

bool OpenExecuate(const CString& strExecuatePath)
{
	bool bRes = false;
	TCHAR szCurrentPath[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurrentPath);
	CString strDialogPath = CString(szCurrentPath) + strExecuatePath;
	CString strWorkPath = CString(szCurrentPath) + _T("\\qisda");

	SHELLEXECUTEINFO shellInfo = {0};
	shellInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	shellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	shellInfo.lpFile = strDialogPath.GetString();
	shellInfo.lpParameters = NULL;
	shellInfo.nShow = SW_SHOW;
	shellInfo.lpDirectory = strWorkPath;
	DWORD result;
	bRes = ::ShellExecuteEx(&shellInfo) == TRUE;

	while(bRes) 
	{
		MSG msg;
		while(::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			//Filter any mouse event on the parent's window
			if(msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST) 
				continue;

			//Avoid parent close 
			if(msg.message == WM_QUIT || msg.message == WM_CLOSE) 
			{
				::TerminateProcess(shellInfo.hProcess,0);
				::WaitForSingleObject(shellInfo.hProcess, INFINITE);
			}

			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		result = ::MsgWaitForMultipleObjects(1, &shellInfo.hProcess, FALSE, INFINITE, QS_ALLINPUT);
		if(result == (WAIT_OBJECT_0 + 1))
			continue;
		else
			break;
	}

	return bRes;
}



MIDDLEWARE_API bool OpenBackupRestoreDialog(HWND hWnd) 
{
	return OpenExecuate(_T("\\Qisda\\BRDialog.exe"));
}

MIDDLEWARE_API bool OpenMultiDownload(HWND hWnd)
{
    TCHAR szModelName[512] = {0};
    bool bReadOut = ReadModel(szModelName);
    if(!bReadOut) return false;
    TSTRING strModelName = szModelName;
    if(strModelName == _T("Ahtens"))
        return OpenExecuate(_T("\\Qisda\\MultiDownload_ShareU.exe"));
    else if(strModelName == _T("Lavernock"))
        return OpenExecuate(_T("\\Qisda\\MultiDownloadForAll_ShareU.exe"));
    else if(strModelName == _T("Melos"))
        return OpenExecuate(_T("\\Qisda\\MultiDownloadForAll_ShareU.exe"));
}

MIDDLEWARE_API bool OpenRestoreBRTDialog(HWND hWnd) {
    return OpenExecuate(_T("\\Qisda\\BackupRestoreBRT.exe"));
}

bool OpenComport(CSerCOMLibrary &objCom, int& nPort) 
{
	bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    string strOutput;
    if(!QL_GetDiagPort(&nPorts,Ports) || nPorts < 1) 
	{
        OutputPrompt("Manual CIT Test", "Port Error!", "OK", strOutput);
    }
	else
	{
		TCHAR szBuf[10] = {0};
		_stprintf(szBuf, _T("\\\\.\\COM%d"), Ports[0]);
		if(objCom.OpenComport(_T("NULL"), szBuf) < 0) 
		{
			OutputPrompt("Manual CIT Test", "Port Error!", "OK", strOutput);
		}
		else
		{
			Sleep(1000);
			nPort = Ports[0];
			bRes = true;
		}
	}
    return bRes;
}

//EFS ************************Start**************************//
MIDDLEWARE_API bool EFS_ReadFile(const TCHAR *sFileSrc, const TCHAR *sFileDst)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
        if(efs.EfsReadFile(sFileSrc,sFileDst,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_WriteFile(const TCHAR *sFileSrc, const TCHAR *sFileDst)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
        if(efs.EfsWriteFile(sFileSrc,sFileDst,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_DeleteFile(const TCHAR *sFile)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
        if(efs.EfsDeleteFile(sFile,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_Rename(const TCHAR *sOldName, const TCHAR* sNewName)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
        if(efs.EFsRename(sOldName,sNewName,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}


MIDDLEWARE_API bool EFS_NewFolder(const TCHAR *sFolder)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
        if(efs.EfsMakeFolder(sFolder,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_DeleteFolder(const TCHAR *sFolder)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
        if(efs.EFsRemoveFolder(sFolder,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_QueryFDStatus()
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
	DWORD iTotalNumOfBlocks, iNumPagesPerBlock, iPageSize, iTotalPageSize, iMakerID, iDeviceID;
	BYTE iDeviceType;
	TCHAR psDeviceName[200];
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EFsQueryFlashDeviceInfo(iTotalNumOfBlocks,iNumPagesPerBlock,iPageSize,iTotalPageSize,iMakerID,iDeviceID,iDeviceType,psDeviceName, iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_QueryFSStatus(const TCHAR *sFile)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
	DWORD iFS_I, iFS_Type, iBlockSize, iTotalBlocks, iAvailBlocks, iFreeBlocks, iMaxFileSize,iNfiles,iMaxNfiles;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EFsQueryFileSystemStatus(sFile, iFS_I, iFS_Type, iBlockSize, iTotalBlocks, iAvailBlocks, iFreeBlocks, iMaxFileSize,iNfiles,iMaxNfiles,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_QueryFileStatus(const TCHAR *sFile)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
	DWORD iMode, iSize, iNlink,iAtime,iMtime,iCtime;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EFsQueryFStatus(sFile, iMode, iSize, iNlink,iAtime,iMtime,iCtime,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_CreateLink(const TCHAR *sFile, const TCHAR* sLinkFile)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EfsCreateLink(sFile,sLinkFile,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_ReadLink(const TCHAR *sLinkFile, TCHAR* sFile)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EfsReadLink(sLinkFile,sFile,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_Chomd(const TCHAR *sFile, const DWORD iMode)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EFsChomd(iMode,sFile,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool EFS_CheckAccessAttribute(const TCHAR *sFile, const WORD iPermissionBits)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EFsCheckAccessAttribute(iPermissionBits,sFile,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}


MIDDLEWARE_API bool EFS_ListFolder(const TCHAR *sFolder, std::vector<Efs2DirItem> *pDirList, DWORD &iSize,BYTE entryType)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CEFs efs(Ports[0]);
		if(efs.EfsDirectoryList(sFolder, pDirList, iSize,entryType,iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}
//EFS ************************End**************************//

//Audio ************************Start**************************//

MIDDLEWARE_API bool Audio_SetPath(const WORD iAudioDevice)
{
    bool bRes = false;
    WORD nPorts = 10;
    WORD Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_SetPath(iAudioDevice, iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_SetVolume(const BYTE iVolume)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_SetVolume(iVolume, iErrorCode))
        {
            bRes = true;
        }
    }
    return bRes;
}
MIDDLEWARE_API bool Audio_DSPLoopBack(const bool bOn)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_DSPLoopBack(bOn, iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_PCMLoopBack(const bool bOn)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_PCMLoopBack(bOn, iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_NSControl(const bool bOn)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_NSControl(bOn, iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_TonesPlay(const WORD iHiFreq, const WORD iLoFreq, const WORD iMethod)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_TonesPlay(iHiFreq,iLoFreq,iMethod, iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_TonesStop()
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_TonesStop(iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_SetAdjustCode(const short iCodecTxGainAdjust)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_SetCodectxgain_Adjust(iCodecTxGainAdjust, iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_PCMCaptureStart(const WORD iNumBuffers, const WORD iDelays)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_PCMCaptureStart(iNumBuffers,iDelays,iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_PCMCaptureStop()
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_PCMCaptureStop(iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Audio_PCMCaptureGetData(const DWORD iOffset, const WORD iDataSize, char* pBuffer)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
	long iErrorCode;
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CAudio audio(Ports[0]);
        if(audio.Audio_GetPCMCaptureData(iOffset,iDataSize,pBuffer,iErrorCode))
		{
            bRes = true;
        }
    }
    return bRes;
}

//Audio ************************End**************************//
//Video ************************Start**************************//
MIDDLEWARE_API bool Video_GetInfo(WORD& iSensorWidth, WORD& iSensorHeight, WORD& iFPS, BYTE& iSensorRotationSensing, WORD& iDefaultRotation, \
				WORD& iDefaultOrientation, WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_GetInfo(iSensorWidth,iSensorHeight,iFPS,iSensorRotationSensing,iDefaultRotation,iDefaultOrientation,iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_GetParam(const WORD iParmId, DWORD& iMininumValue, DWORD& iMaximumValue, DWORD& iStepValue,DWORD& iDefaultValue, DWORD& iCurrentValue, WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_GetParam(iParmId,iMininumValue,iMaximumValue,iStepValue,iDefaultValue,iCurrentValue,iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_GetPicture(const DWORD iDataOffset, WORD& iDataSize, TCHAR* piPicData, WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_GetPciture(iDataOffset,iDataSize,piPicData,iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_PowerOff(WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_PowerOff(iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_PowerOn(WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_PowerOn(iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_PreviewStart(WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_PreviewStart(iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_PreviewStop(WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_PreviewStop(iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_SetDimension(const WORD iPictureWidth, const WORD iPictureHeight, const WORD iLCD_Width, WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_SetDimension(iPictureWidth,iPictureHeight,iLCD_Width,iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_SetParam(const WORD iParmId,const DWORD iParmValue,WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_SetParam(iParmId, iParmValue,iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_SetSensorId(const BYTE iSensorID, WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_SetSensorId(iSensorID,iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}

MIDDLEWARE_API bool Video_TakePicture(const DWORD iQuality, const WORD iFormat,DWORD& iPicDataSize, WORD& iCameraStatus)
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1)
    {
        CVideo video(Ports[0]);
        if(video.Camera_TakePicture(iQuality,iFormat,iPicDataSize,iCameraStatus))
		{
            bRes = true;
        }
    }
    return bRes;
}
//Video ************************End**************************//

//  [6/6/2008 Enzo]
// Start CAL and Test function ------------------------------ 
/*MIDDLEWARE_API int CallTest(TCHAR* band, TCHAR* outputString)
{
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int iBand = 0;
	int iStartBand = 10;
	int iErrCode = 0;
	int iGPIBAddress = 0;
	int iSstartCallBand =0;
	int iBandState = 0;
	int icomport = 0;
	bool bRes = false;
	//  [6/10/2008 Enzo] get comport
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	
	iBand = band[0];
	switch(iBand)
	{
	case FTM_GSM_BER_EGSM_900:
		iBandState = 10;
		break;
	case FTM_GSM_BER_DCS_1800:
		iBandState = 11;
		break;

	case FTM_GSM_BER_PCS_1900:
		iBandState = 12;
		break;
	}

	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1){
		icomport = Ports[0];
	}else{
		sprintf(strTracer, "Fail to get comport!!");
		memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
		return bRes;
	}

	if (iBand != PHONE_MODE_WCDMA_IMT){
		//  [6/10/2008 Enzo] get comport
		if ((iErrCode = doInitCallBox_GSM(iStartBand,icomport))!=PASS){
		//if ((iErrCode = doInitCallBox_GSM(iStartBand))!=PASS){
			sprintf(strTracer, "Fail to Initial call Box!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}

		if ((iErrCode = doFTMGSMStartCall(iGPIBAddress,iSstartCallBand))!=PASS){
			sprintf(strTracer, "Fail to Start call!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}

		switch(iBand)
		{
		case FTM_GSM_BER_EGSM_900:
			if ((iErrCode = doFTMGSMHandover(1))!=PASS){
				sprintf(strTracer, "Fail to hand over!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				return false;
			}
			break;
		case FTM_GSM_BER_DCS_1800:
			if ((iErrCode = doFTMGSMHandover(1))!=PASS){
				sprintf(strTracer, "Fail to hand over!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				return false;
			}
			break;
		case FTM_GSM_BER_PCS_1900:
			if ((iErrCode = doFTMGSMHandover(1))!=PASS){
				sprintf(strTracer, "Fail to hand over!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				return false;
			}
			break;
		default:
			sprintf(strTracer, "GSM Select Band Fail!!");
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}	

		if ((iErrCode = doEndTesting(iBandState))!=PASS){
			sprintf(strTracer, "Fail in end GSM!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}	
	}else if(iBand == PHONE_MODE_WCDMA_IMT){
		//  [6/10/2008 Enzo] get comport
		if ((iErrCode = doTestWCDMAInitSetting(iBand,icomport))!=PASS){
			sprintf(strTracer, "Fail to Initial WCDMA Set!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}

		if ((iErrCode = doInitBSEWCDMA(iGPIBAddress,iBand))!=PASS){
			sprintf(strTracer, "Fail to Initial BSE WCDMA!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}
		Sleep(3000);
		if ((iErrCode = doFTMWCDMAStartCall(iBand))!=PASS){
			sprintf(strTracer, "Fail to WCDMA Start Call!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);

			return false;
		}

		if ((iErrCode = doFTMWCDMARFMchMeasureAll(iBand))!=PASS){
			sprintf(strTracer, "Fail to WCDMA Measure Middle Channel!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}
		if ((iErrCode = doTestWCDMAEnd())!=PASS){
			sprintf(strTracer, "Fail to WCDMAEnd!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			return false;
		}
 
	}

	return true;

}
*/

DWORD gdwStart = 0;

MIDDLEWARE_API int Calibration(TCHAR* band, TCHAR* outputString, int nStartEnd)
{	//nStartEnd == 0		Start
	//nStartEnd == 255		End
	//nStartEnd == other	Middle
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int iBand = 0;
	int iErrCode = 0;
	int iGPIBAddress = 0;
	int iStartBand = 10;
	int iBandState = 0;
	bool bRes = false;
	char* cBandtempPass;
	char* cBandtemp;
	char* cBandtempFail;
	char* cInstrumentType;
	int iTemp = 0;

	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	int icomport = 0;
 	std::string sAbortStatus;

	iBand = band[0];

	if(nStartEnd == 0)
		gdwStart = ::GetTickCount();

	TCHAR cImei[18];
	TCHAR cSW[100];
	TCHAR cHW[100];
	if(iBand == 1)
	{
		int i = 0;
		while(!GetIMEI(cImei) && i < 3)
		{
			i++;
		}
		if(i == 3)
		{
			cImei[0] = _T('N');
			cImei[1] = _T('/');
			cImei[2] = _T('A');
			cImei[3] = _T('\0');
			sprintf(strTracer, "Fail to Get IMEI from ME ErrorCode");
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputData(_T("Read IMEI"), _T("Failed"),(std::string)(""));
//			return false;
		}
	}

	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1){
		icomport = Ports[0];
	}else{

		CString strPort = GetCurrentActivePort();

		if ( strPort.GetLength() > 3)
		{
			icomport = StrToInt( strPort.Right(strPort.GetLength()-3).GetBuffer(0) );
		}
		else
		{
			sprintf(strTracer, "Fail to get comport!!");
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputHeader(_T("Fail to get comport!!"),(std::string)(""));
			return bRes;
		}
	}
	if(iBand == 1)
	{
		int i = 0;
		CQisdaMiscellaneousModule cMiscellaneous(icomport);
		while(!cMiscellaneous.GetSWHWVersion(cSW, cHW) && i < 3)
		{
			i++;
		}
		if(i ==3)
		{
			cSW[0] = _T('N');
			cSW[1] = _T('/');
			cSW[2] = _T('A');
			cSW[3] = _T('\0');
			sprintf(strTracer, "Fail to Get SW Version from ME ErrorCode");
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputData(_T("Get SW Version"), _T("Failed"),(std::string)(""));
		}
	}

	if (iBand == PHONE_MODE_WCDMA_IMT){
		
		OutputHeader(_T("Start to Calibration in UMTSBand1"),(std::string)(""));
		if ((iErrCode = doMobileOnFTM(iGPIBAddress,icomport))!=PASS){
			sprintf(strTracer, "Fail to set FTM mode!! in Band %d ---- ErrorCode = %x",iBandState, iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputDetailResult(_T("Change Mobile On FTM"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
		}

		if(iErrCode == PASS)
		{
			std::string sztemp;
			ManageResource(_T("Status"), _T("TestSet"), sztemp);
			ManageResource(_T("Register"), _T("TestSet"), sztemp);
			OutputDetailResult(_T("Change Mobile On FTM"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			iGPIBAddress = 0;
			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMAInit(iBand))!=PASS){
				sprintf(strTracer, "Fail to Initial WCDMA set!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("WCDMA Initial Setting"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("WCDMA Initial Setting"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));
			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMARxDVGAVsFreq(iBand))!=PASS){
				if ((iErrCode = doCalWCDMARxDVGAVsFreq(iBand))!=PASS){
					sprintf(strTracer, "Fail to Rx DVGA VsFreq!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputDetailResult(_T("WCDMA Rx DVGAVsFreq Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
				}
			}
		}
		
		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("WCDMA Rx DVGAVsFreq Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMARxLNAVsFreq(iBand))!=PASS){
				if ((iErrCode = doCalWCDMARxLNAVsFreq(iBand))!=PASS){
					sprintf(strTracer, "Fail to Rx LNA VsFreq!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputDetailResult(_T("WCDMA Rx LNAVsFreq Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
				}
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("WCDMA Rx LNAVsFreq Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}
		
		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMATxLinMaster(iBand))!=PASS){
				if ((iErrCode = doCalWCDMATxLinMaster(iBand))!=PASS){
					sprintf(strTracer, "Fail to Tx Lin Master!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputDetailResult(_T("WCDMA Tx LinMaster Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
				}
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("WCDMA Tx LinMaster Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMATxCompVsFreq(iBand))!=PASS){
				sprintf(strTracer, "Fail to Tx Comp VsFreq!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("WCDMA Tx CompVsFreq Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}
		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("WCDMA Tx CompVsFreq Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMARxDoDC(iBand))!=PASS){
				sprintf(strTracer, "Fail to Rx Do DC!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("WCDMA Rx Do DC Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}
		if(iErrCode == PASS)
		{

			OutputDetailResult(_T("WCDMA Rx Do DC Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));
			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMAWriteToNV(iBand))!=PASS){
				sprintf(strTracer, "Fail to Write To NV!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("WCDMA Write To NV"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}

		if(iErrCode == PASS)
		{

			OutputDetailResult(_T("WCDMA Write To NV"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));
			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		bool bOnline = false;
		if((nStartEnd == 255) || (iErrCode != PASS))
			bOnline = true;

		if(doCalEnd(bOnline)!=PASS)
		{
			if(iErrCode == PASS)
			{
				sprintf(strTracer, "Fail to end WCDMA Cal!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			}
			OutputDetailResult(_T("End WCDMA Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
		}
		else
			OutputDetailResult(_T("End WCDMA Calibration"),_T("UMTSBand1"),_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));


	}else{
		if ((iErrCode = doMobileOnFTM(iGPIBAddress,icomport))!=PASS){
			sprintf(strTracer, "Fail to select GPIB or change mode fail!! ---- ErrorCode = %x ", iErrCode);
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputData(_T("Change Mobile On FTM"),_T(" FAIL"),(std::string)(""));
		}

		if(iErrCode == PASS)
		{
			if (iBand == 1){
				int nTestSet = iGPIBAddress & 0xFF;
				if (nTestSet == 3){
					OutputData(_T("TestSet"),_T("CMU 200"),(std::string)(""));
				}else if (nTestSet == 4){
					OutputData(_T("TestSet"),_T("HP8960"),(std::string)(""));
				}else if(nTestSet == 5){
					OutputData(_T("TestSet"),_T("WillTek"),(std::string)(""));
				}else{
					OutputData(_T("TestSet"),_T("N/A"),(std::string)(""));
				}

				nTestSet = (iGPIBAddress >> 8) & 0xFF;
				if (nTestSet == 0){
					OutputData(_T("PowerSupply1"),_T("HpE3631"),(std::string)(""));
				}else if (nTestSet == 1){
					OutputData(_T("PowerSupply1"),_T("Hp66311"),(std::string)(""));
				}else if(nTestSet == 2){
					OutputData(_T("PowerSupply1"),_T("Hp66332A"),(std::string)(""));
				}else{
					OutputData(_T("PowerSupply1"),_T("N/A"),(std::string)(""));
				}

				OutputData(_T("PowerSupply2"),_T("N/A"),(std::string)(""));	
				OutputData(_T("IMEI"), cImei, (std::string)(""));
				OutputData(_T("SW Version"), cSW, (std::string)(""));
				return true;
			}

			switch(iBand)
			{
			case GSM_BAND_900:
				iBandState = 10;
				cBandtempPass = "Band GSM900 PASS";
				cBandtempFail = "Band GSM900 FAIL";
				cBandtemp = "Band GSM900";
				break;
			case GSM_BAND_DCS:
				iBandState = 11;
				cBandtempPass = "Band GSM1800 PASS";
				cBandtempFail = "Band GSM1800 FAIL";
				cBandtemp = "Band GSM1800";
				break;
			case GSM_BAND_PCS:
				iBandState = 12;
				cBandtempPass = "Band GSM1900 PASS";
				cBandtempFail = "Band GSM1900 FAIL";
				cBandtemp = "Band GSM1900";
				break;
			case PHONE_MODE_WCDMA_IMT:
				iBandState = 9;
				break;
			}

			OutputDetailResult(_T("Change Mobile On FTM"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));
			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			iGPIBAddress = 0;
			if ((iErrCode = doSetStartInit(iGPIBAddress,iStartBand,icomport))!=PASS){
				sprintf(strTracer, "Fail to calibration initial set!! ---- ErrorCode = %x ", iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("GSM Calibration Initial"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}

		if(iErrCode == PASS)
		{
			ManageResource(_T("Status"), _T("TestSet"), (std::string)(""));
			ManageResource(_T("Register"), _T("TestSet"), (std::string)(""));

			OutputDetailResult(_T("GSM Calibration Initial"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));
			
			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}


		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalThermCalibration(iBandState))!=PASS){
				sprintf(strTracer, "Fail to Therm Calibration!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("GSM Therm Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("GSM Therm Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalGSM4xFMODCalibration(iBandState))!=PASS){
				if ((iErrCode = doCalGSM4xFMODCalibration(iBandState))!=PASS){
					sprintf(strTracer, "Fail to 4xFMOD Calibration!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputDetailResult(_T("GSM 4xFMOD Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
				}
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("GSM 4xFMOD Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iBand == GSM_BAND_DCS) || (iBand == GSM_BAND_PCS)){
				if ((iErrCode = doCalGSMPout(iBandState))!=PASS){
					sprintf(strTracer, "Fail to Pout Calibration!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputDetailResult(_T("GSM Pout Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
				}
			}

			if(iErrCode == PASS)
			{
				OutputDetailResult(_T("GSM Pout Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

				CheckAbortStatus(sAbortStatus);
				if (sAbortStatus == _T("True"))
				{
					sprintf(strTracer, "ABORT KEY PRESSED");
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
					iErrCode = -1;
				}

			}	
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalWCDMALoadRFData())!=PASS){
				sprintf(strTracer, "Fail to reload NV!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("Load RF Data"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("Load RF Data"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalGSMRxFreqCompensation(iBandState))!=PASS){
				sprintf(strTracer, "Fail to Rx Freq Compensation Calibration!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("GSM Rx FreqCompensation Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("GSM Rx FreqCompensation Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		if(iErrCode == PASS)
		{
			if ((iBand == GSM_BAND_DCS) || (iBand == GSM_BAND_PCS)){
				if ((iErrCode = doCalGSMRSBCalibration(iBandState))!=PASS){
					sprintf(strTracer, "Fail to Pout Calibration!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputDetailResult(_T("GSM RSB Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
				}

				if(iErrCode == PASS)
				{
					OutputDetailResult(_T("GSM RSB Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

					CheckAbortStatus(sAbortStatus);
					if (sAbortStatus == _T("True"))
					{
						sprintf(strTracer, "ABORT KEY PRESSED");
						memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
						OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
						iErrCode = -1;
					}
				}

				if(iErrCode == PASS)
				{
					if ((iErrCode = doCalGSMIM2Calibration(iBandState))!=PASS){
						sprintf(strTracer, "Fail to Pout Calibration!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
						memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
						OutputDetailResult(_T("GSM IM2 Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
					}
				}

				if(iErrCode == PASS)
				{
					OutputDetailResult(_T("GSM IM2 Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));

					CheckAbortStatus(sAbortStatus);
					if (sAbortStatus == _T("True"))
					{
						sprintf(strTracer, "ABORT KEY PRESSED");
						memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
						OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
						iErrCode = -1;
					}
				}
			}
		}
		
		if(iErrCode == PASS)
		{
			if ((iErrCode = doCalGSMPolarCalibration(iBandState))!=PASS){
				sprintf(strTracer, "Fail to Polar Calibration!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputDetailResult(_T("GSM Polar Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
			}
		}

		if(iErrCode == PASS)
		{
			OutputDetailResult(_T("GSM Polar Calibration"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));
			CheckAbortStatus(sAbortStatus);
			if (sAbortStatus == _T("True"))
			{
				sprintf(strTracer, "ABORT KEY PRESSED");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				iErrCode = -1;
			}
		}

		bool bOnline = false;
		if((nStartEnd == 255) || (iErrCode != PASS))
			bOnline = true;

		if (doCalGSMEnd(iBandState, bOnline)!=PASS){
			if(iErrCode == PASS)
			{
				sprintf(strTracer, "Fail in end GSM!! in Band %d ---- ErrorCode = %x ",iBandState, iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			}
			OutputDetailResult(_T("GSM Calibration End"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("0"),_T("--"),_T("F"));
		}
		else
			OutputDetailResult(_T("GSM Calibration End"),cBandtemp,_T("Channel: --"),_T("1"),_T("1"),_T("1"),_T("--"),_T("P"));
	}

	if((nStartEnd == 255) || (iErrCode != PASS))
	{
		DWORD dwEnd = ::GetTickCount();

		DWORD dwTime = dwEnd - gdwStart;
		dwTime = dwTime /1000;
		CString szTime;
		szTime.Format(_T("Test Times: %d Mins, %d Secs"), dwTime/60, dwTime%60);
		OutputHeader(szTime.GetBuffer(0),(std::string)(""));

	}

	if(iErrCode == PASS)
	{
		memcpy(outputString, _T("Success,,"), sizeof(TCHAR)*100);
		return true;
	}
	else
		return false;

}

// [6/27/2008] Magan.Mei Online Call Test
MIDDLEWARE_API int CallTest(TCHAR* band, TCHAR* outputString, bool bNotOutput = false)
{
	char strTracer[MAX_ARRAY_SIZE] = {0};

	//  [6/10/2008 Enzo] get comport
	unsigned short nPorts = 10;
	unsigned short Ports[10] = {0};
	int icomport = 0;
	int iErrCode = 0;
	bool bRes = false;
	int iBand = 999;
	int iGPIBAddress = 0;
	CString szBand = band;
	unsigned int uiFlag = POWER_TEST | FREQ_PHASE_TEST | ORFS_TEST | SENSITIVITY_TEST;
	int iStatus = SUCCESS;
	CString szRetrun;
	char* cInstrumentType;
	TCHAR cImei[18];
	TCHAR cSW[100];
	TCHAR cHW[100];
	bool bOutput = bNotOutput;

	//iBand = strcmp(band,"UMTSBand1");
	//if (iBand == 0){
	//	iBand = PHONE_MODE_WCDMA_IMT;
	//}

	if(!bNotOutput)
		gdwStart = ::GetTickCount();

	if(!bOutput)
	{
		int i = 0;
		while(!GetIMEI(cImei) && i < 3)
		{
			i++;
		}
		if(i == 3)
		{
			cImei[0] = _T('N');
			cImei[1] = _T('/');
			cImei[2] = _T('A');
			cImei[3] = _T('\0');
			sprintf(strTracer, "Fail to Get IMEI from ME ErrorCode");
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputData(_T("Read IMEI"), _T("Failed"),(std::string)(""));
			//			return false;
		}
	}
 
	if(QL_GetDiagPort(&nPorts,Ports) && nPorts >= 1){
		icomport = Ports[0];
	}else{

		CString strPort = GetCurrentActivePort();

		if ( strPort.GetLength() > 3)
		{
			icomport = StrToInt( strPort.Right(strPort.GetLength()-3).GetBuffer(0) );
		}
		else
		{
			sprintf(strTracer, "Fail to get comport!!");
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputData(_T("Get Comport"), _T("Failed"),(std::string)(""));
			return false;
		}
	}

	if(!bOutput)
	{
		int i = 0;
		CQisdaMiscellaneousModule cMiscellaneous(icomport);
		while(!cMiscellaneous.GetSWHWVersion(cSW, cHW) && i < 3)
		{
			i++;
		}
		if(i ==3)
		{
			cSW[0] = _T('N');
			cSW[1] = _T('/');
			cSW[2] = _T('A');
			cSW[3] = _T('\0');
			sprintf(strTracer, "Fail to Get SW Version from ME ErrorCode");
			memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			OutputData(_T("Get SW Version"), _T("Failed"),(std::string)(""));
		}
	}

	if(szBand.Find(_T("UMTSBand1")) != -1)
	{
		iBand = PHONE_MODE_WCDMA_IMT;

//	if (iBand == PHONE_MODE_WCDMA_IMT){
		iErrCode = doWCDMAOnlineInitCheckDevice(icomport,&iGPIBAddress);
		if(iErrCode != PASS)
			OutputData(_T("Check Device"), _T("Failed"), (std::string)(""));

		int nTestSet = iGPIBAddress & 0xFF;
		if(iErrCode == PASS)
		{
			if(!bOutput)
			{
				nTestSet = iGPIBAddress & 0xFF;
				if(nTestSet== 3)
					OutputData(_T("TestSet"), _T("CMU 200"), (std::string)(""));
				else if(nTestSet == 4)
					OutputData(_T("TestSet"), _T("HP8960"), (std::string)(""));
				else if(nTestSet == 5)
					OutputData(_T("TestSet"), _T("WillTek"), (std::string)(""));
				else
					OutputData(_T("TestSet"), _T("N/A"), (std::string)(""));

				int nTestSet2 = (iGPIBAddress >> 8) & 0xFF;
				if(nTestSet2 == 0)
					OutputData(_T("PowerSupply1"), _T("HpE3631"),(std::string)(""));
				else if(nTestSet2 == 1)
					OutputData(_T("PowerSupply1"), _T("Hp66311"),(std::string)(""));
				else if(nTestSet2 == 2)
					OutputData(_T("PowerSupply1"), _T("Hp66332A"),(std::string)(""));
				else
					OutputData(_T("PowerSupply1"), _T("N/A"),(std::string)(""));

				OutputData(_T("PowerSupply2"),_T("N/A"),(std::string)(""));	
				OutputData(_T("IMEI"),cImei,(std::string)(""));
	//			OutputData(_T("IMEI"),_T("123456789012345"),(std::string)(""));
	//			OutputData(_T("SW Version"),_T("N/A"), (std::string)(""));
				OutputData(_T("SW Version"),cSW, (std::string)(""));

				bOutput = true;

			}
		}

		if(nTestSet == 5) {//Willtek
			if(iErrCode == PASS)
			{
				if((iErrCode = doWCDMAOnlineStartCall(iBand, RFOutputCallBackFunc)) != PASS){
					sprintf(strTracer, "Fail to Start Call!! in UMTSBand1 ---- ErrorCode = %x ", iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("Start call -- WCDMA --UMTSBnad1"), _T("Failed"),(std::string)(""));
				}
			}

			if(iErrCode == PASS)
			{
				if((iErrCode = doWCDMAOnlineRFHchGen()) != PASS){
					sprintf(strTracer, "Fail to RFHch Generate!! in UMTSBand1 ---- ErrorCode = %x ", iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("RFHch Generate -- WCDMA --UMTSBnad1"), _T("Failed"),(std::string)(""));
				}
			}

			if(iErrCode == PASS)
			{
				if((iErrCode = doWCDMAOnlineRFHchMeasure()) != PASS){
					sprintf(strTracer, "Fail to RFHch Measure!! in UMTSBand1 ---- ErrorCode = %x ", iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("RFHch Measure -- WCDMA --UMTSBnad1"), _T("Failed"),(std::string)(""));
				}
			}

			if(iErrCode == PASS)
			{
				if((iErrCode = doWCDMAOnlineILPCMeasure()) != PASS){
					sprintf(strTracer, "Fail to ILPC Measure!! in UMTSBand1 ---- ErrorCode = %x ", iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("ILPC Measure -- WCDMA --UMTSBnad1"), _T("Failed"),(std::string)(""));
				}
			}

			if((iErrCode = doWCDMAOnlineEndTest()) != PASS){
				sprintf(strTracer, "Fail to End call!! in UMTSBand1 ---- ErrorCode = %x ", iErrCode);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				OutputData(_T("End call -- WCDMA --UMTSBnad1"), _T("Failed"),(std::string)(""));
			}

		}
		else{
			if(iErrCode == PASS)
			{
				if ((iErrCode = doMobileOnFTM(iGPIBAddress,icomport))!=PASS){
					sprintf(strTracer, "Fail to set FTM mode!! in UMTSBand1 ---- ErrorCode = %x ", iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					//			OutputDetailResult(_T("Mobile On FTM"), _T("WCDMA"),_T("UMTSBand1"),"--","--","--","--",_T("F"));
					OutputData(_T("Mobile On FTM -- WCDMA --UMTSBnad1"), _T("Failed"),(std::string)(""));
					//			return bRes;
				}
			}

			if(iErrCode == PASS)
			{
				std::string sztemp;
				ManageResource(_T("Status"), _T("TestSet"), sztemp);
				ManageResource(_T("Register"), _T("TestSet"), sztemp);

				iGPIBAddress = 0;

				if ((iErrCode = doTestWCDMAInitSetting(iBand,icomport))!=PASS){
					sprintf(strTracer, "Fail to Initial WCDMA Set!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				}
			}

			if(iErrCode == PASS)
			{
				if ((iErrCode = doInitBSEWCDMA(iGPIBAddress,iBand))!=PASS){
					sprintf(strTracer, "Fail to Initial BSE WCDMA!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				}
			}

			if(iErrCode == PASS)
			{
				Sleep(3000);
				if ((iErrCode = doFTMWCDMAStartCall(iBand))!=PASS){
					sprintf(strTracer, "Fail to WCDMA Start Call!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				}
			}

			if(iErrCode == PASS)
			{
				if ((iErrCode = doFTMWCDMARFMchMeasureAll(iBand, RFOutputCallBackFunc))!=PASS){
					sprintf(strTracer, "Fail to WCDMA Measure Middle Channel!! in Band %d ---- ErrorCode = %x ",iBand, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					if(iErrCode == -1)
					{
						sprintf(strTracer, "ABORT KEY PRESSED");
						memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
						OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
					}
				}
			}

			if (doTestWCDMAEnd()!=PASS){
				if(iErrCode == PASS)
				{	sprintf(strTracer, "Fail to WCDMAEnd!! in Band %d ---- ErrorCode = %x",iBand, iErrCode);
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
				}
			}

			if(	(szBand.Find(_T("GSM900")) != -1) || (szBand.Find(_T("GSM1800")) != -1) || (szBand.Find(_T("GSM1900")) != -1))
			{
				OutputHeader(_T("Change To On-line Mode, Waiting For Reboot"),(std::string)(""));
				Sleep(12000);
			}
		} //add 20080828
	}
	if((szBand.Find(_T("GSM900")) != -1)  || (szBand.Find(_T("GSM1800")) != -1) || (szBand.Find(_T("GSM1900")) != -1))
	{

		iGPIBAddress = 0;
		if(iStatus == SUCCESS)
		{
			iStatus = doGSMOnlineInit(icomport,&iGPIBAddress, RFOutputCallBackFunc);
			if(iStatus != SUCCESS)
			{	
				sprintf(strTracer, "Fail to GSMOnlineTestInit ErrorCode 0x%X",iStatus);
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			}
		}
		if(!bOutput && (iStatus == SUCCESS))
		{
			int nTestSet = iGPIBAddress & 0xFF;
			if(nTestSet== 3)
				OutputData(_T("TestSet"), _T("CMU 200"), (std::string)(""));
			else if(nTestSet == 4)
				OutputData(_T("TestSet"), _T("HP8960"), (std::string)(""));
			else if(nTestSet == 5)
				OutputData(_T("TestSet"), _T("WillTek"), (std::string)(""));
			else
				OutputData(_T("TestSet"), _T("N/A"), (std::string)(""));

			nTestSet = (iGPIBAddress >> 8) & 0xFF;
			if(nTestSet == 0)
				OutputData(_T("PowerSupply1"), _T("HpE3631"),(std::string)(""));
			else if(nTestSet == 1)
				OutputData(_T("PowerSupply1"), _T("Hp66311"),(std::string)(""));
			else if(nTestSet == 2)
				OutputData(_T("PowerSupply1"), _T("Hp66332A"),(std::string)(""));
			else
				OutputData(_T("PowerSupply1"), _T("N/A"),(std::string)(""));

			OutputData(_T("PowerSupply2"),_T("N/A"),(std::string)(""));	
			OutputData(_T("IMEI"),cImei,(std::string)(""));
//			OutputData(_T("SW Version"),_T("N/A"), (std::string)(""));
			OutputData(_T("SW Version"),cSW, (std::string)(""));

			bOutput = true;
		}

		if(iStatus == SUCCESS)
		{
			OutputHeader(_T("GSM Online Test Start"), (std::string)(""));
			std::string sztemp;
			ManageResource(_T("Status"), _T("TestSet"), sztemp);
			ManageResource(_T("Register"), _T("TestSet"), sztemp);
		}

		if(iStatus == SUCCESS)
		{
			iStatus = doGSMOnlineStartCall_GSM(FTM_GSM_BER_EGSM_900);
			if(iStatus != SUCCESS)
			{
				sprintf(strTracer, "Fail to GSMStartCall_GSM at band EGSM 900");
				memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
			}
		}
		
		if(iStatus == SUCCESS)
		{
			szRetrun = _T("");
			if(	szBand.Find(_T("GSM900")) != -1)
			{
				iStatus = doGSMOnlineTest_GSM(FTM_GSM_BER_EGSM_900,uiFlag);
				if(iStatus != SUCCESS)
				{
					sprintf(strTracer, "Fail to GSMTest_GSM at band GSM900 ErrorCode 0x%X", iStatus);
					szRetrun = strTracer;
				}
				if(iStatus == -1)
				{
					sprintf(strTracer, "ABORT KEY PRESSED");
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				}
			}
			if((iStatus != -1) && (szBand.Find(_T("GSM1800")) != -1 ))
			{
				iStatus = doGSMOnlineTest_GSM(FTM_GSM_BER_DCS_1800,uiFlag);
				if(iStatus != SUCCESS)
				{
					sprintf(strTracer, "Fail to GSMTest_GSM at band DCS1800 ErrorCode 0x%X", iStatus);
					szRetrun += strTracer;
				}
				if(iStatus == -1)
				{
					sprintf(strTracer, "ABORT KEY PRESSED");
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				}
			}

			if((iStatus != -1) && (szBand.Find(_T("GSM1900")) != -1 ))
			{
				iStatus = doGSMOnlineTest_GSM(FTM_GSM_BER_PCS_1900,uiFlag);
				if(iStatus != SUCCESS)
				{
					sprintf(strTracer, "Fail to GSMTest_GSM at band PCS1900 ErrorCode 0x%X", iStatus);
					szRetrun += strTracer;
				}
				if(iStatus == -1)
				{
					sprintf(strTracer, "ABORT KEY PRESSED");
					memcpy(outputString, _T(strTracer), sizeof(TCHAR)*100);
					OutputData(_T("ABORT KEY PRESSED"),_T(" FAIL"),(std::string)(""));
				}
			}
			memcpy(outputString, szRetrun.GetBuffer(0), sizeof(TCHAR)*100);

		}

		doGSMOnlineEndTest();
	}


	DWORD dwEnd = ::GetTickCount();

	DWORD dwTime = dwEnd - gdwStart;
	dwTime = dwTime /1000;
	CString szTime;
	szTime.Format(_T("Test Times: %d Mins, %d Secs"), dwTime/60, dwTime%60);
	OutputHeader(szTime.GetBuffer(0),(std::string)(""));

	if(iStatus == SUCCESS)
	{
		szRetrun = _T("Success,,");
		return true;	
	}
	else
	{
		return false;
	}

}


// End CAL and Test function -------------------------------- 
