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

#endif //_MOTOXPRS_HEADER_