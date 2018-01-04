////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2006 Richard Sposato
// Copyright (c) 2006 Peter Kümmel
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The authors make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////

#ifndef LOKI_CONST_POLICY_INC_
#define LOKI_CONST_POLICY_INC_

// $Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/ShareLib/loki-0.1.5/include/loki/ConstPolicy.h.-arc   1.1   Apr 01 2009 14:21:50   Kevin Tai  $


namespace Loki
{

////////////////////////////////////////////////////////////////////////////////
/// @note These policy classes are used in LockingPtr and SmartPtr to define
///  how const is propagated from the pointee.
////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////
///  \class DontPropagateConst
///
///  \ingroup ConstGroup
///  Don't propagate constness of pointed or referred object.
////////////////////////////////////////////////////////////////////////////////

    template< class T >
    struct DontPropagateConst
    {
        typedef T Type;
    };

////////////////////////////////////////////////////////////////////////////////
///  \class PropagateConst
///
///  \ingroup ConstGroup
///  Propagate constness of pointed or referred object.
////////////////////////////////////////////////////////////////////////////////

    template< class T >
    struct PropagateConst
    {
        typedef const T Type;
    };

// default will not break existing code
#ifndef LOKI_DEFAULT_CONSTNESS
#define LOKI_DEFAULT_CONSTNESS DontPropagateConst
#endif

} // end namespace Loki

#endif // end file guardian

/* $Log:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/ShareLib/loki-0.1.5/include/loki/ConstPolicy.h.-arc  $
 * 
 *    Rev 1.1   Apr 01 2009 14:21:50   Kevin Tai
 * no change
 * 
 *    Rev 1.0   Apr 22 2008 20:56:16   Alan Tu
 * Initial revision.
 * 
 *    Rev 1.2   Nov 10 2006 14:49:52   Alan Tu
 * Update*
 * 
 *    Rev 1.1   Nov 10 2006 14:31:06   Alan Tu
 * Update
// Revision 1.1  2006/02/19 22:04:28  rich_sposato
// Moved Const-policy structs from SmartPtr.h to ConstPolicy.h.
*/
