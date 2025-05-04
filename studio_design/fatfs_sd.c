
/* fatfs_sd.c - SPI SD Card driver source */
#include "fatfs_sd.h"
#include "spi.h"

#define SD_DUMMY_BYTE 0xFF

static uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc);

static void SD_Select(void)
{
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
}

static void SD_Unselect(void)
{
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
}

static uint8_t SD_SPI_Transmit(uint8_t data)
{
    uint8_t recv;
    HAL_SPI_TransmitReceive(&hspi1, &data, &recv, 1, HAL_MAX_DELAY);
    return recv;
}

static void SD_SPI_SendBlock(const uint8_t *buff, uint16_t len)
{
    HAL_SPI_Transmit(&hspi1, (uint8_t*)buff, len, HAL_MAX_DELAY);
}

static void SD_SPI_ReceiveBlock(uint8_t *buff, uint16_t len)
{
    HAL_SPI_Receive(&hspi1, buff, len, HAL_MAX_DELAY);
}

uint8_t SD_Init(void)
{
    uint8_t i, response;
    SD_Unselect();
    for (i = 0; i < 10; i++)
        SD_SPI_Transmit(SD_DUMMY_BYTE);

    SD_Select();
    for (i = 0; i < 100; i++) {
        response = SD_SendCommand(0x40 | 0, 0x00000000, 0x95);
        if (response == 0x01) break;
    }
    SD_Unselect();

    return (response == 0x01) ? 0 : 1;
}

uint8_t SD_ReadBlock(uint8_t *buff, uint32_t sector, uint16_t count)
{
    uint8_t token;
    sector *= 512;

    SD_Select();

    if (SD_SendCommand(0x40 | 17, sector, 0xFF) != 0x00) {
        SD_Unselect();
        return 1;
    }

    while ((token = SD_SPI_Transmit(SD_DUMMY_BYTE)) == 0xFF);

    if (token != 0xFE) {
        SD_Unselect();
        return 2;
    }

    SD_SPI_ReceiveBlock(buff, 512);
    SD_SPI_Transmit(SD_DUMMY_BYTE);
    SD_SPI_Transmit(SD_DUMMY_BYTE);

    SD_Unselect();
    return 0;
}

uint8_t SD_WriteBlock(const uint8_t *buff, uint32_t sector, uint16_t count)
{
    sector *= 512;

    SD_Select();

    if (SD_SendCommand(0x40 | 24, sector, 0xFF) != 0x00) {
        SD_Unselect();
        return 1;
    }

    SD_SPI_Transmit(0xFE);
    SD_SPI_SendBlock(buff, 512);
    SD_SPI_Transmit(SD_DUMMY_BYTE);
    SD_SPI_Transmit(SD_DUMMY_BYTE);

    if ((SD_SPI_Transmit(SD_DUMMY_BYTE) & 0x1F) != 0x05) {
        SD_Unselect();
        return 2;
    }

    while (SD_SPI_Transmit(SD_DUMMY_BYTE) == 0x00);

    SD_Unselect();
    return 0;
}

static uint8_t SD_SendCommand(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t response, frame[6];

    frame[0] = cmd;
    frame[1] = (uint8_t)(arg >> 24);
    frame[2] = (uint8_t)(arg >> 16);
    frame[3] = (uint8_t)(arg >> 8);
    frame[4] = (uint8_t)(arg);
    frame[5] = crc;

    SD_SPI_SendBlock(frame, 6);

    for (uint8_t i = 0; i < 10; i++) {
        response = SD_SPI_Transmit(SD_DUMMY_BYTE);
        if ((response & 0x80) == 0) break;
    }

    return response;
}
