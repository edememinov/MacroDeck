#include <SocketIoClient.h>
#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h> //https://github.com/tzapu/WiFiManager
#include <ESP8266HTTPClient.h> 


#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>

#include <ArduinoJson.h>


#include "SysVariables.h"
#include "ScreenHandler.h"

#include "CSS.h"


#include "FileHandler.h"
#include "ActionHandler.h"
#include "ButtonHandler.h"


#include "SocketHandler.h"
#include "WebPortal.h"
 //ADC_MODE(ADC_VCC);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);  
  delay(100);
  WiFi.mode(WIFI_STA);

 

  
  
  
  wifiManager.setTimeout(180);
  wifiManager.autoConnect("MacroBoardWifiConnect");
  
  
  
  initiateScreen();
  showIpAddress();
  initiateSocket();
  webserver.begin();
  SPIFFS.begin();

  pushAllFilesToJson();

  delay(2000);

  if(!error){
      getButtonJSON();
      getConfigJSON();
      configJson = configDoc.as<JsonObject>();
      buttons = doc.as<JsonObject>();
      client.begin(configJson["socketHost"].as<char*>(), configJson["socketPort"].as<int>(), path);
      setCurrentBoard();
      interateOverButtonsOnPage();
      
  }else{
    tft.setTextColor(TFT_RED, TFT_WHITE);
    tft.println("An error has occured, please fix the error and restart the device.");
    tft.println("The web-interface is available.");
  }
  initiateAPI();
  
  
  
  
}


void loop() {  
  delay(100);
  client.loop();
  webserver.handleClient();
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  getButton();
}









  
