#ifndef ST7789_H
#define ST7789_H

#include "stm32f4xx_hal.h"

void ST7789_Init(void);
void ST7789_FillScreen(uint16_t color);
void ST7789_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);

#endif