/* 	TFT_ARDUINO.h -- Low-level Arduion 168/328 port definition for TFT panel	*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 10/04/2010															*/


#ifndef _TFT_ARDUINO_H_
#define _FTF_ARDUINO_H_

#include <avr/io.h>

#define LCD_LATCH_DDR	DDRB
#define LCD_LATCH_PORT	PORTB
#define LCD_LATCH_PIN	0

/* no backlight control on Arduino platform */
//#define LCD_BL_DDR		DDRC
//#define LCD_BL_PORT		PORTC
//#define LCD_BL_PIN		0

#define LCD_CTRL_DDR	DDRC
#define LCD_RS_PORT		PORTC
#define LCD_RS_PIN		1

#define LCD_CS_PORT		PORTC
#define LCD_CS_PIN		2


#define LCD_WR_PORT		PORTC
#define LCD_WR_PIN		0

#define LCD_DAT_DDR		DDRD
#define LCD_DAT_PORT	PORTD
#define LCD_DAT_PIN		PIND


#define TP_CS_DDR	  	DDRB
#define TP_CS_PORT		PORTB
#define TP_CS_PIN		1

#define MMC_CS_DDR		DDRB
#define MMC_CS_PORT		PORTB
#define MMC_CS_PIN		2


//PORT deifinition for Touchpanel
//SPI configuration (Platform dependent)
#define SPI_DDR		DDRB
#define SPI_PORT 	PORTB
#define SPI_SS		2
#define DD_MOSI		3
#define DD_MISO		4
#define DD_SCK		5




#define sbi(port,bitnum)		port |= _BV(bitnum)
#define cbi(port,bitnum)		port &= ~(_BV(bitnum))

#define CS0 cbi(LCD_CS_PORT,LCD_CS_PIN)
#define CS1 sbi(LCD_CS_PORT,LCD_CS_PIN)
#define RS0 cbi(LCD_RS_PORT,LCD_RS_PIN)
#define RS1 sbi(LCD_RS_PORT,LCD_RS_PIN)
#define WR0 cbi(LCD_WR_PORT,LCD_WR_PIN)
#define WR1 sbi(LCD_WR_PORT,LCD_WR_PIN)
#define MMC0 cbi(MMC_CS_PORT, MMC_CS_PIN)
#define MMC1 sbi(MMC_CS_PORT, MMC_CS_PIN)
#define TP0	cbi(TP_CS_PORT, TP_CS_PIN)
#define TP1 sbi(TP_CS_PORT, TP_CS_PIN)



#define LATCH1  LCD_LATCH_PORT |=  (1<<LCD_LATCH_PIN)
#define LATCH0  LCD_LATCH_PORT &= ~(1<<LCD_LATCH_PIN)
#define LATCHOUT LCD_LATCH_DDR  |=  (1<<LCD_LATCH_PIN)


#define LCD_BL_OFF()	
#define LCD_BL_ON()		

// data output with a latch

#define LCD_SET_DATH(x)	LATCH1; \
						LCD_DAT_PORT = x; \
						LATCH0

#define LCD_SET_DATL(x)	LCD_DAT_PORT = x


// 8bit interface only supports data output 
#define DATAOUT	LCD_DAT_DDR=0xFF	



#endif 