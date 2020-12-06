EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 5
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
L Device:Battery_Cell BT1
U 1 1 5FBF91FD
P 5600 2000
F 0 "BT1" H 5750 2150 50  0000 L CNN
F 1 "Li-Po, 3,7V" H 5750 2000 50  0000 L CNN
F 2 "zmolnar-Battery:Battery_103450" V 5600 2060 50  0001 C CNN
F 3 "~" V 5600 2060 50  0001 C CNN
	1    5600 2000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR017
U 1 1 5FBF96ED
P 5600 2250
F 0 "#PWR017" H 5600 2000 50  0001 C CNN
F 1 "GND" H 5605 2077 50  0000 C CNN
F 2 "" H 5600 2250 50  0001 C CNN
F 3 "" H 5600 2250 50  0001 C CNN
	1    5600 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 2100 5600 2250
$Comp
L Battery_Management:MCP73831-2-OT U2
U 1 1 5FBFA5D5
P 3750 1650
F 0 "U2" H 3750 2131 50  0000 C CNN
F 1 "MCP73831-2-OT" H 3750 2040 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 3800 1400 50  0001 L CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20001984g.pdf" H 3600 1600 50  0001 C CNN
	1    3750 1650
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR011
U 1 1 5FBFAA19
P 3750 2250
F 0 "#PWR011" H 3750 2000 50  0001 C CNN
F 1 "GND" H 3755 2077 50  0000 C CNN
F 2 "" H 3750 2250 50  0001 C CNN
F 3 "" H 3750 2250 50  0001 C CNN
	1    3750 2250
	1    0    0    -1  
$EndComp
$Comp
L Device:C C4
U 1 1 5FBFADAE
P 5050 1900
F 0 "C4" H 5165 1946 50  0000 L CNN
F 1 "4.7u" H 5165 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5088 1750 50  0001 C CNN
F 3 "~" H 5050 1900 50  0001 C CNN
	1    5050 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	4150 1550 5050 1550
Wire Wire Line
	5050 1550 5050 1750
Connection ~ 5050 1550
$Comp
L power:GND #PWR015
U 1 1 5FBFC0A4
P 5050 2250
F 0 "#PWR015" H 5050 2000 50  0001 C CNN
F 1 "GND" H 5055 2077 50  0000 C CNN
F 2 "" H 5050 2250 50  0001 C CNN
F 3 "" H 5050 2250 50  0001 C CNN
	1    5050 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 2250 5050 2050
Wire Wire Line
	3750 2250 3750 1950
Wire Wire Line
	4300 1750 4150 1750
$Comp
L Device:R R4
U 1 1 5FBFC723
P 3050 2000
F 0 "R4" H 3120 2046 50  0000 L CNN
F 1 "2k" H 3120 1955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 2980 2000 50  0001 C CNN
F 3 "~" H 3050 2000 50  0001 C CNN
	1    3050 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3350 1750 3050 1750
Wire Wire Line
	3050 1750 3050 1850
$Comp
L power:GND #PWR09
U 1 1 5FBFCBF9
P 3050 2250
F 0 "#PWR09" H 3050 2000 50  0001 C CNN
F 1 "GND" H 3055 2077 50  0000 C CNN
F 2 "" H 3050 2250 50  0001 C CNN
F 3 "" H 3050 2250 50  0001 C CNN
	1    3050 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3050 2250 3050 2150
$Comp
L Device:C C2
U 1 1 5FBFEFB5
P 2600 1900
F 0 "C2" H 2715 1946 50  0000 L CNN
F 1 "4.7u" H 2715 1855 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 2638 1750 50  0001 C CNN
F 3 "~" H 2600 1900 50  0001 C CNN
	1    2600 1900
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 1750 2600 1350
Connection ~ 2600 1350
$Comp
L power:GND #PWR08
U 1 1 5FBFFD87
P 2600 2250
F 0 "#PWR08" H 2600 2000 50  0001 C CNN
F 1 "GND" H 2605 2077 50  0000 C CNN
F 2 "" H 2600 2250 50  0001 C CNN
F 3 "" H 2600 2250 50  0001 C CNN
	1    2600 2250
	1    0    0    -1  
$EndComp
Wire Wire Line
	2600 2050 2600 2250
$Comp
L Device:C C7
U 1 1 5FC02808
P 10750 1850
F 0 "C7" H 10865 1896 50  0000 L CNN
F 1 "10u" H 10865 1805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 10788 1700 50  0001 C CNN
F 3 "~" H 10750 1850 50  0001 C CNN
	1    10750 1850
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR027
U 1 1 5FC02BE9
P 8900 2300
F 0 "#PWR027" H 8900 2050 50  0001 C CNN
F 1 "GND" H 8905 2127 50  0000 C CNN
F 2 "" H 8900 2300 50  0001 C CNN
F 3 "" H 8900 2300 50  0001 C CNN
	1    8900 2300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR030
