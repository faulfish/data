/***********************************************************************************/
//Purpose:
//Load and save many kinds(4) of data in a file
//	enum CommonDataType
//	{
//		None		= 0,
//		Void		= 1,
//		Integer		= 2,
//		String		= 3,
//		WString		= 4
//	};
//
//Requirment:
//Loki library 0.1.5 for VC
//C++ compatibility
//Without MFC
/***********************************************************************************/
#ifndef _DEFINE_COMMONDATA_H_
#define _DEFINE_COMMONDATA_H_

#pragma once

#include <string.h>
#include "..\..\ShareLib\loki-0.1.5\include\loki\SmallObj.h"
#include "..\..\ShareLib\loki-0.1.5\include\loki\smartptr.h"
#include <cassert>
#include <memory>

namespace QSYNC_LIB
{
	using namespace std;
	using namespace Loki;

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING wstring
#define _TC(x) L ## x
#define _tfopen_s   _wfopen_s
#else
#define TSTRING string
#define _TC(x) "x"
#endif
#endif

#define LOKI_ALLOCATOR_PARAMETERS SingleThreaded, 4096, 128, 4, NoDestroy
#define NAME_SIZE	64
#define	CHUNK_SIZE	1024

	enum CommonDataType
	{
		None		= 0,
		Void		= 1,
		Integer		= 2,
		String		= 3,
		WString		= 4
	};

	typedef unsigned char QBYTE;

	union QT1
	{
		char		qc;
		QBYTE		qb[1];
	};

	union QT2
	{
		long		ql;
		wchar_t		qwc;
		QT1			qt1[2];
		QBYTE		qb[2];
	};

	union QT4
	{
		long long	qdl;
		QT2			qt2[2];
		QBYTE		qb[4];
	};

	struct CommonDataHeader
	{
		CommonDataHeader():m_nSize(0),m_Type(None)
		{ 
			::memset(m_szName,0,sizeof(m_szName));
		}
		CommonDataType m_Type;
		char m_szName[NAME_SIZE+1];
		long m_nSize;
	};

