#include <TFT_Font.h>
#include <TFT_Graphics.h>
#include <Touchpanel.h>
#include <f12x12.h>
#include <stdlib.h>


#define TP_STABLE_COUNT 1

Graphics tft;
TFT_Font font;
touchpanel tp;


int setCalibrationMatrix( POINT * displayPtr,
                          POINT * screenPtr,
                          MATRIX * matrixPtr)
{

    int  retValue = 1 ;



    matrixPtr->Divider = ((screenPtr[0].x - screenPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                         ((screenPtr[1].x - screenPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

    if ( matrixPtr->Divider == 0 )
    {
        retValue = 0 ;
    }
    else
    {
        matrixPtr->An = ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].x - displayPtr[2].x) * (screenPtr[0].y - screenPtr[2].y)) ;

        matrixPtr->Bn = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].x - displayPtr[2].x)) -
                        ((displayPtr[0].x - displayPtr[2].x) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Cn = (screenPtr[2].x * displayPtr[1].x - screenPtr[1].x * displayPtr[2].x) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].x - screenPtr[2].x * displayPtr[0].x) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].x - screenPtr[0].x * displayPtr[1].x) * screenPtr[2].y ;

        matrixPtr->Dn = ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].y - screenPtr[2].y)) -
                        ((displayPtr[1].y - displayPtr[2].y) * (screenPtr[0].y - screenPtr[2].y)) ;

        matrixPtr->En = ((screenPtr[0].x - screenPtr[2].x) * (displayPtr[1].y - displayPtr[2].y)) -
                        ((displayPtr[0].y - displayPtr[2].y) * (screenPtr[1].x - screenPtr[2].x)) ;

        matrixPtr->Fn = (screenPtr[2].x * displayPtr[1].y - screenPtr[1].x * displayPtr[2].y) * screenPtr[0].y +
                        (screenPtr[0].x * displayPtr[2].y - screenPtr[2].x * displayPtr[0].y) * screenPtr[1].y +
                        (screenPtr[1].x * displayPtr[0].y - screenPtr[0].x * displayPtr[1].y) * screenPtr[2].y ;
    }

    return( retValue ) ;

} /* end of setCalibrationMatrix() */


void tp_calibrate()
{
	POINT lcd_screen_pts[3]={{28,18},{308,120}, {160,208}};
	POINT tp_pts[3];
	POINT* capture_pt;
	int16_t pre_tp_x=0, pre_tp_y=0;
	
	// draw 3 points in turn
	
	for(uint8_t i = 0; i<3; i++)
	{
			tft.DrawCircle(lcd_screen_pts[i].x, lcd_screen_pts[i].y, 5, BLUE,0);
			
			uint8_t j=0;  
			// loop to get stable tp point
			while(j<10){
				delay(5);
				capture_pt = tp.read_tp_point();
				if(capture_pt->x!=0 && capture_pt->y!=0){
					
					if(capture_pt->x== pre_tp_x && capture_pt->y == pre_tp_y)
						 j++;
					else{
						pre_tp_x = capture_pt->x;
						pre_tp_y = capture_pt->y;
					}
					if(j== 10) break;
				}
			}
			j=0;
			pre_tp_x = 0;
			pre_tp_y =0;
			tp_pts[i].x = capture_pt->x;
			tp_pts[i].y = capture_pt->y;
			tft.DrawCircle(lcd_screen_pts[i].x, lcd_screen_pts[i].y, 4, WHITE,1);
			delay(500);
	}
	if(setCalibrationMatrix(lcd_screen_pts, tp_pts, &tp.matrix))
		tp.save_matrix();
	
}

void text_box_P(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, char* str_P)
{
	tft.DrawRoundRect(x1,y1,x2,y2, radius, font.BK_color, 1);
	font.set_Cursor( (x1+x2-font.str_Width_P(str_P))>>1, (y1+y2-font.Font_Height)>>1 );
	font.putstr_P(str_P); 
}


void text_box(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, char* str)
{
	tft.DrawRoundRect(x1,y1,x2,y2, radius, font.BK_color, 1);
	font.set_Cursor( (x1+x2-font.str_Width(str))>>1, (y1+y2-font.Font_Height)>>1 );
	font.putstr(str); 
}


void setup()
{
	tft.initialize(LCD_HORIZONTAL);
	tft.ClearScreen(BLACK);
	font.set_Font(f12x12);
	tp.init();

	/* do touchpanel calibration, result will be saved in EEPROM */
	
	font.set_Color(WHITE, RED, 0);
	text_box_P(80, 100, 240, 120, 5, PSTR("TP Calibration"));	
	tp_calibrate();
	tft.ClearScreen(BLACK);
	text_box_P(80, 100, 240, 120, 5, PSTR("TP Calibrated"));

        font.set_Color(WHITE, RED, 0);
	
	text_box_P(0, 0, 40, 20, 5, PSTR("TP X"));
	text_box_P(80,0, 120, 20, 5, PSTR("TP Y"));
	text_box_P(160, 0, 200, 20, 5, PSTR("LCDX"));
	text_box_P(240, 0, 280, 20, 5, PSTR("LCDY")); 
}


uint8_t get_stable_tp(POINT* tp)
{
	static POINT previous_tp;
	static uint8_t count=0;
	
	
	if(tp->x == 0 || tp->y == 0) 
		return 0;
	
	if(tp->x == previous_tp.x && tp->y == previous_tp.y)
		count++;
	else{
		count = 0;
		previous_tp.x = tp->x;
		previous_tp.y = tp->y;
	}
	
	if(count>= TP_STABLE_COUNT){
		count = 0;
		return 1;
	}
	
	return 0;
}


void loop()
{
	char cord[5];
	POINT *tp_dat;
	
		
	font.set_Color(YELLOW, BLACK, 0);
	
	tp_dat=tp.read_lcd_point();
	
	if(get_stable_tp(tp_dat) ){
	
		itoa(tp.tp_point.x,cord,10);
		text_box(41, 0, 79, 20, 0, cord);
		itoa(tp.tp_point.y,cord,10);
		text_box(121, 0, 159, 20, 0, cord);
		itoa(tp.lcd_point.x, cord,10);
		text_box(201, 0, 239, 20, 0, cord);
		itoa(tp.lcd_point.y, cord,10);
		text_box(281, 0, 329, 20, 0, cord);
	}	
}



