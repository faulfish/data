#ifndef _CHANGE_LOG_H_
#define _CHANGE_LOG_H_

#pragma once

#include <vector>
#include <string>

namespace QSYNC_LIB
{

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

	enum ChangeLogStatus
	{
		ChangeLogStatus_Empty = 0,
		ChangeLogStatus_Update = 1,
		ChangeLogStatus_Insert = 2,
		ChangeLogStatus_Delete = 3,
		ChangeLogStatus_Replace = 4
	};

	struct ChangeLogData
	{
		ChangeLogData(GUID guid,const int& nLUID,ChangeLogStatus status,int nAnchor):m_Guid(guid),m_nLUID(nLUID),m_ChangeLogStatus(status),m_nAnchor(nAnchor){}
		ChangeLogData():m_Guid(GUID_NULL),m_ChangeLogStatus(ChangeLogStatus_Empty),m_nAnchor(0),m_nLUID(-1){}
		ChangeLogData(FILE *stream)
		{
			Load(stream);
		}

		GUID				m_Guid;
		int					m_nLUID;
		ChangeLogStatus		m_ChangeLogStatus;
		int					m_nAnchor;
		
		bool operator==(const ChangeLogData& prm1) const
		{
			return CompareGUID(m_Guid,prm1.m_Guid) == 0;
		}
		bool operator<(const ChangeLogData& prm1) const
		{
			return CompareGUID(m_Guid,prm1.m_Guid) < 0;
		}
		
		static int CompareGUID(GUID guid1,GUID guid2)
		{
			WCHAR szTemp1[40] = {0};
			WCHAR szTemp2[40] = {0};
			StringFromGUID2(guid1,szTemp1,40);
			StringFromGUID2(guid2,szTemp2,40);
			return ::wcscmp(szTemp1,szTemp2);
		}

		void Save(FILE *stream)
		{
			WCHAR szTemp[40] = {0};
			StringFromGUID2(m_Guid,szTemp,40);
			size_t nRet = ::fwrite(szTemp,sizeof(WCHAR)*40,1,stream);
			nRet = ::fwrite(&m_nLUID,sizeof(int),1,stream);
			nRet = ::fwrite(&m_ChangeLogStatus,sizeof(int),1,stream);
			nRet = ::fwrite(&m_nAnchor,sizeof(int),1,stream);
		}

		void Load(FILE *stream)
		{
			/*
			typedef struct _GUID { 
			  unsigned long Data1; 
			  unsigned short Data2; 
			  unsigned short Data3; 
			  unsigned char Data4[8]; 
			} GUID; 
			*/
			//sample {4FF65A29-3B37-4C93-AAB4-77B4F834FF22}
			WCHAR szTemp[40] = {0};
			size_t nRet = ::fread(szTemp,sizeof(WCHAR)*40,1,stream);
			int nBuffer[16] = {0};
			::swscanf_s( szTemp, L"{%8x-%4x-%4x-%2x%2x-%2x%2x%2x%2x%2x%2x}", 
				&m_Guid.Data1,&m_Guid.Data2,&m_Guid.Data3,&(m_Guid.Data4[0]),&(m_Guid.Data4[1]),&(m_Guid.Data4[2]),&(m_Guid.Data4[3]),&(m_Guid.Data4[4]),&(m_Guid.Data4[5]),&(m_Guid.Data4[6]),&(m_Guid.Data4[7]));
			nRet = ::fread(&m_nLUID,sizeof(int),1,stream);
			nRet = ::fread(&m_ChangeLogStatus,sizeof(int),1,stream);
			nRet = ::fread(&m_nAnchor,sizeof(int),1,stream);
		}
	};

	class CChangeLog
	{
		struct CChangeLogHeader
		{
			CChangeLogHeader():m_nTargetAnchor(0),m_nSelfAnchor(0)
			{
				::strcpy_s(m_szChangeLogHeader,10,"ChangeLog");
			}
			char	m_szChangeLogHeader[10];
			int		m_nCount;
			int		m_nTargetAnchor;
			int		m_nSelfAnchor;
		};
	public:
		CChangeLog(void);
		virtual ~CChangeLog(void);
		bool GetChangeByIndex(int nIndex,ChangeLogData& changeData);
		int GetChangeCount();
		bool Insert(GUID guid,const int& nLUID,int nAnchor);	
		bool Update(GUID guid);
		bool Delete(GUID guid);		
		bool GetGUIDFromLUID(const int& nLUID,GUID& guid);
		bool SetLUIDByGUID(const GUID& guid,const int& nLUID);
		bool Save(FILE *stream);
		bool Load(FILE *stream);
		void Reset(int nIndex = -1);
		void EnableLog(bool bEnable);
	protected:
		bool ChangeStatus(GUID guid,ChangeLogStatus status);
		bool EraseDelete();
	private:
		typedef std::vector<ChangeLogData>	ChangeLogArray;
		ChangeLogArray						m_Data;
		CChangeLogHeader					m_Header;
		bool													m_bEnable;
	};
}

#endif //_CHANGE_LOG_H_