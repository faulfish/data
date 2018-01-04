#include "StdAfx.h"
#include "ChangeLog.h"
#include <algorithm>
#include <cassert>

namespace QSYNC_LIB
{
	using namespace std;

	template <class Type>
	class ChangeValueFunc
	{
	private:
		ChangeLogStatus Factor;
	public:
		ChangeValueFunc ( const ChangeLogStatus& _Val ) : Factor ( _Val ) {
		}
		void operator ( ) ( Type& elem ) const
		{
			elem.m_ChangeLogStatus = Factor;
		}
	};

	class FindGUIDFunc
	{
	private:
		GUID m_Factor;
	public:
		FindGUIDFunc ( const GUID& Val) : m_Factor ( Val ){}
		bool operator ( ) ( ChangeLogData value ) const
		{
			return ChangeLogData::CompareGUID(value.m_Guid,m_Factor) == 0;
		}
	};

	class FindLUIDFunc
	{
	private:
		int m_Factor;
	public:
		FindLUIDFunc ( const int& Val ) : m_Factor ( Val ){}
		bool operator ( ) ( ChangeLogData value ) const
		{
			return value.m_nLUID == m_Factor;
		}
	};

	class FindStatusFunc
	{
	private:
		ChangeLogStatus m_Factor;
	public:
		FindStatusFunc ( const ChangeLogStatus& Val ) : m_Factor ( Val ){}
		bool operator ( ) ( ChangeLogData value ) const
		{
			return value.m_ChangeLogStatus == m_Factor;
		}
	};

	CChangeLog::CChangeLog(void):m_bEnable(true)
	{
	}

	CChangeLog::~CChangeLog(void)
	{
	}

	int CChangeLog::GetChangeCount()
	{
		int nCount = 0;
		for(ChangeLogArray::iterator Iter = m_Data.begin();Iter != m_Data.end();Iter++)
		{
			if(Iter->m_ChangeLogStatus != ChangeLogStatus_Empty)
				nCount++;
		}
		return nCount;
	}

	bool CChangeLog::GetChangeByIndex(int nIndex,ChangeLogData& changeData)
	{
		bool bRes = false;
		int nCount = -1;
		for(ChangeLogArray::iterator Iter = m_Data.begin();Iter != m_Data.end();Iter++)
		{
			if(Iter->m_ChangeLogStatus != ChangeLogStatus_Empty)
			{
				nCount++;
				if(nIndex == nCount)
				{
					changeData = *Iter;
					bRes = true;
					break;
				}
			}
		}
		return bRes;
	}

	bool CChangeLog::Insert(GUID m_Guid,const int& nLUID,int nAnchor)
	{
		if(m_bEnable == false) return false;

		bool bRes = true;
		ChangeLogArray::iterator Iter = find_if(m_Data.begin(),m_Data.end(),FindGUIDFunc(m_Guid));
		if(Iter == m_Data.end())
		{
			ChangeLogData data(m_Guid,nLUID,ChangeLogStatus_Insert,nAnchor);
			m_Data.push_back(data);
			m_Header.m_nCount = static_cast<int>(m_Data.size());
			sort(m_Data.begin(),m_Data.end());
		}
		else
		{
			Iter->m_ChangeLogStatus = ChangeLogStatus_Insert;
		}
		return bRes;
	}

	bool CChangeLog::Update(GUID guid)
	{
		return ChangeStatus(guid,ChangeLogStatus_Update);
	}

	bool CChangeLog::Delete(GUID guid)
	{
		return ChangeStatus(guid,ChangeLogStatus_Delete);
	}

	bool CChangeLog::EraseDelete()
	{
		bool bRes = true;		
		ChangeLogArray::iterator Iter = remove_if(m_Data.begin(),m_Data.end(),FindStatusFunc(ChangeLogStatus_Delete));
		m_Data.erase (Iter, m_Data.end( ) );
		m_Header.m_nCount = static_cast<int>(m_Data.size());
		return bRes;
	}

	bool CChangeLog::SetLUIDByGUID(const GUID& guid,const int& nLUID)
	{
		bool bRes = true;
		ChangeLogArray::iterator Iter = find_if(m_Data.begin(),m_Data.end(),FindGUIDFunc(guid));
		if(Iter != m_Data.end())
		{
			Iter->m_nLUID = nLUID;
			bRes = true;
		}
		ASSERT(bRes);
		return bRes;
	}

	void CChangeLog::Reset(int nIndex)
	{
		if(m_bEnable)
		{
			if(nIndex == -1)
				for_each (m_Data.begin(),m_Data.end(), ChangeValueFunc<ChangeLogData>(ChangeLogStatus_Empty) );
			else if(nIndex >= 0 && nIndex < static_cast<int>(m_Data.size()))
			{
				m_Data[nIndex].m_ChangeLogStatus = ChangeLogStatus_Empty;
			}
		}
	}

	void CChangeLog::EnableLog(bool bEnable)
	{
		m_bEnable = bEnable;
	}

	bool CChangeLog::Save(FILE *stream)
	{
		EraseDelete();
		size_t nRet = ::fwrite(&m_Header,sizeof(CChangeLogHeader),1,stream);
		for(int nIndex = 0;nIndex<m_Header.m_nCount;nIndex++)
			m_Data[nIndex].Save(stream);
		return true;
	}

	bool CChangeLog::Load(FILE *stream)
	{
		size_t nHadReadSize = ::fread(&m_Header,sizeof(CChangeLogHeader),1,stream);
		assert(nHadReadSize == 1);
		for(int nIndex = 0;nIndex<m_Header.m_nCount;nIndex++)
		{
			ChangeLogData data(stream);
			m_Data.push_back(data);
		}
		sort(m_Data.begin(),m_Data.end());
		return true;
	}

	bool CChangeLog::ChangeStatus(GUID guid,ChangeLogStatus status)
	{
		if(m_bEnable == false) return false;

		bool bRes = false;
		ChangeLogArray::iterator Iter = find_if(m_Data.begin(),m_Data.end(),FindGUIDFunc(guid));
		assert(Iter != m_Data.end());
		if(Iter != m_Data.end())
		{
			Iter->m_ChangeLogStatus = status;
			bRes = true;
		}
		return bRes;
	}

	bool CChangeLog::GetGUIDFromLUID(const int& nLUID,GUID& guid)
	{
		bool bRes = false;
		ChangeLogArray::iterator Iter = find_if(m_Data.begin(),m_Data.end(),FindLUIDFunc(nLUID));
		assert(Iter != m_Data.end());
		if(Iter != m_Data.end())
		{
			guid = Iter->m_Guid;
			bRes = true;
		}
		return bRes;
	}

}