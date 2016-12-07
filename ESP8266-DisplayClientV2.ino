#ifdef ESP8266
#include <ESP8266WiFi.h>
#endif

// Uncomment to use TM1638
#define INCLUDE_TM1638 //{"Name":"INCLUDE_TM1638","Type":"autodefine","Condition":"[TM1638_ENABLEDMODULES]>0"}

// Uncomment to use WS2801 leds
//#define INCLUDE_WS2801 //{"Name":"INCLUDE_WS2801","Type":"autodefine","Condition":"[WS2801_RGBLEDCOUNT]>0"}

// Uncomment to use WS2812b leds
//#define INCLUDE_WS2812B //{"Name":"INCLUDE_WS2812B","Type":"autodefine","Condition":"[WS2812B_RGBLEDCOUNT]>0"}

// Uncomment to use adafruit HT16K33 modules
//#define INCLUDE_LEDBACKPACK //{"Name":"INCLUDE_LEDBACKPACK","Type":"autodefine","Condition":"[ENABLE_ADA_HT16K33_7SEGMENTS]>0 || [ENABLE_ADA_HT16K33_Matrix]>0"}

// Uncomment to use tm1637
//#define INCLUDE_TM1637 //{"Name":"INCLUDE_TM1637","Type":"autodefine","Condition":"[TM1637_ENABLEDMODULES]>0"}

// Uncomment to use 20x4 I2CLCD
//#define INCLUDE_I2CLCD //{"Name":"INCLUDE_I2CLCD","Type":"autodefine","Condition":"[I2CLCD_enabled]>0"}

// Uncomment to use Nokia 5110/3310 LCD
//#define INCLUDE_NOKIALCD //{"Name":"INCLUDE_NOKIALCD","Type":"autodefine","Condition":"[ENABLED_NOKIALCD]>0"}

#ifdef ESP8266
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif
#include <EEPROM.h>

#ifdef INCLUDE_TM1638
#include <TM1638.h>
#endif

#include <SPI.h>
#include "Arduino.h"

#ifdef ESP8266
#include <pgmspace.h>
#else
#include <avr/pgmspace.h>
#endif

#ifdef INCLUDE_WS2812B
#include <Adafruit_NeoPixel.h>
#endif

#include <Wire.h>
#include "LedControl.h"
#include "Adafruit_GFX.h"
#include <Servo.h>

#ifdef INCLUDE_WS2801
#include <Adafruit_WS2801.h>
#endif

#ifdef INCLUDE_TM1637
#include "TM1637.h"
#endif

#ifdef INCLUDE_I2CLCD
#include <LiquidCrystal_I2C.h>
#endif

#ifdef INCLUDE_LEDBACKPACK
#include "Adafruit_LEDBackpack.h"
#endif

#ifdef INCLUDE_NOKIALCD
#include <Adafruit_PCD8544.h>

#include "ACHubCustomFonts/Open24DisplaySt12pt7b.h"
#define CUSTOM_LCD_FONT_1 Open24DisplaySt12pt7b

#include "ACHubCustomFonts/Open24DisplaySt18pt7b.h"
#define CUSTOM_LCD_FONT_2 Open24DisplaySt18pt7b

#endif

// ----------------------------------------------------- HW SETTINGS, PLEASE REVIEW ALL -------------------------------------------

String DEVICE_NAME = String("ACHUB Dash"); //{"Group":"General","Name":"DEVICE_NAME","Title":"Device name","DefaultValue":"ACHUB Dash","Type":"string","Template":"String DEVICE_NAME = String(\"{0}\");"}

// -------------------------------------------------------------------------------------------------------
// TM1638 Modules ----------------------------------------------------------------------------------------
// http://www.dx.com/p/jy-mcu-8x-green-light-digital-tube-8x-key-8x-double-color-led-module-104329
// https://www.fasttech.com/products/0/10006503/1497600-jy-mcu-8x-digital-tube-red-led-module
// -------------------------------------------------------------------------------------------------------

// Number of Connected TM1638/1639 modules
// 0 disabled, > 0 enabled
int TM1638_ENABLEDMODULES = 1; //{"Group":"TM1638","Name":"TM1638_ENABLEDMODULES","Title":"TM1638 modules connected\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int TM1638_ENABLEDMODULES = {0};"}
// 0 disabled, 1 enabled
int TM1638_SWAPLEDCOLORS = 0; //{"Group":"TM1638","Name":"TM1638_SWAPLEDCOLORS","Title":"Reverse Red and green colors \r\n0 = No, 1 = Yes","DefaultValue":"0","Type":"boolean","Template":"int TM1638_SWAPLEDCOLORS = {0};"}
// TM1638 Modules common pins
#define TM1638_DIO 4 //{"Group":"TM1638","Name":"TM1638_DIO","Title":"Common data (DIO) digital pin number","DefaultValue":"8","Type":"integer","Template":"#define TM1638_DIO {0}"}
#define TM1638_CLK 14 //{"Group":"TM1638","Name":"TM1638_CLK","Title":"Common clock (CLK) digital pin number","DefaultValue":"7","Type":"integer","Template":"#define TM1638_CLK {0}"}
// TM1638 1st module strobe pin
#define TM1638_STB1 5 //{"Group":"TM1638","Name":"TM1638_STB1","Title":"1st module strobe (STB0) digital pin number","DefaultValue":"9","Type":"integer","Template":"#define TM1638_STB1 {0}"}
// TM1638 2nd screen strobe pin
#define TM1638_STB2 0 //{"Group":"TM1638","Name":"TM1638_STB2","Title":"2d module strobe (STB1) digital pin number","DefaultValue":"10","Type":"integer","Template":"#define TM1638_STB2 {0}"}
// TM1638 3nd screen strobe pin
#define TM1638_STB3 0 //{"Group":"TM1638","Name":"TM1638_STB3","Title":"3rd module strobe (STB2) digital pin number","DefaultValue":"11","Type":"integer","Template":"#define TM1638_STB3 {0}"}
// TM1638 4rd screen strobe pin
#define TM1638_STB4 0 //{"Group":"TM1638","Name":"TM1638_STB4","Title":"4th module strobe (STB3) digital pin number","DefaultValue":"12","Type":"integer","Template":"#define TM1638_STB4 {0}"}
// TM1638 5th screen strobe pin
#define TM1638_STB5 0 //{"Group":"TM1638","Name":"TM1638_STB5","Title":"5th module strobe (STB4) digital pin number","DefaultValue":"0","Type":"integer","Template":"#define TM1638_STB5 {0}"}
// TM1638 6th screen strobe pin
#define TM1638_STB6 0 //{"Group":"TM1638","Name":"TM1638_STB6","Title":"6th module strobe (STB5) digital pin number","DefaultValue":"0","Type":"integer","Template":"#define TM1638_STB6 {0}"}

// -------------------------------------------------------------------------------------------------------
// TM1637 Modules ----------------------------------------------------------------------------------------
// http://www.dx.com/p/0-36-led-4-digit-display-module-for-arduino-black-blue-works-with-official-arduino-boards-254978
// -------------------------------------------------------------------------------------------------------

