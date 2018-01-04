#ifndef HSDIAG_H
#define HSDIAG_H
/*==========================================================================

                      Diagnostic Packet Definitions

  Description: Packet definitions between the diagnostic subsystem
  and the external device.

  !!! NOTE: All member structures of DIAG packets must be PACKED.
  
  !!! WARNING: Each command code number is part of the externalized
  diagnostic command interface.  This number *MUST* be assigned
  by a member of QCT's tools development team.

Copyright (c) 2001 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/
/*==========================================================================
                              HSDIAG Key Map
                              
Maps key press sequences from invariant diag commands to a table of sequences
provided by clients

Clients create a mapping of client defined actions to sequences of key presses.
The client defined action is a 32 bit id with the upper 16 bits not used.  
A sequence of key presses is defined using the key defined in this header file.
A sequence can refer to other sequences and to a delay action.  The delay
action pauses between key presses for the user provided number of ms.  Note
that the keymap handler function provides a default delay between key presses
of DIAG_KEYMAP_DEFAULT_DELAY milli-seconds.                

DIAG packet handlers calling this function should first commit their response
since the processes is dependent on the particular sequence chosen and could
incorporate delays.  Also the natural default delay pushes out responses from
being timely.

Users create tables like the following:

#define USER_DEFINED_ACTION_1  1
#define USER_DEFINED_ACTIOM_2  2
#define USER_DEFINED_ACTION_3  3
#define USER_DEFINED_ACTIOM_4  4

HSDIAG_KEYMAP_DECLARE( my_keymap, 20 )
   HSKM_ACT( USER_DEFINED_ACTION_1 ), HSKM( UP ), HSKM( UP ), HSKM( UP ), HSKM( SEL ), HSKM( DOWN ), HSKM( SEL ),
   HSKM_ACT( USER_DEFINED_ACTION_2 ), HSKM( DOWN ), HSKM( UP_LEFT ), HSKM( SEL ),
   HSKM_ACT( USER_DEFINED_ACTION_3 ), HSKM( DOWN ), HSKM( UP_LEFT ), HSKM_SEQ( USER_DEFINED_ACTION_2 ),
   HSKM_ACT( USER_DEFINED_ACTION_4 ), HSKM( DOWN ), HSKM_DELAY( 1000 ), HSKM( SEL ),
HSDIAG_KEYMAP_END

Uers define and implement DIAG ICD command packet handlers following
the nominal procedure.  Seperate handlers can be created for the above
four actions, or all the actions can be consolodate into a single action
handler.  The following assumes that the above USER_DEFINED_ACTION_X id's
are also used in the definition of the DIAG ICD command.

packet_handler_pseudo_code
   action = get_action_from_req
   commit_response
   hsdiag_keymap( action, my_keymap ); 
end

Copyright (c) 2002 by QUALCOMM, Incorporated.  All Rights Reserved.
===========================================================================*/

/* <EJECT> */
/*===========================================================================

                            Edit History

  $Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/hsdiag.h.-arc   1.1   Apr 01 2009 14:19:24   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/24/03   kap     Introduced hs key map utility.
08/20/01   jal     Updated for core VU
01/26/01   lad     Created file from what used to be diagpkt.h
===========================================================================*/

#include "customer.h"
#include "comdef.h"
#include "diagcmd.h"
#include "diagpkt.h"


/* -------------------------------------------------------------------------
** Packet definitions for logging services.
** ------------------------------------------------------------------------- */

#define DIAGPKT_HS_SCRN_HIGH 4
#define DIAGPKT_HS_SCRN_WIDE 12

#if defined(T_WINNT)
//#error code not present
#endif

/* Default number of milliseconds to wait between subsequent key presses
** in a keypress map  
*/
#define HSDIAG_KEYMAP_DEFAULT_DELAY 500
#define HSDIAG_KEYMAP_BUTTPRESS_DELAY 100

