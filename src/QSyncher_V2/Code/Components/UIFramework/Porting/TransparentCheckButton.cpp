// TransparentCheckButton.cpp : implementation file
//

#include "stdafx.h"
#include "TransparentCheckButton.h"

namespace QSYNC_LIB 
{
	// CTransparentCheckButton

	IMPLEMENT_DYNAMIC(CTransparentCheckButton, CButton)

		CTransparentCheckButton::CTransparentCheckButton()
	{
#ifndef _WIN32_WCE
		EnableActiveAccessibility();
#endif

	}

	CTransparentCheckButton::~CTransparentCheckButton()
	{
	}


	BEGIN_MESSAGE_MAP(CTransparentCheckButton, CButton)
		ON_WM_ERASEBKGND()
	END_MESSAGE_MAP()



	// CTransparentCheckButton message handlers



	BOOL CTransparentCheckButton::OnEraseBkgnd(CDC* pDC)
	{
		return TRUE;
	}

}