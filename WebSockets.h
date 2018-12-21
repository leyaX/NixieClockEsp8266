
/*
 *  void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght)
 */
                                   
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) { // When a WebSocket message is received
  
 // ------ if the websocket is disconnected
  switch (type) {
    case WStype_DISCONNECTED:             
      dbgprint("[%u] Disconnected!\n", num);
      
 // ------- if a new websocket connection is established
      break;
    case WStype_CONNECTED: {             
        IPAddress ip = webSocket.remoteIP(num);
        dbgprint("[%u] Connected from %d.%d.%d.%d url: %s \n", num, ip[0], ip[1], ip[2], ip[3], payload);
                           } 
      break;
      
 // -------- if new text data is received   
    case WStype_TEXT: {                    

      String text =  String((char *) &payload[0]);
      
      dbgprint("[%u] get Text: %s\n", num, payload);      
       
/* ******************************** ALARM ********************************************************
 *********************************************************************************************** */

//  =>  ALARM SET -------------------------------------

   if (payload[0] == 'Z') 
     {           
      String setTIME = String((char *) &payload[1]);
      for (uint8_t i = 0; i < setTIME.length(); i++)
         {
        if (setTIME.substring(i, i + 1) == ":")               // Parser ":"
          {  
           alarmH = setTIME.substring(0, i).toInt();
           alarmM = setTIME.substring(i + 1).toInt();
         break;
          } 
         }                              // End for
      
    if (radioOrMp3 == false) { // MP3 
       writeALARM(hours_alarm, mins_alarm, mp3, RorM );      // heure et track mp3 => EEPROM struct alarmROM New => 0 pour mp3
       }else 
        {
        writeALARM(hours_alarm, mins_alarm, preset, RorM );   // heure et preset radio => EEPROM struct alarmROM New => 1 pour radio
        }
      
       writeConfEEPROM(add_alarmPROG , 1 );                   // alarm_PROG => 1 => EEPROM alarm_PROG
 
       alarm_ON = 1;
       alarm_check = 0 ;
       PlayMp3 = true;  
       
       }                                // END if ALARM SET  
      
//  =>  ALARM OFF ---------------------------------------

   if (text == "ALARMOFF") {
     writeConfEEPROM(add_alarmPROG , 0 );                     // alarm_PROG => 0 => EEPROM alarm_PROG
     alarm_ON = 0;
     }

//  =>   SET ALARM PROG & TEXT ALARM ------------------

    if (payload[0] == 'Y') 
      {           
      String setALARM = String((char *) &payload[1]);
      for (uint8_t i = 0; i < setALARM.length(); i++)
         {
        if (setALARM.substring(i, i + 1) == ":") { 
           hours_alarm = setALARM.substring(0, i).toInt();
           mins_alarm = setALARM.substring(i + 1).toInt(); 
        }}}

    if (payload[6] == 'V') { 
      txt_alarm = String((char *) &payload[7]); 
      for (uint8_t i = 0; i < txt_alarm.length(); i++);
      } 
        
//  =>  SAVE ALARM PROG ---------------------------------

    if ( text == "RcdAlarm") {                                    // alarm.html
      writeFileAlarmSPIFFS( hours_alarm, mins_alarm, txt_alarm );  
    }
 
//  =>  REMOVE ALARM PROG -------------------------------

    if ( text == "RemoveAlarm") {
      removeFilesSPIFFS("/alarm.html");
     }

//  =>  STOP ALARM --------------------------------------

    if (text == "STOP_ALARM") { 
       
      if (radioOrMp3 == false) {
          myDFPlayer.stop();
        } else {
          httpStopRadio(); 
          myDFPlayer.stop();  
        }
        
      alarm_ON = 0 ;
      alarm_check = 1;    
    }                        // END STOP ALARM

//  =>  REPEAT ALARM -------------------------------------

    if (text == "REPEAT_ALARM"){  
       myDFPlayer.stop();
       DateTime_t now;
       read_current_datetime(&now);
       alarmH = ((bcd_to_decimal(now.hours)));
       alarmM = ((bcd_to_decimal(now.minutes)) +9);
   
     if (alarmM >= 60) {        // correction on 60 minutes for "REPEAT"
       alarmM = (alarmM - 60);
       alarmH = alarmH +1;      // incremente l'heure
      }

      dbgprint("ALARME REPEAT => %d:%d \n", alarmH, alarmM);
      
      alarm_check = 1 ;           
      alarm_ON = 1 ; 
      PlayMp3 = true;
    }                       // END IF REPEAT ALARM

//  =>  Set Music ----------------------------------------

if (payload[0] == 'M')  {
 String setMp3 = String((char *) &payload[1]); 
   for (uint8_t i = 0; i < setMp3.length(); i++);
       mp3 = setMp3.toInt(); 
       dbgprint("Track n°: %d \n", mp3);
       radioOrMp3 = false; // MP3 TRUE
       RorM = 0;
}

//  =>  Set Preset Radio ----------------------------------

if (payload[0] == 'B')  {
 String Preset = String((char *) &payload[1]); 
   for (uint8_t i = 0; i < Preset.length(); i++);
       preset = Preset.toInt(); 
       dbgprint("Preset n°: %d \n", preset);
       radioOrMp3 = true; // RADIO TRUE
       RorM = 1 ;
}

/******************************* CHRONOMETRE **************************************
************************************************************************************/

 // ------- INIT CHRONO
     if (text == "INIT_CHRONO") { 
      blinkChrono = true; affChrono = false; startChrono = false; } 
   
 // ------- PLAY CHRONO 
     if (text == "PLAY_CHRONO") { 
       blinkChrono = false; affChrono = true; startChrono = true; }
   
 // ------- PAUSE CHRONO 
     if (text == "PAUSE_CHRONO") { 
       blinkChrono = true; affChrono = false; startChrono = false; } 
   
 // ------- STOP CHRONO
     if (text == "STOP_CHRONO") { 
       blinkChrono = false; affChrono = false; startChrono = false; 
       sec_chrono = 0; mins_chrono = 0;} 

/* ******************************* TIMER *******************************************
*************************************************************************************/

  // ----- INIT TIMER
     if (text == "INIT_TIMER") { 
       blinkTimer = true; affTimer = false; startTimer = false; } 
   
 // ------ PLAY TIMER 
     if (text == "PLAY_TIMER") { 
      blinkTimer = false; affTimer = true; startTimer = true; }
   
 // ------ PAUSE TIMER 
     if (text == "PAUSE_TIMER") { 
      blinkTimer = true; affTimer = false; startTimer = false; }       
   
 // ------ STOP TIMER
     if (text == "STOP_TIMER") {  
      blinkTimer = false; affTimer = false; 
      startTimer = false; myDFPlayer.stop(); } 

 // ------ SET TIMER and TEXT TIMER                              // TIMER.HTML
    if (payload[0] == 'W') 
      {           
      String setTIMER = String((char *) &payload[1]);
      for (uint8_t i = 0; i < setTIMER.length(); i++)
         {
        if (setTIMER.substring(i, i + 1) == ":") { 
           mins_timer = setTIMER.substring(0, i).toInt();
           sec_timer = setTIMER.substring(i + 1).toInt(); 
        }}}

    if (payload[6] == 'X') { 
      txt_timer = String((char *) &payload[7]); 
      for (uint8_t i = 0; i < txt_timer.length(); i++);
      } 
        
   // ----- SAVE TIMER
    if ( text == "RcdTimer") 
    {
       writeFileTimerSPIFFS( mins_timer, sec_timer, txt_timer );        // add timer.html
    }
 
   // ----- REMOVE TIMER
    if ( text == "RemoveTimer") 
    {
       removeFilesSPIFFS("/timer.html");                                // remove timer.html
    }
    
/* ******************************** DATE **********************************************
*************************************************************************************** */

   if (text == "DATE") {
       animDATE();
     }

/* ******************************** NTP ************************************************
**************************************************************************************** */

   if (text == "SETNTP") {
    NTP_UPDATE_RTC();
    }

/* ******************************** NIXIE ***********************************************
**************************************************************************************** */

   if (text == "nixieDOWN") {
     lampNixie = false;
     blinkTimer = false;
     blinkChrono = false;
     affTimer =false;
     affTimer = false; 
     affiche(999999999,999999999);
    }

  if (text == "nixieUP") {
    lampNixie = true; 
    nixieProg = 0;
    writeConfEEPROM(add_nixieProg, 0);
    writeFileToggleNixieSPIFFS();
   }

  if (payload[0] == 'N') {
      String setNixieTIME = String((char *) &payload[1]);
      for (uint8_t i = 0; i < setNixieTIME.length(); i++)
         {
        if (setNixieTIME.substring(i, i + 1) == ":")  // Parser ":"
          { 
           nixieH = setNixieTIME.substring(0, i).toInt();
           nixieM = setNixieTIME.substring(i + 1).toInt();
      break;
          } 
         }
      nixieProg = 1;   
      writeConfEEPROM(add_nixieH, nixieH);
      writeConfEEPROM(add_nixieM, nixieM);
      writeConfEEPROM(add_nixieProg, 1);
      writeFileToggleNixieSPIFFS();  
   }

/* ******************************** MP3 ******************************************
***********************************************************************************/

    if (text == "PlayMp3") {
      dbgprint("MP3 LOOP \n");
      myDFPlayer.enableLoopAll(); }

    if (text == "PauseMp3") { 
      dbgprint("MP3 PAUSE \n");
      myDFPlayer.pause(); }
    
    if (text == "StartMp3") { 
      dbgprint("MP3 RESTART \n"); 
      myDFPlayer.start(); }
    
    if (text == "StopMp3") { 
      dbgprint("MP3 STOP \n"); 
      myDFPlayer.stop(); }

  // ---------- VOLUME
    if (payload[0] == '*') {
      String setVol = String((char *) &payload[1]);
      int setVOL = setVol.toInt();
      myDFPlayer.volume(setVOL); 
      }

/* **************************** ANIMATIONS ***************************************************
********************************************************************************************* */

if (text == "AnimBM_on") {
  writeConfEEPROM(add_animBM,1);
  ANIM_BM = true; }

if (text == "AnimBM_off") {
  writeConfEEPROM(add_animBM,0);
  ANIM_BM = false; }

if (text == "AnimDATE_on") {
  writeConfEEPROM(add_animDATE,1);
  ANIM_DATE = true; }

if (text == "AnimDATE_off") {
  writeConfEEPROM(add_animDATE,0);
  ANIM_DATE = false; }

if (text == "AnimTEMP_on") {
  writeConfEEPROM(add_animTEMP,1);
  ANIM_TEMP = true; }

if (text == "AnimTEMP_off") {
  writeConfEEPROM(add_animTEMP,0);
  ANIM_TEMP = false; }

// --------------------------------------------------------------------------

     }                         // END WStype_TEXT 
  break;
  }                           // END switch
}                             // END webSocketEvent()

