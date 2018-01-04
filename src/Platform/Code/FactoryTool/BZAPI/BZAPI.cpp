// BZAPI.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "BZAPI.h"
#include <cassert>
#include "audio_import.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CBZAPIApp

BEGIN_MESSAGE_MAP(CBZAPIApp, CWinApp)
END_MESSAGE_MAP()


// CBZAPIApp construction

CBZAPIApp::CBZAPIApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CBZAPIApp object

CBZAPIApp theApp;


// CBZAPIApp initialization

BOOL CBZAPIApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

bool IsQisdaTaipei()
{
	bool bRes = false;
	//20090402 Alan
	char szTest[20] = {0};
	::GetPrivateProfileString("DEBUG", "test", "0", szTest, 20, ".\\TaipeiTestOnly.ini");
	bRes = strcmp(szTest,"1") == 0;
	return bRes;
}

#ifdef BZAPI_API_EXPORTS
#define BZAPI_API extern "C" __declspec(dllexport)
#else
#define BZAPI_API extern "C" __declspec(dllimport)
#endif

BZAPI_API bool TestFunc1(char* szPara1)
{
	bool bRes = false;
	assert(0); //Need to implement
	return bRes;
}

BZAPI_API bool BZ_AudioTest(char* szMsg,char* szLavel1,char* szLavel2)
{
	if(IsQisdaTaipei())
	{
		::Sleep(5000);
		return true;
	}

	int nStatus			= -1;
	double level1		= 777;
	double level2		= 777;
	int nCount			= 0;
	int nMaxtries		= 20;
	CString strDllpath	= _T("");
	CString strArgument = _T("");

	strArgument="C:\\Visual_Audio_Loopback\\PSKILL.EXE AudioGen";

	HINSTANCE DlDllHandle;
	
	strDllpath="C:\\Visual_Audio_Loopback\\AudioLoopbackAnalyzer.dll";
	DlDllHandle = LoadLibraryEx(strDllpath,NULL,LOAD_WITH_ALTERED_SEARCH_PATH);

	if(DlDllHandle)
	{
		pIMPORT_LABVIEWDLL2 = (IMPORT_LABVIEWDLL2_PTR) GetProcAddress(DlDllHandle,"AudioLoopbackAnalyzer");
		nStatus=0;
	}
	if (nStatus == 0 && pIMPORT_LABVIEWDLL2)
	{
		WinExec((LPCSTR)(LPCTSTR)strArgument, SW_SHOWNORMAL);
		WinExec("C:\\Visual_Audio_Loopback\\AudioGen.exe", SW_SHOWNORMAL);
		Sleep(800);

		do
		{
			nCount++;
			nStatus =pIMPORT_LABVIEWDLL2(&level1,&level2);
			//Marcio 20090403
			sprintf_s(szLavel1,1024,"Lavel1 %4.2f",level1);
			sprintf_s(szLavel2,1024,"Lavel2 %4.2f",level2);
			if (level1 < - 70 || level1 > -20)
				nStatus =-1;
			if ((nStatus !=0) && (nCount == (nMaxtries/2)))
			{
				WinExec((LPCSTR)strArgument, SW_SHOWNORMAL);
				Sleep(4000);
				WinExec("C:\\Visual_Audio_Loopback\\AudioGen.exe", SW_SHOWNORMAL);
				Sleep(800);
			}
		}
		while( nStatus!=0 && nCount < nMaxtries );

		WinExec((LPCSTR)strArgument, SW_SHOWNORMAL);

		if(DlDllHandle)
		{
			::FreeLibrary(DlDllHandle);
			pIMPORT_LABVIEWDLL2 = NULL;
			DlDllHandle = NULL;
		}
		
		if (nStatus == 0)
		{
			strcpy_s(szMsg,1024,"TESTE DE AUDIO OK!!!!");
			return true;
		}
		else
			return false;

	}

	CString strError = "Can't find the dll," + strDllpath;
	strcpy_s(szMsg,1024,strError);
	return false;
}