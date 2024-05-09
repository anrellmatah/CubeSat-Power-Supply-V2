EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr USLetter 11000 8500
encoding utf-8
Sheet 1 1
Title "DORA Power Supply V2 Schematic"
Date "2023-02-01"
Rev "V2.3"
Comp "ASU's Interplanetary Initiative Laboratory"
Comment1 "Created By: Danny Jacobs, Christopher McCormick, Anyell Mata"
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L Connector_Generic:Conn_01x02 Banana1
U 1 1 639A5B1A
P 7900 1450
F 0 "Banana1" H 7980 1442 50  0000 L CNN
F 1 "Conn_01x02" H 7980 1351 50  0000 L CNN
F 2 "" H 7900 1450 50  0001 C CNN
F 3 "~" H 7900 1450 50  0001 C CNN
	1    7900 1450
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 Banana2
U 1 1 639A709F
P 7900 2550
F 0 "Banana2" H 7980 2542 50  0000 L CNN
F 1 "Conn_01x02" H 7980 2451 50  0000 L CNN
F 2 "" H 7900 2550 50  0001 C CNN
F 3 "~" H 7900 2550 50  0001 C CNN
	1    7900 2550
	-1   0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 Banana3
U 1 1 639A72F4
P 7900 3650
F 0 "Banana3" H 7980 3642 50  0000 L CNN
F 1 "Conn_01x02" H 7980 3551 50  0000 L CNN
F 2 "" H 7900 3650 50  0001 C CNN
F 3 "~" H 7900 3650 50  0001 C CNN
	1    7900 3650
	-1   0    0    -1  
$EndComp
$Comp
L Device:Fuse 2A1
U 1 1 639ABEF5
P 1950 1650
F 0 "2A1" V 1753 1650 50  0000 C CNN
F 1 "Fuse" V 1844 1650 50  0000 C CNN
F 2 "" V 1880 1650 50  0001 C CNN
F 3 "~" H 1950 1650 50  0001 C CNN
	1    1950 1650
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_DIP_x02 Power-On1
U 1 1 639B2DDA
P 2600 1750
F 0 "Power-On1" H 2600 2117 50  0000 C CNN
F 1 "SW_DIP_x02" H 2600 2026 50  0000 C CNN
F 2 "" H 2600 1750 50  0001 C CNN
F 3 "~" H 2600 1750 50  0001 C CNN
	1    2600 1750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 1600 1800 1650
Wire Wire Line
	2100 1650 2300 1650
Wire Wire Line
	2300 1750 1600 1800
$Comp
L Adafruit:INA260_Power_Sensor_Breakout PM1
U 1 1 639CABFE
P 9350 1800
F 0 "PM1" H 9750 2775 50  0000 C CNN
F 1 "INA260_Power_Sensor_Breakout" H 9750 2684 50  0000 C CNN
F 2 "" H 9350 1200 50  0001 C CNN
F 3 "https://learn.adafruit.com/adafruit-ina260-current-voltage-power-sensor-breakout" H 8250 1250 50  0001 C CNN
	1    9350 1800
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR013
U 1 1 639CD1A2
P 3550 2400
F 0 "#PWR013" H 3550 2150 50  0001 C CNN
F 1 "GND" H 3555 2227 50  0000 C CNN
F 2 "" H 3550 2400 50  0001 C CNN
F 3 "" H 3550 2400 50  0001 C CNN
	1    3550 2400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR09
U 1 1 639CDB67
P 1600 2000
F 0 "#PWR09" H 1600 1750 50  0001 C CNN
F 1 "GND" H 1605 1827 50  0000 C CNN
F 2 "" H 1600 2000 50  0001 C CNN
F 3 "" H 1600 2000 50  0001 C CNN
	1    1600 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 639CE3A4
P 9450 1150
F 0 "#PWR07" H 9450 900 50  0001 C CNN
F 1 "GND" V 9455 977 50  0000 C CNN
F 2 "" H 9450 1150 50  0001 C CNN
F 3 "" H 9450 1150 50  0001 C CNN
	1    9450 1150
	0    -1   1    0   
