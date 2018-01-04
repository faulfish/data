/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   FunctionBar.h  $
*  $Revision:   1.19  $
*  $Date:   Dec 25 2007 10:10:02  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/QSyncer2/FunctionBar.h.-arc  $
 * 
 *    Rev 1.19   Dec 25 2007 10:10:02   Jame Tsai
 * fix toggle setting
 * 
 *    Rev 1.18   Dec 13 2007 20:48:38   Alan Tu
 * update
 * 
 *    Rev 1.17   Nov 14 2007 09:54:12   Alan Tu
 * update
*
*
***************************************************************************/
#pragma once

#include <vector>
#include "QSyncer2.h"
#include "..\..\CommonUtil\notifybase.h"
#include "..\..\Components\Localization\Ilocalization.h"
#include "..\..\Components\UIFramework\Porting\xSkinButton.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\Components\Configuration\QConfigure\QMobileConfigure.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "../../Components/ConfigureTool/IConfigure.h"
#include "..\..\Components\UIFramework\LayoutManager\WIN32IconButtonWidgeVer2.h"

namespace QSYNC_LIB
{
	class CFunctionBar : public Win32DoubleBufferDialogWidge  , public CNotifyBase, public INotify
	{
		DECLARE_DYNAMIC(CFunctionBar)

	public:
		CFunctionBar(IMainframe *pIMainframe,CWnd* pParent = NULL);   // standard constructor
		virtual ~CFunctionBar();
		virtual  BOOL OnInitDialog( );

		void AddViewButtonToBar( CQSetting &oISetting, QSyncerFramework* pQSyncerFramework, std::wstring strImagePrefix );
		virtual void Event(const TSTRING& strEvent,long nParam);

		// Dialog Data
		enum { IDD = IDD_FUNCTION_DLG };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnDestroy();
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg LRESULT ChangeAlignment( WPARAM wParam, LPARAM lParam);
		static void FunctionButtonClicked(UINT nID);
		void OnFunctionButtonClicked(UINT nID);
		void FindViews();

	protected:
		bool ActiveView(UINT nID);
		bool UnactiveView(UINT nID);
		//bool GetApplicationMobileConFig(const TSTRING& strModel,CQSetting& oMobSetting);
		bool GetApplicationMobileConFig(CQSetting& oMobSetting);
		bool IsSupport(const TSTRING& strID,const CQSetting& oISetting) const;
		bool EnableFunc(QSyncerFramework* pQSyncerFramework,bool bEnable);	
		CString GetString(const TSTRING& strID){return m_pILocalization->GetString(strID).c_str();}
		BOOL IsHorz() const;

	private:
		typedef std::map<UINT,QSyncerFramework*>				QSyncerFrameworkMap;
		typedef std::map<UINT,QSyncerFramework*>::iterator	QSyncerFrameworkMapIter;
		QSyncerFrameworkMap m_mapFunctions;
		typedef std::vector<CxSkinButton*> ButtonArray;
		ButtonArray m_FunctionButtons;
		bool m_bInit;
		UINT m_nDockBarID;
		UINT m_nCurrentID;
		CQSyncer2App* m_pApp;
		ILocalization*		m_pILocalization;
		IMainframe* m_pIMainframe;
		IConfigure* m_pCfgTool;
	private:
		BoxLayout<HDC>* m_pLayout;
		std::map<UINT,WIN32IconButton*>                      m_mapFunctionButton;
	};
}