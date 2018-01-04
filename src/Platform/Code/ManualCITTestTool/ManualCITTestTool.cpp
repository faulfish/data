// ManualCITTestTool.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "ManualCITTestTool.h"

#include "../STE/MiddleWare/CallbackTypes.h"
#include "../Modules/QisdaMiscellaneousModule/QisdaMiscellaneousModule.h"
#include "../Modules/QisdaNVModule/NVItem.h"
#include "../QLib_Export/QLib_export.h"
#include "../QLib_Export/QLib_export_class.h"
#include "KeypadCommand.h"

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


bool OpenComport(CSerCOMLibrary &objCom, int& nPort) 
{
    bool bRes = false;
    unsigned short nPorts = 10;
    unsigned short Ports[10] = {0};
    string strOutput;
    if(!QL_GetDiagPort(&nPorts,Ports) || nPorts < 1) 
    {
        OutputPrompt("Manual CIT Test", "Port Error!", "OK", strOutput);
    }
    else
    {
        TCHAR szBuf[10] = {0};
        _stprintf(szBuf, _T("\\\\.\\COM%d"), Ports[0]);
        if(objCom.OpenComport(_T("NULL"), szBuf) < 0) 
        {
            OutputPrompt("Manual CIT Test", "Port Error!", "OK", strOutput);
        }
        else
        {
            Sleep(1000);
            nPort = Ports[0];
            bRes = true;
        }
    }
    return bRes;
}

bool TurnOnOffEM(long nComport,bool bOpen)
{
    bool bRes = false;
    CNVItem nv(nComport);

    DIAG_NVITEM_PACKET_F_type ReadResp = {0};
    BYTE item_value = bOpen ? 1 : 0;
    bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_MFG_TEST_I,ReadResp,&item_value,1);
    if(bRes)
        bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_TEXT_LABEL_TEST_I,ReadResp,&item_value,1);
    if(bRes)
        bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_LCD_TEST_I,ReadResp,&item_value,1);
    if(bRes)
        bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_SLT_ALT_TEST_I,ReadResp,&item_value,1);
    if(bRes)
        bRes = nv.WriteNVItem(NV_QISDA_EM__ENABLE_EFEM_I,ReadResp,&item_value,1);
    return bRes;
}

//////////////////////////////////////////////////////////////////////////
// This function is used for Manual CIT Test
// Get the model name to change the flow to Wellfleet or Lavernock
//////////////////////////////////////////////////////////////////////////
MANUALCITTESTTOOL_API bool ManualCITTest(TCHAR* outputString) {    
    HandsetModelName name = GetHandsetModdelName();
    CSerCOMLibrary objCom;
    int nPort = 0;
    if(!OpenComport(objCom, nPort)) return false;

    switch (name)
    {
    case HANDSET_MODEL_LAVERNOCK:
        return ManualCITTest_Lavernock(objCom, nPort, outputString);
        break;
    case HANDSET_MODEL_WELLFLEET:
        return ManualCITTest_Wellfleet(objCom, nPort, outputString);
        break;
    case HANDSET_MODEL_MELOS:
        break;
    case HANDSET_MODEL_UNKNOWN:
        break;
    }
}

