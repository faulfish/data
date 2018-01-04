/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - BEGIN >>> *** */
/***************************************************************************
 *
 *                           QSyncher V2 Project
 *
 *	             Copyright (c) 2005 BENQ Corpration
 *
 ***************************************************************************
 *
 *  $Workfile:   BackupRestoreDLL.cpp  $
 *  $Revision:   1.1  $
 *  $Date:   Nov 09 2007 13:04:30  $
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
 * $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/Code/Dll/BackupRestore/BackupRestoreDLL.cpp.-arc  $
   
      Rev 1.1   Nov 09 2007 13:04:30   Eddy Guo
   add header
 *
 *
 ***************************************************************************/
/* *** <<< DO NOT MODIFY OR REMOVE THIS LINE - END >>> *** */
/*! @file BackupRestoreDLL.cpp
    @brief {brief description of BackupRestoreDLL.cpp}

    {detail description of BackupRestoreDLL.cpp}
*/
#include "stdafx.h"
#include <afxdllx.h>
 
#ifdef _MANAGED
#error Please read instructions in FileManager_Version2.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//	1. Remove the above include for afxdllx.h
//	2. Add a .cpp file to your project that does not have /clr thrown and has
//	   Precompiled headers disabled, with the following text:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif
#include "..\..\CommonUtil\QSyncerFrameworks.h"
#include "BackupRestoreView.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
static AFX_EXTENSION_MODULE Backup_Restore = { NULL, NULL };
QSYNC_LIB::QSyncerFramework* g_pQSyncerFramework;
 
#ifdef _MANAGED
#pragma managed(push, off)
#endif
 
extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);
 
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("BackupRestore.DLL Initializing!\n");
 
		g_pQSyncerFramework = NULL;
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(Backup_Restore, hInstance))
			return 0;
 
		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.
 
		new CDynLinkLibrary(Backup_Restore);
 
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("BackupRestore.DLL Terminating!\n");
 
		if(g_pQSyncerFramework)
		{
			delete g_pQSyncerFramework;
			g_pQSyncerFramework = NULL;
		}
		// Terminate the library before destructors are called
		AfxTermExtensionModule(Backup_Restore);
	}
	return 1;   // ok
}
 
QSYNC_LIB::NotifyReceiver g_NotifyReceiver;
 
extern "C" AFX_EXT_API UINT GetQSyncerView(QSYNC_LIB::QSyncerFramework** ppQSyncerFramework)
{
	using namespace QSYNC_LIB;
	ILocalization*  pILocalization = NULL;
	CreateILocalization(&pILocalization);
    // Replace YourDLLName with the AFX_EXTENSION_MODULE variable above
    // your DllMain.
    new CDynLinkLibrary(Backup_Restore);
    // Replace CYourClass with the name of CView derived class you are exposing.
	/*
	<Applications>
       <AppDefault>
           <Settings		ID="Phonebook" 			Value="PhonebookV2" 		Description="Phonebook Application" />
           <Settings 		ID="Message" 			Value="MessageV2" 			Description="Message Application"/>
           <Settings 		ID="Calendar" 			Value="CalendarV2" 			Description="Calendar Application"/>
           <Settings 		ID="PIMSync" 			Value="OutlookSync" 		Description="PIMSync Application"/>
           <Settings 		ID="FileManager" 		Value="FileManagerV2" 	Description="FileManager Application"/>
           <Settings 		ID="BackupRestore" 		Value="BackupRestoreV2" Description="BackupRestore Application"/>
           <Settings 		ID="Options" 			Value="Options" 				Description="Options Application"/>
       </AppDefault>
	</Applications>
	*/
	//ID must be the value on the configuration
	*ppQSyncerFramework = new QSyncerFramework(_T("BackupRestore"),RUNTIME_CLASS(CBackupRestoreView),&g_NotifyReceiver);
	if(g_pQSyncerFramework == NULL)
		g_pQSyncerFramework = *ppQSyncerFramework;
	if(pILocalization)
		g_pQSyncerFramework->m_strDisplayName = _T("IDS_BACKUP_RESTORE_FUNCTION");
 
    return(0);
}
 
#ifdef _MANAGED
#pragma managed(pop)
#endif
 
