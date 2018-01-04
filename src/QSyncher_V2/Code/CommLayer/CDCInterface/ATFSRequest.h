/*****************************************************************************/
/** @file:		ATFSRequest.h
This is the header file that contains all the file system related classes.

$Author:   Alan Tu  $
$Version:		1.0 $
$Date:   Mar 03 2008 16:55:34  $
*/
/*****************************************************************************/
#ifndef __ATFSREQUEST_H__
#define __ATFSREQUEST_H__

#include "..\..\CommonUtil\UnicodeUtility.h"
#include "PotocolRequestBase.h"
#include "Global_Def.h"
#include <string>

const LPCTSTR NOTIFY_GET_AT_FILE_DATA = _T("AT_GET");
const LPCTSTR NOTIFY_DATA_MODE = _T("AT_DATA_MODE");

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

class CATFSBaseRequest: public CPotocolRequestBase
{	
public:
	CATFSBaseRequest(DWORD dwTimeout = 5000):CPotocolRequestBase(dwTimeout) {;}
	virtual ~CATFSBaseRequest() {;}

	/* function inherited from CRequest */
	virtual const enumServiceType GetServiceType() const { return SERVICE_AT_CMD; }
	virtual const enumProtocolState GetProtocolType() const { return STATE_AT_CMD; }
	virtual enumProtocolConnection Open() { return CONNECTION_SUCCEEDED; }
	virtual enumProtocolConnection Close() { return CONNECTION_SUCCEEDED; }
	virtual void Abort(){;};
protected:	
	string ToString(CBufferPtr& spBuffer)
	{
		TRACE_FILE_LINE_INFO();
		string strTemp;
		char* szBuf = new char [spBuffer->GetLength() + 1];
		memset(szBuf,0,spBuffer->GetLength() + 1);
		memcpy(szBuf,spBuffer->GetData(),spBuffer->GetLength());
		strTemp = szBuf;
		delete [] szBuf;
		return strTemp;
	}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		TRACE_FILE_LINE_INFO();
		//Filter for echo
		//m_ptBufferSend
		int nCmp = memcmp(m_ptBufferSend->GetData(),spBuffer->GetData(),m_ptBufferSend->GetLength());
		if(nCmp == 0)
		{
			//'6' means \r\nOK\r\n
			int nNewSize = spBuffer->GetLength() - m_ptBufferSend->GetLength() - 6;
			const char* szTemp = spBuffer->GetData();
			szTemp += m_ptBufferSend->GetLength();
			spBuffer = new CBuffer(nNewSize , szTemp);
		}
		return true;
	}
private:
	virtual const bool	DoClose(){return true;}
	virtual const bool	DoOpen(){return true;}
	virtual void		DoSetOpen(const bool bIsOpen){}
	virtual const bool	DoIsOpen(){return true;}
	virtual const bool	DoCanClose(){return true;}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{		
		TRACE_FILE_LINE_INFO();
		//Make sure \r\nOK\r\n at the latest
		return IsCompleteATPackage(spBuffer);
	}
	virtual int GetReceiverResult(const CBufferPtr& ptReceiver)
	{
		TRACE_FILE_LINE_INFO();
		return IsOK(ptReceiver) ? 1 : 0;
	}
};

