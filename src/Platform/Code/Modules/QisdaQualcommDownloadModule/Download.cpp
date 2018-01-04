#include "StdAfx.h"
#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h>
#include "..\..\CommonUtil\unicodeutility.h"
#include "..\..\CommonUtil\PIMStringUtility.h"
#include "..\..\odmtma_api\DownloadDLL\HexFile.h"
#include "..\..\Components\MobileFinder\MobileFinderInterface.h"
#include "Download.h"

CString GetDownloadType(BYTE nType)
{
	//Image_type_PRIBL		= 0x01,	/* Primary boot loader */
	//	Image_type_QCSECBLHD	= 0x02,	/* Qc Secondary boot loader header and config data */
	//	Image_type_QCSECBL		= 0x03,	/* Qc secondary boot loader */
	//	Image_type_OEMSECBL		= 0x04,	/* Oem secondary boot loader and need Oem header */
	//	Image_type_AMSSMODEL	= 0x05,	/* AMSS modem executable and need AMSS modem header */
	//	Image_type_AMSSAPP		= 0x06,	/* AMSS applications executable and need AMSS applications header */
	//	Image_type_6250OTPBL	= 0x07, /* MSM6250 OTP boot loader */
	//	Image_type_FOTAUI		= 0x08,	/* FOTA UI binary */ 
	//	Image_type_CEFS			= 0x09,	/* Compact EFS2 image */
	//	Image_type_AMSSAPPBL	= 0x0A,	/* AMSS applications boot loader and need AMSS applications boot loader header */
	//	Image_type_Flash		= 0x0C, /* Flash.Bin */
	switch(nType)
	{
	case Image_type_PRIBL:
		return _T("Image_type_PRIBL");
		break;
	case Image_type_QCSECBLHD:
		return _T("QCSECBLHD");
		break;
	case Image_type_QCSECBL:
		return _T("QCSECBL");
		break;
	case Image_type_OEMSECBL:
		return _T("OEMSECBL");
		break;
	case Image_type_AMSSMODEL:
		return _T("AMSSMODEL");
		break;
	case Image_type_AMSSAPP:
		return _T("AMSSAPP");
		break;
	case Image_type_6250OTPBL:
		return _T("6250OTPBL");
		break;
	case Image_type_FOTAUI:
		return _T("FOTAUI");
		break;
	case Image_type_CEFS:
		return _T("CEFS");
		break;
	case Image_type_AMSSAPPBL:
		return _T("AMSSAPPBL");
		break;
	case Image_type_Flash:
		return _T("Flash");
		break;
	default:
		return _T("Unknown");
		break;
	}

	return _T("Unknown");
}

CDLCommand::CDLCommand(int nCOMPort, const std::string& strPlatform):
	CInterfaceBase(nCOMPort, strPlatform), m_nProgress(0), CLog(nCOMPort), CLastError(nCOMPort)
{
	DefineNotify(EVENT_DL_PROGRESS);
    DefineNotify(EVENT_DL_PARTITION_DIF);
}

CDLCommand::~CDLCommand(void)
{}

//************************************
// Method:    SetLastError
// Returns:   void
// Parameter: IN DL_ERROR nError
// Parameter: IN bool bExecuteResult: if execution fails, we need to set error code.
//                                    (bExecuteResult==false,then set ErrorCode) 
//************************************
void CDLCommand::SetLastError(DL_ERROR nError, bool bExecuteResult)
{
	if(!bExecuteResult)
	{
		SetCFCLastError(nError);
	}
}


bool CDLCommand::Register(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pNotify, strEvent);
}
bool CDLCommand::Register(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Register(pEventFunc, strEvent);
}
bool CDLCommand::Unregister(QSYNC_LIB::INotify_ANSI* pNotify, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pNotify, strEvent);
}
bool CDLCommand::Unregister(QSYNC_LIB::EventFunc_ANSI* pEventFunc, const std::string& strEvent) 
{
	return QSYNC_LIB::CNotifyBase_ANSI::Unregister(pEventFunc, strEvent);
}
void CDLCommand::Fire(const std::string& strEvent,long nParam)
{
	return QSYNC_LIB::CNotifyBase_ANSI::Fire(strEvent, nParam);
}
void CDLCommand::DefineNotify(const std::string& strNewEvent)
{
	return QSYNC_LIB::CNotifyBase_ANSI::DefineNotify(strNewEvent);
}


bool ByteToChar(BYTE *pucHexBcdBuf, int iHexBcdBufSize, CStringA& strBuffer)
{
	bool bRes = false;
	if (pucHexBcdBuf && (iHexBcdBufSize > 0))
	{
		for(int i=0; i< iHexBcdBufSize; i++)
		{
			CStringA strTemp;
			strTemp.Format("%02X ",pucHexBcdBuf[i]);
			strBuffer+=strTemp;
			if(i!=0 && (i+1)%18 ==0 )
				strBuffer += "\r\n";
		}
		bRes = true;
	}
	return bRes;
}

//************************************
// Method:    AddMsg
// Parameter: IN bool bResult : Command execution result
// Parameter: IN const char * szTitle : Command title
// Parameter: IN int nStep : Which step of download
//************************************
void CDLCommand::AddMsg(bool bResult, const char* szTitle, int nStep)
{
	std::string strMsg = szTitle;
	int nProgress = 0;
	strMsg += bResult ? " success" : " fail";
	nProgress = bResult ? 100 : m_nProgress;
	AddMsg(strMsg.c_str(), nStep, nProgress);

	//Add DL Error Log
#ifndef DISABLE_DL_LOG
	if(!bResult && nStep!=0)
	{
		CString cstrProgress;
		cstrProgress.Format("  %d%%", m_nProgress);
#ifndef _UNICODE
		cstrProgress = CString(szTitle) + cstrProgress;
		AddDLErrorLog((std::string)cstrProgress);

		// Print out fail packet
		CBufferPtr ptData = new CBuffer;
		CStringA cstraData;
		if(GetSendData(ptData) && ByteToChar((BYTE*)ptData->GetData(), ptData->GetLength(), cstraData))
		{  // print SendData
			AddDLErrorLog("SendData:");
			AddDLErrorLog((std::string)cstraData);
		}
		ptData->SetEmpty();
		cstraData = _T("");
		if(GetReceiveData(ptData) &&  ByteToChar((BYTE*)ptData->GetData(), ptData->GetLength(), cstraData))
		{   // print ReceiveData
			AddDLErrorLog("ReceiveData:"); 
			AddDLErrorLog((std::string)cstraData);
		}

#else
		std::wstring wstrMsg = ansitowide(szTitle);
		cstrProgress = CString(wstrMsg.c_str()) + cstrProgress;
		AddDLErrorLog((std::wstring)cstrProgress);
#endif
	}
#endif
}
//************************************
// Method:    AddMsg
// Parameter: IN BYTE nType : notice which Image would be Open/Write/Close
// Parameter: IN int nStep : Which step of download
//************************************
void CDLCommand::AddMsg(BYTE nType, int nStep)
{
	std::string strMsg = "********** [Image] ";
	if(nType == OPEN_MULTI_MODE_QCSBLHDCFG)
		strMsg += "QCSBLHD";
	else if(nType == OPEN_MULTI_MODE_QCSBL)
		strMsg += "QCSBL";
	else if(nType == OPEN_MULTI_MODE_OEMSBL)
		strMsg += "OEMSBLHD/OEMSBL";
	else if(nType == OPEN_MULTI_MODE_AMSS)
		strMsg += "(AMSSHD)/AMSS";
	else if(nType == OPEN_MULTI_MODE_CEFS)
		strMsg += "CEFS";
	else if(nType == OPEN_MULTI_MODE_DBL)
		strMsg += "DBL";
	else if(nType == OPEN_MULTI_MODE_OSBL)
		strMsg += "OSBL";
	else if(nType == OPEN_MULTI_MODE_FSBL)
		strMsg += "FSBL";
	strMsg += " **********";
	AddMsg(strMsg.c_str(), None, 0);
}

