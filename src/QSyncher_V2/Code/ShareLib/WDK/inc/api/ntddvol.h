/*++

Copyright (c) Microsoft Corporation. All rights reserved.

Module Name:

    ntddvol.h

Abstract:

    This header file defines the IOCTLs supported by Windows NT volumes.

--*/


#ifndef _NTDDVOL_
#define _NTDDVOL_

#if _MSC_VER > 1000
#pragma once
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IOCTL_VOLUME_BASE
// begin_winioctl

#define IOCTL_VOLUME_BASE   ((ULONG) 'V')

#if (NTDDI_VERSION >= NTDDI_WIN2K)

//
// These IOCTLs are handled by hard disk volumes.
//

#define IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS    CTL_CODE(IOCTL_VOLUME_BASE, 0, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// Disk extent definition.
//

typedef struct _DISK_EXTENT {
    ULONG           DiskNumber;
    LARGE_INTEGER   StartingOffset;
    LARGE_INTEGER   ExtentLength;
} DISK_EXTENT, *PDISK_EXTENT;

//
// Output structure for IOCTL_VOLUME_GET_VOLUME_DISK_EXTENTS.
//

typedef struct _VOLUME_DISK_EXTENTS {
    ULONG       NumberOfDiskExtents;
    DISK_EXTENT Extents[1];
} VOLUME_DISK_EXTENTS, *PVOLUME_DISK_EXTENTS;

#endif  // NTDDI_VERSION >= NTDDI_WIN2K

#if (NTDDI_VERSION >= NTDDI_WINXP)

//
// These IOCTLs are handled by hard disk volumes.
//

#define IOCTL_VOLUME_IS_CLUSTERED               CTL_CODE(IOCTL_VOLUME_BASE, 12, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif  // NTDDI_VERSION >= NTDDI_WINXP

// end_winioctl
#endif  // IOCTL_VOLUME_BASE

#if (NTDDI_VERSION >= NTDDI_WIN2K)

//
// These IOCTLs are to support cluster services.
//

#define IOCTL_VOLUME_SUPPORTS_ONLINE_OFFLINE    CTL_CODE(IOCTL_VOLUME_BASE, 1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VOLUME_ONLINE                     CTL_CODE(IOCTL_VOLUME_BASE, 2, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_VOLUME_OFFLINE                    CTL_CODE(IOCTL_VOLUME_BASE, 3, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_VOLUME_IS_OFFLINE                 CTL_CODE(IOCTL_VOLUME_BASE, 4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VOLUME_IS_IO_CAPABLE              CTL_CODE(IOCTL_VOLUME_BASE, 5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VOLUME_QUERY_FAILOVER_SET         CTL_CODE(IOCTL_VOLUME_BASE, 6, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// Output structure for IOCTL_VOLUME_QUERY_FAILOVER_SET.
//

typedef struct _VOLUME_FAILOVER_SET {
    ULONG   NumberOfDisks;
    ULONG   DiskNumbers[1];
} VOLUME_FAILOVER_SET, *PVOLUME_FAILOVER_SET;

//
// More IOCTLs for hard disk volumes.
//

#define IOCTL_VOLUME_QUERY_VOLUME_NUMBER        CTL_CODE(IOCTL_VOLUME_BASE, 7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VOLUME_LOGICAL_TO_PHYSICAL        CTL_CODE(IOCTL_VOLUME_BASE, 8, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VOLUME_PHYSICAL_TO_LOGICAL        CTL_CODE(IOCTL_VOLUME_BASE, 9, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// Output structure for IOCTL_VOLUME_QUERY_VOLUME_NUMBER.  If the volume
// manager name is less than 8 characters then it should be padded with
// blanks (0x20).
//

typedef struct _VOLUME_NUMBER {
    ULONG   VolumeNumber;
    WCHAR   VolumeManagerName[8];
} VOLUME_NUMBER, *PVOLUME_NUMBER;

//
// Input structure for IOCTL_VOLUME_LOGICAL_TO_PHYSICAL.
// Output structure for IOCTL_VOLUME_PHYSICAL_TO_LOGICAL.
//

typedef struct _VOLUME_LOGICAL_OFFSET {
    LONGLONG    LogicalOffset;
} VOLUME_LOGICAL_OFFSET, *PVOLUME_LOGICAL_OFFSET;

//
// Input structure for IOCTL_VOLUME_PHYSICAL_TO_LOGICAL.
//

typedef struct _VOLUME_PHYSICAL_OFFSET {
    ULONG       DiskNumber;
    LONGLONG    Offset;
} VOLUME_PHYSICAL_OFFSET, *PVOLUME_PHYSICAL_OFFSET;

//
// Output structure for IOCTL_VOLUME_LOGICAL_TO_PHYSICAL.
//

