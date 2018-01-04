#ifndef RFNV_H
#define RFNV_H
/*===========================================================================

                             R F   N V   M a n a g e r

DESCRIPTION
  This header file contains the data structure and function declarations to interface with
  the RF NV Manager.

Copyright (c) 1997 - 2003 by QUALCOMM, Inc.  All Rights Reserved.
===========================================================================*/


/*===========================================================================

                      EDIT HISTORY FOR FILE

$Header:   //BQT/VM67/Prjbase3/QSyncher_V2/PlatformTool/Code/Lib/MSM/rfnv.h.-arc   1.1   Apr 01 2009 14:19:28   Kevin Tai  $


when       who     what, where, why
--------   ---     ----------------------------------------------------------
11/18/03   eh      Modified to support new RFNV structure.
08/21/03   ap      Final modifications to support WCDMA 2100/1900 and GSM 850/900/1800/1900.
06/17/03   eh      Updated and clean up to support WCDMA 2100/1900 and GSM 850/900/1800/1900.
12/06/03   ap      Modifications made including structure changes... details later
08/08/02   eh      Initial Revision.

===========================================================================*/

/*===========================================================================

                                 INCLUDE FILES

===========================================================================*/
#include "comdef.h"
#include "nv.h"
//#include "customer.h"
//#include "rficap.h"
//#include "target.h"
//#include "rfc.h"

#include "nv_items.h"

#include "rfnv_wcdma.h"
#include "rfnv_cmn.h"
#include "rfnv_gsm.h"
#include "rfnv_cdma.h"
/*===========================================================================

                           DATA TYPE DECLARATIONS

===========================================================================*/

/* EJECT  */
/*===========================================================================

                       PUBLIC DATA DECLARATIONS

===========================================================================*/

/* -----------------------------------------------------------------------
** NV data
** ----------------------------------------------------------------------- */
typedef struct {
  boolean	rfnv_rfcal_valid;                       /* RF CAL Valid?         */
  byte		rfnv_config;                            /* RF Config             */
  rfnv_fm_items_type		*rfnv_fm_items_ptr;     /* Ptr to FM NV items    */
  rfnv_common_items_type    *rfnv_cdma_common_items_ptr;  /* Ptr to CDMA Common NV itmes*/
  rfnv_digital_items_type	*rfnv_cdma_items_ptr;   /* Ptr to CDMA NV items  */
  rfnv_digital_items_type	*rfnv_pcs_items_ptr;    /* Ptr to PCS NV items*/
  rfnv_digital_items_type   *rfnv_gps_items_ptr;     /* Ptr to GPS NV items   */
  rfnv_gsm_nv_tbl_type		*rfnv_gsm_nv_items_ptr; /* Ptr to GSM NV items*/
  rfnv_gsm_cmn_nv_tbl_type	*rfnv_gsm_cmn_nv_items_ptr; /* Ptr to GSM Common NV items*/
  rfnv_gsm_nv_tbl_type		*rfnv_gsm_1800_nv_items_ptr;
  rfnv_gsm_cmn_nv_tbl_type	*rfnv_gsm_1800_cmn_nv_items_ptr;
  rfnv_gsm_nv_tbl_type		*rfnv_gsm_1900_nv_items_ptr;
  rfnv_gsm_cmn_nv_tbl_type	*rfnv_gsm_1900_cmn_nv_items_ptr;
  rfnv_wcdma_nv_tbl_type	*rfnv_wcdma_nv_items_ptr; /* Ptr to WCDMA NV items */
  rfnv_cmn_nv_tbl_type		*rfnv_common_nv_items_ptr; /* Ptr to Common NV items*/
} rfnv_all_data_type;



#define NV_ITEM_ATTRIB_CRTCL_BMSK  0x01  /* bit 0 = critical */
#define NV_ITEM_ATTRIB_NON_CRTCL_BMSK 0x00  /* bit 0 = critical */


typedef enum
{
  RFNV_READ_SUCCESS,
  RFNV_READ_FAILED,
  RFNV_WRITE_SUCCESS,
  RFNV_WRITE_FAILED,
  RFNV_INACTIVE_ITEM
} rfnv_status_type;

typedef struct
{
  /* nv item identifier */
  nv_items_enum_type   nv_item_id;

  /* flag to indicate calibration item */
  byte              nv_item_attrib;

} rfnv_items_list_type;

/* function description for extracting nv item list size              */
typedef word (*rfnv_get_list_size_type)(void);

/* function description for accessing the pointer to the nv structure */
typedef void *(*rfnv_get_nv_item_table_type)(void);

