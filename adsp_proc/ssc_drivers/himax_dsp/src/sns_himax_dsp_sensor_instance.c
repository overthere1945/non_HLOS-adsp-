/*
 * 파일명: sns_himax_dsp_sensor_instance.c
 * 목적 및 기능: ADSP 환경에서 Himax DSP와 SPI(Instance 5) 통신을 수행하여, 
 * 인터럽트(GPIO 102) 발생 시 확장된 17 Bytes 헤더를 파싱하고 동적 크기의 JPEG를 128KB 공유 메모리에 씁니다.
 * 첨부된 ct7117x 성공 코드를 바탕으로 QuRT 메모리 맵핑 및 SPI 청크 리딩 로직을 최적화하여 적용하였습니다.
 */

/*==============================================================================
  Include Files
  ============================================================================*/
#include "sns_mem_util.h"
#include "sns_sensor_instance.h"
#include "sns_service_manager.h"
#include "sns_stream_service.h"
#include "sns_rc.h"
#include "sns_request.h"
#include "sns_time.h"
#include "sns_sensor_event.h"
#include "sns_types.h"
#include "sns_event_service.h"
#include "sns_memmgr.h"
#include "sns_com_port_priv.h"

#include "sns_himax_dsp_sensor.h"
#include "sns_himax_dsp_sensor_instance.h"

#include "sns_interrupt.pb.h"
#include "sns_async_com_port.pb.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "sns_pb_util.h"
#include "sns_sync_com_port_service.h"
#include "sns_island.h"
#include <qurt.h>
#include <stdio.h>

#define SHARED_PHYS_ADDR  0x81EC0000
#define SHARED_SIZE       0x20000 

/* 전역 QuRT 변수 */
unsigned int myddr_base_addr = 0; // 매핑된 가상 메모리 주소 저장
void *psram_virtual_addr = NULL;  // PSRAM 가상 주소 (현재 미사용)
qurt_mem_pool_t hwio_pool = 0;    // 메모리 풀 핸들
qurt_mem_region_t shared_mem_region = 0; // 공유 메모리 리전 핸들
qurt_mem_region_attr_t hwio_attr; // 메모리 리전 속성
int qurt_init = 0; // QuRT 초기화 상태 플래그
int int_count = 0; // 인터럽트 발생 횟수 카운터

/*==============================================================================
  Function Definitions
  ============================================================================*/

/*
 * 함수명: himax_com_read_wrapper
 * 목적 및 기능: SCP 서비스를 통해 SPI 버스에서 데이터를 읽어오는 편의 래퍼 함수입니다.
 * 입력 변수: 
 * - scp_service: 동기화 통신 포트 서비스 포인터
 * - port_handle: 통신 포트 핸들
 * - reg_addr: 읽기를 시작할 레지스터 주소 (여기서는 0x00 사용)
 * - bytes: 읽어올 바이트 수
 * 출력 변수: 
 * - buffer: 읽어온 데이터를 저장할 버퍼
 * - xfer_bytes: 실제 전송(읽기) 완료된 바이트 수
 * 리턴 값: sns_rc (통신 성공 여부, 성공 시 SNS_RC_SUCCESS)
 */
static sns_rc himax_com_read_wrapper(
  sns_sync_com_port_service *scp_service,
  sns_sync_com_port_handle *port_handle,
  uint32_t reg_addr, uint8_t  *buffer, uint32_t bytes, uint32_t *xfer_bytes)
{
  sns_port_vector port_vec; // 포트 벡터 구조체 선언
  port_vec.buffer = buffer; // 수신 버퍼 연결
  port_vec.bytes = bytes; // 수신할 크기 설정
  port_vec.is_write = false; // 읽기 동작이므로 false 설정
  port_vec.reg_addr = reg_addr; // 레지스터 주소 설정
  // SCP API를 호출하여 실제 레지스터(SPI) 읽기 수행
  return scp_service->api->sns_scp_register_rw(port_handle, &port_vec, 1, false, xfer_bytes);
}

