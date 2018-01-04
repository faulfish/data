#include "StdAfx.h"
#include "HexFile.h"


CHexFile::CHexFile(void)
{
	m_address[0]=0;
	m_address[1]=1;
	m_nlength=0;
	m_npostion=0;
}

CHexFile::~CHexFile(void)
{
}

BOOL CHexFile::Open(LPCTSTR lpszFileName, UINT nOpenFlags, CFileException * pError)
{
	if (!CFile::Open(lpszFileName, nOpenFlags, pError))
	{
		return FALSE;
	}
	char cbuffer[18] = {0};
	UINT nBytesRead = CFile::Read(cbuffer, 17);
	//UINT nBytesRead2 = CFile::Read(cbuffer, 17);
	if (nBytesRead < 15)
	{
		return FALSE;
	}
	// m_address store the first address of Flash to write
	m_address[0] = TwoAsciToByte(cbuffer+9);
	m_address[1] = TwoAsciToByte(cbuffer+11);
	return TRUE;
}


UINT CHexFile::Read(char * lpBuf , UINT nCount)
{
	
	UINT nnowlocation=0;
	UINT nBytesRead=0;
	while (nnowlocation < nCount) // nCount = 1017
	{
		char cbuffer[100] = {0};
		if (m_npostion!=0){
			if (nCount>=(m_nlength-m_npostion)){  // Process not finish data in the same line
				nBytesRead=CFile::Read(cbuffer,(m_nlength-m_npostion)*2+4);
				//m_nlength=0;
				m_npostion=0;
				for (UINT i=0;i<(nBytesRead-4)/2;i++){
					lpBuf[nnowlocation]=TwoAsciToByte(cbuffer+i*2);
					nnowlocation++;
				}
			}
			else{
				nBytesRead=CFile::Read(cbuffer,nCount*2);
				m_npostion=m_npostion+nBytesRead;
				for (UINT i=0;i<nBytesRead/2;i++){
					*(lpBuf+nnowlocation)=TwoAsciToByte(cbuffer+i*2);
					nnowlocation++;
				}
			}
			
		}
		else {
			nBytesRead=CFile::Read(cbuffer , 3);
			if (nBytesRead<3){
				return 0;
			}
			BYTE nlength = TwoAsciToByte(cbuffer+1);
			if (nlength==0){
				break;
			}else if (nlength == 2){
				ZeroMemory(cbuffer,100);
				nBytesRead=CFile::Read(cbuffer, 6+nlength*2+4);
				//nBytesRead=CFile::Read(cbuffer, 3);
				//nlength=TwoAsciToByte(cbuffer+1);
			}
			// for test
			else if(nlength != 2) {
				if (nlength+nnowlocation <= nCount){
					nBytesRead=CFile::Read(cbuffer,6+nlength*2+4);  // Reading one line except :10
					for (UINT i=0;i<nlength;i++){
						*(lpBuf+nnowlocation)=TwoAsciToByte(cbuffer+i*2+6);
						nnowlocation++;
					}
					m_nlength=nlength;
					m_npostion=0;
				} 
				else
				{
					nBytesRead=CFile::Read(cbuffer,6+(nCount-nnowlocation)*2);
					UINT untmep=nCount-nnowlocation;
					for (UINT i=0;i<untmep;i++){
						*(lpBuf+nnowlocation)=TwoAsciToByte(cbuffer+i*2+6);
						nnowlocation++;
					}
					m_nlength=nlength;
					m_npostion=untmep;
				}
			}
		}
		if (m_nlength<0X10)
		{
			break;
		}
	}
	return nnowlocation;
}

BYTE CHexFile::TwoAsciToByte(char * pcassci)
{
	const UINT nmask = 0X5F;
	BYTE byvalue=0;
	UINT ctemp=0;
	for (int i=0;i<2;i++){
		ctemp=pcassci[i];
		if (ctemp>=97 && ctemp<=102){
			ctemp=ctemp & nmask;
		}
		int nlocation=0;
		if (i==0){
			nlocation=16;
		} 
		else{
			nlocation=1;
		}
		if (ctemp >= 65 && ctemp <= 70){
			byvalue = byvalue+(ctemp - 55) * nlocation; 
		} 
		else if(ctemp >= 48 && ctemp <= 57){
			byvalue = byvalue+(ctemp - 48) * nlocation; 
		}
	}
	return byvalue;
}

WORD CHexFile::GetHexAddrss(void)
{
	return m_address[1]+m_address[0]*256;
}



CHexDataParser::CHexDataParser(BYTE** pData, int nDataSize, WORD& wStartAddress)
:m_pData(*pData), m_bEndOfData(false), m_ptLeftData(new CBuffer)
{
	/************************************************************************/
	/* Example:																*/
	/*																		*/
	/* :0200000400807A                                                      */
	/*          ----                                                        */
	/* m_wAddress(start address) should be 0x0080                           */
	/* Total size is 17 including \r\n										*/
	/************************************************************************/
	BYTE nAddress[2] = {CHexFile::TwoAsciToByte((char*)m_pData+11), CHexFile::TwoAsciToByte((char*)m_pData+9)};
	memcpy(&wStartAddress, nAddress, 2);
	m_dwAddress = wStartAddress;
	m_dwAddress	<<= 16;
	m_pData = m_pData+17; 
	
	m_nTotalDataSize = nDataSize;
	m_nLeftSize = nDataSize;
}
CHexDataParser::~CHexDataParser(){}


