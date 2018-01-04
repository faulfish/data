#ifndef __DLREQUEST__H__
#define __DLREQUEST__H__
#pragma once
#include "QualcommRequest.h"
#include "..\..\Lib\RequestUtility\DiagCommand.h"
#include "..\..\odmtma_api\DownloadDLL\DLcommand.h"
#include "..\..\Lib\RequestUtility\DownloadCommand.h"
#include <atlstr.h>
#include <stdlib.h>
#include <tchar.h>


#ifdef REQUESTMANAGER_EXPORTS
#define RM_API extern "C" __declspec(dllexport)
#define RM_CLASS_API __declspec(dllexport)
#else
#define RM_API extern "C" __declspec(dllimport)
#define RM_CLASS_API __declspec(dllimport)
#endif

namespace PLATFORM_TOOL
{	
	
	class RM_CLASS_API CDownloadRequestBase  : public CRequestBase
	{
	public:
		CDownloadRequestBase(bool bHadReturnvalue = true);
		CDownloadRequestBase(const TSTRING& strName,PotocolType nPotocolType,int	nTimeout = 5000,bool bHadReturnvalue = true);
		virtual ~CDownloadRequestBase();

	public:
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual bool GetReceiveData(CBufferPtr& ptBufferReceived); // use for response function execute result
		
	protected:
		bool Tx(const char* szCommand,int nCommandLen, bool bSpeedUp = false);
		bool Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend, bool bSpeedup = false);
		
		bool GetProcessResult();
		void SetProcessResult(bool bResult);
		int GetLastError();
		void SetLastError(int nError);
		
