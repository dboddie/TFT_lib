#ifndef _TFT_PFF_H_
#define _TFT_PFF_H_


extern "C"
{
  #include <inttypes.h>
  #include "integer.h"
  #include "pff.h"
  #include "diskio.h"
}


class TFT_PFF
{
  public:
    FATFS fs;
    TFT_PFF();
   
};


#endif 
