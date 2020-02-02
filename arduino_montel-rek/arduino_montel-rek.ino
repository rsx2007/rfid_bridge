#define  DEBUG 1
#define DEBUG_LEVEL "info"  //   info, warning, error, debug;


#define CARD_DELAY 5000
#define CARD_READ_DELAY 500

#define RS485_A_PIN 4
#define RS485_B_PIN 5
#define RS485_DIRECTION_PIN 7

#include <OneWire.h>
#include <SoftwareSerial.h>
SoftwareSerial RS485 (RS485_A_PIN, RS485_B_PIN); // RX, TX

OneWire  ds(10);


unsigned long RFMillis, RFIDMillis, RFIDReadMillis;  // read timers

unsigned int cardId[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};  //  CARD PACKAGE
unsigned char msg_card[9] = {0x23, 0x09, 0x06, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00};   //  MESSAGE TO RF MODULE    { INIT, ADR, CMD, CARD 5 BYTES, CRC8 }

//unsigned char msg_request[5] = {0x33, 0x09, 0x06, 0x55, 0xFF}; //  MESSAGE FROM RF MODULE    { INIT, ADR, CMD, CRC8 }

bool cardState = false;
bool sendCardRF = false;
unsigned char msg[5];

int cnt = 0;
boolean ready = false;

int cmd;
int adr;
int crc;


void loger(char * msg, char * type, bool newline){
  #ifdef DEBUG

  
  if(DEBUG_LEVEL == "info"){
    if(newline){
      Serial.print("INFO: ");
      Serial.println(msg);
    }else{
      Serial.print("INFO: ");
      Serial.print(msg);
    }
    return; 
  }
  if(type == "info" || type == "all"){
    if(newline){
      Serial.print("INFO: ");
      Serial.println(msg);
    }else{
      Serial.print("INFO: ");
      Serial.print(msg);
    }
  }else if(type == "error" || type == "all"){
      Serial.println("************************************");
      Serial.print("ERROR: ");
      Serial.print(msg);
      Serial.println("************************************");
  }else if(type == "debug" || type == "all"){
    if(newline){
      Serial.print("DEBUG: ");
      Serial.println(msg);
    }else{
      Serial.print("DEBUG: ");
      Serial.print(msg);
    }
  } 
  #endif
}

void setup() {
  pinMode(RS485_DIRECTION_PIN,   OUTPUT);    // устанавливаем режим работы вывода PIN_direction_TX_RX, как "выход"
  digitalWrite(RS485_DIRECTION_PIN, LOW);    // устанавливаем уровень логического «0» на выводе PIN_direction_TX_RX (переводим модуль в режим приёма данных)
  Serial.begin(9600);
  RS485.begin(19200);
  
  RFMillis = millis();
  RFIDMillis = millis();
  RFIDReadMillis = millis();
  loger("Start", "info", true);
}

unsigned char CRC_table[256] = {
  0, 94, 188, 226, 97, 63, 221, 131, 194, 156, 126, 32, 163, 253, 31, 65,
  157, 195, 33, 127, 252, 162, 64, 30, 95,  1, 227, 189, 62, 96, 130, 220,
  35, 125, 159, 193, 66, 28, 254, 160, 225, 191, 93,  3, 128, 222, 60, 98,
  190, 224,  2, 92, 223, 129, 99, 61, 124, 34, 192, 158, 29, 67, 161, 255,
  70, 24, 250, 164, 39, 121, 155, 197, 132, 218, 56, 102, 229, 187, 89,  7,
  219, 133, 103, 57, 186, 228,  6, 88, 25, 71, 165, 251, 120, 38, 196, 154,
  101, 59, 217, 135,  4, 90, 184, 230, 167, 249, 27, 69, 198, 152, 122, 36,
  248, 166, 68, 26, 153, 199, 37, 123, 58, 100, 134, 216, 91,  5, 231, 185,
  140, 210, 48, 110, 237, 179, 81, 15, 78, 16, 242, 172, 47, 113, 147, 205,
  17, 79, 173, 243, 112, 46, 204, 146, 211, 141, 111, 49, 178, 236, 14, 80,
  175, 241, 19, 77, 206, 144, 114, 44, 109, 51, 209, 143, 12, 82, 176, 238,
  50, 108, 142, 208, 83, 13, 239, 177, 240, 174, 76, 18, 145, 207, 45, 115,
  202, 148, 118, 40, 171, 245, 23, 73,  8, 86, 180, 234, 105, 55, 213, 139,
  87,  9, 235, 181, 54, 104, 138, 212, 149, 203, 41, 119, 244, 170, 72, 22,
  233, 183, 85, 11, 136, 214, 52, 106, 43, 117, 151, 201, 74, 20, 246, 168,
  116, 42, 200, 150, 21, 75, 169, 247, 182, 232, 10, 84, 215, 137, 107, 53
};