//************************************
// Method:    AddMsg
// Parameter: IN const char * szMsg : message
// Parameter: IN int nStep : Which step of download
// Parameter: IN int nProgress : download nProgress% 
//************************************
void CDLCommand::AddMsg(const char* szMsg, int nStep, int nProgress)
{
    if(!(m_nProgress == nProgress && m_strMsg == szMsg))
	{
		m_nProgress = nProgress;
		m_strMsg = szMsg;
		DL_PROGRESS Msg = {(char*)szMsg, (DownloadStep)nStep, nProgress};
		Fire(EVENT_DL_PROGRESS, (long)&Msg);
	}
}
bool CDLCommand::DMSSNop(int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSNop", DMSSProtocol, 0);
	bool bNone, bDiag;
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		bNone = false; bDiag = false;
		CRequestPtr ptRequest = new CDMSSNop;
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		if(!bRes)
		{
			if(ptRequest->GetPotocolType() == NONE)
			{
				AddMsg(bRes, "Connection", Connection);
				bNone = true;
			}
			else if(ptRequest->GetPotocolType() == DIAG)
			{
				AddMsg(bRes, "Change Download Mode", ChangeDLMode);
				bDiag = true;
			}
		}
	}
	AddMsg(bRes, "DMSSNop", DMSSProtocol);
	AddDLLog(bRes, "Nop", _T("DownloadProcess"));
	
	if(bNone)
		SetLastError(DL_ERROR_TX_ERROR, bRes);
	else if(bDiag)
		SetLastError(DL_ERROR_ChangeDLMode, bRes);
	else
		SetLastError(DL_ERROR_DMSS_NOP, bRes);
	return bRes;
}
bool CDLCommand::DMSSParameter(int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSParameter", DMSSProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CDMSSParameter;
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "DMSSParameter", DMSSProtocol);
	AddDLLog(bRes, "DMSSParameter", _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_PARAMETER, bRes);
	return bRes;
}
bool CDLCommand::DMSSWrite32Bit(BYTE* lpWData, WORD dwWLength, DWORD dwAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpWData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CDMSSWrite32Bit(lpWData, dwWLength, dwAddress);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}
	return bRes;
}
bool CDLCommand::DMSSGo(WORD nwSegment,WORD nwoffset, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSGo", DMSSProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CDMSSGo(nwSegment, nwoffset);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "DMSSGo", DMSSProtocol);
	AddDLLog(bRes, "DMSSGo", _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_GO, bRes);
	return bRes;
}

bool CDLCommand::StreamingHello(bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingHello", StreamingProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLHello(bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "StreamingHello", StreamingProtocol);
	AddDLLog(bRes, "StreamingHello", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Hello, bRes);
	return bRes;
}

bool CDLCommand::StreamingHello_CheckBadBlock(bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingHello_CheckBadBlock", StreamingProtocol, 0);
	if(bSpeedUp)
	{
		bool bGetFirstResponse = false;
		bool bIsSupported = true; // timeout means supports, too.
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLHello_CheckBadBlock(bIsSupported);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer) && !(ptRequest->WaitForOtherResponse());
			bGetFirstResponse = !(ptRequest->WaitForOtherResponse());
			if(!bIsSupported &&/* for testing if timeout. If timeout, needs re-try, or log will be wrong: */ 
				GetReceiveData(ptBuffer) && ptBuffer->GetLength()!=0 )
				break;
		}
		if (bGetFirstResponse == false && bIsSupported)
			AddDLErrorLog(_T("Get CheckBadBlock2ndResponse fail."));
		if(!bIsSupported)
			AddMsg("Not Support Streaminghello_CheckBadBlock command", None, 0);
	}
	else
		AddMsg("Not Support non-speedup Streaminghello_CheckBadBlock command", None, 0);

	AddMsg(bRes, "StreamingHello_CheckBadBlock", StreamingProtocol);
	AddDLLog(bRes, "StreamingHello_CheckBadBlock", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Hello_CheckBadBlock, bRes);
	return bRes;
}

bool CDLCommand::StreamingSecurity(const std::string& strPBL, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingSecurity", StreamingProtocol, 0);
	BYTE nSecurity = (strPBL == "") ? 0x01 : 0x00 ;
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLSecurity(nSecurity, bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "StreamingSecurity", StreamingProtocol);
	AddDLLog(bRes, "StreamingSecurity", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Security, bRes);
	return bRes;
}
//************************************
// Method:    StreamingStrWrite: only supported in Speedup DL format.
// Returns:   bool: execute successful
// Parameter: IN BYTE * lpData: data for streaming write
// Parameter: IN WORD wnLength: lpData's length
// Parameter: IN DWORD dwAddress: which address you want to write in
// Parameter: IN/OUT Struct_XOR & nXOR: input(nXOR) will XOR with the next packet's CheckXOR and become output(nXOR)
// Parameter: IN bool bSpeedUp: if not in Speedup format, return false.
// Parameter: IN int nRetryTimes
// Parameter: IN int nTimeout
//************************************
bool CDLCommand::StreamingStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, Struct_XOR& nXOR, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	if(bSpeedUp && lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			Struct_XOR nTemp = nXOR;
			CRequestPtr ptRequest = new CStreamingDLStrWriteWithXOR(lpData, wnLength, dwAddress, nTemp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
			if(bRes)
			{
				nXOR = nTemp;
			}
		}
		
	}
	return bRes;
}
//************************************
// Method:    StreamingStrWrite512: only supported in Speedup DL format.
// Purpose: For writing dump image 
// Returns:   bool: execute successful
// Parameter: IN BYTE * lpData: data for streaming write
// Parameter: IN WORD wnLength: lpData's length
// Parameter: IN DWORD dwAddress: which address you want to write in
// Parameter: IN/OUT Struct_XOR & nXOR: input(nXOR) will XOR with the next packet's CheckXOR and become output(nXOR)
// Parameter: IN bool bSpeedUp: if not in Speedup format, return false.
// Parameter: IN int nRetryTimes
// Parameter: IN int nTimeout
//************************************
bool CDLCommand::StreamingStrWrite512(BYTE* lpData, WORD wnLength, DWORD dwAddress, Struct_XOR& nXOR, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(bSpeedUp && lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			Struct_XOR nTemp = nXOR;
			CRequestPtr ptRequest = new CStreamingDLStrWriteWithXOR(lpData, wnLength, dwAddress, nXOR);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
			if(bRes)
			{
				nXOR = nTemp;
			}
		}
	}
	return bRes;
}

//************************************
// Method:    WriteDumpImage512 - always in Speedup DL
// Parameter: IN BYTE * lpData: data for streaming write
// Parameter: IN WORD wnLength: lpData's length
// Parameter: IN DWORD dwAddress: which address you want to write in
// Parameter: IN int nRetryTimes
// Parameter: IN int nTimeout
//************************************
bool CDLCommand::WriteDumpImage512(BYTE* lpData, WORD wnLength, DWORD dwAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CWriteDumpImage512(lpData, wnLength, dwAddress);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}
	return bRes;
}

//************************************
// Method:    WriteDumpImage(Normal) - always in Speedup DL
// Parameter: IN BYTE * lpData: data for streaming write
// Parameter: IN WORD wnLength: lpData's length
// Parameter: IN DWORD dwAddress: which address you want to write in
// Parameter: IN int nRetryTimes
// Parameter: IN int nTimeout
//************************************
bool CDLCommand::WriteDumpImage(BYTE* lpData, WORD wnLength, DWORD dwAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CWriteDumpImage(lpData, wnLength, dwAddress);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}
	return bRes;
}

