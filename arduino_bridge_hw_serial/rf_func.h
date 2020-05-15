void sendRF02() { // sending  response message to device

  // STRUCT: 0x23 0x09 0x06 0x00 0x00 0x00 0x00 0x00 0xFF
  // 0x23 - response init
  // 0x09  - response device addr
  // 0x06 - cmd to request card
  // 0x00 * 5 times -  card ID
  // 0xFF - CRC value


  in_rf_request_state = false;  // set request state to false (to read again next time)

  // set message  bytes  buff[0] = 0x23 always
  out_msg[1] = in_rf_msg[1]; // set message  bytes   device addr
  out_msg[2] = in_rf_msg[2];  // cmd


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

}

void readRF02() {
  int cnt = 0; // set byte counter to 0
  unsigned char buff [4];
  bool parse_flag = false; //  init start of package  to false
  in_rf_request_state = false; //  init full valid package to false

  for (byte i = 0; i < 4; i++) {  // clear income message to 0xFF
    in_rf_msg[i] = 0xFF;
  }

  if (Serial.available()) { // check if we got message
    // STRUCT :   0x33 0x09 0x06
    //  0x33 - start of message
    // 0x09  end device address (Serial)
    // 0x06 -  cmd to request card
    // 0xFF  -  CRC  check value

    //delay(50);
    while (Serial.available())
    { // read while got message
      char c = Serial.read();
      delay(2);
      if (c == 0x33) {  // if we got start of message

        parse_flag = true;
        cnt = 0;
        buff[0] = c;  // init,  put 0x33 to bufffer
        buff[1] = Serial.read(); // addr
        buff[2] = Serial.read();   // cmd
        buff[3] = Serial.read();   // crc
        if (OneWire::crc8( buff, 3) == buff[3]) {  // check if crc valid

          in_rf_request_state = true;  // set request flash - true
          delay(1);
          for (byte i = 0; i < 4; i++) { // write buffer to array
            in_rf_msg[i] = buff[i];
          }
          sendRF02();
        } else {  // if crc wrong - set state to false

          return;
        }
      }
    }
  }

  return;
}
