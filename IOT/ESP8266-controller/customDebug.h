void dbmsg(String message){
  #ifdef DEBUGGING
    Serial.println("DEBUG: " + message);
  #endif
}