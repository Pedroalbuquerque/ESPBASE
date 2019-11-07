#include <Arduino.h>

#include <ESPBASE.h>
#include <DebugTools.h>
#define LED_PIN 2

WiFiServer TelnetServer(23);  // Optional in case you want to use telnet as Monitor
WiFiClient Telnet;            // Optional in case you want to use telnet as Monitor


ESPBASE Esp;

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;
Ticker wifiReconnectTimer;


void setup() {
  Serial.begin(115200);
   Esp.initialize();

   // put your setup code here, to run once:


   TelnetServer.begin();            // Optional in case you want to use telnet as Monitor
   TelnetServer.setNoDelay(true);   // Optional in case you want to use telnet as Monitor
   Serial.printf("*******   OTA PWD:%s****\n",config.OTApwd.c_str());

   pinMode(LED_PIN,OUTPUT);

   wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
   wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);

}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  DEBUG_MSG("Disconnected from Wi-Fi.");
  delay(2000); //wait 2 sec and try again
  //mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
  Esp.WiFiconnect(); // when wifi connect, mqtt will connect also
}
void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  DEBUG_MSG("[mqtt] Connected to Wi-Fi.");
  //connectToMqtt();
}



void loop() {

  // OTA request handling
  ArduinoOTA.handle();

  //  WebServer requests handling
  server.handleClient();

   //  feed de DOG :)
  customWatchdog = millis();

 // activate telnet service to act as output console

  if (TelnetServer.hasClient()){            // Optional in case you want to use telnet as Monitor
    if (!Telnet || !Telnet.connected()){
      if(Telnet) Telnet.stop();
      Telnet = TelnetServer.available();
    } else {
      TelnetServer.available().stop();
    }
  }


    //**** Normal Skecth code here ...
    Telnet.print(".");

    // put your main code here, to run repeatedly:

    //blink a LED as an example
    static uint32_t blinkTimer = millis();
    uint16_t interval = 1000;
    static uint8_t ledON = false;
    if(millis() > blinkTimer + interval){
      ledON = !ledON;
      digitalWrite(LED_PIN,ledON);
      blinkTimer = millis();
    }




}
