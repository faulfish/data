/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   PhonebookView.cpp  $
*  $Revision:   1.126  $
*  $Date:   Mar 28 2008 10:57:50  $
*
***************************************************************************
*
* File Description
* ----------------
*
*
***************************************************************************

***************************************************************************/
#pragma once
#include "stdafx.h"
#include "resource.h"
#include "PhonebookView.h"
#include "PhonebookTreeDlg.h"
#include "ShowPhonebook.h"
#include "..\..\CommonUtil\QSyncerFrameworks.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "../../Components/UIFramework/LayoutManager/Win32Widge.h"
#include "../../Components/UIFramework/interface/ShowMessage.h"
#include "..\..\CommonUtil\Win32SHELLUtility.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32SplitLayout.h"
#include "..\..\Components\PIMDB\SyncManager.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

// CPhonebookView
extern class NotifyReceiver g_NotifyReceiver;

namespace QSYNC_LIB
{
	CPhonebookView* g_pThis = NULL;

	IMPLEMENT_DYNCREATE(CPhonebookView, Win32DoubleBufferFormViewWidge)
		CPhonebookView::CPhonebookView()
		: Win32DoubleBufferFormViewWidge(CPhonebookView::IDD)
		,m_bIsConnect(false)
		,m_bIsShown(false)
		,m_TreeLogic(&m_TreeCtrl,&m_TreeDatasource)
		,m_ListLogical(&m_ListDatasource,&m_ListCtrl)
		,m_pInsertButton(NULL)
		,m_pEditButton(NULL)
		,m_pDeleteButton(NULL)
		,m_pRefreshFromDefaultButton(NULL)
		,m_pSyncFromOutlookButton(NULL)
		,m_pSyncToOutlookButton(NULL)
		,m_pTwoWaySyncButton(NULL)
		,m_pCurrentNode(NULL)
		,m_IConfigure(QSYNC_LIB::CreateIConfigure(DEF_Configure_Name))
		,m_strUserTargetDevice(m_IConfigure,_T("UseTargetDevice"))
		,m_strUserSourceDevice(m_IConfigure,_T("UseSourceDevice"))
		,m_SyncProcess(&m_TreeDatasource)
		,m_bDragging(false)
		,m_bDragEnd(false)
		,m_bEffectMove(false)
	{
		g_pThis = this;
		g_NotifyReceiver.m_pINotify = this;
		CreateILocalization(&m_pILocale);

		if(m_IConfigure)
			m_IConfigure->Load(QSYNC_LIB::STORAGE_INIFILE);
	}

	CPhonebookView::~CPhonebookView()
	{
		m_pILocale->Unregister(this, TNotify_Language_Change);

		if(m_IConfigure)
			m_IConfigure->Save();

		DeleteIConfigure(DEF_Configure_Name);
	}

	BEGIN_MESSAGE_MAP(CPhonebookView, CFormView)
		ON_WM_DESTROY()
		ON_CBN_SELCHANGE(IDC_COMBO_TARGET_DEVICE, &CPhonebookView::OnCbnSelchangeComboTargetDevice)
		ON_CBN_SELCHANGE(IDC_COMBO_SOURCE_DEVICE, &CPhonebookView::OnCbnSelchangeComboSourceDevice)
	END_MESSAGE_MAP()

	void CPhonebookView::DoDataExchange(CDataExchange* pDX)
	{
		CFormView::DoDataExchange(pDX);
		DDX_Control(pDX, IDC_GROUPTREE, m_TreeCtrl);
		DDX_Control(pDX, IDC_CONTACTLIST, m_ListCtrl);
	}
	
	void CPhonebookView::EnableConnectionRelatedButton()
	{			
		if(m_pRefreshFromDefaultButton)
			m_pRefreshFromDefaultButton->SetEnable(m_bIsConnect);		
		if(m_pSyncToOutlookButton)
			m_pSyncToOutlookButton->SetEnable(m_bIsConnect);		
		if(m_pSyncFromOutlookButton)
			m_pSyncFromOutlookButton->SetEnable(m_bIsConnect);
		if(m_bIsConnect == false)
		{
			if(m_pInsertButton)
				m_pInsertButton->SetEnable(false);
			if(m_pEditButton)
				m_pEditButton->SetEnable(false);
			if(m_pDeleteButton)
				m_pDeleteButton->SetEnable(false);			
		}
	}

	void CPhonebookView::OnInitialUpdate()
	{
		CFormView::OnInitialUpdate();
		m_pILocale->Register(this, TNotify_Language_Change);

		m_TreeCtrl.SetDropable(true);
		m_TreeDatasource.SetHWnd(this->GetSafeHwnd());

		SetXMLUI();

		EnableConnectionRelatedButton();

		m_TreeLogic.Register(this,EVENT_TREE_UI_DRAG_BEGIN);
		m_TreeLogic.Register(this,EVENT_TREE_SELECT_NODE);
		m_TreeLogic.Register(this, EVENT_TREE_RBUTTONDOWN_NODE);
		m_TreeLogic.Register(this, EVENT_TREE_UI_DROP);
		m_TreeLogic.Register(this, EVENT_DRAG_ENTER);		
		m_TreeLogic.Register(this, EVENT_DRAG_OVER);		
		m_TreeLogic.Register(this, EVENT_DRAG_LEAVE);		
		
		m_ListLogical.Register(this, EVENT_LIST_UI_LDBCLICK);
		m_ListLogical.Register(this, EVENT_LIST_UI_ITEMCHANGED);	
		m_ListLogical.Register(this, EVENT_LIST_UI_DRAG_ENTER);
		m_ListLogical.Register(this, EVENT_LIST_UI_DRAG_OVER);
		m_ListLogical.Register(this, EVENT_LIST_UI_DRAG_LEAVE);
		m_ListLogical.Register(this, EVENT_LIST_UI_HEADER_BUTTON_HIT);
		m_ListLogical.Register(this, EVENT_LIST_UI_DRAG_BEGIN);
		m_ListLogical.Register(this, EVENT_LIST_UI_DROP);
		m_ListLogical.Register(this, EVENT_DRAG_END);
		m_ListLogical.Register(this, EVENT_LIST_UI_DELETE_ROW);		

		m_ListDatasource.InsertColumnKeyAndName(str_STORAGE_LISTKEY, _T(""),20);
		m_ListDatasource.InsertColumnKeyAndName(str_NAME_LISTKEY, str_NAME_LISTKEY,150);
		m_ListDatasource.InsertColumnKeyAndName(str_PHONE_LISTKEY, str_PHONE_LISTKEY,150);
	}

