/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   Message_Version2.h  $
 *  $Revision:   1.1  $
 *  $Date:   Nov 09 2007 13:02:44  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 *
 *
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/Message_Version2/Message_Version2.h.-arc  $
 * 
 *    Rev 1.1   Nov 09 2007 13:02:44   Eddy Guo
 * add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file Message_Version2.h
    @brief {brief description of Message_Version2.h}

    {detail description of Message_Version2.h}
*/
// Message_Version2.h : main header file for the Message_Version2 application
//
#pragma once
 
#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif
 
#include "resource.h"       // main symbols
 
 
// CMessage_Version2App:
// See Message_Version2.cpp for the implementation of this class
//
 
class CMessage_Version2App : public CWinApp
{
public:
	CMessage_Version2App();
 
 
// Overrides
public:
	virtual BOOL InitInstance();
 
// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};
 
extern CMessage_Version2App theApp;
