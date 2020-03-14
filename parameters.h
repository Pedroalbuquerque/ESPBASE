 
#ifndef PARAMETERS_H
#define PARAMETERS_H

struct strConfig {
  boolean dhcp;                         // 1 Byte - EEPROM 16  // from 0-15 reserved, so start on 16
  boolean isDayLightSaving;             // 1 Byte - EEPROM 17
  long Update_Time_Via_NTP_Every;       // 4 Byte - EEPROM 18
  long timeZone;                        // 4 Byte - EEPROM 22
  byte  IP[4];                          // 4 Byte - EEPROM 26 //32 // error addr jump ??
  byte  Netmask[4];                     // 4 Byte - EEPROM 30 //36
  byte  Gateway[4];                     // 4 Byte - EEPROM 34 //40
  String ssid;                          // up to 32 Byte - EEPROM 38 //64
  String password;                      // up to 32 Byte - EEPROM 70 //96
  String ntpServerName;                 // up to 32 Byte - EEPROM 102 //128
  String DeviceName;                    // up to 32 Byte - EEPROM 134 //160
  String OTApwd;                        // up to 32 Byte - EEPROM 166 //192
  String CFGpwd;                        // up to 32 Byte - EEPROM 198 - 220 

  // Application Settings here... from EEPROM 220 - 511 // 224 up to 511 (0 - 511)

} config;



#if defined(ESP32) // ARDUINO_ESP32_DEV

  #include <Preferences.h>

  Preferences EEPROM;

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


  }
  boolean ReadConfig(){
    ECHO_MSG("Reading Configuration\n");
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

      // Application parameters here ... from EEPROM 192 to 511

      return true;

    }
    else
    {
      ECHO_MSG("Configurarion NOT FOUND!!!!\n");
      return false;
    }
  }



#elif defined(ESP8266) //ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
  //  Auxiliar function to handle EEPROM
  // EEPROM-parameters

  void EEPROMWritelong(int address, long value){
    byte four = (value & 0xFF);
    byte three = ((value >> 8) & 0xFF);
    byte two = ((value >> 16) & 0xFF);
    byte one = ((value >> 24) & 0xFF);

    //Write the 4 bytes into the eeprom memory.
    EEPROM.write(address, four);
    EEPROM.write(address + 1, three);
    EEPROM.write(address + 2, two);
    EEPROM.write(address + 3, one);
  }

  long EEPROMReadlong(long address){
    //Read the 4 bytes from the eeprom memory.
    long four = EEPROM.read(address);
    long three = EEPROM.read(address + 1);
    long two = EEPROM.read(address + 2);
    long one = EEPROM.read(address + 3);

    //Return the recomposed long by using bitshift.
    return ((four << 0) & 0xFF) + ((three << 8) & 0xFFFF) + ((two << 16) & 0xFFFFFF) + ((one << 24) & 0xFFFFFFFF);
  }

  void WriteStringToEEPROM(int beginaddress, String string){
    char  charBuf[string.length() + 1];
    string.toCharArray(charBuf, string.length() + 1);
    for (uint16_t t =  0; t < sizeof(charBuf); t++)
    {
      EEPROM.write(beginaddress + t, charBuf[t]);
    }
  }

  String  ReadStringFromEEPROM(int beginaddress){
    volatile byte counter = 0;
    char rChar;
    String retString = "";
    while (1)
    {
      rChar = EEPROM.read(beginaddress + counter);
      if (rChar == 0) break;
      if (counter > 31) break;
      counter++;
      retString.concat(rChar);

    }
    return retString;
  }

  void WriteConfig(){

    ECHO_MSG("Writing Config\n");
    EEPROM.write(0, 'C');
    EEPROM.write(1, 'F');
    EEPROM.write(2, 'G');

    EEPROM.write(16, config.dhcp);
    EEPROM.write(17, config.isDayLightSaving);

    EEPROMWritelong(18, config.Update_Time_Via_NTP_Every); // 4 Byte
    EEPROMWritelong(22, config.timeZone); // 4 Byte

    EEPROM.write(26, config.IP[0]);
    EEPROM.write(27, config.IP[1]);
    EEPROM.write(28, config.IP[2]);
    EEPROM.write(29, config.IP[3]);

    EEPROM.write(30, config.Netmask[0]);
    EEPROM.write(31, config.Netmask[1]);
    EEPROM.write(32, config.Netmask[2]);
    EEPROM.write(33, config.Netmask[3]);

    EEPROM.write(34, config.Gateway[0]);
    EEPROM.write(35, config.Gateway[1]);
    EEPROM.write(36, config.Gateway[2]);
    EEPROM.write(37, config.Gateway[3]);

    WriteStringToEEPROM(38, config.ssid);
    WriteStringToEEPROM(70, config.password);
    WriteStringToEEPROM(102, config.ntpServerName);
    WriteStringToEEPROM(134, config.DeviceName);
    WriteStringToEEPROM(166, config.OTApwd);
    WriteStringToEEPROM(198, config.CFGpwd);

      // Application Settings here... from EEPROM 192 up to 511 (0 - 511)

    EEPROM.commit();

  }

  boolean ReadConfig(){
    ECHO_MSG("Reading Configuration\n");
    if (EEPROM.read(0) == 'C' && EEPROM.read(1) == 'F'  && EEPROM.read(2) == 'G' )
    {
      ECHO_MSG("Configuration Found!\n");
      config.dhcp = 	EEPROM.read(16);
      config.isDayLightSaving = EEPROM.read(17);
      config.Update_Time_Via_NTP_Every = EEPROMReadlong(18); // 4 Byte
      config.timeZone = EEPROMReadlong(22); // 4 Byte
      config.IP[0] = EEPROM.read(26);
      config.IP[1] = EEPROM.read(27);
      config.IP[2] = EEPROM.read(28);
      config.IP[3] = EEPROM.read(29);
      config.Netmask[0] = EEPROM.read(30);
      config.Netmask[1] = EEPROM.read(31);
      config.Netmask[2] = EEPROM.read(32);
      config.Netmask[3] = EEPROM.read(33);
      config.Gateway[0] = EEPROM.read(34);
      config.Gateway[1] = EEPROM.read(35);
      config.Gateway[2] = EEPROM.read(36);
      config.Gateway[3] = EEPROM.read(37);
      config.ssid = ReadStringFromEEPROM(38);
      config.password = ReadStringFromEEPROM(70);
      config.ntpServerName = ReadStringFromEEPROM(102);
      config.DeviceName = ReadStringFromEEPROM(134);
      config.OTApwd = ReadStringFromEEPROM(166);
      config.CFGpwd = ReadStringFromEEPROM(198);

      // Application parameters here ... from EEPROM 220 to 511

      return true;

    }
    else
    {
      ECHO_MSG("Configurarion NOT FOUND!!!!\n");
      return false;
    }
  }