U 1 1 5FC02E07
P 10750 2300
F 0 "#PWR030" H 10750 2050 50  0001 C CNN
F 1 "GND" H 10755 2127 50  0000 C CNN
F 2 "" H 10750 2300 50  0001 C CNN
F 3 "" H 10750 2300 50  0001 C CNN
	1    10750 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	8900 1950 8900 2300
Wire Wire Line
	10750 2000 10750 2300
$Comp
L power:+3V3 #PWR029
U 1 1 5FC0476B
P 10750 1350
F 0 "#PWR029" H 10750 1200 50  0001 C CNN
F 1 "+3V3" H 10765 1523 50  0000 C CNN
F 2 "" H 10750 1350 50  0001 C CNN
F 3 "" H 10750 1350 50  0001 C CNN
	1    10750 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	5050 1550 5600 1550
Wire Wire Line
	5600 1550 5600 1800
$Comp
L Transistor_FET:DMG2301L Q2
U 1 1 5FC0DD3F
P 6900 1450
F 0 "Q2" V 7149 1450 50  0000 C CNN
F 1 "DMG2301L" V 7240 1450 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7100 1375 50  0001 L CIN
F 3 "https://www.diodes.com/assets/Datasheets/DMG2301L.pdf" H 6900 1450 50  0001 L CNN
F 4 "3127313" V 6900 1450 50  0001 C CNN "Farnell"
	1    6900 1450
	0    -1   1    0   
$EndComp
$Comp
L Device:C C6
U 1 1 5FC118C8
P 7350 1950
F 0 "C6" H 7465 1996 50  0000 L CNN
F 1 "4.7u" H 7465 1905 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 7388 1800 50  0001 C CNN
F 3 "~" H 7350 1950 50  0001 C CNN
	1    7350 1950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR023
U 1 1 5FC12C8E
P 7350 2300
F 0 "#PWR023" H 7350 2050 50  0001 C CNN
F 1 "GND" H 7355 2127 50  0000 C CNN
F 2 "" H 7350 2300 50  0001 C CNN
F 3 "" H 7350 2300 50  0001 C CNN
	1    7350 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	7350 2300 7350 2100
Wire Wire Line
	7100 1550 7350 1550
Wire Wire Line
	7350 1550 7350 1800
Wire Wire Line
	6900 1000 6350 1000
Wire Wire Line
	6900 1000 6900 1250
Wire Wire Line
	2600 1000 2600 1350
Connection ~ 6900 1000
Wire Wire Line
	7350 1550 7350 1400
Connection ~ 7350 1550
$Comp
L Connector:USB_B_Mini J1
U 1 1 5FC33C36
P 1000 1550
F 0 "J1" H 1057 2017 50  0000 C CNN
F 1 "USB_B_Mini" H 1057 1926 50  0000 C CNN
F 2 "Connector_USB:USB_Mini-B_Lumberg_2486_01_Horizontal" H 1150 1500 50  0001 C CNN
F 3 "~" H 1150 1500 50  0001 C CNN
	1    1000 1550
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR01
U 1 1 5FC38EE7
P 950 2150
F 0 "#PWR01" H 950 1900 50  0001 C CNN
F 1 "GND" H 955 1977 50  0000 C CNN
F 2 "" H 950 2150 50  0001 C CNN
F 3 "" H 950 2150 50  0001 C CNN
	1    950  2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	900  1950 900  2050
Wire Wire Line
	900  2050 950  2050
Wire Wire Line
	1000 2050 1000 1950
Wire Wire Line
	950  2150 950  2050
Connection ~ 950  2050
Wire Wire Line
	950  2050 1000 2050
Text GLabel 1650 1550 2    50   Input ~ 0
USB-DP
Text GLabel 1650 1650 2    50   Input ~ 0
USB-DM
Wire Wire Line
	1650 1550 1300 1550
Wire Wire Line
	1650 1650 1300 1650
Wire Wire Line
	3750 1350 2600 1350
Wire Wire Line
	6900 1000 7350 1000
Wire Wire Line
	7350 1000 7350 1100
