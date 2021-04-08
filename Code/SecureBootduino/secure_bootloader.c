/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

//#define DEBUG

#ifndef DEBUG
#define USERAWSTRINGS //for UART comms. Saves code space
#endif

#include <stdlib.h>

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#ifdef DEBUG
#include <stdio.h> // Handy for debugging but bloats code significantly
#endif

#include "mcu_defs.h"
#include "secure_bootloader.h"

// Set up MCU
#define F_CPU CLOCK
#define BAUD UART_BAUD
#include <util/setbaud.h>
#include <util/delay.h>

// Set up USART
#include "uart.h"

// Monitor
#include "monitor.h"

//AVRNaCL Encryption Library
#include "avrnacl.h"

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, MCU);

#ifdef DEBUG
FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);
#endif


uint16_t *get512block(uint16_t baseaddress) // Address always less than 64K
{
    uint16_t *buffer = malloc(64); // 64 byte (512 bit) buffer
    if (!buffer) // Unable to allocate
        return NULL;
    for (uint16_t i = baseaddress; i < (baseaddress + 64); i++)
    {
        *(buffer + i) = pgm_read_byte(baseaddress + i);
    }
    return buffer;
}


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
        my_printf("\nWelcome to the monitor\n");
        
        /*uint16_t *blockptr = get512block(0x7000);
        
        my_printf("\ngot block!\n");
        
        for (int i = 0; i < 64; i++)
        {
            my_printf(bytetohex(*(blockptr + i)));
        }
        //free(blockptr);
         */
    }
    _delay_ms(1000);
    run_firmware();
}
