/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

_WdfVersionBuild_

Module Name:

    WdfDmaTransaction.h

Abstract:

    WDF DMA Transaction support

Environment:

    Kernel mode only.

Notes:

Revision History:

--*/

#ifndef _WDFDMATRANSACTION_H_
#define _WDFDMATRANSACTION_H_



#if (NTDDI_VERSION >= NTDDI_WIN2K)


typedef
BOOLEAN
(*PFN_WDF_PROGRAM_DMA) (
    IN WDFDMATRANSACTION     Transaction,
    IN WDFDEVICE             Device,
    IN WDFCONTEXT            Context,
    IN WDF_DMA_DIRECTION     Direction,
    IN PSCATTER_GATHER_LIST  SgList
    );

//
// WDF Function: WdfDmaTransactionCreate
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDMATRANSACTIONCREATE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMAENABLER DmaEnabler,
    PWDF_OBJECT_ATTRIBUTES Attributes,
    WDFDMATRANSACTION* DmaTransaction
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDmaTransactionCreate(
    __in
    WDFDMAENABLER DmaEnabler,
    __in_opt
    PWDF_OBJECT_ATTRIBUTES Attributes,
    __out
    WDFDMATRANSACTION* DmaTransaction
    )
{
    return ((PFN_WDFDMATRANSACTIONCREATE) WdfFunctions[WdfDmaTransactionCreateTableIndex])(WdfDriverGlobals, DmaEnabler, Attributes, DmaTransaction);
}

//
// WDF Function: WdfDmaTransactionInitialize
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDMATRANSACTIONINITIALIZE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction,
    PFN_WDF_PROGRAM_DMA EvtProgramDmaFunction,
    IN WDF_DMA_DIRECTION DmaDirection,
    PMDL Mdl,
    PVOID VirtualAddress,
    IN size_t Length
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDmaTransactionInitialize(
    __in
    WDFDMATRANSACTION DmaTransaction,
    __in
    PFN_WDF_PROGRAM_DMA EvtProgramDmaFunction,
    IN WDF_DMA_DIRECTION DmaDirection,
    __in
    PMDL Mdl,
    __in
    PVOID VirtualAddress,
    IN size_t Length
    )
{
    return ((PFN_WDFDMATRANSACTIONINITIALIZE) WdfFunctions[WdfDmaTransactionInitializeTableIndex])(WdfDriverGlobals, DmaTransaction, EvtProgramDmaFunction, DmaDirection, Mdl, VirtualAddress, Length);
}

//
// WDF Function: WdfDmaTransactionInitializeUsingRequest
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDMATRANSACTIONINITIALIZEUSINGREQUEST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction,
    WDFREQUEST Request,
    PFN_WDF_PROGRAM_DMA EvtProgramDmaFunction,
    IN WDF_DMA_DIRECTION DmaDirection
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDmaTransactionInitializeUsingRequest(
    __in
    WDFDMATRANSACTION DmaTransaction,
    __in
    WDFREQUEST Request,
    __in
    PFN_WDF_PROGRAM_DMA EvtProgramDmaFunction,
    IN WDF_DMA_DIRECTION DmaDirection
    )
{
    return ((PFN_WDFDMATRANSACTIONINITIALIZEUSINGREQUEST) WdfFunctions[WdfDmaTransactionInitializeUsingRequestTableIndex])(WdfDriverGlobals, DmaTransaction, Request, EvtProgramDmaFunction, DmaDirection);
}

//
// WDF Function: WdfDmaTransactionExecute
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDMATRANSACTIONEXECUTE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction,
    WDFCONTEXT Context
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDmaTransactionExecute(
    __in
    WDFDMATRANSACTION DmaTransaction,
    __in_opt
    WDFCONTEXT Context
    )
{
    return ((PFN_WDFDMATRANSACTIONEXECUTE) WdfFunctions[WdfDmaTransactionExecuteTableIndex])(WdfDriverGlobals, DmaTransaction, Context);
}

//
// WDF Function: WdfDmaTransactionRelease
//
typedef
WDFAPI
NTSTATUS
(*PFN_WDFDMATRANSACTIONRELEASE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction
    );

__checkReturn
NTSTATUS
FORCEINLINE
WdfDmaTransactionRelease(
    __in
    WDFDMATRANSACTION DmaTransaction
    )
{
    return ((PFN_WDFDMATRANSACTIONRELEASE) WdfFunctions[WdfDmaTransactionReleaseTableIndex])(WdfDriverGlobals, DmaTransaction);
}

//
// WDF Function: WdfDmaTransactionDmaCompleted
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFDMATRANSACTIONDMACOMPLETED)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction,
    NTSTATUS* Status
    );

BOOLEAN
FORCEINLINE
WdfDmaTransactionDmaCompleted(
    __in
    WDFDMATRANSACTION DmaTransaction,
    __out
    NTSTATUS* Status
    )
{
    return ((PFN_WDFDMATRANSACTIONDMACOMPLETED) WdfFunctions[WdfDmaTransactionDmaCompletedTableIndex])(WdfDriverGlobals, DmaTransaction, Status);
}

