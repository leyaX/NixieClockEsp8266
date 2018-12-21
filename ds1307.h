#ifndef DS1307_H
#define DS1307_H

/* Address I2C of the mudule RTC DS1307 */
const uint8_t DS1307_ADDRESS = 0x68;

/* Address of the module control register RTC DS1307 */
const uint8_t DS1307_CTRL_REGISTER = 0x07;
//const uint8_t DS1307_CTRL_REGISTER = 0x0E; // Pour ds3231

/* Address and size of the module NVRAMe RTC DS1307 */
const uint8_t DS1307_NVRAM_BASE = 0x08;
const uint8_t DS1307_NVRAM_SIZE = 56;

/* Structure containing date and time information from or to the module RTC */
typedef struct {
  uint8_t seconds;      /**!< Secondes 00 - 59 */
  uint8_t minutes;      /**!< Minutes 00 - 59 */
  uint8_t hours;        /**!< Heures 00 - 23 (format 24h), 01 - 12 (format 12h) */
  uint8_t is_pm;        /**!< Vaut 1 si l'heure est en format 12h et qu'il est l'aprés midi, sinon 0 */
  uint8_t day_of_week;  /**!< Jour de la semaine 01 - 07, 1 = lundi, 2 = mardi, etc.  */
  uint8_t days;         /**!< Jours 01 - 31 */
  uint8_t months;       /**!< Mois 01 - 12 */
  uint8_t years;         /**!< Année au format yy (exemple : 16 = 2016) */
} DateTime_t;

/* Operating mode for SQW spindle */
typedef enum {
  SQW_1_HZ = 0,   /**!< Signal à 1Hz sur la broche SQW */
  SQW_4096_HZ,    /**!< Signal à 4096Hz sur la broche SQW */
  SQW_8192_HZ,    /**!< Signal à 8192Hz sur la broche SQW */
  SQW_32768_HZ,   /**!< Signal à 32768Hz sur la broche SQW */
  SQW_DC          /**!< Broche SQW toujours à LOW ou HIGH */
} DS1307_Mode_t;

/* Conversion function decimal -> BCD */
byte decimal_to_bcd(byte decimal) {
  return (decimal / 10 * 16) + (decimal % 10);
}

/* Conversion function BCD -> decimal */
byte bcd_to_decimal(byte bcd) {
  return (bcd / 16 * 10) + (bcd % 16); 
}

byte read_current_datetime(DateTime_t *datetime) {
  
  /* Start transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0); // Read address 0x00
  Wire.endTransmission(); // End transaction I2C
 
  /* Reads 7 bytes from the RTC module memory */
  Wire.requestFrom(DS1307_ADDRESS, (byte) 7);
  byte raw_seconds = Wire.read();
  datetime->seconds = raw_seconds;
  datetime->minutes = Wire.read();
  byte raw_hours = Wire.read();
  if (raw_hours & 64) { // Format 12h
    datetime->hours = (raw_hours & 31);
    datetime->is_pm = raw_hours & 32;
  } else { // Format 24h
    datetime->hours = (raw_hours & 63);
    datetime->is_pm = 0;
  }

  datetime->day_of_week = Wire.read();
  datetime->days = Wire.read();
  datetime->months = Wire.read();
  datetime->years =  Wire.read();
    
  /* If bit 7 of seconds == 1: the RTC module is stopped */
  return raw_seconds & 128;
}

void adjust_current_datetime(DateTime_t *datetime) {
   
  /* Start transaction I2C */
  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write((byte) 0);                                   // Write address 0x00
  Wire.write(decimal_to_bcd(datetime->seconds) & 127);    // CH = 0
  Wire.write(decimal_to_bcd(datetime->minutes));
  Wire.write(decimal_to_bcd(datetime->hours) & 63);       // Mode 24h
  Wire.write(decimal_to_bcd(datetime->day_of_week));
  Wire.write(decimal_to_bcd(datetime->days));
  Wire.write(decimal_to_bcd(datetime->months));
  Wire.write(decimal_to_bcd(datetime->years));
  Wire.endTransmission();                                 // End transaction I2C

} // End adjust_current_datetime()

/* ----------------------------------------------------------------- 
                         NIXIE LAMP
   ----------------------------------------------------------------- */

void affiche(unsigned int heure, unsigned int minutes) { 

  digitalWrite(verrou, LOW);
  // we send all the data thanks to shiftOut
  shiftOut(donnee, horloge, MSBFIRST, heure);
  shiftOut(donnee, horloge, MSBFIRST, minutes);
  // and finally we release the lock
  digitalWrite(verrou, HIGH);

} // End affiche()

/* ******************** TEMPERATURE ************************************ */

float get3231Temp() {
  
  byte tMSB, tLSB;
  float temp3231;

  Wire.beginTransmission(DS1307_ADDRESS);
  Wire.write(0x11);
  Wire.endTransmission();
  Wire.requestFrom(DS1307_ADDRESS, (byte) 2);
 
  if(Wire.available()) {
    tMSB = Wire.read(); //2's complement int portion
    tLSB = Wire.read(); //fraction portion
   
    temp3231 = (tMSB & B01111111); //do 2's math on Tmsb
    temp3231 += ( (tLSB >> 6) * 0.25 ); //only care about bits 7 & 8
  }
  return temp3231;
}

#endif /* DS */

