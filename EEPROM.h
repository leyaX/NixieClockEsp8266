
/* 
 *    void writeEEPROM
 *    void readEEPROM
 *    bool writeConfEEPROM 
 *    bool readConfEEPROM
 *    bool readWifi
 *    bool writeALARM
 *    bool readALARM
 *    void factoryReset
 *    void startCONF                                         */

/* ------------------------------------------------------------------------------------------------ */
void writeEEPROM(int startAdr, int laenge, char* writeString) {         // => for wifi EEPROM
  EEPROM.begin(512); //Max bytes of eeprom to use
  yield();
  for (int i = 0; i < laenge; i++)
       EEPROM.write(startAdr + i, writeString[i]);
  EEPROM.commit();
  EEPROM.end();
}

/* ------------------------------------------------------------------------------------------------ */
void readEEPROM(int startAdr, int maxLength, char* dest) {
  EEPROM.begin(512);
  delay(10);
  for (int i = 0; i < maxLength; i++)
      dest[i] = char(EEPROM.read(startAdr + i));
  EEPROM.end();
  dbgprint("%s \n",dest);
}

/* ------------------------------------------------------------------------------------------------ */
void writeConfEEPROM(int address, byte value) {
  EEPROM.begin(512);
  EEPROM.write(address, value);
  EEPROM.commit();
  EEPROM.end();
}

/* ------------------------------------------------------------------------------------------------ */
bool readConfEEPROM(int address) {
  EEPROM.begin(512);
  int  nameConf = EEPROM.read(address);
  dbgprint("READ EEPROM: %d %d \n",address, nameConf);
  EEPROM.end();
    if ( nameConf == 1) return true; else return false;
}

/* ------------------------------------------------------------------------------------------------ */
void readWifi() {
  dbgprint("READ \"ESSID \" EEPROM: ");
  readEEPROM(0, 32, ssid);
  dbgprint("READ \"PASSWD\" EEPROM: ");
  readEEPROM(32, 32, pass);
  dbgprint("\n");
}

/* ------------------------------------------------------------------------------------------------ */
void writeALARM( int h, int m, int zic, int RorM ){
   EEPROM.begin(512); 

   alarmROM New;
   strcpy( New.named , "Alarm New") ;
   New.h = h ;
   New.m = m ;
   New.zic = zic;
   New.RorM = RorM;
 
   EEPROM.put(add_alarm , New); 
//   delay(1000);
//   EEPROM.get(add_alarm, New);
   dbgprint("writeALARM() => ");
   dbgprint("Alarm : %s , %d:%d , Music/preset : %d , Radio/Mp3 : %d \n",
                      New.named, New.h, New.m, New.zic, New.RorM); 
   EEPROM.end();
}

/* ------------------------------------------------------------------------------------------------ */
void readALARME() {
 alarmROM New;
 EEPROM.begin(512); 
 EEPROM.get(add_alarm, New);
 EEPROM.get(add_alarmPROG, alarm_PROG);
 dbgprint("Found Alarm : %s \nHeure : %d:%d \nMusic Preset : %d \nRadio/Mp3 : %d \n", New.named, New.h, New.m, New.zic, RorM );
 EEPROM.end();
}

/* ------------------------------------------------------------------------------------------------ */
void factoryReset() {
  dbgprint("\n !!!!!!!!!!!!!!!!! FACTORY RESET !!!!!!!!!!!!!!!!!!!  \n\n");
 
 // => FANION  
  writeConfEEPROM(add_fanion,1);
  
 // => ANIM
  writeConfEEPROM(add_animBM,0);
  writeConfEEPROM(add_animDATE,0);
  writeConfEEPROM(add_animTEMP,0);
  
 // => ALARME
  writeALARM( 9, 30, 1, 1);
  
 // => WIFI
  strcpy(ssid, "AndroidYa");  
  writeEEPROM(0,32,ssid);       
  dbgprint("%s \n",ssid);       
  strcpy(pass, "lateteatoto");  
  writeEEPROM(32,32,pass); 
  dbgprint("%s \n",pass); 
}

/* ------------------------------------------------------------------------------------------------ */
void startCONF() {
  dbgprint("WIFI_________________________ \n");
  readWifi();
  
  dbgprint("ALARME_______________________  \n");
  readALARME();

  if(alarm_PROG == 1) { 
   dbgprint("ALARM ON \n\n");
   alarm_ON = 1;  
   alarm_check = 0;                
    } else {
   dbgprint("ALARM OFF \n\n");
    }

  dbgprint("NIXIE ON/OFF_________________ \n");
  readConfEEPROM(add_nixieH);
  readConfEEPROM(add_nixieM);
  nixieProg = readConfEEPROM(add_nixieProg);
  
   writeFileToggleNixieSPIFFS();
   if(nixieProg == 1) { 
    dbgprint("Nixie Prog ON \n\n");
    } else { 
    dbgprint("Nixie Prog OFF \n\n");
     }
   
  dbgprint("FANION_______________________ \n");
  if (readConfEEPROM(add_fanion) == false) { 
    factoryReset();
   } else { 
    dbgprint("Fanion OK \n\n");
   }
  
  dbgprint("ANIM_________________________ \n");
  if (readConfEEPROM(add_animDATE) == true) ANIM_DATE = true;
  if (readConfEEPROM(add_animTEMP) == true) ANIM_TEMP = true;
  if (readConfEEPROM(add_animBM) == true) ANIM_BM = true;
  dbgprint("_____________________________ \n\n"); 
}