/*
 * 함수명: himax_dsp_handle_interrupt_event
 * 목적 및 기능: GPIO 102 인터럽트 발생 시 호출되며, QuRT 메모리 초기화, SPI Chunk 리딩 및 Shared Memory 적재를 수행합니다.
 * 입력 변수: 
 * - instance: 센서 인스턴스 포인터
 * - timestamp: 인터럽트 발생 시간
 * 출력 변수: 없음
 * 리턴 값: 없음
 */
void himax_dsp_handle_interrupt_event(sns_sensor_instance *const instance, sns_time timestamp)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*)instance->state->state; // 인스턴스 상태 획득
    SNS_INST_PRINTF(LOW, instance, "20260407 Himax DSP: Interrupt triggered"); // 인터럽트 진입 로그

    // change(add)-hyungchul-20260325-1538 시작: 스택 오버플로우 방지를 위한 동적 메모리 할당
    // QuRT 환경에서 지역변수로 2KB를 선언하면 스택이 터져 루프 변수가 오염(Tearing 유발)될 수 있으므로 heap 할당.
    uint8_t *buffer = (uint8_t*)sns_malloc(SNS_HEAP_MAIN, 2048); // 2KB 버퍼 동적 할당
    if(buffer == NULL) // 할당 실패 예외 처리
    {
        SNS_INST_PRINTF(ERROR, instance, "Memory Allocation Failed for SPI buffer");
        return; // 함수 조기 종료
    }
    // change(add)-hyungchul-20260325-1538 끝

    uint32_t xfer_bytes = 0; // 1회 읽기 시 전송된 바이트 수
    uint32_t total_read_bytes = 0; // 전체 읽은 바이트 수 누적용

    if(int_count < 30) int_count=int_count+1; // 초기 인터럽트 30회까지 카운트 증가
    
    // 초기 부팅 시 안정성을 위해 20번째 인터럽트 이후에 QuRT 메모리 맵핑 시도
    if(int_count > 20 && qurt_init <= 3)
    {
        // 1. 메모리 풀 어태치
        if(QURT_EOK == qurt_mem_pool_attach("smem_pool", &hwio_pool))
        {
            qurt_mem_region_attr_init(&hwio_attr); // 리전 속성 초기화
            qurt_mem_region_attr_set_cache_mode(&hwio_attr, QURT_MEM_CACHE_NONE_SHARED); // 캐시 비활성화 (공유 메모리 동기화 목적)
            qurt_mem_region_attr_set_mapping(&hwio_attr, QURT_MEM_MAPPING_PHYS_CONTIGUOUS); // 물리적으로 연속된 메모리 매핑
            qurt_mem_region_attr_set_physaddr(&hwio_attr, SHARED_PHYS_ADDR); // 물리 주소 할당 (0x81EC0000)
            qurt_init = 1; // 1단계 완료

            // 2. 메모리 리전 생성 (128KB)
            if (QURT_EOK != qurt_mem_region_create(&shared_mem_region, SHARED_SIZE, hwio_pool, &hwio_attr))
            {
                qurt_init = 2; // 생성 실패
            }
            else
            {
                // 3. 생성된 리전의 속성 획득
                if (QURT_EOK != qurt_mem_region_attr_get(shared_mem_region, &hwio_attr))
                {
                    qurt_init = 3; // 속성 획득 실패
                }
                else
                {
                    // 4. 물리 주소에 매핑된 가상 주소 획득
                    qurt_mem_region_attr_get_virtaddr(&hwio_attr, &myddr_base_addr);
                    qurt_init = 4; // QuRT 메모리 초기화 최종 성공
                }
            }
        }
    }

    if(qurt_init <= 3) // QuRT 초기화 진행 중 상태 로그 출력
    {
        SNS_INST_PRINTF(HIGH, instance, "qurt_init: %d, int_count: %d", qurt_init, int_count);
    }
    
    {
        // SPI 통신 중 슬립 방지를 위해 Island Exit 호출 (DDR 접근 및 긴 통신 시간 확보)
        sns_island_exit_internal();
        
        // change(add)-hyungchul-20260406-1704 시작: 헤더 사이즈를 기존 11바이트에서 17바이트로 변경
        uint32_t header_len = 17; // Himax DSP 펌웨어 변경에 따른 신규 헤더 길이 적용
        // change(add)-hyungchul-20260406-1704 끝
        uint32_t jpeg_size = 0; // 수신될 JPEG 페이로드 크기
        uint32_t payload_read_bytes = 0; // 현재까지 읽은 페이로드 바이트 수
        uint8_t *shared_mem_ptr = (uint8_t *)myddr_base_addr; // 공유 메모리의 가상 주소 포인터

        // change(add)-hyungchul-20260325-1538 시작: Race Condition 체크 로직 추가
        // Android App이 아직 공유 메모리를 읽고 있는 상태(0xC0)라면, 메모리를 덮어쓰지 않습니다. (Tearing 방지)
        bool drop_frame = false; // 프레임 드랍 여부 플래그
        if(qurt_init == 4 && shared_mem_ptr != NULL) // 메모리 매핑이 완료된 경우
        {
            if(shared_mem_ptr[0] == 0xC0) // C++ 스레드가 아직 처리를 완료하지 않은 경우
            {
                drop_frame = true; // 프레임 드랍 설정
                SNS_INST_PRINTF(ERROR, instance, "Android is busy processing previous frame! Dropping Memory Write to prevent Tearing.");
                // 단, SPI 버퍼는 읽어서 비워주어야 다음 인터럽트가 정상 발생하므로 함수를 종료(return)하지 않고 SPI Read는 계속 진행합니다.
            }
        }
        // change(add)-hyungchul-20260325-1538 끝

        // 1. 첫 번째 청크(2KB) 리딩. Himax 더미 구조와 동기화를 위해 무조건 2048 크기 고정 읽기.
        uint32_t first_chunk = 2048; // 첫 읽기 크기
        state->com_read(state->scp_service, state->com_port_info.port_handle,
                        0x00, buffer, first_chunk, &xfer_bytes); // SPI 데이터 수신
        total_read_bytes += xfer_bytes; // 누적 읽기 바이트 갱신

        // 2. 헤더 파싱 및 데이터 정합성 확인 (SYNC: 0xC0 0x5A, Type: 0x01(JPEG) or 0x16(YUV420P))
        if(buffer[0] == 0xC0 && buffer[1] == 0x5A && (buffer[2] == 0x01 || buffer[2] == 0x16))
        {
            // change(add)-hyungchul-20260325-1538 시작: 부호 확장(Sign Extension) 버그 수정
            // uint32_t로 명시적 캐스팅을 하여 비트 시프트 연산 시 데이터가 음수로 변질되는 것을 방지합니다.
            jpeg_size = ((uint32_t)buffer[6] << 24) | ((uint32_t)buffer[5] << 16) | ((uint32_t)buffer[4] << 8) | (uint32_t)buffer[3];
            // change(add)-hyungchul-20260325-1538 끝

            SNS_INST_PRINTF(HIGH, instance, "Header Parsed successfully. JPEG Size: %u bytes", jpeg_size);

            // change(add)-hyungchul-20260406-1704 시작: 17바이트 확장 헤더 파라미터 파싱 및 디버그 로깅
            // 수신된 17바이트 버퍼에서 추가된 이미지 정보 및 노출 제어 값을 추출하여 확인합니다.
            uint8_t low_illumination = buffer[7];     // 저조도 상태 (1 byte)
            uint8_t similarity = buffer[8];           // 이미지 유사도 (1 byte, ex: 70 = 70%)
            uint8_t scene_index = buffer[9];          // 씬 인덱스 (1 byte)
            uint8_t occl_prob = buffer[10];           // Occlusion 확률 (1 byte, 0~100)
            uint8_t blur_value = buffer[11];          // 블러 값 (1 byte)
            uint8_t expose_0 = buffer[12];            // 노출 값 0 (1 byte)
            uint8_t expose_1 = buffer[13];            // 노출 값 1 (1 byte)
            uint8_t a_gain = buffer[14];              // 아날로그 게인 (1 byte)
            uint8_t d_gain_0 = buffer[15];            // 디지털 게인 0 (1 byte)
            uint8_t d_gain_1 = buffer[16];            // 디지털 게인 1 (1 byte)

            SNS_INST_PRINTF(HIGH, instance, "Ext Header: low_illu=%u,  Sim=%u, Scene=%u, Occ=%u, Blur=%u, Exp=[%u,%u], GainA=%u, GainD=[%u,%u]", 
                            low_illumination, similarity, scene_index, occl_prob, blur_value, expose_0, expose_1, a_gain, d_gain_0, d_gain_1);
            // change(add)-hyungchul-20260406-1704 끝

            // 메모리 오버플로우 방지 로직 (헤더 크기 + JPEG 크기가 공유 메모리 크기를 초과하는지 검사)
            if((header_len + jpeg_size) > SHARED_SIZE)
            {
                SNS_INST_PRINTF(ERROR, instance, "Error: JPEG Size exceeds Shared Memory Capacity!");
                jpeg_size = SHARED_SIZE - header_len; // 초과 시 공유 메모리에 들어갈 수 있는 최대 크기로 제한
            }

            // 프레임을 드랍하지 않는 경우에만 공유 메모리에 복사 진행
            if(!drop_frame && qurt_init == 4 && shared_mem_ptr != NULL)
            {
                buffer[0] = 0x00; // Race Condition 보호용 Lock Flag Down (헤더 복사 시 첫 바이트를 0x00으로 덮어씀)
                memcpy(shared_mem_ptr, buffer, xfer_bytes); // 첫 번째 청크(헤더 포함)를 공유 메모리에 복사
            }

            // 첫 번째 청크에서 읽은 페이로드(순수 JPEG 데이터) 크기 계산
            if (xfer_bytes > header_len) {
                payload_read_bytes += (xfer_bytes - header_len);
            }

            // 3. 남은 Payload 루프 돌며 수신 (2KB Chunk 철저 유지)
            while(payload_read_bytes < jpeg_size)
            {
                uint32_t remain = jpeg_size - payload_read_bytes; // 남은 읽어야 할 바이트 수
                // change(add)-hyungchul-20260325-1538 시작: 청크 사이즈 계산 수정
                // Himax는 정확히 2048 데이터 바이트마다 Dummy를 넣습니다. 중간에 끊어 읽으면 SPI Sync가 틀어지므로 2048을 유지합니다.
                uint32_t chunk = remain > 2048 ? 2048 : remain; 
                // change(add)-hyungchul-20260325-1538 끝
                
                // 남은 데이터 청크 단위로 SPI 수신
                state->com_read(state->scp_service, state->com_port_info.port_handle,
                                0x00, buffer, chunk, &xfer_bytes);

                // 프레임 드랍 상태가 아니면 공유 메모리에 이어 붙이기
                if(!drop_frame && qurt_init == 4 && shared_mem_ptr != NULL)
                {
                    // 현재 복사할 공유 메모리의 목적지 주소 계산 (시작 주소 + 헤더 크기 + 지금까지 읽은 페이로드 크기)
                    void* current_dest = (void*)(shared_mem_ptr + header_len + payload_read_bytes);
                    memcpy(current_dest, buffer, xfer_bytes); // 청크 데이터 복사
                }
                
                payload_read_bytes += xfer_bytes; // 수신한 페이로드 크기 누적
                total_read_bytes += xfer_bytes;   // 전체 수신 크기 누적
            }

            qurt_mem_barrier(); // 메모리 배리어 발동: 모든 캐시/레지스터 쓰기가 물리 램(RAM)에 도달했음을 하드웨어적으로 보장

            // 4. 복사가 완벽히 끝나면 헤더의 첫 바이트를 복원(0xC0)하여 Android C++ Polling Thread를 깨움
            if(!drop_frame && qurt_init == 4 && shared_mem_ptr != NULL)
            {
                shared_mem_ptr[0] = 0xC0; // Lock Flag Up -> Ready to read (C++ 측에 데이터 소비 가능 알림)
                SNS_INST_PRINTF(HIGH, instance, "Memory Flag Unlocked(0xC0). Delivery Complete.");
            }
        }
        else
        {
            // 헤더 동기화 바이트가 맞지 않을 경우 에러 출력
            SNS_INST_PRINTF(ERROR, instance, "Header Sync Failed! Found: [%02X %02X %02X]", buffer[0], buffer[1], buffer[2]);
        }
    }

    // 할당한 힙 메모리 해제 (메모리 누수 방지)
    sns_free(buffer);

    SNS_INST_PRINTF(HIGH, instance, "Total read bytes: %u", total_read_bytes); // 최종 수신 바이트 수 로그 출력
    
    // 컴파일러의 Unused Parameter 경고(-Werror) 회피 처리
    UNUSED_VAR(timestamp);
    UNUSED_VAR(total_read_bytes);
}

