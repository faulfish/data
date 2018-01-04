#include "stdafx.h"
#include "QualcommDiag.h"
#include "Header_MotoXPRS.h"
#include "..\Lib\msm\nv_items.h"
#include "..\CommonUtil\UnicodeUtility.h"
#include "..\CommonUtil\PIMStringUtility.h"


#include <stdio.h>
#include <assert.h>

CQualcommDiag::CQualcommDiag(LPCTSTR lpszModelName, int  nComPort)
:m_bConnected(false), m_nLastError(DIAG_ERROR_SUCCESS)
{
	CString strFormat;
	strFormat.Format(_T("Test_COM%02d.txt"),nComPort);
	m_strLogFile = (LPCTSTR)strFormat;	

	TCHAR szComport[50] = {0};
	::_stprintf(szComport,_T("\\\\.\\COM%d"),nComPort);
	TRACE_FILE_LINE_INFO();
	
	if(RUN_SUCCESSFULLY == m_objsercom.OpenComport(lpszModelName,szComport))
	{
		m_bConnected = m_objsercom.IsDLorDIAGMode();
	}
	else
	{
		AddLog(_T("m_objsercom.OpenComport() fail"));
	}
	TRACE_FILE_LINE_INFO();
}

CQualcommDiag::~CQualcommDiag(void)
{
	//AddLog("CQualcommDiag::~CQualcommDiag");
}

void CQualcommDiag::AddLog(const TSTRING& strMessage)
{
#ifndef DISABLE_LOG
	LogPolicy::Log(strMessage.c_str(),m_strLogFile.c_str());
#endif//#ifndef DISABLE_LOG
}

bool CQualcommDiag::IsConnected()
{
	m_bConnected = m_objsercom.IsDLorDIAGMode();
	return m_bConnected;
}
// Get MotoXPRS by spec
bool CQualcommDiag::GetMotoXPRS(char* szReceiverBuffer,int nBufferLength,int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		int nMotoXPRSDATASize = sizeof(MotoXPRSDATA);
		if(nBufferLength >= nMotoXPRSDATASize )
		{
			char szBuf[7]={0x4B,0x43,0x1,0x0,0xB6,0xDA,0x7E};		
			m_objsercom.MakesureToDIAGMode();

			int nReceivered = m_objsercom.TxRx(szBuf,sizeof(szBuf),szReceiverBuffer,nMotoXPRSDATASize,nTimeOut);
			if(nReceivered >= 0)
				bRes = true;
		}
	}
	else
	{
		AddLog(_T("GetMotoXPRS fail"));
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	}
	return bRes;
}