	// CPhonebookView diagnostics

#ifdef _DEBUG
	void CPhonebookView::AssertValid() const
	{
		CFormView::AssertValid();
	}

#ifndef _WIN32_WCE
	void CPhonebookView::Dump(CDumpContext& dc) const
	{
		CFormView::Dump(dc);
	}
#endif
#endif //_DEBUG


	ePhonebookType CPhonebookView::GetPBTypeByNode(CQTreeNode* pNode)
	{
		ePhonebookType eType = ePhonebookME;

		if(pNode == NULL)
			pNode = m_TreeCtrl.GetCurrentNode();

		if(pNode)
		{
			TSTRING strName = pNode->GetName();
			if(strName == g_strSIM)
				eType = ePhonebookSIM;
			else if(strName == g_strMS)
				eType = ePhonebookME;
			else
				eType = ePhonebookME;
		}
		return eType;
	}

	void CPhonebookView::OnBnClickedPhonebookBtnNew()
	{
		ePhonebookType eType = GetPBTypeByNode();
		TSTRING strCallerID = m_TreeCtrl.GetCurrentNode()->GetName();

		vector<CPIMGroupInfo> vectPIMGroup;
		m_TreeDatasource.GetCallerID(vectPIMGroup);

		IPIMObjPt PIMObjPt;
		if(m_TreeDatasource.CreateEmptyPIMObj(PIMObjPt,strCallerID, eType == ePhonebookSIM))
		{
			CQTreeNode* pCurrentNode = m_TreeCtrl.GetCurrentNode();
			if(pCurrentNode)
			{
				//Prepare for Type
				CommonDataPt ptType = new CommonData("PBTYPE");
				ptType->SetInteger(eType);
				PIMObjPt->SetByName(ptType->GetName(),ptType);
			}

			vector<TSTRING> vDisplayString;
			m_TreeDatasource.GetAllPIMDisplayName(eType == ePhonebookSIM, vDisplayString);
			bool bRes = false;
			if(eType == ePhonebookME && !IsOverMSLimit() && !IsOverGroupLimit(pCurrentNode) &&  m_ContactEditor.DoModal(PIMObjPt,vectPIMGroup, vDisplayString) == IDOK)
				bRes = true;
			else if(eType == ePhonebookSIM && m_SIMContactEditor.DoModal(PIMObjPt, vDisplayString) == IDOK)
				bRes = true;

			if(bRes)
			{
				if(m_TreeDatasource.CreateNewPIMObj(PIMObjPt))
					DisplayNodeInfoToList(pCurrentNode,true);
				else
				{
					CommonDataPt spVCardErrData;
					PIMObjPt->GetByName("VCARDERR",spVCardErrData);

					//手機記憶體已滿時會回傳VCardErr 7,但這個值沒有寫在文件裡,所以先不使用enum而直接判斷7
					if(spVCardErrData->GetInteger()==7)
						ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_MEMemoryFull"),_T("IDS_PHONEBOOK_FUNCTION"));

					//SIM卡已滿跟上傳檔案格式錯誤都會回傳VCardErr 8,這個值也沒有寫在文件裡,一樣先不使用enum而直接判斷8
					else if(spVCardErrData->GetInteger()==8)
						ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_SIMFull"),_T("IDS_PHONEBOOK_FUNCTION"));
					else
						ShowMessage(GetSafeHwnd(),_T("IDS_InsertFail"),_T("IDS_PHONEBOOK_FUNCTION"));
				}
			}
		}
	}

	bool CPhonebookView::UpdateDefaultDevice(CQTreeNode* pNode,IPIMObjPt& PIMObjPt,CQListData* pListData,int nListIndex)
	{
		bool bRes = false;
		if(pNode && m_TreeDatasource.UpdateNode(pNode) )
		{	
			TSTRING strDisplayName = GetPIMObjDisplayName(PIMObjPt);
			if(strDisplayName == _T(""))
			{
				CVCardCtrl VCardCtrl(PIMObjPt);
				strDisplayName = VCardCtrl.GetOfficialDisplayName();
			}
				
			pNode->SetDisplayName(strDisplayName.c_str());
			(static_cast<CGUIDTreeNode*>(pNode))->SetPhone(GetPIMObjPhone(PIMObjPt));

			if(pListData)
			{
				pListData->SetData(str_NAME_LISTKEY,CBasicData(strDisplayName));
				pListData->SetData(str_PHONE_LISTKEY,CBasicData(GetPIMObjPhone(PIMObjPt)));
				bRes = m_ListDatasource.UpdateDataByIndex(pListData,nListIndex);
			}
		}
		return bRes;
	}

	void CPhonebookView::OnBnClickedPhonebookBtnEdit()
	{
		vector<CPIMGroupInfo> vectPIMGroup;
		m_TreeDatasource.GetCallerID(vectPIMGroup);

		vector<FireListDataParameter> vectSelected;
		m_ListLogical.GetSelectedItem(vectSelected);
		
		vector<TSTRING> vDisplayString;
		m_TreeDatasource.GetAllPIMDisplayName(GetPBTypeByNode() == ePhonebookSIM, vDisplayString);

		for(vector<FireListDataParameter>::iterator Iter = vectSelected.begin();Iter != vectSelected.end();Iter++)
		{
			IPIMObjPt PIMObjPt;
			if(GetPIMObjByListdata(Iter->pListData,PIMObjPt))
			{
				if(GetPBTypeByNode() ==ePhonebookSIM )
				{
					if(m_SIMContactEditor.DoModal(PIMObjPt, vDisplayString) == IDOK)
						UpdateDefaultDevice((CQTreeNode*)Iter->pListData->GetLPARAM(),PIMObjPt,Iter->pListData,Iter->nRow);
				}
				else if(GetPBTypeByNode() ==ePhonebookME )
				{
					TSTRING strOrginalType = GetPIMObjGroup(PIMObjPt);					
					strOrginalType = strOrginalType == _T("0") || strOrginalType == _T("MS") ? _T("") : strOrginalType;
					if(m_ContactEditor.DoModal(PIMObjPt,vectPIMGroup,vDisplayString) == IDOK)
					{
#ifdef _DEBUG
						TSTRING strRaw;
						PIMObjPt->encode(strRaw);
#endif			
						UpdateDefaultDevice((CQTreeNode*)Iter->pListData->GetLPARAM(),PIMObjPt,Iter->pListData,Iter->nRow);
						TSTRING strType = GetPIMObjGroup(PIMObjPt);
						if(strOrginalType != strType)
							Func_SyncFromMS(RT_Self);
							
					}
				}
			}
		}
	}

