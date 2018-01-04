#pragma once
#include "SyncManager.h"

#ifdef		PIMDB_DLL
	#define		PIMDB_DLL_DECLSPEC		__declspec(dllexport)
#else
	#define		PIMDB_DLL_DECLSPEC		__declspec(dllimport)
#endif

namespace QSYNC_LIB
{	
	PIMDB_DLL_DECLSPEC bool CreatePIMDB(const TSTRING& strDatabase,IPIMPt& ptIPIM,IPIMDBPt& ptIPIMDB);
    PIMDB_DLL_DECLSPEC void RemoveSyncManagerDevice();
}
