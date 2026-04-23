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

static sns_rc
sensor_create_stub(const sns_sensor *sensor, uint32_t state_len,
                   struct sns_sensor_api const *sensor_api,
                   struct sns_sensor_instance_api const *instance_api)
{
  UNUSED_VAR(sensor);
  UNUSED_VAR(state_len);
  UNUSED_VAR(sensor_api);
  UNUSED_VAR(instance_api);

  return SNS_RC_FAILED;
}

static sns_registration_service_api
    registration_service_api SNS_SECTION(".data.sns") = {
        .struct_len = sizeof(sns_registration_service_api),
        .sns_sensor_create = &sensor_create_stub,
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
