// ODM_Export.h : main header file for the ODM_Export DLL
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif
#include "../odmtpa_qisda_Melos/InterfaceExport.h"
#include <map>
#include <string>
#include "afxmt.h"
#include "../../../odmtma_api/1FFMaker/C1FFMaker.h"
#include "../../../Modules/QisdaEFsModule/EFs.h"
#include "../../../Modules/QisdaMiscellaneousModule/QisdaMiscellaneousModule.h"
#include "../../../Modules/QisdaNVModule/NVItem.h"
#include "../../../Modules/QisdaSecureActivateModule/SecureActivate.h"
#include "../../../Modules/QisdaSimLockModule/SimLockStore.h"
#include "../../../Modules/MotoDownloadModule/MotoDownloadModule.h"
#include "../../../Components/MobileFinder/MobileFinderInterface.h"
#include "../../../CommonUtil/ErrorCode.h"

#ifdef ODM_EXPORT_EXPORTS
#define ODM_API extern "C" __declspec(dllexport)
#define ODM_CLASS_API __declspec(dllexport)
#else
#define ODM_API extern "C" __declspec(dllimport)
#define ODM_CLASS_API __declspec(dllimport)
#endif


class CODM_Export : public IODMExport
{
	class CODMConf
	{
	public:
		CODMConf():
		  m_strNWSCP(""),
		  m_strSSCP(""), 
		  m_str1ffLocPath(""),
		  m_ptQisdaMiscModule(NULL),
		  m_ptEFSModule(NULL),
		  m_ptNVModule(NULL),
		  m_ptQisdaSecureActivateModule(NULL),
		  m_ptQisdaSimLockModule(NULL){}
		~CODMConf(){}
	public:
		// Module SmartPtr
		CQisdaMiscellaneousModulePtr m_ptQisdaMiscModule;
		CCFCEFSModulePtr			 m_ptEFSModule;
		CCFCDiagCommandPtr			 m_ptNVModule;
		CCFCOTPRelatedPtr			 m_ptQisdaSecureActivateModule;
		CCFCSimLockRelatedPtr		 m_ptQisdaSimLockModule;
		

		// Setting
		std::string m_strNWSCP; // CFC provides that NW, NS, SP, CP, SIM codes must be the same, this is that code.
		std::string m_strSSCP;
		std::string m_str1ffLocPath;
		int		m_nComPort;  //0 means
		bool	m_bMotoConnedcted;
		bool	m_bDLMode;
	};
public:
	CODM_Export();
	virtual ~CODM_Export();
	virtual bool IsRSDInitiated();
	virtual void InitializeParameter(long nPort, bool bAlwaysFlash = true/*This is created for Wellfleet, maybe it will be removed in Lavernock.*/);
	virtual bool ConnectComPort(long nPort);
	virtual bool DisconnectComPort(long nPort);
	virtual bool TempDisconnectComPort(long nPort);
	bool CreateQisdaModule(long nPort);
	bool TryMotoConnection(long nPort);
	bool IsDiagMode(long nPort);

	virtual bool SetTestMode(long nPort);
	virtual bool Restart(long nPort);
	virtual bool Reset(long nPort);
	virtual bool ReadIMEI(long nPort, char* szIMEI100);
	virtual bool ProgramIMEI(long nPort, char* szIMEI);
	virtual bool WriteSimData(long nPort);
	virtual bool ReadBTAddr(long nPort,char* szAddress100, long& nLength);
	virtual bool WriteBTAddr(long nPort,char* szAddress);
	virtual bool IsUnitOn(long nPort);
	virtual bool IsUnitOff(long nPort);
	virtual bool TurnUnitOff(long nPort);
	virtual bool ReadFlexVersion(long nPort, char* szReadValue1024, long& nLength);
	virtual bool ReadSWVersion(long nPort, char* szReadValue1024, long& nLength);
	virtual bool ReadTransceiverKitNumber(long nPort,  char* szTransceiverKitNumber100);
	virtual bool WriteTransceiverKitNumber(long nPort,  char* szTransceiverKitNumber100);
	virtual bool ReadTrackID(long nPort,  char * szTrackID);
	virtual bool WriteTrackID(long nPort,  char * szTrackID);
	virtual bool ReadFactoryInforBlock(long nPort, char* szBlock, long& nLength);
	virtual bool WriteFactoryInforBlock(long nPort, char* szBlock);
	virtual bool SetInFactoryFlag(long nPort, bool bOpen);
	virtual bool MasterClear(long nPort);
	virtual bool IsNWSCPLocked(long nPort,bool& bNWUnlocked, bool& bNSUnlocked, bool& bSPUnlocked, bool& bCPUnlocked, bool& bSIMUnlocked);
	virtual bool IsSPCLocked(long nPort, long& nStatus);
	virtual bool IsUnitLocked(long nPort, long& nStatus, int nLockType/*0=NWSCP, 1=SSCP*/);
	virtual bool UnlockSPC(long nPort, char* szLock, bool& bUnlockOK);
	virtual bool ProgramSPC(long nPort, char* szLock);
	virtual bool UnLockNWSCP(long nPort, int nLockType/*0=NW, 1=NS, 2=SP, 3=CP, 4=SIM*/, char* szLock, bool& bUnlockOK);
	virtual bool ProgramNWSCP(long nPort, char* szLock);
	virtual bool UnlockSSCP(long nPort, char* szLock, bool& bUnlockOK);
	virtual bool ProgramSSCP(long nPort, char* szLock);
	virtual bool SetRTC(long nPort);
	virtual bool LockSPC(long nPort);


public:
	virtual bool Flash(long nPort, char* szMotPath);
	virtual bool Flex(long nPort, char* szMotPath);
	virtual bool RestoreNV(long nPort, int nTimeout);
public:
	virtual int  GetLastError(long nPort);
	virtual bool GetLastError(long nPort,const TCHAR* szModule, char* szErrMsg500);
	//virtual bool GetLastError(long nComPort,const TCHAR* szFunctionName,const TCHAR* szModule, char* szErrMsg500);
private:
	bool SetLastError(long nPort, int nError);
	void AddLog(long nPort, const char* szMsg);
	bool SetLocName(long nPort, char* szMotPath);
	bool CompareVersion(long nPort, CFileGetter& FileManager, bool& bDownloadOEMSBL);

private:
	bool GenSimData(long nPort, char* szIMEI100, BYTE* szFSN16, BYTE *pSimDataBuf, int pSimDataBufLen);
private:
	CMotoDownload m_MotoDLModule;
	CCriticalSection m_CritMap;
	CCriticalSection m_CritUnZip;
	std::map<long, CODMConf> m_PortMap;
};

ODM_API bool D_NewODMExportInterface(IODMExport**);
ODM_API bool D_DeleteODMExportInterface(IODMExport*);