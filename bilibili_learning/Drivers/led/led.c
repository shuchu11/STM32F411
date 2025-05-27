/*
 * led.c
 *
 *  Created on: May 27, 2025
 *      Author: user
 */

#include "led.h"

// 控制LED1亮滅
void LED1(uint8_t t) {
    if (t)
        HAL_GPIO_WritePin(GPIOG, LED1_Pin, GPIO_PIN_RESET);  // 亮
    else
        HAL_GPIO_WritePin(GPIOG, LED1_Pin, GPIO_PIN_SET);    // 滅
}

// 控制LED2亮滅
void LED2(uint8_t t) {
    if (t)
        HAL_GPIO_WritePin(GPIOG, LED2_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOG, LED2_Pin, GPIO_PIN_SET);
}

// 控制LED3亮滅
void LED3(uint8_t t) {
    if (t)
        HAL_GPIO_WritePin(GPIOG, LED3_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOG, LED3_Pin, GPIO_PIN_SET);
}
