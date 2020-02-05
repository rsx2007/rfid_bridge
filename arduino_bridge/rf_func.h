
void readMessage() {
  loger("Read RF module message");
  int cnt = 0; // set byte counter to 0
  unsigned char buff [4];
  bool parse_flag = false; //  init start of package  to false
  in_rf_request_state = false; //  init full valid package to false

  for (byte i = 0; i < 4; i++) {  // clear income message to 0xFF
    in_rf_msg[i] = 0xFF;
  }

  if (RS485.available()) { // check if we got message    
  // STRUCT :   0x33 0x09 0x06
  //  0x33 - start of message
  // 0x09  end device address (RS485)
  // 0x06 -  cmd to request card
  // 0xFF  -  CRC  check value

    while (RS485.available())
    {  // read while got message
      char c = RS485.read();
      delay(1);
      if (c == 0x33) {  // if we got start of message
        loger("Start of message 0x33");
        parse_flag = true;
        cnt = 0;
        buff[0] = c;  // init,  put 0x33 to bufffer
        buff[1] = RS485.read(); // addr
        buff[2] = RS485.read();   // cmd
        buff[3] = RS485.read();   // crc
        loger("Check RF  message CRC");
        if (OneWire::crc8( buff, 3) == buff[3]) {  // check if crc valid
          loger("RF  message CRC - OK");
          in_rf_msg_crc_state = true;  // set crc flag  - true
          in_rf_request_state = true;  // set request flash - true
          delay(1);
          for (byte i = 0; i < 4; i++) { // write buffer to array
            in_rf_msg[i] = buff[i];
          }
          //memcpy(in_rf_msg, buff, sizeof(buff));
        } else {  // if crc wrong - set state to false
          loger("RF  message CRC error", "warning");
          in_rf_msg_crc_state = false;
          return;
        }
      }
    }
  }

  return;
}


void sendMessage() { // sending  response message to device

  // STRUCT: 0x23 0x09 0x06 0x00 0x00 0x00 0x00 0x00 0xFF
  // 0x23 - response init
  // 0x09  - response device addr
  // 0x06 - cmd to request card
  // 0x00 * 5 times -  card ID
  // 0xFF - CRC value

  loger("Send message");
  in_rf_request_state = false;  // set request state to false (to read again next time)

 // set message  bytes  buff[0] = 0x23 always
  out_msg[1] = in_rf_msg[1]; // set message  bytes   device addr
  out_msg[2] = in_rf_msg[2];  // cmd

  
  for (byte i = 0; i < 5; i++) {
    out_msg[i + 3] = cardId[i]; // card id
  }

  enableRS();
  loger("Maker CRC");
  out_msg[8] = OneWire::crc8( out_msg, 8);  // generate CRC
  enableRS();
  loger("Send package to RF");
  for (byte i = 0; i < 9; i++) { // send message to RS485
    RS485.write(out_msg[i]);
  }
  disableRS();
}
