void SessionID_gen() { // Generate a new Session_ID. 
session_id = "";
for ( int m=0; m<30; m++ )
  {
  selectchar = random(0,64); //Serial.println(selectchar);
  simbol =  pattern.substring(selectchar, selectchar+1); //Serial.print(simbol);
  session_id +=simbol;
  }
Serial.println("New Session_ID is: " + session_id);
}
