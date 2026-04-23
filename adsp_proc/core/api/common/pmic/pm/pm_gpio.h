#ifndef __PM_GPIO_H__
#define __PM_GPIO_H__

/*! \file  pm_gpio.h
*  \n
 *  \brief  PMIC-GPIO MODULE RELATED DECLARATION
 *  \details  This file contains functions and variable declarations to support 
*   the PMIC GPIO module.
*
*
*  \n Copyright 2010-2023 QUALCOMM Technologies Incorporated, All Rights Reserved
*/

/*===========================================================================

EDIT HISTORY FOR MODULE

This section contains comments describing changes made to the module.
Notice that changes are listed in reverse chronological order.

$Header: //components/dev/core.qdsp6/8.2/shirish.core.qdsp6.8.2.channel_rename_adc/pmic/inc/pm_gpio_p.h#1 $

when       who     what, where, why
--------   ---     ---------------------------------------------------------- 

===========================================================================*/
#include "pm_err_flags.h"
#include <stdint.h>
#include <stdbool.h>
#include "pm_gpio_types_external.h"
/** @addtogroup pm_gpio
@{ */

/*===========================================================================*/
/*                                  NOTES                                    */
/*===========================================================================*/
/**
Commonn usage:
Input GPIO:
  1) pm_gpio_cfg_mode: use this API to configure a GPIO as input
  2) pm_gpio_enable: use this API to enable this GPIO peripheral
  3) pm_gpio_input_level_status: Use this API to read the status (high/low)

Output GPIO:
  1) pm_gpio_cfg_mode: use this API to configure a GPIO as output
  2) pm_gpio_enable: use this API to enable this GPIO peripheral
  3) pm_gpio_set_output_level: Set output level (high/low)

Usually PMIC system team will aware of configures like pull current, input source,
output buf, etc. and configure these settings in PSI. But cusomters design may
be different with reference design so they may need to configure those settings
themselves. In this case, they can follow below guide.

Following api's can be used to configure GPIO as Input
  - To configure the mode of GPIO to Input
    pm_gpio_cfg_mode 
    
  - To configure Pull Up or Pull Down 
    pm_gpio_set_cur_src_pull
  
  - To configure input voltage levels for GPIO
    pm_gpio_set_voltage_source

  - To enable GPIO, since gpio's are not enabled by default
    they should be enabled after configuration 
    pm_gpio_enable 
    
  - To get the status of input level high or low 
    pm_gpio_input_level_status


    
Following api's can be used to configure GPIO as Output
  
  - To configure mode of GPIO to output
    pm_gpio_cfg_mode
    
    - To configure Output buffer type eg. CMOS, Open Drain High, Open Drain Low
    pm_gpio_set_out_buf_cfg
    
  - To configure drive strength of GPIO 
    pm_gpio_set_out_buf_drv_str
    
  - To configure voltage levels for GPIO
    pm_gpio_set_voltage_source
    
  - To drive GPIO level high or low 
    pm_gpio_set_output_level
  
  - To enable GPIO, since gpio's are not enabled by default
    they should be enabled after configuration 
    pm_gpio_enable
  
  -  To check the configuration of output level 
    pm_gpio_input_level_status
  
  - To check configuration of various parameters 
    pm_gpio_reg_status_get
    
*/
/*=========================================================================== */
/*                     FUNCTION DEFINITIONS                                   */   
/*=========================================================================== */

/*=========================================================================== */
/*                        pm_gpio_enable                                      */
/*=========================================================================== */

/** 
*  Enables GPIO.
* 
* @param[in] pmic_chip Each PMIC device in the system is enumerated
*                              starting with zero.
* @param[in] gpio GPIO for which to configure voltage source. See
*                 #pm_gpio_perph_index.
* @param[in] enable Set true to enable GPIO, false to disable.
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
*
*  <b>Example </b> \n
*  Enable GPIO5:
* @code
*  errFlag = pm_gpio_enable(0, PM_GPIO_5, true); @endcode
*/
pm_err_flag_type 
pm_gpio_enable(uint32_t pmic_chip, pm_gpio_perph_index  gpio, bool enable);


/*=========================================================================== */
/*                        pm_gpio_cfg_mode                                    */
/*=========================================================================== */

