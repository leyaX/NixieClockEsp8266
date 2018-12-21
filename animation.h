
/* ************************** BANDIT MANCHOT ****************************************************** 
     ANIM_BM ; add_animBM 
   ************************************************************************************************ */
   
void animBanditM() {

    for(int i=0; i<=20; i++){
      int chiffre_aleatoire = decimal_to_bcd(rand()%99);
      affiche(chiffre_aleatoire,chiffre_aleatoire);
      delay(50);
    }
}                                              // END animBanditM()

/* ------------------------------------------------------------------------------------------------ */

void Chrono() {
  
 if (chronoTime - chronoTimeOld > 1000) {
  chronoTimeOld = chronoTime;                 
  sec_chrono++;
  if (sec_chrono > 59) { 
    sec_chrono = 0;
    mins_chrono++;   

    if (mins_chrono > 59) mins_chrono = 0;  
   }
  } 
}                                              // END chronometre()

/* ------------------------------------------------------------------------------------------------ */

void BlinkChrono() {
  
  if (chronoTime - chronoTimeOld > 500) {
     affiche(decimal_to_bcd(mins_chrono), decimal_to_bcd(sec_chrono));
   }
 
  if (chronoTime - chronoTimeOld > 1000)
   {
     chronoTimeOld = chronoTime;  
     affiche(999999999,999999999);
   }
}                                              // END afficheBlinkChrono() 

/* ------------------------------------------------------------------------------------------------ */

void afficheChrono() {
 affiche(decimal_to_bcd(mins_chrono), decimal_to_bcd(sec_chrono));
}                                              // END afficheChrono() 

/* ------------------------------------------------------------------------------------------------ */

void BlinkTimer() {
  
  if (chronoTime - chronoTimeOld > 500) {
     affiche(decimal_to_bcd(mins_timer), decimal_to_bcd(sec_timer));
   }

  if (chronoTime - chronoTimeOld > 1000)
   {
     chronoTimeOld = chronoTime;   
     affiche(999999999,999999999);
   }
}                                               // END afficheBlinkTimer

/* ------------------------------------------------------------------------------------------------ */

void Timer() {
  
  if (chronoTime - chronoTimeOld > 1000){
    
   chronoTimeOld = chronoTime;                  

   sec_timer--; 
   
    if (sec_timer < 0) { 
      sec_timer = 59 ; mins_timer-- ; 
      }
   
    if ( sec_timer == 0 && mins_timer == 0 ) { 
     startTimer = false;
     affTimer = false; 
     
     alarm_ON = 1;

     myDFPlayer.loop(mp3);

     }
   } 
} // END timer()

/* ------------------------------------------------------------------------------------------------ */

void afficheTimer() {
 affiche(decimal_to_bcd(mins_timer), decimal_to_bcd(sec_timer));
} // END afficheTimer()

/* ------------------------------------------------------------------------------------------------ */

void timerChrono() {

  if (blinkChrono == true) BlinkChrono();   
  
  if (affChrono == true) afficheChrono(); 

  if (startChrono == true) Chrono(); 

  if (blinkTimer == true) BlinkTimer(); 
  
  if (affTimer == true) afficheTimer(); 

  if (startTimer == true) Timer(); 
}

/*---------------------------- Temperature ---------------------------------------------------------- */
   
void animTEMP() {   

   float tempDS = get3231Temp();
   
   char tempChar[5];                                            // Conversion Float → String: dtostrf( )
   String tempStr = dtostrf(tempDS, 5, 2, tempChar);      // https://blog.samuel.domains/blog/programming/dtostrf-lexplication
     
      int secondValTemp = 0; 
      int firstValTemp = 0 ;
   
   for (uint8_t i = 0; i < tempStr.length(); i++)
      if (tempStr.substring(i, i + 1) == ".")                   // Parser "dot"
          {                                         
          firstValTemp = tempStr.substring(0, i).toInt();
          secondValTemp = tempStr.substring(i + 1).toInt();
          }
 
   affiche(decimal_to_bcd(firstValTemp),  decimal_to_bcd(secondValTemp));
   delay(3000);
}

/* ******************************** DATE ******************************************************** */

void animDATE() {
     DateTime_t now;
     read_current_datetime(&now);
     affiche(now.days, now.months);
     delay(3000);
     affiche(decimal_to_bcd(20), now.years);
     delay(3000);
}

