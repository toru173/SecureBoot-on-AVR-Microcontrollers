/*
 * Header files for uart comms
 */

#include <avr/io.h>

void init_uart(void)
{
    UCSRA = 0x00; // Clear status register
    UCSRB = (1 << RXEN ) | (1 << TXEN); // Enable Transmit & receive, even though we're transmitting
    UCSRC = (1<<UCSZ1) | (1<<UCSZ0); // 8 bit, no parity, one stop bit
}
