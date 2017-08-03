bool setParameter(String name, String value){
	for (int i = 0; i < numOfParameters; i++){
    	if (parametersName[i] == name) {
    		parametersValue[i] = value;
    		return true;
    	}
  	}
	return false;
}

String getParameter(String name){
	for (int i = 0; i < numOfParameters; i++){
		if (parametersName[i] == name) {
			return parametersValue[i];
		}
	}
	return "";	
}

String getParametersAsJSON(){
	String result = "";

	// Резервируем память для json обекта буфер может рости по мере необходимти, предпочтительно для ESP8266
	DynamicJsonBuffer jsonBuffer;
	JsonObject& json = jsonBuffer.createObject();

	// Заполняем поля json
	for (int i = 0; i < numOfParameters; i++){
		json[parametersName[i]] = parametersValue[i];
				#if defined(DEBUG) 
					Serial.println(parametersName[i] + ' : ' + parametersValue[i]);
				#endif
	}

	//Заполняем состояние розеток
	JsonObject& jsonSW = json.createNestedObject("Switches");
	for (int i = 0; i < numOfSwitches; i++){
	  jsonSW[String(i)] = switchesState[i];
	}

	// Помещаем созданный json в переменную root
	json.printTo(result);

	return result;
}

void setParametersFromJSON(String jsonString){

	DynamicJsonBuffer jsonBuffer;                       	// Резервируем памяь для json обекта буфер может рости по мере необходимти предпочтительно для ESP8266 
	JsonObject& root = jsonBuffer.parseObject(jsonString);	//  вызовите парсер JSON через экземпляр jsonBuffer. строку возьмем из глобальной переменной String jsonConfig

	// Теперь можно получить значения из root :: параметры
	for (int i = 0; i < numOfParameters; i++){
		parametersValue[i] = root[parametersName[i]].as<String>();
	}  

	// Теперь можно получить значения из root :: состояние розеток
	for (int i = 0; i < numOfSwitches; i++){
		switchesState[i] = root["Switches"][String(i)].as<int>();
	}
}
