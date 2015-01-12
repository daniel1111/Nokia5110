/* Test sketch for Nokia5110 lib. Draws a square on the LCD, wipes it, then starts again. */

#include <Nokia5110.h>
#include <SPI.h>

Nokia5110 *disp;

void setup() 
{
 
/*
Create Nokia5110 display object with defaults of:
  Description             LCD pin    Arduino pin
  SCE - Chip select       3          7
  RST - Reset             4          6
  DC  - Data/Command      5          5
  DN(MOSI) - Serial data, 6          11
  SCLK - Serial clock     7          13
  LED - Backlight LED     8          9
  
  Nb: Screen dimentions are 84x48 (width x height), and the macros LCD_WIDTH and LCD_HEIGHT are defined for this.
*/  
  disp = new Nokia5110();
  disp->lcdBegin();
  disp->setContrast(30); // defaults to 45 if not set. made need adjusting if the display is unreadable (higher=darker)
}

void loop() 
{
  disp->clearDisplay(false); // clear display buffer
  disp->updateDisplay();     // update display to be blank
  delay(1000);  
  
  drawBox(5,5,  80, 40);
  disp->updateDisplay();  
  delay (1000);
}


// Draw a box with coords topleft = (x1, y1), bottom right = (x2, y2)
void drawBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
  // top line
  for (uint8_t n = x1; n < x2; n++)
    disp->setPixel(n, y1, true);
    
  // right line
  for (uint8_t n = y1; n < y2; n++)
    disp->setPixel(x2, n, true);  
    
  // bottom line
  for (uint8_t n = x1; n < x2; n++)
    disp->setPixel(n, y2, true);
    
  // left line    
  for (uint8_t n = y1; n < y2; n++)
    disp->setPixel(x1, n, true);    
}

