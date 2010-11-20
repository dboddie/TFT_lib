/* 	LCD_ILI9325.cpp -- Low-level LCD (based on ILI9325)	 function definition 	*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 10/04/2010															*/

#include <avr/io.h>
#include <avr/pgmspace.h>
#define F_CPU 16000000L
#include <util/delay.h>
#include "TFT_ILI9325.h"

#define REG_SIZE 60

uint16_t _color;
uint8_t _orientation;

uint8_t lcd_reg_addr[REG_SIZE] PROGMEM={
    0x00E3,             // 0x3008 set the internal timing
    0x00E7,             // 0x0012 set the internal timing
    0x00EF,             // 0x1231 set the internal timing
    LCD_START_OSC,      // 1 start oscillator
    0xFF,               // Delay 50

    LCD_DRIV_OUT_CTRL,      // 0x0100				    //set SS, // SM
    LCD_DRIV_WAV_CTRL,      // 0x0700			        //set 1 line inversion
    LCD_ENTRY_MOD,          // 0x0030	            //set GRAM write direction, // BGR=0
    LCD_RESIZE_CTRL,        // 0x0000					    //no resizing
    LCD_DISP_CTRL2,         // 0x0202				    //front & back porch periods = 2
    LCD_DISP_CTRL3,         // 0x0000
    LCD_DISP_CTRL4,         // 0x0000
    LCD_RGB_DISP_IF_CTRL1,  // 0x0000		    //select system interface				
    LCD_FRM_MARKER_POS,     // 0x0000
    LCD_RGB_DISP_IF_CTRL2,  // 0x0000

    LCD_POW_CTRL1, // 0x0000
    LCD_POW_CTRL2, // 0x0007
    LCD_POW_CTRL3, // 0x0000
    LCD_POW_CTRL4, // 0x0000
    0xFF, // Delay 200

    LCD_POW_CTRL1, // 0x1690
    LCD_POW_CTRL2, // 0x0227 //0x0137, // 					
    0xFF, // Delay 50

    LCD_POW_CTRL3, // 0x001A //0x013C,
    0xFF, // Delay 50

    LCD_POW_CTRL4, // 0x1800 //0x1400,
    LCD_POW_CTRL7, // 0x002A //0x0007,
    0xFF, // Delay 50

    LCD_GAMMA_CTRL1, // 0x0007
    LCD_GAMMA_CTRL2, // 0x0605
    LCD_GAMMA_CTRL3, // 0x0106
    LCD_GAMMA_CTRL4, // 0x0206
    LCD_GAMMA_CTRL5, // 0x0808
    LCD_GAMMA_CTRL6, // 0x0007
    LCD_GAMMA_CTRL7, // 0x0201
    LCD_GAMMA_CTRL8, // 0x0007
    LCD_GAMMA_CTRL9, // 0x0602
    LCD_GAMMA_CTRL10, // 0x0808

    LCD_GRAM_HOR_AD, // 0x0000
    LCD_GRAM_VER_AD, // 0x0000
    LCD_HOR_START_AD, // 0x0000
    LCD_HOR_END_AD, // 0x00EF
    LCD_VER_START_AD, // 0x0000
    LCD_VER_END_AD, // 0x013F
    LCD_GATE_SCAN_CTRL1, // 0xA700
    LCD_GATE_SCAN_CTRL2, // 0x0001
    LCD_GATE_SCAN_CTRL3, // 0x0000

    LCD_PART_IMG1_DISP_POS, // 0x0000
    LCD_PART_IMG1_START_AD, // 0x0000
    LCD_PART_IMG1_END_AD, // 0x0000
    LCD_PART_IMG2_DISP_POS, // 0x0000
    LCD_PART_IMG2_START_AD, // 0x0000
    LCD_PART_IMG2_END_AD, // 0x0000

    LCD_PANEL_IF_CTRL1, // 0x0010
    LCD_PANEL_IF_CTRL2, // 0x0000
    LCD_PANEL_IF_CTRL3, // 0x0003
    LCD_PANEL_IF_CTRL4, // 0x0110
    LCD_PANEL_IF_CTRL5, // 0x0000
    LCD_PANEL_IF_CTRL6, // 0x0000

    LCD_DISP_CTRL1, // 0x0133
};


