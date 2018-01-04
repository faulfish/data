// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOUNDUTILITY_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOUNDUTILITY_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SOUNDUTILITY_EXPORTS
#define SOUNDUTILITY_API extern "C" __declspec(dllexport)
#else
#define SOUNDUTILITY_API extern "C" __declspec(dllimport)
#endif

typedef bool (*pFnAnalysisRecordStereo)(int nDurationMiniSecond,int& nAmplitudeLeft,int& nFrequenceLeft,int& nAmplitudeRight,int& nFrequenceRight,const char* szSaveFile);
SOUNDUTILITY_API bool AnalysisRecordStereo(int nDurationMiniSecond,int& nAmplitudeLeft,int& nFrequenceLeft,int& nAmplitudeRight,int& nFrequenceRight,const char* szSaveFile = NULL);

typedef bool (*pFnAnalysisRecord)(int nDurationMiniSecond,int& nAmplitude,int& nFrequence,const char* szSaveFile);
SOUNDUTILITY_API bool AnalysisRecord(int nDurationMiniSecond,int& nAmplitude,int& nFrequence,const char* szSaveFile = NULL);
