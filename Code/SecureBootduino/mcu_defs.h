/*
 * Header files for mcu definitions. Used in secure_bootloader, secure_payload and secureduino
 */

#define MCU "atmega328p"
#define CLOCK 16000000
#define BLS_START 0x7000 // Equivalent to BOOTSZ1 = 0, BOOTSZ0 = 0. 4K of bootloader space (2K words)
#define UART_BAUD 57600
