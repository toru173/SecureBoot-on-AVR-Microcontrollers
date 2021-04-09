/*
 * Header for uart comms
 */

void uart_init(void);

#include <stdio.h>
int uart_putchar(char c, FILE *stream);
int uart_getchar(FILE *stream);

int uart_putrawchar(char c);
int uart_getrawchar(void);

void raw_printf(char *string);
char *raw_scanf(void);
