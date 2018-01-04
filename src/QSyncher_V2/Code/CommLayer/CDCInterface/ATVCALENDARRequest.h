#ifndef _ATVCALENDARREQUEST_H_
#define _ATVCALENDARREQUEST_H_

#include "Global_Def.h"
#include "ATFSRequest.h"

#include "..\MobileInterface\Interface_Include\ICalendar.h"

class CVCALENDARBaseRequest: public CATFSBaseRequest
{
public:
	CVCALENDARBaseRequest(DWORD dwTimeout = 3000)
		:CATFSBaseRequest(dwTimeout){}
};

class CVCALENDARReadRequest: public CVCALENDARBaseRequest
{
public:
	CVCALENDARReadRequest(int idx,CBufferPtr& spbufData,int nTimeOut)
		:CVCALENDARBaseRequest(nTimeOut)
		,m_nIndex(idx)
		,m_spbufData(spbufData)
	  {
		  /*
		  017184: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:59.3242911 +0.0156290
		  Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
		  Send 0xe bytes to the device:
		  41 54 24 42 51 56 43 41 4C 52 3D 31 37 0D         AT$BQVCALR=17.
		  017185: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:59.3399201 +0.0156290
		  Pipe Handle: 0x8954f98c (Endpoint Address: 0x81)
		  Get 0xa bytes from the device:
		  A1 20 00 00 00 00 02 00 03 00                     ! ........
		  017186: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:59.3555491 +0.0156290
		  Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		  Get 0x2 bytes from the device:
		  0D 0A                                             ..
		  017187: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:59.3555491 +0.0
		  Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		  Get 0x123 bytes from the device:
		  24 42 51 56 43 41 4C 52 3A 31 37 2C 32 39 35 0D   $BQVCALR:17,295.
		  0A 42 45 47 49 4E 3A 56 43 41 4C 45 4E 44 41 52   .BEGIN:VCALENDAR
		  0D 0A 56 45 52 53 49 4F 4E 3A 31 2E 30 0D 0A 42   ..VERSION:1.0..B
		  45 47 49 4E 3A 56 45 56 45 4E 54 0D 0A 55 49 44   EGIN:VEVENT..UID
		  3A 31 37 0D 0A 44 41 4C 41 52 4D 3A 32 30 30 30   :17..DALARM:2000
		  31 32 32 39 54 32 33 35 35 30 30 5A 3B 50 30 30   1229T235500Z;P00
		  30 30 59 30 30 4D 30 30 44 54 30 30 48 30 30 4D   00Y00M00DT00H00M
		  30 30 53 3B 30 30 30 0D 0A 52 52 55 4C 45 3A 59   00S;000..RRULE:Y
		  4D 31 20 20 31 32 20 23 30 0D 0A 44 54 45 4E 44   M1  12 #0..DTEND
		  3A 32 30 30 30 31 32 33 30 54 30 31 30 30 30 30   :20001230T010000
		  5A 0D 0A 44 54 53 54 41 52 54 3A 32 30 30 30 31   Z..DTSTART:20001
		  32 33 30 54 30 30 30 30 30 30 5A 0D 0A 53 55 4D   230T000000Z..SUM
		  4D 41 52 59 3B 43 48 41 52 53 45 54 3D 55 54 46   MARY;CHARSET=UTF
		  2D 38 3B 45 4E 43 4F 44 49 4E 47 3D 51 55 4F 54   -8;ENCODING=QUOT
		  45 44 2D 50 52 49 4E 54 41 42 4C 45 3A 3D 45 37   ED-PRINTABLE:=E7
		  3D 39 34 3D 39 46 3D 45 36 3D 39 37 3D 41 35 20   =94=9F=E6=97=A5 
		  31 32 33 34 35 36 0D 0A 45 4E 44 3A 56 45 56 45   123456..END:VEVE
		  4E 54 0D 0A 45 4E 44 3A 56 43 41 4C 45 4E 44 41   NT..END:VCALENDA
		  52 0D 0A                                          R..
		  017188: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:59.3555491 +0.0
		  Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		  Get 0x6 bytes from the device:
		  0D 0A 4F 4B 0D 0A                                 ..OK..
		  */
		  char szCmd[128] = {0};
		  sprintf(szCmd,"AT$BQVCALR=%d\r",m_nIndex);
		  m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	  }
	virtual ~CVCALENDARReadRequest(){;}

	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{		
		if(IsOK(spBuffer))
		{
			const char* pData = spBuffer->GetData();
			int nLen = spBuffer->GetLength() - 6;
			if(nLen > 0)
			{
				char* szBuf = new char [nLen + 1];
				memset(szBuf,0,nLen + 1);
				memcpy(szBuf,pData,nLen);

				char szSeps[] = {",\r\n"};
				// Establish string and get the first token:
				char* sztoken = strtok( szBuf, szSeps ); // C4996
				// Note: strtok is deprecated; consider using strtok_s instead
				if( sztoken != NULL )
				{
					sztoken = strtok( NULL, szSeps );
					//int nLen = atoi(sztoken);
					sztoken = strtok( NULL, szSeps );
					__w64 int nGap = sztoken - szBuf;
					const char* szContentStart = pData + nGap;
					m_spbufData = new CBuffer((int)(nLen - nGap),szContentStart);
				}
				delete [] szBuf;
			}
		}
		return true;
	}
private:
	int			m_nIndex;
	CBufferPtr&	m_spbufData;
};

