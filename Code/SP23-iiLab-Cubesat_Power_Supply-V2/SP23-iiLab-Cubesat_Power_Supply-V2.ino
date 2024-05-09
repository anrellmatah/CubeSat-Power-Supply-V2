/*** ****************************************************
 *  Title: Cubesat Power Supply Code V1.1
 *  Name(s): Anyell Mata, Christopher McCormick
 *  Date: 4-4-23
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
 *  V2:
 *  The code demonstrates the old code and example code. There are issues with the touchscreen getPoint() function
 *  which appears to run only once. Need to organize SD card wiritng functions.
 ********************************************************/

#include <Adafruit_GFX.h>
#include <SPI.h>
#include "Adafruit_HX8357.h"
#include "TouchScreen.h"
#include "constants.h"
#include "power.h"
#include "sd_card_writer.h"
#include "lcd.h"

// Uncomment if Diagnostics are desired.
//#define PRINT_DIAG 1
//#define LOG_SERIAL 1
//#define ENABLE_TOUCH 1

// Timing Variables and Defintions
#define REFRESH_TIME_MS 200
#define REFRESH_SENSORS_MS 100
#define SERIAL_SEND_DATA_MS 200
#define TOUCH_LCD_QUERY_MS 100

uint32_t next_refresh_lcd_ms = 100;
uint32_t next_refresh_sens_ms = 100;
uint32_t next_serial_send_ms = 100;
uint32_t next_sd_refresh_ms = 100;
uint32_t next_ts_query_ms = 100;

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
	Serial.begin(115200);
  Serial.println();
  Serial.println("HX8357D Test!");
  
  lcd_init();
//  ts_init();
//  meters_init();
  
  // Perform initial write of LCD Options
  lcd_print_power(outputs);  

  tft.begin();
  tft.fillScreen(HX8357_BLACK);
	
  // make the color selection boxes
  tft.fillRect(0, 0, BOXSIZE, BOXSIZE, HX8357_RED);
  tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_YELLOW);
  tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, HX8357_GREEN);
  tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, HX8357_CYAN);
  tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, HX8357_BLUE);
  tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, HX8357_MAGENTA);
  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_BLACK);
  tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
   // select the current color 'red'
  tft.drawRect(0, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
  currentcolor = HX8357_RED;
  

  
	Serial.println("finished setup");
  Serial.println(F("Touch Paint!"));
}

void loop()
{

	collect_sens_data();
	refresh_lcd();
/*	#ifdef LOG_SERIAL
		if (serial_button.curr_state == 1)
			send_data_serial();		
	#endif
*/
	lcd_button_print();
/*	#ifdef ENABLE_TOUCH
		touch_query();
	#endif
*/
	send_data_sd();

//##############
  // Retrieve a point  
  TSPoint p = ts.getPoint();
  Serial.println("completed getPoint");
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
//  if (p.z < MINPRESSURE || p.z > MAXPRESSURE) {
//     return;
//  }

  Serial.print("X = "); Serial.print(p.x);
  Serial.print("\tY = "); Serial.print(p.y);
  Serial.print("\tPressure = "); Serial.println(p.z);  
 
  // Scale from ~0->1000 to tft.width using the calibration #'s
  p.x = map(p.x, TS_MINX, TS_MAXX, 0, tft.width());
  p.y = map(p.y, TS_MINY, TS_MAXY, 0, tft.height());
  
//##############

/*
  Serial.print("("); Serial.print(p.x);
  Serial.print(", "); Serial.print(p.y);
  Serial.println(")");
*/
    
  if (p.y < BOXSIZE) {
     oldcolor = currentcolor;

     if (p.x < BOXSIZE) { 
       currentcolor = HX8357_RED; 
       tft.drawRect(0, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*2) {
       currentcolor = HX8357_YELLOW;
       tft.drawRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*3) {
       currentcolor = HX8357_GREEN;
       tft.drawRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*4) {
       currentcolor = HX8357_CYAN;
       tft.drawRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*5) {
       currentcolor = HX8357_BLUE;
       tft.drawRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*6) {
       currentcolor = HX8357_MAGENTA;
       tft.drawRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     } else if (p.x < BOXSIZE*7) {
       currentcolor = HX8357_WHITE;
       tft.drawRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_RED);
     } else if (p.x < BOXSIZE*8) {
       currentcolor = HX8357_BLACK;
       tft.drawRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
     }
    if (oldcolor != currentcolor) {
        if (oldcolor == HX8357_RED) 
          tft.fillRect(0, 0, BOXSIZE, BOXSIZE, HX8357_RED);
        if (oldcolor == HX8357_YELLOW) 
          tft.fillRect(BOXSIZE, 0, BOXSIZE, BOXSIZE, HX8357_YELLOW);
        if (oldcolor == HX8357_GREEN) 
          tft.fillRect(BOXSIZE*2, 0, BOXSIZE, BOXSIZE, HX8357_GREEN);
        if (oldcolor == HX8357_CYAN) 
          tft.fillRect(BOXSIZE*3, 0, BOXSIZE, BOXSIZE, HX8357_CYAN);
        if (oldcolor == HX8357_BLUE) 
          tft.fillRect(BOXSIZE*4, 0, BOXSIZE, BOXSIZE, HX8357_BLUE);
        if (oldcolor == HX8357_MAGENTA) 
          tft.fillRect(BOXSIZE*5, 0, BOXSIZE, BOXSIZE, HX8357_MAGENTA);
        if (oldcolor == HX8357_WHITE) 
          tft.fillRect(BOXSIZE*6, 0, BOXSIZE, BOXSIZE, HX8357_WHITE);
        if (oldcolor == HX8357_BLACK) 
          tft.fillRect(BOXSIZE*7, 0, BOXSIZE, BOXSIZE, HX8357_BLACK);
     }
  }
  if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
  }
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
	if (sd_button.curr_state == 1)
	{
		if (next_sd_refresh_ms < millis())
		{
      writeDataToSD();
//			write_power_data(outputs);
			next_sd_refresh_ms + millis()+REFRESH_TIME_MS;
		}
	}	
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
