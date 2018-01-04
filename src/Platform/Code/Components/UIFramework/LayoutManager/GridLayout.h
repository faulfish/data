#ifndef _GridLayout_Header_
#define _GridLayout_Header_
#pragma once

#include "LayoutBase.h"
namespace QSYNC_LIB
{
	struct LayoutBaseSetting
	{
		LayoutBaseSetting(int nTop=0, int nBottom=0, int nLeft=0, int nRight=0, int nW=0, int nH=0, int nAlignment=0, int nFixed=0):
			nTopMargin(nTop), nBottomMargin(nBottom), nLeftMargin(nLeft), nRightMargin(nRight), 
			nMinWidth(nW), nMinHeight(nH){}	
		int nTopMargin;
		int nBottomMargin;
		int nLeftMargin;
		int nRightMargin;

		int nMinWidth;
		int nMinHeight;
	
		int nAlignment;
	};
	template <class GraphContext>
	class GridLayout : public LayoutBaseWidge<GraphContext>
	{
		struct GridLayoutData
		{
			GridLayoutData(LayoutBase<GraphContext>* pW = NULL,int nR = 0,int nC = 0,int nRSpan = 1,int nCSpan = 1):pLayoutBase(pW),nRow(nR),nColumn(nC),nRowSpan(nRSpan),nColumnSpan(nCSpan){}
			LayoutBase<GraphContext>* pLayoutBase;
			int nRow;
			int nColumn;
			int nRowSpan;
			int nColumnSpan;
		};

		
	public:
		GridLayout():m_nRow(0),m_nColumn(0),m_nCommonTopMargin(0),m_nCommonBottomMargin(0),
			m_nCommonLeftMargin(0),m_nCommonRightMargin(0),m_strClassName(_T("GridLayout")){}
		virtual ~GridLayout()
		{
			m_GridLayoutData.clear();
		}
		virtual bool AddWidget(Widge* pWidget, const TSTRING& strParameter)
		{
			bool bRes = true;
			vector<TSTRING> vstrParameters;
			Tokenize<TSTRING>(_T(","), strParameter, vstrParameters, false);
			if(vstrParameters.size() == 2)
			{
				int nRow = _ttoi(vstrParameters[0].c_str());
				int nColumn = _ttoi(vstrParameters[1].c_str());
				bRes = AddWidget(pWidget, nRow, nColumn);
			}
			else if(vstrParameters.size() == 2)
			{
				int nRow = _ttoi(vstrParameters[0].c_str());
				int nColumn = _ttoi(vstrParameters[1].c_str());
				int nRowSpan = _ttoi(vstrParameters[2].c_str());
				int nColumnSpan = _ttoi(vstrParameters[3].c_str());
				bRes = AddWidget(pWidget, nRow, nColumn, nRowSpan, nColumnSpan);
			}
			else
			{
				bRes = false;
			}
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
		bool AddWidget(Widge* pIWidget,int nRow,int nColumn,int nRowSpan = 1,int nColumnSpan = 1,int nAlignment =VCenter|HCenter)
		{
			bool bRes = false;
			if((nRow+nRowSpan) > m_nRow)
				m_nRow = (nRow+nRowSpan);

			if((nColumn+nColumnSpan) > m_nColumn)
				m_nColumn = (nColumn+nColumnSpan);
			if(pIWidget)
			{
				LayoutBase<GraphContext>* pLayoutBase = new LayoutBase<GraphContext>;
				if(pLayoutBase->AddWidget(pIWidget, nAlignment))
				{
					GridLayoutData data(pLayoutBase,nRow,nColumn,nRowSpan,nColumnSpan);
					pLayoutBase->SetMargin(m_nCommonTopMargin,m_nCommonBottomMargin,m_nCommonLeftMargin,m_nCommonRightMargin);
					m_GridLayoutData.push_back(data);
					m_vectWidget.push_back(pLayoutBase);
					bRes = true;
				}
				else
				{
					delete pLayoutBase;
				}
			}
			return bRes;
		}
		bool AddWidget(Widge* pIWidget, LayoutBaseSetting* pSetting, int nRow,int nColumn,int nRowSpan = 1,int nColumnSpan = 1)
		{
			bool bRes = false;
			if((nRow+nRowSpan) > m_nRow)
				m_nRow = (nRow+nRowSpan);

			if((nColumn+nColumnSpan) > m_nColumn)
				m_nColumn = (nColumn+nColumnSpan);
			
			if(pSetting && pIWidget)
			{
				LayoutBase<GraphContext>* pLayoutBase = new LayoutBase<GraphContext>;
				if(pLayoutBase->AddWidget(pIWidget, pSetting->nAlignment))
				{
					pLayoutBase->SetMargin(pSetting->nTopMargin, pSetting->nBottomMargin, pSetting->nLeftMargin, pSetting->nRightMargin);
					pLayoutBase->SetMinFrame(pSetting->nMinWidth, pSetting->nMinHeight);

					GridLayoutData data(pLayoutBase,nRow,nColumn,nRowSpan,nColumnSpan);
					m_GridLayoutData.push_back(data);
					bRes = true;
				}
				else
				{
					delete pLayoutBase;
				}
			}
			return bRes;
		}
		
		bool SetAlignment(int nAlignment)
		{
			bool bRes = true;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end() && bRes;Iter++)
			{
				bRes = Iter->pLayoutBase->SetAlignment(nAlignment);
			}
			return bRes;
		}
		bool SetAlignment(int nRow, int nColumn, int nAlignment)
		{
			bool bRes = false;
			LayoutBase<GraphContext>* pLayoutBase = GetLayoutBase(nRow, nColumn);
			if(pLayoutBase)
			{
				bRes = pLayoutBase->SetAlignment(nAlignment);
			}
			return bRes;
		}
		void SetMargin(int nTop, int nBottom, int nLeft, int nRight)
		{
			m_nCommonTopMargin = nTop;
			m_nCommonBottomMargin = nBottom;
			m_nCommonLeftMargin = nLeft;
			m_nCommonRightMargin = nRight;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
			{
				Iter->pLayoutBase->SetMargin(nTop, nBottom, nLeft, nRight);
			}
		}
		bool SetMargin(int nRow, int nColumn, int nTop, int nBottom, int nLeft, int nRight)
		{
			bool bRes = false;
			LayoutBase<GraphContext>* pLayoutBase = GetLayoutBase(nRow, nColumn);
			if(pLayoutBase)
			{
				pLayoutBase->SetMargin(nTop, nBottom, nLeft, nRight);
				bRes = true;
			}
			return bRes;
		}
		void SetMinSize(int nWidth, int nHeight)
		{
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
			{
				if(Iter->pLayoutBase)
					Iter->pLayoutBase->SetMinFrame(nWidth, nHeight);
			}
		}
		bool SetMinSize(int nRow, int nColumn, int nWidth, int nHeight)
		{
			bool bRes = false;
			LayoutBase<GraphContext>* pLayoutBase = GetLayoutBase(nRow, nColumn);
			if(pLayoutBase)
			{
				pLayoutBase->SetMinFrame(nWidth, nHeight);
				bRes = true;
			}
			return bRes;
		}

