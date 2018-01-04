/***********************************************************************************/
//Purpose:
//Load and save many data from commondata in a file
//
//Requirment:
//Need commondata.h
//Loki library 0.1.5 for VC
//C++ compatibility
//Without MFC
/***********************************************************************************/
#ifndef _DEFINE_COMMONDATAFILE_H_
#define _DEFINE_COMMONDATAFILE_H_

#pragma once

#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>
#include "commondata.h"
#include <map>

namespace QSYNC_LIB
{

	struct CCommonDataFileHeader
	{
		CCommonDataFileHeader():m_nCount(0)
		{
			m_qt4Size.qdl = 0;
			::memset(m_szFormat,0,sizeof(m_szFormat));
			::memset(m_szVersion,0,sizeof(m_szVersion));
			::strcpy_s(m_szFormat,sizeof(m_szFormat),"DATAFILE");
			::strcpy_s(m_szVersion,sizeof(m_szVersion),"10000000");
		}
		~CCommonDataFileHeader(){}
		CCommonDataFileHeader& operator=(const CCommonDataFileHeader& Header)
		{
			this->m_GUID = Header.m_GUID;
			this->m_nCount = Header.m_nCount;
			memcpy(&this->m_qt4Size, &Header.m_qt4Size, sizeof(QT4));
			strcpy_s(this->m_szFormat, Header.m_szFormat);
			strcpy_s(this->m_szVersion, Header.m_szVersion);
			return *this;
		}
		char	m_szFormat[9];
		char	m_szVersion[9];
		QT4		m_qt4Size;
		long	m_nCount;
		GUID	m_GUID;
	};
	
	interface IPIMObj
	{		
		virtual bool encode(TSTRING& strRAW) = 0;
		virtual bool decode(const TSTRING& strRAW) = 0;
		virtual bool encode(void** pVoid,long& nSize) = 0;
		virtual bool decode(void* pVoid,const long& nSize) = 0;
		virtual size_t GetByName(const char *szName,CommonDataPt& spData) = 0;
		virtual size_t SetByName(const char *szName,const CommonDataPt& spData) = 0;
		virtual bool RemoveByName(const char *szName) = 0;
		virtual const long GetCount() const = 0;
		virtual const long long GetSize() const = 0;
		virtual const GUID& GetGUID() const = 0;
		virtual bool SetGUID(const GUID& guid) = 0;
		virtual void Assign(const IPIMObj& PIMObj) = 0;
		virtual void Save(FILE *stream) = 0;		
		virtual void Load(FILE *stream) = 0;		
		virtual bool IsModify() = 0;
		virtual void ResetModify() = 0;
		virtual string GetString(const char *szName) = 0;
		virtual wstring GetWString(const char *szName) = 0;
		virtual int GetInteger(const char *szName) = 0;
		virtual void Clear() = 0;
	public:		
		virtual ~IPIMObj(){};
	};

	class CCommonDataFile : public IPIMObj
	{
		typedef std::map<std::string,CommonDataPt>					CommonDataMap;
		typedef std::pair<std::string,CommonDataPt>					CommonDataMapPair;
		typedef std::map<std::string,CommonDataPt>::iterator		CommonDataMapIter;
	public:
		CCommonDataFile():m_bModify(false),m_nHeaderSize(static_cast<long>(sizeof(CCommonDataFileHeader)))
		{
		}
		CCommonDataFile(const GUID& guid):m_bModify(false),m_nHeaderSize(static_cast<long>(sizeof(CCommonDataFileHeader)))
		{
			m_Header.m_GUID = guid;
		}
		virtual ~CCommonDataFile()
		{
			m_DataMap.clear();
		}
		virtual void Clear()
		{
			SetGUID(GUID_NULL);
			m_Header.m_qt4Size.qdl = 0;
			m_Header.m_nCount = 0;
			ResetModify();
			m_DataMap.clear();
		}
		virtual bool encode(TSTRING& strRAW){assert(0);return false;}
		virtual bool decode(const TSTRING& strRAW){assert(0);return false;}
		virtual bool encode(void** pVoid,long& nSize){assert(0);return false;}
		virtual bool decode(void* pVoid,const long& nSize){assert(0);return false;}
		virtual const GUID& GetGUID() const
		{
			return m_Header.m_GUID;
		}	
		virtual bool SetGUID(const GUID& guid)
		{
			m_Header.m_GUID = guid;
			return true;
		}
		virtual size_t GetByName(const char *szName,CommonDataPt& spData)  
		{
			size_t nRealIndex = -1;			
			CommonDataMapIter cIter = FindByName(szName);
			if(cIter != m_DataMap.end())
			{
				spData = cIter->second;
				nRealIndex = 999;
			}
			return nRealIndex;
		}
		virtual size_t SetByName(const char *szName,const CommonDataPt& spData)
		{
			size_t nRealIndex = 999;
			CommonDataMapIter Iter = FindByName(szName);
			spData->SetName(szName);
			if(Iter != m_DataMap.end())
			{			
				Iter->second = spData;
			}
			else
			{
				InsertData(spData);
			}
			Modify();
			return nRealIndex;
		}
		
