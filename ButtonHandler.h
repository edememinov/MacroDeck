//Registers a button press 
void handleButtonPress(int keyNumber){
  if(chosingFile){
      chosingFile = false;
      chooseFile(keyNumber);
   }
  
  else if(altKeyPressed && !chosingFile){
    handleBoardChange(keyNumber);
    altKeyPressed = false;
  }
  else if(!altKeyPressed && !chosingFile){
    if(buttons["pages"][page]["buttons"][keyNumber - 1]["type"].as<String>() == "socket"){
      handleSocketCommand(buttons["pages"][page]["buttons"][keyNumber - 1]["command_id"].as<String>());
    }else if(buttons["pages"][page]["buttons"][keyNumber - 1]["type"].as<String>() == "apiCall"){
      if(buttons["pages"][page]["buttons"][keyNumber - 1].containsKey("fingerprint")){
        handleApiCall(buttons["pages"][page]["buttons"][keyNumber - 1]["url"].as<String>(), buttons["pages"][page]["buttons"][keyNumber - 1]["fingerprint"].as<String>());
      }else{
        handleApiCall(buttons["pages"][page]["buttons"][keyNumber - 1]["url"].as<String>());
      }
    }
    else{
        tft.setTextColor(TFT_RED, TFT_WHITE);
        tft.println("This file is not compatible: " + BUTTON_JSON[currentFile]);
        tft.println();
        delay(5000);
        error = true;
      }
  }
}

bool hasInput(){
  return analogRead(AD_PIN) > buttonConfigJson["defaultValue"];
}


bool checkButtonValue(int minValue, int maxValue, int buttonId){
  
  if (analogRead(AD_PIN) >= minValue && analogRead(AD_PIN) <= maxValue){
    if(buttonId == 16){
      if(!altKeyPressed){
            altKeyPressed = true;
            interateOverPages();
          }else if(altKeyPressed){
            interateOverFiles();
            chosingFile = true;
          }
    }
    else{
      handleButtonPress(buttonId);
    }
    is_button_pressed = true;
  }
}

void iterateThroughButtons(){
   for(int i = 0; i < buttonConfigJson["buttons"].size();i++){
    checkButtonValue(buttonConfigJson["buttons"][i]["minValue"],buttonConfigJson["buttons"][i]["maxValue"], buttonConfigJson["buttons"][i]["buttonValue"]);
  }
  
}


void handleButtonPress(){
  if(!hasInput() && is_button_pressed){
    is_button_pressed = false;
     
  }
  else if (hasInput() && !is_button_pressed){
    iterateThroughButtons();
  }
}
