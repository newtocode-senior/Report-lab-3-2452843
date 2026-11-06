#include "main.h"
#include "input_reading.h"
#include "output_display.h"
#include "input_processing.h"

#define BLINK_2HZ_CYCLE (250 / TIMER_CYCLE)
#define ONE_SECOND_CYCLE (1000 / TIMER_CYCLE)

extern int current_mode;

static int scan_index = 0;
static int blink_counter = 0;
static int one_sec_counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim->Instance == TIM2) {
        // 1. Quét đọc nút nhấn (10ms)
        button_reading();

        // 2. Quét luân phiên 4 con LED 7 đoạn (10ms đổi 1 con)
        scan_7segment(scan_index);
        scan_index = (scan_index + 1) % 4;

        // 3. Đếm nhịp 1 giây cho đèn giao thông ở Mode 1
        if (current_mode == 1) {
            one_sec_counter++;
            if (one_sec_counter >= ONE_SECOND_CYCLE) {
                one_sec_counter = 0;
                traffic_run_1s();
            }
        } else {
            one_sec_counter = 0; // Giữ reset để giây đầu tiên khi về Mode 1 luôn đủ 1000ms
        }

        // 4. Đếm nhịp chớp tắt 2 Hz cho các Mode cài đặt (2, 3, 4)
        if (current_mode >= 2 && current_mode <= 4) {
            blink_counter++;
            if (blink_counter >= BLINK_2HZ_CYCLE) {
                blink_counter = 0;
                blinkled(current_mode);
            }
        } else {
            blink_counter = 0;
        }
    }
}