/* Masks for action identifiers
**   Client must not define action id's with the MS 16 bits used.
**   Bits are used to indicate to the keymap routines such actions as referring
**     to other action sequences or inserting additional delay
*/
#define HSDIAG_KEYMAP_DELAY_BIT    0x80000000
#define HSDIAG_KEYMAP_ACTION_BIT   0x40000000
#define HSDIAG_KEYMAP_SEQUENCE_BIT 0x20000000

/* Delay mask - to assure user does not clobber the reserved bits
*/
#define HSDIAG_KEYMAP_MS_MASK    0x0008FFFF

/* Last action in a table indicating the end
*/
#define HSDIAG_KEYMAP_TABLE_END  0xFFFFFFFF

/* -------------------------------------------------------------------------
** Invariant Keys
**   Defined here for DIAG clients to be independent of the keypad and
**   handset implementations.
**   !!! NOTE: This enumeration indexes an array          !!!
**   !!! NOTE: The values must be representable in 16bits !!!
** ------------------------------------------------------------------------- */
typedef enum
{
  HSDIAG_KEY_NONE = 0,
  HSDIAG_KEY_POWER,
  HSDIAG_KEY_UP,
  HSDIAG_KEY_DOWN,
  HSDIAG_KEY_LEFT,
  HSDIAG_KEY_RIGHT,
  HSDIAG_KEY_UP_LEFT,
  HSDIAG_KEY_UP_RIGHT,
  HSDIAG_KEY_DOWN_LEFT,
  HSDIAG_KEY_DOWN_RIGHT,
  HSDIAG_KEY_NAV_SEL,
  HSDIAG_KEY_SEL,
  HSDIAG_KEY_CLR,
  HSDIAG_KEY_0,
  HSDIAG_KEY_1,
  HSDIAG_KEY_2,
  HSDIAG_KEY_3,
  HSDIAG_KEY_4,
  HSDIAG_KEY_5,
  HSDIAG_KEY_6,
  HSDIAG_KEY_7,
  HSDIAG_KEY_8,
  HSDIAG_KEY_9,
  HSDIAG_KEY_POUND,
  HSDIAG_KEY_STAR,
  HSDIAG_KEY_PF1,
  HSDIAG_KEY_PF2,
  HSDIAG_KEY_SEND,
  HSDIAG_KEY_END,
  HSDIAG_KEY_INFO,
  HSDIAG_KEY_VOL_UP,
  HSDIAG_KEY_VOL_DWN
}
hsdiag_keymap_keys_type;

