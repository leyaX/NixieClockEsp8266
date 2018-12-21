        
void httpPresetRadio() {   

HTTPClient http;

  PRESET = String(preset);

   String presetToRadio = "http://192.168.43.45/?preset=" + PRESET;

   dbgprint("Connexion 192.168.43.45 => PRESET n° %s", PRESET.c_str());

        http.begin(presetToRadio); //HTTP
      
        dbgprint("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            dbgprint("[HTTP] GET... code: %d", httpCode);
           
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                dbgprint("%s \n", payload.c_str());
            }
        }
        
        else {
            dbgprint("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
            myDFPlayer.play(1);
        }

        http.end();
}

void httpStopRadio() {
HTTPClient http;

  http.begin("http://192.168.43.45/?stop"); //HTTP

        dbgprint("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();

        // httpCode will be negative on error
        if(httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            dbgprint("[HTTP] GET... code: %d\n", httpCode);
            
            // file found at server
            if(httpCode == HTTP_CODE_OK) {
                String payload = http.getString();
                dbgprint("%s \n",payload.c_str());

            }
        } else {
            dbgprint("[HTTP] GET... failed, erro \n");
        }

        http.end();
}

