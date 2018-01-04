// QisdaWirelessModule.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "QisdaWirelessModule.h"
#include "MSM6280.H"
#include "EnumFile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWinApp theApp;

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

//WCDMA Calibration
QWM_API int doMobileOnFTM(int &iGPIBAddress,int icomport) {
//QWM_API int doMobileOnFTM(int iGPIBAddress) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = MobileOnFTM(iGPIBAddress,icomport))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMAInit(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMAInit(iBand))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMARxDVGAVsFreq(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMARxDVGAVsFreq(iBand))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMARxLNAVsFreq(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMARxLNAVsFreq(iBand))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMATxLinMaster(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMATxLinMaster(iBand))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMATxCompVsFreq(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMATxCompVsFreq(iBand))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMARxDoDC(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMARxDoDC(iBand))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMAWriteToNV(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMAWriteToNV(iBand))!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalWCDMALoadRFData(void) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalWCDMALoadRFData())!=SUCCESS){
		CalEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doCalEnd(bool bOnline) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalEnd(bOnline))!=SUCCESS){
		return iErrCode;
	}
	return true;
}

//GSM Calibration
QWM_API int doSetStartInit(int iGPIBAddress,int iBand,int icomport) {
//QWM_API int doSetStartInit(int iGPIBAddress,int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;
	iGPIBAddress = 0;

	if((iErrCode = SetStartInit(iGPIBAddress,iBand,icomport))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalThermCalibration(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalThermCalibration(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMInitPaDACSearch(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMInitPaDACSearch(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSM4xFMODCalibration(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSM4xFMODCalibration(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMPout(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMPout(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMPolarCalibration(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMPolarCalibration(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMRSBCalibration(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMRSBCalibration(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMIM2Calibration(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMIM2Calibration(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMRxFreqCompensation(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMRxFreqCompensation(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMTimingDelaySearch(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMTimingDelaySearch(iBand))!=SUCCESS){
		CalGSMEnd(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doCalGSMEnd(int iBand, bool bOnline) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = CalGSMEnd(iBand, bOnline))!=SUCCESS){
		return iErrCode;
	}
	return true;
}

//GPS Test
QWM_API int doGPSTest(int iGPIBAddress) {
	return false;
}

//WCDMA Test
QWM_API int doTestWCDMAInitSetting(int iBand,int icomport) {
//QWM_API int doTestWCDMAInitSetting(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = TestWCDMAInitSetting(iBand,icomport))!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doInitBSEWCDMA(int iGPIBAddress,int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = InitBSEWCDMA(iGPIBAddress,iBand))!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doFTMWCDMAStartCall(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = FTMWCDMAStartCall(iBand))!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doFTMWCDMARFLchMeasureAll(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = FTMWCDMARFLchMeasureAll(iBand))!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doFTMWCDMARFMchMeasureAll(int iBand,OutputCallBackFunc pCallBack) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = FTMWCDMARFMchMeasureAll(iBand,pCallBack))!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doFTMWCDMARFHchMeasureAll(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = FTMWCDMARFHchMeasureAll(iBand))!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doTestWCDMADiversityRx(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = TestWCDMADiversityRx(iBand))!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doTestWCDMAEnd(void) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;
	int iBand = 10;

	if((iErrCode = TestWCDMAEnd())!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

QWM_API int doSwitchToOnlineMode(void) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;
	int iBand = 10;

	if((iErrCode = SwitchToOnlineMode())!=SUCCESS){
		TestWCDMAEnd();
		return iErrCode;
	}
	return true;
}

// Testing GSM
QWM_API int doInitCallBox_GSM(int iBand,int icomport) {
//QWM_API int doInitCallBox_GSM(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = InitCallBox_GSM(iBand,icomport))!=SUCCESS){
		EndTesting(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doFTMGSMStartCall(int iGPIBAddress,int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = FTMGSMStartCall(iGPIBAddress,iBand))!=SUCCESS){
		EndTesting(iBand);
		return iErrCode;
	}
	return true;
}

QWM_API int doFTMGSMHandover(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = FTMGSMHandover(iBand))!=SUCCESS){
		EndTesting(iBand);
		return iErrCode;
	}
	return true;
}

//Test EDGE
QWM_API int doInitCallBox_EDGE(int iBand) {
	return false;
}

QWM_API int doFTMGSMStartCall_EDGE(int iBand) {
	return false;
}

QWM_API int doFTMGSMHandover_EDGE(int iBand) {
	return false;
}

QWM_API int doEndTesting(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = EndTesting(iBand))!=SUCCESS){
		return iErrCode;
	}
	return true;
}

QWM_API int doSetPhoneOnlineMode(int iBand) {
	char strTracer[MAX_ARRAY_SIZE] = {0};
	int i =0;
	int iErrCode = 0;

	if((iErrCode = SetPhoneOnlineMode(iBand))!=SUCCESS){
		return iErrCode;
	}
	return true;
}

//On-line Call function
QWM_API int doPhoneOnlineCallTest(int iGPIBAddress) {
	return false;
}

//Online Testing GSM  -----------------------------------------------------------------
QWM_API int doGSMOnlineInit(int icomport, int* nTestSet,OutputCallBackFunc pCallBack){

	int iErrCode = GSMOnlineInit(icomport, nTestSet, pCallBack);

	return iErrCode;
}
QWM_API int doGSMOnlineStartCall_GSM(int iBand){

	int iErrCode = GSMOnlineStartCall_GSM(iBand);
	return iErrCode;

}
QWM_API int doGSMOnlineTest_GSM(int iBand, unsigned int uiFlag){
	int iErrCode = GSMOnlineTest_GSM(iBand, uiFlag);
	return iErrCode;
}
QWM_API int doGSMOnlineEndTest(void){
	int iErrCode = GSMOnlineEndTest();
	return iErrCode;
}
//Online Testing EDGE  -----------------------------------------------------------------
QWM_API int doGSMOnlineStartCall_EDGE(int iBand){
	int iErrCode = GSMOnlineStartCall_EDGE(iBand);
	return iErrCode;
}
QWM_API int doGSMOnlineTest_EDGE(int iBand, unsigned int uiFlag){
	int iErrCode = GSMOnlineTest_EDGE(iBand, uiFlag);
	return iErrCode;
}

QWM_API int doXQTPowerSupplyOnOff(int iSwitch){
	int iErrCode = XQTPowerSupplyOnOff(iSwitch);
	return iErrCode;
}

QWM_API int doWCDMAOnlineInitCheckDevice(int icomport, int* nTestSet)
{
	int iErrCode = 0;
	iErrCode = WCDMAOnlineInitCheckDevice(icomport, nTestSet);
	if(iErrCode == SUCCESS)
		return true;
	else
		return iErrCode;
}
QWM_API int doWCDMAOnlineStartCall(int iBand,  OutputCallBackFunc pCallBack)
{
	int iErrCode = 0;
	iErrCode = WCDMAOnlineStartCall(iBand, pCallBack);
	if(iErrCode == SUCCESS)
		return true;
	else
		return iErrCode;
}
QWM_API int doWCDMAOnlineEndTest(void)
{
	int iErrCode = 0;
	iErrCode = WCDMAOnlineEndTest();
	if(iErrCode == SUCCESS)
		return true;
	else
		return iErrCode;
}
QWM_API int doWCDMAOnlineRFHchGen(void)
{
	int iErrCode = 0;
	iErrCode = WCDMAOnlineRFHchGen();
	if(iErrCode == SUCCESS)
		return true;
	else
		return iErrCode;
}
QWM_API int doWCDMAOnlineRFHchMeasure(void)
{
	int iErrCode = 0;
	iErrCode = WCDMAOnlineRFHchMeasure();
	if(iErrCode == SUCCESS)
		return true;
	else
		return iErrCode;
}
QWM_API int doWCDMAOnlineILPCMeasure(void)
{
	int iErrCode = 0;
	iErrCode = WCDMAOnlineILPCMeasure();
	if(iErrCode == SUCCESS)
		return true;
	else
		return iErrCode;
}
