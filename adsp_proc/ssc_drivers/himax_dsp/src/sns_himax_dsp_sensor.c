/*
* 파일명: sns_himax_dsp_sensor.c
* 목적 및 기능: Himax DSP 센서의 라이프사이클(초기화/해제)을 관리하고, SUID 획득 및 클라이언트 요청을 인스턴스로 라우팅합니다.
* 온도 센서의 모든 보정/스케일링 로직을 완벽히 제거하였습니다.
*/

/*==============================================================================
  Include Files
  ============================================================================*/
#include <string.h>
#include "sns_mem_util.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"
#include "sns_sensor_util.h"
#include "sns_himax_dsp_sensor.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "sns_suid.pb.h"

#include "sns_pb_util.h"
#include "sns_suid_util.h"
#include "sns_gpio_service.h"
/*==============================================================================
  Function Definitions
  ============================================================================*/
/*
* 함수명: himax_dsp_send_suid_req
* 목적 및 기능: 프레임워크(SEE)에 필요한 하위 서비스(인터럽트, 통신 등)의 SUID를 요청합니다.
* 입력 변수: this(센서 포인터), data_type(요청할 서비스명), data_type_len(길이)
* 출력 변수: 없음
* 리턴 값: 없음
*/
void himax_dsp_send_suid_req(sns_sensor *this, char * const data_type, uint32_t data_type_len)
{
  uint8_t buffer[50];
  sns_memset(buffer, 0, sizeof(buffer));
  himax_dsp_state *state = (himax_dsp_state*)this->state->state;
  sns_service_manager *manager = this->cb->get_service_manager(this);
  sns_stream_service *stream_service = (sns_stream_service*)manager->get_service(manager, SNS_STREAM_SERVICE);
  
  // change(add)-hyungchul-20260323-2250: pb_buffer_arg에 struct 키워드 명시
  struct pb_buffer_arg data = (struct pb_buffer_arg){.buf = data_type, .buf_len = data_type_len};
  sns_suid_req suid_req = sns_suid_req_init_default;
  suid_req.has_register_updates = true;
  suid_req.register_updates = true;
  suid_req.data_type.funcs.encode = &pb_encode_string_cb;
  suid_req.data_type.arg = &data;

  // 스트림이 없으면 생성
  if (state->fw_stream == NULL) {
    stream_service->api->create_sensor_stream(stream_service, this, sns_get_suid_lookup(), &state->fw_stream);
  }
  
  // 요청 전송
  size_t encoded_len = pb_encode_request(buffer, sizeof(buffer), &suid_req, sns_suid_req_fields, NULL);
  if (encoded_len > 0) {
    sns_request request = { .request_len = encoded_len, .request = buffer, .message_id = SNS_SUID_MSGID_SNS_SUID_REQ };
    state->fw_stream->api->send_request(state->fw_stream, &request);
  }
}

/*
* 함수명: himax_dsp_init
* 목적 및 기능: 센서 객체가 생성될 때 호출되며 SUID 설정 및 필요 서비스 의존성을 요청합니다.
* 입력 변수: this (센서 포인터)
* 출력 변수: 없음
* 리턴 값: sns_rc (초기화 성공 여부)
*/
sns_rc himax_dsp_init(sns_sensor * const this)
{
  himax_dsp_state *state = (himax_dsp_state*) this->state->state;
  struct sns_service_manager *smgr = this->cb->get_service_manager(this);
  sns_gpio_service *gpio_service;
  
  state->diag_service = (sns_diag_service *) smgr->get_service(smgr, SNS_DIAG_SERVICE);
  state->scp_service = (sns_sync_com_port_service *) smgr->get_service(smgr, SNS_SYNC_COM_PORT_SERVICE);
  
  gpio_service = (sns_gpio_service *)smgr->get_service(smgr, SNS_GPIO_SERVICE);
  if (NULL != gpio_service && NULL != gpio_service->api)
  {
    // Set GPIO 106 to HIGH
    gpio_service->api->write_gpio(106, true, SNS_GPIO_DRIVE_STRENGTH_2_MILLI_AMP, SNS_GPIO_PULL_TYPE_NO_PULL, SNS_GPIO_STATE_HIGH);
    SNS_PRINTF(HIGH, this, "Set GPIO 106 to HIGH");

    // Set GPIO 14 to HIGH
    gpio_service->api->write_gpio(14, true, SNS_GPIO_DRIVE_STRENGTH_2_MILLI_AMP, SNS_GPIO_PULL_TYPE_NO_PULL, SNS_GPIO_STATE_HIGH);
    SNS_PRINTF(HIGH, this, "Set GPIO 14 to HIGH");

    // Set GPIO 15 to HIGH
    gpio_service->api->write_gpio(15, true, SNS_GPIO_DRIVE_STRENGTH_2_MILLI_AMP, SNS_GPIO_PULL_TYPE_NO_PULL, SNS_GPIO_STATE_HIGH);
    SNS_PRINTF(HIGH, this, "Set GPIO 15 to HIGH");
  }
  
  // 고유 SUID 복사
  sns_sensor_uid suid = (sns_sensor_uid) HIMAX_DSP_SUID_0;
  sns_memscpy(&state->my_suid, sizeof(state->my_suid), &suid, sizeof(sns_sensor_uid));

  // 필수 서비스 SUID 요청 (Async COM Port, Interrupt)
  // change(add)-hyungchul-20260320-1352: Timer, Registry 등 Himax에 불필요한 의존성 모두 제거
  himax_dsp_send_suid_req(this, "async_com_port", 15);
  himax_dsp_send_suid_req(this, "interrupt", 9);

  return SNS_RC_SUCCESS;
}

