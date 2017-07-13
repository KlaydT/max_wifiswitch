void SSDP_init(void) {
    Serial.println("Start SSDP");
  // SSDP дескриптор
  HTTP.on("/description.xml", HTTP_GET, []() {
    SSDP.schema(HTTP.client());
  });
  //Если версия  2.0.0 закаментируйте следующую строчку
  SSDP.setDeviceType("upnp:rootdevice");
  SSDP.setSchemaURL("description.xml");
  SSDP.setHTTPPort(80);
  SSDP.setName(SSDP_Name);
  SSDP.setSerialNumber("001788102201");
  SSDP.setURL("/");
  SSDP.setModelName("WiFiPowerSwitch");
  SSDP.setModelNumber("000000000001");
  SSDP.setModelURL("http://good.net.ua");
  SSDP.setManufacturer("pmg@ukr.net");
  SSDP.setManufacturerURL("http://good.net.ua");
  SSDP.begin();
}
