 
/* 
 *  String GetMacAddress
 *  void send_wifi_values_html
 *  void send_ntp_values_html
 *  void send_connection_state_values_html
 *  void send_network_configuration_html
                                                     */
                                                     
/* ------------------------------------------------------------------------------------------------ */
String GetMacAddress() {
        uint8_t mac[6];
        char macStr[18] = { 0 };
        WiFi.macAddress(mac);
        sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return  String(macStr);
}

/* ------------------------------------------------------------------------------------------------ */
void send_wifi_values_html() {

  String values = "";

  values += "x_ssid|" + (String)WiFi.SSID() + "|div\n";
  values += "x_ip|" + (String)WiFi.localIP()[0] + "." + (String)WiFi.localIP()[1] + "." + (String)WiFi.localIP()[2] + "." + (String)WiFi.localIP()[3] + "|div\n";
  values += "x_gateway|" + (String)WiFi.gatewayIP()[0] + "." + (String)WiFi.gatewayIP()[1] + "." + (String)WiFi.gatewayIP()[2] + "." + (String)WiFi.gatewayIP()[3] + "|div\n";
  values += "x_netmask|" + (String)WiFi.subnetMask()[0] + "." + (String)WiFi.subnetMask()[1] + "." + (String)WiFi.subnetMask()[2] + "." + (String)WiFi.subnetMask()[3] + "|div\n";
  values += "x_mac|" + GetMacAddress() + "|div\n";
  values += "x_dns|" + (String)WiFi.dnsIP()[0] + "." + (String)WiFi.dnsIP()[1] + "." + (String)WiFi.dnsIP()[2] + "." + (String)WiFi.dnsIP()[3] + "|div\n";
/*  values += "x_ntp_sync|" + NTP.getTimeDateString(NTP.getLastNTPSync()) + "|div\n";
  values += "x_ntp_time|" + NTP.getTimeStr() + "|div\n";
  values += "x_ntp_date|" + NTP.getDateStr() + "|div\n"; */
  server.send(200, "text/plain", values);
}

/* ------------------------------------------------------------------------------------------------ */
void send_ntp_values_html() {
  
  String values = "";

  values += "x_ntp_sync|" + NTP.getTimeDateString(NTP.getLastNTPSync()) + "|div\n";
  values += "x_ntp_time|" + NTP.getTimeStr() + "|div\n";
  values += "x_ntp_date|" + NTP.getDateStr() + "|div\n";
  server.send(200, "text/plain", values);
}

/* ------------------------------------------------------------------------------------------------ */
void send_connection_state_values_html() {

  String state = "N/A";
  String Networks = "";
  if (WiFi.status() == 0) state = "Idle";
  else if (WiFi.status() == 1) state = "NO SSID AVAILBLE";
  else if (WiFi.status() == 2) state = "SCAN COMPLETED";
  else if (WiFi.status() == 3) state = "<span style=color:green>CONNECTED</span>";
  else if (WiFi.status() == 4) state = "CONNECT FAILED";
  else if (WiFi.status() == 5) state = "CONNECTION LOST";
  else if (WiFi.status() == 6) state = "DISCONNECTED";

  int n = WiFi.scanNetworks();

  if (n == 0)
  {
    Networks = "<font color='#FF0000'>No networks found!</font>";
  }
  else
  {
    Networks = "Found " + String(n) + " Networks<br>";
    Networks += "<table style='width:300px' border='0' cellspacing='0' cellpadding='3'>";
    Networks += "<tr bgcolor='#DDDDDD' ><td><strong>Name</strong></td><td><strong>Quality</strong></td><td><strong>Enc</strong></td><tr>";
    for (int i = 0; i < n; ++i)
    {
      int quality = 0;
      if (WiFi.RSSI(i) <= -100) {
        quality = 0;
      }
       else if (WiFi.RSSI(i) >= -50) {
        quality = 100;
       }  else {
        quality = 2 * (WiFi.RSSI(i) + 100);
        }

      Networks += "<tr><td><a style='color:black' class=values href='javascript:selssid(\"" + String(WiFi.SSID(i)) + "\")'>" + String(WiFi.SSID(i)) + "</a></td><td>" 
                  + String(quality) + "%</td><td>" + String((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*") + "</td></tr>";
    }
      Networks += "</table>";
  }

  String values = "";
  values += "connectionstate|" + state + "|div\n";
  values += "networks|" + Networks + "|div\n";
  server.send(200, "text/plain", values);
}

/* ------------------------------------------------------------------------------------------------ */
void send_network_configuration_html() {
  String arg_val_str , arg_val_str1 ;

  for ( uint8_t i = 0; i < server.args(); i++ )
  {
   if (server.argName(i) == "ssid") {
      arg_val_str = server.arg("ssid");
     }
   if (server.argName(i) == "pass") {
      arg_val_str1 = server.arg("pass");
     }
  }

 String wifi_msg ="<a href=/><h2 class=collapsible style=width:200px>Reload</h2></a>"
                   "</center></body></html>";
 
 server.send(200, "text/html", PAGE_INDEX_HTML + wifi_msg );
 
 arg_val_str.toCharArray(ssid,32);                 // conversion String to char[]
 arg_val_str1.toCharArray(pass,32);

 dbgprint("WRITING EEPROM => SSID : %s PASS : %s \n",ssid,pass);
 
 writeEEPROM(0,32,ssid);
 writeEEPROM(32,32,pass);

 yield();
 ESP.restart();

}
