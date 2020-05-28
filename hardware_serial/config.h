#define DEV_ADDRESS 0x09
#define REBOOT 1
#define RS485_DIRECTION_PIN 2 //  arduino pins 6
#define WIRE_RFID_PIN 3
#define LED_PIN 4 //  arduino pins 4

#define READ_CARD_DELAY 200    //  Read card from 1wire for X ms
#define READ_RF_DELAY 200       // Read rf messages every X ms
#define REBOOT_DELAY 60000     //180000 // Reboot  MCU  every X  ms
#define CARD_HOLD_DELAY 1000    //  Hold card in memory for X ms

#define LED_HIGH_LEVEL LOW
#define LED_LOW_LEVEL HIGH
