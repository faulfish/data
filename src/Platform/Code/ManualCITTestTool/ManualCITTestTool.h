// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the MANUALCITTESTTOOL_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// MANUALCITTESTTOOL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.

#ifndef __MANUAL_CIT_TEST_H__
#define __MANUAL_CIT_TEST_H__

#ifdef MANUALCITTESTTOOL_EXPORTS
#define MANUALCITTESTTOOL_API extern "C" __declspec(dllexport)
#else
#define MANUALCITTESTTOOL_API extern "C" __declspec(dllimport)
#endif


#include "../odmtma_api/SerCOMLib/SerCOMLibrary.h"

//  [3/13/2009 Eddy.Guo]
typedef enum _HandsetModelName {
    HANDSET_MODEL_WELLFLEET,      //< VE538(Athens)
    HANDSET_MODEL_LAVERNOCK,      //< W7
    HANDSET_MODEL_MELOS,          //<
    HANDSET_MODEL_UNKNOWN         //< UNKNOWN model name
} HandsetModelName;

MANUALCITTESTTOOL_API bool ManualCITTest(TCHAR* outputString);
MANUALCITTESTTOOL_API HandsetModelName GetHandsetModdelName();
MANUALCITTESTTOOL_API bool ManualCITTest_Wellfleet(CSerCOMLibrary &objCom, int nPort, TCHAR* outputString);
MANUALCITTESTTOOL_API bool ManualCITTest_Lavernock(CSerCOMLibrary &objCom, int nPort, TCHAR* outputString);


#endif