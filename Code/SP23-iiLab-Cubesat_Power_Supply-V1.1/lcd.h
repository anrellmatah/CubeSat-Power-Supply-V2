#include <Adafruit_GFX.h>
#include <SPI.h>
#include "Adafruit_HX8357.h"
//#include <Adafruit_STMPE610.h>
#include "TouchScreen.h"

//LCD Pin definitions
//#define STMPE_CS 6//
#define TFT_CS   8
#define TFT_DC   9
//#define SD_CS    7//

#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like

// These are the four touchscreen analog pins
#define YP A7  // must be an analog pin, use "An" notation!
#define XM A4  // must be an analog pin, use "An" notation!
#define YM 19   // can be a digital pin
#define XP 20   // can be a digital pin

#define PRESSURE_TRIGGER 100
#define TS_MINX 110
#define TS_MINY 80
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 100
#define MAXPRESSURE 1000

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;


// Box Coordinates
struct button
{
	uint16_t x;
	uint16_t y;
	uint16_t w;
	uint16_t h;
	uint8_t last_state;
	uint8_t curr_state;
};

struct button serial_button = {200,180,70,50,1,0};
struct button sd_button = {50,180,70,50,1,0};

// Function Declarations
void lcd_init();
//void ts_init();
void lcd_print_power_faster(struct power_output outputs[POWER_METER_COUNT]);
void lcd_button_print();
//void touch_listener();
//void lcd_test_print();
uint8_t in_area_button(uint16_t x, uint16_t y,uint8_t z, struct button but_arg);

void lcd_init()
{
	// LCD Initialization
	tft.begin();

	#ifdef PRINT_DIAG
		uint8_t x = tft.readcommand8(HX8357_RDPOWMODE);
		Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
		x = tft.readcommand8(HX8357_RDMADCTL);
		Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
		x = tft.readcommand8(HX8357_RDCOLMOD);
		Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
		x = tft.readcommand8(HX8357_RDDIM);
		Serial.print("Image Format: 0x"); Serial.println(x, HEX);
		x = tft.readcommand8(HX8357_RDDSDR);
		Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 

		Serial.println(F("Benchmark                Time (microseconds)"));
	#endif

	tft.setRotation(3);
}
/*
void ts_init()
{
	pinMode(STMPE_CS,OUTPUT);

	// Touch Screen initialization
	if (! ts.begin()) {
		Serial.println("STMPE not found!");
		while(1);
	}
}
*/
// seeing if adjusting printing so only that values
void lcd_print_power_faster(struct power_output outputs[POWER_METER_COUNT])
{
	// Sensor 1
	tft.fillRect(30,18,94,60, HX8357_BLACK);
	tft.setCursor(30,18);
	tft.setTextColor(HX8357_WHITE); tft.setTextSize(2);
	tft.println(outputs[0].voltage);
	tft.setCursor(30,18*2);
	tft.println(outputs[0].current);
	tft.setCursor(30,18*3);
	tft.println(outputs[2].power);

	// Sensor 2
	tft.fillRect(30+150,18,94,60, HX8357_BLACK);
	tft.setTextColor(HX8357_WHITE); tft.setTextSize(2);
	tft.setCursor(30+150,18);
	tft.println(outputs[1].voltage);
	tft.setCursor(30+150,18*2);
	tft.println(outputs[1].current);
	tft.setCursor(30+150,18*3);
	tft.println(outputs[1].power);

	// Sensor 3
	tft.fillRect(30,18+80,94,60, HX8357_BLACK);
	tft.setTextColor(HX8357_WHITE); tft.setTextSize(2);
	tft.setCursor(30,80+18*1);
	tft.println(outputs[2].voltage);
	tft.setCursor(30,80+18*2);
	tft.println(outputs[2].current);
	tft.setCursor(30,80+18*3);
	tft.println(outputs[2].power);

}

