/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                               3G Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   LayoutManager.cpp  $
 *  $Revision:   1.3  $
 *  $Date:   Apr 01 2009 14:17:24  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Components/UIFramework/LayoutManager/LayoutManager.cpp.-arc  $
   
      Rev 1.3   Apr 01 2009 14:17:24   Kevin Tai
   no change
   
      Rev 1.2   Jun 25 2008 09:52:52   Jame Tsai
   Update to the latest version of this is QSyncer
   
      Rev 1.28   May 13 2008 14:14:48   Jame Tsai
   Refactory XML layout of UI
   
      Rev 1.27   Mar 18 2008 10:11:10   Alan Tu
   refactory
   
      Rev 1.26   Feb 27 2008 13:24:28   Mei Yang
   add RemoveOrgElements
   
      Rev 1.25   Feb 26 2008 17:34:58   Mei Yang
   fix Initial OrderSetting
   
      Rev 1.24   Feb 26 2008 17:10:52   Mei Yang
   update SaveToXML
   
      Rev 1.23   Feb 25 2008 22:05:42   Mei Yang
   update for Save
   
      Rev 1.22   Jan 31 2008 14:43:00   Mei Yang
   add INotify
   
      Rev 1.21   Jan 31 2008 13:47:06   Mei Yang
   update
   
      Rev 1.20   Jan 31 2008 09:49:24   Mei Yang
   fix Alignment bug
   
      Rev 1.19   Jan 30 2008 16:45:06   Mei Yang
   add assert
   
      Rev 1.18   Jan 30 2008 11:16:08   Mei Yang
   give assert to check ID error
   
      Rev 1.17   Jan 30 2008 10:41:48   Mei Yang
   add GetWidgetByID
   
      Rev 1.16   Jan 30 2008 09:12:26   Mei Yang
   fix SetOrderSetting bug
   
      Rev 1.15   Jan 29 2008 10:09:00   Mei Yang
   add Min/Max Height/Width
   
      Rev 1.14   Jan 25 2008 15:10:50   Mei Yang
   update 
   
      Rev 1.13   Jan 25 2008 13:45:12   Mei Yang
   add SetLayoutBase
   
      Rev 1.12   Jan 24 2008 17:36:50   Mei Yang
   fix error
   
      Rev 1.11   Jan 24 2008 17:33:16   Mei Yang
   update
   
      Rev 1.10   Jan 24 2008 17:15:22   Mei Yang
   add NewWin32Widget instead of all New Widget functions
   
      Rev 1.9   Jan 24 2008 14:24:54   Mei Yang
   add NewLayout and remove all New Layout function
   
      Rev 1.8   Jan 23 2008 15:52:00   Mei Yang
   new version
   
      Rev 1.7   Nov 30 2007 21:08:30   Mei Yang
   fix error!! 
   
      Rev 1.6   Nov 30 2007 20:47:04   Mei Yang
   update
   
      Rev 1.5   Nov 28 2007 19:31:44   Mei Yang
   temporarily save this file
   
      Rev 1.4   Nov 20 2007 14:16:20   Mei Yang
   update
   
      Rev 1.3   Nov 16 2007 17:07:10   Mei Yang
   update
   
      Rev 1.2   Nov 16 2007 16:56:06   Mei Yang
   Update functions
   
      Rev 1.1   Nov 15 2007 17:56:02   Mei Yang
   temporarily update
   
      Rev 1.0   Nov 14 2007 14:19:44   Alan Tu
   Initial revision.
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file LayoutManager.cpp
    @brief {brief description of LayoutManager.cpp}

    {detail description of LayoutManager.cpp}
*/
#pragma once
#include "stdafx.h"
#include "LayoutManager.h"


namespace QSYNC_LIB
{
	CLayoutManager::CLayoutManager(CWnd* pThis, INotify* pINotify, const TSTRING& strFileName)
		:m_qXMLdoc(&m_qXMLlib)
		,m_strFileName(g_strDeafultPath + strFileName)
		,m_bLoad(false)
		,m_pWnd(pThis)
		,m_pINotify(pINotify)
	{ 
		LoadXML(m_strFileName);
	}
	CLayoutManager::CLayoutManager(const TSTRING& strFileName)
		:m_qXMLdoc(&m_qXMLlib)
		,m_strFileName(strFileName)
		,m_bLoad(false)
		,m_pWnd(NULL)
		,m_pINotify(NULL)
	{
		LoadXML(m_strFileName);
	}

