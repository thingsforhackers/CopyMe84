EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:MiscellaneousDevices
LIBS:IHE
LIBS:switch_tactile_-_ts0b22
LIBS:copyMe84-cache
EELAYER 25 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATTINY84A-P IC1
U 1 1 5730C8CE
P 4000 3950
F 0 "IC1" H 3150 4700 50  0000 C CNN
F 1 "ATTINY84A-P" H 4700 3200 50  0000 C CNN
F 2 "Housings_DIP:DIP-14_W7.62mm_LongPads" H 4000 3750 50  0000 C CIN
F 3 "" H 4000 3950 50  0000 C CNN
	1    4000 3950
	1    0    0    -1  
$EndComp
$Comp
L LED D1
U 1 1 5730C94B
P 7000 3850
F 0 "D1" H 7000 3950 50  0000 C CNN
F 1 "RED" H 7000 3750 50  0000 C CNN
F 2 "LEDs:LED-10MM" H 7000 3850 50  0001 C CNN
F 3 "" H 7000 3850 50  0000 C CNN
	1    7000 3850
	0    -1   -1   0   
$EndComp
$Comp
L R R1
U 1 1 5730CB5A
P 7000 3400
F 0 "R1" V 7080 3400 50  0000 C CNN
F 1 "220" V 7000 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 6930 3400 50  0001 C CNN
F 3 "" H 7000 3400 50  0000 C CNN
	1    7000 3400
	1    0    0    -1  
$EndComp
$Comp
L R R2
U 1 1 5730CC8E
P 7350 3400
F 0 "R2" V 7430 3400 50  0000 C CNN
F 1 "220" V 7350 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 7280 3400 50  0001 C CNN
F 3 "" H 7350 3400 50  0000 C CNN
	1    7350 3400
	1    0    0    -1  
$EndComp
$Comp
L R R3
U 1 1 5730CCB0
P 7700 3400
F 0 "R3" V 7780 3400 50  0000 C CNN
F 1 "220" V 7700 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 7630 3400 50  0001 C CNN
F 3 "" H 7700 3400 50  0000 C CNN
	1    7700 3400
	1    0    0    -1  
$EndComp
$Comp
L R R4
U 1 1 5730CCD7
P 8050 3400
F 0 "R4" V 8130 3400 50  0000 C CNN
F 1 "220" V 8050 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 7980 3400 50  0001 C CNN
F 3 "" H 8050 3400 50  0000 C CNN
	1    8050 3400
	1    0    0    -1  
$EndComp
$Comp
L LED D2
U 1 1 5730CD43
P 7350 3850
F 0 "D2" H 7350 3950 50  0000 C CNN
F 1 "BLUE" H 7350 3750 50  0000 C CNN
F 2 "LEDs:LED-10MM" H 7350 3850 50  0001 C CNN
F 3 "" H 7350 3850 50  0000 C CNN
	1    7350 3850
	0    -1   -1   0   
$EndComp
$Comp
L LED D3
U 1 1 5730CD7A
P 7700 3850
F 0 "D3" H 7700 3950 50  0000 C CNN
F 1 "YELLOW" H 7700 3750 50  0000 C CNN
F 2 "LEDs:LED-10MM" H 7700 3850 50  0001 C CNN
F 3 "" H 7700 3850 50  0000 C CNN
	1    7700 3850
	0    -1   -1   0   
$EndComp
$Comp
L LED D4
U 1 1 5730CDAC
P 8050 3850
F 0 "D4" H 8050 3950 50  0000 C CNN
F 1 "GREEN" H 8050 3750 50  0000 C CNN
F 2 "LEDs:LED-10MM" H 8050 3850 50  0001 C CNN
F 3 "" H 8050 3850 50  0000 C CNN
	1    8050 3850
	0    -1   -1   0   
$EndComp
$Comp
L Earth #PWR01
U 1 1 5730CDDA
P 7000 4150
F 0 "#PWR01" H 7000 3900 50  0001 C CNN
F 1 "Earth" H 7000 4000 50  0001 C CNN
F 2 "" H 7000 4150 50  0000 C CNN
F 3 "" H 7000 4150 50  0000 C CNN
	1    7000 4150
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR02
U 1 1 5730CE07
P 7350 4150
F 0 "#PWR02" H 7350 3900 50  0001 C CNN
F 1 "Earth" H 7350 4000 50  0001 C CNN
F 2 "" H 7350 4150 50  0000 C CNN
F 3 "" H 7350 4150 50  0000 C CNN
	1    7350 4150
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR03
U 1 1 5730CE2D
P 7700 4150
F 0 "#PWR03" H 7700 3900 50  0001 C CNN
F 1 "Earth" H 7700 4000 50  0001 C CNN
F 2 "" H 7700 4150 50  0000 C CNN
F 3 "" H 7700 4150 50  0000 C CNN
	1    7700 4150
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR04
U 1 1 5730CE53
P 8050 4150
F 0 "#PWR04" H 8050 3900 50  0001 C CNN
F 1 "Earth" H 8050 4000 50  0001 C CNN
F 2 "" H 8050 4150 50  0000 C CNN
F 3 "" H 8050 4150 50  0000 C CNN
	1    8050 4150
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 3550 7000 3650
Wire Wire Line
	7350 3550 7350 3650