	public:
		virtual int GetMinWidth()
		{
			int nLayoutWidth = -1;
			int nCellWidth = -1;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
			{
				int nEachCellWidth =(Iter->pLayoutBase->GetMinWidth()*100) / Iter->nColumnSpan;
				nCellWidth = nEachCellWidth > nCellWidth ? nEachCellWidth : nCellWidth;
			}
			if(nCellWidth != -1)
				nLayoutWidth = (nCellWidth * m_nColumn + 99) / 100;
			return nLayoutWidth;
		}
		virtual int GetMinHeight()
		{
			int nLayoutHeight = -1;
			int nCellHeight = -1;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
			{
				int nEachCellHeight =(Iter->pLayoutBase->GetMinHeight()*100) / Iter->nRowSpan;
				nCellHeight = nEachCellHeight > nCellHeight ? nEachCellHeight : nCellHeight;
			}
			if(nCellHeight != -1)
				nLayoutHeight = (nCellHeight * m_nRow + 99) / 100;
			return nLayoutHeight;
		}
		virtual bool SetGeometry(int nX,int nY,int nWidth,int nHeight)
		{
			/*
			If SetGeometry return fail, it means it can't fit the size and it asign a new size automatically
			*/

			bool bRes = Widge<GraphContext>::SetGeometry(nX,nY,nWidth,nHeight);
			
			int nTotalMinCellWidth =0;
			int nTotalMinCellHeight = 0;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end() && bRes;Iter++)
			{
				GeometryRect rect = GetGeometryRect(Iter->nRow,Iter->nColumn,Iter->nRowSpan,Iter->nColumnSpan);
				
				if(Iter->pLayoutBase && false == Iter->pLayoutBase->SetGeometry(rect))
				{
					bRes = false;

					/*
					When the new cell size is not  fit the cell widge, 
					the grid layout should recover to the new size until all cells are fit of the new size
					*/
 
					//For avoid precision loss, nCellWidth will grow with 100 times.
					int nCellWidth = (Iter->pLayoutBase->GetGeometry().m_nWidth*100) / Iter->nColumnSpan;
					int nCellHeight = (Iter->pLayoutBase->GetGeometry().m_nHeight*100) / Iter->nRowSpan;

					int nLayoutWidth = (nCellWidth * m_nColumn + 99) / 100;
					int nLayoutHeight = (nCellHeight * m_nRow + 99) / 100;


					SetGeometry(0,0,nLayoutWidth,nLayoutHeight);
					break; //Avoid to cause recursive
				}
			}
			return bRes;
		}
		virtual bool SetLayout(Widge* pIWidget = 0)
		{
			bool bVaild = true;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end() && bVaild;Iter++)
			{
				if(Iter->pLayoutBase)
					Iter->pLayoutBase->SetLayout();
			}
			return bVaild;
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
			int nIndex = 0;
			for(vector<GridLayoutData>::iterator iter = m_GridLayoutData.begin(); iter != m_GridLayoutData.end() && bRes; ++iter)
			{
				AddOrderSetting(EleOrder, strID, GetOrderParemeter(iter->nRow, iter->nColumn, iter->nRowSpan, iter->nColumnSpan));
				iter->pLayoutBase->AddLayoutBaseSetting(EleLayoutBase, strID, nIndex);
				bRes = ((Widge*)iter->pLayoutBase)->Save(MainElement, EleOrder, EleLayoutBase);
				nIndex++;
			}

