/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    hubbusif.h

Abstract:

    Services exported by the Port driver for use by the hub driver.

Environment:

    Kernel mode
   
--*/

#ifndef   __HUBBUSIF_H__
#define   __HUBBUSIF_H__

/*
  Bus interfaces are supported for Windows XP and later only
*/

#if (NTDDI_VERSION >= NTDDI_WINXP)

typedef PVOID PUSB_DEVICE_HANDLE;

typedef struct _ROOTHUB_PDO_EXTENSION {

    ULONG Signature;

} ROOTHUB_PDO_EXTENSION, *PROOTHUB_PDO_EXTENSION;

// legacy flags
#define USBD_DEVHACK_SLOW_ENUMERATION   0x00000001
#define USBD_DEVHACK_DISABLE_SN         0x00000002
#define USBD_DEVHACK_SET_DIAG_ID        0x00000004



#ifndef USB_BUSIFFN
#define USB_BUSIFFN __stdcall
#endif

/****************************************************************************************************

Module: hubbusif.h

name:  Bus interface for USB Hub
Copyright (c) Microsoft Corporation. All rights reserved

Revision History:

    6-20-99 : created
    1-5-03 : revised 


*******************************************************************************************************/


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_CREATE_USB_DEVICE) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE *,
        IN PUSB_DEVICE_HANDLE,
        IN USHORT,
        IN USHORT
    );


#define CD_ERR_V1       0x00000001

typedef enum _USBPORT_CREATEDEV_ERROR {
    CreateDevErrNotSet = 0,
    CreateDevBadHubDevHandle,
    CreateDevFailedAllocDevHandle,
    CreateDevFailedOpenEndpoint,
    CreateDevFailedAllocDsBuff,
    CreateDevFailedGetDs,
    CreateDevTtNotFound,
    CreateDevBadDevHandlePtr
} USBPORT_CREATEDEV_ERROR;

typedef struct _USB_CD_ERROR_INFORMATION {

    ULONG Version;
    USBPORT_CREATEDEV_ERROR PathError;
    ULONG UlongArg1;
    ULONG UlongArg2;
    NTSTATUS NtStatus;
    UCHAR XtraInfo[64];

} USB_CD_ERROR_INFORMATION, *PUSB_CD_ERROR_INFORMATION;


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_CREATE_USB_DEVICE_EX) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE *,
        IN PUSB_DEVICE_HANDLE,
        IN USHORT,
        IN USHORT,
        IN OUT PUSB_CD_ERROR_INFORMATION,
        IN USHORT
    );


#define ID_ERR_V1       0x00000001

typedef enum _USBPORT_INITDEV_ERROR {
    InitDevErrNotSet = 0,
    InitDevFailedSetAddress,
    InitDevFailedPokeEndpoint,
    InitDevBadDeviceDescriptor,
} USBPORT_INITDEV_ERROR;

typedef struct _USB_ID_ERROR_INFORMATION {

    ULONG Version;
    USBPORT_INITDEV_ERROR PathError;
    ULONG Arg1;
    ULONG UsbAddress;
    NTSTATUS NtStatus;
    USBD_STATUS UsbdStatus;
    UCHAR XtraInfo[64];

} USB_ID_ERROR_INFORMATION, *PUSB_ID_ERROR_INFORMATION;


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_INITIALIZE_USB_DEVICE) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE
    );

typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_INITIALIZE_USB_DEVICE_EX) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE,
        IN OUT PUSB_ID_ERROR_INFORMATION
    );


/*
flags passed to remove device
*/

#define USBD_KEEP_DEVICE_DATA   0x00000001
#define USBD_MARK_DEVICE_BUSY   0x00000002


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_REMOVE_USB_DEVICE) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE,
        IN ULONG
    );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_USB_DESCRIPTORS) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE,
        IN OUT PUCHAR,
        IN OUT PULONG,
        IN OUT PUCHAR,
        IN OUT PULONG
    );

typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_RESTORE_DEVICE) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE,
        IN OUT PUSB_DEVICE_HANDLE
    );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_POTRTHACK_FLAGS) (
        IN PVOID,
        IN OUT PULONG
        );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_DEVICE_INFORMATION) (
        IN PVOID,
        IN PUSB_DEVICE_HANDLE,
        IN OUT PVOID,
        IN ULONG,
        IN OUT PULONG
        );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_CONTROLLER_INFORMATION) (
        IN PVOID,
        IN OUT PVOID,
        IN ULONG,
        IN OUT PULONG
        );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_CONTROLLER_SELECTIVE_SUSPEND) (
        IN PVOID,
        IN BOOLEAN
        );

/*
    Not supported in LH
*/
typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_EXTENDED_HUB_INFO) (
        IN PVOID,
        IN PDEVICE_OBJECT,
        IN OUT PVOID,
        IN ULONG,
        IN OUT PULONG
        );

/*
    Not supported in LH
*/
typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_ROOTHUB_SYM_NAME) (
        IN PVOID,
        IN OUT PVOID,
        IN ULONG,
        IN OUT PULONG
        );

typedef PVOID
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_DEVICE_BUSCONTEXT) (
        IN PVOID,
        IN PVOID
        );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_INITIALIZE_20HUB) (
        IN PVOID,
        IN PUSB_DEVICE_HANDLE,
        IN ULONG
        );


typedef BOOLEAN
    (USB_BUSIFFN *PUSB_BUSIFFN_IS_ROOT) (
        IN PVOID,
        IN PVOID
    );


typedef VOID
    (USB_BUSIFFN *PUSB_BUSIFFN_ACQUIRE_SEMAPHORE) (
        IN PVOID
    );


typedef VOID
    (USB_BUSIFFN *PUSB_BUSIFFN_RELEASE_SEMAPHORE) (
        IN PVOID
    );


typedef VOID
    (__stdcall *PRH_INIT_CALLBACK) (
        IN PVOID
        );

typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_ROOTHUB_INIT_NOTIFY) (
        IN PVOID,
        IN PVOID,
        IN PRH_INIT_CALLBACK
        );



typedef VOID
    (USB_BUSIFFN *PUSB_BUSIFFN_FLUSH_TRANSFERS) (
        IN PVOID,
        IN PVOID
    );


typedef ULONG
    (USB_BUSIFFN *PUSB_BUSIFFN_CALC_PIPE_BANDWIDTH) (
        IN PVOID,
        IN PUSBD_PIPE_INFORMATION,
        IN USB_DEVICE_SPEED
    );


typedef VOID
    (USB_BUSIFFN *PUSB_BUSIFFN_SET_BUS_WAKE_MODE) (
        IN PVOID,
        IN ULONG
    );


typedef VOID
    (USB_BUSIFFN *PUSB_BUSIFFN_SET_DEVICE_FLAG) (
        IN PVOID,
        IN GUID *,
        IN PVOID,
        IN ULONG
    );


typedef VOID
    (USB_BUSIFFN *PUSB_BUSIFFN_SET_DEVHANDLE_DATA) (
        IN PVOID,
        IN PVOID,
        IN PDEVICE_OBJECT
    );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_TEST_POINT) (
        IN PVOID,
        IN PVOID,
        IN ULONG,
        IN PVOID
    );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_DEVICE_PERFORMANCE_INFO) (
        IN PVOID,
        IN PUSB_DEVICE_HANDLE,
        IN OUT PVOID,
        IN ULONG,
        IN OUT PULONG
        );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_WAIT_ASYNC_POWERUP) (
        IN PVOID
        );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_GET_DEVICE_ADDRESS) (
        IN PVOID,
        IN PUSB_DEVICE_HANDLE,
        IN OUT PUSHORT DeviceAddress
        );