/* -------------------------------------------------------------------------
** MACRO HSKM
** DESCRIPTION
**   Map a slightly short hand name to actual key names.  Basically expands
**   to HSDIAG_KEY_##<name>
** ------------------------------------------------------------------------- */
#define HSKM( xx_key )\
   (HSDIAG_KEY_##xx_key)

/* -------------------------------------------------------------------------
** MACRO HSKM_DELAY
** DESCRIPTION
**    Requests a delay in milliseconds before the next action.  Must be
**    less than 5 minutes (300,000ms) (Chosen somewhat arbitrarily, but also
**    made arbitrarily large)
** ------------------------------------------------------------------------- */
#define HSKM_DELAY( xx_ms )\
   (((xx_ms) & HSDIAG_KEYMAP_MS_MASK) | HSDIAG_KEYMAP_DELAY_BIT)

/* -------------------------------------------------------------------------
** MACRO HSKM_ACT
** DESCRIPTION
**    Defines or refers to a sequnce of key presses.  When used in the
**    action column of a DIAG key map table this defines a key sequence
**    to execute when the action is referred from another sequence.
** ------------------------------------------------------------------------- */
#define HSKM_ACT( xx_id )\
   ((xx_id) | HSDIAG_KEYMAP_ACTION_BIT)

/* -------------------------------------------------------------------------
** MACRO HSKM_SEQ
** DESCRIPTION
**    Refers to an action entry in the table.
** ------------------------------------------------------------------------- */
#define HSKM_SEQ( xx_id )\
   ((xx_id) | HSDIAG_KEYMAP_SEQUENCE_BIT)

/* -------------------------------------------------------------------------
** MACRO HSDIAG_KEYMAP_DECLARE
** DESCRIPTION
**   Starts a key map declaration
**      xx_name            - Name of the table, used in call to hsdiag_keymap()
** ------------------------------------------------------------------------- */
#define HSDIAG_KEYMAP_DECLARE( xx_name )\
   static const uint32 xx_name[] = {

/* -------------------------------------------------------------------------
** MACRO HSDIAG_KEYMAP_END
** DESCRIPTION
**   Ends a key map declaration
** ------------------------------------------------------------------------- */
#define HSDIAG_KEYMAP_END\
      ,HSDIAG_KEYMAP_TABLE_END\
   };


/*===========================================================================

PACKET   diag_hs_key_req_type

ID       DIAG_HS_KEY_F

PURPOSE  Sent by DM to cause a keypress input to the handset interface.

RESPONSE Diag queues a keypress to hsi and echos the request packet.

===========================================================================*/
DIAGPKT_REQ_DEFINE( DIAG_HS_KEY_F )

 boolean hold;                     /* If true, diag witholds key release */
 byte    key;                      /* enumerated key, e.g. HS_DOWN_K */

DIAGPKT_REQ_END

DIAGPKT_DEFINE_RSP_AS_REQ( DIAG_HS_KEY_F )


/*===========================================================================

PACKET   diag_hs_lock_req_type

ID       DIAG_HS_LOCK_F

PURPOSE  Sent by DM to lock or unlock the real handset.

RESPONSE Diag queues a command to hsi with new hs lock state, then
         echos the packet to the DM.

===========================================================================*/
typedef enum {
  HS_LOCK_F,                       /* command to lock real handset */
  HS_REL_F                         /* command to release real handset */
} diag_hs_lock_type;

DIAGPKT_REQ_DEFINE( DIAG_HS_LOCK_F )

 word        lock;        /* diag locks or unlocks real hs
                             Use diag_hs_lock_type.          */

DIAGPKT_REQ_END

DIAGPKT_DEFINE_RSP_AS_REQ( DIAG_HS_LOCK_F )

/*===========================================================================

PACKET   diag_screen_req_type

ID       DIAG_HS_SCREEN_F 

PURPOSE  Sent by DM to request an image of the handset screen and annun-
         ciators.

RESPONSE DMSS returns diag_screen_rsp_type packet that contains handset
         screen and annunciators.

===========================================================================*/

DIAGPKT_REQ_DEFINE( DIAG_HS_SCREEN_F )
DIAGPKT_REQ_END

DIAGPKT_RSP_DEFINE( DIAG_HS_SCREEN_F )

  boolean backlight_on;         /* Backlight state         */
  byte scrn[ DIAGPKT_HS_SCRN_HIGH * DIAGPKT_HS_SCRN_WIDE ];
  byte reserved1;
  word annun;                   /* Annunciators            */
  byte blink_scrn[ DIAGPKT_HS_SCRN_HIGH * DIAGPKT_HS_SCRN_WIDE ];
  word blink_annun;             /* Annunciators for blink  */
  byte rssi;                    /* RSSI indicator          */
  byte reserved2;
  word blink_rate;              /* Blink rate for blinking */

DIAGPKT_RSP_END


#if defined(FEATURE_BITMAP_RASTER)
#error code not present
#endif

/* -------------------------------------------------------------------------
** Functions 
** ------------------------------------------------------------------------- */
#ifdef __cplusplus
extern "C"
{
#endif

/*===========================================================================

FUNCTION DIAG_KEYMAP

DESCRIPTION
   This routine queues a keymap processing action to be handled by the DIAG
   task in the background.  The caller should commit its response packet
   prior to calling this routine.
    
   Given a table of key sequences and an action, finds the action in the
   table and injects the associated sequence of keys into the keypad driver
   emulating the sequence as though entered physically on the keypad

===========================================================================*/
  void hsdiag_keymap (uint32 action, const uint32 * table);

#ifdef __cplusplus
}
#endif

#if defined(T_WINNT)
//#error code not present
#endif


#endif /* HSDIAG_H */