			return bRes;
		}
		virtual bool Load(const XMLMANAGER::XMLElement& Element)
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
	protected:
		GeometryRect GetGeometryRect(int nRow,int nColum,int nRowSpan,int nColumSpan)
		{
			GeometryRect rectOriginal = GetGeometry();
			GeometryRect rect;

			//For avoid precision loss, nCellWidth will grow with 100 times.
			int nCellWidth = (GetGeometry().m_nWidth*100) / m_nColumn;
			int nCellHeight = (GetGeometry().m_nHeight*100) / m_nRow;

			rect.m_nX = (nColum*nCellWidth + 99)/100 + GetGeometry().m_nX;
			rect.m_nY = (nRow*nCellHeight + 99)/100 + GetGeometry().m_nY;
			rect.m_nWidth = (nCellWidth*nColumSpan + 99)/100;
			rect.m_nHeight = (nCellHeight*nRowSpan + 99)/100;

			return rect;
		}
		LayoutBase<GraphContext>* GetLayoutBase(int nRow, int nColumn)
		{
			LayoutBase<GraphContext>* pWidget = NULL;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
			{
				if(Iter->nColumn == nColumn && Iter->nRow == nRow)
				{
					pWidget = Iter->pLayoutBase;
					break;
				}
			}
			return pWidget;
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
		TSTRING GetOrderParemeter(int nRow, int nColumn, int nRowSpan, int nColumnSpan)
		{
			TSTRING strRes, strTmp;
			iToTStr<TSTRING>(nRow, strTmp);
			strRes += strTmp;
			iToTStr<TSTRING>(nColumn, strTmp);
			strRes += (_T(",")+strTmp);
			iToTStr<TSTRING>(nRowSpan, strTmp);
			strRes += (_T(",")+strTmp);
			iToTStr<TSTRING>(nColumnSpan, strTmp);
			strRes += (_T(",")+strTmp);
			return strRes;
		}
	private:
		std::vector<GridLayoutData>					m_GridLayoutData;
		int											m_nRow;
		int											m_nColumn;

		int											m_nCommonTopMargin;
		int											m_nCommonBottomMargin;
		int											m_nCommonLeftMargin;
		int											m_nCommonRightMargin;
		TSTRING										m_strClassName;
	};
}

#endif //_GridLayout_Header_