$EndComp
$Comp
L MCU_Module:Adafruit_Feather_M0_Basic_Proto Feather1
U 1 1 639D3251
P 1900 5950
F 0 "Feather1" H 1900 7550 50  0000 C CNN
F 1 "Adafruit_Feather_M0_Basic_Proto" H 1900 7450 50  0000 C CNN
F 2 "Module:Adafruit_Feather" H 2000 4600 50  0001 L CNN
F 3 "https://cdn-learn.adafruit.com/downloads/pdf/adafruit-feather-m0-basic-proto.pdf" H 1900 4750 50  0001 C CNN
	1    1900 5950
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 639D757B
P 1900 7250
F 0 "#PWR023" H 1900 7000 50  0001 C CNN
F 1 "GND" H 1905 7077 50  0000 C CNN
F 2 "" H 1900 7250 50  0001 C CNN
F 3 "" H 1900 7250 50  0001 C CNN
	1    1900 7250
	-1   0    0    -1  
$EndComp
Wire Wire Line
	2450 6750 2400 6750
Wire Wire Line
	2450 6850 2400 6850
Wire Wire Line
	2900 1750 3000 1750
Wire Wire Line
	3000 1750 3000 2000
Wire Wire Line
	3000 2000 3050 2000
Wire Wire Line
	2900 1650 3000 1650
Wire Wire Line
	3000 1650 3000 1400
Wire Wire Line
	3000 1400 3050 1400
Text GLabel 2450 6750 2    50   Input ~ 0
SDA
Text GLabel 2450 6850 2    50   Input ~ 0
SCL
Wire Wire Line
	9600 1350 9450 1350
Wire Wire Line
	9600 1250 9450 1250
Text GLabel 9600 1350 2    50   Input ~ 0
SDA
Text GLabel 9600 1250 2    50   Input ~ 0
SCL
$Comp
L Sanken:C300H-ND PSU1
U 1 1 639C943D
P 3550 1700
F 0 "PSU1" H 3550 2475 50  0000 C CNN
F 1 "C300H-ND" H 3550 2384 50  0000 C CNN
F 2 "Converter_ACDC:Converter_ACDC_CUI_PBO-3-Sxx_THT_Vertical" H 3550 1750 50  0001 C CNN
F 3 "https://www.digikey.com/en/products/detail/sanken/C300H/4486044" H 3550 1700 50  0001 C CNN
	1    3550 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 1450 8450 1450
Wire Notes Line
	3700 2000 3700 2400
Wire Notes Line
	3700 2400 4450 2400
Wire Notes Line
	4400 2400 4400 2000
Wire Notes Line
	4400 2000 3700 2000
Text Notes 3950 2400 0    50   ~ 0
3.3V AUX
$Comp
L Adafruit:INA260_Power_Sensor_Breakout PM2
U 1 1 639F59C3
P 9350 2900
F 0 "PM2" H 9750 3875 50  0000 C CNN
F 1 "INA260_Power_Sensor_Breakout" H 9750 3784 50  0000 C CNN
F 2 "" H 9350 2300 50  0001 C CNN
F 3 "https://learn.adafruit.com/adafruit-ina260-current-voltage-power-sensor-breakout" H 8250 2350 50  0001 C CNN
	1    9350 2900
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR015
U 1 1 639F59C9
P 9450 2250
F 0 "#PWR015" H 9450 2000 50  0001 C CNN
F 1 "GND" V 9455 2077 50  0000 C CNN
F 2 "" H 9450 2250 50  0001 C CNN
F 3 "" H 9450 2250 50  0001 C CNN
	1    9450 2250
	0    -1   1    0   
$EndComp
Wire Wire Line
	9600 2450 9450 2450
Wire Wire Line
	9600 2350 9450 2350
Text GLabel 9600 2450 2    50   Input ~ 0
SDA
Text GLabel 9600 2350 2    50   Input ~ 0
SCL
$Comp
L Adafruit:INA260_Power_Sensor_Breakout PM3
U 1 1 639F77CD
P 9350 4000
F 0 "PM3" H 9750 4975 50  0000 C CNN
F 1 "INA260_Power_Sensor_Breakout" H 9750 4884 50  0000 C CNN
F 2 "" H 9350 3400 50  0001 C CNN
F 3 "https://learn.adafruit.com/adafruit-ina260-current-voltage-power-sensor-breakout" H 8250 3450 50  0001 C CNN
	1    9350 4000
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR020
U 1 1 639F77D3
P 9450 3350
F 0 "#PWR020" H 9450 3100 50  0001 C CNN
F 1 "GND" V 9455 3177 50  0000 C CNN
F 2 "" H 9450 3350 50  0001 C CNN
F 3 "" H 9450 3350 50  0001 C CNN
	1    9450 3350
	0    -1   1    0   
