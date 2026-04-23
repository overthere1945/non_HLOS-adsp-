#ifndef __PM_RESOURCES_AND_TYPES_H__
#define __PM_RESOURCES_AND_TYPES_H__
/** @file pm_resources_and_types.h
*/
/* This file contains enumerations with resource names for different
 *  module types that should be used by external clients when accessing
 *   the resources that are required
 
 Copyright (c) 2010-2021, 2023 Qualcomm Technologies, Inc.
 All Rights Reserved.
 Confidential and Proprietary - Qualcomm Technologies, Inc.
 */
/* ======================================================================= */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/core.qdsp6/8.2.3/api/common/pmic/pm/pm_resources_and_types.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
09/25/12   hw      moving some npa type to this file so we don't need to include pmapp_npa.h
09/29/11   hs      Added CLK resource IDs.
09/14/11   dy      Move UICC resource IDs to this file
04/25/11   wra     Adding Battery Temperature Management BTM, LPG, more LVS, ADC_ARB,
                    OVP & INTerrupt channel enumeration needed for PM8921
02/08/11   hw      Merging changes from the PMIC Distributed Driver Arch branch
07/05/10   wra     Modified Header includes to accomodate MSM8660
=============================================================================*/
/*===========================================================================

                        HEADER FILES

===========================================================================*/
#include <stdint.h>
#include <stdbool.h>

/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

/** @addtogroup pm_resources_and_types
@{ */
/**
 * PMIC power module voltage level
 */
typedef uint32_t pm_volt_level_type;

typedef enum
{
   PM_OFF,
   PM_ON,
   PM_INVALID
}pm_on_off_type;

/** SMPS peripheral index. This enum type contains all required LDO
    regulators. */
enum
{
  PM_SMPS_1,   /**< SMPS 1. */
  PM_SMPS_2,   /**< SMPS 2. */
  PM_SMPS_3,   /**< SMPS 3. */
  PM_SMPS_4,   /**< SMPS 4. */
  PM_SMPS_5,   /**< SMPS 5. */    
  PM_SMPS_6,   /**< SMPS 6. */
  PM_SMPS_7,   /**< SMPS 7. */
  PM_SMPS_8,   /**< SMPS 8. */
  PM_SMPS_9,   /**< SMPS 9. */
  PM_SMPS_10,  /**< SMPS 10. */ 
  PM_SMPS_11,  /**< SMPS 11. */ 
  PM_SMPS_12,  /**< SMPS 12. */ 
  PM_SMPS_INVALID
};

/**
 *  Voltage switch peripheral index. This enum type contains all required
 *  VS regulators. 
 */
enum
{
  PM_VS_LVS_1,
  PM_VS_LVS_2,
  PM_VS_LVS_3,
  PM_VS_LVS_4,
  PM_VS_LVS_5,
  PM_VS_LVS_6,
  PM_VS_LVS_7,
  PM_VS_LVS_8,
  PM_VS_LVS_9,
  PM_VS_LVS_10,
  PM_VS_LVS_11,
  PM_VS_LVS_12,
  PM_VS_MVS_1,
  PM_VS_OTG_1,
  PM_VS_HDMI_1,
  PM_VS_INVALID
};

/**
 * LDO peripheral index. This enum type contains all LDO regulators that are
 * required. 
 */
