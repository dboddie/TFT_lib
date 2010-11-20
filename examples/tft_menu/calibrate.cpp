#include <f12x12.h>
#include <TFT_Font.h>
#include <TFT_Graphics.h>
#include <Touchpanel.h>
#include <util/delay.h>
#include <stdlib.h>

#define TP_STABLE_COUNT	3

extern Graphics tft;

extern TFT_Font font;
extern touchpanel tp;

void text_box_P(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, char* str_P);


void text_box(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, char* str);



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
	
	
	font.set_Color(WHITE, RED, 0);
	
	text_box_P(80, 100, 240, 120, 5, PSTR("TP Calibration"));	

	// draw 3 points in turn
	
	for(uint8_t i = 0; i<3; i++)
	{
			tft.DrawCircle(lcd_screen_pts[i].x, lcd_screen_pts[i].y, 5, BLUE,0);
			
			uint8_t j=0;  
			// loop to get stable tp point
			while(j<10){
				_delay_ms(5);
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
			_delay_ms(500);
	}
	if(setCalibrationMatrix(lcd_screen_pts, tp_pts, &tp.matrix))
		tp.save_matrix();
	
	tft.ClearScreen(BLACK);
		
			
	text_box_P(80, 100, 240, 120, 5, PSTR("TP Calibrated"));
	

}



