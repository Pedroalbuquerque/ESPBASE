//
//  HTML PAGE
//

const char PAGE_Password[] PROGMEM =  R"=====(
<meta name="viewport" content="width=device-width, initial-scale=1" />
<meta http-equiv="Content-Type" content="text/html; charset=utf-8" />
<a href="/"  class="btn btn--s"><</a>&nbsp;&nbsp;<strong>Enter your password</strong>
<hr>
<form action="" method="post">
<table border="0"  cellspacing="0" cellpadding="3" >
<tr>
	<td align="right">Password</td>
	<td><input type="text" id="password" name="password" value=""></td>
</tr>
<tr><td colspan="2" align="center"><input type="submit" style="width:150px" class="btn btn--m btn--blue" value="OK"></td></tr>
</table>
</form>
<script>



window.onload = function ()
{
	load("style.css","css", function()
	{
		load("microajax.js","js", function()
		{
				setValues("/pwdvalues");
		});
	});
}
function load(e,t,n){if("js"==t){var a=document.createElement("script");a.src=e,a.type="text/javascript",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}else if("css"==t){var a=document.createElement("link");a.href=e,a.rel="stylesheet",a.type="text/css",a.async=!1,a.onload=function(){n()},document.getElementsByTagName("head")[0].appendChild(a)}}



</script>
)=====";

String tmpPassword;

// Functions for this Page
/*
void send_password_value_html()
{

	String values ="";
	values += "password|" + (String) tmpPassword + "|div\n";
	server.send ( 200, "text/plain", values);
	ECHO_MSG(__FUNCTION__);

}
*/
void send_password_html( )
{

	if (server.args() > 0 )  // Save Settings
	{
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			DEBUG_MSG("[send_passwrod_html] %s\t arg:%s::%s\n",server.uri(), server.argName(i),server.arg(i));

			if (server.argName(i) == "password") tmpPassword = urldecode(server.arg(i));
            /*
			if (server.argName(i) == "OTApwd") {
				if(strlen(urldecode(server.arg(i)).c_str()) >=8)
					config.OTApwd = urldecode(server.arg(i));
				else
					config.OTApwd = "";
			}
			*/
		}
		
		DEBUG_MSG(" received PWD: %s\t stored pwd:%s\n",tmpPassword, config.CFGpwd);
		if(tmpPassword == config.CFGpwd){
			DEBUG_MSG("PWD ok!\n");
			
			String tmp = "<head>";
  					tmp +="<meta http-equiv=\"Refresh\" content=\"0; URL=/general.html\">";
					tmp +="</head>";
			server.send ( 200, "text/html", tmp );
			
			//server.send_P ( 200, "text/html", PAGE_AdminGeneralSettings );
			return;
		}
		else
		{
			DEBUG_MSG("Wrong password!\n");
		}
		
		//WriteConfig();
		//firstStart = true;
		//ESP.restart();
	}

	server.send_P ( 200, "text/html", PAGE_Password );
	ECHO_MSG(__FUNCTION__);


}

void send_password_configuration_values_html()
{
	String values ="";
	values += "password|" +  tmpPassword +  "|input\n";

	server.send ( 200, "text/plain", values);
	ECHO_MSG(__FUNCTION__);
  AdminTimeOutCounter=0;
}