	class CommonData// : public SmallObject< LOKI_ALLOCATOR_PARAMETERS > 
	{
		friend class CCommonDataFile;
	public:
		CommonData():m_nVoidSize(0),m_pQBBuffer(NULL),m_nHeaderSize(static_cast<long>(sizeof(CommonDataHeader))),m_bModify(false)
		{
			m_qt2Buffer.ql = 0;
		}
		CommonData(const char* szName):m_nVoidSize(0),m_pQBBuffer(NULL),m_nHeaderSize(static_cast<long>(sizeof(CommonDataHeader))),m_bModify(false)
		{
			m_qt2Buffer.ql = 0;
			SetName(szName);
		}
		CommonData(const CommonData& rhs):m_nVoidSize(0),m_pQBBuffer(NULL),m_nHeaderSize(static_cast<long>(sizeof(CommonDataHeader))),m_bModify(false)
		{		
			m_qt2Buffer.ql = 0;
			*this = rhs;
		}
		virtual ~CommonData()
		{
			if(m_pQBBuffer)
				m_QBAllocator.deallocate(m_pQBBuffer,m_nVoidSize);
			m_pQBBuffer = NULL;
		}
	public:
		const CommonDataType GetType() const {return m_Header.m_Type;}
		void SetType(CommonDataType type)	
		{
			if(m_Header.m_Type != type)
			{
				Modify();
				m_Header.m_Type = type;
			}
		}
		const char* GetName() const {return m_Header.m_szName;}
		void SetName(const char *szBuffer)
		{
			if(szBuffer && ::strlen(szBuffer) > 0)
			{
				Modify();
				::strcpy_s(m_Header.m_szName,sizeof(m_Header.m_szName),szBuffer);
			}
		}
		operator const long()
		{		
			assert(m_Header.m_Type == Integer);
			return  GetInteger();
		}
		operator const char*()
		{		
			assert(m_Header.m_Type == String);
			return  GetString();
		}
		operator const wchar_t*()
		{		
			assert(m_Header.m_Type == WString);
			return  GetWString();
		}
		const char* GetString() const {return m_strBuffer.c_str();}
		const wchar_t* GetWString() const {return m_wstrBuffer.c_str();}
		void SetWString(const wchar_t *wszBuffer)
		{
			SetType(WString);
			if(wszBuffer && ::wcslen(wszBuffer) > 0)
				m_wstrBuffer = wszBuffer;
			else
				m_wstrBuffer.empty();
			long nSize = static_cast<long>(m_wstrBuffer.size()*sizeof(wchar_t));
			long nHeaderSize = static_cast<long>(m_nHeaderSize);
			m_Header.m_nSize = nSize + nHeaderSize;
			Modify();
		}
		void SetString(const char *szBuffer)
		{
			SetType(String);
			if(szBuffer && ::strlen(szBuffer) > 0)
				m_strBuffer = szBuffer;
			else
				m_strBuffer.empty();
			long nSize = static_cast<long>(m_strBuffer.size());
			m_Header.m_nSize = nSize + m_nHeaderSize;
			Modify();
		}
		long GetDataSize() const {return m_Header.m_nSize - m_nHeaderSize;}
		long GetSize() const {return m_Header.m_nSize;}
		const long GetInteger() const {return m_qt2Buffer.ql;}
		void SetInteger(long nInt)
		{
			SetType(Integer);
			m_Header.m_nSize = static_cast<long>(sizeof(long)) + m_nHeaderSize;
			if(m_qt2Buffer.ql != nInt)
			{
				m_qt2Buffer.ql = nInt;
				Modify();
			}
		}
		void SetBuffer(void* pBuffer,long nSize)
		{
			SetType(Void);
			m_nVoidSize = nSize;
			if(m_pQBBuffer)
				m_QBAllocator.deallocate(m_pQBBuffer,m_nVoidSize);
			m_pQBBuffer = m_QBAllocator.allocate(m_nVoidSize);
			::memcpy_s(m_pQBBuffer,m_nVoidSize,pBuffer,m_nVoidSize);
			m_Header.m_nSize = m_nVoidSize + m_nHeaderSize;
			Modify();
		}
		const long GetBuffer (void *pBuffer,long& nSize) const
		{
			long nReturn = 0;
			if(NULL == pBuffer)
			{//Request for size
				nSize = m_nVoidSize;
			}
			else
			{//Request for pBuffer
				::memcpy_s(pBuffer,nSize,m_pQBBuffer,m_nVoidSize);
				nReturn = m_nVoidSize;
			}
			return nReturn;
		}
		CommonData& operator=(const long rhs)
		{
			SetInteger(rhs);
			return *this;
		}
		CommonData& operator=(const std::string& str)
		{
			SetString(str.c_str());
			return *this;
		}	
		CommonData& operator=(const char* szBuffer)
		{
			SetString(szBuffer);
			return *this;
		}
		CommonData& operator=(const CommonData& rhs)
		{
			SetHeader(rhs.GetHeader());
			long nSize = 0;
			rhs.GetDataBuffer(NULL,nSize);
			if(nSize > 0)
			{
				QByteAllocator alloc;
				QBPoint pBuffer = alloc.allocate(nSize+2);
				::ZeroMemory(pBuffer,nSize+2);
				rhs.GetDataBuffer(pBuffer,nSize);
				SetDataBuffer(pBuffer,nSize);
				alloc.deallocate(pBuffer,nSize+2);
			}
			return *this;
		}