typedef struct _VOLUME_PHYSICAL_OFFSETS {
    ULONG                   NumberOfPhysicalOffsets;
    VOLUME_PHYSICAL_OFFSET  PhysicalOffset[1];
} VOLUME_PHYSICAL_OFFSETS, *PVOLUME_PHYSICAL_OFFSETS;

#endif  // NTDDI_VERSION >= NTDDI_WIN2K


#if (NTDDI_VERSION >= NTDDI_WINXP)

//
// IOCTL to determine whether  the volume
// is suitable for OS installation or not
//

#define IOCTL_VOLUME_IS_PARTITION               CTL_CODE(IOCTL_VOLUME_BASE, 10, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// IOCTL_VOLUME_IS_PARTITION
//
// Input Buffer:
//     None
//
// Output Buffer:
//     None
//


//
// IOCTL to perform a  read operation from
// the specified plex of a mirrored volume
//

#define IOCTL_VOLUME_READ_PLEX                  CTL_CODE(IOCTL_VOLUME_BASE, 11, METHOD_OUT_DIRECT, FILE_READ_ACCESS)

//
// IOCTL_VOLUME_READ_PLEX
//
// Input Buffer:
//     Structure of type VOLUME_READ_PLEX_INPUT
//
// Output Buffer:
//     None
//

typedef struct _VOLUME_READ_PLEX_INPUT {

    //
    // Specifies the offset within the
    // volume from where  to read data
    //
    LARGE_INTEGER ByteOffset;

    //
    // Specifies the  length in
    // bytes that is to be read
    //    
    ULONG Length;

    //
    // Specifies  the corresponding plex
    // from which the data is to be read
    //    
    ULONG PlexNumber;

} VOLUME_READ_PLEX_INPUT, *PVOLUME_READ_PLEX_INPUT;


//
// IOCTLs to query and modify
// the  attributes on volumes
//

#define IOCTL_VOLUME_SET_GPT_ATTRIBUTES         CTL_CODE(IOCTL_VOLUME_BASE, 13, METHOD_BUFFERED, FILE_ANY_ACCESS)
#define IOCTL_VOLUME_GET_GPT_ATTRIBUTES         CTL_CODE(IOCTL_VOLUME_BASE, 14, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// IOCTL_VOLUME_SET_GPT_ATTRIBUTES
//
// Input Buffer:
//     Structure of type VOLUME_SET_GPT_ATTRIBUTES_INFORMATION
//
// Output Buffer:
//     None
//

typedef struct _VOLUME_SET_GPT_ATTRIBUTES_INFORMATION {

    //
    // Specifies  the  attributes that
    // are to be applied to the volume
    //
    ULONGLONG GptAttributes;

    //
    // Indicates whether  this is to be
    // undone when the handle is closed
    //
    BOOLEAN RevertOnClose;

    //
    // Indicates  whether the  attributes
    // apply  to all the  volumes  on the
    // disk that  this  volume resides on
    // Required if the disk layout is MBR
    //
    BOOLEAN ApplyToAllConnectedVolumes;

    //
    // For alignment purposes
    //
    USHORT Reserved1;
    ULONG  Reserved2;

} VOLUME_SET_GPT_ATTRIBUTES_INFORMATION, *PVOLUME_SET_GPT_ATTRIBUTES_INFORMATION;

//
// IOCTL_VOLUME_GET_GPT_ATTRIBUTES
//
// Input Buffer:
//     None
//
// Output Buffer:
//     Structure of type VOLUME_GET_GPT_ATTRIBUTES_INFORMATION
//

typedef struct _VOLUME_GET_GPT_ATTRIBUTES_INFORMATION {

    //
    // Specifies  the  attributes
    // associated with the volume
    //
    ULONGLONG GptAttributes;
    
} VOLUME_GET_GPT_ATTRIBUTES_INFORMATION, *PVOLUME_GET_GPT_ATTRIBUTES_INFORMATION;

#endif  // NTDDI_VERSION >= NTDDI_WINXP


#if (NTDDI_VERSION >= NTDDI_LONGHORN)

//
// IOCTLs for bandwidth contracts on volumes
//

#define IOCTL_VOLUME_GET_BC_PROPERTIES          CTL_CODE(IOCTL_VOLUME_BASE, 15, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_VOLUME_ALLOCATE_BC_STREAM         CTL_CODE(IOCTL_VOLUME_BASE, 16, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)
#define IOCTL_VOLUME_FREE_BC_STREAM             CTL_CODE(IOCTL_VOLUME_BASE, 17, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

#define IOCTL_VOLUME_BC_VERSION                 1

//
// IOCTL_VOLUME_GET_BC_PROPERTIES
//
// Input Buffer:
//     Structure of type VOLUME_GET_BC_PROPERTIES_INPUT
//
// Output Buffer:
//     Structure of type VOLUME_GET_BC_PROPERTIES_OUTPUT
//

