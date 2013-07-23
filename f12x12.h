/*
    created with FontEditor written by H. Reddmann
    HaReddmann at t-online dot de

    File Name           : f12x12.h
    Date                : 30.03.2004
    Font size in bytes  : 0x02E4, 740
    Font width          : 12
    Font height         : 12
    Font first char     : 0x20
    Font last char      : 0x7A
    Font bits per pixel : 1
    Font is compressed  : false

    The font data are defined as

    struct _FONT_ {
     // common shared fields
       uint16_t   font_Size_in_Bytes_over_all_included_Size_it_self;
       uint8_t    font_Width_in_Pixel_for_fixed_drawing;
       uint8_t    font_Height_in_Pixel_for_all_Characters;
       uint8_t    font_Bits_per_Pixels;
                    // if MSB are set then font is a compressed font
       uint8_t    font_First_Char;
       uint8_t    font_Last_Char;
       uint8_t    font_Char_Widths[font_Last_Char - font_First_Char +1];
                    // for each character the separate width in pixels,
                    // characters < 128 have an implicit virtual right empty row
                    // characters with font_Char_Widths[] == 0 are undefined

     // if compressed font then additional fields
       uint8_t    font_Byte_Padding;
                    // each Char in the table are aligned in size to this value
       uint8_t    font_RLE_Table[3];
                    // Run Length Encoding Table for compression
       uint8_t    font_Char_Size_in_Bytes[font_Last_Char - font_First_Char +1];
                    // for each char the size in (bytes / font_Byte_Padding) are stored,
                    // this get us the table to seek to the right beginning of each char
                    // in the font_data[].

     // for compressed and uncompressed fonts
       uint8_t    font_data[];
                    // bit field of all characters
    }
*/

#include <inttypes.h>
#include <avr/pgmspace.h>

#ifndef f12x12_H
#define f12x12_H

#define f12x12_WIDTH 12
#define f12x12_HEIGHT 12

