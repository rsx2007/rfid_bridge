/*
// ADDRESS 1
#define DEV_ADDRESS 0x01
#define RCV_CRC 0x23
*/

// ADDRESS 2
#define DEV_ADDRESS 0x02
#define RCV_CRC 0x76


/*
// ADDRESS 9
#define DEV_ADDRESS 0x09
#define RCV_CRC 0x55
*/

/*
// ADDRESS 10
#define DEV_ADDRESS 0x0A
#define RCV_CRC 0x00
*/


#define REBOOT 1
#define RS485_DIRECTION_PIN 2 //  arduino pins 6
#define WIRE_RFID_PIN 3
#define LED_PIN 4 //  arduino pins 4

#define READ_CARD_DELAY 200    //  Read card from 1wire for X ms
//#define READ_RF_DELAY 200       // Read rf messages every X ms
#define REBOOT_DELAY 180000     //180000 // Reboot  MCU  every X  ms
#define CARD_HOLD_DELAY 2000    //  Hold card in memory for X ms

#define LED_HIGH_LEVEL LOW
#define LED_LOW_LEVEL HIGH