	CLayoutManager::~CLayoutManager()
	{
		if(!m_pWidgetMap.empty())
		{
			m_pWidgetMap.clear();
		}
	}
	
	
	bool CLayoutManager::LoadXML(const TSTRING& strFileName)
	{
		if(!strFileName.empty())
			m_bLoad = m_qXMLdoc.load(strFileName);
		if(m_bLoad && m_qXMLdoc.getAllElement()->size() > 0)
		{
			m_EleRoot = m_qXMLdoc.getAllElement()->at(0);
			m_AllFrame = *(m_EleRoot.getAllElement());
		}
		return m_bLoad;
	}

	bool CLayoutManager::GetElementByID(XMLMANAGER::XMLElement& Element, const TSTRING& strID)
	{
		using namespace XMLMANAGER;
		bool bRes = false;
		if(m_bLoad)
		{
			for(XMLAllElement::iterator iter = m_AllFrame.begin(); iter != m_AllFrame.end() && !bRes; ++iter)
			{
				if(iter->name() == g_strWidget)
				{
					TSTRING strIterID;
					iter->getAttr(g_strID, strIterID);
					if(strID == strIterID)
					{
						Element = (*iter);
						bRes = true;
						break;
					}
					else
					{
						bRes = RecursiveFind((*iter), strID, Element);
					}
				}
				else
					bRes = RecursiveFind((*iter), strID, Element);
			}
		}
		return bRes;
	}
	
	bool CLayoutManager::RecursiveFind(const XMLMANAGER::XMLElement& Element,const TSTRING& strID, XMLMANAGER::XMLElement& endElement)
	{
		using namespace XMLMANAGER;
		bool bFind = false;
		
		for(XMLAllElement::const_iterator iter = Element.getAllElement()->begin(); iter != Element.getAllElement()->end() && !bFind; ++iter)
		{
			if(Element.name() == g_strWidget)
			{
				TSTRING strIterID;
				iter->getAttr(g_strID, strIterID);
				if(strID == strIterID)
				{
					endElement = (*iter);
					bFind = true;
				}
				else
				{
					bFind = RecursiveFind((*iter), strID, endElement);
				}
			}
		}
		return bFind;
	}