typedef VOID
    (USB_BUSIFFN *PUSB_BUSIFFN_DEREF_DEVICE_HANDLE) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE,
        IN PVOID,
        IN ULONG
    );


typedef NTSTATUS
    (USB_BUSIFFN *PUSB_BUSIFFN_REF_DEVICE_HANDLE) (
        IN PVOID,
        IN OUT PUSB_DEVICE_HANDLE,
        IN PVOID,
        IN ULONG
    );


#define USB_IDLE_NOT_READY                      0
#define USB_IDLE_READY                          1

typedef ULONG
    (USB_BUSIFFN *PUSB_BUSIFFN_SET_DEVICE_HANDLE_IDLE_READY_STATE) (
        IN PVOID,
        IN PUSB_DEVICE_HANDLE,
        IN ULONG
    );



#define USB_BUSIF_HUB_VERSION_0         0x0000
#define USB_BUSIF_HUB_VERSION_1         0x0001
#define USB_BUSIF_HUB_VERSION_2         0x0002
#define USB_BUSIF_HUB_VERSION_3         0x0003
#define USB_BUSIF_HUB_VERSION_4         0x0004
#define USB_BUSIF_HUB_VERSION_5         0x0005
#define USB_BUSIF_HUB_VERSION_6         0x0006

/* {B2BB8C0A-5AB4-11d3-A8CD-00C04F68747A}*/
DEFINE_GUID(USB_BUS_INTERFACE_HUB_GUID,
0xb2bb8c0a, 0x5ab4, 0x11d3, 0xa8, 0xcd, 0x0, 0xc0, 0x4f, 0x68, 0x74, 0x7a);

typedef struct _USB_BUS_INTERFACE_HUB_V0 {

    USHORT Size;
    USHORT Version;
    // returns
    PVOID BusContext;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;

    // interface specific entries go here

} USB_BUS_INTERFACE_HUB_V0, *PUSB_BUS_INTERFACE_HUB_V0;


typedef struct _USB_BUS_INTERFACE_HUB_V1 {

    USHORT Size;
    USHORT Version;
    // returns
    PVOID BusContext;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;

    // interface specific entries go here

    //
    // functions for the hub driver
    //
    PUSB_BUSIFFN_CREATE_USB_DEVICE CreateUsbDevice;
    PUSB_BUSIFFN_INITIALIZE_USB_DEVICE InitializeUsbDevice;
    PUSB_BUSIFFN_GET_USB_DESCRIPTORS GetUsbDescriptors;
    PUSB_BUSIFFN_REMOVE_USB_DEVICE RemoveUsbDevice;
    PUSB_BUSIFFN_RESTORE_DEVICE RestoreUsbDevice;

    PUSB_BUSIFFN_GET_POTRTHACK_FLAGS GetPortHackFlags;
    PUSB_BUSIFFN_GET_DEVICE_INFORMATION QueryDeviceInformation;


} USB_BUS_INTERFACE_HUB_V1, *PUSB_BUS_INTERFACE_HUB_V1;

/*
*/

typedef struct _USB_BUS_INTERFACE_HUB_V2 {

    USHORT Size;
    USHORT Version;
    // returns
    PVOID BusContext;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;

    // interface specific entries go here

    //
    // functions for the hub driver
    //
    PUSB_BUSIFFN_CREATE_USB_DEVICE CreateUsbDevice;
    PUSB_BUSIFFN_INITIALIZE_USB_DEVICE InitializeUsbDevice;
    PUSB_BUSIFFN_GET_USB_DESCRIPTORS GetUsbDescriptors;
    PUSB_BUSIFFN_REMOVE_USB_DEVICE RemoveUsbDevice;
    PUSB_BUSIFFN_RESTORE_DEVICE RestoreUsbDevice;

    PUSB_BUSIFFN_GET_POTRTHACK_FLAGS GetPortHackFlags;
    PUSB_BUSIFFN_GET_DEVICE_INFORMATION QueryDeviceInformation;

    //
    // new functions for version 2
    //
    PUSB_BUSIFFN_GET_CONTROLLER_INFORMATION GetControllerInformation;
    PUSB_BUSIFFN_CONTROLLER_SELECTIVE_SUSPEND ControllerSelectiveSuspend;
    PUSB_BUSIFFN_GET_EXTENDED_HUB_INFO GetExtendedHubInformation;
    PUSB_BUSIFFN_GET_ROOTHUB_SYM_NAME GetRootHubSymbolicName;
    PUSB_BUSIFFN_GET_DEVICE_BUSCONTEXT GetDeviceBusContext;
    PUSB_BUSIFFN_INITIALIZE_20HUB Initialize20Hub;

} USB_BUS_INTERFACE_HUB_V2, *PUSB_BUS_INTERFACE_HUB_V2;


