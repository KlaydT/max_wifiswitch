// Загрузка данных сохраненных в файл  config.json
bool loadConfig() {
  String jsonConfig = "";

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

  jsonConfig = configFile.readString();                 

#if defined(DEBUG)
  Serial.println(jsonConfig);
#endif

  setParametersFromJSON(jsonConfig);
  return true;
}


bool saveConfig() {
  // Запись данных в файл config.json
  File configFile = SPIFFS.open("/configs.json", "w");    // Открываем файл для записи
  if (!configFile) {    
    return false;
  }

  String root = getParametersAsJSON();
  configFile.println(root);
  return true;
}