/*! @class		CRequestATFSGetFile
*  @brief		get a file
*  @author		Kenny Chang
*  @date		2007/04/11
*/
class CRequestATFSGetFile : public CATFSBaseRequest 
{
public:
	CRequestATFSGetFile(LPCTSTR szPath, BOOL bIsDBB, int nTimeOut,int& nFileSize,int& nATFileErrCode):CATFSBaseRequest(nTimeOut),m_nFileSize(nFileSize),m_dwHadReceived(0),m_nATFileErrCode(nATFileErrCode)
	{
		TRACE_FILE_LINE_INFO();

		//1.Define notify
		DefineNotify(NOTIFY_GET_AT_FILE_DATA);
		DefineNotify(NOTIFY_DATA_MODE);

		//2.Prepare AT command string to get the file
		string strAnsi = QSYNC_LIB::widetoansi(szPath,CP_UTF8);
		size_t nAnsiSize = strAnsi.size();
		char* sz = new char[nAnsiSize + 30];
		::ZeroMemory(sz,nAnsiSize + 30);
		//sprintf(sz, "AT$FSRF=%d,0,\"%s\"\r", bIsDBB ? 1 : 4, strAnsi);
		sprintf(sz, "AT$FSRF=5,0,\"%s\"\r", strAnsi.c_str());
		m_ptBufferSend = new CBuffer(strlen(sz),sz);
		delete [] sz;
	}
	virtual ~CRequestATFSGetFile() {;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bFinish = false;

		if(m_nFileSize == 0)
		{
			TRACE_FILE_LINE_INFO();

			const char szHeader[] = {"\r\n$FSRF:"};
			const char szErrorHeader[] = {"\r\n$FSERROR:"};

			if(memcmp(spBuffer->GetData(),szHeader,strlen(szHeader)) == 0 && m_nFileSize <= 0)
				GetFileLength(spBuffer);
			else if(memcmp(spBuffer->GetData(),szErrorHeader,strlen(szErrorHeader)) == 0)
			{
				bFinish = true;
				GetErrorCode(spBuffer);
			}
		}
		else
		{
			TRACE_FILE_LINE_INFO();

			if(spBuffer->GetLength() > 0)
			{
				//Notify to client to receiver data
				m_dwHadReceived += spBuffer->GetLength();

				if(m_nFileSize > 0 && m_dwHadReceived >= m_nFileSize)
				{
					if(IsOK(spBuffer))
						spBuffer->ReleaseBuffer(spBuffer->GetLength()-6);
					bFinish = true;
					FreeToken();
				}

				assert(spBuffer->GetLength() > 0);
				if(spBuffer->GetLength() > 0)
					Fire(NOTIFY_GET_AT_FILE_DATA,(long)spBuffer.GetRawPointer());
			}
			if(bFinish == false)
			{
				if(WaitReceiver() == false)
				{
					m_nATFileErrCode = -32768;
					bFinish = true;
				}
			}
		}
		return bFinish;
	}

