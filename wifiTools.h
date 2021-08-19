#ifndef WIFITOOLS_H
#define WIFITOOLS_H


#if defined(ESP32)
  ESP32WebServer server(80);							// The Webserver
#elif defined(ESP8266) //ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
  ESP8266WebServer server(80);							// The Webserver
#else
  #error NO Board defined
#endif

/*
**
** CONFIGURATION HANDLING
**
*/
void ConfigureWifi(){
  ECHO_MSG("Configuring Wifi\n");
  //WiFi.mode(WIFI_OFF);
  WiFi.begin (config.ssid, config.password);

  while (WiFi.status() != WL_CONNECTED) {
    ECHO_MSG("WiFi not connected\n");
    delay(500);
  }
  if (!config.dhcp)
  {
    WiFi.config(IPAddress(config.IP[0], config.IP[1], config.IP[2], config.IP[3] ),  IPAddress(config.Gateway[0], config.Gateway[1], config.Gateway[2], config.Gateway[3] ) , IPAddress(config.Netmask[0], config.Netmask[1], config.Netmask[2], config.Netmask[3] ));
  }
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

  #if defined(ESP32)
    uint64_t chipId;
    chipId = ESP.getEfuseMac();
    id = (uint16_t)(chipId>>32);
  #elif defined(ESP8266)
    id = ESP.getChipId();
  #else
    #error NO Board defined
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

String urldecode(String input){ // (based on https://code.google.com/p/avr-netino/)

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





#endif
