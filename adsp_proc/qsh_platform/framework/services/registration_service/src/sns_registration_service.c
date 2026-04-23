/*============================================================================
  @file sns_registration_service.c

  @brief
  Registration service provides functionality to sensors to register new
  sensors within the same library

  @copyright Copyright (c) Qualcomm Technologies, Inc. and/or its subsidiaries.
  All Rights Reserved.
  Confidential and Proprietary - Qualcomm Technologies, Inc.

  ============================================================================*/

/*=============================================================================
  Include Files
  ===========================================================================*/
#include "sns_fw_sensor.h"
#include "sns_island.h"
#include "sns_registration_service.h"

/*=============================================================================
  Static Data Definitions
  ===========================================================================*/
static sns_registration_service registration_service SNS_SECTION(".data.sns");
static const uint16_t
    sns_registration_service_version SNS_SECTION(".rodata.sns") = 1;

static sns_registration_service_api
    registration_service_api SNS_SECTION(".data.sns") = {
        .struct_len = sizeof(sns_registration_service_api),
        .sns_sensor_create = &sns_sensor_create,
};

/*=============================================================================
  Public Function Definitions
  ===========================================================================*/

sns_registration_service *sns_registration_service_init(void)
{
  registration_service.api = &registration_service_api;
  registration_service.service.type = SNS_REGISTRATION_SERVICE;
  registration_service.service.version = sns_registration_service_version;
  return &registration_service;
}
