String SessionID_gen(int sessKeyLength) { // Generate a new Session_ID. 
	
	int selectchar =         0;
	String simbol =          "";
	String pattern =         "qwertyuiopasdfghjklzxcvbnm-QWERTYUIOPASDFGHJKLZXCVBNM-1234567890";
	String result = "";
 
	for ( int m=0; m < sessKeyLength; m++ ) {
		selectchar = random(0,64); 
		simbol =  pattern.substring(selectchar, selectchar+1); 
		result +=simbol;
	}
	Serial.println("New Session_ID is: " + result);
	return result;
}
