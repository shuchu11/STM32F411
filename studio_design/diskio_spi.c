/*
 * diskio_spi.c
 *
 *  Created on: May 3, 2025
 *      Author: user
 */


/*-----------------------------------------------------------------------
 * SPI disk I/O driver for FatFs on STM32
 *-----------------------------------------------------------------------*/

#include "diskio.h"
#include "stm32f4xx_hal.h"
#include "fatfs_sd.h"  // SD卡SPI驅動用

// SPI相關外部宣告 (請依實際情況修改)
extern SPI_HandleTypeDef hspi1;
#define SD_SPI_HANDLE hspi1
#define SD_CS_GPIO_Port GPIOB
#define SD_CS_Pin GPIO_PIN_6

// 狀態旗標
static volatile DSTATUS Stat = STA_NOINIT;

/*-----------------------------------------------------------------------*/
/* 初始化磁碟                                                          */
/*-----------------------------------------------------------------------*/
DSTATUS disk_initialize (BYTE pdrv)
{
    if (pdrv != 0) return STA_NOINIT;

    // SD卡初始化 (使用自行撰寫的fatfs_sd.h內函式)
    if (SD_Init() == 0)
        Stat &= ~STA_NOINIT;
    else
        Stat = STA_NOINIT;

    return Stat;
}


/*-----------------------------------------------------------------------*/
/* 磁碟狀態                                                              */
/*-----------------------------------------------------------------------*/
DSTATUS disk_status (BYTE pdrv)
{
    if (pdrv != 0) return STA_NOINIT;
    return Stat;
}


/*-----------------------------------------------------------------------*/
/* 讀取扇區                                                              */
/*-----------------------------------------------------------------------*/
DRESULT disk_read (BYTE pdrv, BYTE* buff, LBA_t sector, UINT count)
{
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    for (UINT i = 0; i < count; i++)
    {
        if (SD_ReadBlock(buff + (512 * i), sector + i, 512) != 0)
            return RES_ERROR;
    }

    return RES_OK;
}


/*-----------------------------------------------------------------------*/
/* 寫入扇區 (僅當 _USE_WRITE 為 1 時啟用)                                 */
/*-----------------------------------------------------------------------*/
#if _USE_WRITE
DRESULT disk_write (BYTE pdrv, const BYTE* buff, LBA_t sector, UINT count)
{
    if (pdrv != 0 || !count) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    for (UINT i = 0; i < count; i++)
    {
        if (SD_WriteBlock((uint8_t*)buff + (512 * i), sector + i, 512) != 0)
            return RES_ERROR;
    }

    return RES_OK;
}
#endif


/*-----------------------------------------------------------------------*/
/* I/O 控制 (僅當 _USE_IOCTL 為 1 時啟用)                                 */
/*-----------------------------------------------------------------------*/
#if _USE_IOCTL
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    if (pdrv != 0) return RES_PARERR;
    if (Stat & STA_NOINIT) return RES_NOTRDY;

    switch (cmd)
    {
    case CTRL_SYNC:
        return RES_OK;

    case GET_SECTOR_COUNT:
        *(DWORD*)buff = 32768; // 16MB SD卡為例 (請依實際卡片大小修改)
        return RES_OK;

    case GET_SECTOR_SIZE:
        *(WORD*)buff = 512;
        return RES_OK;

    case GET_BLOCK_SIZE:
        *(DWORD*)buff = 8;
        return RES_OK;
    }

    return RES_PARERR;
}
#endif


/*-----------------------------------------------------------------------*/
/* 提供時間戳 (可選，避免出錯)                                            */
/*-----------------------------------------------------------------------*/
DWORD get_fattime (void)
{
    return ((DWORD)(2025 - 1980) << 25)  // 年
         | ((DWORD)5 << 21)              // 月
         | ((DWORD)3 << 16)              // 日
         | ((DWORD)0 << 11)              // 時
         | ((DWORD)0 << 5)               // 分
         | ((DWORD)0 >> 1);              // 秒
}
