//Executes if the socket is reconnecting
void socket_Reconnecting(const char * payload, size_t length) {
  //client.disconnect();
  tft.fillRect(160, 0, 240, 16, TFT_BLUE);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString(" Reconnecting ", 169, 0, 2);
}

//Executes if client is connected to the socket
void socket_Connected(const char * payload, size_t length) {
  Serial.println("Socket.IO Connected!");
  tft.fillRect(160, 0, 240, 16, TFT_BLUE);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.drawString(" Socket ", 180, 0, 2);
}
//Executes if client is disconnected to the socket
void socket_Disconnected(const char * payload, size_t length) {
  tft.fillRect(160, 0, 240, 16, TFT_BLUE);
  tft.setTextColor(TFT_RED, TFT_BLACK);
  tft.drawString(" No Socket ", 169, 0, 2);
}

//Listens for socket commands
void initiateSocket() {
  client.on("reconnecting", socket_Reconnecting);
  client.on("connect", socket_Connected);
  client.on("disconnect", socket_Disconnected);
}
