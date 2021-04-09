/*
 * Header files for mcu definitions. Used in secure_bootloader, secure_payload and secureduino
 */

// valid MCUs include atmega328p or atmega2560
#define ATMEGA328p "atmega328p"
#define ATMEGA2560 "atmega2560"

#define MCU ATMEGA2560
#define CLOCK 16000000

// Define bootloader address. 4K on atmega328p = 0x7000, 8K on atmega2560 = 0x3FE00
#if MCU == "atmega328p"
    #define BLS_START 0x7000
#elif MCU == "atmega2560"
    #define BLS_START 0x3FE00
#else
    #error "Unexpected MCU"
#endif

#define UART_BAUD 9600
