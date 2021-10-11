//Draw main screen
void initiateScreen() {
  tft.init();
  tft.setRotation(2);
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 32, 1);
  tft.setTextSize(1);
  tft.setTextFont(2);
  tft.fillRect(0, 0, 240, 16, TFT_BLUE);
  tft.drawCentreString(" MacroDeck ", 120, 0, 2);
  tft.fillRect(0, 16, 240, 45, TFT_BLUE);
  tft.fillRect(0, 70, 240, 320, TFT_WHITE);
}

void clearScreen() {
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(5, 32, 1);

}

//Shows the local ip address on the screen
void showIpAddress() {

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawCentreString("No WIFI", 30, 0, 2);
  }
  IPAddress ip = WiFi.localIP();
  tft.fillRect(0, 0, 60, 16, TFT_BLUE);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawCentreString("WIFI", 30, 0, 2);
  while (ip[0] == 0) {
    delay(100);
  }
  String fullIp = String() + "Server: "  +  ip[0] + "." + ip[1] +  "." + ip[2] + "."  + ip[3];
  //tft.drawString(String() + ESP.getVcc(), 0,18,2);
  tft.drawCentreString(fullIp, 120, 18, 2);
}

//Shows currently selected page to the user
void setCurrentBoard() {

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.fillRect(0, 35, 240, 70, TFT_BLUE);
  tft.drawCentreString(buttons["pages"][page]["name"].as<char*>(), 120, 36, 2);
}

//Shows all available pages to the user
void interateOverPages() {
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  //Reset page by filling in a black rectangle
  tft.setCursor(0, 75);
  tft.fillRect(0, 70, 240, 320, TFT_WHITE);
  for (int i = 1; i <= buttons["pages"].size(); i++) {
    tft.print(i);
    tft.print(" : ");
    tft.println(buttons["pages"][i - 1]["name"].as<String>());
  }
  tft.setTextColor(TFT_BLUE, TFT_WHITE);
  tft.print(16);
  tft.print(" : ");
  tft.println("Choose different file");
}

//Shows all available buttons to the user
void interateOverButtonsOnPage() {
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  //Reset page by filling in a black rectangle
  tft.setCursor(0, 75);
  tft.fillRect(0, 70, 240, 320, TFT_WHITE);
  for (int i = 1; i <= buttons["pages"][page]["buttons"].size(); i++) {
    tft.print(i);
    tft.print(" : ");
    tft.println(buttons["pages"][page]["buttons"][i - 1]["description"].as<String>());
  }
  tft.setTextColor(TFT_BLUE, TFT_WHITE);
  tft.print(16);
  tft.print(" : ");
  tft.println("Choose different page");
}

//Shows all available files to the user
void interateOverFiles() {
  tft.setTextColor(TFT_BLACK, TFT_WHITE);
  //Reset page by filling in a black rectangle
  tft.setCursor(0, 75);
  tft.fillRect(0, 70, 240, 320, TFT_WHITE);
  for (int i = 1; i <= amount_of_files; i++) {
    tft.print(i);
    tft.print(" : ");
    tft.println(BUTTON_JSON[i - 1]);
  }
  tft.setTextColor(TFT_BLUE, TFT_WHITE);
  tft.print(16);
  tft.print(" : ");
  tft.println("Choose different page");
}

void showCalibrationComplete() {
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawCentreString("Calibration complete", 0, 32, 1);
}
