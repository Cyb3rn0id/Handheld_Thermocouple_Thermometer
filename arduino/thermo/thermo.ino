/*
 * Handheld Thermocouple Thermometer
 * by Giovanni 'Cyb3rn0id' Bernardo
 * https://www.settorezero.com
 * 
 *
 * Used Digistump Digispark (ATtiny85)
 * 
 * Libraries to be installed:
 * - TinyWireM by Adafruit
 * - Tiny4kOLED by Stephen Denne (datacute)
 * - MAX6675 by Adafruit
 *
 * Connections:
 * 9V Battery to Vin/GND on Digispark
 * MAX6675 to Digispark: SCK=>P4, CS=>P3, SO=>P1, VCC=>5V
 * 0.91" OLED to Digispark: SCL=>P2, SDA=>P0, VCC=>5V
 *
 */
 
// Digispark has his TinyWire library installed but doesn't works well with
// the display (my issue #39: https://github.com/datacute/Tiny4kOLED/issues/39)
// so it's needed to include TinyWireM from Adafruit
#include <TinyWireM.h>

// Defining this constant, 'print' function will be disabled, saving space on the ATtiny
// (anyway if you don't use print function, will not included anyway). Print function 
// must be not used since there will be not enough space in the flash memory
#define TINY4KOLED_NO_PRINT
#include <Tiny4kOLED.h>

// MAX6675 library by Adafruit
#include "max6675.h"

// Include the big font coming with the Tiny4KOLED library. This font contains only
// numbers, minus and dots/commas
#include "font16x32digits.h"
const DCfont *currentFont = FONT16X32DIGITS;

// MAX6675 breakout board connections to Digistump
// MAX6675 thermocouple(SCLK, CS, S0);
MAX6675 thermocouple(4, 3, 1); // CLK, CS, DO

void setup() 
  {
  oled.begin();
  //oled.enableChargePump();
  oled.setFont(currentFont);
  //oled.setRotation(0);
  oled.clear();
  oled.on();
  // Switch the half of RAM that we are writing to, to be the half that is non currently displayed
  oled.switchRenderFrame();
  delay(500);
  }

void loop() 
  {
  oled.clear();
  double t = thermocouple.readCelsius(); // read thermocouple
  int t1 = (int)t; // temperature as integer
  double dec = t - t1; // decimal part as 0.xx
  int t2 = (int)(dec*100); // decimal part as integer (0.xx => xx)

  // Notice: MAX6675 can read from 0 to 1023.75°C
  // So there will be not negative temperatures. 
  // Anyway I included this part since can be useful for other applications
  
  // If negative temperature, print minus sign and convert number in positive
  /*
  if (t1<0)
    {
    oled.write('-'); // print minus sign
    t1*=-1; 
    t2*=-1;  
    }
   */
  int i;
  // Print integer part
  if (t1>99)
    {
    i = t1/100; // first digit
    oled.write(i+'0'); // print the number in ascii format ('0' will be interpreted as ascii code of '0')
    t1=t1-(i*100); // remaining tenths
    if (t1<10) // remaining part can be 0x if the number was 10x
      {
      // so I must write the 0
      oled.write('0');    
      }
    }
    
  if (t1>9) // tenths
      {
      // tenth
      i=t1/10;
      oled.write(i+'0');     
      t1=t1-(i*10); // remaining units
      }
      
   oled.write(t1+'0'); // print units

   oled.write('.'); // decimal separator

    // Decimal part is always 00, 25, 50 or 75 since
    // the MAX6675 has only 2 bits for decimal part (quarter of a grade)
    if (t2>0) // decimal part is not zero
      {
      // tenth
      i=t2/10;
      oled.write(i+'0');     
      t2=t2-(i*10); // units
      oled.write(t2+'0'); 
      }
    else // is 0
      {
      oled.write('0');
      oled.write('0');
      }
  
  oled.switchFrame();
  delay(1000); // give some delay between readings
  }
