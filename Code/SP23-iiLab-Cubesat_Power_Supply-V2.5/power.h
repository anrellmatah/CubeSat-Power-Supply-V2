//https://forums.adafruit.com/viewtopic.php?f=19&p=873330
//https://github.com/adafruit/Adafruit_INA260/issues/9

#include <Adafruit_INA260.h>

#define POWER_METER_COUNT 3

Adafruit_INA260 power_meter_arr[POWER_METER_COUNT];
uint8_t power_meter_addr[] = {0x40,0x41,0x44,0x45};   //https://learn.adafruit.com/adafruit-ina260-current-voltage-power-sensor-breakout/pinouts

//  int dataLine[4] = {0,1,2,3};  //An array of 3 integers for each power monitor's data string

//struct for cleaning up output variables
struct power_output 
  {
	double voltage;     //was a 'double' data type but didn't necessarily need less than a milli-unit.
	double current;
	double power;
	long time_ms;
  };

  double zero = 0;

//Function declarations
void meters_init();
void get_data_sensors(struct power_output outputs[POWER_METER_COUNT]);
void print_power(struct power_output outputs[POWER_METER_COUNT]);

void meters_init()    // performs intialization and loops until it can be performed.
{
  int i;
	for (i=0;i<POWER_METER_COUNT;i++)
	{
		power_meter_arr[i] = Adafruit_INA260();
		if (!power_meter_arr[i].begin(power_meter_addr[i]))
    {
			Serial.print("Could not find power meter: "); Serial.print(i);Serial.print(", Address: "); Serial.println(power_meter_addr[i], HEX);
		}
    else
    {
/*      power_meter_arr[i].setMode(INA260_MODE_CONTINUOUS);                 // continuously update the current.
      power_meter_arr[i].setAlertType(INA260_ALERT_OVERVOLTAGE);          // set alert mode to overcurrent.
      power_meter_arr[i].setAlertLimit(4000);                             // set alert current threshold to 700mA.
      power_meter_arr[i].setAlertPolarity(INA260_ALERT_POLARITY_NORMAL);  // set alert polarity to active high.
      power_meter_arr[i].setAlertLatch(INA260_ALERT_LATCH_ENABLED);       // latch the alert pin until mask/enable register is read.
*/
      Serial.print("Found power meter: "); Serial.print(i); Serial.print(", Address: "); Serial.println(power_meter_addr[i], HEX);
    }
	}
}

// loops through each sensor and reads the voltage,current,power registers
void get_data_sensors(struct power_output outputs[POWER_METER_COUNT])
{
	int i;
	for (i=0; i<POWER_METER_COUNT; i++)
	{
		outputs[i].voltage = power_meter_arr[i].readBusVoltage();
    if (power_meter_arr[i].readCurrent() >= 0)
    {
		outputs[i].current = power_meter_arr[i].readCurrent();
    }
    else
    {
    outputs[i].current = ((power_meter_arr[i].readCurrent())*0);
    }
		outputs[i].power = power_meter_arr[i].readPower();
		outputs[i].time_ms = millis();
    
	}
//	#ifdef PRINT_DIAG
//		print_power(outputs);	
//	#endif
}

//output in csv format all in milli units "sens index, voltage, current, power"
void print_power(struct power_output outputs[POWER_METER_COUNT])
{
	int i;
	Serial.println("");
	for (i=0; i < POWER_METER_COUNT; i++)
	{
		Serial.print(i);Serial.print(",");Serial.print(outputs[i].voltage);
		Serial.print(",");Serial.print(outputs[i].current);
		Serial.print(",");Serial.println(outputs[i].power);
	}
}