Wire Wire Line
	7700 3550 7700 3650
Wire Wire Line
	8050 3550 8050 3650
Wire Wire Line
	7000 3250 7000 2900
Wire Wire Line
	7350 3250 7350 2900
Wire Wire Line
	7700 3250 7700 2900
Wire Wire Line
	8050 3250 8050 2900
Text Label 7000 3200 1    60   ~ 0
ledRED
Text Label 7350 3200 1    60   ~ 0
ledBLUE
Text Label 7700 3250 1    60   ~ 0
ledYELLOW
Text Label 8050 3250 1    60   ~ 0
ledGREEN
Wire Wire Line
	5050 4250 6000 4250
Wire Wire Line
	5050 4350 6000 4350
Wire Wire Line
	5050 4550 6000 4550
Wire Wire Line
	5050 4450 6000 4450
Text Label 5300 4250 0    60   ~ 0
ledRED
Text Label 5300 4350 0    60   ~ 0
ledBLUE
Wire Wire Line
	5050 3850 6000 3850
Wire Wire Line
	5050 3750 6000 3750
Wire Wire Line
	5050 3650 6000 3650
Wire Wire Line
	5050 3550 6000 3550
Wire Wire Line
	5050 3950 6000 3950
Text Label 5300 3950 0    60   ~ 0
SPEAKER
$Comp
L R R5
U 1 1 5730F598
P 8750 3400
F 0 "R5" V 8830 3400 50  0000 C CNN
F 1 "220" V 8750 3400 50  0000 C CNN
F 2 "Resistors_ThroughHole:Resistor_Horizontal_RM10mm" V 8680 3400 50  0001 C CNN
F 3 "" H 8750 3400 50  0000 C CNN
	1    8750 3400
	1    0    0    -1  
$EndComp
$Comp
L SPEAKER SP1
U 1 1 5730F614
P 9050 3750
F 0 "SP1" H 8950 4000 50  0000 C CNN
F 1 "SPEAKER" H 8950 3500 50  0000 C CNN
F 2 "SpeakerMCABS-201-RC:Speaker - MCABS-201-RC" H 9050 3750 50  0001 C CNN
F 3 "" H 9050 3750 50  0000 C CNN
	1    9050 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 3250 8750 2900
$Comp
L Earth #PWR05
U 1 1 5731087E
P 8750 4100
F 0 "#PWR05" H 8750 3850 50  0001 C CNN
F 1 "Earth" H 8750 3950 50  0001 C CNN
F 2 "" H 8750 4100 50  0000 C CNN
F 3 "" H 8750 4100 50  0000 C CNN
	1    8750 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 3550 8750 3650
Wire Wire Line
	8750 3850 8750 4100
Text Label 8750 3250 1    60   ~ 0
SPEAKER
$Comp
L Battery BT1
U 1 1 573111F5
P 2150 3900
F 0 "BT1" H 2250 3950 50  0000 L CNN
F 1 "CR2477" H 2250 3850 50  0000 L CNN
F 2 "BatteryHolder2462:Battery Holder - Keystone 2462" V 2150 3940 50  0001 C CNN
F 3 "" V 2150 3940 50  0000 C CNN
	1    2150 3900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 3200 2150 3750
Wire Wire Line
	2150 3350 2950 3350
Wire Wire Line
	2150 4550 2950 4550
Wire Wire Line
	2150 4050 2150 4700
$Comp
L Earth #PWR06
U 1 1 57311594
P 2550 4750
F 0 "#PWR06" H 2550 4500 50  0001 C CNN
F 1 "Earth" H 2550 4600 50  0001 C CNN
F 2 "" H 2550 4750 50  0000 C CNN
F 3 "" H 2550 4750 50  0000 C CNN
	1    2550 4750
	1    0    0    -1  
$EndComp
Wire Wire Line
	2550 4550 2550 4750
Connection ~ 2550 4550
NoConn ~ 5050 4050
NoConn ~ 5050 3450
NoConn ~ 5050 3350
$Comp
L PWR_FLAG #FLG07
U 1 1 57311F7A
P 2150 3200
F 0 "#FLG07" H 2150 3295 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 3380 50  0000 C CNN
F 2 "" H 2150 3200 50  0000 C CNN
F 3 "" H 2150 3200 50  0000 C CNN
	1    2150 3200
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG08
U 1 1 57311FBC
P 2150 4700
F 0 "#FLG08" H 2150 4795 50  0001 C CNN
F 1 "PWR_FLAG" H 2150 4880 50  0000 C CNN
F 2 "" H 2150 4700 50  0000 C CNN
F 3 "" H 2150 4700 50  0000 C CNN
	1    2150 4700
	1    0    0    1   
$EndComp
Connection ~ 2150 3350
Connection ~ 2150 4550
Wire Wire Line
	7000 4050 7000 4150
