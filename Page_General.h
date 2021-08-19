//
//  HTML PAGE
//

const char PAGE_AdminGeneralSettings[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>General Settings</strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >
<tr>
	<td align="right">Name of Device</td>
	<td><input type="text" id="devicename" name="devicename" value=""></td>
</tr>
<tr>
<td align="right">OTA Password</td>
<td><input type="text" id="OTApwd" name="OTApwd" value=""></td>
</tr>
<tr>
<td align="right">CFG Password</td>
<td><input type="text" id="CFGpwd" name="CFGpwd" value=""></td>
</tr>
<tr>
<td align="right">WiFi Password</td>
<td><input type="text" id="WIFIpwd" name="WIFIpwd" value=""></td>
</tr>
<tr>
<td align="right">MQTT Server</td>
<td><input type="text" id="mqttServer" name="mqttServer" value=""></td>
<td align="right">MQTT Node ID</td>
<td><input type="text" id="nodeID" name="nodeID" value=""></td>
</tr>
<tr>
<td align="right">MQTT User</td>
<td><input type="text" id="mqttUser" name="mqttUser" value=""></td>
</tr>
<tr>
<td align="right">MQTT Password</td>
<td><input type="text" id="mqttPwd" name="mqttPwd" value=""></td>
</tr>

<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="Save"></td></tr>
</table>
</form>
<script>



window.onload = function ()
{
	load("style.css","css", function()
	{
		load("microajax.js","js", function()
		{
				setValues("/admin/generalvalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";


void send_general_html()
{

	if (server.args() > 0 )  // Save Settings
	{
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			DEBUG_MSG("[send_general_html] %s\t arg:%s::%s\n",server.uri().c_str(), server.argName(i).c_str(),server.arg(i).c_str());
			if (server.argName(i) == "devicename") strcpy_cln(config.DeviceName, urldecode(server.arg(i)).c_str(),sizeof(config.DeviceName));
      if (server.argName(i) == "OTApwd") {
				if(strlen(urldecode(server.arg(i)).c_str()) >=8)
					strcpy_cln(config.OTApwd, urldecode(server.arg(i)).c_str(),sizeof(config.OTApwd));
				else
					strcpy_cln(config.OTApwd, "", sizeof(config.OTApwd));
			}
      if (server.argName(i) == "CFGpwd") {
				if(strlen(urldecode(server.arg(i)).c_str()) >=8)
					strcpy_cln(config.CFGpwd,urldecode(server.arg(i)).c_str(),sizeof(config.CFGpwd));
				else
					strcpy_cln(config.CFGpwd, "", sizeof(config.CFGpwd));
			}
      if (server.argName(i) == "WIFIpwd") {
				if(strlen(urldecode(server.arg(i)).c_str()) >=8)
					strcpy_cln(config.WIFIpwd, urldecode(server.arg(i)).c_str(), sizeof(config.WIFIpwd));
				else
					strcpy_cln(config.WIFIpwd, "", sizeof(config.WIFIpwd));
			}
			if (server.argName(i) == "mqttServer") strcpy_cln(config.mqttServer, urldecode(server.arg(i)).c_str(),sizeof(config.mqttServer));
			if (server.argName(i) == "mqttUser") strcpy_cln(config.mqttUser, urldecode(server.arg(i)).c_str(),sizeof(config.mqttUser));
      if (server.argName(i) == "mqttPwd") {
				if(strlen(urldecode(server.arg(i)).c_str()) >=8)
					strcpy_cln(config.mqttPwd, urldecode(server.arg(i)).c_str(), sizeof(config.mqttPwd));
				else
					strcpy_cln(config.mqttPwd, "", sizeof(config.mqttPwd));
			}
			if (server.argName(i) == "nodeID") config.nodeID = urldecode(server.arg(i)).toInt();


		}
		WriteConfig();
		firstStart = true;
		ESP.restart();
	}
	
	//uint8_t count = 3;
	
	if (!server.authenticate("admin", config.CFGpwd))  {
		#ifdef ESP8266
		server.requestAuthentication(BASIC_AUTH,__null,"Failed Authetication");
		#elif defined(ESP32)
		server.requestAuthentication();
		#endif
	 }
	else{
		
		server.send_P ( 200, "text/html", PAGE_AdminGeneralSettings );
	 }

	//server.send( 200, "text/html", PAGE_AdminGeneralSettings);
	//server.send ( 400, "text/html", "Authentication failed" );
	

	ECHO_MSG(__FUNCTION__);


}

void send_general_configuration_values_html()
{
	String values ="";
	values += "devicename|" +  (String)  config.DeviceName +  "|input\n";
    values += "OTApwd|" +  (String)  config.OTApwd +  "|input\n";
    values += "CFGpwd|" +  (String)  config.CFGpwd +  "|input\n";
    values += "WIFIpwd|" +  (String)  config.WIFIpwd +  "|input\n";
    values += "mqttServer|" +  (String)  config.mqttServer +  "|input\n";
    values += "mqttUser|" +  (String)  config.mqttUser +  "|input\n";
    values += "mqttPwd|" +  (String)  config.mqttPwd +  "|input\n";
    values += "nodeID|" +  (String)  config.nodeID +  "|input\n";


	server.send ( 200, "text/plain", values);
	ECHO_MSG(__FUNCTION__);
  AdminTimeOutCounter=0;
}
