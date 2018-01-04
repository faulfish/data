#ifndef _MOTOXPRS_HEADER_
#define _MOTOXPRS_HEADER_

typedef struct  {
	unsigned short   usHdr;
	unsigned short   usDataLen;
	unsigned char    ucIMEI[8];
	unsigned char    PICASSO[80];
	unsigned char    SJUG[16];
	unsigned char    DOM1[6];
	unsigned char    DOM2[6];
	unsigned char    DOM3[6];
	unsigned char    ProductID[6];
	unsigned int     uiTFTONCnt;
	char             cFailByteId;
	unsigned char    SWVer[24];
	unsigned char    HWVer[20];
	unsigned char    FilesInPhone[30][5];
	unsigned short   SoftResetCnt;
	unsigned short   PowerCutCnt;
	unsigned int     uiTotalCallCnt;
	unsigned short   usIncomingDroppedCallCnt;
	unsigned short   usOutgoingDroppedCallCnt;
	unsigned short   usIncomingCallCnt;
	unsigned short   usOutgoingCallCnt;
	unsigned short   usIncomingSMSCnt;
	unsigned short   usOutgoingSMSCnt;
	unsigned short   usIncomingMMSCnt;
	unsigned short   usOutgoingMMSCnt;
	unsigned short   usKEY0Cnt;
	unsigned short   usKEY1Cnt;
	unsigned short   usKEY2Cnt;
	unsigned short   usKEY3Cnt;
	unsigned short   usKEY4Cnt;
	unsigned short   usKEY5Cnt;
	unsigned short   usKEY6Cnt;
	unsigned short   usKEY7Cnt;
	unsigned short   usKEY8Cnt;
	unsigned short   usKEY9Cnt;
	unsigned short   usKEYStarCnt;
	unsigned short   usKEYPoundCnt;
	unsigned short   usSoftKEY1Cnt;
	unsigned short   usSoftKEY2Cnt;
	unsigned short   usSendKEYCnt;
	unsigned short   usEndKEYCnt;
	unsigned short   usMenuKEYCnt;
	unsigned short   usSmartKEYCnt;
	unsigned short   usVolumnUpCnt;
	unsigned short   usVolumnDownCnt;
	unsigned short   usClearKEYCnt;
	unsigned short   usJoyStickUpCnt;
	unsigned short   usJoyStickdownCnt;
	unsigned short   usJoyStickLeftCnt;
	unsigned short   usJoyStickRightCnt;
	unsigned short   usFlipOpenCnt;
	unsigned short   usWiredHeadsetCnt;
	unsigned short   usBTHeadsetCnt;
	unsigned short   usCameraVGACnt;
	unsigned short   usCamera2MCnt;
	unsigned short   usBrowseCnt;
	unsigned short   usFMRadioCnt;
	unsigned short   usGameUsageCnt;
	unsigned short   usSpeakerPhoneCnt;
	unsigned short   usNetworkSearchCnt;
	unsigned short   usTFlashXferCnt;
	unsigned short   usDiffSIMInsertCnt;
	unsigned short   usMIDCHGRCnt;
	unsigned short   usUSBCABLECnt;
	unsigned short   usWallOutletCableCnt;
	unsigned short   usChargingProfile[144];
	unsigned short   usPowerProfile[48];
	unsigned char    ucCustomerComplaintCode[6];
	unsigned char    ucProblemFoundCode[6];
	unsigned char    ucRepairCode[6];
	unsigned char    ucCountryCode[6];
	unsigned char    ucOperator[12];
	unsigned char    ucServiceCenterCode[6];

	unsigned char    usSelectUseTime;
	unsigned int     usFirstUseTime;
	unsigned int     usLastUseTime;

	unsigned int     NANDFlashID;
	unsigned int     LCMdisplayID;

	unsigned int     TTF;

	unsigned int     uiChecksum;
} opxprofile_data;

typedef struct{
	unsigned char subsys_id;
	unsigned short subsys_cmd_code;
	opxprofile_data data;
	unsigned short crc;
	unsigned char packetend;
} MotoXPRSDATA;

