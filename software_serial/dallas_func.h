void readCard() {
  byte i;
  byte tmp_data[8];
  ds.reset_search();  // reset  class current search address
  if (millis() - card_hold_timer > CARD_HOLD_DELAY) {  // if card was in memory  and  card hold delay has passed -
    loger("Clear card timer");
    card_hold_timer = millis();  // update timer for holding card in memory
    dallas_cardstate = false; // clear card state
    for (i = 0; i < 5; i++) {  // clear card data
      cardId[i] = 0xFF;
    }
    digitalWrite(LED_PIN, HIGH);
  }

  if (millis() - read_card_timer > READ_CARD_DELAY) { // if card read delay passed
    loger("Read card");
    read_card_timer = millis();  // update timer for card read
    if ( !ds.search(tmp_data)) {  // didnt find  any card -> exit
      loger("No rfid card inserted", "warning"); 
      ds.reset_search(); // reset  class current search address
      return;
    }
    if ( tmp_data[0] != 0x01) {  // check if acceptable device
      loger("Device is not a DS1990A family device", "warning");
      return;
    }

    if ( OneWire::crc8( tmp_data, 7) != tmp_data[7]) {  // check  message valid
      Serial.print("CRC is not valid!");
      return;
    }else{
      dallas_msg_crc_state = true;  //  if CRC OK -> set CRC state to true   (optional)
    }

    for (i = 0; i < 5; i++) {  // write card number to var
      cardId[i] = tmp_data[i + 1];
      Serial.print(tmp_data[i + 1],HEX);
    }
    Serial.println(";");
    digitalWrite(LED_PIN, LOW);
    card_hold_timer = millis(); // update timer for holding card in memory
    dallas_cardstate = true; //  everything is OK -> set card state to true
    ds.reset();  // reset  RFID class
  }


}
