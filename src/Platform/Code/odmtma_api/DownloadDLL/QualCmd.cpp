#include "StdAfx.h"
#include "QualCmd.h"
#include "..\..\..\Code\CommonUtil\UnicodeUtility.h"
#include "..\..\..\Code\CommonUtil\LogPolicy.h"
#include "..\..\..\Code\Lib\RequestUtility\DownloadCommand.h"
#include "../../Components/MobileFinder/MobileFinderInterface.h"


BYTE	g_lpbyRXdata[0x1000];

CQualCmd::CQualCmd(CQualcommDiag* pQualcommDiag) : 
	m_bIsComPortOpen(true),
	m_CallBackFunc(NULL),
	m_CallBack(NULL),
	m_hDuckDll(NULL),
	m_bIsDuckActive(false),
	bNewSercomByItself(false),
	m_pQualcommDiag(pQualcommDiag)
{
	if(pQualcommDiag)
	{
		pQualcommDiag->GetCSerCOMLibrary(&m_objsercom);
		TCHAR szComport[50] = {0};
		int nComPort = m_objsercom->GetComPort();
		if(nComPort > 0)
			::_stprintf(szComport,_T("\\\\.\\COM%d"),nComPort);
		m_strComPort = szComport;
	}
	TRACE_FILE_LINE_INFO();
	//int RunDuckLibUsingHandler( HANDLE hResourceContext, int iEmerg /*= 0*/ );
	/*m_hDuckDll = ::LoadLibrary(_T("Duck6.dll"));
	typedef int (_cdecl *pfRunDuckLib)(HANDLE, int);
	pfRunDuckLib RunDuck = (pfRunDuckLib)GetProcAddress(m_hDuckDll, "RunDuckLibUsingHandler");
	int nRet = RunDuck(m_objsercom->GetCommHandle(), 1);
	m_bIsDuckActive = nRet == 0;*/
}

CQualCmd::CQualCmd() : 
	m_bIsComPortOpen(false),
	m_hDuckDll(NULL),
	m_CallBack(NULL),
	m_bIsDuckActive(false),
	m_CallBackFunc(NULL),
	bNewSercomByItself(true)
{
	m_objsercom = new CSerCOMLibrary;
}

CQualCmd::~CQualCmd(void)
{
	//FreeLibrary(m_hDuckDll);
	if(bNewSercomByItself)
		delete m_objsercom;
}

int CQualCmd::SetCallBackFunc(LPVOID pfunc)
{
	int nRet = RUN_FAILED;
	_ASSERTE(pfunc != NULL);
	if(pfunc)
	{
		CallBackFunc pFunc = (CallBackFunc)pfunc;
		m_CallBackFunc = pFunc;
		nRet = RUN_SUCCESSFULLY;
	}
	return nRet;
}

bool CQualCmd::SetCallBack(LPVOID pfunc)
{
	bool bRes = false;
	if(pfunc)
	{
		m_CallBack = (CallBack)pfunc;
		bRes = true;
	}
	return bRes;
}

void CQualCmd::LogFunc(const char* cpMsg, int nRet)
{
	bool bFail = (nRet < 0);
	std::string strMsg = cpMsg;
	strMsg = bFail ? strMsg+" Fail!" : strMsg+" Success!";
	if(m_CallBackFunc)
		m_CallBackFunc(strMsg.c_str(), bFail ? -1 : 0 );
}

void CQualCmd::LogFunc(const char* cpMsg, int nRet, int nStep)
{
	bool bFail = (nRet < 0);
	std::string strMsg = cpMsg;
	strMsg = bFail ? strMsg+" Fail" : strMsg+" Success";
	if(m_CallBack)
		m_CallBack(strMsg.c_str(), nStep, bFail? 0 : 100);
}

BOOL CQualCmd::SetComPort(LPCTSTR szCom)
{
	BOOL bRes = RUN_SUCCESSFULLY;
	_ASSERTE(szCom != NULL);
	if(szCom)
		m_strComPort = szCom;
	else 
		bRes = RUN_FAILED;
	return bRes;
}

LPCTSTR CQualCmd::GetComPort()
{
	LogFunc("Get ComPort", m_strComPort.IsEmpty()? RUN_FAILED : RUN_SUCCESSFULLY);
	return (LPCTSTR)m_strComPort;
}

int CQualCmd::OpenComPort()
{
	int nRet = RUN_FAILED;
	if(!m_strComPort.IsEmpty())
	{
        TCHAR szComport[50] = {0};
        int nComPort = m_objsercom->GetComPort();
        if(nComPort > 0)
            ::_stprintf(szComport,_T("\\\\.\\COM%d"),nComPort);
        m_strComPort = szComport;
		nRet = m_objsercom->OpenComport(_T("Target download"), m_strComPort);
		m_bIsComPortOpen = true;
		std::string strLog = "Open ComPort ";
#ifdef _UNICODE
		strLog += strQSYNC_LIB::widetoansi((std::wstring)m_strComPort);
#else
		strLog += m_strComPort;
#endif
		LogFunc(strLog.c_str(), nRet);
		if(nRet < 0)
			CloseComPort();
		else
			nRet = RUN_SUCCESSFULLY;
	}
	return nRet;
}

