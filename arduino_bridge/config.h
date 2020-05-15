//#define  DEBUG   //  1 - on    0 - off
#define DEBUG_LEVEL "debug"  //   info, warning, error, debug;
#define REBOOT 0
#define RS485_RX_PIN 1     // arduino pins 7 RX
#define RS485_TX_PIN 0     //  arduino pins 8 TX
#define RS485_DIRECTION_PIN 2 //  arduino pins 2

#define WIRE_RFID_PIN 3 // pin 3
#define LED_PIN 4 //  arduino pins 4

#define READ_CARD_DELAY 100    //  Read card from 1wire for X ms
#define READ_RF_DELAY 10       // Read rf messages every X ms
#define REBOOT_DELAY 180000 // Reboot  MCU  every X  ms
#define CARD_HOLD_DELAY 60000    //  Hold card in memory for X ms
