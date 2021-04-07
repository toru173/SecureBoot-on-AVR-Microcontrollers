/*
 * Header files for uart comms
 */
#include <stdlib.h>

void uart_init(void);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

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
    if (c == '\n')
        uart_putchar('\r', stream); // Handle newline
    loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for transmit buffer to be empty
    UDR0 = c;
    
    return 0;
}

int uart_putrawchar(char c)
{
    if (c == '\n')
        uart_putrawchar('\r'); // Handle newline
    loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for transmit buffer to be empty
    UDR0 = c;
    
    return 0;
}

int uart_getchar(FILE *stream)
{
    char c;
    loop_until_bit_is_set(UCSR0A, RXC0); // Wait until byte received
    c = UDR0;
    uart_putchar(c, stream); //echo byte back so we can see what we typed
    return c;
}

int uart_getrawchar()
{
    char c;
    loop_until_bit_is_set(UCSR0A, RXC0); // Wait until byte received
    c = UDR0;
    uart_putrawchar(c); //echo byte back so we can see what we typed
    return c;
}

void my_printf(char *string)
{
    for (int i = 0; i < sizeof(string); i++)
    {
        uart_putrawchar(string[i]);
    }
}

char *bytetohex(uint8_t byte)
{
    char *c = "\0\0";
    uint8_t nybble0 = (byte >> 4) & 0x0F;
    uint8_t nybble1 = byte& 0x0F;
    if (nybble0 > 0x09)
        nybble0 += 0x35; // A-F
    else
        nybble0 += 0x30; // 0-9

    if (nybble1 > 0x09)
        nybble1 += 0x35; // A-F
    else
        nybble1 += 0x30; // 0-9
    c[0] = nybble0;
    c[1] = nybble1;
    return c;
}

// Finish me later
uint8_t hextobyte(char hex[2])
{
    uint8_t byte = 0x00;
    return byte;
}
