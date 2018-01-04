#include "stdafx.h"
#include "PIMConfigure.h"

IPIMCONFIG *g_pSingleton = NULL;

CDCINTERFACE_CLASS_DLLEXT IPIMCONFIG* CreatePIMConfigure(const TSTRING& strModalName)
{
    if(g_pSingleton == NULL)
		g_pSingleton = new CPIMCONFIGURE(strModalName);
	return g_pSingleton;
}

CDCINTERFACE_CLASS_DLLEXT void DeletePIMConfigure()
{
    if(g_pSingleton)
	{
		delete g_pSingleton;
		g_pSingleton = NULL;
	}    
}