PACKED typedef struct  {
	unsigned short   usHdr;                //header
	unsigned short   usDataLen;            //length
	//unsigned char    ucIMEI[8];         //in NV item
	//unsigned char    PICASSO[80];       //in NV item
	//unsigned char    SJUG[16];          //in NV item
	//unsigned char    DOM1[6];           //in NV item
	//unsigned char    DOM2[6];           //in NV item
	//unsigned char    DOM3[6];           //in NV item
	//unsigned char    ProductID[6];      //in NV item
	unsigned int     uiTFTONCnt;          //Time to failure (TFT) on timer
	char             cFailByteId;         //Fail Byte Identifier (FBI)
	unsigned char    SWVer[24];

	//Remove it
	//unsigned char    HWVer[20];

	//unsigned char    FilesInPhone[30][5];     //in NV item
	unsigned short   SoftResetCnt;              //Count of soft reset
	unsigned short   PowerCutCnt;               //Count of power cut

	unsigned int     uiTotalDroppedCallCnt;     //Total Dropped Call
	unsigned short   usIncomingDroppedCallCnt;  //Count of dropped calls (call received)
	unsigned short   usOutgoingDroppedCallCnt;  //Count of dropped calls (called out)
	unsigned short   usIncomingCallCnt;         //Count of calls (incoming)
	unsigned short   usOutgoingCallCnt;         //Count of calls (outgoing)
	unsigned short   usNetworkSearchCnt;         //Network Search counter
	unsigned int     uiNoServiceAreaTmr;          //time in no-service area
	unsigned short   usIncomingSMSCnt;          //Count of SMS (incoming)
	unsigned short   usOutgoingSMSCnt;          //Count of SMS (outgoing)
	unsigned short   usIncomingMMSCnt;          //Count of MMS (incoming)
	unsigned short   usOutgoingMMSCnt;          //Count of MMS (outgoing)
	unsigned short   usKEY0Cnt;
	unsigned short   usKEY1Cnt;
	unsigned short   usKEY2Cnt;
	unsigned short   usKEY3Cnt;
	unsigned short   usKEY4Cnt;
	unsigned short   usKEY5Cnt;
	unsigned short   usKEY6Cnt;
	unsigned short   usKEY7Cnt;
	unsigned short   usKEY8Cnt;
	unsigned short   usKEY9Cnt;
	unsigned short   usKEYStarCnt;
	unsigned short   usKEYPoundCnt;
	unsigned short   usSoftKEY1Cnt;
	unsigned short   usSoftKEY2Cnt;
	unsigned short   usSendKEYCnt;
	unsigned short   usEndKEYCnt;
	unsigned short   usMenuKEYCnt;
	unsigned short   usSmartKEYCnt;
	unsigned short   usVolumnUpCnt;
	unsigned short   usVolumnDownCnt;
	unsigned short   usClearKEYCnt;
	unsigned short   usJoyStickUpCnt;
	unsigned short   usJoyStickdownCnt;
	unsigned short   usJoyStickLeftCnt;
	unsigned short   usJoyStickRightCnt;
	unsigned short   usFlipOpenCnt;
	unsigned short   usWiredHeadsetCnt;          //Wired headset plug in counter
	unsigned short   usBTHeadsetCnt;             //Bluetooth headset plug in counter
	unsigned int     uiBTEnableTmr;               //During bluetooth is Enabled
	unsigned short   usCameraVGACnt;             //Camera use counter
	unsigned int     uiCameraVGATmr;             //Total minutes camera used
	unsigned short   usCamera2MCnt;              //Camera use counter
	unsigned int     uiCamera2MTmr;              //Total minutes camera used
	unsigned short   usFMRadioCnt;               //FM Radio use counter
	unsigned short   usBrowseCnt;                //Browser use counter
	unsigned short   usGameUsageCnt;             //Game_Usage_Timer(Counter)
	unsigned short   usSpeakerPhoneCnt;          //counter speakerphone enabled
	unsigned int     uiSpeakerPhoneTmr;          //duration speakerphone enabled
	unsigned short   usVibratorCnt;              //counter vibrator is on
	unsigned int     uiVibratorTmr;              //duration vibrator is on
	unsigned short   usTFlashXferCnt;            //T-Flash plug in counter
	unsigned short   usDiffSIMInsertCnt;         //Number of different SIM card insertions (TF - NTF)
	//unsigned short   usMIDCHGRCnt;
	unsigned short   usUSBCABLECnt;              //The number of times a USB cable was connected
	unsigned short   usWallOutletCableCnt;       //The number of times a wall charger was connected
	unsigned short   usChargingProfile[144];
	unsigned short   usPowerProfile[48];
	//unsigned char    ucCustomerComplaintCode[6];
	//unsigned char    ucProblemFoundCode[6];
	//unsigned char    ucRepairCode[6];
	//unsigned char    ucCountryCode[6];
	//unsigned char    ucOperator[12];
	//unsigned char    ucServiceCenterCode[6];

	unsigned char    usSelectUseTime;               //Phone custody date stamps
	unsigned int     usFirstUseTime;
	unsigned int     usLastUseTime;

	unsigned int     uiLastCall;//Last Completed Calls (Timestamps)

	unsigned int     NANDFlashID;
	unsigned int     LCMdisplayID;
	unsigned short   usBacklightDisplayOnCnt;   //display backlights enabled counter
	unsigned int     uiBacklightDisplayOnTmr;   //Total minutes display backlights enabled
	unsigned short   usBacklightKeypadOnCnt;    //keypad backlights enabled counter
	unsigned int     uiBacklightKeypadOnTmr;    //Total minutes keypad backlights enabled

	unsigned int     uiTotalCallCnt;            //Total accumulated call times
	unsigned int     uiRSSITmr[6];              //Timer for each bar of RSSI

	//Add for GSensor on 2009/2/10
	unsigned short   usuPResetCnt;             //Count of uP reset
	unsigned int     uiHitShakeTwiceCnt;       //Hit of shake twice
	unsigned int     uiHitLeftRightCnt;        //Hit of Hit of left/right
	unsigned int     uiINTShakeTwiceCnt;       //Interrupt count of shake twice
	unsigned int     uiINTLeftRightCnt;        //Interrupt count of left/right
	unsigned short   usShakeTwiceTmr;          //Duration of shake twice (ms)
	unsigned short   usTipTapTmr;              //Duration of tip tap (ms)
	unsigned short   usFreeFallCnt;            //Count of free fall
	unsigned char    ucuPVersion;              //Latest working normal uP version
	unsigned short   usuPRedownloadCnt;        //uP redownload count

	unsigned int     uiChecksum;               //Checksum status
} opxprofile_data_Lavernock;

