#if !defined(AFX_FLASHADAPTERAPI_H__DE41D690_697E_4FC4_9737_3643C02AC3C5__INCLUDED_)
#define AFX_FLASHADAPTERAPI_H__DE41D690_697E_4FC4_9737_3643C02AC3C5__INCLUDED_

// FlashAdapterApi.h: definition of the CFlashAdapterApi class.
//
//////////////////////////////////////////////////////////////////////


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//--------------------------------------------------------------------------------------------------
//                                                                               
//   Header Name: FlashAdapterApi.h
//
//   General Description: This header file contains the definition of the CFlashAdapterApi class
//
//--------------------------------------------------------------------------------------------------
//
//                            Motorola Confidential Proprietary
//                       Advanced Technology and Software Operations
//                    (c) Copyright Motorola 2004, All Rights Reserved
//   
//
// Revision History:
//                             Modification     Tracking
// Author                          Date          Number     Description of Changes
// -------------------------   ------------    ----------   ----------------------------------------
// Marc Risop                  05/17/2004       LIBee05617   Create adapter object
// Dindo Jao                   10/06/2004       LIBff00480   Added a CRC error code.
// Satya Calloji               02/07/2006       LIBhh76766   Added new method for finding interface values 
// Samir Khial                 05/17/2006       LIBii41706   Add Serial Flash API support
// Samir Khial                 06/07/2006       LIBii47509   Added support for Kenai dll
// Samir Khial                 07/03/2006       LIBii76588   Add ETM test command interface support
// Samir Khial                 08/01/2006       LIBjj44900   Add Nextest status enums
// Samir Khial                 12/19/2006       LIBkk61289   Fixed CDMA-D multi-up issue
// Zhiyong Yang                08/27/2008       LIBqq62200   Flash: Add workaround for Ischia Zero-Length packet issue
// Satya Calloji               09/30/2008       LIBrr11791   Add new API to close handles, if needed (Needed only for Nextest)
//
//--------------------------------------------------------------------------------------------------
//                                       INCLUDE FILES
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                         CONSTANTS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                          MACROS
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//                                           ENUMS
//--------------------------------------------------------------------------------------------------
enum EFlashAdapterStatus
{
    FLASH_ADAPTER_STATUS_OK = 0,
    FLASH_ADAPTER_STATUS_INVALID_HANDLE,
    FLASH_ADAPTER_STATUS_FAIL,
    FLASH_ADAPTER_STATUS_CRC_ERROR,
    FLASH_ADAPTER_STATUS_TIMEOUT,
    FLASH_ADAPTER_STATUS_READ_COMMUNICATIONS_ERROR
};
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


struct _PST_USB_DEVICE_DESCRIPTOR;
typedef struct _PST_USB_DEVICE_DESCRIPTOR PST_USB_DEVICE_DESCRIPTOR;

class CFlashAdapterApi  
{
public:
	CFlashAdapterApi();
	virtual ~CFlashAdapterApi();
  
    virtual int GetInterfaceHandle(IN WORD DeviceID, IN const CString& szInterfaceName, OUT LPHANDLE phInterface);
    virtual int FindInterfaceHandle(IN WORD DeviceID, IN const CString& szInterfaceName, OUT LPHANDLE phInterface);
    virtual int GetProductStringDescriptor(IN WORD DeviceID, CString& sDescriptor);
    virtual int GetInterfaceDescriptorValue(IN WORD DeviceID,HANDLE hInterface, unsigned int vType,BYTE &iDescValue);
    virtual int GetUSBDeviceDescriptor(IN WORD DeviceID, OUT PST_USB_DEVICE_DESCRIPTOR& structDeviceDesc);
    virtual int WriteData(IN WORD DeviceID, IN HANDLE hInterface, IN int PacketType, IN LPCVOID pBuffer, IN LPDWORD pdwLen, IN DWORD timeout);
    virtual int ReadData(IN WORD DeviceID, IN HANDLE hInterface, IN int PacketType, OUT LPVOID pBuffer, OUT LPDWORD pdwLen, IN DWORD timeout);
    virtual int ReleaseDevice(IN WORD DeviceID);
    virtual int OpenDevice(IN WORD DeviceID);
    virtual int SetBaudRate(IN DWORD BaudRate, IN WORD DeviceID);
    virtual int GetBaudRate(IN WORD DeviceID, OUT PDWORD pBaudRate);
    virtual int StartSerialDevice(CString& strComPort, IN WORD DeviceID, BOOL bODMPhone);
    virtual int StopSerialDevice(CString &strComPort);
    virtual int SuspendScanningDevice(IN WORD DeviceID);
    virtual int ResumeScanningDevice(IN WORD DeviceID);
    virtual int RefreshConnections();
    virtual int RegisterForNotifications(HWND hWnd);
    virtual int UnregisterForNotifications(HWND hWnd);
	virtual int SetSerialState(IN WORD DeviceID,IN WORD eState);
    virtual int CloseInterfaceHandle(IN WORD DeviceId, IN HANDLE hInterface);
};

#endif // !defined(AFX_FLASHADAPTERAPI_H__DE41D690_697E_4FC4_9737_3643C02AC3C5__INCLUDED_)
