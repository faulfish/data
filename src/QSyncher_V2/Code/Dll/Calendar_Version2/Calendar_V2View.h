// Calendar_V2View.h : interface of the CCalendar_V2View class
//
#pragma once

#include "resource.h"
#include "QTabCtrl.h"
#include "AppointmentEditor.h"
#include "..\..\CommonUtil\QSyncerFrameworks.h"
#include "../../CommLayer/MobileInterface/Interface_Include/ICommMobile.h"
#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"
#include "..\..\Components\UIFramework\LayoutManager\BasicLayout.h"
#include "ListPage.h"
#include "afxcmn.h"


//#define CPS_MEDIA_TAB_TL_CX				0
//#define CPS_MEDIA_TAB_TL_CY				25

#ifdef CalendarView_EXPORT	
		#define AFX_EXT_CLASS       AFX_CLASS_EXPORT
		#define AFX_EXT_API         AFX_API_EXPORT
#else
		#define AFX_EXT_CLASS       AFX_CLASS_IMPORT
		#define AFX_EXT_API         AFX_API_IMPORT
#endif

class NotifyReceiver : public QSYNC_LIB::INotify
{
public:
	NotifyReceiver():m_pINotify(NULL){}
	virtual void Event(const TSTRING& strEvent,long nParam)
	{
		if(m_pINotify)
			m_pINotify->Event(strEvent,nParam);
	}
	QSYNC_LIB::INotify* m_pINotify;
};

class CCalendar_V2View : public Win32DoubleBufferFormViewWidge , public QSYNC_LIB::INotify
{
public:
	enum{ IDD = IDD_CALENDAR_V2_FORM };
	bool m_bSwitchView;
public:	
	virtual void Event(const TSTRING& strEvent,long nParam);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	CCalendar_V2View();
	virtual ~CCalendar_V2View();
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

	//afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	BOOL							m_bInit;
	int								m_nRightCx;
	CListPage*						m_pPage;
	bool								m_bShow;
	bool								m_bConnected;
 
protected:
	// create from serialization only
	DECLARE_DYNCREATE(CCalendar_V2View)

	//void InitControlSize();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
private:
	QSYNC_LIB::BasicLayout<HDC>* m_pBasicLayout;
	
};