typedef struct{
	unsigned char subsys_id;
	unsigned short subsys_cmd_code;
	opxprofile_data_Lavernock data;
	unsigned short crc;
	unsigned char packetend;
} MotoXPRSDATA_Lavernock;

PACKED typedef struct  {
	unsigned short   usHdr;                //header
	unsigned short   usDataLen;            //length
	//unsigned char    ucIMEI[8];         //in NV item
	//unsigned char    PICASSO[80];       //in NV item
	//unsigned char    SJUG[16];          //in NV item
	//unsigned char    DOM1[6];           //in NV item
	//unsigned char    DOM2[6];           //in NV item
	//unsigned char    DOM3[6];           //in NV item
	//unsigned char    ProductID[6];      //in NV item
	unsigned int     uiTFTONCnt;          //Time to failure (TFT) on timer
	char             cFailByteId;         //Fail Byte Identifier (FBI)
	unsigned char    SWVer[24];
	unsigned char    HWVer[20];
	//unsigned char    FilesInPhone[30][5];     //in NV item
	unsigned short   SoftResetCnt;              //Count of soft reset
	unsigned short   PowerCutCnt;               //Count of power cut

	unsigned int     uiTotalDroppedCallCnt;     //Total Dropped Call
	unsigned short   usIncomingDroppedCallCnt;  //Count of dropped calls (call received)
	unsigned short   usOutgoingDroppedCallCnt;  //Count of dropped calls (called out)
	unsigned short   usIncomingCallCnt;         //Count of calls (incoming)
	unsigned short   usOutgoingCallCnt;         //Count of calls (outgoing)
	unsigned short   usNetworkSearchCnt;         //Network Search counter
	unsigned int     uiNoServiceAreaTmr;          //time in no-service area
	unsigned short   usIncomingSMSCnt;          //Count of SMS (incoming)
	unsigned short   usOutgoingSMSCnt;          //Count of SMS (outgoing)
	unsigned short   usIncomingMMSCnt;          //Count of MMS (incoming)
	unsigned short   usOutgoingMMSCnt;          //Count of MMS (outgoing)
	unsigned short   usKEY0Cnt;
	unsigned short   usKEY1Cnt;
	unsigned short   usKEY2Cnt;
	unsigned short   usKEY3Cnt;
	unsigned short   usKEY4Cnt;
	unsigned short   usKEY5Cnt;
	unsigned short   usKEY6Cnt;
	unsigned short   usKEY7Cnt;
	unsigned short   usKEY8Cnt;
	unsigned short   usKEY9Cnt;
	unsigned short   usKEYStarCnt;
	unsigned short   usKEYPoundCnt;
	unsigned short   usSoftKEY1Cnt;
	unsigned short   usSoftKEY2Cnt;
	unsigned short   usSendKEYCnt;
	unsigned short   usEndKEYCnt;
	unsigned short   usMenuKEYCnt;
	unsigned short   usSmartKEYCnt;
	unsigned short   usVolumnUpCnt;
	unsigned short   usVolumnDownCnt;
	unsigned short   usClearKEYCnt;
	unsigned short   usJoyStickUpCnt;
	unsigned short   usJoyStickdownCnt;
	unsigned short   usJoyStickLeftCnt;
	unsigned short   usJoyStickRightCnt;
	unsigned short   usFlipOpenCnt;
	unsigned short   usWiredHeadsetCnt;          //Wired headset plug in counter
	unsigned short   usBTHeadsetCnt;             //Bluetooth headset plug in counter
	unsigned int     uiBTEnableTmr;               //During bluetooth is Enabled
	unsigned short   usCameraVGACnt;             //Camera use counter
	unsigned int     uiCameraVGATmr;             //Total minutes camera used
	unsigned short   usCamera2MCnt;              //Camera use counter
	unsigned int     uiCamera2MTmr;              //Total minutes camera used
	unsigned short   usFMRadioCnt;               //FM Radio use counter
	unsigned short   usBrowseCnt;                //Browser use counter
	unsigned short   usGameUsageCnt;             //Game_Usage_Timer(Counter)
	unsigned short   usSpeakerPhoneCnt;          //counter speakerphone enabled
	unsigned int     uiSpeakerPhoneTmr;          //duration speakerphone enabled
	unsigned short   usVibratorCnt;              //counter vibrator is on
	unsigned int     uiVibratorTmr;              //duration vibrator is on
	unsigned short   usTFlashXferCnt;            //T-Flash plug in counter
	unsigned short   usDiffSIMInsertCnt;         //Number of different SIM card insertions (TF - NTF)
	//unsigned short   usMIDCHGRCnt;
	unsigned short   usUSBCABLECnt;              //The number of times a USB cable was connected
	unsigned short   usWallOutletCableCnt;       //The number of times a wall charger was connected
	unsigned short   usChargingProfile[144];
	unsigned short   usPowerProfile[48];
	//unsigned char    ucCustomerComplaintCode[6];
	//unsigned char    ucProblemFoundCode[6];
	//unsigned char    ucRepairCode[6];
	//unsigned char    ucCountryCode[6];
	//unsigned char    ucOperator[12];
	//unsigned char    ucServiceCenterCode[6];

	unsigned char    usSelectUseTime;               //Phone custody date stamps
	unsigned int     usFirstUseTime;
	unsigned int     usLastUseTime;

	unsigned int     uiLastCall;//Last Completed Calls (Timestamps)

	unsigned int     NANDFlashID;
	unsigned int     LCMdisplayID;
	unsigned short   usBacklightDisplayOnCnt;   //display backlights enabled counter
	unsigned int     uiBacklightDisplayOnTmr;   //Total minutes display backlights enabled
	unsigned short   usBacklightKeypadOnCnt;    //keypad backlights enabled counter
	unsigned int     uiBacklightKeypadOnTmr;    //Total minutes keypad backlights enabled

	unsigned int     uiTotalCallCnt;            //Total accumulated call times
	unsigned int     uiRSSITmr[6];              //Timer for each bar of RSSI

	unsigned short     download_times;			  //number of download
	unsigned long long start_download_datetime_ms;// start time of download
	unsigned long long end_download_datetime_ms;  // end time of download
	unsigned char      cnv_restore_status_code;	//CEFS NV restore status
	unsigned char      nv_partition_restore_status_code;   //partition NV restore status
	unsigned short     last_nv_restore_ms;        // last time of nv restore
	unsigned long long alarm_user_setting_datetime_ms;	//alarm user setting time
	unsigned long long alarm_int_datetime_ms;		//alarm interrupt
	unsigned char    ucLastMsgType;			  //last message type
	unsigned int uiLastAlarm;					 //last alarm launch time
	unsigned int uiLastMC;					 //last master clear time
	unsigned int uiLastMR;					 //last master reset time

	unsigned int     uiChecksum;               //Checksum status
} opxprofile_data_Melos;

typedef struct{
	unsigned char subsys_id;
	unsigned short subsys_cmd_code;
	opxprofile_data_Melos data;
	unsigned short crc;
	unsigned char packetend;
} MotoXPRSDATA_Melos;

#endif //_MOTOXPRS_HEADER_