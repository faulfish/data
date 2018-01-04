/////////////////////////////////////////////////////////////////////////////
//
//  Name        BlueComChat.h
//  $Header:
//
//  Function    main header file for the BLUECOMCHAT application
//              
//
//  Date                         Modification
//  -----------------------------------------
//  22Mar2001   Jim French      Create
//
//  Copyright (c) 2000-2006, Broadcom Corporation, All Rights Reserved.
//
//////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLUECOMCHAT_H__CF7DFA6F_1EFE_402E_B1C2_4390676D8AC8__INCLUDED_)
#define AFX_BLUECOMCHAT_H__CF7DFA6F_1EFE_402E_B1C2_4390676D8AC8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBlueComChatApp:
// See BlueComChat.cpp for the implementation of this class
//

class CBlueComChatApp : public CWinApp
{
public:
	CBlueComChatApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlueComChatApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBlueComChatApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLUECOMCHAT_H__CF7DFA6F_1EFE_402E_B1C2_4390676D8AC8__INCLUDED_)
