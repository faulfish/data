//
// Copyright (c) Microsoft Corporation. All rights reserved.
// Copyright 2005 OSR, Open Systems Resources, Inc. All rights Reserved.
// 
// Module Name:
// 
//     irclass_ioctl.h
// 
// Abstract:
// 
//    This module contains the IOCTL definitions for the 
//    WDF IRCLASS class driver
//
//  
// Author:
//
// Revision History:
//      
//
#ifndef __IRCLASS_IOCTL_H__
#define __IRCLASS_IOCTL_H__

#define FILE_DEVICE_IRCLASS     0x0F60


#define IOCTL_IR_GET_DEV_CAPS           CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 1,                    \
                                                 METHOD_BUFFERED,      \
                                                 FILE_READ_ACCESS)
/*++
    
    IOCTL_IR_GET_DEVCAPS

    Returns device capabilities.  For legacy devices, the Capabilities 
    registry entry can be used to populate this structure.  For new devices, 
    the implementation is left as an exercise for the reader.

    Parameters:

    lpInBuffer      - pointer to caller-allocated IR_DEV_CAPS structure

    nInBufferSize   - sizeof (IR_DEV_CAPS)

--*/

typedef struct _IR_DEV_CAPS {

    //
    // Protocol version.  Currently must be 100 (1.0)
    //
	OUT ULONG_PTR ProtocolVersion;	
    //
    // Number of transmit ports - 0-32
    //
	OUT ULONG_PTR NumTransmitPorts;	

    //
    // Number of receive ports - 0-32 (for snowflake, 
    // this would be one.  For beanbag, this would be 
    // two (one for learning, one for normal)
    //
    OUT ULONG_PTR NumReceivePorts;    

    //
    // Bitmask identifying which receivers are 
    // learning receivers - low bit is the first 
    // receiver, second-low bit is the second receiver, 
    // etc	
    //
    OUT ULONG_PTR LearningReceiverMask; 

    //
    // Flags
    //
	OUT ULONG_PTR DevCapsFlags; 

}IR_DEV_CAPS, *PIR_DEV_CAPS;

#define DEV_CAPS_PROTOCOL_VERSION        0x100

#define DEV_CAPS_SUPPORTS_LEGACY_SIGNING 0x1
#define DEV_CAPS_HAS_UNIQUE_SERIAL       0x2
#define DEV_CAPS_CAN_FLASH_RECEIVER_LED  0x4
#define DEV_CAPS_IS_LEGACY               0x8


#define IOCTL_IR_GET_EMITTERS           CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 2,                    \
                                                 METHOD_BUFFERED,      \
                                                 FILE_READ_ACCESS)
/*++

    IOCTL_IR_GET_EMITTERS

    Gets attached emitters and returns the information in a bitmask.  
    Information returned in lpOutBuffer.

    Parameters:

    lpOutBuffer      - pointer to caller-allocated buffer sizeof(ULONG)

    nOutBufferSize   - sizeof(ULONG)

--*/


#define IOCTL_IR_FLASH_RECEIVER           CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 3,                      \
                                                 METHOD_BUFFERED,        \
                                                 FILE_WRITE_ACCESS)
/*++

    IOCTL_IR_FLASH_RECEIVER

    Flash an LED on the given receiver.  Used to tell the user where to point 
    their remote, so a given "receiver box" with multiple receiver parts only 
    needs one LED to flash.

    Parameters:

    lpInBuffer      - pointer to caller-allocated buffer sizeof(ULONG) with 
                      bitmask of receivers to flash

    nInBufferSize   - sizeof(ULONG)

--*/


#define IOCTL_IR_RESET_DEVICE           CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 4,                    \
                                                 METHOD_BUFFERED,      \
                                                 FILE_WRITE_ACCESS)

/*++

    IOCTL_IR_RESET_DEVICE
    
    Resets the given device.  When a device is reset, all pending transmit and 
    receive IOCTLs are cancelled by the class driver

    Parameters:

--*/



#define IOCTL_IR_TRANSMIT           CTL_CODE(FILE_DEVICE_IRCLASS,    \
                                                 5,                  \
                                                 METHOD_IN_DIRECT,  \
                                                 FILE_WRITE_ACCESS)

/*++

    IOCTL_IR_TRANSMIT


    Transmits the given IR stream on the given port(s) at the given carrier 
    frequency. On legacy devices, this maintains the pre-existing carrier 
    frequency, port masks, and sample period values.  (ie. it gets the old 
    values, changes them, transmits, and then changes them back.)

    This IOCTL is synchronous.  It does not return until the IR has actually 
    been transmitted.

    Parameters:

    lpInBuffer      - pointer to caller-allocated IR_TRANSMIT_PARAMS structure

    nInBufferSize   - sizeof(IR_TRANSMIT_PARAMS)

    lpOutBuffer     - pointer to caller-allocated IR_TRANSMIT_CHUNCK that contains
                      the data to be transmitted

    nOutBufferSize  - size of caller-allocated buffer. 

--*/