// Number of Connected TM1637 modules
// 0 disabled, > 0 enabled
int TM1637_ENABLEDMODULES = 0; //{"Group":"TM1637","Name":"TM1637_ENABLEDMODULES","Title":"TM1637 modules connected\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int TM1637_ENABLEDMODULES = {0};"}
#ifdef INCLUDE_TM1637
#define TM1637_DIO1 4 //{"Group":"TM1637","Name":"TM1637_DIO1","Title":"1st TM1637 DIO digital pin number","DefaultValue":"4","Type":"integer","Template":"#define TM1637_DIO1 {0}"}
#define TM1637_CLK1 3 //{"Group":"TM1637","Name":"TM1637_CLK1","Title":"1st TM1637 CLK digital pin number","DefaultValue":"3","Type":"integer","Template":"#define TM1637_CLK1 {0}"}

#define TM1637_DIO2 4 //{"Group":"TM1637","Name":"TM1637_DIO2","Title":"2nd TM1637 DIO digital pin number","DefaultValue":"4","Type":"integer","Template":"#define TM1637_DIO2 {0}"}
#define TM1637_CLK2 3 //{"Group":"TM1637","Name":"TM1637_CLK2","Title":"2nd TM1637 CLK digital pin number","DefaultValue":"3","Type":"integer","Template":"#define TM1637_CLK2 {0}"}

#define TM1637_DIO3 4 //{"Group":"TM1637","Name":"TM1637_DIO3","Title":"3rd TM1637 DIO digital pin number","DefaultValue":"4","Type":"integer","Template":"#define TM1637_DIO3 {0}"}
#define TM1637_CLK3 3 //{"Group":"TM1637","Name":"TM1637_CLK3","Title":"3rd TM1637 CLK digital pin number","DefaultValue":"3","Type":"integer","Template":"#define TM1637_CLK3 {0}"}

#define TM1637_DIO4 4 //{"Group":"TM1637","Name":"TM1637_DIO4","Title":"4th TM1637 DIO digital pin number","DefaultValue":"4","Type":"integer","Template":"#define TM1637_DIO4 {0}"}
#define TM1637_CLK4 3 //{"Group":"TM1637","Name":"TM1637_CLK4","Title":"4th TM1637 CLK digital pin number","DefaultValue":"3","Type":"integer","Template":"#define TM1637_CLK4 {0}"}

#define TM1637_DIO5 4 //{"Group":"TM1637","Name":"TM1637_DIO5","Title":"5th TM1637 DIO digital pin number","DefaultValue":"4","Type":"integer","Template":"#define TM1637_DIO5 {0}"}
#define TM1637_CLK5 3 //{"Group":"TM1637","Name":"TM1637_CLK5","Title":"5th TM1637 CLK digital pin number","DefaultValue":"3","Type":"integer","Template":"#define TM1637_CLK5 {0}"}

#define TM1637_DIO6 4 //{"Group":"TM1637","Name":"TM1637_DIO6","Title":"6th TM1637 DIO digital pin number","DefaultValue":"4","Type":"integer","Template":"#define TM1637_DIO6 {0}"}
#define TM1637_CLK6 3 //{"Group":"TM1637","Name":"TM1637_CLK6","Title":"6th TM1637 CLK digital pin number","DefaultValue":"3","Type":"integer","Template":"#define TM1637_CLK6 {0}"}
#endif

// -------------------------------------------------------------------------------------------------------
// MAX7219 / MAX7221 7 Segment modules -----------------------------------------------------------------------------
// http://www.dx.com/p/max7219-led-dot-matrix-digital-led-display-tube-module-cascade-391256
// -------------------------------------------------------------------------------------------------------

// 0 disabled, > 0 enabled
int MAX7221_ENABLEDMODULES = 0; //{"Group":"MAX7221","Name":"MAX7221_ENABLEDMODULES","Title":"MAX7219 / MAX7221 7 Segment modules connected \r\nSet to 0 if none\r\nMultiple modules can be cascaded connected module output to next module input","DefaultValue":"0","Type":"integer","Template":"int MAX7221_ENABLEDMODULES = {0};"}
// DATA IN - pin of the first MAX7221
#define MAX7221_DATA 3 //{"Group":"MAX7221","Name":"MAX7221_DATA","Title":"DATA (DIN) digital pin number","DefaultValue":"3","Type":"integer","Template":"#define MAX7221_DATA {0}"}
// CLK - pin of the first MAX7221
#define MAX7221_CLK 5 //{"Group":"MAX7221","Name":"MAX7221_CLK","Title":"CLOCK (CLK) digital pin number","DefaultValue":"5","Type":"integer","Template":"#define MAX7221_CLK {0}"}
// LOAD(/ CS) - pin of the first MAX7221
#define MAX7221_LOAD 4 //{"Group":"MAX7221","Name":"MAX7221_LOAD","Title":"LOAD (LD) digital pin number","DefaultValue":"4","Type":"integer","Template":"#define MAX7221_LOAD {0}"}
LedControl MAX7221 = LedControl(MAX7221_DATA, MAX7221_CLK, MAX7221_LOAD, MAX7221_ENABLEDMODULES);

// -------------------------------------------------------------------------------------------------------
// MAX7219 / MAX7221 Led Matrix module -----------------------------------------------------------------------------
// http://www.dx.com/p/max7219-dot-matrix-module-w-5-dupont-lines-184854
// -------------------------------------------------------------------------------------------------------

// 0 disabled, 1 enabled
int MAX7221_MATRIX_ENABLED = 0; //{"Group":"MAX7221_MATRIX","Name":"MAX7221_MATRIX_ENABLED","Title":"MAX7221 led matrix enabled \r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"integer","Template":"int MAX7221_MATRIX_ENABLED = {0};"}
int MAX7221_MATRIX_LUMINOSITY = 15;
// DATA IN - pin of the first MAX7221
#define MAX7221_MATRIX_DATA 3 //{"Group":"MAX7221_MATRIX","Name":"MAX7221_MATRIX_DATA","Title":"DATA (DIN) digital pin number","DefaultValue":"3","Type":"integer","Template":"#define MAX7221_MATRIX_DATA {0}"}
// CLK - pin of the first MAX7212
#define MAX7221_MATRIX_CLK 5 //{"Group":"MAX7221_MATRIX","Name":"MAX7221_MATRIX_CLK","Title":"CLOCK (CLK) digital pin number","DefaultValue":"5","Type":"integer","Template":"#define MAX7221_MATRIX_CLK {0}"}
// LOAD(/ CS) - pin of the first MAX7221
#define MAX7221_MATRIX_LOAD 4 //{"Group":"MAX7221_MATRIX","Name":"MAX7221_MATRIX_LOAD","Title":"LOAD (LD) digital pin number","DefaultValue":"4","Type":"integer","Template":"#define MAX7221_MATRIX_LOAD {0}"}
LedControl MAX7221_MATRIX = LedControl(MAX7221_MATRIX_DATA, MAX7221_MATRIX_CLK, MAX7221_MATRIX_LOAD, MAX7221_MATRIX_ENABLED);

// -------------------------------------------------------------------------------------------------------
// WS2812b RGBLEDS ---------------------------------------------------------------------------------------
// http://www.dx.com/p/8-bit-ws2812-5050-rgb-led-development-board-w-built-in-full-color-drive-387667
// -------------------------------------------------------------------------------------------------------

