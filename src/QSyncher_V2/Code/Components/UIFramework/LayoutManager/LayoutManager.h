/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                               3G Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   LayoutManager.h  $
 *  $Revision:   1.29  $
 *  $Date:   Mar 18 2008 10:11:10  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Components/UIFramework/LayoutManager/LayoutManager.h.-arc  $
 * 
 *    Rev 1.29   Mar 18 2008 10:11:10   Alan Tu
 * refactory
 * 
 *    Rev 1.28   Mar 18 2008 09:47:26   Alan Tu
 * add getPath
 * 
 *    Rev 1.27   Mar 18 2008 09:26:46   Alan Tu
 * update
 * 
 *    Rev 1.26   Mar 11 2008 14:08:28   Alan Tu
 * remove remark
 * 
 *    Rev 1.25   Feb 27 2008 13:24:28   Mei Yang
 * add RemoveOrgElements
 * 
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file LayoutManager.h
    @brief Manage Widgets and provide functions of Save and Load.
*/
#ifndef _LayoutManager_Header_
#define _LayoutManager_Header_
#pragma once

#include <map>
#include "..\..\..\CommonUtil\XMLManager.h"
#include "..\..\..\CommonUtil\MSXMLImpl.h"
#include "..\..\..\Components\UIFramework\LayoutManager\TWidge.h"
#include "..\..\..\Components\UIFramework\LayoutManager\LayoutBase.h"
#include "..\..\..\Components\UIFramework\LayoutManager\BoxLayout.h"
#include "..\..\..\Components\UIFramework\LayoutManager\GridLayout.h"
#include "..\..\..\Components\UIFramework\LayoutManager\BasicLayout.h"
#include "..\..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
#include "..\..\..\Components\UIFramework\LayoutManager\StackLayout.h"
#include "..\..\..\Components\UIFramework\LayoutManager\WIN32IconButtonWidgeVer2.h"
#include "..\..\..\Components\UIFramework\LayoutManager\Win32ProgressBarWidge.h"

#include "../CommUIExportAPI.h"
#include "Winuser.h"
namespace QSYNC_LIB
{	
#ifdef _DEBUG
	const TSTRING g_strDeafultPath = _T("..\\Debug\\Pictures2\\");
#else
	const TSTRING g_strDeafultPath = _T("Pictures2\\");
#endif

	const TSTRING strQXML = _T("QLayoutManager.xml");

	class COMMONUI_API CLayoutManager
	{
	public:
		CLayoutManager(CWnd* pThis, INotify* pINotify);
		CLayoutManager(const TSTRING& strFileName = g_strDeafultPath + strQXML);
		virtual ~CLayoutManager();

		bool Save(Widge<HDC>* pWidget, const TSTRING& strClsName, const TSTRING& strWidgetID, const TSTRING& strFilePath = g_strDeafultPath + strQXML);
		
		Widge<HDC>* Load(const TSTRING& strWidgetID,const TSTRING& strFileName = g_strDeafultPath + strQXML);
		Widge<HDC>* GetWidgetByID(const TSTRING& strWidgetID);
        bool GetElementByID(XMLMANAGER::XMLElement& Element, const TSTRING& strID);
		const TSTRING& GetPath() const {return m_strFileName;}
	private:
		//Save
		bool SaveToXML(const XMLElement& Element, const TSTRING& strFilePath = g_strDeafultPath + strQXML);
		XMLElement InitEleRoot(); 
		XMLElement InitEleOrder(const TSTRING& strClsName, const TSTRING& strWidgetID);
		bool SaveWidgets(Widge<HDC>* pWidget, XMLElement& Element, XMLElement& EleOrder);
		bool RemoveOrgElements(const XMLElement& Element);

		//Load
		bool LoadXML(const TSTRING& strFileName);
		bool RecursiveFind(const XMLMANAGER::XMLElement& Element, const TSTRING& strID, XMLMANAGER::XMLElement& endElement);

		bool MakeWidgets(const XMLMANAGER::XMLElement& FrameElement);
		Widge<HDC>* SetOrder(const XMLMANAGER::XMLElement& FrameElement);
		Widge<HDC>* SetOrderSetting(const XMLMANAGER::XMLAllElement* pSettingElement); // for SetOrder
		bool SetLayoutBase(const XMLMANAGER::XMLElement& FrameElement);
		bool SetLayoutBaseSetting(const XMLMANAGER::XMLElement& FrameElement);

		bool NewWidget(const TSTRING& strClassName,Widge<HDC>** pWidget, const XMLMANAGER::XMLElement& Element);
		template<class Layout>
		Widge<HDC>* NewLayout(XMLMANAGER::XMLAllElement* pWidgetProperty)
		{
			Layout* pResWidget = NULL;
			XMLElement Element;
			bool bGetProperty = false;
			for(XMLAllElement::iterator iter=pWidgetProperty->begin(); iter!=pWidgetProperty->end(); ++iter)
			{
				if(iter->name() == g_strProperty)
				{	
					Element = (*iter);
					bGetProperty = true;
				}
			}
			pResWidget = new Layout;
			if(pResWidget && bGetProperty)
			{
				bool bLoad = pResWidget->Load(Element);
				assert(bLoad);
			}
			return pResWidget;
		}
		template<class Win32Widget>
		Widge<HDC>* NewWin32Widget(XMLMANAGER::XMLAllElement* pWidgetProperty)
		{
			Win32Widget* pResWidget = NULL;
			XMLElement Element;
			bool bGetProperty = false;
			for(XMLAllElement::iterator iter=pWidgetProperty->begin(); iter!=pWidgetProperty->end(); ++iter)
			{
				if(iter->name() == g_strProperty)
				{	
					Element = (*iter);
					bGetProperty = true;
				}
			}
			pResWidget = new Win32Widget(m_pWnd, m_pINotify);
			if(pResWidget && bGetProperty)
			{
				bool bLoad = pResWidget->Load(Element);
				assert(bLoad);
			}
			return pResWidget;
		}
	private:
		MSXMLLibrary								m_qXMLlib;
		XMLMANAGER::XMLDocument						m_qXMLdoc;
		XMLMANAGER::XMLElement						m_EleRoot;  // Root element
		XMLMANAGER::XMLAllElement					m_AllFrame; // flexibility for save a lot of FrameElement 
		
		map<TSTRING, Widge<HDC>*>					m_pWidgetMap; // for Load
		TSTRING										m_strFileName;
		bool										m_bLoad;
		CWnd*										m_pWnd;
		INotify*									m_pINotify;
	};
}; //namespace QSYNC_LIB
 
#endif  //#ifndef _LayoutManager_Header_