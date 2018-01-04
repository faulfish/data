#ifndef DIAGPKT_H
#define DIAGPKT_H

/*==========================================================================

                    Diagnostic System Packet Interface

  Description: Interface definitions use the diag packet processing service.

  !!! NOTE: Diagnostic packets are sent over an external interface.  
  Structure definitions must be portable to other C compilers.  The easiest
  way to do this is to byte-align the packet definitions.  The ARM compiler
  uses the PACKED keyword to denote byte alignment.  Most other compilers 
  use #pragma pack(1) to enable structure packing.  The structure is not
  required to be byte-aligned, but it is required to be portable to other
  compilers.
  
  !!! WARNING: Each command code is part of the externalized diagnostic command
  interface.  Internally within QCT, these numbers *MUST* be assigned by a
  member of QCT's tools development team.  3rd party developers may use the
  reserved subsystem IDs to define and grow the diagnostic packet needs.

Copyright (c) 1992-2002 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */

/*===========================================================================

                            Edit History

  $Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/diagpkt.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/30/02   lad     Revised file for multimode diagnostics version.
05/21/01   lad     Made diagpkt headers in DIAGPKT macros opaque.
                   Added FEATURE_DIAG_PACKET_COUPLING to facililitate migration
                   to this API.
04/17/01   lad     Removed inclusion of diagtune.h.
                   Moved subsystem dispatch IDs to diagcmd.h.
04/06/01   lad     Introduced typedefs for command codes, etc.
                   Updated DIAGPKT_SUBSYS_REQ_DEFINE macros.
                   Added diagpkt_subsys_alloc().
                   Removed diagpkt_process_request() since it is not part of the
                   externalized interface.
02/23/01   lad     Rearchitected the diagnostics subsystem to be a service 
                   rather than a module.  All coupling to targets has been
                   moved to target-specific implementations.  This file now
                   contains an API for packet processing services.  
                   No other information or coupling remains except for
                   featurized legacy code that impacts existing targets.
                   Old history comments have been removed since most of them
                   are no longer applicable.
                   Packet definitions are no longer included in this file.
                   
===========================================================================*/

#include "comdef.h"
#include "customer.h"


/* -------------------------------------------------------------------------
   Type and Packet Definition Macros
   ------------------------------------------------------------------------- */
typedef uint8 diagpkt_cmd_code_type;
typedef uint8 diagpkt_subsys_id_type;
typedef uint16 diagpkt_subsys_cmd_code_type;

typedef PACKED struct
{
  byte opaque_header;
}
diagpkt_header_type;

typedef PACKED struct
{
  byte opaque_header[4];
}
diagpkt_subsys_header_type;

typedef PACKED struct
{
  byte opaque_header;
  word opaque_header2;
  word length;
}
diagpkt_subsys_header_type_Topaz;

/* -------------------------------------------------------------------------
   Function Definitions
   ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif
  /* Packet Handler Types */
  
  /* An array of this type is created by the client and registered with this
     service. It must be declared 'const' (preferrably 'static const').  
     The function is called when an inbound packet matches subsystem ID and
     is within the command code range specified in the table. */ 
  typedef struct
  {
    word cmd_code_lo;
    word cmd_code_hi;
    PACKED void *(*func_ptr) (PACKED void *req_pkt_ptr, uint16 pkt_len);
  }
  diagpkt_user_table_entry_type;

  /* Note: the following 2 items are used internally via the macro below. */
  typedef struct
  {
    word subsysid;
    word count;
    const diagpkt_user_table_entry_type *user_table;
  } diagpkt_master_table_type;

#define DIAGPKT_NO_SUBSYS_ID 0xFF

  void diagpkt_tbl_reg (const diagpkt_master_table_type * tbl_ptr);

  /* Use this macro to register your dispatch table with the diagnostics
     packet dispatching service. */

  /* This macro is used to manually register the client table with the 
     packet dispatch service.  This solution is intended to be temporary
     pending support for C++ on targets.  This macro requires the client
     to explicitly register the table. */
  
  #define DIAGPKT_DISPATCH_TABLE_REGISTER(xx_subsysid, xx_entry) \
    do { \
      static const diagpkt_master_table_type xx_entry##_table = { \
        xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), xx_entry \
      }; \
      diagpkt_tbl_reg (&xx_entry##_table); \
    } while (0)

#ifdef __cplusplus
  /* C++ class used for autoregister */
  class DiagDispatch
  {
  public:
    DiagDispatch (const diagpkt_master_table_type *tbl_ptr)
    {
  	 diagpkt_tbl_reg(tbl_ptr);
    }
  };

  /* If C++ is supported in the build, this macro, defined in the
     preprocessor, instantiates a class with a 'static const' constructor.
     This class is therefore instantiated at boot, calling the constructor
     prior to the system 'main()'.  This  eliminates the need to acquire
     scope to register, it is done automatically. */

  #define DIAGPKT_DISPATCH_AUTOREGISTER(xx_subsysid, xx_entry) \
    static const diagpkt_master_table_type xx_entry##_table = { \
      xx_subsysid, sizeof (xx_entry) / sizeof (xx_entry[0]), xx_entry \
    }; \
    DiagDispatch xx_entry##_instance(&xx_entry##_table)
    
