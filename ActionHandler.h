//Executes the API call with a provided fingerprint
void handleApiCall(String url, String fingerprint=configJson["fingerprint"]){
   if(WiFi.status()== WL_CONNECTED){ //Check WiFi connection status
      HTTPClient http; //Declare an object of class HTTPClient
      http.begin(url, fingerprint); //Specify request destination
      int httpCode = http.GET(); //Send the request
      if (httpCode > 0) { //Check the returning code
        String payload = http.getString(); //Get the request response payload
      }
      http.end(); //Close connection
  }
}


//Handles the changing of the pages
void handleBoardChange(int keyNumber){
  if(buttons["pages"].size() >= keyNumber){
    page = keyNumber - 1;
    setCurrentBoard();
    altKeyPressed = false;
    interateOverButtonsOnPage();
  }
}

//Handles the socket command
void handleSocketCommand(String command_id){
  String command = String() + "{\"id\":" + command_id + ",\"isTapStart\":false}";
  client.emit("exec_shortcut", command.c_str());
}
