//SD Card Writer Functions

#include <SD.h>
#include <SPI.h>

#define SD_CS 7       //The Chip Select for the SD card is attached to digital pin 7 on the Arduino Every
#define SD_PIN 7      //same thing but for legacy definition

#define LOGFILE "data.csv"
File log_file;
boolean groundMode = 0;
// Variable for file name
char logFileName[16];
//Character strings for writing data to memory, took out pm1,pm2,pm4,pm10,VOCI //
//String header = "Time,hp,X,Y,Z,IT,Volt3V,latitude,longitude,Alt";
String dataString = ""; //holds the entire data string for each read cycles

const char header = "Time (ms), Monitor Ind, Voltage (mV), Current (mA), Power (mW)";

void setup_sdcard()
{
	if (!SD.begin(SD_PIN))
	{
		while(1); // wait for sd card to initialize
	}
	log_file = SD.open(LOGFILE, FILE_WRITE);
}

void setup_sdcard_read()
{
	if (!SD.begin(SD_PIN))
	{
		while(1); // wait for SD card initialization
	}
	log_file = SD.open(LOGFILE);
}

void read_line()
{
	if (log_file)
	{
		while(log_file.available())
		{
			Serial.write(log_file.read());
		}
		Serial.println("finished writing");
	}
}

void close_file()
{
	if (log_file)
		log_file.close();
}

// important to note that the sd card is closed each time for each read. So this could slow down the data rate slightly
void write_int(uint16_t data)
{
	if (log_file)
	{
		log_file.print(data);	
		log_file.close();
	}
}

// used for printing out headers, make sure a c string is passed
void write_str_char(const char * data)
{
	if (log_file)
	{
		log_file.println(data);
	}
}

void flush_sd()
{
	if (log_file)
	{
		log_file.flush();
	}
}

void write_power_data(struct power_output * power_log)
{
	if (log_file)
	{
		uint8_t i;
		for (i=0; i<POWER_METER_COUNT; i++)
		{
			log_file.print(power_log[i].time_ms);
			log_file.print(",");
			log_file.print(i);
			log_file.print(",");
			log_file.print(power_log[i].voltage);
			log_file.print(",");
			log_file.print(power_log[i].current);
			log_file.print(",");
			log_file.println(power_log[i].power);
		}
	}
	flush_sd();
}
//#############################################################################################################
//--->FUNCTION is called when the system starts or after a power reset to enable recording data to the SD card.
boolean SDCardInit() 
{
  Serial.println();  
  Serial.println("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CS))
  {
    Serial.println("Card failed, or not present");
    delay(100);
    // Can't do anything more.  Try another time....
    return 1;
  }
  
  // The Card is present so find an unused file name
  // Serial.println("searching for an unused file name... ");
  // Start with LOG0.CSV and count up until an unused file name is found.
  for (int i = 0; i < 10000; i++)
  {
    sprintf(logFileName, "LOG%d.CSV", i);
    if (!SD.exists(logFileName))
    {
      break;
    }
  }

  Serial.print("The Log filename is:  ");
  Serial.println(logFileName);

  File dataFile = SD.open(logFileName, FILE_WRITE);

// Write the header including sensor names to the newly opened file
  dataString = header;

  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    Serial.println("SD Card initialized and header written.");
    return (1);                           // Able to write to SD card
  }
  else
  {
    Serial.println("SD card present but unable to write to file");
    sprintf(logFileName, "LOG0.CSV");     // Clear out the file name to force the program to find a unused file name
    return (0);                           // Unable to write to SD card
  }
  dataString = "";
}

boolean writeDataToSD() 
{
  File dataFile = SD.open(logFileName, FILE_WRITE);

// if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println(dataString);
    dataFile.close();
    return (1);                                             // Return a 1 since the write was successful LED Blink
  }

// if the file isn't open, notify that there was an error and re-initialize  the card
  else 
  {
    if (groundMode) Serial.println();
    if (groundMode) Serial.print("error writing to file: ");
    if (groundMode) Serial.println(logFileName);
    dataFile.close();
    delay(100);
    if (groundMode) Serial.println("Re - Initializing SD card...");
    return (SDCardInit());          //Return a 0 if successful to disable LED Blink
  }
}
