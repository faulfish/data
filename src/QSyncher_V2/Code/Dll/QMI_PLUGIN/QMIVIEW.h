#pragma once

#include "..\..\Components\UIFramework\LayoutManager\Win32FrameworkWidge.h"

namespace QSYNC_LIB
{
	class NotifyReceiver : public QSYNC_LIB::INotify
	{
	public:
		NotifyReceiver():m_pINotify(NULL){}
		virtual void Event(const TSTRING& strEvent,long nParam)
		{
			if(m_pINotify)
				m_pINotify->Event(strEvent,nParam);
		}
		QSYNC_LIB::INotify* m_pINotify;
	};

	typedef Win32DoubleBufferFormCWndWidge<CView>			Win32DoubleBufferViewWidge;

	class CQMIVIEW : public Win32DoubleBufferViewWidge
	{
		DECLARE_DYNCREATE(CQMIVIEW)

	protected:
		CQMIVIEW();           // protected constructor used by dynamic creation
		virtual ~CQMIVIEW();

	public:
		virtual void OnDraw(CDC* pDC);      // overridden to draw this view
#ifdef _DEBUG
		virtual void AssertValid() const;
#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	protected:
		DECLARE_MESSAGE_MAP()
	public:
		virtual void OnInitialUpdate();
	};
}