/*
 * 함수명: himax_dsp_inst_init
 * 목적 및 기능: 인스턴스 초기화 시 SPI 통신 버스 파워를 설정하고 하드웨어 인터럽트를 연결합니다.
 * 입력 변수: 
 * - this: 초기화할 센서 인스턴스 포인터
 * - sstate: 센서의 상태 구조체 포인터
 * 출력 변수: 없음
 * 리턴 값: sns_rc (성공 여부, 성공 시 SNS_RC_SUCCESS)
 */
sns_rc himax_dsp_inst_init(sns_sensor_instance * const this, sns_sensor_state const *sstate)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*) this->state->state;
    himax_dsp_state *sensor_state = (himax_dsp_state*) sstate->state;
    sns_service_manager *service_mgr = this->cb->get_service_manager(this);
    sns_stream_service *stream_mgr = (sns_stream_service*)service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);

    state->scp_service = (sns_sync_com_port_service*)service_mgr->get_service(service_mgr, SNS_SYNC_COM_PORT_SERVICE);
    state->com_read = himax_com_read_wrapper; // 래퍼 함수 매핑

    // SPI 5번(SSC_QUP_SE4) 강제 할당 및 설정 로직
    state->com_port_info.com_config.bus_type = SNS_BUS_SPI;
    state->com_port_info.com_config.bus_instance = 5;
    state->com_port_info.com_config.slave_control = 0;
    state->com_port_info.com_config.min_bus_speed_KHz = 15000; // SPI 클럭 최소 15MHz
    state->com_port_info.com_config.max_bus_speed_KHz = 15000; // SPI 클럭 최대 15MHz
    state->com_port_info.com_config.reg_addr_type = 0x0; 

    // SPI 포트 등록 및 오픈
    state->scp_service->api->sns_scp_register_com_port(&state->com_port_info.com_config, &state->com_port_info.port_handle);
    state->scp_service->api->sns_scp_open(state->com_port_info.port_handle);
    state->scp_service->api->sns_scp_update_bus_power(state->com_port_info.port_handle, false); // 슬립 상태로 대기

    // 인터럽트 SUID 복사
    sns_memscpy(&state->irq_suid, sizeof(state->irq_suid), &sensor_state->irq_suid, sizeof(sensor_state->irq_suid));

    // GPIO 102번 하드웨어 인터럽트 스트림 생성 및 등록 로직
    if (NULL == state->interrupt_data_stream)
    {
        stream_mgr->api->create_sensor_instance_stream(stream_mgr, this, state->irq_suid, &state->interrupt_data_stream);
    }

    if (NULL != state->interrupt_data_stream)
    {
        uint8_t buffer[20];
        sns_request irq_req = { .message_id = SNS_INTERRUPT_MSGID_SNS_INTERRUPT_REQ, .request = buffer };
        sns_interrupt_req req_payload = sns_interrupt_req_init_default;

        req_payload.interrupt_trigger_type = SNS_INTERRUPT_TRIGGER_TYPE_RISING; // Rising Edge 트리거
        req_payload.interrupt_num = 102; // 대상 핀: GPIO 102
        req_payload.interrupt_pull_type = SNS_INTERRUPT_PULL_TYPE_PULL_DOWN; // Pull-down 설정
        req_payload.is_chip_pin = true;
        req_payload.interrupt_drive_strength = SNS_INTERRUPT_DRIVE_STRENGTH_2_MILLI_AMP;

        irq_req.request_len = pb_encode_request(buffer, sizeof(buffer), &req_payload, sns_interrupt_req_fields, NULL);
        if (irq_req.request_len > 0)
        {
            state->interrupt_data_stream->api->send_request(state->interrupt_data_stream, &irq_req);
            SNS_INST_PRINTF(HIGH, this, "Himax DSP: Interrupt registered on GPIO %d", req_payload.interrupt_num);
        }
    }

    return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_inst_deinit
 * 목적 및 기능: 인스턴스 종료 시 스트림과 통신 포트를 해제하여 시스템 메모리 누수를 방지합니다.
 * 입력 변수: 
 * - this: 종료할 센서 인스턴스 포인터
 * 출력 변수: 없음
 * 리턴 값: sns_rc (성공 시 SNS_RC_SUCCESS)
 */
