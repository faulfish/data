/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   PhonebookDatasource.cpp  $
 *  $Revision:   1.63  $
 *  $Date:   Feb 27 2008 21:26:42  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhonebookDatasource.cpp.-arc  $
   
      Rev 1.63   Feb 27 2008 21:26:42   Jame Tsai
   Fix error
   
      Rev 1.62   Feb 20 2008 14:10:32   Alan Tu
   update
   
      Rev 1.61   Feb 18 2008 08:56:50   Alan Tu
   update title
   
      Rev 1.60   Feb 15 2008 18:56:24   Mei Yang
   add m_Hwnd for ShowMessage
   
      Rev 1.59   Feb 15 2008 17:09:16   Mei Yang
   add sync fail msg
   
      Rev 1.58   Feb 15 2008 10:23:28   Mei Yang
   add GetMECount
   
      Rev 1.57   Feb 15 2008 10:03:42   Alan Tu
   Check without SIM
   
      Rev 1.56   Feb 14 2008 17:51:52   Jame Tsai
   Hide SIM in tree when FDN on.
   
      Rev 1.55   Feb 13 2008 19:49:48   Mei Yang
   correct node DisplayName
   
      Rev 1.54   Feb 13 2008 16:50:42   Mei Yang
   fix DeleteNode bug
   
      Rev 1.53   Feb 05 2008 12:02:42   Mei Yang
   add getFolderCount
   
      Rev 1.52   Feb 04 2008 15:37:10   Mei Yang
   fix GetPIMDisplayName
   
      Rev 1.51   Feb 01 2008 16:39:36   Mei Yang
   add GetAllPIMDisplayName
   
      Rev 1.50   Jan 15 2008 13:34:58   Jame Tsai
   update
   
      Rev 1.49   Jan 10 2008 11:23:42   Eddy Guo
   update
   
      Rev 1.48   Jan 10 2008 08:32:38   Eddy Guo
   update
   
      Rev 1.47   Jan 04 2008 19:57:32   Mei Yang
   update
   
      Rev 1.46   Dec 27 2007 10:32:44   Alan Tu
   update
   
      Rev 1.45   Dec 17 2007 15:11:22   Mei Yang
   change map to vector
   
      Rev 1.44   Dec 17 2007 14:00:54   Mei Yang
   add GetCallerID
   
      Rev 1.43   Dec 16 2007 16:10:42   Alan Tu
   update
   
      Rev 1.42   Dec 15 2007 17:18:50   Alan Tu
   update
   
      Rev 1.41   Dec 15 2007 15:33:48   Alan Tu
   update
   
      Rev 1.40   Dec 14 2007 18:49:44   Alan Tu
   update
   
      Rev 1.39   Dec 14 2007 15:22:14   Alan Tu
   update
   
      Rev 1.38   Dec 14 2007 11:44:08   Alan Tu
   update
   
      Rev 1.37   Dec 14 2007 10:13:00   Alan Tu
   update
   
      Rev 1.36   Dec 11 2007 15:01:14   Alan Tu
   update  
 *
 *
 ***************************************************************************/
