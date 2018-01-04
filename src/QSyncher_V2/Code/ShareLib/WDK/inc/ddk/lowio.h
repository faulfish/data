#if !defined(NTDDI_VERSION)

#error NTDDI_VERSION must be defined to include this file.

#elif (NTDDI_VERSION >= NTDDI_LONGHORN)

/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    lowio.h

Abstract:

    This module defines all of the structures and prototypes for Low IO.

Author:

    Joe Linn (JoeLinn)    8-17-94

Revision History:

--*/

#ifndef _RXLOWIO_
#define _RXLOWIO_

#include "mrx.h" // mini redirector related definitions ....

#ifndef WIN9X
extern FAST_MUTEX RxLowIoPagingIoSyncMutex;
#endif

#define RxLowIoIsMdlLocked(MDL)  ( \
      RxMdlIsLocked((MDL)) || RxMdlSourceIsNonPaged((MDL))   \
      )

#define RxLowIoIsBufferLocked(LOWIOCONTEXT)                          \
   ( ((LOWIOCONTEXT)->Operation > LOWIO_OP_WRITE )  ||               \
     ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer == NULL) ||         \
     (                                                               \
      ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer != NULL) &&        \
      RxLowIoIsMdlLocked(((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer)) \
     )                                                               \
   )

typedef struct _LOWIO_PER_FCB_INFO {
    LIST_ENTRY PagingIoReadsOutstanding;
    LIST_ENTRY PagingIoWritesOutstanding;
} LOWIO_PER_FCB_INFO, *PLOWIO_PER_FCB_INFO;

PVOID
NTAPI
RxLowIoGetBufferAddress (
    IN PRX_CONTEXT RxContext
    );

NTSTATUS
NTAPI
RxLowIoPopulateFsctlInfo (
    IN PRX_CONTEXT RxContext,
    IN PIRP Irp
    );

NTSTATUS
NTAPI
RxLowIoSubmit (
    IN PRX_CONTEXT RxContext,
    IN PIRP Irp,
    IN PFCB Fcb,
    IN PLOWIO_COMPLETION_ROUTINE CompletionRoutine
    );

NTSTATUS
NTAPI
RxLowIoCompletion (
    PRX_CONTEXT RxContext
    );

VOID
NTAPI
RxInitializeLowIoContext (
    IN PRX_CONTEXT RxContext,
    IN ULONG Operation,
    OUT PLOWIO_CONTEXT LowIoContext
    );

VOID
RxInitializeLowIoPerFcbInfo (
    PLOWIO_PER_FCB_INFO LowIoPerFcbInfo
    );


#endif   // _RXLOWIO_

#elif (NTDDI_VERSION >= NTDDI_WS03)

/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    lowio.h

Abstract:

    This module defines all of the structures and prototypes for Low IO.

Author:

    Joe Linn (JoeLinn)    8-17-94

Revision History:

--*/

#ifndef _RXLOWIO_
#define _RXLOWIO_

#include "mrx.h" // mini redirector related definitions ....

#ifndef WIN9X
extern FAST_MUTEX RxLowIoPagingIoSyncMutex;
#endif

#define RxLowIoIsMdlLocked(MDL)  ( \
      RxMdlIsLocked((MDL)) || RxMdlSourceIsNonPaged((MDL))   \
      )

#define RxLowIoIsBufferLocked(LOWIOCONTEXT)                          \
   ( ((LOWIOCONTEXT)->Operation > LOWIO_OP_WRITE )  ||               \
     ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer == NULL) ||         \
     (                                                               \
      ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer != NULL) &&        \
      RxLowIoIsMdlLocked(((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer)) \
     )                                                               \
   )

typedef struct _LOWIO_PER_FCB_INFO {
    LIST_ENTRY PagingIoReadsOutstanding;
    LIST_ENTRY PagingIoWritesOutstanding;
} LOWIO_PER_FCB_INFO, *PLOWIO_PER_FCB_INFO;

PVOID
NTAPI
RxLowIoGetBufferAddress (
    IN PRX_CONTEXT RxContext
    );

NTSTATUS
NTAPI
RxLowIoPopulateFsctlInfo (
    IN PRX_CONTEXT RxContext,
    IN PIRP Irp
    );

NTSTATUS
NTAPI
RxLowIoSubmit (
    IN PRX_CONTEXT RxContext,
    IN PIRP Irp,
    IN PFCB Fcb,
    IN PLOWIO_COMPLETION_ROUTINE CompletionRoutine
    );

NTSTATUS
NTAPI
RxLowIoCompletion (
    PRX_CONTEXT RxContext
    );

VOID
NTAPI
RxInitializeLowIoContext (
    IN PRX_CONTEXT RxContext,
    IN ULONG Operation,
    OUT PLOWIO_CONTEXT LowIoContext
    );

