//SD Card Writer Functions

#include <SD.h>
#include <SPI.h>

#define SD_CS 6       //The Chip Select for the SD card is attached to digital pin 7 on the Arduino Every
//#define SD_PIN 6      //same thing but for legacy definition

#define LOGFILE "data.csv"
File log_file;
boolean groundMode = 0;
// Variable for file name
char logFileName[16];
/*
//Character strings for writing data to memory, took out pm1,pm2,pm4,pm10,VOCI //
//String header = "Time,hp,X,Y,Z,IT,Volt3V,latitude,longitude,Alt";
String dataString = ""; //holds the entire data string for each read cycles
const char header = 'Time (ms), Volt (V), Curr (mA), Pow (W)';
*/

//Character strings for writing data to memory, took out pm1,pm2,pm4,pm10,VOCI //
String header1 = "Time (ms), 3V3 Volt (V), 3V3 Curr (mA), 3V3 Pow (W)";
String header2 = ", 5V Volt (V), 5V Curr (mA), 5V Pow (W)";
String header3 = ", 12V Volt (V), 12V Curr (mA), 12V Pow (W)";

String dataString = ""; //holds the entire data string for each read cycles

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

/*
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
*/

boolean SDCardInit() //--->FUNCTION is called when the system starts or after a power reset to enable recording data to the SD card.
{
  pinMode(6, OUTPUT);
  Serial.println("\n Initializing SD card...");

  if (!SD.begin(SD_CS))  //See if the card is present and can be initialized
  {
    Serial.println("Card failed, or not present");
    delay(100);
    return false;     // Can't do anything more.  Try another time....
  }
  
  // The Card is present so find an unused file name
  // Serial.println("searching for an unused file name... ");
  // Start with LOG0.CSV and count up until an unused file name is found.
  for (int i = 0; i < 1000; i++)
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

  if (dataFile)   //Verify that the file exists
  {
    dataFile.print(header1);   //Use a print function to write to the SD card. Write the header including sensor names to the newly opened file
    Serial.print(header1);
    dataFile.print(header2);
    Serial.print(header2);
    dataFile.println(header3);
    Serial.println(header3);
    dataFile.close();               //Close the file after writing the header to it.
    Serial.println("SD Card initialized and header written.");
    return (1); // Able to write to SD card
  }
  else
  {
    Serial.println("SD card present but unable to write to file");
    sprintf(logFileName, "LOG0.CSV");     // Clear out the file name to force the program to find a unused file name
    return (0);                           // Unable to write to SD card
  }
  dataString = "";
}
/*
boolean writeDataToSD() 
  {
    File dataFile = SD.open(logFileName, FILE_WRITE);
  
  // if the file is available, write to it:
    if (dataFile)
    {
//####
*/
void write_power_data(struct power_output * power_log)    //Follow header columns: "Time (ms), 3V3 Volt (V), 3V3 Curr (mA), 3V3 Pow (W), 5V Volt (V), 5V Curr (mA), 5V Pow (W), 12V Volt (V), 12V Curr (mA), 12V Pow (W)"
{
  File dataFile = SD.open(logFileName, FILE_WRITE);
  if (dataFile)
  {
    uint8_t i=0;
    dataFile.print(power_log[i].time_ms);
    Serial.print(power_log[i].time_ms);
    for (i=0; i<POWER_METER_COUNT; i++)
    {
      dataFile.print(", ");
      Serial.print(", ");
      dataFile.print(power_log[i].voltage/1000, 2);
      Serial.print(power_log[i].voltage/1000, 2);
      dataFile.print(", ");
      Serial.print(", ");
      dataFile.print(power_log[i].current, 0);
      Serial.print(power_log[i].current, 0);
      dataFile.print(", ");
      Serial.print(", ");
      dataFile.print(power_log[i].power/1000, 3);
      Serial.print(power_log[i].power/1000, 3);
    }
    dataFile.println(" ");
    Serial.println(" ");
    dataFile.close();
  }

//      dataFile.println(outputs[POWER_METER_COUNT]); //outputs[POWER_METER_COUNT]
//      dataFile.close();
//      return (1);                                             // Return a 1 since the write was successful LED Blink
  
  // if the file isn't open, notify that there was an error and re-initialize  the card
    else 
    {
      if (groundMode)
        {
          Serial.print("\n error writing to file: ");Serial.println(logFileName);
          dataFile.close();
          Serial.println("Re - Initializing SD card...");
          SDCardInit();
        }

//      return (SDCardInit());          //Return a 0 if successful to disable LED Blink
    }
      flush_sd();  
  }