	protected:	
		CBufferPtr m_ptTempBufferReceived;
		bool m_bRunSuccess;
		int m_nErrorCode;
	};
	
	//CDLRAWRequest Request
	class RM_CLASS_API CDLRAWRequest: public CDownloadRequestBase
	{
	public:
		CDLRAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC);
		virtual ~CDLRAWRequest();
	};

	//Send dl raw request with returned value
	class RM_CLASS_API CDLRawWithReturnRequest: public CDownloadRequestBase
	{
	public:
		CDLRawWithReturnRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC);
		virtual ~CDLRawWithReturnRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	//Boot Loader Revocation Baseline Request
	class RM_CLASS_API CDLBootLoaderRevacationRequest: public CDownloadRequestBase
	{
	public:
		CDLBootLoaderRevacationRequest(int& nStatus,int& nVersion);
		virtual ~CDLBootLoaderRevacationRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nStatus;
		int& m_nVersion;
	};

	//Phone code Revocation Baseline Request
	class RM_CLASS_API CDLPhoneCodeRevacationRequest: public CDownloadRequestBase
	{
	public:
		CDLPhoneCodeRevacationRequest(int& nStatus,int& nVersion);
		virtual ~CDLPhoneCodeRevacationRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nStatus;
		int& m_nVersion;
	};

	//Check Bad Block
	class RM_CLASS_API CDLCheckBadBlockRequest: public CDownloadRequestBase
	{
	public:
		CDLCheckBadBlockRequest(int nOption=0);
		virtual ~CDLCheckBadBlockRequest();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};
	//************************************
	// Class:    CDLRepeatBadBlockTestRequest
	// Purpose:  For repeating BadBlock test. It's specific command for driver test.
	// Parameters: IN BYTE* szCmdInput: raw request data
	//			   IN int nCmdLength: raw request data size
	//************************************
	typedef void (*CallBackInfo)(const TSTRING&);
	class RM_CLASS_API CDLRepeatBadBlockTestRequest: public CDownloadRequestBase
	{
	public:
		CDLRepeatBadBlockTestRequest(BYTE* szCmdInput, int nCmdLength, CallBackInfo AddMsg);
		virtual ~CDLRepeatBadBlockTestRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		void AddMsg(const TSTRING& strMsg);
	private: 
		CallBackInfo m_AddMsg;
	};

	class RM_CLASS_API CDLQueryHWVariant: public CDownloadRequestBase
	{
	public:
		CDLQueryHWVariant(int& nHWID, int& nVariantID);
		virtual ~CDLQueryHWVariant();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		int& m_nHWID;
		int& m_nVariantID;
	};


	/***************************************   DMSS   ***************************************/
	/***************** GetExecuteResult by ptRequest->GetReceiveData(ptBuffer)***************/
	class RM_CLASS_API CDMSSNop : public CDownloadRequestBase
	{
	public:
		CDMSSNop();
		virtual ~CDMSSNop();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDMSSParameter: public CDownloadRequestBase
	{
	public:
		CDMSSParameter();
		virtual ~CDMSSParameter();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDMSSWrite32Bit : public CDownloadRequestBase
	{
	public:
		CDMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress);
		virtual ~CDMSSWrite32Bit();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CDMSSGo : public CDownloadRequestBase
	{
	public:
		CDMSSGo(WORD nwSegment,WORD nwoffset);
		virtual ~CDMSSGo();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	/***************************  (Original/Speedup) Streaming Download  ****************************/
	/***************** GetExecuteResult by ptRequest->GetReceiveData(ptBuffer)***************/
	class RM_CLASS_API CStreamingDLHello : public CDownloadRequestBase
	{
	public:
		CStreamingDLHello(bool bSpeedUp = false);
		virtual ~CStreamingDLHello();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		bool m_bSpeedUp;
	};
	
	/***************************  For Check bad block hex, the response is different from normal hex ****************************/
	//************************************
	// Class:    CStreamingDLHello_CheckBadBlock(0x52)
	// Purpose:  check bad block and make it during 1st response and 2nd response.
	// Parameters: OUT bool& bIsHexSupported: for knowing if support its command
	// Note: 
	// 1. there are two responses. 
	//    (1st one(0x53) is instant response, and 2nd one(0x95) is for responding if finishing checking bad block)
	// 2. using this command still enables following speeding up.
	//************************************	
	class RM_CLASS_API CStreamingDLHello_CheckBadBlock : public CDownloadRequestBase
	{
	public:
		CStreamingDLHello_CheckBadBlock(bool& bIsHexSupported);
		virtual ~CStreamingDLHello_CheckBadBlock();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
		virtual bool WaitForWrite();
		virtual bool WaitForOtherResponse() {return m_bWaitForOtherResponse;}

	private:
		bool		m_bFirstResponse; //flag
		bool		m_bWaitForOtherResponse;
		bool&		m_bIsHexSupported;
		HANDLE		m_hSecondResEvent;
	};

	class RM_CLASS_API CStreamingDLSecurity : public CDownloadRequestBase
	{
	public:
		CStreamingDLSecurity(BYTE nMode, bool bSpeedUp = false);
		virtual ~CStreamingDLSecurity();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CStreamingDLPartition : public CDownloadRequestBase
	{
	public:
		CStreamingDLPartition(BYTE* lpbyData, WORD wnlength/*max=512*/, bool bOverride, BYTE& nStatus, bool bSpeedUp = false);
		virtual ~CStreamingDLPartition();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		BYTE& m_nRespStatus;
	};

	class RM_CLASS_API CStreamingDLOpenMultiImage : public CDownloadRequestBase
	{
	public:
		CStreamingDLOpenMultiImage(BYTE nType, BYTE* lpData, WORD wnLength/*max=512*/, bool bSpeedUp = false);
		virtual ~CStreamingDLOpenMultiImage();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	class RM_CLASS_API CStreamingDLStrWrite : public CDownloadRequestBase
	{
	public:
		CStreamingDLStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = false);
		virtual ~CStreamingDLStrWrite();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};

	// XOR the result
	// only support in Speedup DL
	class RM_CLASS_API CStreamingDLStrWriteWithXOR : public CDownloadRequestBase
	{
	public:
		CStreamingDLStrWriteWithXOR(BYTE* lpData, WORD wnLength/*max=1024*/, DWORD dwAddress, Struct_XOR& nXOR);
		virtual ~CStreamingDLStrWriteWithXOR();
		virtual bool Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
		Struct_XOR& m_nXOR;
	};
	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* write dump image with 0x96 command                                   */
	/* Should be in speed up command										*/					
	/************************************************************************/
	class RM_CLASS_API CWriteDumpImage512 : public CDownloadRequestBase
	{
	public:
		CWriteDumpImage512(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = true);
		virtual ~CWriteDumpImage512();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};
	//////////////////////////////////////////////////////////////////////////
	/************************************************************************/
	/* write dump image with 0x96 command                                   */
	/* Should be in speed up command										*/					
	/************************************************************************/
	class RM_CLASS_API CWriteDumpImage : public CDownloadRequestBase
	{
	public:
		CWriteDumpImage(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp = true);
		virtual ~CWriteDumpImage();
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
	};

	//////////////////////////////////////////////////////////////////////////
	// XOR the result
	// only support in Speedup DL
	// For write dump image 
	//////////////////////////////////////////////////////////////////////////
	class RM_CLASS_API CStreamingDLStrWrite512WithXOR : public CDownloadRequestBase
	{
	public:
		CStreamingDLStrWrite512WithXOR(BYTE* lpData, WORD wnLength/*max=512*/, DWORD dwAddress, Struct_XOR& nXOR);
		virtual ~CStreamingDLStrWrite512WithXOR();
		virtual bool Encode(const char* szCommand,int nCommandLen,CBufferPtr& ptBufferSend);
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		DWORD m_dwAddress;
		Struct_XOR& m_nXOR;
	};
	//////////////////////////////////////////////////////////////////////////

	class RM_CLASS_API CStreamingDLCloseImage : public CDownloadRequestBase
	{
	public:
		CStreamingDLCloseImage(bool bSpeedUp = false);
		virtual ~CStreamingDLCloseImage();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};
	
	class RM_CLASS_API CStreamingDLReset : public CDownloadRequestBase
	{
	public:
		CStreamingDLReset(bool bSpeedUp = false);
		virtual ~CStreamingDLReset();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	};

	//////////////////////////////////////////////////////////////////////////
	//************************************
	// Class:    CQueryImageSizeRequest
	// Purpose:  Query StreamWrite Total Image Size
	// Parameters: OUT QuerySizeResp& resp: response of image size and XOR result
	//             OUT bool& bBeSupported: if this command is not supported, return false.
	// GetExecuteResult by ptRequest->GetReceiveData(ptBuffer)
	//************************************
	class RM_CLASS_API CQueryImageSizeRequest : public CDownloadRequestBase
	{
	public:
		CQueryImageSizeRequest(QuerySizeResp& resp, bool& bSupported);
		virtual ~CQueryImageSizeRequest();
		virtual bool IsCompletePacket(CBufferPtr& ptBufferReceived);
		virtual void OnDataReceiver(CBufferPtr& ptBufferReceived);
	private:
		QuerySizeResp& m_resp;
		bool& m_bSupported;
	};

	
}

#endif // __DLREQUEST__H__