bool CDLCommand::StreamingPartition(BYTE* lpbyData, WORD wnLength/*max=512*/, bool bOverride, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	AddMsg("StreamingPartition", StreamingProtocol, 0);
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	bool bPartitionDiffer = false;
	if(lpbyData && wnLength < 513)
	{
		BYTE nRespStatus = 0;
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			nRespStatus = 0;
			CRequestPtr ptRequest = new CStreamingDLPartition(lpbyData, wnLength, bOverride, nRespStatus, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
		if(bRes)
		{
			bRes = nRespStatus == 0; /*0 = Partition table accepted*/ 
			bPartitionDiffer = nRespStatus == 0x01;
            if(bPartitionDiffer) {
				AddMsg("Partition Table is Different", StreamingProtocol, 0);
                Fire(EVENT_DL_PARTITION_DIF, 0);
            }
		}
	}
	AddMsg(bRes, "StreamingPartition", StreamingProtocol);
	AddDLLog(bRes, "StreamingPartition", _T("DownloadProcess"));
	//SetLastError(bPartitionDiffer ? DL_ERROR_DIFFERENT_PARTITION : DL_ERROR_Streaming_Partition, bRes);
	return bRes;
}

bool CDLCommand::StreamingOpenMultiImage(BYTE nType, BYTE* lpData, WORD wnLength/*max=512*/, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg(nType, StreamingProtocol);
	if(wnLength < 513)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLOpenMultiImage(nType, lpData, wnLength, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}
	AddMsg(bRes, "StreamingOpenMultiImage", StreamingProtocol);
	AddDLLog(bRes, GetDownloadType(nType) + _T(" open"), _T("DownloadProcess"));
	return bRes;
}
bool CDLCommand::StreamingStrWrite(BYTE* lpData, WORD wnLength, DWORD dwAddress, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(lpData)
	{
		for(int i=0; i< nRetryTimes && !bRes; i++)
		{
			CRequestPtr ptRequest = new CStreamingDLStrWrite(lpData, wnLength, dwAddress, bSpeedUp);
			CBufferPtr ptBuffer = new CBuffer;
			bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		}
	}

	return bRes;
}

//************************************
// Method:    QueryImageSize
// Returns:   bool: return false in response is not in 0x97 or 0x0D[Error]
// Parameter: OUT int& nSize: Image size on Handset side
// Parameter: OUT BYTE lpXOR[4]: XOR result on Handset side
// Parameter: OUT bool& bSupported: if response.cmdcode returns 0x0D, means hex file doesn't support query command
// Parameter: IN int nTimeout
//************************************
bool CDLCommand::QueryImageSize(int& nSize, BYTE lpXOR[4], bool& bSupported, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	QuerySizeResp resp={0};
	CRequestPtr ptRequest = new CQueryImageSizeRequest(resp, bSupported);
	CBufferPtr ptBuffer = new CBuffer;
	if(Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer))
	{
		if(bSupported)
		{
			memcpy(&nSize, resp.nSize, 4);
			memcpy(lpXOR, resp.nXOR, 4);	
		}
		bRes = true;
	}	
	return bRes;
}


bool CDLCommand::StreamingClose(bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingClose", StreamingProtocol, 0);
	AddDLLog("Close Image start.", _T("DownloadProcess"));
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLCloseImage(bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
		AddDLLog(bRes, "Close Image receiver get", _T("DownloadProcess"));
		CStringA strBuffer;
		if (ByteToChar((BYTE*)ptBuffer->GetData(), ptBuffer->GetLength(), strBuffer))
			AddDLLog(strBuffer, _T("DownloadProcess"));
	}

	AddMsg(bRes, "StreamingClose", StreamingProtocol);
	AddDLLog(bRes, "Close Image end", _T("DownloadProcess"));
	return bRes;
}

bool CDLCommand::StreamingReset(bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingReset", StreamingProtocol, 0);
	for(int i=0; i< nRetryTimes && !bRes; i++)
	{
		CRequestPtr ptRequest = new CStreamingDLReset(bSpeedUp);
		CBufferPtr ptBuffer = new CBuffer;
		bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	}
	AddMsg(bRes, "StreamingReset", StreamingProtocol);
	AddDLLog(bRes, "Reset", _T("DownloadProcess"));
	return bRes;
}
/*************************************************************************/
CDLCompoundCommand::CDLCompoundCommand(int nCOMPort, const std::string& strPlatform):CDLCommand(nCOMPort, strPlatform)
{}
CDLCompoundCommand::~CDLCompoundCommand(){}

//************************************
// Method:    DMSSOpenAndWriteHexFile
// IN Parameter: const std::string & strHexFilePath
// OUT Parameter: WORD & dwAddress : output for DMSS Command Go
// OPTIONAL Parameter: int nTimeout
//************************************
bool CDLCompoundCommand::DMSSOpenAndWriteHexFile(const std::string& strHexFilePath, WORD& wAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 0);
	if(::_taccess(strHexFilePath.c_str(), 0) == 0)
	{
		CHexFile HexFile;
		CFileException pError;
		if(TRUE == HexFile.Open(strHexFilePath.c_str(),CFile::modeRead | CFile::shareDenyNone,&pError))
		{
			int nTotalSize = HexFile.GetLength()*16/45;  //rough WriteSize
			
			wAddress = HexFile.GetHexAddrss();//Get the first address that Hex File Setting
			DWORD dwTempAddress = wAddress;
			dwTempAddress <<= 16;
			DWORD dwStartAddress = dwTempAddress;
			WORD dwSize = 0;
			do {
				BYTE nTemp[1024] = {0};
				dwSize = HexFile.Read((char*)nTemp, 0X03F9); // 0x03F9 = 1017 
				bRes = DMSSWrite32Bit(nTemp, dwSize, dwTempAddress, nRetryTimes, nTimeout);
				dwTempAddress += dwSize;
				Sleep(0); // for multi-download
				AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 100*(dwTempAddress-dwStartAddress)/nTotalSize); 
			}while( dwSize == 0X03F9 && bRes );
			HexFile.Close();
		}
	}
	AddMsg(bRes, "DMSSOpenAndWriteHexFile", DMSSProtocol);
	AddDLLog(bRes, _T("OpenAndWriteHexFile"), _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_Hex, bRes);
	return bRes;
}

bool CDLCompoundCommand::DMSSOpenAndWriteHexFile(const BYTE* pData, int nTotalSize, WORD& wAddress, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 0);
	int nLeftDataSize = nTotalSize;
	BYTE* pHex = (BYTE*)pData;
	if(pData && nTotalSize > 15)
	{
		CHexDataParser HexParser((BYTE**)(&pHex), nLeftDataSize, wAddress);
		DWORD dwTempAddress = HexParser.GetHexAddrss();
		WORD dwReadSize = 0;
		do 
		{
			BYTE nTemp[1024] = {0};
			bool bParsed = false;
			dwReadSize = HexParser.Read((char*)nTemp, 0X03F9, bParsed); // 0x03F9 = 1017 
			bRes = bParsed ? DMSSWrite32Bit(nTemp, dwReadSize, dwTempAddress, nRetryTimes, nTimeout): false;
			dwTempAddress += dwReadSize;
			Sleep(0); // for multi-download
			AddMsg("DMSSOpenAndWriteHexFile", DMSSProtocol, 100*(dwTempAddress-HexParser.GetHexAddrss())/nTotalSize); 
		} while(dwReadSize == 0X03F9 && bRes);
	}
	AddMsg(bRes, "DMSSOpenAndWriteHexFile", DMSSProtocol);
	AddDLLog(bRes, _T("OpenAndWriteHexFile"), _T("DownloadProcess"));
	SetLastError(DL_ERROR_DMSS_Hex, bRes);
	return bRes;
}
//************************************
// Method:    StreamingOpenAndWritePartition
// IN Parameter: const std::string & strPartition
// IN Parameter: bool bOverride : Usual setting should set false. 
//    Set true only when the partition is different from the unit's.
// OPTIONAL Parameter: bool bSpeedUp
// OPTIONAL Parameter: int nTimeout
//************************************
bool CDLCompoundCommand::StreamingOpenAndWritePartition(const std::string& strPartition, bool bOverride, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(::_taccess(strPartition.c_str(), 0) == 0)
	{
		CFile file;
		CFileException pError;
		if(TRUE == file.Open(strPartition.c_str(),CFile::modeRead | CFile::shareDenyNone,&pError))
		{
			UINT unsize = (UINT)file.GetLength();
			BYTE *lpData = new BYTE[unsize+1];
			file.Read(lpData,unsize);
			bRes = StreamingPartition(lpData, unsize, bOverride, bSpeedUp, nRetryTimes, nTimeout);
			file.Close();
			delete [] lpData;
			lpData = NULL;
		}
	}
	return bRes;
}

