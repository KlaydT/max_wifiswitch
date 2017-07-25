#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>  
#include <ESP8266SSDP.h>       
#include <FS.h>             
#include <ArduinoJson.h>  
#include <WiFiClient.h>

#define sessionKeyLength 30
#define numOfParameters 11
#define numOfSwitches 4
#define numOfFreeFiles 5

ESP8266WebServer HTTP(80); // Web интерфейс 

String freeToUse[numOfFreeFiles] = {
	"/bootstrap.min.css",
	"/style.css",
	"/jquery-3.2.1.min.js",
	"/login.htm",
	"/SSDPName"
};

String parametersName[numOfParameters] = {
	"version",
	"ssidName",
	"ssidPassword",
	"ssidAPName",
	"ssidAPPassword",
	"dev_login",
	"dev_pass",
	"IPaddr",
	"IPmask",
	"Gateway",
	"SSDPName"
};

String parametersValue[numOfParameters] = {
	"v.24.",	            // версия скетча
	"Grand_24",				// Для хранения SSID
	"",				        // Для хранения пароля сети
	"SocketSetup",       	// SSID AP точки доступа
	"",                  	// пароль точки доступа
	"admin",
	"admin",
	"",
	"",
	"",
	"WiFi Power Switch"		// Имя SSDP
};

int iaa = 0;
String session_id = "";
String jsonConfig = "";

int switchesState[numOfSwitches] = {0,0,0,0};
int switchesPins[numOfSwitches] = {13,12,14,16};
int pinState[2] = {LOW,HIGH};

int LED = 2;

void setup() {
	Serial.begin(115200);
	Serial.println("");
	Serial.println(getParameter("version"));

  session_id = SessionID_gen(sessionKeyLength);

	FS_init();              //Запускаем файловую систему
	loadConfig();           //Загружаем сохраненные данные

	for (int i=0; i<numOfSwitches; i++) {
		Serial.print("sw"); Serial.print(i+1); Serial.print(": "); Serial.print(switchesState[i]);
		pinMode(switchesPins[i], OUTPUT);
		digitalWrite(switchesPins[i], pinState[switchesState[i]]);
	}

	pinMode(LED, OUTPUT); 
	digitalWrite(LED, HIGH);

	WIFIinit();                        //Запускаем WIFI   
	SSDP_init();                       //Запускаем SSDP 
	HTTP_init();                       //Запускаем HTTP
}

void loop() {
	HTTP.handleClient();
	delay(1);
}
