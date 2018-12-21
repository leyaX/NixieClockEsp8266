
/*
 *    String formatBytes
 *    void startSPIFFS()
 *    void writeFileTimerSPIFFS
 *    void writeFileAlarmSPIFFS
 *    //void writeFileToogleAlarmSPIFFS
 *    void writeFileToogleNixiesSPIFFS
 *    boolean removeFilesSPIFFS
                                      */
  // Modes:
  // "r"   Opens a file for reading. The file must exist.
  // "w"   Creates an empty file for writing. If a file with the same name already exists, its content is erased and the file is considered as a new empty file.
  // "a"   Appends to a file. Writing operations, append data at the end of the file. The file is created if it does not exist.
  // "r+"  Opens a file to update both reading and writing. The file must exist.
  // "w+"  Creates an empty file for both reading and writing.
  // "a+"  Opens a file for reading and appending.:
                                   
/* ------------------------------------------------------------------------------------------------ */

String formatBytes(size_t bytes) { // convert sizes in bytes to KB and MB
  if (bytes < 1024) {
    return String(bytes) + "B";
  } else if (bytes < (1024 * 1024)) {
    return String(bytes / 1024.0) + "KB";
  } else if (bytes < (1024 * 1024 * 1024)) {
    return String(bytes / 1024.0 / 1024.0) + "MB";
  }else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

/* ------------------------------------------------------------------------------------------------ */

void startSPIFFS() { // Start the SPIFFS and list all contents
  SPIFFS.begin();                             // Start the SPI Flash File System (SPIFFS)
  dbgprint("SPIFFS started. Contents: \n");
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {                      // List the file system contents
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      dbgprint("\tFS File: %s, size: %s\r\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    dbgprint("\n"); 

   // Total size of Spiffs space and size of used space 
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    dbgprint("SPIFFS_______________________ \n");
    dbgprint("Total : %s \n", formatBytes(fs_info.totalBytes).c_str());
    dbgprint("Used  : %s \n\n", formatBytes(fs_info.usedBytes).c_str());
  }
}

/* ------------------------------------------------------------------------------------------------ */

// TOOGLE SWITCH NIXIES => nixieToogle.html

void writeFileToggleNixieSPIFFS() {
  if(nixieProg == 1){ 
    File f = SPIFFS.open("/nixieToogle.html", "w");  
    f.print("<label class=\"switch\"><input type=\"checkbox\" id=\"checkboxNixie\" onclick=\"progNIXIE()\" checked><span class=\"slider\"></span></label>");
    f.close();
     dbgprint("Création nixie.html toogle checked \n");
   } else 
    {
    File f = SPIFFS.open("/nixieToogle.html", "w");
    f.print("<label class=\"switch\"><input type=\"checkbox\" id=\"checkboxNixie\" onclick=\"progNIXIE()\" ><span class=\"slider\"></span></label>");
   f.close();
    dbgprint("Création nixie.html toogle unchecked \n");
    }
}

/* ------------------------------------------------------------------------------------------------ */

// ALARM SPIFFS => alarm.html

void writeFileAlarmSPIFFS( uint16_t hours_alarm, uint16_t mins_alarm,  String txt_alarm ) {
    File f  = SPIFFS.open("/alarm.html", "a");   
    f.print("<div align=left style=margin-top:5px>");
    f.print("<label class=\"switch\"><input name='toogleAlarmCheck' type=\"checkbox\" onclick=\"initALARME('");
    f.print(hours_alarm);
    f.print("','");
    f.print(mins_alarm);
    f.print("','");
    f.print(txt_alarm);
    f.print("')\" id=\"");
    f.print(txt_alarm);
    f.print("\"><span class=\"slider\"></span></label>");
    f.print("<strong  style=\"padding-left:30px;\" id=\"set_txt_alarm\">");
    f.print(txt_alarm);
    f.print("</strong>");
    f.print("<strong style=\"float:right;\">");
   if (hours_alarm < 10) f.print("0");
    f.print(hours_alarm);
    f.print(":");
   if (mins_alarm < 10 ) f.print("0");
    f.print(mins_alarm);
    f.print("</strong></div>");
   f.close();
}

/* ------------------------------------------------------------------------------------------------ */

// TIMER SPIFFS => timer.html

void writeFileTimerSPIFFS( uint16_t mins_timer, uint16_t sec_timer,  String txt_timer ) {
    File f  = SPIFFS.open("/timer.html", "a");
    f.print("<div align=left style=margin-top:5px>");
    f.print("<label class=\"switch\"><input name='toogleTimerCheck' type=\"checkbox\" onclick=\"initTIMER('");
    f.print(mins_timer);
    f.print("','");
    f.print(sec_timer);
    f.print("','");
    f.print(txt_timer);
    f.print("')\" id=\"");
    f.print(txt_timer);
    f.print("\"><span class=\"slider\"></span></label>");
    f.print("<strong style=\"padding-left:30px;\" id=\"set_txt_Timer\">");
    f.print(txt_timer);
    f.print("</strong>");
    f.print("<strong style=\"float:right;\">");
   if (mins_timer < 10) f.print("0");
    f.print(mins_timer);
    f.print(":");
   if (sec_timer < 10 ) f.print("0");
    f.print(sec_timer);
    f.print("</strong></div>");
   f.close();
}

/* ---------------------------------- REMOVE FILE --------------------------------------------------- */

boolean removeFilesSPIFFS(String nameFile){
  SPIFFS.remove(nameFile);                         //  => Destruction of the file (nameFile)
  dbgprint("Remove: %s", nameFile.c_str());
 if (nameFile == "/alarm.html") {                
   File f  = SPIFFS.open("/alarm.html", "a");      // => Creation of the new default "alarm.html" file
   f.print("<div align=left><label class=\"switch\"><input name=toto type=checkbox id='checkboxAlarmeDefault' oninput=\"initALARME('09','00','checkboxAlarmeDefault')\">");
   f.print("<span class=\"slider\"></span></label><strong style=\"padding-left:30px;\">Default</strong>");
   f.print("<strong style=\"float:right;\">09:00</strong></div>");
  f.close();
   dbgprint("Creation du nouveau fichier alarm.html par defaut \n\n");
   }
 if (nameFile == "/timer.html") {             
   File f  = SPIFFS.open("/timer.html", "a");      // => Creation of the new default "timer.html" file
   f.print("<div align=left><label class=\"switch\"><input type=checkbox id=\"checkboxTimerDefault\" oninput=\"initTIMER('04','00','checkboxTimerDefault')\">");
   f.print("<span class=\"slider\"></span></label><strong style=\"padding-left:30px;\">Default</strong>");
   f.print("<strong style=\"float:right;\">04:00</strong></div>");
  f.close();  
   dbgprint("Creation du fichier timer.html par defaut \n\n ");
   }
  return true;
}

/* ------------------------------------------------------------------------------------------------ */

// ALARM SPIFFS =>alarm.json
/*
void writeFileAlarmSPIFFS( uint16_t hours_timer, uint16_t mins_timer,  String txt_alarm ) {
  
   File f  = SPIFFS.open("/alarm.json", "w");
 
    f.print("{ \"alarme\": [ { \"checked\"  : \"\", \"heure\"  : \"");
    if (hours_alarm < 10) f.print("0");
    f.print(hours_alarm);
    f.print("\", \"minutes\"  : \"");
    if (mins_alarm < 10) f.print("0");
    f.print(mins_alarm);
    f.print("\",  \"named\" : \"");
    f.print(txt_alarm);
    f.print("\" } ] } ");
        
   f.close();
}
*/
