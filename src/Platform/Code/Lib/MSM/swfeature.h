#ifndef SWFEATURE_H
#define SWFEATURE_H

/*==========================================================================

    S O F T W A R E    F E A T U R E    D E F I N I T I O N S

DESCRIPTION
  This file contains software feature definitions used by function
  diagp_sw_feature in diagp.c

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

when       who     what, where, why
--------   ---     ----------------------------------------------------------
06/26/02   shinn   Create. 

===========================================================================*/

/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include  "comdef.h"
#include  "customer.h"
#include  "target.h"


/*===========================================================================

                            DEFINITIONS

===========================================================================*/

typedef PACKED enum {
    SW_FEATURE_WAP,         /* 1st bit */
    SW_FEATURE_CDMA_800,    /* 2nd bit */
    SW_FEATURE_CDMA_1900,   /* 3rd bit */
    SW_FEATURE_ACP,         /* 4th bit */
	SW_FEATURE_RUIM,		/* 5th bit */
    SW_FEATURE_FLASH_SIZE_LSB,   /* 6th bit */
    SW_FEATURE_FLASH_SIZE_MSB,   /* 7th bit */
    SW_FEATURE_MUTIL_LANG_LSB,   /* 8th bit */
    SW_FEATURE_MUTIL_LANG_MSB,   /* 9th bit */
    SW_FEATURE_DSC,              /*10th bit */
    SW_FEATURE_MAX          /* max value should not exceed 32 */
} sw_feature_enum_type;

/*--------------------------------------------------------------------------
                            FEATURE MASK
--------------------------------------------------------------------------*/
#if defined (FEATURE_PC_TOOLS)

    #define SW_FEATURE_WAP_MASK           (0x1 << SW_FEATURE_WAP)
    #define SW_FEATURE_CDMA_800_MASK      (0x1 << SW_FEATURE_CDMA_800)
    #define SW_FEATURE_CDMA_1900_MASK     (0x1 << SW_FEATURE_CDMA_1900)
    #define SW_FEATURE_ACP_MASK           (0x1 << SW_FEATURE_ACP)
    #define SW_FEATURE_RUIM_MASK          (0x1 << SW_FEATURE_RUIM)
    #define SW_FEATURE_FLASH_SIZE_MASK    (0x3 << SW_FEATURE_FLASH_SIZE_LSB)
    #define SW_FEATURE_MUTIL_LANG_MASK    (0x3 << SW_FEATURE_MUTIL_LANG_LSB)
    #define SW_FEATURE_DSC_MASK           (0x1 << SW_FEATURE_DSC)

