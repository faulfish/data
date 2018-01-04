// Calendar_Version2.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#ifdef _MANAGED
#error Please read instructions in Calendar_Version2.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//	1. Remove the above include for afxdllx.h
//	2. Add a .cpp file to your project that does not have /clr thrown and has
//	   Precompiled headers disabled, with the following text:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#include "Calendar_V2View.h"
#include "..\..\CommonUtil\QSyncerFrameworks.h"

static AFX_EXTENSION_MODULE Calendar_Version2DLL = { NULL, NULL };
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
		TRACE0("Calendar_Version2.DLL Initializing!\n");
		
		g_pQSyncerFramework = NULL;
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(Calendar_Version2DLL, hInstance))
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

		new CDynLinkLibrary(Calendar_Version2DLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("Calendar_Version2.DLL Terminating!\n");

		if(g_pQSyncerFramework)
		{
			delete g_pQSyncerFramework;
			g_pQSyncerFramework = NULL;
		}
		// Terminate the library before destructors are called
		AfxTermExtensionModule(Calendar_Version2DLL);
	}
	return 1;   // ok
}

NotifyReceiver g_NotifyReceiver;

extern "C" AFX_EXT_API UINT GetQSyncerView(QSYNC_LIB::QSyncerFramework** ppQSyncerFramework)
{
	using namespace QSYNC_LIB;
	ILocalization*  m_pILocalization = NULL;
	CreateILocalization(&m_pILocalization);
    // Replace YourDLLName with the AFX_EXTENSION_MODULE variable above 
    // your DllMain.
    new CDynLinkLibrary(Calendar_Version2DLL);	
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
	*ppQSyncerFramework = new QSyncerFramework(_T("Calendar"),RUNTIME_CLASS(CCalendar_V2View),&g_NotifyReceiver);
	if(g_pQSyncerFramework == NULL)
		g_pQSyncerFramework = *ppQSyncerFramework;
	if(m_pILocalization)
		g_pQSyncerFramework->m_strDisplayName = _T("IDS_CALENDAR_FUNCTION");
    
    return(0);
}


#ifdef _MANAGED
#pragma managed(pop)
#endif

