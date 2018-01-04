/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   QSyncer2.cpp  $
*  $Revision:   1.86  $
*  $Date:   Mar 27 2008 16:37:32  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/QSyncer2/QSyncer2.cpp.-arc  $
   
      Rev 1.86   Mar 27 2008 16:37:32   Alan Tu
   Set default value
   
      Rev 1.85   Mar 18 2008 09:48:06   Alan Tu
   remove pictures2
   
      Rev 1.84   Mar 18 2008 09:16:14   Alan Tu
   update
   
      Rev 1.83   Mar 03 2008 13:51:34   Alan Tu
   remove log
   
      Rev 1.82   Feb 26 2008 18:49:30   Alan Tu
   update for interface fail
   
      Rev 1.81   Feb 26 2008 10:23:56   Alan Tu
   add assert
   
      Rev 1.80   Feb 13 2008 08:55:42   Eddy Guo
   fix bug when download file then plug out the usb.
   CORB1A_Corrs SW_Issue  CORB1A-00137
   
      Rev 1.79   Feb 04 2008 17:16:10   Jame Tsai
   fix error
   
      Rev 1.78   Feb 01 2008 14:22:14   Alan Tu
   update
   
      Rev 1.77   Feb 01 2008 10:16:28   Alan Tu
   update
   
      Rev 1.76   Jan 31 2008 21:54:52   Alan Tu
   update
   
      Rev 1.75   Jan 31 2008 09:18:02   Alan Tu
   update
   
      Rev 1.74   Jan 29 2008 15:57:00   Alan Tu
   update
   
      Rev 1.73   Jan 23 2008 15:51:14   Mei Yang
   // m_pLM->LoadXML(strPath);
   
      Rev 1.72   Jan 15 2008 16:39:02   Alan Tu
   remove WS_MAXIMIZEBOX 
   
      Rev 1.71   Dec 24 2007 17:47:10   Jame Tsai
   change path
   
      Rev 1.70   Dec 18 2007 16:10:34   Alan Tu
   update
   
      Rev 1.69   Dec 12 2007 08:43:52   Eddy Guo
   update
   
      Rev 1.68   Dec 11 2007 18:19:46   Alan Tu
   update
   
      Rev 1.67   Dec 11 2007 14:08:56   Eddy Guo
   update for main frame using xml
   
      Rev 1.66   Dec 10 2007 21:38:10   Alan Tu
   update
   
      Rev 1.65   Nov 14 2007 09:46:32   Alan Tu
   Add header
*
*
***************************************************************************/

#include "stdafx.h"
#include "QSyncer2.h"
#include "MainFrm.h"

#include "..\..\CommLayer\MobileInterface\Interface_Include\ICommMobile.h"
#include "..\..\CommonUtil\LogPolicy.h"
#include "..\..\CommLayer\CDCInterface\ProtocolManager.h"
#include "..\..\Components\ConfigureTool\IConfigure.h"
#include "../../Components/UIFramework/LayoutManager/Win32Widge.h"
#include "LimitSingleInstance.h"
#include "WndShadow.h"
#include "../../Components/UIFramework/LayoutManager/LayoutManager.h"

using namespace QSYNC_LIB;

CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{{1213C408-B1DE-49c7-9EA3-47E079550FE9}}"));

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(push)
#pragma warning(disable:4312)
#pragma warning(disable:4311)

// CQSyncer2App

BEGIN_MESSAGE_MAP(CQSyncer2App, CWinApp)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_APP_EXIT, &CQSyncer2App::OnAppExit)
END_MESSAGE_MAP()

CQSyncer2App::CQSyncer2App():
m_bDeviceConnected(false),
m_pILocalization(NULL),
m_pConfigure(NULL),
m_DisconnectEvent(FALSE, FALSE, _T("QSyncer2.cpp"))
{
	using namespace QSYNC_LIB;
	DefineNotify(DEVICE_CONNECT);
	DefineNotify(DEVICE_DISCONNECT);
	DefineNotify(TNotify_Language_Change);
	m_pConfigure = GetMobileConFig();
}

CQSyncer2App::~CQSyncer2App()
{
	QSYNC_LIB::ReleaseMobileConFig();
}

// The one and only CQSyncer2App object

CQSyncer2App theApp;

// CQSyncer2App initialization

void CQSyncer2App::FindAllViews()
{
	TRACE_FILE_LINE_INFO();

	WIN32_FIND_DATA FindFileData;
	::ZeroMemory(&FindFileData,sizeof(WIN32_FIND_DATA));
	HANDLE hFind = INVALID_HANDLE_VALUE;
	#ifdef _DEBUG
	TCHAR szDirSpec[MAX_PATH] = {_T("..\\Debug\\view\\*.dll")};
	#else
	TCHAR szDirSpec[MAX_PATH] = {_T("*.dll")};
	#endif

	hFind = ::FindFirstFile(szDirSpec, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE) 
	{
		#ifdef _DEBUG
		LoadViewFomDll(CString(_T("view\\")) + FindFileData.cFileName);
		while (::FindNextFile(hFind, &FindFileData) != 0) 
			LoadViewFomDll(CString(_T("view\\")) + FindFileData.cFileName);
		#else
		LoadViewFomDll(CString(_T("")) + FindFileData.cFileName);
		while (::FindNextFile(hFind, &FindFileData) != 0) 
			LoadViewFomDll(CString(_T("")) + FindFileData.cFileName);
		#endif
		::FindClose(hFind);
	}

}

