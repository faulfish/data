#include "QualcommRequest.h"

#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{
	class RM_CLASS_API CMotoRequestBase  : public CDiagRequestBase
	{
	public:
		CMotoRequestBase(int nNewVersion);
		virtual ~CMotoRequestBase();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};
	
	class RM_CLASS_API CWriteExceptionToEFSRequest  : public CDiagRequestBase
	{
	public:
		CWriteExceptionToEFSRequest(int nVersion);
		virtual ~CWriteExceptionToEFSRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CWriteSIMDataRequest  : public CDiagRequestBase
	{
	public:
		CWriteSIMDataRequest(BYTE *pSimDataBuf, int nSimDataBufLen,int& nResult);
		virtual ~CWriteSIMDataRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nResult;
	};

	/*
	typedef struct  
	{
	BYTE command_code;  //75 (0x45)
	BYTE subsys_id;     //67 (0x43)
	WORD subsys_cmd_code; //1 Read,2 Write, 3 Clear
	BYTE FailureByte;      //InOut
	}FailureByte_REQ;
	*/
	class RM_CLASS_API CFailureByteRequest  : public CDiagRequestBase
	{
	public:
        CFailureByteRequest(WORD nFunction,BYTE& byteFailure,bool& bSuccess,int nType=0);
		virtual ~CFailureByteRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		WORD m_nFunction;
		BYTE& m_byteFailure;
		bool& m_bSuccess;
        int m_nType;
	};

	class RM_CLASS_API CWriteDefaultPhoneCodeRequest  : public CDiagRequestBase
	{
	public:
		CWriteDefaultPhoneCodeRequest(bool& bSuccess);
		virtual ~CWriteDefaultPhoneCodeRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};

	class RM_CLASS_API CFreezeOTPRequest  : public CDiagRequestBase
	{
	public:
		CFreezeOTPRequest(bool& bSuccess);
		virtual ~CFreezeOTPRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};

	class RM_CLASS_API CQueryFreezeOTPRequest  : public CDiagRequestBase
	{
	public:
		CQueryFreezeOTPRequest(bool& bSuccess,int &nLastError);
		virtual ~CQueryFreezeOTPRequest();
	protected:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		int& m_nLastError;
	};

	//---------------------------------------------------------------//
	class RM_CLASS_API CQueryHDVersionRequest: public CDiagRequestBase
	{ // same as isOTPFrozen command
	public:
		CQueryHDVersionRequest(int& nPhoneCodeVersion, int& nBootloaderVersion, bool& bExecuteResult);
		virtual ~CQueryHDVersionRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		int& m_nPhoneCodeVersion;
		int& m_nBootloaderVersion;
		bool& m_bExecuteResult;
	};
	
	//************************************
	// Class:    CReleaseDateRequest
	// For getting sw release date
	//************************************
	class RM_CLASS_API CReleaseDateRequest: public CDiagRequestBase
	{
	public:
		CReleaseDateRequest(char* szDate12, char* szTime9, bool& bExecuteResult);
		virtual ~CReleaseDateRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
	public:
		char* m_szDate12;
		char* m_szTime9;
		bool& m_bExecuteResult;
	};
	//---------------------------------------------------------------//

	class RM_CLASS_API CGSensorRequest  : public CDiagRequestBase
	{
	public:
		CGSensorRequest(int nFunction,int& nTemp, bool& bSuccess);
		CGSensorRequest(int& m_nX,int& m_nY,int& m_nZ,  bool& bSuccess);
		CGSensorRequest(bool& bSuccess,int& nOffsetX,int& nOffsetY,int& nOffsetZ);
		CGSensorRequest(bool& bIsGSensorWork,bool& bIsUpWork,int& nTemp);
		virtual ~CGSensorRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int m_nVersion;
		bool &m_bSuccess;
		int& m_nX;
		int& m_nY;
		int& m_nZ;
		bool& m_bIsGSensorWork;
		bool& m_bIsUpWork;
	};

	class RM_CLASS_API CDiagRAWRequest: public CDiagRequestBase
	{
	public:
		CDiagRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC,bool bHasReturn);
		virtual ~CDiagRAWRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CByPassCellTemperatureCheckRequest: public CDiagRequestBase
	{
	public:
		CByPassCellTemperatureCheckRequest();
		virtual ~CByPassCellTemperatureCheckRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
	};

	class RM_CLASS_API CAskLavernockVarientRequest: public CDiagRequestBase
	{
	public:
		CAskLavernockVarientRequest(int& nVersion);
		CAskLavernockVarientRequest(int& nVarientVersion,int& nEVTVersion);
		virtual ~CAskLavernockVarientRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nVarientVersion;
		int& m_nEVTVersion;
		bool m_bNewVersion;
	};
	
	
	class RM_CLASS_API CChargingSwitchRequest: public CDiagRequestBase
	{
	public:
		CChargingSwitchRequest(short Mode_ID,bool bEnable,bool& bSuccess);
		virtual ~CChargingSwitchRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};

	class RM_CLASS_API CKeypadRequest: public CDiagRequestBase
	{
	public:
		CKeypadRequest(short Mode_ID,bool bEnable,bool& bSuccess);
		virtual ~CKeypadRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		short m_nMode_ID;
	};

	class RM_CLASS_API CDisplayRequest: public CDiagRequestBase
	{
	public:
		CDisplayRequest(short Color565,BYTE byteBacklight,bool& bSuccess);
		virtual ~CDisplayRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		short m_Color565;
		BYTE m_byteBacklight;
	};

	class RM_CLASS_API CVibratorRequest: public CDiagRequestBase
	{
	public:
		CVibratorRequest(bool bEnable,bool& bSuccess);
		virtual ~CVibratorRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};

	class RM_CLASS_API CReadCoincellVoltageRequest: public CDiagRequestBase
	{
	public:
		CReadCoincellVoltageRequest(int& nVoltage,bool& bSuccess);
		virtual ~CReadCoincellVoltageRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		int& m_nVoltage;
	};

	class RM_CLASS_API CFlipStatusRequest: public CDiagRequestBase
	{
	public:
		CFlipStatusRequest(bool& m_bOpenFlip,bool& bSuccess);
		virtual ~CFlipStatusRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived)
		{
			return CDiagRequestBase::IsCompletePacket(ptBufferReceived);
		}
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
		bool& m_bOpenFlip;
	};
	
	class RM_CLASS_API CCaptureScreenshotRequest: public CDiagRequestBase
	{
	public:
		CCaptureScreenshotRequest();
		virtual ~CCaptureScreenshotRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool m_bFirst;
	};

	class RM_CLASS_API CBatteryTemperatureDetectionSwitchRequest: public CDiagRequestBase
	{
	public:
		CBatteryTemperatureDetectionSwitchRequest(bool bEnable,bool& bSuccess);
		virtual ~CBatteryTemperatureDetectionSwitchRequest(){}
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool& m_bSuccess;
	};
}
