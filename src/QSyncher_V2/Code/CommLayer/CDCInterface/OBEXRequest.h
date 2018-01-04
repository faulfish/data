#pragma once

#include "..\..\CommLayer\CDCInterface\Request\OBEXSYNCMLBase.h"

class COBEXFS_Open : public COBEXFSBase
{
public:
	COBEXFS_Open()
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXConnectFileSystem();
	}
	virtual ~COBEXFS_Open(){}
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		BYTE* pBuffer = (BYTE*)spBuffer->GetData();
		DoSetOpen((pBuffer[0] == 0xA0));
		if(IsOpen())
		{
			CObexCodecPtr ptObex = new CObexCodec();
			ptObex->DecodePackage(spBuffer,CObexCodec::oreqConnect);
			m_dwConnID() = ptObex->GetConnId();
			m_dwMaxObexPackageSize() = ptObex->GetMaxObexPackageSize();
		}
		return IsOpen();
	}

private:
	virtual const bool DoCanClose(){return true;}
};

class PutFileBase : public COBEXFSBase
{
protected:
	PutFileBase(bool bLastPackage):m_bFinish(false),m_bLastPackage(bLastPackage){}
	virtual ~PutFileBase(){}	
public:
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		BYTE* pBuffer = (BYTE*)m_ptBufferReceiver->GetData();
		assert(pBuffer[0] == 0x90 || pBuffer[0] == 0x10 || pBuffer[0] == 0xA0);
		if(m_bFinish == false && (pBuffer[0] == 0x90 || pBuffer[0] == 0x10))
		{
			PrepareSend(m_spBuffer);
			DoRequest();
		}
		return true;
	}
protected:
	bool IsFinish() const {return m_bFinish;}
	void PrepareSend(CBufferPtr spBuffer)
	{		
		DWORD dwDataSize = 0;
		m_ptBufferSend = CObexCodec::CreateReqOBEXPutFileNext(m_dwConnID(),spBuffer,m_bLastPackage,m_dwMaxObexPackageSize(),dwDataSize);
		ExtractUnsend(spBuffer,dwDataSize);

		//Change bLastPackage flag if true
		if(m_bLastPackage == true)
		{
			if(IsFinish() == false)
				m_ptBufferSend = CObexCodec::CreateReqOBEXPutFileNext(m_dwConnID(),spBuffer,false,m_dwMaxObexPackageSize(),dwDataSize);
		}
	}
	int ExtractUnsend(CBufferPtr spSendBuffer,int nHadSend)
	{
		int nUnsentLength = 0;
		int nBufferLength = spSendBuffer->GetLength();
		m_bFinish = (nHadSend >= nBufferLength);
		if(m_bFinish == false)
		{
			char* pTemp = const_cast<char*>(spSendBuffer->GetData());
			pTemp += nHadSend;
			nUnsentLength = nBufferLength-nHadSend;
			m_spBuffer = new CBuffer(nUnsentLength,pTemp);			
		}
		return nUnsentLength;
	}
private:
	virtual const bool DoCanClose(){return true;}
	bool		m_bFinish;
	CBufferPtr	m_spBuffer;
	bool			m_bLastPackage;
};

