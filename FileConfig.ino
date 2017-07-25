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
  
  // Теперь можно получить значения из root :: параметры
  for (int i = 0; i < numOfParameters; i++){
    parametersValue[i] = root[parametersName[i]].as<String>();
  }  
  
  // Теперь можно получить значения из root :: состояние розеток
  for (int i = 0; i < numOfSwitches; i++){
    switchesState[i] = root["Switches"][i].as<int>();
  }

  return true;
}


bool saveConfig() {
#if !defined(DEBUG)                                        // Запись данных в файл config.json
  DynamicJsonBuffer jsonBuffer;                           // Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
  JsonObject& json = jsonBuffer.parseObject(jsonConfig);  //  вызовите парсер JSON через экземпляр jsonBuffer
  
  // Заполняем поля json 
  for (int i = 0; i < numOfParameters; i++){
    json[parametersName[i]] = parametersValue[i];
  }

  //Заполняем состояние розеток
  JsonObject& jsonSW = json.createNestedObject("Switches");
  for (int i = 0; i < numOfSwitches; i++){
    jsonSW[String(i)+1] = switchesState[i];
  }

  json.printTo(jsonConfig);                               // Помещаем созданный json в глобальную переменную json.printTo(jsonConfig);
  File configFile = SPIFFS.open("/configs.json", "w");    // Открываем файл для записи
  if (!configFile) {    
    //Serial.println("Failed to open config file for writing");
    return false;
  }
  json.printTo(configFile);
#endif                               // Записываем строку json в файл 
  return true;
}