typedef struct _VOLUME_GET_BC_PROPERTIES_INPUT {

    //
    // Specifies the corresponding structure version
    //
    ULONG Version;

    //
    // For alignment purposes
    //
    ULONG Reserved1;

    //
    // Specifies  the  lowest byte offset
    // of the file that is to be streamed
    //
    ULONGLONG LowestByteOffset;
    
    //
    // Specifies  the highest byte offset
    // of the file that is to be streamed
    //
    ULONGLONG HighestByteOffset;

    //
    // Indicates whether the  Io  will be
    // comprised of reads, writes or both
    //
    ULONG AccessType;

    //
    // Indicates whether the  Io  to the
    // file will be sequential or random
    //
    ULONG AccessMode;
    
} VOLUME_GET_BC_PROPERTIES_INPUT, *PVOLUME_GET_BC_PROPERTIES_INPUT;

typedef struct _VOLUME_GET_BC_PROPERTIES_OUTPUT {

    //
    // Specifies the maximum number of requests
    // that can be scheduled per period of time
    //
    ULONG MaximumRequestsPerPeriod;
    
    //
    // Specifies the minimum period that the
    // device uses  when scheduling requests
    //
    ULONG MinimumPeriod;

    //
    // Specifies the maximum transfer size supported
    // for  bandwidth contracts  on this  device. To
    // achieve the highest level of performance, all
    // requests should be of this size
    //
    ULONGLONG MaximumRequestSize;

    //
    // Specifies the estimated time taken to
    // perform an  Io operation. This  field
    // is  for  informational purposes  only
    //
    ULONG EstimatedTimePerRequest;

    //
    // Specifies the number of requests that should be
    // kept outstanding.  This helps  keep the  device
    // device busy  and thus obtain maximum throughput
    // This will only be filled in if the target  file
    // has an outstanding contract
    //
    ULONG NumOutStandingRequests;

    //
    // Specifies the required size of requests in this 
    // stream.  This  will  only  be filled in  if the
    // target file has an outstanding contract
    //
    ULONGLONG RequestSize;

} VOLUME_GET_BC_PROPERTIES_OUTPUT, *PVOLUME_GET_BC_PROPERTIES_OUTPUT;

//
// IOCTL_VOLUME_ALLOCATE_BC_STREAM
//
// Input Buffer:
//     Structure of type VOLUME_ALLOCATE_BC_STREAM_INPUT
//
// Output Buffer:
//     Structure of type VOLUME_ALLOCATE_BC_STREAM_OUTPUT
//

typedef struct _VOLUME_ALLOCATE_BC_STREAM_INPUT {

    //
    // Specifies the corresponding structure version
    //
    ULONG Version;

    //
    // Specifies the number of requests that
    // need to  complete  per period of time
    //
    ULONG RequestsPerPeriod;
    
    //
    // Specifies the period of time wherein the
    // above  number of requests  must complete 
    //
    ULONG Period;

    //
    // Indicates whether failures
    // should  be retried  or not
    //
    BOOLEAN RetryFailures;

    //
    // Indicates whether reqests that  will miss
    // their deadline should be discarded or not
    //
    BOOLEAN Discardable;

    //
    // For alignment purposes
    //
    BOOLEAN Reserved1[2];

    //
    // Specifies  the  lowest byte offset
    // of the file that is to be streamed
    //
    ULONGLONG LowestByteOffset;
    
    //
    // Specifies  the highest byte offset
    // of the file that is to be streamed
    //
    ULONGLONG HighestByteOffset;

    //
    // Indicates whether the  Io  will be
    // comprised of reads, writes or both
    //
    ULONG AccessType;

    //
    // Indicates whether the  Io  to the
    // file will be sequential or random
    //
    ULONG AccessMode;
    
} VOLUME_ALLOCATE_BC_STREAM_INPUT, *PVOLUME_ALLOCATE_BC_STREAM_INPUT;

typedef struct _VOLUME_ALLOCATE_BC_STREAM_OUTPUT {

    //
    // Specifies the required size
    // of  requests in this stream
    //
    ULONGLONG RequestSize;
    
    //
    // Specifies the number of requests that should be
    // kept outstanding.  This helps  keep the  device
    // device busy and thus obtain maximum  throughput
    //
    ULONG NumOutStandingRequests;
    
} VOLUME_ALLOCATE_BC_STREAM_OUTPUT, *PVOLUME_ALLOCATE_BC_STREAM_OUTPUT;

//
// IOCTL_VOLUME_FREE_BC_STREAM
//
// Input Buffer:
//     None
//
// Output Buffer:
//     None
//


//
// IOCTL to  help in differentiating
// between basic and dynamic volumes
//

