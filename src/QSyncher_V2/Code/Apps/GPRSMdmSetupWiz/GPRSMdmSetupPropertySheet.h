/* *** THIS LINE MUST BE THE FIRST LINE AND DO NOT MODIFY OR REMOVE IT *** */
/***************************************************************************
 *
 *               BenQ QSyncher Project
 *
 *	   Copyright (C) 2005 BenQ Corp.
 *
 ***************************************************************************
 *
 *  $Workfile:   GPRSMdmSetupPropertySheet.h  $
 *  $Revision:   1.2  $
 *  $Date:   Dec 10 2007 11:45:56  $
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
 /*$Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Apps/GPRSMdmSetupWiz/GPRSMdmSetupPropertySheet.h.-arc  $
 * 
 *    Rev 1.2   Dec 10 2007 11:45:56   Eddy Guo
 * update
 * 
 *    Rev 1.1   Dec 04 2007 17:35:16   Eddy Guo
 * update
 * 
 *    Rev 1.0   Sep 12 2007 10:44:24   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.0   Sep 15 2005 08:54:22   Jane Yu
 * Initial revision.
  
   */
/**************************************************************************/
/* *** <<<DO NOT MODIFY OR REMOVE THIS LINE>>> *** */

//===========================================================================//
//  Author:  Steven CC Chen <SteveCCChen@BenQ.com>                           //
//  Release Date:  2005/09/13                                                //
//                                                                           //
//  Copyright(C) 2005  All rights reserved.                                  //
//===========================================================================//

// GPRSMdmSetupPropertySheet.h : header file
//
// This class defines custom modal property sheet 
// CGPRSMdmSetupPropertySheet.

#ifndef __GPRSMDMSETUPPROPERTYSHEET_H__
#define __GPRSMDMSETUPPROPERTYSHEET_H__

#pragma once

#include "GPRSMdmSetupPropertyPages.h"



/////////////////////////////////////////////////////////////////////////////
// CGPRSMdmSetupPropertySheet

class CGPRSMdmSetupPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CGPRSMdmSetupPropertySheet)
	
// Construction
public:
	CGPRSMdmSetupPropertySheet(CWnd* pWndParent = NULL);
	
// Attributes
public:
	CGPRSMdmSetupPropertyPage1		m_Page1;
	CGPRSMdmSetupPropertyPage2		m_Page2;
	CGPRSMdmSetupPropertyPage3		m_Page3;
	CGPRSMdmSetupPropertyPage4		m_Page4;
	CGPRSMdmSetupPropertyPage5		m_Page5;
	
// Operations
public:
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGPRSMdmSetupPropertySheet)
	public:
	virtual BOOL					OnInitDialog();
	//}}AFX_VIRTUAL
	
// Implementation
public:
	virtual ~CGPRSMdmSetupPropertySheet();
	
// Generated message map functions
protected:
	//{{AFX_MSG(CGPRSMdmSetupPropertySheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

#endif	// __GPRSMDMSETUPPROPERTYSHEET_H__
