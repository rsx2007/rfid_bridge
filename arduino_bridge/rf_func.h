
void readMessage() {
  loger("Read RF module message");
  int cnt = 0;
  unsigned char buff [4];
  bool parse_flag = false;
  in_rf_request_state = false;

  for (byte i = 0; i < 4; i++) {  // clear card data
    in_rf_msg[i] = 0xFF;
    buff[i] = 0xFF;
  }
  if (RS485.available()) {
    while (RS485.available())
    {
      char c = RS485.read();
      delay(1);
      if (c == 0x33) {
        loger("Start of message 0x33");
        parse_flag = true;
        cnt = 0;
        buff[0] = c;  // init
        buff[1] = RS485.read(); // addr
        buff[2] = RS485.read();   // cmd
        buff[3] = RS485.read();   // crc
        loger("Check RF  message CRC");
        if (OneWire::crc8( buff, 3) == buff[3]) {
          loger("RF  message CRC - OK");
          in_rf_msg_crc_state = true;
          in_rf_request_state = true;
          delay(1);
          for (byte i = 0; i < 4; i++) {
            in_rf_msg[i] = buff[i];
          }
          //memcpy(in_rf_msg, buff, sizeof(buff));
        } else {
          loger("RF  message CRC error", "warning");
          in_rf_msg_crc_state = false;
          return;
        }
      }
    }
  }

  return;
}


void sendMessage() {
  loger("Send message");
  in_rf_request_state = false;
  
  out_msg[1] = in_rf_msg[1];
  out_msg[2] = in_rf_msg[2];
  enableRS();
  for (byte i = 0; i < 5; i++) {
    out_msg[i + 3] = cardId[i];
  }

  loger("Maker CRC");
  out_msg[8] = OneWire::crc8( out_msg, 8);
  enableRS();
  loger("Send package to RF");
  for (byte i = 0; i < 9; i++) {
    RS485.write(out_msg[i]);
  }
  disableRS();
  delay(2000);
}
