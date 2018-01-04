// SPU_Loader.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
#include <vector>
#include "LoaderExport.h"
#include < algorithm >
#include "Loader.h"

void ShowUIInfo(LPVOID pView, std::vector<TSTRING> & nVector, int nType)
{
	for(std::vector<TSTRING>::iterator Iter1 = nVector.begin();Iter1 != nVector.end();Iter1++)
	{
		for(size_t i = 0; i < Iter1->length(); i++)
			printf("%c", (Iter1->c_str())[i]);
		printf("\n");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc == 1)
		printf("Lost the script file path!");
	else
	{
		Loader_CallbackClass *pCallBack = new Loader_CallbackClass;
		HANDLE hResource = GetSPULoader(pCallBack);
		if(hResource)
		{
			if(LoadScript(hResource, argv[1]))
			{
				printf("Start to execute command!\n");
				DWORD dwStart = ::GetTickCount();
				SPU_RESULT bRes = ExecuteCommand(hResource);
				if(bRes == SPU_SUCCESS)
					printf("\t\tSuccess!\t\t");
				else
					printf("\t\tFailed!\t\t");
				DWORD dwEnd = ::GetTickCount();
				printf("Used Time: %d s", (dwEnd-dwStart)/1000/1000);
			}
			ReleaseHandle(hResource);
		}
		delete pCallBack;
	}	

	return 0;
}

LOADER_API UINT GetUIType(const TSTRING& szScript)
{
	CLoader Loader(NULL);
	return Loader.GetUIType(szScript);
}

LOADER_API HANDLE GetSPULoader(ISPU_CALLBACK* pCallBack)
{
	CLoader *pLoader = new CLoader(pCallBack);
	return (HANDLE)pLoader;
}

LOADER_API bool LoadScript(HANDLE hResource, const TSTRING& szScript)
{
	CLoader *pLoader = (CLoader*)hResource;
	return pLoader->LoadScript(szScript);
}

LOADER_API SPU_RESULT ExecuteCommand(HANDLE hResource)
{
	CLoader *pLoader = (CLoader*)hResource;
	return pLoader->ExecuteCommand();
}

LOADER_API SPU_RESULT GetDescription(HANDLE hResource,TCHAR* szTemp,int nBufferSize)
{
	TSTRING strDescription;
	CLoader *pLoader = (CLoader*)hResource;
	pLoader->GetDescription(strDescription);
	_tcscpy_s(szTemp,nBufferSize,strDescription.c_str());
	return SPU_SUCCESS;
}

LOADER_API SPU_RESULT GetHeader(HANDLE hResource,TCHAR* szTemp,int nBufferSize)
{
	TSTRING strHeader;
	CLoader *pLoader = (CLoader*)hResource;
	pLoader->GetHeader(strHeader);
	_tcscpy_s(szTemp,nBufferSize,strHeader.c_str());
	return SPU_SUCCESS;
}

LOADER_API void ReleaseHandle(HANDLE hResource)
{
	CLoader *pLoader = (CLoader*)hResource;
	if(pLoader)
		delete pLoader;
}

//For SPU
LOADER_API bool WriteTestItem(char* szName,char* szFunction,char* szOutputFile)
{
	bool bRes = false;

	TSTRING strFunction = szFunction;

	std::transform(strFunction.begin(),strFunction.end(),strFunction.begin(),tolower);
	
	std::string strMode;
	if("append" == strFunction)
		strMode = "a+";
	else if("create" == strFunction)
		strMode = "w+";

	if(strMode.empty() == false)
	{
		FILE* file = fopen(szOutputFile,strMode.c_str());
		if(file)
		{
			int nIP = 0;
			fclose(file);
			{
				CLoader loader(NULL);
				loader.LoadScript(szOutputFile);
				nIP = loader.GetCommandCount();
			}
			file = fopen(szOutputFile,"a");
			int nSeek = fseek(file,-1,SEEK_END);
			char szBuffer[1024] = {0};

			if(nSeek == -1)
			{
				sprintf(szBuffer,"{Header\n}\n{Description\n}\n{Code\n");
				fwrite(szBuffer,strlen(szBuffer),1,file);
			}
			// [N+0] runscript [szName]\r\n
			// [N+1] comapretrue [N+4]\r\n
			// [N+2] print \"[szName] fail\"\r\n
			// [N+3] comparefalse 9999\r\n
			// [N+4] print \"[szName] success\"\r\n
			
			sprintf(szBuffer,"\n//Add test item -- %s\n",szName);
			fwrite(szBuffer,strlen(szBuffer),1,file);
			sprintf(szBuffer,"%04d runscript %s\n",nIP,szName);
			fwrite(szBuffer,strlen(szBuffer),1,file);
			sprintf(szBuffer,"%04d comapretrue %04d\n",nIP+1,nIP+4);
			fwrite(szBuffer,strlen(szBuffer),1,file);
			sprintf(szBuffer,"%04d print \"%s fail\"\n",nIP+2,szName);
			fwrite(szBuffer,strlen(szBuffer),1,file);
			sprintf(szBuffer,"%04d comparefalse 9999\n",nIP+3,szName);
			fwrite(szBuffer,strlen(szBuffer),1,file);
			sprintf(szBuffer,"%04d print \"%s success\"\n",nIP+4,szName);
			fwrite(szBuffer,strlen(szBuffer),1,file);
			fclose(file);
		}
	}
	return bRes;
}
