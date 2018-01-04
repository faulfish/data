#ifndef __DL__H__
#define __DL__H__
#pragma once

#include "..\..\Lib\RequestUtility\InterfaceBase.h"
#include "..\..\Lib\RequestUtility\DownloadCommand.h"
#include "..\..\RequestManager\Request\DownloadRequest.h"
#include "..\..\CommonUtil\RefObject.h"
#include <vector>
#include "..\..\CommonUtil\NotifyBase.h"
#include "..\..\CommonUtil\ErrorCode.h"

#pragma warning(push)
#pragma warning(disable:4251)


#ifdef QISDA_DOWNLOAD_MODULE_EXPORTS
#define QISDA_DOWNLOAD_API extern "C" __declspec(dllexport)
#define QISDA_DOWNLOAD_CLASS_API __declspec(dllexport)
#else
#define QISDA_DOWNLOAD_API extern "C" __declspec(dllimport)
#define QISDA_DOWNLOAD_CLASS_API __declspec(dllimport)
#endif

const LPCSTR EVENT_DL_PROGRESS		= "TNotify_DL_Progress";
const LPCSTR EVENT_DL_PARTITION_DIF = "TNotify_DL_Partition_Different";

const int g_nTimeout = 20000;
const int g_nRetryTimes = 3;
/************************************************************************/
/* 6240 fine tune data                                                  */
/************************************************************************/
const int g_n6240NopRetryTimes		   		= 30;
const int g_n6240NopTimeout			   		= 1000;
const int g_n6240HelloRetryTimes	   		= 100;
const int g_n6240HelloTimeout		   		= 700;
const int g_n6240ReOpenAfterGoSleepTime		= 35000;
const int g_n6240ReOpenAfterResetSleepTime	= 40000;
const int g_n6240ReOpenAfterResetSleepTime_Path = 30000;
const int g_n6240CloseImageTimeout			= 300000;
/************************************************************************/
/* 6245 fine tune data                                                  */
/************************************************************************/
const int g_n6245NopRetryTimes				= 20;
const int g_n6245NopTimeout					= 1000;
const int g_n6245HelloRetryTimes			= 60;
const int g_n6245HelloTimeout				= 500;
const int g_n6245HelloRetryTimes_CheckBadBlock			= 200;
const int g_n6245HelloTimeout_CheckBadBlock				= 1000;
const int g_n6245ReOpenAfterResetSleepTime  = 35000;
const int g_n6245ReOpenAfterResetSleepTime_Path = 35000;
const int g_n6245CloseImageTimeout			= 120000;

enum DownloadStep
{
	None = 0,  // For notice
	Connection = 1,
	ChangeDLMode = 3,
	DMSSProtocol = 4,
	StreamingProtocol = 5,
	ReOpenComport = 6
};

typedef struct
{
	char* szMsg;
	DownloadStep nStep;
	int nProgress;
}DL_PROGRESS;