//////////////////////////////////////////////////////////////////////////
// This function is used to get the model name of handset
//////////////////////////////////////////////////////////////////////////
MANUALCITTESTTOOL_API HandsetModelName GetHandsetModdelName() {
    CSerCOMLibrary objCom;
    int nPort = 0;
    if(!OpenComport(objCom, nPort)) return HANDSET_MODEL_UNKNOWN;
    objCom.CloseComport();
    Sleep(500);
    char szSW[512] = {0};
    char szHW[512] = {0};

    CQisdaMiscellaneousModule *pMsModule = new CQisdaMiscellaneousModule(nPort);
    if(!pMsModule->GetSWHWVersion(szSW, szHW)) {
        return HANDSET_MODEL_UNKNOWN;
    }
    delete pMsModule;
    Sleep(200);
    HandsetModelName name;
    if(memcmp(szHW, "Lavernock", 9) == 0)
        name = HANDSET_MODEL_LAVERNOCK;
    else if(memcmp(szHW, "Athens", 6) == 0)
        name = HANDSET_MODEL_WELLFLEET;
    else if(memcmp(szHW, "Melos", 6) == 0)
        name = HANDSET_MODEL_MELOS;
    else
        name = HANDSET_MODEL_UNKNOWN;
    return name;
}
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////
MANUALCITTESTTOOL_API bool ManualCITTest_Wellfleet(CSerCOMLibrary &objCom, int nPort, TCHAR* outputString) {
    char* szManualCITTest = "Manual CIT Test";
    string output;

    //TurnOnOffEM(nPort, true);   // Enable EM
    BOOL bRes = TRUE;
    bRes = EnterEMForAthens(objCom);
    string strOutput = "this is a test";
    if(bRes == FALSE) {
        OutputResult("Enter EM mode failed.", "1", "1", "1", "F", strOutput);
        return false;
    }
    int nAudioTest1st = 0, nAudioTest2nd = 0;   //< Audio Test return value
    int nKeyPadTest = 0;
    int nLCD1st = 0, nLCD2nd = 0, nLCD3rd = 0, nLCD4th = 0, nLCD5th = 0, nLCD6th = 0, nLCD7th = 0;  //< LCD test
    int nMainDSC = 0;   //< Main DSC test
    int nSubDSC = 0;    //< Sub DSC test
    int nMP3 = 6;       //< MP3 player test
    int nBTAudio = 0;   //< BT setting audio path setting
    int nVibration = 0; //< Vibration test
    int nSPL = 0;       //< SPL test
    int nSimCard = 0;   //< sim card test
    if(!AudioTest(objCom, nAudioTest1st, nAudioTest2nd)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!KeypadTest(objCom, nKeyPadTest)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!TestLCD(objCom, nLCD1st, nLCD2nd, nLCD3rd, nLCD4th, nLCD5th, nLCD6th, nLCD7th)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!MainDSCTest(objCom, nMainDSC)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!SubDSCTest(objCom, nSubDSC)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!RTCTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!MP3Test(objCom, nMP3)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!BluetoothAudioTest(objCom, nBTAudio)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!VibrationTest(objCom, nVibration)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!SPLTest(objCom, nSPL)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!PeakPowerTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!VideoTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!CoinCellTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!SIMCardTest(objCom, nSimCard)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    SendKeyCommand(objCom, HS_SF2_K);
    objCom.CloseComport();
    Sleep(5000);
    bool bSearchBTDevice = _SearchBTDevice(nPort);
    OutputResult("ManualCITTest - Bluetooth Test", "1", "1", "1", bSearchBTDevice ? "P" : "F", string());
    //OutputPrompt(szManualCITTest, "Finish testing...", "OK", strOutput);
    //memcpy(outputString, _T("This is a test!!!"), sizeof(TCHAR)*17);
    CStringA strResult;
    strResult.Format("\[Main Audio Path %s]\n[Loud Speaker Path %s]\n\
[White frame %s] [Red screen %s] [Green screen %s] [Blue screen %s] [White screen %s] [Black screen with back light %s] [Black screen without back light %s]\n \
[Main DCS Test (back camera) %s]\n \
[Sub DSC Test (front camera) %s]\n \
[MP3 Test %s]\n \
[Bluetooth Audio Path %s] \n \
[Vibration Test %s]\n \
[SIM Card Test %s]\n \
[Bluetooth Test %s]",
        nAudioTest1st == 6 ? "success" : "failed", nAudioTest2nd == 6 ? "success" : "failed",
        nLCD1st == 6 ? "success" : "failed", nLCD2nd == 6 ? "success" : "failed", nLCD3rd == 6 ? "success" : "failed", nLCD4th == 6 ? "success" : "failed", nLCD5th == 6 ? "success" : "failed", nLCD6th == 6 ? "success" : "failed", nLCD7th == 6 ? "success" : "failed",
        nMainDSC == 6 ? "success" : "failed",
        nSubDSC == 6 ? "success" : "failed",
        nMP3 == 6 ? "success" : "failed",
        nBTAudio == 6 ? "success" : "failed",
        nVibration == 6 ? "success" : "failed",
        nSimCard == 6 ? "success" : "failed",
        bSearchBTDevice ? "success" : "failed"
        );
    //OutputResult((char*)strResult.GetString(), "1", "1", "1", "P", strOutput);
    memcpy(outputString, strResult.GetString(), strResult.GetLength());
    //OutputResult("Manual CIT Test", "1", "1", "1", "P", strOutput);
    TurnOnOffEM(nPort, false);  // disable EM
    return true;
}

MANUALCITTESTTOOL_API bool ManualCITTest_Lavernock(CSerCOMLibrary &objCom, int nPort, TCHAR* outputString) {
    char* szManualCITTest = "Manual CIT Test";
    string output;

    //TurnOnOffEM(nPort, true);   // Enable EM
    BOOL bRes = TRUE;
    bRes = EnterEMForLavernock(objCom);
    string strOutput = "this is a test";
    if(bRes == FALSE) {
        OutputResult("Enter EM mode failed.", "1", "1", "1", "F", strOutput);
        return false;
    }
    int nAudioTest1st = 0, nAudioTest2nd = 0;   //< Audio Test return value
    int nKeyPadTest = 0;
    int nLCD1st = 0, nLCD2nd = 0, nLCD3rd = 0, nLCD4th = 0, nLCD5th = 0, nLCD6th = 0, nLCD7th = 0;  //< LCD test
    int nMainDSC = 0;   //< Main DSC test
    int nSubDSC = 0;    //< Sub DSC test
    int nMP3 = 6;       //< MP3 player test
    int nBTAudio = 6;   //< BT setting audio path setting
    int nVibration = 0; //< Vibration test
    int nSPL = 0;       //< SPL test
    int nSimCard = 0;   //< sim card test
    int nFMRadio = 0;   //< FM Radio test
    int nGSensor = 0;   //< G-Sensor test
    if(!AudioTest(objCom, nAudioTest1st, nAudioTest2nd)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!KeypadTest(objCom, nKeyPadTest)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!TestLCD(objCom, nLCD1st, nLCD2nd, nLCD3rd, nLCD4th, nLCD5th, nLCD6th, nLCD7th)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!MainDSCTest(objCom, nMainDSC)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!SubDSCTest(objCom, nSubDSC)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!RTCTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!MP3Test(objCom, nMP3)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!BluetoothAudioTest(objCom, nBTAudio)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!VibrationTest(objCom, nVibration)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    if(!SPLTest(objCom, nSPL)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!PeakPowerTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!VideoTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!FMRadioTest(objCom, nFMRadio)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!CoinCellTest(objCom)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!SIMCardTest(objCom, nSimCard)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    //if(!GSensorTest(objCom, nGSensor)) return false;
    SendKeyCommand(objCom, HS_DOWN_K);
    Sleep(200);
    SendKeyCommand(objCom, HS_SF2_K);
    CStringA strResult;
    strResult.Format("\[Main Audio Path %s]\n[Loud Speaker Path %s]\n\
[White frame %s] [Red screen %s] [Green screen %s] [Blue screen %s] [White screen %s] [Black screen with back light %s] [Black screen without back light %s]\n \
[Main DCS Test (back camera) %s]\n \
[Sub DSC Test (front camera) %s]\n \
[MP3 Test %s]\n \
[Vibration Test %s]\n",
//[Bluetooth Audio Path %s] \n
//[SIM Card Test %s]\n 
//[FM Radio Test %s]\n"
//[GSensor Test %s]\n",
        nAudioTest1st == 6 ? "success" : "failed", nAudioTest2nd == 6 ? "success" : "failed",
        nLCD1st == 6 ? "success" : "failed", nLCD2nd == 6 ? "success" : "failed", nLCD3rd == 6 ? "success" : "failed", nLCD4th == 6 ? "success" : "failed", nLCD5th == 6 ? "success" : "failed", nLCD6th == 6 ? "success" : "failed", nLCD7th == 6 ? "success" : "failed",
        nMainDSC == 6 ? "success" : "failed",
        nSubDSC == 6 ? "success" : "failed",
        nMP3 == 6 ? "success" : "failed",
        nVibration == 6 ? "success" : "failed"
        //nBTAudio == 6 ? "success" : "failed",
        //nSimCard == 6 ? "success" : "failed",
        //nFMRadio == 6 ? "success" : "failed"
        //nGSensor == 6 ? "success" : "failed"
        );
    //OutputResult((char*)strResult.GetString(), "1", "1", "1", "P", strOutput);
    memcpy(outputString, strResult.GetString(), strResult.GetLength());
    //OutputResult("Manual CIT Test", "1", "1", "1", "P", strOutput);
    //TurnOnOffEM(nPort, false);  // disable EM
    return true;
}