/*
* 함수명: himax_dsp_deinit
* 목적 및 기능: 센서가 해제될 때 메모리나 리소스를 정리합니다.
*/
sns_rc himax_dsp_deinit(sns_sensor * const this)
{
  // change(add)-hyungchul-20260323-2250: 미사용 파라미터 경고 회피
  UNUSED_VAR(this);
  return SNS_RC_SUCCESS;
}

/*
* 함수명: himax_dsp_sensor_process_suid_events
* 목적 및 기능: SEE 프레임워크로부터 요청한 서비스들의 SUID 응답 이벤트를 처리합니다.
*/
void himax_dsp_sensor_process_suid_events(sns_sensor *const this)
{
  himax_dsp_state *state = (himax_dsp_state*)this->state->state;

  for(; 0 != state->fw_stream->api->get_input_cnt(state->fw_stream); state->fw_stream->api->get_next_input(state->fw_stream))
  {
    sns_sensor_event *event = state->fw_stream->api->peek_input(state->fw_stream);
    if(SNS_SUID_MSGID_SNS_SUID_EVENT == event->message_id)
    {
      pb_istream_t stream = pb_istream_from_buffer((void*)event->event, event->event_len);
      sns_suid_event suid_event = sns_suid_event_init_default;
      
      // change(add)-hyungchul-20260323-2250: pb_buffer_arg에 struct 키워드 명시
      struct pb_buffer_arg data_type_arg = { .buf = NULL, .buf_len = 0 };
      sns_sensor_uid uid_list;
      sns_suid_search suid_search;
      suid_search.suid = &uid_list;
      suid_search.num_of_suids = 0;

      suid_event.data_type.funcs.decode = &pb_decode_string_cb;
      suid_event.data_type.arg = &data_type_arg;
      suid_event.suid.funcs.decode = &pb_decode_suid_event;
      suid_event.suid.arg = &suid_search;

      if(pb_decode(&stream, sns_suid_event_fields, &suid_event) && suid_search.num_of_suids > 0) 
      {
        if(0 == strncmp(data_type_arg.buf, "interrupt", data_type_arg.buf_len)) {
          state->irq_suid = uid_list;
        } else if (0 == strncmp(data_type_arg.buf, "async_com_port", data_type_arg.buf_len)) {
          state->acp_suid = uid_list;
        }
      }
    }
  }
}

/*
* 함수명: himax_dsp_sensor_notify_event
* 목적 및 기능: 시스템 이벤트(의존성 SUID 수신 완료 등)가 발생했을 때 호출됩니다.
*/
sns_rc himax_dsp_sensor_notify_event(sns_sensor * const this)
{
  himax_dsp_state *state = (himax_dsp_state*) this->state->state;

  if (state->fw_stream) 
  {
    himax_dsp_sensor_process_suid_events(this);
    
    // 필수 SUID 2개를 모두 수신했는지 확인
    if ((0 != sns_memcmp(&state->acp_suid, &((sns_sensor_uid){{0}}), sizeof(state->acp_suid))) &&
        (0 != sns_memcmp(&state->irq_suid, &((sns_sensor_uid){{0}}), sizeof(state->irq_suid))))
    {
      // change(add)-hyungchul-20260320-1352: 모든 서비스 연결 시 인스턴스 강제 생성 (Test 앱 없이 단독 구동 보장)
      state->hw_is_present = true;
      sns_sensor_instance *instance = sns_sensor_util_get_shared_instance(this);
      if (NULL == instance)
      {
          instance = this->cb->create_instance(this, sizeof(himax_dsp_instance_state));
          SNS_PRINTF(HIGH, this, "Himax DSP: Instance Auto-created to wait for interrupts");
      }
      sns_sensor_util_remove_sensor_stream(this, &state->fw_stream);
    }
  }
  return SNS_RC_SUCCESS;
}

/*
* 함수명: himax_dsp_get_sensor_uid
* 목적 및 기능: 본 센서의 고유 SUID 포인터를 반환합니다.
*/
static sns_sensor_uid const* himax_dsp_get_sensor_uid(sns_sensor const * const this)
{
  himax_dsp_state *state = (himax_dsp_state*)this->state->state;
  return &state->my_suid;
}

/*
* 함수명: himax_dsp_sensor_set_client_request
* 목적 및 기능: 클라이언트(App)의 요청을 처리하여 인스턴스로 전달합니다.
*/
sns_sensor_instance* himax_dsp_sensor_set_client_request(sns_sensor * const this,
        struct sns_request const *exist_request,
        struct sns_request const *new_request,
        bool remove)
{
  // change(add)-hyungchul-20260323-2250: 미사용 파라미터 경고 회피
  UNUSED_VAR(exist_request);
  UNUSED_VAR(new_request);
  UNUSED_VAR(remove);

  sns_sensor_instance *instance = sns_sensor_util_get_shared_instance(this);
  
  // 클라이언트 요청 처리는 본 Himax 구조에서는 인터럽트 트리거로 무시되어도 무방함.
  // 인스턴스를 무조건 살려두기 위해 삭제를 막습니다.
  SNS_PRINTF(HIGH, this, "Himax DSP: Ignore client req for dedicated driver");
  return instance;
}

// 센서 API 구조체 바인딩
sns_sensor_api himax_dsp_sensor_api = {
  .struct_len = sizeof(sns_sensor_api),
  .init = &himax_dsp_init,
  .deinit = &himax_dsp_deinit,
  .get_sensor_uid = &himax_dsp_get_sensor_uid,
  .set_client_request = &himax_dsp_sensor_set_client_request,
  .notify_event = &himax_dsp_sensor_notify_event,
};
