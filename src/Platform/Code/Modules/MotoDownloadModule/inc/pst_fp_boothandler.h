#ifndef	BOOT_HANDLER_H
#define BOOT_HANDLER_H

//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: PST_FP_BootHandler.h
//
//   General Description: This is the header file of the Boot Handler class.  It is the base class
//                        of the Flash Device API class.  It contains methods that send boot commands
//                        to SU.
//	 
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2005 - 2010, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Manny Roxas	               04/06/2000      CSGceXXXXX   Initial version.
// Manny Roxas	               05/16/2000	   CSGce58394   Fagan Inspection Update: PST_FP_
//                                                          BootHandler.h and PST_FP_BootHander.cpp.
// Manny Roxas                 07/11/2000      CSGce60032   PST Operation via RS232 transport.
// Manny Roxas                 07/24/2000      CSGce61037   Add SendFlashTestCmd() function.
// Manny Roxas                 08/29/2000      CSGce60032   Add support for new PST command API for
//                                                          RS232.
// Manny Roxas                 09/15/2000      CSGce72792   Fix flashing of naked phones.
//
// Steve Scherer			   08/09/2001      LIBbb01841   Changed units on READ_WRITE_MAX_TIMEOUT  
//
// Mike Doehler                06/13/2002      LIBbb32695   Added max timeout for erase(ERASE_READ_WRITE_MAX_TIMEOUT)
//															from msec to sec, ie 60,000 to 60.
//
// Steve Scherer               04/02/2003      LIBbb91513   Added new pass parameters for handling of 
//                                                          Neptune ROM2 USB issues.
// Steve Scherer               04/21/2003      LIBbb95838   Change the MAX timeout on ERASE to 400sec for Denali
//                                                          stacked flash support.
// 
// Marc Risop                  09/24/2003      LIBdd42886	Added SendSUMaddr method.
//
// Dindo Jao                   08/26/2004      LIBff00480   Retry reading a response to a command sent when an 
//                                                          invalid response is received.
// Marc Risop                  05/04/2004      LIBee05617   Using adapter layer to communicate with core
// Dindo Jao                   09/14/2004      LIBdd66282   Support for Bulverde phone
// Dindo Jao                   11/11/2004      LIBff33728   Remove debug message boxes.
// Marc Risop                   12/2/2004       LIBff41740  Fix problem flashing blank neptune lte with bad rom running
// Dindo Jao                   01/24/2005      LIBff50297   Added wait timeout seconds.
// Dindo Jao                   03/21/2005      LIBff75519   Add an extra parameter when initializing the class.
//
// Satya Calloji               03/30/2005      LIBff71882   For RQVN Issue
//
// Tiago Armani                05/04/2005      LIBff30411   Added IntelReadData and IntelWriteData functions
//
// Tiago Armani                07/28/2005      LIBgg35216   Add support to Boot Command Configuration (timeout and retries)
// Tiago Armani                08/11/2005      LIBgg13321   Added functions to retrieve and set the msg handlers
//
// Satya Calloji               12/30/2005      LIBgg64966   Changing the RQVN logic to improve yield as per GNPO logic
//
// Satya Calloji               08/25/2005      LIBgg77383   Support for ODM flashing
//
// Samir Khial                 02/15/2006      LIBgg77383   Rework ODM Naked/Reflash support
//  
// Satya Calloji               08/07/2006      LIBjj52074   RDL Waitime dynamic assignment
//
// Samir Khial                 25/01/2007      LIBkk83083   Add TI OMAP support
//
// Norton Santos               08/07/2007      LIBff89694   Old PST related code removal
//
// Tingting Dong			   07/17/2007	   LIBmm76870   Add RQHSTAT command sending
//
// Zhiyong Yang                08/27/2008      LIBqq62200   Flash: Add workaround for Ischia Zero-Length packet issue
//
// Satya Calloji               10/08/2008      LIBrr23332   Multi packet logic changes

//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------
// External Dependencies
#include "PstCommon.h"
#include "PST_FP_BootCommand.h"
#include "FlashAdapterApi.h"
#include "PST_FP_FlashDevApiCommon.h"

//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------
#define WAITBINLEN 19
//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                               STRUCTURES AND OTHER TYPEDEFS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                               GLOBAL VARIABLE DECLARATIONS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                   FUNCTION PROTOTYPES
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                         CLASS
//--------------------------------------------------------------------------------------------------

class CBootHandler
{
// Constructor and Destructor
public:
	CBootHandler();
	~CBootHandler();
    
    void SetGUIMsgHandle(HWND hGUI)     { m_hGUIMsgHwnd = hGUI; };

