#ifndef _EnumFiles_H_
#define _EnumFiles_H_



#define MAX_ARRAY_SIZE								100
#define FUNCTIONFAIL								99

#define PHONE_MODE_WCDMA_IMT						9
#define GSM_BAND_900								10
#define GSM_BAND_DCS								11
#define GSM_BAND_PCS								12
#define GSM_BAND_850								18

#define GSM_BAND_900_to_850							28

#define	SUCCESS										0
#define PASS										1


#define POWER_TEST								0x0001
#define FREQ_PHASE_TEST							0x0002
#define ORFS_TEST								0x0004
#define SENSITIVITY_TEST						0x0008

#define OFF				0
#define ON				1
#define START		    2
#define RELEASE         3


typedef enum
{
	FTM_GSM_BER_PGSM_900,
	FTM_GSM_BER_EGSM_900,
	FTM_GSM_BER_PCS_1900,
	FTM_GSM_BER_DCS_1800,
	FTM_GSM_BER_CELL_850
} FTM_GSM_BER_Band_Enum;



#endif //_EnumFiles_H_