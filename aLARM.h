/*
 *   void Buzzer
 *   void ALARM_SET
 */
/* ----------------------------------------------------------------- 
                            BUZZER
   ----------------------------------------------------------------- */
   
void Buzzer() {    // MP3 playback function
 if (PlayMp3 == true ) {
   myDFPlayer.loop(mp3); 
   PlayMp3 = false;
 }
} // END Buzzer()

/* ----------------------------------------------------------------- 
                            ALARM SET
   ----------------------------------------------------------------- */

void ALARM_SET() {
   
  alarmROM New;  // STRUCT alarmROM    

  if (alarm_check == 0) {                 // check time alarme   
    EEPROM.begin(512);
    EEPROM.get(add_alarm, New);
    
    alarmH = New.h;     // Variables to compare with the RTC time
    alarmM = New.m;    //  "     "
    
    dbgprint("ALARM_SET() => %d:%d \n", alarmH, alarmM);
    
   if ( New.RorM == 1 ) {                // Si 1 => Radio defined
      radioOrMp3 = true;                 // 'true' for Radio   
      preset = New.zic;
      dbgprint("ALARM_SET() => Radio Preset n° %d \n\n", preset);
     } else 
      {                                  // Si 0 => MP3 defined
      radioOrMp3 = false;                // 'false' for MP3  
      mp3 = New.zic; 
      dbgprint("ALARM_SET() => MP3 track n° %d \n\n", mp3);
      }
     
    EEPROM.end();

    alarm_check = 1;
}

  DateTime_t now;
  read_current_datetime(&now);
      
  if (alarmH == (bcd_to_decimal(now.hours )) &&  alarmM == (bcd_to_decimal(now.minutes))  )
   { 
    if (radioOrMp3 == false) {
       Buzzer();    // Lecteur MP3 
      }else 
       {
       httpPresetRadio();         // Radio via ESP-Radio => 193.168.43.45
       }
  
    alarm_ON = 0;
    
    webSocket.broadcastTXT("S");  // send All clients "S" for "PoPuP" 'stop_alarm.html'
  
  }
}  // END ALARM_SET()

