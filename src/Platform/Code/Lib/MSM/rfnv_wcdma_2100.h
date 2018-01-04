#ifndef RFNV_WCDMA_2100_H
#define RFNV_WCDMA_2100_H

/*===========================================================================

                      R F   N V   f o r   W C D M A  2100

DESCRIPTION
  This header file contains definitions  to interface with RF NV for WCDMA.

Copyright (c) 1997 - 2003 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/rfnv_wcdma_2100.h.-arc   1.1   Apr 01 2009 14:19:28   Kevin Tai  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
02/03/04   ao      included rfnv_wcdma.h
08/21/03   ap      Final modifications to support WCDMA 2100
06/17/03   eh      Initial Revision.

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/
#include "comdef.h"
#include "customer.h"
#include "nv.h"
#include "rfnv.h"
#include "rfnv_wcdma.h"

/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

extern const rfnv_items_list_type rfnv_wcdma_2100_nv_items_list[ ];

extern rfnv_wcdma_nv_tbl_type rfnv_wcdma_2100_nv_tbl;


/*===========================================================================

                         FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

FUNCTION rfnv_wcdma_2100_copy_nv_item                              

DESCRIPTION
  This function copies one RF NV data item, as specified by "item_code/identifier" 
  into an element of the RF NV item tbl. 
  This function must be called during initialization for each and every RF NV item.

DEPENDENCIES
  Note that there must NOT be a dependency on the read order of the NV
  calibration items.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
extern void rfnv_wcdma_2100_copy_nv_item
(
  nv_items_enum_type  item_code,    /* NV Item to read */
  nv_item_type        *item_ptr     /* Pointer where to get the item */
);


/*===========================================================================

FUNCTION RFNV_WCDMA_2100_LIST_SIZE                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF Common items NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
extern uint16 rfnv_wcdma_2100_list_size( void );

/*===========================================================================

FUNCTION RFNV_WCDMA_2100_ITEM_TABLE                            EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
extern void *rfnv_wcdma_2100_item_table(void);

#endif /* RFNV_WCDMA_2100_H */

