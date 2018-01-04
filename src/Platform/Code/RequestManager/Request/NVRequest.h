#pragma once
#include "QualcommRequest.h"
#include "..\..\Lib\msm\nv_items.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "../../odmtma_api/SerCOMLib/CRCMachine.h"
#include "../../odmtma_api/SerCOMLib/SerCOMLibrary.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{
	class RM_CLASS_API CNVRequest :	public CDiagRequestBase
	{
	public:
		CNVRequest(const WORD& NVItem, const WORD& nDIAGcmd, DIAG_NVITEM_PACKET_F_type& NVResp, const BYTE *WriteData=NULL, const size_t szWriteData=0);
		virtual ~CNVRequest(void);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

	private:
		WORD m_NVItem;
		DIAG_NVITEM_PACKET_F_type& m_ReadResp;
	};

    //  [6/19/2008 Eddy.Guo]
    class RM_CLASS_API CClearActivationDataRequest : public CDiagRequestBase {
    public:
        CClearActivationDataRequest();
        virtual ~CClearActivationDataRequest();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        bool CheckCmdResult();
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) {
            return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
        }
    private:
        bool m_bIsCmdSuccess;
    };

    //  [6/24/2008 Eddy.Guo]
    class RM_CLASS_API CStandbyCurrentTest : public CDiagRequestBase {
    public:
        CStandbyCurrentTest();
        virtual ~CStandbyCurrentTest();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };
    //  [6/26/2008 Eddy.Guo]
    class RM_CLASS_API CMasterClear : public CDiagRequestBase {
    public:
        CMasterClear();
        virtual ~CMasterClear();
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) {
            return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
        }
    };

    //  [6/26/2008 Eddy.Guo]
    class RM_CLASS_API CWriteNVData : public CDiagRequestBase {
    public:
        CWriteNVData(const WORD& NVItem, const WORD& nDIAGcmd, DIAG_NVITEM_PACKET_F_type& WritePacket);
        virtual ~CWriteNVData();
        bool GetResponse(DIAG_NVITEM_PACKET_F_type& ResponsePacket);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived) {
            return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
        }
    };

    class RM_CLASS_API CBTHeadsetCharging : public CDiagRequestBase {
    public:
        CBTHeadsetCharging(bool IsOn);
        virtual ~CBTHeadsetCharging();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        bool m_bIsOn;
    };

    class RM_CLASS_API CSearchBTDevice : public CDiagRequestBase {
    public:
        CSearchBTDevice();
        virtual ~CSearchBTDevice();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
        bool GetStatus();
    private:
        bool m_status;
    };

	/*
	Check BT pair command by BTAddress	
	*/
    class RM_CLASS_API CQueryCommand : public CDiagRequestBase 
	{
    public:
        CQueryCommand(const char* BTAddress, int length,bool& bIsPaired);
        virtual ~CQueryCommand();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	protected:
		int HexToInt(char c);
    private:
        bool& m_bIsPaired;
        char* m_pBTAddress;
        int m_nBTAddressLength;
    };

    //  [7/4/2008 Eddy.Guo] New Class for Read Factory Area
    class RM_CLASS_API CReadFactoryArea : public CDiagRequestBase {
    public:
        CReadFactoryArea(unsigned char* szBuf, int* nState);
        virtual ~CReadFactoryArea();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
        int GetState();
    private:
        int* m_pnState;
        unsigned char* m_szFactoryArea;
    };

    //  [7/7/2008 Eddy.Guo]
    class RM_CLASS_API CCheckSIM : public CDiagRequestBase {
    public:
        CCheckSIM(int* nState);
        virtual ~CCheckSIM();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
        int GetState();
    private:
        int* m_pnState;
    };

    //nFunction 1 = Backup NV partition
	//nFunction 2 = Restore NV partition
	//nFunction 3 = check backup NV exist
    class RM_CLASS_API CBackupRestoreNV : public CDiagRequestBase 
	{
    public:
        CBackupRestoreNV(int nFunction,bool& bStatus);
		CBackupRestoreNV(int nFunction,bool& bStatus,int& nBackupTimes, int& nY, int& nM, int& nD, int& nH, int& nMin, int& nS);
        virtual ~CBackupRestoreNV();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	private:
		int m_nFunction;
		bool& m_bStatus;
		int& m_nBackupTimes;
		int& m_nY;
		int& m_nM;
		int& m_nD;
		int& m_nH;
		int& m_nMin;
		int& m_nS;
    };

    //  [7/15/2008 Eddy.Guo]
    class RM_CLASS_API CBTHandsetVoltageReading : public CDiagRequestBase {
    public:
        CBTHandsetVoltageReading(long* nVoltage);
        virtual ~CBTHandsetVoltageReading();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        long* m_pnVoltage;

    };

	class RM_CLASS_API CSPCRequest: public CDiagRequestBase
	{
	public:
		CSPCRequest(const char *szSPC, bool &status);
		virtual ~CSPCRequest(void);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

	private:
		diagpkt_sec_code_req_type m_Req;
		bool &m_Status;		
	};

    //  [7/17/2008 Eddy.Guo] Set the mobile into online mode
    class RM_CLASS_API COnlineMode: public CDiagRequestBase
    {
    public:
        COnlineMode();
        virtual ~COnlineMode(void);
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };

    //  [7/17/2008 Eddy.Guo] Set the mobile into online mode
    class RM_CLASS_API COfflineMode: public CDiagRequestBase
    {
    public:
        COfflineMode();
        virtual ~COfflineMode(void);
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };

    //  [7/17/2008 Eddy.Guo] Set the mobile into FTM mode
    class RM_CLASS_API CFTMMode: public CDiagRequestBase
    {
    public:
        CFTMMode();
        virtual ~CFTMMode(void);
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };

    class RM_CLASS_API CResetHandset: public CDiagRequestBase {
    public:
        CResetHandset();
        virtual ~CResetHandset();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };

	class RM_CLASS_API CControlMode: public CDiagRequestBase
	{	// ref: DIAG_CONTROL_F_MODE enumeration
	public:
		CControlMode(const unsigned int mode);
		virtual ~CControlMode(void);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	};    
	
    class RM_CLASS_API CWriteQFuse : public CDiagRequestBase 
	{
        typedef struct {
            UINT chain;         //< decimal
            uint32 LSB;         //< hex
            uint32 MSB;         //< hex
            uint32 RegConf2;    //< hex
            uint32 RegConf3;    //< hex
            uint32 RegConf4;    //< hex
        } QFuseParameter;
    public:
        CWriteQFuse(UINT chain, uint32 LSB, uint32 MSB, uint32 RegConf2, uint32 RegConf3, uint32 RegConf4,bool& bIsFTM);
        virtual ~CWriteQFuse();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        QFuseParameter m_parameters;
		bool& m_bIsFTM;
    };

    class RM_CLASS_API CReadQFuseState: public CDiagRequestBase
    {
    public:
        CReadQFuseState(unsigned long* reg1, unsigned long* reg2, bool* bSuccess);
        virtual ~CReadQFuseState(void);
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        unsigned long* m_pReg1;
        unsigned long* m_pReg2;
        bool* m_bSuccess;
    };

    class RM_CLASS_API CChangeOnline: public CDiagRequestBase {
    public:
        CChangeOnline();
        virtual ~CChangeOnline();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };

    class RM_CLASS_API CChangeFTM: public CDiagRequestBase {
    public:
        CChangeFTM();
        virtual ~CChangeFTM();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };

    class RM_CLASS_API CPowerOff: public CDiagRequestBase {
    public:
        CPowerOff();
        virtual ~CPowerOff();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };


    class RM_CLASS_API CGetBootLoaderRevocation: public CDiagRequestBase {
    public:
        CGetBootLoaderRevocation(int* pnVersion);
        virtual ~CGetBootLoaderRevocation();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        int* m_pnVersion;
    };

    class RM_CLASS_API CGetPhonecodeRevocation: public CDiagRequestBase {
    public:
        CGetPhonecodeRevocation(int* pnVersion);
        virtual ~CGetPhonecodeRevocation();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        int* m_pnVersion;
    };

    class RM_CLASS_API CReadFTMNV : public CDiagRequestBase {
    public:
        CReadFTMNV(int* nOutFTMValue);
        virtual ~CReadFTMNV();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        // if the value is 0, handset is in online mode
        // if the value is 1, handset is in FTM
        int* m_pnFTMValue;
    };

    class RM_CLASS_API CAutoCommand : public CDiagRequestBase {
    public:
        CAutoCommand();
        virtual ~CAutoCommand();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    };

    class RM_CLASS_API CQueryAutoCommandResult : public CDiagRequestBase {
    public:
        CQueryAutoCommandResult(bool* bRet);
        virtual ~CQueryAutoCommandResult();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        bool* m_pbResult;
    };

	class RM_CLASS_API CReadBatteryTemperature : public CDiagRequestBase 
	{
    public:
        CReadBatteryTemperature(int& m_nTemperature);
        virtual ~CReadBatteryTemperature();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        int& m_nTemperature;
    };

    class RM_CLASS_API CReadBatteryVoltage : public CDiagRequestBase 
	{
    public:
        CReadBatteryVoltage(int* nVoltage);
        virtual ~CReadBatteryVoltage();
        virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
        bool Encode(CBufferPtr& ptBufferSend);
        virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
    private:
        int* m_pnVoltage;
    };

	/* **************************************************************
	*      Send Raw Data Request
	*    [i] pReqData - raw data the caller want to send
	*    [i] nReqDataSize - the raw data size 
	*    [i] pRespData - the response data for the raw request
	*    [i] nRespDataSize - the buffer data size of pRespData
    *    [o] nRespDataSize - the response data size
	************************************************************** */
	class RM_CLASS_API CSendSync: public CDiagRequestBase
	{
	public:
		CSendSync(const BYTE* pReqData, const UINT nReqDataSize, BYTE* pRespData, UINT& nRespDataSize);
		virtual ~CSendSync();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	private:
		BYTE* m_pRespData;
		UINT& m_nRespDataSize;
	};

	/* **************************************************************
	*      PRL (Preferred Roaming List)
	*    PRL is used to inform the mobile phone how to separate the 
	*    supported bands into band groups
	*    Please ask Protocol team for more details.
	*
	************************************************************** */
	class RM_CLASS_API CPRLRequest: public CDiagRequestBase
	{
	public:
		CPRLRequest(BYTE* pPRLData, UINT& nPRLDataSize, BYTE& Status, BOOL bRead=TRUE);
		virtual ~CPRLRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	private:
		BYTE* m_pPRLData;
		UINT& m_nPRLDataSize;
		BYTE &m_Status;
		BOOL m_bRead;
	};

	/* **************************************************************
	*   Query if EMU cable is plugged in.
	*   Reurn values: OUT bool bIsEMU
	*				  if bIsEMU is true; // it means EMU cable is plugged in 
	************************************************************** */
	class RM_CLASS_API CIsEMURequest: public CDiagRequestBase
	{
	public:
		CIsEMURequest(bool& bIsEMU);
		virtual ~CIsEMURequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bIsEMU;
	};
	//************************************
	// Class:    CWriteNVItemRequest
	// Parameter: IN  DIAG_NVITEM_PACKET_F_type& req
	//            OUT WORD& nNVStatus:
	// typedef enum {
	//		NV_DONE_S,          // Request completed okay
	//		NV_BUSY_S,          // Request is queued
	//		NV_BADCMD_S,        // Unrecognizable command field
	//		NV_FULL_S,          // The NVM is full
	//		NV_FAIL_S,          // Command failed, reason other than NVM was full
	//		NV_NOTACTIVE_S,     // Variable was not active
	//		NV_BADPARM_S,       // Bad parameter in command block
	//		NV_READONLY_S,      // Parameter is write-protected and thus read only
	//		NV_BADTG_S,         // Item not valid for Target
	//		NV_NOMEM_S,         // free memory exhausted
	//		NV_NOTALLOC_S,      // address is not a valid allocation
	//		NV_STAT_ENUM_PAD = 0x7FFF     // Pad to 16 bits on ARM
	//} nv_stat_enum_type;
	//************************************
	class RM_CLASS_API CWriteNVItemRequest: public CDiagRequestBase
	{
	public:
		CWriteNVItemRequest(DIAG_NVITEM_PACKET_F_type& req, WORD& nNVStatus);
		virtual ~CWriteNVItemRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	private:
		WORD& m_nNVStatus;
	};

	//************************************
	// Class:    CCloseConnectionRequest
	// Purpose:  For CFC 71M
	// Close the handset's connection until cut off the power supply
	//************************************
	class RM_CLASS_API CCloseConnectionRequest: public CDiagRequestBase
	{
	public:
		CCloseConnectionRequest(bool& bExecuteResult);
		virtual ~CCloseConnectionRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		bool& m_bExecuteResult;
	};

	//************************************
	// Class:    CGetPMICTimeRequest
	// Purpose:  GetRTC
	// Parameter: IN BYTE nMode: 2=24hrs
	//            IN nMonth, nDay, nYear, nHour, nMin, nSec: please input date and time
	//************************************
	class RM_CLASS_API CGetPMICTimeRequest: public CDiagRequestBase
	{
	public:
		CGetPMICTimeRequest(bool& bExecuteResult, BYTE nMode/*2=24hrs*/, BYTE& nMonth, BYTE& nDay, WORD& nYear, BYTE& nHour, BYTE& nMin, BYTE& nSec);
		virtual ~CGetPMICTimeRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		bool& m_bExecuteResult;
		BYTE& m_nMonth;
		BYTE& m_nDay;
		WORD& m_nYear;
		BYTE& m_nHour;
		BYTE& m_nMin;
		BYTE& m_nSec;
	};

	//************************************
	// Class:    CSetPMICTimeRequest
	// Purpose:  SetRTC
	// Parameter: IN BYTE nMode: 2=24hrs
	//            IN nMonth, nDay, nYear, nHour, nMin, nSec: please input date and time
	//************************************
	class RM_CLASS_API CSetPMICTimeRequest: public CDiagRequestBase
	{
	public:
		CSetPMICTimeRequest(bool& bExecuteResult, BYTE nMode/*2=24hrs*/, BYTE nMonth, BYTE nDay, WORD nYear, BYTE nHour, BYTE nMin, BYTE nSec);
		virtual ~CSetPMICTimeRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		bool& m_bExecuteResult;
	};

	//************************************
	// Class:    CReadQfuseRequest
	// same as CReadQfuseState 
	// created for making interface and program simpler
	//************************************
	class RM_CLASS_API CReadQfuseRequest: public CDiagRequestBase
	{
	public:
		CReadQfuseRequest(unsigned int& nReg1, unsigned int& nReg2, bool& bExecuteResult);
		virtual ~CReadQfuseRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		bool& m_bExecuteResult;
		unsigned int& m_nReg1;
		unsigned int& m_nReg2;
	};

	//************************************
	// Class:    CWriteQfuseRequest
	// same as CWriteQfuse 
	// created for making interface and program simpler
	//************************************
	class RM_CLASS_API CWriteQfuseRequest: public CDiagRequestBase
	{
	public:
		CWriteQfuseRequest(WORD chain_select, int config_register[5],  int& nQfusedStatus/*0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported*/, bool& bExecuteResult);
		virtual ~CWriteQfuseRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		bool& m_bExecuteResult;
		int& m_nQfusedStatus;
	};

	class RM_CLASS_API CReadSWVersionRequest: public CDiagRequestBase
	{
	public:
		CReadSWVersionRequest(char* szReadValue1024, long& nLength);
		virtual ~CReadSWVersionRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		long& m_nLength;
		char* m_szReadValue1024;
	};

	class RM_CLASS_API CUnlockSPCRequest: public CDiagRequestBase
	{
	public:
		CUnlockSPCRequest(const char *szSPC, bool &bUnlockOK, bool& bExecuteResult);
		virtual ~CUnlockSPCRequest(void);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);

	private:
		bool &m_bUnlockOK;
		bool& m_bExecuteResult;
	};

}