void CQualCmd::CloseComPort()
{
	if(m_bIsComPortOpen)
		m_objsercom->CloseComport();
	m_bIsComPortOpen = false;
}

int CQualCmd::ChangeToDLMode()
{
	if(m_CallBack)
		m_CallBack("Change To DL mode", 3, 0);
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	for(int i=0; i<2 && nRet<0 ; i++)
	{
		nRet = m_objsercom->IsDLConnected();
		if(m_CallBack)
			m_CallBack("Change To DL mode", 3, (100/2)*(i+1));
	}

	if(nRet < 0 && m_pQualcommDiag)
		m_pQualcommDiag->SetLastError(DL_ERROR_ChangeDLMode);
	LogFunc("Change to DLMode", nRet);
	LogFunc("Change to DLMode", nRet, 3);

	TRACE_FILE_LINE_INFO();
	return nRet;
}

int CQualCmd::ChangeToDiagMode(bool bMakeSure)
{
	int nRet = RUN_FAILED;
	if(!bMakeSure)
		nRet = m_objsercom->IsDIAGConnected();
	else
		nRet = m_objsercom->MakesureToDIAGMode()? RUN_SUCCESSFULLY: RUN_FAILED;
	/*if(nRet < 0)
		CmdSReset();*/
	LogFunc("Change to DiagMode", nRet);
	return nRet;
}

int CQualCmd::CmdNopOpeation(void)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	BYTE nDloadNopCmd = DMSS_CMD_NOP;
	DmssACKandNACKResp dmssACKandNACKResp = {0};
	if(m_objsercom->TXRXDl((char *) &nDloadNopCmd, sizeof(nDloadNopCmd),(char*)&dmssACKandNACKResp, sizeof (DmssACKandNACKResp), DMSS_CMD_TIME_OUT) >= 0 )
	{	
		if(dmssACKandNACKResp.nNopResp == DMSS_CMD_ACK)
			nRet = RUN_SUCCESSFULLY;
	}
	/*if(nRet < 0)
		CmdSReset();*/
	LogFunc("DMSS Nop", nRet);
	return nRet;
}

int CQualCmd::CmdParameter(void)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	DmssParaReq dmssParaReq = {0};
	DmssPraResp dmssPraResp = {0};
	dmssParaReq.nDloadCmd = DMSS_CMD_PREQ;
	ZeroMemory(g_lpbyRXdata,sizeof(0x1000));
	UINT nRXsize=0;
	nRet = m_objsercom->TXRXDl((char*)&dmssParaReq, sizeof(DmssParaReq),(char*)&dmssPraResp, sizeof(DmssPraResp),DMSS_CMD_TIME_OUT);
	//if (lpbyRXdata[0] == DMSS_CMD_PARAMS){
	if(dmssPraResp.nDloadCmd == DMSS_CMD_PARAMS)
	{
		nRet = RUN_SUCCESSFULLY;
		if(m_CallBack)
			m_CallBack("DMSS Download", 4, (100/4)*2);
	}
	else
	{
		if(m_pQualcommDiag)
			m_pQualcommDiag->SetLastError(DL_ERROR_DMSS_PARAMETER);
	}
	if(nRet < 0)
	{
		CmdSReset();
	}
	
	LogFunc("Request Parameter", nRet);
	return nRet;
}

int CQualCmd::CmdSoftwareverion(void)
{
	DmssVERREQ dmssVERREQ;
	DmssVERRSP dmssVERRSP;
	ZeroMemory(&dmssVERREQ, sizeof(DmssVERREQ));
	ZeroMemory(&dmssVERRSP, sizeof(DmssVERRSP));
	dmssVERREQ.nDloadCmd = DMSS_CMD_VERREQ;
	return m_objsercom->TXRXDl((char*)&dmssVERREQ, sizeof(DmssVERREQ),(char*)&dmssVERRSP, sizeof(DmssVERRSP));
}


int CQualCmd::CmdWrite32bit(BYTE * lpWData, WORD dwWLength,DWORD dwAddress)
{
	int nRet = RUN_FAILED;
	DmssWrite32bitAddressReq Request;
	DmssACKandNACKResp Response;
	ZeroMemory(&Request, sizeof(DmssWrite32bitAddressReq));
	ZeroMemory(&Response, sizeof(DmssACKandNACKResp));
	Request.nDloadCmd = DMSS_CMD_WRITE32BITADDRESS;
	DWORD dwtemp=dwAddress;
	for (int i=0;i<4;i++)
	{
		Request.nsAddress[3-i] = dwtemp%256;
		dwtemp=dwtemp/256;
	}
	//DmssWrite32bitAddressReq.nsAddress=dwAddress;
	WORD wtemp=dwWLength;
	for (int i=0;i<2;i++)
	{
		Request.nsLength[1-i]=wtemp%256;
		wtemp=wtemp/256;
	}
	//DmssWrite32bitAddressReq.nsLength=dwWLength;
	memcpy(Request.nsData,lpWData,dwWLength);
	int nSize=dwWLength+7;
	nRet = m_objsercom->TXRXDl((char *) &Request, nSize,(char *)&Response, sizeof (DmssACKandNACKResp), DMSS_CMD_TIME_OUT);
	if (nRet < 0){
		nRet = m_objsercom->TXRXDl((char *) &Request, nSize,(char *)&Response, sizeof (DmssACKandNACKResp), DMSS_CMD_TIME_OUT);
	}
	if(Response.nNopResp == DMSS_CMD_ACK)
		nRet = RUN_SUCCESSFULLY;
	
	return nRet;

}