void enableRS() {
  digitalWrite(RS485_DIRECTION_PIN, HIGH);
}

void disableRS() {
  digitalWrite(RS485_DIRECTION_PIN, LOW);
}

void fWrite (unsigned char what) {


}

int fAvailable () {
  return RS485.available ();
}

int fRead () {
  return RS485.read();
}

void readMessage() {
  unsigned char buff [5];
  bool ready = false;

  while (RS485.available())
  {
    loger("readMessage() -> Read message", "info", true);
    delay(1);
    char c = RS485.read();

    if ((c == '0xFF') || (cnt == sizeof(buff) - 1))
    {
      loger("readMessage() -> Emd of message", "info", true);
      buff[cnt] = '\0';
      cnt = 0;
      sendCardRF = true;
    }
    if (c == 0x33) {
      buff[cnt++] = c;
      for (byte i = 0; i < 3; i++) {
        //Serial.println(c, HEX);
        c = RS485.read();
        buff[cnt++] = c;
      }
      ready = true;
    }
  }
  if (ready) {
    memcpy(msg, buff, sizeof(buff));
  }
  return;
}


unsigned char checkRFOutCRC() {
  bool status = false;
  unsigned char our_crc = 0;
  Serial.print("CRC MSG IN =");
  for (byte i = 0; i < 8; i++)
  {
    Serial.print(msg_card[i], HEX);
    Serial.print(" ");
    CRC(&our_crc, msg_card[i]);
  }
  Serial.println(" ");
  msg_card[8] = our_crc;
  return our_crc;
}

bool checkRFInCRC() {
  bool status = false;
  unsigned char our_crc = 0;

  for (byte i = 0; i < 3; i++)
  {
    CRC(&our_crc, msg[i]);
  }

  if (our_crc == msg[3]) {
    status = true;
  }
  return status;
}

void readCard() {
  // 61 00 20 e1 3a   (REAL)
  // 3A E1 20 0 61  IN
  byte i;
  byte addr[8];
  bool card_exist = false;

  if (millis() - RFIDReadMillis > CARD_READ_DELAY) {
    //READ RFID
    Serial.println("READ RFID");

    if ( !ds.search(addr)) {
      ds.reset_search();
      return false;
    }

    if ( OneWire::crc8( addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return false;
    }

    if ( addr[0] != 0x01) {
      Serial.print("Device is not a DS1990A family device.\n");
      return false;
    }

    ds.reset();

    for ( i = 1; i < 6; i++) {
      cardId[i - 1] = addr[i];

    }
    cardState = true;
    card_exist = true;

    if (card_exist) {
      RFIDMillis = millis();
      //Serial.println("CARD NUM = ");
      for (byte i = 0; i < 5; i++) {
        Serial.print(cardId[i], HEX);
        Serial.print(" ");
      }
    }
    RFIDReadMillis = millis();
  }

  if (millis() - RFIDMillis > CARD_DELAY) {
    //Serial.println("CLEAR CARD AND STATUS");
    cardState = false;
    for (byte i = 0; i < 5; i++)
    {
      cardId[i] = 0xFF;
    }
    ds.reset_search();

    ds.reset();
    RFIDMillis = millis();
  }
}

void sendMessage() {
  Serial.println("SENDING MESSAGE");

  sendCardRF = false;
  enableRS();
  for (byte i = 0; i < 5; i++) {
    msg_card[i + 3] = cardId[i];
  }
  checkRFOutCRC();
  for (byte i = 0; i < 9; i++) {
    RS485.write(msg_card[i]);
  }
  disableRS();
  for (byte i = 0; i < 9; i++) {
    Serial.print(msg_card[i], HEX);
    Serial.print("  ");
  }
  Serial.println("");

}

void loop() {


  //CHECK IF REQUEST FROM RF
  if (fAvailable ()) {
    Serial.println("MESSAGE AVAILABLE");
    readMessage();
  }


  //SEND TO RF

  if (sendCardRF) {

    Serial.println("MESSAGE FROM RF - OK");
    if (checkRFInCRC()) {
      Serial.println("RF CRC - OK");

      if (msg[1] == 0x09 || msg[1] == 0x0A) {
        Serial.println("ADDRESS - OK");

        Serial.print("Head = ");
        Serial.println(msg[0], HEX);

        Serial.print("Address = ");
        Serial.println(msg[1], HEX);

        Serial.print("Cmd = ");
        Serial.println(msg[2], HEX);

        Serial.print("CRC_IN:");
        Serial.println(msg[3], HEX); // выводим в шестнадцатеричном

        readCard();
        sendMessage();
      } else {
        Serial.println("WRONG ADDRESS");
      }
    } else {
      Serial.println("WRONG CRC");
    }
  }



}


void CRC (unsigned char *crc, unsigned char data)
{
  *crc = CRC_table[*crc ^ data];
  return;
}