Wire Wire Line
	7350 4050 7350 4150
Wire Wire Line
	7700 4050 7700 4150
Wire Wire Line
	8050 4050 8050 4150
$Comp
L Switch_Tactile_-_TS0B22 S1
U 1 1 5732B6FA
P 6900 5150
F 0 "S1" H 7050 5000 60  0000 C CNN
F 1 "RED" H 6900 5350 60  0000 C CNN
F 2 "Switch - TS0B22:Tactile Switch - TS0B22" H 6900 5150 60  0001 C CNN
F 3 "" H 6900 5150 60  0000 C CNN
	1    6900 5150
	0    -1   -1   0   
$EndComp
$Comp
L Switch_Tactile_-_TS0B22 S2
U 1 1 5732B826
P 7800 5200
F 0 "S2" H 7950 5050 60  0000 C CNN
F 1 "BLUE" H 7800 5400 60  0000 C CNN
F 2 "Switch - TS0B22:Tactile Switch - TS0B22" H 7800 5200 60  0001 C CNN
F 3 "" H 7800 5200 60  0000 C CNN
	1    7800 5200
	0    -1   -1   0   
$EndComp
$Comp
L Switch_Tactile_-_TS0B22 S3
U 1 1 5732B86A
P 8600 5150
F 0 "S3" H 8750 5000 60  0000 C CNN
F 1 "YELLOW" H 8600 5350 60  0000 C CNN
F 2 "Switch - TS0B22:Tactile Switch - TS0B22" H 8600 5150 60  0001 C CNN
F 3 "" H 8600 5150 60  0000 C CNN
	1    8600 5150
	0    -1   -1   0   
$EndComp
$Comp
L Switch_Tactile_-_TS0B22 S4
U 1 1 5732B8B7
P 9600 5150
F 0 "S4" H 9750 5000 60  0000 C CNN
F 1 "GREEN" H 9600 5350 60  0000 C CNN
F 2 "Switch - TS0B22:Tactile Switch - TS0B22" H 9600 5150 60  0001 C CNN
F 3 "" H 9600 5150 60  0000 C CNN
	1    9600 5150
	0    -1   -1   0   
$EndComp
$Comp
L Earth #PWR09
U 1 1 5732BC63
P 6950 5950
F 0 "#PWR09" H 6950 5700 50  0001 C CNN
F 1 "Earth" H 6950 5800 50  0001 C CNN
F 2 "" H 6950 5950 50  0000 C CNN
F 3 "" H 6950 5950 50  0000 C CNN
	1    6950 5950
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR010
U 1 1 5732BC9E
P 7850 5950
F 0 "#PWR010" H 7850 5700 50  0001 C CNN
F 1 "Earth" H 7850 5800 50  0001 C CNN
F 2 "" H 7850 5950 50  0000 C CNN
F 3 "" H 7850 5950 50  0000 C CNN
	1    7850 5950
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR011
U 1 1 5732BCD9
P 8650 5950
F 0 "#PWR011" H 8650 5700 50  0001 C CNN
F 1 "Earth" H 8650 5800 50  0001 C CNN
F 2 "" H 8650 5950 50  0000 C CNN
F 3 "" H 8650 5950 50  0000 C CNN
	1    8650 5950
	1    0    0    -1  
$EndComp
$Comp
L Earth #PWR012
U 1 1 5732BD14
P 9650 5950
F 0 "#PWR012" H 9650 5700 50  0001 C CNN
F 1 "Earth" H 9650 5800 50  0001 C CNN
F 2 "" H 9650 5950 50  0000 C CNN
F 3 "" H 9650 5950 50  0000 C CNN
	1    9650 5950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6950 5400 6950 5950
Wire Wire Line
	7850 5450 7850 5950
Wire Wire Line
	9650 5400 9650 5950
Wire Wire Line
	6950 4550 6950 4900
Wire Wire Line
	7850 4550 7850 4950
Wire Wire Line
	8650 4550 8650 4900
Wire Wire Line
	9650 4550 9650 4900
Text Label 6950 4850 1    60   ~ 0
swRED
Text Label 7850 4850 1    60   ~ 0
swBLUE
Text Label 8650 4850 1    60   ~ 0
swYELLOW
NoConn ~ 6850 4900
NoConn ~ 6850 5400
Text Label 9650 4850 1    60   ~ 0
swGREEN
NoConn ~ 7750 4950
NoConn ~ 7750 5450
NoConn ~ 8550 4900
NoConn ~ 8550 5400
NoConn ~ 9550 4900
NoConn ~ 9550 5400
Wire Wire Line
	8650 5400 8650 5950
Text Label 5300 4450 0    60   ~ 0
ledGREEN
Text Label 5300 4550 0    60   ~ 0
ledYELLOW
Text Label 5300 3550 0    60   ~ 0
swRED
Text Label 5300 3650 0    60   ~ 0
swBLUE
Text Label 5300 3750 0    60   ~ 0
swYELLOW
Text Label 5300 3850 0    60   ~ 0
swGREEN
$EndSCHEMATC