sns_rc himax_dsp_inst_deinit(sns_sensor_instance *const this)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*) this->state->state;
    sns_service_manager *service_mgr = this->cb->get_service_manager(this);
    sns_stream_service *stream_mgr = (sns_stream_service*) service_mgr->get_service(service_mgr, SNS_STREAM_SERVICE);
    
    // 인터럽트 스트림 해제
    if(state->interrupt_data_stream) {
        stream_mgr->api->remove_stream(stream_mgr, state->interrupt_data_stream);
    }
    // SPI 포트 연결 종료 및 등록 해제
    if(state->scp_service) {
        state->scp_service->api->sns_scp_close(state->com_port_info.port_handle);
        state->scp_service->api->sns_scp_deregister_com_port(&state->com_port_info.port_handle);
    }
    return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_inst_notify_event
 * 목적 및 기능: 인터럽트 발생 시 프레임워크로부터 이벤트를 수신하여 실제 데이터 처리 핸들러로 전달합니다.
 * 입력 변수: 
 * - this: 이벤트가 발생한 센서 인스턴스 포인터
 * 출력 변수: 없음
 * 리턴 값: sns_rc (성공 시 SNS_RC_SUCCESS)
 */
sns_rc himax_dsp_inst_notify_event(sns_sensor_instance * const this)
{
    himax_dsp_instance_state *state = (himax_dsp_instance_state*) this->state->state;
    sns_sensor_event *event;

    // 인터럽트 처리 전 통신 버스 전원 활성화 (ON)
    state->scp_service->api->sns_scp_update_bus_power(state->com_port_info.port_handle, true);

    if (state->interrupt_data_stream != NULL)
    {
        event = state->interrupt_data_stream->api->peek_input(state->interrupt_data_stream);
        while (NULL != event)
        {
            if (event->message_id == SNS_INTERRUPT_MSGID_SNS_INTERRUPT_EVENT)
            {
                sns_interrupt_event irq_event = sns_interrupt_event_init_default;
                pb_istream_t stream = pb_istream_from_buffer((pb_byte_t*)event->event, event->event_len);
                // 이벤트 디코딩 성공 시, 실질적인 JPEG 파싱 함수 호출
                if (pb_decode(&stream, sns_interrupt_event_fields, &irq_event))
                {
                    himax_dsp_handle_interrupt_event(this, irq_event.timestamp);
                }
            }
            event = state->interrupt_data_stream->api->get_next_input(state->interrupt_data_stream);
        }
    }

    // 통신 버스 전원 비활성화 (절전 모드 전환)
    state->scp_service->api->sns_scp_update_bus_power(state->com_port_info.port_handle, false);
    return SNS_RC_SUCCESS;
}

/*
 * 함수명: himax_dsp_inst_set_client_config
 * 목적 및 기능: 클라이언트(App)의 설정 요청을 처리합니다. Himax 전용 드라이버이므로 요청을 무시하고 Dummy로 응답합니다.
 * 입력 변수: 
 * - this: 센서 인스턴스 포인터
 * - client_request: 클라이언트로부터 수신된 요청
 * 출력 변수: 없음
 * 리턴 값: sns_rc (성공 시 SNS_RC_SUCCESS)
 */
sns_rc himax_dsp_inst_set_client_config(sns_sensor_instance * const this, sns_request const *client_request) 
{ 
    // 컴파일 에러 회피 (Unused Parameter)
    UNUSED_VAR(this);
    UNUSED_VAR(client_request);
    return SNS_RC_SUCCESS; 
}

// 인스턴스 API 바인딩 구조체 초기화
sns_sensor_instance_api himax_dsp_sensor_instance_api =
{
  .struct_len = sizeof(sns_sensor_instance_api),
  .init = &himax_dsp_inst_init,
  .deinit = &himax_dsp_inst_deinit,
  .set_client_config = &himax_dsp_inst_set_client_config,
  .notify_event = &himax_dsp_inst_notify_event
};