$EndComp
Wire Wire Line
	9600 3550 9450 3550
Wire Wire Line
	9600 3450 9450 3450
Text GLabel 9600 3550 2    50   Input ~ 0
SDA
Text GLabel 9600 3450 2    50   Input ~ 0
SCL
Wire Wire Line
	8100 2550 8450 2550
Wire Wire Line
	8450 3650 8100 3650
$Comp
L Switch:SW_DIP_x01 SW3
U 1 1 63A0968D
P 4950 2550
F 0 "SW3" V 4950 2680 50  0000 L CNN
F 1 "SW_DIP_x01" V 4995 2680 50  0001 L CNN
F 2 "" H 4950 2550 50  0001 C CNN
F 3 "~" H 4950 2550 50  0001 C CNN
	1    4950 2550
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_DIP_x01 SW2
U 1 1 63A176B4
P 5400 2550
F 0 "SW2" V 5400 2680 50  0000 L CNN
F 1 "SW_DIP_x01" V 5445 2680 50  0001 L CNN
F 2 "" H 5400 2550 50  0001 C CNN
F 3 "~" H 5400 2550 50  0001 C CNN
	1    5400 2550
	0    1    1    0   
$EndComp
$Comp
L Switch:SW_DIP_x01 SW1
U 1 1 63A182EF
P 5850 2550
F 0 "SW1" V 5850 2680 50  0000 L CNN
F 1 "SW_DIP_x01" V 5895 2680 50  0001 L CNN
F 2 "" H 5850 2550 50  0001 C CNN
F 3 "~" H 5850 2550 50  0001 C CNN
	1    5850 2550
	0    1    1    0   
$EndComp
Wire Wire Line
	5400 2850 5850 2850
Connection ~ 5400 2850
Wire Wire Line
	4050 1950 4950 1950
Wire Wire Line
	3650 2400 3650 2850
Connection ~ 4950 2850
Wire Wire Line
	4950 2850 5400 2850
Wire Wire Line
	4050 2250 4500 2250
Wire Wire Line
	4950 2250 4950 1950
Wire Wire Line
	5400 1650 5400 2250
Wire Wire Line
	5850 2250 5850 1350
Wire Wire Line
	4050 1350 5850 1350
Wire Wire Line
	4050 1650 5400 1650
$Comp
L power:+3.3VP #PWR018
U 1 1 63A57421
P 1800 4750
F 0 "#PWR018" H 1950 4700 50  0001 C CNN
F 1 "+3.3VP" H 1815 4923 50  0000 C CNN
F 2 "" H 1800 4750 50  0001 C CNN
F 3 "" H 1800 4750 50  0001 C CNN
	1    1800 4750
	-1   0    0    -1  
$EndComp
$Comp
L power:+3.3VP #PWR011
U 1 1 63A59209
P 4050 2050
F 0 "#PWR011" H 4200 2000 50  0001 C CNN
F 1 "+3.3VP" V 4100 2250 50  0000 C CNN
F 2 "" H 4050 2050 50  0001 C CNN
F 3 "" H 4050 2050 50  0001 C CNN
	1    4050 2050
	0    1    1    0   
$EndComp
$Comp
L power:+3.3VP #PWR019
U 1 1 63A638FF
P 9450 3250
F 0 "#PWR019" H 9600 3200 50  0001 C CNN
F 1 "+3.3VP" V 9450 3500 50  0000 C CNN
F 2 "" H 9450 3250 50  0001 C CNN
F 3 "" H 9450 3250 50  0001 C CNN
	1    9450 3250
	0    1    1    0   
$EndComp
$Comp
L power:+3.3VP #PWR014
U 1 1 63A66FC9
P 9450 2150
F 0 "#PWR014" H 9600 2100 50  0001 C CNN
F 1 "+3.3VP" V 9450 2400 50  0000 C CNN
F 2 "" H 9450 2150 50  0001 C CNN
F 3 "" H 9450 2150 50  0001 C CNN
	1    9450 2150
	0    1    1    0   
