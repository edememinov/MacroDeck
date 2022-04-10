//Sets error to true to let the user know something is wrong
void setErrorToTrue() {
  error = true;
  Serial.println(error);
}

void writeLogTimeAlive(){
   if (SPIFFS.exists("/time_alive.txt")){
      SPIFFS.remove("/time_alive.txt");
   }
  File file = SPIFFS.open("/time_alive.txt", "w");
  file.print(millis());
  DeserializationError error = deserializeJson(aliveSince, file);
  file.close();
}

void readLogTimeAlive(){
  if (!SPIFFS.exists("/time_alive.txt")){
      writeLogTimeAlive();
   }
  File file = SPIFFS.open("/time_alive.txt", "r");
  DeserializationError error = deserializeJson(aliveSince, file);
  file.close();
}

//All txt files with the exception of config.txt are added to an arraylist
void pushAllFilesToJson() {
  dir = SPIFFS.openDir("/");
  int i = 0;
  while (dir.next()) {
    if (dir.fileName().indexOf(".txt") > 0 && dir.fileName().indexOf("config.txt") == -1) {
      if (dir.fileName().indexOf("button_config.txt") == -1) {
        BUTTON_JSON[i] = String() + dir.fileName();
        i++;
        amount_of_files = i;
      }

    }

  }

  if (amount_of_files < 1) {
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("No .txt files have been found");
    setErrorToTrue();
  }
}


//The button files are desirialized to json
void getButtonJSON() {
  if (amount_of_files > 0) {
    if (SPIFFS.exists(BUTTON_JSON[currentFile])) {
      File file = SPIFFS.open(BUTTON_JSON[currentFile], "r");
      // Allocate a temporary JsonDocument
      // Don't forget to change the capacity to match your requirements.
      // Use arduinojson.org/v6/assistant to compute the capacity.


      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, file);
      if (error) {
        tft.setTextColor(TFT_RED, TFT_WHITE);
        tft.println("Error parsing file: " + BUTTON_JSON[currentFile]);
        tft.println(error.c_str());
        setErrorToTrue();
        delay(5000);

      }
      // Close the file (Curiously, File's destructor doesn't close the file)
      file.close();
    }
    else {
      setErrorToTrue();
    }
  }
  else {
    setErrorToTrue();
  }
}

//The config file is desirialized to json
void getConfigJSON() {
  if (SPIFFS.exists(configFile)) {
    File file = SPIFFS.open(configFile, "r");
    DeserializationError error = deserializeJson(configDoc, file);
    if (error) {
      tft.setTextColor(TFT_RED, TFT_WHITE);
      tft.println("Error parsing config file");
      tft.println(error.c_str());
      setErrorToTrue();
      delay(5000);
    }
    file.close();
  }
  else {
    setErrorToTrue();
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("config.txt not found");
    delay(5000);
  }
}

//The config file is desirialized to json
void getButtonConfigJSON() {
  if (SPIFFS.exists(buttonConfigFile)) {
    File file = SPIFFS.open(buttonConfigFile, "r");
    DeserializationError error = deserializeJson(buttonConfigDoc, file);
    if (error) {
      tft.setTextColor(TFT_RED, TFT_WHITE);
      tft.println("Error parsing button_config file");
      tft.println(error.c_str());
      setErrorToTrue();
      delay(5000);
    }
    file.close();
  }
  else {
    setErrorToTrue();
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("button_config.txt not found");
    tft.println("Buttons will not work");
    delay(5000);
  }
}

//File changes based on user input
void chooseFile(int keyNumber) {
  currentFile = keyNumber - 1;
  if (amount_of_files < keyNumber) {
    currentFile = 0;
  }
  getButtonJSON();
  interateOverPages();
}

void writeToButtonConfigFile() {
  File file = SPIFFS.open(buttonConfigFile, "w");
  serializeJsonPretty(buttonConfigDoc, file);

  tft.println("Writing to doc");
  tft.println(error);
  file.close();
}
void writeToConfigFile() {
  File file = SPIFFS.open(configFile, "w");
  serializeJsonPretty(configDoc, file);

  tft.println("Writing to doc");
  tft.println(error);
  file.close();
}

int getPageIndex(String fileName) {
  if (!fileName.startsWith("/")) {
    fileName = "/" + fileName;
  }
  if (!fileName.endsWith(".txt")) {
    fileName = fileName + ".txt";
  }
  for (int x = 0; x < amount_of_files; x++) {
    if (fileName == BUTTON_JSON[x]) {
      return x;
    }
  }
}

void writeToButtonFile(String jsonButtons) {
  
  DynamicJsonDocument docBackUp(10422);
  DeserializationError error = deserializeJson(docBackUp, jsonButtons);
  int index = getPageIndex(docBackUp["fileName"]);
  File file = SPIFFS.open(BUTTON_JSON[index], "w");
  docBackUp.remove("fileName");
  serializeJsonPretty(docBackUp, file);
  docBackUp.clear();
  file.close();
}

void createNewButtonFile(String jsonButtons){
  DynamicJsonDocument docBackUp(10422);
  DeserializationError error = deserializeJson(docBackUp, jsonButtons);
  String fileName = docBackUp["fileName"];
  if (!fileName.startsWith("/")) {
    fileName = "/" + fileName;
  }
  if (!fileName.endsWith(".txt")) {
    fileName = fileName + ".txt";
  }
  File file = SPIFFS.open(fileName, "w");
  docBackUp.remove("fileName");
  serializeJsonPretty(docBackUp, file);
  docBackUp.clear();
  file.close();
}

void deleteButtonFile(String jsonButtons){
  DynamicJsonDocument docBackUp(10422);
  DeserializationError error = deserializeJson(docBackUp, jsonButtons);
  String fileName = docBackUp["fileName"];
  if (!fileName.startsWith("/")) {
    fileName = "/" + fileName;
  }
  if (!fileName.endsWith(".txt")) {
    fileName = fileName + ".txt";
  }
  
  if (dir.fileName().indexOf("button_config.txt") == -1){
    if (dir.fileName().indexOf("config.txt") == -1){
      SPIFFS.remove(fileName);
    }
  }
  docBackUp.clear();
}
