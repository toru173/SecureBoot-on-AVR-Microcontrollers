/*
 * Header for uart comms
 */

#ifndef USERAWSTRINGS
#define USERICHSTRINGS
#endif

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
