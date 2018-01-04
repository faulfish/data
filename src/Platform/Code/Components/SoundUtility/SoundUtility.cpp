// SoundUtility.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "SoundUtility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// The one and only application object

CWinApp theApp;

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		_tprintf(_T("Fatal Error: MFC initialization failed\n"));
		nRetCode = 1;
	}
	else
	{
		// TODO: code your application's behavior here.
	}

	return nRetCode;
}

#include "Wave.h"
#include "WaveDevice.h"
#include "WaveInterface.h"
#include "WaveOut.h"
#include "WaveIn.h"
#include "..\..\CommonUtil\UnicodeUtility.h"
#include "..\ConfigureTool\IConfigure.h"

double GetSampleRateFromINI(double nDefaultSampleRate)
{
	using namespace QSYNC_LIB;

	double nSampleRate = nDefaultSampleRate;

	if(true)
	{
		typedef IConfigure* (*pFnCreateIConfigure)(LPCTSTR szConfigureTitle, LPCTSTR szFolderName);
		typedef bool (*pFnDeleteIConfigure)(LPCTSTR szConfigureTitle);
#ifdef _DEBUG
		HMODULE hConfigureDll =::LoadLibrary(_T("ConfigureTool_StaticUD.dll"));
#else
		HMODULE hConfigureDll =::LoadLibrary(_T("ConfigureTool_StaticU.dll"));
#endif    
		if(hConfigureDll)
		{			
			pFnCreateIConfigure g_CreateIConfigure = NULL;
			pFnDeleteIConfigure g_DeleteIConfigure = NULL;
			if(g_CreateIConfigure == NULL)
			{
				g_CreateIConfigure = (pFnCreateIConfigure)GetProcAddress(hConfigureDll, "CreateIConfigure");
				g_DeleteIConfigure = (pFnDeleteIConfigure)GetProcAddress(hConfigureDll, "DeleteIConfigure");
			}
			if(g_CreateIConfigure)
			{
				IConfigure* pIConfigure = g_CreateIConfigure(L"SoundUtility",L"SoundUtility");
				if(pIConfigure)
				{
					pIConfigure->Load();
					CCfgInt nCfgFilterRate(pIConfigure,L"SampleRate",nSampleRate*100);
					nSampleRate = nCfgFilterRate;
					//pIConfigure->Save();
					g_DeleteIConfigure(L"SoundUtility");
				}
			}

			::FreeLibrary(hConfigureDll);
		}
	}

	return nSampleRate;
}

SOUNDUTILITY_API bool AnalysisRecordStereo(int nDurationMiniSecond,int& nAmplitudeLeft,int& nFrequenceLeft,int& nAmplitudeRight,int& nFrequenceRight,const char* szSaveFile)
{
	bool bRes = false;

	CWave monWave;
	CWaveIn waveIn;
	CWaveDevice monDevice;

	int nChannel = 2;
	if(nAmplitudeRight == -1 && nFrequenceRight == -1)
		nChannel = 1;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	//2008/12/1 Alan tu
	//Don't change 8000,16
	//For now,it can support 8000,16bits only
	monWave.BuildFormat(nChannel, GetSampleRateFromINI(22050), 16);
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	if ( monDevice.IsInputFormat(monWave) ) 
	{
		waveIn.SetWaveFormat( monWave.GetFormat() );
		waveIn.SetDevice(monDevice);
		if ( waveIn.Open() ) 
		{
			bRes = waveIn.Record();

			::Sleep(nDurationMiniSecond);

			if(bRes)
			{
				nAmplitudeLeft = waveIn.GetAmplitudeLeft();
				nFrequenceLeft = waveIn.GetFrequenceLeft();
				nAmplitudeRight = waveIn.GetAmplitudeRight();
				nFrequenceRight = waveIn.GetFrequenceRight();
			}

			if ( waveIn.Close() ) 
			{
				if(szSaveFile)
				{
					using namespace QSYNC_LIB;
					monWave = waveIn.MakeWave();
					monWave.Save(GetTSTRING(szSaveFile).c_str());
				}
			}
		}
	}

	return bRes;
}

SOUNDUTILITY_API bool AnalysisRecord(int nDurationMiniSecond,int& nAmplitudeLeft,int& nFrequenceLeft,const char* szSaveFile)
{
	int nAmplitudeRight = -1;
	int nFrequenceRight = -1;
	return AnalysisRecordStereo(nDurationMiniSecond,nAmplitudeLeft,nFrequenceLeft,nAmplitudeRight,nFrequenceRight,szSaveFile);
}