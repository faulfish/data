// QMI_PLUGIN.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#ifdef _MANAGED
#error Please read instructions in QMI_PLUGIN.cpp to compile with /clr
// If you want to add /clr to your project you must do the following:
//	1. Remove the above include for afxdllx.h
//	2. Add a .cpp file to your project that does not have /clr thrown and has
//	   Precompiled headers disabled, with the following text:
//			#include <afxwin.h>
//			#include <afxdllx.h>
#endif

#include "QMIView.h"
#include "..\..\CommonUtil\QSyncerFrameworks.h"

QSYNC_LIB::QSyncerFramework* g_pQSyncerFramework;
static AFX_EXTENSION_MODULE QMI_PLUGINDLL = { NULL, NULL };

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
		TRACE0("QMI_PLUGIN.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(QMI_PLUGINDLL, hInstance))
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

		new CDynLinkLibrary(QMI_PLUGINDLL);

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("QMI_PLUGIN.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(QMI_PLUGINDLL);
	}
	return 1;   // ok
}

QSYNC_LIB::NotifyReceiver g_NotifyReceiver;

extern "C" AFX_EXT_API UINT GetQSyncerView(QSYNC_LIB::QSyncerFramework** ppQSyncerFramework)
{
	using namespace QSYNC_LIB;
	ILocalization*  m_pILocalization = NULL;
	CreateILocalization(&m_pILocalization);
    // Replace YourDLLName with the AFX_EXTENSION_MODULE variable above 
    // your DllMain.
    new CDynLinkLibrary(QMI_PLUGINDLL);	

	*ppQSyncerFramework = new QSyncerFramework(_T("QMI_PLUGIN"),RUNTIME_CLASS(CQMIVIEW),&g_NotifyReceiver);
	
	if(g_pQSyncerFramework == NULL)
		g_pQSyncerFramework = *ppQSyncerFramework;
	if(m_pILocalization)
		g_pQSyncerFramework->m_strDisplayName = _T("IDS_QMIPLUGIN_FUNCTION");
	
    
    return(0);
}
#ifdef _MANAGED
#pragma managed(pop)
#endif

