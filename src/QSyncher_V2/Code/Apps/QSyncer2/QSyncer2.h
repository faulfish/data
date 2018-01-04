/***************************************************************************
*
*                               QSyncer 2.0 Project
*
*	             Copyright (c) 2007 Qisda Corpration
*
***************************************************************************
*
*  $Workfile:   QSyncer2.h  $
*  $Revision:   1.27  $
*  $Date:   Feb 13 2008 08:55:40  $
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
* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/QSyncer2/QSyncer2.h.-arc  $
 * 
 *    Rev 1.27   Feb 13 2008 08:55:40   Eddy Guo
 * fix bug when download file then plug out the usb.
 * CORB1A_Corrs SW_Issue  CORB1A-00137
 * 
 *    Rev 1.26   Feb 01 2008 10:16:28   Alan Tu
 * update
 * 
 *    Rev 1.25   Dec 12 2007 08:43:52   Eddy Guo
 * update
 * 
 *    Rev 1.24   Dec 11 2007 18:19:46   Alan Tu
 * update
 * 
 *    Rev 1.23   Nov 14 2007 09:46:32   Alan Tu
 * Add header
*
*
***************************************************************************/
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols
#include "vector"
#include "..\..\CommonUtil\QSyncerFrameworks.h"
#include "..\..\CommLayer\MobileFinder\MobileFinderInterface.h"
#include "..\..\Components\Localization\Ilocalization.h"
#include "..\..\Components/Configuration/QConfigure/QMobileConfigure.h"


// CQSyncer2App:
// See QSyncer2.cpp for the implementation of this class
//

class CQSyncer2App : public CWinApp, public QSYNC_LIB::INotify, public QSYNC_LIB::CNotifyBase
{
public:
	CQSyncer2App();
	virtual ~CQSyncer2App();

	virtual void Event(const TSTRING& strEvent,long nParam);
	bool CloseAllView();
// Overrides
public:
	virtual BOOL InitInstance();
	size_t GetViewCount();
	bool GetViewByIndex(size_t nIndex,QSYNC_LIB::QSyncerFramework**);	
	void FindAllViews();
	bool SwitchView(QSYNC_LIB::QSyncerFramework* pQSyncerFramework);	
	bool IsConnected(TSTRING& strName, TSTRING& strIMEI);
	CString GetString(const TSTRING& strID){return m_pILocalization->GetString(strID).c_str();}
	void EnableShadowByConfig();

	virtual bool Register(INotify* pINotify,const TSTRING& strEvent){return QSYNC_LIB::CNotifyBase::Register(pINotify,strEvent);}
	virtual bool Unregister(INotify* pINotify,const TSTRING& strEvent){return QSYNC_LIB::CNotifyBase::Unregister(pINotify,strEvent);}

// Implementation
	DECLARE_MESSAGE_MAP()

private:
	void LoadViewFomDll(LPCTSTR strDll);	
	afx_msg void OnAppExit();

public:
    CEvent m_DisconnectEvent;   // if the event is set, disconnect.
	typedef std::vector<QSYNC_LIB::QSyncerFramework*> Frames;
	Frames m_Frames;
	typedef Frames::iterator FramesIter;
	/*
	std::map<QSYNC_LIB::QSyncerFramework*,CMultiDocTemplate*> m_mapDocTemplate;
	typedef std::pair<QSYNC_LIB::QSyncerFramework*,CMultiDocTemplate*> mapDocTemplatePair;
	typedef std::map<QSYNC_LIB::QSyncerFramework*,CMultiDocTemplate*>::iterator mapDocTemplateIter;
	*/

	std::map<QSYNC_LIB::QSyncerFramework*,CWnd*> m_mapView;
	typedef std::pair<QSYNC_LIB::QSyncerFramework*,CWnd*> mapViewPair;
	typedef std::map<QSYNC_LIB::QSyncerFramework*,CWnd*>::iterator mapViewIter;

private:
	bool m_bDeviceConnected;
	CString m_strCOMPort;
	CString m_strModal;
	QSYNC_LIB::ILocalization* m_pILocalization;
	QSYNC_LIB::CQMobileConfigure* m_pConfigure;	
	CMenu m_LangMenu;
	UINT m_nCurrentLangID;
};

extern CQSyncer2App theApp;