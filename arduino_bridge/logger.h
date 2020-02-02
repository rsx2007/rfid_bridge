void loger(char * msg, char * type = "info") {

#ifdef DEBUG

  byte lvl = 0;
  if (type == "info") {
    lvl = 0;
  } else if (type == "warning") {
    lvl = 10;
  } else if (type == "error") {
    lvl = 20;
  } else if (type == "debug") {
    lvl = 30;
  }
  if (DEBUG_LEVEL == "info") {
    if (lvl < 10) {
      Serial.println(msg);
      return;
    }
  } else if (DEBUG_LEVEL == "warning") {
    if (lvl < 20) {
      Serial.print(type);
      Serial.print(": ");
      Serial.println(msg);
      return;
    }
  } else if (DEBUG_LEVEL == "error") {
    if (lvl < 30) {
      if (type == "error") {
        Serial.println("************************************");
        Serial.print("ERROR: ");
        Serial.print(msg);
        Serial.println("************************************");
      } else {
        Serial.print(type);
        Serial.print(": ");
        Serial.println(msg);
      }
      return;
    }
  } else if (DEBUG_LEVEL == "debug") {
    if (lvl < 40) {
      Serial.print(type);
      Serial.print(": ");
      Serial.println(msg);
      return;
    }
  }
#endif
}
