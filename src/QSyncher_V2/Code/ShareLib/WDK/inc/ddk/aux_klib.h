/*++

Copyright (c) 2004  Microsoft Corporation

Module Name:

    aux_klib.h

Abstract:

    Kernel mode shim to access system functionality that is not properly exposed
    to applications in currently shipping operating systems.

--*/

#ifndef _AUX_KLIB_H
#define _AUX_KLIB_H

#ifndef PIMAGE_EXPORT_DIRECTORY
#include <ntimage.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*++

Routine Description:

    This routine must be successfully called by a kernel mode component before
    any other routine in the library may be called. It serves to initialize any global
    state that may be required by other routines in the file.

    It is safe to call this routine in a multi-threaded environment.

Arguments:

    None.

Return Value:

    NTSTATUS

--*/

NTSTATUS
__stdcall
AuxKlibInitialize (
    VOID
    );

#define AUX_KLIB_MODULE_PATH_LEN 256

typedef struct _AUX_MODULE_BASIC_INFO {
    PVOID ImageBase;
} AUX_MODULE_BASIC_INFO, *PAUX_MODULE_BASIC_INFO;

typedef struct _AUX_MODULE_EXTENDED_INFO {
    AUX_MODULE_BASIC_INFO BasicInfo;
    ULONG ImageSize;
    USHORT FileNameOffset;
    UCHAR FullPathName [AUX_KLIB_MODULE_PATH_LEN];
} AUX_MODULE_EXTENDED_INFO, *PAUX_MODULE_EXTENDED_INFO;

/*++

Routine Description:

    This queries the system for information concerning the currently loaded set of
    modules. The format of the output is determined by the passed element size.

    If extended information is requested, note that the full path name is guaranteed
    to be NULL terminated.

Arguments:

    BufferSize - On input, the size of the passed QueryInfo buffer. On failure or
        success, this is the actual required size. Note that on failure, the value
        may change between subsequent calls to this routine as the system's
        module list may expand in the interim.

    ElementSize - Size of an individual element in the passed array. This determines
        the format of the output.

    QueryInfo - Pointer to an output buffer to receive the module information. If not
        specified, then the routine simply returns the required buffer size via the
        relevant output parameter. Note that kernel callers must ensure a valid
        kernel buffer is passed.

Return Value:

    NTSTATUS as follows:

        STATUS_UNSUCCESSFUL - The library has not been successfully initialized.

        STATUS_BUFFER_TOO_SMALL - The passed buffer is too small, but the
            appropriate size has been written to the BufferSize output parameter.

        Other NTSTATUS codes as appropriate.

--*/

NTSTATUS
__stdcall
AuxKlibQueryModuleInformation (
    IN OUT PULONG BufferSize,
    IN ULONG ElementSize,
    OUT PVOID QueryInfo OPTIONAL
    );

typedef struct _KBUGCHECK_DATA {
    ULONG     BugCheckDataSize;
    ULONG     BugCheckCode;
    ULONG_PTR Parameter1;
    ULONG_PTR Parameter2;
    ULONG_PTR Parameter3;
    ULONG_PTR Parameter4;
} KBUGCHECK_DATA, *PKBUGCHECK_DATA;


/*++

Routine Description:

    This routine provide access to the BugCheck parameters.
    It should only be called from registerd BugCheck callback routines
    during BugCheck processing.

Arguments:

    BugCheckData  - Pointer to  the buffer which will receive the BugCheck data.

Return Value:

    VOID

--*/

NTSTATUS
AuxKlibGetBugCheckData(
    OUT PKBUGCHECK_DATA BugCheckData
    );


/*++

Routine Description:

   This routine provides a function to retrieve an image's export table address.
   This function should only called at PASSIVE_LEVEL.

Arguments:

    ImageBase    - A pointer to the image's base address.

Return Value:

    On success a pointer to the import table (IMAGE_EXPORT_DIRECTORY) is
    returned. On failure a NULL pointer is returned.

--*/

PIMAGE_EXPORT_DIRECTORY
AuxKlibGetImageExportDirectory(
    IN PVOID ImageBase
    );


#ifdef __cplusplus
}
#endif

#endif // _AUX_KLIB_H