typedef struct _USB_BUS_INTERFACE_HUB_V3 {

    USHORT Size;
    USHORT Version;
    // returns
    PVOID BusContext;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;

    // interface specific entries go here

    //
    // functions for the hub driver
    //
    PUSB_BUSIFFN_CREATE_USB_DEVICE CreateUsbDevice;
    PUSB_BUSIFFN_INITIALIZE_USB_DEVICE InitializeUsbDevice;
    PUSB_BUSIFFN_GET_USB_DESCRIPTORS GetUsbDescriptors;
    PUSB_BUSIFFN_REMOVE_USB_DEVICE RemoveUsbDevice;
    PUSB_BUSIFFN_RESTORE_DEVICE RestoreUsbDevice;

    PUSB_BUSIFFN_GET_POTRTHACK_FLAGS GetPortHackFlags;
    PUSB_BUSIFFN_GET_DEVICE_INFORMATION QueryDeviceInformation;

    //
    // new functions for version 2
    //
    PUSB_BUSIFFN_GET_CONTROLLER_INFORMATION GetControllerInformation;
    PUSB_BUSIFFN_CONTROLLER_SELECTIVE_SUSPEND ControllerSelectiveSuspend;
    PUSB_BUSIFFN_GET_EXTENDED_HUB_INFO GetExtendedHubInformation;
    PUSB_BUSIFFN_GET_ROOTHUB_SYM_NAME GetRootHubSymbolicName;
    PUSB_BUSIFFN_GET_DEVICE_BUSCONTEXT GetDeviceBusContext;
    PUSB_BUSIFFN_INITIALIZE_20HUB Initialize20Hub;

    //
    // new for version 3
    //

    PUSB_BUSIFFN_ROOTHUB_INIT_NOTIFY RootHubInitNotification;

} USB_BUS_INTERFACE_HUB_V3, *PUSB_BUS_INTERFACE_HUB_V3;


typedef struct _USB_BUS_INTERFACE_HUB_V4 {

    USHORT Size;
    USHORT Version;
    // returns
    PVOID BusContext;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;

    // interface specific entries go here

    //
    // functions for the hub driver
    //
    PUSB_BUSIFFN_CREATE_USB_DEVICE CreateUsbDevice;
    PUSB_BUSIFFN_INITIALIZE_USB_DEVICE InitializeUsbDevice;
    PUSB_BUSIFFN_GET_USB_DESCRIPTORS GetUsbDescriptors;
    PUSB_BUSIFFN_REMOVE_USB_DEVICE RemoveUsbDevice;
    PUSB_BUSIFFN_RESTORE_DEVICE RestoreUsbDevice;

    PUSB_BUSIFFN_GET_POTRTHACK_FLAGS GetPortHackFlags;
    PUSB_BUSIFFN_GET_DEVICE_INFORMATION QueryDeviceInformation;

    //
    // new functions for version 2
    //
    PUSB_BUSIFFN_GET_CONTROLLER_INFORMATION GetControllerInformation;
    PUSB_BUSIFFN_CONTROLLER_SELECTIVE_SUSPEND ControllerSelectiveSuspend;
    PUSB_BUSIFFN_GET_EXTENDED_HUB_INFO GetExtendedHubInformation;
    PUSB_BUSIFFN_GET_ROOTHUB_SYM_NAME GetRootHubSymbolicName;
    PUSB_BUSIFFN_GET_DEVICE_BUSCONTEXT GetDeviceBusContext;
    PUSB_BUSIFFN_INITIALIZE_20HUB Initialize20Hub;

    //
    // new for version 3
    //

    PUSB_BUSIFFN_ROOTHUB_INIT_NOTIFY RootHubInitNotification;

    //
    // new for version 4
    //

    PUSB_BUSIFFN_FLUSH_TRANSFERS FlushTransfers;

} USB_BUS_INTERFACE_HUB_V4, *PUSB_BUS_INTERFACE_HUB_V4;


