/*
 * Initialises UART then prints out 'Hello World!' over serial
 *
 * Used as a basic demo payload for SecureBoot on AVR
 */

#define F_CPU 8000000UL // 8MHz oscillator
#define BAUD 9600 // Could be faster if desired. This is just a PoC

#include <avr/io.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/setbaud.h>
#include <avr/pgmspace.h>

#include <stdio.h>

#include "uart.h"

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega2560");

/*
 * This small section tells simavr to generate a VCD trace dump with changes to these
 * registers. Hanging the trace specifically on the TX pin (PE1) of USART0
 */
const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
    { AVR_MCU_VCD_SYMBOL("UCSR0A"), .what = (void*)&UCSR0A, },
    { AVR_MCU_VCD_SYMBOL("UCSR0B"), .what = (void*)&UCSR0B, },
    { AVR_MCU_VCD_SYMBOL("UCSR0C"), .what = (void*)&UCSR0C, },
    { AVR_MCU_VCD_SYMBOL("UBRR0H"), .what = (void*)&UBRR0H, },
    { AVR_MCU_VCD_SYMBOL("UBRR0L"), .what = (void*)&UBRR0L, },
    { AVR_MCU_VCD_SYMBOL("UDRE0"), .mask = (1 << UDRE0), .what = (void*)&UCSR0A, },
    { AVR_MCU_VCD_SYMBOL("UDR0"), .what = (void*)&UDR0, },
    { AVR_MCU_VCD_SYMBOL("PORTE"), .what = (void*)&PORTE, },
    { AVR_MCU_VCD_SYMBOL("PE1"), .mask = (1 << PE1), .what = (void*)&PORTE, },
};

FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);

void uart_init(void)
{
    UCSR0A = 0x00; // Clear USART0 status register
    UBRR0H = UBRRH_VALUE; // Set baud values correctly
    UBRR0L = UBRRL_VALUE;
    UCSR0B = (1 << RXEN0 ) | (1 << TXEN0); // Enable transmit & receive
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit, no parity, one stop bit
}


int uart_putchar(char c, FILE *stream)
{
    loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for transmit buffer to be empty
    UDR0 = c;
    
    return 0;
}


int main (void)
{
    uart_init();
    stdout = &uart_stdio;

    char message[] = "What the fuck, avr-gcc -_-\n";
    
    printf(message);
        
        
    // this quits the simulator, since interupts are off
    // this is a "feature" that allows running tests cases and exit
    sleep_cpu();
}