class CVCALENDARWriteRequest: public CVCALENDARBaseRequest
{
public:
	CVCALENDARWriteRequest(char* pData, DWORD nDataSize, UINT nIndex, int nTimeOut,UINT& nRetValue,int& nRetError)
		: CVCALENDARBaseRequest(nTimeOut)
		,m_spbufData( new CBuffer(nDataSize,pData))
		,m_bNeedSendData(true)
		,m_nIndex(nIndex)
		,m_nRetValue(nRetValue)
		,m_nRetError(nRetError)
	{
		/*
		058312: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:50.2347704 +0.0
		Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
		Send 0x11 bytes to the device:
		41 54 24 42 51 56 43 41 4C 57 3D 30 2C 32 31 34   AT$BQVCALW=0,214
		0D                                                .
		058313: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:50.2347704 +0.0
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..

		058314: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:50.2660202 +0.0312498
		Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
		Send 0xd6 bytes to the device:
		42 45 47 49 4E 3A 56 43 41 4C 45 4E 44 41 52 0D   BEGIN:VCALENDAR.
		0A 56 45 52 53 49 4F 4E 3A 32 2E 31 0D 0A 42 45   .VERSION:2.1..BE
		47 49 4E 3A 56 45 56 45 4E 54 0D 0A 44 54 53 54   GIN:VEVENT..DTST
		41 52 54 3A 32 30 30 37 31 32 30 37 54 30 30 30   ART:20071207T000
		30 30 30 5A 0D 0A 44 54 45 4E 44 3A 32 30 30 37   000Z..DTEND:2007
		31 32 30 37 54 30 30 33 30 30 30 5A 0D 0A 53 55   1207T003000Z..SU
		4D 4D 41 52 59 3B 45 4E 43 4F 44 49 4E 47 3D 51   MMARY;ENCODING=Q
		55 4F 54 45 44 2D 50 52 49 4E 54 41 42 4C 45 3B   UOTED-PRINTABLE;
		43 48 41 52 53 45 54 3D 55 54 46 2D 38 3A 77 77   CHARSET=UTF-8:ww
		71 77 71 0D 0A 44 41 4C 41 52 4D 3A 32 30 30 37   qwq..DALARM:2007
		31 32 30 36 54 32 33 35 39 30 30 5A 0D 0A 52 52   1206T235900Z..RR
		55 4C 45 3A 44 31 20 23 30 0D 0A 45 4E 44 3A 56   ULE:D1 #0..END:V
		45 56 45 4E 54 0D 0A 45 4E 44 3A 56 43 41 4C 45   EVENT..END:VCALE
		4E 44 41 52 0D 0A                                 NDAR..
		058315: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:50.2660202 +0.0
		Pipe Handle: 0x8954f98c (Endpoint Address: 0x81)
		Get 0xa bytes from the device:
		A1 20 00 00 00 00 02 00 03 00                     ! ........

		058316: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:50.5472684 +0.2812482
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..

		058317: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:50.5472684 +0.0
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0xd bytes from the device:
		24 42 51 56 43 41 4C 57 3A 33 34 0D 0A            $BQVCALW:34..

		058318: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:50.5472684 +0.0
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/
		char szCmd[128] = {0};
		sprintf(szCmd,"AT$BQVCALW=%d,%d\r",m_nIndex,m_spbufData->GetLength());
		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CVCALENDARWriteRequest(){;}

	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bRes = true;
		if(IsOK(spBuffer))
		{
			if(m_bNeedSendData)
			{
				m_bNeedSendData = false;
				if(m_spbufData->GetLength() > 0)
				{
					m_ptBufferSend = m_spbufData;
					DoRequest();
					m_spbufData = new CBuffer;
					bRes = true;
				}
			}
			else if(m_bNeedSendData == false)
			{		
				string strTemp;
				char* szBuf = new char [spBuffer->GetLength() + 1];
				memset(szBuf,0,spBuffer->GetLength() + 1);
				memcpy(szBuf,spBuffer->GetData(),spBuffer->GetLength());
				strTemp = szBuf;
				delete [] szBuf;

				string::size_type nIndex = strTemp.find("$BQVCALW:");
				if(nIndex != string::npos)
					m_nRetValue = atoi(strTemp.substr(nIndex+strlen("$BQVCALW:")).c_str());
				else
				{
					m_nRetValue = -1;
					nIndex = strTemp.find("$BQVCALWERR:");
					if(nIndex != string::npos)
						m_nRetError = atoi(strTemp.substr(nIndex+strlen("$BQVCALWERR:")).c_str());
				}
			}
		}
		return bRes;
	}
	private:
		CBufferPtr	m_spbufData;
		bool				m_bNeedSendData;
		int				m_nIndex;
		
		UINT&			m_nRetValue;
		int&				m_nRetError;
};

class CVCALENDARDeleteRequest: public CVCALENDARBaseRequest
{
public:
	CVCALENDARDeleteRequest(UINT nIndex,int nTimeOut,bool& bRes)
		:CVCALENDARBaseRequest(nTimeOut)
		,m_bRes(bRes)
	{
		/*
		057736: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:35.3129909 +0.0156249
		Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
		Send 0x10 bytes to the device:
		41 54 24 42 51 56 43 41 4C 44 3D 30 2C 33 33 0D   AT$BQVCALD=0,33.
		057737: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:35.6098640 +0.2968731
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/
		char szCmd[128];
		sprintf(szCmd,"AT$BQVCALD=0,%u\r",nIndex);
		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CVCALENDARDeleteRequest(){;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/*
		003243: Bulk or Interrupt Transfer (UP), 10.12.2007 20:18:55.9291250 +16.4531250
		Pipe Handle: 0x89de4ae4 (Endpoint Address: 0x2)
		Send 0x18 bytes to the device:
		41 54 24 42 51 56 43 41 4C 44 3D 30 2C 2D 38 34   AT$BQVCALD=0,-84
		32 31 35 30 34 35 31 0D                           2150451.
		003244: Bulk or Interrupt Transfer (UP), 10.12.2007 20:18:55.9291250 +0.0
		Pipe Handle: 0x89de4ac4 (Endpoint Address: 0x82)
		Get 0x15 bytes from the device:
		0D 0A 24 42 51 56 43 41 4C 45 52 52 3A 20 31 0D   ..$BQVCALERR: 1.
		0A 4F 4B 0D 0A                                    .OK..
		00324
		*/
		m_bRes = IsOK(spBuffer) && spBuffer->GetLength() == 6;
		return true;
	}
private:
	bool&	m_bRes;
};