DWORD CHexDataParser::GetHexAddrss()
{
	return m_dwAddress;	
}
UINT CHexDataParser::Read(char* lpBuf, UINT nRequestCount, bool& bParsed)
{
	bParsed = false;

	CBufferPtr ptReadTotalBuffer = new CBuffer;
	if(m_ptLeftData->GetLength() != 0 && m_ptLeftData->GetLength() < nRequestCount)
	{
		ptReadTotalBuffer->Append(m_ptLeftData);
		m_ptLeftData->SetEmpty();
	}
	if(m_ptLeftData->GetLength() == 0)
	{
		do 
		{
			CBufferPtr ptBuffer = new CBuffer;
			if(ReadALine(ptBuffer) && ParseALine(ptBuffer))
			{
				if(ptReadTotalBuffer->GetLength()+ptBuffer->GetLength() < nRequestCount)
				{
					ptReadTotalBuffer->Append(ptBuffer);
				}
				else // SizeReadBuffer > nRequestCount
				{	
					int nAppendBufferSize = nRequestCount-ptReadTotalBuffer->GetLength(); // |-nAppendBufferSize-|-LeftDataSize-|=ptBufferSize
					ptReadTotalBuffer->Append(new CBuffer( nAppendBufferSize , ptBuffer->GetData()));
					m_ptLeftData->Append(new CBuffer( ptBuffer->GetLength()-nAppendBufferSize, ((char*)ptBuffer->GetData())+ nAppendBufferSize));
				}
				bParsed = true;
			}
			else
				bParsed = false;
		} while(ptReadTotalBuffer->GetLength() < nRequestCount && bParsed && !m_bEndOfData);
	}
	else
		bParsed = false;

	if(bParsed)
	{
		memcpy(lpBuf, ptReadTotalBuffer->GetData(), ptReadTotalBuffer->GetLength());
	}
	
	return ptReadTotalBuffer->GetLength();
}

//************************************
// Method:    ReadALine
// IN/OUT Parameter: CBufferPtr ptBuffer
// IN: empty buffer
// OUT: a line
// Hint:
// \r\n means the end of a line
//************************************
bool CHexDataParser::ReadALine(CBufferPtr& ptBuffer)
{
	bool bEndOfLine = false;
	int nReadDataCount = 0;
	do 
	{	
		if(*(m_pData+nReadDataCount) == '\r' && *(m_pData+nReadDataCount+1) == '\n')  // End of line
		{
			bEndOfLine = true;
			m_pData+=(nReadDataCount+2);
			m_nLeftSize -= nReadDataCount+1;
		}
		else
		{
			ptBuffer->Append(new CBuffer(1, (const char*)(m_pData+nReadDataCount)));
		}

		nReadDataCount++;
	} while(!bEndOfLine && m_nLeftSize>=nReadDataCount);
	return bEndOfLine;
}

/************************************************************************/
/* Example:																*/
/*																		*/
/* :100000000500000003000000280080002800800098                          */
/*  --      --------------------------------                            */
/*  Length(UsefulData)          UsefulData		 						*/
/*																		*/
/* Hint:																*/
/* 1. Data Length = char[2] 10 = 0x10									*/
/* 2. char DataLength = 0x10*2											*/
/* 3. BYTE DataLength = 0x10											*/
/************************************************************************/
//************************************
// Method:    ParseALine
// IN/OUT Parameter: CBufferPtr ptBuffer
//    Parser Buffer and fill in useful data back
// Hint:
// 1. don't care the data which DataLength == 4 or 2
// 2. DataLength == 0 means the end of FileData
// 3. Others should be parsed
//************************************
bool CHexDataParser::ParseALine(CBufferPtr& ptBuffer)
{
	bool bRes = false;
	
	BYTE nDataLength = CHexFile::TwoAsciToByte(((char*)ptBuffer->GetData())+1);
	if(nDataLength > 0x04)
	{
		CBufferPtr ptReadCharBuffer = new CBuffer(nDataLength*2, (char*)(ptBuffer->GetData())+9);
		CBufferPtr ptReadByteBuffer = new CBuffer;
		for(int n=0; n<nDataLength; n++)
		{
			BYTE nTemp = CHexFile::TwoAsciToByte((char*)(ptReadCharBuffer->GetData()+n*2));
			ptReadByteBuffer->Append(new CBuffer(1, (const char*)&nTemp));
		}
		ptBuffer = ptReadByteBuffer; // fill in useful data back
		bRes = true;
	}
	else if(nDataLength == 0x00)
	{
		m_bEndOfData = true;
		ptBuffer->SetEmpty();
		bRes = true;
	}
	else if(nDataLength == 0x02 || nDataLength == 0x04)
	{
		ptBuffer->SetEmpty();
		bRes = true;
	}
	ASSERT(bRes);
	return bRes;
}