$Comp
L Device:R R10
U 1 1 5FC6CFCC
P 6350 1950
F 0 "R10" H 6420 1996 50  0000 L CNN
F 1 "100k" H 6420 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6280 1950 50  0001 C CNN
F 3 "~" H 6350 1950 50  0001 C CNN
	1    6350 1950
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 1800 6350 1000
$Comp
L power:GND #PWR019
U 1 1 5FC6F94D
P 6350 2300
F 0 "#PWR019" H 6350 2050 50  0001 C CNN
F 1 "GND" H 6355 2127 50  0000 C CNN
F 2 "" H 6350 2300 50  0001 C CNN
F 3 "" H 6350 2300 50  0001 C CNN
	1    6350 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	6350 2300 6350 2100
Wire Wire Line
	1300 1350 1500 1350
Wire Wire Line
	8400 1650 8600 1650
$Comp
L Regulator_Switching:MCP1640CH U1
U 1 1 5FCA6772
P 2350 6550
F 0 "U1" H 2350 7017 50  0000 C CNN
F 1 "MCP1640CH" H 2350 6926 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-6" H 2400 6300 50  0001 L CIN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/20002234D.pdf" H 2100 7000 50  0001 C CNN
	1    2350 6550
	1    0    0    -1  
$EndComp
$Comp
L Device:L L1
U 1 1 5FCA724D
P 2350 5950
F 0 "L1" V 2540 5950 50  0000 C CNN
F 1 "4u7" V 2449 5950 50  0000 C CNN
F 2 "zmolnar-Device:Viking_Tech_SDIA0315" H 2350 5950 50  0001 C CNN
F 3 "~" H 2350 5950 50  0001 C CNN
F 4 "93-04-72" V 2350 5950 50  0001 C CNN "Lomex"
	1    2350 5950
	0    -1   -1   0   
$EndComp
$Comp
L Device:R R5
U 1 1 5FCA7A11
P 3150 6650
F 0 "R5" H 3220 6696 50  0000 L CNN
F 1 "680k" H 3220 6605 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3080 6650 50  0001 C CNN
F 3 "~" H 3150 6650 50  0001 C CNN
	1    3150 6650
	1    0    0    -1  
$EndComp
$Comp
L Device:R R6
U 1 1 5FCA7C73
P 3150 6950
F 0 "R6" H 3220 6996 50  0000 L CNN
F 1 "220k" H 3220 6905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 3080 6950 50  0001 C CNN
F 3 "~" H 3150 6950 50  0001 C CNN
	1    3150 6950
	1    0    0    -1  
$EndComp
$Comp
L Device:C C3
U 1 1 5FCA7EF7
P 3750 6800
F 0 "C3" H 3865 6846 50  0000 L CNN
F 1 "10u" H 3865 6755 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3788 6650 50  0001 C CNN
F 3 "~" H 3750 6800 50  0001 C CNN
	1    3750 6800
	1    0    0    -1  
$EndComp
$Comp
L Device:C C1
U 1 1 5FCA8064
P 1400 6750
F 0 "C1" H 1515 6796 50  0000 L CNN
F 1 "4u7" H 1515 6705 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 1438 6600 50  0001 C CNN
F 3 "~" H 1400 6750 50  0001 C CNN
	1    1400 6750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR07
U 1 1 5FCA8315
P 2350 7100
F 0 "#PWR07" H 2350 6850 50  0001 C CNN
F 1 "GND" H 2355 6927 50  0000 C CNN
F 2 "" H 2350 7100 50  0001 C CNN
F 3 "" H 2350 7100 50  0001 C CNN
	1    2350 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 6550 1750 6550
Wire Wire Line
	1750 6550 1750 6350
Wire Wire Line
	1750 6350 1950 6350
Wire Wire Line
	1750 6350 1750 5950
Wire Wire Line
	1750 5950 2200 5950
Connection ~ 1750 6350
Wire Wire Line
	2500 5950 2900 5950
Wire Wire Line
	2900 5950 2900 6350
Wire Wire Line
	2900 6350 2750 6350
Wire Wire Line
	2750 6450 3150 6450
Wire Wire Line
	3150 6500 3150 6450
Connection ~ 3150 6450
Wire Wire Line
	2750 6650 2750 6800
Wire Wire Line
	2750 6800 3150 6800
Connection ~ 3150 6800
Wire Wire Line
	2350 6850 2350 7100
Wire Wire Line
	3750 6450 3750 6650
Wire Wire Line
	3150 6450 3750 6450
Wire Wire Line
	1400 6600 1400 6350
Wire Wire Line
	1400 6350 1750 6350
