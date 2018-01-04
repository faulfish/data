// QUALCOMM_IMEI_CODING.h : main header file for the QUALCOMM_IMEI_CODING application
//

#if !defined(AFX_QUALCOMM_IMEI_CODING_H__57BF401F_B82E_41A8_B082_3FFAC7ADD7AA__INCLUDED_)
#define AFX_QUALCOMM_IMEI_CODING_H__57BF401F_B82E_41A8_B082_3FFAC7ADD7AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQUALCOMM_IMEI_CODINGApp:
// See QUALCOMM_IMEI_CODING.cpp for the implementation of this class
//

class CQUALCOMM_IMEI_CODINGApp : public CWinApp
{
public:
	CQUALCOMM_IMEI_CODINGApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQUALCOMM_IMEI_CODINGApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQUALCOMM_IMEI_CODINGApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUALCOMM_IMEI_CODING_H__57BF401F_B82E_41A8_B082_3FFAC7ADD7AA__INCLUDED_)
