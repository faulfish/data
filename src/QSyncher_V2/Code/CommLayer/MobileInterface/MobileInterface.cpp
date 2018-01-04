// MobileInterface.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "MobileInterface.h"
#include "CommProxy.h"
#include "CommAdapter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CMobileInterfaceApp

BEGIN_MESSAGE_MAP(CApp, CWinApp)
END_MESSAGE_MAP()


// CMobileInterfaceApp construction

CApp::CApp():m_nIndex(0)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CMobileInterfaceApp object

CApp theApp;


// CMobileInterfaceApp initialization

BOOL CApp::InitInstance()
{
	CWinApp::InitInstance();
	return TRUE;
}

int CApp::AddMobileInterface(CMobileInterface* pInterface)
{
	m_nIndex++;
	m_MobileInterfaces.insert(std::pair<int,CMobileInterface*>(m_nIndex,pInterface));	
    return m_nIndex;
}

CMobileInterface* CApp::GetMobileInterface(int nID)
{
	CMobileInterface* pInterface = NULL;
	assert(nID >= 1);
	if(m_MobileInterfaces.find(nID) != m_MobileInterfaces.end())
		pInterface = m_MobileInterfaces[nID];
	return pInterface;
}

CMobileInterface* CApp::GetMobileInterfaceByIndex(int nIndex)
{
	CMobileInterface* pInterface = NULL;
	if(static_cast<int>(m_MobileInterfaces.size()) > 0 && nIndex >= 0 && static_cast<int>(m_MobileInterfaces.size()) > nIndex)
	{
		std::map<int,CMobileInterface*>::iterator Iter = m_MobileInterfaces.begin();
		advance(Iter, nIndex);
		pInterface = Iter->second;
	}
	return pInterface;
}

void CApp::RemoveMobileInterface(int nID)
{
	if(GetMobileInterface() != NULL)
	{
		delete m_MobileInterfaces[nID];
		m_MobileInterfaces.erase(m_MobileInterfaces.find(nID));
	}
    m_nIndex--;
}

int CApp::GetMobileInterfaceCount()
{
	return static_cast<int>(m_MobileInterfaces.size());
}

int CApp::ExitInstance()
{
	for(std::map<int,CMobileInterface*>::iterator Iter = m_MobileInterfaces.begin();Iter != m_MobileInterfaces.end();Iter++)
	{
		delete Iter->second;
	}
	m_MobileInterfaces.clear();
    return 0;
}