typedef struct _USB_BUS_INTERFACE_HUB_V5 {

    USHORT Size;
    USHORT Version;
    // returns
    PVOID BusContext;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;

    // interface specific entries go here

    //
    // functions for the hub driver
    //
    PUSB_BUSIFFN_CREATE_USB_DEVICE CreateUsbDevice;
    PUSB_BUSIFFN_INITIALIZE_USB_DEVICE InitializeUsbDevice;
    PUSB_BUSIFFN_GET_USB_DESCRIPTORS GetUsbDescriptors;
    PUSB_BUSIFFN_REMOVE_USB_DEVICE RemoveUsbDevice;
    PUSB_BUSIFFN_RESTORE_DEVICE RestoreUsbDevice;

    PUSB_BUSIFFN_GET_POTRTHACK_FLAGS GetPortHackFlags;
    PUSB_BUSIFFN_GET_DEVICE_INFORMATION QueryDeviceInformation;

    //
    // new functions for version 2
    //
    PUSB_BUSIFFN_GET_CONTROLLER_INFORMATION GetControllerInformation;
    PUSB_BUSIFFN_CONTROLLER_SELECTIVE_SUSPEND ControllerSelectiveSuspend;
    PUSB_BUSIFFN_GET_EXTENDED_HUB_INFO GetExtendedHubInformation;
    PUSB_BUSIFFN_GET_ROOTHUB_SYM_NAME GetRootHubSymbolicName;
    PUSB_BUSIFFN_GET_DEVICE_BUSCONTEXT GetDeviceBusContext;
    PUSB_BUSIFFN_INITIALIZE_20HUB Initialize20Hub;

    //
    // new for version 3
    //

    PUSB_BUSIFFN_ROOTHUB_INIT_NOTIFY RootHubInitNotification;

    //
    // new for version 4
    //

    PUSB_BUSIFFN_FLUSH_TRANSFERS FlushTransfers;

    // new for version 5

    PUSB_BUSIFFN_SET_DEVHANDLE_DATA SetDeviceHandleData;

} USB_BUS_INTERFACE_HUB_V5, *PUSB_BUS_INTERFACE_HUB_V5;