#include "StdAfx.h"
#include "PhonebookDatasource.h"
#include "../../Components/PIMDB/PIMRecord.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "../../CommonUtil\UnicodeUtility.h"
#include "..\..\Components\PIMDB\VCardCtrl.h"
#include "..\..\Components\OutlookAccess\OutlookDevice.h"
#include "../../Components/UIFramework/Porting/QProgressServer.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB
{

	CPhonebookDatasource::CPhonebookDatasource()
		:m_bConnected(false)
		,m_pSIMNode(NULL)
		,m_pMENode(NULL)
		,m_pIPIMCONFIG(NULL)
		,m_ptPIMRecordFactory(new PIMFactory<CPIMRecord>)
		,m_PhonebookEvent(false,true,_T("vcard"))
	{
	}

	CPhonebookDatasource::~CPhonebookDatasource()
	{
	}

	bool CPhonebookDatasource::CreateEmptyPIMObj(IPIMObjPt& ptIPIM,const TSTRING& strCallerID, bool bSIM)
	{
		bool bRes = false;
		ptIPIM = m_ptPIMRecordFactory->CreatePIMObject();
		CVCardCtrl VCardCtrl(ptIPIM);
		if(!bSIM)
			bRes = VCardCtrl.SetEmptyIPIMObjPt();
		else
			bRes = VCardCtrl.SetEmptySIMIPIMObjPt();
		if(!strCallerID.empty())
			VCardCtrl.SetSpecificString(strCallerID,"X-BENQ",_T("CallerID"));
		return bRes;	
	}

	//interface IQTreeDataSource
	bool CPhonebookDatasource::CreateNode(CQTreeNode* pNode)
	{
		if(pNode == NULL)
		{
			pNode = new CQTreeNode(NULL);
			SetRootNode(pNode);
			CString strRoot = _T("IDS_PHONEBOOK_ROOT");
			GetRoot()->SetDisplayName(strRoot);
			Fire(EVENT_TREE_DATA_INSERT_NODE, (long)pNode);
		}
		else 
		{
			if(GetRoot() == NULL && pNode->GetParent() == NULL) //Root
				SetRootNode(pNode);
		}

		CGUIDTreeNode* pGuidNode = static_cast<CGUIDTreeNode*>(pNode);
		if(pGuidNode->GetGUID() == GUID_NULL) 
		{
			Fire(EVENT_TREE_DATA_INSERT_NODE, (long)pNode);
			ExpandNode(const_cast<QSYNC_LIB::CQTreeNode *>(pNode->GetParent()));
		}

		return true;
	}

	CQTreeNode* CPhonebookDatasource::CreateNodeBySelf(CQTreeNode* pParentNode,const CString& strName,const CString& strDisplay,const CString& strPhone,int nPBType,const GUID& guid)
	{
		CGUIDTreeNode* pNewNode = new CGUIDTreeNode(pParentNode,(LPCTSTR)strPhone,nPBType,guid);
		pNewNode->SetDisplayName(LPCTSTR(strDisplay));
		pNewNode->SetName(LPCTSTR(strName));
		CreateNode(static_cast<CQTreeNode*>(pNewNode));
		return pNewNode;
	}

	IPIMCONFIG* CPhonebookDatasource::GetInterface(int nIndex)
	{
		if(m_pIPIMCONFIG == NULL)
		{
			m_pIPIMCONFIG = (IPIMCONFIG*)CommCreateInterface(_T("QSyncer"),_T("Phonebook"),eIPIMConfigure,nIndex);
		}
		return m_pIPIMCONFIG;
	}

	void CPhonebookDatasource::DeleteInterface(LPVOID pInterface ,int nIndex)
	{
		CommDeleteInterface(pInterface,nIndex);
	}	

	IPIMDBPt CPhonebookDatasource::Connect(const TSTRING& strModule,const TSTRING& strID)
	{
		IPIMPt ptIPIM = new PIMFactory<CPIMRecord>;		
		TSTRING strNormalizeDatabaseName = _T("ID:") + strID + _T(",MODULE:") + strModule + _T(",TYPE:vcard");
		IPIMDBPt spIPIMDB;
		CreatePIMDB(strNormalizeDatabaseName,ptIPIM,spIPIMDB);
		if(spIPIMDB)
		{
			IDevicePt ptNewDevice = new OutlookDevice(KEYWORD_CONTACT);
			spIPIMDB->AddRemoteDevice(ptNewDevice,spIPIMDB);
		}
		return spIPIMDB;
	}

	void CPhonebookDatasource::RecursiveCreateNode()
	{		
		m_pIPIMCONFIG = GetInterface();
		if(m_pIPIMCONFIG)
		{
			if(m_pIPIMCONFIG->ListPIMGroup(eContact,&m_vectPIMGroup) > 0)
			{
				bool bFDNon = false;
				CommGetFDNStatus(bFDNon);
				if(bFDNon == false)
					m_pSIMNode = CreateNodeBySelf(GetRoot(),g_strSIM.c_str(),_T("IDS_PHONEBOOK_SIM"));
				m_pMENode = CreateNodeBySelf(GetRoot(),g_strMS.c_str(),_T("IDS_PHONEBOOK_MS"));

				for(vector<CPIMGroupInfo>::iterator Iter = m_vectPIMGroup.begin();Iter != m_vectPIMGroup.end();Iter++)
				{
					CString strFolderID;
					strFolderID.Format(_T("%d"),Iter->m_nID);

					TSTRING strDisplay;
					QPDecoder(Iter->m_strDisplayText, strDisplay, CP_UTF8);
					CreateNodeBySelf(m_pMENode,strFolderID,strDisplay.c_str());
				}
			}
			else
			{
				//Please check without SIM case
			}

			DeleteInterface(m_pIPIMCONFIG);
		}	
	}

	bool CPhonebookDatasource::GetCallerID(vector<CPIMGroupInfo>& vectPIMGroup)
	{
		bool bRes = true;
		vectPIMGroup = m_vectPIMGroup;
		return bRes;
	}

	bool CPhonebookDatasource::Sync(IDevice* pIDevice,SyncType type)
	{		
		bool bRes = false;
		if(m_spIPIMDB)
		{
			m_spIPIMDB->Register(this,g_strNotifyDBFinish);
			m_spIPIMDB->Register(this,g_strNotifyDBInsert);
			m_spIPIMDB->Register(this,g_strNotifyDBRemove);

			if(pIDevice->CanSync())
			{
				pIDevice->Sync(type);
				m_ProgressBar.m_RunningEvent.SetEvent();
				m_bSyncSucceed = false;
				CQProgressServer::Exec( &m_ProgressBar );  //Show progressing bar				
				if(m_bSyncSucceed)
					ShowMessage(m_Hwnd,GetString(_T("IDS_Sync_Finish")), GetString(_T("IDS_PHONEBOOK_FUNCTION")));
				else
					ShowMessage(m_Hwnd,GetString(_T("IDS_Sync_Finish_Fail")), GetString(_T("IDS_PHONEBOOK_FUNCTION")));
				bRes = true;
			}
		}
		return bRes;
	}

	void CPhonebookDatasource::CreateFolder()
	{
		CreateNode(NULL);
		RecursiveCreateNode();
	}

	bool CPhonebookDatasource::DeleteNode(CQTreeNode* pNode)
	{
		bool bRes = false;
		CGUIDTreeNode* pGuidNode = static_cast<CGUIDTreeNode*>(pNode);
		if(pNode) {
			if(!pNode->IsRoot())
			{
				IPIMObjPt ptPIM;
				if(m_spIPIMDB->BeginTransaction(pGuidNode->GetGUID(),ptPIM))
				{
					if(m_spIPIMDB->RemoveItemByGUID(pGuidNode->GetGUID()))
					{
						//Fire(EVENT_TREE_DATA_DELETE_NODE, (long)pNode);
						delete pNode;
						bRes =  true;
					}
					m_spIPIMDB->EndTransaction();
				}
			}
			else
			{
				Fire(EVENT_TREE_DATA_DELETE_NODE, (long)pNode);
				delete pNode;
			}
		}
		return bRes;
	}

	bool CPhonebookDatasource::ExpandNode(CQTreeNode* pNode)
	{
		bool bRes = true;
		Fire(EVENT_TREE_DATA_EXPAND_NODE, (long)pNode);
		return bRes;
	}

	bool CPhonebookDatasource::UpdateNode(CQTreeNode* pNode)
	{
		bool bRes = false;
		IPIMObjPt ptPIM;
		CGUIDTreeNode* pGuidNode = static_cast<CGUIDTreeNode*>(pNode);
		if(m_spIPIMDB->BeginTransaction(pGuidNode->GetGUID(),ptPIM))
		{
			bRes = m_spIPIMDB->UpdateItem(ptPIM);
			m_spIPIMDB->EndTransaction();
		}
		return bRes;
	}

	void CPhonebookDatasource::Connect()
	{
		if(m_bConnected == false)
		{
            RefreshClear();
			m_bConnected = true;			
			GetMobileData(m_strApp,m_strMobileModel,m_strIMEI,m_strDisplayName);
			m_spIPIMDB = Connect(m_strMobileModel,m_strIMEI);	
		}
	}

	void CPhonebookDatasource::Disconnect()
	{
		if(m_bConnected == true)
		{
			m_bConnected = false;
            RemoveSyncManagerDevice();
            m_spIPIMDB = NULL;
            m_pIPIMCONFIG = NULL;
		}
	}

	void CPhonebookDatasource::RefreshClear()
	{
		m_vectPIMGroup.clear();
		if(m_spIPIMDB)
			m_spIPIMDB->RemoveAll(true);
		CITreeDatasourceBase::RefreshClear();
	}

	bool CPhonebookDatasource::CreateNewPIMObj(IPIMObjPt& ptIPIM)
	{
		bool bRes = false;
		if(m_spIPIMDB->BeginTransaction())
		{
			bRes = m_spIPIMDB->InsertItem(ptIPIM);
			m_spIPIMDB->EndTransaction();
		}
		return bRes;
	}

	CQTreeNode* CPhonebookDatasource::CreateNodeByPIMObj(IPIMObjPt& ptPIM)
	{
		CQTreeNode* pNewNode = NULL;
		/*
		42 45 47 49 4E 3A 56 43 41 52 44 0D 0A 4E 3B    BEGIN:VCARD..N;
		43 48 41 52 53 45 54 3D 55 54 46 2D 38 3B 45 4E   CHARSET=UTF-8;EN
		43 4F 44 49 4E 47 3D 51 55 4F 54 45 44 2D 50 52   CODING=QUOTED-PR
		49 4E 54 41 42 4C 45 3A 41 42 43 3B 3B 3B 3B 0D   INTABLE:ABC;;;;.
		0A 54 45 4C 3B 48 4F 4D 45 3B 56 4F 49 43 45 3A   .TEL;HOME;VOICE:
		31 32 33 0D 0A 54 45 4C 3B 57 4F 52 4B 3B 56 4F   123..TEL;WORK;VO
		49 43 45 3A 34 35 36 0D 0A 54 45 4C 3B 48 4F 4D   ICE:456..TEL;HOM
		45 3B 43 45 4C 4C 3A 37 38 39 0D 0A 54 45 4C 3B   E;CELL:789..TEL;
		57 4F 52 4B 3B 43 45 4C 4C 3A 39 36 33 32 31 34   WORK;CELL:963214
		35 38 37 0D 0A 58 2D 42 45 4E 51 3B 43 61 6C 6C   587..X-BENQ;Call
		65 72 49 44 3A 36 35 35 33 39 0D 0A 56 45 52 53   erID:65539..VERS
		49 4F 4E 3A 32 2E 31 0D 0A 45 4E 44 3A 56 43 41   ION:2.1..END:VCA
		52 44 0D 0A                                       RD..
		*/
		TSTRING strName1,strName2,strName3;
		TSTRING strTel,strCallerID,strType;
		GetPIMString("VCARD:N:0",ptPIM,strName1);
		//QPDecoder(strName1.c_str(), strName1, CP_UTF8);
		GetPIMString("VCARD:N:1",ptPIM,strName2);
		//QPDecoder(strName2.c_str(), strName2, CP_UTF8);
		//GetPIMString("VCARD:TEL",ptPIM,strTel);
		GetPIMSpecificString(strTel, ptPIM,"TEL",_T("HOME;VOICE"),0);
		GetPIMString("VCARD:X-BENQ",ptPIM,strCallerID);
		GetPIMString("VCARD:X-BENQ:NULL",ptPIM,strType);		

		int nPBType = ePhonebookAll;
		GetPIMInteger("PBTYPE",ptPIM,nPBType);

		CQTreeNode* pParentNode = NULL;

		int nType = 999;
		if(GetPIMInteger("PBTYPE",ptPIM,nType))
		{
			if(nType == 0)//ePhonebookSIM
				pParentNode = m_pSIMNode;
			else if(nType == 1)//ePhonebookME
				pParentNode = m_pMENode;
		}
		else
		{//Set default PBTYpe = ePhonebookME
			CommonDataPt ptType = new CommonData("PBTYPE");
			ptType->SetInteger(ePhonebookME);
			ptPIM->SetByName(ptType->GetName(),ptType);
			pParentNode = m_pMENode;
		}

		if(strType == _T("CallerID") && !strCallerID.empty())
		{
			for(int i=0;i<m_pMENode->GetChildSize();i++)
			{
				CQTreeNode* pNode = m_pMENode->GetChild(i);
				if(pNode)
				{
					if(pNode->GetName() == strCallerID.c_str())
					{
						pParentNode = pNode;
						break;
					}
				}
			}
		}

		GUID guid = ptPIM->GetGUID();
		TSTRING strFullName = strName1 + _T(" ") + strName2;
		CVCardCtrl VCardCtrl(ptPIM);
		TSTRING strDisplay = VCardCtrl.GetOfficialDisplayName();

		pNewNode = CreateNodeBySelf(pParentNode,strFullName.c_str(),strDisplay.c_str(),strTel.c_str(),nPBType,guid);
		return pNewNode;
	}
	
	//interface INotify
	void CPhonebookDatasource::Event(const TSTRING& strEvent,long nParam)
	{
		if(g_strNotifyDBFinish == strEvent)
		{
			m_ProgressBar.m_RunningEvent.ResetEvent();
			m_bSyncSucceed = nParam == 1;
		}
		else if(g_strNotifyDBInsert == strEvent)
		{
			GUID* pGuid = (GUID*)nParam;
			IPIMObjPt ptPIM;
			if(m_spIPIMDB->BeginTransaction(*pGuid,ptPIM))
			{
				CreateNodeByPIMObj(ptPIM);
				m_spIPIMDB->EndTransaction();
			}				
		}
	}

	bool CPhonebookDatasource::GetPIMString(const char *szName,IPIMObjPt& ptPIM,TSTRING& strValue)
	{
		bool bRes = false;
		CommonDataPt spData;
		size_t nNotFoundIndex = -1;
		if(ptPIM->GetByName(szName,spData) != nNotFoundIndex && spData)
		{
#ifdef _UNICODE
			bRes = spData->GetType() == WString;
			if(bRes)
				strValue = spData->GetWString();
#else
			bRes = spData->GetType() == String;
			if(bRes)
				strValue = spData->GetString();
#endif
		}
		return bRes;
	}

	bool CPhonebookDatasource::GetPIMSpecificString(TSTRING& strValue, IPIMObjPt& ptPIM, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount)
	{
		bool bRes = true;
		CVCardCtrl VCardCtrl(ptPIM);
		strValue = VCardCtrl.GetSpecificString(cpKey, strParameter, nPropertyCount, nValueCount);
		return bRes;
	}

	bool CPhonebookDatasource::GetPIMInteger(const char *szName,IPIMObjPt& ptPIM,int& nVal)
	{
		bool bRes = false;
		CommonDataPt spData;
		size_t nNotFoundIndex = -1;
		if(ptPIM->GetByName(szName,spData) != nNotFoundIndex && spData)
		{
			bRes = spData->GetType() == Integer;
			if(bRes)
				nVal = spData->GetInteger();
		}
		return bRes;
	}

	void CPhonebookDatasource::GetMobileData(TSTRING& strApp,TSTRING& strMobileModel,TSTRING& strIMEI,TSTRING& strDisplayName)
	{
		LPTSTR	szApp = new TCHAR[MAX_RET_STRING_LENGTH];
		LPTSTR	szMobileModel = new TCHAR[MAX_RET_STRING_LENGTH];
		LPTSTR	szIMEI = new TCHAR[MAX_RET_STRING_LENGTH];
		LPTSTR	szDisplayName = new TCHAR[MAX_RET_STRING_LENGTH];

		::ZeroMemory(szApp, MAX_RET_STRING_LENGTH);
		::ZeroMemory(szMobileModel, MAX_RET_STRING_LENGTH);
		::ZeroMemory(szIMEI, MAX_RET_STRING_LENGTH);
		::ZeroMemory(szDisplayName, MAX_RET_STRING_LENGTH);		
				
		CommGetMobileInfo(szApp, szMobileModel, szIMEI, szDisplayName);
		strApp = szApp;
		strMobileModel = szMobileModel;
		strIMEI = szIMEI;
		strDisplayName = szDisplayName;

		delete [] szApp;
		delete [] szMobileModel;
		delete [] szIMEI;
		delete [] szDisplayName;
	}

	bool	 CPhonebookDatasource::GetPIMByGuid(GUID guid,IPIMObjPt& ptIPIM)
	{
		bool bRes = false;
		if(m_spIPIMDB)
		{
			if(m_spIPIMDB->BeginTransaction(guid,ptIPIM))
			{
				m_spIPIMDB->EndTransaction();
				bRes = true;
			}
		}
		return bRes;
	}

	void CPhonebookDatasource::GetDevices(IDevicePts& Devices)
	{
		if(m_spIPIMDB)
		{
				IDevicePt ptIDevice;
				m_spIPIMDB->GetDefaultDevice(ptIDevice);
				Devices.push_back(ptIDevice);
		}
	}

	bool CPhonebookDatasource::GetDefaultDevice(IDevicePt& Device)
	{
		if(m_spIPIMDB)
			m_spIPIMDB->GetDefaultDevice(Device);
		return true;
	}

	bool	CPhonebookDatasource::GetRemoteDevices(IDevicePts& Devices)
	{
		if(m_spIPIMDB)
			m_spIPIMDB->EnumRemoteDevice(Devices);
		return true;
	}

	void CPhonebookDatasource::GetAllPIMDisplayName(bool bSIM, vector<TSTRING>& vDisplayName)
	{
		vDisplayName.clear();
		CQTreeNode* pTreeNode = bSIM ? m_pSIMNode : m_pMENode;

		GetPIMDisplayName(bSIM, static_cast<CGUIDTreeNode*>(pTreeNode), vDisplayName);
	}

	void CPhonebookDatasource::GetPIMDisplayName(bool bSIM, CGUIDTreeNode* pFolder, vector<TSTRING>& vDisplayName)
	{
		int nChildSize = pFolder->GetChildSize();
		for(int i=0; i< nChildSize; ++i)
		{
			CGUIDTreeNode* pGuidNode = static_cast<CGUIDTreeNode*>(pFolder->GetChild(i));
			GUID aGUID = pGuidNode->GetGUID();
			IPIMObjPt PIMObjPt;
			if(aGUID != GUID_NULL && GetPIMByGuid(aGUID, PIMObjPt))
			{					
				CVCardCtrl VCardCtrl(PIMObjPt);
				TSTRING strDisplayName, strPhone;
				if(bSIM)
					VCardCtrl.GetCmnStrData(KEYWORD_FIRSTNAME, strDisplayName);
				else 
					VCardCtrl.GetCmnStrData(KEYWORD_DISPLAY, strDisplayName);

				strPhone = VCardCtrl.GetSpecificString("TEL",_T("HOME;VOICE"),0);
				if(!strDisplayName.empty())
					vDisplayName.push_back(strDisplayName);
				else if(strDisplayName.empty() && !strPhone.empty())
					vDisplayName.push_back(strPhone);
			}
			else
			{
				GetPIMDisplayName(bSIM, pGuidNode, vDisplayName);
			}
		}
	}
	int	CPhonebookDatasource::GetFolderCount(CGUIDTreeNode* pFolder)
	{
		int nFolderCount = 0;
		if(pFolder->GetGUID() == GUID_NULL)
		{
			int nChildSize = pFolder->GetChildSize();
			for(int i=0; i< nChildSize; i++)
			{
				CGUIDTreeNode* pChildNode = static_cast<CGUIDTreeNode*>(pFolder->GetChild(i));
				if(pChildNode->GetGUID() != GUID_NULL)
					++nFolderCount;
				else
					nFolderCount += GetFolderCount(pChildNode);
			}
		}
		return nFolderCount;
	}
	int	CPhonebookDatasource::GetMECount()
	{
		int nCount = 0;
		int nChildSize = m_pMENode->GetChildSize();
		for(int i=0; i< nChildSize; i++)
		{
			CGUIDTreeNode* pChildNode = static_cast<CGUIDTreeNode*>(m_pMENode->GetChild(i));
			if(pChildNode->GetGUID() != GUID_NULL)
				++nCount;
			else
				nCount += GetFolderCount(pChildNode);
		}
		return nCount;
	}
}

#pragma warning(pop)