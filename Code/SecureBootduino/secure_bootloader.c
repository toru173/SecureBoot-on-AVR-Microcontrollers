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

//AVRNaCl Encryption Library
#include "avrnacl.h"
#define MAXTEST_BYTES 128


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


uint16_t *get1024block(uint16_t baseaddress) // Address always less than 64K.
{
    uint16_t *buffer = malloc(128); // 128 byte (1024 bit) buffer
    if (!buffer) // Unable to allocate
        return NULL;
    for (int i = 0; i < 128; i++)
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
    
    //AVRNaCl Defines (Could be put in their own header, above?)
    static unsigned char *h;
    static unsigned char *m;
    
    h  = calloc(crypto_hashblocks_STATEBYTES,1);
    if(!h) my_printf("allocation of h failed\n");
    m  = calloc(MAXTEST_BYTES,1);
    if(!m) my_printf("allocation of m failed\n");
    
    if (c == 'm')
    {
        // Enter monitor
        my_printf("\nWelcome to the monitor\n");
        
        // Create IV
        for (i = 0; i < 64; i++)
          h[i] = avrnacl_sha512_iv[i];
        
        uint16_t *blockptr = get1024block(0x0000);
        
        for (i = 0; i < 128; i++)
            m[i] = blockptr[i];

        my_printf("Hashing beginning. Success should be zero...\n");
        myprintf(bytetohex((uint8_t) (crypto_hashblocks_sha512(h, m, 128))));
        my_printf("\nhashing finished!\n");
        
        for (i = 0; i < 64; i++)
            my_printf(bytetohex(h[i]));
        
        free(h);
        free(m);
        
    }
    _delay_ms(1000);
    run_firmware();
}
