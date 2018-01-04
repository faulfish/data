// QisdaWirelessModule.h : main header file for the QisdaWirelessModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols

typedef bool (*OutputDResult) (char* pDescription, char* pBand, char* pChannel, char* pLowLimit, char* pHighLimit, char* pResult, char* pUnits, char* pPassFailFlag);
typedef char* (*OutputCallBackFunc) (char* pType, char* pData);

#ifdef QISDA_WIRELESS_MODULE_EXPORTS
#define QWM_API extern "C" __declspec(dllexport)
#else
#define QWM_API extern "C" __declspec(dllimport)
#endif
// CQisdaWirelessModuleApp
// See QisdaWirelessModule.cpp for the implementation of this class


//WCDMA Calibration
QWM_API int doMobileOnFTM(int &iGPIBAddress,int icomport);
//QWM_API int doMobileOnFTM(int iGPIBAddress);
QWM_API int doCalWCDMAInit(int iBand);
QWM_API int doCalWCDMARxDVGAVsFreq(int iBand);
QWM_API int doCalWCDMARxLNAVsFreq(int iBand);
QWM_API int doCalWCDMATxLinMaster(int iBand);
QWM_API int doCalWCDMATxCompVsFreq(int iBand);
QWM_API int doCalWCDMARxDoDC(int iBand);
QWM_API int doCalWCDMAWriteToNV(int iBand);
QWM_API int doCalWCDMALoadRFData(void);
QWM_API int doCalEnd(bool bOnline = false);

//GSM Calibration
QWM_API int doSetStartInit(int iGPIBAddress,int iBand,int icomport);
//QWM_API int doSetStartInit(int iGPIBAddress,int iBand);
QWM_API int doCalThermCalibration(int iBand);
QWM_API int doCalGSMInitPaDACSearch(int iBand);
QWM_API int doCalGSM4xFMODCalibration(int iBand);
QWM_API int doCalGSMPout(int iBand);
QWM_API int doCalGSMPolarCalibration(int iBand);
QWM_API int doCalGSMRSBCalibration(int iBand);
QWM_API int doCalGSMIM2Calibration(int iBand);
QWM_API int doCalGSMRxFreqCompensation(int iBand);
QWM_API int doCalGSMTimingDelaySearch(int iBand);
QWM_API int doCalGSMEnd(int iBand, bool bOnline = false);
//GPS Test
QWM_API int doGPSTest(int iGPIBAddress);


//WCDMA Test
QWM_API int doTestWCDMAInitSetting(int iBand,int icomport);
//QWM_API int doTestWCDMAInitSetting(int iBand);
QWM_API int doInitBSEWCDMA(int iGPIBAddress,int iBand);
QWM_API int doFTMWCDMAStartCall(int iBand);
QWM_API int doFTMWCDMARFLchMeasureAll(int iBand);      
QWM_API int doFTMWCDMARFMchMeasureAll(int iBand, OutputCallBackFunc pCallBack = NULL);
QWM_API int doFTMWCDMARFHchMeasureAll(int iBand);
QWM_API int doTestWCDMADiversityRx(int iBand);
QWM_API int doTestWCDMAEnd(void);
QWM_API int doSwitchToOnlineMode(void);


// Testing GSM  ----------------------------------------------------------------------
QWM_API int doInitCallBox_GSM(int iBand,int icomport);
//QWM_API int doInitCallBox_GSM(int iBand);
QWM_API int doFTMGSMStartCall(int iGPIBAddress,int iBand);
QWM_API int doFTMGSMHandover(int iBand);

//Test EDGE --------------------------------------------------------------------------
QWM_API int doInitCallBox_EDGE(int iBand);
QWM_API int doFTMGSMStartCall_EDGE(int iBand);
QWM_API int doFTMGSMHandover_EDGE(int iBand);    	      
QWM_API int doEndTesting(int iBand); 
QWM_API int doSetPhoneOnlineMode(int iBand);

//Online Testing GSM  -----------------------------------------------------------------
QWM_API int doGSMOnlineInit(int icomport, int* nTestSet,OutputCallBackFunc pCallBack = NULL);
QWM_API int doGSMOnlineStartCall_GSM(int iBand);
QWM_API int doGSMOnlineTest_GSM(int iBand, unsigned int uiFlag);
QWM_API int doGSMOnlineEndTest(void);
//Online Testing EDGE  -----------------------------------------------------------------
QWM_API int doGSMOnlineStartCall_EDGE(int iBand);
QWM_API int doGSMOnlineTest_EDGE(int iBand, unsigned int uiFlag);


//On-line Call function
QWM_API int doPhoneOnlineCallTest(int iGPIBAddress); 

//Power Supply  ------------------------------------------------------------------------
QWM_API int doXQTPowerSupplyOnOff(int iSwitch);

//WCDMA Online test ------------------------------------------------------------------------
QWM_API int doWCDMAOnlineInitCheckDevice(int icomport, int* nTestSet);
QWM_API int doWCDMAOnlineStartCall(int iBand,  OutputCallBackFunc pCallBack = NULL);
QWM_API int doWCDMAOnlineEndTest(void);
QWM_API int doWCDMAOnlineRFHchGen(void);
QWM_API int doWCDMAOnlineRFHchMeasure(void);
QWM_API int doWCDMAOnlineILPCMeasure(void);
