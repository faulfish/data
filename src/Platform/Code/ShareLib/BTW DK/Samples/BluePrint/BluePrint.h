// BluePrint.h : main header file for the BluePrint application
//

#if !defined(AFX_BLUEPRINT_H__C9DB989C_37D7_4554_B0CD_AF6D6C5E46D8__INCLUDED_)
#define AFX_BLUEPRINT_H__C9DB989C_37D7_4554_B0CD_AF6D6C5E46D8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBluePrintApp:
// See BluePrint.cpp for the implementation of this class
//

class CBluePrintApp : public CWinApp
{
public:
	CBluePrintApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBluePrintApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBluePrintApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUEPRINT_H__C9DB989C_37D7_4554_B0CD_AF6D6C5E46D8__INCLUDED_)
