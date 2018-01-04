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
#include "../../../Modules/QisdaQualcommDownloadModule/Download.h"
#include "../../../Modules/QisdaSecureActivateModule/SecureActivate.h"
#include "../../../Modules/QisdaSimLockModule/SimLockStore.h"

#ifdef ODM_EXPORT_EXPORTS
#define ODM_API extern "C" __declspec(dllexport)
#define ODM_CLASS_API __declspec(dllexport)
#else
#define ODM_API extern "C" __declspec(dllimport)
#define ODM_CLASS_API __declspec(dllimport)
#endif

class CFileGetter_Melos : public CFileGetter
{
public:
	virtual bool SetMot(const TSTRING& strMot);
	virtual bool SetFileName(bool bFindAll = true);
	virtual const bool IsBufferGotten() const;
	const bool GetBuffer();

	CBufferPtr m_ptPartition;
	
	CBufferPtr m_ptHex;

	CBufferPtr m_ptDBL;
	CBufferPtr m_ptFSBL;
	CBufferPtr m_ptOSBL;
	CBufferPtr m_ptAmss;
	CBufferPtr m_ptCEFS;
};

class CODM_Export : public IODMExport
{
	class CODMConf
	{
	public:
		CODMConf():
		  m_strNWSCP(""),
			  m_strSSCP(""), 
			  m_str1ffLocPath(""),
			  m_strUSBIndex(""){}
		  ~CODMConf(){}
	public:
		// Module SmartPtr
		CQisdaMiscellaneousModulePtr m_ptQisdaMiscModule;
		CCFCEFSModulePtr			 m_ptEFSModule;
		CShareDownload6240Ptr		 m_ptDownloadModule;
		CCFCDiagCommandPtr			 m_ptNVModule;
		CCFCOTPRelatedPtr			 m_ptQisdaSecureACtivateModule;
		CCFCSimLockRelatedPtr		 m_ptQisdaSimLockModule;

		// Setting
		std::string m_strNWSCP; // CFC provides that NW, NS, SP, CP, SIM codes must be the same, this is that code.
		std::string m_strSSCP;
		std::string m_str1ffLocPath;
		std::string m_strUSBIndex;  // for log
	};
public:
	CODM_Export();
	virtual ~CODM_Export();
	virtual void InitializeParameter(long nComport, bool bAlwaysFlash = true/*This is created for Wellfleet, maybe it will be removed in Lavernock.*/);
	virtual bool ConnectComPort(long nComport);
	virtual bool DisconnectComPort(long nComport);
	virtual bool TempDisconnectComPort(long nComport);

	virtual bool SetTestMode(long nComport);
	virtual bool Restart(long nComport);
	virtual bool Reset(long nComport);
	virtual bool ReadIMEI(long nComport, char* szIMEI100);
	virtual bool ProgramIMEI(long nComport, char* szIMEI);
	virtual bool WriteSimData(long nComport);
	virtual bool ReadBTAddr(long nComport,char* szAddress100, long& nLength);
	virtual bool WriteBTAddr(long nComport,char* szAddress);
	virtual bool IsUnitOn(long nComport);
	virtual bool IsUnitOff(long nComport);
	virtual bool TurnUnitOff(long nComport);
	virtual bool ReadFlexVersion(long nComport, char* szReadValue1024, long& nLength);
	virtual bool ReadSWVersion(long nComport, char* szReadValue1024, long& nLength);
	virtual bool ReadTransceiverKitNumber(long nComport,  char* szTransceiverKitNumber100);
	virtual bool WriteTransceiverKitNumber(long nComport,  char* szTransceiverKitNumber100);
	virtual bool ReadTrackID(long nComport,  char * szTrackID);
	virtual bool WriteTrackID(long nComport,  char * szTrackID);
	virtual bool ReadFactoryInforBlock(long nComport, char* szBlock, long& nLength);
	virtual bool WriteFactoryInforBlock(long nComport, char* szBlock);
	virtual bool SetInFactoryFlag(long nComport, bool bOpen);
	virtual bool MasterClear(long nComport);
	virtual bool IsNWSCPLocked(long nComport,bool& bNWUnlocked, bool& bNSUnlocked, bool& bSPUnlocked, bool& bCPUnlocked, bool& bSIMUnlocked);
	virtual bool IsSPCLocked(long nComport, long& nStatus);
	virtual bool IsUnitLocked(long nComport, long& nStatus, int nLockType/*0=NWSCP, 1=SSCP*/);
	virtual bool UnlockSPC(long nComport, char* szLock, bool& bUnlockOK);
	virtual bool ProgramSPC(long nComport, char* szLock);
	virtual bool UnLockNWSCP(long nComport, int nLockType/*0=NW, 1=NS, 2=SP, 3=CP, 4=SIM*/, char* szLock, bool& bUnlockOK);
	virtual bool ProgramNWSCP(long nComport, char* szLock);
	virtual bool UnlockSSCP(long nComport, char* szLock, bool& bUnlockOK);
	virtual bool ProgramSSCP(long nComport, char* szLock);
	virtual bool SetRTC(long nComport);
	virtual bool LockSPC(long nComport);


public:
	virtual bool Flash(long nComport, char* szMotPath);
	virtual bool Flex(long nComport, char* szMotPath);
	virtual bool RestoreNV(long nComport, int nTimeout);
public:
	virtual int  GetLastError(long nComport);
	virtual bool GetLastError(long nComPort,const TCHAR* szModule, char* szErrMsg500);
	//virtual bool GetLastError(long nComPort,const TCHAR* szFunctionName,const TCHAR* szModule, char* szErrMsg500);

private:
	bool SetLastError(long nComport, int nError);
	void AddLog(long nComport, const CString& strMessage);
	bool SetLocName(long nComport, CFileGetter& FileManager);
	bool CompareVersion(long nComPort, CFileGetter& FileManager, bool& bDownloadOEMSBL);

private:
	bool GenSimData(long nComport, char* szIMEI100, BYTE* szFSN16, BYTE *pSimDataBuf, int pSimDataBufLen);
private:
	CCriticalSection m_CritMap;
	CCriticalSection m_CritUnZip;
	std::map<long, CODMConf> m_PortMap;
	CFileGetter_Melos m_FileManager;
};

ODM_API bool D_NewODMExportInterface(IODMExport**);
ODM_API bool D_DeleteODMExportInterface(IODMExport*);