	Widge<HDC>* CLayoutManager::Load(const TSTRING& strWidgetID,const TSTRING& strFileName)
	{
		using namespace XMLMANAGER;
		Widge<HDC>* pResWidget = NULL;

		XMLElement FrameElement;
		if(GetElementByID(FrameElement, strWidgetID))
		{
			MakeWidgets(FrameElement);
			pResWidget = SetOrder(FrameElement);
			SetLayoutBase(FrameElement);
		}
		if(pResWidget)
			m_pWidgetMap[strWidgetID] = pResWidget;
		return pResWidget;
	}
	bool CLayoutManager::MakeWidgets(const XMLMANAGER::XMLElement& FrameElement)
	{
		using namespace XMLMANAGER;
		bool bRes = true;
		
		XMLAllElement* pAllElement = (XMLAllElement*)FrameElement.getAllElement();
		// Create Widgets
		for(XMLAllElement::iterator iter= pAllElement->begin(); iter!= pAllElement->end(); ++iter)
		{
			TSTRING strClassName, strID;
			if(iter->name() == _T("Widget"))
			{
				iter->getAttr(_T("ClassName"), strClassName);
				iter->getAttr(_T("ID"), strID);
				Widge<HDC>* pWidget = NULL;
				if(NewWidget(strClassName, &pWidget, (*iter)) && pWidget)
					m_pWidgetMap[strID] = pWidget;
			}
		}
		return bRes;
	}
	bool CLayoutManager::NewWidget(const TSTRING& strClassName, Widge<HDC>** pWidget, const XMLMANAGER::XMLElement& Element)
	{
		bool bRes = true;
		
		XMLMANAGER::XMLAllElement* pWidgetProperty = (XMLMANAGER::XMLAllElement*)Element.getAllElement();
		if	   (strClassName == _T("")){ bRes = false;}
		else if(strClassName == _T("BasicLayout"))		{*pWidget = NewLayout<BasicLayout<HDC>>(pWidgetProperty);}
		else if(strClassName == _T("BoxLayout"))		{*pWidget = NewLayout<BoxLayout<HDC>>(pWidgetProperty);}
		else if(strClassName == _T("GridLayout"))		{*pWidget = NewLayout<GridLayout<HDC>>(pWidgetProperty);}				
		else if(strClassName == _T("SplitLayout"))		{*pWidget = NewLayout<SplitLayout<HDC>>(pWidgetProperty);}
		else if(strClassName == _T("TStrStackLayout"))	{*pWidget = NewLayout<TStrStackLayout<HDC>>(pWidgetProperty);}	
		else if(strClassName == _T("Win32Button")) 		{*pWidget = NewWin32Widget<WIN32ButtonWidge>(pWidgetProperty);}
		else if(strClassName == _T("Win32Image"))  		{*pWidget = NewWin32Widget<WIN32DrawerWidge>(pWidgetProperty);}
		else if(strClassName == _T("Win32Label"))  		{*pWidget = NewWin32Widget<WIN32LabelWidge>(pWidgetProperty);}
		else if(strClassName == _T("Win32Windows"))		{*pWidget = NewWin32Widget<WIN32WindowWidge>(pWidgetProperty);}
		else if(strClassName == _T("Win32IconButton"))	{*pWidget = NewWin32Widget<WIN32IconButton>(pWidgetProperty);}
		else if(strClassName == _T("Win32ProgressBar"))	{*pWidget = NewWin32Widget<Win32ProgressBarWidge>(pWidgetProperty);}
		else // User Defined
		{
			TSTRING strID;
			Element.getAttr(_T("ID"), strID);
			*pWidget = Load(strID);
		} 
		
		bRes = (*pWidget) != NULL;
		assert( (*pWidget) != NULL );
		return bRes;
	}

