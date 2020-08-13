/*********************************************************************
This is an example for Adafuit's Monochrome OLEDs based on SSD1306 drivers
  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98
This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)
Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!
Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans18pt7b.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// '4w2r0', 29x29px
const unsigned char myBitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1c, 0x00, 0x00, 0xf0, 0x18, 0x00, 
  0x07, 0xc0, 0x18, 0x00, 0x1f, 0x00, 0x18, 0x00, 0x03, 0x01, 0x18, 0x00, 0x03, 0x01, 0xd9, 0x00, 
  0x03, 0x01, 0x98, 0x80, 0x03, 0x31, 0x18, 0xc0, 0x3f, 0xf3, 0x18, 0x60, 0x03, 0x03, 0x18, 0x60, 
  0x03, 0x02, 0x18, 0x70, 0x03, 0x06, 0x18, 0x20, 0x07, 0xc4, 0x18, 0x00, 0x07, 0x64, 0x18, 0x80, 
  0x07, 0x30, 0x19, 0xc0, 0x0b, 0x30, 0x19, 0x80, 0x0b, 0x10, 0x7b, 0x00, 0x13, 0x00, 0x26, 0x00, 
  0x13, 0x00, 0x0e, 0x00, 0x23, 0x00, 0x1c, 0x00, 0x03, 0x00, 0x38, 0x00, 0x03, 0x00, 0x60, 0x00, 
  0x03, 0x00, 0xc0, 0x00, 0x03, 0x03, 0x00, 0x00, 0x03, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00
};




void setup()   {                
  // On my display, I had to used 0x3C as the address, something to do with the RESET not being
  // connected to the Arduino. THe 0x3D address below is the address used in the original
  // Adafruit OLED example 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3c);  // initialize with the I2C addr 0x3D (for the 128x64)
  
  display.clearDisplay(); // Make sure the display is cleared
  // Draw the bitmap:
  // drawBitmap(x position, y position, bitmap data, bitmap width, bitmap height, color)
  delay(1000);
  
  /*display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,50); 

  
  display.setFont(&FreeSans18pt7b);
  
  display.println("38");
  display.drawBitmap(98, 34, myBitmap, 29, 29, WHITE);

  // Update the display
  display.display();*/
}

int count = 0;

void loop() {
  
  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,57); 

  
  display.setFont(&FreeSans18pt7b);
  
  display.println(count);
  display.drawBitmap(98, 34, myBitmap, 29, 29, WHITE);
  // Update the display
  display.display();
  count++;
  delay(200);
  display.clearDisplay();
}
