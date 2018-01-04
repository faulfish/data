#ifndef RFNV_CMN_H
#define RFNV_CMN_H

/*===========================================================================

                              R F   N V   C O M M O N

DESCRIPTION
  This header file contains definitions in order to interface with
  the RF NV common.

Copyright (c) 1997 - 2003 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/rfnv_cmn.h.-arc   1.1   Apr 01 2009 14:19:26   Kevin Tai  $
//Bonnie update 2005/07/15

when       who     what, where, why
--------   ---     ----------------------------------------------------------
08/21/03   ap      Final modifications to support WCDMA 2100/1900 and GSM 850/900/1800/1900.
06/25/03   ap      added functionality for Test NV Items 
06/17/03   eh      Initial Revision.


===========================================================================*/


/*===========================================================================

                              INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "nv.h"
//#include "rfnv.h"

/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

//extern const rfnv_items_list_type rfnv_cmn_nv_items_list[ ];


#define RF_NV_FREQ_COMP_TEMP_TABLE_SIZ 64

#define RFNV_VCO_TEMP_TBL_SIZ  64

 
typedef struct {

  /* The RF configuration as read from the NV Services */
  uint8 rf_config;

  /* min and max value for VBATT ADC reading */
  uint8 vbatt[2];  
   
  uint8 rf_cal_ver[NV_SIZE_OF_VERSION]; 

  /* min and max value for THERM ADC reading */
  uint8 therm[2];

  uint16 trk_lo_adj_default;
  uint16 trk_lo_adj_slope_default;
  uint8 rfr_bb_filter; 
  uint8 rfr_iq_line_resistor;
  int16 vco_default;
  uint16 vco_slope;
  uint8 vco_slope_range;
  int16 vco_vs_temp[ RFNV_VCO_TEMP_TBL_SIZ ];
  uint32 rgs_time;
  uint8 rgs_type;
  uint8 rgs_temp;
  int16 rgs_vco;
  int16 rgs_rot;

  byte  ftm_mode;
 
} rfnv_cmn_nv_tbl_type;

/* pointer to CMN NV table */
//extern rfnv_cmn_nv_tbl_type   *rfnv_cmn_nv_tbl_ptr;
//extern rfnv_cmn_nv_tbl_type rfnv_cmn_nv_tbl;

/*===========================================================================

                         FUNCTION PROTOTYPES

===========================================================================*/

/*===========================================================================

FUNCTION rfnv_cmn_copy_nv_item                              

DESCRIPTION
  This function copies one RF NV data item, as specified by "item_code/identifier" 
  into an element of the RF NV item table. 
  This function must be called during initialization for each and every RF NV item.

DEPENDENCIES
  Note that there must NOT be a dependency on the read order of the NV
  calibration items.

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
//extern void rfnv_cmn_copy_nv_item
//(
//  nv_items_enum_type  item_code,    /* NV Item to read */
//  nv_item_type        *item_ptr     /* Pointer where to get the item */
//);

/*===========================================================================

FUNCTION rfnv_cmn_get_nv_tbl_ptr                                 

DESCRIPTION
  Returns pointer to Common RF NV table.

DEPENDENCIES
  None

RETURN VALUE
  Returns pointer to common RF NV table.

SIDE EFFECTS
  None

===========================================================================*/
//extern rfnv_cmn_nv_tbl_type *rfnv_cmn_get_nv_tbl_ptr( void);


/*===========================================================================

FUNCTION RFNV_CMN_LIST_SIZE                             EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF Common items NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
//extern uint16 rfnv_cmn_list_size( void );


/*===========================================================================

FUNCTION RFNV_CMN_ITEM_TABLE                            EXTERNALIZED FUNCTION

DESCRIPTION
  This function returns the number of items in the RF NV list.

DEPENDENCIES
  None

RETURN VALUE
  Number of item in the NV list.

SIDE EFFECTS
  None

===========================================================================*/
//extern void *rfnv_cmn_item_table(void);


#endif /* RFNV_CMN_H */