// WS2812b chained RGBLEDS count
// 0 disabled, > 0 enabled
int WS2812B_RGBLEDCOUNT = 0; //{"Group":"WS2812B_RGBLED","Name":"WS2812B_RGBLEDCOUNT","Title":"WS2812B RGB leds count\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int WS2812B_RGBLEDCOUNT = {0};"}
// 0 leds will be used from left to right, 1 leds will be used from right to left
int WS2812B_RIGHTTOLEFT = 0; //{"Group":"WS2812B_RGBLED","Name":"WS2812B_RIGHTTOLEFT","Title":"Reverse led order \r\n0 = No, 1 = Yes","DefaultValue":"0","Type":"boolean","Template":"int WS2812B_RIGHTTOLEFT = {0};"}
// WS2812b chained RGBLEDS pins
#define WS2812B_DATAPIN 6 //{"Group":"WS2812B_RGBLED","Name":"WS2812B_DATAPIN","Title":"Data (DIN) digital pin number","DefaultValue":"6","Type":"boolean","Template":"#define WS2812B_DATAPIN {0}"}

#ifdef INCLUDE_WS2812B
Adafruit_NeoPixel WS2812B_strip = Adafruit_NeoPixel(WS2812B_RGBLEDCOUNT, WS2812B_DATAPIN, NEO_GRB + NEO_KHZ800);
#endif

// -------------------------------------------------------------------------------------------------------
// WS2801 RGBLEDS ----------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------------

// 0 disabled, > 0 enabled
int WS2801_RGBLEDCOUNT = 0; //{"Group":"WS2801_RGBLED","Name":"WS2801_RGBLEDCOUNT","Title":"WS2801 RGB leds count\r\nSet to 0 if none","DefaultValue":"0","Type":"integer","Template":"int WS2801_RGBLEDCOUNT = {0};"}

#ifdef INCLUDE_WS2801
// 0 leds will be used from left to right, 1 leds will be used from right to left
int WS2801_RIGHTTOLEFT = 0; //{"Group":"WS2801_RGBLED","Name":"WS2801_RIGHTTOLEFT","Title":"WS2801 right to left\r\nSet to 0 for left to right, 1 for right to left","DefaultValue":"0","Type":"integer","Template":"int WS2801_RIGHTTOLEFT = {0};"}

							// WS2801 Pins
#define WS2801_DATAPIN 5 //{"Group":"WS2801_RGBLED","Name":"WS2801_DATAPIN","Title":"Data (DIN) digital pin number","DefaultValue":"5","Type":"integer","Template":"#define WS2801_DATAPIN {0}"}
#define WS2801_CLOCKPIN 6 //{"Group":"WS2801_RGBLED","Name":"WS2801_CLOCKPIN","Title":"Clock (DIN) digital pin number","DefaultValue":"6","Type":"integer","Template":"#define WS2801_CLOCKPIN {0}"}
Adafruit_WS2801 WS2801_strip = Adafruit_WS2801(WS2801_RGBLEDCOUNT, WS2801_DATAPIN, WS2801_CLOCKPIN);

#endif

// -------------------------------------------------------------------------------------------------------
// I2C LiquidCristal - WIP no finished
// http://www.dx.com/p/arduino-iic-i2c-serial-3-2-lcd-2004-module-display-138611#.Vb0QtW7tlBd
// Board				I2C / TWI pins
// Uno, Ethernet, Nano	A4(SDA), A5(SCL)
// Mega2560				20 (SDA), 21 (SCL)
// Leonardo				2 (SDA), 3 (SCL)
// Due					20 (SDA), 21 (SCL), SDA1, SCL1
// -------------------------------------------------------------------------------------------------------
int I2CLCD_enabled = 0; //{"Group":"I2C LCD","Name":"I2CLCD_enabled","Title":"I2C LCD (2004) enabled\r\n0 = disabled, 1 = enabled\r\nUno, Ethernet, Nano : A4(SDA), A5(SCL)\r\nMega2560 : 20 (SDA), 21 (SCL)\r\nLeonardo : 2 (SDA), 3 (SCL)\r\nDue : 20 (SDA), 21 (SCL), SDA1, SCL1","DefaultValue":"0","Type":"integer","Template":"int I2CLCD_enabled = {0};"}
int I2CLCD_width = 20;
int I2CLCD_height = 4;
byte I2CLCD_address = 0x3f; //{"Group":"I2C LCD","Name":"I2CLCD_address","Title":"I2C address","DefaultValue":"0x3f","Type":"hex","Template":"byte I2CLCD_address = {0};"}
#ifdef INCLUDE_I2CLCD
LiquidCrystal_I2C I2CLCD(I2CLCD_address, I2CLCD_width, I2CLCD_height);
#endif

// --------------------------------------------------------------------------------------------------------
// 74HC595 Shift register, one 7seg module for gear indicator ---------------------------------------------
// DIY : http://lucidtronix.com/tutorials/2
// --------------------------------------------------------------------------------------------------------

// 0 disabled, 1 enabled
int ENABLE_74HC595_GEAR_DISPLAY = 0; //{"Group":"74HC595 GEAR DISPLAY","Name":"ENABLE_74HC595_GEAR_DISPLAY","Title":"74HC595 gear display enabled\r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"hex","Template":"int ENABLE_74HC595_GEAR_DISPLAY = {0};"}
// Reverse led state
int RS_74HC595_INVERT = 0;
#define RS_74HC595_DATAPIN 2 //{"Group":"74HC595 GEAR DISPLAY","Name":"RS_74HC595_DATAPIN","Title":"DATA digital pin number","DefaultValue":"2","Type":"hex","Template":"#define RS_74HC595_DATAPIN {0}"}
#define RS_74HC595_LATCHPIN 3 //{"Group":"74HC595 GEAR DISPLAY","Name":"RS_74HC595_LATCHPIN","Title":"LATCH digital pin number","DefaultValue":"3","Type":"hex","Template":"#define RS_74HC595_LATCHPIN {0}"}
#define RS_74HC595_CLOCKPIN 4 //{"Group":"74HC595 GEAR DISPLAY","Name":"RS_74HC595_CLOCKPIN","Title":"CLOCK digital pin number","DefaultValue":"4","Type":"hex","Template":"#define RS_74HC595_CLOCKPIN {0}"}
// RS_74HC595 DIGITS
// 0,1,2 ....
byte RS_74HC595_dec_digits[] = { 0b11111100, 0b01100000, 0b11011010, 0b11110010, 0b01100110, 0b10110110, 0b10111110, 0b11100000, 0b11111110, 0b11110110 };
// Empty
byte RS_74HC595_ClearDigit = 0b00000000;
// R
byte RS_74HC595_ReverseDigit = 0b10001100;
// N
byte RS_74HC595_NeutralDigit = 0b11101100;

// --------------------------------------------------------------------------------------------------------
// Adafruit 4x7segment with HT16K33 I2C Backpack
// These displays use I2C to communicate, 2 pins are required to interface.
// There are multiple selectable I2C addresses.
// For backpacks with 2 Address Select addresses : 0x70, 0x71, 0x72 or 0x73.
// For backpacks with 3 Address Select addresses : 0x70 thru 0x77
// --------------------------------------------------------------------------------------------------------
// 0 disabled, 1 enabled
int ENABLE_ADA_HT16K33_7SEGMENTS = 0; //{"Group":"HT16K33_7SEGMENTS","Name":"ENABLE_ADA_HT16K33_7SEGMENTS","Title":"Adafruit HT16K33 7 segments display enabled\r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"hex","Template":"int ENABLE_ADA_HT16K33_7SEGMENTS = {0};"}
#ifdef INCLUDE_LEDBACKPACK
// I2C Adress
byte ADA_HT16K33_7SEGMENTS_I2CADDRESS = 0x70;
Adafruit_7segment ADA_HT16K33_7SEGMENTS = Adafruit_7segment();
#endif

