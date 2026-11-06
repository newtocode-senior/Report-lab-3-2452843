#include "main.h"
#include "input_reading.h"
#include "output_display.h"
#include "input_processing.h"

enum ButtonState {BUTTON_RELEASED, BUTTON_PRESSED, BUTTON_PRESSED_MORE_THAN_1_SECOND};
enum ButtonState buttonState = BUTTON_RELEASED;

int current_mode = 1;
int red_time = 5;
int yellow_time = 2;
int green_time = 3;

int temp_time = 0;

static uint8_t btn2_pressed = 0;
static uint8_t btn3_pressed = 0;

enum TrafficState {PHASE_RED_GREEN, PHASE_RED_YELLOW, PHASE_GREEN_RED, PHASE_YELLOW_RED};
static enum TrafficState traffic_state = PHASE_RED_GREEN;

int countdown_x = 5;
int countdown_y = 3;

void reset_traffic_mode1(void) {
    traffic_state = PHASE_RED_GREEN;
    countdown_x = red_time;
    countdown_y = green_time;
    set_traffic_red_green();
    update_led_buffer(countdown_x, countdown_y);
}

void traffic_run_1s(void) {
    if (current_mode != 1) return;

    // Chặn không cho biến đếm tụt xuống số âm
    if (countdown_x > 0) countdown_x--;
    if (countdown_y > 0) countdown_y--;

    switch (traffic_state) {
        case PHASE_RED_GREEN:
            set_traffic_red_green();
            if (countdown_y <= 0) {
                traffic_state = PHASE_RED_YELLOW;
                countdown_y = yellow_time;
                set_traffic_red_yellow();
            }
            break;

        case PHASE_RED_YELLOW:
            set_traffic_red_yellow();
            if (countdown_x <= 0) {
                traffic_state = PHASE_GREEN_RED;
                countdown_x = green_time;
                countdown_y = red_time;
                set_traffic_green_red();
            }
            break;

        case PHASE_GREEN_RED:
            set_traffic_green_red();
            if (countdown_x <= 0) {
                traffic_state = PHASE_YELLOW_RED;
                countdown_x = yellow_time;
                set_traffic_yellow_red();
            }
            break;

        case PHASE_YELLOW_RED:
            set_traffic_yellow_red();
            if (countdown_y <= 0) {
                traffic_state = PHASE_RED_GREEN;
                countdown_x = red_time;
                countdown_y = green_time;
                set_traffic_red_green();
            }
            break;
    }

    update_led_buffer(countdown_x, countdown_y);
}

void fsm_for_input_processing(void){
    // 1. CHUYỂN MODE KHI BẤM NÚT 1 (mod)
    switch (buttonState) {
        case BUTTON_RELEASED:
            if (is_button_pressed(0)){
                buttonState = BUTTON_PRESSED;
                current_mode++;
                if (current_mode > 4) {
                    current_mode = 1;
                }

                if (current_mode == 1) {
                    reset_traffic_mode1();
                } else if (current_mode == 2) {
                    turn_off_all_leds();
                    temp_time = red_time;
                } else if (current_mode == 3) {
                    turn_off_all_leds();
                    temp_time = yellow_time;
                } else if (current_mode == 4) {
                    turn_off_all_leds();
                    temp_time = green_time;
                }
            }
            break;

        case BUTTON_PRESSED:
            if (!is_button_pressed(0)){
                buttonState = BUTTON_RELEASED;
            } else {
                if (is_button_pressed_1s(0)) {
                    buttonState = BUTTON_PRESSED_MORE_THAN_1_SECOND;
                }
            }
            break;

        case BUTTON_PRESSED_MORE_THAN_1_SECOND:
            if (!is_button_pressed(0)){
                buttonState = BUTTON_RELEASED;
            }
            break;
    }

    // 2. HIỂN THỊ VÀ CÀI ĐẶT
    if (current_mode == 1) {
        // Mode 1 đã được cập nhật chuẩn xác trong traffic_run_1s và reset_traffic_mode1
    }
    else if (current_mode == 2) {
        update_led_buffer(2, temp_time);

        if (is_button_pressed(1)) {
            if (!btn2_pressed) {
                btn2_pressed = 1;
                temp_time++;
                if (temp_time > 99) temp_time = 1;
            }
        } else {
            btn2_pressed = 0;
        }

        if (is_button_pressed(2)) {
            if (!btn3_pressed) {
                btn3_pressed = 1;
                red_time = temp_time;
            }
        } else {
            btn3_pressed = 0;
        }
    }
    else if (current_mode == 3) {
        update_led_buffer(3, temp_time);

        if (is_button_pressed(1)) {
            if (!btn2_pressed) {
                btn2_pressed = 1;
                temp_time++;
                if (temp_time > 99) temp_time = 1;
            }
        } else {
            btn2_pressed = 0;
        }

        if (is_button_pressed(2)) {
            if (!btn3_pressed) {
                btn3_pressed = 1;
                yellow_time = temp_time;
            }
        } else {
            btn3_pressed = 0;
        }
    }
    else if (current_mode == 4) {
        update_led_buffer(4, temp_time);

        if (is_button_pressed(1)) {
            if (!btn2_pressed) {
                btn2_pressed = 1;
                temp_time++;
                if (temp_time > 99) temp_time = 1;
            }
        } else {
            btn2_pressed = 0;
        }

        if (is_button_pressed(2)) {
            if (!btn3_pressed) {
                btn3_pressed = 1;
                green_time = temp_time;
            }
        } else {
            btn3_pressed = 0;
        }
    }
}
