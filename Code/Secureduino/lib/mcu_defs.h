/*
 * Header files for mcu definitions. Used in secure_bootloader, secure_payload and secureduino
 */

<<<<<<< HEAD:Code/Secureduino/lib/mcu_defs.h
// valid MCUs include atmega328p or atmega2560
#define ATMEGA328P "atmega328p"
#define ATMEGA2560 "atmega2560"

#define MCU "atmega328p"
#define CLOCK 16000000

// Define bootloader address. 4K on atmega328p = 0x7000, 8K on atmega2560 = 0x3FE00
#if MCU == ATMEGA328P
    #define BLS_START 0x7000
#elif MCU == ATMEGA2560
    #define BLS_START 0x3FE00
#else
    #error "Unexpected MCU"
#endif

=======
#define MCU "atmega328p"
#define CLOCK 16000000
#define BLS_START 0x7000 // Equivalent to BOOTSZ1 = 0, BOOTSZ0 = 0. 4K of bootloader space (2K words)
>>>>>>> parent of ce924ac (Update mcu_defs.h):Code/Secureduino/include/mcu_defs.h
#define UART_BAUD 9600
