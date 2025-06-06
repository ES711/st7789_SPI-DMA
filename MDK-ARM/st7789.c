#include "st7789.h"
#include "spi.h"
extern SPI_HandleTypeDef hspi1; 

#define CS_PORT  GPIOA
#define CS_PIN   GPIO_PIN_4 //CS PA4
#define DC_PORT  GPIOB
#define DC_PIN   GPIO_PIN_5 //DC PB5
#define RST_PORT GPIOD
#define RST_PIN  GPIO_PIN_14 //RST PD14
#define BL_PORT  GPIOD
#define BL_PIN   GPIO_PIN_15 //PD15

/*
DC -> receive data or cmd
0 -> cmd
1 -> data
*/

void ST7789_WriteCommand(uint8_t cmd) {
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_RESET); // DC = 0
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); // CS = 0
    HAL_SPI_Transmit(&hspi1, &cmd, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);   // CS = 1
}

void ST7789_WriteData(uint8_t data) {
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);   // DC = 1
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); // CS = 0
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);   // CS = 1
}

void ST7789_Init(void) {
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_RESET); //RST = 0
    HAL_Delay(10);
    HAL_GPIO_WritePin(RST_PORT, RST_PIN, GPIO_PIN_SET); //RST = 1
    HAL_Delay(120);

    HAL_GPIO_WritePin(BL_PORT, BL_PIN, GPIO_PIN_SET); //BL = 1

    
    ST7789_WriteCommand(0x11); //disable sleep
    HAL_Delay(120);
    ST7789_WriteCommand(0x36); 
    ST7789_WriteData(0x00);    
    ST7789_WriteCommand(0x3A); //RGB 5-6-5
    ST7789_WriteData(0x05); //RGB 5-6-5
    ST7789_WriteCommand(0x29); //display on
}

void ST7789_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
    ST7789_WriteCommand(0x2A); 
    ST7789_WriteData(x0 >> 8);
    ST7789_WriteData(x0 & 0xFF);
    ST7789_WriteData(x1 >> 8);
    ST7789_WriteData(x1 & 0xFF);

    ST7789_WriteCommand(0x2B); 
    ST7789_WriteData(y0 >> 8);
    ST7789_WriteData(y0 & 0xFF);
    ST7789_WriteData(y1 >> 8);
    ST7789_WriteData(y1 & 0xFF);

    ST7789_WriteCommand(0x2C); 
}

void ST7789_FillScreen(uint16_t color) {
    ST7789_SetWindow(0, 0, 239, 319);
    HAL_GPIO_WritePin(DC_PORT, DC_PIN, GPIO_PIN_SET);   // DC = 1
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_RESET); // CS = 0
    for (uint32_t i = 0; i < 240 * 320; i++) {
        HAL_SPI_Transmit(&hspi1, (uint8_t*)&color, 2, HAL_MAX_DELAY);
    }
    HAL_GPIO_WritePin(CS_PORT, CS_PIN, GPIO_PIN_SET);   // CS = 1
}