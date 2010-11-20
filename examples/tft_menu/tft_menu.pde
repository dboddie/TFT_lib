#include <f12x12.h>
#include <TFT_Font.h>
#include <TFT_Graphics.h>
#include <Touchpanel.h>
#include <util/delay.h>
#include <stdlib.h>
#include "f15x22.h"

#define TP_STABLE_COUNT 1

Graphics tft;

TFT_Font font;
touchpanel tp;

void colormap();
void fontmap();
void bitmap();
uint8_t mount_sd();
uint8_t open_root_dir();
uint8_t show_bitmap();
void calibrate();
void tp_calibrate();

typedef struct menu_item{
	uint16_t sx;
	uint16_t sy;
	uint16_t ex;
	uint16_t ey;
	char *str;
	void (*func)(void);
}MENUITEM;




#define MENU_START_X 40
#define MENU_START_Y 60
#define MENU_WIDTH	80
#define MENU_HEIGHT	40
#define MENU_GAP	20

#define MENU_FONT_COLOR		WHITE
#define MENU_NORMAL_COLOR	GREEN
#define MENU_HIGHLIGHT_COLOR	RED

MENUITEM menu[4];
int current_menu_no = -1;




void setup()
{
	tft.initialize(LCD_HORIZONTAL);
	tft.ClearScreen(WHITE);
	font.set_Font(f12x12);
	tp.init();
	tp.get_matrix();
	
	//setup menu
	menu[0].sx = MENU_START_X;
	menu[0].sy = MENU_START_Y;
	menu[0].ex = MENU_START_X + MENU_WIDTH;
	menu[0].ey = MENU_START_Y + MENU_HEIGHT;
	menu[0].str = PSTR("COLORMAP");
	menu[0].func = colormap;
	
	menu[1].sx = MENU_START_X;
	menu[1].sy = MENU_START_Y + MENU_HEIGHT + MENU_GAP;
	menu[1].ex = MENU_START_X + MENU_WIDTH;
	menu[1].ey = MENU_START_Y + 2*MENU_HEIGHT + MENU_GAP;
	menu[1].str = PSTR("FONTMAP");
	menu[1].func = fontmap;
	
	menu[2].sx = MENU_START_X + MENU_WIDTH +MENU_GAP;
	menu[2].sy = MENU_START_Y;
	menu[2].ex = MENU_START_X + 2*MENU_WIDTH+MENU_GAP;
	menu[2].ey = MENU_START_Y + MENU_HEIGHT;
	menu[2].str = PSTR("BITMAP");
	menu[2].func = bitmap;
	
	menu[3].sx = MENU_START_X + MENU_WIDTH +MENU_GAP;
	menu[3].sy = MENU_START_Y + MENU_HEIGHT + MENU_GAP;
	menu[3].ex = MENU_START_X + 2*MENU_WIDTH+MENU_GAP;
	menu[3].ey = MENU_START_Y + 2*MENU_HEIGHT + MENU_GAP;
	menu[3].str = PSTR("TP CALIBRATION");
	menu[3].func = calibrate;

}

void text_box_P(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, char* str_P)
{
	tft.DrawRoundRect(x1,y1,x2,y2, radius, font.BK_color, 1);
	font.set_Cursor( (x1+x2-font.str_Width_P(str_P))>>1, (y1+y2-font.Font_Height)>>1 );
	font.putstr_P(str_P); 
}

void init_menu()
{
	int i;
	font.set_Color(MENU_FONT_COLOR, MENU_NORMAL_COLOR,1);
	for(i=0; i<4; i++)
		text_box_P(	menu[i].sx, menu[i].sy, menu[i].ex, menu[i].ey, 8, menu[i].str);
		
	current_menu_no = -1 ;
}




