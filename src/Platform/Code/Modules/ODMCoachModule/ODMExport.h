#pragma once
#include "..\..\Lib\RequestUtility\InterfaceBase.h"
#include "..\QisdaNVModule\NVItem.h"
#include <string>
#include <vector>
#include "..\..\CommonUtil\XMLManager.h"
#include "Header_MotoXPRS.h"

#ifdef ODM_COACH_MODULE_EXPORTS
#define ODM_COACH_API extern "C" __declspec(dllexport)
#define ODM_COACH_CLASS_API __declspec(dllexport)
#else
#define ODM_COACH_API extern "C" __declspec(dllimport)
#define ODM_COACH_CLASS_API __declspec(dllimport)
#endif

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

typedef bool (*pFnGetMotoXprs_Athens)(TCHAR* szPATH,int nLen,int nPort, int nTimeout);
ODM_COACH_API bool GetMotoXprs_Athens(TCHAR* szPATH,int nLen,int nPort, int nTimeout);

typedef bool (*pFnGetMotoXprs_Lavernock)(TCHAR* szPATH,int nLen,int nPort, int nTimeout);
ODM_COACH_API bool GetMotoXprs_Lavernock(TCHAR* szPATH,int nLen,int nPort, int nTimeout);

typedef bool (*pFnGetMotoXprs_Melos)(TCHAR* szPATH,int nLen,int nPort, int nTimeout);
ODM_COACH_API bool GetMotoXprs_Melos(TCHAR* szPATH,int nLen,int nPort, int nTimeout);
