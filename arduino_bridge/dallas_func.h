void readCard() {
  byte i;
  byte tmp_data[8];
  ds.reset_search();
  if (millis() - card_hold_timer > CARD_HOLD_DELAY) {  // if card was in memory  and  card hold delay has passed -
    card_hold_timer = millis();
    loger("Hold card timer passed ");
    dallas_cardstate = false; // clear card state
    for ( i = 0; i < 5; i++) {  // clear card data
      cardId[i] = 0xFF;
    }
  }

  if (millis() - read_card_timer > READ_CARD_DELAY) { // if card read delay passed

    loger("Read card timer passed ");
    read_card_timer = millis();
    if ( !ds.search(tmp_data)) {
      loger("No rfid card inserted", "warning");
      ds.reset_search();
      return;
    }
    if ( OneWire::crc8( tmp_data, 7) != tmp_data[7]) {
      Serial.print("CRC is not valid!");
      return;
    }else{
      dallas_msg_crc_state = true;
    }
    if ( tmp_data[0] != 0x01) {
      loger("Device is not a DS1990A family device", "warning");
      return;
    }
    for ( i = 0; i < 5; i++) {
      cardId[i] = tmp_data[i + 1];
    }
    card_hold_timer = millis();
    dallas_cardstate = true;
    ds.reset();
  }


}
