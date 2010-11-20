#include <string.h>
#include <TFT_ILI9325.h>
#include <TFT_PFF.h>


TFT_PFF pff;

uint8_t bitmap_show(char* pFile);
void buf_display(uint8_t *buf_ptr, uint16_t count);
void setup()
{
	uint8_t res;
	
	LCD::init(LCD_HORIZONTAL);
	LCD::resetArea();
	LCD::fill(LCD::GetWidth()-1, LCD::GetHeight()-1, BLACK);
	
	
  res = disk_initialize();
  if (!res)
    res = pf_mount(&pff.fs);
  
}


void loop()
{
  uint8_t res;
  DIR dir;      
  FILINFO file_info;  
	
  res = pf_opendir(&dir, "/");
  	
  if (!res )
  {
 
    while(1)
    {
      res = pf_readdir(&dir, &file_info);
      if(res) break; 
       
      if (!file_info.fname[0])
        break;
      if (!(file_info.fattrib & AM_DIR))
      {
        if (strstr(file_info.fname, ".BMP") != 0)
        {
					if(bitmap_show(file_info.fname)==0)          
	      		delay(5000);
        }
      }
		}
	}
	
}

#define BUF_SIZE 512

typedef struct BMIH { //declares BitMap Info Header structure
  unsigned long biSize;            //Length of bitmap information header (40 bytes for Windows V3 bitmaps)
  unsigned long biWidth;           //Width (pixels)
  unsigned long biHeight;          //Height (pixels)
  unsigned short biPlanes;         //Color planes, always 1
  unsigned short biBitCount;       //Color bits per pixel (1, 4, 8, 16, 24 and 32)
  unsigned long biCompression;     //Compression method, we only read uncompressed (type 0)
  unsigned long biSizeImage;       //Image data length
  unsigned long biXPelsPerMeter;   //Horizontal resolution (pixels per meter)
  unsigned long biYPelsPerMeter;   //Vertical resolution (pixel per meter)
  unsigned long biClrUsed;         //Number of colors, ignored.
  unsigned long biClrImportant;    //Number of important colors, ignored.                                  
}BMP_INFO;

typedef struct BMFH { //declares BitMap File Header structure
  unsigned short bfType;           //Always 0x42 0x4D (hex for BM <-- indicating a bitmap)
  unsigned long  bfSize;           //File size (bytes)
  unsigned short bfReserved1;      //Reserved, ignored
  unsigned short bfReserved2;      //Reserved, ignored
  unsigned long  bfOffBits;        //Location in file of the first bitmap data               
}BMP_HEADER;

uint16_t bitmap_width;
uint16_t bitmap_height;
uint8_t buf[BUF_SIZE];

uint8_t rgb_cnt=0;		
uint8_t pad_len=0;		
uint8_t pad_cnt=0;
uint16_t pixel_cnt = 0;
uint8_t RGB[3];	

void buf_display(uint8_t *buf_ptr, uint16_t count)
{
	uint16_t i;
	
	for(i=0; i<count; i++)
	{
		if(!pad_cnt){  // skip padding bytes
			RGB[rgb_cnt++]= *(buf_ptr+i);
			if(rgb_cnt==3){
				// output data
				LCD_SET_DATH((RGB[2]&0xF8)|(RGB[1]>>5));
      	LCD_SET_DATL ( ((RGB[1]&0xFC)<<3) |(RGB[0]>>3) );
      	WR0;
      	WR1;
      	rgb_cnt = 0;
				// check if there is padding data
				if(pad_len){
      		pixel_cnt++;
      		if(pixel_cnt>= bitmap_width)
      		{
      			pad_cnt = 1;
						pixel_cnt = 0;  
      		}
      	}
      }
   	}
   	else {  // pad bytes
   		if(pad_cnt++>= pad_len) pad_cnt = 0;
   	}
  }
}
      	 



uint8_t bitmap_show(char* pFile)
{
  BMP_HEADER *ptr_header;
  BMP_INFO *ptr_info;

  uint8_t res;
  unsigned short br;			// File Read count

  res = pf_open(pFile);
  if (res)
    return res;

  res = pf_read(&buf, BUF_SIZE, &br);	// read bmp header
  if (res)
    return res;
	
	// set LCD area position
  LCD::resetArea();
 	LCD::fill(LCD::GetWidth()-1, LCD::GetHeight()-1, SILVER);
 	
	ptr_header= (BMP_HEADER *)buf;
	ptr_info = (BMP_INFO *)(buf+sizeof(BMP_HEADER));
	
  if (ptr_header->bfType != 0x4d42 )		// type is 'BM'
    return 0xff;
  

 	if (ptr_info->biWidth       >  240) return 0xff; 
 	if (ptr_info->biHeight      > 320) return 0xff; 
  if (ptr_info->biCompression !=   0) return 0xff; 

	// reset count values
	rgb_cnt=0;		
	pad_cnt=0;
	pixel_cnt = 0;

  // set data
  bitmap_width  = ptr_info->biWidth;
  bitmap_height  = ptr_info->biHeight;
  
  pad_len = (4 - ((bitmap_width*3)%4))%4;
  //pad_len = 1;
  LCD::setArea(  (LCD::GetWidth()-bitmap_height)>>1, (LCD::GetHeight()-bitmap_width)>>1, 
  							 ((LCD::GetWidth()+bitmap_height)>>1)-1, ((LCD::GetHeight()+bitmap_width)>>1)-1);
  //LCD::setArea(0,0,319,239);
  
  // first display buffer - need to skip headers
  CS0;
  buf_display( buf+sizeof(BMP_HEADER)+sizeof(BMP_INFO), br-(sizeof(BMP_HEADER)+sizeof(BMP_INFO)) );
  
  if(br!= BUF_SIZE) return 0;  // very small bitmap?
  
  
  do{
  	res = pf_read((void*)buf, BUF_SIZE, &br);		// read data
    buf_display( buf,br);
  } while (!res && (br == BUF_SIZE));  				// error or eof 
  
  CS1;
 
	return 0;
  
}


