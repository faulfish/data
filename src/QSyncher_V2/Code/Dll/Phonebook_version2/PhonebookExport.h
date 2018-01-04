#ifndef _PHONEBOOK_EXPORT_H_
#define _PHONEBOOK_EXPORT_H_

#include <map>
#include <string>
#pragma once

namespace QSYNC_LIB
{

#ifndef TSTRING
#ifdef _UNICODE
#define TSTRING std::wstring
#define _TC(x) L ## x
#else
#define TSTRING std::string
#define _TC(x) "x"
#endif
#endif

#ifdef		VCARDCTRL_DLL
#define		PHONEBOOK_DLL_DECLSPEC		__declspec(dllexport)
#else
#define		PHONEBOOK_DLL_DECLSPEC		__declspec(dllimport)
#endif

	extern "C" PHONEBOOK_DLL_DECLSPEC bool GetPhonebook(std::map<TSTRING,TSTRING>& mapPB);
	typedef bool ( * LPGetPhonebook)(std::map<TSTRING,TSTRING>& mapPB);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*Sample code
#include "..\Phonebook_version2\PhonebookExport.h"

void GetPhoneBooke()
{
	using namespace QSYNC_LIB;

	HINSTANCE hDLL		  = NULL;     	  
	hDLL = ::LoadLibrary(_T("Phonebook_version2.dll"));
	if(hDLL)
	{		
		LPGetPhonebook lpfnDllFunc = (LPGetPhonebook)::GetProcAddress(hDLL,"GetPhonebook");
		if (lpfnDllFunc)
		{
			std::map<TSTRING,TSTRING> mapPB;
			lpfnDllFunc(mapPB);
		}
	}
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif //_PHONEBOOK_EXPORT_H_