// LC.h : main header file for the LC application
//

#if !defined(AFX_LC_H__D4CF4392_5EC0_4EC1_B01E_051A03D7AAAC__INCLUDED_)
#define AFX_LC_H__D4CF4392_5EC0_4EC1_B01E_051A03D7AAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLCApp:
// See LC.cpp for the implementation of this class
//

class CLCApp : public CWinApp
{
public:
	CLCApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLCApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLCApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LC_H__D4CF4392_5EC0_4EC1_B01E_051A03D7AAAC__INCLUDED_)
