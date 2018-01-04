#ifndef __MSM6280_H__
#define __MSM6280_H__

//#ifdef MSM6280_EXPORT
	#define MSM6280API __declspec (dllexport)
//#else
//	#define MSM6280API __declspec (dllimport)
//#endif

#ifdef __cplusplus
extern "C" {
#endif

//WCDMA Calibration
MSM6280API int _stdcall MobileOnFTM(int &iGPIBAddress,int icomport);
//MSM6280API int _stdcall MobileOnFTM(int iGPIBAddress);
MSM6280API int _stdcall CalWCDMAInit(int iBand);
MSM6280API int _stdcall CalWCDMARxDVGAVsFreq(int iBand);
MSM6280API int _stdcall CalWCDMARxLNAVsFreq(int iBand);
MSM6280API int _stdcall CalWCDMATxLinMaster(int iBand);
MSM6280API int _stdcall CalWCDMATxCompVsFreq(int iBand);
MSM6280API int _stdcall CalWCDMARxDoDC(int iBand);
MSM6280API int _stdcall CalWCDMAWriteToNV(int iBand);
MSM6280API int _stdcall CalWCDMALoadRFData(void);
MSM6280API int _stdcall CalEnd(bool bOnline = false);

//GSM Calibration
//MSM6280API int _stdcall SetStartInit(int iGPIBAddress,int iBand);
MSM6280API int _stdcall SetStartInit(int iGPIBAddress,int iBand,int icomport);
MSM6280API int _stdcall CalThermCalibration(int iBand);
MSM6280API int _stdcall CalGSMInitPaDACSearch(int iBand);
MSM6280API int _stdcall CalGSM4xFMODCalibration(int iBand);
MSM6280API int _stdcall CalGSMPout(int iBand);
MSM6280API int _stdcall CalGSMPolarCalibration(int iBand);
MSM6280API int _stdcall CalGSMRSBCalibration(int iBand);
MSM6280API int _stdcall CalGSMIM2Calibration(int iBand);
MSM6280API int _stdcall CalGSMRxFreqCompensation(int iBand);
MSM6280API int _stdcall CalGSMTimingDelaySearch(int iBand);
MSM6280API int _stdcall CalGSMEnd(int iBand, bool bOnline = false);
//GPS Test
MSM6280API int _stdcall GPSTest(int iGPIBAddress);


//WCDMA Test
MSM6280API int _stdcall TestWCDMAInitSetting(int iBand,int icomport);
//MSM6280API int _stdcall TestWCDMAInitSetting(int iBand);
MSM6280API int _stdcall InitBSEWCDMA(int iGPIBAddress,int iBand);
MSM6280API int _stdcall FTMWCDMAStartCall(int iBand);
MSM6280API int _stdcall FTMWCDMARFLchMeasureAll(int iBand);      
MSM6280API int _stdcall FTMWCDMARFMchMeasureAll(int iBand, OutputCallBackFunc pCallBack = NULL);
MSM6280API int _stdcall FTMWCDMARFHchMeasureAll(int iBand);
MSM6280API int _stdcall TestWCDMADiversityRx(int iBand);
MSM6280API int _stdcall TestWCDMAEnd(void);
MSM6280API int _stdcall SwitchToOnlineMode(void);


// Testing GSM  ----------------------------------------------------------------------
//MSM6280API int _stdcall InitCallBox_GSM(int iBand);
MSM6280API int _stdcall InitCallBox_GSM(int iBand,int icomport);
MSM6280API int _stdcall FTMGSMStartCall(int iGPIBAddress,int iBand);
MSM6280API int _stdcall FTMGSMHandover(int iBand);
//Test EDGE --------------------------------------------------------------------------
MSM6280API int _stdcall InitCallBox_EDGE(int iBand);
MSM6280API int _stdcall FTMGSMStartCall_EDGE(int iBand);
MSM6280API int _stdcall FTMGSMHandover_EDGE(int iBand);    	      
MSM6280API int _stdcall EndTesting(int iBand); 
MSM6280API int _stdcall SetPhoneOnlineMode(int iBand);


//On-line Call function
MSM6280API int _stdcall PhoneOnlineCallTest(int iGPIBAddress); 

//Online Testing GSM  -----------------------------------------------------------------
MSM6280API int _stdcall GSMOnlineInit(int icomport,int* nTestSet, OutputCallBackFunc pCallBack = NULL);
MSM6280API int _stdcall GSMOnlineStartCall_GSM(int iBand);
MSM6280API int _stdcall GSMOnlineTest_GSM(int iBand, unsigned int uiFlag);
MSM6280API int _stdcall GSMOnlineEndTest(void);
//Online Testing EDGE  -----------------------------------------------------------------
MSM6280API int _stdcall GSMOnlineStartCall_EDGE(int iBand);
MSM6280API int _stdcall GSMOnlineTest_EDGE(int iBand, unsigned int uiFlag);
//Power Supply  ------------------------------------------------------------------------
MSM6280API int _stdcall XQTPowerSupplyOnOff(int iSwitch);

MSM6280API int _stdcall WCDMAOnlineInitCheckDevice(int icomport, int* nTestSet);
MSM6280API int _stdcall WCDMAOnlineStartCall(int iBand,  OutputCallBackFunc pCallBack = NULL);
MSM6280API int _stdcall WCDMAOnlineEndTest(void);
MSM6280API int _stdcall WCDMAOnlineRFHchGen(void);
MSM6280API int _stdcall WCDMAOnlineRFHchMeasure(void);
MSM6280API int _stdcall WCDMAOnlineILPCMeasure(void);

#ifdef __cplusplus
}	// extern "C"
#endif 

#endif //__MSM6280_H__