	void CPhonebookView::Edit(IPIMObjPt PIMObjPt)
	{
	}

	void	CPhonebookView::Insert()
	{
		g_pThis->OnBnClickedPhonebookBtnNew();
	}

	void	CPhonebookView::Edit()
	{
		g_pThis->OnBnClickedPhonebookBtnEdit();
	}

	void	CPhonebookView::Delete()
	{
		g_pThis->OnBnClickedPhonebookBtnDelete();
	}

	void CPhonebookView::OnBnClickedPhonebookBtnDelete(bool bForceDelete)
	{
		vector<FireListDataParameter> vectSelected;
		m_ListLogical.GetSelectedItem(vectSelected);

		if(vectSelected.empty() == false)
		{
			if(bForceDelete || ShowMessage(g_pThis->GetSafeHwnd(),_T("IDS_PHONEBOOK_DELETE_PROMPT"),g_pThis->GetString(_T("IDS_PHONEBOOK_FUNCTION")),QMB_YESNO) == QIDYES)
			{
				bool bHadDeleted = false;
				CQTreeNode* pParentTreeNode = NULL;
				CWaitCursor wait;
				for(vector<FireListDataParameter>::iterator Iter = vectSelected.begin();Iter != vectSelected.end();Iter++)
				{
					CQListData* pListRowData = (CQListData*)Iter->pListData;
					CGUIDTreeNode* pTreeNode = (CGUIDTreeNode*)pListRowData->GetLPARAM();
					pParentTreeNode = pTreeNode->GetParent();
					if(m_TreeDatasource.DeleteNode(pTreeNode))						
						bHadDeleted = true;	
				}
				if(bHadDeleted && pParentTreeNode)
					DisplayNodeInfoToList(pParentTreeNode,true);
			}
		}
	}

	void CPhonebookView::OnDestroy()
	{
		DestoryPhonebook();
		CFormView::OnDestroy();		
	}

