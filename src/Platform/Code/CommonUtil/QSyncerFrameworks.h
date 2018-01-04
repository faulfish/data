///////////////////////////////////////////////////////////
//  QSyncerFrameworks.h
//  Implementation of the Class QSyncerFrame
//  Created on:      01-¤G¤ë-2007 ¤W¤È 11:00:45
//  Original author: Alan Tu
///////////////////////////////////////////////////////////

#if !defined(EA_E9C1402C_E07C_4cfc_AF06_1A82806189B1__INCLUDED_)
#define EA_E9C1402C_E07C_4cfc_AF06_1A82806189B1__INCLUDED_

#include "..\CommonUtil\notifybase.h"

namespace QSYNC_LIB
{

	/*
	Notify comment

	#define in $\CommLayer\MobileFinder\MobileFinderInterface.h
	#define DEVICE_CONNECT		_T("TNotify_Connect")		//parameter is point of DeviceData
	#define DEVICE_DISCONNECT	_T("TNotify_Disconnect")	//parameter is point of DeviceData

	//#define in $\Components\Localization\Ilocalization.h
	//#define TNotify_Language_Change	_T("TNotify_Language_Change")	//no parameter
	*/
#define TNotify_Before_Switch	_T("TNotify_Before_Switch")	//point of SwitchViewPara
#define TNotify_After_Switch	_T("TNotify_After_Switch")	//point of SwitchViewPara
#define TNotify_Close			_T("TNotify_Close")	//point of CloseViewPara

	interface IMainframe 
	{
		virtual bool IsConnected(TSTRING& strName, TSTRING& strIMEI) = 0;
	};

	struct SwitchViewPara
	{
		SwitchViewPara(IMainframe* pIMainframe):m_bCanSwitch(true),m_pIMainframe(pIMainframe){}
		bool m_bCanSwitch;
		IMainframe* m_pIMainframe;
	};

	struct CloseViewPara
	{
		CloseViewPara():m_bCanClose(true){}
		bool m_bCanClose;
	};

	class QSyncerFramework
	{

	public:	
		inline QSyncerFramework(CString strID, CRuntimeClass* pViewClass,INotify* pINotify = NULL)
			: m_strID(strID)
			,m_pViewClass(pViewClass)
			,m_pINotify(pINotify)
		{
		}
		virtual ~QSyncerFramework()
		{
		}
		/**
		* necessary Specific View Class Must derived from CView
		*/
		CRuntimeClass* m_pViewClass;
		/**
		* necessary Specific default display name Has locialization issue
		*/
		CString m_strDisplayName;
		/**
		* not necessary Specific display ICON Can be jpg,png...etc. Has locialization
		* issue
		*/
		CString m_strICONPath;
		/**
		* necessary Specific unique ID Can be GUID...etc.
		* It will be mapped to button
		*
		*const std::wstring strBKImage = m_strID +_T("\\Button\\Btn_Original.png");
		*const std::wstring strHighLightImage = m_strID +_T("\\Button\\Btn_MouseOver.png");
		*const std::wstring strClickImage = m_strID +_T("\\Button\\Btn_MouseDown.png");
		*const std::wstring strDisableImage = m_strID +_T("\\Button\\Btn_Disable.png");
		*const std::wstring strBKImage2 = m_strID +_T("\\Button\\Btn_Original_2.png");
		*const std::wstring strHighLightImage2 = m_strID +_T("\\Button\\Btn_MouseOver_2.png");
		*const std::wstring strClickImage2 = m_strID +_T("\\Button\\Btn_MouseDown_2.png");
		*const std::wstring strDisableImage2 = m_strID +_T("\\Button\\Btn_Disable_2.png");
		*/
		CString m_strID;
		/**
		* not necessary for receiver notify for mainframe
		* issue
		*/
		INotify* m_pINotify;

	};

	typedef UINT ( * LPQSyncerFunc)(QSyncerFramework**);
}
#endif // !defined(EA_E9C1402C_E07C_4cfc_AF06_1A82806189B1__INCLUDED_)
