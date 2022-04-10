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
  webserver.client().stop();
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
  webpage += F("<FORM action='/"); webpage += command + "' method='post'>";
  webpage += F("<input type='text' name='"); webpage += arg_calling_name; webpage += F("' value=''><br>");
  webpage += F("<type='submit' name='"); webpage += arg_calling_name; webpage += F("' value=''><br><br>");
  webpage += "<h2> Available files:</h2>";
  webpage += "<ol>";
  while (dir.next()) {
    if (dir.fileName().endsWith(".txt")) {
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
  webpage += F("<h3 class='rcorners_m'>"); webpage += "Free space</h3><br>";
  webpage += "<h1>Used space: " + String() + fsInfo.usedBytes + " bytes </h1>";
  webpage += "<h1>Total space: " + String() + fsInfo.totalBytes + "bytes </h1>";
  webpage += "<h1>Free left: " + String() + (fsInfo.totalBytes - fsInfo.usedBytes) + "bytes</h1>";
  webpage += "<h1>Free left percetage: " + String() + (100 - ((fsInfo.usedBytes / fsInfo.totalBytes) * 100)) + "%</h1>";

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
  if (!filename.startsWith("/")) {
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
  if (!filename.startsWith("/")) {
    filename = "/" + filename;
  }
  File file = SPIFFS.open(filename, "r");
  if (file) {
    file.close();
    SPIFFS.remove(filename);
    webserver.sendHeader("Location", "/success.html");
    //Reload all files
    pushAllFilesToJson();
    getButtonJSON();
    getConfigJSON();
  } else ReportFileNotPresent("delete");
}

//Gets args for the download
void File_Download() {
  if (webserver.args() > 0 ) { // Arguments were received
    if (webserver.hasArg("download")) {
      SD_file_download(webserver.arg(0));

    }
  }
  else SelectInput("File Download", "Enter filename to download", "download", "download");
}

//Gets args for the delete function
void File_Delete() {
  if (webserver.args() > 0 ) {
    if (webserver.hasArg("delete")) {
      SD_file_delete(webserver.arg(0));

    }
  }
  else SelectInput("File Delete", "Enter filename to delete", "delete", "delete");
}

//Gets the content for the uploaded file
String getContentType(String filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

void setCrossOrigin() {
  webserver.sendHeader(F("Access-Control-Allow-Origin"), F("*"));
  webserver.sendHeader(F("Access-Control-Max-Age"), F("10000"));
  webserver.sendHeader(F("Access-Control-Allow-Methods"), F("*"));
  webserver.sendHeader(F("Access-Control-Allow-Headers"), F("*"));
};

//Returns file to user if available
bool handleFileRead(String path) {
  setCrossOrigin();
  Serial.println("handleFileRead: " + path);
  if (path.endsWith("/")) path += "index.html";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = webserver.streamFile(file, contentType);
    file.close();
    Serial.println(String("\tSent file: ") + path);
    return true;
  }
  Serial.println(String("\tFile Not Found: ") + path);
  return false;
}

//Handles file upload
void handleFileUpload() {
  HTTPUpload& upload = webserver.upload();
  if (upload.status == UPLOAD_FILE_START) {
    String filename = upload.filename;
    if (!filename.startsWith("/")) filename = "/" + filename;
    Serial.print("handleFileUpload Name: "); Serial.println(filename);
    fsUploadFile = SPIFFS.open(filename, "w");
    filename = String();
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (fsUploadFile)
      fsUploadFile.write(upload.buf, upload.currentSize);
  } else if (upload.status == UPLOAD_FILE_END) {
    if (fsUploadFile) {
      fsUploadFile.close();
      Serial.print("handleFileUpload Size: "); Serial.println(upload.totalSize);
      webserver.sendHeader("Location", "/success.html");
      webserver.send(303);
      //Reload all files
      pushAllFilesToJson();
      getButtonJSON();
      getConfigJSON();
    } else {
      webserver.send(500, "text/plain", "500: couldn't create file");
    }
  }
}

//Listens for requests
void initiateAPI() {

  webserver.on("/upload", HTTP_GET, []() {
    if (!handleFileRead("/upload.html"))
      webserver.send(404, "text/plain", "404: Not Found");
  });

  webserver.on("/upload", HTTP_POST,
  []() {
    webserver.send(200);
  },
  handleFileUpload
              );


  webserver.on("/getoptions", HTTP_GET, []() {
    setCrossOrigin();
    String buf;
    serializeJson(configDoc, buf);
    webserver.send(200, F("application/json"), buf);
  });

  webserver.on("/setoptions", HTTP_POST, []() {
    setCrossOrigin();
    String postBody = webserver.arg("plain");
    DeserializationError error = deserializeJson(configDoc, postBody);
    if (error) {
      webserver.send(404, "text/plain", "404: Not Found");
    }
    writeToConfigFile();
    webserver.send(200);
    delay(500);
    ESP.restart();
  });

  webserver.on("/getbuttonoptions", HTTP_GET, []() {
    setCrossOrigin();
    String buf;
    serializeJson(buttonConfigDoc, buf);
    webserver.send(200, F("application/json"), buf);
  });


  webserver.on("/version", HTTP_GET, []() {
    setCrossOrigin();
    webserver.send(200, F("text/plain"), FirmwareVersion);
  });

  webserver.on("/setbuttonoptions", HTTP_POST, []() {
    setCrossOrigin();
    String postBody = webserver.arg("plain");
    DeserializationError error = deserializeJson(buttonConfigDoc, postBody);
    if (error) {
      webserver.send(404, "text/plain", "404: Not Found");
    }
    writeToButtonConfigFile();
    webserver.send(200);
    delay(500);
    ESP.restart();
  });

  webserver.on("/getfiles", HTTP_GET, []() {
    setCrossOrigin();
    const size_t CAPACITY = JSON_ARRAY_SIZE(15);
    // allocate the memory for the document
    StaticJsonDocument<CAPACITY> buttonNamesDoc;
    JsonArray buttonNames = buttonNamesDoc.to<JsonArray>();

    for (int x = 0; x < amount_of_files; x++) {
      buttonNames.add(BUTTON_JSON[x]);
    }
    String buf;
    serializeJson(buttonNames, buf);
    webserver.send(200, F("application/json"), buf);
    buttonNamesDoc.clear();
  });

  webserver.on("/createnewfile", HTTP_POST, []() {
    setCrossOrigin();
    String postBody = webserver.arg("plain");
    createNewButtonFile(postBody);
    webserver.send(200);
    delay(500);
    ESP.restart();
  });


  webserver.on("/deletefile", HTTP_POST, []() {
    setCrossOrigin();
    String postBody = webserver.arg("plain");
    deleteButtonFile(postBody);
    webserver.send(200);
    delay(500);
    ESP.restart();
  });

  webserver.on("/editfile", HTTP_POST, []() {
    setCrossOrigin();
    String postBody = webserver.arg("plain");
    writeToButtonFile(postBody);
    webserver.send(200);
    delay(500);
    ESP.restart();
  });

  webserver.onNotFound([]() {
    if (webserver.method() == HTTP_OPTIONS)
    {
      webserver.sendHeader("Access-Control-Allow-Origin", "*");
      webserver.sendHeader("Access-Control-Max-Age", "10000");
      webserver.sendHeader("Access-Control-Allow-Methods", "*");
      webserver.sendHeader("Access-Control-Allow-Headers", "*");
      webserver.send(204);
    }
    else
    {
      if (!handleFileRead(webserver.uri()))
        webserver.send(404, "text/plain", "404: Not Found");
    }

  });

  webserver.on("/download", File_Download);
  webserver.on("/delete", File_Delete);
  webserver.on("/freespace", Free_space);
}
