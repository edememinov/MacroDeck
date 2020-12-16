//Sets error to true to let the user know something is wrong
void setErrorToTrue(){
  error = true;
  Serial.println(error);
}

//All txt files with the exception of config.txt are added to an arraylist
void pushAllFilesToJson(){
  dir = SPIFFS.openDir("/");
  int i = 0;
  while (dir.next()) {
    if(dir.fileName().indexOf(".txt") > 0 && dir.fileName().indexOf("config.txt") == -1){
      if(dir.fileName().indexOf("button_config.txt") == -1){
          BUTTON_JSON[i] = String() + dir.fileName();
          i++;
          amount_of_files = i;
      }
     
    }
      
  }

  if(amount_of_files < 1){
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("No .txt files have been found");
    setErrorToTrue();    
  }
}


//The button files are desirialized to json
void getButtonJSON(){
  if(amount_of_files > 0){
    if(SPIFFS.exists(BUTTON_JSON[currentFile])){
      File file = SPIFFS.open(BUTTON_JSON[currentFile], "r");
        // Allocate a temporary JsonDocument
      // Don't forget to change the capacity to match your requirements.
      // Use arduinojson.org/v6/assistant to compute the capacity.
      
    
      // Deserialize the JSON document
      DeserializationError error = deserializeJson(doc, file);
      if (error){
        tft.setTextColor(TFT_RED, TFT_WHITE);
        tft.println("Error parsing file: " + BUTTON_JSON[currentFile]);
        tft.println(error.c_str());
        setErrorToTrue();
        delay(5000);
        
      }
      // Close the file (Curiously, File's destructor doesn't close the file)
      file.close(); 
    }
    else{
      setErrorToTrue();
    } 
  }
  else{
      setErrorToTrue();
    }
}

//The config file is desirialized to json
void getConfigJSON(){
  if(SPIFFS.exists(configFile)){
    File file = SPIFFS.open(configFile, "r");
    DeserializationError error = deserializeJson(configDoc, file);
    if (error){
      tft.setTextColor(TFT_RED, TFT_WHITE);
      tft.println("Error parsing config file");
      tft.println(error.c_str());
      setErrorToTrue();
      delay(5000);  
    }
    file.close();
  }
  else{
    setErrorToTrue();
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("config.txt not found");
    delay(5000); 
  }
}

//The config file is desirialized to json
void getButtonConfigJSON(){
  if(SPIFFS.exists(buttonConfigFile)){
    File file = SPIFFS.open(buttonConfigFile, "r");
    DeserializationError error = deserializeJson(buttonConfigDoc, file);
    if (error){
      tft.setTextColor(TFT_RED, TFT_WHITE);
      tft.println("Error parsing button_config file");
      tft.println(error.c_str());
      setErrorToTrue();
      delay(5000);  
    }
    file.close();
  }
  else{
    setErrorToTrue();
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("button_config.txt not found");
    tft.println("Buttons will not work");
    delay(5000); 
  }
}

//File changes based on user input
void chooseFile(int keyNumber){
  currentFile = keyNumber - 1;
  if(amount_of_files < keyNumber){
    currentFile = 0;
  }
  getButtonJSON();
  interateOverPages();
}