// --------------------------------------------------------------------------------------------------------
// Adafruit 8x8 bi color with HT16K33 I2C Backpack
// These displays use I2C to communicate, 2 pins are required to interface.
// There are multiple selectable I2C addresses.
// For backpacks with 2 Address Select addresses : 0x70, 0x71, 0x72 or 0x73.
// For backpacks with 3 Address Select addresses : 0x70 thru 0x77
// --------------------------------------------------------------------------------------------------------
// 0 disabled, 1 enabled
int ENABLE_ADA_HT16K33_Matrix = 0; //{"Group":"HT16K33_Matrix","Name":"ENABLE_ADA_HT16K33_Matrix","Title":"Adafruit HT16K33 matrix display enabled\r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"hex","Template":"int ENABLE_ADA_HT16K33_Matrix = {0};"}
// I2C Adress
#ifdef INCLUDE_LEDBACKPACK
byte ADA_HT16K33_MATRIX_I2CADDRESS = 0x71;
Adafruit_BicolorMatrix ADA_HT16K33_MATRIX = Adafruit_BicolorMatrix();
byte ADA_HT16K33_Matrix_luminosity = 0;
#endif

// ----------------------- SERVO MOTOR ---------------------------------------------------------------------
// https://www.arduino.cc/en/Tutorial/Sweep
// ----------------------------------------------------------------------------------------------------------
// 0 disabled, 1 enabled
int ENABLE_SERVO = 0; //{"Group":"SERVO","Name":"ENABLE_SERVO","Title":"Enable servo motor\r\n0 = disabled, 1 = enabled","DefaultValue":"0","Type":"integer","Template":"int ENABLE_SERVO = {0};"}
int SERVO_PIN = 9; //{"Group":"SERVO","Name":"SERVO_PIN","Title":"Servo motor control pin\r\nPlease use PWM pin","DefaultValue":"9","Type":"integer","Template":"int SERVO_PIN = {0};"}
int upDelay = 2; // Automatically updated by server
int downDelay = 4; // Automatically updated by server
//int SERVO_MAX_ANGLE = 180;
//int SERVO_MIN_ANGLE = 0;
int targetValue = 0;
int currentValue = 0;
//int upStep = 2;
//int downStep = 1;
double updateTime = 0;
Servo myservo;

// ----------------------- ADDITIONAL BUTTONS ---------------------------------------------------------------
// https://www.arduino.cc/en/Tutorial/InputPullupSerial
// ----------------------------------------------------------------------------------------------------------
int ENABLED_BUTTONS_COUNT = 0; //{"Group":"ADDITIONAL BUTTONS","Name":"ENABLED_BUTTONS_COUNT","Title":"Additional buttons (directly connected to the arduino) buttons count\r\n0 = disabled, >0  = enabled","DefaultValue":"0","Type":"integer","Template":"int ENABLED_BUTTONS_COUNT = {0};"}
int BUTTON_PIN_1 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_1","Title":"1'st Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_1 = {0};"}
int BUTTON_PIN_2 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_2","Title":"2'nd Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_2 = {0};"}
int BUTTON_PIN_3 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_3","Title":"3'rd Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_3 = {0};"}
int BUTTON_PIN_4 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_4","Title":"4'th Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_4 = {0};"}
int BUTTON_PIN_5 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_5","Title":"5'th Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_5 = {0};"}
int BUTTON_PIN_6 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_6","Title":"6'th Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_6 = {0};"}
int BUTTON_PIN_7 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_7","Title":"7'th Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_7 = {0};"}
int BUTTON_PIN_8 = 3; //{"Group":"ADDITIONAL BUTTONS","Name":"BUTTON_PIN_8","Title":"8'th Additional button digital pin","DefaultValue":"3","Type":"integer","Template":"int BUTTON_PIN_8 = {0};"}
int BUTTON_PINS[] = { BUTTON_PIN_1, BUTTON_PIN_2, BUTTON_PIN_3, BUTTON_PIN_4, BUTTON_PIN_5, BUTTON_PIN_6, BUTTON_PIN_7, BUTTON_PIN_8 };



// -------------------- NOKIA GLCD -------------------------------------------------------------------------
int ENABLED_NOKIALCD = 0; //{"Group":"NOKIA_GLCD","Name":"ENABLED_NOKIALCD","Title":"Number of Nokia GLCD connected (up to 2)\r\n0 = disabled, >0  = enabled","DefaultValue":"0","Type":"integer","Template":"int ENABLED_NOKIALCD = {0};"}
//#define ENABLE_SECONDARY_NOKIALCD //{"Name":"ENABLE_SECONDARY_NOKIALCD","Type":"autodefine","Condition":"[ENABLED_NOKIALCD]>1"}

#ifdef INCLUDE_NOKIALCD
// Adafruit_PCD8544 display = Adafruit_PCD8544(SCLK, DIN, DC, CS or SCE, RST);
Adafruit_PCD8544 nokiaLCD = Adafruit_PCD8544(A0, A1, A2, A4, A3);
#ifdef ENABLE_SECONDARY_NOKIALCD
Adafruit_PCD8544 nokiaLCD2 = Adafruit_PCD8544(3, 4, 5, 7, 6);
Adafruit_PCD8544 * nokiaLCDs[] = { &nokiaLCD, &nokiaLCD2 };
#else
Adafruit_PCD8544 * nokiaLCDs[] = { &nokiaLCD };

#endif


#endif

// ----------------------------------------------------- END OF HW SETTINGS --------------------------------------------------------------

struct ScreenItem {
public:
#ifdef INCLUDE_TM1638
	TM1638 * Screen;
#endif
	byte Buttons;
	byte Oldbuttons;
	byte Intensity;

	ScreenItem() { }
#ifdef INCLUDE_TM1638
	ScreenItem(TM1638 * module) : Screen(module) {
		this->Buttons = 0;
		this->Oldbuttons = 0;
		this->Intensity = 7;
	}
#endif
};


uint8_t  fontSize;
int16_t  posX;
int16_t  posY;
char action;
#ifdef INCLUDE_TM1638
TM1638 TM1638_module1(TM1638_DIO, TM1638_CLK, TM1638_STB1, false);
ScreenItem TM1638_screen1(&TM1638_module1);

TM1638 TM1638_module2(TM1638_DIO, TM1638_CLK, TM1638_STB2, false);
ScreenItem TM1638_screen2(&TM1638_module2);

TM1638 TM1638_module3(TM1638_DIO, TM1638_CLK, TM1638_STB3, false);
ScreenItem TM1638_screen3(&TM1638_module3);

TM1638 TM1638_module4(TM1638_DIO, TM1638_CLK, TM1638_STB4, false);
ScreenItem TM1638_screen4(&TM1638_module4);

TM1638 TM1638_module5(TM1638_DIO, TM1638_CLK, TM1638_STB5, false);
ScreenItem TM1638_screen5(&TM1638_module5);

TM1638 TM1638_module6(TM1638_DIO, TM1638_CLK, TM1638_STB6, false);
ScreenItem TM1638_screen6(&TM1638_module6);




