#include "stdafx.h"
#include "RFCalibration.h"
#include "RFCalibrationTool.h"

BOOL RFCalibration(TCHAR *lpszCalItems)
{
	CRFCalibrationToolApp *pApp = (CRFCalibrationToolApp*)AfxGetApp();
	return pApp->RFCalibration(lpszCalItems);
}

BOOL RFTest(TCHAR *lpszTestItems)
{
	CRFCalibrationToolApp *pApp = (CRFCalibrationToolApp*)AfxGetApp();
	return pApp->RFTest(lpszTestItems);
}

BOOL RFCompletePhaseAndCallTest(TCHAR *lpszCalTestItems)
{
	CRFCalibrationToolApp *pApp = (CRFCalibrationToolApp*)AfxGetApp();
	return pApp->RFCompletePhaseAndCallTest(lpszCalTestItems);
}