	Widge<HDC>* CLayoutManager::SetOrder(const XMLMANAGER::XMLElement& FrameElement)
	{
		using namespace XMLMANAGER;
		Widge<HDC>* pWidget = NULL;
		XMLAllElement* pAllSetting = NULL;
		XMLAllElement* pAllElement = (XMLAllElement*)FrameElement.getAllElement();
		bool bSet = false;
		for(XMLAllElement::iterator iter= pAllElement->begin(); iter!= pAllElement->end(); ++iter)
		{
			if(iter->name() == _T("Order"))
			{
				pAllSetting = iter->getAllElement();
				bSet = true;
				break;
			}
		}
		if(!bSet && !m_pWidgetMap.empty())
			pWidget = m_pWidgetMap.begin()->second;
		// Set Order
		if(bSet && pAllSetting)
			pWidget = SetOrderSetting(pAllSetting);
		
		return pWidget;
	}
	Widge<HDC>* CLayoutManager::SetOrderSetting(const XMLMANAGER::XMLAllElement* pSettingAllElement)
	{
		using namespace XMLMANAGER;
		Widge<HDC>* pWidget = NULL;
		XMLAllElement* pSetting = (XMLAllElement*)pSettingAllElement;
		for(XMLAllElement::iterator iter= pSetting->begin(); iter!= pSetting->end(); ++iter)
		{
			Widge<HDC>* pTmpWidget = NULL;
			if(iter->name() == _T("Setting"))
			{
				TSTRING strParentWidget, strID, strParameter; 
				iter->getAttr(_T("ParentWidget"), strParentWidget);
				iter->getAttr(_T("ID"), strID);
				iter->getAttr(_T("Parameter"), strParameter);
				
				if((strParentWidget == _T("") && m_pWidgetMap.find(strID) != m_pWidgetMap.end()) ||
					(m_pWidgetMap.find(strParentWidget) != m_pWidgetMap.end() && m_pWidgetMap.find(strID) != m_pWidgetMap.end()))
				{
					if(strParentWidget !=_T(""))
					{
						bool bAdd = ((LayoutBaseWidge<HDC>*)m_pWidgetMap[strParentWidget])->AddWidget(m_pWidgetMap[strID], strParameter);
						assert(bAdd);
					}
					else
						pWidget = m_pWidgetMap[strID];
				}
				else
				{
					assert( m_pWidgetMap.find(strParentWidget) != m_pWidgetMap.end()  );
					assert(m_pWidgetMap.find(strID) != m_pWidgetMap.end());
					break;
				}
			}
		}
		return pWidget;
	}
	bool CLayoutManager::SetLayoutBase(const XMLMANAGER::XMLElement& FrameElement)
	{
		using namespace XMLMANAGER;
		bool bRes = true;
		XMLAllElement* pAllSetting = NULL;
		XMLAllElement* pAllElement = (XMLAllElement*)FrameElement.getAllElement();
		bool bSet = false;
		for(XMLAllElement::iterator iter= pAllElement->begin(); iter!= pAllElement->end(); ++iter)
		{
			if(iter->name() == _T("LayoutBase"))
			{
				pAllSetting = iter->getAllElement();
				bSet = true;
				break;
			}
		}
		for(XMLAllElement::iterator iter= pAllSetting->begin(); iter!= pAllSetting->end() && bRes; ++iter)
		{
			if(iter->name() == _T("Setting"))
			{
				bRes = SetLayoutBaseSetting((*iter));
			}
		}
		return bRes;
	}
	bool CLayoutManager::SetLayoutBaseSetting(const XMLMANAGER::XMLElement& Element)
	{
		bool bRes = true;
	
		TSTRING strID;
		Element.getAttr(_T("ID"),strID);
		LayoutBaseWidge<HDC>* pWidget = NULL;
		pWidget = (LayoutBaseWidge<HDC>*)m_pWidgetMap[strID];
		if(pWidget)
		{
			int nIndex = Element.getAttr(_T("Index"));
			LayoutBaseSettings Setting;
			int nTopMargin = Element.getAttr(_T("TopMargin"));
			Setting.nTopMargin = nTopMargin==-1 ? 0 : nTopMargin;
			int nBottomMargin = Element.getAttr(_T("BottomMargin"));
			Setting.nBottomMargin = nBottomMargin==-1 ? 0 : nBottomMargin;
			int nLeftMargin = Element.getAttr(_T("LeftMargin"));
			Setting.nLeftMargin = nLeftMargin==-1 ? 0 : nLeftMargin;
			int nRightMargin = Element.getAttr(_T("RightMargin"));
			Setting.nRightMargin = nRightMargin==-1 ? 0 : nRightMargin;
			
			Setting.nMinWidth = Element.getAttr(_T("MinWidth"));
			Setting.nMinHeight = Element.getAttr(_T("MinHeight"));
			Setting.nMaxWidth = Element.getAttr(_T("MaxWidth"));
			Setting.nMaxHeight = Element.getAttr(_T("MaxHeight"));

			TSTRING strAlignment;
			Element.getAttr(_T("Alignment"), strAlignment);
			if(strAlignment != _T(""))
			{
				vector<TSTRING> vstrAlignment;
				Tokenize<TSTRING>(_T("|"), strAlignment,vstrAlignment,false);
				Setting.nAlignment = 0;
				for(vector<TSTRING>::iterator iter=vstrAlignment.begin(); iter!= vstrAlignment.end(); ++iter)
				{
					if((*iter) == _T("Left"))
						Setting.nAlignment |= Left;
					else if((*iter) == _T("HCenter"))
						Setting.nAlignment |= HCenter;
					else if((*iter) == _T("Right"))
						Setting.nAlignment |= Right;
					else if((*iter) == _T("Top"))
						Setting.nAlignment |= Top;
					else if((*iter) == _T("VCenter"))
						Setting.nAlignment |= VCenter;
					else if((*iter) == _T("Bottom"))
						Setting.nAlignment |= Bottom;
				}
			}
			else
				Setting.nAlignment = Default;

			pWidget->SetLayoutBase(nIndex,Setting);
		}
		else
		{
			bRes = false;
		}
		return bRes;
	}
	
