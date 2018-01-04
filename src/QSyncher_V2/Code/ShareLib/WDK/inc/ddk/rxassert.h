#if !defined(NTDDI_VERSION)

#error NTDDI_VERSION must be defined to include this file.

#elif (NTDDI_VERSION >= NTDDI_LONGHORN)

#error This file cannot be included for this NTDDI_VERSION.

#elif (NTDDI_VERSION >= NTDDI_WS03)

/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    RxAssert.h

Abstract:

    This module declares the prototypes and global data used by the special RDBSS assert facilties.

Author:

    Joe Linn     [JoeLinn]    21-oct-1996

Revision History:


Notes:


--*/

#ifndef _RDBSSASSERT_INCLUDED_
#define _RDBSSASSERT_INCLUDED_

VOID RxDbgBreakPoint(ULONG LineNumber);

//only do this is my routine is the one of interest.......

#ifdef RDBSS_ASSERTS

#if 0
#if !DBG

//here, ntifs will have already defined the asserts away..........
//   so, we just put them back.....this code is duplicated from ntifs.h


#undef ASSERT
#define ASSERT( exp ) \
    if (!(exp)) \
        RxDbgBreakPoint(__LINE__)

#undef ASSERTMSG
#define ASSERTMSG( msg, exp ) \
    if (!(exp)) \
        RxDbgBreakPoint(__LINE__)

#endif //!DBG
#endif

//this will make asserts go to our routine

#define RtlAssert RxAssert
VOID
RxAssert(
    IN PVOID FailedAssertion,
    IN PVOID FileName,
    IN ULONG LineNumber,
    IN PCHAR Message OPTIONAL
    );

#endif //ifdef RDBSS_ASSERTS


#endif // _RDBSSASSERT_INCLUDED_



#elif (NTDDI_VERSION >= NTDDI_WINXP)

/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    RxAssert.h

Abstract:

    This module declares the prototypes and global data used by the special RDBSS assert facilties.

Author:

    Joe Linn     [JoeLinn]    21-oct-1996

Revision History:


Notes:


--*/

#ifndef _RDBSSASSERT_INCLUDED_
#define _RDBSSASSERT_INCLUDED_

VOID RxDbgBreakPoint(ULONG LineNumber);

//only do this is my routine is the one of interest.......

#ifdef RDBSS_ASSERTS

#if 0
#if !DBG

//here, ntifs will have already defined the asserts away..........
//   so, we just put them back.....this code is duplicated from ntifs.h


#undef ASSERT
#define ASSERT( exp ) \
    if (!(exp)) \
        RxDbgBreakPoint(__LINE__)

#undef ASSERTMSG
#define ASSERTMSG( msg, exp ) \
    if (!(exp)) \
        RxDbgBreakPoint(__LINE__)

#endif //!DBG
#endif

//this will make asserts go to our routine

#define RtlAssert RxAssert
VOID
RxAssert(
    IN PVOID FailedAssertion,
    IN PVOID FileName,
    IN ULONG LineNumber,
    IN PCHAR Message OPTIONAL
    );

#endif //ifdef RDBSS_ASSERTS


#endif // _RDBSSASSERT_INCLUDED_



#elif (NTDDI_VERSION >= NTDDI_WIN2K)

/*++

Copyright (c) 1989  Microsoft Corporation

Module Name:

    RxAssert.h

Abstract:

    This module declares the prototypes and global data used by the special RDBSS assert facilties.

Author:

    Joe Linn     [JoeLinn]    21-oct-1996

Revision History:


Notes:


--*/

#ifndef _RDBSSASSERT_INCLUDED_
#define _RDBSSASSERT_INCLUDED_

VOID RxDbgBreakPoint(ULONG LineNumber);

//only do this is my routine is the one of interest.......

#ifdef RDBSS_ASSERTS

#if 0
#if !DBG

//here, ntifs will have already defined the asserts away..........
//   so, we just put them back.....this code is duplicated from ntifs.h


#undef ASSERT
#define ASSERT( exp ) \
    if (!(exp)) \
        RxDbgBreakPoint(__LINE__)

#undef ASSERTMSG
#define ASSERTMSG( msg, exp ) \
    if (!(exp)) \
        RxDbgBreakPoint(__LINE__)

#endif //!DBG
#endif

//this will make asserts go to our routine

#define RtlAssert RxAssert
VOID
RxAssert(
    IN PVOID FailedAssertion,
    IN PVOID FileName,
    IN ULONG LineNumber,
    IN PCHAR Message OPTIONAL
    );

#endif //ifdef RDBSS_ASSERTS


#endif // _RDBSSASSERT_INCLUDED_



#else /* NTDDI_VERSION */

#error This file cannot be included for this NTDDI_VERSION.

#endif /* NTDDI_VERSION */