// Screen referencing
ScreenItem * TM1638_screens[] = { &TM1638_screen1, &TM1638_screen2, &TM1638_screen3, &TM1638_screen4, &TM1638_screen5, &TM1638_screen6 };
#endif


ScreenItem MAX7221_screen1;
ScreenItem MAX7221_screen2;
ScreenItem MAX7221_screen3;
ScreenItem MAX7221_screen4;
ScreenItem MAX7221_screen5;
ScreenItem MAX7221_screen6;

ScreenItem * MAX7221_screens[] = { &MAX7221_screen1, &MAX7221_screen2, &MAX7221_screen3, &MAX7221_screen4, &MAX7221_screen5, &MAX7221_screen6 };

#ifdef INCLUDE_TM1637
TM1637 TM1637_module1(TM1637_CLK1, TM1637_DIO1);
TM1637 TM1637_module2(TM1637_CLK2, TM1637_DIO2);
TM1637 TM1637_module3(TM1637_CLK3, TM1637_DIO3);
TM1637 TM1637_module4(TM1637_CLK4, TM1637_DIO4);
TM1637 TM1637_module5(TM1637_CLK5, TM1637_DIO5);
TM1637 TM1637_module6(TM1637_CLK6, TM1637_DIO6);

TM1637 * TM1637_screens[] = { &TM1637_module1, &TM1637_module2, &TM1637_module3, &TM1637_module4, &TM1637_module5, &TM1637_module6 };
#endif

int i;
byte displayValues[] = { 1, 2, 4, 8, 16, 32, 64, 128 };

char LCD_buffer[20];
int  LCD_BufferPos = 0;

byte MAX7221_ByteReorder(byte x)
{
	x = ((x >> 1) & 0x55) | ((x << 1) & 0xaa);
	x = ((x >> 2) & 0x33) | ((x << 2) & 0xcc);
	x = ((x >> 4) & 0x0f) | ((x << 4) & 0xf0);
	return (x >> 1) | ((x & 1) << 7);
}

int FlowSerialPosition = 0;
int FlowSerialTrigger = 8;

int FlowSerialRead() {
	FlowSerialPosition++;
	if (FlowSerialPosition == FlowSerialTrigger) {
		FlowSerialPosition = 0;
		Serial.write(0x13);
		Serial.flush();
	}
	return Serial.read();
}
#define FlowSerialAvailable Serial.available
#define FlowSerialFlush Serial.flush

//#define FlowSerialWrite Serial.write
void FlowSerialWrite(uint8_t data) {
	Serial.write(0x14);
	Serial.write(data);
}

void FlowSerialPrint(char data)
{
	FlowSerialWrite(data);
}

void FlowSerialPrint(String& data)
{
	int len = data.length();
	for (int x = 0; x < len; x++)
		FlowSerialWrite(data.charAt(x));
}

//#define FlowSerialPrint Serial.print
void FlowSerialPrint(const char str[]) {
	for (int x = 0; x < strlen(str); x++)
		FlowSerialWrite(str[x]);
}

#define FlowSerialBegin Serial.begin
//#define FlowSerialReadStringUntil Serial.readStringUntil

long _startMillis;
int FlowSerialTimedRead()
{
	int c;
	_startMillis = millis();
	do {
		c = Serial.read();
		if (c >= 0) {
			FlowSerialPosition++;
			if (FlowSerialPosition == FlowSerialTrigger) {
				FlowSerialPosition = 0;
				Serial.write(0x13);
				Serial.flush();
			}
			return c;
		}
	} while (millis() - _startMillis < 1000);
	return -1;     // -1 indicates timeout
}

String FlowSerialReadStringUntil(char terminator) {
	String ret;
	int c = FlowSerialTimedRead();
	while (c >= 0 && c != terminator)
	{
		ret += (char)c;
		c = FlowSerialTimedRead();
	}
	return ret;
}

void RS_74HC595_SendData(byte data) {
	// take the latchPin low so
	// the LEDs don't change while you're sending in bits:
	digitalWrite(RS_74HC595_LATCHPIN, LOW);
	// shift out the bits:
	shiftOut(RS_74HC595_DATAPIN, RS_74HC595_CLOCKPIN, MSBFIRST, data);
	//take the latch pin high so the LEDs will light up:
	digitalWrite(RS_74HC595_LATCHPIN, HIGH);
}

void RS_74HC595_SetChar(char c) {
	byte data = 0;
	if (c == ' ') {
		data = RS_74HC595_ClearDigit;
	}
	else if (c == 'R') {
		data = RS_74HC595_ReverseDigit;
	}
	else if (c == 'N') {
		data = RS_74HC595_NeutralDigit;
	}
	else {
		int value = c - '0';
		if (value >= 0 && value < 10) {
			data = RS_74HC595_dec_digits[value];
		}
	}

	if (RS_74HC595_INVERT == 1) {
		data = data ^ 0xFF;
	}
	RS_74HC595_SendData(data);
}

void RS_74HC595_Init() {
	//set pins to output so you can control the shift register
	pinMode(RS_74HC595_DATAPIN, OUTPUT);
	pinMode(RS_74HC595_LATCHPIN, OUTPUT);
	pinMode(RS_74HC595_CLOCKPIN, OUTPUT);
	RS_74HC595_SetChar(' ');
}

void setup()
{
	FlowSerialBegin(19200);
#ifdef INCLUDE_TM1638
	// TM1638 INIT
	for (i = 0; i < TM1638_ENABLEDMODULES; i++)
	{
		TM1638_screens[i]->Screen->setupDisplay(true, 7);
		TM1638_screens[i]->Screen->clearDisplay();
	}
#endif

	// NOKIALCD INIT
#ifdef INCLUDE_NOKIALCD
	if (ENABLED_NOKIALCD) {
		nokiaLCD.begin();
		nokiaLCD.clearDisplay();
		nokiaLCD.setRotation(2);
		//nokiaLCD.setFont(&DJB_Get_Digital15pt7b);
		nokiaLCD.setContrast(45);
		nokiaLCD.setCursor(0, 20);
		//nokiaLCD.setFont(&FONT_1);
		nokiaLCD.setTextSize(1);

		nokiaLCD.print("123");

		nokiaLCD.display();
		nokiaLCD.setCursor(0, 20);

#ifdef ENABLE_SECONDARY_NOKIALCD
		nokiaLCD2.begin();
		nokiaLCD2.setRotation(2);
		nokiaLCD2.setContrast(54);
		nokiaLCD2.clearDisplay();
		nokiaLCD2.print("2 345");
		nokiaLCD2.display();
#endif
	}
#endif

	// TM1637 INIT
#ifdef INCLUDE_TM1637
	for (int i = 0; i < TM1637_ENABLEDMODULES; i++) {
		TM1637_screens[i]->init();
		TM1637_screens[i]->set(BRIGHT_TYPICAL);
		TM1637_screens[i]->clearDisplay();
	}
#endif
#ifdef INCLUDE_WS2812B
	// WS2812B INIT
	if (WS2812B_RGBLEDCOUNT > 0) {
		WS2812B_strip.begin();
		WS2812B_strip.show();
	}
#endif

	// WS2801 INIT
#ifdef INCLUDE_WS2801
	if (WS2801_RGBLEDCOUNT > 0) {
		WS2801_strip.begin();
		WS2801_strip.show();
	}
#endif

	// MAX7221 7SEG INIT
	for (i = 0; i < MAX7221_ENABLEDMODULES; i++) {
		MAX7221.shutdown(i, false);
		MAX7221.setIntensity(i, 15);
		MAX7221.clearDisplay(i);
	}

	// MAX7221 MATRIX INIT
	for (i = 0; i < MAX7221_MATRIX_ENABLED; i++) {
		MAX7221_MATRIX.shutdown(i, false);
		MAX7221_MATRIX.setIntensity(i, 15);
		MAX7221_MATRIX.clearDisplay(i);
	}

	// 74HC595 INIT
	if (ENABLE_74HC595_GEAR_DISPLAY == 1)
	{
		RS_74HC595_Init();
	}

	// LCD INIT
#ifdef INCLUDE_I2CLCD
	if (I2CLCD_enabled == 1) {
		I2CLCD.init();
		I2CLCD.backlight();
		I2CLCD.print("Hello world...");
		I2CLCD.clear();
	}
#endif

#ifdef INCLUDE_LEDBACKPACK
	// ADA_HT16K33_7SEGMENTS INIT
	if (ENABLE_ADA_HT16K33_7SEGMENTS == 1) {
		ADA_HT16K33_7SEGMENTS.begin(ADA_HT16K33_7SEGMENTS_I2CADDRESS);
	}

	// ADA_HT16K33_MATRIX INIT
	if (ENABLE_ADA_HT16K33_Matrix == 1) {
		ADA_HT16K33_MATRIX.begin(ADA_HT16K33_MATRIX_I2CADDRESS);
	}
#endif

	// SERVO INIT
	if (ENABLE_SERVO) {
		myservo.attach(SERVO_PIN);
		myservo.write(0);
		myservo.write(180);
	}

	// EXTERNAL BUTTONS INIT
	for (int btnIdx = 0; btnIdx < ENABLED_BUTTONS_COUNT; btnIdx++) {
		pinMode(BUTTON_PINS[btnIdx], INPUT_PULLUP);
	}
}

