// ImplementedDiagCommandTesterDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "ExportFunction.h"
#include "../../QLib_Export/QLib_export.h"
#include "../../Modules\QisdaMiscellaneousModule\Download.h"
#include "../MobileToolKit/MobileToolKit.h"
#include "../MobileToolKit/MobileToolKit_ForSPU.h"
#include "../../Components\SoundUtility\SoundUtility.h"
#include "../QCH_DatabastAccess/IQCH_Database.h"
#include "..\SPU\ISPU_CALLBACK.h"
#include "..\..\Components\GPIBControl\GPIBControl.h"
#include "../../Modules\QisdaEFsModule\EFsExport.h"

//#include "../IMEIStation\IMEIStation.h"
typedef bool (*pFnBackupBrtV2)(long nComport, const LPCSTR szCsv, const LPCSTR szBrt,LPSTR szErrorListBuffer,int nBufferSize);
typedef bool (*pFnRestoreBrt)(long nComPort, const LPCSTR szBrt,bool bFilterRealonly);
typedef bool (*pFnReadSWVersion)(long nComPort,char* szReadValue1024, long& nLength, int nTimeout);
typedef bool (*pFnGetReleaseDateTime)(long nComPort, char* szDate12, char* szTime9, int nTimeout);

// CImplementedDiagCommandTesterDlg dialog
class CImplementedDiagCommandTesterDlg : public CDialog ,  public ISPU_CALLBACK
{
	class CalFunctionTime
	{
	public:
		CalFunctionTime(CImplementedDiagCommandTesterDlg* pThis,const CString& strFuncName = _T(""))
			:m_strFuncName(strFuncName)
			,m_tcStart(GetTickCount())
			,m_pThis(pThis)
		{
			CString strTemp;
			strTemp.Format(_T("FUNCTION(%s) CALCLATE START\n"),m_strFuncName);
			if(m_pThis)
				m_pThis->AddLog(strTemp);
		}
		~CalFunctionTime()
		{		
			DWORD tcEnd = GetTickCount();
			
			CString strTemp;
			strTemp.Format(_T("FUNCTION(%s) CALCLATE END----%d clocks\n"),m_strFuncName,tcEnd - m_tcStart);
			if(m_pThis)
				m_pThis->AddLog(strTemp);
		}
	private:
		DWORD m_tcStart;
		CString m_strFuncName;
		CImplementedDiagCommandTesterDlg* m_pThis;
	};
    // Construction
public:
    CImplementedDiagCommandTesterDlg(CWnd* pParent = NULL);	// standard constructor
    virtual ~CImplementedDiagCommandTesterDlg();
	
	virtual bool CheckBreak();
	virtual bool WaitInput(const TSTRING& strName,TCHAR* szBuffer,int nBufferSize);
	virtual bool Print(const TSTRING& strName,const TSTRING& strOutput);

    // Dialog Data
    enum { IDD = IDD_IMPLEMENTEDDIAGCOMMANDTESTER_DIALOG };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

    // Implementation
protected:
    HICON m_hIcon;

    // Generated message map functions
    virtual BOOL OnInitDialog();
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnBnClickedOk();
    DECLARE_MESSAGE_MAP()

private:
    void LocdFunction();
	void UpdatePort(int nPort);
	void Function(int nPort);
	void AddLog(const CString& strLog);
	void LogBinary(const char* szTemp,DWORD nBufferSize);
	static DWORD WINAPI ThreadProc( LPVOID lpParam ) ;

private:
	//EFS functions
	pFnEFS_ReadFile EFS_ReadFile;
	pFnEFS_ListFolder EFS_ListFolder;
	pFnEFS_GetLocFile EFS_GetLocFile;
	pFnEFS_GetLocFileName EFS_GetLocFileName;

	//SPC functions
	pFnLockSPC LockSPC;
	pFnIsSPCLocked IsSPCLocked;
	pFnUnlockSPC UnlockSPC;
	pFnProgramSPC ProgramSPC;

	pFnEnableLog      EnableLog;

    pFnGetIMEI			ReadIMEI;
	pFnWriteIMEI			WriteIMEI;	
    pFnWriteHeadsetBTAddress    WriteHeadsetBTAddress;
    pFnSetTestMode              SetTestMode;
    pFnSetOnlineMode            SetOnlineMode;
    pFnIsDLMode                 IsDLMode;
    pFnUnlockPhoneCode          UnlockPhoneCode;
    pFnUnlockNW                 UnlockNW;
    pFnUnlockNS                 UnlockNS;
    pFnUnlockSP                 UnlockSP;
    pFnUnlockCP                 UnlockCP;
    pFnUnlockSIM                UnlockSIM;
    pFnUnlockPROP               UnlockPROP;
    pFnReadFactoryArea          ReadFactoryArea;
    pFnWriteFactoryArea         WriteFactoryArea;

	//Factory Area
	pFnReadFactoryHWSWMW ReadFactoryHWSWMW;
	pFnWriteFactoryHWSWMW WriteFactoryHWSWMW;
	pFnReadProcessInfo ReadProcessInfo;
	pFnWriteProcessInfo WriteProcessInfo;
	pFnReadTrackID ReadTrackID;
	pFnWriteTrackID WriteTrackID;
	pFnReadPicassoNumber	ReadPicassoNumber;
	pFnWritePicassoNumber	WritePicassoNumber;
	pFnReadMobileID_ForSPU	ReadMobileID_ForSPU;
	pFnWriteMobileID_ForSPU	WriteMobileID_ForSPU;
    pFnReadFactoryAreaNew       ReadFactoryAreaNew;
	pFnReadHWVersion_FromFactoryArea     ReadHWVersion_FromFactoryArea;
	pFnWriteHWVersion_ToFactoryArea  WriteHWVersion_ToFactoryArea;