typedef struct _IR_TRANSMIT_PARAMS {

    //
    // Bitmask containing ports to transmit on.
    //
	IN ULONG_PTR TransmitPortMask;

    //
    // Carrier period to use.  If zero, Flags 
    // needs to define DC mode or pulse mode.
    //
	IN ULONG_PTR CarrierPeriod;

    //
    // Flags 
    //
	IN ULONG_PTR Flags;			

    //
    // If pulse mode is set, this contains the length of pulse
    // to use.
    //
	IN ULONG_PTR PulseSize;			
						
} IR_TRANSMIT_PARAMS, *PIR_TRANSMIT_PARAMS;

#define TRANSMIT_FLAGS_PULSE_MODE 0x0001
#define TRANSMIT_FLAGS_DC_MODE    0x0002

typedef struct _IR_TRANSMIT_CHUNK {

    //
    // offset, in bytes, from Data member of this buffer to next 
    //   IR_TRANSMIT_CHUNK (or zero if no more chunks in buffer)
    //
    ULONG_PTR    OffsetToNextChunk;

    //
    // number of times to serially repeat "ByteCount" bytes of data
    //
    ULONG_PTR    RepeatCount;   

    //
    // count of data bytes to be sent
    //
    ULONG_PTR    ByteCount;         

    //
    // First byte of "ByteCount" bytes of data. 
    //   Note: Each chunk is filled to integral ULONG_PTR boundary
    //
    LONG        Data[1];

} IR_TRANSMIT_CHUNK, *PIR_TRANSMIT_CHUNK;


#define IOCTL_IR_RECEIVE           CTL_CODE(FILE_DEVICE_IRCLASS,    \
                                                 6,                 \
                                                 METHOD_OUT_DIRECT,  \
                                                 FILE_READ_ACCESS)


/*++

    IOCTL_IR_RECEIVE

    Receives IR.  Does not return until IR is available.  If there is more IR 
    data available than space in the buffer, IrReceiveParms->fMore is set to 
    TRUE.  The provided timeout is used to define the end of a keypress.  So, 
    once the driver starts receiving IR from the hardware, it will continue to 
    add it to the buffer until the specified time passes with no IR.

    Parameters:

    lpOutBuffer      - pointer to caller-allocated IR_RECEIVE_PARAMS structure

    nOutBufferSize   - sizeof(IR_RECEIVE_PARAMS)

--*/
typedef struct _IR_RECEIVE_PARAMS {

    //
    // Does this receive represent a data end event?
    //
    OUT ULONG_PTR DataEnd;

    //
    // Size of the data buffer
    //
    IN  ULONG_PTR ByteCount;

    //
    // The data buffer itself.
    //
    OUT LONG     Data[1];

}IR_RECEIVE_PARAMS, *PIR_RECEIVE_PARAMS;


#define IOCTL_IR_PRIORITY_RECEIVE           CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 8,                   \
                                                 METHOD_OUT_DIRECT,    \
                                                 FILE_READ_ACCESS)

typedef struct _IR_PRIORITY_RECEIVE_PARAMS {

    //
    // Does this receive represent a data end event?
    //
    OUT ULONG_PTR DataEnd;

    //
    // Size of the data buffer
    //
    IN  ULONG_PTR ByteCount;

    //
    // Carrier frequency (only valid if DataEnd != 0)
    //
    OUT ULONG_PTR CarrierFrequency;

    //
    // The data buffer itself.
    //
    IN  LONG     Data[1];

}IR_PRIORITY_RECEIVE_PARAMS, *PIR_PRIORITY_RECEIVE_PARAMS;



#define IOCTL_IR_HANDSHAKE             CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 9,                   \
                                                 METHOD_BUFFERED,     \
                                                 FILE_ANY_ACCESS)

#define IOCTL_IR_ENTER_PRIORITY_RECEIVE        CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 10,                  \
                                                 METHOD_BUFFERED,     \
                                                 FILE_WRITE_ACCESS)

typedef struct _IOCTL_IR_ENTER_PRIORITY_RECEIVE_PARAMS {

    //
    // Index of the receiver to use
    //
    IN ULONG_PTR  Receiver;	
    
    //
    // Timeout value, in micsec.  Used to define 
    // the end of a given sample.
    //
    IN ULONG_PTR  TimeOut;
    
}IOCTL_IR_ENTER_PRIORITY_RECEIVE_PARAMS, *PIOCTL_IR_ENTER_PRIORITY_RECEIVE_PARAMS;


#define IOCTL_IR_EXIT_PRIORITY_RECEIVE         CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 11,                  \
                                                 METHOD_BUFFERED,     \
                                                 FILE_WRITE_ACCESS)

#define IOCTL_IR_USER_OPEN             CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 12,                  \
                                                 METHOD_BUFFERED,     \
                                                 FILE_WRITE_ACCESS)


#define IOCTL_IR_USER_CLOSE            CTL_CODE(FILE_DEVICE_IRCLASS,  \
                                                 13,                  \
                                                 METHOD_BUFFERED,     \
                                                 FILE_WRITE_ACCESS)

#endif