int CQualCmd::CmdGO(WORD nwSegment,WORD nwoffset)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	DmssGOReq dmssGOReq;
	DmssACKandNACKResp dmssACKandNACKResp;
	ZeroMemory(&dmssGOReq, sizeof(DmssGOReq));
	ZeroMemory(&dmssACKandNACKResp, sizeof(DmssACKandNACKResp));
	dmssGOReq.nDloadCmd = DMSS_CMD_GO;
	WORD wtemp = nwSegment;
	for (int i=0;i<2;i++)
	{
		dmssGOReq.nsSegment[1-i]=wtemp%256;
		wtemp=wtemp/256;
	}
	wtemp=nwoffset;
	for (int i=0;i<2;i++)
	{
		dmssGOReq.nsOffset[1-i]=wtemp%256;
		wtemp=wtemp/256;
	}
	nRet = m_objsercom->TXRXDl((char *) &dmssGOReq, sizeof(DmssGOReq),(char *)&dmssACKandNACKResp, sizeof (DmssACKandNACKResp));
	
	if(nRet >= 0 && dmssACKandNACKResp.nNopResp == DMSS_CMD_ACK)
		nRet = RUN_SUCCESSFULLY;
	/*if(nRet < 0)
		CmdSReset();*/
	/*if(nRet < 0 && m_pQualcommDiag)
		m_pQualcommDiag->SetLastError(DL_ERROR_DMSS_GO);*/
	if(m_CallBack)
		m_CallBack("DMSS Download", 4, (100/4)*4);
	LogFunc("Run Hex", nRet);
	return nRet;
}

int CQualCmd::CmdHello(void)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	StreamingHelloParmReq Req;
	StreamingHelloParmResp Resp;
	ZeroMemory(&Req, sizeof(StreamingHelloParmReq));
	ZeroMemory(&Resp, sizeof(StreamingHelloParmResp));
	Req.nCmdCode = Streaming_CMD_HELLO;
	char * ctemp =new char [64];
	sprintf_s(ctemp,60,"QCOM fast download protocol host");
	memcpy(Req.nMagicNumber,ctemp,32);
	delete [] ctemp;
	Req.nVersion = 0x03;
	Req.nCVersion = 0x03;
	Req.nFeaturebit = 0x09;
	nRet = m_objsercom->TXRXDl((char*)&Req, sizeof(StreamingHelloParmReq),(char*)&Resp, sizeof (StreamingHelloParmResp),500);

	if (nRet >=0 && Resp.nCmdCode == Streaming_CMD_HELLORESP)
		nRet = RUN_SUCCESSFULLY;
	LogFunc("Hello", nRet);
	return nRet;
}

int CQualCmd::CmdSpeedUpOrgHello(void)
{
	int nRet = RUN_FAILED;
	StreamingHelloParmReq Req;
	StreamingHelloParmResp Resp;
	ZeroMemory(&Req, sizeof(StreamingHelloParmReq));
	ZeroMemory(&Resp, sizeof(StreamingHelloParmResp));
	Req.nCmdCode = Streaming_CMD_SpeedUp_HELLO;
	char * ctemp =new char [64];
	sprintf_s(ctemp,60,"QCOM fast download protocol host");
	memcpy(Req.nMagicNumber,ctemp,32);
	delete [] ctemp;
	Req.nVersion = 0x03;
	Req.nCVersion = 0x03;
	Req.nFeaturebit = 0x09;
	nRet = m_objsercom->TxRx((char*)&Req, sizeof(StreamingHelloParmReq),(char*)&Resp, sizeof (StreamingHelloParmResp),1000, true);

	if (nRet >=0 && Resp.nCmdCode == Streaming_CMD_SpeedUp_HELLORESP)
		nRet = RUN_SUCCESSFULLY;
	LogFunc("Hello", nRet);
	return nRet;
}




int CQualCmd::CmdCloseImage(bool bSpeedUp)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	StreamingCloseReq streamingCloseReq;
	StreamingCloseResp streamingCloseResp;
	ZeroMemory(&streamingCloseReq, sizeof(StreamingCloseReq));
	ZeroMemory(&streamingCloseResp, sizeof(StreamingCloseResp));
	streamingCloseReq.nDloadCmd = Streaming_CMD_CLOSE;
	//return m_objsercom->DLSend((char *) &StreamingCloseReq, sizeof(StreamingCloseReq));
	if(!bSpeedUp)
		nRet = m_objsercom->TXRXDl((char*)&streamingCloseReq, sizeof(StreamingCloseReq), (char*)&streamingCloseResp, sizeof(StreamingCloseResp), 15000);
	else
		nRet = m_objsercom->SpeedUpTxRx((char*)&streamingCloseReq, sizeof(StreamingCloseReq), (char*)&streamingCloseResp, sizeof(StreamingCloseResp), 15000);
	if (nRet >=0 && streamingCloseResp.nDloadCmd == Streaming_CMD_CLOSED)
		nRet = RUN_SUCCESSFULLY;
	return nRet;
}