uint16_t lcd_reg_data[REG_SIZE] PROGMEM={
    /*0x00E3*/              0x3008,     //set the internal timing
    /*0x00E7*/              0x0012,     //set the internal timing
    /*0x00EF*/              0x1231,     //set the internal timing
    /*LCD_START_OSC*/       1,          //start oscillator
    /*_delay_ms*/           50,

    /*LCD_DRIV_OUT_CTRL*/   0x0100,				    //set SS*/SM
    /*LCD_DRIV_WAV_CTRL*/   0x0700,			        //set 1 line inversion
    /*LCD_ENTRY_MOD*/       0x1030,	            //set GRAM write direction*/BGR=0
    /*LCD_RESIZE_CTRL*/     0x0000,					    //no resizing
    /*LCD_DISP_CTRL2*/      0x0202,				    //front & back porch periods = 2
    /*LCD_DISP_CTRL3*/      0x0000,
    /*LCD_DISP_CTRL4*/      0x0000,
    /*LCD_RGB_DISP_IF_CTRL1*/0x0000,		    //select system interface				
    /*LCD_FRM_MARKER_POS*/  0x0000,
    /*LCD_RGB_DISP_IF_CTRL2*/0x0000,

    /*LCD_POW_CTRL1*/       0x0000,
    /*LCD_POW_CTRL2*/       0x0007,
    /*LCD_POW_CTRL3*/       0x0000,
    /*LCD_POW_CTRL4*/       0x0000,
    /*_delay_ms*/           200,

    /*LCD_POW_CTRL1*/       0x1690,
    /*LCD_POW_CTRL2*/       0x0227, //0x0137*/					
    /*_delay_ms*/           50,

    /*LCD_POW_CTRL3*/       0x001A, //0x013C,
    /*_delay_ms*/           50,

    /*LCD_POW_CTRL4*/       0x1800, //0x1400,
    /*LCD_POW_CTRL7*/       0x002A, //0x0007,
    /*_delay_ms*/           50,

    /*LCD_GAMMA_CTRL1*/     0x0000,//0x0007,
    /*LCD_GAMMA_CTRL2*/     0x0000,//0x0605,
    /*LCD_GAMMA_CTRL3*/     0x0000,//0x0106,
    /*LCD_GAMMA_CTRL4*/     0x0206,
    /*LCD_GAMMA_CTRL5*/     0x0808,
    
    /*LCD_GAMMA_CTRL6*/     0x0007,
    /*LCD_GAMMA_CTRL7*/     0x0201,
    /*LCD_GAMMA_CTRL8*/     0x0000,//0x0007,
    /*LCD_GAMMA_CTRL9*/     0x0000,//0x0602,
    /*LCD_GAMMA_CTRL10*/    0x0000,//0x0808,

    /*LCD_GRAM_HOR_AD*/     0x0000,
    /*LCD_GRAM_VER_AD*/     0x0000,
    /*LCD_HOR_START_AD*/    0x0000,
    /*LCD_HOR_END_AD*/      0x00EF,
    /*LCD_VER_START_AD*/    0x0000,
    /*LCD_VER_END_AD*/      0x013F,
    /*LCD_GATE_SCAN_CTRL1*/ 0xA700,
    /*LCD_GATE_SCAN_CTRL2*/ 0x0003, // 0x0001
    /*LCD_GATE_SCAN_CTRL3*/ 0x0000, // 0x0000,

    /*LCD_PART_IMG1_DISP_POS*/  0x0000,
    /*LCD_PART_IMG1_START_AD*/  0x0000,
    /*LCD_PART_IMG1_END_AD*/    0x0000,
    /*LCD_PART_IMG2_DISP_POS*/  0x0000,
    /*LCD_PART_IMG2_START_AD*/  0x0000,
    /*LCD_PART_IMG2_END_AD*/    0x0000,

    /*LCD_PANEL_IF_CTRL1*/  0x0010,
    /*LCD_PANEL_IF_CTRL2*/  0x0000,
    /*LCD_PANEL_IF_CTRL3*/  0x0003,
    /*LCD_PANEL_IF_CTRL4*/  0x0110,
    /*LCD_PANEL_IF_CTRL5*/  0x0000,
    /*LCD_PANEL_IF_CTRL6*/  0x0000,

    /*LCD_DISP_CTRL1*/      0x0133,

};