class CVCALENDARListRequest: public CVCALENDARBaseRequest
{
public:
	CVCALENDARListRequest(vector<UINT>* pIdxList,int nTimeOut)
		:CVCALENDARBaseRequest(nTimeOut)
		,m_pIdxList(pIdxList)
	{
		/*
		017173: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:58.9179371 +0.0312580
		Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
		Send 0xd bytes to the device:
		41 54 24 42 51 56 43 41 4C 52 3D 3F 0D            AT$BQVCALR=?.
		017174: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:58.9179371 +0.0
		Pipe Handle: 0x8954f98c (Endpoint Address: 0x81)
		Get 0xa bytes from the device:
		A1 20 00 00 00 00 02 00 03 00                     ! ........
		017175: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:59.2774041 +0.3594670
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..
		017176: Bulk or Interrupt Transfer (UP), 07.12.2007 09:17:59.2774041 +0.0
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0x55 bytes from the device:
		24 42 51 56 43 41 4C 52 3A 31 38 0D 0A 31 37 0D   $BQVCALR:18..17.
		0A 31 38 0D 0A 32 30 0D 0A 32 31 0D 0A 32 32 0D   .18..20..21..22.
		0A 32 33 0D 0A 32 34 0D 0A 32 35 0D 0A 32 36 0D   .23..24..25..26.
		0A 32 37 0D 0A 32 38 0D 0A 32 39 0D 0A 33 30 0D   .27..28..29..30.
		0A 33 31 0D 0A 33 32 0D 0A 33 33 0D 0A 31 34 0D   .31..32..33..14.
		0A 31 30 0D 0A                                    .10..
		*/
		char szCmd[128] = {"AT$BQVCALR=?\r"};
		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CVCALENDARListRequest(){;}

	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		if(IsOK(spBuffer))
		{
			const char* pData = spBuffer->GetData();
			int nLen = spBuffer->GetLength();
			if(nLen > 0)
			{
				char* szBuf = new char [nLen + 1];
				memset(szBuf,0,nLen + 1);
				memcpy(szBuf,pData,nLen);

				int nIndex = 0;
				char szSeps[] = {":\r\nOK"};
				// Establish string and get the first token:
				char* sztoken = strtok( szBuf, szSeps ); // C4996
				// Note: strtok is deprecated; consider using strtok_s instead
				while( sztoken != NULL )
				{
					nIndex++;
					if(nIndex > 2)
					{
						int nValue = atoi(sztoken);
						m_pIdxList->push_back(nValue);
					}
					// Get next token: 
					sztoken = strtok( NULL, szSeps ); // C4996
				}

				delete [] szBuf;
			}
		}
		return true;
	}
private:
	vector<UINT>* m_pIdxList;
};

