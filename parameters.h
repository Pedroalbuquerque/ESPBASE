 
#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <PACharTools.h>
#include <PAcrc32.h>
#if defined(ESP32) // ARDUINO_ESP32_DEV
  #include <Preferences.h>
  Preferences EEPROM;
#endif

extern uint16_t getChipId();

struct strConfig {
  uint32_t crc=0;                         // 32 Byte
  boolean dhcp;                         // 1 Byte - EEPROM 16  // from 0-15 reserved, so start on 16
  boolean isDayLightSaving;             // 1 Byte - EEPROM 17
  long Update_Time_Via_NTP_Every;       // 4 Byte - EEPROM 18
  long timeZone;                        // 4 Byte - EEPROM 22
  byte  IP[4];                          // 4 Byte - EEPROM 26 //32 // error addr jump ??
  byte  Netmask[4];                     // 4 Byte - EEPROM 30 //36
  byte  Gateway[4];                     // 4 Byte - EEPROM 34 //40
  char ssid[16]="";                          // up to 16 Byte - EEPROM 38 //64
  char password[16]="";                      // up to 16 Byte - EEPROM 70 //96
  uint8_t connectToWifi = false;             // 1 byte
  char ntpServerName[32]="";                 // up to 32 Byte - EEPROM 102 //128
  char DeviceName[16]="";                    // up to 16 Byte - EEPROM 134 //160
  char OTApwd[16]="";                        // up to 16 Byte - EEPROM 166 //192
  char CFGpwd[16]="";                        // up to 16 Byte - EEPROM 198 
  char WIFIpwd[16]="";                       // up to 16 Byte - EEPROM 220 - 251
  char mqttServer[32]="";                    // up to 32 Byte
  char mqttUser[16]="";                      // up to 16 Byte
  char mqttPwd[16]="";                       // up to 16 Byte
  uint8_t nodeID;                         // 1 byte
  // Application Settings here... from EEPROM 252 - 511 // (0 - 511)

} config;

void printConfig(){

  ECHO_MSG("Printing Config\n");

  ECHO_MSG("DHCP:%d\n", config.dhcp);
  ECHO_MSG("DayLight:%d\n", config.isDayLightSaving);

  ECHO_MSG("NTP update every %ld sec\n", config.Update_Time_Via_NTP_Every); // 4 Byte
  ECHO_MSG("Timezone %ld\n", config.timeZone); // 4 Byte

  ECHO_MSG("IP:%d.%d.%d.%d\n", config.IP[0],config.IP[1],config.IP[2],config.IP[3]);
  ECHO_MSG("Mask:%d.%d.%d.%d\n", config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3]);
  ECHO_MSG("Gateway:%d.%d.%d.%d\n", config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3]);


  ECHO_MSG("SSID:%s\n", config.ssid);
  ECHO_MSG("PWD:%s\n", config.password);
  ECHO_MSG("connectToWifi:%d\n", config.connectToWifi);
  ECHO_MSG("ntp ServerName:%s\n", config.ntpServerName);
  ECHO_MSG("Device Name:%s\n", config.DeviceName);
  ECHO_MSG("OTA password:%s\n", config.OTApwd);
  ECHO_MSG("CFG password:%s\n", config.CFGpwd);
  ECHO_MSG("WIFI password:%s\n", config.WIFIpwd);
  ECHO_MSG("mqttServer:%s\n", config.mqttServer);
  ECHO_MSG("nodeID:%d\n", config.nodeID);
  ECHO_MSG("mqttUser:%s\n", config.mqttUser);
  ECHO_MSG("mqttPwd:%s\n", config.mqttPwd);

    // Application Settings here... from EEPROM 192 up to 511 (0 - 511)

}


  /*
  void WriteConfig(){

    ECHO_MSG("Writing Config\n");

    
    EEPROM.putString("head", "CFG");
    EEPROM.putUChar("dhcp", config.dhcp);
    EEPROM.putUChar("isDayLight", config.isDayLightSaving);
    EEPROM.putULong("NTPrate", config.Update_Time_Via_NTP_Every); // 4 Byte
    EEPROM.putULong("timeZone", config.timeZone); // 4 Byte
    EEPROM.putUChar("ip0", config.IP[0]);
    EEPROM.putUChar("ip1", config.IP[1]);
    EEPROM.putUChar("ip2", config.IP[2]);
    EEPROM.putUChar("ip3", config.IP[3]);
    EEPROM.putUChar("nm0", config.Netmask[0]);
    EEPROM.putUChar("nm1", config.Netmask[1]);
    EEPROM.putUChar("nm2", config.Netmask[2]);
    EEPROM.putUChar("nm3", config.Netmask[3]);
    EEPROM.putUChar("gw0", config.Gateway[0]);
    EEPROM.putUChar("gw1", config.Gateway[1]);
    EEPROM.putUChar("gw2", config.Gateway[2]);
    EEPROM.putUChar("gw3", config.Gateway[3]);
    EEPROM.putString("ssid", config.ssid);
    EEPROM.putString("pwd", config.password);
    EEPROM.putString("ntpSN", config.ntpServerName);
    EEPROM.putString("DevN", config.DeviceName);
    EEPROM.putString("OTApwd", config.OTApwd);
    EEPROM.putString("CFGpwd", config.CFGpwd);
    EEPROM.putString("WIFIpwd", config.WIFIpwd);
    


  }
  boolean ReadConfig(){
    ECHO_MSG("\nReading Configuration\n");

    
    if (EEPROM.getString("head") == "CFG" )
    {
      ECHO_MSG("Configurarion Found!\n");


      
      config.dhcp = EEPROM.getUChar("dhcp",0);
      config.isDayLightSaving = EEPROM.getUChar("isDayLight");
      config.Update_Time_Via_NTP_Every = EEPROM.getULong("NTPrate"); // 4 Byte
      config.timeZone = EEPROM.getULong("timeZone"); // 4 Byte
      config.IP[0] = EEPROM.getUChar("ip0");
      config.IP[1] = EEPROM.getUChar("ip1");
      config.IP[2] = EEPROM.getUChar("ip2");
      config.IP[3] = EEPROM.getUChar("ip3");
      config.Netmask[0] = EEPROM.getUChar("nm0");
      config.Netmask[1] = EEPROM.getUChar("nm1");
      config.Netmask[2] = EEPROM.getUChar("nm2");
      config.Netmask[3] = EEPROM.getUChar("nm3");
      config.Gateway[0] = EEPROM.getUChar("gw0");
      config.Gateway[1] = EEPROM.getUChar("gw1");
      config.Gateway[2] = EEPROM.getUChar("gw2");
      config.Gateway[3] = EEPROM.getUChar("gw3");
      config.ssid = EEPROM.getString("ssid");
      config.password = EEPROM.getString("pwd");
      config.ntpServerName = EEPROM.getString("ntpSN");
      config.DeviceName = EEPROM.getString("DevN");
      config.OTApwd = EEPROM.getString("OTApwd");
      config.CFGpwd = EEPROM.getString("CFGpwd");
      config.WIFIpwd = EEPROM.getString("WIFIpwd");
      
      // Application parameters here ... from EEPROM 192 to 511

      return true;

    }
    else
    {
      ECHO_MSG("Configurarion NOT FOUND!!!!\n");
      return false;
    }
    
  }

*/

