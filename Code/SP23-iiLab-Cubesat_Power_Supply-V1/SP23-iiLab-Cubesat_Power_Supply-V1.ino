/*** ****************************************************
 *  Title: Cubesat Power Supply Code V1.0
 *  Name(s): Anyell Mata, Christopher McCormick
 *  Date: 2-6-23
 *  Description:
 *  
 *  V1.0: Anyell
 *  This code will utilize an Adafruit Feather M0 Proto and a 320x480 TFT Touchscreen Breakout
 *  to display voltage, current, power values that are supplying DORA cubesat. Logged data is stored
 *  on a built-in microSD holder. 
 ********************************************************/

#include <SPI.h>
#include "constants.h"
#include "power.h"
#include "sd_card_writer.h"
#include "lcd.h"

// Uncomment if Diagnostics are desired.
//#define PRINT_DIAG 1
#define LOG_SERIAL 1
#define ENABLE_TOUCH 1

// Timing Variables and Defintions
#define REFRESH_TIME_MS 20
#define REFRESH_SENSORS_MS 1
#define SERIAL_SEND_DATA_MS 20
#define TOUCH_LCD_QUERY_MS 10

uint32_t next_refresh_lcd_ms = 0;
uint32_t next_refresh_sens_ms = 0;
uint32_t next_serial_send_ms = 0;
uint32_t next_sd_refresh_ms = 0;
uint32_t next_ts_query_ms = 0;

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
	Serial.begin(9600);
	Serial.println("test");
	delay(1000);
	lcd_init();
	ts_init();
	meters_init();
	delay(1000);

	// Perform initial write of LCD Options
	lcd_print_power(outputs);
	Serial.println("finished setup");
}

void loop()
{
	collect_sens_data();
	refresh_lcd();
	#ifdef LOG_SERIAL
		if (serial_button.curr_state == 1)
			send_data_serial();		
	#endif
	lcd_button_print();

	#ifdef ENABLE_TOUCH
		touch_query();
	#endif

	send_data_sd();
}

void touch_query()
{
	if (next_ts_query_ms < millis())
	{
		touch_listener();
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
			write_power_data(outputs);
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