	void GetErrorCode(CBufferPtr& spBuffer)
	{
		TRACE_FILE_LINE_INFO();
		/*Fail
		002657: Bulk or Interrupt Transfer (UP), 27.08.2007 10:54:59.2968750 +49.2500000
		Pipe Handle: 0x859d95dc (Endpoint Address: 0x2)
		Send 0x20 bytes to the device:
		41 54 24 46 53 52 46 3D 31 2C 30 2C 22 2F 54 68   AT$FSRF=1,0,"/Th
		65 6D 65 73 2F 44 65 65 70 20 42 6C 75 65 22 0D   emes/Deep Blue".
		002658: Bulk or Interrupt Transfer (UP), 27.08.2007 10:54:59.2968750 +0.0
		Pipe Handle: 0x859d95bc (Endpoint Address: 0x82)
		Get 0x20 bytes from the device:
		41 54 24 46 53 52 46 3D 31 2C 30 2C 22 2F 54 68   AT$FSRF=1,0,"/Th
		65 6D 65 73 2F 44 65 65 70 20 42 6C 75 65 22 0D   emes/Deep Blue".
		002659: Bulk or Interrupt Transfer (UP), 27.08.2007 10:54:59.2968750 +0.0
		Pipe Handle: 0x859d95bc (Endpoint Address: 0x82)
		Get 0x15 bytes from the device:
		0D 0A 24 46 53 45 52 52 4F 52 3A 2D 33 0D 0A 0D   ..$FSERROR:-3...
		0A 4F 4B 0D 0A                                    .OK..
		*/
		/*
		File system AT command error codes
		File system will automatically send out an error code of the form ¡§$FSERROR:<error-code>¡¨ if it encounters some error when accessing the flash ROM. The error codes are provided by XPIFS, and they are listed as below:
		0        Successful
		-1       File system fails to initialize
		-2       File system has been initialized
		-3       Specified file does not exist
		-4       Checksum of data is wrong
		-5       Size of given buffer is larger
		-6       Size of given buffer is smaller
		-7       Free flash space is not enough
		-8       Unknown parameters
		-9       Specified record does not exist
		-10      Can not handle this file type
		-11      Open too many files/directories
		-12      File/directory already exists
		-13      Directory doesn't exist
		-14      No next item
		-15      Flash program error
		-16      Flash erase error
		-17      Delete split file error
		-18      One or some split file`s segment not found
		-19      Too many objects in the directory
		-20      Directory not empty
		-21      Result of Close_An_Opened_Segment
		-22      Object already be opened
		-23      Whole file too large (> 60KB)
		-24      File can't be rewritten but read only 
		-25      File should not be deleted 
		-26      Destination already be programmed
		-27      Invalid access to specified place 
		-28      Path too deep (>10)
		-29      too many files/directories in root directory (based on volume capacity)
		-30      unsupported volume
		-32767   Unknown error
		-32768   Timeout
		*/
		const char szCRLF[] = {"\r\n"};
		char* szRet = strstr(const_cast<char*>(spBuffer->GetData()+2),szCRLF);
		__w64 int nLen = szRet - spBuffer->GetData();
		if(nLen > 0)
		{
			//1. Get error code
			char* szTemp = new char[nLen+1];
			memset(szTemp,0,nLen+1);
			strncpy(szTemp,spBuffer->GetData(),nLen);
			TRACE("Get file ERROR: %s\n",szTemp);
			char *szNum = szTemp + 11;
			m_nATFileErrCode = ::atoi(szNum);
			delete [] szTemp;
		}			

		spBuffer->SetEmpty();

		//Notify for change to data mode
		Fire(NOTIFY_DATA_MODE);
	}

	void GetFileLength(CBufferPtr& spBuffer)
	{
		TRACE_FILE_LINE_INFO();
		/*Succeed
		003003: Bulk or Interrupt Transfer (UP), 20.08.2007 17:27:33.0781250 +38.1562500
		Pipe Handle: 0x85d3749c (Endpoint Address: 0x2)
		Send 0x26 bytes to the device:
		41 54 24 46 53 52 46 3D 31 2C 30 2C 22 2F 50 69   AT$FSRF=1,0,"/Pi
		63 74 75 72 65 73 2F 46 6F 74 6F 5F 30 30 30 31   ctures/Foto_0001
		2E 6A 70 67 22 0D                                 .jpg".
		003004: Bulk or Interrupt Transfer (UP), 20.08.2007 17:27:33.0937500 +0.0156250
		Pipe Handle: 0x85d3747c (Endpoint Address: 0x82)
		Get 0x26 bytes from the device:
		41 54 24 46 53 52 46 3D 31 2C 30 2C 22 2F 50 69   AT$FSRF=1,0,"/Pi
		63 74 75 72 65 73 2F 46 6F 74 6F 5F 30 30 30 31   ctures/Foto_0001
		2E 6A 70 67 22 0D                                 .jpg".
		003005: Bulk or Interrupt Transfer (UP), 20.08.2007 17:27:33.0937500 +0.0
		Pipe Handle: 0x85d3747c (Endpoint Address: 0x82)
		Get 0x2c bytes from the device:
		0D 0A 24 46 53 52 46 3A 31 2C 30 2C 22 2F 50 69   ..$FSRF:1,0,"/Pi
		63 74 75 72 65 73 2F 46 6F 74 6F 5F 30 30 30 31   ctures/Foto_0001
		2E 6A 70 67 22 2C 37 32 35 36 0D 0A               .jpg",7256..
		00300

		............................................................

		*/
		const char szCRLF[] = {"\r\n"};
		char* szRet = strstr(const_cast<char*>(spBuffer->GetData()+2),szCRLF);
		__w64 int nLen = szRet - spBuffer->GetData();
		if(nLen > 0)
		{
			//1. Get length
			char* szTemp = new char[nLen+1];
			memset(szTemp,0,nLen+1);
			strncpy(szTemp,spBuffer->GetData(),nLen);
			char* szLen = strstr(szTemp,"\",");
			m_nFileSize = atoi(szLen+2);
			delete [] szTemp;

			//2.Free response
			spBuffer->PopFront(static_cast<DWORD>(nLen+2));

			//3. Notify for change to data mode
			Fire(NOTIFY_DATA_MODE);
		}
	}
	virtual const bool DoRequest(bool bResursive = false)
	{	
		TRACE_FILE_LINE_INFO();
		GetReceiver()  = new CBuffer;
		bool bRes = g_ProtocolManager.ExecuteProtocolRequest(m_ptBufferSend, this) == S_OK;
		if(bRes)
		{
			WaitReceiver();
			while(DoDataArrival(GetReceiver()) == false);			
			Fire(_T("Received"));
			bRes = true;
		}		
		return bRes;
	}
	void WaitMoreDataWithoutNewRequest()
	{
		TRACE_FILE_LINE_INFO();
		WaitReceiver();
		DoDataArrival(GetReceiver());
	}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{
		return true;
	}
private:
	int&											m_nATFileErrCode;
	int&											m_nFileSize;
	int											m_dwHadReceived;
};

