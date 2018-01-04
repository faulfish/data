/***************************************************************************
 *
 *                               QSyncer 2.0 Project
 *
 *	             Copyright (c) 2007 Qisda Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   PhonebookDatasource.h  $
 *  $Revision:   1.38  $
 *  $Date:   Feb 20 2008 14:10:32  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Phonebook_version2/PhonebookDatasource.h.-arc  $
 * 
 *    Rev 1.38   Feb 20 2008 14:10:32   Alan Tu
 * update
 * 
 *    Rev 1.37   Feb 15 2008 18:55:56   Mei Yang
 * add SetHWnd
 * 
 *    Rev 1.36   Feb 15 2008 10:23:28   Mei Yang
 * add GetMECount
 * 
 *    Rev 1.35   Feb 13 2008 13:07:46   Mei Yang
 * add SetPhone
 * 
 *    Rev 1.34   Feb 05 2008 12:02:42   Mei Yang
 * add getFolderCount
 * 
 *    Rev 1.33   Feb 04 2008 15:37:10   Mei Yang
 * fix GetPIMDisplayName
 * 
 *    Rev 1.32   Feb 01 2008 16:39:36   Mei Yang
 * add GetAllPIMDisplayName
 * 
 *    Rev 1.31   Jan 04 2008 19:57:32   Mei Yang
 * update
 * 
 *    Rev 1.30   Dec 27 2007 10:32:44   Alan Tu
 * update
 * 
 *    Rev 1.29   Dec 17 2007 15:11:22   Mei Yang
 * change map to vector
 * 
 *    Rev 1.28   Dec 17 2007 14:00:54   Mei Yang
 * add GetCallerID
 * 
 *    Rev 1.27   Dec 16 2007 16:10:40   Alan Tu
 * update
 * 
 *    Rev 1.26   Dec 14 2007 18:50:00   Alan Tu
 * update
 * 
 *    Rev 1.25   Dec 14 2007 15:22:16   Alan Tu
 * update
 * 
 *    Rev 1.24   Dec 14 2007 11:44:08   Alan Tu
 * update
 * 
 *    Rev 1.23   Dec 14 2007 10:13:18   Alan Tu
 * update
 * 
 *    Rev 1.22   Dec 11 2007 13:52:58   Mei Yang
 * add GetPIMSpecificString
 * 
 *    Rev 1.21   Dec 10 2007 19:36:36   Mei Yang
 * add bSIM for CreateEmptyPIMObj
 * 
 *
 ***************************************************************************/
#ifndef _PHONEBOOK_DATASOURCE_H_
#define _PHONEBOOK_DATASOURCE_H_

#pragma once

#include "../../Components/UIFramework/interface/ITreeDatasourceBase.h"
#include "../../Components/PIMDB/ClientDevice.h"
#include "../../CommLayer/MobileInterface/Interface_Include/IPIMConfig.h"
#include "../../CommLayer\MobileInterface\Interface_Include\IPhonebook.h"
#include "../../Components/PIMDB/VCardCtrl.h"
#include "ProgressBar.h"

namespace QSYNC_LIB
{
const TSTRING g_strSIM = _T("SIM");
const TSTRING g_strMS = _T("MS");

	class CGUIDTreeNode : public CQTreeNode
	{
	public:
		CGUIDTreeNode(CQTreeNode *pParent,const TSTRING& strPhone,int nPBType,const GUID& guid)
			:CQTreeNode(pParent)
			,m_guid(guid)
			,m_strPhone(strPhone)
			,m_nPBType(nPBType)
		{
		}
		virtual ~CGUIDTreeNode(){}
		const GUID& GetGUID() const {return m_guid;}
		const TSTRING& GetPhone() const {return m_strPhone;}
		void SetPhone(const TSTRING& strPhone){m_strPhone = strPhone;}
		const int GetPBType() const {return m_nPBType;}
	private:
		GUID		m_guid;
		TSTRING		m_strPhone;
		int			m_nPBType;
	};