typedef struct _USB_BUS_INTERFACE_HUB_V6 {

    USHORT Size;
    USHORT Version;
    // returns
    PVOID BusContext;
    PINTERFACE_REFERENCE InterfaceReference;
    PINTERFACE_DEREFERENCE InterfaceDereference;

    // interface specific entries go here

    //
    // functions for the hub driver
    //
    PUSB_BUSIFFN_CREATE_USB_DEVICE_EX CreateUsbDevice;
    PUSB_BUSIFFN_INITIALIZE_USB_DEVICE_EX InitializeUsbDevice;
    PUSB_BUSIFFN_GET_USB_DESCRIPTORS GetUsbDescriptors;
    PUSB_BUSIFFN_REMOVE_USB_DEVICE RemoveUsbDevice;
    PUSB_BUSIFFN_RESTORE_DEVICE RestoreUsbDevice;

    PUSB_BUSIFFN_GET_POTRTHACK_FLAGS GetPortHackFlags;
    PUSB_BUSIFFN_GET_DEVICE_INFORMATION QueryDeviceInformation;

    //
    // new functions for version 2
    //
    PUSB_BUSIFFN_GET_CONTROLLER_INFORMATION GetControllerInformation;
    PUSB_BUSIFFN_CONTROLLER_SELECTIVE_SUSPEND ControllerSelectiveSuspend;
    PUSB_BUSIFFN_GET_EXTENDED_HUB_INFO GetExtendedHubInformation;
    PUSB_BUSIFFN_GET_ROOTHUB_SYM_NAME GetRootHubSymbolicName;
    PUSB_BUSIFFN_GET_DEVICE_BUSCONTEXT GetDeviceBusContext;
    PUSB_BUSIFFN_INITIALIZE_20HUB Initialize20Hub;

    //
    // new for version 3
    //

    PUSB_BUSIFFN_ROOTHUB_INIT_NOTIFY RootHubInitNotification;

    //
    // new for version 4
    //

    PUSB_BUSIFFN_FLUSH_TRANSFERS FlushTransfers;

    // new for version 5

    PUSB_BUSIFFN_SET_DEVHANDLE_DATA SetDeviceHandleData;

    // new for version 6 - Longhorn

    PUSB_BUSIFFN_IS_ROOT HubIsRoot;
    PUSB_BUSIFFN_ACQUIRE_SEMAPHORE AcquireBusSemaphore;
    PUSB_BUSIFFN_RELEASE_SEMAPHORE ReleaseBusSemaphore;
    PUSB_BUSIFFN_CALC_PIPE_BANDWIDTH CaculatePipeBandwidth;
    PUSB_BUSIFFN_SET_BUS_WAKE_MODE SetBusSystemWakeMode;
    PUSB_BUSIFFN_SET_DEVICE_FLAG SetDeviceFlag;
    PUSB_BUSIFFN_TEST_POINT HubTestPoint;
    PUSB_BUSIFFN_GET_DEVICE_PERFORMANCE_INFO GetDevicePerformanceInfo;
    PUSB_BUSIFFN_WAIT_ASYNC_POWERUP WaitAsyncPowerUp;
    PUSB_BUSIFFN_GET_DEVICE_ADDRESS GetDeviceAddress;
    PUSB_BUSIFFN_REF_DEVICE_HANDLE RefDeviceHandle;
    PUSB_BUSIFFN_DEREF_DEVICE_HANDLE DerefDeviceHandle;
    PUSB_BUSIFFN_SET_DEVICE_HANDLE_IDLE_READY_STATE SetDeviceHandleIdleReadyState;

} USB_BUS_INTERFACE_HUB_V6, *PUSB_BUS_INTERFACE_HUB_V6;





/*
    The following structures are used by the GetDeviceInformation APIs
*/

#include <pshpack1.h>

typedef struct _USB_PIPE_INFORMATION_0 {

    /* pad descriptors to maintain DWORD alignment */
    USB_ENDPOINT_DESCRIPTOR EndpointDescriptor;
    UCHAR ED_Pad[1];

    ULONG ScheduleOffset;
} USB_PIPE_INFORMATION_0, *PUSB_PIPE_INFORMATION_0;

typedef struct _USB_LEVEL_INFORMATION {

    /* inputs: information level requested */
    ULONG InformationLevel;

    /* outputs: */
    ULONG ActualLength;

} USB_LEVEL_INFORMATION, *PUSB_LEVEL_INFORMATION;