$Comp
L power:GND #PWR010
U 1 1 5FCBBBDF
P 3150 7100
F 0 "#PWR010" H 3150 6850 50  0001 C CNN
F 1 "GND" H 3155 6927 50  0000 C CNN
F 2 "" H 3150 7100 50  0001 C CNN
F 3 "" H 3150 7100 50  0001 C CNN
	1    3150 7100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR012
U 1 1 5FCBBF91
P 3750 7100
F 0 "#PWR012" H 3750 6850 50  0001 C CNN
F 1 "GND" H 3755 6927 50  0000 C CNN
F 2 "" H 3750 7100 50  0001 C CNN
F 3 "" H 3750 7100 50  0001 C CNN
	1    3750 7100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR03
U 1 1 5FCBC1DE
P 1400 7100
F 0 "#PWR03" H 1400 6850 50  0001 C CNN
F 1 "GND" H 1405 6927 50  0000 C CNN
F 2 "" H 1400 7100 50  0001 C CNN
F 3 "" H 1400 7100 50  0001 C CNN
	1    1400 7100
	1    0    0    -1  
$EndComp
Wire Wire Line
	3750 7100 3750 6950
Wire Wire Line
	1400 7100 1400 6900
$Comp
L power:+5V #PWR013
U 1 1 5FCC7F01
P 4150 6250
F 0 "#PWR013" H 4150 6100 50  0001 C CNN
F 1 "+5V" H 4165 6423 50  0000 C CNN
F 2 "" H 4150 6250 50  0001 C CNN
F 3 "" H 4150 6250 50  0001 C CNN
	1    4150 6250
	1    0    0    -1  
$EndComp
$Comp
L power:+3V3 #PWR02
U 1 1 5FCC8B61
P 950 6350
F 0 "#PWR02" H 950 6200 50  0001 C CNN
F 1 "+3V3" H 965 6523 50  0000 C CNN
F 2 "" H 950 6350 50  0001 C CNN
F 3 "" H 950 6350 50  0001 C CNN
	1    950  6350
	1    0    0    -1  
$EndComp
Connection ~ 1400 6350
Text GLabel 4450 2050 2    50   Input ~ 0
BAT-STAT
Wire Wire Line
	950  6350 1400 6350
Text GLabel 1650 1200 2    50   Input ~ 0
USB-VCC
Wire Wire Line
	1650 1200 1500 1200
Wire Wire Line
	1500 1200 1500 1350
Connection ~ 1500 1350
Wire Wire Line
	1500 1350 2600 1350
$Comp
L Device:R R1
U 1 1 5FDD2FA9
P 1450 1950
F 0 "R1" H 1520 1996 50  0000 L CNN
F 1 "100k" H 1520 1905 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1380 1950 50  0001 C CNN
F 3 "~" H 1450 1950 50  0001 C CNN
	1    1450 1950
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR04
U 1 1 5FDD3316
P 1450 2150
F 0 "#PWR04" H 1450 1900 50  0001 C CNN
F 1 "GND" H 1455 1977 50  0000 C CNN
F 2 "" H 1450 2150 50  0001 C CNN
F 3 "" H 1450 2150 50  0001 C CNN
	1    1450 2150
	1    0    0    -1  
$EndComp
Wire Wire Line
	1450 2100 1450 2150
Wire Wire Line
	1450 1800 1450 1750
Wire Wire Line
	1450 1750 1300 1750
Wire Wire Line
	10750 1550 10750 1350
Wire Wire Line
	10750 1550 10750 1700
Connection ~ 10750 1550
$Comp
L Device:R R16
U 1 1 5FC0BFF3
P 10400 1700
F 0 "R16" H 10470 1746 50  0000 L CNN
F 1 "680" H 10470 1655 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10330 1700 50  0001 C CNN
F 3 "~" H 10400 1700 50  0001 C CNN
	1    10400 1700
	1    0    0    -1  
$EndComp
$Comp
L Device:R R17
U 1 1 5FC0C239
P 10400 2100
F 0 "R17" H 10470 2146 50  0000 L CNN
F 1 "150" H 10470 2055 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 10330 2100 50  0001 C CNN
F 3 "~" H 10400 2100 50  0001 C CNN
	1    10400 2100
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR028
U 1 1 5FC0C784
P 10400 2300
F 0 "#PWR028" H 10400 2050 50  0001 C CNN
F 1 "GND" H 10405 2127 50  0000 C CNN
F 2 "" H 10400 2300 50  0001 C CNN
F 3 "" H 10400 2300 50  0001 C CNN
	1    10400 2300
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 1550 10750 1550
Wire Wire Line
	10400 1850 10400 1900
Wire Wire Line
	10400 2250 10400 2300