$EndComp
$Comp
L power:+3.3VP #PWR05
U 1 1 63A68E05
P 9450 1050
F 0 "#PWR05" H 9600 1000 50  0001 C CNN
F 1 "+3.3VP" V 9450 1300 50  0000 C CNN
F 2 "" H 9450 1050 50  0001 C CNN
F 3 "" H 9450 1050 50  0001 C CNN
	1    9450 1050
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR03
U 1 1 63A6A5DD
P 4050 1450
F 0 "#PWR03" H 4050 1300 50  0001 C CNN
F 1 "+5V" V 4050 1650 50  0000 C CNN
F 2 "" H 4050 1450 50  0001 C CNN
F 3 "" H 4050 1450 50  0001 C CNN
	1    4050 1450
	0    1    1    0   
$EndComp
$Comp
L power:+12V #PWR06
U 1 1 63A6B152
P 4050 1750
F 0 "#PWR06" H 4050 1600 50  0001 C CNN
F 1 "+12V" V 4050 2000 50  0000 C CNN
F 2 "" H 4050 1750 50  0001 C CNN
F 3 "" H 4050 1750 50  0001 C CNN
	1    4050 1750
	0    1    1    0   
$EndComp
$Comp
L power:-3V3 #PWR02
U 1 1 63A6B965
P 4050 1250
F 0 "#PWR02" H 4050 1350 50  0001 C CNN
F 1 "-3V3" V 4050 1500 50  0000 C CNN
F 2 "" H 4050 1250 50  0001 C CNN
F 3 "" H 4050 1250 50  0001 C CNN
	1    4050 1250
	0    1    1    0   
$EndComp
$Comp
L power:-5V #PWR04
U 1 1 63A6BEC6
P 4050 1550
F 0 "#PWR04" H 4050 1650 50  0001 C CNN
F 1 "-5V" V 4050 1750 50  0000 C CNN
F 2 "" H 4050 1550 50  0001 C CNN
F 3 "" H 4050 1550 50  0001 C CNN
	1    4050 1550
	0    1    1    0   
$EndComp
$Comp
L power:-12V #PWR08
U 1 1 63A6C572
P 4050 1850
F 0 "#PWR08" H 4050 1950 50  0001 C CNN
F 1 "-12V" V 4065 1978 50  0000 L CNN
F 2 "" H 4050 1850 50  0001 C CNN
F 3 "" H 4050 1850 50  0001 C CNN
	1    4050 1850
	0    1    1    0   
$EndComp
$Comp
L power:-3V3 #PWR012
U 1 1 63A77643
P 8100 1550
F 0 "#PWR012" H 8100 1650 50  0001 C CNN
F 1 "-3V3" V 8100 1800 50  0000 C CNN
F 2 "" H 8100 1550 50  0001 C CNN
F 3 "" H 8100 1550 50  0001 C CNN
	1    8100 1550
	0    1    1    0   
$EndComp
$Comp
L power:+5V #PWR016
U 1 1 63A7C6CF
P 8450 2450
F 0 "#PWR016" H 8450 2300 50  0001 C CNN
F 1 "+5V" V 8450 2650 50  0000 C CNN
F 2 "" H 8450 2450 50  0001 C CNN
F 3 "" H 8450 2450 50  0001 C CNN
	1    8450 2450
	0    -1   -1   0   
$EndComp
$Comp
L power:-5V #PWR017
U 1 1 63A7C6D5
P 8100 2650
F 0 "#PWR017" H 8100 2750 50  0001 C CNN
F 1 "-5V" V 8100 2850 50  0000 C CNN
F 2 "" H 8100 2650 50  0001 C CNN
F 3 "" H 8100 2650 50  0001 C CNN
	1    8100 2650
	0    1    1    0   
$EndComp
$Comp
L power:+12V #PWR021
U 1 1 63A7F6AF
P 8450 3550
F 0 "#PWR021" H 8450 3400 50  0001 C CNN
F 1 "+12V" V 8450 3800 50  0000 C CNN
F 2 "" H 8450 3550 50  0001 C CNN
F 3 "" H 8450 3550 50  0001 C CNN
	1    8450 3550
	0    -1   -1   0   
