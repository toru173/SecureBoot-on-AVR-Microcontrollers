/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

//#define DEBUG

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#ifdef DEBUG
#include <stdio.h> // Handy for debugging but bloats code significantly
#endif

//#include "mcu_defs.h"
#include "secure_bootloader.h"

// Set up MCU
#define F_CPU 20000000
#define BAUD 9600
#include <util/setbaud.h>
#include <util/delay.h>

// Set up USART
#include "uart.h"

// Monitor
#include "monitor.h"

//AVRNaCl Encryption Library
//#include "avrnacl.h"

//AVRCryptoLib Encryption Library
//#include "AVRCrytolib.h"

#define PK_BYTES 32
#define STATE_VEC_BYTES 64
#define BLOCK_SIZE_BYTES 128

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega2560");

#ifdef DEBUG
FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
#endif


int main (void)
{
    uart_init();
    
    #ifdef DEBUG
    stdout = stdin = stderr = &uart_stdio;
    #endif
        
    char c = uart_getrawchar(); // Wait for input before continuing
    
    if (c == 'm')
    {
        // Enter monitor

        raw_printf("\n");
        raw_printf("\nHashing beginning. Success should be zero: ");
        // raw_printf(bytetohex((uint8_t) crypto_hashblocks_sha512(hash, block, sizeof(block))));
        raw_printf("\nhashing finished! Complete hash:\n");

        raw_printf("\n");
        
        raw_printf("\nSigning beginning. Success should be zero: ");
        raw_printf("\nSigning finished!\n");
        
    }
    //_delay_ms(1000);
    run_firmware();
}
