/*
 * Header files for uart comms
 */

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
        uart_putchar('\r'); // Handle newline
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
    uart_putchar(c, stream); //echo byte back so we can see what we typed
    return c;
}
