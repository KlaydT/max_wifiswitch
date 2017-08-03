#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>  
#include <ESP8266SSDP.h>       
#include <FS.h>             
#include <ArduinoJson.h>  
#include <WiFiClient.h>
#include <DNSServer.h>

#define sessionKeyLength 30
#define numOfParameters 11
#define numOfSwitches 4
#define numOfFreeFiles 6

// #define DEBUG


const byte DNS_PORT = 53;
//IPAddress apIP(192, 168, 1, 1);
DNSServer dnsServer;
ESP8266WebServer HTTP(80); // Web интерфейс 

String freeToUse[numOfFreeFiles] = {
	"/bootstrap.min.css",
	"/style.css",
	"/jquery-3.2.1.min.js",
	"/login.htm",
	"/head.min.js",
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
	"v.25.",	            // версия скетча
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

#if defined(DEBUG)

	for (int i=0; i<numOfSwitches; i++) {
		Serial.print("sw"); Serial.print(i); Serial.print(": "); Serial.print(switchesState[i]); Serial.println("");
	}

	for (int i=0; i<numOfParameters; i++) {
		Serial.print(parametersName[i]); Serial.print(": "); Serial.println(parametersValue[i]);
	}

#endif

	for (int i=0; i<numOfSwitches; i++) {
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
  	dnsServer.processNextRequest();
	HTTP.handleClient();
	delay(10);
}