	void CPhonebookView::Event(const TSTRING& strEvent,long nParam)
	{
		if(strEvent ==  TNotify_After_Switch)
		{
			m_bIsShown = true;
			SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
			if(view_switch)
			{
				TSTRING strModel, strIMEI;
				SetConnect(view_switch->m_pIMainframe->IsConnected(strModel, strIMEI));
				//EnableEdit(false);
				//EnableInsert(false);
			}
		}
		else if(strEvent ==  TNotify_Before_Switch)
		{
			SwitchViewPara* view_switch = (SwitchViewPara*)nParam;
			view_switch->m_bCanSwitch = true;
			nParam = (long)view_switch;
			m_bIsShown = false;
		}
		else if(strEvent ==  TNotify_Close)
		{
			CloseViewPara* view_close = (CloseViewPara*)nParam;
			view_close->m_bCanClose = true;
			nParam = (long)view_close;
		}
		else if(strEvent ==  DEVICE_CONNECT)
		{
			if(m_bIsShown)
				SetConnect(true);
		}
		else if(strEvent ==  DEVICE_DISCONNECT)
		{
			m_ContactEditor.Disconnect(_T("IDS_PHONEBOOK_FUNCTION"));
			m_SIMContactEditor.Disconnect(_T("IDS_PHONEBOOK_FUNCTION"));

			SetConnect(false);
		}
		else if(strEvent ==  EVENT_TREE_UI_DRAG_BEGIN)
		{
		}
		else if(strEvent ==  EVENT_TREE_SELECT_NODE)
		{
			DisplayNodeInfoToList((CQTreeNode*)nParam);
		}
		else if(strEvent ==  EVENT_TREE_RBUTTONDOWN_NODE)
		{
		}		
		else if(strEvent == EVENT_LIST_UI_DELETE_ROW)
		{
			OnBnClickedPhonebookBtnDelete();
		}
		else if(strEvent ==  EVENT_LIST_UI_LDBCLICK)
		{
			OnBnClickedPhonebookBtnEdit();
		}
		else if(strEvent ==  EVENT_LIST_UI_ITEMCHANGED)
		{
			vector<fdp>* pvectFDP = (vector <fdp>*)nParam;
			if(pvectFDP->size() == 0)
				EnableEdit(false);
			else if(pvectFDP->size() == 1)
				EnableEdit(true);
			else
			{
				if(m_pEditButton)
					m_pEditButton->SetEnable(false);
				if(m_pDeleteButton)
					m_pDeleteButton->SetEnable(m_bIsConnect);
			}
		}
		else if(strEvent == EVENT_LIST_UI_DRAG_BEGIN)
		{
			ResetFlag();
			m_mapDeleteNodes.clear();
			//Set values for drag item information
			m_bDragging = true;
			if(m_pCurrentNode)
				m_strDragingGroup = m_pCurrentNode->GetName();

			CDragDropFireParam *pFireParam = (CDragDropFireParam*)(nParam);			

			vector<FireListDataParameter> vectSelected;
			m_ListLogical.GetSelectedItem(vectSelected);
			for(vector<FireListDataParameter>::iterator Iter = vectSelected.begin();Iter != vectSelected.end();Iter++)
			{
				pFireParam->SetEffect( DROPEFFECT_COPY | DROPEFFECT_MOVE | DROPEFFECT_LINK);
				TSTRING strTemp = SaveTempVCF(Iter->pListData);
				if(!strTemp.empty())
				{
					pFireParam->Insert(strTemp.c_str());
					CGUIDTreeNode* pTreeNode = (CGUIDTreeNode*)Iter->pListData->GetLPARAM();
					m_mapDeleteNodes[strTemp] = pTreeNode;
				}
			}
		}
		else if(strEvent == EVENT_DRAG_END)
		{
			
			if(DROPEFFECT_MOVE == nParam)
				m_bEffectMove = true;
				//OnBnClickedPhonebookBtnDelete(true);

			//Should Reset m_bDragging
			m_bDragEnd = true;
		}
		else if(strEvent == EVENT_LIST_UI_DROP)
		{
			CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
			if(!m_bDragging && m_pCurrentNode)
				InsertPIMFromFiles(pParam,m_pCurrentNode);

			ResetFlag();
		}
		else if(strEvent == EVENT_TREE_UI_DROP)
		{
			CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
			if(pParam) 
			{
				CQTreeNode* pNode = (CQTreeNode*)pParam->GetPara();
				if(m_bDragging && pNode)
				{
					if(pNode->GetName() != m_strDragingGroup)
						ChangePIMFromFiles(pParam);
				}
				else
					InsertPIMFromFiles(pParam);
			}

			ResetFlag();
		}
		else if(strEvent == EVENT_DRAG_ENTER)
		{
			CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
		}
		else if(strEvent == EVENT_LIST_UI_DRAG_OVER)
		{
			CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
			if(pParam) 
			{
				if(IsDragging())
					pParam->SetEffect(DROPEFFECT_LINK);
			}
		}
		else if(strEvent == EVENT_DRAG_OVER)
		{
			CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
			if(pParam) 
			{
				CQTreeNode* pNode = (CQTreeNode*)pParam->GetPara();
				if(pNode)
					TRACE(_T("Drag target : %s\n"),pNode->GetName().c_str());
				if(IsDragging() && pNode)
				{					
					if(pNode->IsRoot() || pNode->GetName() == m_strDragingGroup || IsOverMSLimit(false) || IsOverGroupLimit(pNode, false)  )
						pParam->SetEffect(DROPEFFECT_LINK);
					else
						pParam->SetEffect(pParam->GetKeyState() &  MK_CONTROL ? DROPEFFECT_COPY : DROPEFFECT_MOVE );
				}
				else if(IsDragging() && pNode == NULL)
				{
					pParam->SetEffect(DROPEFFECT_LINK);
				}
				else
				{
					pParam->SetEffect(pParam->GetKeyState() &  MK_SHIFT ? DROPEFFECT_MOVE : DROPEFFECT_COPY);
				}
			}
		}
		else if(strEvent == EVENT_DRAG_LEAVE)
		{
			CDragDropFireParam *pParam = (CDragDropFireParam*)nParam;
		}
		else if(strEvent == EVENT_SyncProcess_FINISH)
		{
			if(m_pCurrentNode)
				DisplayNodeInfoToList(m_pCurrentNode,true);
			m_nRefresType = RT_Self;
		}
		else if(strEvent == EVENT_LIST_UI_HEADER_BUTTON_HIT)
		{
			ColumnData* pColumnData = (ColumnData*)nParam;
			if(pColumnData)
			{
				CString szSortColumnKey = pColumnData->m_strKey.c_str();
				m_ListDatasource.SortByColumn(szSortColumnKey);
			}
		}
		else if(strEvent == EVENT_BUTTON_CLICK)
		{
			if(m_pInsertButton->GetID() == nParam)
				OnBnClickedPhonebookBtnNew();
			else if(m_pEditButton->GetID() == nParam)
				OnBnClickedPhonebookBtnEdit();
			else if(m_pDeleteButton->GetID() == nParam)
				OnBnClickedPhonebookBtnDelete();
			else if(m_pSyncToOutlookButton->GetID() == nParam)
				Func_SyncToOutlook();
			else if(m_pSyncFromOutlookButton->GetID() == nParam)
				Func_SyncFromOutlook();
			else if(m_pRefreshFromDefaultButton->GetID() == nParam)
				TwowaySync();
		}
	}

	bool CPhonebookView::IsOverGroupLimit(CQTreeNode* pTreeNode, bool bShowMessage) // Default pTreeNode is a folder
	{
		bool bInsert = false;
		if(pTreeNode && pTreeNode->GetParent() &&pTreeNode->GetParent()->GetName() == g_strMS)
		{ 
			int nCount = m_TreeDatasource.GetFolderCount(static_cast<CGUIDTreeNode*>(pTreeNode));
			IPIMObjPt PIMObjPt;
			CVCardCtrl VCardCtrl(PIMObjPt);
			int nLimitGroup = VCardCtrl.GetLimitation(_T("GroupMaxRecords"));
			//assert(nLimitGroup == -1);
			nLimitGroup = nLimitGroup!= -1 ? nLimitGroup : 50;
			if( nCount > (nLimitGroup-1))
			{
				bInsert = true;
				if(bShowMessage)
				{
					CString cstrOverLimit;
					cstrOverLimit.Format(GetString(_T("IDS_PHONEBOOK_MsgGroupLimit")),nLimitGroup);
					ShowMessage(g_pThis->GetSafeHwnd(),cstrOverLimit,GetString(_T("IDS_PHONEBOOK_FUNCTION")));
				}
			}
		}
		return bInsert;
	}
	
	bool CPhonebookView::IsOverMSLimit(bool bShowMessage)
	{
		bool bRes = false;
		int nCount = m_TreeDatasource.GetMECount();
		IPIMObjPt PIMObjPt;
		CVCardCtrl VCardCtrl(PIMObjPt);
		int nLimitCount = VCardCtrl.GetLimitation(_T("TotalRecords"));
		nLimitCount = nLimitCount!= -1 ? nLimitCount : 1000;
		if(nCount > (nLimitCount-1))
		{
			bRes = true;
			if(bShowMessage)
			{
				CString cstrOverLimit;
				cstrOverLimit.Format(GetString(_T("IDS_PHONEBOOK_MSLimit")),nLimitCount);
				ShowMessage(g_pThis->GetSafeHwnd(),cstrOverLimit,GetString(_T("IDS_PHONEBOOK_FUNCTION")));
			}
		}
		return bRes;
	}

