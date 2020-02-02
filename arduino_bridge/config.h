#define  DEBUG   //  1 - on    0 - off
#define DEBUG_LEVEL "debug"  //   info, warning, error, debug;

#define RS485_A_PIN 4     // arduino pins
#define RS485_B_PIN 5     //  arduino pins
#define RS485_DIRECTION_PIN 7 //  arduino pins

#define WIRE_RFID_PIN 9

#define READ_CARD_DELAY 200    //  Read card from 1wire for X ms
#define READ_RF_DELAY 100       // Read rf messages every X ms
#define REBOOT_DELAY 180000 // Reboot  MCU  every X  ms
#define CARD_HOLD_DELAY 1500    //  Hold card in memory for X ms