#ifdef INCLUDE_TM1638
void TM1638_SetDisplayFromSerial(TM1638 * screen)
{
	for (i = 0; i < 8; i++) {
		displayValues[i] = FlowSerialRead();
	}
	screen->setDisplay(displayValues);

	// Leds

	for (i = 0; i < 8; i++) {
		char state = (char)FlowSerialRead();

		// Swap led colors if requested
		if (TM1638_SWAPLEDCOLORS == 1) {
			if (state == 'G')
			{
				state = 'R';
			}
			else if (state == 'R')
			{
				state = 'G';
			}
		}

		if (state == 'G') {
			screen->setLED(TM1638_COLOR_GREEN, i);
		}
		else if (state == 'R') {
			screen->setLED(TM1638_COLOR_RED, i);
		}
		else {
			screen->setLED(TM1638_COLOR_NONE, i);
		}
	}
}
#endif

void MAX7221_SetDisplayFromSerial(int idx)
{
	while (FlowSerialAvailable() < 8) {
	}
	// Digits
	for (i = 0; i < 8; i++) {
		while (FlowSerialAvailable() < 1) {
		}
		displayValues[i] = MAX7221_ByteReorder((char)FlowSerialRead());
	}

	for (i = 0; i < 8; i++) {
		MAX7221.setRow(idx, 7 - i, displayValues[i]);
	}
}

#ifdef INCLUDE_LEDBACKPACK
void ADA7SEG_SetDisplayFromSerial(int idx)
{
	// Digits
	for (i = 0; i < 8; i++) {
		displayValues[i] = (char)FlowSerialRead();
	}

	for (i = 0; i < 2; i++) {
		ADA_HT16K33_7SEGMENTS.writeDigitRaw(i, displayValues[i]);
	}

	ADA_HT16K33_7SEGMENTS.writeDigitRaw(2, 0);

	for (i = 2; i < 4; i++) {
		ADA_HT16K33_7SEGMENTS.writeDigitRaw(i + 1, displayValues[i]);
	}

	ADA_HT16K33_7SEGMENTS.writeDisplay();
}
#endif

void sendButtonState() {
	bool sendButtons = false;
#ifdef INCLUDE_TM1638
	for (i = 0; i < TM1638_ENABLEDMODULES; i++) {
		TM1638_screens[i]->Buttons = TM1638_screens[i]->Screen->getButtons();
		if (TM1638_screens[i]->Buttons != TM1638_screens[i]->Oldbuttons) {
			sendButtons = true;
		}
		TM1638_screens[i]->Oldbuttons = TM1638_screens[i]->Buttons;
	}
#endif

	//if (sendButtons)
	{
#ifdef INCLUDE_TM1638
		for (i = 0; i < TM1638_ENABLEDMODULES; i++) {
			FlowSerialWrite(TM1638_screens[i]->Buttons);
		}
#endif

		for (i = 0; i < ENABLED_BUTTONS_COUNT; i++) {
			FlowSerialWrite(digitalRead(BUTTON_PINS[i]) == HIGH ? 0 : 1);
		}

		FlowSerialFlush();
	}
}

void MAX7221_MATRIX_Read() {
	while (FlowSerialAvailable() < 1) {
	}

	int luminosity = FlowSerialRead();



	if (MAX7221_MATRIX_LUMINOSITY != luminosity) {
		MAX7221_MATRIX.setIntensity(0, luminosity);
		MAX7221_MATRIX_LUMINOSITY = luminosity;
	}

	while (FlowSerialAvailable() < 8) {
	}

	for (int j = 0; j < 8; j++) {
		MAX7221_MATRIX.setRow(0, 7 - j, FlowSerialRead());
	}
}

#ifdef INCLUDE_LEDBACKPACK
void ADA_HT16K33_Matrix_Read() {
	while (FlowSerialAvailable() < 9) {
	}

	int luminosity = FlowSerialRead();

	if (ADA_HT16K33_Matrix_luminosity != luminosity) {
		ADA_HT16K33_MATRIX.setBrightness(luminosity);
		ADA_HT16K33_Matrix_luminosity = luminosity;
	}

	for (int j = 0; j < 8; j++) {
		byte row = FlowSerialRead();
		for (int c = 0; c < 8; c++) {
			ADA_HT16K33_MATRIX.drawPixel(j, c, ((row & (2 ^ c)) > 0) ? LED_GREEN : LED_OFF);
		}
	}

	ADA_HT16K33_MATRIX.writeDisplay();
}
#endif

#ifdef INCLUDE_TM1637
void TM1637_SetDisplayFromSerial(TM1637 * screen)
{
	for (i = 0; i < 4; i++) {
		screen->rawDisplay(i, FlowSerialRead());
	}

	// Skip 4 remaining chars
	for (i = 0; i < 4; i++) {
		FlowSerialRead();
	}
}
#endif
char opt;
int row;

#define MESSAGE_HEADER 0x03

uint8_t header = 0;
uint8_t currentRow = -1;
int16_t v[4 * 2];

#ifdef INCLUDE_NOKIALCD
uint16_t color;
int16_t w;
int16_t h;
int16_t r;
uint8_t align = 0;
boolean wrap = 0;

int16_t boundX;
int16_t boundY;
uint16_t boundW;
uint16_t boundH;

uint8_t nokiaIndex;
int fontType;
Adafruit_PCD8544* currentNokia;
#endif

