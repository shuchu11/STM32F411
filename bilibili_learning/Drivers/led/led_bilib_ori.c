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
        HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_RESET);  // 亮
    else
        HAL_GPIO_WritePin(GPIOB, LED1_Pin, GPIO_PIN_SET);    // 滅
}

// 控制LED2亮滅
void LED2(uint8_t t) {
    if (t)
        HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOB, LED2_Pin, GPIO_PIN_SET);
}

// 控制LED3亮滅
void LED3(uint8_t t) {
    if (t)
        HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOB, LED3_Pin, GPIO_PIN_SET);
}
void LED_ALL(uint8_t a) {
    // LED1
    if (a & 0x01)
        HAL_GPIO_WritePin(GPIOG, LED1_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOG, LED1_Pin, GPIO_PIN_SET);

    // LED2
    if (a & 0x02)
        HAL_GPIO_WritePin(GPIOG, LED2_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOG, LED2_Pin, GPIO_PIN_SET);

    // LED3
    if (a & 0x04)
        HAL_GPIO_WritePin(GPIOG, LED3_Pin, GPIO_PIN_RESET);
    else
        HAL_GPIO_WritePin(GPIOG, LED3_Pin, GPIO_PIN_SET);

}

void LED_1_Contrary(void) {
    HAL_GPIO_WritePin(GPIOG, LED1_Pin, 1 - HAL_GPIO_ReadPin(GPIOG, LED1_Pin));
}

void LED_2_Contrary(void) {
    HAL_GPIO_WritePin(GPIOG, LED2_Pin, 1 - HAL_GPIO_ReadPin(GPIOG, LED2_Pin));
}

void LED_3_Contrary(void) {
    HAL_GPIO_WritePin(GPIOG, LED3_Pin, 1 - HAL_GPIO_ReadPin(GPIOG, LED3_Pin));
}