#endif

  /* ------------------------------------------------------------------------
     Functions
     ------------------------------------------------------------------------ */

/*===========================================================================

FUNCTION DIAGPKT_ALLOC

DESCRIPTION
  This function allocates the specified amount of space in the diag output 
  buffer.  If space is unavailable, access to the allocation buffer will be
  blocked using a semaphore until space is available.

DEPENDENCIES
  This may only be called by the diag task for responses to explicit request
  packets!  This function is *not* re-entrant.  If the OS ever supports
  semaphores other than INTLOCK, this function can be made re-entrant.  
  
  diagpkt_commit() must be called to commit the response packet to be sent.
  Not calling diagpkt_commit() will result in a memory leak.
       
============================================================================*/
  PACKED void *diagpkt_alloc (diagpkt_cmd_code_type code, unsigned int length);

/*===========================================================================

FUNCTION DIAGPKT_SUBSYS_ALLOC

DESCRIPTION
  This call is the same as calling diagpkt_alloc(), but is used for
  allocating responses for subsystem commands.  It  fills in the subsystem
  header info for you.

DEPENDENCIES
  This may only be called by the diag task for responses to explicit request
  packets!  This function is *not* re-entrant.  If the OS ever supports
  semaphores other than INTLOCK, this function can be made re-entrant.  
  
  diagpkt_commit() must be called to commit the response packet to be sent.
  Not calling diagpkt_commit() will result in a memory leak.
       
============================================================================*/
  PACKED void *diagpkt_subsys_alloc (diagpkt_subsys_id_type id,
    diagpkt_subsys_cmd_code_type code, unsigned int length);

/*===========================================================================

FUNCTION DIAGPKT_SHORTEN

DESCRIPTION
  This function reduces the length field of a previously allcated buffer. 

  'ptr' must point to the same address that was returned by a prior call to
  diagpkt_alloc() or diagpkt_subsys_alloc().
       
  Allocating too much and using this to shorten the packet is ideal for 
  situations in which the length of the packet is not known prior to 
  allocation.  Using this scheme does, however, consume resources that would
  otherwise be used to buffer outbound diagnostics data.  Please use this
  capability sparingly.

============================================================================*/
  void diagpkt_shorten (PACKED void *ptr, unsigned int length);

/*===========================================================================

FUNCTION DIAGPKT_COMMIT

DESCRIPTION
  This function commits previously allocated space in the diagnostics output
  buffer.

  'ptr' must be the same pointer that was returned from diagpkt_alloc() or
  diagpkt_subsys_alloc().

  This function signals the DIAG task and may cause a context switch.

  The packet handler type returns the response pointer.  The dispatcher
  calls diagpkt_commit() internally.  This only needs to be called
  explicitly if the packet needs to be committed before the packet handler
  returns.  In this case, the packet handler should return NULL.

============================================================================*/
  void diagpkt_commit (PACKED void *ptr);

/*===========================================================================

FUNCTION DIAGPKT_GET_CMD_CODE 

DESCRIPTION
  This function returns the command code in the specified diagnostics packet.

===========================================================================*/
  diagpkt_cmd_code_type diagpkt_get_cmd_code (PACKED void *ptr);

/*===========================================================================

FUNCTION DIAGPKT_SET_CMD_CODE 

DESCRIPTION
  This function sets the command code in the specified diagnostics packet.

===========================================================================*/
  void diagpkt_set_cmd_code (PACKED void *ptr, diagpkt_cmd_code_type cmd_code);

/*===========================================================================

FUNCTION DIAGPKT_SUBSYS_GET_ID 

DESCRIPTION
  This function returns the subsystem command code in the specified 
  diagnostics packet.
  
  If the packet is not a subsystem packet, 0xFF is returned.

===========================================================================*/
  diagpkt_subsys_id_type diagpkt_subsys_get_id (PACKED void *ptr);

/*===========================================================================

FUNCTION DIAGPKT_SUBSYS_GET_CMD_CODE 

DESCRIPTION
  This function returns the subsystem command code in the specified 
  diagnostics packet.
  
  If the packet is not a DIAG_SUBSYS_CMD_F packet, 0xFFFF is returned.  

===========================================================================*/
  diagpkt_subsys_cmd_code_type diagpkt_subsys_get_cmd_code (PACKED void *ptr);

/*===========================================================================

FUNCTION DIAGPKT_ERR_RSP

DESCRIPTION
  This function builds an error packet.
  Usage: rsp_ptr = diagpkt_err_rsp ( error_code, req_pkt );

===========================================================================*/
  PACKED void *diagpkt_err_rsp (diagpkt_cmd_code_type code,
    PACKED void *req_ptr, uint16 req_len);

