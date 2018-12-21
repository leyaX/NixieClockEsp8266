
/*
 *    void playerMP3
                        */


void playerMP3() {

  mySoftwareSerial.begin(9600);   // serial comm with DFPlayer
  delay(500);                    // delay because DFplayer is low to execute

/*
    if (!myDFPlayer.begin(mySoftwareSerial)) {  //Use softwareSerial to communicate with mp3.
     delay(500);                                // delay because DFplayer is low to execute
 
     while(true);
    }
*/

  myDFPlayer.begin(mySoftwareSerial);
  delay(500);

    //Set serial communication time out 500ms
    myDFPlayer.setTimeOut(500);

    //----Set different EQ----
    myDFPlayer.EQ(DFPLAYER_EQ_NORMAL);

    //----Set device we use SD as default----
    myDFPlayer.outputDevice(DFPLAYER_DEVICE_SD);

    //----Set Volume ----
    myDFPlayer.volume(24);
 
} // END DFPLAYER_MP3()