	void CPhonebookView::ResetFlag()
	{
		//Reset flag
		m_bEffectMove = false;
		m_bDragging = false;
		m_bDragEnd = false;
		m_strDragingGroup = _T("");
	}

	bool CPhonebookView::IsDragging()
	{
		bool bDragging = m_bDragging;
		if(m_bDragging && m_bDragEnd)
			bDragging = false;
		return bDragging;
	}

	void CPhonebookView::GetPIMObjFromFile(const TSTRING& strVCFFile, IPIMObjPt& PIMObjPt,const TSTRING& strCallerID)
	{
		m_TreeDatasource.CreateEmptyPIMObj(PIMObjPt,strCallerID, false);

		CFile strVCF(strVCFFile.c_str(),CFile::modeRead);
		LARGE_INTEGER nLen;
		nLen.QuadPart = strVCF.GetLength();
		char *szBuffer = new char[nLen.LowPart + 1];
		::ZeroMemory(szBuffer,nLen.LowPart + 1);
		strVCF.Read(szBuffer,nLen.LowPart);
		wstring strRaw = ansitowide(szBuffer);
		delete [] szBuffer;
		strVCF.Close();

		PIMObjPt->decode(strRaw);
		
		if(!strCallerID.empty())
			SetPIMObjGroup(PIMObjPt,strCallerID);
	}

	TSTRING CPhonebookView::GetPIMObjGroup(IPIMObjPt& PIMObjPt)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		return VCardCtrl.GetSpecificString("X-BENQ",_T("CallerID"),0);
	}

	void CPhonebookView::SetPIMObjGroup(IPIMObjPt& PIMObjPt,const TSTRING& strCallerID)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		VCardCtrl.SetSpecificString(strCallerID,"X-BENQ",_T("CallerID"));
#ifdef _DEBUG
		TSTRING strRaw;
		PIMObjPt->encode(strRaw);