#endif  // ESP8266

void printConfig(){

  ECHO_MSG("Printing Config\n");

  ECHO_MSG("DHCP:%d\n", config.dhcp);
  ECHO_MSG("DayLight:%d\n", config.isDayLightSaving);

  ECHO_MSG("NTP update every %ld sec\n", config.Update_Time_Via_NTP_Every); // 4 Byte
  ECHO_MSG("Timezone %ld\n", config.timeZone); // 4 Byte

  ECHO_MSG("IP:%d.%d.%d.%d\n", config.IP[0],config.IP[1],config.IP[2],config.IP[3]);
  ECHO_MSG("Mask:%d.%d.%d.%d\n", config.Netmask[0],config.Netmask[1],config.Netmask[2],config.Netmask[3]);
  ECHO_MSG("Gateway:%d.%d.%d.%d\n", config.Gateway[0],config.Gateway[1],config.Gateway[2],config.Gateway[3]);


  ECHO_MSG("SSID:%s\n", config.ssid.c_str());
  ECHO_MSG("PWD:%s\n", config.password.c_str());
  ECHO_MSG("ntp ServerName:%s\n", config.ntpServerName.c_str());
  ECHO_MSG("Device Name:%s\n", config.DeviceName.c_str());
  ECHO_MSG("OTA password:%s\n", config.OTApwd.c_str());
  ECHO_MSG("CFG password:%s\n", config.CFGpwd.c_str());

    // Application Settings here... from EEPROM 192 up to 511 (0 - 511)

}

void configLoadDefaults(uint16_t ChipId){

  #if defined(ESP32) // ARDUINO_ESP32_DEV
    config.ssid = "ESP32-" + String(ChipId,HEX);       // SSID of access point
  #elif defined(ESP8266) //ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
    config.ssid = "ESP8266-" + String(ChipId,HEX);       // SSID of access point
  #endif
  config.password = "" ;   // password of access point
  config.dhcp = true;
  config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
  config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
  config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 254;
  config.ntpServerName = "0.pt.pool.ntp.org"; //"0.ch.pool.ntp.org"; // to be adjusted to PT ntp.ist.utl.pt
  config.Update_Time_Via_NTP_Every =  5;
  config.timeZone = 1;
  config.isDayLightSaving = true;
  config.DeviceName = "Not Named";
  config.OTApwd = "";
  config.CFGpwd = "";

  return;

}


#endif
