/******************************************************************************/
/**
Program: QMSL

\file QLib_SoftwareDownloadDefines.h

\brief Definitions required for software download

 \b QUALCOMM  \b PROPRIETARY 

This document contains propriety information, and except with written
permission of Qualcomm INC, such information shall not be 
published, or disclosed to others, or used for any purpose, and the   
document shall not be duplicated in whole or in part.  


\note   
Compiler:  Microsoft Visual C++ v6.0 SP4  
*******************************************************************************/
#ifndef _QLIB_SOFTWARE_DOWNLOAD_DEFINES_H
#define _QLIB_SOFTWARE_DOWNLOAD_DEFINES_H


#include "QLIB_Defines.h"


/***************************************************************************************************************/
/**                  Definitions for generic Software Download events                                                  */
/***************************************************************************************************************/

// Will be delcared later...
union union_generalSwDownloadEvent;

//! Callback for a General SW Download Event
typedef unsigned char( *swd_generalEvent ) 
(
	unsigned char* pGeneralEvent	//	recievers of this call back must cast to: union_generalSwDownloadEvent
);

/**
	enumeration of SW download event types
*/
typedef enum 
{
	SWD_downloadEvent,			//	uses struct downloadEvent_struct;
	SWD_nvBackupEvent,			//	uses struct nvBackupEvent_struct;
	SWD_doAutoRestore,			//  No data
	SWD_reportLoaderStatus,		//	uses struct bootDownloadStatus_struct;
	SWD_bootDownloadStart,		//	uses struct bootDownloadStart_struct;
	SWD_bootReportLoaderStatus,	//	uses struct bootReportLoaderStatus_struct;
	SWD_bootDownloadStatus,		//  uses struct bootDownloadStatus_struct;
	SWD_bootDownloadProgress,	//	uses struct bootDownloadProgress_struct;
	SWD_bootDownloadComplete,	//  No data
	SWD_MIReportLoaderStatus,	//	uses struct MIReportLoaderStatus_struct;
	SWD_MIReportFlashStatus,	//	uses struct MIReportFlashStatus_struct;
	SWD_MIStartDownload,		//	uses struct MIStartDownload_struct;
	SWD_MIDownloadProgress,		//	uses struct MIDownloadProgress_struct;
	SWD_MIDownloadComplete,		//	No data
	SWD_MIDownloadStatus,		//	uses struct MIDownloadStatus_struct;	
	SWD_Event_None				//  No event specified
} SwDownload_EventTypes;

#define SWD_StringMaxSize 100

/*
	The following is a set of structures and a union which will handle all software download
	events.  This eliminates the need for separate event classes and call back functions.

	In other words, there will be one single event handler call back and it will process
	a union of events
*/

//! Callback for a download event
typedef struct 
{
	unsigned long status;
	unsigned short error;
	unsigned short percentCompleted;
	unsigned short block;
	unsigned long address;
	char dataString[SWD_StringMaxSize ];
} downloadEvent_struct;

//! Callback for a NV Backup Event
typedef struct 
{
	unsigned long status;
	unsigned short error;
	unsigned short percentCompleted;
	unsigned short nvItem;
	char dataString[SWD_StringMaxSize ];
} nvBackupEvent_struct;


typedef struct 
{
	char flashName[SWD_StringMaxSize ];
	unsigned long loaderSize;
} bootDownloadStart_struct;

typedef struct 
{
	unsigned long statusCode;
	unsigned long detailCode;
} bootDownloadStatus_struct;

typedef struct 
{
	char bsLoaderName[SWD_StringMaxSize ];
	long  customLoader;
	long  localFile;
} reportLoaderStatus_struct;


typedef struct 
{
	char bsLoaderName[SWD_StringMaxSize ];
	long  customLoader;
	long  localFile;
} bootReportLoaderStatus_struct;

typedef struct 
{
	unsigned long bytesWritten;
} bootDownloadProgress_struct;

// No need for sepearate structure
// 		virtual void bootDownloadComplete() ;


// IAtlasDownloadEvents3
typedef struct 
{
	char bsLoaderName[SWD_StringMaxSize ];
	long customLoader;
	long localFile;
} MIReportLoaderStatus_struct;

typedef struct 
{
	char flashName[SWD_StringMaxSize ];
} MIReportFlashStatus_struct;

typedef struct 
{
	unsigned long imageSize;
	unsigned long imageType;
} MIStartDownload_struct;

typedef struct 
{
	unsigned long bytesWritten;
} MIDownloadProgress_struct;


typedef struct 
{
	unsigned long statusCode;
	unsigned long detailCode;
} MIDownloadStatus_struct;


