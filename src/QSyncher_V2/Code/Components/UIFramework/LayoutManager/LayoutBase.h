#ifndef _LayoutBase_Header_
#define _LayoutBase_Header_

#pragma once
#include "TWidge.h"
#include "../../../CommonUtil/XMLManager.h"

namespace QSYNC_LIB
{
	enum Alignment
	{ 
		Default  = 0, // Left and Top
		Left = 1,
		HCenter = 2,
		Right = 4,
		Top = 8,
		VCenter = 16,
		Bottom = 32
	};
	
	struct LayoutBaseSettings
	{
		int nAlignment;
		int nTopMargin;
		int nBottomMargin;
		int nLeftMargin;
		int nRightMargin;
		int nMaxWidth;
		int nMaxHeight;
		int nMinWidth;
		int nMinHeight;
	};

#pragma once
	template <class GraphContext>
	class LayoutBaseWidge  : public Widge<GraphContext>
	{
		template <class GraphContext>
		class LayoutBase : public LayoutBaseWidge<GraphContext>
		{
		public:
			LayoutBase():m_pIWidget(NULL), m_nTopMargin(0), m_nBottomMargin(0), m_nLeftMargin(0), 
				m_nRightMargin(0), m_nAlignment(0){}
			virtual ~LayoutBase(){}
			virtual bool AddWidget(Widge* pWidget, const TSTRING& strParameter)
			{
				bool bRes = true;
				bRes = AddWidget(pWidget);
				return bRes;
			}
			virtual bool SetLayoutBase(int nIndex, const LayoutBaseSettings& Setting )
			{
				bool bRes = true;
				m_nTopMargin = Setting.nTopMargin;
				m_nBottomMargin = Setting.nBottomMargin;
				m_nLeftMargin = Setting.nLeftMargin;
				m_nRightMargin = Setting.nRightMargin;
				bRes = SetAlignment(Setting.nAlignment);

				SetMinFrame(Setting.nMinWidth, Setting.nMinHeight);
				SetMaxFrame(Setting.nMaxWidth, Setting.nMinHeight);
				return bRes;
			}
			bool AddWidget(Widge* pIWidget, int nAlignment = VCenter|HCenter)
			{
				bool bRes = false;
				if(pIWidget && SetAlignment(nAlignment))
				{
					m_vectWidget.push_back(pIWidget);
					m_pIWidget = pIWidget;
					int nMinWidth = m_pIWidget->GetMinWidth() == -1 ? 0 : m_pIWidget->GetMinWidth();
					int nMinHeight = m_pIWidget->GetMinHeight() == -1 ? 0 : m_pIWidget->GetMinHeight();
					if((nMinWidth + m_nLeftMargin + m_nRightMargin)!=0)
						this->SetMinWidth(nMinWidth + m_nLeftMargin + m_nRightMargin);
					if((nMinHeight + m_nTopMargin + m_nBottomMargin)!=0)
						this->SetMinHeight(nMinHeight + m_nTopMargin + m_nBottomMargin);
	
					bRes = true;
				}
				return bRes;
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

			void SetMinFrame(int nWidth, int nHeight)
			{
				if(nWidth != -1)
					SetMinWidth(nWidth);
				if(nHeight != -1)	
					SetMinHeight(nHeight);
			}
			void SetMaxFrame(int nWidth, int nHeight)
			{
				if(nWidth != -1)
					SetMaxWidth(nWidth);
				if(nHeight != -1)	
					SetMaxHeight(nHeight);
			}
			void SetMargin(int nTopMargin, int nBottomMargin, int nLeftMargin, int nRightMargin)
			{
				m_nTopMargin = nTopMargin;
				m_nBottomMargin = nBottomMargin;
				m_nLeftMargin = nLeftMargin;
				m_nRightMargin = nRightMargin;
			}
			int GetTopMargin(){return m_nTopMargin;}
			int GetBottomMargin(){return m_nBottomMargin;}
			int GetLeftMargin(){return m_nLeftMargin;}
			int GetRightMargin(){return m_nRightMargin;}
			Widge* GetWidget(){return m_pIWidget;}
		public:
			virtual int GetMinWidth() const
			{
				int nRes = -1;
				if(m_pIWidget)
				{
					nRes = m_pIWidget->GetMinWidth();
					nRes = nRes != -1 ? (nRes+m_nRightMargin+ m_nLeftMargin) : -1;
				}
				return nRes;
			}
			virtual int GetMinHeight() const
			{
				int nRes = -1;
				if(m_pIWidget)
				{
					nRes = m_pIWidget->GetMinHeight();
					nRes = nRes != -1 ? (nRes+m_nTopMargin+ m_nBottomMargin) : -1;
				}
				return nRes;
			}
			virtual int GetMaxWidth() const
			{
				int nRes = -1;
				//if(m_pIWidget)
				//{ 
				//	nRes = m_pIWidget->GetMaxWidth();
				//	nRes = nRes != -1 ? (nRes+m_nRightMargin+ m_nLeftMargin) : -1;
				//}
				return nRes;
			}
			virtual int GetMaxHeight() const
			{
				int nRes = -1;
				//if(m_pIWidget)
				//{
				//	nRes = m_pIWidget->GetMaxHeight();
				//	nRes = nRes != -1 ? (nRes+m_nTopMargin+ m_nBottomMargin) : -1;
				//}
				return nRes;
			}	
			virtual void SetMinWidth(int nValue)
			{
				if(m_pIWidget && (nValue - m_nLeftMargin - m_nRightMargin) != 0 && (nValue - m_nLeftMargin - m_nRightMargin) > m_pIWidget->GetMinWidth())
					m_pIWidget->SetMinWidth(nValue - m_nLeftMargin - m_nRightMargin);
				Widge<GraphContext>::SetMinWidth(nValue);
			}
			virtual void SetMinHeight(int nValue)
			{
				if(m_pIWidget && (nValue - m_nTopMargin - m_nBottomMargin)!=0 && (nValue - m_nTopMargin - m_nBottomMargin) > m_pIWidget->GetMinHeight())
					m_pIWidget->SetMinHeight(nValue - m_nTopMargin - m_nBottomMargin);
				Widge<GraphContext>::SetMinHeight(nValue);
			}
			virtual void SetMaxWidth(int nValue)
			{
				if(m_pIWidget && (nValue - m_nLeftMargin - m_nRightMargin)!=0 && (nValue - m_nLeftMargin - m_nRightMargin) > m_pIWidget->GetMaxWidth())
					m_pIWidget->SetMaxWidth(nValue - m_nLeftMargin - m_nRightMargin);
				Widge<GraphContext>::SetMaxWidth(nValue);

			}
			virtual void SetMaxHeight(int nValue)
			{
				if(m_pIWidget && (nValue - m_nTopMargin - m_nBottomMargin)!=0 && (nValue - m_nTopMargin - m_nBottomMargin) > m_pIWidget->GetMaxHeight())
					m_pIWidget->SetMaxHeight(nValue - m_nTopMargin - m_nBottomMargin);
				Widge<GraphContext>::SetMaxHeight(nValue);
			}
			virtual bool SetGeometry(int nX,int nY,int nWidth,int nHeight)
			{
				bool bRes = false;
				if(Widge<GraphContext>::SetGeometry(nX,nY,nWidth,nHeight)) //最大的先看看成不成功
					bRes = ResetGeometry(nX,nY,nWidth,nHeight);   //小的設應該要看Max/Min Width/Height
				return bRes;
			}
			virtual bool SetGeometry(GeometryRect rect)
			{
				bool bRes = false;
				if(Widge<GraphContext>::SetGeometry(rect))
					bRes = ResetGeometry(rect.m_nX,rect.m_nY,rect.m_nWidth,rect.m_nHeight);
				return bRes;
			}
			virtual bool SetLayout(Widge* pIWidget = 0)
			{
				bool bRes = true;

				if(pIWidget)
					bRes = pIWidget->SetLayout();	
				else
				{
					ResetGeometry(this->GetGeometry().m_nX,this->GetGeometry().m_nY,this->GetGeometry().m_nWidth,this->GetGeometry().m_nHeight);
					if(m_pIWidget)
						bRes = m_pIWidget->SetLayout();
				}
				return bRes;
			}
			virtual int GetID()
			{
				int nID=0;
				return nID;
			}
			virtual bool GetName(TSTRING& strName)
			{
				bool bRes=true;
				strName=_T("LayoutBase");
				return bRes;
			}
			void AddLayoutBaseSetting(XMLMANAGER::XMLElement& EleLayoutBase, const TSTRING& strID, int nIndex)
			{
				using namespace XMLMANAGER;
				XMLElement EleSetting(_T("Setting"));
				EleSetting.addAttribute(_T("ID"), strID);
				EleSetting.addAttribute(_T("Index"), nIndex);
				EleSetting.addAttribute(_T("TopMargin"), m_nTopMargin);
				EleSetting.addAttribute(_T("BottomMargin"), m_nBottomMargin);
				EleSetting.addAttribute(_T("LeftMargin"), m_nLeftMargin);
				EleSetting.addAttribute(_T("RightMargin"), m_nRightMargin);
				EleSetting.addAttribute(_T("MinWidth"), GetMinWidth());
				EleSetting.addAttribute(_T("MinHeight"), GetMinHeight());
				EleSetting.addAttribute(_T("MaxWidth"), GetMaxWidth());
				EleSetting.addAttribute(_T("MaxHeight"), GetMaxHeight());
				EleSetting.addAttribute(_T("Alignment"), GetAlignment(m_nAlignment));
				EleLayoutBase.getAllElement()->push_back(EleSetting);
			}
			virtual bool Save(XMLMANAGER::XMLElement& Element, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase)
			{
				return m_pIWidget->Save(Element, EleOrder, EleLayoutBase);
			}
			virtual bool Load(const XMLMANAGER::XMLElement& Element)
			{
				return true;
			}
		protected:
			int GetWidgetWidth(int nInnerWidth)
			{
				int nWidgetWidth = nInnerWidth;
				if(m_pIWidget && m_pIWidget->GetMaxWidth() != -1 && nInnerWidth > m_pIWidget->GetMaxWidth())
					nWidgetWidth = m_pIWidget->GetMaxWidth();
				if(m_pIWidget && m_pIWidget->GetMinWidth() != -1 && nInnerWidth < m_pIWidget->GetMinWidth())
					nWidgetWidth = m_pIWidget->GetMinWidth();
				return nWidgetWidth;
			}
			int GetWidgetHeight(int nInnerHeight)
			{
				int nWidgetHeight = nInnerHeight;
				if(m_pIWidget && m_pIWidget->GetMaxHeight() != -1 && nInnerHeight > m_pIWidget->GetMaxHeight())
					nWidgetHeight = m_pIWidget->GetMaxHeight();
				if(m_pIWidget && m_pIWidget->GetMinHeight() != -1 && nInnerHeight < m_pIWidget->GetMinHeight())
					nWidgetHeight = m_pIWidget->GetMinHeight();
				return nWidgetHeight;
			}
			int GetWidgetX(int nInnerX,int nInnerWidth,int nWidgetWidth)
			{
				//依據Aligment計算起點
				int nWidgetX = nInnerX;
				if(nInnerWidth != nWidgetWidth)
				{
					if(m_nAlignment & Left) {;}
					else if(m_nAlignment & HCenter)
					{
						nWidgetX = nInnerX + max(nInnerWidth - nWidgetWidth,0)/2;
					}
					else if(m_nAlignment & Right)
						nWidgetX = nInnerX + max(nInnerWidth - nWidgetWidth,0);
				}
				//TRACE(_T("nWidgetX:%d, nInnerX:%d, nInnerWidth:%d, nWidgetWidth:%d\n"),nWidgetX,nInnerX,nInnerWidth,nWidgetWidth);

				return nWidgetX;
			}
			int GetWidgetY(int nInnerY,int nInnerHeight,int nWidgetHeight)
			{
				int nWidgetY = nInnerY;
				if(nInnerHeight != nWidgetHeight)
				{
					//依據Aligment計算起點
					if(m_nAlignment & Top)	{;}
					else if(m_nAlignment & VCenter)
						nWidgetY = nInnerY + max(nInnerHeight - nWidgetHeight,0)/2;
					else if(m_nAlignment & Bottom)
						nWidgetY = nInnerY + max(nInnerHeight - nWidgetHeight,0);
				}
				return nWidgetY;
			}
			void GetWidgetRect(int& nWidgetX,int& nWidgetY,int& nWidgetWidth,int& nWidgetHeight,int nX,int nY,int nWidth,int nHeight)
			{
				if( (nX==0) && (nY==0) && (nWidth==0) && (nHeight==0) )
					nWidgetX = nWidgetY = nWidgetWidth = nWidgetHeight = 0;
				else
				{
					if(m_nAlignment == 0)
						m_nAlignment = 9;
					int nInnerX = nX + m_nLeftMargin;
					int nInnerY = nY + m_nTopMargin;
					int nInnerWidth = nWidth - m_nLeftMargin - m_nRightMargin;
					int nInnerHeight = nHeight - m_nTopMargin - m_nBottomMargin;
					nWidgetWidth = GetWidgetWidth(nInnerWidth);
					nWidgetHeight = GetWidgetHeight(nInnerHeight);
					nWidgetX = GetWidgetX(nInnerX,nInnerWidth,nWidgetWidth);
					nWidgetY = GetWidgetY(nInnerY,nInnerHeight,nWidgetHeight);
				}

			}
			bool ResetGeometry(int nX,int nY,int nWidth,int nHeight)  //小的設應該要看Max/Min Width/Height
			{
				bool bRes = true;

				int nWidgetX = 0;
				int nWidgetY = 0;
				int nWidgetWidth = 0;
				int nWidgetHeight = 0;
				GetWidgetRect(nWidgetX,nWidgetY,nWidgetWidth,nWidgetHeight,nX, nY, nWidth, nHeight);
				if(m_pIWidget)
					bRes = m_pIWidget->SetGeometry(nWidgetX, nWidgetY, nWidgetWidth, nWidgetHeight);
				return bRes;
			}
		private:
			Widge* m_pIWidget;

			int m_nTopMargin;
			int m_nBottomMargin;
			int m_nLeftMargin;
			int m_nRightMargin;

			int m_nAlignment;
		};
	protected:		
		LayoutBaseWidge(){}
		virtual ~LayoutBaseWidge()
		{			
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					delete (*iter);
				(*iter) = NULL;
			}
		}
	public:
		virtual bool AddWidget(Widge* pWidget, const TSTRING& strParameter) = 0;
		virtual bool SetLayoutBase(int nIndex, const LayoutBaseSettings& Setting )=0;
		virtual int GetMinWidth(){return -1;}
		virtual int GetMinHeight(){return -1;}
		virtual int GetMaxWidth(){return -1;}
		virtual int GetMaxHeight(){return -1;}
		virtual void SetMinWidth(int nValue)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->SetMinWidth(nValue);
			}
		}
		virtual void SetMinHeight(int nValue)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->SetMinHeight(nValue);
			}
		}
		virtual void SetMaxWidth(int nValue)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->SetMaxWidth(nValue);
			}
		}
		virtual void SetMaxHeight(int nValue)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->SetMaxHeight(nValue);
			}
		}

		virtual bool IsLayoutManager(){return true;}
		virtual bool IsWindow(){return false;}		
		virtual void Update(bool bErase = false){}

		virtual bool Save(XMLMANAGER::XMLElement& Element, XMLMANAGER::XMLElement& EleOrder, XMLMANAGER::XMLElement& EleLayoutBase){return true;}
		void FillOrderSetting(XMLMANAGER::XMLElement& EleOrder, const TSTRING& strID)
		{
			using namespace XMLMANAGER;
			for(XMLAllElement::iterator iter = EleOrder.getAllElement()->begin(); iter!= EleOrder.getAllElement()->end(); ++iter)
			{
				TSTRING strAttrValue;
				iter->getAttr(_T("ID"), strAttrValue);
				if(strAttrValue == _T(""))
					iter->ReplaceAttr(_T("ID"),strID);
			}
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element){return true;}
		virtual bool GetValidateRect(std::vector<GeometryRect> &rectVaildArray)
		{
			bool bRes = true;
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					bRes = (*iter)->GetValidateRect(rectVaildArray);
			}
			return bRes;
		}
		virtual void Draw(GraphContext GC,bool bErase = false)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->Draw(GC,bErase);
			}
		}
		virtual void Close()
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{				
				if((*iter))
					(*iter)->Close();
			}
		}
		virtual void MouseUp(int nCX,int nCY)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->MouseUp(nCX, nCY);
			}
		}
		virtual void MouseDown(int nCX,int nCY)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{				
				if((*iter))
					(*iter)->MouseDown(nCX, nCY);
			}
		}
		virtual void MouseMove(int nCX,int nCY)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->MouseMove(nCX, nCY);
			}
		}
		virtual void MouseDBClk(int nCX,int nCY)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->MouseDBClk(nCX, nCY);
			}
		}	
		virtual void Timer(WPARAM nTimerID)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->Timer(nTimerID);
			}
		}
		virtual void Focus(bool bFocus)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->Focus(bFocus);
			}
		}
		virtual void CaptureMouse(bool bCapture)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->CaptureMouse(bCapture);
			}
		}
		virtual void Show(bool bShow,bool bErase = false)
		{
			for(Widgets::iterator iter = m_vectWidget.begin(); iter<m_vectWidget.end(); ++iter)
			{
				if((*iter))
					(*iter)->Show(bShow,bErase);
			}
		}
		TSTRING GetAlignment(int nAlignment)
		{
			TSTRING strAlignment;
			if(Default == nAlignment)
				strAlignment = _T("Left|Top");
			else
			{
				if(nAlignment & Left)
					strAlignment = _T("Left");
				else if(nAlignment & HCenter)
					strAlignment = _T("HCenter");
				else if(nAlignment & Right)
					strAlignment = _T("Right");
				if(nAlignment & Top)
					strAlignment = StrConnect<TSTRING>(_T("|"),strAlignment, _T("Top"), false);
				else if(nAlignment & VCenter)
					strAlignment = StrConnect<TSTRING>(_T("|"), strAlignment, _T("VCenter"), false);
				else if(nAlignment & Bottom)
					strAlignment = StrConnect<TSTRING>(_T("|"), strAlignment, _T("Bottom"), false);
			}
			return strAlignment;
		}
	protected:		
		typedef std::vector< Widge* >							Widgets;
		Widgets													m_vectWidget;
	};
}

#endif  //#ifndef _LayoutBase_Header_