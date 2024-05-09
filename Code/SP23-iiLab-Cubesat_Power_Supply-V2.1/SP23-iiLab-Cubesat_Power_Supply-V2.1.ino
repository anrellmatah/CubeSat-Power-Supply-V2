/*** ****************************************************
 *  Title: Cubesat Power Supply Code
 *  Name(s): Anyell Mata, Christopher McCormick
 *  Date: 4-10-23
 *  Description:
 *  
 *  V1.0: Anyell
 *  This code will utilize an Adafruit Feather M0 Proto and a 320x480 TFT Touchscreen Breakout
 *  to display voltage, current, power values that are supplying DORA cubesat. Logged data is stored
 *  on a built-in microSD holder. 
 *  
 *  V1.1:
 *  Components will be utilizing SPI instead of 8-bit wiring because it would require mapping the correct
 *  pin definitions for the chosen microcontroller, which is unavailable at the moment.
 *  
 *  V2.0:
 *  The code demonstrates the old code and example code. There are issues with the touchscreen getPoint() function
 *  which appears to run only once. Need to organize SD card wiritng functions.
 *  
 *  V2.1:
 *  The 'lcd.h' library must be formatted for the dimensions of the new screen which is slightly larger and also rotated 90 degrees. 
 *  Changes to the functions have been made; some easier to read with purpose of expanding on Christopher's functions. The code
 *  also configures the interrupt output for the INA260's in order to turn the panel LED's on.
 ********************************************************/

#include <Adafruit_GFX.h>
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
//#include "constants.h"
#include "power.h"
#include "sd_card_writer.h"
#include "lcd.h"
#include <Grafici.h>

// Uncomment if Diagnostics are desired.
//#define PRINT_DIAG 1
//#define LOG_SERIAL 1
//#define ENABLE_TOUCH 1

// Timing Variables and Defintions
#define REFRESH_TIME_MS 200
#define REFRESH_SENSORS_MS 100
#define SERIAL_SEND_DATA_MS 200
#define TOUCH_LCD_QUERY_MS 100

uint32_t next_refresh_lcd_ms = 1000;
uint32_t next_refresh_sens_ms = 1000;
uint32_t next_serial_send_ms = 1000;
uint32_t next_sd_refresh_ms = 1000;
uint32_t next_ts_query_ms = 1000;

struct power_output outputs[POWER_METER_COUNT];

// Function Pre-Declarations:
void collect_sens_data();
void refresh_lcd();
void send_data_serial();
void touch_query();
void send_data_sd();
void collect_sens_data();

void setup()
{
  delay(1000);
	Serial.begin(115200);               //Set baud rate for components. We want a relatively high baud rate so pcik 115200.

  lcd_init();
//  ts_init();
  meters_init();  
  lcd_print_power(outputs);           // Perform initial write of LCD Options
/*	
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, HX8357_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_YELLOW);

   // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
  currentcolor = HX8357_RED;
*/
  currentcolor = HX8357_RED;
	Serial.println("finished setup");
  Serial.println(F("Touch Paint!"));
      SDCardInit();
}

  uint8_t i;
//  struct power_output outputs[POWER_METER_COUNT];

void loop()
{
	collect_sens_data();

	refresh_lcd();

/*	#ifdef LOG_SERIAL
		if (serial_button.curr_state == 1)
			send_data_serial();		
	#endif
*/

//	lcd_button_print();

/*	#ifdef ENABLE_TOUCH
		touch_query();
	#endif
*/

//##############
  // Retrieve a point  
  TSPoint p = ts.getPoint();
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
//  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
//     return;
//  }

  Serial.print("X = "); Serial.print(p.y);            //width
  Serial.print("\tY = "); Serial.print(p.x);          //height
  Serial.print("\tPressure = "); Serial.println(p.z);  
 
  // Scale from ~0->1000 to tft.width using the calibration #'s
  p.x = tft.height() - map(p.x, TS_MINX, TS_MAXX, 0, tft.height());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.width());  //https://forum.arduino.cc/t/solved-touchscreen-display-graphics-rotated-but-touch-input-is-not/593518/3
  
//##############
  Serial.println("Finished!");
  delay(10000);
/*
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
*/
    
  if (p.y < BOXSIZE) 
  {
    oldcolor = currentcolor;
    if (p.x < BOXSIZE) 
    { 
       currentcolor = HX8357_RED; 
       tft.drawRect(0, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
    }
    if (oldcolor != currentcolor) 
    {
        if (oldcolor == HX8357_RED) 
          tft.fillRect(0, 0, BOXSIZE, BOXSIZE, HX8357_RED);
    }
  }
//  if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
    tft.fillCircle(p.y, p.x, PENRADIUS, currentcolor);

//   dataString += String(i) + ", " + String(outputs[i].voltage) + ", " + String(outputs[i].current + ", " + String(outputs[i].power;
  send_data_sd();
  
  delay(100);
}

void touch_query()
{
	if (next_ts_query_ms < millis())
	{
//		touch_listener();
		lcd_button_print();
		next_ts_query_ms = millis()+TOUCH_LCD_QUERY_MS;
	}
}

void send_data_serial()
{
	if (next_serial_send_ms < millis())
	{
		print_power(outputs);	
		next_serial_send_ms = millis()+SERIAL_SEND_DATA_MS;
	}
}

void send_data_sd()
{
//	if (sd_button.curr_state == 1)
//	{
		if (next_sd_refresh_ms < millis())
		{
      writeDataToSD();
//			write_power_data(outputs);
			next_sd_refresh_ms + millis()+REFRESH_TIME_MS;
		}
//	}	
}

void collect_sens_data()
{
	if (next_refresh_sens_ms < millis())
	{
		get_data_sensors(outputs);
		next_refresh_sens_ms + millis()+REFRESH_TIME_MS;
	}
}

void refresh_lcd()
{
	if (next_refresh_lcd_ms < millis())
	{
		lcd_print_power_faster(outputs);
		next_refresh_lcd_ms = millis()+REFRESH_TIME_MS;
	}
}
