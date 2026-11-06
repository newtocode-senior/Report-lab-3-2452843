#include "output_display.h"

// led_buffer[0], [1]: Đường X (x1led, x0led)
// led_buffer[2], [3]: Đường Y (y1led, y0led)
static int led_buffer[4] = {0, 0, 0, 0};

void update_led_buffer(int road_x_val, int road_y_val) {
    if (road_x_val < 0) road_x_val = 0;
    if (road_y_val < 0) road_y_val = 0;

    led_buffer[0] = (road_x_val / 10) % 10;
    led_buffer[1] = road_x_val % 10;
    led_buffer[2] = (road_y_val / 10) % 10;
    led_buffer[3] = road_y_val % 10;
}

void display7SEG(int num) {
    switch (num) {
        case 0:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, g_Pin, GPIO_PIN_SET);
            break;
        case 1:
            HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, a_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 2:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|d_Pin|e_Pin|g_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, c_Pin|f_Pin, GPIO_PIN_SET);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|g_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, e_Pin|f_Pin, GPIO_PIN_SET);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, b_Pin|c_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, a_Pin|d_Pin|e_Pin, GPIO_PIN_SET);
            break;
        case 5:
            HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, b_Pin|e_Pin, GPIO_PIN_SET);
            break;
        case 6:
            HAL_GPIO_WritePin(GPIOB, a_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, b_Pin, GPIO_PIN_SET);
            break;
        case 7:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
        case 8:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            break;
        case 9:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|f_Pin|g_Pin, GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOB, e_Pin, GPIO_PIN_SET);
            break;
        default:
            HAL_GPIO_WritePin(GPIOB, a_Pin|b_Pin|c_Pin|d_Pin|e_Pin|f_Pin|g_Pin, GPIO_PIN_SET);
            break;
    }
}

void scan_7segment(int index) {
    // 1. TẮT HẾT CẢ 4 LED (Đối với PNP: xuất mức 1 để KHÓA cả 4 transistor)
    HAL_GPIO_WritePin(GPIOA, x1led_Pin | x0led_Pin | y1led_Pin | y0led_Pin, GPIO_PIN_SET);

    // 2. Xuất mã số ra PB0 - PB6
    // 3. BẬT DUY NHẤT 1 LED (Đối với PNP: xuất mức 0 để MỞ transistor tương ứng)
    switch (index) {
        case 0:
            display7SEG(led_buffer[0]);
            HAL_GPIO_WritePin(GPIOA, x1led_Pin, GPIO_PIN_RESET); // Kích mở LED x1
            break;
        case 1:
            display7SEG(led_buffer[1]);
            HAL_GPIO_WritePin(GPIOA, x0led_Pin, GPIO_PIN_RESET); // Kích mở LED x0
            break;
        case 2:
            display7SEG(led_buffer[2]);
            HAL_GPIO_WritePin(GPIOA, y1led_Pin, GPIO_PIN_RESET); // Kích mở LED y1
            break;
        case 3:
            display7SEG(led_buffer[3]);
            HAL_GPIO_WritePin(GPIOA, y0led_Pin, GPIO_PIN_RESET); // Kích mở LED y0
            break;
    }
}

void turn_off_all_leds(void) {
    HAL_GPIO_WritePin(GPIOB, red_Pin | yellow_Pin | green_Pin |
                             red1_Pin | yellow1_Pin | green1_Pin, GPIO_PIN_RESET);
}

// 4 pha đèn chuẩn ngã tư:
void set_traffic_red_green(void) {
    HAL_GPIO_WritePin(GPIOB, red_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, yellow_Pin | green_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, green1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, red1_Pin | yellow1_Pin, GPIO_PIN_RESET);
}

void set_traffic_red_yellow(void) {
    HAL_GPIO_WritePin(GPIOB, red_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, yellow_Pin | green_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, yellow1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, red1_Pin | green1_Pin, GPIO_PIN_RESET);
}

void set_traffic_green_red(void) {
    HAL_GPIO_WritePin(GPIOB, green_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, red_Pin | yellow_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, red1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, yellow1_Pin | green1_Pin, GPIO_PIN_RESET);
}

void set_traffic_yellow_red(void) {
    HAL_GPIO_WritePin(GPIOB, yellow_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, red_Pin | green_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, red1_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, yellow1_Pin | green1_Pin, GPIO_PIN_RESET);
}

void blinkled(int current_mode) {
    switch (current_mode) {
        case 2:
            HAL_GPIO_WritePin(GPIOB, yellow_Pin | green_Pin | yellow1_Pin | green1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_TogglePin(GPIOB, red_Pin | red1_Pin);
            break;
        case 3:
            HAL_GPIO_WritePin(GPIOB, red_Pin | green_Pin | red1_Pin | green1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_TogglePin(GPIOB, yellow_Pin | yellow1_Pin);
            break;
        case 4:
            HAL_GPIO_WritePin(GPIOB, red_Pin | yellow_Pin | red1_Pin | yellow1_Pin, GPIO_PIN_RESET);
            HAL_GPIO_TogglePin(GPIOB, green_Pin | green1_Pin);
            break;
        default:
            break;
    }
}
