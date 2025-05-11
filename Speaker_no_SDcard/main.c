
/* USER CODE BEGIN PD */
#define BUFFER_SIZE 512
/* USER CODE END PD */

/* USER CODE BEGIN PV */
uint8_t audioBuffer[BUFFER_SIZE];
/* USER CODE END PV */

/* USER CODE BEGIN PFP */
void Play_WAV_File(const char* filename);

void Play_WAV_File(const char* filename) {

    // 開啟檔案
    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        printf("Cannot open file %s\r\n", filename);
        return;
    }

    // 略過 WAV檔頭 (一般為44 bytes)
    f_read(&file, audioBuffer, 44, &bytesRead);

    // 讀取並播放
    while (1) {
        res = f_read(&file, audioBuffer, BUFFER_SIZE, &bytesRead);
        if (res != FR_OK || bytesRead == 0) {
            break;  // 播放完畢
        }

        // 播放音訊資料 (用 DMA 非常重要，避免卡頓)
        if (HAL_I2S_Transmit(&hi2s2, (uint16_t*)audioBuffer, bytesRead / 2, HAL_MAX_DELAY) != HAL_OK) {
            printf("I2S Transmit Error\r\n");
            break;
        }
    }

    // 關閉檔案
    f_close(&file);
    printf("Playback finished.\r\n");
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
extern I2S_HandleTypeDef hi2s2;
extern DMA_HandleTypeDef hdma_spi2_tx;

// FATFS 物件
FIL file;
UINT bytesRead;
FRESULT res;

/* USER CODE END 0 */

  /* USER CODE BEGIN 2 */
  // Mount SD卡
   FATFS fs;
   f_mount(&fs, "", 0);

   // 播放音樂
   Play_WAV_File("music.wav");

   // Unmount
   f_mount(NULL, "", 0);

  /* USER CODE END 2 */

