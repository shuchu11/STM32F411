#include "minimp3.h"
#include "minimp3_ex.h"
#include "i2s.h"           // HAL_I2S_Transmit_DMA 與 hi2s3 宣告
#include "fatfs.h"         // FATFS 結構體與 f_mount、f_open 等（minimp3_ex 用得到）
#include "main.h"          // 通常包含 Error_Handler 與其他全域變數
#include <string.h>

void play_mp3_stream(const char* filename)
{
    if (mp3dec_ex_open(&dec, filename, MP3D_SEEK_TO_SAMPLE) != 0) {
        Error_Handler();  // 無法開啟或格式錯誤
    }

    while (1) {
        int samples = mp3dec_ex_read(&dec, pcm_buffer, PCM_BUFFER_SIZE);
        if (samples <= 0) break;

        // stereo: 每個樣本包含 L+R，因此 word 數量 = samples × channels
        HAL_I2S_Transmit_DMA(&hi2s3, (uint16_t*)pcm_buffer, samples);
        while (HAL_I2S_GetState(&hi2s3) != HAL_I2S_STATE_READY);
    }

    mp3dec_ex_close(&dec);
}
