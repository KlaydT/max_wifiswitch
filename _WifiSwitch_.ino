#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>  
#include <ESP8266SSDP.h>       
#include <FS.h>             
#include <ArduinoJson.h>  
#include <WiFiClient.h>

ESP8266WebServer HTTP(80); // Web интерфейс 

String Version =         "v.24.";             // версия скетча
String _ssid     =       "Grand_24";          // Для хранения SSID
String _password =       "V";          // Для хранения пароля сети
String _ssidAP =         "SocketSetup";       // SSID AP точки доступа
String _passwordAP =     "";                  // пароль точки доступа
String dev_login =       "admin";
String dev_pass =        "admin";
String IPaddr =          "";
String IPmask =          "";
String Gateway =         "";
String SSDP_Name =       "WiFi Power Switch"; // Имя SSDP

int selectchar =         0;
String simbol =          "";
String pattern =         "qwertyuiopasdfghjklzxcvbnm-QWERTYUIOPASDFGHJKLZXCVBNM-1234567890";
String session_id =      "";

String jsonConfig = "{}";

String Switch1 = "on";
String Switch2 = "on";
String Switch3 = "on";
String Switch4 = "on";
int switchesState[4] = {0,0,0,0};

int D4_pin = 16;
int D3_pin = 14;
int D2_pin = 12;
int D1_pin = 13;

int switchesPins[4] = {13,12,14,16};

int LED = 2;

String loginHTMLpage = "";

void setup() {
  Serial.begin(115200);
  Serial.println("");
  Serial.println(Version);

  FS_init();              //Запускаем файловую систему
  loadConfig();           //Загружаем сохраненные данные

  Serial.print("Sw1: "); Serial.print(Switch1); Serial.print(", ");
  Serial.print("Sw2: "); Serial.print(Switch2); Serial.print(", ");
  Serial.print("Sw3: "); Serial.print(Switch3); Serial.print(", ");
  Serial.print("Sw4: "); Serial.print(Switch4); Serial.println(". ");

  pinMode(D4_pin, OUTPUT);
  if (Switch1 == "on") { digitalWrite(D4_pin, HIGH); };
  if (Switch1 == "off"){ digitalWrite(D4_pin, LOW);  };
  pinMode(D3_pin, OUTPUT); 
  if (Switch2 == "on") { digitalWrite(D3_pin, HIGH); };
  if (Switch2 == "off"){ digitalWrite(D3_pin, LOW);  };
  pinMode(D2_pin, OUTPUT); 
  if (Switch3 == "on") { digitalWrite(D2_pin, HIGH); };
  if (Switch3 == "off"){ digitalWrite(D2_pin, LOW);  };
  pinMode(D1_pin, OUTPUT); 
  if (Switch4 == "on") { digitalWrite(D1_pin, HIGH); };
  if (Switch4 == "off"){ digitalWrite(D1_pin, LOW);  };

  pinMode(LED, OUTPUT); 
  digitalWrite(LED, HIGH);
  
  SessionID_gen();                   //Генерируем ID сесси
  WIFIinit();                        //Запускаем WIFI   
  SSDP_init();                       //запускаем SSDP 
  HTTP_init();                       //запускаем HTTP
}

void loop() {
  HTTP.handleClient();
  delay(1);
}
