#include "StdAfx.h"
#include "PhonebookAccess.h"
#include "../../Components/PIMDB/PIMRecord.h"

namespace QSYNC_LIB
{
	CPhonebookAccess::CPhonebookAccess(void):m_pRootNode(NULL),m_pCurrentNode(NULL),m_spDevice(new CClientDevice<QSYNC_LIB::VCardSyncMLPolicy,CPIMRecord>)
	{
		CreateILocalization(&m_pILocalization);
		//test code
		PhonebookData* pRoot = new PhonebookData(_T(""),_T("root"));
		if(InsertByIndex(0,pRoot))
		{
			PhonebookData* pNode1 = new PhonebookData(pRoot,_T(""),m_pILocalization->GetString(_T("IDS_SIM")));
			InsertByIndex(0,pNode1);
			PhonebookData* pNode2 = new PhonebookData(pNode1,_T(""),_T("alan"),_T("1234-56789"),false);
			InsertByIndex(0,pNode2);

			pNode1 = new PhonebookData(pRoot,_T(""),m_pILocalization->GetString(_T("IDS_ME")));
			InsertByIndex(0,pNode1);
			pNode2 = new PhonebookData(pNode1,_T(""),_T("benq"),_T("5687-89123"),false);
			InsertByIndex(0,pNode2);
		}
	}

	CPhonebookAccess::~CPhonebookAccess(void)
	{
		RecursiveDelNode(m_pRootNode);
	}

	void CPhonebookAccess::RecursiveDelNode(PhonebookData* pNode)
	{
		if(pNode)
		{
			std::vector<PhonebookData*>::iterator Iter;
			Iter = pNode->m_vectPhonebookData.begin();
			while(Iter != pNode->m_vectPhonebookData.end())
			{	
				RecursiveDelNode(*Iter);
				pNode->m_vectPhonebookData.erase(Iter);
				Iter = pNode->m_vectPhonebookData.begin();
			}
			delete pNode;
			pNode = NULL;
		}
	}

	bool CPhonebookAccess::Connect(const CString& strIMEI)
	{
		bool bRes = false;
		return bRes;
	}

	bool CPhonebookAccess::Disconnect(const CString& strIMEI)
	{
		bool bRes = false;
		return bRes;
	}

	bool CPhonebookAccess::SetPath(const CString& strPath)
	{
		TRACE(strPath + _T("\n"));
		bool bRes = false;
		if(m_pRootNode)
		{
			if(strPath.GetLength() == 0)
			{
				m_pCurrentNode = m_pRootNode;
				bRes = true;
			}
			else
			{
				CString strTemp = strPath;
				PhonebookData* pNewNode = m_pRootNode;
				if(strTemp.Find(_T("\\")) >= 0)
				{
					strTemp = strTemp.Mid(strTemp.Find(_T("\\"))+1);
					while(strTemp.GetLength() > 0)
					{
						int nIndex = strTemp.Find(_T("\\"));
						CString strItem;
						if(nIndex >= 0)
						{
							strItem = strTemp.Left(nIndex);
							strTemp = strTemp.Mid(nIndex+1);
						}
						else
						{
							strItem = strTemp;
							strTemp.Empty();
						}

						std::vector<PhonebookData*>::iterator Iter;
						Iter = pNewNode->m_vectPhonebookData.begin();
						for(;Iter != pNewNode->m_vectPhonebookData.end();Iter++)
						{
							if((*Iter)->m_strName == strItem)
							{
								pNewNode = *Iter;						
								break;
							}
						}
					}
					m_pCurrentNode = pNewNode;
					bRes = true;
				}
			}
		}
		return bRes;
	}

	bool CPhonebookAccess::GetPath(CString& strPath)
	{
		bool bRes = false;
		if(m_pCurrentNode)
		{
			strPath = _T("");
			PhonebookData* pTempNode = m_pCurrentNode;
			while(pTempNode != NULL)
			{
				if(strPath.GetLength() > 0)
					strPath = pTempNode->m_strName + _T("\\") +strPath;
				else
					strPath = pTempNode->m_strName;
				pTempNode = pTempNode->m_pParent;
			}
			bRes = true;
		}
		return bRes;
	}

	bool CPhonebookAccess::GetCount(__w64 int& nCount)
	{
		nCount = 0;
		bool bRes = false;
		if(m_pCurrentNode)
		{
			nCount = m_pCurrentNode->m_vectPhonebookData.size();
			bRes = true;
		}
		return bRes;
	}

	bool CPhonebookAccess::SelectByIndex(__w64 int nIndex,PhonebookData** ppNode)
	{
		bool bRes = false;	
		std::vector<PhonebookData*>::iterator Iter;
		Iter = m_pCurrentNode->m_vectPhonebookData.begin();
		advance(Iter,nIndex);
		if(Iter != m_pCurrentNode->m_vectPhonebookData.end())
		{
			*ppNode = *Iter;
			bRes = true;
		}
		return bRes;
	}

	bool CPhonebookAccess::InsertByIndex(__w64 int nIndex,PhonebookData* pNode)
	{
		bool bRes = false;
		if(m_pRootNode == NULL)
		{
			m_pRootNode = pNode;
			m_pCurrentNode = pNode;
			bRes = true;
		}
		else
		{
			if(pNode->m_pParent)
				pNode->m_pParent->m_vectPhonebookData.push_back(pNode);
			else
				m_pCurrentNode->m_vectPhonebookData.push_back(pNode);
			bRes = true;
		}
		return bRes;
	}

	bool CPhonebookAccess::DeleteByIndex(__w64 int nIndex,PhonebookData* pNode)
	{
		bool bRes = false;
		if(m_pCurrentNode)
		{
			std::vector<PhonebookData*>::iterator Iter;
			Iter = m_pCurrentNode->m_vectPhonebookData.begin();
			advance(Iter,nIndex);
			if(Iter != m_pCurrentNode->m_vectPhonebookData.end())
			{
				delete *Iter;
				m_pCurrentNode->m_vectPhonebookData.erase(Iter);
				bRes = true;
			}
		}
		return bRes;
	}

	bool CPhonebookAccess::UpdateByIndex(__w64 int nIndex,PhonebookData* pNode)
	{
		bool bRes = false;
		PhonebookData* pTempNode = NULL;
		if(SelectByIndex(nIndex,&pTempNode))
		{
			*pNode = *pTempNode;
			bRes = true;
		}
		return bRes;
	}

}