$EndComp
$Comp
L power:-12V #PWR022
U 1 1 63A7F6B5
P 8100 3750
F 0 "#PWR022" H 8100 3850 50  0001 C CNN
F 1 "-12V" V 8115 3878 50  0000 L CNN
F 2 "" H 8100 3750 50  0001 C CNN
F 3 "" H 8100 3750 50  0001 C CNN
	1    8100 3750
	0    1    1    0   
$EndComp
Text Notes 8000 4350 0    50   ~ 0
Power Monitors + Banana Plugs
$Comp
L power:+3V3 #PWR01
U 1 1 63A94F00
P 4050 1150
F 0 "#PWR01" H 4050 1000 50  0001 C CNN
F 1 "+3V3" V 4065 1278 50  0000 L CNN
F 2 "" H 4050 1150 50  0001 C CNN
F 3 "" H 4050 1150 50  0001 C CNN
	1    4050 1150
	0    1    1    0   
$EndComp
$Comp
L power:+3V3 #PWR010
U 1 1 63A97354
P 8450 1350
F 0 "#PWR010" H 8450 1200 50  0001 C CNN
F 1 "+3V3" V 8465 1478 50  0000 L CNN
F 2 "" H 8450 1350 50  0001 C CNN
F 3 "" H 8450 1350 50  0001 C CNN
	1    8450 1350
	0    -1   -1   0   
$EndComp
$Comp
L Adafruit:320x480_Color_TFT_Touchscreen_Breakout TFT1
U 1 1 63AAAC46
P 4200 5700
F 0 "TFT1" H 4150 6875 50  0000 C CNN
F 1 "320x480_Color_TFT_Touchscreen_Breakout" H 4150 6784 50  0000 C CNN
F 2 "" H 4200 4200 50  0001 C CNN
F 3 "https://learn.adafruit.com/adafruit-3-5-color-320x480-tft-touchscreen-breakout/pinouts" H 4500 5500 50  0001 C CNN
	1    4200 5700
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3VP #PWR025
U 1 1 63ACB729
P 3600 6550
F 0 "#PWR025" H 3750 6500 50  0001 C CNN
F 1 "+3.3VP" V 3650 6750 50  0000 C CNN
F 2 "" H 3600 6550 50  0001 C CNN
F 3 "" H 3600 6550 50  0001 C CNN
	1    3600 6550
	0    -1   -1   0   
$EndComp
$Comp
L power:GND #PWR026
U 1 1 63AD3BC3
P 3600 6650
F 0 "#PWR026" H 3600 6400 50  0001 C CNN
F 1 "GND" H 3605 6477 50  0000 C CNN
F 2 "" H 3600 6650 50  0001 C CNN
F 3 "" H 3600 6650 50  0001 C CNN
	1    3600 6650
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 63AD42F6
P 4700 6650
F 0 "#PWR027" H 4700 6400 50  0001 C CNN
F 1 "GND" H 4705 6477 50  0000 C CNN
F 2 "" H 4700 6650 50  0001 C CNN
F 3 "" H 4700 6650 50  0001 C CNN
	1    4700 6650
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 63AD55BF
P 3600 5250
F 0 "#PWR024" H 3600 5000 50  0001 C CNN
F 1 "GND" V 3605 5077 50  0000 C CNN
F 2 "" H 3600 5250 50  0001 C CNN
F 3 "" H 3600 5250 50  0001 C CNN
	1    3600 5250
	0    1    -1   0   
$EndComp
Text Notes 1350 3100 0    50   ~ 0
C300 PSU + Front Panel Switches
Text Notes 2300 4100 0    50   ~ 0
Feather + Touchscreen
Wire Notes Line
	6650 3400 450  3400
Wire Notes Line
	6650 4550 10500 4550
Wire Notes Line
	6650 450  6650 4550
Wire Wire Line
	2400 6250 3600 6250
Wire Wire Line
	2400 6150 3600 6150
Wire Wire Line
	2400 6050 3600 6350
