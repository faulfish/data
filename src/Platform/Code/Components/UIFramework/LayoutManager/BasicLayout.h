#ifndef _BasicLayout_Header_
#define _BasicLayout_Header_
#pragma once


#include "LayoutBase.h"
namespace QSYNC_LIB
{

	template <class GraphContext>
	class BasicLayout : public LayoutBaseWidge<GraphContext>
	{
	public:
		BasicLayout() :m_pBaseWidget(new LayoutBase<GraphContext>),m_nCommonTopMargin(0),m_nCommonBottomMargin(0),
			m_nCommonLeftMargin(0),m_nCommonRightMargin(0),m_strClassName(_T("BasicLayout")){}
		virtual ~BasicLayout(){}
		virtual bool AddWidget(Widge* pWidget, const TSTRING& strParameter)
		{
			bool bRes = true;
			bRes = AddWidget(pWidget);
			return bRes;
		}
		virtual bool SetLayoutBase(int nIndex, const LayoutBaseSettings& Setting )
		{
			bool bRes = true;
			if((UINT)nIndex > m_vectWidget.size())
				bRes = false;
			else
				((LayoutBase<GraphContext>*)m_vectWidget[nIndex])->SetLayoutBase( nIndex, Setting);

			return bRes;
		}
		bool AddWidget(Widge* pIWidget, int nAlignment = VCenter|HCenter)
		{
			bool bRes = false;
			if(pIWidget && m_pBaseWidget)
			{
				bRes = m_pBaseWidget->AddWidget(pIWidget, nAlignment);
				m_pBaseWidget->SetMargin(m_nCommonTopMargin,m_nCommonBottomMargin,m_nCommonLeftMargin,m_nCommonRightMargin);
				m_vectWidget.push_back(m_pBaseWidget);
			}
			return bRes;
		}
		void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
		{
			m_nCommonTopMargin = nTopMargin;
			m_nCommonBottomMargin = nBottomMargin;
			m_nCommonLeftMargin = nLeftMargin;
			m_nCommonRightMargin = nRightMargin;
			
			m_pBaseWidget->SetMargin(nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
		}
		void SetAlignment(int nAlignment)
		{
			m_pBaseWidget->SetAlignment(nAlignment);
		}
		void FixSize(int nWidth, int nHeight)
		{
			m_pBaseWidget->SetMinWidth(nWidth);
			m_pBaseWidget->SetMaxWidth(nWidth);
			m_pBaseWidget->SetMinHeight(nHeight);
			m_pBaseWidget->SetMaxHeight(nHeight);
		}
	public:
		virtual int GetMinWidth(){return m_pBaseWidget->GetMinWidth();}
		virtual int GetMinHeight(){return m_pBaseWidget->GetMinHeight();}
		virtual bool SetGeometry(int nX,int nY,int nWidth,int nHeight)
		{
			bool bRes = true;
			bRes = m_pBaseWidget->SetGeometry(nX,nY,nWidth,nHeight);
			return bRes;
		}
		virtual bool SetGeometry(GeometryRect rect)
		{
			bool bRes = false;
			bRes = SetGeometry(rect.m_nX, rect.m_nY, rect.m_nWidth, rect.m_nHeight);
			return bRes;
		}
		virtual bool SetLayout(Widge* pIWidget = 0)
		{
			bool bRes = true;
			if(pIWidget)
				bRes = pIWidget->SetLayout();	
			else
				bRes = m_pBaseWidget->SetLayout();	
			return bRes;
		}
		virtual int GetID()
		{
			static int nID =0;
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
			bool bRes = true;
			XMLElement Element(g_strWidget);
			Element.addAttribute(g_strClsName, m_strClassName);
			TSTRING strID;
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.addAttribute(g_strID, strID);

			XMLElement Property(g_strProperty);
			Property.addAttribute(_T("CommonTopMargin"), m_nCommonTopMargin);
			Property.addAttribute(_T("CommonBottomMargin"), m_nCommonBottomMargin);
			Property.addAttribute(_T("CommonLeftMargin"), m_nCommonLeftMargin);
			Property.addAttribute(_T("CommonRightMargin"), m_nCommonRightMargin);
			Element.getAllElement()->push_back(Property);
			MainElement.getAllElement()->push_back(Element);

			FillOrderSetting(EleOrder, strID);
			AddOrderSetting(EleOrder, strID);
			m_pBaseWidget->AddLayoutBaseSetting(EleLayoutBase, strID, 0);
			bRes = m_pBaseWidget->Save(MainElement, EleOrder, EleLayoutBase);

			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)  // Element is Item! :D
		{
			bool bRes = false;
			if(Element.name() == g_strProperty)
			{
				m_nCommonTopMargin = Element.getAttr(_T("CommonTopMargin"));
				m_nCommonTopMargin = m_nCommonTopMargin == -1 ? 0 : m_nCommonTopMargin;
				m_nCommonBottomMargin = Element.getAttr(_T("CommonBottomMargin"));
				m_nCommonBottomMargin = m_nCommonBottomMargin == -1 ? 0 : m_nCommonBottomMargin;
				m_nCommonLeftMargin = Element.getAttr(_T("CommonLeftMargin"));
				m_nCommonLeftMargin = m_nCommonLeftMargin == -1 ? 0 : m_nCommonLeftMargin;
				m_nCommonRightMargin = Element.getAttr(_T("CommonRightMargin"));
				m_nCommonRightMargin = m_nCommonRightMargin == -1 ? 0 : m_nCommonRightMargin;

				bRes = true;
			}
			return bRes;
		}
	private:
		void AddOrderSetting(XMLMANAGER::XMLElement& EleOrder, const TSTRING& strID)
		{
			using namespace XMLMANAGER;
			XMLElement EleSetting(_T("Setting"));
			EleSetting.addAttribute(_T("ParentWidget"), strID);
			EleSetting.addAttribute(_T("ID"), _T(""));
			EleSetting.addAttribute(_T("Parameter"), _T(""));
			EleSetting.addAttribute(_T("Description"), _T(""));
			EleOrder.getAllElement()->push_back(EleSetting);
		}
	private:
		LayoutBase<GraphContext>*					m_pBaseWidget;
		int											m_nCommonTopMargin;
		int											m_nCommonBottomMargin;
		int											m_nCommonLeftMargin;
		int											m_nCommonRightMargin;
		TSTRING										m_strClassName;
	};
} // namespace QSYNC_LIB


#endif //_BasicLayout_Header_