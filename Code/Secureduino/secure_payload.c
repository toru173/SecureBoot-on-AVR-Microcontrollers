/*
 * Initialises UART then prints out 'Hello World!' over serial
 *
 * Used as a basic demo payload for SecureBoot on AVR. It uses busy-wait loops
 * instead of interrupts for serial comms at this point
 *
 */

#include <avr/io.h>
#include <avr/sleep.h>

#include <stdio.h>

// Set up MCU
#define F_CPU 20000000
#define BAUD 9600
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
AVR_MCU(F_CPU, "atmega2560");

FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int main (void)
{
    uart_init();
    stdout = stdin = stderr = &uart_stdio;
    
    while (1)
    {
        printf("Hello World!\n");
        _delay_ms(1000);
    }
}
