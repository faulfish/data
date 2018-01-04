//It's a SplipLayout which has CWND

#ifndef _Win32SplitLayout_Header_
#define _Win32SplitLayout_Header_
#pragma once

#include "..\..\Components\UIFramework\LayoutManager\SplitLayout.h"
namespace QSYNC_LIB
{
	template <class GraphContext>
	class Win32SplitLayout : public SplitLayout <GraphContext>
	{
	public:
		Win32SplitLayout(CWnd* pWindow,bool bVSplit = true):SplitLayout(bVSplit),m_pCWnd(pWindow){}
	protected:
		virtual void Update(bool bErase = false)
		{
			if(m_pCWnd)
			{
				GetOwnRect();
				m_pCWnd->InvalidateRect(&m_OwnRect,bErase);
			}
		}
	private:
		void GetOwnRect()
		{
			GeometryRect rect = GetGeometry();
			m_OwnRect.left = rect.m_nX;
			m_OwnRect.top = rect.m_nY;
			m_OwnRect.right = rect.m_nWidth + rect.m_nX;
			m_OwnRect.bottom = rect.m_nHeight + rect.m_nY;
		};
	private:
	CWnd*	m_pCWnd;
	RECT  m_OwnRect;
	};
}

#endif