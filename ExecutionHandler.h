void addHeadersToCall(HTTPClient& http, JsonArray httpHeaders){
  for(JsonVariant v : httpHeaders){
    http.addHeader(v["headerKey"].as<String>(), v["headerValue"].as<String>());
  }
}


void handleGETRequest(String url, JsonArray httpHeaders, String fingerprint){
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http; //Declare an object of class HTTPClient
    if(fingerprint != ""){
      http.begin(url, fingerprint); //Specify request destination
    }
    else{
      http.begin(url); //Specify request destination
    }
    
    if(httpHeaders.size() > 0){
      addHeadersToCall(http, httpHeaders);
    }
    int httpCode = http.GET(); //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString(); //Get the request response payload
    }
    http.end(); 
  }
}

void handlePOSTRequest(String url, String body, JsonArray httpHeaders, String fingerprint){
  if (WiFi.status() == WL_CONNECTED) { //Check WiFi connection status
    HTTPClient http; //Declare an object of class HTTPClient
    if(fingerprint != ""){
      http.begin(url, fingerprint); //Specify request destination
    }
    else{
      http.begin(url); //Specify request destination
    }
    if(httpHeaders.size() > 0){
      addHeadersToCall(http, httpHeaders);
    }
    int httpCode = http.POST(body); //Send the request
    if (httpCode > 0) { //Check the returning code
      String payload = http.getString(); //Get the request response payload
    }
    http.end(); 
  }
}



//Executes the API call with a provided fingerprint
void handleApiCall(String url, String requestType, JsonArray httpHeaders, String body, String fingerprint = configJson["fingerprint"]) {
  if(requestType == "POST"){
    handlePOSTRequest(url, body, httpHeaders, fingerprint);
  }
  else{
    handleGETRequest(url, httpHeaders, fingerprint);
  }
}

//Handles the changing of the pages
void handleBoardChange(int keyNumber) {
  if (buttons["pages"].size() >= keyNumber) {
    page = keyNumber - 1;
    setCurrentBoard();
    altKeyPressed = false;
    interateOverButtonsOnPage();
  }
}

//Handles the socket command
void handleSocketCommand(String command_id) {
  String command = String() + "{\"id\":" + command_id + ",\"isTapStart\":false}";
  client.emit("exec_shortcut", command.c_str());
}
