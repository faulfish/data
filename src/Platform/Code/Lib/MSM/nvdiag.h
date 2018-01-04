#ifndef NVDIAG_H
#define NVDIAG_H
/*==========================================================================

                      Diagnostic Packet Definitions

  Description: Packet definitions between the diagnostic subsystem
  and the external device.

  !!! NOTE: All member structures of diag packets must be PACKED.
  
  !!! WARNING: Each command code number is part of the externalized
  diagnostic command interface.  This number *MUST* be assigned
  by a member of QCT's tools development team.

Copyright (c) 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                            Edit History

  $Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/nvdiag.h.-arc   1.1   Apr 01 2009 14:19:26   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/20/01   jal     Updated for core VU
06/27/01   jal     Created file.

===========================================================================*/

#include "comdef.h"

#include "diagcmd.h"
#include "diagpkt.h"
//#include "diagi.h"
#include "log_codes.h"
#include "feature.h"
#include "nv.h"


/* -------------------------------------------------------------------------
** Packet Definitions
** ------------------------------------------------------------------------- */                           
#if defined(T_WINNT)
#error code not present
#endif

/*==========================================================================

PACKET   DIAG_NV_PEEK_F

PURPOSE  Sent from the DM to the DMSS to request a read of a block of data
         from nonvolatile storage.

============================================================================*/
typedef PACKED struct
{
  
  uint8 xx_cmd_code;

  word  address;                /* NV memory address to read */
  byte  length;                 /* length, up to 32 bytes    */

} nvdiag_peek_req_type;

typedef PACKED struct
{
  uint8 xx_cmd_code;
  
  nv_peek_type  peeknv;        /* The nv definition of a peek item */

} nvdiag_peek_rsp_type;


/*==========================================================================

PACKET   DIAG_NV_POKE_F

PURPOSE  Sent from the DM to the DMSS to request a write of a block of data
         to nonvolatile storage.

============================================================================*/
/* Maximum number of bytes that can be poked into NVM with one request */
#define DIAG_MAX_NV_POKE 4

typedef PACKED struct
{
  uint8 xx_cmd_code;
  
  nv_poke_type nvpoke;        /* nv item type */

} nvdiag_poke_cmd_type;


/*==========================================================================

PACKET   DIAG_NV_READ_F

PURPOSE  Sent from the DM to the DMSS to request a read of an item
         from nonvolatile storage.

============================================================================*/
#define DIAG_NV_ITEM_SIZE 128

DIAGPKT_REQ_DEFINE(DIAG_NV_READ_F)
  
 uint16 item;                         /* Which item - use nv_items_enum_type */
 uint8  item_data[DIAG_NV_ITEM_SIZE]; /* Item itself - use nv_item_type      */
 uint16 nv_stat;                      /* Status of operation
                                                     - use nv_stat_enum_type */

DIAGPKT_REQ_END

DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_NV_READ_F)


/*==========================================================================

PACKET   DIAG_NV_WRITE_F

PURPOSE  Sent from the DM to the DMSS to request a write of an item
         to nonvolatile storage.

============================================================================*/
DIAGPKT_REQ_DEFINE(DIAG_NV_WRITE_F)
  
 uint16 item;                         /* Which item - use nv_items_enum_type */
 uint8  item_data[DIAG_NV_ITEM_SIZE]; /* Item itself - use nv_item_type      */
 uint16 nv_stat;                      /* Status of operation
                                                     - use nv_stat_enum_type */
DIAGPKT_REQ_END

DIAGPKT_DEFINE_RSP_AS_REQ(DIAG_NV_WRITE_F)


/*===========================================================================

PACKET   DIAG_SMS_READ_F

PURPOSE  This message requests a read of the given SMS message out of NV

RESPONSE The diag_sms_read_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  byte address;                                /* Which message to get */
} nvdiag_sms_read_req_type;

typedef PACKED struct
{
  byte cmd_code;                           /* Command code */
  nv_stat_enum_type nv_stat;               /* Status of operation */
  nv_sms_type sms_message;   /* SMS msg read from the phone - variable size */
} nvdiag_sms_read_rsp_type;


/*===========================================================================

PACKET   DIAG_SMS_WRITE_F

PURPOSE  This message requests a write of the given SMS message into NV.
         It is a variable length request message - only as long as dictated
         by the length field in sms_message.

RESPONSE The diag_sms_write_rsp_type will be sent in response.

===========================================================================*/
typedef PACKED struct
{
  byte cmd_code;                               /* Command code */
  nv_sms_type sms_message;                     /* SMS message to write */
} nvdiag_sms_write_req_type;

typedef PACKED struct
{
  byte cmd_code;                             /* Command code */
  nv_stat_enum_type nv_stat;                 /* Status of operation */
} nvdiag_sms_write_rsp_type;

#endif /* NVDIAG_H */