	Widge<HDC>* CLayoutManager::GetWidgetByID(const TSTRING& strWidgetID)
	{
		Widge<HDC>* pWidget = NULL;
		if(m_pWidgetMap.find(strWidgetID) != m_pWidgetMap.end())
			pWidget = m_pWidgetMap[strWidgetID];
		return pWidget;
	}
	
	bool CLayoutManager::Save(Widge<HDC>* pWidget, const TSTRING& strClsName, const TSTRING& strWidgetID, const TSTRING& strFilePath)
	{
		bool bRes = true;
		XMLElement MainElement(g_strWidget);
		MainElement.addAttribute(g_strClsName, strClsName);
		MainElement.addAttribute(g_strID, strWidgetID);

		XMLElement EleOrder = InitEleOrder(strClsName,strWidgetID);
		bRes = SaveWidgets(pWidget, MainElement, EleOrder);
		if(bRes)
			bRes = SaveToXML(MainElement, strFilePath);
		return bRes;
	}

	XMLElement CLayoutManager::InitEleOrder(const TSTRING& strClsName, const TSTRING& strWidgetID)
	{
		XMLElement OrderElement(_T("Order"));
		XMLElement EleInitWidget(_T("Setting"));
		EleInitWidget.addAttribute(_T("ParentWidget"), _T(""));
		EleInitWidget.addAttribute(_T("ID"), _T(""));
		EleInitWidget.addAttribute(_T("Parameter"),	_T(""));
		EleInitWidget.addAttribute(_T("Description"), _T(""));
		OrderElement.getAllElement()->push_back(EleInitWidget);
		return OrderElement;
	}

	bool CLayoutManager::SaveWidgets(Widge<HDC>* pWidget, XMLElement& MainElement, XMLElement& EleOrder)
	{
		bool bRes = true;
		XMLElement EleLayoutBase(_T("LayoutBase"));
		bRes = pWidget->Save(MainElement, EleOrder, EleLayoutBase);
		
		if(EleOrder.getAllElement()->size() > 1 && EleLayoutBase.getAllElement()->size() > 0)
		{
			MainElement.getAllElement()->push_back(EleOrder);
			MainElement.getAllElement()->push_back(EleLayoutBase);
		}
		
		return bRes;
	}
	XMLElement CLayoutManager::InitEleRoot()
	{
		XMLElement Element;
		Element.setname(_T("App"));
		Element.addAttribute(_T("Name"), _T("QSyncer 2.0"));
		Element.addAttribute(_T("Version"),_T("1.1"));
		return Element;
	}

	bool CLayoutManager::SaveToXML(const XMLElement& Element, const TSTRING& strFilePath)
	{
		bool bRes = false;
		
		if(!strFilePath.empty())
		{
			MSXMLLibrary								qXMLlib;
			XMLMANAGER::XMLDocument						qXMLdoc = &qXMLlib;

			if(!m_bLoad || strFilePath != m_strFileName)
				m_qXMLdoc.load(strFilePath);
			
			//size_t nsize = m_EleRoot.getAllElement()->size();
			if(m_qXMLdoc.getAllElement()->size() > 0)
			{
				RemoveOrgElements(Element);
				m_EleRoot.getAllElement()->push_back(Element);
			}
			else
			{
				m_EleRoot = InitEleRoot();
				m_EleRoot.getAllElement()->push_back(Element);
			}
			qXMLdoc.getAllElement()->push_back(m_EleRoot);
			bRes = qXMLdoc.save(strFilePath);			
		}
		assert(bRes);
		return bRes;
	}
	bool CLayoutManager::RemoveOrgElements(const XMLElement& Element)
	{
		bool bRes = true;
		TSTRING strID;
		Element.getAttr(_T("ID"), strID);
		
		for(XMLAllElement::iterator iter = m_EleRoot.getAllElement()->begin(); iter != m_EleRoot.getAllElement()->end(); iter++)
		{
			TSTRING strIterID;
			iter->getAttr(_T("ID"), strIterID);
			if(strIterID == strID)
			{
				XMLAllElement::iterator iterPre = iter-1;
				m_EleRoot.getAllElement()->erase(iter);
				iter = iterPre;
			}
		}

		return bRes;
	}
} //namespace QSYNC_LIB