#ifndef __PM_GPIO_TYPES_EXTERNAL_H__
#define __PM_GPIO_TYPES_EXTERNAL_H__

/** @file pm_gpio.h
*/

/*===========================================================================

           P M I C   G P I O   T Y P E S   H E A D E R   F I L E

DESCRIPTION
  This file contains variable/type/constant
  declarations for the GPIO services developed for the Qualcomm Power
  Management IC.

  Copyright (c) 2023 Qualcomm Technologies, Inc.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: #1 $
$DateTime: 2023/04/20 13:54:26 $

===========================================================================*/

/** Type definition for different GPIOs
*/
typedef enum
{
    PM_GPIO_1,     /**< GPIO 1. */
    PM_GPIO_2,     /**< GPIO 2. */
    PM_GPIO_3,     /**< GPIO 3. */
    PM_GPIO_4,     /**< GPIO 4. */
    PM_GPIO_5,     /**< GPIO 5. */
    PM_GPIO_6,     /**< GPIO 6. */
    PM_GPIO_7,     /**< GPIO 7. */
    PM_GPIO_8,     /**< GPIO 8. */
    PM_GPIO_9,     /**< GPIO 9. */
    PM_GPIO_10,    /**< GPIO 10. */
    PM_GPIO_11,    /**< GPIO 11. */
    PM_GPIO_12,    /**< GPIO 12. */
    PM_GPIO_13,    /**< GPIO 13. */
    PM_GPIO_14,    /**< GPIO 14. */
    PM_GPIO_15,    /**< GPIO 15. */
    PM_GPIO_16,    /**< GPIO 16. */
    PM_GPIO_17,    /**< GPIO 17. */
    PM_GPIO_18,    /**< GPIO 18. */
    PM_GPIO_19,    /**< GPIO 19. */
    PM_GPIO_20,    /**< GPIO 20. */
    PM_GPIO_21,    /**< GPIO 21. */
    PM_GPIO_22,    /**< GPIO 22. */
    PM_GPIO_23,    /**< GPIO 23. */
    PM_GPIO_24,    /**< GPIO 24. */
    PM_GPIO_25,    /**< GPIO 25. */
    PM_GPIO_26,    /**< GPIO 26. */
    PM_GPIO_27,    /**< GPIO 27. */
    PM_GPIO_28,    /**< GPIO 28. */
    PM_GPIO_29,    /**< GPIO 29. */
    PM_GPIO_30,    /**< GPIO 30. */
    PM_GPIO_31,    /**< GPIO 31. */
    PM_GPIO_32,    /**< GPIO 32. */
    PM_GPIO_33,    /**< GPIO 33. */
    PM_GPIO_34,    /**< GPIO 34. */
    PM_GPIO_35,    /**< GPIO 35. */
    PM_GPIO_36,    /**< GPIO 36. */
    PM_GPIO_37,    /**< GPIO 37. */
    PM_GPIO_38,    /**< GPIO 38. */
    PM_GPIO_39,    /**< GPIO 39. */
    PM_GPIO_40,    /**< GPIO 40. */
    PM_GPIO_41,    /**< GPIO 41. */
    PM_GPIO_42,    /**< GPIO 42. */
    PM_GPIO_43,    /**< GPIO 43. */
    PM_GPIO_44,     /**< GPIO 44. */ 
    PM_GPIO_INVALID
}pm_gpio_perph_index;


/** Select GPIO I/O type
*/
typedef enum
{
    PM_GPIO_DIG_IN,
    PM_GPIO_DIG_OUT,
    PM_GPIO_DIG_IN_OUT,
    PM_GPIO_ANA_PASS_THRU,
    PM_GPIO_TYPE_INVALID
} pm_gpio_config_type;

/** Select voltage source.
*/
typedef enum
{
    PM_GPIO_VIN0,  /**< Voltage input 0. */
    PM_GPIO_VIN1,  /**< Voltage input 1. */
    PM_GPIO_VIN2,  /**< Voltage input 2. */
    PM_GPIO_VIN_INVALID
}pm_gpio_volt_src_type;


/** Input/Output level Status.
*/
typedef enum
{
    PM_GPIO_LEVEL_LOW,             /**< Level of pin is low. */
    PM_GPIO_LEVEL_HIGH,            /**< Level of pin is high.  */
    PM_GPIO_LEVEL_HIGH_Z,          /**< Level of pin is high-z. */
    PM_GPIO_LEVEL_INVALID
}pm_gpio_level_type;