class COBEXFS_PutFirst : public PutFileBase
{
public:
	COBEXFS_PutFirst(const TSTRING& strFileName, DWORD dwFileSize,CBufferPtr spBuffer, bool bLastPackage):PutFileBase(bLastPackage)
	{
		DWORD dwDataSize = 0;
		m_ptBufferSend = CObexCodec::CreateReqOBEXPutFileFirst(m_dwConnID(),strFileName.c_str(),dwFileSize,spBuffer,bLastPackage,m_dwMaxObexPackageSize(),dwDataSize);
		ExtractUnsend(spBuffer,dwDataSize);
		
		//Change bLastPackage flag if true
		if(bLastPackage == true)
		{
			if(IsFinish() == false)
				m_ptBufferSend = CObexCodec::CreateReqOBEXPutFileFirst(m_dwConnID(),strFileName.c_str(),dwFileSize,spBuffer,false,m_dwMaxObexPackageSize(),dwDataSize);
		}
	}
	virtual ~COBEXFS_PutFirst(){}

private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_PutData : public PutFileBase
{
public:
	COBEXFS_PutData(CBufferPtr spBuffer, bool bLastPackage):PutFileBase(bLastPackage)
	{
		PrepareSend(spBuffer);
	}
	virtual ~COBEXFS_PutData(){}	
};

class COBEXFS_PutFile : public COBEXFSBase
{
public:
	COBEXFS_PutFile(const TSTRING& strFileName,DWORD dwBUFFERSIZE = 1024):m_dwBUFFERSIZE(dwBUFFERSIZE),m_hFile(NULL),m_dwHadSend(0),m_dwFileSize(0)
	{
		DWORD dwDataSize = 0;
		m_hFile = CreateFile(
			strFileName.c_str(),
			GENERIC_READ,
			0,
			NULL,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,NULL);

		if(m_hFile)
		{
			m_dwFileSize = GetFileSize(m_hFile,NULL);

			CBufferPtr ptBuffer = new CBuffer;
			char* szBuffer = ptBuffer->GetBufferSetLength(m_dwBUFFERSIZE);
			DWORD dwRead = 0;
			if(ReadFile(m_hFile,szBuffer,m_dwBUFFERSIZE,&dwRead,NULL))
			{
				m_dwHadSend += dwRead;
				if(dwRead <= m_dwBUFFERSIZE)
					ptBuffer->ReleaseBuffer(dwRead);
				m_ptBufferSend = CObexCodec::CreateReqOBEXPutFileFirst(
					m_dwConnID(),
					strFileName.c_str(),
					m_dwFileSize,
					ptBuffer,
					dwRead >= m_dwFileSize,
					m_dwMaxObexPackageSize(),
					dwDataSize);
				m_dwHadSend += dwRead;
			}
		}
	}
	virtual ~COBEXFS_PutFile()
	{
		CloseFile();
	}
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bFinish = false;
		if(bFinish == false)
		{				
			BYTE* pBuffer = (BYTE*)m_ptBufferReceiver->GetData();
			if(pBuffer[0] == 0x90 || pBuffer[0] == 0x10)
			{			
				bFinish = false;
				CBufferPtr ptBuffer = new CBuffer;
				char* szBuffer = ptBuffer->GetBufferSetLength(m_dwBUFFERSIZE);
				DWORD dwRead = 0;
				if(ReadFile(m_hFile,szBuffer,m_dwBUFFERSIZE,&dwRead,NULL))
				{				
					if(dwRead <= m_dwBUFFERSIZE)
						ptBuffer->ReleaseBuffer(dwRead);
					m_ptBufferSend = CObexCodec::CreateReqOBEXPutFileNext(
						m_dwConnID(),
						ptBuffer,
						m_dwHadSend + dwRead >= m_dwFileSize,
						m_dwMaxObexPackageSize(),
						dwRead);					
					m_dwHadSend += dwRead;
					DoRequest();
					bFinish = true;
				}
			}
			else if(pBuffer[0] == 0xA0)
			{
				CloseFile();
				bFinish = true;
			}
		}
		return bFinish;
	}
private:
	virtual const bool DoCanClose(){return true;}
	void CloseFile()
	{
		if(m_hFile)
		{
			CloseHandle(m_hFile);
			m_hFile = NULL;
		}
	}
private:
	HANDLE	m_hFile;
	DWORD	m_dwBUFFERSIZE;
	DWORD	m_dwHadSend;
	DWORD	m_dwFileSize;
};

class COBEXFS_GetFirst : public COBEXFSBase
{
public:
	COBEXFS_GetFirst(const TSTRING& strFileName)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXGetFileFirst(m_dwConnID(),strFileName.c_str());
	}
	virtual ~COBEXFS_GetFirst(){}

private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_GetData : public COBEXFSBase
{
public:
	COBEXFS_GetData()
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXGetFileNext(m_dwConnID());
	}
	virtual ~COBEXFS_GetData(){}
private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_SetPath : public COBEXFSBase
{
public:
	COBEXFS_SetPath(const TSTRING& strPath, bool bCreateDir = false)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXSetPath(m_dwConnID(),strPath.c_str(),bCreateDir);
	}
	virtual ~COBEXFS_SetPath(){}
private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_SetFullPath : public COBEXFSBase
{
public:
	COBEXFS_SetFullPath(const TSTRING& strFullPath, bool bCreateDir = false, DWORD dwTimeout = INFINITE):m_bCreateDir(bCreateDir),COBEXFSBase(dwTimeout)
	{// sample /user/Pictures or user/Pictures or \user\Pictures or user\Pictures		
		if(strFullPath.size() > 0)
		{
			TCHAR string[MAX_PATH] = {0};
			_tcscpy_s(string,strFullPath.size()*sizeof(TCHAR),strFullPath.c_str());
			TCHAR seps[]   = _T("/\\");
			TCHAR *token,*next_token;
			token = _tcstok_s(string, seps, &next_token);
			while (token != NULL)
			{
				m_Paths.push_back(token);
				token = _tcstok_s( NULL, seps, &next_token);
			}			
		}
		//to root
		m_ptBufferSend = CObexCodec::CreateReqOBEXSetPath(m_dwConnID(),NULL,false);
	}
	virtual ~COBEXFS_SetFullPath(){}	
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bFinish = false;
		if(bFinish == false)
		{
			BYTE* pBuffer = (BYTE*)m_ptBufferReceiver->GetData();
			if(m_Paths.size() > 0 && pBuffer[0] == 0xA0)
			{
				m_ptBufferReceiver = new CBuffer;
				if(m_Paths.size() == 1)
					m_ptBufferSend = CObexCodec::CreateReqOBEXSetPath(m_dwConnID(),m_Paths.begin()->c_str(),m_bCreateDir);
				else
					m_ptBufferSend = CObexCodec::CreateReqOBEXSetPath(m_dwConnID(),m_Paths.begin()->c_str(),false);
				m_Paths.pop_front();
				DoRequest();
				bFinish = true;
			}
			else bFinish = true;
		}
		return bFinish;
	}
