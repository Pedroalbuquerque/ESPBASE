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


// Functions for this Page
/*
void send_devicename_value_html()
{

	String values ="";
	values += "devicename|" + (String) config.DeviceName + "|div\n";
    values += "OTApwd|" + (String) config.OTApwd + "|div\n";
	server.send ( 200, "text/plain", values);
	ECHO_MSG(__FUNCTION__);

}
*/
void send_general_html()
{

	if (server.args() > 0 )  // Save Settings
	{
		String temp = "";
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			DEBUG_MSG("[send_general_html] %s\t arg:%s::%s\n",server.uri(), server.argName(i),server.arg(i));
			if (server.argName(i) == "devicename") config.DeviceName = urldecode(server.arg(i));
            if (server.argName(i) == "OTApwd") {
				if(strlen(urldecode(server.arg(i)).c_str()) >=8)
					config.OTApwd = urldecode(server.arg(i));
				else
					config.OTApwd = "";
			}
            if (server.argName(i) == "CFGpwd") {
				if(strlen(urldecode(server.arg(i)).c_str()) >=8)
					config.CFGpwd = urldecode(server.arg(i));
				else
					config.CFGpwd = "";
			}
		}
		WriteConfig();
		firstStart = true;
		ESP.restart();
	}
	
	//uint8_t count = 3;
	
	if (!server.authenticate("admin", config.CFGpwd.c_str()))  {
		server.requestAuthentication();
	 }
	else{
		server.send_P ( 200, "text/html", PAGE_AdminGeneralSettings );
	 }
	
	server.send ( 400, "text/html", "Authentication failed" );
	

	ECHO_MSG(__FUNCTION__);


}

void send_general_configuration_values_html()
{
	String values ="";
	values += "devicename|" +  (String)  config.DeviceName +  "|input\n";
    values += "OTApwd|" +  (String)  config.OTApwd +  "|input\n";
    values += "CFGpwd|" +  (String)  config.CFGpwd +  "|input\n";

	server.send ( 200, "text/plain", values);
	ECHO_MSG(__FUNCTION__);
  AdminTimeOutCounter=0;
}
