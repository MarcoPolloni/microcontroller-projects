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
LIBS:special
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
LIBS:custom
EELAYER 27 0
EELAYER END
$Descr User 6000 4000
encoding utf-8
Sheet 2 6
Title "Rectifier"
Date "2 feb 2015"
Rev "1"
Comp "Digital Cave"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
Text HLabel 850  2750 0    60   Input ~ 0
AC1
Text HLabel 850  2900 0    60   Input ~ 0
AC2
Text HLabel 5200 1300 2    60   Output ~ 0
V+
Text HLabel 5200 1450 2    60   Output ~ 0
V-
$Comp
L TRANSFORMER T1
U 1 1 54C7EE64
P 1550 1350
F 0 "T1" H 1550 1650 60  0000 C CNN
F 1 "TRANSFORMER" H 1550 1050 60  0000 C CNN
F 2 "" H 1550 1350 60  0000 C CNN
F 3 "" H 1550 1350 60  0000 C CNN
	1    1550 1350
	1    0    0    -1  
$EndComp
$Comp
L TRANSFORMER T2
U 1 1 54C7EE71
P 1550 2050
F 0 "T2" H 1550 2350 60  0000 C CNN
F 1 "TRANSFORMER" H 1550 1750 60  0000 C CNN
F 2 "" H 1550 2050 60  0000 C CNN
F 3 "" H 1550 2050 60  0000 C CNN
	1    1550 2050
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 1550 1150 1850
Wire Wire Line
	1950 1550 1950 1850
Wire Wire Line
	1950 1700 2100 1700
Connection ~ 1950 1700
$Comp
L GND #PWR3
U 1 1 54C7EE84
P 2100 1700
F 0 "#PWR3" H 2100 1700 30  0001 C CNN
F 1 "GND" H 2100 1630 30  0001 C CNN
F 2 "" H 2100 1700 60  0000 C CNN
F 3 "" H 2100 1700 60  0000 C CNN
	1    2100 1700
	0    -1   -1   0   
$EndComp
$Comp
L BRIDGE D1
U 1 1 54C7EE97
P 3100 1700
F 0 "D1" H 3100 1750 70  0000 C CNN
F 1 "BRIDGE" H 3100 1650 70  0000 C CNN
F 2 "~" H 3100 1700 60  0000 C CNN
F 3 "~" H 3100 1700 60  0000 C CNN
	1    3100 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3950 1300 3950 1700
Wire Wire Line
	2250 1700 2250 2350
Wire Wire Line
	1950 2250 2050 2250
Wire Wire Line
	2050 2250 2050 2550
Wire Wire Line
	1950 1150 2050 1150
Wire Wire Line
	2050 1150 2050 850 
Wire Wire Line
	3950 1300 5200 1300
Wire Wire Line
	4050 1450 4050 2350
Wire Wire Line
	4050 1450 5200 1450
$Comp
L CP1 C1
U 1 1 54C98489
P 4250 1850
F 0 "C1" H 4300 1950 50  0000 L CNN
F 1 "6800uF" V 4350 1500 50  0000 L CNN
F 2 "~" H 4250 1850 60  0000 C CNN
F 3 "~" H 4250 1850 60  0000 C CNN
	1    4250 1850
	1    0    0    -1  
$EndComp
$Comp
L CP1 C2
U 1 1 54C984D1
P 4500 1850
F 0 "C2" H 4550 1950 50  0000 L CNN
F 1 "6800uF" V 4600 1500 50  0000 L CNN
F 2 "~" H 4500 1850 60  0000 C CNN
F 3 "~" H 4500 1850 60  0000 C CNN
	1    4500 1850
	1    0    0    -1  
$EndComp
$Comp
L CP1 C3
U 1 1 54C984D7
P 4750 1850
F 0 "C3" H 4800 1950 50  0000 L CNN
F 1 "6800uF" V 4850 1500 50  0000 L CNN
F 2 "~" H 4750 1850 60  0000 C CNN
F 3 "~" H 4750 1850 60  0000 C CNN
	1    4750 1850
	1    0    0    1   
$EndComp
$Comp
L CP1 C4
U 1 1 54C984DD
P 5000 1850
F 0 "C4" H 5050 1950 50  0000 L CNN
F 1 "6800uF" V 5100 1500 50  0000 L CNN
F 2 "~" H 5000 1850 60  0000 C CNN
F 3 "~" H 5000 1850 60  0000 C CNN
	1    5000 1850
	1    0    0    1   
$EndComp
Wire Wire Line
	4250 1650 4250 1300
Connection ~ 4250 1300
Wire Wire Line
	4500 1650 4500 1300
Connection ~ 4500 1300
Wire Wire Line
	4250 2050 4250 2350
Wire Wire Line
	4250 2350 5000 2350
Wire Wire Line
	5000 2050 5000 2450
Wire Wire Line
	4750 2050 4750 2350
Connection ~ 4750 2350
Wire Wire Line
	4500 2050 4500 2350
Connection ~ 4500 2350
Wire Wire Line
	4050 2350 2250 2350
$Comp
L GND #PWR4
U 1 1 54C9856B
P 5000 2450
F 0 "#PWR4" H 5000 2450 30  0001 C CNN
F 1 "GND" H 5000 2380 30  0001 C CNN
F 2 "" H 5000 2450 60  0000 C CNN
F 3 "" H 5000 2450 60  0000 C CNN
	1    5000 2450
	1    0    0    -1  
$EndComp
Connection ~ 5000 2350
Wire Wire Line
	4750 1650 4750 1450
Connection ~ 4750 1450
Wire Wire Line
	5000 1650 5000 1450
Connection ~ 5000 1450
Wire Wire Line
	2050 2550 3100 2550
Wire Wire Line
	2050 850  3100 850 
$Comp
L FUSE F1
U 1 1 54CED3F4
P 950 1500
F 0 "F1" H 1050 1550 40  0000 C CNN
F 1 "FUSE" H 850 1450 40  0000 C CNN
F 2 "~" H 950 1500 60  0000 C CNN
F 3 "~" H 950 1500 60  0000 C CNN
	1    950  1500
	0    -1   -1   0   
$EndComp
Wire Wire Line
	950  1250 950  1150
Wire Wire Line
	950  1150 1150 1150
Wire Wire Line
	950  1750 950  2750
Wire Wire Line
	950  2750 850  2750
Wire Wire Line
	850  2900 1050 2900
Wire Wire Line
	1050 2900 1050 2250
Wire Wire Line
	1050 2250 1150 2250
$EndSCHEMATC