		virtual string GetString(const char *szName)
		{
			string strRes;
			CommonDataPt spData;
			if(GetByName(szName,spData) != -1)
				strRes = spData->GetString();
			return strRes;
		}

		virtual wstring GetWString(const char *szName)
		{
			wstring strRes;
			CommonDataPt spData;
			if(GetByName(szName,spData) != -1)
				strRes = spData->GetWString();
			return strRes;
		}

		virtual int GetInteger(const char *szName)
		{
			int nRes;
			CommonDataPt spData;
			if(GetByName(szName,spData) != -1)
				nRes = spData->GetInteger();
			return nRes;
		}

		//return false means not found
		virtual bool RemoveByName(const char *szName)
		{
			bool bRes = false;
			CommonDataMapIter Iter = FindByName(szName);
			if(Iter != m_DataMap.end())
			{
				m_DataMap.erase(Iter);
				m_Header.m_nCount = static_cast<int>(m_DataMap.size());
				bRes = true;
			}
			return bRes;
		}
		virtual const long GetCount() const {return static_cast<long>(m_DataMap.size());}
		virtual const long long GetSize() const {return m_Header.m_qt4Size.qdl;}
		virtual void Assign(const IPIMObj& PIMObj)
		{
			const CCommonDataFile *pCmnDataFile = static_cast<const CCommonDataFile*>(&PIMObj);
			m_Header = *(pCmnDataFile->GetHeader());
			m_bModify = pCmnDataFile->IsModify();
			static_cast<long>(m_nHeaderSize) = sizeof(m_Header);
			m_DataMap.clear();
			for(CommonDataMap::const_iterator iter = pCmnDataFile->GetMap()->begin(); iter != pCmnDataFile->GetMap()->end(); ++iter)
			{m_DataMap.insert(*iter);}
		}
		virtual void Save(FILE* pfile)
		{
			if(pfile)
			{
				size_t nRet = ::fwrite(&m_Header,m_nHeaderSize,1,pfile);
				assert(nRet == 1);
				for(CommonDataMap::iterator cIter = m_DataMap.begin();cIter != m_DataMap.end();cIter++)
					cIter->second->Save(pfile);
			}
		}
		virtual void Load(FILE* pfile)
		{
			::ZeroMemory(&m_Header,m_nHeaderSize);
			size_t nHadRead = ::fread(&m_Header,m_nHeaderSize,1,pfile);
			assert(nHadRead == 1);
			if(nHadRead == 1)
			{
				int nCount = m_Header.m_nCount;
				for(int i=0;i<nCount;i++)
				{
					CommonDataPt spData = new CommonData;
					spData->Load(pfile);
					InsertData(spData);
				}
			}
		}		
		virtual bool IsModify()
		{
			return m_bModify;
		}
		virtual bool IsModify() const
		{
			return m_bModify;
		}
		virtual const CommonDataMap* GetMap() const
		{
			return &m_DataMap;
		}
		virtual void ResetModify()
		{
			m_bModify = false;
		}
	protected:
		void InsertData(CommonDataPt spData)
		{
			m_DataMap.insert(CommonDataMapPair(spData->GetName(),spData));			
			m_Header.m_nCount = static_cast<int>(m_DataMap.size());
		}
		void Modify()
		{
			calSize();
			m_bModify = true;
		}
		void calSize()
		{
			m_Header.m_qt4Size.qdl = 0;
			for(CommonDataMap::iterator cIter = m_DataMap.begin();cIter != m_DataMap.end();cIter++)
				m_Header.m_qt4Size.qdl += cIter->second->GetSize();
			m_Header.m_qt4Size.qdl += m_nHeaderSize;
		}	
		CommonDataMapIter FindByName(const char* szName)
		{
			return m_DataMap.find(szName);
		}
		const CCommonDataFileHeader* GetHeader() const
		{
			return &m_Header;
		}
		
	private:
		CCommonDataFileHeader					m_Header;
		CommonDataMap							m_DataMap;
		const long								m_nHeaderSize;
		bool									m_bModify;
	};

}

#endif //_DEFINE_COMMONDATAFILE_H_