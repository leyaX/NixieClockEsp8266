
/*
 *   void serverNTP
 *   void NTP_UPDATE_RTC
 *   void NTP_ClockWorld( String valueLand )
                                              */
                                     
/* ----------------------------------------------------------------- 
                         serverNTP()
   ----------------------------------------------------------------- */
 
void serverNTP(){
 NTP.begin ("fr.pool.ntp.org", 1, true, 0);
 NTP.setInterval (600);

 NTP.onNTPSyncEvent([](NTPSyncEvent_t error) {
  if (error) {
    dbgprint("Time Sync error: \n");
    if (error == noResponse)
      dbgprint("NTP server not reachable \n");
    else if (error == invalidAddress)
      dbgprint("Invalid NTP server address \n");
    }
  else {
    dbgprint("Got NTP time: %s \n", NTP.getTimeDateString(NTP.getLastNTPSync()).c_str() );
  }
});

} // End SERVEUR_NTP()

/* ----------------------------------------------------------------- 
                         NTP_UPDATE_RTC() 
   ----------------------------------------------------------------- */
   
void NTP_UPDATE_RTC() {
 if ( WiFi.status() == WL_CONNECTED ) {
   DateTime_t now;
   
   int firstValNTP, secondValNTP, thirdValNTP, fourValNTP, fiveValNTP, sixValNTP;
   int ntp_hours, ntp_minutes, ntp_seconds, ntp_days, ntp_months, ntp_year;
   String myString = NTP.getTimeStr();
   String myString2 = NTP.getTimeDateString();

     firstValNTP = myString.indexOf(':');
     ntp_hours = myString.substring(0, firstValNTP).toInt();
     secondValNTP = myString.indexOf(':',firstValNTP+1);
     ntp_minutes = myString.substring(firstValNTP+1,secondValNTP).toInt();
     thirdValNTP = myString.indexOf(':',secondValNTP+1);
     ntp_seconds = myString.substring(secondValNTP+1,thirdValNTP).toInt();

     fourValNTP = myString2.indexOf('/');
     ntp_days = myString2.substring(8, fourValNTP).toInt();
     fiveValNTP = myString2.indexOf('/',fourValNTP+1);
     ntp_months = myString2.substring(fourValNTP+1,fiveValNTP).toInt();
     sixValNTP = myString.indexOf('/',fiveValNTP+1);
     ntp_year = myString2.substring(fiveValNTP+1,sixValNTP).toInt();
   
    dbgprint("NTP_UPDATE_RTC() => %10s \n", myString2.c_str());

 if( ((ntp_hours == 7) && (ntp_hours == 28)) or ((ntp_hours == 0) && (ntp_minutes == 0)) or (ntp_year == 1970) )
   {
    dbgprint("Heure NTP non valide!!");
    dbgprint("FAIL => NTP_UPDATE_RTC() => DEBUG : %d:%d:%d \n" , ntp_hours, ntp_minutes, ntp_year);
    
    read_current_datetime(&now);
    heure_t = bcd_to_decimal(now.hours);          // It crashes we shoot in an hour !!!
    dbgprint("DEBUG heure_t : %d \n\n", heure_t);
   } 
    else {
     now.hours = ntp_hours;
     now.minutes= ntp_minutes;
     now.seconds = ntp_seconds;
     now.days = ntp_days;
     now.months = ntp_months;
     now.years = (ntp_year - 2000) ;
     adjust_current_datetime(&now);
     
     read_current_datetime(&now); 
     heure_t =  bcd_to_decimal(now.hours);
     dbgprint("DEBUG heure_t : %d \n", heure_t);
     dbgprint("SUCCESS => NTP_UPDATE_RTC() \n\n");
     
    } // end else
  }
} // End NTP_UPDATE_RTC()

