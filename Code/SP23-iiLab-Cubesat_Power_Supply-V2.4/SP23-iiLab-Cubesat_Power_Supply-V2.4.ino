/*** ****************************************************
 *  Title: Cubesat Power Supply Code
 *  Name(s): Anyell Mata, Christopher McCormick
 *  Date: 5-17-23
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
 ********************************************************/

#include <Adafruit_GFX.h>
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
  delay(3000);
	Serial.begin(115200);               //Set baud rate for components. We want a relatively high baud rate so pick 115200.

  pinMode(6, OUTPUT);

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
      SDCardInit();
//  pinMode(2, INPUT);
  pinMode(9, OUTPUT);  
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);



} //END of LOOP

  uint8_t i;
//  struct power_output outputs[POWER_METER_COUNT];

int Xpos = 0;

void loop() //BEGINNING OF LOOP
{
/*
	if (digitalRead(2) == LOW)
  {
    digitalWrite(4, HIGH);
  }
  else
  {
    digitalWrite(4, LOW);    
  }
*/	
	collect_sens_data();

	refresh_lcd();

  for (i=0;i<3;i++)
  {
    if (outputs[i].current > 2)
    {
      digitalWrite(9+i, HIGH);
    }
    else
    {
      digitalWrite(9+i, LOW);
    }
  }
/*
  for (i=0;i<3;i++)
  {
        power_meter_arr[i].setAlertType(INA260_ALERT_NONE);
    power_meter_arr[i].setAlertPolarity(INA260_ALERT_POLARITY_NORMAL);

    power_meter_arr[i].setAlertType(INA260_ALERT_OVERVOLTAGE);    // set alert mode to overcurrent
    power_meter_arr[i].setAlertLimit(1000);

    power_meter_arr[i].setAlertLatch(INA260_ALERT_LATCH_ENABLED);
  }
*/
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
  TSPoint p = ts.getPoint();    // This function retrieves a point on the TFT resistive screen which scales from 0-1024 on both axes.


  Serial.print("X = "); Serial.print(p.y);            //width
  Serial.print("\tY = "); Serial.print(p.x);          //height. Swapped because of the orientation.
  Serial.print("\tPressure = "); Serial.print(p.z);   //The z-axis on the TFT screen represents the pressure applied to the resistive screen.

 
  // Scale from ~0->1024 to tft.width using the calibration #'s
  p.y = map(p.y, 200, 870, 0, tft.height());              // which should be 320 pixels high. 
  p.x = tft.width() - map(p.x, 120, 900, 0, tft.width()); // which should be 480 pixels wide. https://forum.arduino.cc/t/solved-touchscreen-display-graphics-rotated-but-touch-input-is-not/593518/3
 
//##############


  Serial.print("\tXX = "); Serial.print(p.y);            //width
  Serial.print("\tYY = "); Serial.print(p.x);          //height. Swapped because of the orientation.

    
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

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (p.z > MINPRESSURE || p.z < MAXPRESSURE) 
  {
    tft.fillCircle(p.x, p.y, PENRADIUS, currentcolor);
//void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

  }
    
//  if (((p.y-PENRADIUS) > BOXSIZE) && ((p.y+PENRADIUS) < tft.height())) {
//   dataString += String(i) + ", " + String(outputs[i].voltage) + ", " + String(outputs[i].current + ", " + String(outputs[i].power;

//#############################################################

//  tft.line();

//  tft.stroke();

  if (Xpos >= tft.width()) 
  {
    Xpos = 0;
//    TFTscreen.background(250,16,200);
  }
  else 
  {
    Xpos++;
  }

//#############################################################

  send_data_sd();


  Serial.println();
  delay(100);
}   //END OF LOOP

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
