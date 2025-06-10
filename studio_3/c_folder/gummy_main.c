/* USER CODE BEGIN Includes */

#include "gummyB_data.h"

/* USER CODE END Includes */

/* USER CODE BEGIN PV */

extern const int16_t audio_data[];
extern const uint32_t audio_length;

/* USER CODE END PV */


/* USER CODE BEGIN 0 */
uint16_t txData;
int txIndex;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2)
  {
    txData = (uint16_t)audio_data[txIndex];
    txIndex = txIndex + 1;
    if( txIndex > audio_length ) txIndex = 0;
    HAL_I2S_Transmit(&hi2s2, &txData, 1, 10);
  }
}
/* USER CODE END 0 */

/* USER CODE BEGIN 2 */
txIndex = 0;
HAL_TIM_Base_Start_IT(&htim2);
/* USER CODE END 2 */