private:
	virtual const bool DoCanClose(){return true;}
private:
	std::list<TSTRING>	m_Paths;
	bool				m_bCreateDir;
};


class COBEXFS_GetCapacity : public COBEXFSBase
{
public:
	COBEXFS_GetCapacity(const TSTRING& strDrive, bool bFreeSpace)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXGetCapacity(m_dwConnID(),strDrive.c_str(),bFreeSpace);
	}
	virtual ~COBEXFS_GetCapacity(){}
private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_Abort : public COBEXFSBase
{
public:
	COBEXFS_Abort()
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXAbort(m_dwConnID());
	}
	virtual ~COBEXFS_Abort(){}
private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_DeleteFile : public COBEXFSBase
{
public:
	COBEXFS_DeleteFile(const TSTRING& strPath)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXDeleteFile(m_dwConnID(),strPath.c_str());
	}
	virtual ~COBEXFS_DeleteFile(){}
private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_CreateFolder : public COBEXFSBase
{
public:
	COBEXFS_CreateFolder(const TSTRING& strDrive)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXCreateStdDirs(m_dwConnID(),strDrive.c_str());
	}
	virtual ~COBEXFS_CreateFolder(){}
private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_MoveFile : public COBEXFSBase
{
public:
	COBEXFS_MoveFile(const TSTRING& strOldFile,const TSTRING& strNewFile,DWORD dwTimeout = 5000):COBEXFSBase(dwTimeout)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXMoveFile(m_dwConnID(),strOldFile.c_str(),strNewFile.c_str());
	}
	virtual ~COBEXFS_MoveFile(){}
private:
	virtual const bool DoCanClose(){return true;}
};

class COBEXFS_GetObject : public COBEXFSBase
{
public:
	COBEXFS_GetObject(std::string strType,CObexCodec::Opcode_t op):m_op(op)
	{
		m_ptBufferSend = CObexCodec::CreateReqOBEXGet(m_dwConnID(),strType.c_str());
	}	
	virtual const bool DoDataArrival(CBufferPtr& spBuffer)
	{
		bool bFinish = false;
		if(bFinish == false)
		{
			BYTE* pBuffer = (BYTE*)m_ptBufferReceiver->GetData();		
			BYTE byteResponse = pBuffer[0];
			if(byteResponse == 0x90 || byteResponse == 0x10)
			{
				CObexCodecPtr ptObex = new CObexCodec;
				ptObex->DecodePackage(m_ptBufferReceiver,m_op);
				m_BufferPts.push_back(ptObex->GetBody());
				m_ptBufferReceiver = new CBuffer;
				m_ptBufferSend = CObexCodec::CreateReqOBEXGetFileNext(m_dwConnID());
				DoRequest();
				bFinish = true;
			}
			else if(byteResponse == 0xA0)
			{						
				m_ptBufferReceiver = new CBuffer;
				for(std::vector<CBufferPtr>::iterator Iter = m_BufferPts.begin();Iter != m_BufferPts.end();Iter++)
					m_ptBufferReceiver->Append(*Iter);
				bFinish = true;
			}
			else
			{
				ASSERT(0);
				bFinish = true;
			}
		}
		return bFinish;
	}
	virtual int GetReceiverResult(const CBufferPtr& ptReceiver)
	{
		int nRet = -1;
		if(ptReceiver->GetLength() > 0)
			nRet = 0xA0;
		return nRet;
	}
	virtual ~COBEXFS_GetObject(){}
private:
	virtual const bool DoCanClose(){return true;}
private:
	CObexCodec::Opcode_t	m_op;
	std::vector<CBufferPtr> m_BufferPts;
};

typedef CRefObjectPtr<COBEXFS_PutFirst>			COBEXFS_PutFirstPtr;
typedef CRefObjectPtr<COBEXFS_PutData>			COBEXFS_PutDataPtr;
typedef CRefObjectPtr<COBEXFS_PutFile>			COBEXFS_PutFilePtr;
typedef CRefObjectPtr<COBEXFS_GetFirst>			COBEXFS_GetFirstPtr;
typedef CRefObjectPtr<COBEXFS_GetData>			COBEXFS_GetDataPtr;
typedef CRefObjectPtr<COBEXFS_SetPath>			COBEXFS_SetPathPtr;
typedef CRefObjectPtr<COBEXFS_SetFullPath>		COBEXFS_SetFullPathPtr;
typedef CRefObjectPtr<COBEXFS_GetCapacity>		COBEXFS_GetCapacityPtr;
typedef CRefObjectPtr<COBEXFS_Abort>			COBEXFS_AbortPtr;
typedef CRefObjectPtr<COBEXFS_DeleteFile>		COBEXFS_DeleteFilePtr;
typedef CRefObjectPtr<COBEXFS_CreateFolder>		COBEXFS_CreateFolderPtr;
typedef CRefObjectPtr<COBEXFS_MoveFile>			COBEXFS_MoveFilePtr;
typedef CRefObjectPtr<COBEXFS_GetObject>		COBEXFS_GetObjectPtr;