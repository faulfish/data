// QisdaService.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include <string.h>
#include "KeypadCommand.h"
#include "../STE/MiddleWare/CallbackTypes.h"
#include "../QLib_Export\QLib_export.h"
#include "../Modules/QisdaNVModule/NVItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define BUF_SIZE	1024
char szReturn[1024] = {0};
char szError[1024] = {0};

// The one and only application object

using namespace std;

//Simulates a vibrator command.
BOOL __stdcall Vibrator( BOOL turnOn) {

    return TRUE;
}

//Simulates a shutdown command.
BOOL __stdcall Shutdown() {

    return TRUE;
}

void ShowEachTestItemResult(char* szItem, char* szDesc, int nCode) {
    OutputResult(szItem, "1", "1", "1", nCode == 6 ? "P" : "F", string(szDesc));
}

BOOL SendKeyCommand(CSerCOMLibrary& objCom, byte KeyID){
    int nRet = true;
    SKeyPressReq PressReq = {0};
    SKeyPressResp PressResp = {0};	

    PressReq.cmd_code = 0x20;
    PressReq.Hold = 0;
    PressReq.Key = KeyID;

    nRet = objCom.TXRXDiag((CHAR*)&PressReq, sizeof(PressReq), (CHAR*)&PressReq, sizeof(PressReq), 3000);

    if(nRet < 0)
        nRet = false;

    return nRet;
}

BOOL SendKeyLongPressCommand(CSerCOMLibrary& objCom, byte KeyID, int nPeriod){
    int nRet = true;
    SKeyPressReq PressReq = {0};	
    SKeyPressResp PressResp = {0};	

    PressReq.cmd_code = 0x20;
    PressReq.Hold = 1;
    PressReq.Key = KeyID;

    nRet = objCom.TXRXDiag((CHAR*)&PressReq, sizeof(PressReq), (CHAR*)&PressReq, sizeof(PressReq), 3000);

    ::Sleep(nPeriod);
    PressReq.cmd_code = 0x20;
    PressReq.Hold = 0;
    PressReq.Key = KeyID;

    nRet = objCom.TXRXDiag((CHAR*)&PressReq, sizeof(PressReq), (CHAR*)&PressReq, sizeof(PressReq), 3000);

    if ( nRet < 0)
        nRet = false;

    return nRet;
}

BOOL __stdcall UnlockKeyPad(CSerCOMLibrary &objCom) {
    SendKeyLongPressCommand(objCom, HS_STAR_K, 3000);
    return true;
}

BOOL __stdcall CleanScreenKeyBuffer(CSerCOMLibrary &objCom) {
    SendKeyCommand(objCom, HS_END_K);
    ::Sleep(200);
    return true;
}

BOOL __stdcall EnterLCDTest(CSerCOMLibrary &objCom) {
    //SendKeyCommand(objCom, HS_DOWN_K);
    //::Sleep(100);
    //SendKeyCommand(objCom, HS_DOWN_K);
    //::Sleep(100);
    SendKeyCommand(objCom, HS_SF1_K);
    return true;
}

BOOL __stdcall TestLCDNextPattern(CSerCOMLibrary &objCom) {
    SendKeyCommand(objCom, HS_DOWN_K);
    return true;
}

BOOL __stdcall ReturnToHomeScreen(CSerCOMLibrary &objCom) {
    SendKeyCommand(objCom, HS_END_K);
    ::Sleep(5000);
    SendKeyCommand(objCom, HS_END_K);
    ::Sleep(3000);
    return true;
}

BOOL __stdcall EnterVibratorTest(CSerCOMLibrary &objCom) {
    SendKeyCommand(objCom, HS_0_K);
    ::Sleep(300);
    SendKeyCommand(objCom, HS_SF1_K);
    return true;
}

BOOL __stdcall BackToEM(CSerCOMLibrary &objCom) {
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(1000);
    return true;
}

BOOL __stdcall EnterEMForAthens(CSerCOMLibrary &objCom) {

    // before this open the trapdoor

    // *#36446337301#
    BOOL bRes = TRUE;


    bRes &= SendKeyCommand(objCom, HS_END_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_STAR_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_POUND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_1_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_2_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_3_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_POUND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_2_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_2_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_8_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_4_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_8_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_2_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_8_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_3_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_POUND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_SEND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_END_K);
    ::Sleep(2000);
    bRes &= SendKeyCommand(objCom, HS_END_K);
    ::Sleep(2000);


    bRes &= SendKeyCommand(objCom, HS_STAR_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_POUND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_3_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_6_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_4_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_4_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_6_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_3_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_3_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_7_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_3_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_0_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_1_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_POUND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_SEND_K);
    ::Sleep(400);
    return bRes;
}

