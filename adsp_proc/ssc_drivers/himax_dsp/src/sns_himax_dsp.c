/*
* 파일명: sns_himax_dsp.c
* 목적 및 기능: Himax DSP 이미지 센서 전용 드라이버를 Qualcomm SEE (Sensors Execution Environment) 프레임워크에 등록합니다.
*/

/*==============================================================================
  Include Files
  ============================================================================*/
#include "sns_rc.h"
#include "sns_register.h"
#include "sns_himax_dsp_sensor.h"
#include "sns_himax_dsp_sensor_instance.h"

/*==============================================================================
  Function Definitions
  ============================================================================*/
/*
* 함수명: sns_register_himax_dsp
* 목적 및 기능: Himax DSP 센서 API 및 인스턴스 API를 SEE 프레임워크에 등록합니다.
* 입력 변수: register_api (센서 등록을 위한 콜백 함수 구조체 포인터)
* 출력 변수: 없음
* 리턴 값: sns_rc (등록 성공 여부, 성공 시 SNS_RC_SUCCESS)
*/
// change(add)-hyungchul-20260320-1352: 드라이버 등록 명칭을 ct7117x에서 himax_dsp로 변경
sns_rc sns_register_himax_dsp(sns_register_cb const *register_api)
{ 
  /** Himax DSP 이미지 센서 등록 */
  // 센서의 상태 구조체 크기와 센서/인스턴스 API 구조체를 프레임워크에 전달
  register_api->init_sensor(sizeof(himax_dsp_state), &himax_dsp_sensor_api,
      &himax_dsp_sensor_instance_api);
      
  return SNS_RC_SUCCESS;
}
