#include "StdAfx.h"
#include "RequestManagerExport.h"
#include "RequestManagerInstance.h"
#include "RequestManager.h"

namespace PLATFORM_TOOL
{
	static CRequestManager g_CRequestManager;

	RM_API bool GetIRequestManager(CIRequestManagerPtr& ptIRequestManager,int nCOMPOrt, const std::string& strPlatform)
	{
		ptIRequestManager = new CRequestManagerInstance(nCOMPOrt,&g_CRequestManager,strPlatform);
		return true;
	}
}