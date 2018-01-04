#ifndef __QISDASERVICE_H__
#define __QISDASERVICE_H__

#include "..\odmtma_api\SerCOMLib\SerCOMLibrary.h"
#include "tstl.h"

using namespace std;

// 20080506 Embert Tsai
// The following key definition is from hs.h in \service\hs
typedef enum {            /* KEYS                                        */
    HS_NONE_K        = 0x00,/* No more keycodes available          0x00    */
    HS_ON_HOOK_K     = 0x01,/* phone has gone on-hook              0x01    */
    HS_OFF_HOOK_K    = 0x02,/* phone has gone off-hook             0x02    */
    HS_RING_VOL_0_K  = 0x03,/* ringer volume 0 (Keep 0-2 in order) 0x03    */
    HS_RING_VOL_1_K  = 0x04,/* ringer volume 1 ( and sequential!!) 0x04    */
    HS_RING_VOL_2_K  = 0x05,/* ringer volume 2                     0x05    */
    HS_EAR_UP_K      = 0x06,/* earpiece up                         0x06    */
    HS_EAR_UP_END_K  = 0x07,/* earpiece up + end pressed           0x07    */
    HS_EAR_DOWN_K    = 0x08,/* earpiece down                       0x08    */
    HS_PF1_K         = 0x09,/* softkey #1 (left-most)              0x09    */
#ifdef FEATURE_NAV_KEYPAD                                          
    HS_LEFT_K = HS_PF1_K,   /* left key                            0x09    */
#endif /* FEATURE_NAV_KEYPAD */                                    
    HS_PF2_K         = 0x0A,/* softkey #2 (right-most)             0x0A    */
#ifdef FEATURE_NAV_KEYPAD                                          
    HS_RIGHT_K = HS_PF2_K,  /* right key                           0x0A    */
#endif /* FEATURE_NAV_KEYPAD */                                    
    HS_MSG_K         = 0x0B,/* message waiting                     0x0B    */
    HS_POUND_K       = 0x23,/* '#' key, ASCII '#'                  0x23    */
    HS_STAR_K        = 0x2A,/* '*' key, ASCII '*'                  0x2A    */
    HS_0_K           = 0x30,/* '0' key, ASCII '0'                  0x30    */
    HS_1_K           = 0x31,/* '1' key, ASCII '1'                  0x31    */
    HS_2_K           = 0x32,/* '2' key, ASCII '2'                  0x32    */
    HS_3_K           = 0x33,/* '3' key, ASCII '3'                  0x33    */
    HS_4_K           = 0x34,/* '4' key, ASCII '4'                  0x34    */
    HS_5_K           = 0x35,/* '5' key, ASCII '5'                  0x35    */
    HS_6_K           = 0x36,/* '6' key, ASCII '6'                  0x36    */
    HS_7_K           = 0x37,/* '7' key, ASCII '7'                  0x37    */
    HS_8_K           = 0x38,/* '8' key, ASCII '8'                  0x38    */
    HS_9_K           = 0x39,/* '9' key, ASCII '9'                  0x39    */
    HS_SEND_K        = 0x50,/* Send key                            0x50    */
    HS_END_K         = 0x51,/* End key or Power key (Target Based) 0x51    */
    HS_CLR_K         = 0x52,/* Clear key                           0x52    */
    HS_STO_K         = 0x53,/* Store key                           0x53    */
    HS_UP_K          = 0x54,/* Up-arrow key was pressed            0x54    */
    HS_DOWN_K        = 0x55,/* Down-arrow key was pressed          0x55    */
    HS_MUTE_K        = 0x56,/* Mute Key                            0x56    */
    HS_RCL_K         = 0x57,/* Recall key                          0x57    */
    HS_SD1_K         = 0x58,/* speed dial #1                       0x58    */
    HS_SD2_K         = 0x59,/* speed dial #2                       0x59    */
    HS_SD3_K         = 0x5A,/* speed dial #3                       0x5A    */
    HS_MENU_K        = 0x5B,/* Menu key                            0x5B    */
    HS_ALPHA_K       = 0x5C,/* Alpha key                           0x5C    */
    HS_DEC_POUND_K   = 0x5D,/* Decoded # DTMF                      0x5D    */
    HS_DEC_STAR_K    = 0x5E,/* Decoded * DTMF                      0x5E    */
    HS_DEC_0_K       = 0x5F,/* Decoded 0 DTMF                      0x5F    */
    HS_DEC_1_K       = 0x60,/* Decoded 1 DTMF                      0x60    */
    HS_DEC_2_K       = 0x61,/* Decoded 2 DTMF                      0x61    */
    HS_DEC_3_K       = 0x62,/* Decoded 3 DTMF                      0x62    */
    HS_DEC_4_K       = 0x63,/* Decoded 4 DTMF                      0x63    */
    HS_DEC_5_K       = 0x64,/* Decoded 5 DTMF                      0x64    */
    HS_DEC_6_K       = 0x65,/* Decoded 6 DTMF                      0x65    */
    HS_DEC_7_K       = 0x66,/* Decoded 7 DTMF                      0x66    */
    HS_DEC_8_K       = 0x67,/* Decoded 8 DTMF                      0x67    */
    HS_DEC_9_K       = 0x68,/* Decoded 9 DTMF                      0x68    */
    HS_DEC_A_K       = 0x69,/* Decoded A DTMF                      0x69    */
    HS_DEC_B_K       = 0x6A,/* Decoded B DTMF                      0x6A    */
    HS_DEC_C_K       = 0x6B,/* Decoded C DTMF                      0x6B    */
    HS_DEC_D_K       = 0x6C,/* Decoded D DTMF                      0x6C    */
    HS_PWR_ON_K      = 0x6D,/* Power key is switched 'on'          0x6D    */
    HS_PWR_OFF_K     = 0x6E,/* Power key is switched 'off'         0x6E    */
    HS_PWR_K         = 0x6F,/* Power key                           0x6F    */
    HS_INFO_K        = 0x70,/* Info key                            0x70    */
#if defined(FEATURE_NAV_KEYPAD) || defined(FEATURE_RESISTIVE_KEYPAD)
    HS_SEL_K = HS_INFO_K,   /* Select key                          0x70    */
#endif /* FEATURE_NAV_KEYPAD */
    HS_FREE_K        = 0x71,/* Phone placed in hands-free cradle   0x71    */
    HS_PHONE_K       = 0x72,/* Phone lifted from hands-free cradle 0x72    */
    HS_IGN_OFF_K     = 0x73,/* Ignition was turned off             0x73    */
    HS_IGN_ON_K      = 0x74,/* Ignition was turned on              0x74    */
    HS_TMR_ON_K      = 0x75,/* Power-up timer 'on' pseudo-key      0x75    */
    HS_TMR_OFF_K     = 0x76,/* Power-up timer 'off' pseudo-key     0x76    */
    HS_BAD_BATT_K    = 0x77,/* The car battery is failing          0x77    */
    HS_EXT_PWR_ON_K  = 0x78,/* External power was turned on        0x78    */
    HS_EXT_PWR_OFF_K = 0x79,/* External power was turned off       0x79    */
    HS_REDIAL_K      = 0x7A,/* Redial key                          0x7A    */
    HS_RSSI_K        = 0x7B,/* RSSI key                            0x7B    */
    HS_HFK_CORD_K    = 0x7C,/* Coil cord carkit                    0x7C    */
    HS_HFK_NOCORD_K  = 0x7D,/* Non-coil cord carkit                0x7D    */
    HS_HEADSET_K     = 0x7E,/* (Mono) Headset connected            0x7E    */
    HS_DEV_UNKNOWN_K = 0x7F,/* Unknown device ID                   0x7F    */
    HS_EXT_DEV_K     = 0x80,/* ONES detector finds external device 0x80    */
    HS_STEREO_HEADSET_K = 0x82,/* Stereo Headset connected         0x82    */
    HS_HEADSET_OFF_K = 0x83,/* Headset disconnected                0x83    */

    HS_HEADSET_SWITCH_K = 0x84,/* Headset Switch on                0x84    */
    HS_HEADSET_SWITCH_OFF_K = 0x85,/* Headset Switch off           0x85    */

    HS_LSPKR_K       = 0x86,/* Loudspeaker key                     0x86    */
    HS_PTT_K         = 0x87,/* PTT key                             0x87    */
    HS_CHARGING_ON_K = 0x88,/* Key to note battery charging active 0x88    */
    HS_CHARGING_OFF_K= 0x89,/* Key to note battery charging stopped0x89    */

    HS_SDAC_ON_K     = 0x8A,/* Key to note SDAC in use             0x8A    */
    HS_SDAC_OFF_K    = 0x8B,/* Key to note SDAC not in use         0x8B    */

    HS_USB_HFK_ON_K  = 0x8C,/* Key to note USB in use              0x8C    */
    HS_USB_HFK_OFF_K = 0x8D,/* Key to note USB not in use          0x8D    */
    HS_USB_HFK_BUTTON_K = 0x8E,/* Basic carkit button              0x8E    */
    HS_USB_HFK_GENERIC_BUTTON_K = HS_USB_HFK_BUTTON_K,
    /* rename key, other USB keys start at 0xA6 */

    HS_VOL_UP_K      = 0x8F,/* Volume Up Key                       0x8F    */
    HS_VOL_DOWN_K    = 0x90,/* Volume Down Key                     0x90    */
    HS_FA_K          = 0x91,/* FA Key                              0x91    */
    HS_FB_K          = 0x92,/* FB Key                              0x92    */
    HS_SF1_K         = 0x93,/* SF1 Key                             0x93    */
    HS_SF2_K         = 0x94,/* SF2 Key                             0x94    */
    HS_R_UP_K        = 0x95,/* R+ Key                              0x95    */
    HS_R_DOWN_K      = 0x96,/* R- Key                              0x96    */
    HS_F1_K          = 0x97,/* F1 Key                              0x97    */
    HS_F2_K          = 0x98,/* F2 Key                              0x98    */
    HS_F3_K          = 0x99,/* F3 Key                              0x99    */
    HS_F4_K          = 0x9A,/* F4 Key                              0x9A    */
    HS_F5_K          = 0x9B,/* F5 Key                              0x9B    */
    HS_FLIP_OPEN_K   = 0x9C,/* Flip Open Key (Clamshell)           0x9C    */
    HS_FLIP_CLOSE_K  = 0x9D,/* Flip close Key (Clamshell)          0x9D    */

    HS_LS1_K         = 0x9E,/* Left soft Key                       0x9E    */
    HS_LSK_K         = HS_LS1_K,/* Left front key                  0x9E    */

    HS_LS2_K         = 0x9F,/* Right soft Key                      0x9F    */
    HS_RSK_K         = HS_LS2_K,/* Right front key                 0x9F    */

    HS_TASK_K        = 0xA0,/* Task Key                            0xA0    */
    HS_VIDEO_K       = 0xA1,/* Video Key                           0xA1    */
    HS_CAMERA_K      = 0xA2,/* Camera Key                          0xA2    */
    HS_WEB_K         = 0xA5,/* Web Key                             0xA5    */

    HS_USB_HFK_SEND_BUTTON_K = 0xA6,
    /* Smart carkit send                   0xA6    */
    HS_USB_HFK_END_BUTTON_K = 0xA7,
    /* Smart carkit end                    0xA7    */
    HS_USB_HFK_VOICEDIAL_BUTTON_K = 0xA8,
    /* Smart carkit voice dial             0xA8    */
    HS_USB_HFK_RETURN_TO_CRADLE_K = 0xA9,
    /* Smart carkit return to cradle       0xA9    */
    HS_USB_HFK_REMOVE_FROM_CRADLE_K = 0xAA,
    /* Smart carkit remove from cradle     0xAA    */
    HS_USB_STEREO_HFK_ON_K = 0xAB,
    /* Stereo carkit attached              0xAB    */
    HS_PLAY_K        = 0xAC,
    /* Play                                0xAC    */
    HS_PAUSE_K       = 0xAD,
    /* Pause                               0xAD    */
    HS_STOP_K        = 0xAE,
    /* Stop                                0xAE    */
    HS_FFWD_K        = 0xAF,
    /* Fast Forward                        0xAF    */
    HS_REW_K         = 0xB0,
    /* Rewind                              0xB0    */
    HS_LOCATE_REQ_K  = 0xB1,
    /* Location request                    0xB1    */
    HS_TV_K          = 0xB2,
    /* TV Key on JAFFA FFA for 6280+UBM    0xB2    */
    /*Qisda, Julius Lin, 2007/11/08, for Corrs GPS key*/  
    HS_GPS_K		  = 0xB4,
    /*Qisda, Julius Lin, 2007/11/08, for Corrs GPS key*/ 

    HS_RELEASE_K     = 0xFF /* Key to note that all keys are up    0xFF    */
} hs_key_type;