Wire Wire Line
	9550 1900 9550 1650
Wire Wire Line
	9550 1650 9200 1650
Connection ~ 10400 1900
Wire Wire Line
	10400 1900 10400 1950
Wire Wire Line
	5600 1550 6700 1550
Connection ~ 5600 1550
Wire Wire Line
	6350 1000 2600 1000
Connection ~ 6350 1000
Connection ~ 10400 1550
Wire Wire Line
	9550 1900 10400 1900
$Comp
L power:+BATT #PWR016
U 1 1 5FC50CC7
P 5600 1400
F 0 "#PWR016" H 5600 1250 50  0001 C CNN
F 1 "+BATT" H 5615 1573 50  0000 C CNN
F 2 "" H 5600 1400 50  0001 C CNN
F 3 "" H 5600 1400 50  0001 C CNN
	1    5600 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 1400 5600 1550
$Comp
L Device:R R2
U 1 1 5FC54C9D
P 1950 4000
F 0 "R2" H 2020 4046 50  0000 L CNN
F 1 "220k" H 2020 3955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1880 4000 50  0001 C CNN
F 3 "~" H 1950 4000 50  0001 C CNN
	1    1950 4000
	1    0    0    -1  
$EndComp
$Comp
L Device:R R3
U 1 1 5FC54FF8
P 1950 4400
F 0 "R3" H 2020 4446 50  0000 L CNN
F 1 "680k" H 2020 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 1880 4400 50  0001 C CNN
F 3 "~" H 1950 4400 50  0001 C CNN
	1    1950 4400
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR06
U 1 1 5FC55B55
P 1950 4650
F 0 "#PWR06" H 1950 4400 50  0001 C CNN
F 1 "GND" H 1955 4477 50  0000 C CNN
F 2 "" H 1950 4650 50  0001 C CNN
F 3 "" H 1950 4650 50  0001 C CNN
	1    1950 4650
	1    0    0    -1  
$EndComp
$Comp
L power:+BATT #PWR05
U 1 1 5FC5607A
P 1950 3750
F 0 "#PWR05" H 1950 3600 50  0001 C CNN
F 1 "+BATT" H 1965 3923 50  0000 C CNN
F 2 "" H 1950 3750 50  0001 C CNN
F 3 "" H 1950 3750 50  0001 C CNN
	1    1950 3750
	1    0    0    -1  
$EndComp
Wire Wire Line
	1950 3750 1950 3850
Wire Wire Line
	1950 4150 1950 4200
Wire Wire Line
	1950 4550 1950 4650
Wire Wire Line
	1950 4200 2200 4200
Connection ~ 1950 4200
Wire Wire Line
	1950 4200 1950 4250
Text GLabel 2200 4200 2    50   Input ~ 0
BAT-ADC
Wire Wire Line
	7350 1550 8450 1550
Text GLabel 8400 1650 0    50   Input ~ 0
TLV62565-EN
$Comp
L Device:R R11
U 1 1 5FC4A5AB
P 6500 4450
F 0 "R11" H 6570 4496 50  0000 L CNN
F 1 "100k" H 6570 4405 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6430 4450 50  0001 C CNN
F 3 "~" H 6500 4450 50  0001 C CNN
	1    6500 4450
	1    0    0    -1  
$EndComp
$Comp
L Device:D D2
U 1 1 5FC4B3E3
P 7350 4950
F 0 "D2" H 7350 4734 50  0000 C CNN
F 1 "LL46" H 7350 4825 50  0000 C CNN
F 2 "Diode_SMD:D_MiniMELF" H 7350 4950 50  0001 C CNN
F 3 "~" H 7350 4950 50  0001 C CNN
	1    7350 4950
	-1   0    0    1   
$EndComp
$Comp
L Device:R R12
U 1 1 5FC4C40F
P 7000 5350
F 0 "R12" H 7070 5396 50  0000 L CNN
F 1 "100k" H 7070 5305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6930 5350 50  0001 C CNN
F 3 "~" H 7000 5350 50  0001 C CNN
	1    7000 5350
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR022
U 1 1 5FC5051B
P 7000 5700
F 0 "#PWR022" H 7000 5450 50  0001 C CNN
F 1 "GND" H 7005 5527 50  0000 C CNN
F 2 "" H 7000 5700 50  0001 C CNN
F 3 "" H 7000 5700 50  0001 C CNN
	1    7000 5700
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR024
U 1 1 5FC50A7B
P 7650 5700
F 0 "#PWR024" H 7650 5450 50  0001 C CNN
F 1 "GND" H 7655 5527 50  0000 C CNN
F 2 "" H 7650 5700 50  0001 C CNN
F 3 "" H 7650 5700 50  0001 C CNN
	1    7650 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	7000 5500 7000 5700
