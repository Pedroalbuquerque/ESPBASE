# ESPBASE lib
library to build ESP8266/ESP32 projects using OTA and Web config

This lib was created to simplify the creation of any new project from scretch but starting with many Wifi and Web functionalities already available to be used in code with minimum configuration effort.

Some basic functionalities are provided:
* OTA (Over The Air) programing or wireless upload of code as an alternative to Serial upload
* EEPROM saved configuration to allow the board to connect to your access point when booting
* Web configuration page to allow changing the EEPROM saved configuration
* AP (Access Point) boot when connection to your access point does not suceed
* NTP to allow you to have a variable with current date and time loaded from internet

## How to use it

Download all the files from this folder to a folder on your <projects>/libraries folder .

For a brand new board/sketch you have to build start from the example/WiFi_CFG_OTA_Telnet/WiFi_CFG_OTA_Telnet.ino and load this sketch using a serial port upload via an FTDI ou other UBS to Serial interface.
You should copy the example/WiFi_CFG_OTA_Telnet to your own projects/<myProj> folder and rename it accordingly.

Once loaded, the skecth will try to load configuration from EEPROM(flash) (nothing will be saved when loaded for the first time), and if no configuration is found, board will enter in AP mode. 
In this mode a new AP will be available for you to connect your PC or Mobile phone, with the name "ESP8266-xxxx" or "ESP32-xxxx" and no password.
The board will be assigned the IP:192.168.4.1.
Once you connect your PC or phone to this AP("ESP8266-xxxx" or "ESP32-xxxx"), you can open a browser and try to access the adress 192.168.4.1, that will show you the main configuration page
Choose "Network Configuratin button" to define SSID and Password so the board can connect to your home Access point(router) and use the "Save" button to save it to the EEPROM.
Board will reset and connect to your router aquiring a new IP adress on your lan network.
On your Serial Monitor you can see what is the board new IP adress assigned by your router and you can now connect to the board via this new IP. 
To do so now you have to reconnect your PC or phone to your home router.

You can test it by using a "ping" command from a comamnd line on your PC or Terminal if on a MAC.

If you succeed, a new port shoud show up (may take a 1 to 3 minutes to show up) in your Arduino IDE environment to allow to OTA your next sketch to the board.

## How to write your own code

Assuming you are familiar with the Arduino framework, you have the same two basic function setup() and loop(), that already have some basid code to implement this basic fucntionality of the template.
you can find in this to function a place holder like
    //**** Normal Sketch code here...
      
      write you own setup code after this comment
      
You can naturaly include your libraries in the skecth as necessary for your specific code.

## Dependencies

### ESP8266
To used this libray with ESP8266 not adional libraries are needed (that I can remember, as i have created it for sime time now).

### ESP32
To use with ESP32 this is a bit more demanding to prepare the enviroment.
Assuming ESP 32 environment has already been installed you will need:
- ESP32WebServer from Ivan Grokhtkov <ivan@ESP32.com>
- Ticker (from my repository) (se instrution for special installation




Hope you can find this interesting and usefull.

Pedro Albuquerque
