#include "main.h"
#include "input_reading.h"

#define NO_OF_BUTTONS 3
#define DURATION_FOR_AUTO_INCREASING (1000 / TIMER_CYCLE)
#define BUTTON_IS_PRESSED  GPIO_PIN_RESET
#define BUTTON_IS_RELEASED GPIO_PIN_SET

static GPIO_TypeDef* buttonPort[NO_OF_BUTTONS] = {mod_GPIO_Port, modify_GPIO_Port, set_GPIO_Port};
static uint16_t      buttonPin[NO_OF_BUTTONS]  = {mod_Pin, modify_Pin, set_Pin};

// Khởi tạo trạng thái ban đầu là THẢ NÚT (mức 1)
static GPIO_PinState buttonBuffer[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer1[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};
static GPIO_PinState debounceButtonBuffer2[NO_OF_BUTTONS] = {BUTTON_IS_RELEASED, BUTTON_IS_RELEASED, BUTTON_IS_RELEASED};

static uint8_t  flagForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};
static uint16_t counterForButtonPress1s[NO_OF_BUTTONS] = {0, 0, 0};

void button_reading(void){
    for (int i = 0; i < NO_OF_BUTTONS; i++){
        debounceButtonBuffer2[i] = debounceButtonBuffer1[i];
        debounceButtonBuffer1[i] = HAL_GPIO_ReadPin(buttonPort[i], buttonPin[i]);

        if (debounceButtonBuffer1[i] == debounceButtonBuffer2[i]){
            buttonBuffer[i] = debounceButtonBuffer1[i];

            if (buttonBuffer[i] == BUTTON_IS_PRESSED){
                if (counterForButtonPress1s[i] < DURATION_FOR_AUTO_INCREASING){
                    counterForButtonPress1s[i]++;
                } else {
                    flagForButtonPress1s[i] = 1;
                }
            } else {
                counterForButtonPress1s[i] = 0;
                flagForButtonPress1s[i] = 0;
            }
        }
    }
}

unsigned char is_button_pressed(uint8_t index){
    if (index >= NO_OF_BUTTONS) return 0;
    return (buttonBuffer[index] == BUTTON_IS_PRESSED);
}

unsigned char is_button_pressed_1s(unsigned char index){
    if (index >= NO_OF_BUTTONS) return 0;
    return (flagForButtonPress1s[index] == 1);
}