void lcd_print_power(struct power_output outputs[POWER_METER_COUNT])
{
	tft.fillScreen(HX8357_BLACK);
	tft.setCursor(0,0);
	tft.setTextColor(HX8357_WHITE); tft.setTextSize(2);

	tft.print("Sensor ");tft.print(0);tft.println(":");
	tft.println("V: "); //tft.println(outputs[0].voltage); 
	tft.println("A: "); //tft.println(outputs[0].current); 
	tft.println("P: "); //tft.println(outputs[0].power); 

	tft.setCursor(150,0);
	tft.print("Sensor "); tft.print(1);tft.println(":");
	tft.setCursor(150,18);
	tft.print("V: "); //tft.println(outputs[1].voltage);
	tft.setCursor(150,18*2);
	tft.print("A: "); //tft.println(outputs[1].current);
	tft.setCursor(150,18*3);
	tft.print("A: "); //tft.println(outputs[1].power);
	

	tft.setCursor(0,80);
	tft.print("Sensor ");tft.print(2);tft.println(":");
	tft.println("V: "); //tft.println(outputs[2].voltage); 
	tft.println("A: "); //tft.println(outputs[2].current); 
	tft.println("P: "); //tft.println(outputs[2].power); 

}

void lcd_button_print()
{
	// Serial Button Updates
	if (serial_button.curr_state != serial_button.last_state)
	{
		serial_button.last_state = serial_button.curr_state;
		tft.fillRect(serial_button.x,serial_button.y,serial_button.w,serial_button.h, serial_button.curr_state == 1 ? HX8357_RED : HX8357_GREEN);
		tft.setCursor(serial_button.x,serial_button.y+serial_button.h/2);
		tft.setTextColor(HX8357_BLACK); tft.setTextSize(2);
		tft.println("Serial");
	}
	// SD Button Updates
	if (sd_button.curr_state != sd_button.last_state)
	{
		sd_button.last_state = sd_button.curr_state;
		if (sd_button.curr_state == 0)
		{
			close_file();
		}
		else if (sd_button.curr_state == 1)
		{
			setup_sdcard();
			write_str_char(header);
		}
		tft.fillRect(sd_button.x,sd_button.y,sd_button.w,sd_button.h, sd_button.curr_state == 1 ? HX8357_RED : HX8357_GREEN);
		tft.setCursor(sd_button.x,sd_button.y+sd_button.h/2);
		tft.setTextColor(HX8357_BLACK); tft.setTextSize(2);
		tft.println("SD");
	}
}

uint8_t in_area_button(uint16_t x, uint16_t y,uint8_t z, struct button but_arg)
{
	if (but_arg.x < x && but_arg.x+but_arg.w > x)
	{
		if (but_arg.y < y && but_arg.y+but_arg.h > y)
			return 1;
	}
	return 0;
}
/*
void ts_test()
{
	uint16_t x, y;
	uint8_t z;
	if (ts.touched()) {
		// read x & y & z;
		while (! ts.bufferEmpty()) {
			Serial.print(ts.bufferSize());
			ts.readData(&x, &y, &z);
			Serial.print("->("); 
			Serial.print(x); Serial.print(", "); 
			Serial.print(y); Serial.print(", "); 
			Serial.print(z);
			Serial.println(")");
		}
		ts.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints, in this example unneeded depending in use
	}
}
*/
/*
void touch_listener()
{
	uint16_t x, y,swap;
	uint8_t z;
	if (ts.touched())
	{
		while (!ts.bufferEmpty()) {
			ts.readData(&x, &y, &z);
			x = map(x, 0, 4095, 0, 240); // need to reverse order
			y = map(y, 0, 4095, 0, 320);

			swap = x;
			x = y;
			y = swap;

			Serial.print(ts.bufferSize());
			Serial.print("->("); 
			Serial.print(x); Serial.print(", "); 
			Serial.print(y); Serial.print(", "); 
			Serial.print(z);
			Serial.println(")");
			if (z > PRESSURE_TRIGGER)
			{
				if (in_area_button(x,y,z,serial_button) == 1)
				{
					serial_button.last_state = serial_button.curr_state;
					serial_button.curr_state = !serial_button.curr_state;
				}
				if (in_area_button(x,y,z,sd_button) == 1)
				{
					sd_button.last_state = sd_button.curr_state;
					sd_button.curr_state = !sd_button.curr_state;
				}
			}
		}
		ts.writeRegister8(STMPE_INT_STA, 0xFF); // reset all ints, in this example unneeded depending in use
	}
}
*/
void lcd_test_print()
{
	tft.fillScreen(HX8357_BLACK);
	tft.setCursor(0,0);
	tft.setTextColor(HX8357_WHITE); tft.setTextSize(1);
	tft.println("Test print");
}
