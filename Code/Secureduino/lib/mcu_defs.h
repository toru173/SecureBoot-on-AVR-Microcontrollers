/*
 * Header files for mcu definitions. Used in secure_bootloader, secure_payload and secureduino
 */

// valid MCUs include atmega328p or atmega2560
#define MCU "atmega2560"
// Define bootloader address. 4K on atmega328p = 0x7000, 8K on atmega2560 = 0x3FE00
#define BLS_START 0x3FE00

#define CLOCK 20000000
#define UART_BAUD 9600
