/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

#define DEBUG

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/sleep.h>

#ifdef DEBUG
#include <stdio.h> // Handy for debugging but bloats code. Can be stripped!
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

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, MCU);

FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int main (void)
{
    uart_init();
    stdout = stdin = stderr = &uart_stdio;
    
    uint16_t flashpointer = 0x0000;
    
    // Wait until we connect to the UART
    uart_getrawchar();
    
    uint8_t byte = pgm_read_byte(flashpointer);
    
    printf("%d", sizeof(bytetohex(byte)));
    printf(bytetohex(byte));
    my_printf(bytetohex(byte));
    printf("\nHere we go!\n");
    
    run_firmware();
}
