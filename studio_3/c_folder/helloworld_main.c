/* USER CODE BEGIN Includes */

#include "helloworld.h"

/* USER CODE END Includes */

/* USER CODE BEGIN PV */

extern uint8_t helloworld[];

/* USER CODE END PV */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint16_t txData;
int txIndex;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM2)
  {
    txData = ((uint16_t)helloworld[txIndex+1] << 8) | helloworld[txIndex];
    txIndex = txIndex + 2;
    if(txIndex>sizeof(helloworld)) txIndex = 0;
    HAL_I2S_Transmit(&hi2s2, &txData, 1, 10);
  }
}
/* USER CODE END 0 */

int main(void)
{

  /* USER CODE BEGIN 2 */
  txIndex = 0;
  HAL_TIM_Base_Start_IT(&htim2);
  /* USER CODE END 2 */