$Comp
L Device:LED GREEN
U 1 1 63A27468
P 2550 5250
F 0 "GREEN" H 2550 5150 50  0000 C CNN
F 1 "LED" H 2550 5150 50  0001 C CNN
F 2 "" H 2550 5250 50  0001 C CNN
F 3 "~" H 2550 5250 50  0001 C CNN
	1    2550 5250
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R1
U 1 1 63A28FCD
P 2850 5250
F 0 "R1" V 2750 5200 50  0000 L CNN
F 1 "R_US" V 2750 5150 50  0001 L CNN
F 2 "" V 2890 5240 50  0001 C CNN
F 3 "~" H 2850 5250 50  0001 C CNN
	1    2850 5250
	0    1    1    0   
$EndComp
$Comp
L Device:LED D?
U 1 1 63A4A5A7
P 2550 5350
F 0 "D?" H 2550 5250 50  0001 C CNN
F 1 "LED" H 2550 5250 50  0001 C CNN
F 2 "" H 2550 5350 50  0001 C CNN
F 3 "~" H 2550 5350 50  0001 C CNN
	1    2550 5350
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R?
U 1 1 63A4A5AD
P 2850 5350
F 0 "R?" V 2750 5300 50  0001 L CNN
F 1 "R_US" V 2750 5250 50  0001 L CNN
F 2 "" V 2890 5340 50  0001 C CNN
F 3 "~" H 2850 5350 50  0001 C CNN
	1    2850 5350
	0    1    1    0   
$EndComp
$Comp
L Device:LED D?
U 1 1 63A4B5DB
P 2550 5450
F 0 "D?" H 2550 5350 50  0001 C CNN
F 1 "LED" H 2550 5350 50  0001 C CNN
F 2 "" H 2550 5450 50  0001 C CNN
F 3 "~" H 2550 5450 50  0001 C CNN
	1    2550 5450
	-1   0    0    1   
$EndComp
$Comp
L Device:R_US R?
U 1 1 63A4B5E1
P 2850 5450
F 0 "R?" V 2750 5400 50  0001 L CNN
F 1 "R_US" V 2750 5350 50  0001 L CNN
F 2 "" V 2890 5440 50  0001 C CNN
F 3 "~" H 2850 5450 50  0001 C CNN
	1    2850 5450
	0    1    1    0   
$EndComp
$Comp
L power:GND #PWR?
U 1 1 63A4B5E7
P 3100 5500
F 0 "#PWR?" H 3100 5250 50  0001 C CNN
F 1 "GND" H 3105 5327 50  0000 C CNN
F 2 "" H 3100 5500 50  0001 C CNN
F 3 "" H 3100 5500 50  0001 C CNN
	1    3100 5500
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3000 5250 3100 5250
Wire Wire Line
	3100 5250 3100 5350
Wire Wire Line
	3000 5450 3100 5450
Connection ~ 3100 5450
Wire Wire Line
	3100 5450 3100 5500
Wire Wire Line
	3000 5350 3100 5350
Connection ~ 3100 5350
Wire Wire Line
	3100 5350 3100 5450
NoConn ~ 3200 6250
NoConn ~ 2800 6150
Text Label 9700 1450 2    50   ~ 0
Alert1
Wire Wire Line
	9700 1450 9450 1450
Text Label 9700 2550 2    50   ~ 0
Alert2
Wire Wire Line
	9700 2550 9450 2550
Text Label 9700 3650 2    50   ~ 0
Alert3
Wire Wire Line
	9700 3650 9450 3650
Text Label 2650 5550 2    50   ~ 0
Alert1
Wire Wire Line
	2650 5550 2400 5550
Text Label 2650 5650 2    50   ~ 0
Alert2
Wire Wire Line
	2650 5650 2400 5650
Text Label 2650 5750 2    50   ~ 0
Alert3
Wire Wire Line
	2650 5750 2400 5750
Wire Wire Line
	3650 2850 4500 2850
Wire Wire Line
	4500 2250 4500 2850
Connection ~ 4500 2850
Wire Wire Line
	4500 2850 4950 2850
$Comp
L Connector:Conn_WallPlug_Ground Plug1
U 1 1 63DB029D
P 1300 1700
F 0 "Plug1" H 1367 2025 50  0000 C CNN
F 1 "Conn_WallPlug_Ground" H 1367 1934 50  0000 C CNN
F 2 "" H 1700 1700 50  0001 C CNN
F 3 "~" H 1700 1700 50  0001 C CNN
	1    1300 1700
	1    0    0    -1  
$EndComp
$EndSCHEMATC