void LCD::set_Reg_Address(uint16_t addr){
	CS0;
	RS0;
	LCD_SET_DATH(addr>>8);
	LCD_SET_DATL(addr);
	
	WR0;
	WR1;
	RS1;
	CS1;

}

void LCD::set_Reg(uint16_t addr, uint16_t data){
	set_Reg_Address(addr);
	CS0;
	RS1;
	LCD_SET_DATH(data>>8);
	LCD_SET_DATL( data);
	WR0;
	WR1;
	CS1;
}



	
void LCD::init(uint8_t orientation){

	uint8_t i; 
	uint16_t addr;
	uint16_t dat;
	
	_orientation = orientation;
	// port setup
	CS1; WR1; RS1; LATCH0; TP1; MMC1;
	LCD_CTRL_DDR |= (1<<LCD_RS_PIN)|(1<<LCD_CS_PIN)|(1<<LCD_WR_PIN);
	TP_CS_DDR |=(1<<TP_CS_PIN);
	MMC_CS_DDR |= (1<<MMC_CS_PIN);
	DATAOUT; LATCHOUT;
	
	// initialize LCD
	for(i=0; i<REG_SIZE; i++){
	
       addr = pgm_read_byte(&lcd_reg_addr[i]);
       dat = pgm_read_word(&lcd_reg_data[i]);
      
        if (addr == 0xFF)
            _delay_ms(dat);
        else
            set_Reg(addr, dat);
    }
    
    //if (_orientation == LCD_VERTICAL)
    // 	set_Reg( LCD_ENTRY_MOD, 0x0030);
	
}	
	

uint16_t LCD::GetWidth(){
	if (_orientation == LCD_HORIZONTAL)
		return LCD_WIDTH;
	else
		return LCD_HEIGHT;
}

uint16_t LCD::GetHeight(){
	if (_orientation == LCD_HORIZONTAL)
		return LCD_HEIGHT;
	else
		return LCD_WIDTH;
}
    

void LCD::SetOrientation(uint8_t orient){
	_orientation=orient;
}
    
uint16_t LCD::GetColor(void){
	return _color;
}


uint8_t LCD::GetOrientation(){
	return _orientation;
}
    
void LCD::SetColor(uint16_t color){
	_color = color;
}

void LCD::resetArea(){
	setArea(0,0,GetWidth()-1, GetHeight()-1);
}

void LCD::setArea(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if(_orientation == LCD_HORIZONTAL){
		set_Reg(LCD_HOR_START_AD, y1);
		set_Reg(LCD_HOR_END_AD, y2);
		set_Reg(LCD_VER_START_AD, x1);
		set_Reg(LCD_VER_END_AD, x2);
	}
	else{
		set_Reg(LCD_HOR_START_AD, x1);
		set_Reg(LCD_HOR_END_AD, x2);
		set_Reg(LCD_VER_START_AD, y1);
		set_Reg(LCD_VER_END_AD, y2);
	}
	gotoXY(x1,y1);
}
	

void LCD::fill(uint16_t x_count, uint16_t y_count, uint16_t color){

	register uint16_t i, j;
	DATAOUT;
	CS0;
	LCD_SET_DATH( color>>8);
	LCD_SET_DATL( color);
	
	for(i=0;i<y_count+1; i++){
		for(j=0; j<x_count+1; j++){
			WR0;
			WR1;
		}
	}
	CS1;
}


void LCD::gotoXY(uint16_t x, uint16_t y){
	if(_orientation == LCD_HORIZONTAL){
		set_Reg(LCD_GRAM_HOR_AD, y);
		set_Reg(LCD_GRAM_VER_AD, x);
	}
	else{
		set_Reg(LCD_GRAM_HOR_AD, x);
		set_Reg(LCD_GRAM_VER_AD, y);
	}
		
    set_Reg_Address(LCD_RW_GRAM);
}

void LCD::putPixel(uint16_t x, uint16_t y, uint16_t color){
	gotoXY(x,y);
	CS0;
	LCD_SET_DATH( color>>8);
	LCD_SET_DATL( color);
	WR0;
	WR1;
	CS1;
}



void LCD::Scroll(int y){
	
	while(y>320) y-=320;
	while(y<0) y+=320;

	set_Reg(LCD_GATE_SCAN_CTRL3, y);
}
	



