//#define  DEBUG   //  1 - on    0 - off
#define DEBUG_LEVEL "debug"  //   info, warning, error, debug;
#define REBOOT 0
#define RS485_DIRECTION_PIN 2 //  arduino pins 6
#define WIRE_RFID_PIN 3
#define LED_PIN 13 //  arduino pins 4

#define READ_CARD_DELAY 200    //  Read card from 1wire for X ms
#define READ_RF_DELAY 100       // Read rf messages every X ms
#define REBOOT_DELAY 180000//180000 // Reboot  MCU  every X  ms
#define CARD_HOLD_DELAY 2000    //  Hold card in memory for X ms
