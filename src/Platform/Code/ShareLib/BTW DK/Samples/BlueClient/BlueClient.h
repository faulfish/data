// BlueClient.h : main header file for the BLUECLIENT application
//
//  Copyright (c) 2000-2004, Broadcom Corporation, All Rights Reserved.

#if !defined(AFX_BLUECLIENT_H__16AD7A47_3EF6_4C9E_83AC_FB83050B3868__INCLUDED_)
#define AFX_BLUECLIENT_H__16AD7A47_3EF6_4C9E_83AC_FB83050B3868__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBlueClientApp:
// See BlueClient.cpp for the implementation of this class
//

class CBlueClientApp : public CWinApp
{
public:
	CBlueClientApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBlueClientApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUECLIENT_H__16AD7A47_3EF6_4C9E_83AC_FB83050B3868__INCLUDED_)