/*! @class		CRequestATFSPutFile
*  @brief		put a file
*/
class CRequestATFSPutFile : public CATFSBaseRequest
{
public:
	CRequestATFSPutFile(CString strFilePath, BOOL bIsDBB, DWORD dwFileSize,int& nErrCode, int nTimeOut)
		:CATFSBaseRequest(nTimeOut)
		,m_strFilePath(strFilePath)
		,m_bIsDBB(bIsDBB)
		,m_dwFileSize(dwFileSize)
		,m_nTimeOut(nTimeOut)
		,m_nErrCode(nErrCode)
	{		  
		m_spATCodec = CAtCodec::CreateReqPutFile(m_strFilePath, m_bIsDBB, m_dwFileSize);
		m_ptBufferSend = m_spATCodec->GetPackageBuffer();
	}
	virtual ~CRequestATFSPutFile() {;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		CATFSBaseRequest::DoDataArrival(spBuffer);
		/*
		000094: Bulk or Interrupt Transfer (UP), 11.01.2008 17:45:52.8084670 +6.4926335
		Pipe Handle: 0x892abbcc (Endpoint Address: 0x2)
		Send 0x4d bytes to the device:
		41 54 24 46 53 57 46 3D 31 2C 30 2C 30 2C 22 2F   AT$FSWF=1,0,0,"/
		4D 69 73 63 65 6C 6C 61 6E 65 6F 75 73 2F 4D 37   Miscellaneous/M7
		5F 53 56 4E 31 30 30 5F 4E 6F 72 74 68 41 73 69   _SVN100_NorthAsi
		61 5F 43 48 54 5F 4C 47 31 31 5F 30 30 39 2E 65   a_CHT_LG11_009.e
		78 65 22 2C 38 30 33 34 37 39 35 38 0D            xe",80347958.
		000095: Bulk or Interrupt Transfer (UP), 11.01.2008 17:45:52.8084670 +0.0
		Pipe Handle: 0x892abbac (Endpoint Address: 0x82)
		Get 0x19 bytes from the device:
		0D 0A 24 46 53 45 52 52 4F 52 3A 20 2D 33 0D 0A   ..$FSERROR: -3..
		0D 0A 45 52 52 4F 52 0D 0A                        ..ERROR..
		*/
		if(spBuffer->GetLength() > 0)
		{
			string strBuffer = ToString(spBuffer);
			string::size_type nIndex = strBuffer.find("$FSERROR:");
			if(nIndex != string::npos)
				m_nErrCode = atoi(strBuffer.substr(nIndex+strlen("$FSERROR:")).c_str());
		}

		return true;
	}
private:
	CAtCodecPtr m_spATCodec;
	CString     m_strFilePath;
	BOOL        m_bIsDBB;
	DWORD       m_dwFileSize;
	int         m_nTimeOut;
	int&		m_nErrCode;
};

