#pragma once

#include "../CommUIExportAPI.h"

// CTransparentCheckButton

namespace QSYNC_LIB 
{

	class COMMONUI_API CTransparentCheckButton : public CButton
	{
		DECLARE_DYNAMIC(CTransparentCheckButton)

	public:
		CTransparentCheckButton();
		virtual ~CTransparentCheckButton();

	protected:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	};

}
