// ComportLister.cpp: implementation of the CComportLister class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ComportLister.h"
#include "windows.h"
#include "assert.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CComportLister::CComportLister()
{

}

CComportLister::~CComportLister()
{

}

const char* CComportLister::GetComportList()
{
	OSVERSIONINFO osvinfo;
	/* determine which platform we're running on and forward
	* to the appropriate routine
	*/
	ZeroMemory(&osvinfo,sizeof(osvinfo));
	osvinfo.dwOSVersionInfoSize=sizeof(osvinfo);
	GetVersionEx(&osvinfo);

	// windows 2000 and windows XP
	if( osvinfo.dwPlatformId == VER_PLATFORM_WIN32_NT &&
		osvinfo.dwMajorVersion ==5 &&
		osvinfo.dwMinorVersion == 0 ||
		osvinfo.dwMinorVersion == 1)
	{
		char szValueName[128];
		unsigned char szValueData[128];
		DWORD retCode;
		int i;
		HKEY   hkEnum=NULL;
		ULONG ulValueNameLength = 128, ulValueDataLength = 128;

		retCode = RegOpenKeyEx(HKEY_LOCAL_MACHINE,"HARDWARE\\DEVICEMAP\\SERIALCOMM",0,KEY_QUERY_VALUE,&hkEnum);

		m_strComportList = "";
		for (i = 0; ; i++)
		{
			ulValueNameLength = 128;
			ulValueDataLength = 128;
			retCode = RegEnumValue(hkEnum, i, szValueName, &ulValueNameLength, NULL, NULL, szValueData, &ulValueDataLength);
			if(retCode != ERROR_SUCCESS)
			{
				break;
			}

			//+ Add by Rick Huang, 2004/12/22
			//+ for avoid get bluetooth comport in O2
			_strlwr(szValueName);
//			if(strstr(szValueName, "\\device\\bt") != NULL)
//			{
//				continue;
//			}
//			if(strstr(szValueName, "\\device\\bluetooth") != NULL)
//			{
//				continue;
//			}
//			if(strstr(szValueName, "\\device\\ir") != NULL)
//			{
//				continue;
//			}
			if(strstr(szValueName, "\\device\\qcusb") != NULL)
			{
				if(m_strComportList != "")
				{
					m_strComportList += ";";
				}

				m_strComportList += (char*)szValueData;
			}
			//+
		}

		return m_strComportList.c_str();
	}
	else
	{
		assert(0);
		//char buffer[512];
		//sprintf(buffer, "osvinfo.dwPlatformId = %d\nosvinfo.dwMajorVersion = %d\nosvinfo.dwMinorVersion = %d\nSorry, The EnumPort() dosen't supported in this windows version yet. Please contect Rick Huang: ext 7247.\n", osvinfo.dwPlatformId, osvinfo.dwMajorVersion, osvinfo.dwMinorVersion);
		//MessageBox(buffer, "Worning");
	}
	return NULL;
}
