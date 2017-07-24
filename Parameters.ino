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