/*
* Anders 2007/11/23
* Change to parameter mode
*/
int CQualCmd::CmdSercurity(BYTE nMode)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	StreamingSecurityReq Request;
	StreamingSecurityResp Response;
	ZeroMemory(&Request, sizeof(StreamingSecurityReq));
	ZeroMemory(&Response, sizeof(StreamingSecurityResp));
	Request.nDloadCmd = Streaming_CMD_SECMODE;
	Request.nMode = nMode;
	nRet = m_objsercom->TXRXDl((char*)&Request, sizeof(StreamingSecurityReq), (char*)&Response, sizeof(StreamingSecurityResp));
	
	if (nRet >=0 && Response.nDloadCmd == Streaming_CMD_SECMODER)
		nRet = RUN_SUCCESSFULLY;
	
	LogFunc("Send Security", nRet);
	return nRet;	
}

int CQualCmd::CmdSpeedUpSercurity(BYTE nMode)
{
	int nRet = RUN_FAILED;
	StreamingSecurityReq Request = {0};
	StreamingSecurityResp Response = {0};
	Request.nDloadCmd = Streaming_CMD_SECMODE;
	Request.nMode = nMode;
	nRet = m_objsercom->SpeedUpTxRx((char*)&Request, sizeof(StreamingSecurityReq), (char*)&Response, sizeof(StreamingSecurityResp));

	if (nRet >=0 && Response.nDloadCmd == Streaming_CMD_SECMODER)
		nRet = RUN_SUCCESSFULLY;

	if(nRet <0)
	{
		if(m_pQualcommDiag)
			m_pQualcommDiag->SetLastError(DL_ERROR_Streaming_Security);
	}
	else
	{
		if(m_CallBack)
			m_CallBack("Streaming Download", 5, (100/6)*2);
	}

	return nRet;	
}

int CQualCmd::CmdPartition(BYTE* lpbyData, WORD wnlength)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	StreamingPartitionReq streamingPartitionReq;
	StreamingPartitionResp streamingPartitionResp;
	ZeroMemory(&streamingPartitionReq, sizeof(StreamingPartitionReq));
	ZeroMemory(&streamingPartitionResp, sizeof(StreamingPartitionResp));
	streamingPartitionReq.nDloadCmd = Streaming_CMD_PATTTIONT;
	streamingPartitionReq.nOverrive = 0x00;
	memcpy(streamingPartitionReq.nPartition,lpbyData,wnlength);
	nRet = m_objsercom->TXRXDl((char*)&streamingPartitionReq, wnlength+2, (char*)&streamingPartitionResp, sizeof(StreamingPartitionResp));

	if (streamingPartitionResp.nDloadCmd == Streaming_CMD_PATTTIONTR)
		nRet = RUN_SUCCESSFULLY;

	LogFunc("Write Partition", nRet);
	return nRet;	
}

int CQualCmd::CmdSpeedUpPartition(BYTE * lpbyData, WORD wnlength)
{
	int nRet = RUN_FAILED;
	StreamingPartitionReq streamingPartitionReq;
	StreamingPartitionResp streamingPartitionResp;
	ZeroMemory(&streamingPartitionReq, sizeof(StreamingPartitionReq));
	ZeroMemory(&streamingPartitionResp, sizeof(StreamingPartitionResp));
	streamingPartitionReq.nDloadCmd = Streaming_CMD_PATTTIONT;
	streamingPartitionReq.nOverrive = 0x00;
	memcpy(streamingPartitionReq.nPartition,lpbyData,wnlength);
	nRet = m_objsercom->SpeedUpTxRx((char*)&streamingPartitionReq, wnlength+2, (char*)&streamingPartitionResp, sizeof(StreamingPartitionResp));

	if (streamingPartitionResp.nDloadCmd == Streaming_CMD_PATTTIONTR)
		nRet = RUN_SUCCESSFULLY;
	return nRet;	
}

int CQualCmd::CmdOpenMutiImage(BYTE nType,BYTE * lpbyData, WORD wnlength)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	StreamingOpenmimageReq Req = {0};
	StreamingOpenmimageResp Resp = {0};
	Req.nDloadCmd = Streaming_CMD_OPENMIMAGE;
	Req.nType = nType;
	memcpy(Req.nData,lpbyData,wnlength);
	
	for(int i=0; i<2 && nRet <0; i++)
	{
		if(nType == Image_type_CEFS)
			nRet = m_objsercom->TXRXDl((char*)&Req, wnlength+2,(char*)&Resp, sizeof(StreamingOpenmimageResp), 20000);
		else
			nRet = m_objsercom->TXRXDl((char*)&Req, wnlength+2,(char*)&Resp, sizeof(StreamingOpenmimageResp),10000);
	}

	if (nRet >=0 &&Resp.nDloadCmd == Streaming_CMD_OPENEDMIMAGE)
		nRet = RUN_SUCCESSFULLY;
	
	return nRet;
}


