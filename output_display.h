#ifndef INC_OUTPUT_DISPLAY_H_
#define INC_OUTPUT_DISPLAY_H_

#include "main.h"

#define BLINK_2HZ_CYCLE (250 / TIMER_CYCLE)

void display7SEG(int num);
void scan_7segment(int index);
void update_led_buffer(int road_x_val, int road_y_val);
void blinkled(int current_mode);
void turn_off_all_leds(void);

// Các hàm điều khiển màu đèn ngã tư
void set_traffic_red_green(void);
void set_traffic_red_yellow(void);
void set_traffic_green_red(void);
void set_traffic_yellow_red(void);

#endif /* INC_OUTPUT_DISPLAY_H_ */
