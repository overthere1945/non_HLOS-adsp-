#ifndef __PM_DEVICE_INDEX_H__
#define __PM_DEVICE_INDEX_H__

/*! \file 
 *  \n
 *  \brief  pm_device_index.h ---- PMIC Device Index  
 *  \n
 *  \n This header file contains enum for PMIC Device Indices on different buses and index (s) 
 *  \n
 *  Copyright (c) Qualcomm Technologies, Inc.  All Rights Reserved. 
 *  Confidential and Proprietary - Qualcomm Technologies, Inc
*/

/* ======================================================================= */

/* =======================================================================
                             Edit History
  This section contains comments describing changes made to this file.
  Notice that changes are listed in reverse chronological order.


when       who       what, where, why
--------   ---       ---------------------------------------------------------- 
07/18/24   dvaddem   Created file

=============================================================================*/

/*===========================================================================

                        HEADER FILES

===========================================================================*/


/*===========================================================================

                        MACROS and ROUTINES 

===========================================================================*/


/*===========================================================================

                        TYPE DEFINITIONS 

===========================================================================*/

/*PMIC Spmi/eSpmi Bus Indices*/
typedef enum
{
  PM_ESPMI_BUSID_0,
  PM_ESPMI_BUSID_1,
  PM_ESPMI_BUSID_2,
  PM_ESPMI_BUSID_3
}pm_spmi_busid_type;

/*PMIC Indices*/
enum
{
  PMIC_A = 0,
  PMIC_B,
  PMIC_C,
  PMIC_D,
  PMIC_E,
  PMIC_F,
  PMIC_G,
  PMIC_H,
  PMIC_I,
  PMIC_J,
  PMIC_K,
  PMIC_L,
  PMIC_M,
  PMIC_N,
  PMIC_O,

  /*PMIC on Bus 0*/
  PMIC_A_E0 = (PM_ESPMI_BUSID_0 << 16 | PMIC_A),
  PMIC_B_E0,
  PMIC_C_E0,
  PMIC_D_E0,
  PMIC_E_E0,
  PMIC_F_E0,
  PMIC_G_E0,
  PMIC_H_E0,
  PMIC_I_E0,
  PMIC_J_E0,
  PMIC_K_E0,
  PMIC_L_E0,
  PMIC_M_E0,
  PMIC_N_E0,
  PMIC_O_E0,
  
  /*PMIC on Bus 1*/
  PMIC_A_E1 = (PM_ESPMI_BUSID_1 << 16 | PMIC_A),
  PMIC_B_E1,
  PMIC_C_E1,
  PMIC_D_E1,
  PMIC_E_E1,
  PMIC_F_E1,
  PMIC_G_E1,
  PMIC_H_E1,
  PMIC_I_E1,
  PMIC_J_E1,
  PMIC_K_E1,
  PMIC_L_E1,
  PMIC_M_E1,
  PMIC_N_E1,
  PMIC_O_E1,
  
  /*PMIC on Bus 2*/
  PMIC_A_E2 = (PM_ESPMI_BUSID_2 << 16 | PMIC_A),
  PMIC_B_E2,
  PMIC_C_E2,
  PMIC_D_E2,
  PMIC_E_E2,
  PMIC_F_E2,
  PMIC_G_E2,
  PMIC_H_E2,
  PMIC_I_E2,
  PMIC_J_E2,
  PMIC_K_E2,
  PMIC_L_E2,
  PMIC_M_E2,
  PMIC_N_E2,
  PMIC_O_E2,
  
  /*PMIC on Bus 3*/
  PMIC_A_E3 = (PM_ESPMI_BUSID_3 << 16 | PMIC_A),
  PMIC_B_E3,
  PMIC_C_E3,
  PMIC_D_E3,
  PMIC_E_E3,
  PMIC_F_E3,
  PMIC_G_E3,
  PMIC_H_E3,
  PMIC_I_E3,
  PMIC_J_E3,
  PMIC_K_E3,
  PMIC_L_E3,
  PMIC_M_E3,
  PMIC_N_E3,
  PMIC_O_E3,
};

#endif // __PM_DEVICE_INDEX_H__
