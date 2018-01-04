// Diag_Export.h : main header file for the Diag_Export DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
#include "QualcommDiag.h"
#include "Header_MotoXPRS.h"
#include "../odmtma_api/odmtma_api/InterfaceExport.h"
#include "../odmtma_api/DownloadDLL/QDOperator.h"
#include "../CommonUtil/LogPolicy.h"
#include "../CommonUtil/ErrorCode.h"
#include "../CommonUtil/UnicodeUtility.h"
//#include "../CommonUtil/NotifyBase.h"
//#include "../Components/MobileFinder/MobileFinderInterface.h"
//#include "..\ShareLib\loki-0.1.5\include\loki\SmartPtr.h"
#include "afxmt.h"
#include <string>
#include <map>
#include "../odmtma_api/QFlash/QZip.h"
#include "../odmtma_api/1FFMaker/C1FFMaker.h"



#ifdef DIAG_EXPORT_EXPORTS
#define DIAG_API extern "C" __declspec(dllexport)
#define DIAG_CLASS_API __declspec(dllexport)
#else
#define DIAG_API extern "C" __declspec(dllimport)
#define DIAG_CLASS_API __declspec(dllimport)
#endif


//class CPNPNotify : public QSYNC_LIB::INotify
//{
//public:
//	CPNPNotify(HANDLE hNotify, long nComport) :m_hNotify(hNotify), m_nComport(nComport)
//	{
//		QSYNC_LIB::GetDeviceFinder()->Register(this);
//		QSYNC_LIB::GetDeviceFinder()->StartMonitor();
//		::ResetEvent(m_hNotify);
//	}
//	virtual ~CPNPNotify()
//	{
//		QSYNC_LIB::GetDeviceFinder()->Unregister(this);
//		QSYNC_LIB::GetDeviceFinder()->StopMonitor();
//	}
//
//	virtual void Event(const TSTRING& strEvent,long nParam)
//	{
//		using namespace QSYNC_LIB;
//		DeviceDataPt* pspDeviceData = (DeviceDataPt*)nParam;
//		DeviceDataPt spDeviceData = *pspDeviceData; 
//
//		if(strEvent == DEVICE_DIAG_CONNECT) 
//		{
//			TSTRING strPort = spDeviceData->ID();
//			strPort = strPort.substr(3, strPort.size()-3);
//			long nConnectPort = atol(strPort.c_str());
//			if(nConnectPort == m_nComport)
//				::SetEvent(m_hNotify);
//		}
//		else if(strEvent == DEVICE_DIAG_DISCONNECT)
//		{
//			::ResetEvent(m_hNotify);
//		}
//	}
//
//private:
//	HANDLE m_hNotify;
//	long m_nComport;
//};

class CDiag_Export : public IDiagExport
{
	class COTPControl
	{
	public:
		COTPControl():m_bAlwaysFlash(true), 
					  m_pQualcommDiag(NULL), 
					  m_strNWSCP(""),
					  m_strSSCP(""), 
					  m_str1ffLocPath(""),
					  m_strUSBIndex(""){}
		~COTPControl(){}
	public:
		bool m_bAlwaysFlash;
		CQualcommDiag* m_pQualcommDiag;
		std::string m_strNWSCP;
		std::string m_strSSCP;
		std::string m_str1ffLocPath;
		std::string m_strUSBIndex;  // for log
	};
public:
	CDiag_Export();
	virtual ~CDiag_Export();
	virtual void InitializeParameter(long nComport, bool bAlwaysFlash = true);
	virtual void SetUsbIndex(long nComport, const char* szUSBIndex);
	virtual bool GetUsbIndex(long nComport, CStringA& strUSBIndex);
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
	virtual bool UnLockUnit(long nComport, int nLockType/*0=NW, 1=NS, 2=SP, 3=CP, 4=SIM, 5=PROP1*/, char* szLock, bool& bUnlockOK);
	virtual bool ProgramNWSCP(long nComport, char* szLock);
	virtual bool UnLockPhoneCode(long nComport, char* szLock, bool& bUnlockOK);
	virtual bool ProgramPhoneCode(long nComport, char* szLock);
	virtual bool SetRTC(long nComport);
	virtual bool LockSPC(long nComport);
	virtual bool DeleteFiles();
	//virtual	bool WaitForPNPNotify(long nComport, int nTimeout);

public:
	virtual bool Flash(long nComport, char* szMotPath);
	virtual bool Flex(long nComport, char* szMotPath);
	virtual bool RestoreNV(long nComport, int nTimeout);
public:
	virtual int  GetLastError(long nComport);
	virtual bool GetLastError(long nComPort,const TCHAR* szModule, char* szErrMsg500);
	virtual bool GetLastError(long nComPort,const TCHAR* szFunctionName,const TCHAR* szModule, char* szErrMsg500);

private:
	bool SetFlashName(CFileGetter& FileManager, CQDOperator& QDownload, bool bDownloadOEMSBL);
	bool SetFlexName(CFileGetter& FileManager, CQDOperator& QDownload);
	bool SetLocName(long nComport, CFileGetter& FileManager);
	
	bool CompareVersion(long nComPort, CFileGetter& FileManager, CQDOperator& QDownload, bool& bDownloadOEMSBL);
	
private:
	bool GenSimData(long nComport, char* szIMEI100, BYTE* szFSN16, BYTE *pSimDataBuf, int pSimDataBufLen);
private:
	CCriticalSection m_CritMap;
	CCriticalSection m_CritUnZip;
	std::map<long, COTPControl> m_PortMap;
};



DIAG_API bool D_GetMotoXPRS(long nComPort, MotoXPRSDATA& motoXPRS,int nTimeOut);
DIAG_API bool D_GetMotoUSBPort_LONG(const std::string& strUSB,const std::string& strVID,const std::string& strPID,long& nCOMPort);/*strUSB = "/USB3"*/
DIAG_API bool D_GetMotoUSBPort(const std::string& strUSB,const std::string& strVID,const std::string& strPID,std::string& strCOMPort);/*strUSB = "/USB3"*/

DIAG_API bool D_NewDiagExportInterface(IDiagExport**);
DIAG_API bool D_DeleteDiagExportInterface(IDiagExport*);
DIAG_API bool D_SetTestMode(long nComPort);
DIAG_API bool D_Reset(long nComPort);
DIAG_API bool D_ReadIMEI(long nComPort,char* szIMEI100);
DIAG_API void D_Log(const CString& strLog);
DIAG_API bool D_UnlockSPC(long nComport, char* szLock);  
DIAG_API bool D_UnlockPhoneCode(long nComport, char* szLock);
DIAG_API bool D_UnlockNW(long nComport, char* szLock);		//Network Personalization
DIAG_API bool D_UnlockNS(long nComport, char* szLock);		//Network Subset Personalization
DIAG_API bool D_UnlockSP(long nComport, char* szLock);		//Service Provider Personalization
DIAG_API bool D_UnlockCP(long nComport, char* szLock);		//Corporate Personalization
DIAG_API bool D_UnlockSIM(long nComport, char* szLock);		//SIM Personalization
DIAG_API bool D_UnlockPROP(long nComport, char* szLock);	//Antitheft Personalization
DIAG_API bool D_BackupNV(long nComport, int nTimeout = 8000);



/*Sample code
std::string strCOMPort;
D_GetMotoUSBPort("/USB1","1d45","4510",strCOMPort);
*/