Text GLabel 9100 5250 2    50   Input ~ 0
POW-EN
Text GLabel 5650 4700 0    50   Input ~ 0
nBT0
Text GLabel 8800 4950 2    50   Input ~ 0
TLV62565-EN
$Comp
L Switch:SW_Push SW1
U 1 1 5FC6B4D4
P 7000 4600
F 0 "SW1" V 7046 4552 50  0000 R CNN
F 1 "BT0" V 6955 4552 50  0000 R CNN
F 2 "Button_Switch_SMD:SW_SPST_PTS645" H 7000 4800 50  0001 C CNN
F 3 "~" H 7000 4800 50  0001 C CNN
	1    7000 4600
	0    -1   -1   0   
$EndComp
Wire Wire Line
	7000 4400 7000 4200
Wire Wire Line
	7000 4800 7000 4950
$Comp
L Device:C C5
U 1 1 5FC75E5C
P 5800 5250
F 0 "C5" H 5915 5296 50  0000 L CNN
F 1 "10n" H 5915 5205 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 5838 5100 50  0001 C CNN
F 3 "~" H 5800 5250 50  0001 C CNN
	1    5800 5250
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR021
U 1 1 5FC768C8
P 6500 5700
F 0 "#PWR021" H 6500 5450 50  0001 C CNN
F 1 "GND" H 6505 5527 50  0000 C CNN
F 2 "" H 6500 5700 50  0001 C CNN
F 3 "" H 6500 5700 50  0001 C CNN
	1    6500 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 5700 6500 5150
Text Notes 5450 3900 0    50   ~ 0
To power-on the circuit, BT0 has to be pushed. The MCU can keep \nthe TLV62565 enabled using a GPIO pin, while BT0 is available as \na regular button.
$Comp
L Device:D_Schottky D1
U 1 1 5FCF4FA2
P 7350 1250
F 0 "D1" V 7396 1171 50  0000 R CNN
F 1 "SK24A" V 7305 1171 50  0000 R CNN
F 2 "Diode_SMD:D_SMA_Handsoldering" H 7350 1250 50  0001 C CNN
F 3 "~" H 7350 1250 50  0001 C CNN
	1    7350 1250
	0    -1   -1   0   
$EndComp
$Comp
L power:PWR_FLAG #FLG02
U 1 1 5FD57313
P 10400 1350
F 0 "#FLG02" H 10400 1425 50  0001 C CNN
F 1 "PWR_FLAG" H 10400 1523 50  0000 C CNN
F 2 "" H 10400 1350 50  0001 C CNN
F 3 "~" H 10400 1350 50  0001 C CNN
	1    10400 1350
	1    0    0    -1  
$EndComp
Wire Wire Line
	10400 1350 10400 1550
Wire Wire Line
	3750 6450 4150 6450
Wire Wire Line
	4150 6450 4150 6250
Connection ~ 3750 6450
$Comp
L power:PWR_FLAG #FLG01
U 1 1 5FD6EEE2
P 8450 1400
F 0 "#FLG01" H 8450 1475 50  0001 C CNN
F 1 "PWR_FLAG" H 8450 1573 50  0000 C CNN
F 2 "" H 8450 1400 50  0001 C CNN
F 3 "~" H 8450 1400 50  0001 C CNN
	1    8450 1400
	1    0    0    -1  
$EndComp
Wire Wire Line
	8450 1400 8450 1450
Connection ~ 8450 1550
Wire Wire Line
	8450 1550 8600 1550
Wire Notes Line
	11100 650  11100 2700
Wire Notes Line
	600  2700 600  650 
Wire Notes Line
	600  650  11100 650 
Text Notes 700  800  0    50   ~ 0
Power supply with USB, LiPo battery cell and load sharing
Text Notes 800  5650 0    50   ~ 0
Step-up DC/DC converter
Text Notes 1650 3400 0    50   ~ 0
Battery voltage measurement
Text Notes 5450 3550 0    50   ~ 0
Power control
Wire Notes Line
	9650 3350 9650 6000
Wire Notes Line
	2950 3100 2950 5050
Wire Notes Line
	2950 5050 1500 5050
Wire Notes Line
	1500 5050 1500 3100
Wire Notes Line
	1500 3100 2950 3100
Wire Notes Line
	650  5400 4400 5400
Wire Notes Line
	4400 5400 4400 7600
Wire Notes Line
	4400 7600 650  7600
