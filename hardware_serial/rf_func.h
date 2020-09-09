void sendRF02() { // sending  response message to device
  // STRUCT: 0x23 0x09 0x06 0x00 0x00 0x00 0x00 0x00 0xFF
  // 0x23 - response init
  // 0x09  - response device addr
  // 0x06 - cmd to request card
  // 0x00 * 5 times -  card ID
  // 0xFF - CRC value

  in_rf_request_state = false;  // set request state to false (to read again next time)

  // set message  bytes  buff[0] = 0x23 always
  for (byte i = 0; i < 5; i++) {
    out_msg[i + 3] = cardId[i]; // card id
  }
  
  enableRS();
  
  out_msg[8] = OneWire::crc8( out_msg, 8);  // generate CRC
  for (byte i = 0; i < 9; i++) { // send message to Serial
    Serial.write(out_msg[i]);
    delay(1);
  }
  disableRS();
  /*for(byte i = 0; i < 5; i++){
    digitalWrite(LED_PIN, LED_HIGH_LEVEL);
    delay(10);
    digitalWrite(LED_PIN, LED_LOW_LEVEL);
  }
  */
  
}

void readRF02() {
  in_rf_request_state = false; //  init full valid package to false


  if (Serial.available()) { // check if we got message
    // STRUCT :   0x33 0x09 0x06 0xFF
    // 0x33 - start of message
    // 0x09  end device address (Serial)
    // 0x06 -  cmd to request card
    // 0xFF  -  CRC  check value
    if (Serial.read() == 0x33) {
      if (Serial.read() == DEV_ADDRESS) {
        if (Serial.read() == 0x06) {
          if (Serial.read() == RCV_CRC) {
            in_rf_request_state = true;
          }
        }
      }
    }
  }
  Serial.flush();
  return;
}