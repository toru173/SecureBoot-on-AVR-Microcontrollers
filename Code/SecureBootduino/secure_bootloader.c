/*
 * Secure bootloader checks signature of firmware and provides for other utilities
 */

#include <avr/io.h>
#include <avr/boot.h>
#include <avr/sleep.h>
#include <util/delay.h>
#include <util/setbaud.h>

#include <stdio.h>

#include "mcu_defs.h"
#include "uart.h"
#include "secure_bootloader.h"

// Set up MCU
#define FCPU CLOCK
#define BAUD UART_BAUD
#include <util/setbaud.h>

/*
 * This demonstrate how to use the avr_mcu_section.h file
 * The macro adds a section to the ELF file with useful
 * information for the simulator
 */
#include "avr_mcu_section.h"
AVR_MCU(F_CPU, MCU);

FILE uart_stdio = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

int main (void)
{
    uart_init();
    stdout = stdin = stderr = &uart_stdio;
    
    while (1)
    {
        printf("Hello World!\n");
        _delay_ms(1000);
    }
    
    run_firmware();
}