struct SKeyPressReq{
    byte cmd_code;
    byte Hold;
    byte Key;
};

struct SKeyPressResp{
    byte cmd_code;
    byte Hold;
    byte Key;
};

// Define several functions for MCIT testing
//  [6/4/2008 Eddy.Guo]
BOOL SendKeyCommand(CSerCOMLibrary& objCom, byte KeyID);

BOOL __stdcall AudioTest(CSerCOMLibrary &objCom, int& nRet1, int& nRet2);

BOOL __stdcall KeypadTest(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall LCDTest(CSerCOMLibrary &objCom);

BOOL __stdcall TestLCD(CSerCOMLibrary& objCom, int& nRet1, int& nRet2, int& nRet3, int& nRet4, int& nRet5, int& nRet6, int& nRet7);

BOOL __stdcall MainDSCTest(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall SubDSCTest(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall RTCTest(CSerCOMLibrary &objCom);

BOOL __stdcall MP3Test(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall BluetoothAudioTest(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall VibrationTest(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall SPLTest(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall PeakPowerTest(CSerCOMLibrary &objCom);

BOOL __stdcall VideoTest(CSerCOMLibrary &objCom);

BOOL __stdcall CoinCellTest(CSerCOMLibrary &objCom);

BOOL __stdcall SIMCardTest(CSerCOMLibrary &objCom, int& nRet1);

BOOL __stdcall UnlockKeyPad(CSerCOMLibrary &objCom);

BOOL __stdcall CleanScreenKeyBuffer(CSerCOMLibrary &objCom);

BOOL __stdcall EnterLCDTest(CSerCOMLibrary &objCom);

BOOL __stdcall TestLCDNextPattern(CSerCOMLibrary &objCom);

BOOL __stdcall ReturnToHomeScreen(CSerCOMLibrary &objCom);

BOOL __stdcall EnterEMForAthens(CSerCOMLibrary &objCom);

BOOL __stdcall EnterVibratorTest(CSerCOMLibrary &objCom);

BOOL __stdcall BackToEM(CSerCOMLibrary &objCom);

BOOL __stdcall _SearchBTDevice(int nPort);

BOOL __stdcall EnterEMForLavernock(CSerCOMLibrary &objCom);

BOOL __stdcall FMRadioTest(CSerCOMLibrary &objCom, int& nRet);

BOOL __stdcall GSensorTest(CSerCOMLibrary &objCom, int& nRet);

#endif