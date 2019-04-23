/*
    ESP Base
    Copyright (C) 2017  Pedro Albuquerque

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */



/*
 * * ESP8266 template with phone config web page
 * based on BVB_WebConfig_OTA_V7 from Andreas Spiess https://github.com/SensorsIot/Internet-of-Things-with-ESP8266
 *
 */
#ifndef ESPBASE_H
#define ESPBASE_H

#include <Arduino.h>
#include <string.h>
#include <DebugTools.h>


#if defined(ESP32)
    //ESP32 specific here
    #include <ESP32PWM.h>
    #include <ESP32WebServer.h>
    #include <HTTPClient.h>
    #include <WiFi.h>
    #include <ESPmDNS.h>
    #include <WiFiUdp.h>
    #include <ArduinoOTA.h>
    #include <Ticker.h>
#elif defined(ESP8266) //ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
    // ESP8266 specific here
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
  #include <ESP8266HTTPClient.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <Ticker.h>
  #include <EEPROM.h>
  extern "C" {
  #include "user_interface.h"
  }
#else
  #warning NO Board defined
#endif

extern WiFiClient Telnet;


class ESPBASE {
public:
    bool WIFI_connected, CFG_saved;
    void initialize(uint8_t asStation);
    void httpSetup();
    void OTASetup();
    void WiFiconnect(uint8_t asStation);

};

#include "parameters.h"
#include "global.h"
#include "wifiTools.h"
#include "NTP.h"


// Include the HTML, STYLE and Script "Pages"
#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPSettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "Page_NetworkConfiguration.h"


//char tmpESP[100];

void ESPBASE::initialize( uint8_t asStation = true){

    //  Wifi connect to an AP or start as a AP
    //if (asStation)
      WiFiconnect( asStation );
    //else
    //  startAP();
    //  Http Setup

    httpSetup();

    // ***********  OTA SETUP
    OTASetup();


    // start internal time update ISR
    #if defined(ESP32) //ARDUINO_ESP32_DEV
      tkSecond.attach(1000000, ISRsecondTick);
    #elif defined(ESP8266) //ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
      tkSecond.attach(1, ISRsecondTick);
    #endif

    ECHO_MSG("Ready\n");

}

void ESPBASE::WiFiconnect(uint8_t asStation = true){

  // define parameters storage
  #if defined(ESP32)  //ARDUINO_ESP32_DEV
    EEPROM.begin("ESPBASE", false);
    //PWM_initialize(LED_esp,256,0,5000,13);
  #elif defined(ESP8266) //ARDUINO_ESP8266_NODEMCU || ARDUINO_ESP8266_ESP01
    EEPROM.begin(512); // define an EEPROM space of 512Bytes to store data
  #endif

  //**** Network Config load
  CFG_saved = ReadConfig();

  if(!CFG_saved){
    //load config with default values
    configLoadDefaults(getChipId());

  }

  // is asStaion try to connect and return on sucess
  if(asStation){

    if(!WiFi.isConnected() ){ // if not connected
      WIFI_connected = false;

      String chipID;

      // Connect the ESP8266 to local WIFI network in Station mode
      // using SSID and password saved in parameters (config object)
      ECHO_MSG("Booting");
      //printConfig();
      WiFi.disconnect(); // just to be sure ...
      WiFi.mode(WIFI_OFF);
      WiFi.mode(WIFI_STA);
      WiFi.begin(config.ssid.c_str(), config.password.c_str());
      //WIFI_connected = WiFi.waitForConnectResult();
      uint8_t timeoutClick = 50;
      while((WiFi.status()!= WL_CONNECTED) and --timeoutClick > 0) {
        delay(500);
        ECHO_MSG(".");
      }
      if(WiFi.status()!= WL_CONNECTED )
      {
          ECHO_MSG("Connection Failed! activating to AP mode...\n");
          WIFI_connected = false;
      }
      else
      {
        WIFI_connected = true;
        ECHO_MSG("****** Connected! ******\n");
        ECHO_MSG("Wifi ip:");ECHO_PORT.println(WiFi.localIP());
        return;  // if sucess exit
      }
    }
    else{
      ECHO_MSG("[WiFi]WiFi already connected\n");
      WIFI_connected = true;
      return; // if sucess exit
    }

  }

  //else
  // Start as AP if failed or not asStation

  // DEFAULT CONFIG
  ECHO_MSG("Setting AP mode default parameters\n");

  WiFi.disconnect(); // just to be sure ...
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_AP);
  String ssidAP = config.DeviceName + String(getChipId(),HEX);
  String ssidPWD = config.OTApwd;
  DEBUG_MSG("AP start:%d\n",WiFi.softAP(ssidAP.c_str(),config.OTApwd.c_str()));
  ECHO_MSG("AP:%s\n",ssidAP.c_str());
  ECHO_MSG("Wifi ip:");ECHO_PORT.println(WiFi.softAPIP());

  return;
}

