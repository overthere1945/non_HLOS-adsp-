/*
* 파일명: sns_himax_dsp_sensor_instance.h
* 목적 및 기능: 실제 하드웨어 인터럽트를 처리하고 공유 메모리에 이미지를 적재하는 인스턴스 영역의 상태 및 구조체를 정의합니다.
*/
#pragma once

#include <stdint.h>
#include "sns_sensor_instance.h"
#include "sns_data_stream.h"
#include "sns_com_port_types.h"
#include "sns_sync_com_port_service.h"
#include "sns_async_com_port.pb.h"
#include "sns_interrupt.pb.h"
#include <qurt.h> // QuRT 메모리 맵핑용 헤더

// 공유 물리 메모리 주소 및 사이즈 매크로
#define SHARED_PHYS_ADDR  0x81EC0000
#define SHARED_SIZE       0x20000 

/* 물리 COM 포트 구조체 */
typedef struct himax_dsp_com_port_info
{
  sns_com_port_config        com_config;
  sns_sync_com_port_handle  *port_handle;
} himax_dsp_com_port_info;

/* 인스턴스 프라이빗 상태 구조체 */
typedef struct himax_dsp_instance_state
{
  /* COM 포트 정보 및 서비스 */
  himax_dsp_com_port_info        com_port_info;
  sns_sync_com_port_service      *scp_service;
  sns_async_com_port_config      ascp_config;

  /* SUID 및 스트림 관리 */
  sns_sensor_uid                 irq_suid;
  sns_data_stream                *interrupt_data_stream;
  sns_data_stream                *async_com_port_data_stream;

  /* 통신 Read Wrapper */
  sns_rc (* com_read)(
      sns_sync_com_port_service *scp_service,
      sns_sync_com_port_handle *port_handle,
      uint32_t rega,
      uint8_t  *regv,
      uint32_t bytes,
      uint32_t *xfer_bytes);
} himax_dsp_instance_state;

extern sns_sensor_instance_api himax_dsp_sensor_instance_api;

void himax_dsp_handle_interrupt_event(sns_sensor_instance *const instance, sns_time timestamp);
