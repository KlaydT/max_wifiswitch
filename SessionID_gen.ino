void SessionID_gen() {
  // Generate a new SSID
  ESP8266TrueRandom.uuid(uuidNumber);
  session_id = ESP8266TrueRandom.uuidToString(uuidNumber);
  Serial.println("New Session_ID is: " + session_id);
}
