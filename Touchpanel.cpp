/* 	touch_panel.cpp -- touch panel routines - ADS7864 compatible				*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 12/04/2010															*/

#include <avr/eeprom.h>
#include <TFT_Arduino.h>
#include "touchpanel.h"

touchpanel::touchpanel()
{
	get_matrix();
}

void touchpanel::init(void)
{
	
	SPI_PORT |= (1<<DD_MOSI) | (1<<DD_SCK);
	SPI_DDR   |= (1<<DD_MOSI) | (1<<DD_SCK)|(1<<SPI_SS);	// set spi direction

	TP1;
	TP_CS_DDR |= (1<<TP_CS_PIN);          		// Turns on CS pin as output

	SPCR	   = (1<<SPE)|(1<<MSTR); 			// Initialize SPI port (Mode 0)
	SPSR	   = (1<<SPI2X);					// SPI double speed settings

}

uint8_t spi_rcv(uint8_t dat)
{
	
	SPDR=dat; 
	while(!(SPSR& (1<<SPIF)));
 
	return SPDR;
}

void touchpanel::get_matrix(void)
{
	eeprom_read_block((void*)&matrix, (void*)EEPROM_ADDRESS, sizeof(MATRIX));
}	
void touchpanel::save_matrix(void)
{
	eeprom_write_block((void*)&matrix, (void*)EEPROM_ADDRESS, sizeof(MATRIX));
}

POINT* touchpanel::read_tp_point(void)
{
	
	uint8_t spi_ctrl, cmd;
	uint8_t tp_dat[4];
	
	spi_ctrl = SPCR;
	SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0); // reduce frequency to Fosc/8 = 2Mhz

	TP0;
	cmd=TOUCH_PANEL_START_BIT|TOUCH_PANEL_Y_PLUS|TOUCH_PANEL_12BIT_SAMPLE| TOUCH_PANEL_DIFF_MODE;
  spi_rcv(cmd);
	tp_dat[0] = spi_rcv(0x0); // high 7 bit Y reading
	tp_dat[1] = spi_rcv(0x0);
	cmd=TOUCH_PANEL_START_BIT|TOUCH_PANEL_X_PLUS|TOUCH_PANEL_12BIT_SAMPLE| TOUCH_PANEL_DIFF_MODE ;
	spi_rcv(cmd);
	tp_dat[2] = spi_rcv(0x0);
	tp_dat[3] = spi_rcv(0x0);
	
	tp_point.x = ((uint16_t) tp_dat[0]<<5) | ((uint16_t)tp_dat[1]>>3);
	tp_point.y = 4095- ((uint16_t) tp_dat[2]<<5) | ((uint16_t)tp_dat[3]>>3);
	
	
	TP1;
	SPCR = spi_ctrl;

	if(tp_point.y<TP_MIN_Y) { tp_point.x = 0; tp_point.y=0;}
	return &tp_point;
}


// calculate calibrated point

POINT* touchpanel::read_lcd_point(void)
{
	read_tp_point();

	lcd_point.x = ((matrix.An * tp_point.x) + (matrix.Bn * tp_point.y) + matrix.Cn) / matrix.Divider;
	lcd_point.y = ((matrix.Dn * tp_point.x) + (matrix.En * tp_point.y) + matrix.Fn) / matrix.Divider;


	if (lcd_point.x < 0)
		lcd_point.x = 0;
        
	if (lcd_point.y < 0)
		lcd_point.y = 0;
		
	return &lcd_point;
}