#define IOCTL_VOLUME_IS_DYNAMIC                 CTL_CODE(IOCTL_VOLUME_BASE, 18, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// IOCTL_VOLUME_IS_DYNAMIC
//
// Input Buffer:
//     None
//
// Output Buffer:
//     Structure of type BOOLEAN
//


//
// IOCTL to  inform  the storage stack
// of runs that contain a special file
//

#define IOCTL_VOLUME_PREPARE_FOR_CRITICAL_IO    CTL_CODE(IOCTL_VOLUME_BASE, 19, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

//
// IOCTL_VOLUME_PREPARE_FOR_CRITICAL_IO
//
// Input Buffer:
//     Structure of type VOLUME_CRITICAL_IO
//
// Output Buffer:
//     None
//

typedef struct _FILE_EXTENT {

    ULONGLONG VolumeOffset;
    ULONGLONG ExtentLength;

} FILE_EXTENT, *PFILE_EXTENT;

typedef struct _VOLUME_CRITICAL_IO {

    //
    // Indicates whether the  Io  will be
    // comprised of reads, writes or both
    //
    ULONG AccessType;

    //
    // Count of the number of extents that
    // are contained within this structure
    //
    ULONG ExtentsCount;

    //
    // Specifies the extents along which
    // the critical Io will be performed
    //
    FILE_EXTENT Extents[ANYSIZE_ARRAY];

} VOLUME_CRITICAL_IO, *PVOLUME_CRITICAL_IO;


//
// IOCTL to provide hints to the file system
// as to where to place new file allocations
//

#define IOCTL_VOLUME_QUERY_ALLOCATION_HINT      CTL_CODE(IOCTL_VOLUME_BASE, 20, METHOD_OUT_DIRECT, FILE_READ_ACCESS)

//
// IOCTL_VOLUME_QUERY_ALLOCATION_HINT
//
// Input Buffer:
//     Structure of type VOLUME_ALLOCATION_HINT_INPUT
//
// Output Buffer:
//     Structure of type VOLUME_ALLOCATION_HINT_OUTPUT
//

typedef struct _VOLUME_ALLOCATION_HINT_INPUT {

    //
    // Specifies the granularity of
    // the  bitmap  being requested
    //
    ULONG ClusterSize;

    //
    // Specifies the count of bits
    // that  are  being  requested
    //
    ULONG NumberOfClusters;

    //
    // Specifies the first cluster
    // that  is  to  be  evaluated
    //
    LONGLONG StartingClusterNumber;

} VOLUME_ALLOCATION_HINT_INPUT, *PVOLUME_ALLOCATION_HINT_INPUT;

typedef struct _VOLUME_ALLOCATION_HINT_OUTPUT {

    //
    // Specifies the bits, 1 per cluster
    // indicating all of the good places
    // where file data  may be allocated
    //
    ULONG Bitmap[ANYSIZE_ARRAY];

} VOLUME_ALLOCATION_HINT_OUTPUT, *PVOLUME_ALLOCATION_HINT_OUTPUT;


//
// IOCTL to inform  the storage stack to
// discard and  requery  any information
// about the volume that might be cached
//

#define IOCTL_VOLUME_UPDATE_PROPERTIES          CTL_CODE(IOCTL_VOLUME_BASE, 21, METHOD_BUFFERED, FILE_ANY_ACCESS)

//
// IOCTL_VOLUME_UPDATE_PROPERTIES
//
// Input Buffer:
//     None
//
// Output Buffer:
//     None
//


//
// IOCTLs to coordinate a shrink operation
// with the drivers  in  the storage stack
//

#define IOCTL_VOLUME_QUERY_MINIMUM_SHRINK_SIZE  CTL_CODE(IOCTL_VOLUME_BASE, 22, METHOD_BUFFERED, FILE_READ_ACCESS)
#define IOCTL_VOLUME_PREPARE_FOR_SHRINK         CTL_CODE(IOCTL_VOLUME_BASE, 23, METHOD_BUFFERED, FILE_READ_ACCESS | FILE_WRITE_ACCESS)

//
// IOCTL_VOLUME_QUERY_MINIMUM_SHRINK_SIZE
//
// Input Buffer:
//     None
//
// Output Buffer:
//     Structure of type VOLUME_SHRINK_INFO
//

//
// IOCTL_VOLUME_PREPARE_FOR_SHRINK
//
// Input Buffer:
//     Structure of type VOLUME_SHRINK_INFO
//
// Output Buffer:
//     None
//

typedef struct _VOLUME_SHRINK_INFO {

    //
    // Specifies the size in bytes to
    // which the volume  can be or is
    // going  to be  reduced  down to
    //
    ULONGLONG VolumeSize;

} VOLUME_SHRINK_INFO, *PVOLUME_SHRINK_INFO;


#endif  // NTDDI_VERSION >= NTDDI_LONGHORN

#ifdef __cplusplus
}
#endif

#endif  // _NTDDVOL_