    pFnBackupNVPartition                 BackupNVPartition;
	pFnRestoreNVPartition                 RestoreNVPartition;
	pFnCheckNVPartition                 CheckNVPartition;
    pFnBackupNV2BRT                BackupNV2BRT;
    pFnRestoreBRT2NV               RestoreBRT2NV;
    pFnBackupBrtV2    BackupNV2BRT_IMESTATION;

    pFnClearActivationData      ClearActivationData;
    pFnStandbyCurrentTest       StandbyCurrentTest;
    pFnMasterClear              MasterClear;
    pFnWriteQFuse               WriteQFuse;
	pFnReadQFuseState      ReadQFuse;
    pFnBTHeadsetChargingPower   BTHeadsetChargingPower;
    pFnCheckSIMCardExistence    CheckSIMCardExistence;
    pFnBTHandsetVoltageReading  BTHandsetVoltageReading;
	pFnIsUnitLocked             IsUnitLocked;
    pFnSetOnline                SetOnline;
    pFnSetOffline               SetOffline;
    pFnResetHandset             ResetHandset;
    
	pFnChangeFTM			ChangeFTM;
	pFnChangeMode			ChangeMode;
	pFnCheckFTM				CheckFTM;

    pFnBTHeadsetVoltageReading  BTHeadsetVoltageReading;
    pFnCheckBTHeadsetState           CheckBTHeadset;
    pFnChangeOnline             ChangeOnline;
	pFnNewResetHandset     NewResetHandset;
	pFnEnableQualcommDebugEvent EnableQualcommDebugEvent;
	pFnWriteSIMLockFileNameToNV WriteSIMLockFileNameToNV;
	pFnReadSIMLockFileNameFromNV ReadSIMLockFileNameFromNV;
	pFnWriteSimLockFile WriteSimLockFile;

	pFnGetSWHWVersion GetSWHWVersion;
	pFnGetLavernockHWVersion GetLavernockHWVersion;
	pFnGetLavernockHWVersionV2_ForSPU GetLavernockHWVersionV2_ForSPU;
	pFnGetMicroSDInfo_ForSPU GetMicroSDInfo_ForSPU;
	pFnReadNVItem ReadNVItem;
	pFnReadNVItem_ForSPU ReadNVItem_ForSPU;
	pFnWriteNVItem WriteNVItem;

	pFnWriteFailureByte WriteFailureByte;

	pFnTurnOnOffEM TurnOnOffEM;

	pFnWriteDefaultPhoneCode WriteDefaultPhoneCode;

	pFnReadBatteryVoltage ReadBatteryVoltage;
	pFnReadBatteryTemperature ReadBatteryTemperature;

	pFnFreezeOTP FreezeOTP;
	pFnIsOTPFrozen IsOTPFrozen;

	pFnRestoreBrt RestoreBrt_DOWNLOADEXPORT;
	pFnReplaceEFSFile Replace_GrpDList_File;

	pFnReadSWVersion ReadSWVersion;
	pFnGetReleaseDateTime GetReleaseDateTime;

	pFnIsGSensorWorking IsGSensorWorking;
	pFnIsGSensorWorkingV2 IsGSensorWorkingV2;
	pFnIsGSensorWorkingV3 IsGSensorWorkingV3;
	pFnCheckSIMLock_VC6 CheckSIMLock;
	pFnCheckCEFSFileName CheckCEFSFileName;

	pFnGetBTAddr GetBTAddr;

	pFnDLRAWRequest DLRAWRequest;
	pFnDiagRAWRequest DiagRAWRequest;

	pFnByPassCellTemperatureCheck ByPassCellTemperatureCheck;

	pFnAnalysisRecord AnalysisRecord;
	pFnAnalysisRecordStereo AnalysisRecordStereo;

	pfnGPIB_SetControlIndex GPIBSetControlIndex;
	pfnGPIB_GetDeviceCount GPIBGetDeviceCount;
	pfnGPIB_QueryDeviceDescription GPIBQueryDeviceDescription;
	pfnGPIB_SetCurrentVoltage GPIBSetCurrentVoltage;
	pfnGPIB_IsPowerSupply GPIBIsPowerSupply;
	pfnGPIB_Initialization GPIBInitialization;
	pfnGPIB_SendCommmand GPIBSendCommmand;
	pfnGPIB_SendQueryCommmand GPIBSendQueryCommmand;
	pfnGPIB_ReadCommandResult GPIBReadCommandResult;
	pfnGPIB_ErrorMsg GPIBErrorMsg;

	pFnUSBChargingSwitch USBChargingSwitch;
	pFnCoinCellChargingSwitch CoinCellChargingSwitch;
	pFnKeypadBacklight KeypadBacklight;
	pFnKeypadPressTest KeypadPressTest;

	//interface from QCH_DatabastAccess.dll
	pFnQuerySPC QuerySPC;

	CDownload *m_pDownload;
private:	
    CListBox m_cmdList;
    CEdit m_cmdInput;
    CEdit m_cmdOutput;
    int m_nPortNum;
	CEdit m_COMPortCtrl;
	afx_msg void OnBnClickedUpdateport();
	afx_msg void OnEnChangeEdit2();
	CEdit m_RepeatCtrl;
	afx_msg void OnLbnSelchangeLoglist();
	CListBox m_LogLISTCtrl;
	bool m_bRunSelfTest;
public:
	afx_msg void OnBnClickedLoopbutton();
	CEdit m_editLoop1;
	CEdit m_editLoop2;
	CEdit m_editLoop3;
	CEdit m_editLoop4;
	CEdit m_editLoop5;
	CEdit m_editLoop6;
	afx_msg void OnBnClickedClearButton();
};
