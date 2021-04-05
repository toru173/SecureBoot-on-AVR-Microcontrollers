/*
 * Header files for uart comms
 */

void uart_init(void);
uint8_t uart_putchar(char c, FILE *stream);
uint8_t uart_getchar(FILE *stream);
