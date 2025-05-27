/* USER CODE BEGIN Includes */
#include "../../icode/delay/delay.h"
#include "../../icode/led/led.h"
/* USER CODE END Includes */

while (1)
{
    LED_1(1);           // LED1點亮（參數1為亮）
    HAL_Delay(500);     // 延遲500毫秒（ms）
    
    LED_1(0);           // LED1熄滅（參數0為滅）
    HAL_Delay(500);     // 再延遲500毫秒
}