/*! @class		CRequestATFSPutFileData
*  @brief		put a block of file data
*  @author		Kenny Chang
*  @date		2007/04/13
*/
class CRequestATFSPutFileData : public CATFSBaseRequest
{
public:
	CRequestATFSPutFileData(BYTE* pFileData, DWORD dwFileSize , bool bLast):m_bLast(bLast)
	{
		m_ptBufferSend = new CBuffer(dwFileSize, (const char*)pFileData);
	}
	virtual ~CRequestATFSPutFileData() {;}
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		return true;
	}
	virtual const bool DoRequest(bool bResursive = false)
	{	
		GetReceiver()  = new CBuffer;
		bool bRes = g_ProtocolManager.ExecuteProtocolRequest(m_ptBufferSend, this) == S_OK;
		if(bRes && m_bLast)
		{
			WaitReceiver();
			while(DoDataArrival(GetReceiver()) == false);
			Fire(_T("Received"));
			bRes = GetReceiver()->GetLength() > 0;
		}		
		return bRes;
	}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{
		return IsCompleteATPackage(spBuffer);
	}
private:
	bool	m_bLast;
};

/*! @class		CRequestATFSDeleteFile
*  @brief		delete a file
*  @author		Kenny Chang
*  @date		2007/04/11
*/
class CRequestATFSDeleteFile : public CATFSBaseRequest
{
public:
	CRequestATFSDeleteFile(CString strFilePath, BOOL bIsDBB, int nTimeOut):
	  CATFSBaseRequest(nTimeOut),
		  m_strFilePath(strFilePath),
		  m_bIsDBB(bIsDBB),
		  m_nTimeOut(nTimeOut)
	  {
		  string strAnsi = QSYNC_LIB::widetoansi((LPCTSTR)strFilePath,CP_UTF8);
		  size_t nAnsiSize = strAnsi.size();
		  char* sz = new char[nAnsiSize + 30];
		  ::ZeroMemory(sz,nAnsiSize + 30);
		  sprintf(sz, "AT$FSMISC=1,%d,\"%s\"\r", bIsDBB ? 1 : 4,strAnsi.c_str());
		  m_ptBufferSend = new CBuffer(strlen(sz),sz);
	  }
	  virtual ~CRequestATFSDeleteFile() {}
private:
	CAtCodecPtr m_spATCodec;
	CString m_strFilePath;
	BOOL m_bIsDBB;
	int    m_nTimeOut;
};

class CRequestATFSRenameFile : public CATFSBaseRequest
{
public:
	CRequestATFSRenameFile(const TSTRING& strNewFilePath,const TSTRING& strOldFilePath, BOOL bIsDBB, int nTimeOut):CATFSBaseRequest(nTimeOut)
	  {
		  CStringA	strAnsiOldFile			= QSYNC_LIB::widetoansi(strOldFilePath.c_str(),CP_UTF8).c_str();
		  CStringA	strAnsiNewFile			= QSYNC_LIB::widetoansi(strNewFilePath.c_str(),CP_UTF8).c_str();

		  char* sz = new char[strAnsiOldFile.GetLength()+ strAnsiNewFile.GetLength() + 30];
		  ::ZeroMemory(sz,strAnsiOldFile.GetLength()+ strAnsiNewFile.GetLength() + 30);
		  sprintf(sz, "AT$FSMISC=2,%d,\"%s\",\"%s\"\r", bIsDBB ? 1 : 4, strAnsiNewFile,strAnsiOldFile);
		  m_ptBufferSend = new CBuffer(strlen(sz),sz);
		  delete [] sz;
	  }
	  virtual ~CRequestATFSRenameFile() {}
};