int CQualCmd::CmdSpeedUpOpenMutiImage(BYTE nType, BYTE* lpbyData, WORD wnlength)
{
	int nRet = RUN_FAILED;
	StreamingOpenmimageReq Req = {0};
	StreamingOpenmimageResp Resp = {0};
	Req.nDloadCmd = Streaming_CMD_OPENMIMAGE;
	Req.nType = nType;
	memcpy(Req.nData,lpbyData,wnlength);

	for(int i=0; i<2 && nRet <0; i++)
	{
		if(nType == Image_type_CEFS)
			nRet = m_objsercom->SpeedUpTxRx((char*)&Req, wnlength+2,(char*)&Resp, sizeof(StreamingOpenmimageResp), 20000);
		else
			nRet = m_objsercom->SpeedUpTxRx((char*)&Req, wnlength+2,(char*)&Resp, sizeof(StreamingOpenmimageResp),10000);
	}

	if (nRet >=0 &&Resp.nDloadCmd == Streaming_CMD_OPENEDMIMAGE)
		nRet = RUN_SUCCESSFULLY;

	return nRet;
}


int CQualCmd::CmdSReset(bool bSpeedUp)
{
	TRACE_FILE_LINE_INFO();
	int nRet = RUN_FAILED;
	StreamingResetReq streamingResetReq;
	StreamingResetResp streamingResetResp;
	ZeroMemory(&streamingResetReq, sizeof(StreamingResetReq));
	ZeroMemory(&streamingResetResp, sizeof(StreamingResetResp));
	streamingResetReq.nDloadCmd = Streaming_CMD_RESET;
	if(!bSpeedUp)
		nRet = m_objsercom->TXRXDl((char*)&streamingResetReq, sizeof(StreamingResetReq),(char*)&streamingResetResp, sizeof(StreamingResetResp));
	else
		nRet = m_objsercom->SpeedUpTxRx((char*)&streamingResetReq, sizeof(StreamingResetReq),(char*)&streamingResetResp, sizeof(StreamingResetResp));

	if (nRet >= 0 && streamingResetResp.nDloadCmd == Streaming_CMD_RESETACK /*&& m_objsercom->ResetMode()*/)
	{
		nRet = RUN_SUCCESSFULLY;
	}	
	
	//CloseComPort();
	m_objsercom->ResetMode();
	
	return nRet;
}

int CQualCmd::CmdStrWrite(BYTE * lpData, WORD wnLength, DWORD dwAddress)
{
	StreamingStrWriteReq streamingStrWriteReq = {0};
	StreamingStrWriteResp streamingStrWriteResp = {0};
	streamingStrWriteReq.nDloadCmd = Streaming_CMD_STRWRITE;
	DWORD dwtemp = dwAddress;
	memcpy(streamingStrWriteReq.nAddress,&dwAddress,sizeof(dwAddress));
	memcpy(streamingStrWriteReq.nData,lpData,wnLength);
	int nSize = wnLength+5;	
	return m_objsercom->DLSend((char*)&streamingStrWriteReq, nSize,TRUE);
}

int CQualCmd::CmdSpeedUpStrWrite2(BYTE * lpData, WORD wnLength, DWORD dwAddress)
{
	int nRet = RUN_FAILED;
	StreamingStrWriteReq Req = {0};
	StreamingStrWriteResp Resp = {0};
	Req.nDloadCmd = Streaming_CMD_STRWRITE;  //streaming write
	DWORD dwtemp = dwAddress;
	memcpy(Req.nAddress,&dwAddress,sizeof(dwAddress));
	memcpy(Req.nData,lpData,wnLength);
	int nSize = wnLength+5;

	for(int i=0; i<3 && nRet<0; i++)
	{
		nRet = m_objsercom->SpeedUpTxRx((char*)&Req, nSize,(char*)&Resp, sizeof(Resp),1000); // wait 500
		if(nRet>=0 && memcmp(Req.nAddress,Resp.nAddress,sizeof(Resp.nAddress)) == 0)
			break;
		TRACE("CmdStrWrite2 try %d times! \n", i);
	}
	if ( nRet >=0 && Resp.nDloadCmd == 0x08 )
		nRet = ERROR_SUCCESS;
	else
		ASSERT(0);
	return nRet;
}

int CQualCmd::CmdStrWrite2(BYTE * lpData, WORD wnLength, DWORD dwAddress)
{
	int nRet = RUN_FAILED;
	StreamingStrWriteReq Req = {0};
	StreamingStrWriteResp Resp = {0};
	Req.nDloadCmd = Streaming_CMD_STRWRITE;  //streaming write
	DWORD dwtemp = dwAddress;
	memcpy(Req.nAddress,&dwAddress,sizeof(dwAddress));
	memcpy(Req.nData,lpData,wnLength);
	int nSize = wnLength+5;
	
	for(int i=0; i<10 && nRet<0; i++)
	{
		nRet = m_objsercom->TXRXDl((char*)&Req, nSize,(char*)&Resp, sizeof(Resp),500); // wait 500
		if(nRet>=0 && memcmp(Req.nAddress,Resp.nAddress,sizeof(Resp.nAddress)) == 0)
			break;
		TRACE("CmdStrWrite2 try %d times! \n", i);
	}
	
	if ( nRet >=0 && Resp.nDloadCmd == 0x08 )
		nRet = ERROR_SUCCESS;
	else
		ASSERT(0);
	return nRet;
}

