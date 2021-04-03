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
    { AVR_MCU_VCD_SYMBOL("UDR0"), .what = (void*)&UDR0, },
    { AVR_MCU_VCD_SYMBOL("PORTE"), .what = (void*)&PORTE, },
    { AVR_MCU_VCD_SYMBOL("PE1"), .mask = (1 << PE1), .what = (void*)&PORTE, },
};

unsigned char message[] = "Hello World!";

void init_uart_0(void)
{
    UCSR0A = 0x00; // Clear USART0 status register
    UCSR0B = (1 << RXEN0 ) | (1 << TXEN0); // Enable transmit & receive
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit, no parity, one stop bit
    DDRE &= ~(1 << PE0); // Enable input on Port E (RXD0)
    DDRE |= (1 << PE1); // Enable output on Port E (TXD0)
}

int main (void)
{
    init_uart_0();
    
    for (int i = 0; i < 11; i++)
    {
        loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for transmit buffer to be empty
        UDR0 = message[i];
    }
    
    // this quits the simulator, since interupts are off
    // this is a "feature" that allows running tests cases and exit
    sleep_cpu();
}
