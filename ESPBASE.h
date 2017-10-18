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
#ifdef ARDUINO_ESP32_DEV
    //ESP32 specific here
    #include <ESP32PWM.h>
    #include <ESP32WebServer.h>
    #include <WiFi.h>
    #include <ESPmDNS.h>
    #include <WiFiUdp.h>
    #include <ArduinoOTA.h>
    #include <Ticker-ESP32.h>
#elif ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
    // ESP8266 specific here
  #include <ESP8266WiFi.h>
  #include <ESP8266mDNS.h>
  #include <ESP8266WebServer.h>
  #include <WiFiUdp.h>
  #include <ArduinoOTA.h>
  #include <Ticker.h>
  #include <EEPROM.h>
  extern "C" {
  #include "user_interface.h"
  }
#endif

#include "global.h"
#include "parameters.h"

//#ifdef   ARDUINO_ESP8266_NODEMCU || ARDUINO_ESP8266_ESP01
#include "NTP.h"
//#endif

// Include the HTML, STYLE and Script "Pages"

#include "Page_Admin.h"
#include "Page_Script.js.h"
#include "Page_Style.css.h"
#include "Page_NTPsettings.h"
#include "Page_Information.h"
#include "Page_General.h"
#include "PAGE_NetworkConfiguration.h"


#define LED_esp 2

Ticker ticker;
//os_timer_t myTimer;
char tmpESP[100];

class ESPBASE {
public:
    boolean WIFI_connected, CFG_saved;
    uint8_t initialize();

};


uint8_t ESPBASE::initialize(){

  CFG_saved = false;
  WIFI_connected = false;
  uint8_t timeoutClick = 50;

  String chipID;

  // define parameters storage
  #ifdef ARDUINO_ESP32_DEV
    EEPROM.begin("ESPBASE", false);
    PWM_initialize(LED_esp,256,0,5000,13);
  #elif ARDUINO_ESP8266_NODEMCU || ARDUINO_ESP8266_ESP01
    EEPROM.begin(512); // define an EEPROM space of 512Bytes to store data
  #endif

  //**** Network Config load
  CFG_saved = ReadConfig();

  //  Connect to WiFi acess point or start as Acess point
  if (CFG_saved) { //if config saved use saved values
      // Connect the ESP8266 to local WIFI network in Station mode
      Serial.println("Booting");
      //printConfig();
      WiFi.mode(WIFI_OFF);
      WiFi.mode(WIFI_STA);
      WiFi.begin(config.ssid.c_str(), config.password.c_str());
      Serial.print("millis from:");Serial.println(millis());
      //WIFI_connected = WiFi.waitForConnectResult();
      while((WiFi.status()!= WL_CONNECTED) and --timeoutClick > 0) {
        delay(500);
        Serial.print(".");
      }
      Serial.print("millis to:");Serial.println(millis());
      if(WiFi.status()!= WL_CONNECTED )
      {
          Serial.println("Connection Failed! activating to AP mode...");
          WIFI_connected = false;
      }
      else
      {
        WIFI_connected = true;
        Serial.println("****** Connected! ******");
      }
      Serial.print("Wifi ip:");Serial.println(WiFi.localIP());
  }

  if ( !WIFI_connected or !CFG_saved){ // if no values saved or not good use defaults

    // DEFAULT CONFIG
    Serial.println("Setting AP mode default parameters");
    #ifdef ARDUINO_ESP32_DEV
      config.ssid = "ESP32-" + String(getChipId(),HEX);       // SSID of access point
    #elif ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
      config.ssid = "ESP8266-" + String(getChipId(),HEX);       // SSID of access point
    #endif
    config.password = "" ;   // password of access point
    config.dhcp = true;
    config.IP[0] = 192; config.IP[1] = 168; config.IP[2] = 1; config.IP[3] = 100;
    config.Netmask[0] = 255; config.Netmask[1] = 255; config.Netmask[2] = 255; config.Netmask[3] = 0;
    config.Gateway[0] = 192; config.Gateway[1] = 168; config.Gateway[2] = 1; config.Gateway[3] = 254;
    config.ntpServerName = "0.ch.pool.ntp.org"; //"0.ch.pool.ntp.org"; // to be adjusted to PT ntp.ist.utl.pt
    config.Update_Time_Via_NTP_Every =  5;
    config.timeZone = 1;
    config.isDayLightSaving = true;
    config.DeviceName = "Not Named";
    config.OTApwd = "";
    WiFi.mode(WIFI_AP);
    WiFi.softAP(config.ssid.c_str());
    Serial.print("Wifi ip:");Serial.println(WiFi.softAPIP());
   }


    // Start HTTP Server for configuration
    server.on ( "/", []() {
      Serial.println("admin.html");
      server.send_P ( 200, "text/html", PAGE_AdminMainPage);  // const char top of page
    }  );
    server.on ( "/favicon.ico",   []() {
      Serial.println("favicon.ico");
      server.send( 200, "text/html", "" );
    }  );
    // Network config
    server.on ( "/config.html", send_network_configuration_html );
    // Info Page
    server.on ( "/info.html", []() {
      Serial.println("info.html");
      server.send_P ( 200, "text/html", PAGE_Information );
    }  );
    server.on ( "/ntp.html", send_NTP_configuration_html  );

    //server.on ( "/appl.html", send_application_configuration_html  );
    server.on ( "/general.html", send_general_html  );
    //  server.on ( "/example.html", []() { server.send_P ( 200, "text/html", PAGE_EXAMPLE );  } );
    server.on ( "/style.css", []() {
      Serial.println("style.css");
      server.send_P ( 200, "text/plain", PAGE_Style_css );
    } );
    server.on ( "/microajax.js", []() {
      Serial.println("microajax.js");
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
      Serial.println("Page Not Found");
      server.send ( 400, "text/html", "Page not Found" );
    }  );
    server.begin();
    Serial.println( "HTTP server started" );
    // ***********  OTA SETUP

    //ArduinoOTA.setHostname(host);
    ArduinoOTA.onStart([]() { // what to do before OTA download insert code here
        Serial.println("Start");
      });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onEnd([]() { // do a fancy thing with our board led at end
        for (int i=0;i<30;i++){
          analogWrite(LED_esp,(i*100) % 1001);
          delay(50);
        }
        digitalWrite(LED_esp,HIGH); // Switch OFF ESP LED to save energy
        Serial.println("\nEnd");
        ESP.restart();
      });

    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR) Serial.println("End Failed");
        ESP.restart();
      });

     /* setup the OTA server */
    ArduinoOTA.setPassword(config.OTApwd.c_str());    // ********    to be implemented as a parameter via Browser
    ArduinoOTA.begin();
    Serial.println("Ready");


    #ifdef ARDUINO_ESP32_DEV
      tkSecond.attach(1000000, ISRsecondTick);
    #elif ARDUINO_ESP8266_ESP01 || ARDUINO_ESP8266_NODEMCU
      // start internal time update ISR
      tkSecond.attach(1, ISRsecondTick);
    #endif
}


#endif
