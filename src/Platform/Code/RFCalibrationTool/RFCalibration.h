#pragma once

#ifdef CALTEST_EXPORTS
#define CALTEST_API __declspec(dllexport)
#else
#define CALTEST_API __declspec(dllimport)
#endif


#ifdef __cplusplus
extern "C" {
#endif

CALTEST_API BOOL RFCalibration(TCHAR *lpszCalItems);

CALTEST_API BOOL RFTest(TCHAR *lpszTestItems);

CALTEST_API BOOL RFCompletePhaseAndCallTest(TCHAR *lpszCalTestItems);

#ifdef __cplusplus
}
#endif
