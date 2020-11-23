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

//Figures out which button was pressed
void getButton(){
   if(analogRead(AD_PIN) >= 1020 && analogRead(AD_PIN) <= 1100){
    delay(50);
      if(analogRead(AD_PIN) >= 1020 && analogRead(AD_PIN) <= 1100){
      handleButtonPress(1); 
    }
   }
   else if(analogRead(AD_PIN) >= 970 && analogRead(AD_PIN) <= 1010){
    delay(50);
      if(analogRead(AD_PIN) >= 970 && analogRead(AD_PIN) <= 1010){
      handleButtonPress(2);
    }
   }
   else if(analogRead(AD_PIN) >= 850 && analogRead(AD_PIN) <= 950){
    delay(50);
      if(analogRead(AD_PIN) >= 850 && analogRead(AD_PIN) <= 950){
        handleButtonPress(3);
     }
   }
   else if(analogRead(AD_PIN) >= 780 && analogRead(AD_PIN) <= 851){
    delay(50);
    if(analogRead(AD_PIN) >= 780 && analogRead(AD_PIN) <= 851){
      handleButtonPress(4);
     }
   }
   else if(analogRead(AD_PIN) >= 660 && analogRead(AD_PIN) <= 730){
    delay(50);
    if(analogRead(AD_PIN) >= 660 && analogRead(AD_PIN) <= 730){
      handleButtonPress(5);
     }
   }
   else if(analogRead(AD_PIN) >= 650 && analogRead(AD_PIN) <= 680){
    delay(50);
      if(analogRead(AD_PIN) >= 650 && analogRead(AD_PIN) <= 680){
        handleButtonPress(6);
     }
   }
   else if(analogRead(AD_PIN) >= 620 && analogRead(AD_PIN) <= 645){
    delay(50);
    if(analogRead(AD_PIN) >= 620 && analogRead(AD_PIN) <= 645){
      handleButtonPress(7);
     }
   }
   else if(analogRead(AD_PIN) >= 590 && analogRead(AD_PIN) <= 610){
    delay(50);
    if(analogRead(AD_PIN) >= 590 && analogRead(AD_PIN) <= 610){
      handleButtonPress(8);
     }
   }
   else if(analogRead(AD_PIN) >= 520 && analogRead(AD_PIN) <= 550){
    delay(50);
    if(analogRead(AD_PIN) >= 520 && analogRead(AD_PIN) <= 550){
      handleButtonPress(9);
     }
   }
   else if(analogRead(AD_PIN) >= 495 && analogRead(AD_PIN) <= 515){
    delay(50);
      if(analogRead(AD_PIN) >= 495 && analogRead(AD_PIN) <= 515){
        handleButtonPress(10);
     }
   }
   else if(analogRead(AD_PIN) >= 470 && analogRead(AD_PIN) <= 500){
    delay(50);
    handleButtonPress(11);
   }
   else if(analogRead(AD_PIN) >= 450 && analogRead(AD_PIN) <= 475){
    delay(50);
    if(analogRead(AD_PIN) >= 450 && analogRead(AD_PIN) <= 475){
      handleButtonPress(12);
     }
   }
   else if(analogRead(AD_PIN) >= 410 && analogRead(AD_PIN) <= 435){
    delay(50);
    if(analogRead(AD_PIN) >= 410 && analogRead(AD_PIN) <= 435){
      handleButtonPress(13);
     }
   }
   else if(analogRead(AD_PIN) >= 330 && analogRead(AD_PIN) <= 350){
    delay(50);
    if(analogRead(AD_PIN) >= 330 && analogRead(AD_PIN) <= 350){
      handleButtonPress(14);
     }
   }
   else if(analogRead(AD_PIN) >= 280 && analogRead(AD_PIN) <= 310){
    delay(50);
    if(analogRead(AD_PIN) >= 280 && analogRead(AD_PIN) <= 310){
      handleButtonPress(15);
     }
    
   }
   else if(analogRead(AD_PIN) >= 220 && analogRead(AD_PIN) <= 260){
    delay(50);     
      if(analogRead(AD_PIN) >= 220 && analogRead(AD_PIN) <= 260){     
        if(!altKeyPressed){
          altKeyPressed = true;
          interateOverPages();
        }else if(altKeyPressed){
          interateOverFiles();
          chosingFile = true;
        }
    }
   }
}
