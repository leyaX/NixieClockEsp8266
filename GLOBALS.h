
// -------------------- PLAYER MP3 -------------------------------------------------------------------
 SoftwareSerial mySoftwareSerial(2, 12, false, 256);    // pin 2 = D4 => TX , pin 12 = D6 => RX (dfplayer)
 DFRobotDFPlayerMini myDFPlayer;
 
// -------------------- WIFI CONFIG ------------------------------------------------------------------
 char ssid[32];
 char pass[32];
 const char* APssid = "NiXieClOcK_MODE_AP";  // IP: 192.168.4.1 (for AP MODE)
 IPAddress ip(192, 168, 43, 44);      // the desired IP Address
 IPAddress gateway(192, 168, 43, 1);  // set gateway to match your network
 IPAddress subnet(255, 255, 255, 0);  // set subnet mask to match your network

// -------------------- SERVEUR WEB ------------------------------------------------------------------
 ESP8266WebServer server(80);       // create a web server on port 80

// ------------------ SERVEUR WEBSOCKETS -------------------------------------------------------------
 WebSocketsServer webSocket(81);    // create a websocket server on port 81

// -------------------- HORLOGE ----------------------------------------------------------------------
 const int donnee = 13 ;                      // SER   => D7 
 const int verrou = 14 ;                      // RCLK  => D5
 const int horloge = 15 ;                     // SRCLK => D8

// -------------------- Variable ntpUpade -------------------------------------------------------                           
 int heure_t = 23 ;

// -------------------- ALARME -----------------------------------------------------------------------
 byte add_alarmPROG = 84;
 unsigned int add_alarm = 400;

 struct alarmROM {
    char named[20] = "Alarm Default";
    int h ; 
    int m ;
    int zic ;
    int RorM = 1 ;   // 0 => Radio ; 1 => MP3
   };
 
 int alarmH;
 int alarmM;
 
 byte alarm_PROG = 0 ; 
 byte alarm_ON = 0 ;
 byte alarm_check = 0 ; 
 
 // VARIABLES BUZZER
 byte PlayMp3 = true ;    //to avoid replaying the song several times in the alarm   => TODO!!!!!!!!!!!!!!!!!!!!!
  
 int mp3 = 1;

 bool radioOrMp3 = false;    // radio off by defaut
 int preset;
 String PRESET;

 int hours_alarm;     // alarm.html
 int mins_alarm;      //  " "
 String txt_alarm;    //  " "
 int RorM ;           // par default mp3

// -------------------- SPIFFS -----------------------------------------------------------------------
 File fsUploadFile;       // a File variable to temporarily store the received file

// ------------------- CHRONOMETER and TIMER --------------------------------------------------------
 unsigned long chronoTime, chronoTimeOld;        // milliseconds holder    // old milliseconds holder
  
 int sec_chrono = 0 ;
 int mins_chrono = 0 ;                                  
 int sec_timer = 0 ;
 int mins_timer = 60 ;                                 
 //int seconds_chrono, minutes_chrono ;

 bool blinkChrono = false ;
 bool affChrono = false ;
 bool startChrono = false ;
 bool blinkTimer = false ;
 bool affTimer =false ;
 bool startTimer = false ;

 String txt_timer ;

// ------------------- LAMPS Nixie -------------------------------------------------------------------
 bool lampNixie = true;
 int nixieH ;
 int nixieM ;
 byte nixieProg = 0;
 
// ------------------- EEPROM (see also variables in GLOBALS: ALARME) ----------------------------
 unsigned int add_fanion = 256 ;
 
 unsigned int add_animBM = 260 ;
 unsigned int add_animDATE = 270 ;
 unsigned int add_animTEMP = 272 ;
 
 bool ANIM_BM = false ;
 bool ANIM_DATE = false ;
 bool ANIM_TEMP = false ;

 unsigned int add_nixieH = 280 ;
 unsigned int add_nixieM = 290 ;
 unsigned int add_nixieProg = 300;

// -------------------- DEBUG (Messages on Monitor Serie) -------------------------------------------
int DEBUG = 1 ;  