typedef struct _USB_DEVICE_INFORMATION_0 {

    /* inputs: information level requested */
    ULONG InformationLevel;

    /* outputs: */
    ULONG ActualLength;

    /* begin level_0 information */
    ULONG PortNumber;

    /* pad descriptors to maintain DWORD alignment */
    USB_DEVICE_DESCRIPTOR DeviceDescriptor;
    UCHAR DD_pad[2];

    UCHAR CurrentConfigurationValue;
    UCHAR ReservedMBZ;
    USHORT DeviceAddress;

    ULONG HubAddress;

    USB_DEVICE_SPEED DeviceSpeed;
    USB_DEVICE_TYPE DeviceType;

    ULONG NumberOfOpenPipes;

    USB_PIPE_INFORMATION_0 PipeList[1];

} USB_DEVICE_INFORMATION_0, *PUSB_DEVICE_INFORMATION_0;



typedef struct _USB_CONTROLLER_INFORMATION_0 {

    ULONG InformationLevel;
    ULONG ActualLength;
    BOOLEAN SelectiveSuspendEnabled;
    BOOLEAN IsHighSpeedController;

} USB_CONTROLLER_INFORMATION_0, *PUSB_CONTROLLER_INFORMATION_0;


typedef struct _USB_CONTROLLER_INFORMATION_1 {

    ULONG InformationLevel;
    ULONG ActualLength;
    BOOLEAN SelectiveSuspendEnabled;
    BOOLEAN IsHighSpeedController;
    ULONG HcBusNumber;
    ULONG HcBusDevice;
    ULONG HcBusFunction;

} USB_CONTROLLER_INFORMATION_1, *PUSB_CONTROLLER_INFORMATION_1;



/*
    Structures that define extended hub port characteristics
*/

typedef struct _USB_EXTPORT_INFORMATION_0 {
    /*
       physical port ie number passed in control commands 1, 2, 3..255
    */
    ULONG                 PhysicalPortNumber;
    /*
       label on port may not natch the physical number
     */
    ULONG                 PortLabelNumber;

    USHORT                VidOverride;
    USHORT                PidOverride;
    /*
       extended port attributes as defined in usb.h
    */
    ULONG                 PortAttributes;
} USB_EXTPORT_INFORMATION_0, *PUSB_EXTPORT_INFORMATION;


typedef struct _USB_EXTHUB_INFORMATION_0 {

    /* inputs: information level requested */
    ULONG InformationLevel;

    /* begin level_0 information */
    ULONG NumberOfPorts;

    /* hubs don't have > 255 ports */
    USB_EXTPORT_INFORMATION_0 Port[255];

} USB_EXTHUB_INFORMATION_0, *PUSB_EXTHUB_INFORMATION_0;

/*
* Structures used by the GetPerformanceInfo APIs
*/

typedef struct _USB_DEVICE_PERFORMANCE_INFO_0 {

    /* inputs: information level requested */
    ULONG InformationLevel;

    /* outputs: */
    ULONG ActualLength;

    // total BulkBytes transfered for this device
    ULONG BulkBytes;
    ULONG BulkUrbCount;

    // total control bytes transfered for this device
    ULONG ControlDataBytes;
    ULONG ControlUrbCount;

    // total iso bytes transfered for this device
    ULONG IsoBytes;
    ULONG IsoUrbCount;

    // total interrupt bytes transfered for this device
    ULONG InterruptBytes;
    ULONG InterruptUrbCount;

    // BW in bytes alloced in bits/32ms
    ULONG AllocedInterrupt[6];
    ULONG AllocedIso;

    // Total BW available on the bus in bits/32ms
    ULONG Total32secBandwidth;
    
    // Total BW available on the device's TT in bits/32ms
    ULONG TotalTtBandwidth;
    
    // Count of the total time left between scheduling iso transfers and their start frame
    ULONG TotalIsoLatency;
    
    // ISO packet errors, etc, that are NOT late
    ULONG DroppedIsoPackets;
    
    // Number of transfer URBs completing with errors
    ULONG TransferErrors;

} USB_DEVICE_PERFORMANCE_INFO_0, *PUSB_DEVICE_PERFORMANCE_INFO_0;

#include <poppack.h>

#endif

#endif  /* __HUBBUSIF_H */