int CQualCmd::CmdDiagIsPhoneConnected()
{
	int nRet = RUN_FAILED;
	DIAGIsPhoneConnectedcmdRequest dIAGIsPhoneConnectedcmdRequest;
	DIAGIsPhoneConnectedcmdResponse dIAGIsPhoneConnectedcmdResponse;
	ZeroMemory(&dIAGIsPhoneConnectedcmdRequest, sizeof(DIAGIsPhoneConnectedcmdRequest));
	dIAGIsPhoneConnectedcmdRequest.nDIAGcmd = DIAG_PROTOCOL_LOOPBACK_F;
	for(int i = 0; i< 10; i++)						//Zero to Nine
		dIAGIsPhoneConnectedcmdRequest.nZeroToNine[i] = i;

	ZeroMemory(&dIAGIsPhoneConnectedcmdResponse, sizeof(DIAGIsPhoneConnectedcmdResponse));
	
	nRet = m_objsercom->TXRXDiag((char*)&dIAGIsPhoneConnectedcmdRequest, sizeof(DIAGIsPhoneConnectedcmdRequest), (char*)&dIAGIsPhoneConnectedcmdResponse, sizeof(DIAGIsPhoneConnectedcmdResponse));

	if (dIAGIsPhoneConnectedcmdResponse.nDIAGcmd == DIAG_PROTOCOL_LOOPBACK_F)
		nRet = RUN_SUCCESSFULLY;

	if(nRet < 0)
	{
		CmdSReset();
	}
	LogFunc("Download Process", nRet);
	return nRet;
}

int CQualCmd::CmdDiagNVWrite(USHORT nItemID, UCHAR* pItemData, int nLength, USHORT* nStatus)
{
	int nRet = RUN_FAILED;
	DIAGWriteNVcmdRequest dIAGWriteNVcmdRequest;
	DIAGNVcmdResponse dIAGNVcmdResponse;
	ZeroMemory(&dIAGWriteNVcmdRequest,sizeof(DIAGWriteNVcmdRequest));
	ZeroMemory(&dIAGNVcmdResponse,sizeof(DIAGNVcmdResponse));
	dIAGWriteNVcmdRequest.nDIAGcmd = DIAG_CMD_WriteNVitem;
	CopyMemory(dIAGWriteNVcmdRequest.nNVitem, &nItemID, 2);
	CopyMemory(dIAGWriteNVcmdRequest.nItemData, pItemData, nLength);

	nRet = m_objsercom->TXRXDiag((CHAR*)&dIAGWriteNVcmdRequest, sizeof(DIAGWriteNVcmdRequest), (CHAR*)&dIAGNVcmdResponse, sizeof(DIAGNVcmdResponse));

	if(nRet >= 0 && dIAGNVcmdResponse.nDIAGcmd == DIAG_CMD_WriteNVitem) 
	{
		CopyMemory(nStatus, &dIAGNVcmdResponse.nStatus[0], sizeof(USHORT));
		nRet = RUN_SUCCESSFULLY;
	}

	return nRet;
}

bool CQualCmd::CmdWriteNV(USHORT nItemID, UCHAR* pItemData, int nLength, USHORT* nStatus)
{
	bool bRes = false;
	if(pItemData && nStatus)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
		WriteReq.item = nItemID;
		memcpy(WriteReq.item_data, pItemData, nLength); 
		if(m_pQualcommDiag)
		{
			bRes = m_pQualcommDiag->WriteNVItem(WriteReq);
			if(bRes)
				*nStatus = 0;
			else
			{
				if(m_pQualcommDiag->GetLastError() == DIAG_ERROR_WRITE_NVITEM_VARIABLE_NOT_ACTIVE)
				{	
					bRes = true;
					*nStatus = 5;
				}
			}
		}
		else
		{
			CmdDiagNVWrite(nItemID, pItemData, nLength, nStatus);
		}
	}
	return bRes;
}

