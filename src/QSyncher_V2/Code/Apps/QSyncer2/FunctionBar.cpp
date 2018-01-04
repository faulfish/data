/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   FunctionBar.cpp  $
*  $Revision:   1.61  $
*  $Date:   Mar 21 2008 14:49:06  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/QSyncer2/FunctionBar.cpp.-arc  $
   
      Rev 1.61   Mar 21 2008 14:49:06   Jame Tsai
   update
   
      Rev 1.60   Mar 14 2008 09:39:26   Jame Tsai
   update
   
      Rev 1.59   Mar 06 2008 16:58:52   Alan Tu
   use strID to be button name
   
      Rev 1.58   Feb 01 2008 11:35:44   Alan Tu
   refactory
   
      Rev 1.57   Jan 29 2008 16:37:28   Alan Tu
   update
   
      Rev 1.56   Jan 02 2008 21:16:24   Jame Tsai
   fix error
*
*
***************************************************************************/
#include "stdafx.h"
#include "FunctionBar.h"
#include "MainFrm.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "..\..\CommonUtil\LogPolicy.h"

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)

namespace QSYNC_LIB
{

	IMPLEMENT_DYNAMIC(CFunctionBar, CDialog)

		CFunctionBar* g_pThis = NULL;

	CFunctionBar::CFunctionBar(IMainframe *pIMainframe, CWnd* pParent /*=NULL*/)
		: Win32DoubleBufferDialogWidge(CFunctionBar::IDD, pParent)
		,m_bInit(false)
		,m_nDockBarID(-1)
		,m_nCurrentID(-1)
		,m_pApp(NULL)
		,m_pILocalization(NULL)
		,m_pIMainframe(pIMainframe)
		,m_pLayout(NULL)
	{		
		TRACE_FILE_LINE_INFO();

		CreateILocalization(&m_pILocalization);
		m_pILocalization->Register(this,TNotify_Language_Change);	
		m_pCfgTool = CreateIConfigure(_T("FunctionBar"));
		m_pCfgTool->Load(STORAGE_INIFILE);
	}

	CFunctionBar::~CFunctionBar()
	{
		TRACE_FILE_LINE_INFO();

		m_pILocalization->Unregister(this,TNotify_Language_Change);
	}

	void CFunctionBar::DoDataExchange(CDataExchange* pDX)
	{
		TRACE_FILE_LINE_INFO();

		CDialog::DoDataExchange(pDX);
	}

	void CFunctionBar::AddViewButtonToBar( CQSetting &oISetting, QSyncerFramework* pQSyncerFramework, std::wstring strImagePrefix )
	{
		TRACE_FILE_LINE_INFO();

		for(int j = 0; j < oISetting.Size(); ++j) {
			CString strDisplayName = oISetting.GetValue(j).c_str();
			for(size_t i=0;i<m_pApp->GetViewCount();i++)
			{
				UINT nID = static_cast<long>(i)+5000;
				assert(nID <= 5100);
				if(nID <= 5100)
				{
					m_pApp->GetViewByIndex(i,&pQSyncerFramework);

					if(pQSyncerFramework && pQSyncerFramework->m_strDisplayName == strDisplayName)
					{
						CRect rcButton(0,0,0,0);
						CString strDisplay = pQSyncerFramework->m_strDisplayName;
						strDisplay = GetString(strDisplay.GetBuffer(0));

						std::wstring strFunctionBarName = pQSyncerFramework->m_strID;

						const std::wstring strBKImage = strFunctionBarName +_T("\\Button\\Btn_Original.png");
						const std::wstring strHighLightImage = strFunctionBarName +_T("\\Button\\Btn_MouseOver.png");
						const std::wstring strClickImage = strFunctionBarName +_T("\\Button\\Btn_MouseDown.png");
						const std::wstring strDisableImage = strFunctionBarName +_T("\\Button\\Btn_Disable.png");
						const std::wstring strBKImage2 = strFunctionBarName +_T("\\Button\\Btn_Original_2.png");
						const std::wstring strHighLightImage2 = strFunctionBarName +_T("\\Button\\Btn_MouseOver_2.png");
						const std::wstring strClickImage2 = strFunctionBarName +_T("\\Button\\Btn_MouseDown_2.png");
						const std::wstring strDisableImage2 = strFunctionBarName +_T("\\Button\\Btn_Disable_2.png");

						TSTRING strEmpty;
						if(nID > 4999 && nID < 5101)
						{   // Add button here
							WIN32IconButton* pWidge = new WIN32IconButton(this, strEmpty, strBKImage, strHighLightImage, strClickImage, strDisableImage, FunctionButtonClicked, nID);
							pWidge->BeToggleButten();
							pWidge->SetFourImagesOfButtonDown(strBKImage2,strHighLightImage2,strClickImage2,strDisableImage2);
							m_pLayout->AddWidget(pWidge, VCenter | HCenter);
							m_mapFunctionButton.insert(std::pair<UINT,WIN32IconButton*>(nID,pWidge));
						}
						m_mapFunctions.insert(std::pair<UINT,QSyncerFramework*>(nID,pQSyncerFramework));
						CString strPreviousView = m_pCfgTool->GetCfgVar(_T("CurrentView"), (LPCTSTR)pQSyncerFramework->m_strDisplayName).c_str();
						if(strPreviousView == pQSyncerFramework->m_strDisplayName)
							OnFunctionButtonClicked(nID);
						break;
					}
				}
			}
		}
	}

