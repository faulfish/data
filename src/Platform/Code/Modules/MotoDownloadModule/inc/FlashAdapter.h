// FlashAdapter.h : main header file for the FLASHADAPTER DLL
//

#if !defined(AFX_FLASHADAPTER_H__3BF35196_1C4C_4D86_AE59_10D33821254C__INCLUDED_)
#define AFX_FLASHADAPTER_H__3BF35196_1C4C_4D86_AE59_10D33821254C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif
//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: FlashAdapter.h
//
//   General Description: Main header file for the FlashAdapter DLL
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2004, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Marc Risop                  5/17/04          LIBee05617   Flash Adapter Dll Header file
// Dindo Jao                   12/19/05         LIBhh38363   Made the file common for both static and dynamic library.
// Norton Santos               08/07/2007       LIBff89694   Old PST related code removal
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
#include "FlashAdapterApi.h"

//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------
#ifdef _DEBUG
#define FLASH_ADAPTER_DLL_NAME   _T("FlashAdapter_d.dll")
#else
#define FLASH_ADAPTER_DLL_NAME   _T("FlashAdapter.dll")
#endif

#define FLASH_ADAPTER_CREATE_FUNCTION_NAME  _T("CreateFlashAdapter")
#define FLASH_ADAPTER_FREE_FUNCTION_NAME _T("FreeFlashAdapter")

#ifndef _LIB

	#ifdef FLASHADAPTERDLL
		#define FLASHADAPTERAPIEXPORT extern "C" __declspec(dllexport)
	#else
		#define FLASHADAPTERAPIEXPORT extern "C" __declspec(dllimport)
	#endif

#else

	#define FLASHADAPTERAPIEXPORT

#endif

//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------
typedef CFlashAdapterApi* (*FPCreateFlashAdapter)();
typedef void (*FPFreeFlashAdapter)();

//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------
FLASHADAPTERAPIEXPORT CFlashAdapterApi* CreateFlashAdapter();
FLASHADAPTERAPIEXPORT void FreeFlashAdapter();

//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CFlashAdapterApp
// See FlashAdapter.cpp for the implementation of this class
//

class CFlashAdapterApp 
#ifndef _LIB
: public CWinApp
#endif
{
public:
	CFlashAdapterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFlashAdapterApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CFlashAdapterApp)
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

#endif // !defined(AFX_FLASHADAPTER_H__3BF35196_1C4C_4D86_AE59_10D33821254C__INCLUDED_)
