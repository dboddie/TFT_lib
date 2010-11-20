/* 	LCD_ILI9325.h -- Low-level LCD (based on ILI9325)	 function definition 	*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 10/04/2010															*/
#ifndef _LCD_ILI9325_H_
#define _LCD_ILI9325_H_

#include <inttypes.h>
#include <avr/pgmspace.h>
// Chip specific includes
//#if defined(__AVR_ATmega1280__)
//#include "TFT_Mega.h"		// For the Arduino Mega board  
//#else
#include "TFT_Arduino.h"  // For the Arduino 168/328 boards
//#endif
	
#define LCD_HEIGHT 240
#define LCD_WIDTH	320
	
//ILI9325 register definitions

#define LCD_START_OSC			0x00
#define LCD_DRIV_OUT_CTRL		0x01
#define LCD_DRIV_WAV_CTRL		0x02
#define LCD_ENTRY_MOD			0x03
#define LCD_RESIZE_CTRL			0x04
#define LCD_DISP_CTRL1			0x07
#define LCD_DISP_CTRL2			0x08
#define LCD_DISP_CTRL3			0x09
#define LCD_DISP_CTRL4			0x0A
#define LCD_RGB_DISP_IF_CTRL1	0x0C
#define LCD_FRM_MARKER_POS		0x0D
#define LCD_RGB_DISP_IF_CTRL2	0x0F
#define LCD_POW_CTRL1			0x10
#define LCD_POW_CTRL2			0x11
#define LCD_POW_CTRL3			0x12
#define LCD_POW_CTRL4			0x13
#define LCD_GRAM_HOR_AD			0x20
#define LCD_GRAM_VER_AD			0x21
#define LCD_RW_GRAM				0x22
#define LCD_POW_CTRL7			0x29
#define LCD_FRM_RATE_COL_CTRL	0x2B
#define LCD_GAMMA_CTRL1			0x30
#define LCD_GAMMA_CTRL2			0x31
#define LCD_GAMMA_CTRL3			0x32
#define LCD_GAMMA_CTRL4			0x35 
#define LCD_GAMMA_CTRL5			0x36
#define LCD_GAMMA_CTRL6			0x37
#define LCD_GAMMA_CTRL7			0x38
#define LCD_GAMMA_CTRL8			0x39
#define LCD_GAMMA_CTRL9			0x3C
#define LCD_GAMMA_CTRL10			0x3D
#define LCD_HOR_START_AD			0x50
#define LCD_HOR_END_AD			0x51
#define LCD_VER_START_AD			0x52
#define LCD_VER_END_AD			0x53
#define LCD_GATE_SCAN_CTRL1		0x60
#define LCD_GATE_SCAN_CTRL2		0x61
#define LCD_GATE_SCAN_CTRL3		0x6A
#define LCD_PART_IMG1_DISP_POS	0x80
#define LCD_PART_IMG1_START_AD	0x81
#define LCD_PART_IMG1_END_AD		0x82
#define LCD_PART_IMG2_DISP_POS	0x83
#define LCD_PART_IMG2_START_AD	0x84
#define LCD_PART_IMG2_END_AD		0x85
#define LCD_PANEL_IF_CTRL1		0x90
#define LCD_PANEL_IF_CTRL2		0x92
#define LCD_PANEL_IF_CTRL3		0x93
#define LCD_PANEL_IF_CTRL4		0x95
#define LCD_PANEL_IF_CTRL5		0x97
#define LCD_PANEL_IF_CTRL6		0x98
#define LCD_DELAY 0xFF

#define LCD_HORIZONTAL	0
#define LCD_VERTICAL	1

// color definition
#define _RGB565(r, g, b) ((uint16_t)(((r >> 3) << 11)| ((g >> 2) << 5)| (b >> 3)))

#define BLACK                       _RGB565(0x00, 0x00, 0x00)
#define WHITE                       _RGB565(0xFF, 0xFF, 0xFF)
#define RED                         _RGB565(0xFF, 0x00, 0x00)
#define GREEN                       _RGB565(0x00, 0xFF, 0x00)
#define BLUE                        _RGB565(0x00, 0x00, 0xFF)
#define YELLOW                      _RGB565(0xFF, 0xFF, 0x00)
#define MAGENTA                     _RGB565(0xFF, 0x00, 0xFF)
#define CYAN                        _RGB565(0x00, 0xFF, 0xFF)
#define GRAY                        _RGB565(0x80, 0x80, 0x40)
#define SILVER                      _RGB565(0xA0, 0xA0, 0x80)
#define GOLD                        _RGB565(0xA0, 0xA0, 0x40)





class LCD
{
	
  	
  public:
    static void set_Reg_Address(uint16_t addr);
	static void set_Reg(uint16_t addr, uint16_t data); 
	
 // basic hardware interface code, called from high-level application code
  static void init(uint8_t orientation=LCD_HORIZONTAL);
  static uint16_t GetWidth();
  static uint16_t GetHeight();
  static void SetOrientation(uint8_t orient);	
  static uint8_t GetOrientation();      
  static void SetColor(uint16_t color);
  static uint16_t GetColor(void);
	static void resetArea();
  static void setArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
  static void gotoXY(uint16_t x, uint16_t y);
  static void putPixel(uint16_t x, uint16_t y, uint16_t color);
  static void fill(uint16_t x_count, uint16_t y_count, uint16_t color);
  static void Scroll(int y);
};

#endif
