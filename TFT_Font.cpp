/* 	TFT_Font.cpp -- Font  routines for TFT panel								*/ 
/* 	Author- xy																	*/
/* 	Web - www.nuelectronics.com													*/
/*	Version - v0.1																*/
/*	Date - 12/04/2010															*/

#include "TFT_Font.h"
#include "TFT_ILI9325.h"


TFT_Font::TFT_Font()
{
	Font_color = WHITE;
	BK_color = BLACK;
	CursorX = 0;
	CursorY = 0;
	Transparent = 0;   // Transparent =1 - don't print background color, 0 - print background color
	Char_space=2;
	FixedFont = 0;
}


	
void TFT_Font::set_Color(uint16_t fColor, uint16_t bColor, uint8_t Transp)
{
	this->Font_color = fColor;
	this->BK_color = bColor;
	this->Transparent = Transp;
}
void TFT_Font::set_Cursor(uint16_t x, uint16_t y)
{
	CursorX = x;
	CursorY = y;
}

	
void TFT_Font::set_Font(uint8_t *pfont)
{
	this->Font_Ptr = pfont;
	this->Font_Width = pgm_read_byte(Font_Ptr + 2);
    this->Font_Height = pgm_read_byte(Font_Ptr + 3);
    this->Font_BitsPixel = pgm_read_byte(Font_Ptr + 4);
    this->Font_FirstChar = pgm_read_byte(Font_Ptr + 5);
    this->Font_LastChar = pgm_read_byte(Font_Ptr + 6);
}

uint8_t TFT_Font::CharWidth(char c)     // calculate char width
{
    uint8_t len = 0;
	
	if(!Font_Ptr)
      return 0; 
	 
	if(c< Font_FirstChar || c>Font_LastChar) return 0;
    
	
	
    len = pgm_read_byte(Font_Ptr + FONT_HEADER_SIZE + c - Font_FirstChar);
    
	return len;
}


void TFT_Font::putstr_P(char *str)
{
	char c;
	while( (c = pgm_read_byte(str++)))
		putChar(c);
}

uint16_t TFT_Font::str_Width_P(char *str)
{
	uint16_t len = 0;
	char c;
	
	while( (c = pgm_read_byte(str++)))
		len += CharWidth(c) + Char_space;
	
	return len;
}

void TFT_Font::putstr(char *str)
{
	char c;
	while( c = *(str++))
		putChar(c);
}

uint16_t TFT_Font::str_Width(char *str)
{
	uint16_t len = 0;
	char c;
	
	while( c= *(str++))
		len += CharWidth(c) + Char_space;
	
	return len;
}



void TFT_Font::putChar(char c) 
{

    uint8_t *ptr;
    uint8_t bit_num;
	uint8_t font_data;
	uint8_t i, j;
	uint16_t pix_x, pix_y;
  
    uint8_t charwidth = CharWidth(c);
    if (charwidth == 0) return;

    uint8_t width = charwidth + Char_space;
    

	// pre-set fontwidth for fixedfont
    if ((FixedFont == 1) && (width<Font_Width)) {
      width = Font_Width;
    }

    	// line adjust
	if (CursorX + (uint16_t)charwidth  > LCD::GetWidth())
	{
		CursorY = CursorY + (unsigned int)Font_Height ;
        if ((CursorY + Font_Height) > LCD::GetHeight())
        	CursorY = 0;	
		CursorX = 0;
	}

  uint32_t index = 0;
	
	
	if (Font_BitsPixel & 0x80)  //compressed font 
  {   	
  	uint8_t rle_table[4];
  	uint8_t padding, pixel_count = 0, bit_count=0,pixel_color;
  	uint8_t bitspixel;
  	uint16_t bits = 0;
  	
  	bitspixel = Font_BitsPixel & 0x7f;
  	ptr = Font_Ptr + FONT_HEADER_SIZE + (Font_LastChar - Font_FirstChar +1);
  	
  	padding = pgm_read_byte(ptr++);
  	
  	// get run-length encoding table
  	rle_table[0] = 1;
  	rle_table[1] = pgm_read_byte(ptr++);
		rle_table[2] = pgm_read_byte(ptr++);
  	rle_table[3] = pgm_read_byte(ptr++);
  
  	for (i = Font_FirstChar; i < c; i++) {
			index += pgm_read_byte(ptr++);
		}	
		index *= padding;
		ptr = Font_Ptr + FONT_HEADER_SIZE + (Font_LastChar - Font_FirstChar +1)*2 + 4 + index;
		
		for(i=0;i<charwidth; i++)
		{
			pix_x = (uint16_t)i + CursorX;
		
			for(j=0; j<Font_Height; j++)
			{
				if(LCD::GetOrientation() == LCD_HORIZONTAL)
					pix_y = (uint16_t)(Font_Height-j) + CursorY;
				else
					pix_y = (uint16_t)(j) + CursorY;
					
				if(pixel_count==0)
				{
					if(bit_count<=8){
						bits |= (pgm_read_byte(ptr++))<<bit_count;
						bit_count +=8;
  				}
  				pixel_count = rle_table[bits & 0x3];
  				bits>>=2;
  				bit_count-=2;
  				pixel_color = bits & (0xff>>(8-bitspixel));
  				bits>>=bitspixel;
  				bit_count-= bitspixel;
  			}
  			if(pixel_color){
  				LCD::putPixel(pix_x, pix_y, Font_color);
				}
				else if(!Transparent){
					LCD::putPixel(pix_x , pix_y, BK_color);
				}
				pixel_count--;
			}
		}	
  				
  	
  }
  else   // uncompressed font
  {
  	// find font startpoint	
	ptr = Font_Ptr + FONT_HEADER_SIZE;
	for (i = Font_FirstChar; i < c; i++) {
		index += pgm_read_byte(ptr++);
	}
  
	index *= Font_Height * Font_BitsPixel;

	bit_num = (uint8_t) (index & 0x7);
	index = index>>3;
	ptr = Font_Ptr + FONT_HEADER_SIZE + (Font_LastChar - Font_FirstChar +1) + index;

	// walk through every pixel
	font_data = pgm_read_byte(ptr++);

	for(i=0;i<charwidth; i++)
	{
		pix_x = (uint16_t)i + CursorX;
		
		for(j=0; j<Font_Height; j++)
		{
			if(LCD::GetOrientation() == LCD_HORIZONTAL)
				pix_y = (uint16_t)(Font_Height-j)  + CursorY;
			else
				pix_y = (uint16_t)(j)  + CursorY;
			
			if(font_data & (1<<bit_num))   		//putpixel at fontcolor
				LCD::putPixel(pix_x , pix_y, Font_color);
			else 
			{   		//putpixel at background color
				if(!Transparent)
					LCD::putPixel(pix_x , pix_y, BK_color);
			}
				
			
			bit_num++;
			
			if(bit_num>7){
				font_data = pgm_read_byte(ptr++);  // new data
				bit_num= 0;
			}
				
		}
	}
	}
    // adjust cursor to next position
	CursorX += (uint16_t)(charwidth + Char_space);
}


void TFT_Font::putnumber(long n){
   char buf[10];    
   uint8_t i=0;
   
   if(n==0)
	   putChar('0');
   else	if(n < 0)
   {
      putChar('-');
			n = -n;
	 }
   while(n>0 && i <= 10){
	   buf[i++] = (n % 10)+'0';  // n % base
	   n /= 10;   // n/= base
	 }
	 for(; i >0; i--)
		 putChar(buf[i-1]);	  
   
}