void CQSyncer2App::LoadViewFomDll(LPCTSTR strDll) 
{
	TRACE_FILE_LINE_INFO();

	using namespace QSYNC_LIB;

	HINSTANCE hDLL		  = NULL;     	  
	hDLL = ::LoadLibrary(strDll);
	if(hDLL)
	{		
		LPQSyncerFunc lpfnDllFunc = (LPQSyncerFunc)::GetProcAddress(hDLL,"GetQSyncerView");
		if (lpfnDllFunc)
		{
			QSyncerFramework* pQSyncerFramework = NULL;
			lpfnDllFunc(&pQSyncerFramework);
			ASSERT(pQSyncerFramework);
			
			if(pQSyncerFramework)
			{
				if(pQSyncerFramework->m_pINotify)
				{
					CNotifyBase::Register(pQSyncerFramework->m_pINotify,DEVICE_CONNECT);
					CNotifyBase::Register(pQSyncerFramework->m_pINotify,DEVICE_DISCONNECT);
				}
				m_Frames.push_back(pQSyncerFramework);
			}
		}
		else
		{
			::FreeLibrary(hDLL);
		}
	}
}

size_t CQSyncer2App::GetViewCount()
{
	TRACE_FILE_LINE_INFO();

	return m_Frames.size();
}

bool CQSyncer2App::GetViewByIndex(size_t nIndex,QSYNC_LIB::QSyncerFramework** ppQSyncerFramework)
{
	TRACE_FILE_LINE_INFO();

	bool bRes = false;
	if(nIndex >= 0 && nIndex < m_Frames.size())
	{
		FramesIter Iter = m_Frames.begin();
		advance(Iter,nIndex);
		*ppQSyncerFramework = *Iter;
		bRes  = true;
	}	
	return bRes;
}

/******************************************************************************************/
/*

*/
/******************************************************************************************/
bool CQSyncer2App::SwitchView(QSYNC_LIB::QSyncerFramework* pQSyncerFramework)
{	
	TRACE_FILE_LINE_INFO();

	using namespace QSYNC_LIB;

	bool bRes = false;
	CWnd* pView = NULL;

	mapViewIter Iter = m_mapView.find(pQSyncerFramework);
	if(Iter == m_mapView.end())
	{	
		/******************************************************************************************/
		// Note: can be a CWnd with PostNcDestroy self cleanup
		pView = (CWnd*)pQSyncerFramework->m_pViewClass->CreateObject();
		if (pView == NULL)
		{
			return NULL;
		}
		ASSERT_KINDOF(CWnd, pView);

		// views are always created with a border!
		if (!pView->Create(NULL, NULL, WS_CHILDWINDOW  | WS_VISIBLE ,	CRect(0,0,0,0), GetMainWnd(), AFX_IDW_PANE_FIRST))
		{
			TRACE(traceAppMsg, 0, "Warning: could not create view for frame.\n");
			return NULL;        // can't continue without a view
		}

		m_mapView.insert(mapViewPair(pQSyncerFramework,pView));

		CView *pCVew = static_cast<CView*>(pView);
		if(pCVew)
			pCVew->OnInitialUpdate();

		CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
		if(pMainFrame)
		{
			pMainFrame->AddWidget(pQSyncerFramework,new QSYNC_LIB::WIN32WindowWidge(pView));
			pMainFrame->SwitchWidget(pQSyncerFramework);
		}

		if (pView->GetExStyle() & WS_EX_CLIENTEDGE)
		{
			// remove the 3d style from the frame, since the view is
			//  providing it.
			// make sure to recalc the non-client area
			GetMainWnd()->ModifyStyleEx(WS_EX_CLIENTEDGE, 0, SWP_FRAMECHANGED);
		}
		/*******************************************************************************************/
	}
	else
	{
		pView = Iter->second;
		CMainFrame* pMainFrame = (CMainFrame*)GetMainWnd();
		if(pMainFrame)
			pMainFrame->SwitchWidget(pQSyncerFramework);
	}
	if(pView && ::IsWindow(pView->GetSafeHwnd()) && ::IsWindow(pView->GetParent()->GetSafeHwnd()))
		bRes = true;

	return bRes;
}

bool CQSyncer2App::IsConnected(TSTRING& strName, TSTRING& strIMEI)
{
	TRACE_FILE_LINE_INFO();

	if(m_bDeviceConnected)
	{	
		//取得手機的info, 包含Model name, IMEI
		TCHAR szMobileModel[512] = {0};
		TCHAR szIMEI[512] = {0};
		TCHAR szDisplayName[512] = {0};

		CommGetMobileInfo(_T("CQSyncer2App"), szMobileModel, szIMEI, szDisplayName);
		strIMEI = szIMEI;
		strName = szMobileModel;
	}
	return m_bDeviceConnected;
}

