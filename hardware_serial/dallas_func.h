void readCard() {
  byte i;
  byte tmp_data[8];
  
  ds.reset_search();  // reset  class current search address
  if ((millis() - card_hold_timer > CARD_HOLD_DELAY) && dallas_cardstate) {  // if card was in memory  and  card hold delay has passed -
    card_hold_timer = millis();  // update timer for holding card in memory
    dallas_cardstate = false; // clear card state
    for (i = 0; i < 5; i++) {  // clear card data
      cardId[i] = 0x00;
    }
    digitalWrite(LED_PIN, LED_LOW_LEVEL);
  }

  if (millis() - read_card_timer > READ_CARD_DELAY) { // if card read delay passed

    read_card_timer = millis();  // update timer for card read
    if ( !ds.search(tmp_data)) {  // didnt find  any card -> exit
        
      ds.reset_search(); // reset  class current search address
      return;
    }
        
    if ( tmp_data[0] != 0x01) {  // check if acceptable device
      return;
    }

    if ( OneWire::crc8( tmp_data, 7) == tmp_data[7]) {  // check  message valid
      for (i = 0; i < 5; i++) {  // write card number to var
        cardId[i] = tmp_data[i + 1];
      }
      digitalWrite(LED_PIN, LED_HIGH_LEVEL);
      card_hold_timer = millis(); // update timer for holding card in memory
      dallas_cardstate = true; //  everything is OK -> set card state to true
      ds.reset();  // reset  RFID
    }
    
  }
}