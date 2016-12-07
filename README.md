# ESP8266-DisplayClientV2
SimHub DisplayClientV2 for ESP8266, ESP-WROOM-02

#Hardware
- Generic ESP8266 Module
 - ESP-WROOM-02 (http://espressif.com/en/products/hardware/esp-wroom-02/overview)
- TM1638 Key Display Module
 - (http://www.dx.com/p/8-bit-digital-led-tube-8-bit-tm1638-key-display-module-for-arduino-blue-399381)
 - (http://www.dx.com/p/8x-digital-tube-8x-key-8x-double-color-led-module-81873)

#Pin Descriptions
|TM1638||ESP8266|
|---|---|---|
|VCC|5V|-|
|GND|GND|-|
|DIO|DATA|IO4|
|CLK|CLOCK|IO14|
|STB0|STROBE|IO5|

#Library
 - ESP8266WiFi 1.0 (https://github.com/esp8266/Arduino/tree/master/libraries/ESP8266WiFi)
 - Adafruit_GFX_Library 1.1.5 (https://github.com/adafruit/Adafruit-GFX-Library)
 - tm1638-library 2.2.0 (https://github.com/rjbatista/tm1638-library)
 - SimHub 4.0.5 (http://www.racedepartment.com/downloads/simhub-diy-sim-racing-dash.10252/)
 - EEPROM 1.0
 - Servo 1.0.2
 - SPI 1.0
 - Wire 1.0
