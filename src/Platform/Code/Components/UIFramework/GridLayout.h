#ifndef _GridLayout_Header_
#define _GridLayout_Header_

#include <vector>
#include "TWidge.h"
#pragma once

namespace QSYNC_LIB
{
	class GridLayout : public LayoutBase
	{
		struct GridLayoutData
		{
			IWidge* pIWidge;
			int nRow;
			int nColum;
			int nRowSpan;
			int nColumSpan;
		};
	public:
		GridLayout():m_nRow(0),m_nColumn(0){}
		virtual ~GridLayout()
		{
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
				delete Iter->pIWidge;
			m_GridLayoutData.clear();
		}
		void AddWidge(IWidge* pIWidge,int nRow,int nColum,int nRowSpan = 1,int nColumSpan = 1)
		{
			GridLayoutData data;

			if((nRow+nRowSpan) > m_nRow)
				m_nRow = (nRow+nRowSpan);

			if((nColum+nColumSpan) > m_nColumn)
				m_nColumn = (nColum+nColumSpan);

			data.pIWidge = pIWidge;
			data.nRow = nRow;
			data.nColum = nColum;
			data.nRowSpan = nRowSpan;
			data.nColumSpan = nColumSpan;
			m_GridLayoutData.push_back(data);
		}
	public:
		virtual bool SetLayout(IWidge* pIWidge = 0)
		{
			bool bVaild = true;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
			{				
				bool bMinVaild = true;
				bool bMaxVaild = true;
				GeometryRect rect = GetGeometryRect(Iter->nRow,Iter->nColum,Iter->nRowSpan,Iter->nColumSpan);
				if(bVaild && Iter->pIWidge->GetMinWidth() != -1)
					if(rect.m_nWidth < Iter->pIWidge->GetMinWidth())
						bMinVaild = false;
				if(bVaild && Iter->pIWidge->GetMinHeight() != -1)
					if(rect.m_nHeight < Iter->pIWidge->GetMinHeight())
						bMinVaild = false;
				if(bVaild && Iter->pIWidge->GetMaxWidth() != -1)
					if(rect.m_nWidth > Iter->pIWidge->GetMaxWidth())
						bMaxVaild = false;
				if(bVaild && Iter->pIWidge->GetMaxHeight() != -1)
					if(rect.m_nHeight > Iter->pIWidge->GetMaxHeight())
						rect.m_nHeight = Iter->pIWidge->GetMaxHeight();

				if(bMaxVaild && bMinVaild)
				{
					Iter->pIWidge->SetGeometry(rect);
					Iter->pIWidge->SetLayout();
				}
				if(bMinVaild == false)
				{
					bVaild = false;
					break;
				}
			}
			TRACE(_T("\n"));
			return bVaild;
		}
		virtual bool GetValidateRect(GeometryRect &rect)
		{
			bool bRes = false;
			for(std::vector<GridLayoutData>::iterator Iter = m_GridLayoutData.begin();Iter != m_GridLayoutData.end();Iter++)
			{
				 bRes = Iter->pIWidge->GetValidateRect(rect);
				 if(bRes)
					 break;
			}
			return bRes;
		}
	protected:
		GeometryRect GetGeometryRect(int nRow,int nColum,int nRowSpan,int nColumSpan)
		{
			GeometryRect rect;

			//For avoid precision loss, nCellWidth will grow with 100 times.
			int nCellWidth = (GetGeometry().m_nWidth*100) / m_nRow;
			int nCellHeight = (GetGeometry().m_nHeight*100) / m_nColumn;

			rect.m_nX = (nColum*nCellWidth)/100 + GetGeometry().m_nX;
			rect.m_nY = (nRow*nCellHeight)/100 + GetGeometry().m_nY;
			rect.m_nWidth = (nCellWidth*nRowSpan)/100;
			rect.m_nHeight = (nCellHeight*nColumSpan)/100;

			return rect;
		}
	private:
		std::vector<GridLayoutData>					m_GridLayoutData;
		int																m_nRow;
		int																m_nColumn;
	};
}

#endif //_GridLayout_Header_