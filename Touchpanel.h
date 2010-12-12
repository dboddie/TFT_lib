/* 	touch_panel.h -- touch panel routines - ADS7864 compatible					*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 12/04/2010											*/
#ifndef _TOUCHPANEL_H_
#define _TOUCHPANEL_H_

#include <inttypes.h>




// ADS7864 compatiable commands 
#define TOUCH_PANEL_START_BIT    0x80

#define TOUCH_PANEL_TEMP0        0x00
#define TOUCH_PANEL_X_PLUS       0x10
#define TOUCH_PANEL_VBAT         0x20
#define TOUCH_PANEL_Z1_POS       0x30
#define TOUCH_PANEL_Z2_POS       0x40
#define TOUCH_PANEL_Y_PLUS       0x50
#define TOUCH_PANEL_TEMP1        0x70

#define TOUCH_PANEL_ADC3         0x20
#define TOUCH_PANEL_ADC4         0x60

#define TOUCH_PANEL_12BIT_SAMPLE 0x0
#define TOUCH_PANEL_8BIT_SAMPLE  0x8

#define TOUCH_PANEL_DIFF_MODE    0x0
#define TOUCH_PANEL_SINGLE_MODE  0x4

#define TOUCH_PANEL_PWD_ENBLE    0x0
#define TOUCH_PANEL_IRQ_DISABLE  0x1
#define TOUCH_PANEL_PWD_DISABLE  0x3
#define TOUCH_PANEL_PWD_RESET    0x2

// minimum tp_y as a valid touch

#define TP_MIN_Y 100	
#define EEPROM_ADDRESS 0x0

typedef struct Point 
{
	long x;
	long y;
} POINT ;



typedef struct Matrix 
{
	long An;
	long Bn;
	long Cn;
	long Dn;
	long En;
	long Fn;
	long Divider;
} MATRIX ;

class touchpanel
{
	public:
		POINT tp_point; 		// raw point
		POINT lcd_point;		// calibrated pint
		Matrix matrix;			// calibrate matrix
		
		touchpanel();
		void init(void);
		void get_matrix(void);
		void save_matrix(void);
		POINT* read_tp_point(void);
		POINT* read_lcd_point(void);
	
};

#endif
