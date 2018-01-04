#ifndef _StackLayout_Header_
#define _StackLayout_Header_
#pragma once

#include "LayoutBase.h"
#include <map> 
namespace QSYNC_LIB
{
	template <class KEY,class GraphContext>
	class StackLayout : public LayoutBaseWidge<GraphContext>
	{
	public:
		StackLayout():m_pCurrentWidget(NULL),m_strClassName(_T("TStrStackLayout")){}
		virtual ~StackLayout(){}
		bool AddWidget(const KEY& key,Widge* pWidget)
		{
			m_vectWidget.push_back(pWidget);
			//if(m_vectWidget.size() == 1)
			//	m_pCurrentWidget = pWidget;
			pWidget->Show(false);
			m_Widgets.insert(WidgetsMapPair(key,pWidget));
			return TRUE;
		}
		virtual bool AddWidget(Widge* pWidget, const TSTRING& strParameter)
		{
			return false;
		}
		virtual bool SetLayoutBase(int nIndex, const LayoutBaseSettings& Setting )
		{
			return true;
		}
	public:
		bool Switch(const KEY& key)
		{
			bool bRes = false;
			WidgetsMap::iterator Iter = m_Widgets.find(key);
			if(Iter != m_Widgets.end())
			{
				if(m_pCurrentWidget != Iter->second)
				{
					if(m_pCurrentWidget)
						m_pCurrentWidget->Show(false);
					m_pCurrentWidget = Iter->second;
					m_pCurrentWidget->Show(true);
					m_pCurrentWidget->SetGeometry(GetGeometry());
					m_pCurrentWidget->SetLayout();					
				}
			}
			return bRes;
		}
		virtual bool SetGeometry(int nX,int nY,int nWidth,int nHeight)
		{
			bool bRes = LayoutBaseWidge::SetGeometry(nX,nY,nWidth,nHeight);
			if(m_pCurrentWidget)
				bRes = m_pCurrentWidget->SetGeometry(nX,nY,nWidth,nHeight);
			return bRes;
		}
		virtual bool SetGeometry(GeometryRect rect)
		{
			return SetGeometry(rect.m_nX, rect.m_nY, rect.m_nWidth, rect.m_nHeight);
		}
		virtual bool SetLayout(Widge* pIWidget = 0)
		{
			bool bRes = true;
			if(m_pCurrentWidget)
				m_pCurrentWidget->SetLayout();
			return bRes;
		}
		virtual int GetID()
		{
			static int nID=-1;
			nID++;
			return nID;
		}
		virtual bool GetName(TSTRING& strName)
		{
			bool bRes=true;
			strName=m_strClassName;
			return bRes;
		}
		virtual bool Save(XMLMANAGER::XMLElement& MainElement, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase)
		{
			using namespace XMLMANAGER;
			bool bRes = false;
			XMLElement Element(g_strWidget);
			Element.addAttribute(g_strClsName, m_strClassName);
			TSTRING strID;
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.addAttribute(g_strID, strID);
			MainElement.getAllElement()->push_back(Element);

			/*
			for(WidgetsMap::iterator iter=m_Widgets.begin(); iter!=m_Widgets.end(); ++iter)
			{
				XMLElement Property(g_strProperty);
				TSTRING strKey = iter->first;
				Property.addAttribute(strKey ,iter->second->GetID());  //<Property KEY1="WidgetID1"/>
				Element.getAllElement()->push_back(Property);
		
				XMLElement eleWidget(g_strWidget);
				iter->second->Save(eleWidget);
				Element.getAllElement()->push_back(eleWidget);
			}
			*/
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = false;
			return bRes;
		}
	private:
		Widge*													m_pCurrentWidget;
		typedef std::map<KEY,Widge*>							WidgetsMap;
		WidgetsMap												m_Widgets;
		typedef std::pair<KEY,Widge*>							WidgetsMapPair;

		

		TSTRING													m_strClassName;
	};

	template<class GraphContext>
	class TStrStackLayout : public StackLayout<TSTRING, GraphContext>
	{
	public:
		TStrStackLayout(){}
		virtual ~TStrStackLayout(){}
		virtual bool AddWidget(Widge* pWidget, const TSTRING& strParameter)
		{
			bool bRes = true;
			bRes = StackLayout::AddWidget(strParameter, pWidget);
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
		{
			bool bRes = false;
			return bRes;
		}
	};

}

#endif //_StackLayout_Header_