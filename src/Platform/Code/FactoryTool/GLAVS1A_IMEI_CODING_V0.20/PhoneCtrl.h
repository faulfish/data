// PhoneCtrl.h: interface for the CPhoneCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHONECTRL_H__73B3D444_61D3_41DB_ADE7_6DCB08283523__INCLUDED_)
#define AFX_PHONECTRL_H__73B3D444_61D3_41DB_ADE7_6DCB08283523__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "QLib.h"
#include "QLib_Defines.h"
#include "QLib_SoftwareDownloadDefines.h"
#include "QLib_Ext.h"
#include "Qlib_Ext_Defines.h"

class CPhoneCtrl  
{
public:
	unsigned char VibratorOnOff(HANDLE PhoneHandle,unsigned char OnOffFlag);
	void DisConnect_Server(HANDLE PhoneHandle);
	unsigned char WriteSIMDataToPhone(HANDLE Phone_Handle,
									  unsigned char Slot_ID,
									  unsigned char SIM_AppIdLength,
									  unsigned char *pSIM_AppId,
									  unsigned long SIM_SecureDataLength,
									  unsigned char *pSIM_Data,
									  unsigned long *pCommandStatus,
									  unsigned long *pStatus);
	unsigned char SaveIMEIFSNToFlash(HANDLE Phone_Handle,unsigned char SlotID,unsigned char SIM_AppIDLen,unsigned char *SIM_AppID,unsigned long *pCommandStatus,unsigned long *pStatus);
	unsigned char RootKeyWrite(HANDLE  Phone_Handle,  unsigned char  SlotId,  unsigned char  SIM_AppIdLen,  unsigned char * pSIM_AppId,  unsigned long  RootKeyLen,  unsigned char *  pRootKeyData,  unsigned long *pCommandStatus,  unsigned long *pStatus);
	unsigned char FactoryAreaReadWrite(HANDLE Phone_Handle,unsigned char Read_Write_Flag,unsigned short Address,unsigned char *Factory_Data);
	unsigned char WriteSPCToPhone(HANDLE Phone_Handle, unsigned char SPC_Code[6],unsigned char *SPC_Result);
    void GetLastSoftwareDownloadErrorInfo(HANDLE  PhoneHandle,  
										  unsigned char* ErrorOccurred,  
										  char* ErrorMessage,  
										  int MaxStringLength);
	unsigned char UploadMultiImage_OBL(HANDLE PhoneHandle,  
									   char* PartitionFileName,  
									   char* OTBPL_FileName,  
									   char* PBL_FileName,  
									   char* QCSBL_FileName,  
									   char* QCSBL_HeaderName,  
									   char* OEMSBL_FileName,  
									   char* OEMSBL_HeaderName,  
									   char* ModemFileName,  
									   char* ModemHeaderName,  
									   char* AppsFileName,  
									   char* AppsHeaderName,  
									   char* AppsBlFileName,  
									   char* AppsBlHeaderFileName,  
									   unsigned char OverridePrtnTable,  
									   unsigned char UseTrustedMode,  
									   char*  Service_Programming_Code,  
									   unsigned char  ClearErrorLog,  
									   char*  StorageFile,  
									   unsigned char OverrideModelCheck,  
									   unsigned char SkipReset,  
									   unsigned char UseCustomArmprg,  
									   long  DloadArmprg,  
									   long  DownloadBitMask);
	unsigned char UploadMultiImage(HANDLE  PhoneHandle,  
								   char* PartitionFileName,  
								   char* PBL_FileName,  
								   char* QCSBL_FileName,  
								   char* QCSBL_HeaderName,  
								   char* OEMSBL_FileName,  
								   char* OEMSBL_HeaderName,  
							       char* ModemFileName,  
								   char* ModemHeaderName,  
								   char* AppsFileName,  
								   char* AppsHeaderName,  
								   char* AppsBlFileName,  
								   char* AppsBlHeaderFileName,  
								   unsigned char OverridePrtnTable,  
								   unsigned char UseTrustedMode,  
								   char* Service_Programming_Code,  
								   unsigned char ClearErrorLog,  
								   char* StorageFile,  
								   unsigned char OverrideModelCheck,  
								   unsigned char SkipReset,  
							       unsigned char UseCustomArmprg,  
								   long  DloadArmprg,  
								   long  DownloadBitMask); 
	unsigned char UploadCEFSFile2(HANDLE PhoneHandle,char *CEFSFileName,char *PartitionFileName,long ARMPRG_Type);
	unsigned char UploadCEFSFile(HANDLE PhoneHandle,char *CEFSFileName,char *PartitionFileName);
	unsigned char DownloadQCNFile(HANDLE PhoneHandle,char *QCNFileName,char *SPC);
	unsigned char UploadQCNFile(HANDLE PhoneHandle,
								char* QCNFileName, 
								char* Service_Programming_Code,  
								unsigned char AutoRestore,  
								unsigned char SkipReset,  
								unsigned char  AllowEsnMismatch,  
								unsigned char  IgnorePhoneModel);
	unsigned char ReadESN(HANDLE PhoneHandle,unsigned long *ESN);
	unsigned char BackupNVToCNV(HANDLE PhoneHandle);
	unsigned char SecurityFreeze(HANDLE PhoneHandle);
	unsigned char QueryPhoneStatus(HANDLE PhoneHandle,unsigned char *State);
	unsigned char SendSPC(HANDLE PhoneHandle,unsigned char SPC[6],unsigned char *SPCResult);
	unsigned char NVItemRead(HANDLE PhoneHandle,unsigned short ItemID,unsigned char *ItemData,int ItemLength,unsigned short *Status);
	unsigned char NVItemWrite(HANDLE PhoneHandle,unsigned short ItemID,unsigned char *ItemData,int ItemLength,unsigned short *Status);
	unsigned char ModeSwitch(HANDLE PhoneHandle,int Mode);
	unsigned char GetQpstPhoneStatus(HANDLE PhoneHandle,unsigned long *PhoneStatus);
	unsigned char GetVersionInfoEXT(HANDLE PhoneHandle,unsigned long *MSMHWVersion,unsigned long *MobileModel,char *MobileSWRevision,char *ModelString);
	unsigned char GetVersionInfo(HANDLE PhoneHandle,
		                         char CompileDate[11],
								 char CompileTime[8],
								 char ReleaseData[11],
								 char ReleaseTime[8],
								 char VersionDirectory[8],
								 unsigned char *StationClassMark,
								 unsigned char *MobileCAIRevision,
								 unsigned char *MobileModel,
								 unsigned short *MobileFirmwareRevision,
								 unsigned char *SlotCycleIndex,
								 unsigned char *VocoderMajorVersion,
								 unsigned char *VocoderMinorVersion);
	unsigned char ChangeFTMModeRuntime(HANDLE PhoneHandle,unsigned char ModeFlag);
	unsigned char ChangeFTMBootMode(HANDLE PhoneHandle,unsigned char Mode_Flag,unsigned char Reset_Flag);
	unsigned char SetCommandCode(HANDLE PhoneHandle,short CommandCode);
	unsigned char IsFTMMode(HANDLE PhoneHandle,unsigned char * FTMMode);
	unsigned char IsPhoneConnected (HANDLE PhoneHandle);
	unsigned long GetTimeOut(HANDLE PhoneHandle,unsigned long TimeOutID);
	unsigned char ConfigureTimeOut(HANDLE PhoneHandle,unsigned long TimeOutId,unsigned long NewTimeOutValue_MS);
	void ConfigureEFSCallBacks(HANDLE PhoneHandle,EfsFileTransferCB EFSCallBack,EfsDirCB EFSDirCallBackFunc);
	void ConfigureCallBacks(HANDLE PhoneHandle, EfsDirCB EfsDirCallBackFunc, generalSWDownloadCB GeneralSwDownloadCallBackFunc,asyncMessageCB AsyncMessageCallBackFunc);
	unsigned char GetAvailablePhonesPortList(unsigned short *PortNumber,unsigned short *PortList,unsigned short IgnorePortNumber,unsigned short *IgnorePortList);
	void DisconnectAllServers(void);
	unsigned char GetComPortNumber(HANDLE PhoneHandle,unsigned short *ComPort);
	HANDLE ConnectServerWithWait(unsigned int ComPort,unsigned long Wait_MS);
	void GetLibraryCapabilities(unsigned char * SupportsDiag, unsigned char * SupportsEFS,unsigned char *SupportsSWDownload,unsigned char *UsingQPST );
	void GetLibraryVersion(char *Version_Buffer);
	void SetLibraryMode(unsigned char Mode);
	CPhoneCtrl();
	virtual ~CPhoneCtrl();

};

#endif // !defined(AFX_PHONECTRL_H__73B3D444_61D3_41DB_ADE7_6DCB08283523__INCLUDED_)