int CQualcommDiag::HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize)
{
	int nRet = 0;
	if (pucHexBcdBuf && (iHexBcdBufSize > 0) && sDigitBuf && (iDigitBufSize>0) && iHexBcdBufSize*2 <= iDigitBufSize)
	{
		BYTE ucLowByte, ucHighByte;
		int i = 0;
		for (i = 0; i < iHexBcdBufSize; i++)
		{
			ucLowByte = *(pucHexBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');

			ucHighByte = (*(pucHexBcdBuf+i)&0xF0)>>4;
			*(sDigitBuf+(2*i+1)) = (ucHighByte>=10) ? ((ucHighByte-10)+'a') : (ucHighByte+'0');
		} 

		if (iHexBcdBufSize%2)
		{
			ucLowByte = *(pucHexBcdBuf+i)&0x0F;
			*(sDigitBuf+(2*i)) = (ucLowByte>=10) ? ((ucLowByte-10)+'a') : (ucLowByte+'0');
		}
		nRet = (iHexBcdBufSize%2) ? (2*i+1) : (2*i);
	}
	return nRet;
}

int CQualcommDiag::DigitToHexBcd(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize)
{
	int nRet = 0;
	if(sDigitBuf && pucHexBcdBuf && iHexBcdBufSize >= (iDigitBufSize-1)/2)
	{
		for(int i=0; i< (iDigitBufSize-1)/2; i++)
		{
			char szTemp[3] = {0};
			szTemp[0] = sDigitBuf[i*2];
			szTemp[1] = sDigitBuf[i*2+1];
			pucHexBcdBuf[(iDigitBufSize-1)/2 -i -1] = QSYNC_LIB::HEX2Ineger((LPCTSTR)szTemp);
		}
	}
	
	return nRet;
}

bool CQualcommDiag::ReadIMEI(char* szIMEI100,int nTimeOut)
{
	bool bRes = false;
	TRACE_FILE_LINE_INFO();
	if(szIMEI100)
	{
		memset(szIMEI100, 0, sizeof(szIMEI100));
		DIAG_NVITEM_PACKET_F_type ReadResp = {0};
		TRACE_FILE_LINE_INFO();
		if(ReadNVItem(NV_UE_IMEI_I, ReadResp, nTimeOut))
		{
			TRACE_FILE_LINE_INFO();
			if(ReadResp.nv_stat==0)
			{
				TRACE_FILE_LINE_INFO();
				char sDigitIMEI[128*2+1] = {0};
				HexBcdToDigit(ReadResp.item_data, 128, sDigitIMEI, 128*2+1);
				
				for(int i=0; i<15; i++)
					szIMEI100[i] = sDigitIMEI[i+3];
				int nIMEI = 0;
				for(int i=0; i<15; i++)
				{
					if(szIMEI100[i] == '0')
						++nIMEI;
				}
				if(nIMEI == 15)
					sprintf(szIMEI100, "000000011234560");
				bRes = true;
				TRACE_FILE_LINE_INFO();
			}
			else
			{
				AddLog(_T("DIAG_ERROR_READ_NVITEM_INVALID_STATUS"));
				m_nLastError = DIAG_ERROR_READ_NVITEM_INVALID_STATUS;
			}
		}
		else
		{
			if(ReadResp.nv_stat==5)
			{
				TRACE_FILE_LINE_INFO();
				sprintf(szIMEI100, "000000011234560");
				bRes = true;
			}
		}
	}
	else
	{
		AddLog(_T("ReadIMEI fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;	
	}
	return bRes;
}

bool CQualcommDiag::WriteIMEI(char* szIMEI, int nTimeOut)
{
	bool bRes = false;
	if(szIMEI)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
		WriteReq.item = NV_UE_IMEI_I;
		int nLength = strlen(szIMEI);
		char szReverseIMEI[19] = {0};
		szReverseIMEI[15] = 'A';
		szReverseIMEI[16] = '0';
		szReverseIMEI[17] = '8';
		for(int i=0; i<15; i++)
		{
			szReverseIMEI[14-i] = szIMEI[i];			
		}
		BYTE byteIMEI[9]= {0};
		DigitToHexBcd(szReverseIMEI, 19, WriteReq.item_data, 9);

		TRACE_FILE_LINE_INFO();
		if(WriteNVItem(WriteReq, nTimeOut))	
		{
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
	{
		AddLog(_T("WriteIMEI fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}

bool CQualcommDiag::IsTestMode(int nTimeOut)
{
	bool bRes = false;
	TRACE_FILE_LINE_INFO();
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	if(ReadNVItem(NV_FTM_MODE_I, ReadResp, nTimeOut))
	{
		TRACE_FILE_LINE_INFO();
		if(ReadResp.item_data[0] == 1)
		{	
			bRes = true;
		}
		else
			m_nLastError = DIAG_ERROR_READ_NVITEM_INVALID_STATUS;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::IsEMU(int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		TRACE_FILE_LINE_INFO();
		diagpkt_IsEMU req = {0};
		diagpkt_IsEMU resp = {0};
		req.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_PM;
		req.subsys_cmd_code = 5;

		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0 && resp.IsEMU == 1)
		{
			bRes = true;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
	{
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	}
	return bRes;
}

bool CQualcommDiag::SetTestMode(int nTimeOut)
{
	bool bRes = false;
	TRACE_FILE_LINE_INFO();
	if(!IsTestMode(nTimeOut))
	{
		TRACE_FILE_LINE_INFO();
		/*if(Cmd_DIAG_CONTROL_F(MODE_FTM_F, nTimeOut))
			bRes = true;*/

		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
		WriteReq.item = NV_FTM_MODE_I;
		WriteReq.item_data[0] = 1; //bFTMMode = (1 = FTM mode), (0 = ONLINE mode)  
		TRACE_FILE_LINE_INFO();
		if(WriteNVItem(WriteReq, nTimeOut))	
		{
			TRACE_FILE_LINE_INFO();
			if(Reset(nTimeOut))
			{
				TRACE_FILE_LINE_INFO();
				bRes = true;
			}
		}
	}
	else
	{
		bRes = true;
		TRACE_FILE_LINE_INFO();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::WriteBTAddr(char* szAddress, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(szAddress)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
		WriteReq.item = NV_BD_ADDR_I;
		int nLength = strlen(szAddress);
		if(nLength > 12)
		{
			for(int i=12; i<nLength; i++)
				szAddress[i] = 0;
		}
		DigitToHexBcd(szAddress, 13, WriteReq.item_data, 6);
		TRACE_FILE_LINE_INFO();
		if(WriteNVItem(WriteReq, nTimeOut))	
		{
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
	{
		AddLog(_T("WriteBTAddr fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}
bool CQualcommDiag::ReadBTAddr(char* szAddress100, long& nLength, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	nLength = 0;
	if(szAddress100)
	{
		DIAG_NVITEM_PACKET_F_type ReadResp;
		memset(szAddress100, 0, sizeof(szAddress100));
		if(ReadNVItem(NV_BD_ADDR_I, ReadResp, nTimeOut))
		{
			TRACE_FILE_LINE_INFO();
			if(ReadResp.nv_stat == 0)
			{
				HexBcdToDigit(ReadResp.item_data, 6, szAddress100, 6*2+1);
				szAddress100 = _strrev(szAddress100);
				nLength = 12;
				
				TRACE_FILE_LINE_INFO();
				bRes = true;
			}
			else if(ReadResp.nv_stat == 5)
			{
				m_nLastError = DIAG_ERROR_READ_NVITEM_NOTACTIVE;
				TRACE_FILE_LINE_INFO();
			}
			else
			{
				m_nLastError = DIAG_ERROR_READ_NVITEM_INVALID_STATUS;
				TRACE_FILE_LINE_INFO();
			}
		}
	}
	else
	{
		AddLog(_T("ReadBTAddr fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}


bool CQualcommDiag::ReadNVItem(const WORD& NVItem, DIAG_NVITEM_PACKET_F_type& ReadResp, int nTimeOut)
{
	bool bRes = false;
	TRACE_FILE_LINE_INFO();
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		TRACE_FILE_LINE_INFO();
		memset(&ReadResp, 0, sizeof(ReadResp));
		
		DIAG_NVITEM_PACKET_F_type ReadReq = {0};
		ReadReq.nDIAGcmd = DIAG_NV_READ_F;
		ReadReq.item = NVItem;
		//in case if above statement is wrong
		//ReadReq.item[0] = NVItem & 0x00FF;
		//ReadReq.item[1] = (NVItem & 0xFF00) >> 8;

		int nReceivered = m_objsercom.TxRx((char*)&ReadReq,sizeof(ReadReq),(char*)&ReadResp,sizeof(ReadResp),nTimeOut);
		if(nReceivered >= 0)
		{
			if(ReadReq.nDIAGcmd ==DIAG_NV_READ_F && ReadResp.nv_stat == NV_DONE_S)
			{
				bRes = true;
			}
			else
			{
				m_nLastError = DIAG_ERROR_READ_NVITEM_INVALID_STATUS;
			}
		}
		else
		{
			TRACE_FILE_LINE_INFO();
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
		}
	}
	else
	{
		AddLog(_T("ReadNVItem fail"));
		TRACE_FILE_LINE_INFO();
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::WriteNVItem(DIAG_NVITEM_PACKET_F_type& WriteReq, int nTimeOut)
{ 
	bool bRes = false;
	TRACE_FILE_LINE_INFO();
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		TRACE_FILE_LINE_INFO();
		DIAG_NVITEM_PACKET_F_type WriteResp = {0};
		
		WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;

		int nReceivered = m_objsercom.TxRx((char*)&WriteReq,sizeof(WriteReq),(char*)&WriteResp,sizeof(WriteResp),nTimeOut);
		if(nReceivered >= 0)
		{
			switch(WriteResp.nv_stat)
			{
			case 0:
				TRACE_FILE_LINE_INFO();
				bRes = true;
				break;
			case 1:
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_INTERNAL_DMSS_USED;
				break;
			case 2: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_UNRECOGNIZED_COMMAND;
				break;
			case 3: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_FULL_NV_MEMORY;
				break;
			case 4: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_COMMAND_FAIL;
				break;
			case 5: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_VARIABLE_NOT_ACTIVE;
				break;
			case 6: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_BAD_PARAMETER_IN_COMMAND_BLOCK;
				break;
			case 7: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_READ_ONLY;
				break;
			case 8: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_ITEM_NOT_DEFINED;
				break;
			case 9: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_FREE_MEMORY_EXHAUSTED;
				break;
			case 10: 
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_INTERNAL_USE;
				break;
			default:	
				TRACE_FILE_LINE_INFO();
				m_nLastError = DIAG_ERROR_WRITE_NVITEM_ERROR;
			}
		}
		else
		{
			AddLog(_T("WriteNVItem fail"));
			TRACE_FILE_LINE_INFO();
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
		}
	}
	else
	{
		TRACE_FILE_LINE_INFO();
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::Write1NVValue(WORD item, BYTE item_value, int nTimeOut)
{
	bool bRes = false;
	TRACE_FILE_LINE_INFO();
	
	DIAG_NVITEM_PACKET_F_type WriteReq = {0};
	WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
	WriteReq.item = item;
	WriteReq.item_data[0] = item_value; 
	TRACE_FILE_LINE_INFO();
	if(WriteNVItem(WriteReq, nTimeOut))	
	{
		TRACE_FILE_LINE_INFO();
		bRes = true;
	}
	else
	{
		AddLog(_T("Write1NVValue fail"));
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::Restart(int nTimeOut, bool bWaitForRestart)
{
	AddLog(_T("CQualcommDiag::Restart()"));
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	
	DIAG_NVITEM_PACKET_F_type WriteReq = {0};
	WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
	WriteReq.item = NV_FTM_MODE_I;
	WriteReq.item_data[0] = 0; //bFTMMode = (1 = FTM mode), (0 = ONLINE mode)  
	TRACE_FILE_LINE_INFO();
	if(WriteNVItem(WriteReq, nTimeOut))	
	{
		TRACE_FILE_LINE_INFO();
		if(Cmd_DIAG_CONTROL_F(MODE_OFFLINE_D_F, nTimeOut))
		{
			TRACE_FILE_LINE_INFO();
			if(Cmd_DIAG_CONTROL_F(MODE_RESET_F, nTimeOut))
			{
				if(bWaitForRestart)
				{
					TRACE_FILE_LINE_INFO();
					Sleep(20000);
					TRACE_FILE_LINE_INFO();
				}
				bRes = true;
			}
		}
	}

	if(bRes == false)
	{
		AddLog(_T("CQualcommDiag::Restart() fail"));
	}
	
	return bRes;
}
bool CQualcommDiag::Reset(int nTimeOut, bool bWaitForRestart)
{
	AddLog(_T("CQualcommDiag::Reset()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;

	if(Cmd_DIAG_CONTROL_F(MODE_POWER_OFF_F, nTimeOut))
	{
		if(bWaitForRestart)
		{
			TRACE_FILE_LINE_INFO();
			Sleep(20000);
			TRACE_FILE_LINE_INFO();
		}
		bRes = true;
	}
	else
	{
		AddLog(_T("CQualcommDiag::Reset() fail"));
	}
	return bRes;
}

bool CQualcommDiag::Cmd_DIAG_CONTROL_F(int nMode, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		TRACE_FILE_LINE_INFO();
		DIAG_CONTROL_F_PACKET_type Req = {0};
		DIAG_CONTROL_F_PACKET_type Resp = {0};
		Req.nDIAGcmd = DIAG_CONTROL_F;
		Req.nMode = nMode;
		int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
		else
		{
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
		}
	}
	else
	{
		TRACE_FILE_LINE_INFO();
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	}
	return bRes;
}

bool CQualcommDiag::IsUnitOn()
{
	AddLog(_T("CQualcommDiag::IsUnitOn()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	
	CSerialIO::EState state = m_objsercom.GetState();
	if(state != CSerialIO::EStart && state != CSerialIO::EStop)
	{
		bRes = true;
		TRACE_FILE_LINE_INFO();
	}
	else
	{
		m_nLastError = DIAG_ERROR_MODE_UNDETECTED;
		TRACE_FILE_LINE_INFO();
	}
	
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::IsDLMode()
{
	AddLog(_T("CQualcommDiag::IsDLMode()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	CSerialIO::EState state = m_objsercom.GetState();
	if(state == CSerialIO::EDL)
	{
		bRes = true;
		TRACE_FILE_LINE_INFO();
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::IsUnitOff()
{
	AddLog(_T("CQualcommDiag::IsUnitOff()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	bRes = IsUnitOn()? false: true;
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::ReadFlexVersion(char* szReadValue1024, long& nLength, int nTimeOut)
{
	AddLog(_T("CQualcommDiag::ReadFlexVersion()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	nLength = 0;
	if(szReadValue1024)
	{
		memset(szReadValue1024, 0, sizeof(szReadValue1024));
		DIAG_NVITEM_PACKET_F_type ReadResp;
		if(ReadNVItem(NV_CFGI_PID_SYS_EFS_FILE_NAME, ReadResp, nTimeOut)) //mobicon ini name
		{
			TRACE_FILE_LINE_INFO();
			if(ReadResp.nv_stat == 0)
			{
				memcpy(szReadValue1024, ReadResp.item_data, 128);
				int nLen = strlen(szReadValue1024);
				if( szReadValue1024[nLen-7] == '.' && (szReadValue1024[nLen-6] == 'q' || szReadValue1024[nLen-6] == 'Q') 
				 && (szReadValue1024[nLen-5] == 'v' || szReadValue1024[nLen-5] == 'V') && (szReadValue1024[nLen-4] == 'c' || szReadValue1024[nLen-4] == 'C')
				 && (szReadValue1024[nLen-3] == 'e' || szReadValue1024[nLen-3] == 'E') && (szReadValue1024[nLen-2] == 'f' || szReadValue1024[nLen-2] == 'F')
				 && (szReadValue1024[nLen-1] == 's' || szReadValue1024[nLen-1] == 'S'))
				{
					for(int i=1; i<8; i++)
						szReadValue1024[nLen-i] = 0;
					TRACE_FILE_LINE_INFO();
				}
				nLength = strlen(szReadValue1024);
				TRACE_FILE_LINE_INFO();
				bRes = true;
			}
			else if(ReadResp.nv_stat == 5)
				m_nLastError = DIAG_ERROR_READ_NVITEM_NOTACTIVE;			
			else
				m_nLastError = DIAG_ERROR_READ_NVITEM_INVALID_STATUS;
				
		}
	}

	if(bRes == false)
		AddLog(_T("CQualcommDiag::ReadFlexVersion() fail"));

	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::ReadSWVersion(char* szReadValue1024, long& nLength, int nTimeOut)
{
	AddLog(_T("CQualcommDiag::ReadSWVersion()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	nLength = 0;
	if(szReadValue1024)
	{
		memset(szReadValue1024, 0, sizeof(szReadValue1024));
		BYTE DIAGcmd = DIAG_EXT_BUILD_ID_F;
		toolsdiag_ext_build_id_rsp_type Resp = {0};
		m_objsercom.MakesureToDIAGMode();
		int nReceivered = m_objsercom.TxRx((char*)&DIAGcmd,sizeof(DIAGcmd),(char*)&Resp,sizeof(Resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			strcpy(szReadValue1024, Resp.ver_strings);
			nLength = strlen(szReadValue1024);
			bRes = true;
		}
		else
		{
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
		}
	}
	else
	{
		AddLog(_T("CQualcommDiag::ReadSWVersion() fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::ReadFSN(BYTE* szFSN16, int nTimeOut)
{
	AddLog(_T("CQualcommDiag::ReadFSN()"));

	bool bRes = false;
	if(szFSN16)
	{
		TRACE_FILE_LINE_INFO();
		DIAG_NVITEM_PACKET_F_type Resp = {0};
		if(ReadNVItem(NV_DEVICE_SERIAL_NO_I, Resp, nTimeOut))
		{
			TRACE_FILE_LINE_INFO();
			for(int i=0; i<16; i++)
				szFSN16[i] = Resp.item_data[i];
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
	{
		AddLog(_T("CQualcommDiag::ReadFSN() fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::WriteFSN(BYTE* szFSN16, int nTimeOut)
{
	AddLog(_T("CQualcommDiag::WriteFSN()"));

	bool bRes = false;
	if(szFSN16)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
		WriteReq.item = NV_DEVICE_SERIAL_NO_I;
		for(int i=0; i<16; i++)
			WriteReq.item_data[i] = szFSN16[i];

		if(WriteNVItem(WriteReq, nTimeOut))	
		{
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
	{
		AddLog(_T("CQualcommDiag::WriteFSN() fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}

bool CQualcommDiag::GenFSN(BYTE* szFSN16)
{
	AddLog(_T("CQualcommDiag::GenFSN()"));

	bool bRes = false;
	if(szFSN16)
	{
		srand( (unsigned int)GetTickCount() );
		for(int i=0; i<8; i++)
		{
			
			int nFSN = rand();
			szFSN16[i*2]   =  nFSN & 0x00FF;
			szFSN16[i*2+1] = (nFSN & 0xFF00) >> 8;
		}
		bRes = true;
	}
	else
	{
		AddLog(_T("CQualcommDiag::GenFSN() fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}

//bool CQualcommDiag::GenSimData(const BYTE *szImei, const BYTE *szFsn, const BYTE *szMasterCode, const BYTE *szSubsidyCode, 
//				const BYTE *szLockPath, BYTE *simEncryptedDataBuf, const int simEncryptedDataBufLen)
//{
//	bool bRes = false;
//
//	if(szImei && szFsn && szMasterCode && szSubsidyCode && szLockPath && simEncryptedDataBuf)
//	{
//
//		char szFSN33[33] = {0};
//		HexBcdToDigit((BYTE*)szFsn, 16, szFSN33, 33);
//		for(int i=0; i<16; i++)
//		{
//			char cTemp = szFSN33[i*2];
//			szFSN33[i*2] = szFSN33[i*2+1];
//			szFSN33[i*2+1] = cTemp;
//		}
//		//int nGen = Lib_ISimEnc_GenSimFile(szImei, (const BYTE*)szFSN33, szMasterCode, szSubsidyCode, szLockPath, simEncryptedDataBuf, simEncryptedDataBufLen);
//		int nGen = ISimEnc_GenSimFile(szImei, (const BYTE*)szFSN33, szMasterCode, szSubsidyCode, szLockPath, simEncryptedDataBuf, simEncryptedDataBufLen);
//		bRes = nGen ==0;
//	}
//	else
//	{
//		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
//		TRACE_FILE_LINE_INFO();
//	}
//	return bRes;
//}

bool CQualcommDiag::WriteSimData(BYTE *pSimDataBuf, int nSimDataBufLen, int nTimeOut)
{
	AddLog(_T("CQualcommDiag::WriteSimData()"));

	bool bRes = false;
	if(pSimDataBuf)
	{
		GSIDDIAG_SIM_SECURE_STORE_REQ Req = {0};
		GSIDDIAG_SIM_SECURE_STORE_RESP Resp = {0};
		Req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		Req.subsys_id = DIAG_SUBSYS_GSDI;
		Req.subcommand = 87;
		Req.aps_as_transport =  1;
		Req.subsys_cmd_code = 72; //GSDIDIAG_SIM_SECURE_STORE_CMD
		Req.slot_id = 0x01;
		Req.sim_app_in_len = 16;
		Req.simsecure_data_len = nSimDataBufLen;
		memcpy(Req.simsecure_data, pSimDataBuf, nSimDataBufLen);

		int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut, true);
		if(nReceivered >= 0)
		{
			if(Resp.status == 0)
			{
				bRes = true;
			}
			else
				m_nLastError = DIAG_ERROR_SIMDATA_UNEXPECTED_RESPONSE;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;

	}
	else
	{
		AddLog(_T("CQualcommDiag::WriteSimData() fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}

bool CQualcommDiag::ReadTransceiverKitNumber(char* szTKN100, int nTimeOut)
{ // 104-113 TRANCEIVER NO (10bytes)
	AddLog(_T("CQualcommDiag::ReadTransceiverKitNumber()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(szTKN100)
	{
		TRACE_FILE_LINE_INFO();
		DIAG_NVITEM_PACKET_F_type Resp = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, Resp, nTimeOut))
		{
			BYTE szTKN[11] = {0};
			TRACE_FILE_LINE_INFO();
			for(int i=104; i<114; i++)
				szTKN[i-104] = Resp.item_data[i];
			TRACE_FILE_LINE_INFO();
			strcpy(szTKN100, (char*)szTKN);
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
	{
		AddLog(_T("CQualcommDiag::ReadTransceiverKitNumber() fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::WriteTransceiverKitNumber(char* szTKN100, int nTimeOut)
{ // 104-113 TRANCEIVER NO (10bytes)
	AddLog(_T("CQualcommDiag::WriteTransceiverKitNumber()"));

	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(szTKN100)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, WriteReq, nTimeOut))
		{
			int nLength = strlen(szTKN100);
			if(nLength == 10)
			{
				for(int i=104; i<114; i++)
					WriteReq.item_data[i] = szTKN100[i-104];

				if(WriteNVItem(WriteReq, nTimeOut))	
				{
					TRACE_FILE_LINE_INFO();
					bRes = true;
				}
			}
		}
	}
	else
	{
		AddLog(_T("CQualcommDiag::WriteTransceiverKitNumber() fail"));
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	}
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::ReadTrackID(char* szTrackID, int nTimeOut)
{ // 94-103 PICASSO NO (10bytes)
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(szTrackID)
	{
		DIAG_NVITEM_PACKET_F_type Resp = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, Resp, nTimeOut))
		{
			BYTE szTID[11] = {0};
			TRACE_FILE_LINE_INFO();
			for(int i=94; i<104; i++)
				szTID[i-94] = Resp.item_data[i];
			TRACE_FILE_LINE_INFO();
			strcpy(szTrackID, (char*)szTID);

			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	TRACE_FILE_LINE_INFO();
	return bRes;

}
bool CQualcommDiag::WriteTrackID(char* szTrackID, int nTimeOut)
{ // 94-103 PICASSO NO (10bytes)
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(szTrackID && strlen(szTrackID) ==10)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, WriteReq, nTimeOut))
		{	
			for(int i=94; i<104; i++)
				WriteReq.item_data[i] = szTrackID[i-94];

			if(WriteNVItem(WriteReq, nTimeOut))	
			{
				TRACE_FILE_LINE_INFO();
				bRes = true;
			}
		}
	}
	else
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::ReadFactoryInforBlock(char* szBlock, long& nLength, int nTimeOut)
{ // 3-4 Test-Byte
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(szBlock)
	{
		memset(szBlock, 0 , sizeof(szBlock));
		DIAG_NVITEM_PACKET_F_type Resp = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, Resp, nTimeOut))
		{
			BYTE szTemp = 0;
			TRACE_FILE_LINE_INFO();
			szTemp= Resp.item_data[3];
			TRACE_FILE_LINE_INFO();
			char szBlockTmp[1*2+1] = {0};
			HexBcdToDigit(&szTemp, 1, szBlockTmp, 1*2+1);	
			szBlock[0] = szBlockTmp[1];
			szBlock[1] = szBlockTmp[0];
			szBlock[2] = '\0';
			nLength = (int)strlen(szBlock);
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::WriteFactoryInforBlock(char* szBlock, int nTimeOut)
{	// 3-4 Test-Byte
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(szBlock && strlen(szBlock)>=2)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		if(ReadNVItem(NV_FACTORY_DATA_1_I, WriteReq, nTimeOut))
		{
			char szBlockTemp[2] = {0};
			for(int i=0; i<2; i++)
				szBlockTemp[i] = szBlock[i];
			BYTE byteTmp[2] = {0};
			DigitToHexBcd(szBlockTemp, 3, byteTmp, 1);

			WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
			WriteReq.item_data[3] = byteTmp[0];
			WriteReq.item_data[4] = 0;
			//for(int i=0; i<2; i++)
			//{
			if(WriteNVItem(WriteReq, nTimeOut))	
			{
				TRACE_FILE_LINE_INFO();
				bRes = true;
			}
			//}
		}
	}
	else
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		
	return bRes;
}

bool CQualcommDiag::CloseConnection(int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		diagpkt_subsys_hdr_type Req = {0};
		diagpkt_subsys_hdr_type Resp = {0};
		Req.cmd_code = DIAG_SUBSYS_CMD_F;
		Req.subsys_id = DIAG_SUBSYS_USB;
		Req.subsys_cmd_code = 1000;

		int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut);
		if(nReceivered >= 0)
		{
			if(Resp.cmd_code == Req.cmd_code && Resp.subsys_id == Req.subsys_id && Resp.subsys_cmd_code == Req.subsys_cmd_code)
			{
				bRes = true;
			}
			else
				m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::TurnOnOffEM(bool bOpen, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	BYTE item_value = bOpen ? 1 : 0;
	if(/*Write1NVValue(NV_QISDA_EM__ENABLE_PHONE_ID_INFO_I,		item_value, nTimeOut)
	&& Write1NVValue(NV_QISDA_EM__ENABLE_SWHW_INFO_CUSTOMER_I,  item_value, nTimeOut)
	&& Write1NVValue(NV_QISDA_EM__ENABLE_SWHW_INFO_RD_I,		item_value, nTimeOut)
	&&*/ Write1NVValue(NV_QISDA_EM__ENABLE_MFG_TEST_I,			item_value, nTimeOut)
	&& Write1NVValue(NV_QISDA_EM__ENABLE_TEXT_LABEL_TEST_I,		item_value, nTimeOut)
	&& Write1NVValue(NV_QISDA_EM__ENABLE_LCD_TEST_I,			item_value, nTimeOut)
	&& Write1NVValue(NV_QISDA_EM__ENABLE_SLT_ALT_TEST_I,		item_value, nTimeOut)
	//&& Write1NVValue(NV_QISDA_EM__ENABLE_LANGUAGE_SWITCH_I,		item_value, nTimeOut)
	&& Write1NVValue(NV_QISDA_EM__ENABLE_EFEM_I,				item_value, nTimeOut)
	/*&& Write1NVValue(NV_QISDA_EM__ENABLE_OTHERS_I,				item_value, nTimeOut)*/)
	{
		bRes = true;
	}

	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::SetRTC(int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		bool bNeedRestart = !IsTestMode(nTimeOut);
		if(!bNeedRestart || SetTestMode(nTimeOut))
		{
			COleDateTime CurrentTime(COleDateTime::GetCurrentTime());
			COleDateTime time;
			if(QSYNC_LIB::ConvertTime(CurrentTime, time, false))
			{
				ftm_pmic_pkt_type Req = {0};
				ftm_pmic_pkt_type Resp = {0};
				Req.cmd_code = DIAG_SUBSYS_CMD_F;
				Req.subsys_id = DIAG_SUBSYS_FTM;
				Req.subsys_cmd_code = 11;
				Req.Mode = 2; // 24hr
				Req.Month = time.GetMonth();
				Req.Day = time.GetDay();
				Req.YEAR = time.GetYear();
				Req.Hour = time.GetHour();
				Req.Min = time.GetMinute();
				Req.Sec = time.GetSecond();

				int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut);
				if(nReceivered >= 0)
				{
					TRACE_FILE_LINE_INFO();
					if(Resp.unknown[1] == 0x10)
					{
						if( Write1NVValue(NV_SECTIME_TIME_OFFSETS_I, 0, nTimeOut) /*&& 
							Write1NVValue(NV_TIME_TOD_OFFSET_I, 0, nTimeOut)*/)
						{	
							if(bNeedRestart)
								bRes = Restart(nTimeOut);
							else
								bRes = true;
						}
					}
					else
						m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
				}
				else
					m_nLastError = DIAG_ERROR_TXRX_FAIL;
			}
		}	
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::MasterClear(int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(Write1NVValue(NV_QISDA_SET_TOOL_MASTERCLEAR_ENABLED_I, 1, nTimeOut))
	{
		Sleep(60000);
		m_bConnected = m_objsercom.MakesureToDIAGMode();
		bRes = true;
	}
	
	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::IsSPCLocked(long& nStatus, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	nStatus = 0;
	
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	TRACE_FILE_LINE_INFO();
	if(ReadNVItem(NV_SEC_CODE_I, ReadResp, nTimeOut))
	{
		if(ReadResp.nDIAGcmd == 0x26)
		{
			bool bIsDefault = true;
			for(int i=0; i<6 && bIsDefault; i++)
				bIsDefault = ReadResp.item_data[i] == 0;
			if(bIsDefault)
				nStatus = 2;
		}
		else if(ReadResp.nDIAGcmd == 0x42)
		{
			nStatus = 1;
		}
		bRes = true;
	}

	TRACE_FILE_LINE_INFO();
	return bRes;
}
bool CQualcommDiag::ProgramSPC(char* szLock, int nTimeOut)
{
	bool bRes = false;
	if(szLock && strlen(szLock) >= 6)
	{
		DIAG_NVITEM_PACKET_F_type WriteReq = {0};
		WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
		WriteReq.item = NV_SEC_CODE_I;
		for(int i=0; i<6; i++)
			WriteReq.item_data[i] = szLock[i];

		if(WriteNVItem(WriteReq, nTimeOut))	
		{
			TRACE_FILE_LINE_INFO();
			bRes = true;
		}
	}
	else
	{
		m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		TRACE_FILE_LINE_INFO();
	}
	return bRes;
}

bool CQualcommDiag::IsNWSCPLocked(bool& bNWUnlocked, bool& bNSUnlocked, bool& bSPUnlocked, bool& bCPUnlocked, bool& bSIMUnlocked, int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		TRACE_FILE_LINE_INFO();
		diagpkt_subsidy_lock_req Req = {0};
		diagpkt_subsidy_lock_resp Resp = {0};
		Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		Req.subsys_id = DIAG_SUBSYS_GSDI;
		Req.subsys_cmd_code = 0x42;  // GSDI_GET_FEATURE_INDICATION_CMD

		Req.status = 0;
		Req.delayed_rsp_id = 0;
		Req.rsp_cnt = 0;		
		Req.sim_slot_number = 1;     // GSDI_SLOT1

		int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(Resp.cmd_code == DIAG_SUBSYS_CMD_VER_2_F && Resp.status == 0)
			{
				bNWUnlocked = Resp.network_personalization_status == 0;
				bNSUnlocked = Resp.network_subsuet_personalization_status == 0;
				bSPUnlocked = Resp.service_provider_personalization_status ==0;
				bCPUnlocked = Resp.corporation_personalization_status == 0;
				bSIMUnlocked = Resp.sim_personalization_status ==0;
				bRes = true;
			}
			else
				m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::QuerySimDataStatus(bool& bExist, int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		TRACE_FILE_LINE_INFO();
		diagpkt_subsidy_lock_req Req = {0};
		diagpkt_subsidy_lock_resp Resp = {0};
		Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		Req.subsys_id = DIAG_SUBSYS_GSDI;
		Req.subsys_cmd_code = 0x42;  // GSDI_GET_FEATURE_INDICATION_CMD
		Req.status = 0;
		Req.delayed_rsp_id = 0;
		Req.rsp_cnt = 0;		
		Req.sim_slot_number = 1;     // GSDI_SLOT1

		int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut, true);
		if(nReceivered >= 0)
		{
			if(Resp.cmd_code ==DIAG_SUBSYS_CMD_VER_2_F && 
				Resp.subsys_id == DIAG_SUBSYS_GSDI &&
				Resp.subsys_cmd_code == 0x42)
			{
				bExist = Resp.status ==0;
				bRes = true;
			}
			else
				m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::IsUnitLocked(long& nStatus, int nLockType/*0=NWSCP, 1=SSCP*/,int nTimeOut)
{
	/* nStatus
	0 = Unlocked
	1 = Locked
	2 = Unlocked by default Service Password
	*/
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	nStatus = 0;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		TRACE_FILE_LINE_INFO();
		diagpkt_subsidy_lock_req Req = {0};
		diagpkt_subsidy_lock_resp Resp = {0};
		Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		Req.subsys_id = DIAG_SUBSYS_GSDI;
		Req.subsys_cmd_code = 0x42;  // GSDI_GET_FEATURE_INDICATION_CMD

		Req.status = 0;
		Req.delayed_rsp_id = 0;
		Req.rsp_cnt = 0;		
		Req.sim_slot_number = 1;     // GSDI_SLOT1

		int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut, true);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(Resp.cmd_code ==DIAG_SUBSYS_CMD_VER_2_F && 
				Resp.subsys_id == DIAG_SUBSYS_GSDI &&
				Resp.subsys_cmd_code == 0x42 && Resp.status == 0)
			{
				if(nLockType == 0)
				{
					bool bIsUnlocked = Resp.network_personalization_status == 0 &&
									   Resp.network_subsuet_personalization_status == 0 &&
									   Resp.service_provider_personalization_status ==0 &&
									   Resp.corporation_personalization_status == 0 && 
									   Resp.sim_personalization_status ==0;
					nStatus = bIsUnlocked ? 0 : 1;
					TRACE_FILE_LINE_INFO();
				}
				else if(nLockType == 1)
				{
					nStatus = Resp.antitheft_personalization_status == 0 ? 0 : 1;
					TRACE_FILE_LINE_INFO();
				}
				bRes = true;
			}
			else
				m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::UnLockSPC(char* szLock, bool& bUnlockOK, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		if(strlen(szLock) >=6)
		{
			diagpkt_sec_code_req_type Req = {0};
			diagpkt_sec_code_resp_type Resp = {0};
			Req.cmd_code = DIAG_SPC_F;
			for(int i=0; i<6; i++)
				Req.digits[i] = szLock[i];
			TRACE_FILE_LINE_INFO();

			int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut, true);
			if(nReceivered >= 0)
			{
				TRACE_FILE_LINE_INFO();
				bUnlockOK = Resp.sec_code_ok;
				if(!bUnlockOK)
					m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
				bRes = true;
			}
			else
				m_nLastError = DIAG_ERROR_TXRX_FAIL;
		}
		else
		{
			m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		}
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::UnLockUnit(int nLockType, char* szLock, bool& bUnlockOK, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		diagpkt_subsidy_unlock_req Req = {0};
		diagpkt_subsidy_unlock_resp Resp = {0};
		Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
		Req.subsys_id = DIAG_SUBSYS_GSDI;
		Req.subsys_cmd_code = 0x41;
		Req.status = 0;			
		Req.delayed_rsp_id = 0;	
		Req.rsp_cnt = 0;		
		Req.sim_slot_number = 1;
		if(nLockType == 0)
			Req.lock_type = GSDIDIAG_PERSO_NW;
		else if(nLockType == 1)
			Req.lock_type = GSDIDIAG_PERSO_NS;
		else if(nLockType == 2)
			Req.lock_type = GSDIDIAG_PERSO_SP;
		else if(nLockType == 3)
			Req.lock_type = GSDIDIAG_PERSO_CP;
		else if(nLockType == 4)
			Req.lock_type = GSDIDIAG_PERSO_SIM;
		else if(nLockType == 5)
			Req.lock_type =	GSDIDIAG_PERSO_PROP1;
		else
			Req.lock_type = GSDIDIAG_PERSO_NW; //set default
		
		strcpy(Req.szLock, szLock);
		int nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut, true);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			bUnlockOK = Resp.status==0;
			if(!bUnlockOK)
				m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
			bRes = true;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	TRACE_FILE_LINE_INFO();
	return bRes;
}


bool CQualcommDiag::UnLockPhoneCode(char* szLock, bool& bUnlockOK, int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		if(szLock && strlen(szLock) == 8 || strlen(szLock) == 16)
		{
			int nReceivered = -1;
			gsdidiag_subsidy_unlock_PhoneCode_resp Resp = {0};
			if(strlen(szLock) == 8)
			{
				gsdidiag_subsidy_unlock_PhoneCode_req Req = {0};
				Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
				Req.subsys_id = DIAG_SUBSYS_GSDI;
				Req.subsys_cmd_code = 0x48;
				Req.aps_as_transport = 0;
				Req.subcommand = 93;
				Req.master_key_len = strlen(szLock);
				memcpy(Req.master_key, szLock, strlen(szLock));
				Req.new_phone_code_len = 4;
				Req.new_phone_code[0] = '1';
				Req.new_phone_code[1] = '2';
				Req.new_phone_code[2] = '3';
				Req.new_phone_code[3] = '4';
				nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut, true);
			}
			else if(strlen(szLock) == 16)
			{
				gsdidiag_subsidy_unlock_PhoneCode16_req Req = {0};
				Req.cmd_code = DIAG_SUBSYS_CMD_VER_2_F;
				Req.subsys_id = DIAG_SUBSYS_GSDI;
				Req.subsys_cmd_code = 0x48;
				Req.aps_as_transport = 0;
				Req.subcommand = 93;
				Req.master_key_len = strlen(szLock);
				memcpy(Req.master_key, szLock, strlen(szLock));
				Req.new_phone_code_len = 4;
				Req.new_phone_code[0] = '1';
				Req.new_phone_code[1] = '2';
				Req.new_phone_code[2] = '3';
				Req.new_phone_code[3] = '4';
				nReceivered = m_objsercom.TxRx((char*)&Req,sizeof(Req),(char*)&Resp,sizeof(Resp),nTimeOut, true);
			}
			if(nReceivered >= 0)
			{
				TRACE_FILE_LINE_INFO();
				bUnlockOK = Resp.status==0 && Resp.unknown == 0;
				bRes = true;
			}
			else
				m_nLastError = DIAG_ERROR_TXRX_FAIL;
		}
		else
			m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::WriteLockFileName(const std::string& strLocFileName, int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		if(strLocFileName.length() != 0)
		{
			DIAG_NVITEM_PACKET_F_type WriteReq = {0};
			WriteReq.nDIAGcmd = DIAG_NV_WRITE_F;
			WriteReq.item = NV_QISDA_LOC_FILENAME;
			for(int i=0; i< strLocFileName.length(); i++)
				WriteReq.item_data[i] = strLocFileName[i];

			if(WriteNVItem(WriteReq, nTimeOut))	
			{
				bRes = true;
			}
		}
		else 
		{
			m_nLastError = DIAG_ERROR_INVALID_FUNCTION_PARAMETER;
		}
	}
	return bRes;
}
bool CQualcommDiag::ReadLockFileName(std::string& strLocFileName, int nTimeOut)
{
	bool bRes = false;
	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	if(ReadNVItem(NV_QISDA_LOC_FILENAME, ReadResp, nTimeOut))
	{
		char szLocFileName[128] = {0};
		memcpy(szLocFileName, ReadResp.item_data, 128);
		strLocFileName = szLocFileName;
		strLocFileName.substr(0, strLocFileName.length()-4);
		bRes = true;
	}

	return bRes;
}

bool CQualcommDiag::Backup(int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		diagpkt_subsys_hdr_type_v2 req = {0};
		diagpkt_subsys_hdr_type_v2 resp = {0};
		
		req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_CNV;
		req.subsys_cmd_code = 1;

		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.command_code == DIAG_SUBSYS_CMD_VER_2_F && resp.subsys_id == DIAG_SUBSYS_CNV)
			{
				bRes = resp.status ==0;
				if(!bRes)
					m_nLastError = DIAG_ERROR_READ_NVITEM_INVALID_STATUS;
			}
			else
			{
				m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
			}	
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}
bool CQualcommDiag::Restore(int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		diagpkt_subsys_hdr_type_v2 req = {0};
		diagpkt_subsys_hdr_type_v2 resp = {0};

		req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_CNV;
		req.subsys_cmd_code = 2;

		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.command_code == DIAG_SUBSYS_CMD_VER_2_F && resp.subsys_id == DIAG_SUBSYS_CNV)
			{
				bRes = resp.status ==0;
				if(!bRes)
					m_nLastError = DIAG_ERROR_READ_NVITEM_INVALID_STATUS;
			}
			else
			{
				m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
			}
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::FreezeOTP(int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		GSDIDIAG_SECURE_DEVICE_ACTIVATE_REQ req = {0};
		GSDIDIAG_SECURE_DEVICE_ACTIVATE_RESP resp = {0};
		req.command_code = DIAG_SUBSYS_CMD_VER_2_F;
		req.subsys_id = DIAG_SUBSYS_GSDI;
		req.subsys_cmd_code = 72;
		req.aps_as_transport = 1;
		req.subcommand = 86;
		req.slot_id = 1;
		req.sim_app_in_len = 16;

		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			bRes = resp.status ==0;
			if(!bRes)
				m_nLastError = DIAG_ERROR_UNEXPECTED_RESPONSE;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}
bool CQualcommDiag::IsComPortAlive(long nPort)
{
	bool bRes = false;
	bRes = m_objsercom.IsCOMPortAlive(nPort) == TRUE;
	return bRes;
}

bool CQualcommDiag::IsOTPFrozen(int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		OTP_STATUS_CHK_REQ req = {0};
		OTP_STATUS_CHK_RESP resp = {0};

		req.command_code = 139;
		req.subsys_id = 250;
		req.subsys_cmd_code = 2;
		req.subsys_cmd_id = 0;
		req.req_data_length = 2;
		req.rsp_pkt_size = 12;
		req.protect_code = 3125;
		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.otp_status == 2)
				bRes = true;
			else if(resp.otp_status == 0)
				m_nLastError = DIAG_ERROR_WRONG_OTP_PROTECTED_CODE;
			else if(resp.otp_status == 1)
				m_nLastError = DIAG_ERROR_EMPTY_OTP;
			else if(resp.otp_status == 3)
				m_nLastError = DIAG_ERROR_DAMAGED_OTP;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::GetHDVersion(int& nPhoneCodeVersion, int& nBootloaderVersion, int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		OTP_STATUS_CHK_REQ req = {0};
		OTP_STATUS_CHK_RESP resp = {0};

		req.command_code = 139;
		req.subsys_id = 250;
		req.subsys_cmd_code = 2;
		req.subsys_cmd_id = 0;
		req.req_data_length = 2;
		req.rsp_pkt_size = 12;
		req.protect_code = 3125;
		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.otp_status != 3)
			{
				nPhoneCodeVersion = resp.phonecode_ver_baseline;
				nBootloaderVersion = resp.boot_ver_baseline;
				bRes = true;
			}	
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::ReadSPC(char* szLock6, long& nStatus,int nTimeOut)
{
	TRACE_FILE_LINE_INFO();
	bool bRes = false;
	nStatus = 0;

	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
	TRACE_FILE_LINE_INFO();
	if(szLock6 && ReadNVItem(NV_SEC_CODE_I, ReadResp, nTimeOut))
	{
		if(ReadResp.nDIAGcmd == 0x26)
		{
			bool bIsDefault = true;
			for(int i=0; i<6; i++)
				szLock6[i] = ReadResp.item_data[i];
			for(int i=0; i<6 && bIsDefault; i++)
			{
				bIsDefault = ReadResp.item_data[i] == 0;
			}
			if(bIsDefault)
				nStatus = 2;
			bRes = true;
		}
		else if(ReadResp.nDIAGcmd == 0x42)
		{
			nStatus = 1;
		}
		
	}

	TRACE_FILE_LINE_INFO();
	return bRes;
}

bool CQualcommDiag::GetBackupNVStatus(int& nBackupTimes, CTime& LastBackupTime, int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		GET_NV_PARTITION_STATUS_REQ req = {0};
		GET_NV_PARTITION_STATUS_RESP resp = {0};

		req.subsys_hdr.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_hdr.subsys_id = DIAG_SUBSYS_CNV;
		req.subsys_hdr.subsys_cmd_code = 3;
		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			nBackupTimes = resp.cnv_backup_checked.BackupCount;
			CTime temp(resp.cnv_backup_checked.BackupTime.year, 
					   resp.cnv_backup_checked.BackupTime.month,
					   resp.cnv_backup_checked.BackupTime.day,
					   resp.cnv_backup_checked.BackupTime.hour,
					   resp.cnv_backup_checked.BackupTime.minute,
					   resp.cnv_backup_checked.BackupTime.second);
			LastBackupTime = temp;
			bRes = true;
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::IsQfused(bool& bQfused, int nTimeOut)
{
	bool bRes = false;
	bQfused = true;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		QFUSE_Read_REQ req = {0};
		QFUSE_Read_RESP resp = {0};
		req.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_FTM;
		req.subsys_cmd_code = 0x18;
		req.FTM_REQ_DATA_LEN = 0x02;
		req.FTM_RSP_PKT_SIZE = 0x20;

		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.status == 0)
			{
				if(resp.nConfig_Register[0] == 6 && resp.nConfig_Register[1] == 0)
					bQfused = false;
				bRes = true;
			}
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::ReadQfuse(unsigned int& nReg1, unsigned int& nReg2, int nTimeOut )
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		QFUSE_Read_REQ req = {0};
		QFUSE_Read_RESP resp = {0};
		req.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_FTM;
		req.subsys_cmd_code = 0x18;
		req.FTM_REQ_DATA_LEN = 0x02;
		req.FTM_RSP_PKT_SIZE = 0x20;

		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.status == 0)
			{
				nReg1 = resp.nConfig_Register[0];
				nReg2 = resp.nConfig_Register[1];
				bRes = true;
			}
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::WriteQfused(WORD chain_select, int config_register[5],  int& nQfusedStatus/*0=success, 1=Read Operation failed, 2=Write Operation Failed, 3=Operation on selected chain not supported*/, int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		QFUSE_Write_REQ req = {0};
		QFUSE_Write_RESP resp = {0};
		req.command_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_FTM;
		req.FTM_CDM_ID = 1;
		req.subsys_cmd_code = 0x18;
		req.FTM_REQ_DATA_LEN = 12;
		req.FTM_RSP_PKT_SIZE = 22;
		req.chain_select = chain_select;
		for(int i=0; i<5; i++)
			req.config_register[i] = config_register[i];

		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.command_code == DIAG_SUBSYS_CMD_F && resp.subsys_id == DIAG_SUBSYS_FTM)
			{
				nQfusedStatus = resp.status;
				bRes = true;
			}
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::GetCMState(DWORD& nStatus,int nTimeOut)
{
	bool bRes = false;
	if(m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		CM_STATE_Req req = {0};
		CM_STATE_Resp resp = {0};
		req.cmd_code = DIAG_SUBSYS_CMD_F;
		req.subsys_id = DIAG_SUBSYS_CM;
		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.cmd_code == DIAG_SUBSYS_CMD_F && resp.subsys_id == DIAG_SUBSYS_CM && resp.subsys_cmd_code == 0)
			{
				nStatus = resp.srv_status;
				bRes = true;
			}
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

bool CQualcommDiag::GetReleaseDate(char* szDate12, char* szTime9, int nTimeOut)
{
	bool bRes = false;
	if(szDate12 && szTime9 &&  m_bConnected && m_objsercom.MakesureToDIAGMode())
	{
		Version_Number_Req req ={0};
		Version_Number_Resp resp ={0};
		req.cmd_code = DIAG_VERNO_F;

		memset(szDate12, 0, 12);
		memset(szTime9, 0 , 9);
		int nReceivered = m_objsercom.TxRx((char*)&req,sizeof(req),(char*)&resp,sizeof(resp),nTimeOut);
		if(nReceivered >= 0)
		{
			TRACE_FILE_LINE_INFO();
			if(resp.cmd_code == DIAG_VERNO_F)
			{
				memcpy(szDate12, (const char*)resp.Rel_Date, 11);
				memcpy(szTime9, (const char*)resp.Rel_Time, 8);
				bRes = true;
			}
		}
		else
			m_nLastError = DIAG_ERROR_TXRX_FAIL;
	}
	else
		m_nLastError = DIAG_ERROR_CONNECTION_FAIL;
	return bRes;
}

int CQualcommDiag::GetLastError()
{
	return m_nLastError;
}
void CQualcommDiag::SetLastError(int nError)
{ 
	m_nLastError = nError; 
}

void CQualcommDiag::ReOpenComport(long nPort) {

    TCHAR szComport[50] = {0};
    ::_stprintf(szComport,_T("\\\\.\\COM%d"), nPort);
    if(RUN_SUCCESSFULLY == m_objsercom.OpenComport(_T(""), szComport)) {
        m_bConnected = m_objsercom.IsDLorDIAGMode();
    } else {
        AddLog(_T("m_objsercom.OpenComport() fail"));
    }
}

//bool CQualcommDiag::CheckRF4225(int nTimeOut)
//{
//	bool bRes = false;
//	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
//	TRACE_FILE_LINE_INFO();
//	if(ReadNVItem(4225, ReadResp, nTimeOut))
//	{
//		if(ReadResp.item_data[0] == 0xe2)
//			bRes = true;
//	}
//	return bRes;
//}
//bool CQualcommDiag::CheckRF4226(int nTimeOut)
//{
//	bool bRes = false;
//	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
//	TRACE_FILE_LINE_INFO();
//	if(ReadNVItem(4226, ReadResp, nTimeOut))
//	{
//		if(ReadResp.item_data[0] == 0x55)
//			bRes = true;
//	}
//	return bRes;
//}
//bool CQualcommDiag::CheckRF4794(int nTimeOut)
//{
//	bool bRes = false;
//	DIAG_NVITEM_PACKET_F_type ReadResp = {0};
//	TRACE_FILE_LINE_INFO();
//	if(ReadNVItem(4794, ReadResp, nTimeOut))
//	{
//		if(ReadResp.item_data[0] == 0x43)
//			bRes = true;
//	}
//	return bRes;
//}