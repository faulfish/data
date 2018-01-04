#if !defined(AFX_FLASHDEVAPIDLL_H__EC2BA548_0A7F_11D4_A380_00C04F29D282__INCLUDED_)
#define AFX_FLASHDEVAPIDLL_H__EC2BA548_0A7F_11D4_A380_00C04F29D282__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: PST_FP_FlashDevApi.h
//
//   General Description: This is the header file of the main Flash Device API DLL.
//	 
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2000, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Manny Roxas	               04/06/2000      CSGceXXXXX   Initial version.
// Dindo Jao                   12/19/2005      LIBhh38363   Made the file common for both static and dynamic library.
// Norton Santos               08/07/2007      LIBff89694   Old PST related code removal
//
//
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------


/////////////////////////////////////////////////////////////////////////////
// CFlashDevApiApp
// See FlashDevApi.cpp for the implementation of this class
//

class CFlashDevApiApp 
#ifndef _LIB
: public CWinApp
#endif
{
public:
	CFlashDevApiApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashDevApiApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFlashDevApiApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
#ifndef _LIB
	DECLARE_MESSAGE_MAP()
#endif
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FLASHDEVAPIDLL_H__EC2BA548_0A7F_11D4_A380_00C04F29D282__INCLUDED_)
