/*
 * Header files for uart comms
 */

#ifdef DEBUG
#define USERICHSTRINGS
#endif

void uart_init(void);

#ifdef USERICHSTRINGS
#include <stdio.h>
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
#endif

int uart_putrawchar(char c);
int uart_getrawchar();
void my_printf(char *string);

void uart_init(void)
{
    UCSR0A = 0x00; // Clear USART0 status register
    UBRR0H = UBRRH_VALUE; // Set baud values correctly
    UBRR0L = UBRRL_VALUE;
    UCSR0B = (1 << RXEN0 ) | (1 << TXEN0); // Enable transmit & receive
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bit, no parity, one stop bit
}


#ifdef USERICHSTRINGS
int uart_putchar(char c, FILE *stream)
{
    if (c == '\n')
        uart_putchar('\r', stream); // Handle newline
    loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for transmit buffer to be empty
    UDR0 = c;
    
    return 0;
}
#endif

int uart_putrawchar(char c)
{
    if (c == '\n')
        uart_putrawchar('\r'); // Handle newline
    loop_until_bit_is_set(UCSR0A, UDRE0); // Wait for transmit buffer to be empty
    UDR0 = c;
    
    return 0;
}

#ifdef USERICHSTRINGS
int uart_getchar(FILE *stream)
{
    char c;
    loop_until_bit_is_set(UCSR0A, RXC0); // Wait until byte received
    c = UDR0;
    uart_putchar(c, stream); //echo byte back so we can see what we typed
    return c;
}
#endif

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
