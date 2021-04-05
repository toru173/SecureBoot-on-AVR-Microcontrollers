/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

#define F_CPU 16000000UL // 16MHz oscillator is what is used in the simulator
#define BAUD 9600 // Could be faster if desired. This is just a PoC

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/setbaud.h>

#include <stdio.h>

#include "uart.h"
#include "secure_bootloader.h"

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega328p");

/* Function Prototypes (taken from optiboot.c*/
/* The main function is in init9, which removes the interrupt vector table */
/* we don't need. It is also 'naked', which means the compiler does not    */
/* generate any entry or exit code itself. */
int main(void) __attribute__ ((naked)) __attribute__ ((section (".init9")));

FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int returnme(int i)
{
    return i;
}

BOOTLOADER_SECTION int main (void)
{
    char message[] = "BLS!\n";
    
    //uart init:
    UCSR0A = 0x00; // Clear USART0 status register
    UBRR0H = UBRRH_VALUE; // Set baud values correctly
    UBRR0L = UBRRL_VALUE;
    UCSR0B = (1 << RXEN0 ) | (1 << TXEN0); // Enable transmit & receive
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit, no parity, one stop bit
    
    while (1)
    {
        for (int i = 0; i < 6; i++)
        {
            while((UCSR0A & (1 << UDRE0)) == 0);
            UDR0 = message[i];
        }
    }
}
