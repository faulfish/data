#include "stdafx.h"
#include "findport.h"
#include <string>
#include <cassert>


#define DEVICE_NAME_LENGTH 256

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
			strlwr(szValueName);

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

void EnumerateDiagSerialPorts(CStringArray& strArray)
{
	LPCTSTR szFunc = _T("EnumerateDiagSerialPorts");
	HKEY hKey = NULL;
	int nRet = 0;
	BOOL bIsSetKey = FALSE;
	DWORD dwDataLength = 0;
	DWORD	dwType = 0 ;
	CString strUserInitCmd;
	CString strRegKeyHeader;
	char	szDeviceName[DEVICE_NAME_LENGTH];
	CHAR     achKey[MAX_PATH]; 
    CHAR     achClass[MAX_PATH] = "";  // buffer for class name 
    DWORD    cchClassName = MAX_PATH;  // length of class string 
 
    DWORD i; 
    DWORD retCode; 
	unsigned long ulLength = 0;
 
    DWORD cchValue = DEVICE_NAME_LENGTH; 
	CString strRegKey = _T("");
	CString strDebugMessage = _T("");
	HKEY hSubKey = NULL;

	strArray.RemoveAll();

	strRegKeyHeader = _T("SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E978-E325-11CE-BFC1-08002BE10318}");

	if ( RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegKeyHeader, 0, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS){
		return;
	}

	i = 0 ;
	while( TRUE)    
	{ 
		
        
		// Enumerate all entry
		retCode = RegEnumKey(hKey, i, achKey, DEVICE_NAME_LENGTH);
		
        if (retCode == (DWORD)ERROR_SUCCESS || retCode == (DWORD) ERROR_MORE_DATA) 
        {
			hSubKey = NULL;
			dwType = REG_SZ;
			strRegKey = strRegKeyHeader + _T("\\") + achKey;
			
			
			nRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegKey, 0, KEY_ALL_ACCESS, &hSubKey);
    
			if ( nRet != ERROR_SUCCESS ){
				break;
			}

			ZeroMemory(szDeviceName, DEVICE_NAME_LENGTH);
			dwDataLength = DEVICE_NAME_LENGTH;

			nRet = RegQueryValueEx(hSubKey, _T("AssignedPortForQCDevice"), NULL, &dwType, (LPBYTE)szDeviceName , &dwDataLength);

			if ( nRet != ERROR_SUCCESS ){
				RegCloseKey(hSubKey);
				i++;
				continue;
		
			}
			CString strPortName = szDeviceName;
			strPortName.MakeUpper();
			strArray.Add(strPortName);
		
			RegCloseKey(hSubKey);
      
		}else {
			break;
			
		}
		i++;
	}
	RegCloseKey(hKey);
	
}

CString GetCurrentActivePort(){
	LPCTSTR szFunc = _T("GetCurrentActivePort");
	CComportLister objLister;
	CString strRet = _T("");
	
	CString strList = objLister.GetComportList();
	strList.MakeUpper();
	
	CStringArray strArray;
	EnumerateDiagSerialPorts(strArray);
	int i = 0;
	for ( i = 0 ; i < strArray.GetSize() ; i ++){
		if (-1 != strList.Find(strArray[i],0) ){
			return strArray[i];
		}
	}
	return strRet;
}
