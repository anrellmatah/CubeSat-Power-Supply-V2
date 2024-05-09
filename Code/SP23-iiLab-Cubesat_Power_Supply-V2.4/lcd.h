#include <Adafruit_GFX.h>
#include <SPI.h>
#include "Adafruit_HX8357.h"
//#include <Adafruit_STMPE610.h>
#include "TouchScreen.h"

// Color definitions
#define BLACK    0x0000
#define BLUE     0x001F
#define RED      0xF800
#define GREEN    0x07E0
#define CYAN     0x07FF
#define MAGENTA  0xF81F
#define YELLOW   0xFFE0 
#define WHITE    0xFFFF

//LCD Pin definitions
//#define STMPE_CS 6    //This was used with the old touch screen controller
#define TFT_CS   12
#define TFT_DC   13
#define BACKLITE_PIN  5 //Backlight pin which requires PWM to adjust lite.

#define TFT_RST -1  // dont use a reset pin, tie to arduino RST if you like

// These are the four touchscreen analog pins
#define XM A2  // must be an analog pin, use "An" notation!
#define YM A3   // can be a digital pin
#define XP A4   // can be a digital pin
#define YP A5  // must be an analog pin, use "An" notation!

#define PRESSURE_TRIGGER 100
#define TS_MINX 120
#define TS_MINY 100
#define TS_MAXX 900
#define TS_MAXY 940

#define MINPRESSURE 100
#define MAXPRESSURE 2000

#define Xoffset 160
#define Yoffset 24
int Xoff = 35;
int Yoff = 16;

Adafruit_HX8357 tft = Adafruit_HX8357(TFT_CS, TFT_DC, TFT_RST);
//Adafruit_STMPE610 ts = Adafruit_STMPE610(STMPE_CS);

// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 302);

// Size of the color selection boxes and the paintbrush size
#define BOXSIZE 40
#define PENRADIUS 7
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

