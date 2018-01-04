///////////////////////////////////////////////////////////
//  CQMEFileListDataSource.cpp
//  Implementation of the Class CQMEFileListDataSource
//  Created on:      08-Feb-2007 08:15:23
//  Original author: YC Liang
///////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CQMEFileListDataSource.h"
#include "../../Components/UIFramework/interface/QControlNotify.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB {
	//-------------------------------------------------------------

	CQMEFileListDataSource::CQMEFileListDataSource()
	{
	}

	CQMEFileListDataSource::~CQMEFileListDataSource() 
	{
		Clear();
	}

	class SortFunc
	{
	private:
		TSTRING m_Factor;
		bool	m_bDsc;
	public:
		SortFunc ( const TSTRING& _Val,bool	bDsc ) : m_Factor ( _Val ),m_bDsc( bDsc ) {
		}

		bool operator () ( CQListData* elem1, CQListData* elem2 ) const
		{
			bool bRes;
			if(m_Factor == strFILE_SIZE_LISTKEY)
			{
				int nVal1 = _ttoi(elem1->GetValue(m_Factor)->m_strText.c_str());
				int nVal2 = _ttoi(elem2->GetValue(m_Factor)->m_strText.c_str());
				bRes = m_bDsc ? nVal1 > nVal2 : nVal1 < nVal2;
			}
			else if(m_Factor == strFILE_MDATE_LISTKEY)
			{
				CMEFileNode* pVar1 = (CMEFileNode*)elem1->GetLPARAM();
				CMEFileNode* pVar2 = (CMEFileNode*)elem2->GetLPARAM();
				bRes = m_bDsc ? pVar1->GetFileTime() > pVar2->GetFileTime() : pVar1->GetFileTime() < pVar2->GetFileTime();
			}
			else
			{
				if(m_bDsc)
					bRes = elem1->GetValue(m_Factor)->m_strText > elem2->GetValue(m_Factor)->m_strText;
				else
					bRes = elem1->GetValue(m_Factor)->m_strText < elem2->GetValue(m_Factor)->m_strText;
			}
			return bRes;
		}
	};

	////////////////
	bool CQMEFileListDataSource::SortByColumn(const CString& strKey)
	{
		CSingleLock singleLock(&m_CritSection,TRUE);

		m_strCurrentOrder = strKey;		
		sort( m_ListDatas.begin(), m_ListDatas.end(), SortFunc(LPCTSTR(strKey),m_SortType[m_strCurrentOrder]) );
		m_SortType[m_strCurrentOrder] = !m_SortType[m_strCurrentOrder];
		Fire(EVENT_LIST_COLUMN_SORTOK, (long)&m_ListDatas);
		return true;
	}

	bool CQMEFileListDataSource::GetOrderInfo(CString& strKey, bool& bOrder)
	{
		strKey = m_strCurrentOrder.c_str();
		bOrder = m_SortType[m_strCurrentOrder];
		return true;
	}

	void CQMEFileListDataSource::Event(const TSTRING& strEvent,long nParam)
	{
		ASSERT(0);
	}

	bool CQMEFileListDataSource::DisplayFileNodeInfo(CMEFileNode* pNode)
	{
		bool bRet = false;
		if(pNode && !(pNode->IsFolder()))
		{
			CQListData* pFileListRowData = new CQListData;
			if(pFileListRowData)
			{
				pFileListRowData->SetLPARAM((long)pNode);

				CBasicData NameData(pNode->GetName());
				NameData.m_Alignment = CBasicData::Alignment_Left;
				bRet = pFileListRowData->SetData(strFILE_NAME_LISTKEY, NameData);

				CBasicData FileAttrData(pNode->GetFileAttrInStr());
				FileAttrData.m_Alignment = CBasicData::Alignment_Left;
				bRet = pFileListRowData->SetData(strFILE_ATTR_LISTKEY, FileAttrData);

				CBasicData FilesizeData(pNode->GetFileSizeInStr());
				FilesizeData.m_Alignment = CBasicData::Alignment_Left;
				bRet = pFileListRowData->SetData(strFILE_SIZE_LISTKEY, FilesizeData);

				CBasicData FileTimeData(pNode->GetFileTimeInStr());
				FileTimeData.m_Alignment = CBasicData::Alignment_Left;
				bRet = pFileListRowData->SetData(strFILE_MDATE_LISTKEY, FileTimeData);

				CBasicData FullpathData(pNode->GetFullPath());
				FullpathData.m_Alignment = CBasicData::Alignment_Left;
				bRet = pFileListRowData->SetData(strFILE_FULL_LISTKEY, FullpathData);
				bRet = InsertDataByIndex( pFileListRowData);
			}
		}
		return bRet;
	}

	bool CQMEFileListDataSource::Clear(void)
	{
		RefreshClear();
		return true;
	}

	//-------------------------------------------------------------
} //namespace end


#pragma warning(pop)