BOOL CQualCmd::CmdDiagEFSWrite(CHAR* pszPathInMobile, BYTE* pData, WORD wDataSize)
{//!!!
	int nRet = -1;
	//ObtainInfo Cmd for EFS write
	DIAGEFSFsAvlSpaceCmdRequest dIAGEFSFsAvlSpaceCmdRequest;
	DIAGEFSFsAvlSpaceCmdResponse dIAGEFSFsAvlSpaceCmdResponse;
	ZeroMemory(&dIAGEFSFsAvlSpaceCmdRequest, sizeof(DIAGEFSFsAvlSpaceCmdRequest));
	ZeroMemory(&dIAGEFSFsAvlSpaceCmdResponse, sizeof(DIAGEFSFsAvlSpaceCmdResponse));

	dIAGEFSFsAvlSpaceCmdRequest.nDIAGcmd = DIAG_SUBSYS_CMD_F;
	dIAGEFSFsAvlSpaceCmdRequest.nSubsysID = DIAG_SUBSYS_EFS;
	dIAGEFSFsAvlSpaceCmdRequest.nCmdCodeFsAvlSpace = EFS2_DIAG_STATFS;
	size_t n = sizeof(DIAGEFSFsAvlSpaceCmdRequest);
	nRet = m_objsercom->TXRXDiag((CHAR*)&dIAGEFSFsAvlSpaceCmdRequest, sizeof(DIAGEFSFsAvlSpaceCmdRequest), (CHAR*)&dIAGEFSFsAvlSpaceCmdResponse, sizeof(DIAGEFSFsAvlSpaceCmdResponse));
	if( nRet < 0)
		return nRet;

	if(dIAGEFSFsAvlSpaceCmdResponse.wCmdCodeFsAvlSpace == EFS2_DIAG_STATFS && dIAGEFSFsAvlSpaceCmdResponse.dwErrnum == 0) //ObtainEFSInfo Success 
	{
		INT nFileDescripter = 0;
		DIAGEFSOpenFileCmdRequest dIAGEFSOpenFileCmdRequest;
		DIAGEFSOpenFileCmdResponse dIAGEFSOpenFileCmdResponse;
		ZeroMemory(&dIAGEFSOpenFileCmdRequest, sizeof(DIAGEFSOpenFileCmdRequest));
		ZeroMemory(&dIAGEFSOpenFileCmdResponse, sizeof(DIAGEFSOpenFileCmdResponse));
		dIAGEFSOpenFileCmdRequest.nDIAGcmd = DIAG_SUBSYS_CMD_F;
		dIAGEFSOpenFileCmdRequest.nSubsysID = DIAG_SUBSYS_EFS;
		dIAGEFSOpenFileCmdRequest.wCmdCodeOpenFile = EFS2_DIAG_OPEN;
		dIAGEFSOpenFileCmdRequest.dwFlag = O_WRONLY | O_CREAT | O_TRUNC;
		dIAGEFSOpenFileCmdRequest.dwMode = DEFFILEMODE;
		CopyMemory(dIAGEFSOpenFileCmdRequest.nPath, pszPathInMobile, strlen(pszPathInMobile));
		DWORD dwLength = (DWORD)(12 + strlen(pszPathInMobile) + 1);
		INT nCount = 0;
		BOOL bOpenFileStatus = FALSE;
		while(nCount < 5) //Try 5 times to check folder status
		{
			nRet = m_objsercom->TXRXDiag((CHAR*)&dIAGEFSOpenFileCmdRequest, dwLength, (CHAR*)&dIAGEFSOpenFileCmdResponse, sizeof(DIAGEFSOpenFileCmdResponse));
			if(nRet < 0)
				return FALSE;
			if(dIAGEFSOpenFileCmdResponse.wCmdCodeOpenFile == EFS2_DIAG_OPEN && dIAGEFSOpenFileCmdResponse.nFileDescripter != 0xFFFFFFFF)
			{
				bOpenFileStatus = TRUE;
				nFileDescripter = dIAGEFSOpenFileCmdResponse.nFileDescripter;
				break;
			}
			nCount++;
		}
		INT nWriteCount = 0;
		INT nWriteBytes = 0;
		if(bOpenFileStatus == TRUE)// Start to Write File
		{	
			do{
				DIAGEFSWriteCmdRequest dIAGEFSWriteCmdRequest;
				DIAGEFSWriteCmdResponse dIAGEFSWriteCmdResponse;
				ZeroMemory(&dIAGEFSWriteCmdRequest, sizeof(DIAGEFSWriteCmdRequest));
				ZeroMemory(&dIAGEFSWriteCmdResponse, sizeof(DIAGEFSWriteCmdResponse));
				dIAGEFSWriteCmdRequest.nDIAGcmd = DIAG_SUBSYS_CMD_F;
				dIAGEFSWriteCmdRequest.nSubsysID = DIAG_SUBSYS_EFS;
				dIAGEFSWriteCmdRequest.wCmdCodeWriteFile = EFS2_DIAG_WRITE;
				dIAGEFSWriteCmdRequest.dwFileDescripter = nFileDescripter;
				dIAGEFSWriteCmdRequest.dwOffset += (nWriteCount*FS_MAX_BUFFER_SIZE); 	

				if(FS_MAX_BUFFER_SIZE >= wDataSize)
				{
					CopyMemory(&dIAGEFSWriteCmdRequest.nData, pData, wDataSize);
					nWriteBytes = wDataSize;
					wDataSize = 0;
				}
				else
				{	
					CopyMemory(&dIAGEFSWriteCmdRequest.nData, pData, FS_MAX_BUFFER_SIZE);
					wDataSize -=FS_MAX_BUFFER_SIZE;
					nWriteBytes = FS_MAX_BUFFER_SIZE;
					pData = pData + FS_MAX_BUFFER_SIZE;
				}
				nRet = m_objsercom->TXRXDiag((CHAR*)&dIAGEFSWriteCmdRequest, nWriteBytes + 12, (CHAR*)&dIAGEFSWriteCmdResponse, sizeof(DIAGEFSWriteCmdResponse));
				nWriteCount ++;
				if(nRet < 0)
					return FALSE;
				if(dIAGEFSWriteCmdResponse.nErrNum != 0 ||dIAGEFSWriteCmdResponse.dwBytesWritten == -1)
					return FALSE;
			}while(wDataSize > 0);

			//Close File
			DIAGEFSCloseFileCmdRequest dIAGEFSCloseFileCmdRequest;
			DIAGEFSCloseFileCmdResponse dIAGEFSCloseFileCmdResponse;
			ZeroMemory(&dIAGEFSCloseFileCmdRequest, sizeof(DIAGEFSCloseFileCmdRequest));
			ZeroMemory(&dIAGEFSCloseFileCmdResponse, sizeof(DIAGEFSCloseFileCmdResponse));
			dIAGEFSCloseFileCmdRequest.nDIAGcmd = DIAG_SUBSYS_CMD_F;
			dIAGEFSCloseFileCmdRequest.nSubsysID = DIAG_SUBSYS_EFS;
			dIAGEFSCloseFileCmdRequest.wCmdCodeCloseFile = EFS2_DIAG_CLOSE;
			dIAGEFSCloseFileCmdRequest.dwFileDescripter = nFileDescripter;
			nRet = m_objsercom->TXRXDiag((CHAR*)&dIAGEFSCloseFileCmdRequest, sizeof(DIAGEFSCloseFileCmdRequest), (CHAR*)&dIAGEFSCloseFileCmdResponse, sizeof(DIAGEFSCloseFileCmdResponse));
			if( nRet < 0)
				return FALSE;
			if(dIAGEFSCloseFileCmdResponse.wCmdCodeCloseFile == EFS2_DIAG_CLOSE && dIAGEFSCloseFileCmdResponse.dwErrNum == 0)
				return TRUE;
		}		
	}
	return FALSE;

}

