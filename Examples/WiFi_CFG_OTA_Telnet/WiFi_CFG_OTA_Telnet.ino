#include <ESPBASE.h>


WiFiServer TelnetServer(23);  // Optional in case you want to use telnet as Monitor
WiFiClient Telnet;            // Optional in case you want to use telnet as Monitor


ESPBASE Esp;


void setup() {
  Serial.begin(115200);
   Esp.initialize();

   // put your setup code here, to run once:

   
   TelnetServer.begin();            // Optional in case you want to use telnet as Monitor
   TelnetServer.setNoDelay(true);   // Optional in case you want to use telnet as Monitor

}

void loop() {
  // put your main code here, to run repeatedly:

  //Serial.println(millis());
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


}