class CRequestATFSListDirectory : public CATFSBaseRequest
{
public:
	CRequestATFSListDirectory(LPCTSTR strPath, BOOL bIsDBB,string& strFolders, BOOL bContinue = FALSE) :
	  CATFSBaseRequest(10000),
	  m_bContinue(bContinue),
		  m_strFolders(strFolders)
	  {
		  m_spATCodec = CAtCodec::CreateReqGetEntries(strPath, bIsDBB , m_bContinue);
		  m_ptBufferSend = m_spATCodec->GetPackageBuffer();
	  }
	  virtual ~CRequestATFSListDirectory() {;}
protected:
	virtual const bool	DoDataArrival(CBufferPtr& spBuffer)
	{
		/*
		000442: Bulk or Interrupt Transfer (UP), 20.08.2007 14:35:05.7656250 +12.2656250
		Pipe Handle: 0x85d3749c (Endpoint Address: 0x2)
		Send 0xf bytes to the device:
		41 54 24 46 53 44 49 52 3D 36 2C 22 2F 22 0D      AT$FSDIR=6,"/".
		000443: Bulk or Interrupt Transfer (UP), 20.08.2007 14:35:05.7656250 +0.0
		Pipe Handle: 0x85d3747c (Endpoint Address: 0x82)
		Get 0xf bytes from the device:
		41 54 24 46 53 44 49 52 3D 36 2C 22 2F 22 0D      AT$FSDIR=6,"/".
		000444: Bulk or Interrupt Transfer (UP), 20.08.2007 14:35:06.0156250 +0.2500000
		Pipe Handle: 0x85d3747c (Endpoint Address: 0x82)
		Get 0x14 bytes from the device:
		0D 0A 24 46 53 44 49 52 3A 36 2C 22 2F 22 0D 0A   ..$FSDIR:6,"/"..
		0D 0A 4F 4B                                       ..OK
		000445: Bulk or Interrupt Transfer (UP), 20.08.2007 14:35:06.0156250 +0.0
		Pipe Handle: 0x85d3747c (Endpoint Address: 0x82)
		Get 0x2 bytes from the device:
		0D 0A                                             ..

		000446: Bulk or Interrupt Transfer (UP), 20.08.2007 14:35:06.0156250 +0.0
		Pipe Handle: 0x85d3747c (Endpoint Address: 0x82)
		Get 0x7a bytes from the device:
		3C 54 68 65 6D 65 73 2C 22 52 2D 22 2C 30 3E 3C   <Themes,"R-",0><
		47 61 6D 65 73 2C 22 52 2D 22 2C 30 3E 3C 41 70   Games,"R-",0><Ap
		70 6C 69 63 61 74 69 6F 6E 73 2C 22 52 2D 22 2C   plications,"R-",
		30 3E 3C 4D 69 73 63 65 6C 6C 61 6E 65 6F 75 73   0><Miscellaneous
		2C 22 52 2D 22 2C 30 3E 3C 50 69 63 74 75 72 65   ,"R-",0><Picture
		73 2C 22 52 2D 22 2C 30 3E 3C 53 6F 75 6E 64 73   s,"R-",0><Sounds
		2C 22 52 2D 22 2C 30 3E 3C 56 69 64 65 6F 73 2C   ,"R-",0><Videos,
		22 52 2D 22 2C 30 3E 45 4F 46                     "R-",0>EOF
		000447: Bulk or Interrupt Transfer (UP), 20.08.2007 14:35:06.0156250 +0.0
		Pipe Handle: 0x85d3747c (Endpoint Address: 0x82)
		Get 0x6 bytes from the device:
		0D 0A 4F 4B 0D 0A                                 ..OK..
		*/
		if(IsOK(spBuffer))
			CheckContent(spBuffer,m_strFolders);
		return true;
	}
	virtual const bool  DoIsFinishPackage(CBufferPtr& spBuffer)
	{
		bool bFinish = false;
		bFinish = IsFAIL(spBuffer);
		if(bFinish == false)
		{
			std::string strRetuenContent;
			bFinish = CheckContent(spBuffer,strRetuenContent) == 2;
		}
		return bFinish;
	}
	int CheckContent(CBufferPtr& spBuffer,std::string& strRetuenContent)
	{
		int nCheckFlag = 0;
		const BYTE szATEnd[6] = {0x0d,0x0a,'O','K',0x0d,0x0a};
		const char *pReveiver = spBuffer->GetData();
		DWORD dwLen = spBuffer->GetLength();
		DWORD nContentIndex = -1;
		DWORD nIndex = 0;
		for(nIndex = 0;nIndex <= (dwLen-6);nIndex++)
		{
			if(memcmp(pReveiver,szATEnd,6) == 0)
				nCheckFlag++;

			if(nCheckFlag == 2)
				break;
			else if(nCheckFlag == 1 && nContentIndex == -1)
				nContentIndex = nIndex + 6;

			pReveiver++;
		};
		if(nCheckFlag == 2 && nContentIndex >= 0)
		{
			const char *pReveiver = spBuffer->GetData();
			pReveiver += nContentIndex;
			strRetuenContent = pReveiver;
			strRetuenContent = strRetuenContent.substr(0,nIndex - nContentIndex);
		}
		return nCheckFlag;
	}
private:
	CAtCodecPtr m_spATCodec;
	BOOL		m_bContinue;
	string&		m_strFolders;
};

