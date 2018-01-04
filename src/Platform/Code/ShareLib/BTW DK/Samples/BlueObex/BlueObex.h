// BlueObex.h : main header file for the BLUEOBEX application
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if !defined(AFX_BLUEOBEX_H__9C2B3C66_5786_4905_8F20_ECA471F4A9CD__INCLUDED_)
#define AFX_BLUEOBEX_H__9C2B3C66_5786_4905_8F20_ECA471F4A9CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBlueObexApp:
// See BlueObex.cpp for the implementation of this class
//

class CBlueObexApp : public CWinApp
{
public:
	CBlueObexApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueObexApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBlueObexApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUEOBEX_H__9C2B3C66_5786_4905_8F20_ECA471F4A9CD__INCLUDED_)
