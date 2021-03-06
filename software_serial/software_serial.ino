
#include "config.h"  // Connect params lib

#include <OneWire.h>   // Connect Onewire lib
#include <SoftwareSerial.h>   // Connect SoftSerial 

SoftwareSerial RS485 (RS485_RX_PIN, RS485_TX_PIN); // RX, TX   INIT
OneWire  ds(WIRE_RFID_PIN);   //  DEFINE RFID READE CLASS

//-----------------------------------
//        BASE VARIABLES
//-----------------------------------
unsigned long read_card_timer;
unsigned long card_hold_timer;
unsigned long read_rf_timer;
unsigned long reboot_timer;    // Reboot  MCU  every X  seconds

unsigned in_rf_msg [4] =  {0xFF, 0xFF, 0xFF, 0xFF}; //  MESSAGE FROM RF MODULE    { INIT, ADR, CMD, CRC8 } {0x33, 0x09, 0x06, 0x55, 0xFF}
bool in_rf_msg_crc_state = false; // true - if message crc is OK
bool in_rf_request_state = false; // true - if RF ask for card

unsigned in_dallas_msg [5] =  {0x33, 0x09, 0x06, 0x55, 0xFF}; //  MESSAGE FROM RF MODULE    { INIT, ADR, CMD, CRC8 }
unsigned int cardId[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  //  CARD PACKAGE
bool dallas_msg_crc_state = false; // true - if message crc is OK
bool dallas_cardstate = false; // true - if dallas card state is OK  (card in memory)

unsigned char out_msg[9] = {0x23, 0x09, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};   //  MESSAGE TO RF MODULE    { INIT, ADR, CMD, CARD 5 BYTES, CRC8 }   OLD   msg_card[9]



//-----------------------------------
//        BASE FUNC AND UTILITIES
//-----------------------------------
#include "utility.h"  // connect utilities  funcs
#include "logger.h"  // connect logger  funcs
#include "dallas_func.h"  // connect RFID reader funcs
#include "rf_func.h"  //  connect  RF-RS485  funcs



void setup() {

#ifdef DEBUG
  Serial.begin(115200);
#endif
  // INIT  PINS
  loger("Init pins");
  pinMode(RS485_DIRECTION_PIN,   OUTPUT);    // устанавливаем режим работы вывода PIN_direction_TX_RX, как "выход"
  digitalWrite(RS485_DIRECTION_PIN, HIGH);    // устанавливаем уровень логического «0» на выводе PIN_direction_TX_RX (переводим модуль в режим приёма данных)

  // LED INIT
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // INIT CLASSES
  loger("Init classes");
  RS485.begin(19200);

  //INIT TIMERS
  loger("Init timers");
  read_card_timer = millis();
  card_hold_timer = millis();
  read_rf_timer = millis();
  reboot_timer = millis();


  //SEND START MESSAGE
  loger("Started successfully");
}

void loop() {
  RS485.print("READ CARD");

  readCard();
  //got  dallas_cardstate  true/false
  //got  cardId  id  or FF
  //got dallas_msg_crc_state true/false
  if (dallas_cardstate) {
    Serial.flush();
    readMessage();
  }
  //got  in_rf_request_state  true/false
  //got  in_rf_msg  msg  or FF
  //got in_rf_msg_crc_state true/false

  if (in_rf_request_state) {
    sendMessage();
  }
  
  if (REBOOT == 1) {
    if (millis() - reboot_timer > REBOOT_DELAY) { // if reset delay passed
      loger("Reset CPU");
      resetFunc();
      return;
    }
  }
}