enum
{
  PM_LDO_1,       /**< LDO 1. */
  PM_LDO_2,       /**< LDO 2. */
  PM_LDO_3,       /**< LDO 3. */
  PM_LDO_4,       /**< LDO 4. */
  PM_LDO_5,       /**< LDO 5. */
  PM_LDO_6,       /**< LDO 6. */
  PM_LDO_7,       /**< LDO 7. */
  PM_LDO_8,       /**< LDO 8. */
  PM_LDO_9,       /**< LDO 9. */
  PM_LDO_10,      /**< LDO 10. */
  PM_LDO_11,      /**< LDO 11. */
  PM_LDO_12,      /**< LDO 12. */
  PM_LDO_13,      /**< LDO 13. */
  PM_LDO_14,      /**< LDO 14. */
  PM_LDO_15,      /**< LDO 15. */
  PM_LDO_16,      /**< LDO 16. */
  PM_LDO_17,      /**< LDO 17. */
  PM_LDO_18,      /**< LDO 18. */
  PM_LDO_19,      /**< LDO 19. */
  PM_LDO_20,      /**< LDO 20. */
  PM_LDO_21,      /**< LDO 21. */
  PM_LDO_22,      /**< LDO 22. */
  PM_LDO_23,      /**< LDO 23. */
  PM_LDO_24,      /**< LDO 24. */
  PM_LDO_25,      /**< LDO 25. */
  PM_LDO_26,      /**< LDO 26. */
  PM_LDO_27,      /**< LDO 27. */
  PM_LDO_28,      /**< LDO 28. */
  PM_LDO_29,      /**< LDO 29. */
  PM_LDO_30,      /**< LDO 30. */
  PM_LDO_31,      /**< LDO 31. */
  PM_LDO_32,      /**< LDO 32. */
  PM_LDO_33,      /**< LDO 33. */
  PM_LDO_INVALID  /**< Invalid LDO. */
};

 /** Definitions of the CLK resources in the target.
 */
typedef enum
{
  PM_CLK_SLEEP,
  PM_CLK_XO,
  PM_CLK_1,  
  PM_CLK_2,  
  PM_CLK_3,  
  PM_CLK_4,  
  PM_CLK_5,  
  PM_CLK_6,  
  PM_CLK_7,  
  PM_CLK_8,  
  PM_CLK_9,  
  PM_CLK_10, 
  PM_CLK_11, 
  PM_CLK_12,
  PM_CLK_13,
  PM_CLK_DIST,
  PM_CLK_MAX_INDEX,
  PM_CLK_TYPE_INVALID = PM_CLK_MAX_INDEX,
  PM_CLK_INVALID = PM_CLK_MAX_INDEX    /**< Invalid clock. */
}pm_clk_type;

/*!
 *  \brief 
 *  BOB peripheral index. This enum type contains all bob regulators that you may need. 
 */
enum
{
  PM_BOB_1,
  PM_BOB_2,
  PM_BOB_INVALID
};

/*!
 *  \brief 
 *  Level shifter peripheral index. This enum type contains all bob regulators that you may need. 
 */
enum
{
  PM_LS_1,
  PM_LS_2,
  PM_LS_3,
  PM_LS_4,
  PM_LS_INVALID
};

/* Values of modes in enum reflect  
 * register values in target
 */
typedef enum 
{
   PM_SW_MODE_LPM,             /**< 0 : Low Power mode                  */
   PM_SW_MODE_BYPASS,          /**< 1 : Bypass mode         (LDO only ) */
   PM_SW_MODE_AUTO,            /**< 2 : Auto Mode           (SMPS Only) */
   PM_SW_MODE_NPM,             /**< 3 : Normal Power mode               */
   PM_SW_MODE_RETENTION,       /**< 4 : Retention mode.                 */
   PM_SW_MODE_OPM,			   /**< 5 : OPM Mode supported from LDO530 type.*/
   PM_SW_MODE_INVALID
}pm_sw_mode_type;

/** 
  @struct peripheral_info_type
  @brief Contains Peripheral Information such as Type, Subtype, 
         Analog and Digital Major Versions. 
 */
typedef struct 
{
    uint16_t base_address;
    uint8_t  peripheral_type;
    uint8_t  peripheral_subtype;
    uint8_t  analog_major_version;
    uint8_t  analog_minor_version;
    uint8_t  digital_major_version;
    uint8_t  digital_minor_version;
}peripheral_info_type;

/**
 * Regulator voltage information.
 */
typedef struct
{
    uint32_t    RangeMin; /**< Minimum voltage range supported. */
    uint32_t    RangeMax; /**< Maximum voltage range supported. */
    uint32_t    VStep;	/**< Step size in uV. */	
}pm_pwr_volt_info_type;

/** @} */ /* end_addtogroup pm_resources_and_types */

#endif /* PM_RESOURCES_AND_TYPES__H */