//************************************
// Method:    StreamingOpenHeaderImage
// IN Parameter: BYTE nType 
// IN Parameter: const std::string & strHeaderImagePath
// OPTIONAL Parameter: bool bSpeedUp
// OPTIONAL Parameter: int nTimeout
// Comment: In Qualcomm's design, only OEMSBL/AMSS needs Header Image(OEMSBLHD/AMSSHD).
//          Others only use OpenMultiImage Command.
//************************************
bool CDLCompoundCommand::StreamingOpenHeaderImage(BYTE nType, const std::string& strHeaderImagePath/*MaxImageSize=512*/, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	BYTE* lpData = NULL;
	UINT wnLength = 0;
	if(nType==Image_type_OEMSECBL || nType==Image_type_AMSSMODEL || nType==Image_type_AMSSAPP)
	{ 
		CFile cfheader;
		CFileException pError;
		if(cfheader.Open(strHeaderImagePath.c_str(),CFile::shareDenyNone,&pError) == TRUE)
		{
			wnLength = (UINT)cfheader.GetLength();
			lpData = new BYTE[wnLength+1];
			cfheader.Read(lpData,wnLength);
			cfheader.Close();
			bRes = true;
		}
	}
	else 
		bRes = true;
	if(bRes)
		bRes = StreamingOpenMultiImage(nType, lpData, wnLength, bSpeedUp, nRetryTimes, nTimeout);
	if(lpData)
	{
		delete [] lpData;
		lpData = NULL;
	}
	return bRes;
}


bool CDLCompoundCommand::StreamingOpenHeaderImage(BYTE nType, const BYTE* pImageData, int nImageSize/*MaxImageSize=512*/, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	
	BYTE* pImage = (BYTE*)pImageData;
	if(nType==Image_type_OEMSECBL || nType==Image_type_AMSSMODEL || nType==Image_type_AMSSAPP)
	{ 
		if(pImage && nImageSize>0 && nImageSize<513)
			bRes = true;
	}
	else 
	{
		pImage = NULL;
		nImageSize = 0;
		bRes = true;
	}
	if(bRes)
		bRes = StreamingOpenMultiImage(nType, pImage, nImageSize, bSpeedUp, nRetryTimes, nTimeout);

	return bRes;
}


bool CDLCompoundCommand::StreamingWriteMultiImage(const std::string& strImagePath, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingWriteMultiImage", StreamingProtocol, 0);
	CFile cfImage;
	CFileException pError;
	if(cfImage.Open(strImagePath.c_str(),CFile::shareDenyNone | CFile::modeRead,&pError) == TRUE)
	{
		int nImageSize = cfImage.GetLength();
		Struct_XOR XOR={0};
		int nProgressCount = 0;
		if(nImageSize > 0)
		{
			DWORD dwAddress = 0;
			WORD wReadSize = 0;
			do {
				BYTE nTemp[PAGE_SIZE+32] = {0};
				wReadSize = cfImage.Read(nTemp,PAGE_SIZE);
				ASSERT(wReadSize == PAGE_SIZE);
				if(bSpeedUp)
					bRes = StreamingStrWrite(nTemp, wReadSize, dwAddress, XOR, bSpeedUp, nRetryTimes, nTimeout);	 // speed up download always default check image size and integrity	
				else
					bRes = StreamingStrWrite(nTemp, wReadSize, dwAddress, bSpeedUp, nRetryTimes, nTimeout);
				dwAddress += wReadSize;
				Sleep(0); // for multi-download
				
				nProgressCount++;
				AddMsg("StreamingWriteMultiImage", StreamingProtocol, 100*nProgressCount/(nImageSize/PAGE_SIZE+1));
			}while( wReadSize == PAGE_SIZE && bRes );
		}
		
		if(bRes && bSpeedUp)
			bRes = CompareImageSize(bSpeedUp, nImageSize, XOR.nXOR, nTimeout);

		cfImage.Close();
	}
	AddMsg(bRes, "StreamingWriteMultiImage", StreamingProtocol);
	AddDLLog(bRes, "WriteMultiImage", _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Image, bRes);
	return bRes;
}

bool CDLCompoundCommand::StreamingWriteMultiImage(const BYTE* pImageData, int nImageSize, bool bSpeedUp, int nRetryTimes, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	AddMsg("StreamingWriteMultiImage", StreamingProtocol, 0);
	if(pImageData && nImageSize >0)
	{
		Struct_XOR XOR={0};
		int nLoopCount = nImageSize/PAGE_SIZE +1;
		bRes = true;
		DWORD dwAddress = 0;
		for(int i=0; i < nLoopCount && bRes; i++)
		{
			WORD wReadSize = (i != nLoopCount-1) ? PAGE_SIZE : (nImageSize-(nLoopCount-1)*PAGE_SIZE);
			if(bSpeedUp)
				bRes = StreamingStrWrite((BYTE*)pImageData+dwAddress, wReadSize, dwAddress, XOR, bSpeedUp, nRetryTimes, nTimeout);	 // speed up download always default check image size and integrity	
			else
				bRes = StreamingStrWrite((BYTE*)pImageData+dwAddress, wReadSize, dwAddress, bSpeedUp, nRetryTimes, nTimeout);
			dwAddress += wReadSize;
			Sleep(0); // for multi-download
			AddMsg("StreamingWriteMultiImage", StreamingProtocol, 100*i/nLoopCount);
		}
		
		if(bRes && bSpeedUp)
			bRes = CompareImageSize(bSpeedUp, nImageSize, XOR.nXOR, nTimeout);
	}
	AddMsg(bRes, "StreamingWriteMultiImage", StreamingProtocol);
	AddDLLog(bRes, _T("WriteMultiImage"), _T("DownloadProcess"));
	SetLastError(DL_ERROR_Streaming_Image, bRes);
	return bRes;
}