	BOOL CFunctionBar::OnInitDialog() 
	{
		TRACE_FILE_LINE_INFO();

		CDialog::OnInitDialog();

		g_pThis = this;
		m_bInit = true;		

		TSTRING strBkImage = _T("MainFrame\\BK_Functionbar.png");
		g_pThis->SetBKImage(strBkImage,BK_Stretch);

		m_pLayout = new BoxLayout<HDC>(/*TopToBottom*/);
		m_pLayout->SetCell(108,53+10);
		m_pLayout->SetSpacing(5);
		m_pLayout->SetMargin(5,5,0,0);
		m_pLayout->SetAlignment(VCenter | HCenter);
		SetLayout(m_pLayout);

		return TRUE;   // return TRUE unless you set the focus to a control
		// EXCEPTION: OCX Property Pages should return FALSE
	}

	BEGIN_MESSAGE_MAP(CFunctionBar, CDialog)
		ON_WM_DESTROY()
		ON_MESSAGE( WM_USER+1, ChangeAlignment )
		ON_CONTROL_RANGE(BN_CLICKED, 5000, 5100, OnFunctionButtonClicked )
	END_MESSAGE_MAP()


	// CFunctionBar message handlers

	void CFunctionBar::FindViews()
	{	
		TRACE_FILE_LINE_INFO();

		std::wstring strImagePrefix;
#ifdef _DEBUG
		strImagePrefix = L"..\\Debug\\";
#else
		strImagePrefix = L"";
#endif

		CQSetting oISetting;
		GetApplicationMobileConFig(oISetting);
		QSyncerFramework* pQSyncerFramework = NULL;
		m_pApp = (CQSyncer2App*)::AfxGetApp();
		m_pApp->FindAllViews();

		//make sure mainfram's stack layout is ready
		AddViewButtonToBar(oISetting, pQSyncerFramework, strImagePrefix);		

		SetLayout();
	}

	void CFunctionBar::OnTimer(UINT_PTR nIDEvent)
    {
		TRACE_FILE_LINE_INFO();

	}

	void CFunctionBar::OnDestroy()
	{
		TRACE_FILE_LINE_INFO();

		CDialog::OnDestroy();
		m_pCfgTool->Save();
		DeleteIConfigure(_T("FunctionBar"));
		// TODO: Add your message handler code here
		for(ButtonArray::iterator Iter = m_FunctionButtons.begin();Iter != m_FunctionButtons.end();Iter++)
		{
			(*Iter)->DestroyWindow();
			delete (*Iter);
		}
	}

	BOOL CFunctionBar::IsHorz() const
	{
		TRACE_FILE_LINE_INFO();

		return (m_nDockBarID == AFX_IDW_DOCKBAR_TOP || m_nDockBarID == AFX_IDW_DOCKBAR_BOTTOM);
	}

	LRESULT CFunctionBar::ChangeAlignment( WPARAM wParam, LPARAM lParam )
	{
		TRACE_FILE_LINE_INFO();

		m_nDockBarID = static_cast<UINT>(wParam);
		if(IsHorz() == TRUE)
			m_pLayout->SetDirection(LeftToRight);
		else
			m_pLayout->SetDirection(TopToBottom);
		return 1;
	}

	void CFunctionBar::FunctionButtonClicked(UINT nID)
	{
		TRACE_FILE_LINE_INFO();

		g_pThis->OnFunctionButtonClicked(nID);
	}

	void CFunctionBar::OnFunctionButtonClicked(UINT nID)
	{
		TRACE_FILE_LINE_INFO();

		std::map<UINT,WIN32IconButton*>::iterator Iter;

		for(Iter = m_mapFunctionButton.begin(); Iter != m_mapFunctionButton.end() ; ++Iter)
		{
			if(Iter->first == nID)
				Iter->second->SetButtonCheck(true);
			else if( Iter->second)
				Iter->second->SetButtonCheck(false);
		}

		if(m_nCurrentID != nID)
		{
			m_pCfgTool->SetCfgVar(_T("CurrentView"), (LPCTSTR)(m_mapFunctions[nID]->m_strDisplayName));
			m_pCfgTool->Save();
			if(UnactiveView(m_nCurrentID))
				ActiveView(nID);
		}

	}