void configLoadDefaults(uint16_t ChipId){
  config.crc = 0;
  #if defined(ESP32) // ARDUINO_ESP32_DEV
    strcpy_cln(config.ssid,"",sizeof(config.ssid)); // SSID of access point
    //config.ssid = "ESP32-" + String(ChipId,HEX);       // SSID of access point
  #elif defined(ESP8266) //ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
    //config.ssid = "ESP8266-" + String(ChipId,HEX);       // SSID of access point
    strcpy_cln(config.ssid,"",sizeof(config.ssid));
  #endif
  strcpy_cln(config.password, "",sizeof(config.password)) ;   // password of access point
  config.connectToWifi = false;
  config.dhcp = true;
  config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
  config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
  config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 254;
  //config.ntpServerName = "0.pt.pool.ntp.org"; //"0.ch.pool.ntp.org"; // to be adjusted to PT ntp.ist.utl.pt
  strcpy_cln(config.ntpServerName, "0.pt.pool.ntp.org", sizeof(config.ntpServerName)) ;  
  config.Update_Time_Via_NTP_Every =  5;
  config.timeZone = 1;
  config.isDayLightSaving = true;
  //config.DeviceName = "Not Named";
  strcpy_cln(config.DeviceName, "Not Named", sizeof(config.DeviceName)) ;  
  //config.OTApwd = "";
  strcpy_cln(config.OTApwd, "", sizeof(config.OTApwd)) ;  
  //config.CFGpwd = "";
  strcpy_cln(config.CFGpwd, "", sizeof(config.CFGpwd)) ;  
  //config.WIFIpwd = "";
  strcpy_cln(config.WIFIpwd, "", sizeof(config.WIFIpwd)) ;  
  strcpy_cln(config.mqttServer, "", sizeof(config.mqttServer)) ;  
  strcpy_cln(config.mqttUser, "", sizeof(config.mqttUser)) ;  
  strcpy_cln(config.mqttPwd, "", sizeof(config.mqttPwd)) ;  
  config.nodeID = 0;
  return;

}

void WriteConfig(){

  ECHO_MSG("Writing Config\n");

  config.crc = calculateCRC32((uint8_t *)&config+4 ,sizeof(config) -4 );
  //DEBUG_MSG("[ESPBASE writeEEPROM] crc:%lu\n",config.crc);
  #if defined(ESP32)
    EEPROM.putBytes("config", &config,sizeof(config));
  #elif defined(ESP8266) 
    EEPROM.put(0, config);    
    EEPROM.commit();
  #else
    #error "EEPROM not available on this board"
  #endif
}

boolean ReadConfig(){
  ECHO_MSG("\nReading Configuration\n");
  //DEBUG_MSG("[read config] crc:%lu\tmqttServer(before):%s\n",config.crc,config.mqttServer);
  uint32_t tmpCRC;
  #if defined(ESP32)
    EEPROM.getBytes("config", &config,sizeof(config));
  #elif defined(ESP8266) 
    EEPROM.get(0,config);
    EEPROM.commit();
  #else
    #error "EEPROM not available on this board"
  #endif

  //DEBUG_MSG("[read config] crc:%lu\tmqttServer(after):%s\n",config.crc,config.mqttServer);
  tmpCRC = calculateCRC32((uint8_t *) &config+4 ,sizeof(config) -4 );
  //DEBUG_MSG("[ESPBASE readEEPROM] read crc:%lu\t calc CRC:%lu\n",config.crc,tmpCRC);
  if(config.crc == tmpCRC){
    ECHO_MSG("Configuration Found!\n");
    return true;
  }
  else{
    ECHO_MSG("Configurarion NOT FOUND!!!!\n");
    configLoadDefaults(getChipId());
    return false;
  }

}

#endif