BOOL CheckOuputMessage(TCHAR* szTitle, TCHAR* szMessage) {
    string strOutput;
    OutputPrompt(szTitle, szMessage, "YesNo", strOutput);
    if(strOutput == "No") {
        OutputPrompt("Manual CIT Test", "Please try again", "OK", strOutput);
        return FALSE;
    }
    return TRUE;
}

void ConvertReturnValue(const string& strOutput, int& nRet) {
    if(strOutput.compare("Pass") == 0)
        nRet = IDYES;
    else if(strOutput.compare("Fail") == 0)
        nRet = IDNO;
    else if(strOutput.compare("Quit") == 0)
        nRet = IDCANCEL;
}

BOOL __stdcall AudioTest(CSerCOMLibrary &objCom, int& nRet1, int& nRet2) {
    SendKeyCommand(objCom, HS_SF1_K);
    string strOutput;
    LPTSTR msg1 = "Speak into the phone mic.\nCan you hear your voice in the RECEIVER?";
    LPTSTR msg2 = "Speak into the phone mic.\nCan you hear your voice in the SPEAKER?";

    OutputPrompt("Audio Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - Main Audio Path", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF1_K);

    OutputPrompt("Audio Test", msg2, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet2);
    ShowEachTestItemResult("ManualCITTest - Loud Speaker Path", "", nRet2);
    if(nRet2 == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);

    ::Sleep(200);
    return TRUE;
}