#endif
	}

	TSTRING CPhonebookView::GetPIMObjFullName(IPIMObjPt& PIMObjPt)
	{
		return GetPIMObjLastName(PIMObjPt) + _T(" ") + GetPIMObjFirstName(PIMObjPt);
	}

	TSTRING	CPhonebookView::GetPIMObjFirstName(IPIMObjPt& PIMObjPt)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		TSTRING strFirstText;
		VCardCtrl.GetCmnStrData(KEYWORD_FIRSTNAME, strFirstText);
		return strFirstText;
	}

	TSTRING	CPhonebookView::GetPIMObjLastName(IPIMObjPt& PIMObjPt)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		TSTRING strLastText;
		VCardCtrl.GetCmnStrData(KEYWORD_LASTNAME, strLastText);
		return strLastText;
	}

	TSTRING	CPhonebookView::GetPIMObjDisplayName(IPIMObjPt& PIMObjPt)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		TSTRING strText;
		VCardCtrl.GetCmnStrData(KEYWORD_DISPLAY, strText);
		return strText;
	}

	TSTRING CPhonebookView::GetPIMObjPhone(IPIMObjPt& PIMObjPt)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);		
		return VCardCtrl.GetSpecificString("TEL", _T("HOME;VOICE"), 0, 0);
	}

	void CPhonebookView::SetPIMObjName(IPIMObjPt& PIMObjPt,const TSTRING& strLastName,const TSTRING& strFirstName)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		VCardCtrl.SaveCmnData(KEYWORD_LASTNAME, strLastName);
		VCardCtrl.SaveCmnData(KEYWORD_FIRSTNAME, strFirstName);
	}
	
	void CPhonebookView::SetPIMObjDisplayName(IPIMObjPt& PIMObjPt,TSTRING& strDisplayName)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		if(strDisplayName == _T(""))
			strDisplayName = VCardCtrl.GetOfficialDisplayName();
		VCardCtrl.SaveCmnData(KEYWORD_DISPLAY, strDisplayName);
	}

	void CPhonebookView::SetPIMObjPhone(IPIMObjPt& PIMObjPt,const TSTRING& strPhone)
	{
		CVCardCtrl VCardCtrl(PIMObjPt);
		VCardCtrl.SetSpecificString(strPhone, "TEL",_T("HOME;VOICE"), 0, 0);
	}

	//************************************
	// Method:    ExistSameName
	// FullName:  QSYNC_LIB::CPhonebookView::ExistSameName
	// Access:    private 
	// Returns:   bool
	// Qualifier:
	// Parameter: bool bIsInsertToSIM
	//            敘述要新增到SIM還是MS, 以決定要比對SIM的資料還是MS的資料
	// Parameter: const TSTRING & strName
	//            要比對的姓名
	//************************************
	bool CPhonebookView::ExistSameName(bool bIsInsertToSIM, const TSTRING& strName)
	{
		bool bRes = false;
		{
			vector<TSTRING> vDisplayString;
			m_TreeDatasource.GetAllPIMDisplayName(bIsInsertToSIM, vDisplayString);
			for(vector<TSTRING>::iterator iter= vDisplayString.begin(); iter!= vDisplayString.end(); iter++)
			{
				if(strName == (*iter))
				{	
					bRes = true;
					break;
				}
			}
		}
		
		return bRes;
	}
	void CPhonebookView::InsertSIMFromFiles(CDragDropFireParam *pParam)
	{
		//Set the cursor icon be waiting icon (hourglass)
		CWaitCursor WaitCursor;
		for(int i=0;i<pParam->GetFileCount();++i)
		{					
			IPIMObjPt PIMObjPt;
			m_TreeDatasource.CreateEmptyPIMObj(PIMObjPt,_T(""), true);		

			IPIMObjPt tempPIMObjPt;
			GetPIMObjFromFile(pParam->GetByIndex(i),tempPIMObjPt);
			
			TSTRING strDisplayName = GetPIMObjDisplayName(tempPIMObjPt);
			if(strDisplayName == _T(""))
				strDisplayName = StrConnect<TSTRING>(_T(", "), GetPIMObjLastName(tempPIMObjPt), GetPIMObjFirstName(tempPIMObjPt));
			SetPIMObjName(PIMObjPt, _T(""), strDisplayName);
			if(strDisplayName ==  _T(""))
				strDisplayName = GetPIMObjPhone(tempPIMObjPt);
			SetPIMObjPhone(PIMObjPt,GetPIMObjPhone(tempPIMObjPt));

			if(ExistSameName(true, strDisplayName) == false)
			{
				CommonDataPt ptType = new CommonData("PBTYPE");
				ptType->SetInteger(ePhonebookSIM);
				PIMObjPt->SetByName(ptType->GetName(),ptType);

				bool bInsert = m_TreeDatasource.CreateNewPIMObj(PIMObjPt);
				if(!bInsert)
				{
					CommonDataPt spVCardErrData;
					PIMObjPt->GetByName("VCARDERR",spVCardErrData);

					//手機記憶體已滿時會回傳VCardErr 7,但這個值沒有寫在文件裡,所以先不使用enum而直接判斷7
					if(spVCardErrData->GetInteger()==7)
						ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_MEMemoryFull"),_T("IDS_PHONEBOOK_FUNCTION"));
					
					//SIM卡已滿跟上傳檔案格式錯誤都會回傳VCardErr 8,這個值也沒有寫在文件裡,一樣先不使用enum而直接判斷8
					else if(spVCardErrData->GetInteger()==8)
						ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_FormatErrorOrSIMFull"),_T("IDS_PHONEBOOK_FUNCTION"));
					else
						ShowMessage(GetSafeHwnd(),_T("IDS_InsertFail"),_T("IDS_PHONEBOOK_FUNCTION"));
					break;
				}
				if(m_bEffectMove && bInsert)
					m_TreeDatasource.DeleteNode(m_mapDeleteNodes[pParam->GetByIndex(i)]);
			}
			else
			{
				CString cstrDisplayName = strDisplayName.c_str();
				ShowMessage(GetSafeHwnd(),cstrDisplayName+_T(" ")+GetString(_T("IDS_PHONEBOOK_MsgSameName")),_T("IDS_PHONEBOOK_FUNCTION"));
				break;
			}
			
		}
	}

	void CPhonebookView::InsertMSFromFiles(CDragDropFireParam *pParam,const TSTRING& strCallerID,CQTreeNode* pNode)
	{
		//Set the cursor icon be waiting icon (hourglass)
		CWaitCursor WaitCursor;
		for(int i=0;i<pParam->GetFileCount() && !IsOverGroupLimit(pNode);++i)
		{
			if(IsOverMSLimit())
			{
				break;
			}
			IPIMObjPt PIMObjPt;
			GetPIMObjFromFile(pParam->GetByIndex(i),PIMObjPt,strCallerID);

			TSTRING strDisplayName = GetPIMObjDisplayName(PIMObjPt);
			if(strDisplayName == _T(""))
				SetPIMObjDisplayName(PIMObjPt, strDisplayName);

			//從手機的Group複製到另一個Group
			if (m_strDragingGroup.empty() == false	&& m_strDragingGroup !=_T("SIM") && !m_bEffectMove)
			{
				return;
			}
			else if(
				//在手機的group之間移動
				(m_strDragingGroup.empty() == false	&& m_strDragingGroup !=_T("SIM"))
				
				//若不是在手機的group之間移動, 則要檢查是否有同名檔案
				|| ExistSameName(false, strDisplayName) == false
				)
			{
				CommonDataPt ptType = new CommonData("PBTYPE");
				ptType->SetInteger(ePhonebookME);
				PIMObjPt->SetByName(ptType->GetName(),ptType);
				bool bInsert = m_TreeDatasource.CreateNewPIMObj(PIMObjPt);
				if(!bInsert)
				{
					CommonDataPt spVCardErrData;
					PIMObjPt->GetByName("VCARDERR",spVCardErrData);

					//手機記憶體已滿時會回傳VCardErr 7,但這個值沒有寫在文件裡,所以先不使用enum而直接判斷7
					if(spVCardErrData->GetInteger()==7)
						ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_MEMemoryFull"),_T("IDS_PHONEBOOK_FUNCTION"));
					
					//SIM卡已滿跟上傳檔案格式錯誤都會回傳VCardErr 8,這個值也沒有寫在文件裡,一樣先不使用enum而直接判斷8
					else if(spVCardErrData->GetInteger()==8)
						ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_FormatErrorOrSIMFull"),_T("IDS_PHONEBOOK_FUNCTION"));
					else
						ShowMessage(GetSafeHwnd(),_T("IDS_InsertFail"),_T("IDS_PHONEBOOK_FUNCTION"));
					break;
				}
				if(m_bEffectMove && bInsert)
					m_TreeDatasource.DeleteNode(m_mapDeleteNodes[pParam->GetByIndex(i)]);
			}
			else
			{
				CString cstrDisplayName = strDisplayName.c_str();
				ShowMessage(GetSafeHwnd(),cstrDisplayName+_T(" ")+GetString(_T("IDS_PHONEBOOK_MsgSameName")),_T("IDS_PHONEBOOK_FUNCTION"));
				break;
			}
		}
	}

	void CPhonebookView::ChangePIMFromFiles(CDragDropFireParam *pParam)
	{
		CQTreeNode* pNode = (CQTreeNode*)pParam->GetPara();
		if(pNode)
			InsertPIMFromFiles(pParam,pNode);
	}

	void  CPhonebookView::InsertPIMFromFiles(CDragDropFireParam *pParam,CQTreeNode* pNode)
	{
		if(pNode)
		{
			TSTRING strCallerID = pNode->GetName();
			TRACE(_T("Drag target : %s\n"),pNode->GetName().c_str());

			ePhonebookType eType = GetPBTypeByNode(pNode);

			bool bDisplayNodeInfoToList = true;
			if(eType == ePhonebookSIM)
			{
				if(ShowMessage(GetSafeHwnd(),_T("IDS_PHONEBOOK_PROMPT_COPYTOSIM"),_T("IDS_PHONEBOOK_FUNCTION"),QMB_OKCANCEL)==IDOK)
					InsertSIMFromFiles(pParam);
				else
					bDisplayNodeInfoToList = false;
			}
			else
				InsertMSFromFiles(pParam,strCallerID, pNode);

			if(bDisplayNodeInfoToList)
				DisplayNodeInfoToList(m_TreeCtrl.GetCurrentNode(),true);
		}	
	}

	void CPhonebookView::InsertPIMFromFiles(CDragDropFireParam *pParam)
	{		
		CQTreeNode* pNode = (CQTreeNode*)pParam->GetPara();
		InsertPIMFromFiles(pParam,pNode);
	}

	TSTRING CPhonebookView::SaveTempVCF(CQListData* pListData)
	{
		TSTRING strTemp;
		TCHAR szFolder[MAX_PATH] = {0};
		bool bRes = Win32Utility::GetShellFolder(szFolder,CSIDL_COMMON_TEMPLATES,_T("QSyncer"));
		if(bRes)
		{			
			IPIMObjPt PIMObjPt;
			if(GetPIMObjByListdata(pListData,PIMObjPt))
			{
				CVCardCtrl VCardCtrl(PIMObjPt);
				TSTRING strDisplayName = VCardCtrl.GetOfficialDisplayName();
				CBasicData* pData = pListData->GetValue(str_NAME_LISTKEY);
				if(pData)
					strTemp = szFolder + Win32Utility::GetCompatiableWin32FileName( pData->GetTextString().c_str() ) + _T(".vcf");

				CFile file;
				if(file.Open(strTemp.c_str(),CFile::modeCreate | CFile::modeWrite | CFile::typeBinary))				
				{
					TSTRING strCode;
					PIMObjPt->encode(strCode);
					string strAnsiCode = widetoansi(strCode,CP_ACP);
					file.Write(strAnsiCode.c_str(),static_cast<UINT>(strAnsiCode.size()));
					file.Close();
				}
				else
				{
					strTemp.empty();
				}
			}
		}
		return strTemp;
	}

	CGUIDTreeNode* CPhonebookView::GetGuidNodeByListdata(CQListData* pListData)
	{
		assert(pListData->GetLPARAM());
		return (CGUIDTreeNode*)pListData->GetLPARAM();
	}

	bool CPhonebookView::GetPIMObjByTreeNode(CGUIDTreeNode* pTreeNode,IPIMObjPt& PIMObjPt)
	{
		return m_TreeDatasource.GetPIMByGuid(pTreeNode->GetGUID(),PIMObjPt);
	}

	bool CPhonebookView::GetPIMObjByListdata(CQListData* pListData,IPIMObjPt& PIMObjPt)
	{
		return GetPIMObjByTreeNode(GetGuidNodeByListdata(pListData),PIMObjPt);
	}
	
	bool CPhonebookView::DisplayNodeInfoToList(CQTreeNode* pNode, bool bForceRefresh)
	{		
		bool bRet = false;
		m_ListCtrl.LockWindowUpdate();
		if(pNode)
		{
			EnableInsert(!pNode->IsRoot());

			if(pNode && (bForceRefresh || m_pCurrentNode != pNode))
			{
				bool bNeedFresh = m_pCurrentNode != pNode;

				if(bNeedFresh)
					m_pCurrentNode = pNode;

				if(bNeedFresh || bForceRefresh)
				{
					m_ListDatasource.Clear();

					int nSize = pNode->GetChildSize();
					for(int i = 0; i < nSize; i++)
					{
						CGUIDTreeNode* pChildNode = static_cast<CGUIDTreeNode*>(pNode->GetChild(i));
						if(pChildNode && pChildNode->GetGUID() != GUID_NULL)
							m_ListDatasource.AddItem(pChildNode);
					}
					bRet = true;
				}
			}
		}
		m_ListCtrl.UnlockWindowUpdate();
		return bRet;
	}

	void	CPhonebookView::DefaultRefresh()
	{
		TwowaySync();
	}

	void	CPhonebookView::SyncFromOutlook()
	{
		g_pThis->Func_SyncFromOutlook();
	}

	void	CPhonebookView::SyncToOutlook()
	{
		g_pThis->Func_SyncToOutlook();
	}

	void	CPhonebookView::Func_SyncFromOutlook()
	{
		if (m_bIsConnect == false)
			return;

		if (m_CSyncFromOutlookDlg.DoModal() == IDOK)
		{
			m_nRefresType = RT_OutlookFrom;
			SyncWithDevice(IDC_COMBO_TARGET_DEVICE);
			Sync(OneWayFromClient);
		}
	}

	void	CPhonebookView::Func_SyncToOutlook()
	{
		if (m_bIsConnect == false)
			return;
		if (m_CSyncToOutlookDlg.DoModal() == IDOK)
		{
			m_nRefresType = RT_OutlookTo;
			SyncWithDevice(IDC_COMBO_TARGET_DEVICE);
			Sync(OneWayFromServer);
		}
	}

	void CPhonebookView::Func_SyncFromMS(RefreshType nType)
	{//for refresh UI
		
		//Clear All item
		m_ListLogical.RefreshClear();
 		m_TreeLogic.RefreshClear();
		m_pCurrentNode = NULL;

		if(m_bIsConnect)
		{
			m_nRefresType = nType;
			m_TreeDatasource.CreateFolder();
			SyncWithDevice(IDC_COMBO_SOURCE_DEVICE);
			Sync(TwoWay);

		}
	}

	void CPhonebookView::TwowaySync()
	{
		if(ShowMessage(g_pThis->GetSafeHwnd(),_T("IDS_PHONEBOOK_SYNC_PROMPT"),g_pThis->GetString(_T("IDS_PHONEBOOK_FUNCTION")),QMB_YESNO) == QIDYES)
			g_pThis->Func_SyncFromMS();
	}

	void CPhonebookView::Sync(SyncType synctype)
	{
		using namespace QSYNC_LIB;

		if(m_bIsConnect == true)
		{
			m_SyncProcess.Register(this,EVENT_SyncProcess_FINISH);
			m_SyncProcess.start(synctype);
		}
	}

	void CPhonebookView::SyncWithDevice(UINT nID)
	{
		IDevice* pIDevice = NULL;
		CComboBox* pDevice = (CComboBox*)GetDlgItem(nID);	
		if(pDevice && ::IsWindow(pDevice->GetSafeHwnd()))
		{
			if(pDevice->GetCount() > 0)
				pIDevice = (IDevice*)pDevice->GetItemDataPtr(pDevice->GetCurSel());
		}

		if(pIDevice && pIDevice->GetDisplayName() != _T("IDS_Local"))
			m_SyncProcess.push(pIDevice);
	}
	void CPhonebookView::SetXMLUI()
	{
		const std::wstring strBkImage = L"MainFrame\\Bk_GeneralView.png";
		SetBKImage(strBkImage,BK_Fixed);

		CLayoutManager*	pLayoutManager = new CLayoutManager(this, this);
		Widge<HDC>* pWidget = pLayoutManager->Load(_T("Phonebook_ViewID"));
		//m_pButtonOK = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T("Win32IconButton_0"));
		m_pInsertButton = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T("Win32Button_0"));
		m_pEditButton = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T("Win32Button_1"));
		m_pDeleteButton = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T("Win32Button_2"));
		m_pSyncToOutlookButton = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T("Win32Button_3"));
		m_pSyncFromOutlookButton = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T("Win32Button_4"));
		m_pRefreshFromDefaultButton = (WIN32ButtonWidge *)pLayoutManager->GetWidgetByID(_T("Win32Button_5"));

		SetLayout(pWidget);
		delete pLayoutManager;
	}

	void CPhonebookView::EnableInsert(bool bEnable)
	{
		if(m_pInsertButton)
			m_pInsertButton->SetEnable(bEnable && m_bIsConnect);
	}

	void CPhonebookView::EnableEdit(bool bEnable)
	{
		if(m_pEditButton)
			m_pEditButton->SetEnable(bEnable && m_bIsConnect);
		if(m_pDeleteButton)
			m_pDeleteButton->SetEnable(bEnable && m_bIsConnect);
	}

	void CPhonebookView::AddDeviceToCombobox(UINT nComboID,IDevicePt ptDevice)
	{	
		CComboBox* pComBo = (CComboBox*)GetDlgItem(nComboID);
		TSTRING strName =ptDevice->GetDisplayName();
		int nIndex = pComBo->AddString(GetString(strName));
		pComBo->SetItemDataPtr(nIndex,(void*)ptDevice.GetPointer(ptDevice));
	}

	void CPhonebookView::SetRemotePIMDevice()
	{
		CComboBox* pTargetDevice = (CComboBox*)GetDlgItem(IDC_COMBO_TARGET_DEVICE);
		CComboBox* pSourceDevice = (CComboBox*)GetDlgItem(IDC_COMBO_SOURCE_DEVICE);
		while(pTargetDevice->GetCount() > 0)
			pTargetDevice->DeleteString(0);
		while(pSourceDevice->GetCount() > 0)
			pSourceDevice->DeleteString(0);

		//Assign device to UI
		IDevicePt defaultDevice;
		if(m_TreeDatasource.GetDefaultDevice(defaultDevice) && defaultDevice)
			AddDeviceToCombobox(IDC_COMBO_SOURCE_DEVICE,defaultDevice);

		IDevicePts devices;
		m_TreeDatasource.GetRemoteDevices(devices);
		for(IDevicePts::iterator Iter = devices.begin();Iter != devices.end();Iter++)
			AddDeviceToCombobox(IDC_COMBO_TARGET_DEVICE,*Iter);

		pTargetDevice->SetCurSel(0);
		pTargetDevice->SelectString(0,m_strUserTargetDevice.Get().c_str());
		pSourceDevice->SetCurSel(0);
		pSourceDevice->SelectString(0,m_strUserSourceDevice.Get().c_str());
		EnableSync();
	}

	void CPhonebookView::EnableSync()
	{	
		if(m_pTwoWaySyncButton)
			m_pTwoWaySyncButton->SetEnable(EnableSyncButton());
	}

	bool CPhonebookView::EnableSyncButton()
	{
		return GetComboString(IDC_COMBO_SOURCE_DEVICE) != GetComboString(IDC_COMBO_TARGET_DEVICE);
	}

	CString CPhonebookView::GetComboString(UINT nID)
	{
		CString strString;
		CComboBox* pDevice = (CComboBox*)GetDlgItem(nID);
		if(pDevice && pDevice->GetCurSel() >= 0)		
			pDevice->GetLBText(pDevice->GetCurSel(),strString);
		return strString;
	}

	void CPhonebookView::SaveUserSelected(UINT nID)
	{
		CString strBuffer = GetComboString(nID);
		if(!strBuffer.IsEmpty())
		{
			if(nID == IDC_COMBO_SOURCE_DEVICE)
				m_strUserSourceDevice = (LPCTSTR)strBuffer;
			else if(nID == IDC_COMBO_TARGET_DEVICE)
				m_strUserTargetDevice = (LPCTSTR)strBuffer;
		}
	}

	void CPhonebookView::OnCbnSelchangeComboSourceDevice()
	{
		SaveUserSelected(IDC_COMBO_SOURCE_DEVICE);
		EnableSync();
	}

	void CPhonebookView::OnCbnSelchangeComboTargetDevice()
	{
		SaveUserSelected(IDC_COMBO_TARGET_DEVICE);
		EnableSync();
	}

	void CPhonebookView::SetConnect(bool bIsConnect)
	{
		if(m_bIsConnect != bIsConnect)
		{
			m_bIsConnect = bIsConnect;

			if(m_bIsConnect)
				m_TreeDatasource.Connect();
			else
				m_TreeDatasource.Disconnect();

			EnableConnectionRelatedButton();

			if(m_pTwoWaySyncButton)
				m_pTwoWaySyncButton->SetEnable(m_bIsConnect);

			if(m_bIsConnect)
				SetRemotePIMDevice();

			Func_SyncFromMS();
		}
	}
}

#pragma warning(pop)