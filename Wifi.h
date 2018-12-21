
/*
       void startWiFi
                              */
                              
/* ----------------------------------------------------------------- 
                            start_wifi()
   ----------------------------------------------------------------- */

void startWiFi() { // Start a Wi-Fi access point, and try to connect to some given access points. Then wait for either an AP or STA connection
 
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
  
    dbgprint("Attempting to connect to SSID: %s \n",ssid);
      
    //WiFi.hostname("NixieClock");       // DHCP Hostname  // See WiFi.config() !!! TODO
    WiFi.config(ip, gateway, subnet);  // (DNS not required)

    WiFi.begin(ssid, pass);  

    if (  WiFi.waitForConnectResult() == WL_CONNECTED ) {
     dbgprint("Connected to %s \n", WiFi.SSID().c_str());
     dbgprint( "IP = %d.%d.%d.%d \n", WiFi.localIP()[0], WiFi.localIP()[1], WiFi.localIP()[2], WiFi.localIP()[3] ) ;
     dbgprint("MAC: %s \n", WiFi.macAddress().c_str());
     long rssi = WiFi.RSSI();
     dbgprint("signal strength (RSSI): %ddbm \n\n", rssi);
   } 
  else 
   {
     dbgprint("Connection impossible, mode AP: %s \n\n", APssid);
     WiFi.mode(WIFI_AP_STA);          // 192.168.4.1           
     WiFi.softAP(APssid);
    } 
}
