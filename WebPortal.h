//Draws header
void SendHTML_Header() {
  webserver.sendHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  webserver.sendHeader("Pragma", "no-cache");
  webserver.sendHeader("Expires", "-1");
  webserver.setContentLength(CONTENT_LENGTH_UNKNOWN);
  webserver.send(200, "text/html", ""); // Empty content inhibits Content-length header so we have to close the socket ourselves.
  append_page_header();
  webserver.sendContent(webpage);
  webpage = "";
}


//Draws content
void SendHTML_Content() {
  webserver.sendContent(webpage);
  webpage = "";
}


//Stops the client
void SendHTML_Stop() {
  webserver.sendContent("");
  webserver.client().stop(); // Stop is needed because no content length was sent
}



//Shows the end user that the file does not exist
void ReportFileNotPresent(String target) {
  SendHTML_Header();
  webpage += F("<h3>File does not exist</h3>");
  webpage += F("<a href='/"); webpage += target + "'>[Back]</a><br><br>";
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}


//Select input for the download and delete page
void SelectInput(String heading1, String heading2, String command, String arg_calling_name) {
  dir = SPIFFS.openDir("/");
  SendHTML_Header();
  webpage += F("<h3 class='rcorners_m'>"); webpage += heading1 + "</h3><br>";
  webpage += F("<h3>"); webpage += heading2 + "</h3>";
  webpage += F("<FORM action='/"); webpage += command + "' method='post'>"; // Must match the calling argument e.g. '/chart' calls '/chart' after selection but with arguments!
  webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''><br>");
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''><br><br>");
  webpage += "<h2> Available files:</h2>";
  webpage += "<ol>";
  while (dir.next()) {
    if(dir.fileName().endsWith(".txt")){
      webpage += "<li>" + dir.fileName() + "</li> </br>";
    }
      
  }
  webpage += "</ol>";
  webpage += "</br>";
  
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}

//Shows free space to the user
void Free_space() {
  dir = SPIFFS.openDir("/");
  FSInfo fsInfo;
  SPIFFS.info(fsInfo);
  SendHTML_Header();
  webpage += F("<h3 class='rcorners_m'>"); webpage +="Free space</h3><br>";
  webpage += "<h1>Used space: " + String() + fsInfo.usedBytes + " bytes </h1>";
  webpage += "<h1>Total space: " + String() + fsInfo.totalBytes + "bytes </h1>";
  webpage += "<h1>Free left: " + String() + (fsInfo.totalBytes - fsInfo.usedBytes) + "bytes</h1>";
  webpage += "<h1>Free left percetage: " + String() + (100 - ((fsInfo.usedBytes/fsInfo.totalBytes)*100)) + "%</h1>";
  
  webpage += "<h2> Available files:</h2>";
  webpage += "<ol>";
  while (dir.next()) {
    webpage += "<li>" + dir.fileName() + "</li> </br>";
  }
  webpage += "</ol>";
  webpage += "</br>";
  
  append_page_footer();
  SendHTML_Content();
  SendHTML_Stop();
}

//Handles the file download
void SD_file_download(String filename) {
  if(!filename.startsWith("/")){
    filename = "/" + filename;
  }
  File download = SPIFFS.open(filename, "r");
  if (download) {
    webserver.sendHeader("Content-Type", "text/text");
    webserver.sendHeader("Content-Disposition", "attachment; filename=" + filename);
    webserver.sendHeader("Connection", "close");
    webserver.streamFile(download, "application/octet-stream");
    download.close();
  } else ReportFileNotPresent("download");
}

//Handles file delete
void SD_file_delete(String filename) {
  if(!filename.startsWith("/")){
    filename = "/" + filename;
  }
  File file = SPIFFS.open(filename, "r");
  if (file) {
    file.close();
    SPIFFS.remove(filename);
    webserver.sendHeader("Location","/download");
    pushAllFilesToJson();
  } else ReportFileNotPresent("delete");
}

//Gets args for the download
void File_Download() { // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments
  if (webserver.args() > 0 ) { // Arguments were received
    if (webserver.hasArg("download")){ 
      SD_file_download(webserver.arg(0));
      
    }
  }
  else SelectInput("File Download", "Enter filename to download", "download", "download");
}

//Gets args for the delete function
void File_Delete() { // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments
  if (webserver.args() > 0 ) { // Arguments were received
    if (webserver.hasArg("delete")){ 
      SD_file_delete(webserver.arg(0));
      
    }
  }
  else SelectInput("File Delete", "Enter filename to delete", "delete", "delete");
}


//Gets the content for the uploaded file
String getContentType(String filename) { // convert the file extension to the MIME type
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}


//Returns file to user if available
bool handleFileRead(String path) { // send the right file to the client (if it exists)
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";          // If a folder is requested, send the index file
  String contentType = getContentType(path);             // Get the MIME type
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) { // If the file exists, either as a compressed archive, or normal
    if (SPIFFS.exists(pathWithGz))                         // If there's a compressed version available
      path += ".gz";                                         // Use the compressed verion
    File file = SPIFFS.open(path, "r");                    // Open the file
    size_t sent = webserver.streamFile(file, contentType);    // Send it to the client
    file.close();                                          // Close the file again
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);   // If the file doesn't exist, return false
  return false;
}


//Handles file upload
void handleFileUpload(){ // upload a new file to the SPIFFS
  HTTPUpload& upload = webserver.upload();
  if(upload.status == UPLOAD_FILE_START){
    String filename = upload.filename;
    if(!filename.startsWith("/")) filename = "/"+filename;
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");            // Open the file for writing in SPIFFS (create if it doesn't exist)
    filename = String();
  } else if(upload.status == UPLOAD_FILE_WRITE){
    if(fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize); // Write the received bytes to the file
  } else if(upload.status == UPLOAD_FILE_END){
    if(fsUploadFile) {                                    // If the file was successfully created
      fsUploadFile.close();                               // Close the file again
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      webserver.sendHeader("Location","/success.html");      // Redirect the client to the success page
      webserver.send(303);
    } else {
      webserver.send(500, "text/plain", "500: couldn't create file");
    }
  }
}


//Listens for requests
void initiateAPI(){
  
    webserver.on("/upload", HTTP_GET, []() {                 // if the client requests the upload page
    if (!handleFileRead("/upload.html"))                // send it if it exists
      webserver.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
    });

   // webserver.on("/", HTTP_GET, []() {                 // if the client requests the upload page
   // if (!handleFileRead("/home.html"))                // send it if it exists
   //   webserver.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
   // });
  
    webserver.on("/upload", HTTP_POST,                       // if the client posts to the upload page
      [](){ webserver.send(200); },                          // Send status 200 (OK) to tell the client we are ready to receive
      handleFileUpload                                    // Receive and save the file
    );

  
    webserver.onNotFound([]() {                              // If the client requests any URI
      if (!handleFileRead(webserver.uri()))                  // send it if it exists
        webserver.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
    });

    webserver.on("/download", File_Download);
    webserver.on("/delete", File_Delete);
    webserver.on("/freespace", Free_space);
}