static const uint8_t __attribute__ ((progmem)) f12x12[] = {
    0x02, 0xE4, 0x0C, 0x0C, 0x01, 0x20, 0x7A,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 
    0x06, 0x04, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x02, 0x00, 0x00, 0x00, 0x00, 0x06, 
    0x00, 0x08, 0x08, 0x07, 0x08, 0x07, 0x07, 0x08, 0x08, 0x02, 0x06, 0x08, 0x07, 0x0A, 0x08, 0x08, 
    0x08, 0x08, 0x09, 0x07, 0x08, 0x08, 0x08, 0x0B, 0x09, 0x0A, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x06, 0x06, 0x06, 0x06, 0x06, 0x04, 0x06, 0x06, 0x02, 0x03, 0x06, 0x02, 0x0A, 0x06, 0x06, 
    0x06, 0x06, 0x04, 0x06, 0x04, 0x06, 0x08, 0x0A, 0x08, 0x08, 0x06, 
    0x00, 0x00, 0x30, 0x00, 0xE3, 0x1F, 0xFF, 0x13, 0x20, 0x01, 0xF2, 0x3F, 0xFE, 0x21, 0x00, 0x02, 
    0xF0, 0x3F, 0xFF, 0x63, 0x38, 0xC7, 0x13, 0x26, 0x31, 0xF2, 0x21, 0x0E, 0x22, 0x10, 0x03, 0x13, 
    0x21, 0x11, 0xF2, 0x3F, 0xEE, 0x01, 0x0C, 0xF0, 0xC0, 0x0B, 0x8E, 0xF0, 0x3F, 0xFF, 0xF3, 0x13, 
    0x3F, 0x13, 0x21, 0x11, 0x12, 0x3F, 0xE1, 0xE1, 0x1F, 0xFF, 0x13, 0x21, 0x11, 0x32, 0x3F, 0xE2, 
    0x11, 0x00, 0x81, 0x13, 0x3F, 0x7D, 0xF0, 0x00, 0x03, 0xE0, 0x1E, 0xFF, 0x13, 0x21, 0x11, 0xF2, 
    0x3F, 0xEE, 0xE1, 0x11, 0x3F, 0x13, 0x22, 0x21, 0xF2, 0x3F, 0xFE, 0xC1, 0x30, 0x0C, 0x63, 0x00, 
    0x07, 0x10, 0x36, 0x71, 0xF3, 0x01, 0x0E, 0x00, 0x30, 0xE0, 0xC3, 0x0F, 0x8F, 0xF0, 0x08, 0xFC, 
    0x00, 0x3E, 0x00, 0xF3, 0x3F, 0xFF, 0x13, 0x21, 0x11, 0x12, 0x21, 0x11, 0xF2, 0x3F, 0xEE, 0xC1, 
    0x0F, 0xFE, 0x31, 0x30, 0x01, 0x12, 0x20, 0x03, 0x63, 0x18, 0xFF, 0xF3, 0x3F, 0x01, 0x12, 0x20, 
    0x01, 0x32, 0x30, 0xFE, 0xC1, 0x0F, 0xFF, 0xF3, 0x3F, 0x11, 0x12, 0x21, 0x11, 0x12, 0x21, 0x01, 
    0xF2, 0x3F, 0xFF, 0x13, 0x01, 0x11, 0x10, 0x01, 0x11, 0x10, 0x00, 0xFC, 0xE0, 0x1F, 0x03, 0x13, 
    0x20, 0x21, 0x12, 0x22, 0xE3, 0x61, 0x3E, 0xFF, 0xF3, 0x3F, 0x10, 0x00, 0x01, 0x10, 0x00, 0x01, 
    0xFF, 0xF3, 0x3F, 0xFF, 0xF3, 0x3F, 0x80, 0x01, 0x38, 0x00, 0x02, 0x20, 0xFF, 0xF3, 0x1F, 0xFF, 
    0xF3, 0x3F, 0x38, 0xC0, 0x06, 0xC6, 0x30, 0x18, 0x01, 0x03, 0x20, 0xFF, 0xF3, 0x3F, 0x00, 0x02, 
    0x20, 0x00, 0x02, 0x20, 0x00, 0xF2, 0x3F, 0xFF, 0xC3, 0x03, 0xF0, 0x00, 0x3C, 0xC0, 0x03, 0x0F, 
    0x3C, 0xF0, 0x3F, 0xFF, 0xF3, 0x3F, 0xFF, 0xE3, 0x00, 0x3C, 0x00, 0x0F, 0xC0, 0xF1, 0x3F, 0xFF, 
    0xC3, 0x0F, 0xFE, 0x31, 0x30, 0x01, 0x12, 0x20, 0x03, 0xE3, 0x1F, 0xFC, 0xF0, 0x3F, 0xFF, 0x13, 
    0x02, 0x21, 0x10, 0x02, 0x21, 0xF0, 0x03, 0x1E, 0xC0, 0x0F, 0xFE, 0x31, 0x30, 0x01, 0x12, 0x28, 
    0x83, 0xE3, 0x3F, 0xFC, 0xF2, 0x3F, 0xFF, 0x13, 0x02, 0x21, 0x10, 0x02, 0x21, 0xF0, 0x1F, 0xDE, 
    0x03, 0x20, 0x8E, 0xF1, 0x39, 0x11, 0x12, 0x23, 0x21, 0x72, 0x3E, 0xC6, 0x11, 0x00, 0x01, 0x10, 
    0x00, 0xFF, 0xF3, 0x3F, 0x01, 0x10, 0x00, 0x01, 0xF0, 0x0F, 0xFF, 0x01, 0x30, 0x00, 0x02, 0x20, 
    0x00, 0xF3, 0x1F, 0xFF, 0x30, 0x00, 0x1F, 0xC0, 0x0F, 0xC0, 0x03, 0x3C, 0xFC, 0xF0, 0x01, 0x03, 
    0xF0, 0x01, 0x7F, 0x00, 0x3E, 0xC0, 0x83, 0x07, 0x1C, 0x80, 0x07, 0xC0, 0x03, 0x3E, 0x7F, 0xF0, 
    0x01, 0x03, 0x73, 0x38, 0xCC, 0x80, 0x07, 0x30, 0x80, 0x07, 0xCC, 0x70, 0x38, 0x03, 0x33, 0x00, 
    0x07, 0xC0, 0x00, 0x18, 0x00, 0x3F, 0xF0, 0x83, 0x01, 0x0C, 0x70, 0x00, 0x03, 0x10, 0x30, 0x81, 
    0x13, 0x2C, 0x61, 0x12, 0x23, 0x19, 0xD2, 0x20, 0x07, 0x32, 0x20, 0x90, 0x81, 0x3D, 0x68, 0x82, 
    0x22, 0xF8, 0x03, 0x3F, 0xFF, 0xF3, 0x3F, 0x08, 0x82, 0x20, 0xF8, 0x03, 0x1F, 0xF0, 0x81, 0x3F, 
    0x08, 0x82, 0x20, 0x18, 0x03, 0x11, 0xF0, 0x81, 0x3F, 0x08, 0x82, 0x20, 0xFF, 0xF3, 0x3F, 0xF0, 
    0x81, 0x3F, 0x28, 0x82, 0x22, 0x38, 0x03, 0x13, 0x08, 0xE0, 0x3F, 0xFF, 0x93, 0x00, 0xF0, 0x89, 
    0xBF, 0x08, 0x8A, 0xA0, 0xF8, 0x8F, 0x7F, 0xFF, 0xF3, 0x3F, 0x08, 0x80, 0x00, 0xF8, 0x03, 0x3F, 
    0xFB, 0xB3, 0x3F, 0x00, 0xB8, 0xFF, 0xFB, 0xF7, 0x3F, 0xFF, 0x03, 0x0E, 0xB0, 0x81, 0x31, 0x08, 
    0xF2, 0x3F, 0xFF, 0x83, 0x3F, 0xF8, 0x83, 0x00, 0x08, 0x80, 0x3F, 0xF8, 0x83, 0x00, 0x08, 0x80, 
    0x3F, 0xF0, 0x83, 0x3F, 0xF8, 0x83, 0x00, 0x08, 0x80, 0x3F, 0xF0, 0x03, 0x1F, 0xF8, 0x83, 0x20, 
    0x08, 0x82, 0x3F, 0xF0, 0x81, 0xFF, 0xF8, 0x8F, 0x20, 0x08, 0x82, 0x3F, 0xF0, 0x01, 0x1F, 0xF8, 
    0x83, 0x20, 0x08, 0x82, 0xFF, 0xF8, 0x8F, 0x3F, 0xF8, 0x83, 0x01, 0x08, 0x00, 0x13, 0x78, 0x83, 
    0x24, 0x48, 0x82, 0x3D, 0x90, 0x81, 0x00, 0xFE, 0xE1, 0x3F, 0x08, 0x82, 0x1F, 0xF8, 0x03, 0x20, 
    0x00, 0x82, 0x3F, 0xF8, 0x83, 0x01, 0x78, 0x00, 0x0E, 0x80, 0x03, 0x38, 0xE0, 0x80, 0x07, 0x18, 
    0x80, 0x01, 0xF8, 0x00, 0x3E, 0x80, 0x03, 0x0F, 0xF0, 0x00, 0x38, 0xE0, 0x83, 0x0F, 0x18, 0x80, 
    0x20, 0x18, 0x03, 0x1B, 0xE0, 0x00, 0x0E, 0xB0, 0x81, 0x31, 0x08, 0x82, 0x01, 0x78, 0x00, 0xDE, 
    0x80, 0x0F, 0x78, 0xE0, 0x81, 0x07, 0x18, 0x80, 0x30, 0x88, 0x83, 0x2C, 0x68, 0x82, 0x23, 0x18, 
    0x02, 0x00
};

#endif

