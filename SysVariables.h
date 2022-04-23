//All availables variables are here
#define ServerVersion "1.0"
#define FirmwareVersion "v 0.3"
String webpage = "";
bool   SD_present = false;

//Wifi Manager
WiFiManager wifiManager;

//Button Wemos D1 pin
const int AD_PIN = A0;


/// Socket.IO Settings ///
//char host[] = configJson["socketHost"]; // Socket.IO Server Address
//int port = configJson["socketPort"]; // Socket.IO Port Address
char path[] = "/socket.io/?transport=websocket"; // Socket.IO Base Path
bool useSSL = false; // Use SSL Authentication
const char * sslFingerprint = "";  // SSL Certificate Fingerprint
bool useAuth = false; // use Socket.IO Authentication
const char * serverUsername = "socketIOUsername";
const char * serverPassword = "socketIOPassword";

//Screen
TFT_eSPI tft = TFT_eSPI();  // Invoke library

//Socket IO
SocketIoClient client;

//MQTT
WiFiClient espClient;
PubSubClient mqttClient(espClient);


//Webserver
ESP8266WebServer webserver(80);
File fsUploadFile;

//App specific
Dir dir;
int currentFile = 0;
String BUTTON_JSON[15];
int page = 0;
bool altKeyPressed = false;
bool chosingFile = false;
int amount_of_files = 0;

//JSON
DynamicJsonDocument doc(10422);
JsonObject buttons;
String configFile = "/config.txt";
DynamicJsonDocument configDoc(1024);
DynamicJsonDocument aliveSince(1024);
JsonObject configJson;

String buttonConfigFile = "/button_config.txt";
DynamicJsonDocument buttonConfigDoc(2048);
JsonObject buttonConfigJson;

//Error
bool error = false;

//Button is pressed
bool is_button_pressed = false;
