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

char *bytetohex(uint8_t byte);
uint8_t hextobyte(char hex[2]);