Wire Notes Line
	650  7600 650  5400
$Comp
L Device:R R7
U 1 1 5FC3D862
P 4300 1900
F 0 "R7" H 4370 1946 50  0000 L CNN
F 1 "110k" H 4370 1855 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4230 1900 50  0001 C CNN
F 3 "~" H 4300 1900 50  0001 C CNN
	1    4300 1900
	1    0    0    -1  
$EndComp
$Comp
L Device:R R8
U 1 1 5FC3DCB7
P 4300 2200
F 0 "R8" H 4370 2246 50  0000 L CNN
F 1 "220k" H 4370 2155 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 4230 2200 50  0001 C CNN
F 3 "~" H 4300 2200 50  0001 C CNN
	1    4300 2200
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR014
U 1 1 5FC3E2BE
P 4300 2350
F 0 "#PWR014" H 4300 2100 50  0001 C CNN
F 1 "GND" H 4305 2177 50  0000 C CNN
F 2 "" H 4300 2350 50  0001 C CNN
F 3 "" H 4300 2350 50  0001 C CNN
	1    4300 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 2050 4450 2050
Connection ~ 4300 2050
Wire Notes Line
	11100 2700 600  2700
$Comp
L Transistor_FET:2N7002 Q1
U 1 1 5FC646F9
P 6600 4950
F 0 "Q1" H 6805 4996 50  0000 L CNN
F 1 "2N7002" H 6805 4905 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 6800 4875 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7002.pdf" H 6600 4950 50  0001 L CNN
	1    6600 4950
	-1   0    0    -1  
$EndComp
Wire Wire Line
	6800 4950 7000 4950
Wire Wire Line
	7000 4950 7000 5200
Connection ~ 7000 4950
$Comp
L power:+3V3 #PWR020
U 1 1 5FC98903
P 6500 4200
F 0 "#PWR020" H 6500 4050 50  0001 C CNN
F 1 "+3V3" H 6515 4373 50  0000 C CNN
F 2 "" H 6500 4200 50  0001 C CNN
F 3 "" H 6500 4200 50  0001 C CNN
	1    6500 4200
	1    0    0    -1  
$EndComp
Wire Wire Line
	6500 4300 6500 4200
Wire Wire Line
	7000 4950 7200 4950
$Comp
L Device:R R13
U 1 1 5FCAC727
P 7650 5350
F 0 "R13" H 7720 5396 50  0000 L CNN
F 1 "100k" H 7720 5305 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 7580 5350 50  0001 C CNN
F 3 "~" H 7650 5350 50  0001 C CNN
	1    7650 5350
	1    0    0    -1  
$EndComp
Wire Wire Line
	7500 4950 7650 4950
Wire Wire Line
	7650 4950 7650 5200
Wire Wire Line
	7650 5700 7650 5500
Connection ~ 7650 4950
$Comp
L Device:R R14
U 1 1 5FCD42C4
P 8250 4400
F 0 "R14" H 8320 4446 50  0000 L CNN
F 1 "100k" H 8320 4355 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8180 4400 50  0001 C CNN
F 3 "~" H 8250 4400 50  0001 C CNN
	1    8250 4400
	1    0    0    -1  
$EndComp
$Comp
L Transistor_FET:2N7002 Q4
U 1 1 5FD02EC6
P 8350 5250
F 0 "Q4" H 8200 5150 50  0000 L CNN
F 1 "2N7002" H 8000 5050 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 8550 5175 50  0001 L CIN
F 3 "https://www.fairchildsemi.com/datasheets/2N/2N7002.pdf" H 8350 5250 50  0001 L CNN
	1    8350 5250
	-1   0    0    -1  
$EndComp
$Comp
L power:GND #PWR025
U 1 1 5FD12AFA
P 8250 5700
F 0 "#PWR025" H 8250 5450 50  0001 C CNN
F 1 "GND" H 8255 5527 50  0000 C CNN
F 2 "" H 8250 5700 50  0001 C CNN
F 3 "" H 8250 5700 50  0001 C CNN
	1    8250 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8250 5700 8250 5450
Wire Wire Line
	8250 5050 8250 4600
$Comp
L Device:R R15
U 1 1 5FD1D00B
P 8850 5500
F 0 "R15" H 8920 5546 50  0000 L CNN
F 1 "100k" H 8920 5455 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 8780 5500 50  0001 C CNN
F 3 "~" H 8850 5500 50  0001 C CNN
	1    8850 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 5250 8850 5250
Wire Wire Line
	8850 5250 8850 5350
Wire Wire Line
	8850 5250 9100 5250
