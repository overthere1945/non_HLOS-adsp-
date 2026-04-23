#ifndef __PM_PRM_H__
#define __PM_PRM_H__
/*===========================================================================


                  P M I C    P R M    H E A D E R    F I L E

DESCRIPTION
  This file contains prototype definitions to support interaction
  with the QUALCOMM Power Management ICs using NPA framework.

Copyright (c) 2024 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
010/17/24   vk     New File
===========================================================================*/

/*===========================================================================

                        PMIC INCLUDE FILES

===========================================================================*/

/*===========================================================================

                        TYPE DEFINITIONS

===========================================================================*/


/*===========================================================================

                        DEFINITIONS

===========================================================================*/

/*! 
 * Generic enable,
 */
typedef enum
{
  PM_PRM_DISABLE = 0, // default
  PM_PRM_ENABLE  = 1,
  PM_PRM_INVALID
}pm_prm_enable_type;

/*! 
 * SW MODE,
 */
typedef enum
{   
  /*PMIC 5 Modes*/
  PM_PRM_VREG_MODE_NONE       = 0,
  PM_PRM_VREG_MODE_BYPASS     = 2, 
  PM_PRM_VREG_MODE_RETENTION  = 3,
  PM_PRM_VREG_MODE_LPM        = 4,
  PM_PRM_VREG_MODE_OPM        = 5,
  PM_PRM_VREG_MODE_AUTO       = 6,
  PM_PRM_VREG_MODE_NPM        = 7,
  PM_PRM_VREG_MODE_INVALID
}pm_prm_vreg_mode_type;


#endif // PMAPP_NPA_H