void CQSyncer2App::Event(const TSTRING& strEvent,long nParam)
{
	TRACE_FILE_LINE_INFO();

	using namespace QSYNC_LIB;
	if(strEvent == DEVICE_CONNECT)
	{
        m_DisconnectEvent.ResetEvent();
		TRACE(_T("%s \n"),strEvent.c_str());
		DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
		if(m_bDeviceConnected == false)
		{
			m_strCOMPort = (*pspDeviceData)->ID().c_str();
			m_strModal = (*pspDeviceData)->GetExt().c_str();

			int nReturnIndex =1;
			m_bDeviceConnected = (eCommRetSuccess == ::CommConnectMobile( _T("QSyncer"), eCommSerial, m_strCOMPort, m_strModal,nReturnIndex));
			ASSERT(m_bDeviceConnected);
			if(m_bDeviceConnected && m_pILocalization)
			{
				CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
				pMainFrame->SetStatusText(_T("IDS_CONNECTED"));
				Fire(strEvent,nParam);
			}
		}
	}
	else if(strEvent == DEVICE_DISCONNECT)
	{
        m_DisconnectEvent.SetEvent();
		TRACE(_T("%s \n"),strEvent.c_str());
		DeviceDataPt* pspDeviceData = (QSYNC_LIB::DeviceDataPt*)nParam;
		if(m_bDeviceConnected)
		{
			if(eCommRetSuccess == ::CommDisConnectMobile(_T("QSyncer"), eCommSerial, m_strCOMPort, m_strModal))
			{				
				CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
				pMainFrame->SetStatusText(_T("IDS_DISCONNECTED"));
				m_bDeviceConnected = false;
				m_strCOMPort = _T("");
				m_strModal = _T("");
				Fire(strEvent,nParam);
			}
			else assert(0);
		}
	}
	else if(strEvent == TNotify_Language_Change)
	{
		Fire(TNotify_Language_Change);
	}
}

void CQSyncer2App::EnableShadowByConfig()
{	
	TRACE_FILE_LINE_INFO();

	CLayoutManager LM;
	XMLMANAGER::XMLElement elm;
	LM.GetElementByID(elm, _T("MainFrameID"));
	if(elm.getAttr(_T("EnableShadow")))
		CWndShadow::Initialize(AfxGetInstanceHandle());
}

BOOL CQSyncer2App::InitInstance()
{
	TRACE_FILE_LINE_INFO();

	CWinApp::InitInstance();
    if(g_SingleInstanceObj.IsAnotherInstanceRunning())
	{
		TRACE_FILE_LINE_INFO();
        return FALSE; 
	}

	EnableShadowByConfig();

	using namespace QSYNC_LIB;
	CreateILocalization(&m_pILocalization);
	m_pILocalization->Register(this,TNotify_Language_Change);
	GetDeviceFinder()->Register(this);	

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		TRACE_FILE_LINE_INFO();
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	pMainFrame->SetStatusText(_T("IDS_DISCONNECTED"));

	SetWindowLong( pMainFrame->GetSafeHwnd(), GWL_STYLE, GetWindowLong(pMainFrame->GetSafeHwnd(), GWL_STYLE) & ~WS_MAXIMIZE & ~WS_MAXIMIZEBOX );	
	
	m_nCurrentLangID = 0;

	//#define _SIEMEMS_A31
	#ifdef _SIEMEMS_A31
	DeviceData deviceData(DBT_DEVTYP_PORT);
	deviceData.m_strPort = _T("COM1");
	deviceData.m_strExtName = _T("A31C");
	DeviceDataPt ptDeviceData = &deviceData;
	Event(DEVICE_CONNECT,(long)&ptDeviceData);
	#endif	

	return TRUE;
}

// CQSyncer2App message handlers


void CQSyncer2App::OnAppExit()
{
	TRACE_FILE_LINE_INFO();
	// TODO: Add your command handler code here
}

bool CQSyncer2App::CloseAllView()
{
	TRACE_FILE_LINE_INFO();

	bool bRes = true;
	using namespace QSYNC_LIB;
	size_t nViewCount = GetViewCount();
	for(size_t nIndex = 0;nIndex < nViewCount;nIndex++)
	{
		QSyncerFramework *pQSyncerFramework = NULL;
		if(GetViewByIndex(nIndex,&pQSyncerFramework))
		{
			if(pQSyncerFramework->m_pINotify)
			{
				CloseViewPara closeViewPara;
				pQSyncerFramework->m_pINotify->Event(TNotify_Close,(long)&closeViewPara);
				if(closeViewPara.m_bCanClose == false)
				{
					CString strDisplay;
					strDisplay = GetString(_T("IDS_CLOSE_VIEW_FAIL"));
					strDisplay += GetString(pQSyncerFramework->m_strDisplayName.GetBuffer(0));
					CMainFrame* pMainFrame = (CMainFrame*)::AfxGetMainWnd();
					pMainFrame->SetStatusText(strDisplay);
					bRes = false;
					break;
				}
			}
		}
	}

	if(bRes)
	{
		CDCExist();
		DeleteDeviceFinder();
	}

	return bRes;
}


#pragma warning(pop)