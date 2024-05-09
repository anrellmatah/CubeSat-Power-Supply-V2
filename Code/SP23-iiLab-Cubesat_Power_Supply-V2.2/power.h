//https://forums.adafruit.com/viewtopic.php?f=19&p=873330
//https://github.com/adafruit/Adafruit_INA260/issues/9

#include <Adafruit_INA260.h>

#define POWER_METER_COUNT 4

Adafruit_INA260 power_meter_arr[POWER_METER_COUNT];
uint8_t power_meter_addr[] = {0x40,0x41,0x44,0x45};   //https://learn.adafruit.com/adafruit-ina260-current-voltage-power-sensor-breakout/pinouts

//struct for cleaning up output variables
struct power_output {
	float voltage;     //was a 'double' data type but didn't necessarily need less than a milli-unit.
	float current;
	float power;
	long time_ms;
};

//Function declarations
void meters_init();
void get_data_sensors(struct power_output outputs[POWER_METER_COUNT]);
void print_power(struct power_output outputs[POWER_METER_COUNT]);


// performs intialization and loops until it can be performed.
void meters_init()
{
	// TODO add the implementation for multiple power meters

	uint8_t i;
	for (i=0;i<POWER_METER_COUNT;i++)
	{
		power_meter_arr[i] = Adafruit_INA260();
		if (!power_meter_arr[i].begin(power_meter_addr[i]))
		{
			Serial.print("Could not find power meter: ");Serial.println(i);
//			while(1);
		}
   else
   {

  power_meter_arr[i].setMode(INA260_MODE_CONTINUOUS); // continuously update the current
  power_meter_arr[i].setAlertType(INA260_ALERT_OVERVOLTAGE); // set alert mode to overcurrent
  power_meter_arr[i].setAlertLimit(1000); // set alert current threshold to 700mA
  power_meter_arr[i].setAlertPolarity(INA260_ALERT_POLARITY_NORMAL); // set alert polarity to active high
  power_meter_arr[i].setAlertLatch(INA260_ALERT_LATCH_ENABLED); // latch the alert pin until mask/enable register is read

//  power_meter_arr[i].setAveragingCount(INA260_COUNT_16);
//  power_meter_arr[i].setVoltageConversionTime(INA260_TIME_1_1_ms);
//      power_meter_arr[i].setMode(INA260_MODE_CONTINUOUS);
//    power_meter_arr[i].setAlertType(INA260_ALERT_OVERVOLTAGE);    // set alert mode to overcurrent
//    power_meter_arr[i].setAlertLimit(5000);                         // set alert current threshold to 10mA

//    power_meter_arr[i].setAlertPolarity(INA260_ALERT_POLARITY_NORMAL);
//    power_meter_arr[i].setAlertLatch(INA260_ALERT_LATCH_TRANSPARENT);

//Ina260_5V.setAlertPolarity(INA260_ALERT_POLARITY_NORMAL); // set alert polarity to active high
//Ina260_5V.setAlertLatch(INA260_ALERT_LATCH_ENABLED); // latch the alert pin until mask/enable register is read
      Serial.print("Found power meter: ");Serial.println(i);
   }
	}
}

// loops through each sensor and reads the voltage,current,power registers
void get_data_sensors(struct power_output outputs[POWER_METER_COUNT])
{
	int i;
	for (i=0; i< POWER_METER_COUNT; i++)
	{
		outputs[i].voltage = power_meter_arr[i].readBusVoltage();
		outputs[i].current = power_meter_arr[i].readCurrent();
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
