// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  Serial.println("Loading the saved data");

  File configFile = SPIFFS.open("/configs.json", "r");  // Открываем файл для чтения
  if (!configFile) {                                    // если файл не найден  
    Serial.println("Failed to open config file");       // cоздаем файл c переменными по умолчанию
    saveConfig();
    return false;
  }
  size_t size = configFile.size();                      // Проверяем размер файла, будем использовать файл размером меньше 1024 байта
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }


  jsonConfig = configFile.readString();                 // загружаем файл конфигурации в глобальную переменную
  DynamicJsonBuffer jsonBuffer;                       // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  JsonObject& root = jsonBuffer.parseObject(jsonConfig);//  вызовите парсер JSON через экземпляр jsonBuffer. строку возьмем из глобальной переменной String jsonConfig
  // Теперь можно получить значения из root  
  _ssidAP = root["ssidAPName"].as<String>(); // Так получаем строку
  _passwordAP = root["ssidAPPassword"].as<String>();
  SSDP_Name = root["SSDPName"].as<String>();
  _ssid = root["ssidName"].as<String>();
  _password = root["ssidPassword"].as<String>();
  Switch1 = root["Switch1"].as<String>();
  Switch2 = root["Switch2"].as<String>();
  Switch3 = root["Switch3"].as<String>();
  Switch4 = root["Switch4"].as<String>(); 
  dev_login = root["dev_login"].as<String>();
  dev_pass = root["dev_pass"].as<String>();
  IPaddr = root["IPaddr"].as<String>();
  IPmask = root["IPmask"].as<String>();
  Gateway = root["Gateway"].as<String>();
  return true;
}


bool saveConfig() {                                     // Запись данных в файл config.json
  DynamicJsonBuffer jsonBuffer;                         // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);  //  вызовите парсер JSON через экземпляр jsonBuffer
  // Заполняем поля json 
  json["SSDPName"] = SSDP_Name;
  json["ssidAPName"] = _ssidAP;
  json["ssidAPPassword"] = _passwordAP;
  json["ssidName"] = _ssid;
  json["ssidPassword"] = _password;
  json["Switch1"] = Switch1;
  json["Switch2"] = Switch2;
  json["Switch3"] = Switch3;
  json["Switch4"] = Switch4;
  json["dev_login"] = dev_login;
  json["dev_pass"] = dev_pass;
  json["IPaddr"] = IPaddr;
  json["IPmask"] = IPmask;
  json["Gateway"] = Gateway;
  json.printTo(jsonConfig);                             // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  File configFile = SPIFFS.open("/configs.json", "w");  // Открываем файл для записи
  if (!configFile) {    
    //Serial.println("Failed to open config file for writing");
    return false;
  }
  json.printTo(configFile);                          // Записываем строку json в файл 
  return true;
}