void ESPBASE::httpSetup(){
  // Start HTTP Server for configuration
  server.on ( "/", []() {
    ECHO_MSG("admin.html\n");
    server.send_P ( 200, "text/html", PAGE_AdminMainPage);  // const char top of page
  }  );
  server.on ( "/favicon.ico",   []() {
    ECHO_MSG("favicon.ico\n");
    server.send( 200, "text/html", "" );
  }  );
  // Network config
  server.on ( "/config.html", send_network_configuration_html );
  // Info Page
  server.on ( "/info.html", []() {
    ECHO_MSG("info.html\n");
    server.send_P ( 200, "text/html", PAGE_Information );
  }  );
  server.on ( "/ntp.html", send_NTP_configuration_html  );

  //server.on ( "/appl.html", send_application_configuration_html  );
  server.on ( "/general.html", send_general_html  );
  //  server.on ( "/example.html", []() { server.send_P ( 200, "text/html", PAGE_EXAMPLE );  } );
  server.on ( "/style.css", []() {
    ECHO_MSG("style.css\n");
    server.send_P ( 200, "text/plain", PAGE_Style_css );
  } );
  server.on ( "/microajax.js", []() {
    ECHO_MSG("microajax.js\n");
    server.send_P ( 200, "text/plain", PAGE_microajax_js );
  } );
  server.on ( "/admin/values", send_network_configuration_values_html );
  server.on ( "/admin/connectionstate", send_connection_state_values_html );
  server.on ( "/admin/infovalues", send_information_values_html );
  server.on ( "/admin/ntpvalues", send_NTP_configuration_values_html );
  //server.on ( "/admin/applvalues", send_application_configuration_values_html );
  server.on ( "/admin/generalvalues", send_general_configuration_values_html);
  server.on ( "/admin/devicename",     send_devicename_value_html);
  server.onNotFound ( []() {
    ECHO_MSG("Page Not Found\n");
    server.send ( 400, "text/html", "Page not Found" );
  }  );
  server.begin();
  ECHO_MSG( "HTTP server started\n" );

  return;
}

void ESPBASE::OTASetup(){

      //ArduinoOTA.setHostname(host);
      ArduinoOTA.onStart([]() { // what to do before OTA download insert code here
          ECHO_MSG("Start\n");
        });
      ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        ECHO_MSG("Progress: %u%%\r", (progress / (total / 100)));
      });
      ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
          for (int i=0;i<30;i++){
            analogWrite(LED_esp,(i*100) % 1001);
            delay(50);
          }
          analogWrite(LED_esp,0); //switch OFF PWM
          digitalWrite(LED_esp,HIGH); // Switch OFF ESP LED to save energy
          ECHO_MSG("\nEnd\n");
          ESP.restart();
        });

      ArduinoOTA.onError([](ota_error_t error) {
          ECHO_MSG("Error[%u]: ", error);
          if (error == OTA_AUTH_ERROR) {ECHO_MSG("Auth Failed\n");}
          else if (error == OTA_BEGIN_ERROR) {ECHO_MSG("Begin Failed\n");}
          else if (error == OTA_CONNECT_ERROR) {ECHO_MSG("Connect Failed\n");}
          else if (error == OTA_RECEIVE_ERROR) {ECHO_MSG("Receive Failed\n");}
          else if (error == OTA_END_ERROR) {ECHO_MSG("End Failed\n");}
          ESP.restart();
        });

       /* setup the OTA server */
      ArduinoOTA.setPassword(config.OTApwd.c_str());    // ********    to be implemented as a parameter via Browser
      ArduinoOTA.begin();

}



#endif