//************************************
// Method:    CompareImageSize
// Returns:   bool: return false in 1. not in speedup format
//									2. QueryImageSize fail 
//									3. compare inconsistent
// Parameter: IN bool bSpeedUp: if bSpeedUp==false, CompareImageSize fail
// Parameter: IN int nSize: which is image size on PC side
// Parameter: IN BYTE lpXOR[4]: which is XOR result on PC side
// Parameter: IN int nTimeout
//************************************
bool CDLCompoundCommand::CompareImageSize(bool bSpeedUp, int nSize, BYTE lpXOR[4], int nTimeout)
{
	bool bRes = false;

	if(bSpeedUp)
	{
		int nHandsetSize = 0;
		BYTE lpHandsetXOR[4] = {0};
		bool bSupported = false;
		if(QueryImageSize(nHandsetSize, lpHandsetXOR, bSupported, nTimeout))
		{
			if(bSupported)
			{
				bRes = nSize == nHandsetSize && memcmp(lpXOR, lpHandsetXOR, 4)==0;
				CStringA strPCXOR, strHandsetXOR;
				QSYNC_LIB::ByteToChar(lpXOR, 4, strPCXOR, true);
				QSYNC_LIB::ByteToChar(lpHandsetXOR, 4, strHandsetXOR, true);
				char szTemp[100] = {0};
				sprintf(szTemp, "PC send: Size:%d, XOR: %s, response: Size:%d, XOR:%s", nSize, strPCXOR, nHandsetSize, strHandsetXOR);
				AddMsg(szTemp, None, 0);
			}
			else
			{
				AddMsg("Not Support to Compare Image", None, 0);
				bRes = true; // not support QueryImageSize, always pass true
			}
		}
	}
	AddMsg(bRes, "CompareImageSize", StreamingProtocol);
	AddDLLog(bRes, _T("CompareImageSize"), _T("DownloadProcess"));
	return bRes;
}

bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const std::string& strHeaderImagePath, const std::string& strImagePath, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(::_taccess(strImagePath.c_str(), 0) == 0)
	{
		if(StreamingOpenHeaderImage(nImageType, strHeaderImagePath, bSpeedUp, nRetryTimes, nOpenTimeout) &&
		   StreamingWriteMultiImage(strImagePath, bSpeedUp, nRetryTimes, nWriteTimeout) && 
		   StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure
	return bRes;
}

bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const BYTE* pHeaderData, int nHeaderSize, const BYTE* pImageData, int nImageSize, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(pImageData && nImageSize>0)
	{
		if(StreamingOpenHeaderImage(nImageType, pHeaderData, nHeaderSize, bSpeedUp, nRetryTimes, nOpenTimeout) &&
			StreamingWriteMultiImage(pImageData, nImageSize, bSpeedUp, nRetryTimes, nWriteTimeout) && 
			StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure
	return bRes;
}


bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const std::string& strImagePath, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(::_taccess(strImagePath.c_str(), 0) == 0)
	{
		if(StreamingOpenMultiImage(nImageType, NULL, 0, bSpeedUp, nRetryTimes, nOpenTimeout) &&
			StreamingWriteMultiImage(strImagePath, bSpeedUp, nRetryTimes, nWriteTimeout) && 
			StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure

	return bRes;
}

bool CDLCompoundCommand::StreamingDLMultiImage(BYTE nImageType, const BYTE* pImageData, int nImageSize, bool bSpeedUp, int nRetryTimes, int nOpenTimeout, int nWriteTimeout, int nCloseTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(pImageData && nImageSize>0)
	{
		if(StreamingOpenMultiImage(nImageType, NULL, 0, bSpeedUp, nRetryTimes, nOpenTimeout) &&
			StreamingWriteMultiImage(pImageData, nImageSize, bSpeedUp, nRetryTimes, nWriteTimeout) && 
			StreamingClose(bSpeedUp, nRetryTimes, nCloseTimeout))
		{
			bRes = true;
		}
	}
	else
		bRes = true; // if image is empty, we don't download and pass success for the whole procedure
	
	return bRes;
}

/*************************************************************************/
CDLFormatCommand::CDLFormatCommand(int nCOMPort, const std::string& strPlatform)
:CDLCompoundCommand(nCOMPort, strPlatform)
{
}
CDLFormatCommand::~CDLFormatCommand()
{
}

bool CDLFormatCommand::GetDLBootLoaderRevocation(int& nStatus,int& nVersion)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CDLBootLoaderRevacationRequest(nStatus,nVersion);	
	bRes = Execute(ptRequest);

	return bRes;
}

bool CDLFormatCommand::GetDLPhoneCodeRevocation(int& nStatus,int& nVersion)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;

	CRequestPtr ptRequest = new CDLPhoneCodeRevacationRequest(nStatus,nVersion);	
	bRes = Execute(ptRequest);
	return bRes;
}

bool CDLFormatCommand::RAWRequest(const char* szCmdInput, int nCmdLength, bool bAddCRC)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CRequestPtr ptRequest = new CDLRAWRequest(szCmdInput, nCmdLength, bAddCRC);	
	bRes = Execute(ptRequest);
	return bRes;
}


bool CDLFormatCommand::CheckBadBlock(std::string& strBadBlockInfo, int nOption, int nTimeout)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	CRequestPtr ptRequest = new CDLCheckBadBlockRequest(nOption);
	ptRequest->SetTimeout(nTimeout);
	CBufferPtr ptBuffer = new CBuffer;
	bRes = Execute(ptRequest) && ptRequest->GetReceiveData(ptBuffer);
	if(bRes && ptBuffer->GetLength() !=0)
	{
		CStringA cstrBuffer;
		ByteToChar((BYTE*)ptBuffer->GetData(), ptBuffer->GetLength(), cstrBuffer);
		strBadBlockInfo = QSYNC_LIB::GetTSTRING((std::string)cstrBuffer);
	}
	return bRes;
}

bool CDLFormatCommand::CheckBadBlock(const char* szCmdInput, int nCmdLength, int nTimeout, CallBackInfo CallBack)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	CRequestPtr ptRequest = new CDLRepeatBadBlockTestRequest((BYTE*)szCmdInput, nCmdLength, CallBack);
	bRes = Execute(ptRequest, nTimeout);
	return bRes;
}

bool CDLFormatCommand::CheckBadBlockWithHandShaking(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout,CallBackInfo AddMsg)
{
	using namespace PLATFORM_TOOL;
	bStopAccepting = true;
	bool bRes = false;
	CRequestPtr ptRequest = new CDLRawWithReturnRequest(szData, nDataSize, true);
	CBufferPtr ptBuffer = new CBuffer;
	bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	if(bRes && ptBuffer)
	{
		BYTE* lpData = (BYTE*)(ptBuffer->GetData());
		if(lpData)
		{
			if(lpData[0] == 0x02) // End
			{
				std::string strMsg = "End: ";
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;
				AddMsg(strMsg+" times");
			}
			else if(lpData[0] == 0x00) // Success
			{
				bStopAccepting = false;
				std::string strMsg = "Success: ";
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;

				AddMsg(strMsg+" times");
			}
			else if(lpData[0] == 0x01) // Fail
			{
				bStopAccepting = false;
				std::string strMsg = "Fail:";
				int nTimes = 0; 
				memcpy(&nTimes, lpData+1, 4);
				TCHAR szTimes[100] = {0};
				_itot_s(nTimes, szTimes, 10);
				strMsg += szTimes;
				AddMsg(strMsg+" times");
			}
			else
			{
				bStopAccepting = false;
				AddMsg(_T("Unknown Response(without 7E and CRC):"));
				CStringA strData;
				if(ByteToChar((BYTE*)ptBuffer->GetData(), ptBuffer->GetLength(), strData))
				{	
	#ifdef _UNICODE
					std::string strTemp = strData;
					AddMsg(QSYNC_LIB::GetTSTRING(strTemp));
	#else
					AddMsg((std::string)strData);
	#endif

				}

			}
		}
		
		
	}
	return bRes;
}
bool CDLFormatCommand::DumpImage(bool& bStopAccepting, const char* szData, int nDataSize,int nTimeout, CBufferPtr ptImage)
{
	using namespace PLATFORM_TOOL;
	
	bool bRes = false;
	CBufferPtr ptBuffer = new CBuffer;
	CRequestPtr ptRequest = new CDLRawWithReturnRequest(szData, nDataSize, true);
	bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	if(bRes && ptBuffer)
	{
		BYTE* lpData = (BYTE*)(ptBuffer->GetData());
		if(lpData)
		{
			if(lpData[0] == 0x02) // End
			{
				bStopAccepting = true;
			}
			else if(lpData[0] == 0x03)
			{
				bStopAccepting = false;
				CBufferPtr ptData = new CBuffer(ptBuffer->GetLength()-1,(const char*)(lpData+1));
				ptImage->Append(ptData);
			}
			else
			{
				bStopAccepting = true;
				bRes = false;
			}
		}
	}
	else
		bStopAccepting = true;
	return bRes;
}

