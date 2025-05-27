/*
 * led.h
 *
 *  Created on: May 27, 2025
 *      Author: user
 */

#ifndef LED_LED_H
#define LED_LED_H

#include "stm32f4xx_hal.h"  // HAL庫檔案
#include "main.h"           // 用於與 main.c 溝通的函式（必要時使用）

// 單獨控制 LED1~LED3，參數t=0為熄滅，其他值為點亮
void LED_1(uint8_t t);
void LED_2(uint8_t t);
void LED_3(uint8_t t);

// 同時控制 LED1~4，bit值代表哪顆燈亮（例: 0b0001 表示只亮LED1）
void LED_All(uint8_t t);

// 讓LED反轉狀態（亮變暗，暗變亮）
void LED_1_Contrary(void);
void LED_2_Contrary(void);
void LED_3_Contrary(void);
void LED_4_Contrary(void);

#endif /* LED_LED_H */
