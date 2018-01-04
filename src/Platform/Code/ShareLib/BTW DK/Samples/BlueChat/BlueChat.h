/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueChat.h
//  $Header:
//
//  Function    main header file for the BLUECHAT application
//              
//
//  Date                         Modification
//  -----------------------------------------
//  12/22/2000    P Chaisson     Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLUECHAT_H__CF7DFA6F_1EFE_402E_B1C2_4390676D8AC8__INCLUDED_)
#define AFX_BLUECHAT_H__CF7DFA6F_1EFE_402E_B1C2_4390676D8AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBlueChatApp:
// See BlueChat.cpp for the implementation of this class
//

class CBlueChatApp : public CWinApp
{
public:
	CBlueChatApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueChatApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBlueChatApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUECHAT_H__CF7DFA6F_1EFE_402E_B1C2_4390676D8AC8__INCLUDED_)