class CRequestATFSCreateFolder : public CATFSBaseRequest
{
public:
	CRequestATFSCreateFolder::CRequestATFSCreateFolder(CString strFolderName, BOOL bIsDBB, int nTimeOut) : 
	  CATFSBaseRequest(nTimeOut),
		  m_strFolderName(strFolderName),
		  m_bIsDBB(bIsDBB),
		  m_nTimeOut(nTimeOut)
	  {
		  m_spATCodec = CAtCodec::CreateReqCreateDir(m_strFolderName, m_bIsDBB);
		  m_ptBufferSend = m_spATCodec->GetPackageBuffer();
	  }
	  virtual ~CRequestATFSCreateFolder() {;}
private:
	CAtCodecPtr		m_spATCodec;
	CString			m_strFolderName;
	BOOL			m_bIsDBB;
	int				m_nTimeOut;
};

/*! @class		CRequestATFSRemoveDirectory
*  @brief		remove an empty diectory
*  @author		Kenny Chang
*  @date		2007/04/11
*/
class CRequestATFSRemoveDirectory : public CATFSBaseRequest
{
public:
	CRequestATFSRemoveDirectory(CString strFolderName, BOOL bIsDBB, int nTimeOut) : 
	  CATFSBaseRequest(nTimeOut),
		  m_strFolderName(strFolderName),
		  m_bIsDBB(bIsDBB),
		  m_nTimeOut(nTimeOut)
	  {
		  m_spATCodec = CAtCodec::CreateReqRemoveDir(m_strFolderName, m_bIsDBB);
		  m_ptBufferSend = m_spATCodec->GetPackageBuffer();
	  }
	  virtual ~CRequestATFSRemoveDirectory() {;}
private:
	CAtCodecPtr		m_spATCodec;
	CString			m_strFolderName;
	BOOL			m_bIsDBB;
	int				m_nTimeOut;
};

#pragma warning(pop)

#endif // #ifndef __ATFSREQUEST_H__