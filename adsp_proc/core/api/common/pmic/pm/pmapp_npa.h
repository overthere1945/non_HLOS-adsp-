#ifndef __PMAPP_NPA_H__
#define __PMAPP_NPA_H__
/*===========================================================================


                  P M I C    N P A    H E A D E R    F I L E

DESCRIPTION
  This file contains prototype definitions to support interaction
  with the QUALCOMM Power Management ICs using NPA framework.

Copyright (c) 2017-2018 by Qualcomm Technologies, Inc.  All Rights Reserved.
===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

This section contains comments describing changes made to this file.
Notice that changes are listed in reverse chronological order.

$Header: //components/rel/core.qdsp6/8.2.3/api/common/pmic/pm/pmapp_npa.h#1 $

when       who     what, where, why
--------   ---     ----------------------------------------------------------
01/10/17   akm     New File
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

/*
 * Generic mode IDs that can be used for clients that only use ON / OFF or
 * ACTIVE / STANDBY states.
 */
#define PMIC_NPA_GROUP_ID_SENSOR_VDD        "/pmic/client/sensor_vdd"
#define PMIC_NPA_GROUP_ID_SENSOR_VDD_2      "/pmic/client/sensor_vdd_2"
#define PMIC_NPA_GROUP_ID_SENSOR_VDDIO      "/pmic/client/sensor_vddio"
#define PMIC_NPA_GROUP_ID_SENSOR_VDDIO_2    "/pmic/client/sensor_vddio_2"
#define PMIC_NPA_GROUP_ID_TOUCH_SCREEN      "/pmic/client/touch_screen"
#define PMIC_NPA_GROUP_ID_XO                "/pmic/client/xo"

/* ADSP/LPASS clients */
#define PMIC_NPA_GROUP_ID_PCIE           "/pmic/client/pcie"
#define PMIC_NPA_GROUP_ID_USB_HS1        "/pmic/client/usb_hs1"

#define PMIC_NPA_GROUP_ID_WCN_1P2_RFA          "/pmic/client/wcn_1p2_rfa"
#define PMIC_NPA_GROUP_ID_WCN_WCSS             "/pmic/client/wcn_wcss"
#define PMIC_NPA_GROUP_ID_WCN_1P7_RFA          "/pmic/client/wcn_1p7_rfa"
#define PMIC_NPA_GROUP_ID_WCN_2P2_PA           "/pmic/client/wcn_2p2_pa"
#define PMIC_NPA_GROUP_ID_WCN_ANT_SWITCH       "/pmic/client/wcn_ant_switch"

/* PON/POFF GPIO clients for Aurora WPSS */
#define PMIC_NPA_GROUP_ID_POFF_GPIO           "/pmic/client/poff_gpio"
#define PMIC_NPA_GROUP_ID_PON_GPIO            "/pmic/client/pon_gpio"

/*===========================================================================

                        ENUMERATION

===========================================================================*/
/**
Vote                                  PMIC XO  PMIC XO Buffer  SOC CXO PAD
PMIC_NPA_MODE_ID_CLK_CXO_XO_OFF       OFF      OFF             OFF
PMIC_NPA_MODE_ID_CLK_CXO_BUFFER_OFF    ON      OFF             OFF
PMIC_NPA_MODE_ID_CLK_SOC_CXO_PAD_OFF   ON       ON             OFF
PMIC_NPA_MODE_ID_CLK_CXO_XO_ON         ON       ON              ON
*/
enum
{
   PMIC_NPA_MODE_ID_CLK_CXO_XO_OFF      = 0,
   PMIC_NPA_MODE_ID_CLK_CXO_BUFFER_OFF  = 1,
   PMIC_NPA_MODE_ID_CLK_SOC_CXO_PAD_OFF = 2,
   PMIC_NPA_MODE_ID_CLK_CXO_XO_ON       = 3,
   PMIC_NPA_MODE_ID_XO_MAX              = 4,
};

enum
{
    PMIC_NPA_MODE_ID_GENERIC_OFF = 0,
    PMIC_NPA_MODE_ID_GENERIC_STANDBY = 1,
    PMIC_NPA_MODE_ID_GENERIC_ACTIVE = 2,
    PMIC_NPA_MODE_ID_GENERIC_LV = PMIC_NPA_MODE_ID_GENERIC_ACTIVE,
    PMIC_NPA_MODE_ID_GENERIC_MV = 3,
    PMIC_NPA_MODE_ID_GENERIC_HV = 4,
    PMIC_NPA_MODE_ID_GENERIC_MAX,
};

enum
{
   PMIC_NPA_MODE_ID_SENSOR_POWER_OFF = 0,
   PMIC_NPA_MODE_ID_SENSOR_LPM       = 1,
   PMIC_NPA_MODE_ID_SENSOR_POWER_ON  = 2,
   PMIC_NPA_MODE_ID_SENSOR_MAX       = 3,
};

#endif // PMAPP_NPA_H

