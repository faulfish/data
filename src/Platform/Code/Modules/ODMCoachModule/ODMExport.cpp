#include "StdAfx.h"
#include "ODMEXport.h"
#include "ODM.h"

ODM_COACH_API bool GetMotoXprs_Athens(TCHAR* szPATH,int nLen,int nPort, int nTimeout)
{
	bool bRes = false;
	CODM odm(nPort);
	bRes = odm.GetMotoXprsLogger_Athens(szPATH,nLen);
	return bRes;
}

ODM_COACH_API bool GetMotoXprs_Lavernock(TCHAR* szPATH,int nLen,int nPort, int nTimeout)
{
	bool bRes = false;
	CODM odm(nPort);
	bRes = odm.GetMotoXprsLogger_Lavernock(szPATH,nLen);
	return bRes;
}

ODM_COACH_API bool GetMotoXprs_Melos(TCHAR* szPATH,int nLen,int nPort, int nTimeout)
{
	bool bRes = false;
	CODM odm(nPort);
	bRes = odm.GetMotoXprsLogger_Melos(szPATH,nLen);
	return bRes;
}