#include <OneWire.h>

/* 
 * test 1-Wire whith DS1990A 
 */

OneWire  ds(9);  // на  digital pin 10

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  byte i;
  byte present = 0;
  byte data[12];
  byte addr[8];
  
  if ( !ds.search(addr)) {
      Serial.print("No more addresses.\n");
      ds.reset_search();
      return;
  }
  
            Serial.print("R=");
            for( i = 0; i < 8; i++) {
              Serial.print(addr[i], HEX);
              Serial.print(" ");
            }
          
            if ( OneWire::crc8( addr, 7) != addr[7]) {
                Serial.print("CRC is not valid!\n");
                
                return;
  }
  
  if ( addr[0] != 0x01) {
      Serial.print("Device is not a DS1990A family device.\n");
      return;
  }
  Serial.println();
  ds.reset();
  
  delay(1000);
}
