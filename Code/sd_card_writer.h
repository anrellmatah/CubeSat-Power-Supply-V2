//SD Card Writer Functions

#include <SD.h>
#include <SPI.h>

File log_file;

const char header[] = "time (ms), Monitor Ind, Voltage (mV), Current (mA), Power (mW)";

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