	bool CFunctionBar::UnactiveView(UINT nID)
	{
		TRACE_FILE_LINE_INFO();
		
		bool bRes = false;
		if(nID == -1)
		{
			bRes = true;
		}
		else
		{
			std::map<UINT,QSyncerFramework*>::iterator Iter = m_mapFunctions.find(m_nCurrentID);
			if(Iter != m_mapFunctions.end())
			{
				SwitchViewPara switchViewPara(m_pIMainframe);
				if(Iter->second->m_pINotify)
				{
					Iter->second->m_pINotify->Event(TNotify_Before_Switch,(long)&switchViewPara);
					bRes = switchViewPara.m_bCanSwitch;					
					if(bRes == false)
					{
						CString strDisplay;
						if(m_pILocalization)
						{
							strDisplay = GetString(_T("IDS_SWITCH_FAIL:"));
							strDisplay += GetString(Iter->second->m_strDisplayName.GetBuffer(0));
						}					
						CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
						pMainFrame->SetStatusText(strDisplay);
					}
				}
				else
				{//Without INotify
					bRes = switchViewPara.m_bCanSwitch;//By default
				}
			}
		}
		return bRes;
	}

	bool CFunctionBar::ActiveView(UINT nID)
	{
		TRACE_FILE_LINE_INFO();
		
		bool bRes = false;
		if(nID == -1)
		{
		}
		else
		{
			std::map<UINT,QSyncerFramework*>::iterator Iter = m_mapFunctions.find(nID);
			if(Iter != m_mapFunctions.end())
			{
				if(m_pApp)
				{
					if(m_pApp->SwitchView(Iter->second))
					{
						SwitchViewPara switchViewPara(m_pIMainframe);
						m_nCurrentID = nID;
						if(Iter->second->m_pINotify)
							Iter->second->m_pINotify->Event(TNotify_After_Switch,(long)&switchViewPara);
					}
				}
			}
		}
		return bRes;
	}

	void CFunctionBar::Event(const TSTRING& strEvent,long nParam)
	{
		TRACE_FILE_LINE_INFO();
		
		if(strEvent == TNotify_Language_Change)
		{
			if(m_pApp)
			{
				for(ButtonArray::iterator Iter = m_FunctionButtons.begin();Iter != m_FunctionButtons.end();Iter++)
				{
					int nID = (*Iter)->GetDlgCtrlID();
					QSyncerFrameworkMap::iterator qIter = m_mapFunctions.find(nID);
					if(qIter != m_mapFunctions.end())
					{
						CString strDisplay = qIter->second->m_strDisplayName;
						strDisplay = GetString(strDisplay.GetBuffer(0));
						(*Iter)->SetWindowText(strDisplay);
					}
				}
			}
		}
		else if(strEvent == DEVICE_CONNECT)
		{
		}
		else if(strEvent == DEVICE_DISCONNECT)
		{
		}
	}

	bool CFunctionBar::EnableFunc(QSyncerFramework* pQSyncerFramework,bool bEnable)
	{
		TRACE_FILE_LINE_INFO();

		bool bRes = false;
		for(QSyncerFrameworkMapIter Iter = m_mapFunctions.begin();Iter != m_mapFunctions.end();Iter++)
		{
			if(Iter->second == pQSyncerFramework)
			{
				CWnd* pButton = GetDlgItem(Iter->first);
				if(pButton)
				{
					if(m_nCurrentID == Iter->first)
						m_nCurrentID = -1;//m_nCurrentID is -1 means it need to switch
					if(bEnable && m_nCurrentID == -1)
						ActiveView(Iter->first);
					pButton->EnableWindow(bEnable);
					bRes = true;
				}
				break;
			}
		}
		return bRes;
	}

	bool CFunctionBar::IsSupport(const TSTRING& strID,const CQSetting& oISetting) const
	{
		TRACE_FILE_LINE_INFO();

		bool bRes = false;
		for(int i=0;i<oISetting.Size();i++)
		{
			if(oISetting.GetValue(i) == strID)
			{
				bRes = true;
				break;
			}
		}
		return bRes;
	}

	bool CFunctionBar::GetApplicationMobileConFig(CQSetting& oISetting)
	{		
		TRACE_FILE_LINE_INFO();

		bool bRes = false;
		CQMobileConfigure* pConfigure = GetMobileConFig();
		if(pConfigure)
		{
			if(pConfigure->LoadDefaultXML())
			{
				CQSetting oMobSetting;
				if(pConfigure->GetMobileSettings(_T("DLLViewConfig"), oMobSetting ))
					bRes = pConfigure->GetSettings( _T("Applications"), oISetting);
			}
		}
		return bRes;
	}

}
#pragma warning(pop)