		bool operator!=(const CommonData& rhs) const
		{
			return ::strcmp(GetName(),rhs.GetName()) != 0;
		}
		bool operator==(const CommonData& rhs) const
		{
			return ::strcmp(GetName(),rhs.GetName()) == 0;
		}
		bool operator==(const char* szCompare) const
		{
			return ::strcmp(GetName(),szCompare) == 0;
		}
		bool operator>(const CommonData& rhs) const
		{
			return ::strcmp(GetName(),rhs.GetName()) > 0;
		}
		bool operator<(const CommonData& rhs) const
		{
			return ::strcmp(GetName(),rhs.GetName()) < 0;
		}	
		void Save(FILE* pfile)
		{
			if(m_Header.m_Type != None)
			{
				size_t nRet = ::fwrite(&m_Header,m_nHeaderSize,1,pfile);
				assert(nRet == 1);
				long nSize = m_Header.m_nSize - m_nHeaderSize;
				assert(nSize >= 0);
				if(nSize > 0)
				{
					QBYTE* pBuffer = new QBYTE[nSize];
					GetDataBuffer(pBuffer,nSize);
					::fwrite(pBuffer,nSize,1,pfile);
					delete [] pBuffer;
				}				
			}
		}		
		void Load(FILE* pfile)
		{
			::ZeroMemory(&m_Header,m_nHeaderSize);
			size_t nRet = ::fread(&m_Header,m_nHeaderSize,1,pfile);
			assert(nRet == 1);
			int nBufferSize = m_Header.m_nSize - m_nHeaderSize;
			if(nBufferSize > 0)
			{
				BYTE* pBuffer = new BYTE[nBufferSize+2];
				assert(pBuffer);
				if(pBuffer)
				{
					::ZeroMemory(pBuffer,nBufferSize+2);
					nRet = ::fread(pBuffer,nBufferSize,1,pfile);
					assert(nRet == 1);
					SetDataBuffer(pBuffer,nBufferSize);
					delete [] pBuffer;
				}
			}
		}
	protected:
		bool IsModify()
		{
			return m_bModify;
		}
		void ResetModify()
		{
			m_bModify = false;
		}
		void Modify()
		{
			m_bModify = true;
		}
		const CommonDataHeader* GetHeader() const 
		{
			return &m_Header;
		}
		void SetHeader(const CommonDataHeader* pHeader)
		{
			m_Header.m_nSize = pHeader->m_nSize;
			::strcpy_s(m_Header.m_szName,sizeof(m_Header.m_szName),pHeader->m_szName);
			m_Header.m_Type = pHeader->m_Type;
		}
		const long GetDataBuffer(void *pBuffer,long& nSize) const
		{
			long nRetSize = 0;
			if(NULL == pBuffer)
			{
				switch(GetType())
				{
				case WString:
					nSize = static_cast<long>((m_wstrBuffer.size())*sizeof(wchar_t));
					break;
				case String:
					nSize = static_cast<long>(m_strBuffer.size());
					break;
				case Void:
					nSize = m_nVoidSize;		
					break;
				case Integer:
					nSize = static_cast<long>(sizeof(long));
					break;
				default:			
					break;
				};			
			}
			else
			{
				switch(GetType())
				{
				case WString:
					memcpy_s(pBuffer,nSize,const_cast<wchar_t*>(GetWString()),nSize);
					nRetSize = nSize;
					break;
				case String:
					memcpy_s(pBuffer,nSize,const_cast<char*>(GetString()),nSize);
					nRetSize = nSize;
					break;
				case Void:
					if(nSize > 0)
						nRetSize = GetBuffer(pBuffer,nSize);
					break;
				case Integer:
					memcpy_s(pBuffer,nSize,&(m_qt2Buffer.ql),nSize);				
					nRetSize = nSize;
					break;
				default:			
					break;
				};
			}
			return nRetSize;
		}
		void SetDataBuffer(void* pBuffer,long nSize)
		{
			switch(GetType())
			{
			case WString:
				assert(NULL != pBuffer);
				SetWString(static_cast<wchar_t*>(pBuffer));
				break;
			case String:
				assert(NULL != pBuffer);
				SetString(static_cast<char*>(pBuffer));
				break;
			case Void:
				assert(NULL != pBuffer);
				SetBuffer(pBuffer,m_Header.m_nSize - m_nHeaderSize);		
				break;
			case Integer:
				assert(NULL != pBuffer);
				SetInteger(*(long*)(pBuffer));
				break;
			default:
				break;
			};
		}
	private:
		CommonDataHeader			m_Header;	
		string						m_strBuffer;
		wstring						m_wstrBuffer;
		QT2							m_qt2Buffer;
		typedef allocator<QBYTE>	QByteAllocator;
		typedef allocator<QBYTE>::pointer QBPoint;
		QBPoint						m_pQBBuffer;
		QByteAllocator				m_QBAllocator;
		long						m_nVoidSize;//for record length of m_pQBBuffer only
		const long					m_nHeaderSize;
		bool						m_bModify;
	};

	template <class P>
	class QSyncerRefCounted
	{
	public:
		QSyncerRefCounted() 
			: pCount_(new int)
		{
			assert(pCount_!=0);
			*pCount_ = 1;
		}

		QSyncerRefCounted(const QSyncerRefCounted& rhs) 
			: pCount_(rhs.pCount_)
		{}

		// MWCW lacks template friends, hence the following kludge
		template <typename P1>
		QSyncerRefCounted(const QSyncerRefCounted<P1>& rhs) 
			: pCount_(reinterpret_cast<const RefCounted&>(rhs).pCount_)
		{}

		P Clone(const P& val)
		{
			++*pCount_;
			return val;
		}

		bool Release(const P&)
		{
			if (!--*pCount_)
			{
				delete pCount_;
				pCount_ = NULL;
				return true;
			}
			return false;
		}

		void Swap(QSyncerRefCounted& rhs)
		{ std::swap(pCount_, rhs.pCount_); }

		enum { destructiveCopy = false };

	private:
		// Data
		int* pCount_;
	};

	typedef Loki::SmartPtr<CommonData,QSyncerRefCounted>		CommonDataPt;

}

#endif //_DEFINE_COMMONDATA_H_