void text_box(uint16_t x1,uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, char* str)
{
	tft.DrawRoundRect(x1,y1,x2,y2, radius, font.BK_color, 1);
	font.set_Cursor( (x1+x2-font.str_Width(str))>>1, (y1+y2-font.Font_Height)>>1 );
	font.putstr(str); 
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
	
	
char tp_inside_menu(POINT *tp)
{
	int i;
	for(i=0; i<4; i++){
		if(	tp->x>= menu[i].sx && tp->y>= menu[i].sy &&	
			tp->x <= menu[i].ex && tp->y <= menu[i].ey )
			
			return i;
	}
	return -1;		// not inside any menu item
	
}
			
	
		
	

void loop()
{
	
	POINT *tp_dat;
		
			
	init_menu();
        while(1)
	{
		char cord[5];
		tp_dat = tp.read_lcd_point();

		if(get_stable_tp(tp_dat) )  // got a stable tp point
		{
			current_menu_no = tp_inside_menu(tp_dat);
			
			if(current_menu_no >=0 && current_menu_no<=3){
				font.set_Color(MENU_FONT_COLOR, MENU_HIGHLIGHT_COLOR,1);		// draw current menu in highlight color
				text_box_P(	menu[current_menu_no].sx, menu[current_menu_no].sy, menu[current_menu_no].ex, menu[current_menu_no].ey, 8, menu[current_menu_no].str);
				_delay_ms(50);
				(*(menu[current_menu_no].func))();
				
				init_menu();
			}
		}
		
		_delay_ms(5);
	}
}


void wait_for_OK()
{
	POINT *tp_dat;
	
	font.set_Color(MENU_FONT_COLOR, MENU_NORMAL_COLOR,1);
	
	text_box_P(	120, 20, 200, 50, 5, PSTR("OK"));
	
	while(1)
	{
		tp_dat = tp.read_lcd_point();
		
		if(get_stable_tp(tp_dat) ) 
		{
			if(tp_dat->x>=120 && tp_dat->y >=20&& tp_dat->x <=200 && tp_dat->y<=50) 
			{
				tft.ClearScreen(WHITE);
				return;
				
			}
		}
		_delay_ms(5);
	}
	
}

uint8_t wait_for_input()
{
	POINT *tp_dat;
	
	font.set_Color(MENU_FONT_COLOR, MENU_NORMAL_COLOR,1);
	
	text_box_P(	120, 20, 180, 50, 5, PSTR("OK"));
	text_box_P(	200, 20, 280, 50, 5, PSTR("NEXT"));
	
	while(1)
	{
		tp_dat = tp.read_lcd_point();
		
		if(get_stable_tp(tp_dat) ) 
		{
			if(tp_dat->x>=120 && tp_dat->y >=20&& tp_dat->x <=180 && tp_dat->y<=50) return 1;
			else if (tp_dat->x>=200 && tp_dat->y >=20&& tp_dat->x <=280 && tp_dat->y<=50) return 0;
		}
		_delay_ms(5);
	}
}	


void colormap()
{
	uint8_t i,j,k;
	// draw a clolormap
	uint16_t color;
	tft.ClearScreen(WHITE);
	
	// draw 512 (10x15) boxes with incremantal color
	for(i=0; i<8; i++){		
		for(j=0; j<8; j++){
			for(k=0;k<8;k++){
				color = _RGB565(i<<5, j<<5, k<<5);
				tft.DrawRoundRect(i*40+(j/2)*10, k*15+ (j&0x1)*120,i*40+(j/2)*10 +9, k*15+ (j&0x1)*120 +14, 0, color,1);
			}		
		}
	}
	wait_for_OK();
	
}
void fontmap()
{
	char i;
	uint8_t row=0;

	// draw font
	tft.ClearScreen(WHITE);
	
	tft.DrawRoundRect( 40, 70, 280, 230,  10, MENU_HIGHLIGHT_COLOR,1);
	
	font.set_Font(f15x22);
	font.set_Color(MENU_FONT_COLOR, MENU_NORMAL_COLOR,1);
	
	font.set_Cursor( 50, 80);
	
	for(i=font.Font_FirstChar; i<= font.Font_LastChar; i++)
	{
		if(font.CharWidth(i) !=0)
		{
			 if( (font.CursorX + font.CharWidth(i)) > 270)
			 {
			 		row++;
			 		font.set_Cursor(50, 80+ (row* font.Font_Height + 2));
			 	}
			 
                font.putChar(i);
		}
	}
	
	font.set_Font(f12x12);
	wait_for_OK();
	
}


void bitmap()
{
	uint8_t res;
	char temp[10];
	tft.ClearScreen(WHITE);
      #if defined(__AVR_ATmega328P__)
		res = mount_sd();
		if(res) {
		    text_box_P(	80, 50, 250, 200, 15, PSTR("Cannot mount SDcard"));
                    wait_for_OK();
                  }
		else
		{
			if(open_root_dir()) return;
			while(1){
				res= show_bitmap();
				if(!res){				
					if(wait_for_input()==1) {
						tft.ClearScreen(WHITE);
						break;  // ok button is pressed, break the loop
					}
				}
				else if(res==0x10)  // reopen root dir 
				{
						if(open_root_dir()) return;
				}	
					
			}
		}
			
			
	#else
		text_box_P(80, 60, 250, 200, 15, PSTR("N/A in ATMEGA168"));
		wait_for_OK();
	#endif
	
}


void calibrate()
{
        tft.ClearScreen(BLACK);
	tp_calibrate();
	wait_for_OK();
}



