/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2005 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   GPRSMdmSetupPropertySheet.cpp  $
 *  $Revision:   1.1  $
 *  $Date:   Dec 04 2007 17:35:16  $
 *
 ***************************************************************************
 *
 * File Description
 * ----------------
 * 
 ***************************************************************************
 *
 * Revision Details
 * ----------------
 * 
 ***************************************************************************/
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/GPRSMdmSetupWiz/GPRSMdmSetupPropertySheet.cpp.-arc  $
   
      Rev 1.1   Dec 04 2007 17:35:16   Eddy Guo
   update
   
      Rev 1.0   Sep 12 2007 10:44:24   Alan Tu
   Initial revision.
   
      Rev 1.0   Sep 15 2005 08:54:20   Jane Yu
   Initial revision.
  
   */
/**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */

//===========================================================================//
//  Author:  Steven CC Chen <SteveCCChen@BenQ.com>                           //
//  Release Date:  2005/09/13                                                //
//                                                                           //
//  Copyright(C) 2005  All rights reserved.                                  //
//===========================================================================//

// GPRSMdmSetupPropertySheet.cpp : implementation file
//

#include	"stdafx.h"
#include	"resource.h"
#include	"GPRSMdmSetupPropertySheet.h"


#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertySheet

IMPLEMENT_DYNAMIC(CGPRSMdmSetupPropertySheet, CPropertySheet)

CGPRSMdmSetupPropertySheet::CGPRSMdmSetupPropertySheet(CWnd* pWndParent) : CPropertySheet(IDS_PROPSHT_CAPTION, pWndParent)
{
	// Add all of the property pages here.  Note that
	// the order that they appear in here will be
	// the order they appear in on screen.  By default,
	// the first page of the set is the active one.
	// One way to make a different property page the 
	// active one is to call SetActivePage().
	AddPage(&m_Page1);
	AddPage(&m_Page2);
	AddPage(&m_Page3);
	AddPage(&m_Page4);
	AddPage(&m_Page5);
	
	SetWizardMode();
}


CGPRSMdmSetupPropertySheet::~CGPRSMdmSetupPropertySheet()
{
	;
}


BEGIN_MESSAGE_MAP(CGPRSMdmSetupPropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CGPRSMdmSetupPropertySheet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertySheet message handlers

//--------------------------- Steven generated codes --------------------------
BOOL CGPRSMdmSetupPropertySheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	
	//(Set up the PropertySheet Icon.)
	SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);
	
	
	return bResult;
}
//--------------------------- Steven generated codes --------------------------



