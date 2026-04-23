#ifndef __PM_ERR_FLAGS_H__
#define __PM_ERR_FLAGS_H__
/*! \file
 *  \n
 *  \brief  pm_err_flags.h ---- PMIC library error codes
 *  \n
 *  \n This file is defines error codes returned by PMIC library APIs
 *  \n
 *  \n &copy; Copyright 2017 QUALCOMM Technologies Incorporated, All Rights Reserved
 */
/* ======================================================================= */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/17   akm     New file
=============================================================================*/

/**
 * These are all the error codes returned by PMIC APIs
 *
 * PMIC APIs most commonly return errors if the caller has tried
 * to use parameters that are out of range or if the feature is
 * not supported by the PMIC model detected at run-time.
 */
typedef enum
{
  PM_ERR_FLAG_SUCCESS                  = 0x0000,
  PM_ERR_FLAG__SUCCESS                 = 0x0000,
  PM_ERR_FLAG_FAILURE                  = 0x0001, //0x01<<0
  PM_ERR_FLAG_INVALID_PARAMETER        = 0x0002, //0x01<<1
  PM_ERR_FLAG_FEATURE_NOT_SUPPORTED    = 0x0004, //0x01<<2
  PM_ERR_FLAG_DATA_VERIFY_ERR          = 0x0008, //0x01<<3
  PM_ERR_FLAG_INVALID_CONFIG_SETTING   = 0x0010, //0x01<<4
  PM_ERR_FLAG_INVALID_PERIPHERAL       = 0x0020, //0x01<<5
  PM_ERR_FLAG_PERIPHERAL_ERR           = 0x0040, //0x01<<6
  PM_ERR_FLAG_BUS_ERR                  = 0x0080, //0x01<<7
  PM_ERR_FLAG_OS_ERR                   = 0x0100, //0x01<<8
  PM_ERR_FLAG_INVALID                  = 0x8000, //0x01<<15

} pm_err_flag_type;

#endif