Connection ~ 8850 5250
$Comp
L power:GND #PWR026
U 1 1 5FD25151
P 8850 5700
F 0 "#PWR026" H 8850 5450 50  0001 C CNN
F 1 "GND" H 8855 5527 50  0000 C CNN
F 2 "" H 8850 5700 50  0001 C CNN
F 3 "" H 8850 5700 50  0001 C CNN
	1    8850 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	8850 5650 8850 5700
Wire Wire Line
	7650 4950 8800 4950
Wire Wire Line
	6500 4600 6500 4700
$Comp
L Device:R R9
U 1 1 5FD56A24
P 6150 4700
F 0 "R9" V 5943 4700 50  0000 C CNN
F 1 "1k" V 6034 4700 50  0000 C CNN
F 2 "Resistor_SMD:R_0603_1608Metric_Pad1.05x0.95mm_HandSolder" V 6080 4700 50  0001 C CNN
F 3 "~" H 6150 4700 50  0001 C CNN
	1    6150 4700
	0    1    1    0   
$EndComp
Wire Wire Line
	6300 4700 6500 4700
Connection ~ 6500 4700
Wire Wire Line
	6500 4700 6500 4750
Wire Wire Line
	6000 4700 5800 4700
$Comp
L power:GND #PWR018
U 1 1 5FD66EF0
P 5800 5700
F 0 "#PWR018" H 5800 5450 50  0001 C CNN
F 1 "GND" H 5805 5527 50  0000 C CNN
F 2 "" H 5800 5700 50  0001 C CNN
F 3 "" H 5800 5700 50  0001 C CNN
	1    5800 5700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5800 5700 5800 5400
Wire Wire Line
	5800 5100 5800 4700
Connection ~ 5800 4700
Wire Wire Line
	5800 4700 5650 4700
Wire Notes Line
	5350 6000 5350 3350
Wire Notes Line
	5350 6000 9650 6000
Wire Notes Line
	5350 3350 9650 3350
Wire Wire Line
	7000 4200 7650 4200
Wire Wire Line
	7650 4400 7650 4200
Connection ~ 7650 4200
Text GLabel 8400 1450 0    50   Input ~ 0
TLV62656-VIN
Wire Wire Line
	8400 1450 8450 1450
Connection ~ 8450 1450
Wire Wire Line
	8450 1450 8450 1550
Text GLabel 8000 4000 2    50   Input ~ 0
TLV62656-VIN
Wire Wire Line
	7950 4600 8250 4600
Connection ~ 8250 4600
Wire Wire Line
	8250 4600 8250 4550
Wire Wire Line
	7650 4200 8250 4200
Wire Wire Line
	8250 4200 8250 4250
Wire Wire Line
	7650 4000 8000 4000
Wire Wire Line
	7650 4000 7650 4200
$Comp
L Device:L L2
U 1 1 5FC783E8
P 9900 1550
F 0 "L2" V 10090 1550 50  0000 C CNN
F 1 "2.2uH" V 9999 1550 50  0000 C CNN
F 2 "zmolnar-Device:Murata_2u2H_LQH44PN2R2MP0L" H 9900 1550 50  0001 C CNN
F 3 "~" H 9900 1550 50  0001 C CNN
	1    9900 1550
	0    -1   -1   0   
$EndComp
Wire Wire Line
	10050 1550 10400 1550
Wire Wire Line
	9200 1550 9750 1550
$Comp
L zmolnar-Regulator_Switching:TLV62565 U3
U 1 1 5FC8C7DC
P 8900 1650
F 0 "U3" H 8900 2015 50  0000 C CNN
F 1 "TLV62565" H 8900 1924 50  0000 C CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5_HandSoldering" H 8900 1100 50  0001 C CNN
F 3 "https://www.ti.com/lit/ds/symlink/tlv62565.pdf?ts=1606705630776&ref_url=https%253A%252F%252Fwww.google.de%252F" H 8900 1400 50  0001 C CNN
	1    8900 1650
	1    0    0    -1  
$EndComp
$Comp
L zmolnar-Transistor_FET:FDV304P Q3
U 1 1 5FCACD3C
P 7750 4600
F 0 "Q3" H 7955 4554 50  0000 L CNN
F 1 "FDV304P" H 7955 4645 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23" H 7950 4525 50  0001 L CIN
F 3 "http://www.vishay.com/docs/70209/70209.pdf" H 7750 4600 50  0001 L CNN
	1    7750 4600
	-1   0    0    1   
$EndComp
Wire Wire Line
	7650 4800 7650 4950
$EndSCHEMATC
