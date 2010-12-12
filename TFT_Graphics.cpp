/* 	LCD_GRAPHICS.h -- High-level graphic routines for TFT panel					*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 10/04/2010															*/

#include "TFT_ILI9325.h"
#include "TFT_Graphics.h"


Graphics::Graphics()
{
	
}

void Graphics::initialize(uint8_t orient)
{
	LCD::SetColor(WHITE);
	LCD::init();
	LCD::SetOrientation(orient);
}


void Graphics::SetCursor(uint16_t x, uint16_t y){
	LCD::gotoXY(x,y);
}

void Graphics::ClearScreen(uint16_t color){

	
	LCD::resetArea();
	LCD::fill(LCD::GetWidth()-1, LCD::GetHeight()-1, color);
}


// fast solid rectangle draw method
void Graphics::FastSolidRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	LCD::setArea(x1,y1,x2,y2);
	SetCursor(x1,y1);
	LCD::fill(x2-x1, y2-y1, color);
	LCD::resetArea();

}

void Graphics::DrawPixel(uint16_t x, uint16_t y, uint16_t color){
	LCD::putPixel(x,y, color);
}


void Graphics::VerticalScroll(int16_t y){
	LCD::Scroll(y);
}

void Graphics::DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color){
	
	if(x1==x2)  // vertical line
	{
		FastSolidRect(x1,y1,x2, y2, color);
	}
	else if(y1==y2)  // horizontal line  
	{
		FastSolidRect(x1,y1,x2, y2, color);
	}
	else // any line - use Bresenham line drawing algorithm
	{
		int dy = y2 - y1; 
		int dx = x2 - x1; 
		int stepx, stepy; 
		if (dy < 0) { dy = -dy;  stepy = -1; } else { stepy = 1; } 
		if (dx < 0) { dx = -dx;  stepx = -1; } else { stepx = 1; } 
 
		dy <<= 1;        // dy is now 2*dy 
		dx <<= 1;        // dx is now 2*dx 
 
		DrawPixel(x1, y1, color); 
 
		if (dx > dy) { 					// the line is more horizontal than vertical
			int fraction = dy - (dx >> 1);  // same as 2*dy - dx 
			while (x1!= x2) { 
				if (fraction >= 0) { 
					y1 += stepy; 
					fraction -= dx;    // same as fraction -= 2*dx 
				} 
				x1 += stepx; 
 
				fraction += dy;    // same as fraction -= 2*dy 
				DrawPixel(x1, y1, color); 
			} 
		} 
		else { 							// the line is more vertical than horizontal
			int fraction = dx - (dy >> 1); 
			while (y1 != y2) { 
				if (fraction >= 0) { 
					x1 += stepx; 
					fraction -= dy; 
				} 
				y1 += stepy; 
				fraction += dx; 
				DrawPixel(x1, y1, color); 
			}
		} 
 	} 
 }
		
void Graphics::DrawCircle(uint16_t x, uint16_t y, uint16_t radius, uint16_t color, uint8_t fill)
{
	int xoff, yoff, balance;
	
	xoff = 0;
	yoff = radius;
	balance = -radius;
	
	while (xoff <= yoff) {
		if(!fill){   // no fill - draw pixel in 8 octants 
			DrawPixel(x+xoff, y+yoff, color);
			DrawPixel(x-xoff, y+yoff, color);
			DrawPixel(x-xoff, y-yoff, color);
			DrawPixel(x+xoff, y-yoff, color);
			DrawPixel(x+yoff, y+xoff, color);
			DrawPixel(x-yoff, y+xoff, color);
			DrawPixel(x-yoff, y-xoff, color);
			DrawPixel(x+yoff, y-xoff, color);
		}
		else{  // fill - draw 4 horizontal line to link 8 octant points
			
			DrawLine(x-xoff, y+yoff, x+xoff, y+yoff, color);
			DrawLine(x-xoff, y-yoff, x+xoff, y-yoff, color);
			DrawLine(x-yoff, y+xoff, x+yoff, y+xoff, color);
			DrawLine(x-yoff, y-xoff, x+yoff, y-xoff, color);
		}
			
		if ((balance += xoff++ + xoff)>= 0) {
			balance-= 	2*(yoff-1);//(--yoff)+yoff;
			yoff--;
		}
	}
}


void Graphics::DrawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color, uint8_t fill)
{
    DrawRoundRect(x, y, x + width, y + height, 0, color, fill);
}

void Graphics::DrawRoundRect(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t radius, uint16_t color, uint8_t fill)
{

	
	if(!radius)  // simple rectangle
	{
		if(!fill){   // draw 4 lines
			DrawLine(x1,y1,x2,y1,color);
			DrawLine(x2,y1,x2,y2,color);
			DrawLine(x1,y2,x2,y2,color);
			DrawLine(x1,y1,x1,y2,color);
		}
		else{
			FastSolidRect(x1,y1,x2,y2,color);
		}
	}
	else
	{
		int xoff, yoff, balance;
		int x1_R, y1_R, x2_R, y2_R;
		xoff = 0;
		yoff = radius;
		balance = -radius;
		
		x1_R = x1+radius;
		y1_R = y1+radius;
		x2_R = x2-radius;
		y2_R = y2-radius;
	
		// first draw the rectangle area
		if(!fill){
			DrawLine(x1_R,y1,x2_R,y1,color);
			DrawLine(x2,y1_R,x2,y2_R,color);
			DrawLine(x1_R,y2,x2_R,y2,color);
			DrawLine(x1,y1_R,x1,y2_R,color);
		}
		else{
			FastSolidRect(x1,y1_R,x2,y2_R,color);
		}
		
		while (xoff <= yoff) {
			if(!fill){   // no fill - draw pixel in 8 octants 
				DrawPixel(x2_R+xoff, y2_R+yoff,  color);
				DrawPixel(x1_R-xoff, y2_R+yoff, color);
				DrawPixel(x1_R-xoff, y1_R-yoff, color);
				DrawPixel(x2_R+xoff, y1_R-yoff, color);
				DrawPixel(x2_R+yoff, y2_R+xoff, color);
				DrawPixel(x1_R-yoff, y2_R+xoff, color);
				DrawPixel(x1_R-yoff, y1_R-xoff, color);
				DrawPixel(x2_R+yoff, y1_R-xoff, color);
			}
			else{  // fill - draw 4 horizontal line to link 8 octant points
			
				DrawLine(x1_R-xoff, y2_R+yoff, x2_R+xoff, y2_R+yoff, color);
				DrawLine(x1_R-xoff, y1_R-yoff, x2_R+xoff, y1_R-yoff, color);
				DrawLine(x1_R-yoff, y2_R+xoff, x2_R+yoff, y2_R+xoff, color);
				DrawLine(x1_R-yoff, y1_R-xoff, x2_R+yoff, y1_R-xoff, color);
			}
			
			if ((balance += xoff++ + xoff)>= 0) {
				balance-= 2* (yoff-1);  //--yoff+yoff;
				yoff--;
			}
		}		
	}

}
