#ifndef _BoxLayout_Header_
#define _BoxLayout_Header_
#pragma once

#include "LayoutBase.h"
namespace QSYNC_LIB
{
	enum Direction
	{
		LeftToRight,  //Horizontal
		RightToLeft,  //Horizontal
		TopToBottom,  //Vertical
		BottomToTop   //Vertical
	};	

	template <class GraphContext>
	class BoxLayout : public LayoutBaseWidge<GraphContext>
	{
	public:
		BoxLayout(int nDirection = LeftToRight) : m_nSpacing(0), m_nCellWidth(0), m_nCellHeight(0), 
			m_bReverse(false),m_nCommonTopMargin(0),m_nCommonBottomMargin(0),
			m_nCommonLeftMargin(0),m_nCommonRightMargin(0),m_strClassName(_T("BoxLayout")),
			m_nAlignment(Default)
		{
			if(nDirection < 4 && nDirection > -1)
				m_nDirection = nDirection;
			else
				m_nDirection = LeftToRight;
		}
		virtual ~BoxLayout()
		{
		}
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
			if(pIWidget)
			{
				LayoutBase<GraphContext>* pLayoutBase = new LayoutBase<GraphContext>;
				if(pLayoutBase)
				{
					bRes = pLayoutBase->AddWidget(pIWidget, nAlignment);
					pLayoutBase->SetMargin(m_nCommonTopMargin,m_nCommonBottomMargin,m_nCommonLeftMargin,m_nCommonRightMargin);
					m_vectWidget.push_back(pLayoutBase);
				}
			}
			return bRes;
		}
		bool AddWidget(Widge* pIWidget, int nFixedWidth, int nFixedHeight, int nAlignment = VCenter|HCenter)
		{
			bool bRes = false;
			if(pIWidget)
			{
				LayoutBase<GraphContext>* pLayoutBase = new LayoutBase<GraphContext>;
				if(pLayoutBase)
				{
					bRes = pLayoutBase->AddWidget(pIWidget, nAlignment);
					pLayoutBase->SetMargin(m_nCommonTopMargin,m_nCommonBottomMargin,m_nCommonLeftMargin,m_nCommonRightMargin);
					pLayoutBase->SetMaxFrame(nFixedWidth, nFixedHeight);
					pLayoutBase->SetMinFrame(nFixedWidth, nFixedHeight);
					m_vectWidget.push_back(pLayoutBase);
				}
			}
			return bRes;
		}
		void SetSpacing(int nValue)
		{
			m_nSpacing = nValue;
		}
		int GetSpacing(void)const {return m_nSpacing;}
		bool SetDirection(int nDirection = LeftToRight)
		{
			bool bRes = false;
			if(nDirection < 4 && nDirection > -1)
			{
				m_nDirection = nDirection;
				bRes = true;
			}
			else
			{	
				m_nDirection = LeftToRight;
			}
			return bRes;
		}
		int GetDirection(void)const {return m_nDirection;}
		int GetCount(void) const{return (int)m_vectWidget.size();}
		bool SetMargin(int nIndex, int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
		{
			bool bRes = false;
			if(((size_t)(nIndex)) < m_vectWidget.size())
			{
				((LayoutBase<GraphContext>*)m_vectWidget[nIndex])->SetMargin(nTopMargin, nBottomMargin, nLeftMargin, nRightMargin);
				bRes = true;
			}
			return bRes;
		}
		void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin) //Set All items' Margin
		{
			m_nCommonTopMargin = nTopMargin;
			m_nCommonBottomMargin = nBottomMargin;
			m_nCommonLeftMargin = nLeftMargin;
			m_nCommonRightMargin = nRightMargin;
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				((LayoutBase<GraphContext>*)(*iter))->SetMargin(
					m_nCommonTopMargin
					, m_nCommonBottomMargin
					, m_nCommonLeftMargin
					, m_nCommonRightMargin);
			}
		}
		bool SetAlignment(int nAlignment)
		{
			bool bRes = true;
			bool bAlignment[7]={false}; //[0,1,2,4,8,16,32]
			char cAlignment[7]={0,1,2,4,8,16,32};
			m_nAlignment = nAlignment;
			int nCount=0;
			if(m_nAlignment !=0)
			{
				for(int i=1;i<7 && bRes; i++)
				{
					if(cAlignment[i] & nAlignment)
						bAlignment[i]=true;
					if(i<4 && bAlignment[i])
						nCount++;
					if(i==3)
					{
						m_nAlignment = nCount ==0 ? m_nAlignment+1 : m_nAlignment;
						nCount =0;
					}
					if(i>3 && bAlignment[i])
						nCount++;
					if(i==6)
					{
						m_nAlignment = nCount ==0 ? m_nAlignment+8 : m_nAlignment;
					}
					bRes = nCount > 1 ? false : true;
				}
				if(!bRes)
				{
					m_nAlignment = 0;
				}
			}
			return bRes;
		}
		bool SetCellAlignment(int nIndex, int nAlignment)
		{
			bool bRes = false;
			if(((size_t)(nIndex)) < m_vectWidget.size())
			{
				((LayoutBase<GraphContext>*)(m_vectWidget[nIndex]))->SetAlignment(nAlignment);
				bRes = true;
			}
			return bRes;
		}
		void SetCellAlignment(int nAlignment)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					((LayoutBase<GraphContext>*)(*iter))->SetAlignment(nAlignment);
			}
		}
		void SetCellWidth(int nWidth)
		{
			m_nCellWidth = nWidth;
		}
		void SetCellWidth(int nIndex, int nWidth)
		{
			if(((size_t)(nIndex)) < m_vectWidget.size())
			{
				((LayoutBase<GraphContext>*)(m_vectWidget[nIndex]))->SetMinWidth(nWidth);
				((LayoutBase<GraphContext>*)(m_vectWidget[nIndex]))->SetMaxWidth(nWidth);
			}
		}

		void SetCellHeight(int nHeight)
		{
			m_nCellHeight = nHeight;
		}
		void SetCellHeight(int nIndex, int nHeight)
		{
			if(((size_t)(nIndex)) < m_vectWidget.size())
			{
				((LayoutBase<GraphContext>*)(m_vectWidget[nIndex]))->SetMinHeight(nHeight);
				((LayoutBase<GraphContext>*)(m_vectWidget[nIndex]))->SetMaxHeight(nHeight);
			}
		}
		void SetCell(int nWidth, int nHeight)
		{
			m_nCellWidth = nWidth;
			m_nCellHeight = nHeight;
		}
		void SetCell(int nIndex, int nWidth, int nHeight)
		{
			SetCellWidth(nIndex,nWidth);
			SetCellHeight(nIndex,nHeight);
		}
		int GetCellWidth(void)const {return m_nCellWidth;}
		int GetCellHeight(void)const {return m_nCellHeight;}
		void SetReverse(bool bReverse = true)
		{
			m_bReverse = bReverse;
		}
		void FixAllItemSize(int nWidth, int nHeight)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				(*iter)->SetMinWidth(nWidth);
				(*iter)->SetMaxWidth(nWidth);
				(*iter)->SetMinHeight(nHeight);
				(*iter)->SetMaxHeight(nHeight);
			}
		}
		bool FixItemSize(int nIndex, int nWidth, int nHeight)
		{
			bool bRes = false;
			if(nIndex < m_vectWidget.size() )
			{
				m_vectWidget[nIndex]->SetMinWidth(nWidth);
				m_vectWidget[nIndex]->SetMaxWidth(nWidth);
				m_vectWidget[nIndex]->SetMinHeight(nHeight);
				m_vectWidget[nIndex]->SetMaxHeight(nHeight);
				bRes = true;
			}
			return bRes;
		}
		bool FixItemWidth(int nIndex, int nWidth)
		{
			bool bRes = false;
			if(nIndex < m_vectWidget.size() )
			{
				m_vectWidget[nIndex]->SetMinWidth(nWidth);
				m_vectWidget[nIndex]->SetMaxWidth(nWidth);
				bRes = true;
			}
			return bRes;
		}
		bool FixItemHeight(int nIndex, int nHeight)
		{
			bool bRes = false;
			if(nIndex < static_cast<int>(m_vectWidget.size()) )
			{
				m_vectWidget[nIndex]->SetMinHeight(nHeight);
				m_vectWidget[nIndex]->SetMaxHeight(nHeight);
				bRes = true;
			}
			return bRes;
		}
	public:
		virtual int GetMinWidth(){return GetMinBoxWidth();}
		virtual int GetMinHeight(){return GetMinBoxHeight();}
		virtual bool SetGeometry(int nX,int nY,int nWidth,int nHeight)
		{
			bool bRes = true;
			
			//int nDefaultCell = nWidth > nHeight ? nHeight : nWidth; 
			int nOrgHeight = m_nCellHeight;
			int nOrgWidth = m_nCellWidth;
			//m_nCellHeight = m_nCellHeight == 0 ? nDefaultCell : m_nCellHeight;
			//m_nCellWidth = m_nCellWidth == 0 ? nDefaultCell : m_nCellWidth;
				
			int nMinBoxWidth =  GetMinBoxWidth();
			int nMinBoxHeight = GetMinBoxHeight();
			if(nWidth < nMinBoxWidth)
			{
				bRes = false;
				nWidth = nMinBoxWidth;
			}
			if(nHeight < nMinBoxHeight)
			{
				bRes = false;
				nHeight = nMinBoxHeight;
			}

			if(Widge<GraphContext>::SetGeometry(nX, nY, nWidth, nHeight))
			{				
				typedef Widgets::iterator								BasicIter;
				typedef Widgets::reverse_iterator				rBasicIter;
				int nBoxX = nX;
				int nBoxY = nY;
				if(m_nAlignment == Default){;}//照初始設定
				else
				{
					if(m_nCellWidth!=0)
					{
						if(m_nAlignment & Left){;}
						else if(m_nAlignment & Right)
							nBoxX += nWidth - nMinBoxWidth;
						else if(m_nAlignment & HCenter)
							nBoxX += (nWidth - nMinBoxWidth)/2;
					}
					if(m_nCellHeight!=0)
					{
						if(m_nAlignment & Top){;}
						else if(m_nAlignment & Bottom)
							nBoxY += nHeight - nMinBoxHeight;
						else if(m_nAlignment & VCenter)
							nBoxY += (nHeight - nMinBoxHeight)/2;
					}
				}
				if(m_bReverse)
					SetItemGeometry<rBasicIter>(nBoxX, nBoxY, nWidth, nHeight, m_vectWidget.rbegin(), m_vectWidget.rend());
				else
					SetItemGeometry<BasicIter>(nBoxX, nBoxY, nWidth, nHeight, m_vectWidget.begin(), m_vectWidget.end());
			}

			m_nCellHeight = nOrgHeight == 0 ? 0 : m_nCellHeight;
			m_nCellWidth = nOrgWidth == 0 ? 0 : m_nCellWidth;

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
				for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
				{
					if((*iter))
						bRes = (*iter)->SetLayout();
				}
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
			Property.addAttribute(_T("Direction"), m_nDirection);
			Property.addAttribute(_T("Spacing"), m_nSpacing);
			Property.addAttribute(_T("CellWidth"), m_nCellWidth);
			Property.addAttribute(_T("CellHeight"), m_nCellHeight);
			if(m_bReverse)  // we don't print it if there is no reverse
				Property.addAttribute(_T("Reverse"), 1);
			Property.addAttribute(_T("Alignment"), GetAlignment(m_nAlignment));
			Property.addAttribute(_T("CommonTopMargin"), m_nCommonTopMargin);
			Property.addAttribute(_T("CommonBottomMargin"), m_nCommonBottomMargin);
			Property.addAttribute(_T("CommonLeftMargin"), m_nCommonLeftMargin);
			Property.addAttribute(_T("CommonRightMargin"), m_nCommonRightMargin);
			Element.getAllElement()->push_back(Property);
			MainElement.getAllElement()->push_back(Element);

			FillOrderSetting(EleOrder, strID);
			int nIndex = 0;
			for(Widgets::iterator iter = m_vectWidget.begin(); iter != m_vectWidget.end() && bRes; ++iter)
			{
				AddOrderSetting(EleOrder, strID);
				((LayoutBase<GraphContext>*)(*iter))->AddLayoutBaseSetting(EleLayoutBase, strID, nIndex);
				bRes = (*iter)->Save(MainElement, EleOrder, EleLayoutBase);
				++nIndex;
			}
	
			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element) // Set Property
		{
			bool bRes = false;
			if(Element.name() == g_strProperty)
			{
				m_nDirection = Element.getAttr(_T("Direction"));
				m_nDirection = m_nDirection == -1 ? LeftToRight : m_nDirection;
				m_nSpacing = Element.getAttr(_T("Spacing"));
				m_nSpacing = m_nSpacing == -1 ? 0 : m_nSpacing;
				m_nCellWidth = Element.getAttr(_T("CellWidth"));
				m_nCellWidth = m_nCellWidth == -1 ? 0 : m_nCellWidth;
				m_nCellHeight = Element.getAttr(_T("CellHeight"));
				m_nCellHeight = m_nCellHeight == -1 ? 0 : m_nCellHeight;
				TSTRING strReverse;
				if(Element.getAttr(_T("Reverse"), strReverse))
					m_bReverse = true;

				TSTRING strAlignment;
				Element.getAttr(_T("Alignment"), strAlignment);
				if(strAlignment != _T(""))
				{
					vector<TSTRING> vstrAlignment;
					Tokenize<TSTRING>(_T("|"), strAlignment,vstrAlignment,false);
					m_nAlignment = 0;
					for(vector<TSTRING>::iterator iter=vstrAlignment.begin(); iter!= vstrAlignment.end(); ++iter)
					{
						if((*iter) == _T("Left"))
							m_nAlignment |= Left;
						else if((*iter) == _T("HCenter"))
							m_nAlignment |= HCenter;
						else if((*iter) == _T("Right"))
							m_nAlignment |= Right;
						else if((*iter) == _T("Top"))
							m_nAlignment |= Top;
						else if((*iter) == _T("VCenter"))
							m_nAlignment |= VCenter;
						else if((*iter) == _T("Bottom"))
							m_nAlignment |= Bottom;
					}
				}
				else
					m_nAlignment = Default;
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
		int GetMinBoxWidth()
		{
			int nMin = 0;
			if(m_nDirection == LeftToRight || m_nDirection == RightToLeft)
			{
				for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
				{
					if((*iter)->GetMinWidth() != -1)
						nMin += (*iter)->GetMinWidth();
					nMin += m_nSpacing;
				}
				nMin -= m_nSpacing;
				int nRealWidth = (m_nCellWidth+m_nSpacing)*(int)(m_vectWidget.size())-m_nSpacing;
				nMin = nMin < nRealWidth ? nRealWidth : nMin;
			}
			else if(m_nDirection == TopToBottom || m_nDirection == BottomToTop )
			{
				for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
				{
					if((*iter)->GetMinWidth() > nMin)
						nMin = (*iter)->GetMinWidth();
				}
				nMin = nMin < m_nCellWidth ? m_nCellWidth : nMin;
			}
			
			return nMin;
		}
		int GetMinBoxHeight()
		{
			int nMin = 0;
			if(m_nDirection == LeftToRight || m_nDirection == RightToLeft)
			{
				for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
				{
					if((*iter)->GetMinWidth() > nMin)
					nMin = (*iter)->GetMinHeight();
				}
				nMin = nMin < m_nCellHeight ? m_nCellHeight : nMin;
			}
			else if(m_nDirection == TopToBottom || m_nDirection == BottomToTop )
			{
				for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
				{
					if((*iter)->GetMinHeight() != -1)
						nMin += (*iter)->GetMinHeight();
					nMin += m_nSpacing;
				}
				nMin -= m_nSpacing;
				nMin = nMin < m_nCellHeight*(int)(m_vectWidget.size()) ? m_nCellHeight*(int)(m_vectWidget.size()) : nMin;
			}
			
			return nMin;
		}
		template<class BasicIter>
		void SetItemGeometry(int nX,int nY,int nWidth,int nHeight, BasicIter iterBegin, BasicIter iterEnd)
		{
			if(m_nDirection == LeftToRight)
			{
				int nStretch = 0;
				for(BasicIter iter = iterBegin; iter < iterEnd; ++iter)
				{
					int nItemWidth = (*iter)->GetMinWidth() > m_nCellWidth ? (*iter)->GetMinWidth():m_nCellWidth;
					int nItemHeight = m_nCellHeight > (*iter)->GetMinHeight() ? m_nCellHeight : (*iter)->GetMinHeight();
					nItemHeight = m_nCellHeight != 0 ? nItemHeight : nHeight;
					(*iter)->SetGeometry((nX + nStretch), nY, nItemWidth, nItemHeight);
					nStretch += (nItemWidth + m_nSpacing);
				}		
			}
			else if(m_nDirection == RightToLeft)
			{
				int nStretch = 0;
				for(BasicIter iter = iterBegin; iter < iterEnd; ++iter)
				{
					int nItemWidth = (*iter)->GetMinWidth() > m_nCellWidth ? (*iter)->GetMinWidth():m_nCellWidth;
					int nItemHeight = m_nCellHeight > (*iter)->GetMinHeight() ? m_nCellHeight : (*iter)->GetMinHeight();
					nItemHeight = m_nCellHeight != 0 ? nItemHeight : nHeight;
					nStretch += nItemWidth;
					(*iter)->SetGeometry( (nX+nWidth-nStretch), nY, nItemWidth, nItemHeight);
					nStretch += m_nSpacing;
				}		
			}
			else if(m_nDirection == TopToBottom)
			{
				int nStretch = 0;
				for(BasicIter iter = iterBegin; iter < iterEnd; ++iter)
				{
					int nItemHeight = (*iter)->GetMinHeight() > m_nCellHeight ? (*iter)->GetMinHeight():m_nCellHeight;
					int nItemWidth = m_nCellWidth > (*iter)->GetMinWidth() ? m_nCellWidth : (*iter)->GetMinWidth();
					nItemWidth = m_nCellWidth != 0 ? nItemWidth : nWidth;
					(*iter)->SetGeometry(nX, (nY + nStretch), nItemWidth, nItemHeight);
					nStretch += (nItemHeight+m_nSpacing);
				}
			}
			else if(m_nDirection == BottomToTop)
			{
				int nStretch = 0;
				for(BasicIter iter = iterBegin; iter < iterEnd; ++iter)
				{
					int nItemHeight = (*iter)->GetMinHeight() > m_nCellHeight ? (*iter)->GetMinHeight():m_nCellHeight;
					int nItemWidth = m_nCellWidth > (*iter)->GetMinWidth() ? m_nCellWidth : (*iter)->GetMinWidth();
					nItemWidth = m_nCellWidth != 0 ? nItemWidth : nWidth;
					nStretch += nItemHeight;
					(*iter)->SetGeometry(nX, (nY+nHeight-nStretch),  nItemWidth, nItemHeight);
					nStretch += m_nSpacing;
				}	
			}
		}


	private:
		int m_nSpacing;
		int m_nDirection;
		int m_nCellWidth;
		int m_nCellHeight;
		int m_nAlignment;
		bool m_bReverse;
		TSTRING m_strClassName;

		int											m_nCommonTopMargin;
		int											m_nCommonBottomMargin;
		int											m_nCommonLeftMargin;
		int											m_nCommonRightMargin;
	};
}

#endif //_SplitLayout_Header_