typedef struct
{
  /* pointer to an array of supported NV items for a particular mode  */
  const rfnv_items_list_type *nv_item_list_ptr;

  /* void pointer to the structure with the nv values for the band    */
  rfnv_get_nv_item_table_type get_nv_item_table;

  /* size of the array of supported NV items for a particular mode    */
  rfnv_get_list_size_type get_nv_item_list_size;

  /* flag to indicate that the NV data is valid for that mode         */
  boolean             nv_data_valid_flag;

  /* function pointer to a function to copy NV item from NV item temp
     buffer to the NV table of a particular mode                      */
  void (*copy_nv_item) ( nv_items_enum_type nv_item_id,
                         nv_item_type *nv_item_buf_ptr);

} rfnv_supported_mode_info_type;

typedef enum
{
  RFNV_COMMON = 0,
  RFNV_WCDMA_2100,
  RFNV_WCDMA_1900,
  RFNV_GSM,
  RFNV_GSM_900,
  RFNV_GSM_1800,
  RFNV_GSM_850,
  RFNV_GSM_1900, 
  /* add additional mode enum field here */
  RFNV_SUPPORTED_MODE_NUM
} rfnv_supported_mode_type;

/*===========================================================================
                                                       
                           DATA DECLARATIONS

===========================================================================*/


/*===========================================================================

                         FUNCTION PROTOTYPES

===========================================================================*/
/*===========================================================================

FUNCTION RFNV_SET_NV_ITEM

DESCRIPTION
  This function set a specific NV item to NV.

DEPENDENCIES
  The NV task has been started and is running.

RETURN VALUE
None

SIDE EFFECTS
  While this function is running all other current task activities are
  suspended except for watchdog kicking, and until the NV item is
  written.

===========================================================================*/
//extern void rfnv_set_nv_item(
//  nv_items_enum_type  item_code,       /* Item to set */
//  nv_item_type        *item_ptr,       /* Pointer to the item */
//  rex_tcb_type        *task_ptr,       /* Calling task's TCB pointer */
//  rex_sigs_type       task_wait_sig,   /* Task signal to wait for when
//                                          reading NV */
//  void                (*task_wait_handler)( rex_sigs_type )
//                                       /* Task's wait function to be called
//                                          when reading NV */
//);

/*===========================================================================

FUNCTION rfnv_retrieve_nv_items                              

DESCRIPTION
  This function retrieves NV items for each mode specified in the 
  rfnv_supported_mode_info_table.
  
PARAMETER
  Caller task info. see below. 

DEPENDENCIES
  NV task is started.
  
RETURN VALUE
  status of the NV read process.

SIDE EFFECTS
  It will block the caller task.

===========================================================================*/
//extern rfnv_status_type rfnv_retrieve_nv_items
//(
//  rex_tcb_type  *task_ptr,              /* Calling task's TCB pointer */
//  rex_sigs_type task_wait_sig,          /* Task signal to wait for when
//                                           reading NV */
//  void          (*task_wait_handler)( rex_sigs_type )
//                                        /* Task's wait function to be called
//                                           when reading NV */
//);

/*===========================================================================

FUNCTION rfnv_is_nv_data_valid                              

DESCRIPTION
  This function check whether NV data for a particular mode is valid or not.

PARAMETER
  rfnv_supported_mode_type to indicate which mode to check.
  
DEPENDENCIES
  rfnv_retrieve_nv_items is called first.
  
RETURN VALUE
  boolean

SIDE EFFECTS
  none

===========================================================================*/
//extern boolean rfnv_is_nv_data_valid(rfnv_supported_mode_type mode);

/*===========================================================================

MACRO RFNV_SATURATE

DESCRIPTION
  This macro returns the input (val) limited to the range specified by (min)
  and (max).  This is used for clipping the results of adjustment
  calculations where the newly calculated value may exceed the legal range
  for the control.  Since these calculations are typically closed loop
  integrators, this macro prevents "integrator windup" and/or "control
  overflow/underflow".

PARAMETERS
  val   the value to be limited (saturated) interpreted as a signed integer
  min   the minimum allowed value (e.g. 0)
  max   the maximum allowed value (e.g. 2^n-1)

DEPENDENCIES
  None

RETURN VALUE
  None

SIDE EFFECTS
  None

===========================================================================*/
#define RFNV_SATURATE( val, min, max )  MAX( MIN( (val), (max) ), (min) )

/*===========================================================================

FUNCTION RFNV_GET_NV_TABLE

DESCRIPTION
  This function returns a void pointer to the requested band if the requested
  band is supported. The clients must check returned value to avoid NULL pointer
  access errors.

PARAMETER
  requested mode.

DEPENDENCIES
  rfnv_retrieve_nv_items is called first.

RETURN VALUE
  a void pointer to the requested band.

SIDE EFFECTS
  none

===========================================================================*/
//void  *rfnv_get_nv_table(rfnv_supported_mode_type mode);

#endif /* RFNV_H */

