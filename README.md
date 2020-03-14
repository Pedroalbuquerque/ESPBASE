# ESPBASE lib
library to build ESP8266/ESP32 projects using OTA and Web config

This lib was created to simplify the creation of any new project from scratch but starting with many Wifi and Web functionalities already available to be used in code with minimum configuration effort.

Some basic functionalities are provided:
* OTA (Over The Air) programming or wireless upload of code as an alternative to Serial upload
* EEPROM saved configuration to allow the board to connect to your access point when booting
* Web configuration page to allow changing the EEPROM saved configuration
* AP (Access Point) boot when connection to your access point does not succeed
* NTP to allow you to have a variable with current date and time loaded from internet

## How to use it

Download all the files from this folder to a folder on your <projects>/libraries folder .

For a brand new board/sketch you have to start from the example/WiFi_CFG_OTA_Telnet/WiFi_CFG_OTA_Telnet.ino and load this sketch using a serial port upload via an FTDI ou USB interface.
You should copy the example/WiFi_CFG_OTA_Telnet to your own projects/<myProj> folder and rename it accordingly.

Once loaded, the sketch will try to load configuration from EEPROM(flash) (nothing will be saved when loaded for the first time), and if no configuration is found, board will enter in AP mode.
In this mode a new AP will be available for you to connect your PC or Mobile phone, with the name "ESP8266-xxxx" or "ESP32-xxxx" and no password.
The board will be assigned the IP:192.168.4.1.
Once you connect your PC or phone to this AP("ESP8266-xxxx" or "ESP32-xxxx"), you can open a browser and try to access the IP address 192.168.4.1, that will show you the main configuration page
Choose "Network Configuration button" to define SSID and Password so the board can connect to your home Access point(router) and use the "Save" button to save it to the EEPROM.
Board will reset and connect to your router acquiring a new IP address on your lan network.
On your Serial Monitor you can see what is the board new IP address assigned by your router and you can now connect to the board via this new IP.
To do so now you have to reconnect your PC or phone to your home router.

You can test it by using a "ping" command from a command line on your PC or Terminal if on a MAC.

If you succeed, a new port should show up (may take a 1 to 3 minutes to show up) in your Arduino IDE environment to allow to OTA your next sketch to the board.

## How to write your own code

Assuming you are familiar with the Arduino framework, you have the same two basic function setup() and loop(), that already have some basic code to implement this basic functionality of the template.
you can find in this to function a place holder like

    //**** Normal Sketch code here...

      write you own setup code after this comment

You can naturally include your libraries in the sketch as necessary for your specific code.

## External Dependencies (not builtin to framework)
External dependencies are a set of libraries that need to be installed (if you use Arduino IDE), or referenced in platformio.ini in case you use Platformio

### ESP8266
To used this library with ESP8266 not additional libraries are needed (that I can remember, as i have created it for some time now).

### ESP32
To use with ESP32 this is a bit more demanding to prepare the environment.
Assuming ESP 32 environment has already been installed you will need:
- ESP32WebServer from Ivan Grokhtkov <ivan@ESP32.com> or https://github.com/Pedroalbuquerque/ESP32WebServer
- Ticker-ESP32 (https://github.com/Pedroalbuquerque/Ticker-ESP32)
- ESP32PWM (https://github.com/Pedroalbuquerque/ESP32PWM)

## EEPROM Use
This framework assumes the use o EEPROM to store base configuration .
for ESP8266 the EEPROM size is predefined to 512Bytes and on ESP32 this library declare EEPROM with 512Byte size to keep compatibility t ESP8266

### ESP8266
on ESP8266 the EEPROM is accessed via memory address and EEPROM mapping is:
0         "C" // C character
1         "F" // F character
2         "G" // G character
3-15      (not used)
16-223    config parameters
224-511   (free)

### ESP32
on ESP 32 EEPROM is access via keyword instead for address so EEPROM mapping is:
3 bytes for "CFG" string
208 bytes used by config parameters
301 bytes free

Hope you can find this interesting and useful.

Pedro Albuquerque
