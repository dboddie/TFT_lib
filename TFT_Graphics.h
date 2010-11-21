/* 	LCD_GRAPHICS.h -- High-level graphic routines for TFT panel					*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 10/04/2010															*/
#ifndef _GRAPHICS_H_
#define _GRAPHICS_H_

#include <inttypes.h>
#include "TFT_ILI9325.h"

class Graphics
{
	public:
	uint16_t temp;
	Graphics();
	void initialize(uint8_t orient);
	void SetCursor(uint16_t x, uint16_t y);
	void ClearScreen(uint16_t color = WHITE);
	void FastSolidRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
    void DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint8_t fill);
	void DrawRoundRect(uint16_t x, uint16_t y, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t color, uint8_t fill);
    void DrawPixel(uint16_t x, uint16_t y, uint16_t color);
	void DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, uint8_t fill);
	void VerticalScroll(int16_t y);
};

#endif
