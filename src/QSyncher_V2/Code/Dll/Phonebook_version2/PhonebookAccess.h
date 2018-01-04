#ifndef _PHONEBOOK_ACCESS_H_
#define _PHONEBOOK_ACCESS_H_

#include <vector>
#include "../../Components/Localization/ilocalization.h"
#include "../../Components/PIMDB/ClientDevice.h"
#pragma once
namespace QSYNC_LIB
{
	struct PhonebookData
	{
		PhonebookData()
			:m_pParent(NULL),
			m_bIsFolder(false){}
		PhonebookData(const CString& strGroup,const CString& strName,bool bIsFolder = true)
			:m_pParent(NULL),
			m_bIsFolder(bIsFolder),
			m_strGroup(strGroup),
			m_strName(strName){}
		PhonebookData(PhonebookData* pParent,const CString& strGroup,const CString& strName,bool bIsFolder = true)
			:m_pParent(pParent),
			m_bIsFolder(bIsFolder),
			m_strGroup(strGroup),
			m_strName(strName){}
		PhonebookData(PhonebookData* pParent,const CString& strGroup,const CString& strName,const CString& strPhone1,bool bIsFolder = false)
			:m_pParent(pParent),
			m_bIsFolder(bIsFolder),
			m_strGroup(strGroup),
			m_strName(strName),
			m_strPhone1(strPhone1){}
		virtual ~PhonebookData(){}

		//member
		bool			m_bIsFolder;
		CString			m_strGroup;
		CString			m_strName;
		CString			m_strPhone1;
		CString			m_strPhone2;
		CString			m_strPhone3;
		std::vector<PhonebookData*> m_vectPhonebookData;
		PhonebookData*	m_pParent;
	};

	class CPhonebookAccess
	{
	public:
		CPhonebookAccess();
		virtual ~CPhonebookAccess();

		bool Connect(const CString& strIMEI);
		bool Disconnect(const CString& strIMEI);
		bool SetPath(const CString& strPath);
		bool GetPath(CString& strPath);
		bool GetCount(__w64 int& nCount);
		bool SelectByIndex(__w64 int nIndex,PhonebookData** ppNode);
		bool InsertByIndex(__w64 int nIndex,PhonebookData* pNode);
		bool DeleteByIndex(__w64 int nIndex,PhonebookData* pNode);
		bool UpdateByIndex(__w64 int nIndex,PhonebookData* pNode);
	protected:
		void RecursiveDelNode(PhonebookData* pNode);
	private:
		PhonebookData* m_pRootNode;
		PhonebookData* m_pCurrentNode;
		QSYNC_LIB::ILocalization* m_pILocalization;
		IDevicePt					m_spDevice;	
		IPIMDBPt					m_spIPIMDB;
	};
}
#endif //_PHONEBOOK_ACCESS_H_