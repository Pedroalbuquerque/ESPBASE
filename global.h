#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef ARDUINO_ESP32_DEV
  ESP32WebServer server(80);							// The Webserver
#elif ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
  ESP8266WebServer server(80);							// The Webserver
#endif

boolean firstStart = true;								// On firststart = true, NTP will try to get a valid time
int AdminTimeOutCounter = 0;							// Counter for Disabling the AdminMode
volatile unsigned long UnixTimestamp = 0;	// GLOBALTIME  ( Will be set by NTP)
int cNTP_Update = 0;											// Counter for Updating the time via NTP
Ticker tkSecond;												  // Second - Timer for Updating Datetime Structure

//custom declarations
long absoluteActualTime, actualTime;
long  customWatchdog;                     // WatchDog to detect main loop blocking. There is a builtin WatchDog to the chip firmare not related to this one


struct strConfig {
  boolean dhcp;                         // 1 Byte - EEPROM 16
  boolean isDayLightSaving;             // 1 Byte - EEPROM 17
  long Update_Time_Via_NTP_Every;       // 4 Byte - EEPROM 18
  long timeZone;                        // 4 Byte - EEPROM 22
  byte  IP[4];                          // 4 Byte - EEPROM 32
  byte  Netmask[4];                     // 4 Byte - EEPROM 36
  byte  Gateway[4];                     // 4 Byte - EEPROM 40
  String ssid;                          // up to 32 Byte - EEPROM 64
  String password;                      // up to 32 Byte - EEPROM 96
  String ntpServerName;                 // up to 32 Byte - EEPROM 128
  String DeviceName;                    // up to 32 Byte - EEPROM 160
  String OTApwd;                         // up to 32 Byte - EEPROM 192

  // Application Settings here... from EEPROM 224 up to 511 (0 - 511)

} config;


/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureWifi(){
  Serial.println("Configuring Wifi");
  //WiFi.mode(WIFI_OFF);
  WiFi.begin (config.ssid.c_str(), config.password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi not connected");
    delay(500);
  }
  if (!config.dhcp)
  {
    WiFi.config(IPAddress(config.IP[0], config.IP[1], config.IP[2], config.IP[3] ),  IPAddress(config.Gateway[0], config.Gateway[1], config.Gateway[2], config.Gateway[3] ) , IPAddress(config.Netmask[0], config.Netmask[1], config.Netmask[2], config.Netmask[3] ));
  }
}

void printConfig(){

  Serial.println("Printing Config");

  Serial.printf("DHCP:%d\n", config.dhcp);
  Serial.printf("DayLight:%d\n", config.isDayLightSaving);

  Serial.printf("NTP update every %ld sec\n", config.Update_Time_Via_NTP_Every); // 4 Byte
  Serial.printf("Timezone %ld\n", config.timeZone); // 4 Byte

  Serial.printf("IP:%d.%d.%d.%d\n", config.IP[0],config.IP[1],config.IP[2],config.IP[3]);
  Serial.printf("Mask:%d.%d.%d.%d\n", config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3]);
  Serial.printf("Gateway:%d.%d.%d.%d\n", config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3]);


  Serial.printf("SSID:%s\n", config.ssid.c_str());
  Serial.printf("PWD:%s\n", config.password.c_str());
  Serial.printf("ntp ServerName:%s\n", config.ntpServerName.c_str());
  Serial.printf("Device Name:%s\n", config.DeviceName.c_str());
  Serial.printf("OTA password:%s\n", config.OTApwd.c_str());

    // Application Settings here... from EEPROM 192 up to 511 (0 - 511)

}

String GetMacAddress(){
  uint8_t mac[6];
  char macStr[18] = {0};
  WiFi.macAddress(mac);
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
  return  String(macStr);
}

String GetAPMacAddress(){
  uint8_t mac[6];
  char macStr[18] = {0};
  WiFi.softAPmacAddress(mac);
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0],  mac[1], mac[2], mac[3], mac[4], mac[5]);
  return  String(macStr);
}

uint16_t getChipId(){
  uint16_t id;
  uint64_t chipId;

#ifdef ARDUINO_ESP32_DEV
  chipId = ESP.getEfuseMac();
  id = (uint16_t)(chipId>>32);
#elif ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
  id = ESP.getChipId();

#endif
  return id;

}

// convert a single hex digit character to its integer value (from https://code.google.com/p/avr-netino/)
unsigned char h2int(char c){
  if (c >= '0' && c <= '9') {
    return ((unsigned char)c - '0');
  }
  if (c >= 'a' && c <= 'f') {
    return ((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <= 'F') {
    return ((unsigned char)c - 'A' + 10);
  }
  return (0);
}

String urldecode(String input) // (based on https://code.google.com/p/avr-netino/)
{
  char c;
  String ret = "";

  for (byte t = 0; t < input.length(); t++)
  {
    c = input[t];
    if (c == '+') c = ' ';
    if (c == '%') {


      t++;
      c = input[t];
      t++;
      c = (h2int(c) << 4) | h2int(input[t]);
    }

    ret.concat(c);
  }
  return ret;

}

// Check the Values is between 0-255
boolean checkRange(String Value){
  if (Value.toInt() < 0 || Value.toInt() > 255)
  {
    return false;
  }
  else
  {
    return true;
  }
}
#endif