/** 
*  Set GPIO Configuration to Input, Output, InOut.
* 
* @param[in] pmic_chip Each PMIC device in the system is enumerated
*                              starting with zero.
* @param[in] gpio GPIO for which to configure voltage source. See
*                 #pm_gpio_perph_index.
* @param[in] type Configuration type. See pm_gpio_config_type.
*
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
*
*  <b>Example </b> \n
*  Configure GPIO5 to Output:
* @code
*  errFlag = pm_gpio_cfg_in_out(0, PM_GPIO_5, PM_GPIO_DIG_OUT); @endcode
*/
pm_err_flag_type
pm_gpio_cfg_mode(uint32_t                      pmic_chip, 
                        pm_gpio_perph_index  gpio, 
                        pm_gpio_config_type  type);

/*=========================================================================== */
/*                        pm_gpio_set_output_level()                          */
/*=========================================================================== */

/**
*  Sets level for output.
*
* @param[in] pmic_chip Each PMIC device in the systems is enumerated
*                      starting with zero.
* @param[in] gpio      GPIO for which to set the inversion configuration. 
*                      See #pm_gpio_perph_index.
* @param[in] level     Output level to be set for GPIO 
*                      see #pm_gpio_output_level_type
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
* 
*  <b>Example </b> \n
*  For  GPIO5, set output high:
* @code
*  errFlag = pm_gpio_set_output_level(0, PM_GPIO_5, 
                                      PM_GPIO_OUTPUT_LEVEL_HIGH); @endcode
*/
pm_err_flag_type 
pm_gpio_set_output_level(uint32_t                         pmic_chip, 
                                    pm_gpio_perph_index gpio, 
                                    pm_gpio_level_type  level);



/*=========================================================================== */
/*                        pm_gpio_set_out_buf_drv_str                         */
/*=========================================================================== */

/**
*  Sets the output buffer drive strength.
*
* @param[in] pmic_chip Each PMIC device in the systems is enumerated
*                              starting with zero.
* @param[in] gpio GPIO for which to configure current source pulls. See
*                 #pm_gpio_perph_index.
* @param[in] out_buffer_strength GPIO output buffer drive strength.
*                                #See #pm_gpio_out_buf_drv_str_type.
*
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
* 
*  <b>Example </b> \n
*  Set the output buffer drive strength for GPIO5 to HIGH:
* @code
*  errFlag = pm_gpio_set_out_buf_drv_str(0, PM_GPIO_5, 
*                                        PM_GPIO_OUT_BUFFER_HIGH); @endcode
*/
pm_err_flag_type 
pm_gpio_set_out_drv_str(uint32_t                              pmic_chip,
                                  pm_gpio_perph_index       gpio, 
                                  pm_gpio_out_drv_str_type  out_drv_str);


/*=========================================================================== */
/*                           pm_gpio_set_pull_sel()                           */
/*=========================================================================== */

/**
 * Set pull type for a GPIO peripheral.
 * @param[in] pmic_chip Each PMIC device in the systems is enumerated
 *                              starting with zero.
 * @param[in] gpio GPIO for which to configure the EXT_PIN.
 *                 See #pm_gpio_which_type.
 * @param[in] pull The pull configure want to set for this GPIO.
 *                 See #pm_gpio_pull_sel_type
 *
 */
pm_err_flag_type
pm_gpio_set_pull_sel( uint32_t  pmic_chip,
                      pm_gpio_perph_index gpio,
                      pm_gpio_pull_sel_type pull_type,
                      uint32_t  pull_strength,
                      bool boost
                    );


/*=========================================================================== */
/*                        pm_gpio_set_voltage_source                          */
/*=========================================================================== */

/** 
*  Sets the voltage source.
* 
* @param[in] pmic_chip Each PMIC device in the systems is enumerated
*                              starting with zero.
* @param[in] gpio GPIO for which to configure voltage source. See
*                 #pm_gpio_perph_index.
* @param[in] voltage_source GPIO voltage source. See
*                           #pm_gpio_voltage_source_type.
*
* @return
*  SUCCESS or Error -- See #pm_err_flag_type.
*
*  <b>Example </b> \n
*  Set voltage source to VIN2 for GPIO5:
* @code
*  errFlag = pm_gpio_set_voltage_source(0, PM_GPIO_5, 
*                                       PM_GPIO_VIN2); @endcode
*/
pm_err_flag_type 
pm_gpio_set_voltage_source(uint32_t                             pmic_chip, 
                                       pm_gpio_perph_index    gpio, 
                                       pm_gpio_volt_src_type  voltage_source);

/** @} */ /* end_addtogroup pm_gpio */

#endif /* __PM_GPIO_H__ */