void loop() {
	if (ENABLE_SERVO == 1) {
		if (targetValue != currentValue) {
			if (currentValue < targetValue && updateTime + upDelay <= millis()) {
				currentValue += 1;
				if (currentValue > targetValue) {
					currentValue = targetValue;
				}
				myservo.writeMicroseconds((544 + currentValue));
				updateTime = millis();
			}
			else if (currentValue > targetValue && updateTime + downDelay <= millis()) {
				currentValue -= 1;
				if (currentValue < targetValue) {
					currentValue = targetValue;
				}
				myservo.writeMicroseconds((544 + currentValue));
				updateTime = millis();
			}
		}
	}

	// Wait for data
	if (FlowSerialAvailable() >= 2) {
		// Reader header
		header = FlowSerialRead();
		if (header == 0x03)
		{
			// Read command
			opt = FlowSerialRead();

			// Hello command
			if (opt == '1') {
				while (FlowSerialAvailable() < 1) {
				}
				FlowSerialTrigger = FlowSerialRead();
				delay(10);
				FlowSerialPrint('f');
				FlowSerialFlush();
			}

			// Features command
			if (opt == '0') {
				delay(10);

				// Matrix
				if (MAX7221_MATRIX_ENABLED == 1 || ENABLE_ADA_HT16K33_Matrix == 1) {
					FlowSerialPrint("M");
				}

				// LCD
				if (I2CLCD_enabled == 1) {
#ifdef INCLUDE_I2CLCD
					FlowSerialPrint("L");
#endif
				}
#ifdef INCLUDE_NOKIALCD
				if (ENABLED_NOKIALCD > 0) {
					FlowSerialPrint("K");
				}
#endif
				// SERVO
				if (ENABLE_SERVO) {
					FlowSerialPrint("H");
				}

				// Gear
				FlowSerialPrint("G");

				// Name
				FlowSerialPrint("N");

				// Additional buttons
				FlowSerialPrint("J");

				FlowSerialPrint("\n");
				FlowSerialFlush();
			}

			if (opt == 'J') {
				FlowSerialWrite((byte)(ENABLED_BUTTONS_COUNT));
				FlowSerialFlush();
			}

			if (opt == 'H') {
				while (FlowSerialAvailable() < 4) {
				}
				int percent = FlowSerialRead();
				percent = percent * 255;
				percent += FlowSerialRead();
				targetValue = 2400 - 544 - percent;
				upDelay = FlowSerialRead();
				downDelay = FlowSerialRead();
			}

			//  Module count command
			if (opt == '2') {
				FlowSerialWrite((byte)(TM1638_ENABLEDMODULES));
				FlowSerialFlush();
			}

			//  SIMPLE Module count command
			if (opt == 'B') {
				FlowSerialWrite((byte)(MAX7221_ENABLEDMODULES + TM1637_ENABLEDMODULES + ENABLE_ADA_HT16K33_7SEGMENTS));
				FlowSerialFlush();
			}

			// Write 7seg data
			if (opt == '3') {
#ifdef INCLUDE_TM1638
				// TM1638
				for (int j = 0; j < TM1638_ENABLEDMODULES; j++) {
					while (FlowSerialAvailable() < 17) {
					}

					// Wait for display data
					int newIntensity = FlowSerialRead();
					if (newIntensity != TM1638_screens[j]->Intensity) {
						TM1638_screens[j]->Screen->setupDisplay(true, newIntensity);
						TM1638_screens[j]->Intensity = newIntensity;
					}

					TM1638_SetDisplayFromSerial(TM1638_screens[j]->Screen);
				}
#endif
			}
			if (opt == 'S') {
#ifdef INCLUDE_TM1637
				// TM1637
				for (int j = 0; j < TM1637_ENABLEDMODULES; j++) {
					while (FlowSerialAvailable() < 9) {
					}
					// Intensity
					TM1637_screens[j]->set(FlowSerialRead());
					TM1637_SetDisplayFromSerial(TM1637_screens[j]);
				}
#endif
				// MAX7221
				for (int j = 0; j < MAX7221_ENABLEDMODULES; j++) {
					while (FlowSerialAvailable() < 1) {
					}
					// Wait for display data
					int newIntensity = FlowSerialRead();
					if (newIntensity != MAX7221_screens[j]->Intensity) {
						MAX7221.setIntensity(j, newIntensity * 2 + 1);
						MAX7221_screens[j]->Intensity = newIntensity;
					}

					MAX7221_SetDisplayFromSerial(j);
				}

#ifdef INCLUDE_LEDBACKPACK
				// Simple ADA display
				for (int j = 0; j < ENABLE_ADA_HT16K33_7SEGMENTS; j++) {
					while (FlowSerialAvailable() < 9) {
					}
					int newIntensity = FlowSerialRead();
					ADA_HT16K33_7SEGMENTS.setBrightness(newIntensity * 2 + 1);

					ADA7SEG_SetDisplayFromSerial(j);
				}
#endif
			}

			//  RGBLED count command
			if (opt == '4') {
				FlowSerialWrite((byte)(WS2812B_RGBLEDCOUNT + WS2801_RGBLEDCOUNT));
				FlowSerialFlush();
			}

			// Write RGB LEDS data
			if (opt == '6') {
#ifdef INCLUDE_WS2812B
				for (uint8_t j = 0; j < WS2812B_RGBLEDCOUNT; j++) {
					while (FlowSerialAvailable() < 3) {
					}
					uint8_t r = FlowSerialRead();
					uint8_t g = FlowSerialRead();
					uint8_t b = FlowSerialRead();
					if (WS2812B_RIGHTTOLEFT == 1) {
						WS2812B_strip.setPixelColor(WS2812B_RGBLEDCOUNT - j - 1, r, g, b);
					}
					else {
						WS2812B_strip.setPixelColor(j, r, g, b);
					}
				}
				if (WS2812B_RGBLEDCOUNT > 0) {
					WS2812B_strip.show();
				}

#endif

#ifdef INCLUDE_WS2801
				for (uint8_t j = 0; j < WS2801_RGBLEDCOUNT; j++) {
					while (FlowSerialAvailable() < 3) {
					}
					uint8_t r = FlowSerialRead();
					uint8_t g = FlowSerialRead();
					uint8_t b = FlowSerialRead();
					if (WS2801_RIGHTTOLEFT == 1) {
						WS2801_strip.setPixelColor(WS2801_RGBLEDCOUNT - j - 1, r, g, b);
					}
					else
					{
						WS2801_strip.setPixelColor(j, r, g, b);
					}
				}
				if (WS2801_RGBLEDCOUNT > 0) {
					WS2801_strip.show();
				}
#endif
				// Acq !
				FlowSerialWrite(0x15);
			}

			// Set baudrate
			if (opt == '8') {
				while (FlowSerialAvailable() < 1) {
				}
				int br = FlowSerialRead();

				delay(200);
				//UCSR0A |= _BV(U2X0);

				if (br == 1) FlowSerialBegin(300);
				if (br == 2) FlowSerialBegin(1200);
				if (br == 3) FlowSerialBegin(2400);
				if (br == 4) FlowSerialBegin(4800);
				if (br == 5) FlowSerialBegin(9600);
				if (br == 6) FlowSerialBegin(14400);
				if (br == 7) FlowSerialBegin(19200);
				if (br == 8) FlowSerialBegin(28800);
				if (br == 9) FlowSerialBegin(38400);
				if (br == 10) FlowSerialBegin(57600);
				if (br == 11) FlowSerialBegin(115200);
				if (br == 12) FlowSerialBegin(230400);
				if (br == 13) FlowSerialBegin(250000);
				if (br == 14) FlowSerialBegin(1000000);
				if (br == 15) FlowSerialBegin(2000000);

				if (br == 16) FlowSerialBegin(200000);
			}

			// MAX7221 MATRIX
			if (opt == 'M') {
				if (MAX7221_MATRIX_ENABLED == 1) {
					MAX7221_MATRIX_Read();
				}
#ifdef INCLUDE_LEDBACKPACK
				if (ENABLE_ADA_HT16K33_Matrix == 1) {
					ADA_HT16K33_Matrix_Read();
				}
#endif
			}

			// ACQ Packet
			if (opt == 'A') {
				FlowSerialWrite(0x03);
				FlowSerialFlush();
			}

			if (opt == 'C') {
				sendButtonState();
				FlowSerialFlush();
			}

			// Device Name
			if (opt == 'N') {
				FlowSerialPrint(DEVICE_NAME);
				FlowSerialPrint("\n");
				FlowSerialFlush();
			}

			// -------------- CUSTOM PROTOCOL PART -----------------
			// GEAR
			if (opt == 'G') {
				while (FlowSerialAvailable() < 1) {
				}
				char gear = FlowSerialRead();
				/// DEBUG !!!!
				// screens[0]->Screen->setDisplayToString(String(gear));
				//
				if (ENABLE_74HC595_GEAR_DISPLAY == 1) {
					RS_74HC595_SetChar(gear);
				}
				//
				// Do what you want with current gear here ;)
				//
			}

			if (opt == 'L') {
#ifdef INCLUDE_I2CLCD
				while (FlowSerialAvailable() < 2)
				{
				}
				if (FlowSerialRead() > 0) {
					//I2CLCD.backlight();
				}
				else {
					//I2CLCD.noBacklight();
				}
				row = FlowSerialRead();

				//if (currentRow != row)
				{
					I2CLCD.setCursor(0, row);
				}

				/*for (i = 0; i < I2CLCD_width; i++)
				{
					while (FlowSerialAvailable() < 1)
					{
					}
					I2CLCD.write((char)FlowSerialRead());
				}*/
				I2CLCD.print(FlowSerialReadStringUntil('\n'));
				currentRow = (row + 1) % I2CLCD_height;
#endif
			}

			// -------------- CUSTOM PROTOCOL PART -----------------

			//sendButtonState();

			// Nokia
			if (opt == 'K')
			{
#ifdef INCLUDE_NOKIALCD

				while (FlowSerialAvailable() < 2)
				{
				}

				nokiaIndex = FlowSerialRead();
				action = FlowSerialRead();
				currentNokia = nokiaLCDs[nokiaIndex];

				if (action == 'C')
				{
					currentNokia->clearDisplay();
				}

				if (action == 'N')
				{
					FlowSerialWrite((byte)ENABLED_NOKIALCD);
					FlowSerialFlush();
				}

				if (action == 'I')
				{
					while (FlowSerialAvailable() < 1)
					{
					}
					currentNokia->setContrast(FlowSerialRead());
				}

				else if (action == 'D')
				{
					currentNokia->display();
				}
				else if (action == 'P')
				{
					while (FlowSerialAvailable() < 8)
					{
					}

					fontSize = (uint8_t)FlowSerialRead();
					fontType = (uint8_t)FlowSerialRead();
					posX = (int16_t)FlowSerialRead();
					posY = (int16_t)FlowSerialRead();
					color = FlowSerialRead();
					wrap = FlowSerialRead() > 0;
					align = FlowSerialRead();

					currentNokia->setFont();

					if (fontType == 0) {
					}
#ifdef CUSTOM_LCD_FONT_1
					else if (fontType == 1) {
						currentNokia->setFont(&CUSTOM_LCD_FONT_1);
					}
#endif
#ifdef CUSTOM_LCD_FONT_2
					else if (fontType == 2) {
						currentNokia->setFont(&CUSTOM_LCD_FONT_2);
					}
#endif
#ifdef CUSTOM_LCD_FONT_3
					else if (fontType == 3) {
						currentNokia->setFont(&CUSTOM_LCD_FONT_3);
					}
#endif


					currentNokia->setTextWrap(wrap);
					currentNokia->setTextSize(fontSize);
					currentNokia->setTextColor(color);


					String content = FlowSerialReadStringUntil('\n');
					String content2 = content + "\n ";
					if (align == 2)
					{
						currentNokia->getTextBounds((char*)content2.c_str(), 0, 0, &boundX, &boundY, &boundW, &boundH);
						posX = posX - (boundW / 2);
					}
					else if (align == 3)
					{
						currentNokia->getTextBounds((char*)content2.c_str(), 0, 0, &boundX, &boundY, &boundW, &boundH);
						posX = posX - boundW;
					}

					currentNokia->setCursor(posX, posY);
					currentNokia->print(content);
				}
				else if (action == 'L') {
					while (FlowSerialAvailable() < 5)
					{
					}
					posX = (int16_t)FlowSerialRead();
					posY = (int16_t)FlowSerialRead();
					w = (int16_t)FlowSerialRead(); // x2
					h = (int16_t)FlowSerialRead(); // y2
					color = FlowSerialRead();

					currentNokia->drawLine(posX, posY, w, h, color);
				}
				else if (action == 'T') {
					while (FlowSerialAvailable() < 6)
					{
					}
					posX = (int16_t)FlowSerialRead();
					posY = (int16_t)FlowSerialRead();
					w = (int16_t)FlowSerialRead(); // x2
					h = (int16_t)FlowSerialRead(); // y2
					int thickness = FlowSerialRead();
					color = FlowSerialRead();

					float dx = posX - w;
					float dy = posY - h;
					float d = sqrtf(dx * dx + dy * dy);
					if (!d)
						return;

					int v[4 * 2];

					/* left up */
					v[0] = posX - thickness * dy / d;
					v[1] = posY + thickness * dx / d;
					/* right up */
					v[2] = posX + thickness * dy / d;
					v[3] = posY - thickness * dx / d;
					/* right down */
					v[4] = w + thickness * dy / d;
					v[5] = h - thickness * dx / d;
					/* left down */
					v[6] = w - thickness * dy / d;
					v[7] = h + thickness * dx / d;

					currentNokia->fillTriangle(v[0], v[1], v[6], v[7], v[2], v[3], color);
					currentNokia->fillTriangle(v[2], v[3], v[4], v[5], v[6], v[7], color);
				}

				else if (action == 'F' || action == 'R')
				{
					while (FlowSerialAvailable() < 6)
					{
					}

					posX = (int16_t)FlowSerialRead();
					posY = (int16_t)FlowSerialRead();
					w = (int16_t)FlowSerialRead();
					h = (int16_t)FlowSerialRead();
					r = (int16_t)FlowSerialRead();
					color = FlowSerialRead();

					if (r == 0) {
						if (action == 'F')
							currentNokia->fillRect(posX, posY, w, h, color);
						else
							currentNokia->drawRect(posX, posY, w, h, color);
					}
					else {
						if (action == 'F')
							currentNokia->fillRoundRect(posX, posY, w, h, r, color);
						else
							currentNokia->drawRoundRect(posX, posY, w, h, r, color);
					}
				}

#endif
			}
		}
	}
}
