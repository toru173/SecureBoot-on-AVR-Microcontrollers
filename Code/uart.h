/*
 * Header files for uart comms
 */

void uart_init(void);
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);