BOOL __stdcall KeypadTest(CSerCOMLibrary &objCom, int& nRet1) {
    SendKeyCommand(objCom, HS_SF1_K);
    string strOutput;
    LPTSTR msg1 = "Please press every key which is corresponding to the ones displayed in the phone's display.\nCan all the Keys be successfully pressed?";
    OutputPrompt("Keypad Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - Keypad Test", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    return TRUE;
}
bool LCDTestCommandConfirm(char* szTitle, char* szMessage, char* type, tstring& strOutput, CSerCOMLibrary& objCom) {
    OutputPrompt(szTitle, szMessage, type, strOutput);
    if(memcmp(type, "YesNo", 5) == 0) {
        if(strOutput == "No") {
            OutputPrompt(szTitle, "Please retry again...", "OK", strOutput);
            objCom.CloseComport();
            return false;
        }
    }
    return true;
}

BOOL __stdcall TestLCD(CSerCOMLibrary& objCom, int& nRet1, int& nRet2, int& nRet3, int& nRet4, int& nRet5, int& nRet6, int& nRet7) {
    // First step, outside frame
    LPTSTR msg1 = "Please watch the handset.\nDo you see the outside frame with white color?";
    LPTSTR msg2 = "Please watch the handset.\nDo you see the red color screen?";
    LPTSTR msg3 = "Please watch the handset.\nDo you see the green color screen?";
    LPTSTR msg4 = "Please watch the handset.\nDo you see the blue color screen?";
    LPTSTR msg5 = "Please watch the handset.\nDo you see the white color screen?";
    LPTSTR msg6 = "Please watch the handset.\nDo you see the black color screen with the back light?";
    LPTSTR msg7 = "Please watch the handset.\nDo you see the black color screen without the back light?";

    bool bRes = true;
    string strOutput;
    // First, Frame
    EnterLCDTest(objCom);
    OutputPrompt("LCD Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - Outside frame with white color", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    // Second, Red
    TestLCDNextPattern(objCom);
    OutputPrompt("LCD Test", msg2, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet2);
    ShowEachTestItemResult("ManualCITTest - Red color screen", "", nRet2);
    if(nRet2 == IDCANCEL) return FALSE;
    // Third, Green
    TestLCDNextPattern(objCom);
    OutputPrompt("LCD Test", msg3, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet3);
    ShowEachTestItemResult("ManualCITTest - Green color screen", "", nRet3);
    if(nRet3 == IDCANCEL) return FALSE;
    // Fourth, Blue
    TestLCDNextPattern(objCom);
    OutputPrompt("LCD Test", msg4, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet4);
    ShowEachTestItemResult("ManualCITTest - Blue color screen", "", nRet4);
    if(nRet4 == IDCANCEL) return FALSE;
    // Fourth, White
    TestLCDNextPattern(objCom);
    OutputPrompt("LCD Test", msg5, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet5);
    ShowEachTestItemResult("ManualCITTest - White color screen", "", nRet5);
    if(nRet5 == IDCANCEL) return FALSE;
    // Fifth, Black with light
    TestLCDNextPattern(objCom);
    OutputPrompt("LCD Test", msg6, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet6);
    ShowEachTestItemResult("ManualCITTest - Black with light screen", "", nRet6);
    if(nRet6 == IDCANCEL) return FALSE;
    // Sixth, Black without light
    TestLCDNextPattern(objCom);
    OutputPrompt("LCD Test", msg7, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet7);
    ShowEachTestItemResult("ManualCITTest - Black without light screen", "", nRet7);
    if(nRet7 == IDCANCEL) return FALSE;
    BackToEM(objCom);
    return bRes ? 1: 0;
}

BOOL __stdcall LCDTest(CSerCOMLibrary &objCom) {
    return TRUE;
}

BOOL __stdcall MainDSCTest(CSerCOMLibrary &objCom, int& nRet1) {
    LPTSTR msg1 = "Please watch the handset.\nDo you see the screen from the back camera?";
    string strOutput;
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("Main DSC Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - Back camera", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(300);
    return true;
}

BOOL __stdcall SubDSCTest(CSerCOMLibrary &objCom, int& nRet1) {
    LPTSTR msg1 = "Please watch the handset.\nDo you see the screen from the front camera?";
    string strOutput;
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("Sub DSC Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - Front camera", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(300);
    return true;
}

BOOL __stdcall RTCTest(CSerCOMLibrary &objCom) {
    // Do nothing, handset will APO
    //SendKeyCommand(objCom, HS_DOWN_K);
    //Sleep(100);
    SendKeyCommand(objCom, HS_SF1_K);
    string strOutput;
    OutputPrompt("Manual CIT Test", "handset will power on again, please wait.", "OK", strOutput);
    OutputPrompt("Manual CIT Test", "Please watch the handset.\nIf the handset reboot complete, please press End key to return the main screes.", "YesNo", strOutput);
    if(strOutput == "No") {
        OutputPrompt("Manual CIT Test", "Please try again", "OK", strOutput);
        return FALSE;
    }
    EnterEMForAthens(objCom);
    ::Sleep(300);
    SendKeyCommand(objCom, HS_DOWN_K);
    ::Sleep(300);
    SendKeyCommand(objCom, HS_DOWN_K);
    ::Sleep(300);
    SendKeyCommand(objCom, HS_DOWN_K);
    ::Sleep(300);
    SendKeyCommand(objCom, HS_DOWN_K);
    ::Sleep(300);
    SendKeyCommand(objCom, HS_DOWN_K);
    ::Sleep(300);
    //SendKeyCommand(objCom, HS_SF2_K);
    //::Sleep(100);
    return true;
}

BOOL __stdcall MP3Test(CSerCOMLibrary &objCom, int& nRet1) {
    LPTSTR msg1 = "Please watch the handset.\nDo you hear the MP3 music?";
    string strOutput;
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("MP3 Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - MP3 music", "", nRet1);
    if(nRet1 == IDYES) {
        SendKeyCommand(objCom, HS_SF2_K);
    } else if(nRet1 == IDNO) {
        ::Sleep(3000);
    } else if(nRet1 == IDCANCEL) {
        return FALSE;
    }
    ::Sleep(200);
    return true;
}

BOOL __stdcall BluetoothAudioTest(CSerCOMLibrary &objCom, int& nRet1) {
    //SendKeyCommand(objCom, HS_DOWN_K);
    //Sleep(100);
    LPCTSTR mgs1 = "Please watch the handset.\nIs Bluetooth Audio Test OK?";
    SendKeyCommand(objCom, HS_SF1_K);
    nRet1 = AfxMessageBox(mgs1, MB_YESNOCANCEL);
    ShowEachTestItemResult("ManualCITTest - Bluetooth Audio Test", "", nRet1);
    //if(!CheckOuputMessage("Manual CIT Test", "Please watch the handset.\nIs Bluetooth Audio Test OK?")) return FALSE;
    if(nRet1 == IDYES) {
        SendKeyCommand(objCom, HS_SF2_K);
    } else if(nRet1 == IDNO) {
        //SendKeyCommand(objCom, HS_SF2_K);
    } else if(nRet1 == IDCANCEL) {
        return FALSE;
    }
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(3000);
    return true;
}

BOOL __stdcall VibrationTest(CSerCOMLibrary &objCom, int& nRet1) {
    string strOutput;
    LPTSTR msg1 = "Is vibration on?";
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("Vibration Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - Vibration test", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}

BOOL __stdcall SPLTest(CSerCOMLibrary &objCom, int& nRet1) {
    string strOutput;
    LPTSTR msg1 = "Listen to the speaker of the phone for a ringtone.\nCan you clearly hear the ringtone?";
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("SPL Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet1);
    ShowEachTestItemResult("ManualCITTest - Ringtone test", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}

BOOL __stdcall PeakPowerTest(CSerCOMLibrary &objCom) {
    //SendKeyCommand(objCom, HS_DOWN_K);
    //Sleep(100);
    SendKeyCommand(objCom, HS_SF1_K);
    Sleep(100);
    string strOutput;
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("Manual CIT Test", "GSM Peak Power Test", "YesNoCancel", strOutput);
    SendKeyCommand(objCom, HS_SF2_K);
    Sleep(100);
    SendKeyCommand(objCom, HS_DOWN_K);
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("Manual CIT Test", "DCS Peak Power Test", "YesNo", strOutput);
    SendKeyCommand(objCom, HS_SF2_K);
    Sleep(100);
    SendKeyCommand(objCom, HS_DOWN_K);
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("Manual CIT Test", "PCS Peak Power Test", "YesNo", strOutput);
    SendKeyCommand(objCom, HS_SF2_K);
    Sleep(100);
    SendKeyCommand(objCom, HS_DOWN_K);
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("Manual CIT Test", "WCDMA Peak Power Test", "YesNo", strOutput);
    SendKeyCommand(objCom, HS_SF2_K);
    OutputPrompt("Manual CIT Test", "All functions are correct?", "YesNo", strOutput);
    if(strOutput == "No") {
        OutputPrompt("Manual CIT Test", "Please try again", "OK", strOutput);
        return FALSE;
    }
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}

BOOL __stdcall VideoTest(CSerCOMLibrary &objCom) {
    //SendKeyCommand(objCom, HS_DOWN_K);
    //Sleep(100);
    SendKeyCommand(objCom, HS_SF1_K);
    if(!CheckOuputMessage("Manual CIT Test", "Please watch the handset.\nDo you see the video screen?")) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}

BOOL __stdcall CoinCellTest(CSerCOMLibrary &objCom) {
    //SendKeyCommand(objCom, HS_DOWN_K);
    //Sleep(100);
    SendKeyCommand(objCom, HS_SF1_K);
    if(!CheckOuputMessage("Manual CIT Test", "Please watch the handset.\nIs Coin Cell Test OK?")) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}

BOOL __stdcall SIMCardTest(CSerCOMLibrary &objCom, int& nRet1) {
    //SendKeyCommand(objCom, HS_DOWN_K);
    //Sleep(100);
    LPCTSTR mgs1 = "Please watch the handset.\nIs SIM Card Test OK?";
    SendKeyCommand(objCom, HS_SF1_K);
    nRet1 = AfxMessageBox(mgs1, MB_YESNOCANCEL);
    ShowEachTestItemResult("ManualCITTest - SIM Card test", "", nRet1);
    if(nRet1 == IDCANCEL) return FALSE;
    //if(!CheckOuputMessage("Manual CIT Test", "Please watch the handset.\nIs SIM Card Test OK?")) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}

BOOL __stdcall _SearchBTDevice(int nPort) {
    CNVItem nv(nPort);
    bool bRes = false;
    if((bRes = nv.SearchBTDevice())) {
        Sleep(30000);
        bRes = nv.QuerySearchBTResult();
    }
    if(bRes == false) {
        bRes = nv.SearchBTDevice();
        Sleep(30000);
        bRes = nv.QuerySearchBTResult();
    }
    return bRes;
}

BOOL __stdcall EnterEMForLavernock(CSerCOMLibrary &objCom) {
    BOOL bRes = TRUE;
    bRes &= SendKeyCommand(objCom, HS_END_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_STAR_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_POUND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_3_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_0_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_1_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_POUND_K);
    ::Sleep(400);
    bRes &= SendKeyCommand(objCom, HS_SEND_K);
    ::Sleep(400);
    return bRes;
}

BOOL __stdcall FMRadioTest(CSerCOMLibrary &objCom, int& nRet) {
    string strOutput;
    LPTSTR msg1 = "Is the handset showing Pass?";
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("FM Radio Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet);
    ShowEachTestItemResult("ManualCITTest - FM Radio Test", "", nRet);
    if(nRet == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}

BOOL __stdcall GSensorTest(CSerCOMLibrary &objCom, int& nRet) {
    string strOutput;
    LPTSTR msg1 = "Please shake the handset.\nIs the handset showing Pass?";
    SendKeyCommand(objCom, HS_SF1_K);
    OutputPrompt("GSensor Test", msg1, "PassFailQuit", strOutput);
    ConvertReturnValue(strOutput, nRet);
    ShowEachTestItemResult("ManualCITTest - GSensor Test", "", nRet);
    if(nRet == IDCANCEL) return FALSE;
    SendKeyCommand(objCom, HS_SF2_K);
    ::Sleep(100);
    return true;
}