class QISDA_DOWNLOAD_CLASS_API CDLCommand : public CInterfaceBase, public QSYNC_LIB::CNotifyBase_ANSI, public CLog, public CLastError
{
public:
	CDLCommand(int nCOMPort, const std::string& strPlatform = "Qualcomm6245");
	virtual ~CDLCommand();
public: // DMSS DL
	//Command
	virtual bool DMSSNop(int nRetryTimes = 10, int nTimeout=g_nTimeout);
	virtual bool DMSSParameter(int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool DMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool DMSSGo(WORD nwSegment,WORD nwoffset, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
public: // Streaming DL
	//Command
	virtual bool StreamingHello(bool bSpeedUp = false, int nRetryTimes = 50, int nTimeout= 500);
	virtual bool StreamingHello_CheckBadBlock(bool bSpeedUp = false, int nRetryTimes = 50, int nTimeout= 500); // only support in Speedup DL
	virtual bool StreamingSecurity(const std::string& strPBL, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool StreamingPartition(BYTE* lpbyData, WORD wnlength/*max=512*/, bool bOverride, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool StreamingOpenMultiImage(BYTE nType, BYTE* lpbyData, WORD wnlength/*max=512*/, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool StreamingStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout); // stream write
	virtual bool StreamingStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, Struct_XOR& nXOR, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout); // stream write with return XOR[4]
	virtual bool StreamingClose(bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool StreamingReset(bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);

	virtual bool QueryImageSize(int& nSize, BYTE lpXOR[4],bool& bSupported, int nTimeout=g_nTimeout); // only support in Speedup DL
public: // only for Driver test
	virtual bool StreamingStrWrite512(BYTE* lpData, WORD wnLength, DWORD dwAddress, Struct_XOR& nXOR, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout); // stream write with return XOR[4]
	virtual bool WriteDumpImage512(BYTE* lpData, WORD wnLength, DWORD dwAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool WriteDumpImage(BYTE* lpData, WORD wnLength, DWORD dwAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);


/************************************************************************/
/*                    Callback related function                         */
/************************************************************************/
public:
	void AddMsg(const char* szMsg, int nStep, int nProgress); //Fire function
	int m_nProgress; // Keep Original Progress data
	std::string m_strMsg; // Keep Original Message

	void AddMsg(bool bResult, const char* szTitle, int nStep); // Notify successful or failed in the end of command 
	void AddMsg(BYTE nType/*ImageType*/, int nStep); // only to title what Image before Open/Write/Close Image
public:// CNotifyBase
	virtual bool Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent);
	virtual bool Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent);
	virtual void Fire(const std::string& strEvent,long nParam);
	virtual void DefineNotify(const std::string& strNewEvent);

	/************************************************************************/
	/*                   ErrorCode related function                         */
	/************************************************************************/
public:
	void SetLastError(DL_ERROR nError, bool bExecuteResult = true);
};

/************************************************************************
Class: CDLCompoundCommand
Purpose: Combination of DLCommand
************************************************************************/
class QISDA_DOWNLOAD_CLASS_API CDLCompoundCommand : public CDLCommand
{
public:
	CDLCompoundCommand(int nCOMPort, const std::string& strPlatform = "Qualcomm6245");
	virtual ~CDLCompoundCommand();


public: // DMSS DL
	virtual bool DMSSOpenAndWriteHexFile(const std::string& strHexFilePath, WORD& wAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool DMSSOpenAndWriteHexFile(const BYTE* pData, int nDataSize, WORD& wAddress, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
public: // Streaming DL
	virtual bool StreamingOpenAndWritePartition(const std::string& strPartition, bool bOverride, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);

	virtual bool StreamingOpenHeaderImage(BYTE nType, const std::string& strHeaderImagePath/*MaxImageSize=512*/, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool StreamingOpenHeaderImage(BYTE nType, const BYTE* pImageData, int nImageSize/*MaxImageSize=512*/, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout=g_nTimeout);
	virtual bool StreamingWriteMultiImage(const std::string& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout); // use StreamWrite with return XOR[4]
	virtual bool StreamingWriteMultiImage(const BYTE* pImageData, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nTimeout = g_nTimeout);

	virtual bool StreamingDLMultiImage(BYTE nImageType, const std::string& strHeaderImagePath, const std::string& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open(Header)+Write(Image)+Close
	virtual bool StreamingDLMultiImage(BYTE nImageType, const BYTE* pHeaderData, int nHeaderSize, const BYTE* pImageData, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open(Header)+Write(Image)+Close
	virtual bool StreamingDLMultiImage(BYTE nImageType, const std::string& strImagePath, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open+Write(Image)+Close
	virtual bool StreamingDLMultiImage(BYTE nImageType, const BYTE* pImageData, int nImageSize, bool bSpeedUp = false, int nRetryTimes = g_nRetryTimes, int nOpenTimeout=g_nTimeout, int nWriteTimeout=g_nTimeout, int nCloseTimeout=g_nTimeout); //Open+Write(Image)+Close

protected:
	bool CompareImageSize(bool bSpeedUp, int nSize, BYTE lpXOR[4], int nTimeout=g_nTimeout); //use QueryImageSize
};



/************************************************************************
 Class: CDLFormatCommand
 Purpose: independent of DMSS/Streaming DLCommand/DLCompoundCommand
************************************************************************/
class QISDA_DOWNLOAD_CLASS_API CDLFormatCommand: public CDLCompoundCommand
{
public:
	CDLFormatCommand(int nCOMPort, const std::string& strPlatform = "Qualcomm6245");
	virtual ~CDLFormatCommand();
public:
	bool CheckBadBlock(std::string& strBadBlockInfo, int nOption=0, int nTimeout=60000); // for Driver Test
	typedef void (*CallBackInfo)(const std::string&);
	bool CheckBadBlock(const char* szCmdInput, int nCmdLength, int nTimeout, CallBackInfo CallBack); // for Driver Test
	bool CheckBadBlockWithHandShaking(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout,CallBackInfo Callback); // for Driver Test
	bool DumpImage(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout, CBufferPtr ptImage);  // for Driver Test

	bool RAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC);
	bool GetDLBootLoaderRevocation(int& nStatus,int& nVersion);
	bool GetDLPhoneCodeRevocation(int& nStatus,int& nVersion);
	bool DLGetHWVariant(int& nHWID, int& nVariantID, int nTimeout=g_nTimeout); // ONLY support Lavernock
    
	//////////////////////////////////////////////////////////////////////////
	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/);
	virtual void SetResetMode(bool bReset);
	virtual bool SendHello(bool bSpeedUp = true);
    virtual void SetIsOveridePartition(bool bOverridePartition);
    virtual bool StreamingDLFromPartition(bool bSpeedup = true, bool bReopenAfterReset=false/*false=not to do this step*/); //bSpeedup must be the same as previous one
};

/************************************************************************/
//for 6245(Wellfleet/Lavernock)
class QISDA_DOWNLOAD_CLASS_API CDownload6245 : public CDLFormatCommand, public CRefObject
{
public:
	CDownload6245(int nCOMPort);
	virtual ~CDownload6245(void);
public:
	virtual bool CFCDownload(bool bSpeedup=true);  // for CFC download  = DMSS+Streaming, default speedUp download.
	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/);
	virtual bool DMSSDownload(const std::string& strHexFilePath = "" /*"" means to use m_strHEX*/);
	virtual bool StreamingDL(bool bSpeedUp = true);
	
public: //Setting
	void SetIsOveridePartition(bool bOverridePartition);
	void SetResetMode(bool bReset);
	// Set File Path
	bool SetHex(const std::string& strFilePath);
	bool SetPartition(const std::string& strFilePath);	
	bool SetPBL(const std::string& strFilePath);

	bool SetQCSBLHD(const std::string& strFilePath); 
	bool SetQCSBL(const std::string& strFilePath);   
	bool SetOEMSBLHD(const std::string& strFilePath);
	bool SetOEMSBL(const std::string& strFilePath);	 
	bool SetAMSSHD(const std::string& strFilePath);  
	bool SetAMSS(const std::string& strFilePath);	 
	bool SetCEFS(const std::string& strFilePath);	 
protected:
	std::string m_strHEX;
	std::string m_strPBL;
	std::string m_strPartition;
	std::string m_strQCSBL;
	std::string m_strQCSBLHD;
	std::string m_strOEMSBL;
	std::string m_strOEMSBLHD;
	std::string m_strAMSS;
	std::string m_strAMSSHD;
	std::string m_strCEFS;
	bool	m_bReopenAfterReset; // For QDownload
	bool    m_bOverridePartition;
	bool    m_bResetMode;
};

//for 6240(Melos)
class QISDA_DOWNLOAD_CLASS_API CDownload6240 : public CDLFormatCommand, public CRefObject
{
public:
	CDownload6240(int nCOMPort, const std::string& strPlatform = "Qualcomm6240");
	virtual ~CDownload6240(void);
public: //Setting
	void SetIsOveridePartition(bool bOverridePartition);
	void SetResetMode(bool bReset);
	// Set File Path
	bool SetHex(const std::string& strFilePath);
	bool SetPartition(const std::string& strFilePath);	
	bool SetPBL(const std::string& strFilePath);
	bool SetDBL(const std::string& strFilePath);	  
	bool SetFSBL(const std::string& strFilePath);	  
	bool SetOSBL(const std::string& strFilePath);	
	bool SetAMSS(const std::string& strFilePath);	  
	bool SetCEFS(const std::string& strFilePath);	  
public:
	virtual bool CFCDownload(bool bSpeedup=true);  // for CFC download  = DMSS+Streaming, default speedUp download.
	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/);
	virtual bool DMSSDownload(const std::string& strHexFilePath = "" /*"" means to use m_strHEX*/);
	virtual bool StreamingDL(bool bSpeedUp = true);
	bool StreamingDLFromPartition(bool bSpeedup = true, bool bReopenAfterReset=false/*false=not to do this step*/); //bSpeedup must be the same as previous one
protected:
	std::string m_strHEX;
	std::string m_strPBL;
	std::string m_strPartition;
	std::string m_strAMSS;
	std::string m_strCEFS;

	std::string m_strDBL;
	std::string m_strFSBL;
	std::string m_strOSBL;
	bool	m_bReopenAfterReset; // For QDownload
	bool    m_bOverridePartition;
	bool    m_bResetMode;
};


//for 6270(Ithaca)
class QISDA_DOWNLOAD_CLASS_API CDownload6270 : public CDownload6240
{
public:
	CDownload6270(int nCOMPort);
	virtual ~CDownload6270(void);
public:
	virtual bool CFCDownload(bool bSpeedup=true);  // for CFC download  = DMSS+Streaming, default speedUp download.
	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/);
	virtual bool DMSSDownload(const std::string& strHexFilePath = "" /*"" means to use m_strHEX*/);
	virtual bool StreamingDL(bool bSpeedUp = true);
};
typedef struct
{
	BYTE* pData;
	int nSize;
}IMAGE;

//for 6245(Wellfleet/Lavernock):Share Image
class QISDA_DOWNLOAD_CLASS_API CShareDownload6245 : public CDLFormatCommand, public CRefObject
{
public:
	CShareDownload6245(int nCOMPort);
	virtual ~CShareDownload6245();

public: //Setting
	void SetIsOveridePartition(bool bOverridePartition);
	void SetResetMode(bool bReset);
	// Set File Path
	bool SetHex(const BYTE* pData, const int& nSize);
	bool SetPartition(const BYTE* pData, const int& nSize);	
	bool SetPBL(const BYTE* pData, const int& nSize);

	bool SetQCSBLHD(const BYTE* pData, const int& nSize); 
	bool SetQCSBL(const BYTE* pData, const int& nSize);   
	bool SetOEMSBLHD(const BYTE* pData, const int& nSize);
	bool SetOEMSBL(const BYTE* pData, const int& nSize);	 
	bool SetAMSSHD(const BYTE* pData, const int& nSize);  
	bool SetAMSS(const BYTE* pData, const int& nSize);	 
	bool SetCEFS(const BYTE* pData, const int& nSize);	 

public:
	bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/);
	bool CFCDownload(bool bSpeedup=true);  // for CFC download  = DMSS+Streaming, default speedUp download.
	virtual bool DMSSDownload();
	virtual bool StreamingDL(bool bSpeedUp = true);
protected:
    bool SetImage(IMAGE& image, const BYTE* pData, const int& nSize) {
        if(pData == NULL) return false;
        image.pData = (BYTE*)pData;
        image.nSize = nSize;
        return true;
    };

protected:
	IMAGE m_imgHEX;
	IMAGE m_imgPBL;
	IMAGE m_imgPartition;
	IMAGE m_imgQCSBL;
	IMAGE m_imgQCSBLHD;
	IMAGE m_imgOEMSBL;
	IMAGE m_imgOEMSBLHD;
	IMAGE m_imgAMSS;
	IMAGE m_imgAMSSHD;
	IMAGE m_imgCEFS;

	bool	m_bReopenAfterReset; // For QDownload
	bool    m_bResetMode;
	bool    m_bOverridePartition;
};

//for 6240(Melos):Share Image
class QISDA_DOWNLOAD_CLASS_API CShareDownload6240 : public CDLFormatCommand, public CRefObject
{
public:
	CShareDownload6240(int nCOMPort, const std::string& strPlatform = "Qualcomm6240");
	virtual ~CShareDownload6240();

public: //Setting
	void SetIsOveridePartition(bool bOverridePartition);
	void SetResetMode(bool bReset);
	// Set File Path
	bool SetHex(const BYTE* pData, const int& nSize);
	bool SetPartition(const BYTE* pData, const int& nSize);	
	bool SetPBL(const BYTE* pData, const int& nSize);
	bool SetAMSS(const BYTE* pData, const int& nSize);	 
	bool SetCEFS(const BYTE* pData, const int& nSize);	 

	bool SetDBL(const BYTE* pData, const int& nSize);	  
	bool SetFSBL(const BYTE* pData, const int& nSize);	  
	bool SetOSBL(const BYTE* pData, const int& nSize);	

public:
	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/);
	virtual bool CFCDownload(bool bSpeedup=true);  // for CFC download  = DMSS+Streaming, default speedUp download.
	virtual bool DMSSDownload();
	virtual bool StreamingDL(bool bSpeedUp = true);
	bool StreamingDLFromPartition(bool bSpeedup = true, bool bReopenAfterReset=false/*false=not to do this step*/);  //bSpeedup must be the same as previous one
protected:
    bool SetImage(IMAGE& image, const BYTE* pData, const int& nSize) {
        if(pData == NULL) return false;
        image.pData = (BYTE*)pData;
        image.nSize = nSize;
        return true;
    };

protected:
	IMAGE m_imgHEX;
	IMAGE m_imgPBL;
	IMAGE m_imgPartition;

	IMAGE m_imgDBL;
	IMAGE m_imgFSBL;
	IMAGE m_imgOSBL;
	IMAGE m_imgAMSS;
	IMAGE m_imgCEFS;

	bool	m_bReopenAfterReset; // For QDownload
	bool    m_bOverridePartition;
	bool    m_bResetMode;
};

//for 6270(Ithaca):Share Image
class QISDA_DOWNLOAD_CLASS_API CShareDownload6270 : public CShareDownload6240
{
public:
	CShareDownload6270(int nCOMPort);
	virtual ~CShareDownload6270();
public:
	virtual bool Download(bool bSpeedup=true, bool bReopenAfterReset=false/*false=not to do this step*/);
	virtual bool CFCDownload(bool bSpeedup=true);  // for CFC download  = DMSS+Streaming, default speedUp download.
	virtual bool DMSSDownload();
	virtual bool StreamingDL(bool bSpeedUp = true);
};
typedef CRefObjectPtr<CDownload6245> CDownload6245Ptr;
typedef CRefObjectPtr<CDownload6240> CDownload6240Ptr;
typedef CRefObjectPtr<CDownload6270> CDownload6270Ptr;
typedef CRefObjectPtr<CShareDownload6245> CShareDownload6245Ptr;
typedef CRefObjectPtr<CShareDownload6240> CShareDownload6240Ptr;
typedef CRefObjectPtr<CShareDownload6270> CShareDownload6270Ptr;




#pragma warning(pop)
#endif // __DL__H__