/** Select output buffer configuration.
*/
typedef enum
{
    PM_GPIO_OUT_BUF_CFG_CMOS,             /**< CMOS output. */
    PM_GPIO_OUT_BUF_CFG_OPEN_DRAIN_NMOS,  /**< Open drain NMOS output. */
    PM_GPIO_OUT_BUF_CFG_OPEN_DRAIN_PMOS,  /**< Open drain PMOS output. */
    PM_GPIO_OUT_BUF_CFG_INVALID
}pm_gpio_out_buf_cfg_type;


typedef enum
{
    PM_GPIO_OUT_DRV_STR_LOW,       /**< Output buffer strength low. */
    PM_GPIO_OUT_DRV_STR_MEDIUM,    /**< Output buffer strength medium. */
    PM_GPIO_OUT_DRV_STR_HIGH,      /**< Output buffer strength high. */
    PM_GPIO_OUT_DRV_STR_INVALID
}pm_gpio_out_drv_str_type;


/** Select current source pulls type.
*/
typedef enum
{
    PM_GPIO_I_SRC_PULL_UP_30uA,                   /**< Pull up 30 uA. */
    PM_GPIO_I_SRC_PULL_UP_1_5uA,                  /**< Pull up 1.5 uA. */
    PM_GPIO_I_SRC_PULL_UP_31_5uA,                 /**< Pull up 31.5 uA. */
    PM_GPIO_I_SRC_PULL_UP_1_5uA_PLUS_30uA_BOOST,  /**< Pull up 1.5 uA plus 30 uA boost. */
    PM_GPIO_I_SRC_PULL_DOWN_10uA,                 /**< Pull down 10 uA. */
    PM_GPIO_I_SRC_PULL_NO_PULL,                   /**< No pull. */
    PM_GPIO_I_SRC_PULL_INVALID
}pm_gpio_i_src_pull_type;

/** GPIO source select.
*/
typedef enum
{
    PM_GPIO_SRC_GND,                  /**< Ground. */
    PM_GPIO_SRC_PAIRED_GPIO,          /**< Paired GPIO. */
    PM_GPIO_SRC_SPECIAL_FUNCTION1,    /**< Special function 1. */
    PM_GPIO_SRC_SPECIAL_FUNCTION2,    /**< Special function 2. */
    PM_GPIO_SRC_SPECIAL_FUNCTION3,    /**< Special function 3. */
    PM_GPIO_SRC_SPECIAL_FUNCTION4,    /**< Special function 4. */
    PM_GPIO_SRC_DTEST1,               /**< D-test 1. */
    PM_GPIO_SRC_DTEST2,               /**< D-test 2. */
    PM_GPIO_SRC_DTEST3,               /**< D-test 3. */
    PM_GPIO_SRC_DTEST4,               /**< D-test 4. */
    PM_GPIO_SRC_INVALID
}pm_gpio_src_cfg_type;

// /** D-test buffer enable/disable.
// */
typedef enum
{
    PM_GPIO_DTEST_DISABLE,  /**< GPIO D-test disable. */
    PM_GPIO_DTEST_ENABLE,   /**< GPIO D-test enable. */
    PM_GPIO_DTEST_INVALID
}pm_gpio_dtest_buf_en_type;

/** External pin configuration.
*/
typedef enum
{
  PM_GPIO_EXT_PIN_DISABLE,  /**< Disable EXT_PIN. */
  PM_GPIO_EXT_PIN_ENABLE,   /**< Puts EXT_PIN at high Z state and disablesthe block. */
  PM_GPIO_EXT_PIN_CONFIG_TYPE__INVALID    
}pm_gpio_ext_pin_config_type;
 
typedef enum
{
  PM_GPIO_PULL_UP_30UA,               // 30uA constant pull up
  PM_GPIO_PULL_UP_1P5UA,              // 1.5uA constant pull up
  PM_GPIO_PULL_UP_31P5UA,             // 31.5uA constant pull up
  PM_GPIO_PULL_UP_1P5_AND_30UA_BOOST, // 1.5uA constant pull up combined with 30uA boost
  PM_GPIO_PULL_DOWN_10UA,             // 10uA contant pull down
  PM_GPIO_PULL_NO_PULL,               // No pull
  PM_GPIO_PULL_UP,
  PM_GPIO_PULL_DOWN,
  PM_GPIO_NO_PULL,
  PM_GPIO_PULL_INVALID,
} pm_gpio_pull_sel_type;

#endif