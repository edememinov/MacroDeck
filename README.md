# MacroDeck

# Goal
The goal of this project was to replicate a StreamDeck as closely as possible using an Arduino(clone).

# Function of the device
This project can execute API calls and Websocket commands. 
A txt file in JSON format can be loaded unto the Arduino SPIFFS memory which will contain the commands for the calls & commands.
Each file contains multiple pages (Up to 8 pages because of the RAM limitations) and each page contains up to 15 buttons.
It is possible to swap between files by pressing button 16 and selecting the page by pressing the corresponding button. 
While creating editing the JSON files with buttons and pages the order matters. The order on the page/button corresponds with the button number.
For example 6th button on the page can be selected by pressing button number 6.

# Using the device
## Connecting to WiFi
When the software boots it will start to look for WiFi access points that are stored in its memory. 
If none are found the Arduino will go into AP mode and create an ap called "MacroBoardWifiConnect".
After connecting to "MacroBoardWifiConnect" visit the following ip-address: 192.168.4.1. There a WiFi AP can be chosen to connect to. 
[WiFi manager that was used](https://github.com/tzapu/WiFiManager).

## Configuring the device
The following variables can be set in the config.txt file:

```JSON
{
	"fingerprint" : "SSL fingerprint",
	"socketHost" : "IP address of the PC which is running DeckBoard",
	"socketPort" : "DeckBoard port is always 8500"
}

```

## Using the Web Interface
After the device is connected to WiFi a server ip-address will be displayed. Opening the ip-address in browser will open the web-interface.
With this interface files can be uploaded, downloaded and deleted from the Arduino. Furthermore a page to check free SPIFFS space is available.

## Executing WebSocket commands.
To execute WebSocket commands this project heavily relies on [Deckboard](https://github.com/rivafarabi/deckboard).
After installing Deckboard a script can be run to download the Deckboard button configuration. 
This script will get all available buttons and write them in the correct JSON format. Afterwards these buttons can be copy-pasted into a file used by Macro Board The type of button should be a "socket" in this case.

```JSON
{
	"pages":
		[
			{
				"name":"Page name",
				"buttons":
				[
				
					 {
						"command_id": 27,//Deckboard command id
						"type": "socket",//Type of command socket or apiCall
						"description": "Button Description"
					}
				]
			}	
		]
}

			
```

## Executing API calls
API Calls will be handled just like WebSocket commands. An example of a button which will handle an API Call can be found below.

```JSON
{
	"pages":
		[
			{
				"name":"Page name",
				"buttons":
				[
				
					 {
						"url": "http://url-to-the-api.com/getSomethingFromAPI",// API URL
						"type": "apiCall",//Type of command socket or apiCall
						"description": "Button Description",
            "fingerprint":"optional SSL fingerprint. It can also be set globally in config.txt"
					}
				]
			}	
		]
}

			
```

# Parts used

For this project the following items were used:
- [Wemos D1 R1](https://nl.aliexpress.com/item/33059632653.html?spm=a2g0s.9042311.0.0.53b64c4do3k4Ez)
- [2.2" TFT Screen](https://nl.aliexpress.com/item/4000839144387.html?spm=a2g0s.9042311.0.0.53b64c4do3k4Ez)
- [16 analog button keypad & a transparent case](https://nl.aliexpress.com/item/32635207928.html?spm=a2g0s.9042311.0.0.53b64c4do3k4Ez)
- [12x Male-to-female jumper cables](https://nl.aliexpress.com/item/32809386721.html?spm=a2g0s.9042311.0.0.27424c4dLtiVZy)

Optional:
- [2x Li-Ion Batteries](https://nl.aliexpress.com/item/4001201474699.html?spm=a2g0s.9042311.0.0.27424c4dlxO3Ad)
- [Battery shield](https://nl.aliexpress.com/item/32969651563.html?spm=a2g0s.9042311.0.0.27424c4d52czuC)
- [Qi Receiver](https://nl.aliexpress.com/item/4001095902835.html?spm=a2g0o.productlist.0.0.321acf85pvwBQM&algo_pvid=3094d0d2-666d-4629-bd63-7b8da1760bfe&algo_expid=3094d0d2-666d-4629-bd63-7b8da1760bfe-8&btsid=2100bdec16061577051804078e10ef&ws_ab_test=searchweb0_0,searchweb201602_,searchweb201603_)

# Schematics
Below is a table which shows how the Wemos D1 R1 is connected to the other components

| Wemos D1 R1 Pins  | Screen Pin 
| ------------- |:-------------:
|D12/MISO/D6| SDO/MISO
|5V|LED 
|D13/SCK/D5|SCK
|D11/MOSI/D7|SDI/MOSI
|D15/SCL/D3|DC/RS
|D14/SDA/D4|RESET
|D8|CS
|GND|GND
|5V|VCC 

| Wemos D1 R1 Pins  | Keypad Pin 
| ------------- |:-------------:
|A0| Out
|3.3V|VCC 
|GND|GND

## TFT_eSPI options
User_Setup.h is needed to make the screen work. It holds the configuration and pin definitions. 
It will be added to this repo and has to be pasted to ~/Documents/Arduino/libraries/TFT_eSPI/User_Setup.h

# Other projects used within this project
- [Deckboard](https://github.com/rivafarabi/deckboard)
- [WiFi manager](https://github.com/tzapu/WiFiManager)
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [ESP8266 Core](https://github.com/esp8266/Arduino)
- [Uploading files to Arduino](https://tttapa.github.io/ESP8266/Chap12%20-%20Uploading%20to%20Server.html)
- [Downloading file from SPIFFS](https://github.com/G6EJD/ESP32-8266-File-Download)

# How to get Deckboard data
- Open deckboard_getData folder.
- Run "npm install"
- Run "node index.js"
- Wait for it to finish. This should take around 5 seconds.
- Press ctr+c to force quit.
- Run "node mergeFiles.js"

The folder finalData will contain finalButton.js, this file contains all the deckboard buttons with a name.

# Roadmap
TBD

# Useful information on programming for ESP8266 (Wemos D1 R1)
- [Set up Arduino IDE to work with Wemos D1 R1](https://www.instructables.com/Arduino-WeMos-D1-WiFi-UNO-ESP-8266-IoT-IDE-Compati/)
- [Upload sketch data tp ESP8266](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/)