class CVCALENDARRangeRequest: public CVCALENDARBaseRequest
{
public:
	CVCALENDARRangeRequest(UINT* pnMinIndex,UINT* pnMaxIndex,UINT* pnMaxCount,int nTimeOut)
		:CVCALENDARBaseRequest(nTimeOut)
		,m_pnMaxIndex(pnMaxIndex)
		,m_pnMaxCount(pnMaxCount)
		,m_pnMinIndex(pnMinIndex)
	{
		/*
		057710: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:35.2504913 +0.0156249
		Pipe Handle: 0x8954f9cc (Endpoint Address: 0x2)
		Send 0xd bytes to the device:
		41 54 24 42 51 56 43 41 4C 44 3D 3F 0D            AT$BQVCALD=?.
		057711: Bulk or Interrupt Transfer (UP), 07.12.2007 10:27:35.2504913 +0.0
		Pipe Handle: 0x8954f9ac (Endpoint Address: 0x82)
		Get 0x21 bytes from the device:
		0D 0A 24 42 51 56 43 41 4C 44 3A 20 28 30 2D 31   ..$BQVCALD: (0-1
		29 2C 28 31 2D 35 30 30 29 0D 0A 0D 0A 4F 4B 0D   ),(1-500)....OK.
		0A                                                .
		*/		
		char szCmd[128] = {"AT$BQVCALD=?\r"};
		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CVCALENDARRangeRequest(){;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bRes = true;
		if(IsOK(spBuffer))
		{			
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

					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					//2007/12/14 strange issue for test Athens
					//Flient is ..$BQVCALD: (0-1),(1-500)....OK., but Athens is ..$BQVCALD: (0-1)....OK.
					/*
					if(nIndex == 1 && 0)
					{//(1-500)....OK
						char *pIndex = sztoken;
						for(char *pIndex = sztoken;pIndex[0] != ')';pIndex++)
						{
							if(pIndex[0] == '-')
							{
								pIndex++;
								*m_pnMaxIndex = atoi(pIndex);
								*m_pnMaxCount = *m_pnMaxIndex;
								break;
							}
						}
					}
					*/
					//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
				}

				delete [] szBuf;
			}
		}

		return bRes;
	}
private:
	UINT* m_pnMinIndex;
	UINT* m_pnMaxIndex;
	UINT* m_pnMaxCount;
};

#endif //_ATVCALENDARREQUEST_H_