/**
	Union of all possible download events
*/
typedef union 
{
	downloadEvent_struct			downloadEvent;
	nvBackupEvent_struct			nvBackupEvent;
	reportLoaderStatus_struct		reportLoaderStatus;
	bootDownloadStart_struct		bootDownloadStart;
	bootDownloadStatus_struct		bootDownloadStatus;
	bootReportLoaderStatus_struct	bootReportLoaderStatus;
	bootDownloadProgress_struct		bootDownloadProgress;
	MIReportLoaderStatus_struct		MIReportLoaderStatus;
	MIReportFlashStatus_struct		MIReportFlashStatus;
	MIStartDownload_struct			MIStartDownload;
	MIDownloadProgress_struct		MIDownloadProgress;
	MIDownloadStatus_struct			MIDownloadStatus;	
} generalSwDownloadEvent_union;

/**
	Union of all download events + CONTEXT ID + Event type
*/
typedef struct 
{
	SwDownload_EventTypes eEventType;			//!<' Type of event
	HANDLE hContextID;							//!<' Event type
	generalSwDownloadEvent_union uEventData;	//!<' Event Data
} generalSwDownloadEvent_struct;


/**
	Enumeration of file types to be used selected for download
	by uploadMultiImage()

	taken from QPST's sharedinc/MultiImageDownload.h
*/
typedef enum        // File type bitmasks
{
  miType_None        = 0,
  miTypePrtnFile     = 0x0001,
  miTypePblFile      = 0x0002,
  miTypeQcSblFile    = 0x0004,
  miTypeQcSblHdFile  = 0x0008,
  miTypeOemSblFile   = 0x0010,
  miTypeOemSblHdFile = 0x0020,
  miTypeAmssFile     = 0x0040,
  miTypeAmssHdFile   = 0x0080,
  miTypeAppsFile     = 0x0100,
  miTypeAppsHdFile   = 0x0200,
  miTypeOblFile      = 0x0400,
  miTypeAppsBlFile   = 0x0800,
  miTypeAppsBlHdFile = 0x1000,
  miType_All         = 0x1FFF,
  miType_AllExApps   = 0x04FF
} SWD_miFileTypeBitmask_enum;

/*
	Enumeration of Arm prog types, taken from QPST's sharedinc/ArmprgType.h
*/
typedef enum		// Default armprg to use when mobile alread in download mode.
{
  apStandard,		// MSM3000, 3100, 3300, 5000, 5010, 5100, 5105
  apMSM6000,    // MSM6000
  apMSM6050,    // MSM6050
  apMSM6200_1,  // MSM6200 Cheetah
  apMSM6100,    // MSM6100 Cougar with NOR flash
  apMSM6100NAND,// MSM6100 Cougar with NAND flash.
  apMSM6250,      // MSM6250 (Saber) NOR flash
  apMSM6250NAND,  // MSM6250 (Saber) NAND flash
  apMSM6500,      // MSM6500 (Jaguar) NOR flash
  apMSM6500NAND,  // MSM6500 (Jaguar) NAND flash
  apMSM6100NORFFA, // MSM6100 FFA with NOR flash
  apMSM6550,       // MSM6550 (Eagle) NOR flash
  apMSM6550NAND,   // MSM6550 NAND (3/9/05)
  apMSM6550PB,     // MSM6550 progressive boot
  apEM6700,        // MSM6700 emulator (Quasar) 
  apMSM6275NOR,    // MSM6275 (Raven) NOR
  apMSM6275NAND,   // MSM6275 (Raven) NAND
  apMSM6800NOR,    // MSM6800 (DoRA) NOR
  apMSM6800NAND,   // MSM6800 (DoRA) NAND
  apMSM6250Sec,    // MSM6250 secure (one-time programmable)
  apMSM7500NAND,   // MSM7500 (Phoenix)
  apMSMSC1x,       // SC1x a.k.a. QSC, NOR progressive
  apMSM6280NAND,   // MSM6280 NAND multi-image
  apMSM7200NAND,   // MSM7200 NAND multi-image
  apMSM7500NOR,    // MSM7500 NOR multi-image
  apMSM6260NAND,   // MSM6260 NAND
  apMSM6245NAND,   // MSM6245 NAND multi-image, progressive
  apMSM6255aNAND,  // MSM6255A NAND, progressive
  apMSM6800NAND65nm,  // 65nm MSM6800

  apMarkerForDialog // Don't use!
} dloadArmprgType ;


#endif // _QLIB_SOFTWARE_DOWNLOAD_DEFINES_H