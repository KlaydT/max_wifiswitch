void HTTP_init(void) {
  #if defined(DEBUG)
    Serial.println("Start HTTP");
  #endif

  HTTP.on("/configs.json", [](){
     mainHandler(handle_ConfigJSON); 
  });
  HTTP.on("/setup", [](){
     mainHandler(handle_Setup); 
  });
  HTTP.on("/switch", [](){
     mainHandler(handle_Set_Switch); 
  });
  HTTP.on("/reset", [](){
     mainHandler(handle_Restart); 
  });
  HTTP.on("/SSDPName", handleSSDP_Name); 
  HTTP.on("/auth", handleAuth); 
  HTTP.onNotFound([](){
     mainHandler(anyHandler); 
  }); 

  const char * headerkeys[] = {"User-Agent","Cookie"};
  size_t headerkeyssize = sizeof(headerkeys)/sizeof(char*);
  HTTP.collectHeaders(headerkeys, headerkeyssize);     //ask server to track these headers
  HTTP.begin();                                        // Запускаем HTTP сервер
}


//Check if header is present and correct
bool is_authentified(){
  if (ifFreeToUse(HTTP.uri())){
      return true;
  }
  #if defined(DEBUG)
    Serial.println("Check if user are authentified");
  #endif
  if (HTTP.hasHeader("Cookie")){
    #if defined(DEBUG)
      Serial.print("Found cookie in browser: ");
    #endif
    String cookie = HTTP.header("Cookie");
    #if defined(DEBUG)
      Serial.println(cookie);
    #endif
    
    if (cookie.indexOf("HTTPSESSIONID=" + session_id) != -1) {  
      #if defined(DEBUG)
        Serial.println("Authentification Successful"); // Куки в браузере и куки на сервере совпали
      #endif

      return true;
    }
  }
  #if defined(DEBUG)
    Serial.println("Authentification Failed"); // Куки не совпали
  #endif

  return false;
}


void mainHandler(void handler()){
  if (is_authentified()) {
    handler();
  }  else {
    redirectToLogin();
  }
}


void anyHandler(){
  #if defined(DEBUG)  
       Serial.print(HTTP.uri());
  #endif
  if (!handleFileRead(HTTP.uri())){
      HTTP.send(404, "text/plain", "FileNotFound");
  }
}


bool handleFileRead(String path) {
  if (path.endsWith("/")) path += "index.htm";
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if (SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)) {
    if (SPIFFS.exists(pathWithGz)){
      path += ".gz";
    }
    File file = SPIFFS.open(path, "r");
    size_t sent = HTTP.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}


//Auth function
void handleAuth(){
  String msg;
  if (HTTP.hasHeader("Cookie")){
    #if defined(DEBUG)
      Serial.print("Found cookie: ");
    #endif
    String cookie = HTTP.header("Cookie");
    #if defined(DEBUG)
      Serial.println(cookie);
    #endif
  }
  if (HTTP.hasArg("DISCONNECT")){
    #if defined(DEBUG)
      Serial.println("Disconnection");
    #endif
    session_id = SessionID_gen(sessionKeyLength);; // Вышли. Сервер сгенерил новую ID. Вдруг старую куку кто то услышал.
    #if defined(DEBUG)
      Serial.println("New Session_ID is: " + session_id);
    #endif
    redirectToLogin();
    return;
  }  

  if (HTTP.hasArg("USERNAME") && HTTP.hasArg("PASSWORD")){      
    if (HTTP.arg("USERNAME") == getParameter("dev_login") &&  HTTP.arg("PASSWORD") == getParameter("dev_pass")){
      HTTP.sendHeader("Location","/");
      HTTP.sendHeader("Cache-Control","no-cache");
      HTTP.sendHeader("Set-Cookie","HTTPSESSIONID=" + session_id);
      HTTP.send(301);
      #if defined(DEBUG)
        Serial.println("Log in Successful");
      #endif
      return;
    }
  #if defined(DEBUG)  
    Serial.println("Log in Failed");
  #endif
  redirectToLogin();
  }
}


void handle_Setup() {
  int argsCount = HTTP.args();
  for(int i = 0; i < argsCount; i++){
      setParameter(HTTP.argName(i), HTTP.arg(i));
  }

  saveConfig();
  redirectToMain();
}


//вкл/выкл розетки
void handle_Set_Switch() {             
  String switchNumStr = HTTP.arg("sw");
  int switchNum = switchNumStr.toInt();

  #if defined(DEBUG)
    Serial.print("Switching: "); Serial.println(switchNumStr);
  #endif

  if (switchesState[switchNum] == 0) {
    switchesState[switchNum] = 1;
    digitalWrite(switchesPins[switchNum], HIGH);
  } else {
    switchesState[switchNum] = 0;
    digitalWrite(switchesPins[switchNum], LOW);
  }

  saveConfig();                         // Функция сохранения данных во Flash
  HTTP.send(200, "text/plain", "OK");   // отправляем ответ о выполнении
}


// Перезагрузка модуля по запросу вида http://192.168.2.158/restart?device=ok
void handle_Restart() {
  redirectToLogin();
  ESP.restart();                                // перезагружаем модуль
}


void handleSSDP_Name() {
  #if defined(DEBUG)
    Serial.println("SSDPName" + getParameter("SSDPName"));
  #endif
  HTTP.send(200, "text/plain", getParameter("SSDPName"));
}


void handle_ConfigJSON() {
        #if defined(DEBUG)
          Serial.println("Sending JSON");
        #endif
  String root = getParametersAsJSON();
        #if defined(DEBUG)
          Serial.println(root);
        #endif
  HTTP.send(200, "text/json", root);
}


void redirectToLogin(){
  HTTP.sendHeader("Location", String("/login.htm"), true);
  HTTP.send( 302, "text/plain", "");
  return;
}

void redirectToMain(){
  HTTP.sendHeader("Location", String("/"), true);
  HTTP.send( 302, "text/plain", "");
  return;
}

bool ifFreeToUse(String uri){
  for(int i = 0; i < numOfFreeFiles; i++){
    if (uri == freeToUse[i]) {
      return true;
    }
  }
  return false;
}
