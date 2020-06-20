#include <Arduino.h>

#include <ESPBASE.h>
#include <DebugTools.h>
#define LED_PIN 2

WiFiServer TelnetServer(23);  // Optional in case you want to use telnet as Monitor
WiFiClient Telnet;            // Optional in case you want to use telnet as Monitor




ESPBASE Esp;
#ifdef ESP8266
  // for Esp8266 a trigger was implemented just when the connection is stablished by the client
  // to imediatly do something

  WiFiEventHandler wifiConnectHandler;      // handler for station connect
  WiFiEventHandler wifiDisconnectHandler;   // hander for Station diconnect
  WiFiEventHandler SoftAPconnectedHandler;  // handler for AP client connect
  WiFiEventHandler SoftAPdisconnectedHandler;// hanlder for AP client disconnect


  void onAPclientConnect(const WiFiEventSoftAPModeStationConnected& evt) {
    //Serial.print("Station connected: ");
    //Serial.println(macToString(evt.mac));
    DEBUG_MSG("\n[Wifi AP] Client Connected\n");
  }

  void onAPclientDisconnected(const WiFiEventSoftAPModeStationDisconnected& evt) {
    //Serial.print("Station connected: ");
    //Serial.println(macToString(evt.mac));
    DEBUG_MSG("\n[Wifi AP] Client Disconnected   >>>>  #######  <<<<<< \n");
    
  }

  void onWifiConnect(const WiFiEventStationModeGotIP& event) {
    DEBUG_MSG("[Wifi ST] Connected to Wi-Fi.");
    //connectToMqtt();
  }

  void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
    DEBUG_MSG("[Wifi ST] Disconnected from Wi-Fi.");
    delay(2000); //wait 2 sec and try again
    //mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    Esp.WiFiconnect(); // when wifi connect, mqtt will connect also
  }


#elif ESP32
  // for ESP32 still the detection of client connection is made by pooling interval
  wifi_interface_t current_wifi_interface;

  void  onAPclientConnect(WiFiEvent_t event, WiFiEventInfo_t info){
    //Serial.print("Station connected: ");
    //Serial.println(macToString(evt.mac));
    DEBUG_MSG("\n[WiFi AP] Client Connected\n");
  }
  void onAPclietDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    //Serial.print("Station connected: ");
    //Serial.println(macToString(evt.mac));
    DEBUG_MSG("\n[WiFi AP] Client Disconnected   >>>>  #######  <<<<<< \n");
  }

  void onWifiConnect(WiFiEvent_t event, WiFiEventInfo_t info){
    //const WiFiEventStationModeGotIP& event) {
    DEBUG_MSG("[WiFi ST] Connected to Wi-Fi.");
    //connectToMqtt();
  }

  void onWifiDisconnect(WiFiEvent_t event, WiFiEventInfo_t info) {
    DEBUG_MSG("[WiFi ST] Disconnected from Wi-Fi.");
    delay(2000); //wait 2 sec and try again
    //mqttReconnectTimer.detach(); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
    Esp.WiFiconnect(); // when wifi connect, mqtt will connect also
  }

#endif

//Ticker wifiReconnectTimer;


void setup() {
  Serial.begin(115200);
   Esp.initialize();

   // put your setup code here, to run once:


   TelnetServer.begin();            // Optional in case you want to use telnet as Monitor
   TelnetServer.setNoDelay(true);   // Optional in case you want to use telnet as Monitor
   Serial.printf("*******   OTA PWD:%s****\n",config.OTApwd.c_str());

   pinMode(LED_PIN,OUTPUT);

  #ifdef ESP8266
    wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
    wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);
    SoftAPconnectedHandler = WiFi.onSoftAPModeStationConnected(onAPclientConnect);
    SoftAPdisconnectedHandler = WiFi.onSoftAPModeStationDisconnected(onAPclietDisconnected);

  #else //ESP32
    WiFi.onEvent(onWifiConnect, SYSTEM_EVENT_STA_CONNECTED);
    WiFi.onEvent(onWifiDisconnect, SYSTEM_EVENT_STA_DISCONNECTED);
    WiFi.onEvent(onAPclientConnect, SYSTEM_EVENT_AP_STAIPASSIGNED);
    WiFi.onEvent(onWifiConnect, SYSTEM_EVENT_AP_STADISCONNECTED);
  #endif


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
    //Telnet.print(".");

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