#else

    #if defined (FEATURE_WAP) && !defined (FEATURE_WAP_HIDDEN)
        #define SW_FEATURE_WAP_MASK           (0x1 << SW_FEATURE_WAP)
    #else
        #define SW_FEATURE_WAP_MASK           0x0
    #endif

    #if defined (FEATURE_CDMA_800)
        #define SW_FEATURE_CDMA_800_MASK      (0x1 << SW_FEATURE_CDMA_800)
    #else
        #define SW_FEATURE_CDMA_800_MASK      0x0
    #endif

    #if defined ( FEATURE_CDMA_1900)
        #define SW_FEATURE_CDMA_1900_MASK     (0x1 << SW_FEATURE_CDMA_1900)
    #else
        #define SW_FEATURE_CDMA_1900_MASK     0x0
    #endif

    #if defined ( FEATURE_ACP)
        #define SW_FEATURE_ACP_MASK           (0x1 << SW_FEATURE_ACP)
    #else
        #define SW_FEATURE_ACP_MASK           0x0
    #endif

    #if defined (FEATURE_UIM_RUIM)
        #define SW_FEATURE_RUIM_MASK          (0x1 << SW_FEATURE_RUIM)
    #else
        #define SW_FEATURE_RUIM_MASK          0x0
    #endif

    #if defined (FEATURE_64M_FLASH)
        #define SW_FEATURE_FLASH_SIZE_64M     0x0
    #else
        #define SW_FEATURE_FLASH_SIZE_64M     0x0
    #endif

    #if defined (FEATURE_128M_FLASH)
        #define SW_FEATURE_FLASH_SIZE_128M    0x1
    #else
        #define SW_FEATURE_FLASH_SIZE_128M    0x0
    #endif

    #if defined (FEATURE_256M_FLASH)
        #define SW_FEATURE_FLASH_SIZE_256M    0x2
    #else
        #define SW_FEATURE_FLASH_SIZE_256M    0x0
    #endif

    #if defined (FEATURE_512M_FLASH)
        #define SW_FEATURE_FLASH_SIZE_512M    0x3
    #else
        #define SW_FEATURE_FLASH_SIZE_512M    0x0
    #endif

    #define SW_FEATURE_FLASH_SIZE_MASK        (( (SW_FEATURE_FLASH_SIZE_64M & 0x3) \
                                              |(SW_FEATURE_FLASH_SIZE_128M & 0x3) \
                                              |(SW_FEATURE_FLASH_SIZE_256M & 0x3) \
                                              |(SW_FEATURE_FLASH_SIZE_512M & 0x3)) << SW_FEATURE_FLASH_SIZE_LSB)

    #if defined (FEATURE_UI_LANGSET_CHINESES)
        #define SW_FEATURE_LANG_CHINESES      0x1
    #else
        #define SW_FEATURE_LANG_CHINESES      0x0
    #endif

    #if defined (FEATURE_UI_LANGSET_CHINESEC)
        #define SW_FEATURE_LANG_CHINESEC      0x2
    #else
        #define SW_FEATURE_LANG_CHINESEC      0x0
    #endif

    #define SW_FEATURE_MUTIL_LANG_MASK        (( (SW_FEATURE_LANG_CHINESES & 0x3) \
                                              |(SW_FEATURE_LANG_CHINESEC & 0x3)) << SW_FEATURE_MUTIL_LANG_LSB)

    #if defined (FEATURE_DSC) && !defined (FEATURE_UI_DSC_HIDDEN)
        #define SW_FEATURE_DSC_MASK           (0x1 << SW_FEATURE_DSC)
    #else
        #define SW_FEATURE_DSC_MASK           0x0
    #endif

#endif

/*--------------------------------------------------------------------------
                           SOFTWARE FEATURE
--------------------------------------------------------------------------*/

/* Add your new added feature mask here */

#define SOFTWARE_FEATURE_VALUE  \
    ( SW_FEATURE_WAP_MASK | SW_FEATURE_CDMA_800_MASK | SW_FEATURE_CDMA_1900_MASK | \
       SW_FEATURE_ACP_MASK | SW_FEATURE_RUIM_MASK | SW_FEATURE_FLASH_SIZE_MASK | \
       SW_FEATURE_MUTIL_LANG_MASK | SW_FEATURE_DSC_MASK )

/*---------------------------------------------------------------------------
                       NEW SOFTWARE FEATURE
-----------------------------------------------------------------------------*/
/* The shortest length of the mask to cover all entries in the enum */
#define SW_FEATURE_MASK_LENGTH \
  ((SW_FEATURE_QUERY_ENUM_LENGTH / 8)+1)

typedef enum { /* begin feature_query_enum_type */
	    F_WAP_BROWSER_BIT,
		F_UIM_RUIM_BIT,
		F_UI_LANGSET_CHINESES_BIT,
		F_UI_LANGSET_CHINESEC_BIT,
		F_DSC_BIT,
		F_UI_USER_THEME_BIT,
		SW_FEATURE_QUERY_ENUM_LENGTH /* used to determine mask size */    
}sw_feature_query_enum_type;

extern byte diag_sw_feature_mask[SW_FEATURE_MASK_LENGTH];
#endif /* SWFEATURE_H */