/*===========================================================================

FUNCTION DIAGPKT_ASYNC_BLOCK

DESCRIPTION
  This procedure blocks while we wait for the DIAG_ASYNC_BLOCK_SIG to be set
  to allow for asynchronous delays in packet handling.

============================================================================*/
  void diagpkt_async_block (void);

/*===========================================================================

FUNCTION DIAGPKT_ASYNC_RELEASE

DESCRIPTION
  This procedure sets the DIAG_ASYNC_BLOCK_SIG to end the asynchronous delay
  started with DIAGPKT_ASYNC_BLOCK

============================================================================*/
  void diagpkt_async_release (void);




/*-------------------------------------------------------------
           Diagnostics Packet Type Defintion Macros
           
  These macros were defined to provide and enforce naming
  conventions for declaring packets.  However, these macros
  make editor tags useless and add consufion.  The use of
  these macros has been deprecated, but is included here for
  compatibility with existing usage.  The naming convention
  enforced by these macros is not required for use of this
  service.
  
  !!! It is not recommended to continue use of these macros.

  The naming convention enforced by these macros is outlined
  below:

  Command codes use the naming convention: DIAG_xxx_F
  
  Requests types:
  DIAG_xxx_F_req_type
  
  Response types:
  DIAG_xxx_F_rsp_type
  
-------------------------------------------------------------*/
#ifndef FEATURE_DIAG_EXPOSED_HEADER
#define DIAGPKT_REQ_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_req_tag \
                 xx_cmd_code##_req_type; \
  PACKED struct  xx_cmd_code##_req_tag { \
    diagpkt_header_type xx_header;
#else
#define DIAGPKT_REQ_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_req_tag \
                 xx_cmd_code##_req_type; \
  PACKED struct  xx_cmd_code##_req_tag { \
    diagpkt_cmd_code_type command_code;
#endif

#define DIAGPKT_REQ_END };

  /* If the response is the same structure as the request... */
#define DIAGPKT_DEFINE_RSP_AS_REQ( xx_cmd_code ) \
  typedef xx_cmd_code##_req_type xx_cmd_code##_rsp_type;

#ifndef FEATURE_DIAG_EXPOSED_HEADER
#define DIAGPKT_RSP_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_rsp_tag \
                 xx_cmd_code##_rsp_type; \
  PACKED struct  xx_cmd_code##_rsp_tag { \
    diagpkt_header_type xx_header;
#else
#define DIAGPKT_RSP_DEFINE( xx_cmd_code ) \
  typedef struct xx_cmd_code##_rsp_tag \
                 xx_cmd_code##_rsp_type; \
  PACKED struct  xx_cmd_code##_rsp_tag { \
    diagpkt_cmd_code_type command_code;
#endif

#define DIAGPKT_RSP_END };

/*-------------------------------------------------------------
     Diagnostics Subsystem Packet Type Definition Macros

  These macros were defined to provide and enforce naming
  conventions for declaring packets. 
  
  !!! It is not recommended to continue use of these macros.

  The naming convention enforced by these macros is outlined
  below:

  Diag has a subsystem command that dispatches diag commands to
  various subsystems.  All subsystem packet types use the same
  naming convention throughout the DMSS.  The subsystem command
  uses a 16 bit command code per subsystem.  This results in
  user data starting on a 32 bit boundary.
  
  The naming convention is as follows:
  
  Command codes use the naming convnetion: DIAG_SUBSYS_xxx_F
  
  Requests types:
  DIAG_SUBSYS_xxx_yyy_req_type
  
  Response types:
  DIAG_SUBSYS_xxx_yyy_rsp_type
  
-------------------------------------------------------------*/
#define DIAGPKT_SUBSYS_REQ_DEFINE( xx_subsys, xx_subsys_cmd_code ) \
  typedef struct DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_tag \
                 DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_type; \
  PACKED struct  DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_req_tag { \
    diagpkt_subsys_header_type xx_header;

#define DIAGPKT_SUBSYS_REQ_END };

#define DIAGPKT_SUBSYS_RSP_DEFINE( xx_subsys, xx_subsys_cmd_code ) \
    typedef struct DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag \
                   DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_type; \
    PACKED struct  DIAG_SUBSYS_##xx_subsys##_##xx_subsys_cmd_code##_rsp_tag { \
      diagpkt_subsys_header_type xx_header;

#define DIAGPKT_SUBSYS_RSP_END };

#ifdef FEATURE_DIAG_PACKET_COUPLING

  /* As a temporary measure, this is added since mclog.h expects packet
     definitions to be defined in diagpkt.h  Once mclog.h is updated, this can 
     be removed. */
#include "diagcmd.h"		/* mclog.c needs to include this */
#include "cdma2kdiag.h"
#include "parmdiag.h"
#endif

#ifdef __cplusplus
}
#endif

#endif				/* DIAGPKT_H  */
