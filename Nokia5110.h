#ifndef NOKIA5110_H
#define NOKIA5110_H

#include <stdint.h>
#include "Arduino.h"
#include "SPI.h"

/*
   Class: Nokia5110
   Class that can be used drive a Nokia5110 display module, e.g. as available from: https://www.sparkfun.com/products/10168
*/

/* 84x48 LCD Defines: */
#define LCD_WIDTH   84 // Note: x-coordinates go wide
#define LCD_HEIGHT  48 // Note: y-coordinates go high
#define WHITE       0  // For drawing pixels. A 0 draws white.
#define BLACK       1  // A 1 draws black.

/* PCD8544-specific defines: */
#define LCD_COMMAND  0 
#define LCD_DATA     1

class Nokia5110
{
  public:
    
    /*
        Constructor: Nokia5110
          Create Nokia5110 object, using the default pin configuration of:
            scePin  =  7   SCE - Chip select, pin 3 on LCD.
            rstPin  =  6   RST - Reset, pin 4 on LCD.
            dcPin   =  5   DC - Data/Command, pin 5 on LCD.
            sdinPin = 11   DN(MOSI) - Serial data, pin 6 on LCD.
            sclkPin = 13   SCLK - Serial clock, pin 7 on LCD.
            blPin   =  9   LED - Backlight LED, pin 8 on LCD.
       
       Parameters:
         None
    */
    Nokia5110();


    /*
       Constructor: Nokia5110
         Create Nokia5110 object, and specify pin mappings
       
       Parameters:
        uint8_t scePin   - SCE      - Chip select, pin 3 on LCD.
        uint8_t rstPin   - RST      - Reset, pin 4 on LCD.
        uint8_t dcPin    - DC       - Data/Command, pin 5 on LCD.
        uint8_t sdinPin  - DN(MOSI) - Serial data, pin 6 on LCD.
        uint8_t sclkPin  - SCLK     - Serial clock, pin 7 on LCD.
        uint8_t blPin    - LED      - Backlight LED, pin 8 on LCD.
     */    
    Nokia5110(uint8_t scePin, uint8_t rstPin, uint8_t dcPin, uint8_t sdinPin, uint8_t sclkPin, uint8_t blPin);    
        
    /*
       Function: lcdBegin
         Initialise the LCD module. Must be called before updateDisplay.
       
       Parameters:
        None
     */      
    void lcdBegin();    
    
    /*
       Function: setPixel
         Sets a pixel on the display to the specified colour. Note that updateDisplay() must be called
         for changes to be displayed.
       
       Parameters:
        int x - x coordinate (x=0, y=0 is top left)
        int y - y coordinate
        bool val - false=clear, true=set/black
     */       
    void setPixel(int x, int y, bool val);
    
    
    /*
       Function: setContrast
         Set display contrast
       
       Parameters:
         uint8_t contrast - display contract; 50 seems to be a good starting point
     */      
    void setContrast(uint8_t contrast);
    
    
    /*
       Function: updateDisplay
         Refresh display.
       
       Parameters:
         None
     */        
    void updateDisplay();
    
    
    /*
       Function: clearDisplay
         Clear the display; set all pixels to either white/clear (bw=false) or black (bw=true).
         Note that the display won't be updated until after updateDisplay() is called.
       
       Parameters:
         bool bw - false=clear all pixels, true=black/set all pixels
     */      
    void clearDisplay(bool bw);

    
    
  private:
    uint8_t _scePin;   // SCE - Chip select, pin 3 on LCD.
    uint8_t _rstPin;   // RST - Reset, pin 4 on LCD.
    uint8_t _dcPin;    // DC - Data/Command, pin 5 on LCD.
    uint8_t _sdinPin;  // DN(MOSI) - Serial data, pin 6 on LCD.
    uint8_t _sclkPin;  // SCLK - Serial clock, pin 7 on LCD.
    uint8_t _blPin;    // LED - Backlight LED, pin 8 on LCD.
    
    
    // There are two memory banks in the LCD, data/RAM and commands.
    // This function sets the DC pin high or low depending, and then 
    // sends the data byte    
    void LCDWrite(byte data_or_command, byte data);
        
    // Helpful function to directly command the LCD to go to a 
    // specific x,y coordinate.
    void gotoXY(uint8_t x, uint8_t y);
    
    void init(uint8_t scePin, uint8_t rstPin, uint8_t dcPin, uint8_t sdinPin, uint8_t sclkPin, uint8_t blPin);

    
    /* Because the PCD8544 won't let us write individual pixels at a 
    time, this is how we can make targeted changes to the display. */
    byte displayMap[LCD_WIDTH * LCD_HEIGHT / 8] = {
      0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (0,0)->(11,7) ~ These 12 bytes cover an 8x12 block in the left corner of the display 
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (12,0)->(23,7)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, // (24,0)->(35,7)
      0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0x1E, 0x0E, 0x02, 0x00, // (36,0)->(47,7)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (48,0)->(59,7)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (60,0)->(71,7)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (72,0)->(83,7)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (0,8)->(11,15)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (12,8)->(23,15)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, // (24,8)->(35,15)
      0x0F, 0x1F, 0x3F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, // (36,8)->(47,15)
      0xF8, 0xF0, 0xF8, 0xFE, 0xFE, 0xFC, 0xF8, 0xE0, 0x00, 0x00, 0x00, 0x00, // (48,8)->(59,15)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (60,8)->(71,15)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (72,8)->(83,15)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (0,16)->(11,23)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (12,16)->(23,23)
      0x00, 0x00, 0xF8, 0xFC, 0xFE, 0xFE, 0xFF, 0xFF, 0xF3, 0xE0, 0xE0, 0xC0, // (24,16)->(35,23)
      0xC0, 0xC0, 0xE0, 0xE0, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // (36,16)->(47,23)
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3E, 0x00, 0x00, 0x00, // (48,16)->(59,23)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (60,16)->(71,23)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (72,16)->(83,23)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (0,24)->(11,31)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (12,24)->(23,31)
      0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // (24,24)->(35,31)
      0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, // (36,24)->(47,31)
      0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, // (48,24)->(59,31)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (60,24)->(71,31)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (72,24)->(83,31)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (0,32)->(11,39)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (12,32)->(23,39)
      0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, // (24,32)->(35,39)
      0x0F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x03, // (36,32)->(47,39)
      0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (48,32)->(59,39)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (60,32)->(71,39)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (72,32)->(83,39)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (0,40)->(11,47)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (12,40)->(23,47)
      0x00, 0x00, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, // (24,40)->(35,47)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (36,40)->(47,47)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (48,40)->(59,47)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (60,40)->(71,47)
      0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // (72,40)->(83,47) !!! The bottom right pixel!
    };
    
};

#endif