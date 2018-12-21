
/* 
 *  NiXieClocK WebSocket MP3/RADIO 
 *    esp8266 Version 2.3.0            
                                        */

#include <Wire.h>
#include <ESP8266WiFi.h>
#include <TimeLib.h>                                    
#include <NtpClientLib.h>                            
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>   // Version 1.0.2
#include <FS.h>
#include <WebSocketsServer.h>
#include <ESP8266HTTPClient.h>

#include "GLOBALS.h"
#include "debug.h"
#include "headerHTML.h"
#include "ds1307.h"
#include "Spiffs.h"
#include "EEPROM.h"
#include "Wifi.h"
#include "NTP.h"                                        
#include "dFPlayer.h"
#include "httpClient.h"
#include "aLARM.h"
#include "animation.h"
#include "DynamicData.h"
#include "WebSockets.h"
#include "WebServer.h"

/* ______________________________________________________________

 ______________________ ** SETUP ** ____________________________ */

void setup() {
 
  /* ------- Setup for Chronometre ------------------ */
  chronoTimeOld = millis();   // store Chrono start "time"
  
  /* ------- Start the Serial communication -------- */
  Serial.begin(115200);        
  delay(10);
  dbgprint("\n");
   
  /* ------- Broches en sortie (Registre decalage) - */
  pinMode(verrou, OUTPUT);
  pinMode(donnee, OUTPUT);
  pinMode(horloge, OUTPUT);

  /* ------- Initialise le port I2C ( RTC ) -------- */
  Wire.begin(4, 5);            // D1 SCL & D2 SDA     for RTC
  delay(500);

  /* -------  00:00 au demarrage -------------*/
  DateTime_t now;
  affiche(decimal_to_bcd(0), decimal_to_bcd(0));
  
  /* ------- SETUP FUNCTIONS ----------------------- */ 
  startSPIFFS();               // Start the SPIFFS and list all contents
  startCONF();                 // Read EEPROM Animation & Wifi Values && fanion && nixie ON/OFF && ALARME ON/OFF
  playerMP3();                 // Player MP3
  startWiFi();                 // Start a Wi-Fi access point, and try to connect to some given access points
  serverNTP();                 // Start NTP serveur           
  startWebSocket();            // Start a WebSocket server
  startServer();               // Start a HTTP server with a file read handler and an upload handler

} // => END setup()

/* ______________________________________________________________

 ______________________ ** LOOP ** _____________________________ */

void loop() {
 /* -------------------- Variable Chrono/Timer  --------------- */
  chronoTime = millis(); 

 /* -------------------- AFFICHAGE HEURE NIXIE/CHRONO --------- */ 
  timerChrono();                     
  
  DateTime_t now;
  
  if ( (lampNixie == true) && (blinkChrono == false) && (affChrono == false) 
         && (affTimer == false) && (blinkTimer == false) )
    {
      read_current_datetime(&now);
      affiche(now.hours, now.minutes);   // Nixie lamp

      if (now.seconds == 0) {
               if (ANIM_TEMP == 1) animTEMP(); 
               if (ANIM_DATE == 1) animDATE();
               if (ANIM_BM == 1) animBanditM();
      }
    }

  if  ( nixieProg == true ) {
    if ( nixieH == bcd_to_decimal(now.hours) && nixieM == bcd_to_decimal(now.minutes) ) 
     {  affiche(999999999,999999999);
        lampNixie = false;
     }
   }

 /* -- At each time change Query NTP server ------- */
  if ( heure_t != bcd_to_decimal(now.hours) && blinkTimer== false 
          && blinkChrono == false && affChrono == false && affTimer == false ) 
   {             
    NTP_UPDATE_RTC();
   }

 /* ------------------ ALARM LOOP --------------------------- */
  if (alarm_ON == 1) {
    ALARM_SET();
   }

 /* ----------------- Webserver Websocket ------------------ */
  webSocket.loop();         // constantly check for websocket events
  server.handleClient();    // run the server

} // => END LOOP()
