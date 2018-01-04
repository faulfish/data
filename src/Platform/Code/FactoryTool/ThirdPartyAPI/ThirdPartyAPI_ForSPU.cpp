#include "stdafx.h"
#include "ThirdPartyAPI_ForSPU.h"
#include "../../ShareLib\BTW DK\SDK\Inc\BtIfClasses.h"
#include "../../CommonUtil\unicodeutility.h"
#include <cassert>

bool TransferCharToBool(const char* szBool)
{
	CStringA strb = szBool;
	strb.MakeLower();
	bool bRes = (strb == "true") ||  (strb == "t");
	return bRes;
}

void TransferBoolToChar(bool b, char* sz)
{
	CStringA strTemp = b ? "true" : "false";
	memcpy(sz, strTemp, strTemp.GetLength()+1);
}

void ba2str( BD_ADDR ba, char *addr )
{
	sprintf_s(addr,12, "%02X%02X%02X%02X%02X%02X",ba[0], ba[1], ba[2], ba[3], ba[4], ba[5] );
}

void str2ba( char *addr ,BD_ADDR& ba)
{
	assert(strlen(addr) == 12);
	for(int i=0; i<6; i++ ) 
	{
		char szHEX[3] = {addr[i*2],addr[i*2+1],0};
		ba[i] = QSYNC_LIB::HEX2Ineger(szHEX);
	}
}

THIRDPARTYAPI_API bool BTW_UnBond(char * szBTAddr)
{
	WIDCOMMSDK_Init();
	//if(IsBroadcomStack())
	{
		CBtIf bluetooth;
		BD_ADDR ba = {0};
		str2ba(szBTAddr,ba);
		bluetooth.UnBond(ba);
	}
	WIDCOMMSDK_ShutDown();
	return true;
}

THIRDPARTYAPI_API bool BTW_BondReply(char * szPin)
{
	WIDCOMMSDK_Init();
	//if(IsBroadcomStack())
	{
		CBtIf bluetooth;
		UINT32 nSize = szPin == NULL ?  0 : static_cast<UINT32>(strlen(szPin));
		bluetooth.BondReply(CBtIf::BOND_PIN_ALLOW,nSize,(UCHAR *)szPin);
	}
	WIDCOMMSDK_ShutDown();
	return true;
}