bool CDLFormatCommand::DLGetHWVariant(int& nHWID, int& nVariantID, int nTimeout) // ONLY support Lavernock
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	CBufferPtr ptBuffer = new CBuffer;
	CRequestPtr ptRequest = new CDLQueryHWVariant(nHWID, nVariantID);
	bRes = Execute(ptRequest, nTimeout) && ptRequest->GetReceiveData(ptBuffer);
	return bRes;
}
bool CDLFormatCommand::Download(bool bSpeedup, bool bReopenAfterReset) {
    return false;
}
void CDLFormatCommand::SetResetMode(bool bReset) {
    assert(0);
}
bool CDLFormatCommand::SendHello(bool bSpeedUp)
{
	bool bRes = false;
	if(!StreamingHello_CheckBadBlock(bSpeedUp, g_n6245HelloRetryTimes_CheckBadBlock, g_n6245HelloTimeout_CheckBadBlock))
		bRes = StreamingHello(bSpeedUp, g_n6245HelloRetryTimes, g_n6245HelloTimeout);
	else
		bRes = true;
	return bRes;
}
void CDLFormatCommand::SetIsOveridePartition(bool bOverridePartition) {
    assert(0);
}

bool CDLFormatCommand::StreamingDLFromPartition(bool bSpeedup, bool bReopenAfterReset) {
    assert(0);
    return false;
}

/************************************************************************/
/* DL Setting:															*/
/*  CDownload6245, CDownload6240, CShareDownload6245, CShareDownload6240*/
/*  CDownload6270*/
/************************************************************************/
///////////////////////////////6245///////////////////////////////////////////
bool CDownload6245::SetHex(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strHEX = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetPartition(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strPartition = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetPBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strPBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetQCSBLHD(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strQCSBLHD = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetQCSBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strQCSBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetOEMSBLHD(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strOEMSBLHD = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetOEMSBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strOEMSBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetAMSSHD(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAMSSHD = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6245::SetAMSS(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAMSS = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6245::SetCEFS(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strCEFS = strFilePath;
		bRes = true;
	}
	return bRes;
}

//************************************
// Method:    SetResetMode
// Returns:   void
// Qualifier:
// Parameter: bool bReset : bReset == true means will reset mode if flashing fails
//************************************
void CDownload6245::SetResetMode(bool bReset)
{
	m_bResetMode = bReset;
}

void CDownload6245::SetIsOveridePartition(bool bOverridePartition)
{
	if(bOverridePartition)
		AddMsg("Set Override Partition ON", None, 100);
	m_bOverridePartition = bOverridePartition;
}
//////////////////////////////6240////////////////////////////////////////////
bool CDownload6240::SetHex(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strHEX = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetPartition(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strPartition = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetPBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strPBL = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetAMSS(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strAMSS = strFilePath;
		bRes = true;
	}
	return bRes;
}

bool CDownload6240::SetCEFS(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strCEFS = strFilePath;
		bRes = true;
	}
	return bRes;
}

//************************************
// Method:    SetResetMode
// Returns:   void
// Qualifier:
// Parameter: bool bReset : bReset == true means will reset mode if flashing fails
//************************************
void CDownload6240::SetResetMode(bool bReset)
{
	m_bResetMode = bReset;
}

void CDownload6240::SetIsOveridePartition(bool bOverridePartition)
{
	if(bOverridePartition)
		AddMsg("Set Override Partition ON", None, 100);
	m_bOverridePartition = bOverridePartition;
}

bool CDownload6240::SetDBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strDBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetFSBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strFSBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
bool CDownload6240::SetOSBL(const std::string& strFilePath)
{
	bool bRes = false;
	if(::_taccess(strFilePath.c_str(), 0) == 0)
	{
		m_strOSBL = strFilePath;
		bRes = true;
	}
	return bRes;
}
/////////////////////////////Share6245/////////////////////////////////////////////
bool CShareDownload6245::SetHex(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgHEX, pData, nSize);
}
bool CShareDownload6245::SetPartition(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgPartition, pData, nSize);
}
bool CShareDownload6245::SetPBL(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgPBL, pData, nSize);
}

bool CShareDownload6245::SetQCSBLHD(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgQCSBLHD, pData, nSize);
}
bool CShareDownload6245::SetQCSBL(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgQCSBL, pData, nSize);
}
bool CShareDownload6245::SetOEMSBLHD(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgOEMSBLHD, pData, nSize);
}
bool CShareDownload6245::SetOEMSBL(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgOEMSBL, pData, nSize);
}
bool CShareDownload6245::SetAMSSHD(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgAMSSHD, pData, nSize);
}

bool CShareDownload6245::SetAMSS(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgAMSS, pData, nSize);
}
bool CShareDownload6245::SetCEFS(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgCEFS, pData, nSize);
}

//************************************
// Method:    SetResetMode
// Returns:   void
// Qualifier:
// Parameter: bool bReset : bReset == true means will reset mode if flashing fails
//************************************
void CShareDownload6245::SetResetMode(bool bReset)
{
	m_bResetMode = bReset;
}
void CShareDownload6245::SetIsOveridePartition(bool bOverridePartition)
{
	if(bOverridePartition)
		AddMsg("Set Override Partition ON", None, 100);
	m_bOverridePartition = bOverridePartition;
}
/////////////////////////////Share6240/////////////////////////////////////////////
void CShareDownload6240::SetIsOveridePartition(bool bOverridePartition)
{
	if(bOverridePartition)
		AddMsg("Set Override Partition ON", None, 100);
	m_bOverridePartition = bOverridePartition;
}

//************************************
// Method:    SetResetMode
// Returns:   void
// Qualifier:
// Parameter: bool bReset : bReset == true means will reset mode if flashing fails
//************************************
void CShareDownload6240::SetResetMode(bool bReset)
{
	m_bResetMode = bReset;
}
bool CShareDownload6240::SetHex(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgHEX, pData, nSize);
}
bool CShareDownload6240::SetPartition(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgPartition, pData, nSize);
}
bool CShareDownload6240::SetPBL(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgPBL, pData, nSize);
}


bool CShareDownload6240::SetAMSS(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgAMSS, pData, nSize);
}
bool CShareDownload6240::SetCEFS(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgCEFS, pData, nSize);
}

bool CShareDownload6240::SetDBL(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgDBL, pData, nSize);
}
bool CShareDownload6240::SetFSBL(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgFSBL, pData, nSize);
}	  
bool CShareDownload6240::SetOSBL(const BYTE* pData, const int& nSize)
{
    return SetImage(m_imgOSBL, pData, nSize);
}

/*************************************************************************/
CDownload6245::CDownload6245(int nCOMPort)
:CDLFormatCommand(nCOMPort, "Qualcomm6245"), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(true)
{
}

CDownload6245::~CDownload6245(void)
{
}

bool CDownload6245::CFCDownload(bool bSpeedup)
{
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		PopLastLog();
	}
	return bRes;
}


bool CDownload6245::Download(bool bSpeedup, bool bReopenAfterReset)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		AddMsg("Download Fail", None, 0);
		PopLastLog();
	}
	return bRes;
}


