/*****************************************************************************/
/** @file:		PIMConfigure.h
  This is the header file that contains all the file system related classes.

  $Author:   Alan Tu  $
  $Version:		1.0 $
  $Date:   Dec 03 2007 10:22:02  $
  */
/*****************************************************************************/
#ifndef __AT_PIMCONFIGURE_REQUEST_H__
#define __AT_PIMCONFIGURE_REQUEST_H__

#include "ATFSRequest.h"
#include "..\..\CommonUtil\UnicodeUtility.h"

class CPIMConfigureBaseRequest: public CATFSBaseRequest
{
protected:
	CPIMConfigureBaseRequest(DWORD dwTimeout = 3000):CATFSBaseRequest(dwTimeout){}
	virtual ~CPIMConfigureBaseRequest(){}
};

class CListPIMGroup: public CPIMConfigureBaseRequest
{
public:
	CListPIMGroup(ePIMType eType, vector<CPIMGroupInfo> *pvGroupList,int nTimeOut):CPIMConfigureBaseRequest(nTimeOut),m_pvGroupList(pvGroupList),m_bRequestDetailData(false)
	{
		char szCmd[128] = {"AT$BQCGR=?\r"};
		m_ptBufferSend = new CBuffer(strlen(szCmd),szCmd);
	}
	virtual ~CListPIMGroup(){;}

	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/*
		000001: Bulk or Interrupt Transfer (UP), 07.08.2007 14:05:58.7968750 +15.2968750
		Pipe Handle: 0x85b7c504 (Endpoint Address: 0x2)
		Send 0xb bytes to the device:
		41 54 24 42 51 43 47 52 3D 3F 0D                  AT$BQCGR=?.
		000002: Bulk or Interrupt Transfer (UP), 07.08.2007 14:05:58.8125000 +0.0156250
		Pipe Handle: 0x85b7c4e4 (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..
		000003: Bulk or Interrupt Transfer (UP), 07.08.2007 14:05:58.8125000 +0.0
		Pipe Handle: 0x85b7c4e4 (Endpoint Address: 0x82)
		Get 0x22 bytes from the device:
		24 42 51 43 47 52 3A 28 36 35 35 33 37 2C 36 35   $BQCGR:(65537,65
		35 33 38 2C 36 35 35 33 39 2C 36 35 35 34 30 29   538,65539,65540)
		0D 0A                                             ..
		000004: Bulk or Interrupt Transfer (UP), 07.08.2007 14:05:58.8125000 +0.0
		Pipe Handle: 0x85b7c4e4 (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/
		if(m_FolderIDs.size() == 0 && m_bRequestDetailData == false)
		{
			int nGarbage = 0;
			if(IsOK(spBuffer))
				nGarbage = 6;
			else if(IsFAIL(spBuffer))
				nGarbage = 9;

			const char* pData = spBuffer->GetData();
			int nLen = m_ptBufferReceiver->GetLength() - nGarbage;
			if(nLen > 0)
			{
				char* szBuf = new char [nLen + 1];
				memset(szBuf,0,nLen + 1);
				memcpy(szBuf,pData,nLen);

				char szSeps[] = {"(),\r\n"};
				// Establish string and get the first token:
				char* sztoken = strtok( szBuf, szSeps ); // C4996
				// Note: strtok is deprecated; consider using strtok_s instead
				while( sztoken != NULL )
				{
					sztoken = strtok( NULL, szSeps );
					if(sztoken != NULL)
						m_FolderIDs.push_back(sztoken);
				}
				delete [] szBuf;
			}
		}

		/*
		000736: Bulk or Interrupt Transfer (UP), 07.08.2007 15:47:35.3125000 +37.8125000
		Pipe Handle: 0x85b7c504 (Endpoint Address: 0x2)
		Send 0xf bytes to the device:
		41 54 24 42 51 43 47 52 3D 36 35 35 34 30 0D      AT$BQCGR=65540.
		000737: Bulk or Interrupt Transfer (UP), 07.08.2007 15:47:35.3125000 +0.0
		Pipe Handle: 0x85b7c4e4 (Endpoint Address: 0x82)
		Get 0xf bytes from the device:
		41 54 24 42 51 43 47 52 3D 36 35 35 34 30 0D      AT$BQCGR=65540.
		000738: Bulk or Interrupt Transfer (UP), 07.08.2007 15:47:35.3125000 +0.0
		Pipe Handle: 0x85b7c4e4 (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..
		000739: Bulk or Interrupt Transfer (UP), 07.08.2007 15:47:35.3125000 +0.0
		Pipe Handle: 0x85b7c4e4 (Endpoint Address: 0x82)
		Get 0x1e bytes from the device:
		24 42 51 43 47 52 3A 36 35 35 34 30 2C 22 56 49   $BQCGR:65540,"VI
		50 22 2C 30 2C 22 22 2C 30 2C 22 22 0D 0A         P",0,"",0,""..
		000740: Bulk or Interrupt Transfer (UP), 07.08.2007 15:47:35.3125000 +0.0
		Pipe Handle: 0x85b7c4e4 (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/

		if(m_bRequestDetailData)
		{
			int nGarbage = 0;
			if(IsOK(spBuffer))
				nGarbage = 6;
			else if(IsFAIL(spBuffer))
				nGarbage = 9;

			const char* pData = spBuffer->GetData();
			int nLen = m_ptBufferReceiver->GetLength() - nGarbage;
			if(nLen > 0)
			{
				char* szBuf = new char [nLen + 1];
				memset(szBuf,0,nLen + 1);
				memcpy(szBuf,pData,nLen);

				char szSeps[] = {":(),\r\n"};
				// Establish string and get the first token:
				char* sztoken = strtok( szBuf, szSeps ); // C4996
				// Note: strtok is deprecated; consider using strtok_s instead
				if( sztoken != NULL )
				{
					sztoken = strtok( NULL, szSeps );
					int nFolderID = atoi(sztoken);
					sztoken = strtok( NULL, szSeps );
					string strDisplay = sztoken;
					sztoken = strtok( NULL, szSeps );
					int nRingID = atoi(sztoken);
					sztoken = strtok( NULL, szSeps );
					string strRingPath = sztoken;
					sztoken = strtok( NULL, szSeps );
					int nImageID = atoi(sztoken);
					sztoken = strtok( NULL, szSeps );
					string strImagePath = sztoken;

					CString strTemp = QSYNC_LIB::UTF8ToUCS2(strDisplay);
					strTemp.Trim(_T("\""));
					CPIMGroupInfo groupInfo(nFolderID,(LPCTSTR)strTemp);
					if(m_pvGroupList)
						m_pvGroupList->push_back(groupInfo);
				}
				delete [] szBuf;
			}
		}

		bool bRes = true;
		if(m_FolderIDs.size() > 0)
		{
			m_bRequestDetailData = true;
			m_strQueryFolderID = m_FolderIDs[m_FolderIDs.size()-1];
			string strSend = "AT$BQCGR=" + m_strQueryFolderID + "\r";
			m_FolderIDs.pop_back();
			m_ptBufferSend = new CBuffer(strSend.size(),strSend.c_str());
			DoRequest();
			bRes = m_FolderIDs.size() <= 0;
		}

		return bRes;
	}
private:
	vector<CPIMGroupInfo>*	m_pvGroupList;
	vector<string>			m_FolderIDs;
	bool					m_bRequestDetailData;
	string					m_strQueryFolderID;
};

#endif // #ifndef __AT_PIMCONFIGURE_REQUEST_H__