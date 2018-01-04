#ifndef __INTERFACE_EXPORT_H
#define __INTERFACE_EXPORT_H


interface IODMExport
{
	IODMExport(){}
	virtual ~IODMExport(){}
	virtual bool IsRSDInitiated() =0;
	virtual void InitializeParameter(long nComport, bool bAlwaysFlash = true) =0;
	virtual bool ConnectComPort(long nComport) =0;
	virtual bool DisconnectComPort(long nComport) =0;
	virtual bool TempDisconnectComPort(long nComport) =0;

	virtual bool GetLastError(long nComPort, const TCHAR* szModule, char* szErrMsg500)=0;
	//virtual bool GetLastError(long nComPort, const TCHAR* szFunctionName,const TCHAR* szModule, char* szErrMsg500)=0;
	virtual int  GetLastError(long nComport) =0;
	virtual void AddLog(long nPort, const char* szMsg) =0;

	virtual bool SetTestMode(long nComport) =0;
	virtual bool IsUnitOn(long nComport) =0;
	virtual bool IsUnitOff(long nComport) =0;
	virtual bool TurnUnitOff(long nComport) =0;
	virtual bool Restart(long nComport) =0;
	virtual bool Reset(long nComport) =0;
	virtual bool ReadIMEI(long nComport, char* szIMEI100) =0;
	virtual bool ReadBTAddr(long nComport,char* szAddress100, long& nLength) =0;
	virtual bool WriteBTAddr(long nComport,char* szAddress) =0;
	virtual bool ReadFlexVersion(long nComport, char* szReadValue1024, long& nLength) =0;
	virtual bool ReadSWVersion(long nComport, char* szReadValue1024, long& nLength) =0;
	virtual bool ReadFactoryInforBlock(long nComport, char* szBlock, long& nLength) =0;
	virtual bool WriteFactoryInforBlock(long nComport, char* szBlock) =0;
	virtual bool SetInFactoryFlag(long nComport, bool bOpen) =0;
	virtual bool MasterClear(long nComport) =0;
	virtual bool IsSPCLocked(long nComport, long& nStatus) =0;
	virtual bool IsUnitLocked(long nComport, long& nStatus, int nLockType/*0=NWSCP, 1=SSCP*/) =0;
	virtual bool IsNWSCPLocked(long nComport, bool& bNWUnlocked, bool& bNSUnlocked, bool& bSPUnlocked, bool& bCPUnlocked, bool& bSIMUnlocked) =0;
	virtual bool UnlockSPC(long nComport, char* szLock, bool& bUnlockOK) =0;
	virtual bool UnLockNWSCP(long nComport, int nLockType/*0=NWSCP*/, char* szLock, bool& bUnlockOK) =0;
	virtual bool UnlockSSCP(long nComport, char* szLock, bool& bUnlockOK) =0;
	virtual bool ProgramSPC(long nComport, char* szLock) =0;
	virtual bool LockSPC(long nComport) =0;
	virtual bool ProgramNWSCP(long nComport, char* szLock) =0;
	virtual bool ProgramSSCP(long nComport, char* szLock) =0;
	virtual bool SetRTC(long nComport) =0;
	virtual bool ProgramIMEI(long nComport, char* szIMEI) =0;
	virtual bool WriteSimData(long nComport) =0;

	virtual bool ReadTransceiverKitNumber(long nPort,  char * szTransceiverKitNumber100) =0;
	virtual bool WriteTransceiverKitNumber(long nComport,  char* szTransceiverKitNumber100) =0;
	virtual bool ReadTrackID(long nPort, char * szTrackID) =0;
	virtual bool WriteTrackID(long nComport, char * szTrackID) =0;

	virtual bool Flash(long nComport, char* szMotPath) =0;
	virtual bool Flex(long nComport, char* szMotPath) =0;
	virtual bool RestoreNV(long nComport, int nTimeout) =0;
};

#endif //__INTERFACE_EXPORT_H