void lcd_init() // LCD Initialization
{
  Serial.println(F("HX8357D Test!")); //Print a line to ensure code is proceeding
  tft.begin();                        //Initialize touchscreen
  tft.setRotation(1);                 //Rotate the screen from its default rotation so that it is aligned with the panel.
  tft.fillScreen(HX8357_BLACK);       //Clear the screen by fill the entire screen black.
  Serial.print("Width in pixels = "); Serial.println(tft.width());
  Serial.print("Height in pixels = "); Serial.println(tft.height());
  pinMode(BACKLITE_PIN, OUTPUT);
  int LiteLVL = 255;                   //Max is 255
  analogWrite(BACKLITE_PIN, LiteLVL); //PWM value for TFT backlight
  Serial.print("Backlight PWM level = "); Serial.println(LiteLVL);
  
/*
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
*/
//  tft.setAddrWindow()

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

void lcd_print_power(struct power_output outputs[POWER_METER_COUNT])
{
  tft.fillScreen(HX8357_BLACK);
  tft.setCursor(0,0);tft.setTextColor(HX8357_WHITE); tft.setTextSize(2);
  tft.print("3.3V");tft.println(":");
  tft.setTextSize(3);
  tft.println("V: "); //tft.println(outputs[0].voltage); 
  tft.println("A: "); //tft.println(outputs[0].current); 
  tft.println("P: "); //tft.println(outputs[0].power); 

  tft.setTextSize(2);
  tft.setCursor(Xoffset,0);
  tft.print("5V");tft.println(":");
  tft.setTextSize(3);
  tft.setCursor(Xoffset,Yoff);
  tft.println("V: "); //tft.println(outputs[1].voltage);
  tft.setCursor(Xoffset,Yoff+Yoffset);
  tft.println("A: "); //tft.println(outputs[1].current);
  tft.setCursor(Xoffset,Yoff+(Yoffset*2));
  tft.println("P: "); //tft.println(outputs[1].power);

  tft.setTextSize(2);
  tft.setCursor(Xoffset*2,0);
  tft.print("12V");tft.println(":");
  tft.setTextSize(3);
  tft.setCursor(Xoffset*2,Yoff);
  tft.println("V: "); //tft.println(outputs[2].voltage); 
  tft.setCursor(Xoffset*2,Yoff+Yoffset);
  tft.println("A: "); //tft.println(outputs[2].current); 
  tft.setCursor(Xoffset*2,Yoff+(Yoffset*2));
  tft.println("P: "); //tft.println(outputs[2].power); 


  int x0 = 20;
  int y0 = 300;
  int x1 = 460;

  int tickMark;
  int tickMarkCnt = 8;
  int spacing = (x1-x0)/tickMarkCnt;

 tft.drawLine( x0, y0, x1, y0, HX8357_WHITE);
 for (tickMark=0; tickMark<=tickMarkCnt; tickMark++)
 {
    tft.drawLine( x0+(spacing*tickMark), 300, x0+(spacing*tickMark), 310, HX8357_WHITE);
 }

  int y1 = 160;

  int tickMarkCnt2 = 4;
  int spacing2 = (y1-y0)/tickMarkCnt2;

 tft.drawLine( x0, y0, x0, y1, HX8357_WHITE);
 for (tickMark=0; tickMark<=tickMarkCnt2; tickMark++)
 {
    tft.drawLine( 10, y0+(spacing2*tickMark), x0, y0+(spacing2*tickMark), HX8357_WHITE);
 }
}

// seeing if adjusting printing so only that values
void lcd_print_power_faster(struct power_output outputs[POWER_METER_COUNT])
{
//  tft.fillRect(Xoff, Yoff, Xoffset-Xoff, Yoffset*3, HX8357_BLACK); //Fills the values to make a clean slate for the nxt values.
//  tft.fillRect(Xoff+Xoffset, Yoff, Xoffset-Xoff, Yoffset*3, HX8357_BLACK);
//  tft.fillRect(Xoff+(Xoffset*2), Yoff, Xoffset-Xoff, Yoffset*3, HX8357_BLACK);

// show() , display() or update()
  
	// 3.3V Bus
  tft.setCursor(Xoff,Yoff);tft.setTextColor(HX8357_CYAN, HX8357_BLACK); tft.setTextSize(3);
	tft.println(outputs[0].voltage/1000, 2);
	tft.setCursor(Xoff,Yoff+Yoffset);
	tft.println(outputs[0].current, 0);
	tft.setCursor(Xoff,Yoff+(Yoffset*2));
	tft.println(outputs[0].power/1000, 3);

  // 5V Bus
  tft.setCursor(Xoff+Xoffset,Yoff);tft.setTextColor(HX8357_CYAN, HX8357_BLACK); tft.setTextSize(3);
  tft.println(outputs[1].voltage/1000, 2);
  tft.setCursor(Xoff+Xoffset,Yoff+Yoffset);
  tft.println(outputs[1].current, 0);
  tft.setCursor(Xoff+Xoffset,Yoff+(Yoffset*2));
  tft.println(outputs[1].power);
  
  // 12V Bus
  tft.setCursor(Xoff+(Xoffset*2),Yoff);tft.setTextColor(HX8357_CYAN, HX8357_BLACK); tft.setTextSize(3);
  tft.println(outputs[2].voltage/1000, 2);
  tft.setCursor(Xoff+(Xoffset*2),Yoff+Yoffset);
  tft.println(outputs[2].current, 0);
  tft.setCursor(Xoff+(Xoffset*2),Yoff+(Yoffset*2));
  tft.println(outputs[2].power);
}

void lcd_button_print()
{
	// Serial Button Updates
	if (serial_button.curr_state != serial_button.last_state)
	{
		serial_button.last_state = serial_button.curr_state;
		tft.fillRect(serial_button.x,serial_button.y,serial_button.w,serial_button.h, serial_button.curr_state == 1 ? HX8357_RED : HX8357_GREEN);
		tft.setCursor(serial_button.x,serial_button.y+serial_button.h/2);
		tft.setTextColor(HX8357_BLACK); tft.setTextSize(3);
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
//			setup_sdcard();
      SDCardInit();
//			write_str_char(header);
		}
		tft.fillRect(sd_button.x,sd_button.y,sd_button.w,sd_button.h, sd_button.curr_state == 1 ? HX8357_RED : HX8357_GREEN);
		tft.setCursor(sd_button.x,sd_button.y+sd_button.h/2);
		tft.setTextColor(HX8357_BLACK); tft.setTextSize(3);
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
