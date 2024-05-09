/*** ****************************************************
 *  Title: Cubesat Power Supply Code
 *  Name(s): Anyell Mata, Christopher McCormick
 *  Date: 6-24-23
 *  Description: This code utilizes a feather M0 basic proto microcontroller and Adafruit's 3.5" 320x480 TFT touchscreen breakout
 *               which will be used in ASU's Interplanetary Intitiative Laboratory's for datalogging and displaying a specialized
 *               cubesat power supply.
 *  
 *  V1.0: Christopher, Anyell
 *  This code uses an Arduino Every and a 320x480 TFT Touchscreen Breakout
 *  to display voltage, current, power values that are supplying DORA cubesat. Logged data is stored
 *  on a built-in microSD holder. All pin definitions were changed for the Arduino Every.
 *  
 *  V1.1: Anyell
 *  Components will be utilizing SPI instead of 8-bit wiring because it would require mapping the correct
 *  pin definitions for the chosen microcontroller, which is unavailable at the moment.
 *  
 *  V2.0: Anyell
 *  The code demonstrates the old code and example code. There are issues with the touchscreen getPoint() function
 *  which appears to run only once. Need to organize SD card wiritng functions.
 *  
 *  V2.1: Anyell
 *  The 'lcd.h' library must be formatted for the dimensions of the new screen which is slightly larger and also rotated 90 degrees. 
 *  Changes to the functions have been made; some easier to read with purpose of expanding on Christopher's functions.
 *  
 *  V2.2: Anyell
 *  The code utilizes the interrupt function to in order to turn on the LED's. The code also works on refreshing the power outputs.
 *  
 *  V2.3: Anyell
 *  The code has been altered to fit the pin definitions of an Adafruit Feahter M0 Basic Proto microcontroller. 
 *  
 *  V2.4: Anyell
 *  The code is being made more efficient and eliminating code that is not being used (I'm not seeing the importance of the SD 
 *  and Serial button, so getting rid of it). 
 *  
 *  V2.5: Anyell
 *  This version implements a graph function as well ]
 ********************************************************/

#include <Adafruit_GFX.h>    // Core graphics library
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "power.h"
#include "sd_card_writer.h"
#include "lcd.h"
#include <Grafici.h>

// Uncomment if Diagnostics are desired.
//#define PRINT_DIAG 1
//#define LOG_SERIAL 1
//#define ENABLE_TOUCH 1

//Initial Definitions
uint32_t next_refresh_ms = 1000;

uint32_t next_refresh_lcd_ms = 1000;
uint32_t next_refresh_sens_ms = 1000;
uint32_t next_serial_send_ms = 1000;
uint32_t next_sd_refresh_ms = 1000;
uint32_t next_ts_query_ms = 1000;

// Timing Variables and Defintions
#define REFRESH_TIME_MS 200
#define REFRESH_SENSORS_MS 100
#define SERIAL_SEND_DATA_MS 200
#define TOUCH_LCD_QUERY_MS 100

#define NEXT_REFRESH 1000

#define LiteLVL 255                   //PWM output for LCD backlite with a maximum of 255(brightest).

struct power_output outputs[POWER_METER_COUNT];

void setup()
{
  delay(5000);                        //It is good practice to start a delay to ensure the controller doesn't get "bricked".
	Serial.begin(115200);               //Set baud rate for components. We want a higher baud rate so pick 115200.
  pinMode(13, OUTPUT);                //Built-In LED pin on Feather M0.
  lcd_init(LiteLVL);                  //Call for LCD initialization function and pass over backlight PWM value.
  meters_init();                      //Call for INA power monitor initialization function.
  lcd_print_power(outputs);           //Perform initial write of LCD screen

/*	
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, HX8357_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_YELLOW);

   // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
  currentcolor = HX8357_RED;
*/
  SDCardInit();

	Serial.println("finished setup");
} //END of SETUP

  uint8_t i, j;
  int xx, yy;
  int Xpos = 0;
  
void loop() //BEGINNING OF LOOP
{	
  digitalWrite(13, HIGH);           //Utilize the built-in LED and turn it on at the start of the loop.
  if (next_refresh_ms < millis())
    {
 //     lcd_button_print();
 //     print_power(outputs);
      get_data_sensors(outputs);
      lcd_print_power_faster(outputs);
      write_power_data(outputs);
//      writeDataToSD();
      next_refresh_ms = millis() + NEXT_REFRESH;
    }



/*	#ifdef LOG_SERIAL
		if (serial_button.curr_state == 1)
			send_data_serial();		
	#endif
*/

/*	#ifdef ENABLE_TOUCH
		touch_query();
	#endif
*/

  TSPoint p = ts.getPoint();    // This function retrieves a point on the TFT resistive screen which scales from 0-1024 on both axes.

//  Serial.print("X = "); Serial.print(p.y);            //width
//  Serial.print("\tY = "); Serial.print(p.x);          //height. Swapped because of the orientation.
//  Serial.print("\tPressure = "); Serial.print(p.z);   //The z-axis on the TFT screen represents the pressure applied to the resistive screen.
 
  // Scale from ~0->1024 to tft.width using the calibration #'s
  yy = map(p.y, 200, 870, 0, tft.height());              // which should be 320 pixels high. 
  xx = tft.width() - map(p.x, 120, 900, 0, tft.width()); // which should be 480 pixels wide. https://forum.arduino.cc/t/solved-touchscreen-display-graphics-rotated-but-touch-input-is-not/593518/3

//  Serial.print("\tXX = "); Serial.print(p.y);          //width
//  Serial.print("\tYY = "); Serial.print(p.x);          //height. Swapped because of the orientation.

  if (p.z > MINPRESSURE || p.z < MAXPRESSURE)   // we have some minimum pressure we consider 'valid'. pressure of 0 means no pressing!
  {
  
  }
    
//   dataString += String(i) + ", " + String(outputs[i].voltage) + ", " + String(outputs[i].current + ", " + String(outputs[i].power;

  tft.setRotation(1);

  tft.drawPixel(61+Xpos, 259-map(outputs[0].power, 0, 4000, 0, 120), HX8357_RED);
  tft.drawPixel(61+Xpos, 259-map(outputs[1].power, 0, 4000, 0, 120), HX8357_GREEN);
  tft.drawPixel(61+Xpos, 259-map(outputs[2].power, 0, 4000, 0, 120), HX8357_BLUE);

  if (Xpos >= 390)
  {
    Xpos=0;
    tft.fillRect(61,120,390,140,HX8357_BLACK);
  }
  else 
  {
    Xpos++;
  }
  
  digitalWrite(13, LOW);
  delay(150);
}   //END OF LOOP
