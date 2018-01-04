#ifndef RFNV_GSM_1800_H
#define RFNV_GSM_1800_H


/*===========================================================================

                         R F   N V   M a n a g e r

DESCRIPTION
  This header file contains definitions necessary inorder to interface with
  the RF NV Manager.

Copyright (c) 2002 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/rfnv_gsm_1800.h.-arc   1.1   Apr 01 2009 14:19:26   Kevin Tai  $ $DateTime: 2003/11/19 14:42:07 $ $Author:   Kevin Tai  $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/21/03   ap      Final modifications to include NV support for GSM 1800.
05/27/03   jtn     Use NV_TEST_CODE_VER_I instead of NV_RF_CAL_DATE_I to check for old rx gain settings
04/28/03   jtn     Changes for gain switchpoints in NV
04/17/03   jtn     Added rf_cal_date to RAM structure that holds NV items so only have to read it from NV once
04/02/03   jtn     Added extern boolean rfnv_cal_older_than(uint32 specified_date) called by rfGSM_1800_init().
12/05/02   JC      Added frequency compensated switch point functions.
11/15/02   thh     Removed RFGSM_1800_RTR6200_B2.  B2 is detected via revision #
                   read from the chip.
11/07/02   thh     Added RFGSM_1800_RF_CONFIG_MAX to rfGSM_1800_rf_config_type.
10/15/02   thh     Added RFGSM_1800_RTR6200_B2 for GZIFTRIC B2 support.
08/30/02   JC      Obsoleted super-het radio support.
08/28/02   thh     Added GSM_1800 support.
08/12/02   thh     Added RF_CONFIG support.
06/10/02   JC      New APIs for Rx frequency compensation moving forwards
                   using ZIF architecture.
04/03/02   thh     Added support for new Txprofile API.
03/20/02   ih      Added PA start/stop time calibration
03/14/02   thh     Added Tx frequency compensation support.
02/12/02   JC      Removed extraneous NV declarations. Removed coupling
                   to nv.h for GPRS development.
01/21/02   JC      Initial Revision for GSM_1800.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "rex.h"
#include "nv.h"
#include "rfnv_gsm.h"


/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

extern const rfnv_items_list_type rfnv_gsm_1800_nv_items_list[ ];
extern rfnv_gsm_nv_tbl_type rfnv_gsm_1800_nv_tbl;
extern rfnv_gsm_rx_switch_type gsm_1800_rx_switch_dBm;

// TAN:
/*---------------------------------------------------------------------------
  GSM_1800 Power Index
---------------------------------------------------------------------------*/
#define RF_NUM_PWR_INDEX_GSM_1800  16
#define RF_MAX_PWR_INDEX_GSM_1800  (RF_NUM_PWR_INDEX_GSM_1800 - 1)



/*===========================================================================

FUNCTION RFNV_GSM_1800_COPY_NV_ITEM

DESCRIPTION
  This function updates the specfied item (item_code) in the rfnv_gsm_cmn_nv_tbl
  with the data supplied via the item_ptr.  This function used to be called
  rfnv_read_nv_item().

DEPENDENCIES
  Note that there must NOT be a dependency on the read order of the NV
  calibration items.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rfnv_gsm_1800_copy_nv_item
(
  nv_items_enum_type item_code,
  nv_item_type *item_ptr
);

/*===========================================================================

FUNCTION RFNV_GSM_1800_PRECOMPUTE_RX_SWITCH_POINTS

DESCRIPTION
  This function computes the Rx switch points for the set of calibrated
  channels (ARFCNs).  It places the switch points in an internal buffer
  to reduce run-time overhead.

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rfnv_gsm_1800_precompute_rx_switch_points(void);

/*===========================================================================

FUNCTION RFNV_GSM_1800_LIST_SIZE                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF Common items NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
extern uint16 rfnv_gsm_1800_list_size( void );


/*===========================================================================

FUNCTION RFNV_GSM_1800_ITEM_TABLE                            EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
extern void *rfnv_gsm_1800_item_table(void);

#endif /* RFGSM_1800_NV_H */