bool CDownload6245::DMSSDownload(const std::string& strHexFilePath)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if(DMSSNop(g_n6245NopRetryTimes,g_n6245NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( (strHexFilePath.empty() ? m_strHEX : strHexFilePath), dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
		Sleep(8000);  //buffering hex, needs more time to write.
	}
	return bRes;
}

bool CDownload6245::StreamingDL(bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	if( SendHello(bSpeedUp) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(Image_type_QCSECBLHD, "", m_strQCSBLHD, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout) &&
		StreamingDLMultiImage(Image_type_QCSECBL, "", m_strQCSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout) && 
		StreamingDLMultiImage(Image_type_OEMSECBL, m_strOEMSBLHD, m_strOEMSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout) &&
		StreamingDLMultiImage(Image_type_AMSSMODEL, m_strAMSSHD, m_strAMSS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout) &&
		StreamingDLMultiImage(Image_type_CEFS, "", m_strCEFS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{
		if(m_bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bRes = ReopenAfterReset.ReOepnComport(g_n6245ReOpenAfterResetSleepTime_Path, false, 30, 7000);
			AddMsg(bRes, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bRes);
		}
		else
			bRes = true;
		
	}	
	return bRes;
}

/***************************************************************/

CDownload6240::CDownload6240(int nCOMPort, const std::string& strPlatform)
:CDLFormatCommand(nCOMPort, strPlatform), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(true)
{
}

CDownload6240::~CDownload6240(void)
{
}

bool CDownload6240::CFCDownload(bool bSpeedup)
{
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		PopLastLog();
	}
	return bRes;
}

bool CDownload6240::Download(bool bSpeedup, bool bReopenAfterReset)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode(); 
		AddMsg("Download Fail", None, 0);
		PopLastLog();
	}
	return bRes;
}

bool CDownload6240::DMSSDownload(const std::string& strHexFilePath)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n6240NopRetryTimes,g_n6240NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( (strHexFilePath.empty() ? m_strHEX : strHexFilePath), dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
	}
	return bRes;
}


bool CDownload6240::StreamingDL(bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CReOpenComport ReopenAfterGo(this, true);
	bool bReconnect = ReopenAfterGo.ReOepnComport(g_n6240ReOpenAfterGoSleepTime, false);
	AddMsg(bReconnect, "ReOpenAfterGo", StreamingProtocol);
	ASSERT(bReconnect);

	if( bReconnect && 
		StreamingHello(bSpeedUp, g_n6240HelloRetryTimes, g_n6240HelloTimeout) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL,  m_strDBL,  bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_strFSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_strOSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_strAMSS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_strCEFS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{
		bRes = true;

		if(m_bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bReconnect = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime_Path, false);
			ASSERT(bReconnect);
			AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bReconnect);
		}
		else
			bReconnect = true;
	}

	return bRes && bReconnect;
}
//************************************
// Method:    StreamingDLFromPartition
// Returns:   bool : download success or fail
// Parameter: bool bSpeedUp : it must be the same as previous one
// Purpose: If previous download is failed in different partition table,
//			for overwriting partition table, 
//			you can resend partition with m_bOverridePartition==true by this function to keep downloading.
//			Meanwhile, the unit can't be unplugged and should be in the same waiting status. 
//************************************
bool CDownload6240::StreamingDLFromPartition(bool bSpeedUp, bool bReopenAfterReset)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	if(	StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL,  m_strDBL,  bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_strFSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_strOSBL, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_strAMSS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_strCEFS, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{
		if(bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bRes = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime, true);
			AddMsg(bRes, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bRes);
		}
		else
			bRes = true;
		
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		AddMsg("Download Fail", None, 0);
	}

	return bRes;
}

/*************************************************************************/

CShareDownload6245::CShareDownload6245(int nCOMPort)
:CDLFormatCommand(nCOMPort, "Qualcomm6245"), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(true)
{
	IMAGE imgNULL = {0};
	m_imgHEX = imgNULL;
	m_imgPBL = imgNULL;
	m_imgPartition = imgNULL;
	m_imgQCSBL = imgNULL;
	m_imgQCSBLHD = imgNULL;
	m_imgOEMSBL = imgNULL;
	m_imgOEMSBLHD = imgNULL;
	m_imgAMSS = imgNULL;
	m_imgAMSSHD = imgNULL;
	m_imgCEFS = imgNULL;

}
CShareDownload6245::~CShareDownload6245()
{
	/*Because this project doesn't call any function of mobile finder, so the "smart static link" won't link mobile finder.
	Hence the event of comport won't be set or reset.
	So we call a function of mobile finder here.
	This is meaningless, it is just used to force link mobile finder.
	*/
	QSYNC_LIB::GetDeviceFinder();
}

bool CShareDownload6245::CFCDownload(bool bSpeedup)  // for CFC download  = DMSS+Streaming, default speedUp download.
{
	AddDLLog("Flash start.", _T("DownloadProcess"));
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		PopLastLog();
	}
	AddDLLog("Flash end.", _T("DownloadProcess"));
	return bRes;
}
bool CShareDownload6245::Download(bool bSpeedup, bool bReopenAfterReset)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		AddMsg("Download Fail", None, 0);
		PopLastLog();
	}
	return bRes;
}
bool CShareDownload6245::DMSSDownload()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n6245NopRetryTimes,g_n6245NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( m_imgHEX.pData, m_imgHEX.nSize, dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
		Sleep(8000);  //buffering hex, needs more time to write.
	}
	return bRes;
}

bool CShareDownload6245::StreamingDL(bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;

	bool bRes = SendHello(bSpeedUp);
	if (bRes)
		bRes = StreamingSecurity("", bSpeedUp);
	if (bRes)
		 bRes = StreamingPartition(m_imgPartition.pData, m_imgPartition.nSize, m_bOverridePartition, bSpeedUp) ;
	if (bRes)
		 bRes = StreamingDLMultiImage(Image_type_QCSECBLHD, m_imgQCSBLHD.pData, m_imgQCSBLHD.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout) ;
	if (bRes)
		 bRes = StreamingDLMultiImage(Image_type_QCSECBL, m_imgQCSBL.pData, m_imgQCSBL.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout);
	if (bRes)
		 bRes = StreamingDLMultiImage(Image_type_OEMSECBL, m_imgOEMSBLHD.pData, m_imgOEMSBLHD.nSize, m_imgOEMSBL.pData, m_imgOEMSBL.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout);
	if (bRes)
		 bRes = StreamingDLMultiImage(Image_type_AMSSMODEL, m_imgAMSSHD.pData, m_imgAMSSHD.nSize, m_imgAMSS.pData, m_imgAMSS.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout);
	if (bRes)
		 bRes = StreamingDLMultiImage(Image_type_CEFS, m_imgCEFS.pData, m_imgCEFS.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6245CloseImageTimeout);
	if (bRes)
		 bRes = StreamingReset(bSpeedUp);
	if(bRes && m_bReopenAfterReset)
	{
		CReOpenComport ReopenAfterReset(this);
		bRes = ReopenAfterReset.ReOepnComport(g_n6245ReOpenAfterResetSleepTime, false, 200, 1000);
		AddMsg(bRes, "ReOpenAfterReset", ReOpenComport);
		SetLastError(DL_ERROR_ReOpenComport, bRes);
	}

	return bRes;
}

/*************************************************************************/

CShareDownload6240::CShareDownload6240(int nCOMPort, const std::string& strPlatform)
:CDLFormatCommand(nCOMPort, strPlatform), m_bOverridePartition(false), m_bResetMode(true), m_bReopenAfterReset(true)
{
	IMAGE imgNULL = {0};
	m_imgHEX = imgNULL;
	m_imgPBL = imgNULL;
	m_imgPartition = imgNULL;
	m_imgDBL = imgNULL;
	m_imgFSBL = imgNULL;
	m_imgOSBL = imgNULL;
	m_imgAMSS = imgNULL;
	m_imgCEFS = imgNULL;
}
CShareDownload6240::~CShareDownload6240(){}

