:::success
Speaker
目的 : 希望能用一變數作為接收sd卡內音樂資料的窗口，此實驗中變數已預先儲存音樂

stm32 Project name : Speaker_no_SDcard 

主筆記 : https://hackmd.io/@SHU-CHI/HJTTt5llxe
:::
[TOC]
# ref
**1.** [ chatgpt music Player ](https://chatgpt.com/share/681c3cbe-4a6c-8000-bc40-94cb45e82f9b)
**2.** [ Youtube / I2S2 Settings ](https://youtu.be/hZZkDwArT-U?si=_X1tvxmbcEyi5eC_)
# Configuration & Pinouts
## I2S2
1. **[ Parameter Settings ]**
![螢幕擷取畫面 2025-05-13 231802](https://hackmd.io/_uploads/HJ90MybZxx.png)

2. **[ DMA Settings]**
![image](https://hackmd.io/_uploads/r1uGo2Fxle.png)

## FATFS ( SD card )

**1. Open SDIO**
![image](https://hackmd.io/_uploads/Bk3RAqTell.png)
* **GPIO Settings**
![image](https://hackmd.io/_uploads/BkD7Gjpxeg.png)
* **DMA Settings**
![image](https://hackmd.io/_uploads/BJTIMj6lex.png)

# 2025.05.11
**2. Open FATFS**
* **Set Defined**
![image](https://hackmd.io/_uploads/Hk3z-jalgg.png)
* **Platform Settings**
![螢幕擷取畫面 2025-05-11 121410](https://hackmd.io/_uploads/SkpCVjTglx.png)

# code 
## 嘗試將gpt提供之程式碼放入main.c
* **main.c** https://github.com/shuchu11/STM32F411/blob/main/Speaker_no_SDcard/main.c
## 1. 將wav音樂放入sd卡 (步驟包含以下)
* **複製想要的音樂yt連結，使用以下網址下載該音樂wav檔**
[線上yt轉wav](https://y2down.cc/zh/youtube-wav.html)
![螢幕擷取畫面 2025-05-11 132920](https://hackmd.io/_uploads/ByoHI2aexe.png)
* **將下載的mp4放入sd卡資料夾 (需使用讀卡機)**
![image](https://hackmd.io/_uploads/B1XqD2peee.png)
## MAX98357A 接上 (LRCLK, BCLK, DIN, GND, VCC)

| MAX98357 腳位 | 說明               | STM32 腳位     | 對應訊號     |
| ----------- | ---------------- | ------------ | -------- |
| **DIN**     | 數位音訊輸入           | **PC3**      | I2S2\_SD |
| **BCLK**    | Bit Clock        | **PB10**     | I2S2\_CK |
| **LRCLK**   | Left/Right Clock | **PB12**     | I2S2\_WS |
| **GND**     | 接地               | GND          | GND      |
| **VCC**     | 電源（3.3V或5V）      | VCC（3.3V/5V） | VCC      |

![image](https://hackmd.io/_uploads/S1Yd02pggl.png)
![image](https://hackmd.io/_uploads/HkNh46algg.png)

## Error & Warning
[github /a1. warning&error ](https://github.com/shuchu11/STM32F411/blob/main/Speaker_no_SDcard/warning%20%26%20error)
| 錯誤訊息                            | 原因                    | 解決方法                                       |
| ------------------------------- | --------------------- | ------------------------------------------ |
| `'res' undeclared`              | `res` 變數未宣告就使用了       | 在 `Play_WAV_File` 中加入 `FRESULT res;`       |
| `'file' undeclared`             | `file` 變數未宣告就使用了      | 加入 `FIL file;`（`ff.h` 定義的 FatFs 文件物件）      |
| `'printf'` implicit declaration | 未包含 `stdio.h`         | 加入 `#include <stdio.h>`                    |
| `'bytesRead' undeclared`        | `bytesRead` 變數未宣告就使用了 | 加入 `UINT bytesRead;`，FatFs 的讀取回傳大小用 `UINT` |

* **solution**
add the code blew into **main.c**
```
#include <stdio.h>      // 修正 printf 警告
#include "ff.h"         // FatFs 函數和型別（FIL, FRESULT 等）

void Play_WAV_File(const char *filename)
{
    FRESULT res;         // 修正錯誤1
    FIL file;            // 修正錯誤2
    UINT bytesRead;      // 修正錯誤4

    // 嘗試打開 WAV 檔
    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK)
    {
        printf("Cannot open file %s\r\n", filename);
        return;
    }

    // 略過 WAV 檔前 44 bytes 的 header
    f_read(&file, audioBuffer, 44, &bytesRead);

    // ...其餘播放邏輯
}

```

--> no sound : 忘記在max98357A裝上喇叭，立刻裝上。
-->成功發出聲音，但為一連串雜音 : 使用Audacity檢查wav格式，若是 mono 音檔但 I2S 設定為 stereo，會造成錯位雜音。
--> <font color = "#f00">**發現正確音檔類型應該為PCM，不是mp4(影片)**</font>: 用microsoft clipchamp使mp4檔案大小小於2MB，接著使用以下連結進行轉換(mp4->PCM)
https://products.aspose.app/slides/zh/video/mp4-to-pcm

* **microsoft clipchamp 畫面**
![螢幕擷取畫面 2025-05-11 213137](https://hackmd.io/_uploads/Sy_IwmAgxe.png)
* **導出編輯後的.mp4，確認大小是否小於2MB，命名為""gummyB --> 點擊 "保存到你的電腦" 生成pcm檔**
![image](https://hackmd.io/_uploads/HkqWOX0xlx.png)

## 2025.05.13
* **將pcm檔放入SD card**
![image](https://hackmd.io/_uploads/Hk_GNAeZgx.png)

--> <font color = "#f00">**再次確認I2S2設定**</font>
| 參數              | 建議設定                     |
| --------------- | ------------------------ |
| **Mode**        | Master Transmit          |
| **Standard**    | I2S Philips              |
| **Data Format** | 16-bit                   |
| **Audio Freq**  | 與 WAV 一致（如 44.1kHz）      |
| **MCLK Output** | **Enable**（MAX98357A 需要） |
| **DMA TX**      | 可先不使用，測試 OK 再啟用          |

* **開啟 "MCLK"**
![image](https://hackmd.io/_uploads/HyjOsRl-ge.png)
* **出現了新腳位 PA3**
![image](https://hackmd.io/_uploads/HyOSpAeZxe.png)

| STM32 腳位 | CubeMX 訊號 | 功能說明               | 連接到 MAX98357A                    |
| -------- | --------- | ------------------ | -------------------------------- |
| **PA3**  | I2S2\_MCK | Master Clock       | **MCLK**（可選但建議接）     |
| **PB10** | I2S2\_CK  | Bit Clock (BCLK)   | **BCLK** ✅           |
| **PB12** | I2S2\_WS  | Word Select (LRCK) | **LRC** ✅            |
| **PC3**  | I2S2\_SD  | Serial Data (DIN)  | **DIN** ✅            |
| —        | —         | GND                | **GND** ✅            |
| —        | —         | VCC 3.3V           | **VIN (3V \~ 5V)** ✅ |

--> <font color = "#f00">**我的MAX98357A只剩下腳位gain and SD , i cannot find MCLK on the MAX98357A** </font>
--> 沒有 MCLK 腳位，(MAX98357A)它會從 BCLK 推算出內部取樣率，<font color = "#f00"> 將MCK設定取消，回復原狀</font>

--><font color = "#f00">**check the code**</font>
* **將 mono 資料「複製成 stereo」，因為I2S2為雙聲道系統，add the code blew into my main.c , edit the original code **
```
#define MONO_BUFFER_SIZE 512
#define STEREO_BUFFER_SIZE (MONO_BUFFER_SIZE * 2)

uint16_t monoBuffer[MONO_BUFFER_SIZE];
uint16_t stereoBuffer[STEREO_BUFFER_SIZE];

while (1) {
    res = f_read(&file, (uint8_t*)monoBuffer, MONO_BUFFER_SIZE * 2, &bytesRead);
    if (res != FR_OK || bytesRead == 0) break;

    int sampleCount = bytesRead / 2; // 16-bit sample = 2 bytes
    for (int i = 0; i < sampleCount; i++) {
        stereoBuffer[i * 2]     = monoBuffer[i]; // Left
        stereoBuffer[i * 2 + 1] = monoBuffer[i]; // Right
    }

    HAL_I2S_Transmit(&hi2s2, stereoBuffer, sampleCount * 2, HAL_MAX_DELAY);
}

```
```
    // 跳過 WAV header
    uint8_t temp[44];
    f_read(&file, temp, 44, &bytesRead);
    
    // ** while(1) **
```
-----

<font color = "#f00">**Fail to  play the music**</font>

----
**刪除 DC card 中pcm檔改成wav檔**
* [pcm to wav](https://www.aconvert.com/tw/audio/mp4-to-wav/)
* **check the real sampling frequency of music "gummyB"**
![image](https://hackmd.io/_uploads/SyNyLrbZgg.png)
**and change the settings in I2S2 of STM32CubeIDE**
![image](https://hackmd.io/_uploads/By34IBZZxl.png)
 
 ----
 <font color = "#f00">**Fail to  play the music**</font>
 
 -----
 **發現音樂gummyB為雙聲道，對程式碼進行更改，刪除原程式碼中將單聲道轉為雙聲道的部分，以下為完整的新程式碼(僅自定函式部分)**
```
#define STEREO_BUFFER_SIZE 1024
uint16_t stereoBuffer[STEREO_BUFFER_SIZE];  // stereo 16-bit = 2 bytes/sample

void Play_WAV_File(const char* filename) {
    FIL file;
    FRESULT res;
    UINT bytesRead;

    res = f_open(&file, filename, FA_READ);
    if (res != FR_OK) {
        printf("Cannot open file %s\r\n", filename);
        return;
    }

    // Skip 44-byte WAV header
    uint8_t temp[44];
    f_read(&file, temp, 44, &bytesRead);

    while (1) {
        res = f_read(&file, (uint8_t*)stereoBuffer, STEREO_BUFFER_SIZE * 2, &bytesRead);
        if (res != FR_OK || bytesRead == 0) break;

        HAL_I2S_Transmit(&hi2s2, stereoBuffer, bytesRead / 2, HAL_MAX_DELAY);
    }

    f_close(&file);
    printf("Playback finished.\r\n");
}

``` 

<font color = "#f00">**3 Error** </font>
```
06:37:20 **** Incremental Build of configuration Debug for project Speaker_no_SDcard ****
make -j20 all 
arm-none-eabi-gcc "../Core/Src/main.c" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"Core/Src/main.d" -MT"Core/Src/main.o" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "Core/Src/main.o"
../Core/Src/main.c:51:21: error: 'MONO_BUFFER_SIZE' undeclared here (not in a function); did you mean 'IS_DMA_BUFFER_SIZE'?
   51 | uint16_t monoBuffer[MONO_BUFFER_SIZE];
      |                     ^~~~~~~~~~~~~~~~
      |                     IS_DMA_BUFFER_SIZE
../Core/Src/main.c:52:23: error: 'STEREO_BUFFER_SIZE' undeclared here (not in a function); did you mean 'IS_DMA_BUFFER_SIZE'?
   52 | uint16_t stereoBuffer[STEREO_BUFFER_SIZE];
      |                       ^~~~~~~~~~~~~~~~~~
      |                       IS_DMA_BUFFER_SIZE
make: *** [Core/Src/subdir.mk:46: Core/Src/main.o] Error 1
"make -j20 all" terminated with exit code 2. Build might be incomplete.

06:37:21 Build Failed. 3 errors, 0 warnings. (took 609ms)

```
* USER CODE BEGIN PV中的原程式碼未刪除導致定義錯誤

**忘記將程式模板中"music.wav"改為"gummyB.wav"**

 ----
 <font color = "#f00">**Fail to  play the music**</font>
 
 -----
 
## 參考以下連結，另開新檔實作
https://www.micropeta.com/video117
:::success
stm32 project : studio2
:::

將**helloworld.h**加入資料夾Src ，並在main.c加入
```
#include "helloworld.h"
extern uint8_t helloworld[];
```

--> <font color = "#ff00ff">**successful** </font> ，Next step I am trying to replace the music. 

music : [YouTube /Little Bee](https://youtu.be/8Eox6Cnlcpk?si=D6WgQFojk8nScXcG)
tool : [Tool : YT to mp3](https://turboscribe.ai/zh-TW/downloader/youtube/mp3)

接下來要將下載的mp3轉為陣列 **(需為雙聲道)**

---
Test 1 : exchanging mp3 file to arrary by mp3decoder and saving as .c file 
---
Test software : CodeBlock 
* [Offical website : CodeBlock](https://sourceforge.net/projects/codeblocks/)
![image](https://hackmd.io/_uploads/ByaXLlXbex.png)


mp3decoder (file): https://github.com/lieff/minimp3/blob/master/minimp3_ex.h

Code:

<details>
<summary>main.c</summary>
    
```c
#include "minimp3_ex.h"

mp3dec_t mp3d;
mp3dec_file_info_t info;

if(mp3dec_load(&mp3d, "your_file.mp3", &info, NULL, NULL)) {
    printf("解碼失敗！\n");
} else {
    printf("取樣率: %d, 樣本數: %d\n", info.hz, info.samples);
    // info.buffer 是 int16_t * 型別, 即 16-bit PCM 資料
    // 你可以用 info.buffer 填入你的陣列
}
```
    
</details>

# 2025.05.16