    void SetCallerMsgHandle(HWND hCall) { m_hMsgHwnd = hCall;   };
    void SetWaitTimeCfg(WAIT_TIME_CFG &WaitCfg);
    // Operations
protected:
    BOOT_CMD_CFG_STRUC m_bccsCfg[E_TOT_BOOT_CMDS];
    WAIT_TIME_CFG m_WaitCfg;

    BOOL GenericReadData( BYTE *pbyData,DWORD dwDataSize,DWORD *pdwRetDataSize,
                           WORD  wMaxTimeout,DWORD *pdwErrSts );

    BOOL GenericWriteData( BYTE *pbyData,DWORD dwDataSize,WORD  wMaxTimeout,
                           DWORD *pdwErrSts );

	BOOL InitHandler( DEVICEID  DeviceID, HANDLE hInterface, 
        CFlashAdapterApi *pFlashAdapter, HWND hWnd, DWORD *pdwErrSts );

	BOOL SendRAMbootstrap( DWORD dwAddress, BYTE *pbyDataBuffer, DWORD dwLength,
		                   DWORD *pdwErrSts );

    BOOL RecvCommandAck(BYTE* pbyCommand, DWORD dwCmdSize, WORD wTimeout, BYTE* pbyRespData, DWORD* pdwRespLen, DWORD* pdwErrSts, BOOL bBadRomRunning,int nRetries=3);
	
    BOOL SendSUaddr( BOOL bSUaddr, DWORD dwAddress, DWORD *pdwErrSts, BOOL bBadRom = FALSE );

	BOOL SendCommandData( BYTE *pbyCommand, BYTE *pbyData, DWORD dwDataSize,
                          BOOL  bGetResp, DWORD *pdwErrSts, BOOL bBadRom,
                          eBootCmd ebceActualCmd);

	BOOL CheckRAMdownload( DWORD *pdwErrSts, BOOL bBadRom=FALSE );

    BOOL ReadPacket( BYTE *pbyPcktData, DWORD pdwBufferSize, DWORD *pdwDataLength,
		             WORD  wMaxTimeout, DWORD *pdwErrSts );

	BOOL SendSUMaddr(BYTE byNumAddresses, DWORD *dwAddress, DWORD *pdwErrSts, BOOL bBadRomRunning = FALSE);

    HWND& GetCallerMsgHandle(void)       { return m_hMsgHwnd;    };

    HWND& GetGUIMsgHandle(void)          { return m_hGUIMsgHwnd; };


    void InitBootCmdsCfgStruct();

    void SetBootCmdCfg(BOOT_CMD_CFG_STRUC (*bccsCfgOrig)[E_TOT_BOOT_CMDS]);

	BOOL SendRQHSTAT(DWORD dwMode,			  //IN
					 BYTE *pbyRespData,		  //OUT
					 DWORD *pdwLength,		  //OUT
					 DWORD *pdwErrSts);		  //OUT

    // MNR - RS232
    DEVICEID GetDeviceID( void ){ return m_DeviceID; };

    BOOL SendRQVN(BOOL bGetResp,BOOL bBadRomRunning,BYTE *pbyRespData,DWORD *pdwRespLen,DWORD *pdwErrSts);    

// Attributes
	HINSTANCE     m_hPSTinstance;  // PST Supervisor DLL's instance.
	DEVICEID      m_DeviceID;      // SU's device ID.
	HANDLE		  m_hInterface;    // Interface handle 
    CFlashAdapterApi *m_pFlashAdapterApi;
	HWND		  m_hMsgHwnd;     // this is the caller handle (usually flash.dll)
    HWND		  m_hGUIMsgHwnd;  // this is the GUI handle (for example: RSD LITE)
    
    BOOL          m_bZeroRetryRead; // if TRUE, will retry reading when Zero-Length ACK received
    BYTE          m_byWaitBinData[WAITBINLEN+1];  //used to store the multiple response, current only used for bin command

// Implementations

	BYTE CalByteChkSum( BYTE *pbyBuffer, DWORD dwBufferSize );
    BOOL WritePacket( BYTE *pbyPacket, DWORD dwPacketSize, DWORD *pdwErrSts );
    BOOL ExtractHdrControl( EXT_HDR_CTL_STRUC *pExtHdr, DWORD *pdwErrSts ); 
	BOOL ExtractACKresp( BYTE *pbySrcBuffer, DWORD *pdwSrcDataLen, BYTE  *pbyDestBuffer, 
						 DWORD *pdwErrSts );    
    BOOL RecvRQVNAck(BOOL bBadROMRunning,BYTE *pbyRespData,
                     DWORD *pdwRespLen,bool *bRcvContinue,DWORD *pdwErrSts);
};

#endif // BOOT_HANDLER_H