bool CShareDownload6240::CFCDownload(bool bSpeedup)  // for CFC download  = DMSS+Streaming, default speedUp download.
{
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		PopLastLog();
	}
	return bRes;
}
bool CShareDownload6240::Download(bool bSpeedup, bool bReopenAfterReset)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		AddMsg("Download Fail", None, 0);
		PopLastLog();
	}
	return bRes;
}
bool CShareDownload6240::DMSSDownload()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n6240NopRetryTimes,g_n6240NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( m_imgHEX.pData, m_imgHEX.nSize, dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
	}
	return bRes;
}
bool CShareDownload6240::StreamingDL(bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CReOpenComport ReopenAfterGo(this, true);
	bool bReconnect = ReopenAfterGo.ReOepnComport(g_n6240ReOpenAfterGoSleepTime, false);
	AddMsg(bReconnect, "ReOpenAfterGo", StreamingProtocol);
	ASSERT(bReconnect);

	if( bReconnect && 
		StreamingHello(bSpeedUp, g_n6240HelloRetryTimes, g_n6240HelloTimeout) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingPartition(m_imgPartition.pData, m_imgPartition.nSize, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL, m_imgDBL.pData, m_imgDBL.nSize,  bSpeedUp  , g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_imgFSBL.pData, m_imgFSBL.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_imgOSBL.pData, m_imgOSBL.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_imgAMSS.pData, m_imgAMSS.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_imgCEFS.pData, m_imgCEFS.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{
		bRes = true;

		if(m_bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bReconnect = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime, true);
			ASSERT(bReconnect);
			AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bRes);
		}
		else
			bReconnect = true;
	}	

	return bRes && bReconnect;
}

//************************************
// Method:    StreamingDLFromPartition
// Returns:   bool : download success or fail
// Parameter: bool bSpeedUp : it must be the same as previous one
// Purpose: If previous download is failed in different partition table,
//			for overwriting partition table, 
//			you can resend partition with m_bOverridePartition==true by this function to keep downloading.
//			Meanwhile, the unit can't be unplugged and should be in the same waiting status. 
//************************************
bool CShareDownload6240::StreamingDLFromPartition(bool bSpeedUp, bool bReopenAfterReset)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;
	if(	StreamingPartition(m_imgPartition.pData, m_imgPartition.nSize, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL, m_imgDBL.pData, m_imgDBL.nSize,  bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_imgFSBL.pData, m_imgFSBL.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_imgOSBL.pData, m_imgOSBL.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_imgAMSS.pData, m_imgAMSS.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_imgCEFS.pData, m_imgCEFS.nSize, bSpeedUp, g_nRetryTimes, g_nTimeout, g_nTimeout, g_n6240CloseImageTimeout) &&
		StreamingReset(bSpeedUp))
	{
		if(bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bRes = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime, true);
			AddMsg(bRes, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bRes);
		}
		else
			bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		AddMsg("Download Fail", None, 0);
	}

	return bRes;
}

/*************************************************************************/

CShareDownload6270::CShareDownload6270(int nCOMPort)
:CShareDownload6240(nCOMPort, "Qualcomm6270")
{
}
CShareDownload6270::~CShareDownload6270(){}

bool CShareDownload6270::CFCDownload(bool bSpeedup)  // for CFC download  = DMSS+Streaming, default speedUp download.
{
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
	}
	return bRes;
}
bool CShareDownload6270::Download(bool bSpeedup, bool bReopenAfterReset)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
		AddMsg("Download Fail", None, 0);
	}
	return bRes;
}
bool CShareDownload6270::DMSSDownload()
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n6240NopRetryTimes,g_n6240NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( m_imgHEX.pData, m_imgHEX.nSize, dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
	}
	return bRes;
}
bool CShareDownload6270::StreamingDL(bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CReOpenComport ReopenAfterGo(this, true);
	bool bReconnect = ReopenAfterGo.ReOepnComport(g_n6240ReOpenAfterGoSleepTime, false);
	AddMsg(bReconnect, "ReOpenAfterGo", StreamingProtocol);
	ASSERT(bReconnect);

	if( bReconnect && 
		StreamingHello(bSpeedUp, g_n6240HelloRetryTimes, g_n6240HelloTimeout) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingPartition(m_imgPartition.pData, m_imgPartition.nSize, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL, m_imgDBL.pData, m_imgDBL.nSize,  bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_imgFSBL.pData, m_imgFSBL.nSize, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_imgOSBL.pData, m_imgOSBL.nSize, bSpeedUp) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_imgAMSS.pData, m_imgAMSS.nSize, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_imgCEFS.pData, m_imgCEFS.nSize, bSpeedUp) &&
		StreamingReset(bSpeedUp))
	{
		bRes = true;

		if(m_bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bReconnect = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime, true);
			ASSERT(bReconnect);
			AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bRes);
		}
		else
			bReconnect = true;
	}	

	return bRes && bReconnect;
}

//**************************************************************/

CDownload6270::CDownload6270(int nCOMPort)
:CDownload6240(nCOMPort, "Qualcomm6270")
{
}

CDownload6270::~CDownload6270(void)
{
}

bool CDownload6270::CFCDownload(bool bSpeedup)
{
	bool bRes = false;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode();
	}
	return bRes;
}

bool CDownload6270::Download(bool bSpeedup, bool bReopenAfterReset)
{
	bool bRes = false;
	m_bReopenAfterReset = bReopenAfterReset;
	if(DMSSDownload() && StreamingDL(bSpeedup))
	{
		AddMsg("Download Success", None, 100);
		bRes = true;
	}
	else
	{
		if(m_bResetMode)
			CInterfaceBase::ResetMode(); 
		AddMsg("Download Fail", None, 0);
	}
	return bRes;
}

bool CDownload6270::DMSSDownload(const std::string& strHexFilePath)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	WORD dwAddress=0;
	if( DMSSNop(g_n6240NopRetryTimes,g_n6240NopTimeout) && 
		DMSSParameter() &&
		DMSSOpenAndWriteHexFile( (strHexFilePath.empty() ? m_strHEX : strHexFilePath), dwAddress) &&
		DMSSGo(dwAddress, 0))
	{
		bRes = true;
	}
	return bRes;
}


bool CDownload6270::StreamingDL(bool bSpeedUp)
{
	using namespace PLATFORM_TOOL;
	bool bRes = false;

	CReOpenComport ReopenAfterGo(this, true);
	bool bReconnect = ReopenAfterGo.ReOepnComport(g_n6240ReOpenAfterGoSleepTime, false);
	AddMsg(bReconnect, "ReOpenAfterGo", StreamingProtocol);
	ASSERT(bReconnect);

	if( bReconnect && 
		StreamingHello(bSpeedUp, g_n6240HelloRetryTimes, g_n6240HelloTimeout) &&
		StreamingSecurity("", bSpeedUp) && 
		StreamingOpenAndWritePartition(m_strPartition, m_bOverridePartition, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_DBL,  m_strDBL,  bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_FSBL, m_strFSBL, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_OSBL, m_strOSBL, bSpeedUp) && 
		StreamingDLMultiImage(OPEN_MULTI_MODE_AMSS, m_strAMSS, bSpeedUp) &&
		StreamingDLMultiImage(OPEN_MULTI_MODE_CEFS, m_strCEFS, bSpeedUp) &&
		StreamingReset(bSpeedUp))
	{
		bRes = true;

		if(m_bReopenAfterReset)
		{
			CReOpenComport ReopenAfterReset(this);
			bReconnect = ReopenAfterReset.ReOepnComport(g_n6240ReOpenAfterResetSleepTime, true);
			ASSERT(bReconnect);
			AddMsg(bReconnect, "ReOpenAfterReset", ReOpenComport);
			SetLastError(DL_ERROR_ReOpenComport, bReconnect);
		}
		else
			bReconnect = true;
		
	}

	return bRes && bReconnect;
}

