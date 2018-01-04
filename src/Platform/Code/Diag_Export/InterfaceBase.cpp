#include "StdAfx.h"
#include "InterfaceBase.h"
#include "..\RequestManager\Request\QualcommRequest.h"
#include "..\CommonUtil\UnicodeUtility.h"
#include "..\CommonUtil\LogPolicy.h"

CInterfaceBase::CInterfaceBase(int nCOMPort):m_nCOMPort(nCOMPort),m_bIsConnected(false)
{
	using namespace PLATFORM_TOOL;
	m_bIsConnected = GetIRequestManager(m_ptIRequestManager,m_nCOMPort);
	if(IsConnected())
		m_ptIRequestManager->Open();
}

CInterfaceBase::~CInterfaceBase()
{
	//2008/5/6 Alan tu
	//Don't close on destructor, due to other intsance
	//It will close byitself
	//If you should close it for some specially case, please call Close() and don't forget to reopen it.

	//if(IsConnected())
	//	m_ptIRequestManager->Close();
}

bool CInterfaceBase::Execuate(PLATFORM_TOOL::CRequestPtr& ptRequest)
{
	LogPolicy::Log(_T("CInterfaceBase::Execuate{"));
	TRACE_FILE_LINE_INFO();

	bool bRes = false;
	if(IsConnected())
		bRes = m_ptIRequestManager->Execuate(ptRequest);
	assert(bRes);
	if(bRes)
		bRes = ptRequest->WaitForWrite();
	assert(bRes);
	
	LogPolicy::Log(_T("}"));
	TRACE_FILE_LINE_INFO();

	return bRes;
}

bool CInterfaceBase::IsConnected() const
{
	return m_bIsConnected;
}

bool CInterfaceBase::EnableDiagEvent(bool bEnable)
{
	using namespace PLATFORM_TOOL;

	bool bRes = false;	
	CRequestPtr ptRequest = new CEventRequest(bEnable);
	bRes = Execuate(ptRequest);
	return bRes;
}

int CInterfaceBase::HexBcdToDigit(BYTE *pucHexBcdBuf, int iHexBcdBufSize, char *sDigitBuf, int iDigitBufSize)
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

int CInterfaceBase::DigitToHexBcd(char *sDigitBuf, int iDigitBufSize, BYTE *pucHexBcdBuf, int iHexBcdBufSize)
{
	int nRet = 0;
	if(sDigitBuf && pucHexBcdBuf && iHexBcdBufSize >= (iDigitBufSize-1)/2)
	{
		for(int i=0; i< (iDigitBufSize-1)/2; i++)
		{
			char szTemp[3] = {0};
			szTemp[0] = sDigitBuf[i*2];
			szTemp[1] = sDigitBuf[i*2+1];
			pucHexBcdBuf[(iDigitBufSize-1)/2 -i -1] = QSYNC_LIB::HEX2Ineger(szTemp);
		}
	}
	
	return nRet;
}

unsigned int CInterfaceBase::IntEndianTransfer(unsigned int usValue)
{
	unsigned int usNewValue = ntohl(usValue);

	/*
	//Little Endian to Big Endian (intel)
	//00000000111111112222222233333333 => 33333333222222221111111100000000
	BYTE byte1 = (0xFF000000 & usValue) >> 24;
	BYTE byte2 = (0x00FF0000 & usValue) >> 16;
	BYTE byte3 = (0x0000FF00 & usValue) >> 8;
	BYTE byte4 = (0x000000FF & usValue);
	unsigned int usNewValue1 = byte4 << 24 | byte3 << 16 | byte2 << 8 | byte1;
	assert(usNewValue1 == usNewValue);
	*/

	return usNewValue;
}

unsigned short CInterfaceBase::ShortEndianTransfer(unsigned short usValue)
{
	unsigned short usNewValue = ntohs(usValue);

	/*
	//Little Endian to Big Endian (intel)
	//0000000011111111 => 1111111100000000
	BYTE byte1 = (0xFF00 & usValue) >> 8;
	BYTE byte2 = (0x00FF & usValue);
	unsigned short usNewValue1 = byte2 << 8 | byte1;
	assert(usNewValue1 == usNewValue);
	*/

	return usNewValue;
}