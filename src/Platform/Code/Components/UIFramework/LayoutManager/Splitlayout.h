#ifndef _SplitLayout_Header_
#define _SplitLayout_Header_
#pragma once

#include "LayoutBase.h"
#include <stdlib.h>
namespace QSYNC_LIB
{
	template <class GraphContext>
	class SplitLayout : public LayoutBaseWidge<GraphContext>
	{
	public:
		SplitLayout(bool bVSplit = true):m_nSpacing(0), m_nFixedValue(-1), m_bFirstFixed(true), m_fRatio(-1),
			m_bVSplit(bVSplit),m_pFirstWidget(new LayoutBase<GraphContext>), 
			m_pSecondWidget(new LayoutBase<GraphContext>),
			m_pBarWidget(NULL),
			m_strClassName(_T("SplitLayout")),
			m_nFirstLimit(-1), m_nSecondLimit(-1),
			m_bSplitBarMovable(false), m_bDrag(false)
			,m_bUserSetFixValue(false)

		{
			m_vectWidget.push_back(m_pFirstWidget);
			m_vectWidget.push_back(m_pSecondWidget);
		}
		virtual ~SplitLayout()
		{
		}
		virtual bool AddWidget(Widge* pWidget, const TSTRING& strParameter)
		{
			bool bRes = true;
			if(strParameter == _T("First"))
				AddWidget(pWidget);
			else if(strParameter == _T("Second"))
				AddWidget(pWidget,0,false);
			else if(strParameter == _T("SplitBar"))
				AddBarWidget(pWidget);
			else
				bRes = false;
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
		bool AddWidget(Widge* pFirstWidget,Widge* pSecondWidget, int nFirstAlignment = VCenter|HCenter, int nSecondAlignment = VCenter|HCenter)
		{
			bool bRes = false;
			bRes = AddWidget(pFirstWidget, nFirstAlignment, true) && AddWidget(pSecondWidget, nSecondAlignment, false);
			return bRes;
		}
		bool AddWidget(Widge* pFirstWidget,Widge* pSecondWidget, Widge* pMiddleWidget, int nFirstAlignment = VCenter|HCenter, int nSecondAlignment = VCenter|HCenter)
		{
			bool bRes = false;
			bRes = AddWidget(pFirstWidget,pSecondWidget,nFirstAlignment,nSecondAlignment);
			if(bRes)			
				bRes = AddBarWidget(pMiddleWidget);
			return bRes;
		}

		bool AddWidget(Widge* pIWidget, int nAlignment = VCenter|HCenter, bool bFirst/*Top or Left*/ = true)
		{
			bool bRes = false;
			if(pIWidget)
			{
				LayoutBase<GraphContext>* pLayoutBase = bFirst ? m_pFirstWidget : m_pSecondWidget;
				if(pLayoutBase)
				{
					bRes = pLayoutBase->AddWidget(pIWidget, nAlignment);
				}
			}
			return bRes;
		}
		bool AddWidget(Widge* pIWidget, int nFixedWidth, int nFixedHeight, int nAlignment = VCenter|HCenter, bool bFirst/*Top or Left*/ = true)
		{
			bool bRes = false;
			if(pIWidget)
			{
				LayoutBase<GraphContext>* pLayoutBase = bFirst ? m_pFirstWidget : m_pSecondWidget;
				if(pLayoutBase)
				{
					bRes = pLayoutBase->AddWidget(pIWidget, nAlignment);
					pLayoutBase->SetMinFrame(nFixedWidth, nFixedHeight);
					pLayoutBase->SetMaxFrame(nFixedWidth, nFixedHeight);
				}
			}
			return bRes;
		}
		bool AddBarWidget(Widge* pBarWidget, bool bMovable = false, int nFirstLimit = -1, int nSecondLimit = -1)
		{
			bool bRes = false;
			if(pBarWidget)
			{
				m_pBarWidget = pBarWidget;
				Widgets::iterator iter = m_vectWidget.begin();
				if(m_vectWidget.size()==3)
				{
					iter = iter+2;
					if((*iter))
						delete (*iter);
					m_vectWidget.erase(iter);
				}
				m_vectWidget.push_back(m_pBarWidget);
				bRes = true;
			}
			m_nSpacing = (m_bVSplit ? m_pBarWidget->GetMinWidth() : m_pBarWidget->GetMinHeight());
			if(bMovable)
				SetBarMovable(bMovable,nFirstLimit,nSecondLimit);
			return bRes;
		}

		bool IsBarMovable() const
		{
			 return m_bSplitBarMovable && m_pBarWidget;
		}
		bool SetBarMovable(bool bMovable, int nFirstLimit = -1, int nSecondLimit = -1)
		{
			bool bRes = true;
			m_bSplitBarMovable = bMovable;
			if(bMovable)
			{
				m_bSplitBarMovable = true;
				if(nFirstLimit != -1)
					m_nFirstLimit = nFirstLimit;
				else
					m_nFirstLimit = max(m_bVSplit ?  m_pFirstWidget->GetMinWidth() : m_pFirstWidget->GetMinHeight(),1);
				if(nSecondLimit != -1)
					m_nSecondLimit = nSecondLimit;
				else
					m_nSecondLimit = max(m_bVSplit ?  m_pSecondWidget->GetMinWidth() : m_pSecondWidget->GetMinHeight(),1);
			}
			return bRes;
		}

		void SetFixedValue(int nValue,bool bFirstFixed = true)
		{
			m_bUserSetFixValue = true;
			m_bFirstFixed = bFirstFixed;
			if(IsBarMovable())
				m_nFixedValue = nValue;
			else
			{
				LayoutBase<GraphContext>* pLayoutBase = m_bFirstFixed ? m_pFirstWidget : m_pSecondWidget;
				if(pLayoutBase && GetMinValue(pLayoutBase) > nValue)	
					m_nFixedValue = GetMinValue(pLayoutBase);
				else
					m_nFixedValue = nValue;
			}
			m_fRatio = -1;
		}
		bool SetRatio(double fRatio,bool bFirstFixed = true)
		{
			bool bRes = false;
			if(!IsBarMovable())
			{
				m_bFirstFixed = bFirstFixed;
				if(fRatio <= 1)
				{
					m_fRatio = fRatio;
					m_nFixedValue = -1;
					bRes = true;
				}
			}
			return bRes;
		}
		bool SetSpacing(int nValue)
		{
			bool nRes = true;
			if(IsBarMovable())
				nRes = false;
			else
				m_nSpacing = nValue;
			return nRes;
		}
		int GetSpacing(void) const {return m_nSpacing;}
		void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
		{
			m_pFirstWidget->SetMargin(nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
			m_pSecondWidget->SetMargin(nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
		}
		void SetVariouslyMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin,bool bFirst)
		{
			if(bFirst)
				m_pFirstWidget->SetMargin(nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
			else
				m_pSecondWidget->SetMargin(nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
		}
		int GetFixValue() const {return m_nFixedValue;}
		LayoutBase<GraphContext>* GetWidget(bool bLeft = true) const
		{
			return bLeft ? m_pFirstWidget : m_pSecondWidget;
		}
		Widge* GetBarWidget() const {return m_pBarWidget;}
		
	public:
		virtual int GetMinWidth()
		{
			int nRes = -1;
			int nLeftMinWidth = m_pFirstWidget->GetMinWidth();
			int nRightMinWidth = m_pSecondWidget->GetMinWidth();
			if(m_bVSplit)
			{
				nLeftMinWidth = (m_bFirstFixed && (m_nFixedValue > nLeftMinWidth)) ? m_nFixedValue : nLeftMinWidth;
				nRightMinWidth = (!m_bFirstFixed && (m_nFixedValue > nRightMinWidth)) ? m_nFixedValue : nRightMinWidth;
				if(nLeftMinWidth != -1)
				{	
					nRes = nLeftMinWidth;
					if(nRightMinWidth != -1)
						nRes+=nRightMinWidth;
				}
				else
				{
					if(nRightMinWidth != -1)
						nRes=nRightMinWidth;
				}
			}
			else
			{
				nRes = nLeftMinWidth > nRightMinWidth ? nLeftMinWidth : nRightMinWidth;
			}
			return nRes;
		}
		virtual int GetMinHeight()
		{
			int nRes = -1;
			int nTopMinHeight = m_pFirstWidget->GetMinHeight();
			int nBottomMinHeight = m_pSecondWidget->GetMinHeight();
			if(!m_bVSplit)
			{
				nTopMinHeight = (m_bFirstFixed && (m_nFixedValue > nTopMinHeight)) ? m_nFixedValue : nTopMinHeight;
				nBottomMinHeight = (!m_bFirstFixed && (m_nFixedValue > nBottomMinHeight)) ? m_nFixedValue : nBottomMinHeight;
				if(nTopMinHeight != -1)
				{	
					nRes = nTopMinHeight;
					if(nBottomMinHeight != -1)
						nRes+=nBottomMinHeight;
				}
				else
				{
					if(nBottomMinHeight != -1)
						nRes=nBottomMinHeight;
				}
			}
			else
			{
				nRes = nTopMinHeight > nBottomMinHeight ? nTopMinHeight : nBottomMinHeight;
			}
			return nRes;
		}
		virtual bool SetGeometry(int nX,int nY,int nWidth,int nHeight)
		{
			bool bRes = false;

			int nFirstValue=0;
			int nSecondValue=0;
			int nNewWidth = 0;
			int nNewHeight = 0;

			if(IsBarMovable())
			{
				if( (nX != 0) || (nY != 0) || (nWidth != 0) || (nHeight != 0) )
				{
					if((m_bVSplit ? nWidth : nHeight)<GetLogeticLimitLength())
					{
						if(m_bVSplit) 
							nWidth = GetLogeticLimitLength();
						else
							nHeight =GetLogeticLimitLength();
					}
					if(m_nFixedValue == -1)
					{
						int nTempValue = m_bVSplit ? nWidth : nHeight;//先把長度存起來
						SetFixedValuePrivate(nTempValue/2,m_bFirstFixed) ;
						if(m_nFixedValue > nTempValue - (m_bFirstFixed ? m_nSecondLimit : m_nFirstLimit) - m_nSpacing)
							m_nFixedValue = max(0,(m_bFirstFixed ? m_nSecondLimit : m_nFirstLimit));
					}
					if(m_bUserSetFixValue)
					{
						int nTempValue = m_bVSplit ? nWidth : nHeight;//先把長度存起來讓SetFixdValue用
						if( (m_nFixedValue < m_nFirstLimit) || 
							(m_nFixedValue > nTempValue - (m_bFirstFixed ? m_nSecondLimit : m_nFirstLimit)) )
							m_nFixedValue = m_nFirstLimit;

						if(m_bFirstFixed && (m_nFixedValue < m_nFirstLimit) )
							m_nFixedValue = m_nFirstLimit;
						else if(m_bFirstFixed && (m_nFixedValue > (m_bVSplit ? nWidth : nHeight) - m_nSecondLimit - m_nSpacing))
							m_nFixedValue = (m_bVSplit ? nWidth : nHeight) - m_nSecondLimit;
						else if(!m_bFirstFixed && (m_nFixedValue < m_nSecondLimit) )
							m_nFixedValue = m_nSecondLimit;
						else if(!m_bFirstFixed && (m_nFixedValue > (m_bVSplit ? nWidth : nHeight) - m_nFirstLimit - m_nSpacing))
							m_nFixedValue = (m_bVSplit ? nWidth : nHeight) - m_nFirstLimit - m_nSpacing;
						m_bUserSetFixValue=false;
					}
					int nValue = m_bVSplit ? nWidth : nHeight;
					if((m_nFixedValue + m_nSpacing + (m_bFirstFixed ? m_nSecondLimit : m_nFirstLimit)) < nValue){;}
					else if((m_nFixedValue+m_nSpacing < nValue)/*&&(m_nFixedValue > (m_bFirstFixed?m_nFirstLimit:m_nSecondLimit))*/ )
					{
						int nDiffer =  m_nFixedValue + m_nSpacing - nValue + (m_bFirstFixed ? m_nSecondLimit : m_nFirstLimit);
						int nNewFixValue = max(m_nFixedValue - nDiffer , (m_bFirstFixed ? m_nFirstLimit : m_nSecondLimit));
						SetFixedValuePrivate(nNewFixValue,m_bFirstFixed);
					}
					else if ((m_nFixedValue+m_nSpacing >= nValue)&&(m_nFirstLimit + m_nSecondLimit + m_nSpacing < nValue))
					{
						int nNewFixValue = nValue - m_nSpacing - (m_bFirstFixed ? m_nSecondLimit : m_nFirstLimit);
						SetFixedValuePrivate(nNewFixValue,m_bFirstFixed);
					}
					else if(m_nFirstLimit + m_nSecondLimit + m_nSpacing >= nValue)
					{
						int nNewFixValue = m_bFirstFixed ? m_nFirstLimit : m_nSecondLimit;
						SetFixedValuePrivate(nNewFixValue,m_bFirstFixed);
					}
				}
			}

			if(m_bVSplit)
			{
				nNewWidth = GetSplitValue(nWidth, nFirstValue, nSecondValue);
				nNewHeight = GetUnsplitValue(nHeight);
			}
			else
			{
				nNewHeight = GetSplitValue(nHeight, nFirstValue, nSecondValue);
				nNewWidth = GetUnsplitValue(nWidth);
			}
			if(nNewWidth == nWidth && nNewHeight == nHeight)
				bRes = true;

			if(Widge<GraphContext>::SetGeometry(nX,nY, nNewWidth, nNewHeight))
			{	
				if(m_bVSplit)
				{				
					m_pFirstWidget->SetGeometry(nX, nY, nFirstValue, nHeight);
					m_pSecondWidget->SetGeometry(nX+nFirstValue+m_nSpacing, nY, nSecondValue, nHeight);	
				}
				else
				{				
					m_pFirstWidget->SetGeometry(nX, nY, nWidth, nFirstValue);
					m_pSecondWidget->SetGeometry(nX, nY+nFirstValue+m_nSpacing, nWidth, nSecondValue);
				}	
			}
			if(m_pBarWidget)//將來如果要改Bar的長寬設定,要記得修改IsMouseOnSplitBar函式
			{				
				if(m_bVSplit)
					if(IsBarMovable())
						if(m_bFirstFixed)
							m_pBarWidget->SetGeometry(
							nX + m_nFixedValue,
							nY ,
							m_nSpacing ,
							nHeight );
						else
							m_pBarWidget->SetGeometry(
							nX + nWidth - m_nSpacing - m_nFixedValue,
							nY ,
							m_nSpacing ,
							nHeight );
					else
						m_pBarWidget->SetGeometry(
						nX + m_pFirstWidget->GetGeometry().m_nWidth ,
						nY ,
						m_nSpacing ,
						nHeight );
				else//水平切,分上下兩塊
					if(IsBarMovable())
						if(m_bFirstFixed)
							m_pBarWidget->Widge<GraphContext>::SetGeometry(
							nX,
							nY + m_nFixedValue ,
							nWidth ,
							m_nSpacing );
						else
							m_pBarWidget->Widge<GraphContext>::SetGeometry(
							nX ,
							nY + nHeight - m_nFixedValue - m_nSpacing ,
							nWidth ,
							m_nSpacing );
					else
						m_pBarWidget->Widge<GraphContext>::SetGeometry(
						nX ,
						nY + m_pFirstWidget->GetGeometry().m_nHeight ,
						nWidth ,
						m_nSpacing );
			}
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
			{
				bRes = m_pFirstWidget->SetLayout();
				if(bRes)				
					bRes = m_pSecondWidget->SetLayout();
				if(bRes&&m_pBarWidget)
					bRes = m_pBarWidget->SetLayout();
			}
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
			bool bRes = true;
			XMLElement Element(g_strWidget);
			Element.addAttribute(g_strClsName, m_strClassName);
			TSTRING strID;
			iToTStr(GetID(),strID);
			strID = m_strClassName+_T("_")+strID;
			Element.addAttribute(g_strID, strID);

			XMLElement Property(g_strProperty);
			Property.addAttribute(_T("Spacing"), m_nSpacing);
			TSTRING strDirection = m_bVSplit ? _T("V") : _T("H");
			Property.addAttribute(_T("Direction"), strDirection);
			Property.addAttribute(_T("FirstLimit"), m_nFirstLimit);
			Property.addAttribute(_T("SecondLimit"), m_nSecondLimit);
			TSTRING strMovable = IsBarMovable() ? _T("true") : _T("false");
			Property.addAttribute(_T("Movable"), strMovable);
			TSTRING strFixedItem = m_bFirstFixed ? _T("first") : _T("second");
			Property.addAttribute(_T("FixedItem"), strFixedItem);
			if(m_fRatio == -1 && m_nFixedValue != -1)
			{
				Property.addAttribute(_T("FixedStyle"), _T("Value"));
				Property.addAttribute(_T("FixedValue"), m_nFixedValue);
			}
			else if(m_fRatio != -1 && m_nFixedValue == -1)
			{
				Property.addAttribute(_T("FixedStyle"), _T("Ratio"));
				Property.addAttribute(_T("FixedValue"),m_fRatio);
			}
			else  // default
			{
				Property.addAttribute(_T("FixedStyle"), _T("Ratio"));
				Property.addAttribute(_T("FixedValue"),0.5);
			}
			Element.getAllElement()->push_back(Property);
			MainElement.getAllElement()->push_back(Element);

			FillOrderSetting(EleOrder, strID);

			AddOrderSetting(EleOrder, strID, _T("First"));
			m_pFirstWidget->AddLayoutBaseSetting(EleLayoutBase, strID, 0);
			bRes = m_pFirstWidget->Save(MainElement, EleOrder, EleLayoutBase);

			if(bRes && m_pBarWidget)
			{
				AddOrderSetting(EleOrder, strID, _T("SplitBar"));
				bRes = m_pBarWidget->Save(MainElement, EleOrder, EleLayoutBase);
			}
			if(bRes)
			{
				AddOrderSetting(EleOrder, strID, _T("Second"));
				m_pSecondWidget->AddLayoutBaseSetting(EleLayoutBase, strID, 1);
				bRes = m_pSecondWidget->Save(MainElement, EleOrder, EleLayoutBase);
			}

			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element) // Property
		{
			bool bRes = false;
			if(Element.name() == g_strProperty)
			{
				m_nSpacing = Element.getAttr(_T("Spacing"));
				m_nSpacing = m_nSpacing == -1 ? 0 : m_nSpacing; 

				TSTRING strDirection;
				Element.getAttr(_T("Direction"),strDirection);
				m_bVSplit = strDirection != _T("H"); // default Vertical

				m_nFirstLimit = Element.getAttr(_T("FirstLimit")); // default -1
				m_nSecondLimit = Element.getAttr(_T("SecondLimit")); // default -1

				TSTRING strMovable;
				Element.getAttr(_T("Movable"), strMovable);
				m_bSplitBarMovable = strMovable == _T("true"); // default false

				TSTRING strFixedItem;
				Element.getAttr(_T("FixedItem"),strFixedItem);
				m_bFirstFixed = strFixedItem != _T("second"); // default first

				TSTRING strFixedStyle;
				Element.getAttr(_T("FixedStyle"), strFixedStyle);
				if(strFixedStyle == _T("Value"))
				{
					SetFixedValuePrivate(Element.getAttr(_T("FixedValue")),m_bFirstFixed); // default fixedValue -1
				}
				else if(strFixedStyle == _T("Ratio"))
				{
					TSTRING strRatio;
					Element.getAttr(_T("FixedValue"), strRatio);
					if(strRatio!=_T(""))
						SetRatio(_tstof(strRatio.c_str()), m_bFirstFixed);
				}
				else
				{
					SetRatio(0.5, m_bFirstFixed);
				}
				bRes = true;
			}
			return bRes;
		}
	private:
		void AddOrderSetting(XMLMANAGER::XMLElement& EleOrder, const TSTRING& strID, const TSTRING& strParameter)
		{
			using namespace XMLMANAGER;
			XMLElement EleSetting(_T("Setting"));
			EleSetting.addAttribute(_T("ParentWidget"), strID);
			EleSetting.addAttribute(_T("ID"), _T(""));
			EleSetting.addAttribute(_T("Parameter"), strParameter);
			EleSetting.addAttribute(_T("Description"), _T(""));
			EleOrder.getAllElement()->push_back(EleSetting);
		}
		int GetLogeticLimitLength() const
		{
			return ((m_nFirstLimit==-1) ? 0 : m_nFirstLimit) + ((m_nSecondLimit==-1) ? 0 : m_nSecondLimit) + m_nSpacing;
		}
		int GetSplitValue(int nValue, int& nFirstValue, int& nSecondValue)//nValue在m_bVSplit時指nWidth,否則就是nHeigh
		{
			if(!IsBarMovable())
			{
				if(m_nFixedValue != -1 && (m_nFixedValue+m_nSpacing) < nValue )
					GetFixedGeometry(nValue, nFirstValue, nSecondValue); 
				else if(m_fRatio != -1 && ((m_fRatio*nValue)+m_nSpacing) < nValue)
					GetRatioGeometry(nValue, nFirstValue, nSecondValue); 
				else
					GetDefaultGeometry(nValue, nFirstValue, nSecondValue);
			}
			else
			{
				if((m_nFixedValue + m_nSpacing + (m_bFirstFixed ? m_nSecondLimit : m_nFirstLimit)) < nValue)
					GetFixedGeometry(nValue, nFirstValue, nSecondValue); 
				else if((m_nFixedValue+m_nSpacing < nValue) )
					GetFixedGeometry(nValue, nFirstValue, nSecondValue); 
				else if ((m_nFixedValue+m_nSpacing >= nValue)&&(m_nFirstLimit + m_nSecondLimit + m_nSpacing < nValue))
					GetFixedGeometry(nValue, nFirstValue, nSecondValue); 
				else
				{
					nFirstValue = m_nFirstLimit;
					nSecondValue = m_nSecondLimit;
				}
			}
			nValue = nFirstValue+nSecondValue+m_nSpacing;

			return nValue;
		}
		int GetUnsplitValue(int nValue)
		{
			int nMinFirstValue= GetMinValue(m_pFirstWidget);
			if(	nMinFirstValue !=1 )
				nValue = nMinFirstValue > nValue ? nMinFirstValue : nValue;
			int nMinSecondValue = GetMinValue(m_pSecondWidget);
			if(	nMinSecondValue !=1 )
				nValue = nMinSecondValue > nValue ? nMinSecondValue : nValue;
			return nValue;
		}	
		void GetFixedGeometry(int nValue, int& nFirstValue, int& nSecondValue)
		{
			int nFirstMinValue = IsBarMovable() ? m_nFirstLimit : GetMinValue(m_pFirstWidget);
			int nSecondMinValue = IsBarMovable() ? m_nSecondLimit : GetMinValue(m_pSecondWidget);

			int nUnFixedWidth = nValue-m_nFixedValue-m_nSpacing;
			if(m_bFirstFixed && nSecondMinValue > nUnFixedWidth)
				nUnFixedWidth = nSecondMinValue;
			else if(!m_bFirstFixed && nFirstMinValue > nUnFixedWidth)
				nUnFixedWidth = nFirstMinValue;
				
			nFirstValue = m_bFirstFixed ? m_nFixedValue : nUnFixedWidth;
			nSecondValue = m_bFirstFixed ? nUnFixedWidth : m_nFixedValue ;
		
		}
		void GetRatioGeometry(int nValue, int& nFirstValue, int& nSecondValue)
		{
			int nFirstMinValue = GetMinValue(m_pFirstWidget);
			int nSecondMinValue = GetMinValue(m_pSecondWidget);

			int nProportion = (int)(m_fRatio*(nValue-m_nSpacing));
			if(m_bFirstFixed && nFirstMinValue > nProportion)
				nProportion = nFirstMinValue;
			else if(!m_bFirstFixed && nSecondMinValue > nProportion)
				nProportion = nSecondMinValue;
			
			int nUnProportion = nValue-m_nSpacing-nProportion;
			if(m_bFirstFixed && nSecondMinValue > nUnProportion)
				nUnProportion = nSecondMinValue;
			else if(!m_bFirstFixed && nFirstMinValue > nUnProportion)
				nUnProportion = nFirstMinValue;
			
			nFirstValue = m_bFirstFixed ? nProportion : nUnProportion;
			nSecondValue = m_bFirstFixed ? nUnProportion : nProportion;
		}
		void GetDefaultGeometry(int nValue, int& nFirstValue, int& nSecondValue)
		{
			int nFirstMinValue = GetMinValue(m_pFirstWidget);
			int nSecondMinValue = GetMinValue(m_pSecondWidget);

			nFirstValue = (nValue-m_nSpacing)/2;
			nSecondValue = nFirstValue;
			if(nFirstMinValue > nFirstValue)
				nFirstValue = nFirstMinValue;
			if(nSecondMinValue > nSecondValue)
				nSecondValue = nSecondMinValue;
		}
		int GetMinValue(LayoutBase<GraphContext>* pLayoutBase)
		{
			int nRes = m_bVSplit ? pLayoutBase->GetMinWidth() : pLayoutBase->GetMinHeight();
			return nRes;
		}

		bool IsMouseOnSplitBar(int nCX,int nCY)
		{
			bool nRes = false;
			int nSplitLength = m_bVSplit ? GetGeometry().m_nWidth : GetGeometry().m_nHeight;
			int nUnsplitLenth = m_bVSplit ? GetGeometry().m_nHeight : GetGeometry().m_nWidth;
			int nStartPointX = GetGeometry().m_nX;
			int nStartPointY = GetGeometry().m_nY;
			int nSplitMouseSeat = m_bVSplit ? nCX - nStartPointX : nCY - nStartPointY; //已減去初使點,所以是相對座標
			int nUnsplitMouseSeat = m_bVSplit ? nCY - nStartPointY : nCX - nStartPointX ; 
				if(m_bFirstFixed)
					nRes = (
					(nSplitMouseSeat >= m_nFixedValue) &&
					(nSplitMouseSeat < m_nFixedValue + m_nSpacing) &&
					(nUnsplitMouseSeat >= 0) &&
					(nUnsplitMouseSeat < nUnsplitLenth) );
				else
					nRes = ( 
					(nSplitMouseSeat >= nSplitLength - m_nFixedValue - m_nSpacing) &&
					(nSplitMouseSeat <= nSplitLength - m_nFixedValue) &&
					(nUnsplitMouseSeat >= 0) &&
					(nUnsplitMouseSeat < nUnsplitLenth) );
			return nRes;
		};

		void SetFixedValuePrivate(int nValue,bool bFirstFixed = true)
		{
			m_bFirstFixed = bFirstFixed;
			if(IsBarMovable())
			{
				if(m_bFirstFixed)
					m_nFixedValue = max(nValue, m_nFirstLimit);
				else
					m_nFixedValue = max(nValue, m_nSecondLimit);
			}
			else
			{
				LayoutBase<GraphContext>* pLayoutBase = m_bFirstFixed ? m_pFirstWidget : m_pSecondWidget;
				if(pLayoutBase && GetMinValue(pLayoutBase) > nValue)	
					m_nFixedValue = GetMinValue(pLayoutBase);
				else
					m_nFixedValue = nValue;
			}
			m_fRatio = -1;
		}

	protected:
		virtual void MouseMove(int nCX,int nCY)
		{
			LayoutBaseWidge::MouseMove(nCX,nCY);
			if(IsBarMovable())
			{
				if (m_bDrag)
				{
					SetCursor(LoadCursor (NULL, m_bVSplit ? IDC_SIZEWE : IDC_SIZENS));
					int nBarSeat;
					if(m_nFirstLimit + m_nSecondLimit + m_nSpacing > (m_bVSplit ? GetGeometry().m_nWidth : GetGeometry().m_nHeight))
						if(m_bFirstFixed)
							nBarSeat = m_nFirstLimit;
						else
							nBarSeat = (m_bVSplit ? GetGeometry().m_nWidth : GetGeometry().m_nHeight) - m_nSecondLimit - m_nSpacing;
					else if(m_bVSplit)
						nBarSeat = min(max((nCX-m_nOffSet),m_nFirstLimit),GetGeometry().m_nWidth - m_nSecondLimit - m_nSpacing);
					else
						nBarSeat = min(max((nCY-m_nOffSet),m_nFirstLimit),GetGeometry().m_nHeight - m_nSecondLimit - m_nSpacing);

					SetFixedValuePrivate(nBarSeat,true);
					SetGeometry(GetGeometry().m_nX,GetGeometry().m_nY,GetGeometry().m_nWidth,GetGeometry().m_nHeight);
					SetLayout();
					Update();
				}
				else if(IsMouseOnSplitBar(nCX,nCY))
					SetCursor(LoadCursor (NULL, m_bVSplit ? IDC_SIZEWE : IDC_SIZENS));
			}
		}

		virtual void MouseDown(int nCX,int nCY)
		{
			LayoutBaseWidge::MouseDown(nCX,nCY);
			if(IsBarMovable())
			{
				if (IsMouseOnSplitBar(nCX,nCY))
				{
					this->CaptureMouse(true);
					SetCursor(LoadCursor (NULL, m_bVSplit ? IDC_SIZEWE : IDC_SIZENS));
					m_bDrag = true;
					if(m_bFirstFixed)
						m_nOffSet = (m_bVSplit ? nCX : nCY )- m_nFixedValue;
					else
					{
						int SplitLength = m_bVSplit ? GetGeometry().m_nWidth : GetGeometry().m_nHeight;
						m_nOffSet = (m_bVSplit ? nCX : nCY )- (SplitLength - m_nFixedValue - m_nSpacing);
					}
				}
			}
		}
		virtual void MouseUp(int nCX,int nCY)
		{
			LayoutBaseWidge::MouseUp(nCX,nCY);
			if(IsBarMovable())
			{
				if(m_bDrag)
				{
					this->CaptureMouse(false);
					m_bDrag = false;
				}
			}
		}
		virtual void Timer(WPARAM nTimerID)
		{
			LayoutBaseWidge::Timer(nTimerID);
		}
	private:
		LayoutBase<GraphContext>* m_pFirstWidget;
		LayoutBase<GraphContext>* m_pSecondWidget;
		Widge*      m_pBarWidget;

		int m_nSpacing;
		double m_fRatio;
		int m_nFixedValue;
		bool m_bFirstFixed;
		bool m_bVSplit;
		TSTRING m_strClassName;

		int m_nFirstLimit;
		int m_nSecondLimit;
		bool m_bSplitBarMovable;
		bool m_bDrag;
		int m_nOffSet;
		bool m_bUserSetFixValue;
	};
}

#endif //_SplitLayout_Header_