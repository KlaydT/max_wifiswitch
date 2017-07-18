void HTTP_init(void) {
  Serial.println("Start HTTP");
  HTTP.on("/configs.json",  handle_ConfigJSON);   // формирование configs.json страницы для передачи данных в web интерфейс
  // API для устройства
  HTTP.on("/ssdp",          handle_Set_Ssdp);     // Установить имя SSDP устройства по запросу вида /ssdp?ssdp=proba
  HTTP.on("/ssid",          handle_Set_Ssid);     // Установить имя и пароль роутера по запросу вида /ssid?ssid=home2&password=12345678
  HTTP.on("/devauth",       handle_Set_devauth);
  HTTP.on("/ssidap",        handle_Set_Ssidap);   // Установить имя и пароль для точки доступа по запросу вида /ssidap?ssidAP=home1&passwordAP=8765439
  HTTP.on("/restart",       handle_Restart);      // Перезагрузка модуля по запросу вида /restart?device=ok
  HTTP.on("/Switch1",       handle_Set_Switch1);
  HTTP.on("/Switch2",       handle_Set_Switch2);
  HTTP.on("/Switch3",       handle_Set_Switch3);
  HTTP.on("/Switch4",       handle_Set_Switch4);
  HTTP.on("/ip",            handle_Set_ip);
  
  HTTP.on("/login",         handleLogin);
  HTTP.on("/",              handleRoot);
  HTTP.on("/index.htm",     handleRoot);
  HTTP.on("/management.htm",handleRoot);
  //HTTP.on("/inline", [](){
  //  HTTP.send(200, "text/plain", "this works without need of authentification");
  //});
  //here the list of headers to be recorded
  const char * headerkeys[] = {"User-Agent","Cookie"} ;
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  HTTP.collectHeaders(headerkeys, headerkeyssize );  //ask server to track these headers
  
  HTTP.begin();                          // Запускаем HTTP сервер
}


//login page, also called for disconnect
void handleLogin(){
  String msg;
  if (HTTP.hasHeader("Cookie")){
    Serial.print("Found cookie: ");
    String cookie = HTTP.header("Cookie");
    Serial.println(cookie);
  }
  if (HTTP.hasArg("DISCONNECT")){
    Serial.println("Disconnection");
    SessionID_gen(); // Вышли. Сервер сгенерил новую ID. Вдруг старую куку кто то услышал.
    Serial.println("New Session_ID is: " + session_id);
    HTTP.sendHeader("Location","/login");
    HTTP.sendHeader("Cache-Control","no-cache");
    HTTP.send(301);
    return;
  }  
  
  if (WiFi.status() != WL_CONNECTED) { // В режиме точки доступа логин/пароль будет всегда admin/admin
      if (HTTP.hasArg("USERNAME") && HTTP.hasArg("PASSWORD")){      
      if (HTTP.arg("USERNAME") == "admin" &&  HTTP.arg("PASSWORD") == "admin" ){
      HTTP.sendHeader("Location","/");
      HTTP.sendHeader("Cache-Control","no-cache");
      HTTP.sendHeader("Set-Cookie","HTTPSESSIONID=" + session_id);
      HTTP.send(301);
      Serial.println("Log in Successful");
      return;
    }
   msg = "<FONT color=red>In AP mode use default login: 'admin', password: 'admin'.</FONT>";
   Serial.println("Log in Failed");
   }
  } 
  else
  {
      if (HTTP.hasArg("USERNAME") && HTTP.hasArg("PASSWORD")){      
      if (HTTP.arg("USERNAME") == dev_login &&  HTTP.arg("PASSWORD") == dev_pass ){
      HTTP.sendHeader("Location","/");
      HTTP.sendHeader("Cache-Control","no-cache");
      HTTP.sendHeader("Set-Cookie","HTTPSESSIONID=" + session_id);
      HTTP.send(301);
      Serial.println("Log in Successful");
      return;
    }
   msg = "Wrong username/password! try again.";
   Serial.println("Log in Failed");
   }    
  }

 loginHTMLpage = "<!DOCTYPE html>";
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
 //loginHTMLpage += "You also can go <a href='/inline'>here</a></body></html>";
 
 HTTP.send(200, "text/html", loginHTMLpage);
}

//Check if header is present and correct
bool is_authentified(){
  Serial.println("Check if user are authentified");
  if (HTTP.hasHeader("Cookie")){
    Serial.print("Found cookie in browser: ");
    String cookie = HTTP.header("Cookie");
    Serial.println(cookie);
    if (cookie.indexOf("HTTPSESSIONID=" + session_id) != -1) {  
      Serial.println("Authentification Successful"); // Куки в браузере и куки на сервере совпали
      return true;
    }
  }
  Serial.println("Authentification Failed"); // Куки не совпали
  return false;
}