//
// WDF Function: WdfDmaTransactionDmaCompletedWithLength
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFDMATRANSACTIONDMACOMPLETEDWITHLENGTH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction,
    IN size_t TransferredLength,
    NTSTATUS* Status
    );

BOOLEAN
FORCEINLINE
WdfDmaTransactionDmaCompletedWithLength(
    __in
    WDFDMATRANSACTION DmaTransaction,
    IN size_t TransferredLength,
    __out
    NTSTATUS* Status
    )
{
    return ((PFN_WDFDMATRANSACTIONDMACOMPLETEDWITHLENGTH) WdfFunctions[WdfDmaTransactionDmaCompletedWithLengthTableIndex])(WdfDriverGlobals, DmaTransaction, TransferredLength, Status);
}

//
// WDF Function: WdfDmaTransactionDmaCompletedFinal
//
typedef
WDFAPI
BOOLEAN
(*PFN_WDFDMATRANSACTIONDMACOMPLETEDFINAL)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction,
    IN size_t FinalTransferredLength,
    NTSTATUS* Status
    );

BOOLEAN
FORCEINLINE
WdfDmaTransactionDmaCompletedFinal(
    __in
    WDFDMATRANSACTION DmaTransaction,
    IN size_t FinalTransferredLength,
    __out
    NTSTATUS* Status
    )
{
    return ((PFN_WDFDMATRANSACTIONDMACOMPLETEDFINAL) WdfFunctions[WdfDmaTransactionDmaCompletedFinalTableIndex])(WdfDriverGlobals, DmaTransaction, FinalTransferredLength, Status);
}

//
// WDF Function: WdfDmaTransactionGetBytesTransferred
//
typedef
WDFAPI
size_t
(*PFN_WDFDMATRANSACTIONGETBYTESTRANSFERRED)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction
    );

size_t
FORCEINLINE
WdfDmaTransactionGetBytesTransferred(
    __in
    WDFDMATRANSACTION DmaTransaction
    )
{
    return ((PFN_WDFDMATRANSACTIONGETBYTESTRANSFERRED) WdfFunctions[WdfDmaTransactionGetBytesTransferredTableIndex])(WdfDriverGlobals, DmaTransaction);
}

//
// WDF Function: WdfDmaTransactionSetMaximumLength
//
typedef
WDFAPI
VOID
(*PFN_WDFDMATRANSACTIONSETMAXIMUMLENGTH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction,
    IN size_t MaximumLength
    );

VOID
FORCEINLINE
WdfDmaTransactionSetMaximumLength(
    __in
    WDFDMATRANSACTION DmaTransaction,
    IN size_t MaximumLength
    )
{
    ((PFN_WDFDMATRANSACTIONSETMAXIMUMLENGTH) WdfFunctions[WdfDmaTransactionSetMaximumLengthTableIndex])(WdfDriverGlobals, DmaTransaction, MaximumLength);
}

//
// WDF Function: WdfDmaTransactionGetRequest
//
typedef
WDFAPI
WDFREQUEST
(*PFN_WDFDMATRANSACTIONGETREQUEST)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction
    );

WDFREQUEST
FORCEINLINE
WdfDmaTransactionGetRequest(
    __in
    WDFDMATRANSACTION DmaTransaction
    )
{
    return ((PFN_WDFDMATRANSACTIONGETREQUEST) WdfFunctions[WdfDmaTransactionGetRequestTableIndex])(WdfDriverGlobals, DmaTransaction);
}

//
// WDF Function: WdfDmaTransactionGetCurrentDmaTransferLength
//
typedef
WDFAPI
size_t
(*PFN_WDFDMATRANSACTIONGETCURRENTDMATRANSFERLENGTH)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction
    );

size_t
FORCEINLINE
WdfDmaTransactionGetCurrentDmaTransferLength(
    __in
    WDFDMATRANSACTION DmaTransaction
    )
{
    return ((PFN_WDFDMATRANSACTIONGETCURRENTDMATRANSFERLENGTH) WdfFunctions[WdfDmaTransactionGetCurrentDmaTransferLengthTableIndex])(WdfDriverGlobals, DmaTransaction);
}

//
// WDF Function: WdfDmaTransactionGetDevice
//
typedef
WDFAPI
WDFDEVICE
(*PFN_WDFDMATRANSACTIONGETDEVICE)(
    IN PWDF_DRIVER_GLOBALS DriverGlobals,
    WDFDMATRANSACTION DmaTransaction
    );

WDFDEVICE
FORCEINLINE
WdfDmaTransactionGetDevice(
    __in
    WDFDMATRANSACTION DmaTransaction
    )
{
    return ((PFN_WDFDMATRANSACTIONGETDEVICE) WdfFunctions[WdfDmaTransactionGetDeviceTableIndex])(WdfDriverGlobals, DmaTransaction);
}



#endif // (NTDDI_VERSION >= NTDDI_WIN2K)


#endif // _WDFDMATRANSACTION_H_

