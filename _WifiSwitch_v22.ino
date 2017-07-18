#include <ESP8266WiFi.h>        
#include <ESP8266WebServer.h>  
#include <ESP8266SSDP.h>       
#include <FS.h>             
#include <ArduinoJson.h>  
#include <WiFiClient.h>
#include "ESP8266TrueRandom.h"

ESP8266WebServer HTTP(80); // Web интерфейс 

String Version =         "v.22.";             // версия скетча
String _ssid     =       "Grand_24";          // Для хранения SSID
String _password =       "V";          // Для хранения пароля сети
String _ssidAP =         "SocketSetup";       // SSID AP точки доступа
String _passwordAP =     "";                  // пароль точки доступа
String dev_login =       "admin";
String dev_pass =        "admin";
String IPaddr =          "";
String IPmask =          "";
String Gateway =         "";
String session_id =      "";
String SSDP_Name =       "WiFi Power Switch"; // Имя SSDP

String jsonConfig = "{}";

String Switch1 = "on";
String Switch2 = "on";
String Switch3 = "on";
String Switch4 = "on";
int D4_pin = 16;
int D3_pin = 14;
int D2_pin = 12;
int D1_pin = 13;

int LED = 2;

byte uuidNumber[16]; // UUIDs in binary form are 16 bytes long

String loginHTMLpage = "";
/*
 String loginHTMLpage = "<!DOCTYPE html>";
 loginHTMLpage += "<html lang='ru'>";
 loginHTMLpage += "<head>";
 loginHTMLpage += "<meta http-equiv='Content-type' content='text/html; charset=utf-8'>";
 loginHTMLpage += "<link rel='stylesheet' href='/bootstrap.min.css'>";
 loginHTMLpage += "<link rel='stylesheet' type='text/css' href='/style.css'>";
 loginHTMLpage += "<script type='text/javascript' src='/function.js'></script>";
 loginHTMLpage += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
 loginHTMLpage += "<title>Login Page</title>";
 loginHTMLpage += "</head>";
 loginHTMLpage += "<body onload=\"load();\">";
 loginHTMLpage += "<div class=\"container\">";
 loginHTMLpage += "<div class=\"row\" style=\"text-align:center;\">";
 loginHTMLpage += "<p></p>";
 loginHTMLpage += "<div class=\"col-sm-offset-2 col-sm-8 col-md-offset-3 col-md-6\">";
 loginHTMLpage += "<h5 class=\"alert-info\" id=\"ssdp_t\">{{SSDP}}</h5><hr>";  
 loginHTMLpage += "<form action='/login' method='POST'>Log in to use PowerSocket<br>";
 loginHTMLpage += "<TABLE align=center>";
 loginHTMLpage += "<TR><TD align=right>User:</TD><TD><input type='text' name='USERNAME' placeholder='user name'></TD></TR>";
 loginHTMLpage += "<TR><TD>Password:</TD><TD><input type='password' name='PASSWORD' placeholder='password'></TD></TR>";
 loginHTMLpage += "<TR><TD colspan=2><input type='submit' name='SUBMIT' value='Submit'></TD></TR>";
 loginHTMLpage += "</TABLE>";
 loginHTMLpage += "</form>" + msg + "<br>";
 loginHTMLpage += "<hr></div></div></div></body></html>";
*/

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
