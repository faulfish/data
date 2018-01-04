#ifndef _ATVCARDREQUEST_H_
#define _ATVCARDREQUEST_H_

#include "Global_Def.h"
#include "ATFSRequest.h"

#include "..\MobileInterface\Interface_Include\IPhonebook.h"

class CVCARDBaseRequest: public CATFSBaseRequest
{
public:
	CVCARDBaseRequest(ePhonebookType eType,DWORD dwTimeout = 3000):CATFSBaseRequest(dwTimeout),m_eStorage(eType){}
public:
	ePhonebookType m_eStorage;
};

class CATVCARDReadRequest: public CVCARDBaseRequest
{
public:
	CATVCARDReadRequest(int idx,CBufferPtr& spbufData,ePhonebookType eStorage,int& nErrcode,int nTimeOut)
		:CVCARDBaseRequest(eStorage,nTimeOut)
		,m_nIndex(idx)
		,m_spbufData(spbufData)
		,m_nErrcode(nErrcode)
	  {
		  char szCmd[128] = {0};
		  if(m_eStorage == ePhonebookSIM)
			  sprintf(szCmd, "AT$BQVCARDR=\"SM\",%d\r", m_nIndex);
		  else
			  sprintf(szCmd, "AT$BQVCARDR=\"ME\",%d\r", m_nIndex);

		  m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	  }
	virtual ~CATVCARDReadRequest(){;}

	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/*
		000995: Bulk or Interrupt Transfer (UP), 30.07.2007 14:59:52.3437500 +0.0156250
		Pipe Handle: 0x8595b904 (Endpoint Address: 0x2)
		Send 0x13 bytes to the device:
		41 54 24 42 51 56 43 41 52 44 52 3D 22 4D 45 22   AT$BQVCARDR="ME"
		2C 31 0D                                          ,1.

		000996: Bulk or Interrupt Transfer (UP), 30.07.2007 14:59:52.3593750 +0.0156250
		Pipe Handle: 0x8595b8c4 (Endpoint Address: 0x81)
		Get 0xa bytes from the device:
		A1 20 00 00 00 00 02 00 03 00                     ! ........

		000997: Bulk or Interrupt Transfer (UP), 30.07.2007 14:59:52.3593750 +0.0
		Pipe Handle: 0x8595b8e4 (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..

		000998: Bulk or Interrupt Transfer (UP), 30.07.2007 14:59:52.3593750 +0.0
		Pipe Handle: 0x8595b8e4 (Endpoint Address: 0x82)
		Get 0x85 bytes from the device:
		24 42 51 56 43 41 52 44 52 3A 31 2C 31 31 34 0D   $BQVCARDR:1,114.
		0A 42 45 47 49 4E 3A 56 43 41 52 44 0D 0A 4E 3B   .BEGIN:VCARD..N;
		43 48 41 52 53 45 54 3D 55 54 46 2D 38 3B 45 4E   CHARSET=UTF-8;EN
		43 4F 44 49 4E 47 3D 51 55 4F 54 45 44 2D 50 52   CODING=QUOTED-PR
		49 4E 54 41 42 4C 45 3A 45 6E 69 64 3B 3B 3B 3B   INTABLE:Enid;;;;
		0D 0A 54 45 4C 3B 48 4F 4D 45 3B 56 4F 49 43 45   ..TEL;HOME;VOICE
		3A 30 39 31 38 31 31 35 33 38 30 0D 0A 56 45 52   :0918115380..VER
		53 49 4F 4E 3A 32 2E 31 0D 0A 45 4E 44 3A 56 43   SION:2.1..END:VC
		41 52 44 0D 0A                                    ARD..

		000999: Bulk or Interrupt Transfer (UP), 30.07.2007 14:59:52.3593750 +0.0
		Pipe Handle: 0x8595b8e4 (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/

		/*FAIL when FDN On
		000349: Bulk or Interrupt Transfer (UP), 10.01.2008 19:11:13.9296258 +0.0468783
		Pipe Handle: 0x8940b2cc (Endpoint Address: 0x2)
		Send 0x16 bytes to the device:
		41 54 24 42 51 56 43 41 52 44 52 3D 22 53 4D 22   AT$BQVCARDR="SM"
		2C 34 30 39 37 0D                                 ,4097.
		000350: Bulk or Interrupt Transfer (UP), 10.01.2008 19:11:13.9296258 +0.0
		Pipe Handle: 0x8940b2ac (Endpoint Address: 0x82)
		Get 0x16 bytes from the device:
		0D 0A 24 42 51 56 43 41 52 44 45 52 52 3A 20 36   ..$BQVCARDERR: 6
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/
		const char* pData = spBuffer->GetData();
		int nLen = spBuffer->GetLength();
		if(nLen > 0)
		{
			char* szBuf = new char [nLen + 1];
			memset(szBuf,0,nLen + 1);
			memcpy(szBuf,pData,nLen);

			if(::memcmp(szBuf,"\r\n$BQVCARDERR",13) == 0)
			{
				char* szErrCode = szBuf + 14;
				m_nErrcode = ::atoi(szErrCode);
			}
			else
			{
				char szSeps[] = {",\r\n"};
				// Establish string and get the first token:
				char* sztoken = strtok( szBuf, szSeps ); // C4996
				// Note: strtok is deprecated; consider using strtok_s instead
				if( sztoken != NULL )
				{
					sztoken = strtok( NULL, szSeps );
					int nLen = atoi(sztoken);
					sztoken = strtok( NULL, szSeps );
					__w64 int nGap = sztoken - szBuf;
					const char* szContentStart = pData + nGap;
					m_spbufData = new CBuffer(nLen,szContentStart);
				}
			}
			delete [] szBuf;
		}
		return true;
	}
private:
	int					m_nIndex;
	CBufferPtr&	m_spbufData;
	int&					m_nErrcode;
};

class CATVCARDWriteRequest: public CVCARDBaseRequest
{
public:
	CATVCARDWriteRequest(char* pData, DWORD nDataSize, UINT nIndex, ePhonebookType eStorage, int nTimeOut,UINT& nRetValue,int& nRetError)
		: CVCARDBaseRequest(eStorage,nTimeOut)
		,m_spbufData( new CBuffer(nDataSize,pData))
		,m_bNeedSendVCardData(true)
		,m_bHadSwitchToStorage(false)
		,m_nIndex(nIndex)
		,m_nRetValue(nRetValue)
		,m_nRetError(nRetError)
	{//AT+CPBS="ME
		char szCmd[128] = {0};
		if(m_eStorage == ePhonebookSIM)
			sprintf(szCmd,"AT+CPMS=\"SM\"\r");
		else
			sprintf(szCmd,"AT+CPMS=\"ME\"\r");
		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CATVCARDWriteRequest(){;}

	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/*Pipe Handle: 0x89ed4c94 (Endpoint Address: 0x2)
		Send 0x17 bytes to the device:
		41 54 24 42 51 56 43 41 52 44 57 3D 22 4D 45 22   AT$BQVCARDW="ME"
		2C 31 2C 31 32 33 0D                              ,1,123.
		010409: Bulk or Interrupt Transfer (UP), 28.11.2007 21:05:50.8593750 +0.0
		Pipe Handle: 0x89ed4c74 (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		010410: Bulk or Interrupt Transfer (UP), 28.11.2007 21:05:50.8593750 +0.0
		Pipe Handle: 0x89ed4c54 (Endpoint Address: 0x81)
		Get 0xa bytes from the device:
		A1 20 00 00 00 00 02 00 03 00                     ? ........
		010411: Bulk or Interrupt Transfer (UP), 28.11.2007 21:05:50.8750000 +0.0156250
		Pipe Handle: 0x89ed4c94 (Endpoint Address: 0x2)
		Send 0x7b bytes to the device:
		42 45 47 49 4E 3A 56 43 41 52 44 0D 0A 56 45 52   BEGIN:VCARD..VER
		53 49 4F 4E 3A 32 2E 31 0D 0A 4E 3B 45 4E 43 4F   SION:2.1..N;ENCO
		44 49 4E 47 3D 51 55 4F 54 45 44 2D 50 52 49 4E   DING=QUOTED-PRIN
		54 41 42 4C 45 3B 43 48 41 52 53 45 54 3D 55 54   TABLE;CHARSET=UT
		46 2D 38 3A 3D 45 38 3D 41 39 3D 42 32 3D 45 38   F-8:=E8=A9=B2=E8
		3D 41 31 3D 38 43 0D 0A 58 2D 42 45 4E 51 3B 50   =A1=8C..X-BENQ;P
		72 65 66 65 72 4C 69 73 74 3A 4E 4F 4E 45 0D 0A   referList:NONE..
		45 4E 44 3A 56 43 41 52 44 0D 0A                  END:VCARD..
		010412: Bulk or Interrupt Transfer (UP), 28.11.2007 21:05:50.8906250 +0.0156250
		Pipe Handle: 0x89ed4c54 (Endpoint Address: 0x81)
		Get 0xa bytes from the device:
		A1 20 00 00 00 00 02 00 03 00                     ? ........

		010413: Bulk or Interrupt Transfer (UP), 28.11.2007 21:05:50.9375000 +0.0468750
		Pipe Handle: 0x89ed4c74 (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..

		010414: Bulk or Interrupt Transfer (UP), 28.11.2007 21:05:50.9531250 +0.0156250
		Pipe Handle: 0x89ed4c74 (Endpoint Address: 0x82)
		Get 0xd bytes from the device:
		24 42 51 56 43 41 52 44 57 3A 31 0D 0A            $BQVCARDW:1..

		010415: Bulk or Interrupt Transfer (UP), 28.11.2007 21:05:50.9531250 +0.0
		Pipe Handle: 0x89ed4c74 (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/

		/* Fail case
		082904: Bulk or Interrupt Transfer (UP), 29.11.2007 09:21:34.2500000 +0.0156250
		Pipe Handle: 0x89eb9dc4
		Get 0x16 bytes from the device:
		0D 0A 24 42 51 56 43 41 52 44 45 52 52 3A 20 38   ..$BQVCARDERR: 8
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/

		bool bRes = true;
		if(IsOK(spBuffer))
		{
			if(m_bHadSwitchToStorage == false)
			{				
				m_bHadSwitchToStorage = true;
				char szCmd[128] = {0};
				if(m_eStorage == ePhonebookSIM)
					sprintf(szCmd,"AT$BQVCARDW=\"SM\",%d,%d\r",m_nIndex,m_spbufData->GetLength());
				else
					sprintf(szCmd,"AT$BQVCARDW=\"ME\",%d,%d\r",m_nIndex,m_spbufData->GetLength());

				m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
				DoRequest();
			}
			else if(m_bNeedSendVCardData)
			{
				m_bNeedSendVCardData = false;
				if(m_spbufData->GetLength() > 0)
				{
					m_ptBufferSend = m_spbufData;
					DoRequest();
					m_spbufData = new CBuffer;
					bRes = false;
				}
			}
			else if(m_bHadSwitchToStorage && m_bNeedSendVCardData == false)
			{		
				string strTemp;
				char* szBuf = new char [spBuffer->GetLength() + 1];
				memset(szBuf,0,spBuffer->GetLength() + 1);
				memcpy(szBuf,spBuffer->GetData(),spBuffer->GetLength());
				strTemp = szBuf;
				delete [] szBuf;

				string::size_type nIndex = strTemp.find("$BQVCARDW:");
				if(nIndex != string::npos)
					m_nRetValue = atoi(strTemp.substr(nIndex+strlen("$BQVCARDW:")).c_str());
				else
				{
					m_nRetValue = -1;
					nIndex = strTemp.find("$BQVCARDERR:");
					if(nIndex != string::npos)
						m_nRetError = atoi(strTemp.substr(nIndex+strlen("$BQVCARDERR:")).c_str());
				}
			}
		}
		return bRes;
	}
	private:
		CBufferPtr	m_spbufData;
		bool				m_bNeedSendVCardData;
		bool				m_bHadSwitchToStorage;
		int				m_nIndex;
		
		UINT&			m_nRetValue;
		int&				m_nRetError;
};

class CATVCARDDeleteRequest: public CVCARDBaseRequest
{
public:
	CATVCARDDeleteRequest(int nIndex,ePhonebookType eStorage,int nTimeOut): CVCARDBaseRequest(eStorage,nTimeOut)
	{

		char szCmd[128];
		if(m_eStorage == ePhonebookSIM)
			sprintf(szCmd,"AT$BQVCARDD=\"SM\",0,%d\r",nIndex);
		else
			sprintf(szCmd,"AT$BQVCARDD=\"ME\",0,%d\r",nIndex);

		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CATVCARDDeleteRequest(){;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer){return true;}
private:
	int m_nIndex;
};

typedef CRefObjectPtr<CATVCARDDeleteRequest> CATVCARDDeleteRequestPtr;

class CATVCARDListRequest: public CVCARDBaseRequest
{
public:
	CATVCARDListRequest(ePhonebookType eStorage,vector<UINT>* pIdxList,int nTimeOut): CVCARDBaseRequest(eStorage,nTimeOut),m_pIdxList(pIdxList)
	{
		char szCmd[128] = {0};
		if(m_eStorage == ePhonebookSIM)
			sprintf(szCmd,"AT$BQVCARDL=\"SM\"\r");
		else
			sprintf(szCmd,"AT$BQVCARDL=\"ME\"\r");

		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CATVCARDListRequest(){;}

	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/*
		000698: Bulk or Interrupt Transfer (UP), 31.07.2007 10:10:42.5000000 +14.9062500
		Pipe Handle: 0x85a2b564 (Endpoint Address: 0x2)
		Send 0x11 bytes to the device:
		41 54 24 42 51 56 43 41 52 44 4C 3D 22 4D 45 22   AT$BQVCARDL="ME"
		0D                                                .
		000699: Bulk or Interrupt Transfer (UP), 31.07.2007 10:10:42.5000000 +0.0
		Pipe Handle: 0x85a2b544 (Endpoint Address: 0x82)
		Get 0x11 bytes from the device:
		41 54 24 42 51 56 43 41 52 44 4C 3D 22 4D 45 22   AT$BQVCARDL="ME"
		0D                                                .
		000700: Bulk or Interrupt Transfer (UP), 31.07.2007 10:10:42.5156250 +0.0156250
		Pipe Handle: 0x85a2b544 (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..

		000701: Bulk or Interrupt Transfer (UP), 31.07.2007 10:10:42.5156250 +0.0
		Pipe Handle: 0x85a2b544 (Endpoint Address: 0x82)
		Get 0x12 bytes from the device:
		24 42 51 56 43 41 52 44 4C 3A 22 4D 45 22 2C 31   $BQVCARDL:"ME",1
		0D 0A                                             ..
		000702: Bulk or Interrupt Transfer (UP), 31.07.2007 10:10:42.5156250 +0.0
		Pipe Handle: 0x85a2b544 (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/

		const char* pData = spBuffer->GetData();
		int nLen = spBuffer->GetLength();
		if(nLen > 0)
		{
			char* szBuf = new char [nLen + 1];
			memset(szBuf,0,nLen + 1);
			memcpy(szBuf,pData,nLen);

			int nIndex = 0;
			char szSeps[] = {","};
			// Establish string and get the first token:
			char* sztoken = strtok( szBuf, szSeps ); // C4996
			// Note: strtok is deprecated; consider using strtok_s instead
			while( sztoken != NULL )
			{
				nIndex++;
				if(nIndex > 1)
				{
					int nValue = atoi(sztoken);
					m_pIdxList->push_back(nValue);
				}
				// Get next token: 
				sztoken = strtok( NULL, szSeps ); // C4996
			}

			delete [] szBuf;
		}
		return true;
	}
private:
	vector<UINT>* m_pIdxList;
};

class CATVCARDRangeRequest: public CVCARDBaseRequest
{
public:
	CATVCARDRangeRequest(UINT* pnMinIndex,UINT* pnMaxIndex,UINT* pnMaxCount,ePhonebookType eStorage,int nTimeOut):CVCARDBaseRequest(eStorage,nTimeOut),m_pnMaxIndex(pnMaxIndex),m_pnMaxCount(pnMaxCount),m_pnMinIndex(pnMinIndex)
	{
		if(m_eStorage == ePhonebookSIM)
		{
			char szCmd[128] = {"AT+CPMS=\"SM\"\r"};
			m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
		}
		else 
		{
			char szCmd[128] = {"AT+CPMS=\"ME\"\r"};
			m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
		}
	}
	virtual ~CATVCARDRangeRequest(){;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bRes = true;
		if(IsOK(spBuffer))
		{		
			char szCmd[128] = {"AT+CPMS?\r"};
			if(m_ptBufferSend->GetLength() > 0 && m_ptBufferSend->GetLength() != strlen(szCmd))
			{
				m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
				DoRequest();
				bRes = false;
			}
			else
			{
				/*
				000348: Bulk or Interrupt Transfer (UP), 31.07.2007 09:52:31.3593750 +0.0
				Pipe Handle: 0x85a2b544 (Endpoint Address: 0x82)
				Get 0x9 bytes from the device:
				41 54 2B 43 50 4D 53 3F 0D                        AT+CPMS?.
				000349: Bulk or Interrupt Transfer (UP), 31.07.2007 09:52:31.3593750 +0.0
				Pipe Handle: 0x85a2b544 (Endpoint Address: 0x82)
				Get 0x31 bytes from the device:
				0D 0A 2B 43 50 4D 53 3A 20 22 4D 45 22 2C 33 2C   ..+CPMS: "ME",3,
				35 30 30 2C 22 4D 45 22 2C 33 2C 35 30 30 2C 22   500,"ME",3,500,"
				4D 45 22 2C 33 2C 35 30 30 0D 0A 0D 0A 4F 4B 0D   ME",3,500....OK.
				0A                                                .
				*/

				*m_pnMinIndex = 1;
				const char* pData = spBuffer->GetData();
				int nLen = spBuffer->GetLength();
				if(nLen > 0)
				{
					char* szBuf = new char [nLen + 1];
					memset(szBuf,0,nLen + 1);
					memcpy(szBuf,pData,nLen);

					int nIndex = 0;
					char szSeps[] = {","};
					// Establish string and get the first token:
					char* sztoken = strtok( szBuf, szSeps ); // C4996
					// Note: strtok is deprecated; consider using strtok_s instead
					while( sztoken != NULL )
					{
						nIndex++;						
						// Get next token: 
						sztoken = strtok( NULL, szSeps ); // C4996
						if(nIndex == 1)
							;//*m_pnMECurCount = atoi(sztoken);
						else if(nIndex == 2)
						{
							*m_pnMaxIndex = atoi(sztoken);
							*m_pnMaxCount = *m_pnMaxIndex;
						}
						else break;
					}

					delete [] szBuf;
				}
			}
		}

		return bRes;
	}
private:
	UINT* m_pnMinIndex;
	UINT* m_pnMaxIndex;
	UINT* m_pnMaxCount;
};

#endif