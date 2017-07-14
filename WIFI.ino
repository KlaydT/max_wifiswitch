void WIFIinit() {
  // Попытка подключения к WiFi сети
  WiFi.disconnect(); // обрываем WIFI соединения
  WiFi.softAPdisconnect(); // отключаем отчку доступа(если она была)
  WiFi.mode(WIFI_OFF); // отключаем WIFI
  delay(500);
  WiFi.mode(WIFI_STA);
  if (IPaddr != "" && IPmask !="" && Gateway !="" ) 
      { IPconversion(); Serial.println("Use static IP parameters.");} else { Serial.println("Use DHCP:"); };  
  byte tries = 20;
  delay(500);
  Serial.print("Connecting to WiFi network: ");
  Serial.print(_ssid.c_str());
  WiFi.begin(_ssid.c_str(), _password.c_str());
  
  // Делаем проверку подключения до тех пор пока счетчик tries не станет равен нулю или не получим подключение
  while (--tries && WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    if (tries & 1) { digitalWrite(LED, HIGH); } else {digitalWrite(LED, LOW);}; // мигаем светодиодом пока конектимся
    delay(500);
    }
  
  if (WiFi.status() != WL_CONNECTED) { // Если не удалось подключиться запускаем в режиме AP 
    Serial.println("");
    for ( int count=0; count<20; count++ )
       { digitalWrite(LED, LOW); delay(50); digitalWrite(LED, HIGH); delay(50);}; // мигаме светодиодом 20 раз и гасим его. Он инверсный.
    Serial.print("Connection fault. Start AP mode. ");
    Serial.println(_ssidAP.c_str());
    WiFi.disconnect(); // Отключаем WIFI
    WiFi.mode(WIFI_AP);  // Меняем режим на режим точки доступа
    IPAddress apIP(192, 168, 0, 1); //Задаем настройки сети
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(_ssidAP.c_str(), _passwordAP.c_str()); // Включаем WIFI в режиме точки доступа
  }
  else { // удалось подключиться 
    digitalWrite(LED, LOW); // зажигаем светодиод. Он инверсный.
    Serial.println("Connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.print("IP network: ");
    Serial.println(WiFi.subnetMask());
    Serial.print("Default GW: ");
    Serial.println(WiFi.gatewayIP());
  }
}