VOID
RxInitializeLowIoPerFcbInfo (
    PLOWIO_PER_FCB_INFO LowIoPerFcbInfo
    );


#endif   // _RXLOWIO_


#elif (NTDDI_VERSION >= NTDDI_WINXP)

/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    lowio.h

Abstract:

    This module defines all of the structures and prototypes for Low IO.

Author:

    Joe Linn (JoeLinn)    8-17-94

Revision History:

--*/

#ifndef _RXLOWIO_
#define _RXLOWIO_

#include "mrx.h" // mini redirector related definitions ....

#ifndef WIN9X
extern FAST_MUTEX RxLowIoPagingIoSyncMutex;
#endif

#define RxLowIoIsMdlLocked(MDL)  ( \
      RxMdlIsLocked((MDL)) || RxMdlSourceIsNonPaged((MDL))   \
      )

#define RxLowIoIsBufferLocked(LOWIOCONTEXT)                          \
   ( ((LOWIOCONTEXT)->Operation > LOWIO_OP_WRITE )  ||               \
     ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer == NULL) ||         \
     (                                                               \
      ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer != NULL) &&        \
      RxLowIoIsMdlLocked(((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer)) \
     )                                                               \
   )

typedef struct _LOWIO_PER_FCB_INFO {
    LIST_ENTRY PagingIoReadsOutstanding;
    LIST_ENTRY PagingIoWritesOutstanding;
} LOWIO_PER_FCB_INFO, *PLOWIO_PER_FCB_INFO;

PVOID
NTAPI
RxLowIoGetBufferAddress (
    IN PRX_CONTEXT RxContext
    );

NTSTATUS
NTAPI
RxLowIoPopulateFsctlInfo (
    IN PRX_CONTEXT RxContext
    );

NTSTATUS
NTAPI
RxLowIoSubmit(
    PRX_CONTEXT RxContext,
    PLOWIO_COMPLETION_ROUTINE CompletionRoutine
    );

NTSTATUS
NTAPI
RxLowIoCompletion(
    PRX_CONTEXT RxContext
    );

VOID
NTAPI
RxInitializeLowIoContext(
    PLOWIO_CONTEXT LowIoContext,
    ULONG Operation
    );

VOID
RxInitializeLowIoPerFcbInfo(
    PLOWIO_PER_FCB_INFO LowIoPerFcbInfo
    );


#endif   // _RXLOWIO_


#elif (NTDDI_VERSION >= NTDDI_WIN2K)

/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    lowio.h

Abstract:

    This module defines all of the structures and prototypes for Low IO.

Author:

    Joe Linn (JoeLinn)    8-17-94

Revision History:

--*/

#ifndef _RXLOWIO_
#define _RXLOWIO_

#include "mrx.h" // mini redirector related definitions ....

#ifndef WIN9X
extern FAST_MUTEX RxLowIoPagingIoSyncMutex;
#endif

#define RxLowIoIsMdlLocked(MDL)  ( \
      RxMdlIsLocked((MDL)) || RxMdlSourceIsNonPaged((MDL))   \
      )

#define RxLowIoIsBufferLocked(LOWIOCONTEXT)                          \
   ( ((LOWIOCONTEXT)->Operation > LOWIO_OP_WRITE )  ||               \
     ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer == NULL) ||         \
     (                                                               \
      ((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer != NULL) &&        \
      RxLowIoIsMdlLocked(((LOWIOCONTEXT)->ParamsFor.ReadWrite.Buffer)) \
     )                                                               \
   )

typedef struct _LOWIO_PER_FCB_INFO {
    LIST_ENTRY PagingIoReadsOutstanding;
    LIST_ENTRY PagingIoWritesOutstanding;
} LOWIO_PER_FCB_INFO, *PLOWIO_PER_FCB_INFO;

PVOID
NTAPI
RxLowIoGetBufferAddress (
    IN PRX_CONTEXT RxContext
    );

NTSTATUS
NTAPI
RxLowIoPopulateFsctlInfo (
    IN PRX_CONTEXT RxContext
    );

NTSTATUS
NTAPI
RxLowIoSubmit(
    PRX_CONTEXT RxContext,
    PLOWIO_COMPLETION_ROUTINE CompletionRoutine
    );

NTSTATUS
NTAPI
RxLowIoCompletion(
    PRX_CONTEXT RxContext
    );

VOID
NTAPI
RxInitializeLowIoContext(
    PLOWIO_CONTEXT LowIoContext,
    ULONG Operation
    );

VOID
RxInitializeLowIoPerFcbInfo(
    PLOWIO_PER_FCB_INFO LowIoPerFcbInfo
    );


#endif   // _RXLOWIO_


#else /* NTDDI_VERSION */

#error This file cannot be included for this NTDDI_VERSION.

#endif /* NTDDI_VERSION */