	class CPhonebookDatasource : public CITreeDatasourceBase , public INotify
	{
	public:
		CPhonebookDatasource(void);
		virtual ~CPhonebookDatasource(void);

		//interface of INotify
		virtual void Event(const TSTRING& strEvent,long nParam);

		//interface IQTreeDataSource
		virtual bool			CreateNode(CQTreeNode* pNode);
		virtual bool			DeleteNode(CQTreeNode* pNode);
		virtual bool			ExpandNode(CQTreeNode* pNode);
		virtual bool			UpdateNode(CQTreeNode* pNode);		
		virtual void			RefreshClear();

		void					Connect();
		void					Disconnect();
		bool					GetPIMByGuid(GUID guid,IPIMObjPt& ptIPIM);
		void					GetDevices(IDevicePts& devices);
		bool					Sync(IDevice* pIDevice,SyncType type);
		bool					CreateEmptyPIMObj(IPIMObjPt& ptIPIM,const TSTRING& strCallerID, bool bSIM = false);		
		bool					CreateNewPIMObj(IPIMObjPt& ptIPIM);
		bool					GetDefaultDevice(IDevicePt& Device);
		bool					GetRemoteDevices(IDevicePts& Devices);
		void					CreateFolder();
		bool					GetCallerID(vector<CPIMGroupInfo>& vectPIMGroup);
		void					GetAllPIMDisplayName(bool bSIM, vector<TSTRING>& vDisplayName);
		void					GetPIMDisplayName(bool bSIM,CGUIDTreeNode* pFolder, vector<TSTRING>& vDisplayName);
		int						GetFolderCount(CGUIDTreeNode* pFolder);
		int						GetMECount();
		void					SetHWnd(HWND hwnd){m_Hwnd = hwnd;}

	protected:		
		CQTreeNode*				CreateNodeByPIMObj(IPIMObjPt& ptIPIM);
		CQTreeNode*				CreateNodeBySelf(CQTreeNode* pParentNode,const CString& strName,const CString& strDisplay,const CString& strPhone = _T(""),int nPBType = ePhonebookAll,const GUID& guid = GUID_NULL);
		void					RecursiveCreateNode();
		void					GetMobileData(TSTRING& strApp,TSTRING& strMobileModel,TSTRING& strIMEI,TSTRING& strDisplayName);
		bool					GetPIMString(const char *szName,IPIMObjPt& ptPIM,TSTRING& strValue);
		bool					GetPIMSpecificString(TSTRING& strValue, IPIMObjPt& ptPIM, const char* cpKey, const TSTRING& strParameter/*Para1;Para2;.*/,int nPropertyCount,int nValueCount =0);
		bool					GetPIMInteger(const char *szName,IPIMObjPt& ptPIM,int& nVal);
		IPIMCONFIG*				GetInterface(int nIndex = 0);
		void					DeleteInterface(LPVOID pInterface ,int nIndex = 0);	
		IPIMDBPt				Connect(const TSTRING& strModule,const TSTRING& strID);		

	private:
		vector<CPIMGroupInfo>	m_vectPIMGroup;
		HWND					m_Hwnd;
		IPIMCONFIG*					m_pIPIMCONFIG;		
		IPIMDBPt							m_spIPIMDB;
		bool										m_bConnected;
		TSTRING							m_strApp;
		TSTRING							m_strMobileModel;
		TSTRING							m_strIMEI;
		TSTRING							m_strDisplayName;
		CQTreeNode*					m_pMENode;
		CQTreeNode*					m_pSIMNode;
		IPIMPt									m_ptPIMRecordFactory;
		ProgressBar						m_ProgressBar;
		CEvent							m_PhonebookEvent;
		bool										m_bSyncSucceed;
	};

}

#endif //_PHONEBOOK_DATASOURCE_H_
