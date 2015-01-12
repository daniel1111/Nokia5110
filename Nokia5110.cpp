
/* Basic Arduino library to drive a Nokia 5100 LCD. 
 * Provides functions to set/clear a pixel, wipe and update 
 * the display. That's about it.
 * - Daniel Swann, Jan 2015.
 * 
 * Based on:
 * 
   Graphics driver and PCD8544 interface code for SparkFun's
   84x48 Graphic LCD.
   https://www.sparkfun.com/products/10168
   
  by: Jim Lindblom 
    adapted from code by Nathan Seidle and mish-mashed with
    code from the ColorLCDShield.
  date: October 10, 2013
  license: Beerware. Feel free to use, reuse, and modify this
  code as you see fit. If you find it useful, and we meet someday,
  you can buy me a beer.
 */


#include "Nokia5110.h"


Nokia5110::Nokia5110()
{
  init(7, 6, 5, 11, 13, 9);
}

Nokia5110::Nokia5110(uint8_t scePin, uint8_t rstPin, uint8_t dcPin, uint8_t sdinPin, uint8_t sclkPin, uint8_t blPin)
{
  init(scePin, rstPin, dcPin, sdinPin, sclkPin, blPin);
}

void Nokia5110::init(uint8_t scePin, uint8_t rstPin, uint8_t dcPin, uint8_t sdinPin, uint8_t sclkPin, uint8_t blPin)
{
  _scePin = scePin;    // SCE - Chip select, pin 3 on LCD.
  _rstPin = rstPin;    // RST - Reset, pin 4 on LCD.
  _dcPin = dcPin;      // DC - Data/Command, pin 5 on LCD.
  _sdinPin = sdinPin;  // DN(MOSI) - Serial data, pin 6 on LCD.
  _sclkPin = sclkPin;  // SCLK - Serial clock, pin 7 on LCD.
  _blPin = blPin;      // LED - Backlight LED, pin 8 on LCD.    
}

void Nokia5110::updateDisplay()
{
  gotoXY(0, 0);
  for (int i=0; i < (LCD_WIDTH * LCD_HEIGHT / 8); i++)
  {
    LCDWrite(LCD_DATA, displayMap[i]);
  }
}

// This function sets a pixel on displayMap to your preferred
// color. 1=Black, 0= white.
void Nokia5110::setPixel(int x, int y, bool bw)
{
  // First, double check that the coordinate is in range.
  if ((x >= 0) && (x < LCD_WIDTH) && (y >= 0) && (y < LCD_HEIGHT))
  {
    byte shift = y % 8;

    if (bw) // If black, set the bit.
      displayMap[x + (y/8)*LCD_WIDTH] |= 1<<shift;
    else   // If white clear the bit.
      displayMap[x + (y/8)*LCD_WIDTH] &= ~(1<<shift);
  }
}


void Nokia5110::setContrast(uint8_t contrast)
{  
  LCDWrite(LCD_COMMAND, 0x21); //Tell LCD that extended commands follow
  LCDWrite(LCD_COMMAND, 0x80 | contrast); //Set LCD Vop (Contrast): Try 0xB1(good @ 3.3V) or 0xBF if your display is too dark
  LCDWrite(LCD_COMMAND, 0x20); //Set display mode
}

void Nokia5110::clearDisplay(bool bw)
{
  for (int i=0; i<(LCD_WIDTH * LCD_HEIGHT / 8); i++)
  {
    if (bw)
      displayMap[i] = 0xFF;
    else
      displayMap[i] = 0;
  }
}

void Nokia5110::gotoXY(uint8_t x, uint8_t y)
{
  LCDWrite(0, 0x80 | x);  // Column.
  LCDWrite(0, 0x40 | y);  // Row.  ?
}

// There are two memory banks in the LCD, data/RAM and commands.
// This function sets the DC pin high or low depending, and then 
// sends the data byte
void Nokia5110::LCDWrite(byte data_or_command, byte data) 
{
  //Tell the LCD that we are writing either to data or a command
  digitalWrite(_dcPin, data_or_command); 

  //Send the data
  digitalWrite(_scePin, LOW);
  SPI.transfer(data); //shiftOut(sdinPin, sclkPin, MSBFIRST, data);
  digitalWrite(_scePin, HIGH);
}

//This sends the magical commands to the PCD8544
void Nokia5110::lcdBegin() 
{
  //Configure control pins
  pinMode(_scePin, OUTPUT);
  pinMode(_rstPin, OUTPUT);
  pinMode(_dcPin, OUTPUT);
  pinMode(_sdinPin, OUTPUT);
  pinMode(_sclkPin, OUTPUT);
  pinMode(_blPin, OUTPUT);
  analogWrite(_blPin, 255);

  SPI.begin();
  SPI.setDataMode(SPI_MODE0);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV2);

  //Reset the LCD to a known state
  digitalWrite(_rstPin, LOW);
  digitalWrite(_rstPin, HIGH);

  LCDWrite(LCD_COMMAND, 0x21); //Tell LCD extended commands follow
  LCDWrite(LCD_COMMAND, 0xB0); //Set LCD Vop (Contrast)
  LCDWrite(LCD_COMMAND, 0x04); //Set Temp coefficent
  LCDWrite(LCD_COMMAND, 0x14); //LCD bias mode 1:48 (try 0x13)
  //We must send 0x20 before modifying the display control mode
  LCDWrite(LCD_COMMAND, 0x20); 
  LCDWrite(LCD_COMMAND, 0x0C); //Set display control, normal mode.
  
  setContrast(45); // default to 45 - seems a reasonable starting value  
}

