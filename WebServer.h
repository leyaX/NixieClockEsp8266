
/*
 *        String getContentType(String filename)
 *        bool handleFileRead(String path) 
 *        void handleNotFound
 *        void handleFileUpload
 *        void startWebSocket
 *        void startServer
                                                         */


/* ------------------------------  getContentType() ----------------------------------------------------------------- */

String getContentType(String filename) { // determine the filetype of a given filename, based on the extension
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js"))  return "application/javascript";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".jpg")) return "image/jpeg";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz"))  return "application/x-gzip";
  else if (filename.endsWith(".xml")) return "txt/xml";
  return "text/plain";
}

/* ------------------------------- handleFileRead() ------------------------------------------------------------------ */

bool handleFileRead(String path) { // send the right file to the client (if it exists)
   dbgprint("handleFileRead: %s \n", path.c_str());
  if (path.endsWith("/")) path += "index.html";            // If a folder is requested, send the index file
  String contentType = getContentType(path);               // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {  // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                       // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = server.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    dbgprint("\tSent file: %s \n", path.c_str());
    return true;
  }
  dbgprint("\tFile Not Found: %s \n ", path.c_str());     // If the file doesn't exist, return false
  return false;
}

/* -------------------------------- handleNotFound() ------------------------------------------------------------------- */

void handleNotFound(){ // if the requested file or page doesn't exist, return a 404 not found error
  if(!handleFileRead(server.uri())){          // check if the file exists in the flash memory (SPIFFS), if so, send it
    server.send(404, "text/plain", "404: File Not Found");
  }
}

/* ------------------------------- handleFileUpload() ------------------------------------------------------------------ */

void handleFileUpload(){ // upload a new file to the SPIFFS
  HTTPUpload& upload = server.upload();
  String path;
  if(upload.status == UPLOAD_FILE_START){
    path = upload.filename;
    if(!path.startsWith("/")) path = "/"+path;
    if(!path.endsWith(".gz")) {                           // The file server always prefers a compressed version of a file 
      String pathWithGz = path+".gz";                     // So if an uploaded file is not compressed, the existing compressed
      if(SPIFFS.exists(pathWithGz))                       // version of that file must be deleted (if it exists)
         SPIFFS.remove(pathWithGz);
    }
    dbgprint("handleFileUpload Name: "); 
    dbgprint("%s \n", path.c_str());
    fsUploadFile = SPIFFS.open(path, "w");                // Open the file for writing in SPIFFS (create if it doesn't exist)
    path = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      dbgprint("handleFileUpload Size: ");
      dbgprint("%d \n", upload.totalSize);
      server.sendHeader("Location","/success.html");      // Redirect the client to the success page
      server.send(303);
    } else {
      server.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

/* -------------------------------   startWebSocket() --------------------------------------------------------------------------------------------------- */

void startWebSocket() {                       // Start a WebSocket server
  webSocket.begin();                          // start the websocket server
  webSocket.onEvent(webSocketEvent);          // if there's an incomming websocket message, go to function 'webSocketEvent'
  dbgprint("WebSocket server started. \n");
}

/* -------------------------------   startServer() and REQUESTS ------------------------------------------------------------------------------------------ */

void startServer() { // Start a HTTP server with a file read handler and an upload handler
 
  server.on("/wifivalues", send_wifi_values_html);                       // wifi info
  server.on("/ntpvalues",  send_ntp_values_html);                        // ntp info  
  server.on("/connectionstate", send_connection_state_values_html);      // wifi scan
  server.on("/submitWifi", send_network_configuration_html);             // wifi configuration

  // -------------------------------------------------
  server.on("/CONF/general.html" , []()
    {     
        String checkedBM, checkedDATE, checkedTEMP ;
        if (readConfEEPROM(add_animBM) == true) { checkedBM = "checked" ; } else { checkedBM = "" ; }  
        if (readConfEEPROM(add_animDATE) == true) { checkedDATE = "checked" ;  } else { checkedDATE = "" ; }
        if (readConfEEPROM(add_animTEMP) == true) { checkedTEMP = "checked" ; } else { checkedTEMP = "" ; }
        String body;
        body += "<hr style='width:100%;margin-top:30%'>" ;
        body += "<div class=toogleL><strong>Date</strong></div>";
        body += "<div class=toogleR><label class=switch><input type=checkbox id=AnimDATE oninput='Anim(\"AnimDATE\");'" ; 
        body += checkedDATE ;
        body += "><span class='slider round'></span></label></div><br><hr style=width:100%>" ; 
        body += "<div class=toogleL><strong>Température</strong></div>";
        body += "<div class=toogleR><label class=switch><input type=checkbox id=AnimTEMP oninput='Anim(\"AnimTEMP\");'" ; 
        body += checkedTEMP ;
        body += "><span class='slider round'></span></label></div><br><hr style=width:100%>" ;
        body += "<div class=toogleL><strong>Bandit Manchot</strong></div>";
        body += "<div class=toogleR><label class=switch><input type=checkbox id=AnimBM oninput='Anim(\"AnimBM\");'" ; 
        body += checkedBM ;
        body += "><span class='slider round'></span></label></div><br><hr style=width:100%>" ; 
        body +="<div style=float:bottom;box-shadow:none><a href=\"/admin.html\"><h2 class=collapsible style='margin-top:50px;width:200px;text-align:center'>EXIT</h2></a></div>";
        body += "</center></body></html>" ;
                
        server.send(200, "text/html", PAGE_INDEX_HTML + body);
    });

  // ------------------------------------------------
  
  server.onNotFound(handleNotFound);          // if someone requests any other file or page, go to function 'handleNotFound' and check if the file exists
  server.begin();                             // start the HTTP server
  dbgprint("HTTP server started. \n\n");
}