//page can be accessed only if authentification is ok
void handleRoot(){
  Serial.println("Enter handleRoot");
  String header;
  if (!is_authentified()){
    HTTP.sendHeader("Location","/login");
    HTTP.sendHeader("Cache-Control","no-cache");
    HTTP.send(301);
    return;
  }
 if (!handleFileRead(HTTP.uri()))
      HTTP.send(404, "text/plain", "FileNotFound");
}
//no need authentification
void handleNotFound (){
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += HTTP.uri();
  message += "\nMethod: ";
  message += (HTTP.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += HTTP.args();
  message += "\n";
  for (uint8_t i=0; i<HTTP.args(); i++){
    message += " " + HTTP.argName(i) + ": " + HTTP.arg(i) + "\n";
  }http://192.168.2.158/management.htm
  HTTP.send(404, "text/plain", message);
}

// Функции API-Set
// Установка SSDP имени по запросу вида http://192.168.0.101/ssdp?ssdp=proba
void handle_Set_Ssdp() {
    if (is_authentified()){
    SSDP_Name = HTTP.arg("ssdp"); // Получаем значение ssdp из запроса сохраняем в глобальной переменной
    saveConfig();                 // Функция сохранения данных во Flash пока пустая
    HTTP.send(200, "text/plain", "OK"); // отправляем ответ о выполнении
    }
  HTTP.send(200, "text/html", loginHTMLpage);
}

// Установка параметров для подключения к внешней AP по запросу вида http://192.168.0.101/ssid?ssid=home2&password=12345678
void handle_Set_Ssid() {
  if (is_authentified()){
  _ssid = HTTP.arg("ssid");            // Получаем значение ssid из запроса сохраняем в глобальной переменной
  _password = HTTP.arg("password");    // Получаем значение password из запроса сохраняем в глобальной переменной
  saveConfig();                        // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}

// dev authentication http://192.168.0.101/devauth?dev_login=admin&dev_pass=123
void handle_Set_devauth() {
  if (is_authentified()){
  dev_login = HTTP.arg("dev_login");           
  dev_pass = HTTP.arg("dev_pass");   
  saveConfig();                     
  HTTP.send(200, "text/plain", "OK"); 
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}

// ip settings http://192.168.2.158/ip?IPaddr=192.168.2.159&IPmask=255.255.255.0&Gateway=192.168.2.1
void handle_Set_ip() { 
  if (is_authentified()){
  IPaddr =  HTTP.arg("IPaddr"); 
  IPmask =  HTTP.arg("IPmask"); 
  Gateway = HTTP.arg("Gateway"); 
  saveConfig();                     
  HTTP.send(200, "text/plain", "OK"); 
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}

//Установка параметров внутренней точки доступа http://192.168.2.158/ssidap?ssidAP=home1&passwordAP=8765439 
void handle_Set_Ssidap() {
  if (is_authentified()){
  _ssidAP = HTTP.arg("ssidAP");         // Получаем значение ssidAP из запроса сохраняем в глобальной переменной
  _passwordAP = HTTP.arg("passwordAP"); // Получаем значение passwordAP из запроса сохраняем в глобальной переменной
  saveConfig();                         // Функция сохранения данных во Flash пока пустая
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}

//вкл/выкл D4_pin http://192.168.2.158/Switch1?Switch1=on
void handle_Set_Switch1() {
  if (is_authentified()){             
  Switch1 = HTTP.arg("Switch1");        
  if (Switch1 == "on") {  digitalWrite(D4_pin, HIGH); };
  if (Switch1 == "off"){  digitalWrite(D4_pin, LOW);  };
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "ok");   // отправляем ответ о выполнении
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}
//вкл/выкл D3_pin http://192.168.2.158/Switch2?Switch2=on
void handle_Set_Switch2() {
  if (is_authentified()){             
  Switch2 = HTTP.arg("Switch2");        
  if (Switch2 == "on") {  digitalWrite(D3_pin, HIGH);};
  if (Switch2 == "off"){  digitalWrite(D3_pin, LOW); };
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}
//вкл/выкл D2_pin http://192.168.2.158/Switch3?Switch3=on
void handle_Set_Switch3() {
  if (is_authentified()){ 
  Switch3 = HTTP.arg("Switch3");        
  if (Switch3 == "on") {  digitalWrite(D2_pin, HIGH);};
  if (Switch3 == "off"){  digitalWrite(D2_pin, LOW); };
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}
//вкл/выкл D1_pin http://192.168.2.158/Switch4?Switch4=on
void handle_Set_Switch4() {
  if (is_authentified()){              
  Switch4 = HTTP.arg("Switch4");        
  if (Switch4 == "on") {  digitalWrite(D1_pin, HIGH);};
  if (Switch4 == "off") { digitalWrite(D1_pin, LOW); };
  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}

// Перезагрузка модуля по запросу вида http://192.168.2.158/restart?device=ok
void handle_Restart() {
  if (is_authentified()){  
  String restart = HTTP.arg("device");          // Получаем значение device из запроса
  if (restart == "ok") {                         // Если значение равно Ок
    HTTP.send(200, "text / plain", "Reset OK"); // Oтправляем ответ Reset OK
    ESP.restart();                                // перезагружаем модуль
  }
  else {                                        // иначе
    HTTP.send(200, "text / plain", "No Reset"); // Oтправляем ответ No Reset
    }
  }
  HTTP.send(200, "text/html", loginHTMLpage);
}

void handle_ConfigJSON() {
  String root = "{}";  // Формировать строку для отправки в браузер json формат
  //{"SSDP":"SSDP-test","ssid":"home","password":"i12345678","ssidAP":"WiFi","passwordAP":"","ip":"192.168.0.101"}
  // Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
  DynamicJsonBuffer jsonBuffer;
  //  вызовите парсер JSON через экземпляр jsonBuffer
  JsonObject& json = jsonBuffer.parseObject(root);
  // Заполняем поля json
  json["SSDP"] = SSDP_Name;
  if (is_authentified()){ 
    json["ssidAP"] = _ssidAP;
    json["passwordAP"] = _passwordAP;
    json["ssid"] = _ssid;
    json["password"] = _password;
    json["ip"] = WiFi.localIP().toString();
    json["Switch1"] = Switch1;
    json["Switch2"] = Switch2;
    json["Switch3"] = Switch3;
    json["Switch4"] = Switch4;
    json["dev_login"] = dev_login;
    json["dev_pass"] = dev_pass;
    json["IPaddr"] = IPaddr;
    json["IPmask"] = IPmask;
    json["Gateway"] = Gateway;
    }
  // Помещаем созданный json в переменную root
  root = "";
  json.printTo(root);
  HTTP.send(200, "text/json", root);
}