BOOL CQualCmd::CmdDiagEFSMkDir(CHAR* pszMkDirPath)
{
	bool bRes = false;
	int nRet = -1;
	DIAGEFSMkDirCmdRequest dIAGEFSMkDirCmdRequest = {0};
	DIAGEFSMkDirCmdResponse dIAGEFSMkDirCmdResponse = {0};

	dIAGEFSMkDirCmdRequest.nDIAGcmd = DIAG_SUBSYS_CMD_F;
	dIAGEFSMkDirCmdRequest.nSubsysID = DIAG_SUBSYS_EFS;
	dIAGEFSMkDirCmdRequest.wCmdMkDir = EFS2_DIAG_MKDIR;
	dIAGEFSMkDirCmdRequest.wCreateMode = ALLPERMS;
	CopyMemory(dIAGEFSMkDirCmdRequest.nPath, pszMkDirPath, strlen(pszMkDirPath));
	nRet = m_objsercom->TXRXDiag((CHAR*)&dIAGEFSMkDirCmdRequest, (DWORD)(7 + strlen(pszMkDirPath)), (CHAR*)&dIAGEFSMkDirCmdResponse, sizeof(DIAGEFSMkDirCmdResponse));

	if(nRet >=0 && dIAGEFSMkDirCmdResponse.wCmdMkDir == EFS2_DIAG_MKDIR)
		bRes = true;

	return bRes;
}

int CQualCmd::CmdDiagTxRx(CHAR* lpTxData, DWORD dwTxLength, CHAR* lpRxData, DWORD dwRxLength, UINT uiTimeout)
{
	return m_objsercom->TXRXDiag(lpTxData, dwTxLength, lpRxData, dwRxLength, uiTimeout);
}


int CQualCmd::CmdGetBootLoaderRevocation(UCHAR* pStatus,UCHAR* pVersion, UINT uiTimeout)
{
	int nRet = RUN_FAILED;
	GET_DL_REVOCATION cmd = {0x14};
	GET_DL_REVOCATION_RESP resp = {0};
	nRet = m_objsercom->TXRXDlGetRevocation((CHAR *)&cmd, sizeof(GET_DL_REVOCATION), (CHAR *)&resp, sizeof(GET_DL_REVOCATION_RESP), uiTimeout);
	if(ERROR_SUCCESS == nRet)
	{
		if(pStatus)
			*pStatus = resp.status;
		if(pVersion)
			*pVersion = resp.version;
	}
	return nRet;
}

int CQualCmd::CmdGetPhonecodeRevocation(UCHAR* pStatus,UCHAR* pVersion, UINT uiTimeout)
{
	int nRet = RUN_FAILED;
	GET_DL_REVOCATION cmd = {0x16};
	GET_DL_REVOCATION_RESP resp = {0};
	nRet = m_objsercom->TXRXDlGetRevocation((CHAR *)&cmd, sizeof(GET_DL_REVOCATION), (CHAR *)&resp, sizeof(GET_DL_REVOCATION_RESP), uiTimeout);
	if(ERROR_SUCCESS == nRet)
	{
		if(pStatus)
			*pStatus = resp.status;
		if(pVersion)
			*pVersion = resp.version;
	}
	return nRet;
}

int CQualCmd::CmdReset(UINT uiTimeout)
{
	int nRet = RUN_FAILED;
	DMSS_CMD_RESET_REQ req = {0};
	DMSS_CMD_RESET_RESP resp = {0};
	req.command_code = DMSS_CMD_RESET;
	nRet = m_objsercom->TXRXDl((CHAR *)&req, sizeof(DMSS_CMD_RESET_REQ), (CHAR *)&resp, sizeof(DMSS_CMD_RESET_RESP), uiTimeout);
	if(nRet>=0)
	{
		if(resp.command_code == DMSS_CMD_ACK)
			nRet = RUN_SUCCESSFULLY;
	}
	return nRet;
}